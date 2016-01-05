/****************************************************************************
    Copyright (C) 1987-2005 by Jeffery P. Hansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include "tkgate.h"
#include "yybasic.h"
#include "vgrammar.h"
#include "ycmalloc.h"

/*
 * New built-in gate naming convension.
 *
 * _GGAND3B4D5 g1 (.Z(w1), .I0({{4}{w2}}), .I1(w3), .I2(w4));
 *
 * Here _GG is the prefix for built-in gates. "AND" is the base name
 * of the gate, "3" indicates the number of inputs (or outputs depending
 * on type) and B4 indicates the base bit width.  A leading "N" in front
 * of the base name means the output is inverted.  For gates with multiple
 * outputs, no qualifier means no inversion, and a qualifier such as "PN"
 * means the first output is non-inverted and the second is inverted.
 *
 */

int yyparse();
void BeginVR();

#ifndef yyrestart
void yyrestart(FILE*);
#endif

#define HDLTEXTBLOCKSIZE 512

#define WENDSBLOCKSIZE	64

GModuleDef *topModule = 0;

int VerilogErrorCount = 0;


Encoder *parserEncoder = 0;

static char *VerilogReadPrefix = 0;
static int translateDemux = 0;

/*
 * Table of previous versions.  As of version 1.6, the table includes only
 * versions for which the flags change.  If the flag after the version name is
 * non-zero, then the save file is incompatable with the current version.
 */
VersionDef VERSIONS[]={
  {TKGATE_FULL_VERSION,0},
  {"2.0-b5",	VF_NOBACK},
  {"1.9",	VF_NOBACK},
  {"1.6h",	VF_NOBACK},
  {"1.5c",	VF_NOBACK},
  {"0.5",	VF_OLD}
};
int numVersions = sizeof(VERSIONS)/sizeof(VERSIONS[0]);

/*
 * Version where extender bar features was added.
 */
static char *extBarVersion = "1.6i";

/*
 * Version where demux behavior was changed.
 */
static char *newDemuxVersion = "2.0-b6";

/*
 * Version where net name display on nets was added.
 */
static char *showSwitchNetsVersion = "2.0-b7";

/*****************************************************************************
 *
 *   Data structure used to keep intermediate parser state.
 *
 *****************************************************************************/
static struct {
  Version	gate_ver;	/* Version number of tkgate */
  Version	ver;		/* Version number of current file */

  GModuleDef	*mod;		/* Current module being processed */
  GNet		*net;		/* Current net being processed */
  GCElement	*gate;		/* Current gate being processed */
  int		vnum;		/* Variant number of gate */
  GWire		*wire;
  GWireNode	*node;

  List		*modHPorts;	/* Module ports that are in the #() list */
  List		*modPorts;	/* Standard module ports */


  char		invSpec[STRMAX]; /* Output inverter specification */

  int		del_mod;	/* Module is marked for deletion (e.g., top-level of library). */
  List		*del_list;	/* List of modules scheduled for deletion */

  List		*mparm_list;	/* List of module parameters */

  int		numAllocedWends;/* Number of wends elements allocated */
  GWire		**wends;	/* Array of wire ends */
  int		numWEnds;	/* Number of wire ends */

  SHash		*netWEnds;

  int		numAllocedgPin;	/* Number of gPin/gNet/gComp elements allocated */
  const char	**gPin;		/* Array of pin names */
  GNet		**gNet;		/* Array of net objects */
  int		*gComp;		/* Array of complement flags */
  int		numPins;	/* Number of pins */
  int		pidx;		/* Current pin index */
  unsigned	revPads;	/* Bit array of pads that should be reversed */

  const char	*libraryName;	/* Name of current library being loaded (0 if not loading library) */
  const char	*fileName;	/* Name of current file */

  struct {			/* Current symbol icon */
    GModSymbol	*ms;
    int		which;		/* What type of icon is this 0=normal, 1=select */
    int		N;		/* Number of bytes allocated */
    int		x,y;
    char	*data;		/* Data buffer */
    char	*p;		/* Current position in buffer */
  } icon;

  int		numRequires;	/* Number of required libraries */
  char		**requires;	/* Array of required libraries */

  int		isInterface;	/* We are parsing an interface statement */

  int		didTimescale;	/* Default timescale has been set */
} cur = {{{0}}};

NHash *modSymbolTable = 0;

static void cur_initModule()
{
  cur.net = 0;
  cur.gate = 0;
  cur.wire = 0;
  cur.node = 0;
  cur.del_mod = 0;
  cur.numWEnds = 0;
  cur.numPins = 0;
  cur.wends = 0;
  cur.gPin = 0;
  cur.gNet = 0;
  cur.gComp = 0;
  cur.vnum = 0;
  cur.netWEnds = new_SHash();
}

static void cur_endModule()
{
  if (cur.netWEnds) delete_SHash(cur.netWEnds);
}

static void cur_initCircuit()
{
  memset(&cur,0,sizeof(cur));
}

/*
 * Ensure that element n of wends has been allocated. 
 */
static void cur_wends_extend(int n)
{
  if (!cur.wends) {
    cur.wends = (GWire**)yc_calloc(WENDSBLOCKSIZE,sizeof(GWire*),ALGN_WORD);
    cur.numAllocedWends = WENDSBLOCKSIZE;
  }
  if (n >= cur.numAllocedWends) {
    while (n >= cur.numAllocedWends)
      cur.numAllocedWends += WENDSBLOCKSIZE;
    cur.wends = (GWire**)yc_realloc(cur.wends,
				    (cur.numAllocedWends - WENDSBLOCKSIZE)*sizeof(GWire*),
				    cur.numAllocedWends*sizeof(GWire*),ALGN_WORD);
  }
}

static void cur_gpin_extend(int n)
{
  if (!cur.gPin) {
    cur.gPin = (const char**)yc_calloc(WENDSBLOCKSIZE,sizeof(char*),ALGN_WORD);
    cur.gNet = (GNet**)yc_calloc(WENDSBLOCKSIZE,sizeof(GNet*),ALGN_WORD);
    cur.gComp = (int*)yc_calloc(WENDSBLOCKSIZE,sizeof(int),ALGN_WORD);
    cur.numAllocedgPin = WENDSBLOCKSIZE;
  }
  if (n >= cur.numAllocedgPin) {
    while (n >= cur.numAllocedgPin)
      cur.numAllocedgPin += WENDSBLOCKSIZE;
    cur.gPin = (const char**)yc_realloc(cur.wends,
					(cur.numAllocedgPin-WENDSBLOCKSIZE)*sizeof(char*),
					cur.numAllocedgPin*sizeof(char*),ALGN_WORD);
    cur.gNet = (GNet**)yc_realloc(cur.gNet,
				  (cur.numAllocedgPin-WENDSBLOCKSIZE)*sizeof(GNet*),
				  cur.numAllocedgPin*sizeof(GNet*),ALGN_WORD);
    cur.gComp = (int*)yc_realloc(cur.gComp,
				 (cur.numAllocedgPin-WENDSBLOCKSIZE)*sizeof(int),
				 cur.numAllocedgPin*sizeof(int),ALGN_WORD);
  }
}

static unsigned spread(unsigned M,unsigned x)
{
  unsigned R = 0;
  int i,j;

  j = 0;
  for (i = 0;i < 32;i++) 
    if ((M & (1 << i)) && (x & (1 << j++)))
      R |= (1 << i);

  return R;
}

static void cur_endFile()
{
  cur.libraryName = 0;
  cur.fileName = 0;

  if (cur.del_list) {
    ListElem *le;

    for (le = List_first(cur.del_list);le;le = List_next(cur.del_list, le)) {
      GModuleDef *M = (GModuleDef*) ListElem_obj(le);
      delete_GModuleDef(M);
    }

    delete_List(cur.del_list);
  }
  cur.del_list = 0;
}

static void cur_scheduleModDelete(GModuleDef *M)
{
  if (!cur.del_list)
    cur.del_list = new_List();
  List_addToTail(cur.del_list, M);
}


int VerilogLoadRequired(EditState **es)
{
  int n = cur.numRequires;
  char **requires = cur.requires;
  int i;

  cur.numRequires = 0;
  cur.requires = 0;

  if (TkGate.tcl) {
    for (i = 0;i < n;i++) {
      if (VerilogOpenLibrary(requires[i]) < 0) {
	message(1,msgLookup("err.badlibopen"),requires[i]);	/* Unable to open library file '%s'. */
      }
      free(requires[i]);
    }
    free(requires);
  }

  return 0;
}


/*****************************************************************************
 *
 * Load a new circuit from the named file.
 *
 * Parameters:
 *      **es		The current editor state.
 *      name		Name of circuit file to load.
 *
 *
 *
 *****************************************************************************/
int VerilogOpen(EditState **es,const char *name,int append)
{
  FILE *f;
  char buf1[STRMAX];
  extern int click_count;

  if (!(f = fopen(name,"r")))
    return -1;

  click_count = 0;

  strcpy(buf1,name);
  cur_initCircuit();
  cur.fileName = buf1;
  cur.libraryName = 0;
  translateDemux = 0;

  VerilogReadPrefix = 0;
  if (!append) {
    Circuit_unloadAllLibraries(TkGate.circuit);
    editstate_flushModules(es);
    Circuit_clear();
    Circuit_initOptions(TkGate.circuit);
    topModule = 0;
  } else
    topModule = TkGate.circuit->root_mod;
  modSymbolTable = new_NHash_noob();

  ycSetDefaultModuleType(HDLMODULE);

  ycFileName = name;
  ycLineNumber = 1;
  parserEncoder = Circuit_getLoadFileEncoder(TkGate.circuit);
#if LOCALE_DEBUG
  printf("parserEncoding <%s> to <%s>\n",TkGate.locale->l_encVerilog, CE_UTF8);
#endif
  yc_setup();
  BeginVR();
  yyrestart(f);
  yc_pushpool();
  yyparse();
  yc_poppool();
  fclose(f);

  delete_NHash(modSymbolTable);

  if (!topModule)
    topModule = env_findAdd("main",1);

  editstate_push(es,topModule,0);
  editstate_setCurrent(*es);
  TkGate.circuit->root_mod = (*es)->env;
  modint_deleteInterface(TkGate.circuit->root_mod);
  DoTcl("BlockTree::openRoot");

  SetModified(MF_MODULE);
#if 0
  env_updateMTCircuit();
#endif

  Circuit_setCurrentFileVersion(cur.ver.str);

  if (!TkGate.circuit->title) {
    char *p = strrchr(cur.fileName,'/');
    Circuit_setTitle(p ? (p+1) : cur.fileName);
  }

  cur_endFile();

  VerilogLoadRequired(es);

  DoTcl("LibraryMgr::loadBaseLibraries");

  return 0;
}

/*****************************************************************************
 *
 * Load the named library
 *
 *****************************************************************************/
int VerilogOpenLibrary(const char *name)
{
  FILE *f;
  char fileName[STRMAX];
  char libraryName[STRMAX];
  char *p;

  /*
   * Remove directory path and extension from library name.
   */
  strcpy(fileName,name);
  strcpy(libraryName,name);
  p = strrchr(libraryName,'/');
  if (p) memmove(libraryName,p+1,strlen(p));
  p = strrchr(libraryName,'.');
  if (p) *p = 0;

  /*
   * Library is already loaded.
   */
  if (SHash_find(TkGate.libraries,libraryName))
    return TCL_OK;

  /*
   * Register library as loaded.
   */
  SHash_insert(TkGate.libraries,libraryName,(void*)1);

  /*
   * Look up filename for library.
   */
  if (*name != '/') {
    DoTclL("findLibraryFile",name,NULL);
    name = Tcl_GetStringResult(TkGate.tcl);
    if (!*name) {
      SHash_remove(TkGate.libraries,libraryName);
      return -1;
    }
  }

  if (!(f = fopen(name,"r"))) {
    SHash_remove(TkGate.libraries,libraryName);
    return -1;
  }

  VerilogReadPrefix = 0;

  ob_touch(TkGate.circuit);

  /*
   * Set file name and library names.
   */
  cur_initCircuit();
  cur.fileName = fileName;
  cur.libraryName = libraryName;
  translateDemux = 0;

  topModule = 0;
  modSymbolTable = new_NHash_noob();

  ycSetDefaultModuleType(HDLMODULE);

  ycFileName = name;
  ycLineNumber = 1;
  parserEncoder = getEncoder(CE_UTF8,TkGate.locale->l_encVerilog);
#if LOCALE_DEBUG
  printf("parserEncoding <%s> to <%s>\n",TkGate.locale->l_encVerilog,CE_UTF8);
#endif
  yc_setup();
  BeginVR();
  yyrestart(f);
  yc_pushpool();
  yyparse();
  yc_poppool();
  fclose(f);

  delete_NHash(modSymbolTable);

  SetModified(MF_MODULE);

  cur_endFile();

  return 0;
}

void yyerror(char *err,...)
{
  char buf[STRMAX];
  extern int quietMode;

  va_list ap;

  VerilogErrorCount++;

  va_start(ap, err);

  if (TkGate.tcl) {
    sprintf(buf,"tkg_verilogErr %s \"Line %d, ",ycFileName,ycLineNumber);
    vsprintf(buf+strlen(buf),err,ap);
    va_end(ap);
    strcat(buf,"\"");
    Tcl_Eval(TkGate.tcl,buf);
  } else {
    if (!quietMode) {
      sprintf(buf,"%s,%d: ",ycFileName,ycLineNumber);
      vsprintf(buf+strlen(buf),err,ap);
      printf("%s\n",buf);
    }
  }
}

/*****************************************************************************
 *   Functions called by parser to construct modules
 *****************************************************************************/
int ParseVersion(const char *version,Version *V)
{
  char p,x;
  int n;

  if (*version == 'V') version++;	/* Ignore leading V if present */

  strncpy(V->str,version,sizeof(V->str));
  V->str[sizeof(V->str)-1] = 0;
  V->alpha = V->beta = 0;
  V->revision = 0;

  if (sscanf(version,"%d.%d.%d-a%d",&V->major,&V->minor,&V->revision,&V->alpha) == 4) {
    return 0;
  } else if (sscanf(version,"%d.%d.%d-b%d",&V->major,&V->minor,&V->revision,&V->beta) == 4) {
    return 0;
  } else if (sscanf(version,"%d.%d.%d",&V->major,&V->minor,&V->revision) == 3) {
    return 0;
  } else if (sscanf(version,"%d.%d-a%d",&V->major,&V->minor,&V->alpha) == 3 && V->major >= 2) {
    return 0;
  } else if (sscanf(version,"%d.%d-b%d",&V->major,&V->minor,&V->beta) == 3 && V->major >= 2) {
    return 0;
  }

  /*
   * Handle pre-2.0 version numbers that may use letters for releases
   */
  n = sscanf(version,"%d.%d%c%c",&V->major,&V->minor,&p,&x);
  switch (n) {
  case 3 :
    V->revision = p - 'a';
    break;
  case 2 :
    V->revision = 0;
    break;
  default :
    return -1;
  }

  return 0;
}

/*
  Compare versions
 */
int VersionCmp(Version *V1,Version *V2)
{
  if (V1->major < V2->major) return -1;
  if (V1->major > V2->major) return 1;

  if (V1->minor < V2->minor) return -1;
  if (V1->minor > V2->minor) return 1;

  if (V1->revision < V2->revision) return -1;
  if (V1->revision > V2->revision) return 1;

  if (V1->beta == 0 && V1->alpha == 0) {
    if (V2->beta || V2->alpha) return 1;
  }

  if (V2->beta == 0 && V2->alpha == 0) {
    if (V1->beta || V1->alpha) return -1;
  }

  if (V1->beta < V2->beta) return -1;
  if (V1->beta > V2->beta) return 1;

  if (V1->alpha < V2->alpha) return -1;
  if (V1->alpha > V2->alpha) return 1;

  return 0;
}

/*****************************************************************************
 *
 * Check the version number to see if there are any special transformations we
 * need to make on the circuit to bring it into compliance with the current
 * version of tkgate.
 *
 *****************************************************************************/
void VerSetSpecialLoadFeatures(Version *V)
{
  Version x_V;

  /*
   * Check version number to set default value for extension bar use.
   */
  ParseVersion(extBarVersion,&x_V);
  if (VersionCmp(V,&x_V) >= 0)
    TkGate.circuit->useExtBars = 1;
  else
    TkGate.circuit->useExtBars = 0;

  /*
   * Set default value of show switches based on file version.  Enable
   * feature only for files created after the feature was added.
   */
  ParseVersion(showSwitchNetsVersion,&x_V);
  if (VersionCmp(V,&x_V) >= 0)
    TkGate.circuit->showSwitchNets = 1;
  else
    TkGate.circuit->showSwitchNets = 0;

  /*
   * If we are older than "newDemuxVersion", then translate and DEMUX
   * components into OLDDEMUX components.
   */
  ParseVersion(newDemuxVersion,&x_V);
  if (VersionCmp(V,&x_V) < 0)
    translateDemux = 1;
}

/*****************************************************************************
 *
 * Check the version of the save file to see if we need to do any special
 * load features or issue warnings about old format save files. 
 *
 *****************************************************************************/
void VerCheckVersion(const char *version)
{
  int i;
  unsigned flags = VF_UNKNOWN;
  Version V;

  ParseVersion(VERSIONS[0].vd_name, &V);
  cur.gate_ver = V;

  if (ParseVersion(version, &V) < 0) {
    message(1,msgLookup("err.badversion"));
    return;
  }

  cur.ver = V;

  for (i = 0;i < numVersions;i++) {
    Version x_V;

    ParseVersion(VERSIONS[i].vd_name, &x_V);

    if (VersionCmp(&x_V,&V) < 0)
      break;

    flags = VERSIONS[i].vd_flags;
  }

  /*
   * Set the default module type to netlist if a version tag is present.  For >2.0 save files
   * we expect all modules to be tagged anyway, but pre 2.0 save files were untagged and
   * support only netlist modules.  If no version tag is present, this is a non-tkgate
   * save file and we want to use the default of all modules being HDL.  This allows
   * us to read a regular verilog file into tkgate.
   */
  ycSetDefaultModuleType(NETMODULE);

  /* Loaded file with outdated version... */
  if ((flags & VF_OLD))
    message(1,msgLookup("err.oldversion"),cur.fileName,version,VERSIONS[0].vd_name);

  /* Loaded file with no backwards compatability */
  if ((flags & VF_NOBACK))
    message(1,msgLookup("err.noback"),cur.fileName,version,VERSIONS[0].vd_name);

  /* Loaded file with future? version number */
  if ((flags & VF_UNKNOWN))
    message(1,msgLookup("err.futureversion"),cur.fileName,version,VERSIONS[0].vd_name);


  VerSetSpecialLoadFeatures(&V);
}

/*****************************************************************************
 *
 * Apply a circuit property from the save file.
 *
 *
 * Note, some properties are 'file' properties, some are 'circuit'
 * properties and some are both.   If the file being loaded is a library,
 * or append file, then circuit properties are ignored and only file properties
 * are applied for the scope of the load operation.
 *
 *****************************************************************************/
void VerCircuitProp(const char *name,const void *value,int ptype)
{
  int bad_type = 0;
  int do_circ_props = (cur.libraryName == 0);
  Circuit *c = TkGate.circuit;


  if (strcmp(name,"title") == 0) {
    if (ptype == TYPE_STR) {
      if (do_circ_props)
	Circuit_setTitle(value);
    } else
      bad_type = 1;
  } else if (strcmp(name,"locale") == 0) {
    Locale *locale = SHash_find(TkGate.localeTable, value);
    if (!locale) {
      yyerror(msgLookup("err.yy.badpropvalue"),value,name);
      locale = SHash_find(TkGate.localeTable, "en");
    }
    Circuit_setLocale(c, locale);
#if LOCALE_DEBUG
    printf("file locale is <%s>\n",locale->l_code);
#endif
  } else if (strcmp(name,"encoding") == 0) {
    Circuit_setFileEncoding(c, value);
    parserEncoder = Circuit_getLoadFileEncoder(c);
#if LOCALE_DEBUG
    printf("parserEncoding <%s> to <%s>\n",value,CE_UTF8);
#endif
  } else if (strcmp(name,"prefix") == 0) {
    if (ptype == TYPE_STR)
      VerilogReadPrefix = yc_strdup(value);
    else
      bad_type = 1;
  } else if (strcmp(name,"discardChanges") == 0) {
    if (ptype == TYPE_INT) {
      if (do_circ_props)
	c->discardChanges = *(int*)value;
    } else
      bad_type = 1;
  } else if (strcmp(name,"timingViolationMode") == 0) {
    c->c_tvMode = *(int*)value;
  } else if (strcmp(name,"initTime") == 0) {
    char units[STRMAX];
    int ucode;

    if (sscanf(value,"%lf %s",&c->c_startup,units) != 2
	|| c->c_startup < 0
	|| (ucode = Timescale_unitsToCode(units)) < 0) {
      yyerror(msgLookup("err.yy.badpropvalue"),value,name);
      return;
    }
    c->c_startupUnits = Timescale_unitsToCode(units);

    //fprintf(f,"//: property initTime = \"%g %s\"\n",c->c_startup,
    //  Timescale_codeToUnits(c->c_startupUnits));
  } else if (strcmp(name,"technology") == 0) {
    if (ptype == TYPE_STR) {
      if (do_circ_props) {
	if (TkGate.defaultTech)
	  Tcl_Free(TkGate.defaultTech);
	TkGate.defaultTech = Tcl_Alloc(strlen(value)+1);
	strcpy(TkGate.defaultTech,(char*)value);
	DoTcl("catch { ToolBar::resetTechList }");
      }
    } else
      bad_type = 1;

  } else if (strcmp(name,"useExtBars") == 0) {
    if (ptype == TYPE_INT) {
      if (do_circ_props) {
	c->useExtBars = *(int*)value;
      }
    } else
      bad_type = 1;
  } else if (strcmp(name,"showSwitchNets") == 0) {
    if (ptype == TYPE_INT) {
      if (do_circ_props) {
	c->showSwitchNets = *(int*)value;
      }
    } else
      bad_type = 1;
  } else if (strcmp(name,"simClockMode") == 0) {
    if (ptype == TYPE_INT) {
      if (do_circ_props) {
	c->simClockMode = *(int*)value;
      }
    } else
      bad_type = 1;
  } else if (strcmp(name,"simClockName") == 0) {
    if (ptype == TYPE_STR) {
      if (do_circ_props) {
	c->simClockName = ob_strdup(value);
      }
    } else
      bad_type = 1;
  } else if (strcmp(name,"simAutoStart") == 0) {
    if (ptype == TYPE_INT) {
      if (do_circ_props) {
	c->simAutoStart = *(int*)value;
      }
    } else
      bad_type = 1;
  } else {
    yyerror(msgLookup("err.yy.badprop"),name);
  }

  if (bad_type)
    yyerror(msgLookup("err.yy.baddata"),name);
}

/*
 * Called to set a symbol for a module instance
 */
void VerSetBlockSymbol(unsigned id)
{
  if (modSymbolTable) {
    GModSymbol *ms = (GModSymbol*) NHash_find(modSymbolTable,id);
    cur.gate = SymBlock_convert(cur.gate,ms,cur.mod,0);
  }
}


/*
 * Called when parsing a /symbolBegin (with non-zero id), or a /symbolEnd (with 0 id).
 */
void VerSymbol(unsigned id)
{
  if (id) {
    cur.icon.ms = new_GModSymbol();
    if (modSymbolTable) {
      NHash_insert(modSymbolTable,id,cur.icon.ms);
    }
  } else {
    cur.icon.ms = 0;
  }
}

/*
 * This function is called at the beginning and end of an icon definition block.
 * At the end of the block it is called with all zeros.
 */
void VerSetIcon(const char *name,int N,int x,int y)
{
  GModSymbol *ms = cur.icon.ms;

  if (!ms) {
    yyerror(msgLookup("err.yy.nosym"));
    return;
  }

  if (name) {
    if (N < 0) N = 0;
    N++;			/* space for end-of-string */

    cur.icon.N = N;
    cur.icon.data = cur.icon.p = (char*) ob_malloc(N,"char*");
    *cur.icon.p = 0;
    cur.icon.x = x;
    cur.icon.y = y;

    if (strcmp(name,"normal") == 0) {
      cur.icon.which = 0;
    } else if (strcmp(name,"select") == 0) {
      cur.icon.which = 1;
    } else {
      yyerror(msgLookup("err.yy.badicon"),name);
      return;
    }
  } else {
    if (cur.icon.which == 0)
      GModSymbol_setNormalIcon(ms,cur.icon.data,cur.icon.x,cur.icon.y);
    else
      GModSymbol_setSelectIcon(ms,cur.icon.data,cur.icon.x,cur.icon.y);

  }
}

void VerSymPort(const char *ptype,const char *name,int nbits,int x,int y,int orient)
{
  GModSymbol *ms = cur.icon.ms;
  int t = IN;

  if (!ms) {
    yyerror(msgLookup("err.yy.nosym"));
    return;
  }

  if (strcmp(ptype,"input") == 0)
    t = IN;
  else if (strcmp(ptype,"output") == 0)
    t = OUT;
  else if (strcmp(ptype,"inout") == 0)
    t = TRI;

  GModSymbol_addPort(ms,new_GSymPort(name,x,y,t,orient,nbits));
}

void VerIconData(const char *data)
{
  int L = strlen(data);

  if ( (cur.icon.p - cur.icon.data) + L+1 >= cur.icon.N) {
    yyerror(msgLookup("err.yy.toomuchdata"));
    return;
  }

  cur.icon.p += sprintf(cur.icon.p,"%s\n",data);
}

/*
 * Set a module property
 */
void VerModuleProp(const char *name,const void *value,int ptype)
{
  int bad_type = 0;

  if (strcmp(name,"prot_data") == 0) {
    if (ptype == TYPE_INT) {
      if (cur.mod)
	cur.mod->m_protData = *(int*)value;
    } else
      bad_type = 1;
  } else if (strcmp(name,"prot_intf") == 0) {
    if (ptype == TYPE_INT) {
      if (cur.mod)
	cur.mod->m_protIntf = *(int*)value;
    } else
      bad_type = 1;
  } else if (strcmp(name,"symbol") == 0) {
    if (cur.mod) {
      unsigned id = *(unsigned*)value;
      GModSymbol *ms;

      /*
       * Convert the reference to a symbol block.
       */
      if (cur.mod && modSymbolTable && cur.mod->m_interface) {
	GCElement *g = cur.mod->m_interface;
	ms = (GModSymbol*) NHash_find(modSymbolTable,id);
	if (ms)
	  g = SymBlock_convert(g,ms,TkGate.circuit->mid_mod,1);
	else
	  yyerror(msgLookup("err.yy.nosyminst"),id,g->ename,cur.mod->m_name);
	cur.gate = g;
      }
    }
  } else if (strcmp(name,"pptype") == 0) {
    if (ptype == TYPE_INT) {
      if (cur.mod)
	cur.mod->m_pptype = *(int*)value;
    } else
      bad_type = 1;
  } else {
    yyerror(msgLookup("err.yy.badmodprop"),name);
  }

  if (bad_type)
    yyerror(msgLookup("err.yy.badmoddata"),name);
}


void VerAddScript(const char *name)
{
  if (TkGate.tcl) {
    DoTcl("gat_setCircProp -script \"%s\"",name);
  }
}

void VerAddVLib(const char *name)
{
  if (cur.numRequires == 0) {
    cur.requires = (char**) malloc(8*sizeof(char*));
  } else if ((cur.numRequires % 8) == 0) {
    cur.requires = (char**) realloc(cur.requires,(cur.numRequires+8)*sizeof(char*));
  }
  cur.requires[cur.numRequires++] = strdup(name);
}

/*****************************************************************************
 *
 * Create a new module from a verilog description
 *
 * Parameters:
 *     mtype		Type of module (MT_NETLIST or MT_TEXTHDL)
 *     name		Name of the module
 *     isMain		Non-zero if this is the top-level module
 *
 * Creates a new module and sets cur.mod equal to the generated module.  In
 * most cases, the new module is also registered with the circuit, but if we
 * are currently reading a library and the module already exists as a non-library
 * module, then we do not want to overwrite that module.  In this case we
 * create a module, set a delete flag and do not register the module.
 *
 *****************************************************************************/
void VerNewModule(int mtype, const char *name,int isMain)
{
  GModuleDef *M;

  cur_initModule();

  if (cur.libraryName) {
    /*
     * Prepare to read in a library module.  We create a module definition even
     * if we don't intend to retain it in the final circuit so we don't cause problems
     * for the parser.  We set the flag cur.del_mod if we want to delete the module
     * after loading.  We retain a library module if: the block does not exist in the
     * current circuit, if the existing block is also a library block, or the existing
     * module is an empty block other than the root module.
     */
    M = env_findModule(name);
    if (!M  || M->m_isLib ||  (!GModuleDef_isTop(M) && Hash_numElems(M->m_gates) == 0 && !M->m_text)) {
      M = env_removeModule(name,1);
      if (M) delete_GModuleDef(M);
      M = new_GModuleDef(name);
    } else {
      M = new_GModuleDef(name);
      cur.del_mod = 1;
    }
    if (isMain)
      cur.del_mod = 1;

    GModuleDef_setLibrary(M,cur.libraryName);
  } else {
    M = env_removeModule(name,1);
    if (!M) M = new_GModuleDef(name);
    if (isMain) {
      TkGate.circuit->root_mod = M;
    }
  }

  GModuleDef_setFileName(M,cur.fileName);

  /*
   * mtype is passed using yacc tokens.  We need to translate it to the internal
   * symbol codes we are using.
   */
  M->m_type = (mtype == NETMODULE) ? MT_NETLIST : MT_TEXTHDL;

  env_clear(M);
  cur.mod = M;
  if (isMain && !topModule) {
    topModule = M;
    TkGate.circuit->root_mod = M;
  }

  if (!cur.del_mod) {
    env_insertModule(cur.mod);
    if (!GModuleDef_isTop(cur.mod))
      modint_setInterface(cur.mod,0);
  }
}

/*****************************************************************************
 *
 * Collects hash parameter list data of the form:
 *
 * #(.a(4), b = 8)
 *
 * These parameter lists are used in module declarations.  This function is
 * only called when reading a native (non-tkgate) verilog file.  Parameter
 * values and expressions are stored so that they can be regenerated as part
 * of an HDL module definition.  
 *
 *****************************************************************************/
void VerModHashParm(const char *name,Expr *e)
{
  char buf[STRMAX],*p;

  if (!nativeVerilog) return;

  if (!cur.modHPorts) cur.modHPorts = new_List();

  p = buf + sprintf(buf,"%s = ",name);
  Expr_sprint(p,STRMAX-strlen(buf),e);
  
  List_addToTail(cur.modHPorts, (char*)strdup(buf));
}

void VerModParm(const char *name)
{
  if (!nativeVerilog) return;

  if (name) {
    /*
     * If called with non-null 'name' we are adding ports to the module.
     */
    if (!cur.modPorts) cur.modPorts = new_List();
    List_addToTail(cur.modPorts, (char*)name);
  } else {
    /*
     * If called with null 'name' we generate the module declaration line.
     */
    char buf[STRMAX],*p;

    p = buf;
    p += sprintf(p,"module %s",cur.mod->m_name);

    if (cur.modHPorts && List_numElems(cur.modHPorts) > 0) {
      ListElem *le;
      int doComma = 0;

      p += sprintf(p," #(");
      for (le = List_first(cur.modHPorts);le;le = List_next(cur.modHPorts, le)) {
	const char *name = (const char*) ListElem_obj(le);
	if (doComma) p += sprintf(p,", ");
	doComma = 1;
	p += sprintf(p,"%s",name);
      }
      p += sprintf(p,") ");
    }


    if (cur.modPorts && List_numElems(cur.modPorts) > 0) {
      ListElem *le;
      int doComma = 0;

      p += sprintf(p,"(");
      for (le = List_first(cur.modPorts);le;le = List_next(cur.modPorts, le)) {
	const char *name = (const char*) ListElem_obj(le);
	if (doComma) p += sprintf(p,", ");
	doComma = 1;
	p += sprintf(p,"%s",name);
      }
      p += sprintf(p,")");
    }

    p += sprintf(p,";");
    VerAddHdlLine(buf);
  }
}

/*****************************************************************************
 *
 * Called to finish up reading a netlist module
 *
 *****************************************************************************/
void VerEndNetlistModule()
{
  HashElem *E;
  GWireList *wl;
  NHash *bad_wires;
  NHash *bad_nets;

  bad_wires = new_NHash_noob();
  bad_nets = new_NHash_noob();

  /*
     Fix up wire nets.  There may be redunant processing here,
     but I'm too lazy to worry about it right now.
   */
  for (E = Hash_first(cur.mod->m_nets);E;E = Hash_next(cur.mod->m_nets,E)) {
    GNet *n = (GNet*) HashElem_obj(E);
    if (n->n_driver) {
      wire_finalizeNet(n->n_driver);
    } else {
      /* Net %s has no driver in module %s. (only for net list) */
      if (cur.mod->m_type != MT_TEXTHDL)
	message(1,msgLookup("err.nodrive"),n->n_signame,cur.mod->m_name);
    }
  }

  for (E = Hash_first(cur.netWEnds);E;E = Hash_next(cur.netWEnds,E)) {
    GWire **wends = (GWire**) HashElem_obj(E);
    ob_free(wends);
  }
  SHash_flush(cur.netWEnds);

  for (wl = cur.mod->m_wires;wl;wl = wl->wl_next) {
    GWire *w1 = wl->wl_wire;
    GWire *w2 = wire_driver(w1);

    if (w1 == w2) continue;

    if (!w1->gate && !w2->gate) {
      NHash_insert(bad_wires,(int)w2,w2);
      NHash_insert(bad_wires,(int)w1,w1);
      NHash_insert(bad_nets,(int)w1->net,w1->net);
    }
  }

  if (Hash_numElems(bad_wires) > 0) {
    for (E = Hash_first(bad_wires);E;E = Hash_next(bad_wires,E)) {
      GWire *W = (GWire*) HashElem_obj(E);

      message(1,msgLookup("err.noconn"),W->net->n_signame,W->nidx); /* Wire %s{%d} has no connections - deleted. */
      cur.mod->m_wires = wire_unlink(cur.mod->m_wires,W);
    }
    for (E = Hash_first(bad_nets);E;E = Hash_next(bad_nets,E)) {
      GNet *n = (GNet*) HashElem_obj(E);

      delete_GNet(n);
    }
  }

  /*
   * If file version does not match current version, apply the version
   * delta function to all gates in the module.
   */
  if (VersionCmp(&cur.gate_ver,&cur.ver) != 0) {
    for (E = Hash_first(cur.mod->m_gates);E;E = Hash_next(cur.mod->m_gates,E)) {
      GCElement *g = (GCElement*) HashElem_obj(E);
      GGateInfo *gi = g->typeinfo;
      if (gi->VersionDelta)
	(*gi->VersionDelta)(g,&cur.ver);
    }
  }

  if (cur.del_mod) {
    cur_scheduleModDelete(cur.mod);
    cur.mod = 0;
  }


  delete_NHash(bad_wires);
  delete_NHash(bad_nets);

  cur.mod = 0;
}

/*****************************************************************************
 *
 * Called to finish up reading an HDL  module
 *
 *****************************************************************************/
void VerEndHDLModule()
{
  if (cur.modPorts) {
    delete_List(cur.modPorts);
    cur.modPorts = 0;
  }
  if (cur.modHPorts) {
    delete_List(cur.modHPorts);
    cur.modHPorts = 0;
  }

  if (nativeVerilog) {
    VerAddHdlLine("endmodule");
  }

  if (TkGate.tcl) {
    if (nativeVerilog && !GModuleDef_isTop(cur.mod))
      igen_generate_initial(cur.mod);
  }

  cur.mod = 0;
}

/*****************************************************************************
 *
 * Called to finish up reading a module and call the appropriate finish function.
 *
 *****************************************************************************/
void VerEndModule()
{
  VerCallMParmFlush();

  if (cur.mod->m_type == MT_TEXTHDL)
    VerEndHDLModule();
  else
    VerEndNetlistModule();

  cur_endModule();
}

/*****************************************************************************
 *
 * Make the current module the root module.
 *
 *****************************************************************************/
void VerSetRootMod()
{
  if (!cur.libraryName) {
    topModule = cur.mod;
    TkGate.circuit->root_mod = cur.mod;
  } else {
    if (!cur.del_mod)
      env_removeModule(cur.mod->m_name,1);
    cur.del_mod = 1;
  }
}

/*****************************************************************************
 *
 * Finish up a net declaration.
 *
 *****************************************************************************/
void VerFinishNet()
{
  GWire **wends;
  int i;

  if (!cur.net) return;

#if 0
  printf("FinishNet: %s\n",cur.net->n_signame);
#endif

  wends = (GWire**) ob_calloc(cur.numWEnds,sizeof(GWire*),"GWire*[]");
  for (i = 0;i < cur.numWEnds;i++)
    wends[i] = cur.wends[i];
  cur.net->n_wnum = cur.numWEnds;

  SHash_insert(cur.netWEnds,cur.net->n_signame,wends);

  if (!VerilogReadPrefix) {
    /*
     * Tag something if we are reading an old-style file.
     */
    if (GNet_getShowName(cur.net)) {
      GNet_showName(cur.net);
    }
  }

  cur.net = 0;
}

/*****************************************************************************
 *
 * Called at end of net declarations in a netlist module.
 *
 *****************************************************************************/
void VerEndDecls()
{
  VerFinishNet();
}

/*****************************************************************************
 *
 * Begin a new net declarations.
 *
 * Parameters:
 *     name		Name of the new net.
 *     nbits		Number of bits on the net.
 *     vtype		Verilog type code (by token code)
 *
 *****************************************************************************/
void VerNewNet(const char *name,int nbits,int vtype)
{
  VerFinishNet();

  cur.net = (GNet*) new_GNet(name,cur.mod);

  /*
   * Need to map the token code to a verilog type code
   */
  switch (vtype) {
  case TRI :
    cur.net->n_vtype = NV_TRI;
    break;
  case WIRE :
    cur.net->n_vtype = NV_WIRE;
    break;
  case WAND :
    cur.net->n_vtype = NV_WAND;
    break;
  case TRIAND :
    cur.net->n_vtype = NV_TRIAND;
    break;
  case WOR :
    cur.net->n_vtype = NV_WOR;
    break;
  case TRIOR :
    cur.net->n_vtype = NV_TRIOR;
    break;
  case TRI0 :
    cur.net->n_vtype = NV_TRI0;
    break;
  case TRI1 :
    cur.net->n_vtype = NV_TRI1;
    break;
  case TRIREG :
    cur.net->n_vtype = NV_TRIREG;
    break;
  default :
    cur.net->n_vtype = NV_WIRE;
  }

  net_setSize(cur.net,nbits);
  cur.numWEnds = 0;
}

/*****************************************************************************
 *
 * Add a line of HDL text to the current module
 *
 * Parameters:
 *     text		Line of text to add.
 *
 *****************************************************************************/
void VerAddHdlLine(const char *text)
{
  int mlen,alen,msize,amsize;
  char buf[STRMAX];

  recodeText(parserEncoder,buf,STRMAX,text);
  text = buf;

  cur.mod->m_type = MT_TEXTHDL;

  if (!cur.mod->m_text) {
    GModuleDef_allocText(cur.mod,HDLTEXTBLOCKSIZE);
    *cur.mod->m_text = 0;
  }

  mlen = strlen(cur.mod->m_text);
  alen = strlen(text);
  msize = ((mlen+1+HDLTEXTBLOCKSIZE-1) / HDLTEXTBLOCKSIZE)*HDLTEXTBLOCKSIZE;		/* Current buffer size */
  amsize = ((mlen+alen+2+HDLTEXTBLOCKSIZE-1) / HDLTEXTBLOCKSIZE)*HDLTEXTBLOCKSIZE;	/* Required buffer size */

  if ( msize != amsize) {
    GModuleDef_allocText(cur.mod,amsize);
  }

  sprintf(cur.mod->m_text+mlen,"%s\n",text);
}

/*****************************************************************************
 *
 * Create a primitive gate from its base name
 *
 * Parameters:
 *
 *
 *
 *****************************************************************************/
void VerGate(const char *func,const char *name)
{
  GGateInfo *gi = 0;
  int vnum = 0;
  GCElement *g;

  /*
   * Try new-style primitive gate names
   */
  gi = GGateInfo_lookup(func);

  /*
   * If not found, try old-style primitive gate names
   */
  if (!gi) {
    gi = GGateInfo_vlookup(func);
    vnum = gi ? GGateInfo_variantNum(gi,func) : 0;
  }

  if (gi) {
    g = (*gi->MakeFunction)(0,cur.mod,gi->Code,0,0,0,name,1,0,0);
    cur.gate = g;
    cur.vnum = vnum;
    cur.numPins = 0;
    cur.pidx = 0;
    cur.revPads = 0;
  } else {
    yyerror(msgLookup("err.yy.badprim"),func);
    cur.gate = 0;
  }
}

/*****************************************************************************
 *
 * Rename the current instance.
 *
 * Parameters:
 *     name		New name for current instance.
 *
 *****************************************************************************/
void VerRenameCurrent(const char *name)
{
  if (cur.gate) {
    gate_setName(cur.gate, name, cur.mod);
  }
}

/*****************************************************************************
 *
 * Create a new gate from a cell name.
 *
 * Parameters:
 *   func		Name of function (a block name or a _GG primitive name)
 *   name		Name of the instance
 *
 *****************************************************************************/
void VerCell(const char *func,const char *name)
{
  GGateInfo *gi = 0;
  int vnum = -1;
  GCElement *g;

  if (VerilogReadPrefix) {
    /*
     * Use new-style primitive gate names
     */
    GCellSpec gcs;


    if (GGateInfo_decodeTypeName(func,&gcs)) {
      if (translateDemux && strcmp(gcs.gc_info->name,"DEMUX") == 0) {
	char funcx[STRMAX],*p;

	strcpy(funcx,func);
	p = strstr(funcx,"DEMUX");
	memmove(p+2,p,strlen(p)+1);
	memmove(p,"DECODER",7);
	GGateInfo_decodeTypeName(funcx,&gcs);
      }

      gi = gcs.gc_info;
      strcpy(cur.invSpec,gcs.gc_invSpec);
      vnum = -1;
    } else
      strcpy(cur.invSpec,"");

  } else {
    /*
     * Use old-style primitive gate names
     */

    /*
     * Always translate "demux" for old save files.
     */
    if (strcmp(func,"demux") == 0) {
      func = "olddemux";
    }

    gi = GGateInfo_vlookup(func);
    vnum = gi ? GGateInfo_variantNum(gi,func) : 0;
  }

  if (gi) {
    g = (*gi->MakeFunction)(0,cur.mod,gi->Code,0,0,0,name,1,0,0);
  } else {
    gi = GGateInfo_lookup("MODULE");
    g = (*gi->MakeFunction)(0,cur.mod,gi->Code,0,0,0,name,1,0,0);
    g->u.block.moduleName = ob_strdup(func);

    if (cur.mparm_list) {
      ListElem *le;

      for (le = List_first(cur.mparm_list);le;le = List_next(cur.mparm_list, le)) {
	char **pair = (char**) ListElem_obj(le);
	Block_setParmPort(g,pair[0],pair[1]);
      }
    }
  }
  cur.gate = g;
  cur.vnum = vnum;
  cur.numPins = 0;
  cur.pidx = 0;
  cur.revPads = 0;
}

/*****************************************************************************
 *
 * Complete construction of a gate read from verilog file.
 *
 *****************************************************************************/
void VerEndGate()
{
  if (!cur.gate) return;
  if (cur.vnum == 0) {
    cur.gate = 0;
    return;
  }

  if (cur.vnum != -1) {
    int N = GCElement_numPads(cur.gate);
    int M = spread(cur.gate->typeinfo->vmask,cur.vnum);
    int i;

    for (i = 0;i < N;i++) {
      GWire *w;
      if (!(M & (1<<i))) continue;

      for (w = cur.gate->wires[i];w;w = w->next)
	w->invert = 1;
    }
  } else {
    char *ip = cur.invSpec;
    GGateInfo *gi = cur.gate->typeinfo;
    int N = GCElement_numPads(cur.gate);
    int i;

    for (i = 0;i < N;i++) {
      GWire *w;

      if (gi->Pad[i].iotype == IN) continue;

      for (w = cur.gate->wires[i];w;w = w->next) {
	if (*ip == 'N') {
	  w->invert = 1;
	}
	if (*ip && ip[1]) ip++;
      }
    }
  }

  cur.gate = 0;
}

void VerAttach(const char *pin,const char *net,int isComp)
{
  int p = cur.numPins++;

  cur_gpin_extend(p);
  cur.gPin[p] = pin;
  cur.gNet[p] = (GNet*) SHash_find(cur.mod->m_nets,net);
  cur.gComp[p] = isComp;
  if (!cur.gNet[p])
    yyerror(msgLookup("err.yy.nonet"),net);
}

void VerRevPad(int pad)
{
  cur.revPads |= 1 << pad;
}

void VerTranDup()
{
  int s = cur.numPins-1;
  int d = cur.numPins++;

  cur_gpin_extend(d);
  cur.gPin[d] = "D";
  cur.gNet[d] = cur.gNet[s];
  cur.gComp[d] = 0;
}

void VerTranRange(int msb,int lsb)
{
  cur.gate->u.tap.msb = msb;
  cur.gate->u.tap.lsb = lsb;
}

void VerJointNet(const char *nname)
{
  static GGateInfo *pi = 0;
  GNet *n;
  int i;

  if (!pi) pi = GGateInfo_lookup("JOINT");
  n = (GNet*) SHash_find(cur.mod->m_nets,nname); 
  for (i = 0;i < pi->NumPads;i++) {
    cur_gpin_extend(i);
    cur.gPin[i] = pi->Pad[i].Name;
    cur.gNet[i] = n;
    cur.gComp[i] = 0;
  }
  cur.numPins = pi->NumPads;
}

void VerMakeNode(int x,int y,int showSize,int isLabeled,int offset)
{
  GWireNode *n = new_GWireNode();

#if 0
  printf("%p: makeNode(%d, %d, size:%d, islabeled:%d, offset:%d)\n",n,x,y,showSize,isLabeled,offset);
#endif

  n->x = x;
  n->y = y;
  n->showSize = showSize;
  n->isLabeled = isLabeled;
  if (offset >= 0) {
    n->offset = offset;
    n->labelSide = 0;
  } else {
    n->offset = -offset;
    n->labelSide = 1;
  }

  if (!cur.node) {
    cur.wire->nodes = n;
    n->end = cur.wire;
  } else {
    cur.node->out = n;
    n->in = cur.node;
  }
  cur.node = n;
}

void VerMakeWire(int p)
{
  GWire *w;

  while (cur.numWEnds <= p) {
    cur_wends_extend(cur.numWEnds);
    cur.wends[cur.numWEnds++] = 0;
  }

  w = wire_newend(cur.mod,cur.net,0);
  cur.wends[p] = w;
  w->nidx = p;

  if (p == 0)
    w->net->n_driver = w;

  if (cur.wire) {
    if (cur.node) {
      w->nodes = cur.node;
      w->nodes->end = w;
      w->driver = cur.wire;
    }
    cur.wire = 0;
  } else {
    cur.wire = w;
    w->driver = w;
  }
  cur.node = 0;
}

void VerSetPos(int x,int y)
{
  if (cur.gate) {
    cur.gate->xpos = x;
    cur.gate->ypos = y;
  }
}

void VerSetSize(int w,int h)
{
  if (cur.gate && cur.gate->typeinfo->Code == GC_BLOCK) {
    cur.gate->u.block.gwidth = w;
    cur.gate->u.block.gheight = h;
  }
}

void VerSetRot(int r)
{
  if (cur.gate) 
    cur.gate->orient = r;
}

void VerSetShowName(int sn)
{
  if (cur.gate)
    cur.gate->show_name = sn;
  else if (cur.net) {
    cur.net->n_show_name = sn;
    net_update(cur.net);
  }
}

void VerSetWireDecorationPos(int wire,int segment,int offset)
{
}

void VerSetInterfaceProperty(const char *prop,int n)
{
  if (!cur.mod) return;

  if (strcmp(prop,"/pd") == 0)
    cur.mod->m_protData = n;
  else if (strcmp(prop,"/pi") == 0)
    cur.mod->m_protIntf = n;
  else if (strcmp(prop,"/pe") == 0)
    cur.mod->m_protEdit = n;
  else if (strcmp(prop,"/pp") == 0)
    cur.mod->m_pptype = n;
}

void VerSetProperty(const char *prop,int n)
{
  if (cur.isInterface) {
    VerSetInterfaceProperty(prop, n);
    return;
  }


  if (strcmp(prop,"/anc") == 0)
    cur.gate->anchored = (n != 0);

  if (strcmp(prop,"/cpc") == 0)
    cur.gate->cpath_cut = (n != 0);

  if (cur.gate->typeinfo->SetProp)
    (*cur.gate->typeinfo->SetProp)(cur.gate,prop,&n);
}

void VerSetStrProperty(const char *prop,const char *value)
{
  if (strcmp(prop,"/tech") == 0) {
    cur.gate->cust_delay = 0; 
    if (cur.gate->tech) ob_free(cur.gate->tech);
    cur.gate->tech = ob_strdup(value);
  }
  if (strcmp(prop,"/delay") == 0) {
    char buf[STRMAX],*p;
    int i;

    cur.gate->cust_delay = 1; 
    strcpy(buf,value);
    for (i = 0, p = strtok(buf," ");p;i++, p = strtok(0," ")) {
      int d;
      sscanf(p,"%d",&d);
      cur.gate->delays[i] = d;
    }
  }

  if (cur.gate->typeinfo->SetProp)
    (*cur.gate->typeinfo->SetProp)(cur.gate,prop,value);
}

void VerAddScriptLine(const char *line)
{
  if (!cur.gate) return;

  if (cur.gate->typeinfo->SetProp)
    (*cur.gate->typeinfo->SetProp)(cur.gate,"/line",line);
}

static GWire *InsertPadWire(GWire *w,GWire *r)
{
  if (!r) {
    r = w;
  } else if (w == r) {
    yyerror(msgLookup("err.yy.bogussave"));
  } else if (w->PadNum < r->PadNum) {
    w->next = r;
    r = w;
  } else {
    r->next = InsertPadWire(w,r->next);
  }
  return r;
}

/*****************************************************************************
 *
 * Fasten a wire to a gate.
 *
 * Parameters:
 *    g			Gate on which wire is to be fastened
 *    pspec		The port specifier on the gate
 *    n			The net which is to be attached
 *    p			The wire position number
 *    bdir              The direction of the wire if applicable
 *
 *****************************************************************************/
static GWire *FastenToGate(GCElement *g,const char *pspec,GNet *n,int p,int bdir)
{
  GGateInfo *gi = cur.gate->typeinfo;
  GWire **wends = (GWire**) SHash_find(cur.netWEnds,n->n_signame);
  int N = GCElement_numPads(g);
  int padNum;				/* Pad number (which pad) */
  int padPos;				/* Pad position (number within pad) */
  GWire *w;				/* Wire to attach */
  extern GGateInfo gate_symblock_info;
  GPadLoc *loc;

  /*
   * Compensate for a bug with nmos/pmos in old save files.  Older save files
   * had the S and Z ports reversed.  The old simlator also had an independent
   * bug which caused the ports to be reveresed back to the correct position
   * so this problem went unnoticed.
   */
  if (!VerilogReadPrefix) {
    if (strcmp(gi->name,"NMOS") == 0 || strcmp(gi->name,"PMOS") == 0) {
      if (*pspec == 'Z')
	pspec = "S";
      else if (*pspec == 'S')
	pspec = "Z";
    }
  }

  w = wends[p];

  switch (bdir) {
  case '=' : w->WireDir = TRI; break;
  case '>' : w->WireDir = IN; break;
  case '<' : w->WireDir = OUT; break;
  }

  if (g->typeinfo == &gate_symblock_info) {		/* Symbol block instances */
    GModSymbol *ms = g->u.block.symbol;
    int i;
    padNum = 0;
    for (i = 0;i < N;i++) {
      GSymPort *p = GModSymbol_getPort(ms,i);
      if (strcmp(pspec,p->msp_name) == 0)
	padNum = i;
    }
    padPos = 0;
  } else if (pspec)  {					/* Instances with a pspec */
    char pn[STRMAX];

    switch (sscanf(pspec,"%[^0123456789]%d",pn,&padPos)) {
    case 2: 
      break;
    case 1:
      strcpy(pn,pspec);
      padPos = 0; 
      break;
    default :
      yyerror(msgLookup("err.yy.pinformat"),pspec);
      return 0;
    }
    for (padNum = 0;padNum < gi->NumPads;padNum++)
      if (strcmp(gi->Pad[padNum].Name,pn) == 0)
	break;

    if (padNum == N) {
      yyerror(msgLookup("err.yy.pinname"),pn,gi->name);
      return 0;
    }
  } else {						/* Instances with no pspec */
    padNum = padPos = 0;
  }

  loc = GCElement_getPadLoc(g,padNum,cur.gate->orient);
  w->PadNum = padPos;
  w->next = 0;
  if (cur.revPads & (1 << padNum)) {
    w->next = cur.gate->wires[padNum];
    cur.gate->wires[padNum] = w;
  } else
    cur.gate->wires[padNum] = InsertPadWire(w,cur.gate->wires[padNum]);
  w->gate = cur.gate;
  if (loc) w->orient = loc->dir;

#if 0
  printf("FastenToGate(%s/%s,%s,%s,%d,%d)  padNum=%d  padPos=%d ldir=%d\n",
	 g->ename, gi->name ,pspec, n->n_signame, p, bdir,padNum,padPos,loc->dir);
#endif

  if (gi->Code == GC_BLOCK)
    block_setWireEnd(cur.gate,w,padNum);

  return w;
}

/*****************************************************************************
 *
 * Place wire on a gate.
 *
 * Parameters:
 *     p	Place wire end p on gate
 *
 * This function is called for each element in the /w:[ ] group for a gate to
 * attach the specific wire ends to a gate.
 *
 *****************************************************************************/
void VerPlaceWire(int p)
{
  int x;			/* Position in argument list */ 
  GNet *n;			/* Net for position x */
  const char *pin;		/* Pin name for position x */
  GWire *w;

  x = cur.pidx++;
  if (p < 0) return;
  if (!cur.gate) return;

  n = cur.gNet[x];
  if (n) {
    pin = cur.gPin[x];
    w = FastenToGate(cur.gate,pin,n,p,0);
    if (cur.gComp[x]) {
      w->invert = 1;
    }
  } else {
    yyerror(msgLookup("err.yy.nonetpos"),x,cur.gate->ename);
  }
}

void VerBlockPort(const char *pname,int pdir,int widx)
{
  int x;			/* Position in argument list */ 
  GNet *n;			/* Net for position x */
  const char *pin;		/* Pin name for position x */
  GWire *w;

  x = cur.pidx++;
  if (!cur.gate) return;

  n = cur.gNet[x];
  pin = pname;
  w = FastenToGate(cur.gate,pin,n,widx,pdir);
  w->name = ob_strdup(cur.gPin[x]);
}

void VerBeginBD()
{
  GCElement *g;

  if (!cur.del_mod) {
    //    modint_setInterface(cur.mod,0);
    g = modint_find(cur.mod->m_name);
    cur.gate = g;
  } else
    cur.gate = 0;

  cur.isInterface = 1;
}

void VerEndBD()
{
  cur.gate = 0;
  cur.isInterface = 0;
}

/*
 * Add a port to a module interface definition.
 */
void VerBDPort(const char *pname,int pdir,const char *name,int nbits,int onum,int oden)
{
  int x,y;
  GWire *e1,*e2;
  GCElement *g = cur.gate;
  int p;

  if (!g) return; /* We are ignoring this interface */

  x = y = 0;

  p = GGateInfo_getPadNum(g->typeinfo,pname);
  if (p < 0) {
    printf("failed to find pad '%s'\n",pname);
    return;
  }

  wire_new(TkGate.circuit->mid_mod,&e1,&e2);

  switch (p) {
  case BLOCK_TIN :
  case BLOCK_TOUT :
  case BLOCK_TTRI :
    x = g->xpos + (g->u.block.gwidth*onum)/oden;
    y = g->ypos;
    break;
  case BLOCK_LIN :
  case BLOCK_LOUT :
  case BLOCK_LTRI :
    x = g->xpos;
    y = g->ypos + (g->u.block.gheight*onum)/oden;
    break;
  case BLOCK_RIN :
  case BLOCK_ROUT :
  case BLOCK_RTRI :
    x = g->xpos + g->u.block.gwidth;
    y = g->ypos + (g->u.block.gheight*onum)/oden;
    break;
  case BLOCK_BIN :
  case BLOCK_BOUT :
  case BLOCK_BTRI :
    x = g->xpos + (g->u.block.gwidth*onum)/oden;
    y = g->ypos + g->u.block.gheight;
    break;
  }
  e1->offset.num = onum;
  e1->offset.den = oden;
  block_attach(TkGate.circuit->mid_mod,g,e1,e2,x,y,name,pdir);

  net_setSize(e1->net,nbits);
}

/*
 * Check for trailing ".v"
 */
int isVerilogFile(const char *name)
{
  int l;

  if (!name) return 0;
  l = strlen(name);
  if (l < 2) return 0;

  if (strcmp(name+l-2,".v") == 0)
    return 1;
  else
    return 0;
}

void VerBeginDelayDef(const char *tech, const char *prim)
{
  GDelayDef_begin(tech,prim);
}

void VerEndDelayDef()
{
  GDelayDef_end();
}

void VerCallMParmFlush()
{
  if (cur.mparm_list)
    List_flush(cur.mparm_list);

  cur.mparm_list = 0;
}

void VerCallMParmAdd(const char *name,const char *value)
{
  char **pair;

  if (!cur.mparm_list)
    cur.mparm_list = new_List();

  pair = (char**) yc_malloc(2*sizeof(char*),ALGN_WORD);
  pair[0] = name ? yc_strdup(name) : 0;
  pair[1] = yc_strdup(value);

  List_addToTail(cur.mparm_list, pair);
}

void ycDirective(char *dtext)
{
  int n1,n2;
  char u1[STRMAX],u2[STRMAX],*p;

  if (strlen(dtext) > STRMAX) return;

  p = strrchr(dtext,'\n');
  if (p) *p = 0;

  if (sscanf(dtext, "`timescale %d %[^/ \t] / %d %s", &n1, u1, &n2, u2) == 4) {
    unsigned long long x1 = Timescale_parse(n1,u1);
    unsigned long long x2 = Timescale_parse(n2,u2);

    if (x1 == 0 || x2 == 0) {
      yyerror(msgLookup("err.yy.baddirective"),dtext); 
      return;
    }

    if (!cur.didTimescale) {
      TkGate.circuit->c_timescale.ts_units = x1;
      TkGate.circuit->c_timescale.ts_precision = x2;
      cur.didTimescale = 1;
    }
  }
}
