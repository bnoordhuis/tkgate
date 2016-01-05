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

    Last edit by hansen on Tue May  5 20:45:31 2009
****************************************************************************/
#include "tkgate.h"

#define DEBUG_REDISPLAY 0

#define MAX_STACKDEPTH 256

extern int baderp;

extern int GlobalElementCount;
extern int RedrawRequested;
extern int baderp,startrekp;

void SetErrorPosition(int x,int y);

void TkGate_setOrigin(int x,int y)
{
#if 0
  printf("ORIGIN SET (%d, %d)\n",x,y);
#endif
  if (TkGate.circuit->org_x != x || TkGate.circuit->org_y != y) {
    ob_touch(TkGate.circuit);
    TkGate.circuit->org_x = x;
    TkGate.circuit->org_y = y;
  }
  TkGate.idle_ev.scroll_new_x = TkGate.circuit->org_x;
  TkGate.idle_ev.scroll_new_y = TkGate.circuit->org_y;
  editstate_saveOrig(TkGate.circuit->es);
}

GModuleDef *env_findModule(const char *name)
{
  return (GModuleDef*) SHash_find(TkGate.circuit->moduleTable,name);
}

/*
 * Dump the names of child blocks of M to the hierarchical block list tree
 */
static void env_updateMTChildren(GModuleDef *M)
{
  HashElem *gl;

  DoTcl("BlockTree::disownChildren %s",M->m_name);
  for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);

    /*
     * Mark all modules found in M as children of M.  We first make to sure that the
     * block actually has a module name.  In the case of a block that is currently in
     * the process of being defined, it is possible that there may temporarily be some
     * modules with no defined name.
     */
    if (GCElement_isModule(g) && g->u.block.moduleName && *g->u.block.moduleName) {
      const char *prefix = TkGate.circuit->c_gatePrefix;
      if (strncmp(g->u.block.moduleName,prefix,strlen(prefix)) != 0)
	DoTcl("BlockTree::makeChild %s %s",g->u.block.moduleName,M->m_name);
    }
  }
}

static void env_updateMT_simulate(GModuleDef *M,GCElement *g)
{
  HashElem *he;
  char *T = (M->m_type == MT_TEXTHDL) ? "hdl" : "netlist";

  if (M->m_isLib) T = "part";

  if (g) {
    DoTcl("BlockTree::add %s<%s> -type %s",g->ename,M->m_name,T);
    DoTcl("BlockTree::disownChildren %s<%s>",g->ename,M->m_name);
  } else {
    DoTcl("BlockTree::add <%s> -type %s",M->m_name,T);
    DoTcl("BlockTree::disownChildren <%s>",M->m_name);
    DoTcl("BlockTree::setRoot <%s>",M->m_name);

  }

  for (he = Hash_first(M->m_gates);he;he = Hash_next(M->m_gates,he)) {
    GCElement *cg = (GCElement*) HashElem_obj(he);
    GModuleDef *cm = 0;

    if (GCElement_isModule(cg) && cg->u.block.moduleName && *cg->u.block.moduleName) {
      const char *prefix = TkGate.circuit->c_gatePrefix;

      cm = env_findModule(cg->u.block.moduleName);

      /*
       * Ignore any built-in gates.
       */
      if (strncmp(cg->u.block.moduleName,prefix,strlen(prefix)) == 0)
	continue;
    }

    if (cm) {
      if (g)
	DoTcl("BlockTree::makeChild %s<%s> %s<%s>",cg->ename,cm->m_name,g->ename,M->m_name);
      else
	DoTcl("BlockTree::makeChild %s<%s> <%s>",cg->ename,cm->m_name,M->m_name);
      env_updateMT_simulate(cm,cg);
    }
  }
}

/*****************************************************************************
 *
 * Update the module tree while in edit (or related) mode
 *
 *****************************************************************************/
static void env_updateMT_edit(void)
{
  HashElem *he;

  for (he = Hash_first(TkGate.libraries);he;he = Hash_next(TkGate.libraries,he)) {
    DoTclL("BlockTree::addLibrary",SHashElem_key(he),NULL);
  }

  for (he = Hash_first(TkGate.circuit->moduleTable);he;he = Hash_next(TkGate.circuit->moduleTable,he)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(he);

    if (M->m_isLib) {
      DoTclL("BlockTree::addPart",M->m_name,M->m_libName,NULL);
    } else {
      char *T = (M->m_type == MT_TEXTHDL) ? "hdl" : "netlist";
      char ID[8];

      sprintf(ID,"%s%s" , (M->m_protIntf ? "I" : ""), (M->m_protData ? "D" : ""));
      DoTclL("BlockTree::add",M->m_name,"-locks",ID,"-type",T,NULL);
    }

    env_updateMTChildren(M);
  }

  if (TkGate.circuit->root_mod) {
    DoTclL("BlockTree::setRoot",TkGate.circuit->root_mod->m_name,NULL);
  }
}


/*****************************************************************************
 *
 * Flush and reconstruct the module lists for edit mode
 *
 *****************************************************************************/
static void env_updateModuleList(void)
{
  HashElem *E;

  for (E = Hash_first(TkGate.circuit->moduleTable);E;E = Hash_next(TkGate.circuit->moduleTable,E)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(E);

    if (M->m_isLib) {
      DoTcl("BlockList::addPart %s",M->m_name);
    } else {
      char *I = M->m_protIntf ? "I" : "";
      char *D = M->m_protData ? "D" : "";
      char *T = (M->m_type == MT_TEXTHDL) ? "hdl" : "netlist";
      DoTcl("BlockList::add %s -locks \"%s%s\" -type %s",M->m_name,I,D,T);
    }
  }

  if (TkGate.circuit->root_mod) {
    DoTcl("BlockList::setRoot %s",TkGate.circuit->root_mod->m_name);
  }
}

/*****************************************************************************
 *
 * Flush and reconstruct the module lists.
 *
 *****************************************************************************/
void env_updateMTCircuit()
{
  int is_simulate = (TkGate.ed->major_mode == MM_SIMULATE || TkGate.ed->major_mode == MM_ANALYZE);

  DoTcl("BlockTree::flush\n");
  DoTcl("BlockList::flush\n");

  env_updateModuleList();

  if (is_simulate) {
    env_updateMT_simulate(TkGate.circuit->root_mod,0);
  } else
    env_updateMT_edit();

  DoTcl("BlockTree::rebuild");
}

/*****************************************************************************
 *
 * Insert a module into the current circuit environment and create a module
 * interface for it if it is not the top level.
 *
 *****************************************************************************/
void env_insertModule(GModuleDef *M)
{
  char *mflags = "";
  
  if (GModuleDef_isTop(M))
    mflags = "+";

  SHash_insert(TkGate.circuit->moduleTable,M->m_name,M);
}

/*****************************************************************************
 *
 * Define a new module.
 *
 * Parameters:
 *     name		Name of new module
 *     isMain		Flag to indicate if this is the top-level module
 *
 *****************************************************************************/
GModuleDef *env_defineModule(const char *name,int isMain)
{
  GModuleDef *M;

  M = new_GModuleDef(name);
  if (isMain)
    TkGate.circuit->root_mod = M;

  env_insertModule(M);
  if (!GModuleDef_isTop(M)) {
    modint_setInterface(M,0);
  }

  SetModified(MF_MODULE);

  return M;
}

GModuleDef *env_findAdd(const char *name,int isMain)
{
  GModuleDef *M;

  if (!(M = env_findModule(name))) {
    M = env_defineModule(name,isMain);
  }
  return M;
}

GCElement *env_getInterface(GCElement *g)
{
  GModuleDef *M;

  if (GCElement_getType(g) != GC_BLOCK && 
      GCElement_getType(g) != GC_SYMBLOCK)
    return 0;

  M = env_findModule(g->u.block.moduleName);
  if (M)
    return M->m_interface;
  else
    return 0;
}

void env_checkname(GCElement *g)
{
  if (!GCElement_isModule(g)) {
    printf("Huh?  checkname gets non-block\n");
    exit(1);
  }

  env_findAdd(g->u.block.moduleName,0);
}

/*
 * Free memory from all gates in a module (used to delete a module).
 */
void freegatelist(GModuleDef *M)
{
  HashElem *gl;
  GCElement **gates;
  int i,N;

  /*  printf("freegatelist: %s\n",GModuleDef_getName(M));*/
  if (Hash_numElems(M->m_gates) > 0) {
    N = 0;
    gates = (GCElement**)ob_calloc(Hash_numElems(M->m_gates),sizeof(GCElement*),"GCElement*[]");
    for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
      GCElement *g = (GCElement*) HashElem_obj(gl);
      if (g->typeinfo->Code != GC_JOINT)
	gates[N++] = g;
    }

    for (i = 0;i < N;i++) {
      gate_delete(gates[i],M,0);
    }
    ob_free(gates);
  }

  SHash_flush(M->m_gates);
}

void freewirelist(GModuleDef *M)
{
  GWireList *l = M->m_wires;

  while (l) {
    GWireList *n = l->wl_next;
    if (l->wl_wire->driver == l->wl_wire)
      GWireNode_freenodelist(l->wl_wire->nodes);
    ob_free(l->wl_wire);
    ob_free(l);
    l = n;
  }
}

/*
   Remove all gates, wires and nets from a module.
*/
void env_clear(GModuleDef *M)
{
  HashElem *E;

  if (!M) return;

  freegatelist(M);
  freewirelist(M);

  /*
   * Nets in netlist modules will have already been deleted by the freewirelist()
   * call above.  This additional deletion scan deletes nets used by the HDL module
   * scanner.
   */
  while ((E = Hash_first(M->m_nets))) {
    GNet *n = (GNet*) HashElem_obj(E);
    delete_GNet(n);
  }
}

void env_clearmark()
{
  HashElem *E;

  for (E = Hash_first(TkGate.circuit->moduleTable);E;E = Hash_next(TkGate.circuit->moduleTable,E)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(E);
    ob_touch(M);
    M->m_used = 0;
  }
}

/*
   Mark this module and all children of it.
*/
void env_markenv(GModuleDef *M)
{
  HashElem *gl;

  if (!M->m_used) {
    ob_touch(M);
    M->m_used = 1;
    for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
      GCElement *g = HashElem_obj(gl);

      if (GCElement_isModule(g)) {
	GModuleDef *M = env_findAdd(g->u.block.moduleName,0);
	env_markenv(M);
      }
    }
  }
}

void editstate_saveOrig(EditState *es)
{
  if (es->save_x != TkGate.circuit->org_x || es->save_y != TkGate.circuit->org_y) {
    ob_touch(es);
    es->save_x = TkGate.circuit->org_x;
    es->save_y = TkGate.circuit->org_y;
  }
}

GModuleDef *env_removeModule(const char *name,int force)
{
  GModuleDef *M;

  M = (GModuleDef*) SHash_find(TkGate.circuit->moduleTable,name);
  if (!M) return 0;

  if (!force && (GModuleDef_isTop(M) || GModuleDef_isDataProtected(M))) {
    return 0;
  }

  SHash_remove(TkGate.circuit->moduleTable,name);
  modint_deleteInterface(M);

  return M;
}

/*
   Delete the specified block
*/
int env_delete(EditState *es,const char *name)
{
  GModuleDef *M;

  while (es) {
    if (strcmp(es->env->m_name,name) == 0) {
      message(1,msgLookup("err.badopendel"));			/* "Can't delete open module." */
      return -1;
    }
    es = es->parent;
  }

  /*****************************************************************************
   *
   * Do error checking
   *
   *****************************************************************************/
  M = (GModuleDef*) SHash_find(TkGate.circuit->moduleTable,name);
  if (!M) {
    message(1,msgLookup("err.nomod"),name);
    return -1;
  }
  if (GModuleDef_isTop(M)) {
    message(1,msgLookup("err.deltop"),name);
    return -1;
  }
  if (GModuleDef_isDataProtected(M)) {
    message(1,msgLookup("err.delprot"),name);
    return -1;
  }


  /*****************************************************************************
   *
   * Do the actual delete here.
   *
   *****************************************************************************/
  if (!env_removeModule(name,0))
    return -1;

  SetModified(MF_MODULE);
  FlagRedraw();
  return 0;
}


/*****************************************************************************
 *
 * Renames a module definition.
 *
 * Parameters:
 *     oldName		Old module name.
 *     newName		New module name.
 *
 *****************************************************************************/
int env_rename(const char *oldName,const char *newName)
{
  GModuleDef *M;

  M = env_findModule(oldName);
  if (!M) return ENOENT;

  if (SHash_find(TkGate.circuit->moduleTable,newName)) return EEXIST;
  SHash_remove(TkGate.circuit->moduleTable,oldName);
  SHash_insert(TkGate.circuit->moduleTable,newName,M);
  ob_touch(M);
  ob_free(M->m_name);
  M->m_name = ob_strdup(newName);
  M->m_needScan = 1;

  modint_renameInterface(M);

  if (GModuleDef_getType(M) == MT_TEXTHDL) {
    hdl_replaceName(M,M->m_name);
  }

  FlagRedraw();

  return 0;
}

/*****************************************************************************
 *
 * Copy the contents of on module definition to another.
 *
 * Parameters:
 *      src		Name of the source module for copy
 *      dst		Name of the destination module for copy
 *****************************************************************************/
void env_copy(EditState *es,const char *src,const char *dst)
{
  GModuleDef *S,*D;

  S = env_findModule(src);
  D = env_findModule(dst);

  if (!S) {
    message(1,msgLookup("err.nosrcmod"),src);		/* "Source module '%s' not found" */
    return;
  }

  if (D) {
    char buf[1024];

    sprintf(buf,msgLookup("msg.modoverwt"),dst);	/* Destination module '%s' already exists.  Overwrite? */
    DoTcl("confirmMsg \"%s\" ",buf);
    if (*TkGate.tcl->result != '1')
      return;

    env_removeModule(dst,0);
    if (env_findModule(dst))
      return;
  }
#if 0
  printf("defining block |%s|\n",dst);
#endif
  D = env_defineModule(dst,0);

  GModuleDef_copy(D,S);
}

void editstate_Init(EditState *es)
{
  ob_touch(es);
  es->isInterface = 0;
  es->save_x = es->save_y = 0;
  es->parent = 0;
  es->env = 0;
  es->smod = 0;
  es->inst = 0;
}

EditState *new_EditState()
{
  EditState *es;
    
  es = (EditState*) ob_malloc(sizeof(EditState),"EditState");
    
  editstate_Init(es);
    
  return es;
}

void delete_editstate(EditState *es)
{
  ob_free(es);
}

void editstate_update(EditState *es)
{
#if 0
  ob_touch(TkGate.circuit);
#endif

#if 1
  /* TkGate.ed not subject to undo */
  TkGate.ed->min_x = 0x7fffffff;
  TkGate.ed->min_y = 0x7fffffff;
  TkGate.ed->max_x = -0x7fffffff;
  TkGate.ed->max_y = -0x7fffffff;
#endif

  mark_redraw();

  if (es->clip.isActive) {
    XRectangle cr;
    cr.x = es->clip.xmin;
    cr.y = es->clip.ymin;
    cr.width = es->clip.xmax - es->clip.xmin;
    cr.height = es->clip.ymax - es->clip.ymin;
    XSetClipRectangles(TkGate.D,TkGate.instGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.moduleGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.modportGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.frameGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.commentGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.hyperlinkGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.wireGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.busGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.selWireGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.selBusGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.toolGC,0,0,&cr,1,YXSorted);
    XSetClipRectangles(TkGate.D,TkGate.cpathGC,0,0,&cr,1,YXSorted);
    if (TkGate.japaneseMode) {
      XSetClipRectangles(TkGate.D,TkGate.kanjiGC,0,0,&cr,1,YXSorted);
    }
  }

  gate_displayall(es->env);

  /*
   * Happens only if the module is empty.
   */
  if (TkGate.ed->min_x > TkGate.ed->max_x)
    TkGate.ed->min_x = TkGate.ed->max_x = TkGate.ed->min_y = TkGate.ed->max_y = 0;


  scrollbar_update();

  /*
   * Draw special mode-specific items 
   */
  switch (tkgate_currentMode()) {
  case MM_SIMULATE :
    SimInterface_drawProbes(&TkGate.circuit->simulator);
    break;
  default :
    break;
  }


  if (TkGate.ErrorMarkTimeout) {
    DrawErrorPositionMark();
    TkGate.ErrorMarkTimeout = 1;
  }

  if (es->clip.isActive) {
    XSetClipMask(TkGate.D,TkGate.instGC,None);
    XSetClipMask(TkGate.D,TkGate.moduleGC,None);
    XSetClipMask(TkGate.D,TkGate.modportGC,None);
    XSetClipMask(TkGate.D,TkGate.frameGC,None);
    XSetClipMask(TkGate.D,TkGate.commentGC,None);
    XSetClipMask(TkGate.D,TkGate.hyperlinkGC,None);
    XSetClipMask(TkGate.D,TkGate.wireGC,None);
    XSetClipMask(TkGate.D,TkGate.busGC,None);
    XSetClipMask(TkGate.D,TkGate.selWireGC,None);
    XSetClipMask(TkGate.D,TkGate.selBusGC,None);
    XSetClipMask(TkGate.D,TkGate.toolGC,None);
    XSetClipMask(TkGate.D,TkGate.cpathGC,None);
    if (TkGate.japaneseMode) {
      XSetClipMask(TkGate.D,TkGate.kanjiGC,None);
    }
  }
}

void editstate_fullUpdate(EditState *es)
{
#if DEBUG_REDISPLAY
  printf("editstate_fullUpdate(*)\n");
#endif
  wm_GetDimensions(&TkGate.width,&TkGate.height);
  XClearWindow(TkGate.D,TkGate.W);
  mark_flush();

  if (!es) return;

  if (es->env == TkGate.circuit->mid_display) {
    modint_center();
  }

#if 0
  /* Not needed */
  ob_touch(es);
#endif

  ob_begin_framef("-SaveOrig",FF_TRANSPARENT|FF_BACKGROUND);
  editstate_saveOrig(es);
  ob_touch(es);
  es->clip.isActive = 0;
  es->clip.xmin = 0;
  es->clip.ymin = 0;
  es->clip.xmax = TkGate.width;
  es->clip.ymax = TkGate.height;
  ob_end_frame();
  editstate_update(es);
}

/*
   Do a partial screen update.
*/
void editstate_regionUpdate(EditState *es,int xmin,int ymin,int xmax,int ymax)
{
#if DEBUG_REDISPLAY
  printf("editstate_regionUpdate(*,%d,%d,%d,%d)\n",xmin,ymin,xmax,ymax);
#endif

  wm_GetDimensions(&TkGate.width,&TkGate.height);
  mark_hide();
  XClearArea(TkGate.D,TkGate.W,xmin,ymin,xmax-xmin,ymax-ymin,False);

  if (!es) return;

#if 0
  /* Not needed */
  ob_touch(es);
#endif
  editstate_saveOrig(es);
  es->clip.isActive = 1;
  es->clip.xmin = xmin;
  es->clip.ymin = ymin;
  es->clip.xmax = xmax;
  es->clip.ymax = ymax;
  
  editstate_update(es);
}

/*
 * Set the tkg_currentPath and tkg_currentModule variables
 */
void editstate_displayPathString(EditState *cur_es)
{
  EditState *estack[MAX_STACKDEPTH];
  EditState *es;
  char path[STRMAX],*p;
  int esp,i;

  es = cur_es;
  for (esp = 0;esp < MAX_STACKDEPTH && es;es = es->parent)
    estack[esp++] = es;

  p = path;
  sprintf(p,"/");
  p = p + strlen(p);

  DoTcl("catch { unset tkg_pathInstance }");
  for (i = esp-1;i >= 0;i--) {
    if (i != esp-1) *p++ = '/';
    if (estack[i]->inst) {
      DoTcl("set ::tkg_pathInstance(%s) \" (%s)\"",estack[i]->env->m_name,estack[i]->inst->ename);
    } else {
      DoTcl("set ::tkg_pathInstance(%s) \"\"",estack[i]->env->m_name);
    }
    strcpy(p,estack[i]->env->m_name);
    p = p + strlen(p);
  }

  if (TkGate.tcl) {
    Tcl_SetVar(TkGate.tcl,"tkg_currentPath",path,TCL_GLOBAL_ONLY); 
    Tcl_SetVar(TkGate.tcl,"tkg_currentModule",cur_es->env->m_name,TCL_GLOBAL_ONLY);
  }
}

/*****************************************************************************
 *
 * Do any special processing when an editstate becomes active
 *
 *****************************************************************************/
void editstate_setCurrent(EditState *es)
{
  GModuleDef_listNets(es->env);
  GModuleDef_listPorts(es->env);
  editstate_displayPathString(es);

  if (tkgate_currentMode() == MM_SIMULATE && es && es->smod)
    SimInterface_updateNetlistProbes(es->smod);
}

/*****************************************************************************
 *  Check for a module along the specified path.  Returns
 *  '1' on success, '0' on failure.
 *
 *  Two types of paths are recognized:
 *
 *     mod:inst			A module name and an instance in the module
 *     inst1.inst2.inst3	A path of instances from the root module all
 *				but the last instance must be a module instance.
 *****************************************************************************/
int editstate_checkPath(EditState **es,const char *path)
{
  GModuleDef *M = TkGate.circuit->root_mod;
  GCElement *g;
  char buf[STRMAX],*p,*q;

  strcpy(buf,path);
  p = buf;

  if ((q = strchr(p,':'))) {
    *q++ = 0;
    M = env_findModule(p);
    if (!M) return 0;
    p = q;
  } else {
    for (;;) {
      q = strchr(p,'.');
      if (q) {					/* Looking for a module instance */
	*q++ = 0;
	g = GModuleDef_findGate(M,p);
	if (!g) return 0;
	if (!GCElement_isModule(g)) return 0;

	M = env_findModule(g->u.block.moduleName);
	if (!M) return 0;
	p = q;
      } else
	break;
    }
  }

  g = GModuleDef_findGate(M,p);
  if (!g && !GModuleDef_findNet(M,p))
    return 0;

  return 1;
}

/*
  Set the current module along the specified path.  Returns
  '1' on success, '0' on failure.
 */
int editstate_setPath(EditState **es,const char *path)
{
  GModuleDef *M = TkGate.circuit->root_mod;
  GCElement *g;
  GNet *n;
  char buf[STRMAX],*p,*q;

  strcpy(buf,path);
  p = buf;

  if ((q = strchr(p,':'))) {
    *q++ = 0;
    M = env_findModule(p);
    if (!M) return 0;
    editstate_navigateToModule(es,M);
    p = q;
  } else {
    while ((*es)->parent) editstate_pop(es);

    for (;;) {
      q = strchr(p,'.');
      if (q) {					/* Looking for a module instance */
	*q++ = 0;
	g = GModuleDef_findGate(M,p);
	if (!g) return 0;
	if (!GCElement_isModule(g)) return 0;

	M = env_findModule(g->u.block.moduleName);
	if (!M) return 0;
	p = q;
	editstate_push(es,M,g);
      } else
	break;
    }
  }

  g = GModuleDef_findGate(M,p);
  n = GModuleDef_findNet(M,p);
  if (g) {
    editstate_setCurrent(TkGate.circuit->es);
    GSearchContext_setPosition(M,g,0);
  } else if (n) {
    editstate_setCurrent(TkGate.circuit->es);
    GSearchContext_setPosition(M,0,n);
  } else
    return 0;

  return 1;
}

char *editstate_getPath(EditState *es,char *buf)
{
  if (!es->parent) {
    *buf = 0;
    return buf;
  } else {
    buf = editstate_getPath(es->parent,buf);
    if (es->parent->parent)
      *buf++ = '.';
    if (es->inst)
      strcpy(buf,es->inst->ename);
    else
      strcpy(buf,"-");
    return buf + strlen(buf);
  }
}

/*****************************************************************************
 *
 * Push module M via instance g onto the module editing stack.
 *
 * Parameters
 *     *es		The current editstate (updated with new editstate).
 *     M		The module to open for editing
 *     g		Optional instance of M
 *
 *****************************************************************************/
void editstate_push(EditState **es,GModuleDef *M,GCElement *g)
{
  EditState *newes;
  GSimModule *old_sm = (es && *es) ? (*es)->smod : 0;

  mark_flush();

  newes = new_EditState();
  ob_touch(newes);

  newes->env = M;
  if (*es && ((*es)->inst || !(*es)->parent))
    newes->inst = g;

  if (g && tkgate_currentMode() == MM_SIMULATE) {
    GSimModule *sM = (GSimModule*) SHash_find((*es)->smod->children,g->ename);
    newes->smod = sM;
  }

  if (*es) {
    ob_touch(*es);
    editstate_saveOrig(*es);
  }

  newes->parent = (*es);
  *es = newes;

  sel_clear(*es,1);
  TkGate_clearSelection();

  if (tkgate_currentMode() == MM_SIMULATE && es && *es)
    SimInterface_changeCurrentModule((*es)->smod,old_sm);

  TkGate_setOrigin(0,0);
}

void editstate_pop(EditState **es)
{
  EditState *newes;
  GSimModule *old_sm = (es && *es) ? (*es)->smod : 0;

#if 0
  if (tkgate_currentMode() == MM_SIMULATE && !(*es)->parent) return;
#endif

  if ((*es)->isInterface) {
    modint_update(*es);
  }

  if (!(newes = (*es)->parent)) {
    *es = 0;
    return;
  }

  sel_clear(*es,1);
  TkGate_clearSelection();
  TkGate_setOrigin(newes->save_x,newes->save_y);
  mark_flush();
  delete_editstate(*es);
  *es = newes;

  if (tkgate_currentMode() == MM_SIMULATE && es && *es)
    SimInterface_changeCurrentModule((*es)->smod,old_sm);
}

void editstate_navigateToModule(EditState **es,GModuleDef *M)
{
  EditState *xes;

  for (xes = *es;xes && xes->env != M;xes = xes->parent);

  if (xes) {
    /* Move up to block */
    while ((*es)->env != M)
      editstate_pop(es);
  } else {
    /* Push down to block */
    if (!(*es)->inst)
      editstate_pop(es);
    editstate_push(es,M,0);
  }
}

void editstate_makeRootAtTop(EditState **es)
{
  EditState *pes;
  int need_reroot = 0;

  EditState_unselectAll(*es);

  pes = *es;
  while (pes->parent) {
    if (!pes->inst) {
      need_reroot = 1;
      break;
    }
    pes = pes->parent;
  }

  if (pes->env != TkGate.circuit->root_mod)
    need_reroot = 1;

  if (need_reroot) {
    while (*es)
      editstate_pop(es);
    editstate_push(es,TkGate.circuit->root_mod,0);
    editstate_setCurrent(*es);
    ClearErrorMark();
    net_unselect(0);
    FlagRedraw();
  }

}

/*****************************************************************************
 *
 * Delete all modules in the current circuit except library modules.
 *
 *****************************************************************************/
void editstate_flushModules(EditState **es)
{
  HashElem *E;
  NHash *lmhash = new_NHash();

  sel_clear(*es,1);

  while (*es)
    editstate_pop(es);


  TkGate.circuit->root_mod = 0;

  /*  printf("**begin flushModules xes=%x\n",TkGate.circuit->es);*/

  for (E = Hash_first(TkGate.circuit->moduleTable);E; E = Hash_next(TkGate.circuit->moduleTable,E)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(E);
    /*printf("  deleting module: %s\n",GModuleDef_getName(M));*/
    if (M->m_isLib) {
      NHash_insert(lmhash,(int)M,M);
    } else {
      /*printf("  deleting interface: %s %p\n",GModuleDef_getName(M),M->m_interface);*/
      modint_deleteInterface(M);
      delete_GModuleDef(M);
    }
  }
  SHash_flush(TkGate.circuit->moduleTable);

#if 0
  DoTcl("tkg_blockListClear\n");
#endif
  DoTcl("gat_setCircProp -clear");

  for (E = Hash_first(lmhash);E;E = Hash_next(lmhash,E)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(E);
    SHash_insert(TkGate.circuit->moduleTable,M->m_name,M);
  }

  delete_NHash(lmhash);

  /* i don't think we need this.  It cause trouble by deleting interfaces for
   * library modules we are retaining.
   *
   *  modint_flush();
   *  printf("**end flushModules\n");
   */
}

/*****************************************************************************
 *
 * Called before and after an undo or redo to synchronize the tcl/tk interface
 * elements with the new internal state.
 *
 *****************************************************************************/
void undo_sync(int is_after)
{
  char buf[STRMAX];
  static int old_mode;

  if (!is_after) {
    /* BEFORE UNDO/REDO */
    old_mode = EditState_getMode();
  } else {
    /* AFTER UNDO/REDO */

    /*
     * Update list of nets/ports
     */
    editstate_setCurrent(TkGate.circuit->es);

    /*
     * Update tcl-side "mode" variable
     */
    sprintf(buf,"%d",EditState_getMode());
    Tcl_SetVar(TkGate.tcl,"mode",buf,TCL_GLOBAL_ONLY);

    /*
     * Update tcl-side "rot" variable
     */
    sprintf(buf,"%d",EditState_getRotation());
    Tcl_SetVar(TkGate.tcl,"rot",buf,TCL_GLOBAL_ONLY);

    /*
     * Reset correct cursor for mode.
     */
    if (old_mode != EditState_getMode())
      setEditCursor(EditState_getMode());

    if (editstate_isInterfaceMode()) {
      DoTcl("IPanel::undoSync");
    }

    DoTcl("HdlEditor::undoSync");
    DoTcl("Breakpoint::undoSync");

    SetModified(MF_MODULE|MF_SYNCONLY);
  }

  UpdateModifiedIndicator();
}

/*****************************************************************************
 *
 * Returns non-zero if we are in interface editing mode.
 *
 *****************************************************************************/
int editstate_isInterfaceMode()
{
  return TkGate.circuit->es && TkGate.circuit->es->isInterface;
}

/*****************************************************************************
 *
 * Returns a INTFMODE_ value to indicate which interface sub-mode we are in.
 *
 *****************************************************************************/
int editstate_getInterfaceMode()
{
  if (!TkGate.circuit->es) return INTFMODE_NONE;
  if (!TkGate.circuit->es->isInterface) return INTFMODE_NONE;

  if (TkGate.circuit->es->env == TkGate.circuit->mid_display)
    return INTFMODE_SINGLE;
  else
    return INTFMODE_ALL;
}

/*****************************************************************************
 *
 * Move the edit window the the nth error in the error list.
 *
 * Parameter:
 *      n		Index of error to move to
 *      es		Current edit state
 *
 * Returns:		New edit state
 *
 *****************************************************************************/
EditState *EditState_moveToError(int n,EditState *es)
{
  GError *E;

  if (!TkGate.errl->errors) {
    message(0,msgLookup("err.misserr"));
    return es;
  }

  E = TkGate.errl->errors[n];
  return Error_open(E,es);
}

void EditState_setMode(int mode)
{
  ob_touch(TkGate.circuit);
  TkGate.circuit->mode = mode;
}


void EditState_setRotation(int rot)
{
  ob_touch(TkGate.circuit);
  TkGate.circuit->rot = rot;
}

