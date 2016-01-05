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

    Last edit by hansen on Tue Mar 17 06:49:35 2009
****************************************************************************/
#include "tkgate.h"

static const char *unitCodes[] = {"s", "ms", "us", "ns", "ps", "fs"};
static int numUnitCodes  = sizeof(unitCodes)/sizeof(unitCodes[0]);


/*
 * 1ns/1ns
 */
static Timescale defaultTimescale = {1000000,1000000};

void Timescale_decode(simtime_t n, int *num,char *units)
{
  int index = 0;

  while (n >= 1000) {
    n /= 1000;
    index++;
  }

  if (index >= numUnitCodes || (n != 1 && n != 10 && n != 100)) {
    /* Invalid code, so just default to 1ns */
    if (num) *num = 1;
    if (units) strcpy(units,"ns");
    return;
  }


  if (units) strcpy(units,unitCodes[numUnitCodes-1-index]);
  if (num) *num = n;
}

const char *Timescale_codeToUnits(int code)
{
  if (code >= 0 && code < numUnitCodes)
    return unitCodes[code];
  else
    return 0;
}

int Timescale_unitsToCode(const char *units)
{
  int i;

  for (i = 0;i < numUnitCodes;i++) {
    if (strcmp(units, unitCodes[i]) == 0) return i;
  }
  return -1;  
}

simtime_t Timescale_parse(int num,const char *units)
{
  simtime_t U = 1;
  int i;

  for (i = numUnitCodes-1;i >= 0;i--) {
    if (strcmp(units, unitCodes[i]) == 0) break;
    U *= 1000;
  }

  if (i < 0) {
    return 0;
  }

  if (num != 1 && num != 10 && num != 100) {
    return 0;
  }

  U *= num;

  return U;
}

void Timescale_save(Timescale *ts, FILE *f)
{
  int n1,n2;
  char u1[STRMAX],u2[STRMAX];

  Timescale_decode(ts->ts_units, &n1, u1);
  Timescale_decode(ts->ts_precision, &n2, u2);

  fprintf(f,"`timescale %d%s/%d%s\n",n1,u1,n2,u2);
}

Circuit *new_Circuit()
{
  Circuit *c = (Circuit*) ob_malloc(sizeof(Circuit),"Circuit");
  extern Timescale defaultTimescale;

  c->currentFile = new_CurrentFile();
  c->fileVersion = ob_strdup("");
  c->title = 0;
  c->c_encoding = 0;
  c->c_locale = 0;
  c->c_timescale = defaultTimescale;
  c->mid_mod = new_GModuleDef("<interfaces>");		/* Buffer for all interfaces */
  c->mid_display = new_GModuleDef("<display>");		/* Temporary buffer for editing an interface */
  c->mid_altMod = new_GModuleDef("<alternate>");	/* Buffer for all alternate interfaces */
  c->c_breakpoints = new_NHash();
  c->c_scripts = new_NHash();
  c->c_probes = new_List();
  c->cut_buffer = 0;
  c->mg_selection = 0;
  c->numInitScripts = 0;
  c->initScripts = 0;
  c->zoom_factor = 1;
  c->modified_flags = 0;
  c->search = new_GSearchContext();
  c->es = new_EditState();
  c->moduleTable = new_SHash();
  c->c_gatePrefix = TKGATE_DEFAULT_PREFIX;
  c->labelsel = new_GrabbedLabel();
  c->rot = 0;
  c->mode = MODE_MOVE;
  c->c_isNewFile = 0;

  Circuit_initOptions(c);
  Circuit_setLocale(c,TkGate.locale);
  Circuit_setFileEncoding(c,TkGate.locale->l_encVerilog);

  return c;
}

void Circuit_initOptions(Circuit *c)
{
  c->fileVersion = ob_strdup(VERSIONS[0].vd_name);

  c->c_tvMode = TV_ALL;
  c->c_startup = 0;
  c->c_startupUnits = UC_NANOSEC;

  c->discardChanges = 0;
  c->useExtBars = TkGate.extendBars;
  c->showSwitchNets = TkGate.showSwitchNets;
  c->simAutoStart = 0;

  c->simClockMode = 0;
  c->simClockName = 0;
}

void Circuit_setCurrentFile(const char *name)
{
  CurrentFile_set(TkGate.circuit->currentFile,name);
}

void Circuit_setCurrentFileVersion(const char *ver)
{
  ob_touch(TkGate.circuit);
  if (TkGate.circuit->fileVersion)
    ob_free(TkGate.circuit->fileVersion);
  TkGate.circuit->fileVersion = ob_strdup(ver);
}

void Circuit_setTitle(const char *title)
{
  ob_touch(TkGate.circuit);
  if (TkGate.circuit->title) ob_free(TkGate.circuit->title);
  TkGate.circuit->title = title ? ob_strdup(title) : 0;
}

void Circuit_setLocale(Circuit *c,Locale *locale)
{
  ob_touch(c);
  c->c_locale = locale;
  c->c_displayEncoder = getEncoder(locale->l_encDisplay,CE_UTF8);
  c->c_psEncoder = getEncoder(locale->l_encPostscript,CE_UTF8);
#if LOCALE_DEBUG
  printf("Circuit_setLocale(%s)\n",locale->l_code);
#endif
}

void Circuit_setFileEncoding(Circuit *c,const char *encoding)
{
  ob_touch(c);
  if (c->c_encoding) ob_free(c->c_encoding);
  c->c_encoding = ob_strdup(encoding);

  TkGate.japaneseMode = (strcmp(encoding,CE_EUC_JP) == 0);

#if LOCALE_DEBUG
  printf("Circuit_setFileEncoding(%s)\n",encoding);
#endif

  if (TkGate.japaneseMode)
    initJapanese();
  c->c_loadEncoder = getEncoder(CE_UTF8,c->c_encoding);
  c->c_saveEncoder = getEncoder(c->c_encoding,CE_UTF8);
}

void Circuit_setScripts(int nScripts,const char **scripts)
{
  int i;

  ob_touch(TkGate.circuit);
  if (TkGate.circuit->initScripts)
    ob_free(TkGate.circuit->initScripts);

  TkGate.circuit->numInitScripts = nScripts;
  if (nScripts == 0) {
    TkGate.circuit->initScripts = 0;
    return;
  }
  TkGate.circuit->initScripts = (char**) ob_malloc(sizeof(char**)*nScripts,"char**");
  for (i = 0;i < nScripts;i++)
    TkGate.circuit->initScripts[i] = ob_strdup(scripts[i]);
}

#if 0
void Circuit_setLibraries(int nLibraries,const char **libraries)
{
  int i;

  ob_touch(TkGate.circuit);
  if (TkGate.circuit->libraries)
    ob_free(TkGate.circuit->libraries);

  TkGate.circuit->numLibraries = nLibraries;
  if (nLibraries == 0) {
    TkGate.circuit->libraries = 0;
    return;
  }
  TkGate.circuit->libraries = (char**) ob_malloc(sizeof(char**)*nLibraries,"char**");
  for (i = 0;i < nLibraries;i++)
    TkGate.circuit->libraries[i] = ob_strdup(libraries[i]);
}
#endif

void Circuit_setClockName(const char *name)
{
  Circuit *C = TkGate.circuit;
  char *p;

  ob_touch(C);
  if (C->simClockName) ob_free(C->simClockName);
  C->simClockName = 0;

  if (!name || !*name) return;

  C->simClockName = ob_strdup(name);

  for (p = C->simClockName;*p;p++) {
    if (!isalpha(*p) && !isdigit(*p) && !strchr("_.",*p))
      memmove(p,p+1,strlen(p));
  }

  if (!*C->simClockName) {
    ob_free(C->simClockName);
    C->simClockName = 0;
  }
}

/*****************************************************************************
 *
 * Change the root/top module.
 *
 *****************************************************************************/
void Circuit_changeRoot(GModuleDef *newR)
{
  GModuleDef *oldR = TkGate.circuit->root_mod;

  /*
   * No change in root.
   */
  if (newR == oldR)
    return;

  ob_touch(TkGate.circuit);
  TkGate.circuit->root_mod = newR;

  /*
   * The current root shouldn't have an interface, but if it does, delete it.
   */
  if (oldR->m_interface) {
    gate_delete(oldR->m_interface, TkGate.circuit->mid_mod,0);
    ob_touch(oldR);
    oldR->m_interface = 0;
    oldR->m_altInterface = 0;
  }

  /*
   * If the current root module has an alternate interface, move it to be the primary interface.
   */
  if (oldR->m_altInterface) {
    GCElement *g;

    ob_touch(oldR);
    g = (*oldR->m_altInterface->typeinfo->CopyGate)(TkGate.circuit->mid_mod,oldR->m_altInterface,0,0,0);
    gate_delete(oldR->m_altInterface, TkGate.circuit->mid_altMod,0);
    oldR->m_interface = g;
  } else
    modint_setInterface(oldR,0);

  /*
   * If the new root has an alternate interface, delete it.
   */
  if (newR->m_altInterface) {
    gate_delete(newR->m_altInterface, TkGate.circuit->mid_altMod,0);
    ob_touch(newR);
    newR->m_interface = 0;
  }

  /*
   * If the new root module has an interface, move it to be the alternate interface.
   */
  if (newR->m_interface) {
    GCElement *g;

    ob_touch(newR);
    g = (*newR->m_interface->typeinfo->CopyGate)(TkGate.circuit->mid_altMod,newR->m_interface,0,0,0);
    gate_delete(newR->m_interface, TkGate.circuit->mid_mod,0);
    newR->m_altInterface = g;
  }

  if (TkGate.circuit->es->isInterface) {
    modint_arrange(TkGate.circuit->es);
  }

  SetModified(MF_MODULE);
}

void Circuit_clearSavedProbes(Circuit *c)
{
  ListElem *le;

  /*
   * Saved probe names do not use ob_malloc system.
   */
  for (le = List_first(c->c_probes);le;le = List_next(c->c_probes,le)) {
    free(ListElem_obj(le));
  }
  List_flush(c->c_probes);
}

void Circuit_addProbeName(Circuit *c,const char *name,GNet *net)
{
  /*
   * Saved probe names do not use ob_malloc system.
   */
  List_addToTail(c->c_probes,strdup(name));
}

void Circuit_clear()
{
  Circuit *c = TkGate.circuit;
  extern Timescale defaultTimescale;

  BrkPtTable_flush(c->c_breakpoints);
  c->c_timescale = defaultTimescale;

  Circuit_clearSavedProbes(c);

  Circuit_setTitle(0);
  Circuit_setFileEncoding(c, TkGate.locale->l_encVerilog);
  Circuit_setLocale(c, TkGate.locale);
}

/*****************************************************************************
 *
 * Test to see if there is anything selected in a circuit.
 *
 *****************************************************************************/
int Circuit_isSelection(Circuit *c)
{
  if (TkGate.circuit->select || TkGate.circuit->mg_selection || TkGate.circuit->wnsel)
    return 1;
  else
    return 0;
}



void Circuit_loadLibrary(Circuit *c,const char *name)
{
  if (tkgate_currentMode() != MM_EDIT)
    tkgate_setMajorMode(MM_EDIT);

  if (VerilogOpenLibrary(name) < 0) {
    message(1,msgLookup("err.badlibopen"),name);	/* Unable to open library file '%s'. */
    return;
  }

  if (c->es->isInterface)
    modint_arrange(c->es);

  FlagRedraw();
}

void Circuit_unloadLibrary(Circuit *c,const char *name)
{
  HashElem *he;
  NHash *dhash;

  /*
   * Make sure "name" is a loaded library.
   */
  if (!SHash_find(TkGate.libraries, name))
    return;

  dhash = new_NHash_noob();

  SHash_remove(TkGate.libraries, name);

  /*
   * Get list of modules to be deleted.
   */
  for (he = Hash_first(TkGate.circuit->moduleTable);he;he = Hash_next(TkGate.circuit->moduleTable,he)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(he);
    if (M->m_isLib && strcmp(M->m_libName,name) == 0) {
      NHash_insert(dhash,(int)M,M);
    }
  }

  /*
   * Delete modules found in library
   */
  for (he = Hash_first(dhash);he;he = Hash_next(dhash,he)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(he);
    ob_touch(M);
    M->m_isLib = 0;
    M->m_protIntf = 0;
    M->m_protEdit = 0;
    env_delete(TkGate.circuit->es,M->m_name);
  }
  delete_NHash(dhash);
}

void Circuit_unloadAllLibraries(Circuit *c)
{
  HashElem *he;
  const char *names[Hash_numElems(TkGate.libraries)+1];
  int n,i;

  for (he = Hash_first(TkGate.libraries), n = 0;he;he = Hash_next(TkGate.libraries,he))
    names[n++] = SHashElem_key(he);

  for (i = 0;i < n;i++)
    Circuit_unloadLibrary(c, names[i]);
}

GrabbedLabel *new_GrabbedLabel()
{
  GrabbedLabel *gl = (GrabbedLabel *) ob_malloc(sizeof(GrabbedLabel),"GrabbedLabel");

  gl->net = 0;
  gl->label = 0;

  return gl;
}

void GrabbedLabel_draw(int x,int y)
{
  GrabbedLabel *gl = TkGate.circuit->labelsel;
  GC gc;

  if (gl->net->n_nbits > 1)
    gc = TkGate.selBusGC;
  else
    gc = TkGate.selWireGC;

  if (gl->net)
    dce_DrawString(gc,x + gl->ox,y + gl->oy,gl->position,gl->label);
}

void GrabbedLabel_unset()
{
  GrabbedLabel *gl = TkGate.circuit->labelsel;
  gl->net = 0;
  if (gl->label) ob_free(gl->label);
  gl->label = 0;
}

void GrabbedLabel_set(GNet *net,int ox,int oy,int p)
{
  GrabbedLabel *gl = TkGate.circuit->labelsel;
  char label[STRMAX];

  GNet_getDisplayLabel(net,label,STRMAX, DLM_GET_ALWAYS);
  gl->net = net;
  gl->label = ob_strdup(label);
  gl->ox = ox;
  gl->oy = oy;
  gl->position = p;

}

