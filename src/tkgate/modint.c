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

    Last edit by hansen on Wed Mar 18 05:13:38 2009
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "tkgate.h"

#define MODINT_DEBUG 0
#define MODINT_BLOCKSPACE	50

/*****************************************************************************
 *
 * Compare two interfaces for sorting purposes.
 *
 *****************************************************************************/
static int blknam_compare(const void *pA,const void *pB)
{
  GCElement *A = *(GCElement**) pA;
  GCElement *B = *(GCElement**) pB;

  return strcasecmp(A->u.block.moduleName,B->u.block.moduleName);
}

/*****************************************************************************
 *
 * Find the display version of an interface.
 *
 *****************************************************************************/
GCElement *modint_findDisplay(const char *name)
{
  GModuleDef *M = TkGate.circuit->mid_display;
  HashElem *E;

#if MODINT_DEBUG
  printf("modint_find(%s)\n",name);
#endif
  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
#if MODINT_DEBUG
    printf("  checking %s\n",g->u.block.moduleName);
#endif
    if (strcmp(g->u.block.moduleName,name) == 0)
      return g;
  }
#if MODINT_DEBUG
  printf("  not found\n");
#endif
  return 0;
}

/*****************************************************************************
 *
 * Find the module interface for the named module.
 *
 *****************************************************************************/
GCElement *modint_find(const char *name)
{
  GModuleDef *M = TkGate.circuit->mid_mod;
  HashElem *E;

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (strcmp(g->u.block.moduleName,name) == 0)
      return g;
  }
  return 0;
}

/*****************************************************************************
 *
 * Find the alternate module interface for the named module.
 *
 *****************************************************************************/
GCElement *modint_findInAlt(const char *name)
{
  GModuleDef *M = TkGate.circuit->mid_altMod;
  HashElem *E;

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (strcmp(g->u.block.moduleName,name) == 0)
      return g;
  }
  return 0;
}

/*****************************************************************************
 *
 * Reset the module interface to a plain box.
 *
 *****************************************************************************/
void modint_reset(EditState *es, GCElement *g)
{
  int N;
  int i;

  if (!g || g->typeinfo->Code != GC_BLOCK)
    return;

  N = GCElement_numPads(g);
  for (i = 0;i < N;i++) {
    GWire *w,*n_w;

    for (w = g->wires[i];w; w = n_w) {
      n_w = w->next;
      block_cutoffwire(w,es);
    }
  }

  gate_draw(g,GD_NOWIRE);
  ob_touch(g);
  g->u.block.gwidth = MINSIZE;
  g->u.block.gheight = MINSIZE;
  gate_draw(g,GD_NOWIRE);
}

/*****************************************************************************
 *
 * Delete the module interface for a module.
 *
 *****************************************************************************/
void modint_deleteInterface(GModuleDef *M)
{
#if MODINT_DEBUG
  printf("modint_deleteInterface(%s)\n",M->m_name);
#endif

  if (M->m_interface) {
    gate_delete(M->m_interface,TkGate.circuit->mid_mod,0);
    M->m_interface = 0;
  }

  if (TkGate.circuit->es && TkGate.circuit->es->isInterface) {
    modint_arrange(TkGate.circuit->es);
    FlagRedraw();
  }
}

/*****************************************************************************
 *
 * Rename the interface of a module to match the module name.  This is generally
 * called after a module is renamed to synchronize the interface name to the
 * new module name.
 *
 *****************************************************************************/
void modint_renameInterface(GModuleDef *M)
{
  GCElement *g;

  if (M->m_interface) {
    g = M->m_interface;
    if (g->u.block.moduleName)
      ob_free(g->u.block.moduleName);
    ob_touch(g);
    g->u.block.moduleName = ob_strdup(M->m_name);
  }
  if (M->m_altInterface) {
    g = M->m_altInterface;
    if (g->u.block.moduleName)
      ob_free(g->u.block.moduleName);
    ob_touch(g);
    g->u.block.moduleName = ob_strdup(M->m_name);
  }

  if (TkGate.circuit->es->isInterface) {
    modint_arrange(TkGate.circuit->es);
    FlagRedraw();
  }
}

/*****************************************************************************
 *
 * Set the interface of a module.
 *
 * Parameters:
 *      M		Module whos interface is to be set.
 *      g		New interface for module.
 *
 * Updates the interface of a module.  The specified gate is copied and that
 * copy is stored in the module interfaces buffer.
 *
 *****************************************************************************/
int modint_setInterface(GModuleDef *M,GCElement *g)
{
  GCElement *r = 0;

#if 0
  printf("modint_setInterface(M=%s, g=%p) [d=%p]\n",M->m_name, g, d);
#endif

  if (g && GModuleDef_isIntfProtected(M)) {
#if MODINT_DEBUG
    printf("modint_setInterface failed: %s\n",g->u.block.moduleName);
#endif
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return -1;
  }

  if (M->m_interface) {
    gate_delete(M->m_interface,TkGate.circuit->mid_mod,0);
    M->m_interface = 0;
  }

  if (g) {
    r = (*g->typeinfo->CopyGate)(TkGate.circuit->mid_mod,g,0,0,0);
    ob_touch(r);
    r->show_name = 0;
  } else {
    GGateInfo *bgi = GGateInfo_codeLookup(GC_BLOCK);
    const char *args[2];
    args[0] = "-func";
    args[1] = M->m_name;
    r = (*bgi->MakeFunction)(0,TkGate.circuit->mid_mod,GC_BLOCK,50,50,0,0,0,args,2);
  }
  ob_touch(M);
  M->m_interface = r;
  r->isModuleInterface = 1;
#if MODINT_DEBUG
  printf("modint_setInterface(%s) -> %p [n=%d]\n",
	 M->m_name,r,Hash_numElems(TkGate.circuit->mid_mod->m_gates));
#endif

  return 0;
}

/*****************************************************************************
 *
 * Nicely arrange modules in the module interface buffer.
 *
 * Parameters:
 *    es		The current edit state.
 *
 *****************************************************************************/
void modint_arrange(EditState*es)
{
  GModuleDef *M = TkGate.circuit->mid_mod;
  HashElem *E;
  GCElement **blist;
  int N = Hash_numElems(M->m_gates);
  int i;
  int x,y,maxh;
  int isFirst;  

  if (N < 1) return;					/* No defined blocks */

  ob_touch(TkGate.circuit);
  TkGate.circuit->no_set_modify = 1;
  blist = (GCElement**) calloc(N,sizeof(GCElement*));	/* Temporary allocation */

  /*
   * Get the list of modules and sort them alphabetically.
   */
  for (i = 0,E = Hash_first(M->m_gates);E;i++, E = Hash_next(M->m_gates,E))
    blist[i] = (GCElement*) HashElem_obj(E);
  qsort(blist,N,sizeof(GCElement*),blknam_compare);

  /*
   * Initialize the first row
   */
  x = MODINT_BLOCKSPACE;
  y = MODINT_BLOCKSPACE;
  maxh = 0;
  isFirst = 1;

  for (i = 0;i < N;i++) {
    GCElement *g = blist[i];
    int minx,miny,maxx,maxy;
    int width,height,cx,cy;

    ob_touch(g);
    g->top = g->bottom = g->right = g->left = 0;
    (*g->typeinfo->GetExtents)(g,TD_X11,&minx,&miny,&maxx,&maxy,0);
    width = maxx-minx;
    height = maxy-miny;

    if (GCElement_getType(g) == GC_SYMBLOCK) {
      cx = width/2;
      cy = height/2;
    } else
      cx = cy = 0;


    if (!isFirst && (x + width + MODINT_BLOCKSPACE) > TkGate.width) { 
      x = MODINT_BLOCKSPACE;
      y += MODINT_BLOCKSPACE + maxh;
      maxh = 0;
    }

    /*
     * Place the block/symbol at (x,y)
     */
    gate_moveTo(g,x+cx,y+cy);
#if MODINT_DEBUG
    printf("  place %s::%s @ (%d, %d)\n",g->u.block.moduleName,g->ename,g->xpos,g->ypos);
#endif


    x += width + MODINT_BLOCKSPACE;
    if (height > maxh) maxh = height;

    isFirst = 0;
  }

  ob_touch(TkGate.circuit);
  TkGate.circuit->no_set_modify = 0;

  free(blist);
}

void modint_update(EditState *es)
{
#if MODINT_DEBUG
  printf("modint_update\n");
#endif
}

/*****************************************************************************
 *
 * Center the displayed module interface in the available screen area.
 *
 *****************************************************************************/
void modint_center()
{
  HashElem *he;
  GCElement *g;
  int x,y;
  if (!TkGate.circuit->mid_display) return;

  he = Hash_first(TkGate.circuit->mid_display->m_gates);
  if (!he) return;
  g = (GCElement*) HashElem_obj(he);
  if (!g) return;

  SetModified(MF_BEGINSPEC);

  if (GCElement_getType(g) == GC_SYMBLOCK) {
    x = TkGate.width/2;
    y = TkGate.height/2;
  } else {
    x = TkGate.width/2 - (g->u.block.gwidth)/2;
    y = TkGate.height/2 - g->u.block.gheight/2;
  }

  if (x != g->xpos || y != g->ypos || TkGate.circuit->org_x != 0 || TkGate.circuit->org_y != 0) {
    TkGate.circuit->org_x = TkGate.circuit->org_y = 0;
    ob_begin_framef("-Center",FF_TRANSPARENT);
    gate_moveTo(g,x,y);
    ob_end_frame();
    FlagRedraw();
  }

  SetModified(MF_ENDSPEC);
}

/*****************************************************************************
 *
 * Synchronoize the module int the display buffer with the actual module interface
 *
 *****************************************************************************/
void modint_syncDisplay(GModuleDef *M)
{
  GCElement *g = M->m_interface;
  int x,y;

  if (!g) return;

  TkGate.circuit->es->isInterface = 0;
  GModuleDef_flush(TkGate.circuit->mid_display);
  x = TkGate.width/2;
  y = TkGate.height/2;
  TkGate.circuit->org_x = TkGate.circuit->org_y = 0;

  DoTcl("gat_make MODULE -x %d -y %d -func %s -doinit 1 -dodraw 0",x,y,M->m_name);
  TkGate.circuit->es->isInterface = 1;

  DoTcl("NetList::flush");	/* Hack to clear the netlist */
}

/*****************************************************************************
 *
 * Edit a module interface
 *
 *****************************************************************************/
void modint_edit(EditState **es,GModuleDef *M)
{
  ob_touch(*es);

#if MODINT_DEBUG
  printf("modint_edit(%s)\n",M ? M->m_name : "0");
#endif

  /*
   * If we are in interface mode, just pop the current interface editstate before
   * pushing the new one.
   */
  if ((*es)->isInterface) {
    editstate_pop(es);
    ob_touch(*es);
  }

  ob_touch(*es);


  /*
   * Treat root module as list of all modules.
   */
  if (M == TkGate.circuit->root_mod) M = 0;

  /*
   * If we are given a module, we just display that interface.  If no module, then
   * we display all interfaces.
   */
  if (M) {
    editstate_push(es,TkGate.circuit->mid_display,0);
    ob_touch(TkGate.circuit->mid_display);
    ob_free(TkGate.circuit->mid_display->m_name);
    TkGate.circuit->mid_display->m_name = ob_strdup(M->m_name);

    editstate_setCurrent(*es);

    modint_syncDisplay(M);
  } else {
    editstate_push(es,TkGate.circuit->mid_mod,0);
    (*es)->isInterface = 1;
    modint_arrange(*es);
    editstate_setCurrent(*es);
  }

  DoTcl("IPanel::loadCurrent");

  SetModified(MF_MODULE|MF_SYNCONLY);
  SynchronizeInterface();

  FlagRedraw();
}

/*****************************************************************************
 *
 * Close the module interface editor.
 *
 *****************************************************************************/
void modint_close()
{
  GModuleDef *m;

  if (TkGate.circuit->es->env == TkGate.circuit->mid_display)
    m = env_findModule(TkGate.circuit->mid_display->m_name);	/* Editing an interface */
  else
    m = TkGate.circuit->root_mod;				/* Top-level interface editor */

#if MODINT_DEBUG
  printf("modint_close: (%s)=%p\n",TkGate.circuit->mid_display->m_name,m);
#endif

  ob_touch(TkGate.circuit);

  EditState_unselectGate(TkGate.circuit->es);

  while (TkGate.circuit->es && TkGate.circuit->es->isInterface)
    editstate_pop(&TkGate.circuit->es);
  //  editstate_push(&TkGate.circuit->es, m, 0);

  editstate_setCurrent(TkGate.circuit->es);
  SetModified(MF_MODULE|MF_SYNCONLY);
  SynchronizeInterface();
  FlagRedraw();
}

/*****************************************************************************
 *
 * Make the interface for this module the default symbol.
 *
 *****************************************************************************/
void modint_makeSymbolInterface(GModuleDef *M,GModSymbol *ms)
{
  GGateInfo *bgi = GGateInfo_codeLookup(GC_BLOCK);
  const char *args[2];
  GCElement *g;

  /*
   * Create a module and convert it to a symbol block.
   */
  args[0] = "-func";
  args[1] = M->m_name;
  g = (*bgi->MakeFunction)(0,0,GC_BLOCK,50,50,0,0,0,args,2);
  g = SymBlock_convert(g,ms,M,0);

  /*
   * Make this new symbol block gate the new interface for M. 
   */
  modint_setInterface(M, g);
}

/*****************************************************************************
 *
 * Flush all module instances
 *
 *****************************************************************************/
void modint_flush()
{
#if 0
  GModuleDef *mm = TkGate.circuit->mid_mod;
  List dlist;

  List_init(&dlist);

  for (E = Hash_first(mm->m_gates);E;E = Hash_next(mm->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (GCElement_getType(g) != GC_BLOCK && 
	GCElement_getType(g) != GC_SYMBLOCK)
      continue;

    List_addToTail(&dlist, 
  }

  List_uninit(&dlist);
    modint_deleteInterface(mi);
#endif
    printf("begin: modint_flush\n");
    env_clear(TkGate.circuit->mid_mod);
    printf("end: modint_flush\n");
}

/*****************************************************************************
 *
 * Open the selected module.  If modName is non-null, it is the name of a module
 * selected from the module list/tree.  If it is null, then we look up the module
 * from the current selection.
 *
 *****************************************************************************/
void modint_openSelected(const char *modName)
{
  
  EditState **es = &TkGate.circuit->es;
  GModuleDef *M;
  char *p;
  int listSelection = (modName != 0);

  /*
   * Make sure the selected object is a module, and look it up.  If we do not
   * find it, then return and do nothing.
   */
  if (!modName && TkGate.circuit->select && GCElement_isModule(TkGate.circuit->select))
    modName = TkGate.circuit->select->u.block.moduleName;
  if (!modName) return;
  p = strrchr(modName,'/');
  if (p) modName = p+1;
  M = env_findModule(modName);
  if (!M) return;

  if (editstate_getInterfaceMode() == INTFMODE_ALL || listSelection) {
    /*
     * We are in the all interface display mode or we selected the module from
     * the module list.  Open it for individual editing.
     */
    modint_edit(es,M);
    editstate_setCurrent(*es);
    SetModified(MF_MODULE|MF_SYNCONLY);
    SynchronizeInterface();
    FlagRedraw();
  }
}
