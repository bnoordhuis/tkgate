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
#ifndef __editstate_h
#define __editstate_h

/*
 * A GSearchContext is used to control a string search operation.
 */
typedef struct {
  int		mode;		/* Search mode */

  char		*target;	/* Target string */

  HashElem	*m_elem;	/* Current position in table of modules */
  HashElem	*g_elem;	/* Current position in table of gates */
  HashElem	*n_elem;	/* Current position in table of nets */
} GSearchContext;

/*****************************************************************************
 *
 * A GCutBuffer is used to implement the cut/paste buffer.  It holds anything
 * that has been put here through the cut/copy commands, and can be pasted
 * using the paste command.  The selected item can be either a group of gates
 * and wires, or it can be a block of text if cut/copied from the HDL editor.
 * When the cut buffer contains gates/wires, and a paste is done to an HDL
 * module, the items are converted to text.  When the cut buffer contains
 * text and the paste is done to a netlist module, the text is converted
 * to a comment object.
 *
 *****************************************************************************/
typedef struct {
  int		cb_minx,cb_maxx;	/* Minimum and maximum x value for cut objects */
  int		cb_miny,cb_maxy;	/* Minimum and maximum y value for cut objects */
  int		cb_ctrx,cb_ctry;	/* Center point of selected region */

  GModuleDef    *cb_lastTarget;		/* Target of the last yank operation */
  int		cb_dx,cb_dy;		/* Offset for yank operations */
  GModuleDef	*cb_buf;		/* Gates/nets represented as a module */
} GCutBuffer;

/*****************************************************************************
 *
 * A GSelection is used to keep track of the set of gates and wires that are
 * currently selected.
 *
 *****************************************************************************/
typedef struct {
  SHash		*s_gates;	/* Gates in the selection */
  SHash		*s_wires;	/* Wires between gates in s_gates */
  SHash		*s_edgeWires;	/* Wires with one end in s_gates */
  int		s_hasAnchored;	/* Selection has anchored gates */
} GSelection;

/*****************************************************************************
 *
 * An editstate is the context in which a module is edited.
 *
 *****************************************************************************/
struct EditState_str {
  GModuleDef	*env;			/* Module edited in this block */
  GCElement	*inst;			/* Module instance (if opened from instance) */
  GSimModule	*smod;			/* Corresponding simulation module */

  int save_x,save_y;			/* Saved origin position */

  unsigned isInterface : 1;		/* Set if this is the module interface context */

  /*
   * The clip region is used when updating a portion of the screen such as
   * when scrolling.
   */
  struct {
    int isActive;			/* Set if clipping is active */
    int xmin,ymin;			/* The minimum and maximum */
    int xmax,ymax;			/* coordinates of the clip region */
  } clip;

  EditState *parent;			/* The parent edit state */
};

EditState *new_EditState();
void delete_EditState(EditState*);

void editstate_saveOrig(EditState *es);
void editstate_fullUpdate(EditState *es);
void editstate_regionUpdate(EditState *es,int xmin,int ymin,int xmax,int ymax);
void editstate_setCurrent(EditState *es);
int editstate_setPath(EditState **es,const char *path);
int editstate_checkPath(EditState **es,const char *path);
void editstate_flushModules(EditState **es);
void editstate_Init(EditState *es);
void editstate_update(EditState *es);
void editstate_push(EditState **es,GModuleDef *M,GCElement *g);
void editstate_pop(EditState **es);
void editstate_navigateToModule(EditState **es,GModuleDef *M);
char *editstate_getPath(EditState *es,char *buf);
void editstate_makeRootAtTop(EditState **es);
int editstate_isInterfaceMode();
int editstate_getInterfaceMode();
EditState *EditState_moveToError(int n,EditState *es);

EditState *outoferrorblocks(EditState *es,EditState *o);

void EditState_unselectAll(EditState*);
void EditState_selectGate(EditState *es,int tx,int ty);
void EditState_unselectGate(EditState *es);

void sel_rotate(EditState*,int);
void sel_clear(EditState *es,int doDraw);
void sel_move(EditState*,int,int);
void sel_draw(EditState*);
void sel_dropFixup(EditState*);
void sel_delete(EditState*);
void sel_copy(EditState*);
void sel_copyAppend(EditState*);
void sel_kill(EditState*);
void sel_killAppend(EditState*);
void sel_hdlyank(EditState*);
void sel_yank(EditState*);
int sel_select(EditState*);
int sel_selectAll(EditState *es);
int sel_isSelGate(GCElement *g);
void sel_appendGate(EditState*,GCElement*,int);
void sel_unselectGate(EditState*,GCElement*);
int sel_num(EditState*);
void sel_updateMenuState();
int sel_refinish(EditState *es);
int sel_finish(EditState *es);
void sel_anchor(EditState *es,int);
void sel_setTech(EditState *es,const char*);
void sel_alignHorz(EditState *es);
void sel_alignVert(EditState *es);
void sel_clearDelta();
int sel_writeToFile(const char*);
void sel_interfaceReset(EditState *es);

void EditState_selectobject(EditState *es);
void EditState_dropobject(EditState *es);
void EditState_moveobject(EditState *es);

void setEditMode(EditState *es,int mode);
int modifyOK(EditState *es,unsigned flags);

GSearchContext *new_GSearchContext();
void GSearchContext_clear(GSearchContext*);
void GSearchContext_find(GSearchContext *sc,const char *target,int mode,int qual);
void GSearchContext_list(GSearchContext *sc,const char *target,int mode,int qual,const char *result);
void GSearchContext_setPosition(GModuleDef *M,GCElement *g,GNet *n);
void GSearchContext_goto(const char *spec);

GModuleDef *env_findModule(const char *n);
GModuleDef *env_findAdd(const char *name,int isMain);
int env_rename(const char *old,const char *new);
GModuleDef *env_defineModule(const char *name,int);
int env_delete(EditState*,const char*);
GModuleDef *env_removeModule(const char *name,int force);
void env_insertModule(GModuleDef *);
void env_copy(EditState *es,const char *src,const char *dst);
void env_updateMTCircuit();
GCElement *env_getInterface(GCElement *g);
void env_clear(GModuleDef *e);
void env_checkname(GCElement *g);
void env_markenv(GModuleDef *env);

#endif

