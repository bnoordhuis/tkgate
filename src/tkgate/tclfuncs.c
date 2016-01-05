/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

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

    Last edit by hansen on Tue May  5 20:53:56 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "tkgate.h"

char *readprintcapentry(char *buf,int N,FILE *f)
{
  char *p,*q;
  int l;

  for (;;) {
    p = buf;
    for (;;) {
      if (!fgets(p,N-(p-buf),f)) return 0;
      q = strrchr(p,'\n');
      if (q) *q = 0;
      l = strlen(p);
      if (l > 0 && p[l-1] == '\\') {
	p += l-1;
      } else
	break;
    }
    for (p = buf;*p && strchr(" \t",*p);p++);
    if (*p == '#' || *p == 0) continue;

    return buf;
  }
  return 0;
}

/*
  Check to see if modifications are OK, and if not issue a message. 
 */
int modifyOK(EditState *es,unsigned flags)
{
  if (!(flags & 0x2) && es->isInterface) {
    message(0,msgLookup("err.badeditop"));	/* Illegal interface edit mode operation. */
    return 0;
  }
  if (!(flags & 0x1) && GModuleDef_isDataProtected(es->env)) {
    message(0,msgLookup("err.protdata"),es->env->m_name);			/* Not a library module */      
    return 0;
  }
  return 1;
}

/*****************************************************************************
 *****************************************************************************/
static int gat_hdl(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  int i;
  const char *name = ((argc > 2) && (*argv[2] != '-')) ? argv[2] : 0;

#if 0
  {
    printf("gat_hdl");
    for (i = 1;i < argc;i++)
      printf(" %s",argv[i]);
    printf("\n");
  }
#endif

  if (argc < 2) return TCL_ERROR;

  for (i = 1;i < argc;i++) {
    if (strcmp(argv[i],"-cursor") == 0) {
      int line,pos;
      if (sscanf(argv[i+1],"%d.%d",&line,&pos) == 2) {
	hdl_saveCursor(name,line,pos);
	break;
      }
    }
  }


  if (strcmp(argv[1],"load") == 0)
    hdl_load(es->env);
  else if (strcmp(argv[1],"save") == 0) {
    if (hdl_save(name) < 0)
      sprintf(tcl->result,"cancel");
  } else if (strcmp(argv[1],"checksave") == 0) {
    if (hdl_checkSave(name) < 0)
      sprintf(tcl->result,"cancel");
  } else if (strcmp(argv[1],"close") == 0) {
    hdl_close();
  } else if (strcmp(argv[1],"getindex") == 0) {
    int line,pos;
    if (hdl_getCursor(&line,&pos) == 0) {
      sprintf(tcl->result,"%d.%d",line,pos);
    } else
      sprintf(tcl->result,"1.0");
  } else if (strcmp(argv[1],"touch") == 0) {
    ob_touch(TkGate.circuit);			/* Modify somthing to force undo to thing there are changes */
  }


  return TCL_OK; 
}



/*
 * Called to do final tkgate initilization
 */
static int gat_init(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkGate_init(tcl);
  ClearModified();
  ob_mode(OM_START);
 
  return TCL_OK;
}

static int gat_reinitDelay(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkGate_initDelay();
  return TCL_OK;
}

/*
 * Retrieve or store interface information about a module.
 *
 * gat_interface load <name> <array>
 * gat_interface generate <name> <options>
 * gat_interface puticon <name> <options>
 * gat_interface geticon <name> <options>
 *
 */
static int gat_interface(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc < 3) {
    strcpy(tcl->result,"too few arguments.");
    return TCL_ERROR;
  }

  if (igen_command(argv[1],argv[2],argc-3,argv + 3) != 0) {
    strcpy(tcl->result,"invalid command.");
    return TCL_ERROR;
  }

  return TCL_OK;
}

/*
   gat_new "name.v" topmod
*/
static int gat_new(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  const char *file,*topMod;
  GModuleDef *M;
  int old_mode;

  assert(argc == 3);

  tkgate_setMajorMode(MM_EDIT);

  old_mode = ob_get_mode();
  if (old_mode != OM_DISABLED) {
    ob_mode(OM_DISABLED);
  }
  ob_clear();

  file = argv[1];
  topMod = argv[2];
  ob_touch(TkGate.circuit);
  Circuit_initOptions(TkGate.circuit);
  Circuit_setTitle(file);
  ClearModified();

  Circuit_clear();
  Circuit_setCurrentFile(file);

  Circuit_unloadAllLibraries(TkGate.circuit);

  editstate_flushModules(es);

  M = env_findAdd(topMod,1);
  ob_touch(M);
  TkGate.circuit->root_mod = M;

  editstate_push(es,M,0);
  editstate_setCurrent(*es);
  ob_touch(TkGate.circuit);

  ClearModified();

  DoTcl("LibraryMgr::loadBaseLibraries");

  /*
   * Mark this so we can warn if we try to overwrite existing file.
   */
  TkGate.circuit->c_isNewFile = 1;

  FlagRedraw();

  if (old_mode == OM_ENABLED) {
    ob_mode(old_mode);
  }
  return TCL_OK;
}

/*
   gat_load "name.v"
*/
static int gat_load(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  const char *name;
  const char *modFlag;
  int old_mode;

  assert(argc == 2);
  name = argv[1];

  tkgate_setMajorMode(MM_EDIT);

  if ((modFlag = Tcl_GetVar(tcl,"tkg_modifiedFlag",TCL_GLOBAL_ONLY)) && *modFlag != '0') {
    DoTcl("File::confDelMods");
    if (!TkGate.tcl->result || strcmp(TkGate.tcl->result,"yes") != 0) {
      Tcl_SetResult(tcl,"0", TCL_STATIC);
      return TCL_OK;
    }
  }

  old_mode = ob_get_mode();
  if (old_mode != OM_DISABLED) {
    ob_mode(OM_DISABLED);
  }
  ob_clear();

  ob_touch(gw->parms->circuit);
  if (VerilogOpen(&gw->parms->circuit->es,name,0) < 0) {

    /*
     * We failed to open the file, but if there is no current file we assume
     * this was given on the command line and we are starting a new circuit.
     */
    if (!*CurrentFile_path(TkGate.circuit->currentFile)) {
      /*  Circuit_setCurrentFile(name);*/
    } else
      message(1,msgLookup("err.badopen"),name);	/* Unable to open input file '%s' */

    ClearModified();

    if (old_mode == OM_ENABLED) {
      ob_mode(old_mode);
    }

    Tcl_SetResult(tcl,"0", TCL_STATIC);
    return TCL_OK;
  }

  sel_clearDelta();

  Circuit_setCurrentFile(name);

  ClearModified();
  FlagRedraw();

  if (old_mode == OM_ENABLED) {
    ob_mode(old_mode);
  }

  Tcl_SetResult(tcl,"1", TCL_STATIC);
  return TCL_OK;
}

/*
   gat_loadMore "name.v"
*/
static int gat_loadMore(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  const char *name;

  assert(argc == 2);
  name = argv[1];

  tkgate_setMajorMode(MM_EDIT);

  ob_touch(gw->parms->circuit);

  if (VerilogOpen(&gw->parms->circuit->es,name,1) < 0) {
    sprintf(tcl->result,"0");
    return TCL_OK;
  }

  sel_clearDelta();

  Circuit_setCurrentFile(name);

  FlagRedraw();


  sprintf(tcl->result,"1");
  return TCL_OK;
}


static int gat_libIsLoaded(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc < 2) 
    return TCL_OK;

  if (SHash_find(TkGate.libraries,argv[1]))
    sprintf(TkGate.tcl->result,"1");
  else
    sprintf(TkGate.tcl->result,"0");

  return TCL_OK;
}

/*
   gat_loadLibrary "name.v"
*/
static int gat_loadLibrary(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  const char *name;

  assert(argc == 2);
  name = argv[1];

  Circuit_loadLibrary(TkGate.circuit,name);

  return TCL_OK;
}

/*****************************************************************************
 *
 * gat_unloadLibrary <name>
 *
 * Unload the named library.
 *
 *****************************************************************************/
static int gat_unloadLibrary(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  const char *name;

  assert(argc == 2);
  name = argv[1];

  Circuit_unloadLibrary(TkGate.circuit,name);

  FlagRedraw();

  return TCL_OK;
}

static int gat_mode(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  int mode = 0;

  assert(argc == 2);

  sscanf(argv[1],"%d",&mode);

  setEditMode(gw->parms->circuit->es,mode);

  return TCL_OK;
}


/*
   gat_make and {opts}
*/
static int gat_make(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  EditState **pes = &gw->parms->circuit->es;
  GGateInfo *GI;
  int x,y,r;
  GCElement *g;
  const char *xstr,*ystr;
  const char *dodraw_s = "1";
  int dodraw = 1;

  assert(argc >=  2);


  dodraw_s  = seekOption("-dodraw",argv+2,argc-2);
  if (dodraw_s)
    dodraw = (*dodraw_s != '0');

  /*
   * If we are in interface mode and we are requesting to make a block, use this as
   * an indication that we want to open that block for interface testing.
   */
  if (editstate_isInterfaceMode()) {
    const char *func = seekOption("-func",argv+2,argc-2);

    if (func) {
      modint_openSelected(func);
    }
    
    return TCL_OK; 
  }

  if (tkgate_currentMode() != MM_EDIT) return TCL_OK;
  if (!modifyOK(es,0)) return TCL_OK;

  xstr = seekOption("-x",argv+2,argc-2);
  ystr = seekOption("-y",argv+2,argc-2);

  if (xstr && ystr) {
    sscanf(xstr,"%d",&x);
    sscanf(ystr,"%d",&y);
    r = EditState_getRotation();
    x /= TkGate.circuit->zoom_factor;
    y /= TkGate.circuit->zoom_factor;
    x = wtoc_x(x);
    y = wtoc_y(y);
  } else if (TkGate.ed->mark_vis) {
    x = TkGate.ed->tx;
    y = TkGate.ed->ty;
    r = EditState_getRotation();
  } else {
    message(0,msgLookup("err.nomark"));	/* Please set a mark with the left mouse button before selecting a gate type. */
    return TCL_OK;
  }

  GI = GGateInfo_lookup(argv[1]);
  if (!GI) {
    message(1,msgLookup("err.badgate"),argv[1]);	/* Unknown gate type. */
    return TCL_OK;
  }

  badermessage(GI->name);

  mark_unpost();

  g = (*GI->MakeFunction)(pes,es->env,GI->Code,x,y,r,0,0,argv+2,argc-2);

  if (!g) return TCL_OK;			/* Make was canceled */

  if (TkGate.defaultTech) {
    ob_touch(g);
    ob_free(g->tech);
    g->tech = ob_strdup(TkGate.defaultTech);
  }

  if (dodraw)
    gate_draw(g,0);

  if ((GI->Code == GC_BLOCK || GI->Code == GC_SYMBLOCK))
    SetModified(MF_MODULE);

  if (dodraw && g && pes) {
    sel_appendGate(*pes,g,1);
    sel_finish(*pes);
    ob_touch(TkGate.circuit);
    TkGate.circuit->select = g;

    if ((GI->Code == GC_BLOCK || GI->Code == GC_SYMBLOCK) && r != 0) {
      gate_draw(g,0);
      while (r-- > 0)
	sel_rotate(*pes,1);
      gate_draw(g,0);
    }
  }

  scrollbar_bbx_update();

  SynchronizeInterface();

  return TCL_OK;
}

static int gat_closeBox(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;

  if (!(*es)->parent) {
    message(0,msgLookup("err.closeroot"));	/* "Can't close top-level module.  Use quit." */
    return TCL_OK;
  }

  ob_touch(TkGate.circuit);
  EditState_unselectGate(*es);
  editstate_pop(es);
  editstate_setCurrent(*es);
  ClearErrorMark();
  net_unselect(0);
  cpath_reshow();

  SetModified(MF_MODULE);
  SynchronizeInterface();

  FlagRedraw();

  return TCL_OK;
}

/*****************************************************************************
 *
 * Open the specified or currently selected module in the module editor.
 *
 * Usage:
 *    gat_openBox [<module-name>]
 *    gat_openBox -hierarchical [<module-name>]
 *
 *****************************************************************************/
static int gat_openBox(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  GModuleDef *M = 0;
  GCElement *b = 0;
  const char *modName = 0;
  int hierarchical = 1;

  if (argc == 2) {
    modName = argv[1];
    hierarchical = 0;
  } else if (argc == 3) {
    modName = argv[2];
    hierarchical = 1;
  }

  if (hdl_checkSave(0) < 0) {		/* Make sure hdl is up to date */
    sprintf(tcl->result,"cancel");
    return TCL_OK;
  }

  /*
   * Opening already selected module.
   */
  if (modName && es && *es && (*es)->env && strcmp((*es)->env->m_name,modName) == 0)
    return TCL_OK;

  ob_touch(TkGate.circuit);

  /*
   * If we are in interface mode, just change the interface we are editing.
   */
  if ((*es)->isInterface) {
    modint_openSelected(modName);
    return TCL_OK;
  }

  if (!hierarchical) {
    /* NOTE: The 'jump-to-module' feature can not be used in simulation or analysis mode. */

    if (tkgate_currentMode() == MM_SIMULATE) {
      SimInterface_navigateToModule(es,modName);
      return TCL_OK;
    } 
  }

  if (modName) {
    char *p;

    /*
     * Trim all but the last module name
     */
    p = strrchr(modName,'/');
    if (p) modName = p+1;

    M = env_findModule(modName);
    if (!M) {
      message(1,msgLookup("err.nomod"),modName);		/* Module '%s' is not defined. */
      return TCL_OK;
    }
  } else {
    b = TkGate.circuit->select;
    if (b && GCElement_isModule(b))
      M = env_findAdd(b->u.block.moduleName,0);
    else
      return TCL_OK;
  }

  if (M->m_hidden && tkgate_currentMode() == MM_EDIT) { 
    message(1,msgLookup("err.modlock"));			/* Logic Block is Locked! */
    return TCL_OK;
  }

  EditState_unselectGate(*es);

  if (hierarchical) {
    if (b) block_explode(b);
    editstate_push(es,M,b);
  } else {
    while (*es)
      editstate_pop(es);
    editstate_push(es,M,0);
  }
  editstate_setCurrent(*es);
  ClearErrorMark();
  net_unselect(0);
  cpath_reshow();

  SetModified(MF_MODULE|MF_SYNCONLY);
  SynchronizeInterface();

  FlagRedraw();

  return TCL_OK;
}

/*****************************************************************************
 *
 * Set top-level module.
 *
 * Usage:
 *    gat_reroot <module-name>
 *
 *****************************************************************************/
static int gat_reroot(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GModuleDef *M;

  assert(argc == 2);
  M = env_findModule(argv[1]);
  if (!M) {
    message(1,msgLookup("err.nomod"),argv[1]);		/* Module '%s' is not defined. */
    return TCL_OK;
  }

  Circuit_changeRoot(M);

  SynchronizeInterface();
  FlagRedraw();
  return TCL_OK;
}

/*****************************************************************************
 * Get the list of available printers 
 *
 * Usage:
 *    gat_getPrinterList
 *
 * The returned list is of the form: {{name1 longname1} {name2 longname2} ...}
 *
 *****************************************************************************/
static int gat_getPrinterList(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char *printerList = malloc(STRMAX);
  int N = STRMAX;
  int C = 0;
  FILE *f;

  strcpy(printerList,"{default \"Default Printer\"}");
  C = strlen(printerList);

  f = fopen("/etc/printcap","r");
  if (f) {
    char buf[STRMAX*2];

    while (readprintcapentry(buf,2*STRMAX,f)) {
      char *nameEntry;
      char *shortName = 0;
      char *longName = 0;
      char *t;
      int l;

      nameEntry = strtok(buf,":");
      for (t = strtok(nameEntry,"|");t && !(shortName && longName);t = strtok(0,"|")) {
	if (strchr(t,' ') && !longName)
	  longName = t;
	else if (!shortName)
	  shortName = t;
      }
      if (!shortName) continue;
      if (!longName) longName = shortName;

      l = strlen(shortName) + strlen(longName) + 6;
      if (C >= N) {
	N += STRMAX;
	printerList = realloc(printerList,N);
      }

      sprintf(printerList + C," {%s \"%s\"}",shortName,longName);
      C += l;
    }
    fclose(f);
  }

  Tcl_SetResult(TkGate.tcl, printerList, TCL_VOLATILE);

  free(printerList);

  return TCL_OK;
}

/*****************************************************************************
 *
 * setrot {0/90/180/270}
 *
 * set rotation for new gates
 *
 *****************************************************************************/
static int gat_setrot(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{ 
  char buf[STRMAX];
  int N = 0;

  assert(argc == 2);

  sscanf(argv[1],"%d",&N);
  EditState_setRotation(N);

  sprintf(buf,"%d",N);
  Tcl_SetVar(TkGate.tcl,"rot",buf,TCL_GLOBAL_ONLY);


  return TCL_OK;
}

/*
   Relay a command from the simulator to the scope
*/
static int gat_scopeCommand(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  SimInterface_command(&TkGate.circuit->simulator,argv[1]);
  return TCL_OK;
}

/*
   Relay a command from the analyzer to cpath 
*/
static int gat_analyzerCommand(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  cpath_command(argv[1]);
  return TCL_OK;
}

static int gat_errBoxReport(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  int N = 0;
  
  ob_touch(TkGate.circuit);

  if (argc != 2 || sscanf(argv[1],"%d",&N) != 1) {
    if (argc != 2)
      sprintf(tcl->result,"argument error argc=%d",argc);
    else
      sprintf(tcl->result,"argument error argv[1]=<%s>",argv[1]);
    return TCL_ERROR;
  }

  if (N >=0 && tkgate_currentMode() == MM_SIMULATE) {
    DoTcl("errmsg [m db.err.neededit]");
    return TCL_OK;
  }

  if (N >= 0)
    *es = EditState_moveToError(N,*es);
  else {
    Error_purge();
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Save the current circuit to a file.
 *
 * Tcl syntax:
 *
 * gat_save <filename>
 *
 *****************************************************************************/
static int gat_saveCircuit(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  const char *fileName = argv[1];

  assert(argc == 2);

  if (TkGate.circuit->c_isNewFile) {
    FILE *f = fopen(fileName,"r");
    int abortSave = 0;

    if (f) {
      DoTcl("yesno [format [m circ.reallyoverwrite] %s]",fileName);
      if (strcmp(tcl->result,"yes") != 0)
	abortSave = 1;

      fclose(f);
    }
    if (abortSave) return TCL_OK;
  }


  ob_begin_framef("-Save",FF_TRANSPARENT);
  ob_touch(TkGate.circuit);
  TkGate.circuit->c_isNewFile = 0;
  VerilogSave(fileName);
  Circuit_setCurrentFile(fileName);
  ob_end_frame();

  return TCL_OK;
}


static int gat_addPort(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  GCElement *g;
  GGateInfo *gi;
  int i;

  if (!modifyOK(es,2)) return TCL_OK;

  g = TkGate.circuit->select;
  if (!g) return TCL_OK;
  gi = g->typeinfo;

  if (gi->Code == GC_BLOCK) {
    message(1,msgLookup("err.oldportact"));
    return TCL_OK;
  }


  /* Find port that can be extended. */
  for (i = 0;i < gi->NumPads;i++)
    if (gi->Pad[i].CanAdd) break;

  if (i >= gi->NumPads) {
    message(1,msgLookup("err.badportadd"));
    return TCL_OK;
  }

  if (gi->Pad[i].iotype == IN )
    (*gi->AddInput)(es,TkGate.circuit->select);
  else if (gi->Pad[i].iotype == OUT)
    (*gi->AddOutput)(es,TkGate.circuit->select);
  else
    (*gi->AddTri)(es,TkGate.circuit->select);

  sel_refinish(es);

  return TCL_OK;
}

static int gat_changePinDir(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (!TkGate.circuit->select)
    return TCL_OK;

  (*TkGate.circuit->select->typeinfo->ChangePin)(es,TkGate.circuit->select);

  sel_refinish(es);

  return TCL_OK;
}

static int gat_dohyperlink(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc == 2) {
    dohyperlink(argv[1]);
  }
  return TCL_OK;
}

/*
    Delete selected gate.
*/
static int gat_deleteSelected(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (!es) return TCL_OK;

  if (!modifyOK(es,2)) return TCL_OK;

  if ((es)->isInterface) {
    sel_interfaceReset(es);
    return TCL_OK;
  }

  if (TkGate.circuit->select || TkGate.circuit->mg_selection) {
    sel_delete(es);
    scrollbar_bbx_update();
  } else if (DoTcl("tkg_getSelectedBlock") == TCL_OK && *tcl->result != 0) {
    DoTcl("BlockOp::delete");
  } else {
    message(1,msgLookup("err.nodel"));		/* "No deletable selection." */
    return TCL_OK;
  }

  SynchronizeInterface();

  return TCL_OK;
}

static int gat_replicate(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (!modifyOK(es,0)) return TCL_OK;

  setEditMode(es,MODE_REPLICATE);
  scrollbar_bbx_update();

  return TCL_OK;
}

static int gat_renameBlock(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  const char *oldName,*newName;
  const char *curName = TkGate.circuit->es->env->m_name;
  int isCurrentRename = 0;

  assert(argc == 3);
  oldName = argv[1];
  newName = argv[2];


  /*
   * Test to see if we are renaming the module we have open.
   */
  if (strcmp(oldName,curName) == 0) {
    isCurrentRename = 1;
  }

  if (checkValidName(argv[2],0) != 0) {
    message(1,msgLookup("err.badid"),argv[2]);
    return TCL_OK;
  }

  if (!env_findModule(oldName)) {
    message(1,msgLookup("err.modnotdef"),oldName);	/* "Module '%s' is undefined." */
    return TCL_OK;
  }

  if (env_findModule(newName)) {
    message(1,msgLookup("err.moddef"),newName);		/* "The block '%s' already exists." */
    return TCL_OK;
  }

  env_rename(oldName,newName);


  SetModified(MF_NET|MF_GATE|MF_MODULE);

  if (isCurrentRename) {
    editstate_setCurrent(*es);
    SynchronizeInterface();
    FlagRedraw();
  }

  return TCL_OK;
}

static int gat_computestrhash(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc != 2) {
    sprintf(tcl->result,"wrong number of parameters.");
    return TCL_ERROR;
  }

  sprintf(tcl->result,"%u",computestrhash(argv[1]));
  
  return TCL_OK;
}

static int gat_copyBlock(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (checkValidName(argv[2],0) != 0) {
    message(1,msgLookup("err.badid"),argv[2]);
    return TCL_OK;
  }

  env_copy(es,argv[1],argv[2]);

  SetModified(MF_MODULE);

  return TCL_OK;
}

/*****************************************************************************
 *
 * This function has no specific purpose.  It is simply a placehold to put
 * debugging code for whatever bug I am currently trying to fix
 *
 *****************************************************************************/
static int gat_debug(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GModuleDef *m = env_findModule("REGS");
  GCElement *g;
  int n;
  int i;

  if (GCElement_getType(m->m_interface) == GC_SYMBLOCK)
    g = m->m_interface;
  else
    g = m->m_altInterface;


  n = GCElement_numPads(g);

  printf("REGS-symbol %p\n",g);
  for (i = 0;i < n;i++) {
    const char *name = GCElement_getPadName(g,i);
    GWire *w = g->wires[i];
    printf("   pad %s:\n",name);
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * gat_deletBlock <name>
 *
 * Delete the named module
 *
 *****************************************************************************/
static int gat_deleteBlock(ClientData _d,Tcl_Interp *tcl,
			   int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (argv[1])
    env_delete(es,argv[1]);

  return TCL_OK;
}

static int gat_claimBlock(ClientData _d,Tcl_Interp *tcl,
			  int argc,const char *argv[])
{
  GModuleDef *M;

  if (argc < 2) return TCL_OK;

  M = env_findModule(argv[1]);
  if (M) {
    if (!M->m_isLib) {
      message(1,msgLookup("msg.notlib"),argv[1]);	/* Not a library module */      
      return TCL_OK;
    }

    /*    DoTcl("tkg_blockListSetLibFlag %s 0",argv[1]);*/
    GModuleDef_claim(M);
    SetModified(MF_MODULE);
  }

  return TCL_OK;
}

/*****************************************************************************
 * gat_newBlock name
 *
 *****************************************************************************/
static int gat_newBlock(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GModuleDef *M;
  char *aname;
  const char *modName;

  if (argc != 2) {
    strcpy(tcl->result,"wrong number of arguments.");
    return TCL_ERROR;
  }

  aname = (char*)argv[1];

  Tcl_SetVar2(tcl,aname,"ok","0",TCL_GLOBAL_ONLY);

  modName = Tcl_GetVar2(tcl,aname,"name",TCL_GLOBAL_ONLY);
  if (!modName || !*modName || checkValidName(modName,0) != 0) {
    message(1,msgLookup("err.badid"),modName);
    return TCL_OK;
  }
  if (env_findModule(modName)) {
    message(1,msgLookup("err.moddef"),modName);
    return TCL_OK;
  }
  if (strncmp(modName,TkGate.circuit->c_gatePrefix,strlen(TkGate.circuit->c_gatePrefix)) == 0) {
    message(1,msgLookup("err.primprefix"),modName,TkGate.circuit->c_gatePrefix);
    return TCL_OK;
  }
  if (ycIsKW((char*)modName)) {
    message(1,msgLookup("err.iskeyword"),modName);
    return TCL_OK;
  }

  M = env_defineModule(modName,0);
  if (M) {
    const char *r;

    r = Tcl_GetVar2(tcl,aname,"ppos",TCL_GLOBAL_ONLY);
    M->m_pptype = (strcmp(r,"fix") == 0) ? PPT_FIXED : PPT_SCALED;

    r = Tcl_GetVar2(tcl,aname,"type",TCL_GLOBAL_ONLY);
    M->m_type = (strcmp(r,"netlist") == 0) ? MT_NETLIST : MT_TEXTHDL;

    r = Tcl_GetVar2(tcl,aname,"prot",TCL_GLOBAL_ONLY);
    M->m_protData = strbin(r);

    r = Tcl_GetVar2(tcl,aname,"protint",TCL_GLOBAL_ONLY);
    M->m_protIntf = strbin(r);

    r = Tcl_GetVar2(tcl,aname,"proted",TCL_GLOBAL_ONLY);
    M->m_protEdit = strbin(r);

    Tcl_SetVar2(tcl,aname,"ok","1",TCL_GLOBAL_ONLY);
  }

  if (M->m_type == MT_TEXTHDL) {
    char buf[STRMAX];

    sprintf(buf,"module %s;\n\nendmodule\n",M->m_name);
    GModuleDef_saveText(M,buf);
  }

  return TCL_OK;
}

static int gat_batInc(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (TkGate.batp) {
    SetBatCursor(es);
    Tcl_Eval(tcl,"after $tkg_batRate gat_batInc");
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 *  Set variables for the delays of the currently seleced gate in the specified tecnology
 *
 ******************************************************************************/
static int gat_setGateTechDelays(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GCElement *g = TkGate.circuit->select;
  GDelayDef *dd;
  GGateInfo *gi;
  int i;
  int delays[MAXDELAYS];

  if (!g) return TCL_OK;
  gi = g->typeinfo;

  dd = GDelayDef_findTech(gi->delay_defs,argv[1]);
  if (!dd) return TCL_OK;

  if (GDelayDef_getDelays(dd, g, delays) < 0)
    return TCL_OK;

  for (i = 0;i < gi->num_delays;i++) {
    DoTcl("set ::edgat_techdelay(%s) %d",gi->delayNames[i],delays[i]);
  }
  DoTcl("update");
  return TCL_OK; 
}

/*
 * gat_editProps
 *
 * Used for implementing the edit gate (or net) properties boxes.  This
 * should probably be rewritten at some time to use the same dialog box
 * model as module properties boxes.
 *
 */
static int gat_editProps(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  GCElement *g = TkGate.circuit->select;
  GGateInfo *gi;
  const char *temp;
  int ok;

  /*
   * If we are editing the interface, then switch to editing a specific module.
   */
  if ((*es)->isInterface) {
    if (g->typeinfo->Code == GC_BLOCK) {
      GWire *w = block_hitPort(g,TkGate.ed->tx,TkGate.ed->ty);
      if (w) {
	block_setPortName(g,w,*es);
	return TCL_OK;
      }
    }

    if (editstate_getInterfaceMode() == INTFMODE_ALL)
      modint_openSelected(0);
    return TCL_OK;
  }

  if ((!g || g->typeinfo->Code == GC_JOINT) && TkGate.circuit->nsel) {
    DoTcl("gat_editNet %s",TkGate.circuit->nsel->n_signame);
    return TCL_OK;
  }

  if (!g) {
    DoTcl("SideBar::editModuleOrNet");
    return TCL_OK;
  }
  gi = g->typeinfo;

  switch (gi->Code) {
  case GC_BLOCK :
    {
      GWire *w = block_hitPort(g,TkGate.ed->tx,TkGate.ed->ty);
      if (w) {
	block_setPortName(g,w,*es);
	return TCL_OK;
      }
    }
    break;
  case GC_JOINT :
    return TCL_OK;
  case GC_LOGICIN :
  case GC_LOGICTRI :
  case GC_LOGICOUT :
    net_editProps(g->wires[0]->net,TkGate.ed->tx,TkGate.ed->ty);
    return TCL_OK;
  }

  if (!gi->EditProps) {
    message(1,msgLookup("err.noprop"));	/* Selected gate has no editable properites. */
    return TCL_OK;
  }


  /*
   * Standard gate properties editing.
   */
#if 0
  printf("gat_editProps g=%p gi=%p [%s]\n",g,gi,gi->name);
#endif
  (*gi->EditProps)(g,1);
  DoTcl("EditGate::post");
  if ((temp = Tcl_GetVar(tcl,"edgat_ok",TCL_GLOBAL_ONLY)) && sscanf(temp,"%d",&ok) == 1 && ok) {
    if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
      message(0,msgLookup("err.protdata"));		/* "Can not edit library module." */
      return TCL_OK;
    }

    if (tkgate_currentMode() == MM_EDIT)
      (*gi->EditProps)(g,0);
    else
      message(1,msgLookup("err.noteditpchg"));
  }

  sel_refinish(*es);

  return TCL_OK;
}

/*****************************************************************************
 * gat_editCircProps <command> <array>
 *
 * Save or load properties about the circuit. "<command>" is either "load" or "save"
 *
 *
 *   file		File name
 *   filev		File version
 *   title		Circuit title
 *   discard		Discard changes on quiting (used for tutorial circuits)
 *   extbar		Extender bar style
 *   scripts		List of simulator scripts
 *
 *****************************************************************************/
static int gat_editCircProps(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  static char *tvModeNames[] = {"never","after","all"};
  char *aname = 0;
  Circuit *c = TkGate.circuit;

  if (argc != 3) {
    strcpy(tcl->result,"wrong number of arguments.");
    return TCL_ERROR;
  }

  /* Tcl/Tk 8.3 wants non-const parameters to SetVar */
  aname = (char*)argv[2];

  if (strcmp(argv[1],"load") == 0) {
    int n;
    char u[STRMAX];

    Tcl_SetVar2(tcl,aname,"file",CurrentFile_path(c->currentFile),TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"filev",c->fileVersion,TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"title",c->title,TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"encoding",c->c_encoding,TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"locale",c->c_locale->l_name,TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"discard",binstr(c->discardChanges),TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"extbar",binstr(c->useExtBars),TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"shownet",binstr(c->showSwitchNets),TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"autostart",binstr(c->simAutoStart),TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"clockMode",binstr(c->simClockMode),TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"clockName",c->simClockName,TCL_GLOBAL_ONLY);
    if (c->numInitScripts > 0) {
      char *scripts = Tcl_Merge(c->numInitScripts,(const char**)c->initScripts);
      Tcl_SetVar2(tcl,aname,"scripts",scripts,TCL_GLOBAL_ONLY);
      if (scripts)
	Tcl_Free((char*)scripts);
    } else
      Tcl_SetVar2(tcl,aname,"scripts","",TCL_GLOBAL_ONLY);

    Timescale_decode(c->c_timescale.ts_units, &n, u);
    Tcl_SetVar2(tcl,aname,"unitsU",u,TCL_GLOBAL_ONLY);
    sprintf(u,"%d",n);
    Tcl_SetVar2(tcl,aname,"unitsN",u,TCL_GLOBAL_ONLY);

    Timescale_decode(c->c_timescale.ts_precision, &n, u);
    Tcl_SetVar2(tcl,aname,"precisionU",u,TCL_GLOBAL_ONLY);
    sprintf(u,"%d",n);
    Tcl_SetVar2(tcl,aname,"precisionN",u,TCL_GLOBAL_ONLY);

    sprintf(u,"%lg",c->c_startup);
    Tcl_SetVar2(tcl,aname,"initMode",tvModeNames[c->c_tvMode],TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"initTime",u,TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"initUnits",Timescale_codeToUnits(c->c_startupUnits),TCL_GLOBAL_ONLY);
  } else if (strcmp(argv[1],"save") == 0) {
    const char *r;
    int numScripts;
    const char **scriptList;
    int n;
    Locale *locale;

    r = Tcl_GetVar2(tcl,aname,"ok",TCL_GLOBAL_ONLY);
    if (r && *r == '1') {
      r = Tcl_GetVar2(tcl,aname,"discard",TCL_GLOBAL_ONLY);
      c->discardChanges = strbin(r);
      r = Tcl_GetVar2(tcl,aname,"extbar",TCL_GLOBAL_ONLY);
      c->useExtBars = strbin(r);
      r = Tcl_GetVar2(tcl,aname,"shownet",TCL_GLOBAL_ONLY);
      c->showSwitchNets = strbin(r);
      r = Tcl_GetVar2(tcl,aname,"autostart",TCL_GLOBAL_ONLY);
      c->simAutoStart = strbin(r);
      r = Tcl_GetVar2(tcl,aname,"title",TCL_GLOBAL_ONLY);
      Circuit_setTitle(r);
      r = Tcl_GetVar2(tcl,aname,"encoding",TCL_GLOBAL_ONLY);
      Circuit_setFileEncoding(c,r);

      r = Tcl_GetVar2(tcl,aname,"locale",TCL_GLOBAL_ONLY);
      locale = (Locale*)SHash_find(TkGate.localeNameTable,r);
      if (locale && c->c_locale != locale) {
	Circuit_setLocale(c,locale);
	FlagRedraw();
      }

      r = Tcl_GetVar2(tcl,aname,"clockMode",TCL_GLOBAL_ONLY);
      c->simClockMode = strbin(r);
      r = Tcl_GetVar2(tcl,aname,"clockName",TCL_GLOBAL_ONLY);
      Circuit_setClockName(r);

      r = Tcl_GetVar2(tcl,aname,"scripts",TCL_GLOBAL_ONLY);
      if (Tcl_SplitList(tcl,r,&numScripts,&scriptList) == TCL_OK) {
	Circuit_setScripts(numScripts,scriptList);
	if (scriptList)
	  Tcl_Free((char*)scriptList);
      }

      r = Tcl_GetVar2(tcl,aname,"unitsN",TCL_GLOBAL_ONLY);
      sscanf(r,"%d",&n);
      r = Tcl_GetVar2(tcl,aname,"unitsU",TCL_GLOBAL_ONLY);
      c->c_timescale.ts_units = Timescale_parse(n,r);
      r = Tcl_GetVar2(tcl,aname,"precisionN",TCL_GLOBAL_ONLY);
      sscanf(r,"%d",&n);
      r = Tcl_GetVar2(tcl,aname,"precisionU",TCL_GLOBAL_ONLY);
      c->c_timescale.ts_precision = Timescale_parse(n,r);

      if (c->c_timescale.ts_precision > c->c_timescale.ts_units)
	c->c_timescale.ts_precision = c->c_timescale.ts_units;

      r = Tcl_GetVar2(tcl,aname,"initMode",TCL_GLOBAL_ONLY);
      for (n = 0;n < 3;n++)
	if (strcmp(r,tvModeNames[n]) == 0) {
	  c->c_tvMode = n;
	  break;
	}
      r = Tcl_GetVar2(tcl,aname,"initTime",TCL_GLOBAL_ONLY);
      sscanf(r,"%lf",&c->c_startup);
      r = Tcl_GetVar2(tcl,aname,"initUnits",TCL_GLOBAL_ONLY);
      c->c_startupUnits = Timescale_unitsToCode(r);
      if (c->c_startupUnits < 0) {
	c->c_startupUnits = UC_NANOSEC;
	logError(ERL_WARN,"bad unit code in gat_editCircProps.");
      }
    }
  } else {
    strcpy(tcl->result,"bad command.");
    return TCL_ERROR;
  }

 Tcl_SetVar2(tcl,aname,"ok","1",TCL_GLOBAL_ONLY);

  return TCL_OK;
}


/*****************************************************************************
 *
 * gat_editModProps <command> <array> <module>
 *
 * Save or load properties about a module. "<command>" is either "load" or "save"
 *
 *****************************************************************************/
static int gat_editModProps(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char *aname = 0;
  char *mname = 0;
  GModuleDef *M;

  if (argc != 4) {
    strcpy(tcl->result,"wrong number of arguments.");
    return TCL_ERROR;
  }

  /* Tcl/Tk 8.3 wants non-const parameters to SetVar */
  aname = (char*)argv[2];
  mname = (char*)argv[3];

  M = env_findModule(mname);
  if (!M) {
    Tcl_SetVar2(tcl,aname,"ok","0",TCL_GLOBAL_ONLY);
    return TCL_OK;
  }
  
  if (strcmp(argv[1],"load") == 0) {
    GModuleDef_getPropsToTcl(M,tcl,aname);
  } else if (strcmp(argv[1],"save") == 0) {
    GModuleDef_setPropsFromTcl(M,tcl,aname);
    SetModified(MF_MODULE);
    SynchronizeInterface();
  } else {
    strcpy(tcl->result,"bad command.");
    return TCL_ERROR;
  }

  Tcl_SetVar2(tcl,aname,"ok","1",TCL_GLOBAL_ONLY);

  return TCL_OK;
}

/*****************************************************************************
 *
 *   Update net properties
 *
 *****************************************************************************/
static int gat_changeNet(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  const char *oldName = argv[1];
  const char *newName = argv[2];
  const char *vtype = argv[6];
  int hide = 0;
  int nbits = 1;
  int io = 0;
  GNet *n;
  GWire *w;

  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));		/* "Can not edit library module." */
    return TCL_OK;
  }

  sscanf(argv[3],"%d",&hide);
  sscanf(argv[4],"%d",&nbits);
  sscanf(argv[5],"%d",&io);

  n = (GNet*) SHash_find(es->env->m_nets,oldName);

  if (!n) return TCL_OK;

  w = n->n_driver;
  net_setSize(w->net,nbits);
  net_rename(n,newName,!hide);
  GNet_setVType(n, vtype);

  if (w->net->n_ionet) {
    GGateInfo *gi = GGateInfo_codeLookup(io);
    if (gi && (gi->Code == GC_LOGICIN || gi->Code == GC_LOGICOUT || gi->Code == GC_LOGICTRI))
	gate_transmute(w->net->n_ionet,gi);
  }

  FlagRedraw();

  return TCL_OK;
}

static int gat_unselectGates(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  EditState_unselectAll(es);
  return TCL_OK;
}

/*****************************************************************************
 *
 * Repalace a module instance with the current interface for a module and
 * attempt to reconnect all ports.  If the -all switch is given, then all instance
 * of the selected module are updated.
 *
 * Usage:
 *     gat_updateInterface [name]
 *     gat_updateInterface -all
 *
 *****************************************************************************/
static int gat_updateInterface(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GCElement *g = TkGate.circuit->select;
  const char *modName = 0;
  GModuleDef *M = 0;


  if (argc == 1) {
    if (!g) return TCL_OK;
    if (GCElement_getType(g) != GC_BLOCK && GCElement_getType(g) != GC_SYMBLOCK) return TCL_OK;
    block_updateInterface(g, TkGate.circuit->es->env);

    return TCL_OK;
  }

  if (strcmp(argv[1],"-all") == 0) {
    if (DoTcl("tkg_getSelectedBlock") == TCL_OK && *tcl->result != 0)
      modName = tcl->result;
    else if (g && (GCElement_getType(g) == GC_BLOCK || GCElement_getType(g) == GC_SYMBLOCK))
      modName = g->u.block.moduleName;
    else
      return TCL_OK;
  } else {
    modName = TkGate.circuit->es->env->m_name;
  }

  M = env_findModule(modName);
  if (!M) return TCL_OK;

  DoTcl("yesno [format [m circ.reallyupdate] %s]",GModuleDef_getName(M));


  /*
   * Scan circuit for instance of module M and update their interfaces.
   */
  if (strcmp(tcl->result,"yes") == 0) {
    Circuit *C =  TkGate.circuit;
    HashElem *he; 
    for (he = Hash_first(C->moduleTable);he;he = Hash_next(C->moduleTable,he)) {
      GModuleDef *PM = (GModuleDef*)HashElem_obj(he);
      HashElem *ghe; 
      for (ghe = Hash_first(PM->m_gates);ghe;ghe = Hash_next(PM->m_gates,ghe)) {
	g = (GCElement*)HashElem_obj(ghe);
	if ((GCElement_getType(g) == GC_BLOCK || GCElement_getType(g) == GC_SYMBLOCK) && 
	    strcmp(g->u.block.moduleName,GModuleDef_getName(M)) == 0) {
	  block_updateInterface(g, PM);
	}
      }
    }
  }

  return TCL_OK;
}

static int gat_updateModuleTree(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  SetModified(MF_MODULE);
  SynchronizeInterface();
#if 0
  /* We now do this indirectly */
  env_updateMTCircuit();
#endif

  return TCL_OK;
}

static int gat_updateMenuState(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  sel_updateMenuState();
  return TCL_OK;
}




static int gat_setBlockDesc(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GCElement *g = TkGate.circuit->select;

  if (tkgate_currentMode() != MM_EDIT) {
    message(0,msgLookup("err.editonly"));
    return TCL_OK;
  }

  if (g && g->typeinfo->Code == GC_BLOCK) {
    GModuleDef *M = env_findAdd(g->u.block.moduleName,0);
    if (modint_setInterface(M,g) == 0)
      message(0,msgLookup("msg.setinterface"),g->u.block.moduleName);/* Set module interface for '%s'. */
  } else
    message(0,msgLookup("msg.needsel"));		/* Please select a module instance. */

  return TCL_OK;
}

/*****************************************************************************
 *
 * Begin interface edit mode
 *
 * If no argument are given, the current module is opened.  If a module name
 * is given then that interface is opened.
 *
 *****************************************************************************/
static int gat_editBlockDesc(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  GModuleDef *M = 0;

  if (argc > 1) {
    M = env_findAdd(argv[1],0);
  } else {
    M = (TkGate.circuit->root_mod == (*es)->env) ? 0 : (*es)->env;
  }

  if (tkgate_currentMode() == MM_EDIT && !TkGate.circuit->es->isInterface && hdl_checkSave(0) < 0) 
    return TCL_OK;

  ob_touch(TkGate.circuit);

  modint_edit(es,M);

  return TCL_OK;
}

static int gat_moveGate(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  int dx = 0,dy =0;

  assert(argc == 3);

  sscanf(argv[1],"%d",&dx);
  sscanf(argv[2],"%d",&dy);

  sel_draw(es);
  sel_move(es,dx,dy);
  sel_draw(es);

  return TCL_OK;
}

static int gat_editNet(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GNet *net;
  char buf[STRMAX],*p;

  if (tkgate_currentMode() != MM_EDIT) return TCL_OK;

  strcpy(buf,argv[1]);
  if ((p = strchr(buf,'['))) *p = 0;
  if ((p = strchr(buf,'@'))) *p = 0;

  net = GModuleDef_findNet(TkGate.circuit->es->env,buf);
  if (net && net->n_driver) 
    net_editProps(net,net->n_driver->nodes->x,net->n_driver->nodes->y);

  return TCL_OK;
}

/****************************************************************************
 *
 * Get the selected object, append it to the cut buffer and delete the object
 * in the edit window.  Only usable for HDL.
 *
 * Usage: gat_CutToBufAppend
 *
 *****************************************************************************/
static int gat_cutToBufAppend(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (es->isInterface) {
    sel_interfaceReset(es);
  } else {
    sel_killAppend(es);
    FlagRedraw();
  }

  return TCL_OK;
}

/****************************************************************************
 *
 * Get the selected object, copy it to the cut buffer and delete the object
 * in the edit window.
 *
 * Usage: gat_cutToBuf
 *
 *****************************************************************************/
static int gat_cutToBuf(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (es->isInterface) {
    sel_interfaceReset(es);
  } else {
    sel_kill(es);
    FlagRedraw();
  }

  return TCL_OK;
}

/****************************************************************************
 *
 * Get the selected object and copy it to the cut buffer
 *
 * Usage: gat_copyToBuf
 *
 *****************************************************************************/
static int gat_copyToBuf(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (!es->isInterface)
    sel_copy(es);

  return TCL_OK;
}

/****************************************************************************
 *
 * Yank the object(s) from the cut buffer and paste it into the current module.
 *
 * Usage: gat_yankFromBuf [dx] [dy]
 *
 *
 *****************************************************************************/
static int gat_yankFromBuf(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  GModuleDef *m = es->env;
  int dx,dy;

  if (!modifyOK(es,0)) return TCL_OK;

  if (hdl_isactive) {
    sel_hdlyank(es);
  } else {
    if (argc >= 3) {
      sscanf(argv[1],"%d",&dx);
      sscanf(argv[2],"%d",&dy);

      if (TkGate.circuit->cut_buffer) {
	mark_unpost();
	TkGate.ed->tx = TkGate.circuit->cut_buffer->cb_ctrx + dx;
	TkGate.ed->ty = TkGate.circuit->cut_buffer->cb_ctry + dy;
      }
    }
    sel_yank(es);
    FlagRedraw();
  }


  return TCL_OK;
}

/*****************************************************************************
 *
 * Usage: gat_selectAll
 *
 * Select all gates in the current module
 *
 *****************************************************************************/
static int gat_selectAll(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  mark_unpost();
  sel_clear(es,1);
  sel_selectAll(es);

  EditState_setRotation(MODE_MOVESEL);
  return TCL_OK;
}


/*****************************************************************************
 *
 * Usage: gat_selectNet [name]
 *
 * Select the specified net in the current module
 *
 *****************************************************************************/
static int gat_selectNet(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  GModuleDef *m = es->env;
  GNet *n;

  if (argc != 2) {
    return TCL_OK;
  }

  n = GModuleDef_findNet(m,argv[1]);
  if (n) {
    net_select(n,1);
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Usage: gat_rotate
 *
 * Rotates the gates in the selection.
 *
 *****************************************************************************/
static int gat_rotate(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  int rdir = 1;

  if (argc >= 2) {
    sscanf(argv[1],"%d",&rdir);
    if (rdir < 0)
      rdir = -1;
    else
      rdir = 1;
  }

  if (!modifyOK(es,0)) return TCL_OK;
  if (tkgate_currentMode() != MM_EDIT) return TCL_OK;
  if (es->isInterface) return TCL_OK;

  sel_refinish(es);
 
  sel_draw(es);
  sel_rotate(es,rdir);
  sel_draw(es);

  return TCL_OK;
}


/*
  Check to see if breakpoint expression is valid.

  The first argument specifies the breakpoint operation to perform.

  -load				Load breakpoints into tk listbox
  -clear			Clear all breakpoints
  -replace n bp			Replace a breakpoint
  -insert n bp			Insert a breakpoint
  -delete n			Delete a breakpoint

  The 'replace' and 'insert' commands will return a negative value on error.
  Otherwise they will return the id number of the breakpoint.
 */
static int gat_breakpoint(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  NHash *bpm = TkGate.circuit->c_breakpoints;
  int i,r;

  r = 0;

#if 0
  {
    for (i = 0;i < argc;i++)
      printf("%s ",argv[i]);
    printf("\n");
  }
#endif


  if (strcmp(argv[1],"-load") == 0) {
    /*
     * Load breakpoints from internal state
     */
    BrkPtTable_loadInterface(bpm);
    r = 0;
  } else if (strcmp(argv[1],"-clear") == 0) {
    /*
     * Clear all breakpoints from internal state
     */
    BrkPtTable_clearStop(bpm);
  } else if (strcmp(argv[1],"-replace") == 0) {
    /*
     * Clear all breakpoints from internal state
     */
    if (sscanf(argv[2],"%d",&i) != 1) return -1;
    BrkPtTable_delete(bpm,i);
    r = BrkPtTable_insert(bpm,i,argv[3]);
  } else if (strcmp(argv[1],"-insert") == 0) {
    /*
     * Insert a new breakpoint
     */
    if (sscanf(argv[2],"%d",&i) != 1) return -1;
    r = BrkPtTable_insert(bpm,i,argv[3]);
  } else if (strcmp(argv[1],"-delete") == 0) {
    /*
     * Delete a new breakpoint
     */
    if (sscanf(argv[2],"%d",&i) != 1) return -1;
    BrkPtTable_delete(bpm,i);
    r = 0;
  } else if (strcmp(argv[1],"-enable") == 0) {
    /*
     * Delete a new breakpoint
     */
    if (sscanf(argv[2],"%d",&i) != 1) return -1;
    BrkPtTable_enable(bpm,i);
    r = 0;
  } else if (strcmp(argv[1],"-disable") == 0) {
    /*
     * Delete a new breakpoint
     */
    if (sscanf(argv[2],"%d",&i) != 1) return -1;
    BrkPtTable_disable(bpm,i);
    r = 0;
  }

  sprintf(tcl->result,"%d",r);
  return TCL_OK;
}

static int gat_simSelected(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GCElement *g = TkGate.circuit->select;
  int i;
  int match = 0;

  if (!g) {
    *tcl->result = 0;
    return TCL_OK;
  }

  if (argc == 1) 
    match = 1;
  else {
    for (i = 1;i < argc;i++)
      if (g->typeinfo == GGateInfo_vlookup(argv[i])) {
	match = 1;
	break;
      }
  }

  if (!match) {
    *tcl->result = 0;
    return TCL_OK;
  }

  GSimModule_getFullPath(TkGate.circuit->es->smod,g,tcl->result);

  return TCL_OK;
}

static int gat_print(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int i;
  GPrintOpt PO;

  GPrintOpt_init(&PO);

  for (i = 1;i < argc;i+=2) {
    if (i+1 >= argc) break;
    if (strcmp(argv[i],"-printer") == 0)
      PO.po_cmd = argv[i+1];
    else if (strcmp(argv[i],"-file") == 0)
      PO.po_file = argv[i+1];
    else if (strcmp(argv[i],"-paper") == 0)
      PO.po_paper = argv[i+1];
    else if (strcmp(argv[i],"-orient") == 0)
      PO.po_orient = argv[i+1];
    else if (strcmp(argv[i],"-style") == 0)
      PO.po_style = argv[i+1];
    else if (strcmp(argv[i],"-select") == 0)
      PO.po_select = argv[i+1];
    else if (strcmp(argv[i],"-modlist") == 0)
      PO.po_modlist = argv[i+1];
    else if (strcmp(argv[i],"-index") == 0)
      PO.po_index = argv[i+1];
    else if (strcmp(argv[i],"-graph") == 0)
      PO.po_graph = argv[i+1];
    else if (strcmp(argv[i],"-epsf") == 0)
      PO.po_epsf = argv[i+1];
    else if (strcmp(argv[i],"-merge") == 0)
      PO.po_4up = argv[i+1];
    else if (strcmp(argv[i],"-duplex") == 0)
      PO.po_isDuplex = argv[i+1];
    else if (strcmp(argv[i],"-inclib") == 0)
      PO.po_incLib = argv[i+1];
  }

  GPrintOpt_print(&PO);

  return TCL_OK;
}

static int gat_cleanUp(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  gateCleanUp();
  return TCL_OK;
}


static int gat_tracePrint(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int i;
  GPrintOpt PO;

  GPrintOpt_init(&PO);

  for (i = 1;i < argc;i+=2) {
    if (i+1 >= argc) break;
    if (strcmp(argv[i],"-printer") == 0)
      PO.po_cmd = argv[i+1];
    else if (strcmp(argv[i],"-file") == 0)
      PO.po_file = argv[i+1];
    else if (strcmp(argv[i],"-paper") == 0)
      PO.po_paper = argv[i+1];
    else if (strcmp(argv[i],"-orient") == 0)
      PO.po_orient = argv[i+1];
    else if (strcmp(argv[i],"-epsf") == 0)
      PO.po_epsf = argv[i+1];
    else if (strcmp(argv[i],"-duplex") == 0)
      PO.po_isDuplex = argv[i+1];
    else if (strcmp(argv[i],"-start") == 0)
      PO.po_start = argv[i+1];
    else if (strcmp(argv[i],"-end") == 0)
      PO.po_end = argv[i+1];
    else if (strcmp(argv[i],"-scalelength") == 0)
      PO.po_scaleLength = argv[i+1];
  }

  GPrintOpt_tracePrint(&PO);

  return TCL_OK;
}

/*
 * Get trace range data.  Returns a tripple of the form {sel_start sel_end trace_end}.
 */
static int gat_getTraceData(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  extern GScope *Scope;
  simtime_t start;
  simtime_t stop;
  simtime_t first = 0;
  int i;
  int did_first_set = 0;
  double timeCorrection;

  if (!Scope) {
    sprintf(tcl->result,"0 0 0 0 ns 0");
    return TCL_OK;
  }

  for (i = 0;i < Scope->NumTraces;i++) {
    GValue *v;
    for (v = Scope->Traces[i]->t_first;v;v = v->v_next)
      if (v->v_code != VC_UNRECORDED)
	break;
    if (v && (v->v_time < first || !did_first_set)) {
      first = v->v_time;
      did_first_set = 1;
    }
  }


  if (Scope->mark_count == 2) {
    start = Scope->mark_val[0];
    stop = Scope->mark_val[1];
  } else {
    start = Scope->s_leftTime;
    stop = Scope->s_leftTime+Scope->s_range;
  }

  if (start < first) start = first;

  if (stop < start) { int x = start; start = stop; stop = x; }

  start = imax(start,0);
  stop = imin(stop,Scope->s_time);

  timeCorrection = TkGate.circuit->simulator.si_tsmult/(double)Scope->s_precision;

  sprintf(tcl->result,"%llu %llu %llu %lf %s %llu %llu",
	  start,
	  stop,
	  Scope->s_range,
	  timeCorrection,
	  SimInterface_unitsToStr(TkGate.circuit->simulator.si_units),
	  first,
	  Scope->s_time);

  return TCL_OK;
}

static int gat_traceLPPEst(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char paper[STRMAX];
  int i;
  const char *orient = "landscape";
  double lpp;

  for (i = 1;i < argc;i+=2) {
    if (i+1 >= argc) break;

    if (strcmp(argv[i],"-paper") == 0)
      sscanf(argv[i+1],"%s",paper);
    else if (strcmp(argv[i],"-orient") == 0)
      orient = argv[i+1];
  }

  lpp = traceLinesPerPage(orient, paper);
  sprintf(tcl->result,"%lf",lpp);

  return TCL_OK;
}


static int gat_setCircProp(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc == 1) return TCL_OK;

  ob_touch(TkGate.circuit);
  ob_touch(TkGate.circuit->initScripts);

  if (strcmp(argv[1],"-clear") == 0) {
    int i;

    for (i = 0;i < TkGate.circuit->numInitScripts;i++)
      ob_free(TkGate.circuit->initScripts[i]);
    if (TkGate.circuit->initScripts)
      ob_free(TkGate.circuit->initScripts);
    TkGate.circuit->numInitScripts = 0;
    TkGate.circuit->initScripts = 0;

  } else if (strcmp(argv[1],"-script") == 0 && argc > 2) {
    if (TkGate.circuit->numInitScripts) {
      TkGate.circuit->initScripts = (char**) ob_realloc(TkGate.circuit->initScripts,sizeof(char*)*(TkGate.circuit->numInitScripts+1));
    } else
      TkGate.circuit->initScripts = (char**) ob_malloc(sizeof(char*),"char*");
    TkGate.circuit->initScripts[TkGate.circuit->numInitScripts++] = ob_strdup(argv[2]);
  } else if (strcmp(argv[1],"-vlib") == 0 && argc > 2) {
  } else if (strcmp(argv[1],"-top") == 0 && argc > 2) {
    GModuleDef *M = env_findModule(argv[2]);
    if (M)
      TkGate.circuit->root_mod = M;
  }

  return TCL_OK;
}

static int gat_getCircProp(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  *tcl->result = 0;

  if (strcmp(argv[1],"-script") == 0 && argc > 2) {
    int i;

    if (sscanf(argv[2],"%d",&i) != 1) return TCL_OK;
    if (i >= TkGate.circuit->numInitScripts) return TCL_OK;
    strcpy(tcl->result,TkGate.circuit->initScripts[i]);
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Get list of available locales
 *
 * Usage: gat_getLocales
 *
 * Returns a list of the form: {{English en} {Japanese ja} ... }
 *
 *****************************************************************************/
static int gat_getLocales(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int i;
  HashElem *he; 

  Tcl_ResetResult(tcl);
  
  for (he = Hash_first(TkGate.localeTable), i = 0;he;he = Hash_next(TkGate.localeTable,he), i++) {
    char buf[STRMAX],buf2[STRMAX];
    Locale *l = (Locale*) HashElem_obj(he);

    sprintf(buf,"\"%s\" %s",quoteChars(buf2,l->l_name,TCL_SPECIALCHARS),l->l_code);
    Tcl_AppendElement(tcl,buf);
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Set the value of a dip switch in simulation mode.
 *
 *****************************************************************************/
static int gat_setDip(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GCElement *g;
  GSimSwitch *ss;
  GSimModule *M;

  if (argc != 3) return TCL_OK;

  if (SimInterface_lookupGate(&TkGate.circuit->simulator,argv[1],&M,&g,&ss) == 0) {

    if (M == TkGate.circuit->es->smod) gate_draw(g,0);

    ob_touch(g);
    if (sscanf(argv[2],"%x",&g->u.sw.dipval) != 1) {
      g->u.sw.dipval = 0;
    }

    g->u.sw.dipval &= nmask(g->wires[DIP_OUT]->net->n_nbits);

    if (M == TkGate.circuit->es->smod) gate_draw(g,0);

    DoTcl("tkg_simNetSet %s %d'h%x",
	  ss->name,
	  g->wires[0]->net->n_nbits,
	  g->u.sw.dipval);

    sprintf(tcl->result,"%x",g->u.sw.dipval);
  } else {
    sprintf(tcl->result,"0");
  }


  return TCL_OK;
}

/*****************************************************************************
 *
 * Tcl procedure to do a text search
 *
 * Tcl syntax:
 *
 *   gat_find cmd [options]
 *
 * Commands are one of:
 *   search		Search for the next occurence of the target in the current context
 *   list		List all matches
 *   goto		Goto a specific match
 *
 *****************************************************************************/
static int gat_find(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int mode = 0;
  int qualifier = 0;
  const char *target = 0;
  const char *command = 0;
  const char *variable = 0;
  int i;

  if (argc < 2) return TCL_ERROR;

  command = argv[1];

  for (i = 2;i < argc;i++)
    if (strcmp(argv[i],"-type") == 0 && i+1 < argc)
      sscanf(argv[++i],"%d",&mode);
    else if (strcmp(argv[i],"-qualifier") == 0 && i+1 < argc)
      sscanf(argv[++i],"%d",&qualifier);
    else if (strcmp(argv[i],"-target") == 0 && i+1 < argc)
      target = argv[++i];
    else if (strcmp(argv[i],"-variable") == 0 && i+1 < argc)
      variable = argv[++i];

  if (strcmp(command,"search") == 0) {
    if (!target) return TCL_OK;
    GSearchContext_find(TkGate.circuit->search,target,mode,qualifier);
  } else if (strcmp(command,"list") == 0) {
    if (!target || !variable) return TCL_OK;
    GSearchContext_list(TkGate.circuit->search,target,mode,qualifier,variable);
  } else if (strcmp(command,"goto") == 0) {
    if (argc > 2)
      GSearchContext_goto(argv[2]);
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Format a time in epochs to use units (simulator must be active to use this)
 *
 * Tcl syntax:
 *
 *   gat_formatTime <time>
 *
 *****************************************************************************/
static int gat_formatTime(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  simtime_t t;
  SimInterface *si = &TkGate.circuit->simulator;

  if (argc != 2) return TCL_ERROR;
  if (sscanf(argv[1],"%llu",&t) != 1) return TCL_ERROR;

  if (tkgate_currentMode() == MM_SIMULATE)
    SimInterface_formatTime(si, tcl->result, t);
  else
    sprintf(tcl->result,"%llu",t);

  return TCL_OK;
}


static int gat_anchor(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  int mode = 0;
  int old_mode;

  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));
    return TCL_OK;
  }

  if (TkGate.circuit->select)
    old_mode = TkGate.circuit->select->anchored;
  else if (TkGate.circuit->mg_selection)
    old_mode = TkGate.circuit->mg_selection->s_hasAnchored;
  else
    old_mode = 0;

  sprintf(tcl->result,"%d",old_mode);

  if (argc > 1) {
    sscanf(argv[1],"%d",&mode);
    sel_anchor(es,mode);
  }

  return TCL_OK;
}

static int gat_setcolors(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  setGCcolors();
  FlagRedraw();
  if (tkgate_currentMode() == MM_SIMULATE)
    ReqScopeRedisplay();

  return TCL_OK;
}

static int gat_scopeMotion(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int x = 0,y = 0,state = 0,n=1;
  int px =0,py = 0;

  if (argc < 7) return TCL_OK;

  sscanf(argv[1],"%d",&x);
  sscanf(argv[2],"%d",&y);
  sscanf(argv[3],"%d",&px);
  sscanf(argv[4],"%d",&py);
  sscanf(argv[5],"%d",&n);
  sscanf(argv[6],"%d",&state);

  scopeButtonMotion(x,y,px,py,n,state);

  return TCL_OK;
}

static int gat_scopeButton(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int x = 0,y = 0,state = 0,n=1;
  int px =0,py = 0;

  if (argc < 7) return TCL_OK;

  sscanf(argv[1],"%d",&x);
  sscanf(argv[2],"%d",&y);
  sscanf(argv[3],"%d",&px);
  sscanf(argv[4],"%d",&py);
  sscanf(argv[5],"%d",&n);
  sscanf(argv[6],"%d",&state);

  scopeButtonPress(x,y,px,py,n,state);

  return TCL_OK;
}

static int gat_scopeRelease(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int x = 0,y = 0,state = 0,n=1;
  int px =0,py = 0;

  if (argc < 7) return TCL_OK;

  sscanf(argv[1],"%d",&x);
  sscanf(argv[2],"%d",&y);
  sscanf(argv[3],"%d",&px);
  sscanf(argv[4],"%d",&py);
  sscanf(argv[5],"%d",&n);
  sscanf(argv[6],"%d",&state);

  scopeButtonRelease(x,y,px,py,n,state);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Tcl procedure to change the zoom level.
 *
 * Tcl syntax:
 *
 *   gat_zoom <zoom-change>
 *
 *****************************************************************************/
static int gat_zoom(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int zoom = 0;
  int old_zf;
  int org_x, org_y;

  if (argc < 2) return TCL_OK;

  sscanf(argv[1],"%d",&zoom);

  old_zf = TkGate.circuit->zoom_factor;
  zoom = TkGate.circuit->zoom_factor + zoom;		/* The new zoom factor */

  if (zoom < 1 || zoom > ZOOM_MAX || zoom == TkGate.circuit->zoom_factor)
    return TCL_OK;

  ob_touch(TkGate.circuit);
  TkGate.circuit->zoom_factor = zoom; 
  if (zoom == 1) {
    DoTcl("ToolBar::toolConfigure Zoom zoomout -state disabled");
    DoTcl("ToolBar::toolConfigure Zoom zoomin -state normal");
  } else if (zoom == ZOOM_MAX) {
    DoTcl("ToolBar::toolConfigure Zoom zoomout -state normal");
    DoTcl("ToolBar::toolConfigure Zoom zoomin -state disabled");
  } else {
    DoTcl("ToolBar::toolConfigure Zoom zoomout -state normal");
    DoTcl("ToolBar::toolConfigure Zoom zoomin -state normal");
  }

  org_x = TkGate.circuit->org_x - (TkGate.width/old_zf/2 - TkGate.width/TkGate.circuit->zoom_factor/2);
  org_y = TkGate.circuit->org_y - (TkGate.height/old_zf/2 - TkGate.height/TkGate.circuit->zoom_factor/2);

#if 0
  printf("@zoom %dx%d  old=(%d,%d) new=(%d,%d)\n",TkGate.width,TkGate.height,TkGate.circuit->org_x,TkGate.circuit->org_y,org_x,org_y);
#endif

  TkGate_setOrigin(org_x,org_y);


  if (TkGate.circuit->zoom_factor > 1) {
    XSetLineAttributes(TkGate.D,TkGate.moduleGC,TkGate.circuit->zoom_factor,LineSolid,CapButt,JoinMiter);
    XSetLineAttributes(TkGate.D,TkGate.instGC,TkGate.circuit->zoom_factor,LineSolid,CapButt,JoinMiter);
  } else {
    XSetLineAttributes(TkGate.D,TkGate.moduleGC,0,LineSolid,CapButt,JoinMiter);
    XSetLineAttributes(TkGate.D,TkGate.instGC,0,LineSolid,CapButt,JoinMiter);
  }

  XSetFont(TkGate.D, TkGate.instGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.moduleGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.modportGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.frameGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.commentGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.hyperlinkGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.wireGC,		TkGate.stextXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.busGC,		TkGate.stextXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.selWireGC,		TkGate.stextbXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.selBusGC,		TkGate.stextbXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.toolGC,		TkGate.textXF[zoom]->fid);
  XSetFont(TkGate.D, TkGate.cpathGC,		TkGate.textXF[zoom]->fid);
  if (TkGate.japaneseMode) {
    XSetFont(TkGate.D, TkGate.kanjiGC,		TkGate.ktextXF[zoom]->fid);
  }

  FlagRedraw();

  return TCL_OK;
}

static int gat_align(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  int x = 0;

  if (argc < 2) return TCL_OK;

  sscanf(argv[1],"%d",&x);

  if (x)
    sel_alignHorz(es);
  else
    sel_alignVert(es);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Set location for a popup menu
 *
 * Tcl Syntax:
 *
 *    get_setpop x y
 *
 * Get ready to open a popup menu in the main edit window and return the type of
 * popup we should use based on the context.
 *
 *****************************************************************************/
static int gat_setpop(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int x,y,cx,cy;
  GCElement *g = TkGate.circuit->select;

  x = y = 0;
  if (argc > 1) sscanf(argv[1],"%d",&x);
  if (argc > 2) sscanf(argv[2],"%d",&y);

  TkGate.popstate.g = g;
  TkGate.popstate.net = TkGate.circuit->nsel;
  TkGate.popstate.w = TkGate.circuit->wsel;
  TkGate.popstate.n = TkGate.circuit->wnsel;
  TkGate.popstate.wx = wtoc_x(x);
  TkGate.popstate.wy = wtoc_y(y);
  TkGate.popstate.x = wtoc_x(x/TkGate.circuit->zoom_factor);
  TkGate.popstate.y = wtoc_y(y/TkGate.circuit->zoom_factor);


  cx = TkGate.popstate.x;
  cy = TkGate.popstate.y;

  /*
   * If we are in simulate mode, use one of the simulate popups
   */
  if (tkgate_currentMode() == MM_SIMULATE) {
    if (g && GCElement_isModule(g))
      strcpy(tcl->result,"blksimu");
    else if (TkGate.popstate.n)
      strcpy(tcl->result,"wiresimu");
    else
      strcpy(tcl->result,"simu");
    return TCL_OK;
  }

  /*
   * If we are in analysis mode, use one of the analysis popups
   */
  if (tkgate_currentMode() == MM_ANALYZE) {
    if (g && GCElement_isModule(g))
      strcpy(tcl->result,"blkanal");
    else
      strcpy(tcl->result,"anal");
    return TCL_OK;
  }

  /*
   * If we are on the edit interfaces screen, use an edit interfaces popup
   */
  if (TkGate.circuit->es->isInterface) {
    if (g && GCElement_getType(g) == GC_BLOCK) {
      if (block_hitPort(g,cx,cy))
	strcpy(tcl->result,"intfblockport");
      else if (block_edgehit(g,x,y))
	strcpy(tcl->result,"intfblockedge");
      else
	strcpy(tcl->result,"intfblock");
    } else if (g && GCElement_getType(g) == GC_SYMBLOCK) {
	strcpy(tcl->result,"intfblock");
    } else
      strcpy(tcl->result,"intf");

    return TCL_OK;
  }
    

  if (EditState_getMode() != MODE_MOVE && EditState_getMode() != MODE_MOVESEL) {
    strcpy(tcl->result,"notmove");
    return TCL_OK;
  }

  if (TkGate.circuit->mg_selection && !g)
    strcpy(tcl->result,"multi");
  else if (g) {
    int N = GCElement_numPads(g);
    int can_add = 0;
    int i;

    for (i = 0;i < N;i++)
      if (GCElement_getPadCanAdd(g,i))
	can_add = 1;

    if (g->typeinfo->Code == GC_BLOCK) {
      if (block_hitPort(g,cx,cy))
	strcpy(tcl->result,"blockport");
      else if (block_edgehit(g,cx,cy))
	strcpy(tcl->result,"blockedge");
      else
	strcpy(tcl->result,"block");
    } else if (g->typeinfo->Code == GC_SYMBLOCK) {
      strcpy(tcl->result,"block");
    } else if (g->typeinfo->Code == GC_JOINT) {
      if (!g->wires[0] || !g->wires[1] || !g->wires[2] || !g->wires[3])
	strcpy(tcl->result,"joint3");
      else
	strcpy(tcl->result,"joint4");
    } else if (can_add)
      strcpy(tcl->result,"MIgate");
    else
      strcpy(tcl->result,"gate");
  } else if (TkGate.popstate.n) {
    char *labelCode = TkGate.popstate.n->isLabeled ? "L" : "";
    char *sizeCode = TkGate.popstate.n->showSize ? "S" : "";

    if (TkGate.popstate.net && TkGate.popstate.net->n_nbits > 1)
      sprintf(tcl->result,"mbwire%s%s",labelCode,sizeCode);
    else
      sprintf(tcl->result,"wire%s",labelCode);
  }
  else
    strcpy(tcl->result,"canv");

  /*     ob_touch(TkGate.popstate); (popstate may need to be made undoable?) */

  return TCL_OK;
}

/*****************************************************************************
 *
 * Tcl procedure to set the tkgate major mode.
 *
 * Parameters:
 *    _d		Client data (unused)
 *    tcl		Tcl interpreter
 *    argc		Number of arguments
 *    argv		Vector of arguments
 *
 * Returns:		Tcl status
 *
 * Tcl forms:
 *
 *   gat_setMajorMode <mode>
 *
 *****************************************************************************/
static int gat_setMajorMode(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int target_mode;
  int has_plus = 0;

  if (argc < 2) {
    strcpy(tcl->result,"bad mode ");
    return TCL_ERROR;
  }

  if (!TkGate.circuit || !TkGate.circuit->es) return TCL_OK;

  if (strcmp(argv[1],"simulate") == 0) {
    target_mode = MM_SIMULATE;
  } else if (strcmp(argv[1],"+edit") == 0) {
    target_mode = MM_EDIT;
    has_plus = 1;
  } else if (strcmp(argv[1],"edit") == 0) {
    target_mode = MM_EDIT;
  } else if (strcmp(argv[1],"analyze") == 0) {
    target_mode = MM_ANALYZE;
  }  else {
    strcpy(tcl->result,"bad mode ");
    return TCL_ERROR;
  }


  /*
   * If we are in normal edit mode, do an HDL save with checks before changing modes.
   */
#if 0
  if (tkgate_currentMode() == MM_EDIT && target_mode != MM_EDIT 
      && !TkGate.circuit->es->isInterface && hdl_checkSave(0) < 0) 
    return TCL_OK;
#endif

  tkgate_setMajorMode(target_mode);
  if (has_plus && TkGate.circuit->es->isInterface) {
    modint_close();
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Tcl procedure to get the tkgate major mode.
 *
 * Parameters:
 *    _d		Client data (unused)
 *    tcl		Tcl interpreter
 *    argc		Number of arguments
 *    argv		Vector of arguments
 *
 * Returns:		Tcl status
 *
 * Tcl forms:
 *
 *   gat_getMajorMode
 *
 * Tcl Return:		name of the major mode
 *
 *****************************************************************************/
static int gat_getMajorMode(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc != 1) {
    strcpy(tcl->result,"illegal argument.");
    return TCL_ERROR;
  }

  if (!TkGate.circuit || !TkGate.circuit->es) return TCL_OK;

  switch (tkgate_currentMode()) {
    case MM_SIMULATE :
      strcpy(tcl->result,"simulate");
      break;
    case MM_EDIT :
      if (editstate_isInterfaceMode())
	strcpy(tcl->result,"interface");
      else
	strcpy(tcl->result,"edit");
      break;
    case MM_ANALYZE :
      strcpy(tcl->result,"analyze");
      break;
  }
  return TCL_OK;
}

static int gat_gotoCPathNet(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  const char *p;
  GNet *n;
  int x,y;
  GWireNode *wn1,*wn2;

  if (argc < 2) {
    strcpy(tcl->result,"bad cpath");
    return TCL_ERROR;
  }

  Error_navigateToModule(&TkGate.circuit->es,argv[1]);
  editstate_setCurrent(TkGate.circuit->es);
  FlagRedraw();

  p = strrchr (argv[1],'.');
  if (p)
    p++;
  else
    p = argv[1];

  n = GModuleDef_findNet(TkGate.circuit->es->env,p);
  if (!n) {
    sprintf(tcl->result,"net '%s' not found.",p);
    return TCL_ERROR;
  }

  wn1 = n->n_driver->nodes;
  wn2 = wn1->out ? wn1->out : wn1->in;
  x = (wn1->x + wn2->x)/2;
  y = (wn1->y + wn2->y)/2;

  TkGate_setOrigin(TkGate.width/2 - x,TkGate.height/2 - y);
  ob_touch(TkGate.circuit->es);
  editstate_saveOrig(TkGate.circuit->es);
  SetErrorPosition(x,y);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Handle commands changeing properties of a port on a block edge, or add a
 * port to a block edge.
 *
 * Usage: gat_popupPortCmd command [arg]
 *
 * Command List:
 *     add-in		Add an input port
 *     add-out		Add an output port
 *     add-inout	Add an inout port
 *     change-in	Change port type to input
 *     change-out	Change port type to output
 *     change-inout	Change port type to inout
 *     delete		Delete a port
 *     size n		Set bit size
 *
 *****************************************************************************/
static int gat_popupPortCmd(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;
  GWire *w = 0;

  if (argc < 2) {
    strcpy(tcl->result,"bad popup");
    return TCL_ERROR;
  }

#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->tx = TkGate.popstate.x;
  TkGate.ed->ty = TkGate.popstate.y;

  /*
   * These commands are not applied to an existing port
   */
  if (strncmp(argv[1],"add-",4) == 0) {
    int iotype = strIOType(argv[1]+4);
    if (iotype >= 0)
      block_newport(es,iotype);
    return TCL_OK;
  }

  /*
   * All other command must be applied to a port.  Go try to find it.
   */
  if (TkGate.circuit->select && TkGate.circuit->select->typeinfo->Code == GC_BLOCK)
    w = block_hitPort(TkGate.circuit->select,TkGate.ed->tx,TkGate.ed->ty);

  /*
   * Do nothing if we could not find a port (popup menu context testing should
   * prevent this from happening.
   */
  if (!w)
    return TCL_OK;


  if (strcmp(argv[1],"change-in") == 0) {
    block_setdir(w->nodes,es,IN);
  } else if (strcmp(argv[1],"change-out") == 0) {
    block_setdir(w->nodes,es,OUT);
  } else if (strcmp(argv[1],"change-inout") == 0) {
    block_setdir(w->nodes,es,TRI);
  } else if (strcmp(argv[1],"change") == 0) {
    block_changedir(w->nodes,es);
  } else if (strcmp(argv[1],"size") == 0) {
    int size;
    if (argc < 3 || sscanf(argv[2],"%d",&size) != 1) {
      strcpy(tcl->result,"bad port size");
      return TCL_ERROR;
    }
    GNet_draw(w->net);
    net_setSize(w->net,size);
    GNet_draw(w->net);
    SetModified(MF_INTERFACE);
    SynchronizeInterface();
  } else if (strcmp(argv[1],"delete") == 0) {
    if (w->gate && (w->gate->typeinfo->Code == GC_BLOCK)) {
      SetModified(MF_NET);
      net_unselect(1);
      wire_cut(w->nodes->x,w->nodes->y,w->nodes, es->env);
      ob_touch(TkGate.circuit);
      TkGate.circuit->wsel = NULL;
      TkGate.circuit->wnsel = NULL;
    }
    SetModified(MF_INTERFACE);
    SynchronizeInterface();
  } else {
    strcpy(tcl->result,"illegal port command.");
    return TCL_ERROR;
  }

  return TCL_OK;
}

static int gat_popupSetState(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc > 1)
    sscanf(argv[1],"%d",&TkGate.popstate.isSet);
  else
    sprintf(tcl->result,"%d",TkGate.popstate.isSet);

  return TCL_OK;
}

static int gat_popupWireProps(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GNet *net;

  net = TkGate.popstate.w->net;
  if (net) {
#if TOUCH_XGATE_ED
    ob_touch(TkGate.ed);
#endif
    TkGate.ed->tx = TkGate.popstate.wx;
    TkGate.ed->ty = TkGate.popstate.wy;
    net_editProps(net,TkGate.ed->tx,TkGate.ed->ty);
  }

  return TCL_OK;
}

static int gat_popupWireAddStub(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (TkGate.popstate.g) {
    if (TkGate.popstate.g->typeinfo->Code == GC_JOINT)
      joint_addstub(TkGate.popstate.g,es);
  } else
  wire_addstub(es,TkGate.popstate.x,TkGate.popstate.y);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Hide or show the bit size on a wire segment
 *
 *****************************************************************************/
static int gat_popupWireSize(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (TkGate.popstate.n && TkGate.popstate.w) {
    GWireNode *n = TkGate.popstate.n;
    GWire *w = TkGate.popstate.w;

    GWire_draw(w);
    ob_touch(n);
    n->showSize = !n->showSize;
    GWire_draw(w);
  }


  return TCL_OK;
}

/*****************************************************************************
 *
 * Hide or show the label on a wire segment
 *
 * Usage:
 *     gat_popupWireLabel 			Toggle label
 *     gat_popupWireLabel -show			Show label
 *     gat_popupWireLabel -hide			Hide This Label
 *     gat_popupWireLabel -hideall		Hide All Labels
 *     gat_popupWireLabel -clearall		Remove all labels
 *
 *****************************************************************************/
static int gat_popupWireLabel(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  GWireNode *n = TkGate.popstate.n;
  GWire *w = TkGate.popstate.w;
  GNet *net = TkGate.popstate.net;
  int y = TkGate.ed->ty;
  int x = TkGate.ed->tx;
  int isLabeled;

  if (!net || !n || !w) 
    return TCL_OK;

  isLabeled = n->isLabeled;

  if (argc > 1) {
    if (strcmp(argv[1],"-hideall") == 0) {
      GNet_draw(net);
      GNet_hideName(net);
      GNet_draw(net);
      return TCL_OK;
    } else if (strcmp(argv[1],"-clearall") == 0) {
      GWire *wlist[GNet_numWires(net)];
      int count = GNet_getWires(net,wlist,GW_DRIVER);
      int i;

      GNet_draw(net);
      GNet_hideName(net);
      for (i = 0;i < count;i++) {
	for (n = wlist[i]->nodes;n;n = n->out) {
	  if (n->isLabeled) {
	    ob_touch(n);
	    n->isLabeled = 0;
	  }
	}
      }
      GNet_draw(net);
      return TCL_OK; 
    } else if (strcmp(argv[1],"-show") == 0)
      isLabeled = 1;
    else if (strcmp(argv[1],"-hide") == 0)
      isLabeled = 0;
    else
      isLabeled = !isLabeled;
  }

  GNet_draw(net);
  ob_touch(n);

  n->isLabeled = isLabeled;
  if (n->isLabeled && n->out) {
    int offset = 50;

    if (n->y != n->out->y) {
      offset = 100*(y - n->y)/(n->out->y - n->y);
    } else if (n->x != n->out->x) {
      offset = 100*(x - n->x)/(n->out->x - n->x);
    }
    n->offset = offset;

    GNet_showName(net);
  }
  GNet_draw(net);

  return TCL_OK;
}


static int gat_dumpWires(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  wire_dump(es);

  return TCL_OK;
}

static int gat_getTechList(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char **techs = GDelayDef_getTechList();
  char *s,*p;
  int l,i;

  if (!techs) {
    strcpy(TkGate.tcl->result,TKGATE_DEFAULT_TECH);
    return TCL_OK;
  }

  l = 0;
  for (i = 0;techs[i];i++)
    l += strlen(techs[i])+1;
  l++;

  p = s = (char*)ob_malloc(l,"char*");
  for (i = 0;techs[i];i++)
    p += sprintf(p," %s",techs[i]);

  TkGate.tcl->result = s;

  return TCL_OK;
}

/*
  Set the technology for all selected gates.
 */
static int gat_setTech(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState *es = gw->parms->circuit->es;

  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));		/* "Can not edit library module." */
    return TCL_OK;
  }
  
  if (argc > 1) {
    ob_begin_frame("SetTech");
    sel_setTech(es,argv[1]);
    ob_end_frame();
  }

  return TCL_OK;
}

/*
  Get the technology of the selected gate
 */
static int gat_getTech(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char *tech = 0;

  if (TkGate.circuit->select) {
    tech = TkGate.circuit->select->tech;
  } else if (TkGate.circuit->mg_selection) {
    HashElem *E;
    char *s_tech = 0;

    for (E = Hash_first(TkGate.circuit->mg_selection->s_gates);E;E = Hash_next(TkGate.circuit->mg_selection->s_gates,E)) {
      GCElement *g = (GCElement*) HashElem_obj(E);
      s_tech = g->tech ? g->tech : "";
      if (!tech)
	tech = s_tech;
      else {
	if (strcmp(tech,s_tech) != 0) {
	  tech = "";
	  break;
	}
      }
    }
  }

  if (!tech) tech = "";
  strcpy(tcl->result,tech);

  return TCL_OK;
}

static int gat_makeMakeMenu(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc != 2) {
    strcpy(tcl->result,"bad make menu");
    return TCL_ERROR;
  }

  makeMakeMenu(argv[1]);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Validate the suggested name of a port. 
 *
 * gat_validatePortName module name orient dir nbits
 * gat_validatePortName module name orient dir nbits
 *
 *
 *****************************************************************************/
static int gat_validatePortName(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  TkgGateWin *gw = TkGate.gw;
  EditState **es = &gw->parms->circuit->es;
  GModuleDef *m = 0;
  const char *modName = 0;
  const char *oldPortName = 0;
  const char *newPortName = 0;
  char validName[STRMAX];
  SHash *curPorts = 0;
  int orient = 0;
  int dir = 0;
  int nbits = 0;
  int i;
  const char *s;



  if (argc < 3) {
    sprintf(tcl->result,"wrong number of parameters.");
    return TCL_ERROR;
  }

  modName = argv[1];
  newPortName = argv[2];
  oldPortName = seekOption("-old",argv+3,argc-3);

  if ((s = seekOption("-dir",argv+3,argc-3))) {
    dir = strIOType(s);
  }
  
  if ((s = seekOption("-bits",argv+3,argc-3)))
    sscanf(s,"%d",&nbits);

  if ((s = seekOption("-orient",argv+3,argc-3))) {
    orient = igen_strToSide(s);
  }

  if (*modName)
    m = env_findModule(modName);

  if (m && m->m_interface) {
    GCElement *g = m->m_interface;

    if (g) {
      if (GCElement_getType(g) == GC_BLOCK) {
	int N = GCElement_numPads(g);
	GWire *w;
	int i;

	curPorts = new_SHash_noob();
	for (i = 0;i < N;i++) {
	  for (w = g->wires[i];w;w = w->next) {
	    if (!oldPortName || strcmp(w->name,oldPortName) != 0) {
	      SHash_insert(curPorts,w->name,w);
	    }
	  }
	}
      } else if (GCElement_getType(g) == GC_SYMBLOCK) {
	int N = GCElement_numPads(g);
	GWire *w;
	int i;

	curPorts = new_SHash_noob();
	for (i = 0;i < N;i++) {
	  for (w = g->wires[i];w;w = w->next) {
	    if (!oldPortName || strcmp(w->name,oldPortName) != 0) {
	      SHash_insert(curPorts,w->name,w);
	    }
	  }
	}
      }
      if (strcmp(newPortName,DEFAULT_PORT_NAME) == 0 || !*newPortName) {
	guessPortName(validName,g,orient,dir,nbits);
	newPortName = validName;
      }
    }
  }

  pickValidName(validName,newPortName,"P",curPorts);

  strcpy(tcl->result,validName);

  if (curPorts)
    delete_SHash(curPorts);

  return TCL_OK;
}


static int gat_makeMakeShortcuts(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  makeMakeShortcuts();
  return TCL_OK;
}

static int gat_showPath(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  cpath_show(argc-1,argv+1);
  FlagRedraw();
  return TCL_OK;
}

static int gat_clearPath(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  cpath_clear();
  cpath_flush();
  FlagRedraw();
  return TCL_OK;
}

static int gat_cpathFlash(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  cpath_flash();
  return TCL_OK;
}

static int gat_refreshScreen(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  FlagRedraw();
  return TCL_OK;
}


/*
 * Verify that internal data structures are correct.
 */
static int gat_verify(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  verify_circuit();
  return TCL_OK;
}

/*
 * Toggle the existence of a probe on the selected net.
 */
static int gat_toggleProbe(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char buf[STRMAX];
  char *netName = 0;
  SimInterface *si = &TkGate.circuit->simulator;
  GSimModule *SM;
  GWire *w;
  GNet *net;
  int x,y;

  if (tkgate_currentMode() != MM_SIMULATE)
    return TCL_OK;

  if (argc > 1) {
    GSimModule_getPathPrefix(TkGate.circuit->es->smod,buf);
    strcat(buf,argv[1]);
    netName = buf;
  } else if (TkGate.circuit->nsel) {
    GSimModule_getNetPathName(TkGate.circuit->es->smod,TkGate.circuit->nsel,buf);
    netName = buf;
  }

  if (!netName) {
    return TCL_OK;
  }

  if (SimInterface_lookupWire(si,netName,&SM,&w,&net) == 0) {
    if (w) {
      x = w->nodes->x;
      y = w->nodes->y;
      if (TkGate.popstate.isSet) { 
	x = TkGate.popstate.x;
	y = TkGate.popstate.y;
	GWire_pickProbePosition(TkGate.popstate.w,&x,&y);
      }
      SimInterface_addDelProbe(si,TkGate.circuit->es->smod,buf,w,w->nodes,x,y);
    } else {
      SimInterface_addDelHDLProbe(si,TkGate.circuit->es->smod,buf,net);
    }
  } else
    message(0,msgLookup("err.sim.nonet"),netName);

  return TCL_OK;
}

static int gat_delScopeProbe(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  extern GScope *Scope;
  GScope_deleteSelectedTrace(Scope);
  return TCL_OK;
}

/*
 * Undo the last edit
 */
static int gat_undo(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int n = 1;

  if (argc > 1)
    sscanf(argv[1],"%d",&n);

  ob_undo(n);
  FlagRedraw();
  return TCL_OK;
}

/*
 * Redo the last undone edit
 */
static int gat_redo(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int n = 1;

  if (argc > 1)
    sscanf(argv[1],"%d",&n);

  ob_redo(n);
  FlagRedraw();
  return TCL_OK;
}

static int gat_obBeginFrame(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  unsigned flags = 0;

  if (ob_get_mode() == OM_DISABLED) {
    sprintf(tcl->result,"0");
    return TCL_OK;
  }

  if (argc < 2) {
    abort();
    return TCL_OK;
  }

  if (argc > 2)
    sscanf(argv[2],"%u",&flags);

  if (strcmp(argv[1],"-") == 0)
    ob_begin_framef(0,flags);
  else
    ob_begin_framef(argv[1],flags);

  sprintf(tcl->result,"1");

  return TCL_OK;
}

static int gat_obAppendFrame(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (argc != 2) return TCL_OK;
  if (strcmp(argv[1],"-") == 0)
    ob_append_frame(0);
  else
    ob_append_frame(argv[1]);
  return TCL_OK;
}

static int gat_obEndFrame(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  ob_end_frame();
  return TCL_OK;
}

static int gat_getUndoList(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  const char *undoList[1024];
  char *p;
  int N,i;
  int nc = 0;

  N = ob_getUndoList(undoList,1024);


  nc = 10;
  for (i = 0;i < N;i++)
    nc += strlen(undoList[i])+1;

  tcl->result = malloc(nc);	/* Really use malloc/free here */
  tcl->freeProc = (Tcl_FreeProc*)free;

  p = tcl->result;
  *p = 0;
  for (i = 0;i < N;i++)
    p += sprintf(p," %s",undoList[i]);


  return TCL_OK;
}

static int gat_getRedoList(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  const char *redoList[1024];
  char *p;
  int N,i;
  int nc = 0;

  N = ob_getRedoList(redoList,1024);


  nc = 10;
  for (i = 0;i < N;i++)
    nc += strlen(redoList[i])+1;

  tcl->result = malloc(nc);	/* Really use malloc/free here */
  tcl->freeProc = (Tcl_FreeProc*)free;

  p = tcl->result;
  *p = 0;
  for (i = 0;i < N;i++)
    p += sprintf(p," %s",redoList[i]);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Return the current selection in the edit window.
 *
 * Parameters:
 *    _d		Client data (unused)
 *    tcl		Tcl interpreter
 *    argc		Number of arguments
 *    argv		Vector of arguments
 *
 * Returns:		Tcl status
 *
 * Tcl syntax:
 *
 *   gat_getSelected gates		Returns list of all selected element names
 *
 *   gat_getSelected module		Returns the selected module name if it is the only selection.
 *
 *   gat_getSelected			Same as "gates" option
 *
 *****************************************************************************/
static int gat_getSelected(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  HashElem *E;
  GSelection *gs = TkGate.circuit->mg_selection;
  const char *what = "gates";

  if (argc > 1) what = argv[1];

  if (strcmp(what,"gates") == 0) {
    /*
     * WARNING: This function may not work correctly if there are a lot of gates
     * selected since the tcl->result buffer is not expanded as necessary.  However,
     * this features is not currently used, so it should not matter for now.
     */
    char *p = tcl->result;
    for (E = Hash_first(gs->s_gates);E;E = Hash_next(gs->s_gates,E)) {
      GCElement *g = (GCElement*) HashElem_obj(E);
      if (p != tcl->result)
	p += sprintf(p," ");
      p += sprintf(p,"%s",g->ename);
    }
  } else {
    if (sel_num(TkGate.circuit->es) == 1) {
      GCElement *g = TkGate.circuit->select;

      if (g && GCElement_isModule(g)) {
	strcpy(tcl->result,g->u.block.moduleName);
      }
    }
  }

  return TCL_OK;
}

static int gat_obMode(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int m = 1;
  
  if (argc == 1) {
    m = ob_get_mode();
    sprintf(tcl->result,"%d",m);
  } else {
    sscanf(argv[1],"%d",&m);
    ob_mode(m);
  }

  return TCL_OK;
}

static int gat_obFlush(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  ob_clear();
  return TCL_OK;
}

static int gat_obUnstick(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  ob_unstick();
  return TCL_OK;
}

static int gat_setShowXHairState(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int n;

  assert(argc == 2);
  sscanf(argv[1],"%d",&n);

  GScope_setShowXHairState(n);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Called at the end of an editing action to cause internal circuit data to
 * be resynchronized with tcl interface data.
 *
 *****************************************************************************/
static int gat_syncInterface(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  SynchronizeInterface();
  return TCL_OK;
}

static int gat_modintCenter(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  modint_center();
  return TCL_OK;
}


static int bit_width = 0;
static int bit_height = 0;
static unsigned **bit_data = 0;

/*
 * Basic word size/byte size declarations.
 */
#define SSWORDSIZE		TKGATE_WORDSIZE		/* # bits in an unsigned */
#define SSWORDMASK		((unsigned)~0)		/* Word with all bits set */
#if (SSWORDSIZE == 32)
#define SSBITMASK		0x1f			/* Mask to get bit in word */
#define SSWORDSHIFT		5			/* Shift to get word index */
#define SSWORDBYTES		4			/* Number of bytes per word */
#define SSHALFWORDMASK		0xffff			/* Mask for a half word */
#elif (SSWORDSIZE == 64)
#define SSBITMASK		0x3f			/* Mask to get bit in word */
#define SSWORDSHIFT		6			/* Shift to get word index */
#define SSWORDBYTES		8			/* Number of bytes per word */
#define SSHALFWORDMASK		0xffffffff		/* Mask for a half word */
#else
#error Unsupported word size.
#endif

static void Bit_extend(int x,int y)
{
  x++; y++;

  if (x > bit_width) {
    int i;

    bit_width = x;
    for (i = 0;i < bit_height;i++)
      bit_data[i] = ob_realloc(bit_data[i],bit_width*sizeof(unsigned));
  }

  if (y > bit_height) {
    int i;

    if (bit_data)
      bit_data = ob_realloc(bit_data,y*sizeof(unsigned*));
    else
      bit_data = ob_malloc(y*sizeof(unsigned*),"unsigned*");

    for (i = bit_height;i < y;i++) 
      bit_data[i] = ob_malloc(bit_width*sizeof(unsigned),"unsigned");
    bit_height = y;
  }
}

/*****************************************************************************
 *
 * Get bit data from bit buffer
 *
 * set data [gat_getBit x y]
 *
 *****************************************************************************/
static int gat_getBit(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int x,y;

  sscanf(argv[1],"%d",&x);
  sscanf(argv[2],"%d",&y);

  Bit_extend(x,y);

  if ((bit_data[y][(x>>SSWORDSHIFT)] & (1 << (x & SSBITMASK))))
    Tcl_SetResult(tcl,"1",TCL_STATIC);
  else
    Tcl_SetResult(tcl,"0",TCL_STATIC);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Set bit data in bit buffer
 *
 * gat_setBit x y d
 *
 *****************************************************************************/
static int gat_setBit(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  int x,y,d;

  sscanf(argv[1],"%d",&x);
  sscanf(argv[2],"%d",&y);
  sscanf(argv[3],"%d",&d);

  Bit_extend(x,y);

  if (d)
    bit_data[y][(x>>SSWORDSHIFT)] |= (1 << (x & SSBITMASK));
  else
    bit_data[y][(x>>SSWORDSHIFT)] &= ~(1 << (x & SSBITMASK));

  return TCL_OK;
}

/*****************************************************************************
 *
 * Exit tkgate
 *
 *****************************************************************************/
static int gat_exit(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  extern int exit_flag;

  exit_flag = 1;

  DoTcl("exit");
  return TCL_OK;
}


/*****************************************************************************
 *
 * Table of command names and handler functions for tkgate.
 *
 *****************************************************************************/
static Tkg_Command tkg_procTable[] = {
  {"gat_addPort",		gat_addPort},
  {"gat_align",			gat_align},
  {"gat_analyzerCommand",	gat_analyzerCommand},
  {"gat_anchor",		gat_anchor},
  {"gat_batInc",		gat_batInc},
  {"gat_breakpoint",		gat_breakpoint},
  {"gat_changeNet",		gat_changeNet},
  {"gat_changePinDir",		gat_changePinDir},
  {"gat_claimBlock",		gat_claimBlock},
  {"gat_cleanUp",		gat_cleanUp},
  {"gat_clearPath",		gat_clearPath},
  {"gat_closeBox",		gat_closeBox},
  {"gat_computestrhash",	gat_computestrhash},
  {"gat_copyBlock",		gat_copyBlock},
  {"gat_copyToBuf",		gat_copyToBuf},
  {"gat_cpathFlash",		gat_cpathFlash},
  {"gat_cutToBuf",		gat_cutToBuf},
  {"gat_cutToBufAppend",	gat_cutToBufAppend},
  {"gat_debug",			gat_debug},
  {"gat_delScopeProbe",		gat_delScopeProbe},
  {"gat_deleteBlock",		gat_deleteBlock},
  {"gat_deleteSelected",	gat_deleteSelected},
  {"gat_dohyperlink",		gat_dohyperlink},
  {"gat_dumpWires",		gat_dumpWires},
  {"gat_editBlockDesc", 	gat_editBlockDesc},
  {"gat_editNet",		gat_editNet},
  {"gat_editProps",		gat_editProps},
  {"gat_editModProps",		gat_editModProps},
  {"gat_editCircProps",		gat_editCircProps},
  {"gat_errBoxReport",		gat_errBoxReport},
  {"gat_exit",			gat_exit},
  {"gat_find",			gat_find},
  {"gat_formatTime",		gat_formatTime},
  {"gat_getBit",		gat_getBit},
  {"gat_getCircProp",		gat_getCircProp},
  {"gat_getLocales",		gat_getLocales},
  {"gat_getMajorMode",		gat_getMajorMode},
  {"gat_getPrinterList",	gat_getPrinterList},
  {"gat_getRedoList",		gat_getRedoList},
  {"gat_getSelected",		gat_getSelected},
  {"gat_getTech",		gat_getTech},
  {"gat_getTechList",		gat_getTechList},
  {"gat_getTraceData",		gat_getTraceData},
  {"gat_getUndoList",		gat_getUndoList},
  {"gat_gotoCPathNet",		gat_gotoCPathNet},
  {"gat_hdl",			gat_hdl},
  {"gat_init",			gat_init},
  {"gat_interface",		gat_interface},
  {"gat_libIsLoaded",		gat_libIsLoaded},
  {"gat_load",			gat_load},
  {"gat_loadLibrary",		gat_loadLibrary},
  {"gat_loadMore",		gat_loadMore},
  {"gat_make",			gat_make},
  {"gat_makeMakeMenu",		gat_makeMakeMenu},
  {"gat_makeMakeShortcuts",	gat_makeMakeShortcuts},
  {"gat_mode",			gat_mode},
  {"gat_modintCenter",		gat_modintCenter},
  {"gat_moveGate",		gat_moveGate},
  {"gat_new",			gat_new},
  {"gat_newBlock",		gat_newBlock},
  {"gat_openBox",		gat_openBox},
  {"gat_obAppendFrame",		gat_obAppendFrame},
  {"gat_obBeginFrame",		gat_obBeginFrame},
  {"gat_obEndFrame",		gat_obEndFrame},
  {"gat_obFlush",		gat_obFlush},
  {"gat_obMode",		gat_obMode},
  {"gat_obUnstick",		gat_obUnstick},
  {"gat_popupPortCmd",		gat_popupPortCmd},
  {"gat_popupSetState",		gat_popupSetState},
  {"gat_popupWireProps",	gat_popupWireProps},
  {"gat_popupWireAddStub",	gat_popupWireAddStub},
  {"gat_popupWireLabel",	gat_popupWireLabel},
  {"gat_popupWireSize",		gat_popupWireSize},
  {"gat_print",			gat_print},
  {"gat_redo",			gat_redo},
  {"gat_refreshScreen",		gat_refreshScreen},
  {"gat_reinitDelay",		gat_reinitDelay},
  {"gat_renameBlock",		gat_renameBlock},
  {"gat_replicate",		gat_replicate},
  {"gat_reroot",		gat_reroot},
  {"gat_rotate",		gat_rotate},
  {"gat_saveCircuit",		gat_saveCircuit},
  {"gat_scopeButton",		gat_scopeButton},
  {"gat_scopeCommand",		gat_scopeCommand},
  {"gat_scopeMotion",		gat_scopeMotion},
  {"gat_scopeRelease",		gat_scopeRelease},
  {"gat_selectAll",		gat_selectAll},
  {"gat_selectNet",		gat_selectNet},
  {"gat_setBlockDesc",		gat_setBlockDesc},
  {"gat_setBit",		gat_setBit},
  {"gat_setCircProp",		gat_setCircProp},
  {"gat_setcolors",		gat_setcolors},
  {"gat_setDip",		gat_setDip},
  {"gat_setGateTechDelays",	gat_setGateTechDelays},
  {"gat_setMajorMode",		gat_setMajorMode},
  {"gat_setpop",		gat_setpop},
  {"gat_setShowXHairState",	gat_setShowXHairState},
  {"gat_setTech",		gat_setTech},
  {"gat_setrot",		gat_setrot},
  {"gat_showPath",		gat_showPath},
  {"gat_simSelected",		gat_simSelected},
  {"gat_syncInterface",		gat_syncInterface},
  {"gat_traceLPPEst",		gat_traceLPPEst},
  {"gat_tracePrint",		gat_tracePrint},
  {"gat_toggleProbe",		gat_toggleProbe},
  {"gat_undo",			gat_undo},
  {"gat_unloadLibrary",		gat_unloadLibrary},
  {"gat_unselectGates",		gat_unselectGates},
  {"gat_updateInterface",	gat_updateInterface},
  {"gat_updateModuleTree",	gat_updateModuleTree},
  {"gat_updateMenuState",	gat_updateMenuState},
  {"gat_validatePortName",	gat_validatePortName},
  {"gat_verify",		gat_verify},
  {"gat_yankFromBuf",		gat_yankFromBuf},
  {"gat_zoom",			gat_zoom},
};
static int tkg_procTableLen = sizeof(tkg_procTable)/sizeof(tkg_procTable[0]);

/*****************************************************************************
 *
 * Tcl handler function initialization.
 *
 * Parameters:
 *      tcl		tcl interpreter handel
 *
 * Create entries for all of the tcl commands that are used by tkgate.
 *
 *****************************************************************************/
void init_tclProcs(Tcl_Interp *tcl)
{
  int i;

  for (i = 0;i < tkg_procTableLen;i++) {
    Tcl_CreateCommand(tcl,tkg_procTable[i].command,tkg_procTable[i].func,0,0);
  }
}
