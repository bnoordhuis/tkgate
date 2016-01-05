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

    Last edit by hansen on Wed Mar 18 05:13:57 2009
****************************************************************************/
/*
  Functions for manageing the multi-gate selection and cut buffer.

  We have separate handling for single-gate and multi-gate selection
  mainly due to historical baggage.  One day, I will merge them into
  a single selection


*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pwd.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include "tkgate.h"

void Comment_addLine(GCElement *g,const char *text);

GSelection *new_GSelection()
{
  GSelection *S = (GSelection*) ob_malloc(sizeof(GSelection),"GSelection");

  ob_touch(S);
  S->s_gates = new_SHash();
  S->s_wires = new_NHash();
  S->s_edgeWires = new_NHash();
  S->s_hasAnchored = 0;

  return S;
}

void delete_GSelection(GSelection *S)
{
  delete_SHash(S->s_gates);
  delete_NHash(S->s_wires);
  delete_NHash(S->s_edgeWires);
  ob_free(S);
}

GCutBuffer *new_GCutBuffer()
{
  GCutBuffer *C = (GCutBuffer*) ob_malloc(sizeof(GCutBuffer),"GCutBuffer");

  ob_touch(C);
  C->cb_minx = C->cb_maxx = C->cb_miny = C->cb_maxy = 0;
  C->cb_lastTarget = 0;
  C->cb_dx = C->cb_dy = 0;
  C->cb_buf = new_GModuleDef("<cut-buffer>");

  return C;
}

/*
 * Compute the bounding box of a sub-circuit defined by the hash table of gates and wires.
 */
static void subcircuitBBX(SHash *ghash,NHash *whash,GWireList *wires,
			  int *minX,int *minY,int *maxX,int *maxY,int append)
{
  HashElem *E;
  GWireList *wl;
  int is_first = !append;

  /*
   * This will only be used if this is not an append, and there are no gates or wires.
   */
  if (is_first) {
    *minX = 0;
    *maxX = 0;
    *minY = 0;
    *maxY = 0;
  }

  if (ghash) {
    for (E = Hash_first(ghash);E;E = Hash_next(ghash,E)) {
      GCElement *g = (GCElement *) HashElem_obj(E);
      int MinX,MinY,MaxX,MaxY;

      gate_getbbx(g,TD_X11,&MinX,&MinY,&MaxX,&MaxY);

      if (is_first || MinX < *minX) *minX = MinX;
      if (is_first || MaxX > *maxX) *maxX = MaxX;
      if (is_first || MinY < *minY) *minY = MinY;
      if (is_first || MaxY > *maxY) *maxY = MaxY;
      is_first = 0;

    }
  }

  if (whash) {
    for (E = Hash_first(whash);E;E = Hash_next(whash,E)) {
      GWire *w = (GWire *) HashElem_obj(E);
      GWireNode *n;

      w = w->driver;
      for (n = w->nodes;n;n = n->out) {
	int x = n->x;
	int y = n->y;

	if (is_first || x < *minX) *minX = x;
	if (is_first || x > *maxX) *maxX = x;
	if (is_first || y < *minY) *minY = y;
	if (is_first || y > *maxY) *maxY = y;
	is_first = 0;
      }
    }
  }

  for (wl = wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    GWireNode *n;

    w = w->driver;
    for (n = w->nodes;n;n = n->out) {
      int x = n->x;
      int y = n->y;

      if (is_first || x < *minX) *minX = x;
      if (is_first || x > *maxX) *maxX = x;
      if (is_first || y < *minY) *minY = y;
      if (is_first || y > *maxY) *maxY = y;
      is_first = 0;
    }
  }

}

/*
 * Compute the bounding box of the cut buffer.
 */
void GCutBuffer_computeBounds(GCutBuffer *C)
{
  subcircuitBBX(C->cb_buf->m_gates,0, C->cb_buf->m_wires,
		&C->cb_minx, &C->cb_miny, &C->cb_maxx, &C->cb_maxy, 0);

  C->cb_ctrx = (C->cb_minx + C->cb_maxx)/2;
  C->cb_ctry = (C->cb_miny + C->cb_maxy)/2;
}

void delete_GCutBuffer(GCutBuffer *C)
{
  delete_GModuleDef(C->cb_buf);
  ob_free(C);
}

/*
 * Change the state of menu entries and toolbar icons depending on whether we
 * can cut and/or paste.
 */
void sel_updateMenuState()
{
  static char *states[] = {"disabled","normal"};
  int paste_ok = TkGate.circuit->cut_buffer != 0;
  int sel_ok;

  if (!TkGate.tcl) return;


  if (TkGate.circuit && TkGate.circuit->es && hdl_isactive) {  /* GModuleDef_getType(TkGate.circuit->es->env) == MT_TEXTHDL*/
    DoTcl("HdlEditor::isselection");
    sel_ok = (*TkGate.tcl->result == '1');
  } else {
    sel_ok = (TkGate.circuit->select != 0) || (TkGate.circuit->mg_selection != 0);
  }

  DoTcl("Menu::setFlags %s C %s P",(sel_ok ? "-set" : "-clear"),(paste_ok ? "-set" : "-clear"));

  DoTcl("ToolBar::toolConfigure Edit cut -state %s",states[sel_ok]);
  DoTcl("ToolBar::toolConfigure Edit copy -state %s",states[sel_ok]);
  DoTcl("ToolBar::toolConfigure Edit paste -state %s",states[paste_ok]);

  DoTcl("set ::tkg_cutBufferActive %d",paste_ok);
  DoTcl("set ::tkg_selectionActive %d",sel_ok);
}

int sel_isSelGate(GCElement *g)
{
  if (!TkGate.circuit->mg_selection)
    return 0;

  if (SHash_find(TkGate.circuit->mg_selection->s_gates,g->ename))
    return 1;
  else
    return 0;
}


int sel_num(EditState *es)
{
  GSelection *S = TkGate.circuit->mg_selection;

  if (!S) return 0;

  return Hash_numElems(S->s_gates);
}

static void sel_addGate(EditState *es,GCElement *g,int doDraw)
{
  GSelection *S = TkGate.circuit->mg_selection;

  SHash_insert(S->s_gates,g->ename,g);

  ob_touch(S);
  ob_touch(g);

  if (g->anchored)
    S->s_hasAnchored = 1;

  if (doDraw) gate_draw(g,GD_NOWIRE);
  g->selected = 1; 
  if (doDraw) gate_draw(g,GD_NOWIRE);
}

int sel_refinish(EditState *es)
{
  if (TkGate.circuit->mg_selection) {
    NHash_flush(TkGate.circuit->mg_selection->s_wires);
    NHash_flush(TkGate.circuit->mg_selection->s_edgeWires);
    sel_finish(es);
  }
  return 0;
}

/*
 * Finish up making a selection.  Add the wires to the hash tables.
 */
int sel_finish(EditState *es)
{
  GWireList *wl;
  GSelection *S = TkGate.circuit->mg_selection;
  int n = Hash_numElems(S->s_gates);

  if (n == 0) {
    sel_updateMenuState();
    return 0;
  }

  ob_touch(TkGate.circuit);

  if (n == 1) {
    GCElement *g = (GCElement*) HashElem_obj(Hash_first(TkGate.circuit->mg_selection->s_gates));
    TkGate.circuit->select = g;
  } else
    TkGate.circuit->select = 0;

  for (wl = es->env->m_wires;wl;wl = wl->wl_next) {
    GWire *w1 = wl->wl_wire;
    GWire *w2 = w1->driver;

    if (w1 == w2) continue;	/* Process each each wire only once */

    if (!w1->gate || !SHash_find(S->s_gates,w1->gate->ename)) 
      w1 = 0;

    if (!w2->gate || !SHash_find(S->s_gates,w2->gate->ename)) 
      w2 = 0;

    if (w1 && w2) {
      NHash_insert(S->s_wires,(int)w2,w2);
    } else if (w1) {
      NHash_insert(S->s_edgeWires,(int)w1,w1);
    } else if (w2) {
      NHash_insert(S->s_edgeWires,(int)w2,w2);
    }
  }

  sel_updateMenuState();
  return n;
}

void sel_clear(EditState *es,int doDraw)
{
  ob_touch(TkGate.circuit);

  if (TkGate.circuit->mg_selection) {
    HashElem *gl;
    SHash *H = TkGate.circuit->mg_selection->s_gates;

    for (gl = Hash_first(H);gl;gl = Hash_next(H,gl)) {
      GCElement *g = (GCElement*) HashElem_obj(gl);
      ob_touch(g);
      if (doDraw)
	gate_draw(g,GD_NOWIRE);
      g->selected = 0;
      if (doDraw)
	gate_draw(g,GD_NOWIRE);
    }

    delete_GSelection(TkGate.circuit->mg_selection);
    TkGate.circuit->mg_selection = 0;
  }

  TkGate.circuit->select = 0;

  sel_updateMenuState();
}

void sel_interfaceReset(EditState *es)
{
  if (TkGate.circuit->mg_selection) {
    HashElem *E;
    SHash *H = TkGate.circuit->mg_selection->s_gates;

    for (E = Hash_first(H);E;E = Hash_next(H,E)) {
      GCElement *g = (GCElement*) HashElem_obj(E);
      modint_reset(es,g);
    }

    sel_clear(es,1);
  }
}

int sel_select(EditState *es)
{
  int x = TkGate.ed->sx;
  int y = TkGate.ed->sy;
  int width = TkGate.ed->tx-TkGate.ed->sx;
  int height = TkGate.ed->ty-TkGate.ed->sy;
  HashElem *gl;

  ob_touch(TkGate.circuit);

  if (!TkGate.circuit->mg_selection)
    TkGate.circuit->mg_selection = new_GSelection();
  else {
    NHash_flush(TkGate.circuit->mg_selection->s_wires);
    NHash_flush(TkGate.circuit->mg_selection->s_edgeWires);
  }

  if (width < 0) {
    width = -width;
    x = x - width;
  } 
  if (height < 0) {
    height = -height;
    y = y - height;
  } 

  for (gl = Hash_first(es->env->m_gates);gl;gl = Hash_next(es->env->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);
    int gx,gy;

    gx = g->xpos;
    gy = g->ypos;
    if (g->typeinfo->Code == GC_BLOCK) {
      gx += g->u.block.gwidth/2;
      gy += g->u.block.gheight/2;
    }

    if (gx >= x && gx <= x+width && gy >= y && gy <=y+height)
      sel_addGate(es,g,1);
  }

  return sel_finish(es);
}

/*
 * Add an additional gate to the selection.
 */
void sel_appendGate(EditState *es,GCElement *g,int doDraw)
{
  ob_touch(TkGate.circuit);

  if (!TkGate.circuit->mg_selection)
    TkGate.circuit->mg_selection = new_GSelection();

  NHash_flush(TkGate.circuit->mg_selection->s_wires);
  NHash_flush(TkGate.circuit->mg_selection->s_edgeWires);

  sel_addGate(es,g,doDraw);
}

void sel_unselectGate(EditState *es,GCElement *g)
{
  GSelection *S = TkGate.circuit->mg_selection;

  if (!S) return;

  SHash_remove(S->s_gates,g->ename);
  g->selected = 0;
  
  if (Hash_numElems(S->s_gates) == 0)
    sel_clear(es,1);

  sel_refinish(es);
}


int sel_selectMarked(EditState *es)
{
  HashElem *gl;

  ob_touch(TkGate.circuit);
  TkGate.circuit->mg_selection = new_GSelection();

  for (gl = Hash_first(es->env->m_gates);gl;gl = Hash_next(es->env->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);
    if (g->selected)
      sel_addGate(es,g,1);
  }
  return sel_finish(es);
}

int sel_selectAll(EditState *es)
{
  HashElem *gl;

  ob_touch(TkGate.circuit);

  if (!TkGate.circuit->mg_selection)
    TkGate.circuit->mg_selection = new_GSelection();

  for (gl = Hash_first(es->env->m_gates);gl;gl = Hash_next(es->env->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);
    sel_addGate(es,g,1);
  }

  return sel_finish(es);
}

/*
 * Rotate the selection.
 */
void sel_rotate(EditState *es,int rdir)
{
  int minX, minY, maxX, maxY, centX, centY, newcentX, newcentY;
  GSelection *S = TkGate.circuit->mg_selection;
  HashElem *E;

  if (!S) return;

  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));
    return;
  }
  if (S->s_hasAnchored) {
    message(0,msgLookup("err.gatanchor"));		/* Gate(s) are anchored and can not be moved. */
    return;
  }

  /*
   * If there are edge wires, make sure that non are attached to anything.
   */
  if (Hash_numElems(S->s_edgeWires) > 0) {
    int ok = 1;

    for (E = Hash_first(S->s_edgeWires);E;E = Hash_next(S->s_edgeWires,E)) {
      GWire *w = (GWire*) HashElem_obj(E);

      w = w->driver;
      if (w->gate && !w->gate->selected)
	ok = 0;

      w = wire_drivee(w);
      if (w->gate && !w->gate->selected)
	ok = 0;
    }

    if (!ok) {
      message(0,msgLookup("err.wirerot"));
      return;
    }
  }

  SetModified(MF_NET|MF_GATE);
  ob_touch(TkGate.circuit);

  subcircuitBBX(S->s_gates, S->s_wires, 0,
		&minX, &minY, &maxX, &maxY, 0);

  centX = (minX + maxX)/2;
  centY = (minY + maxY)/2;

  for (E = Hash_first(S->s_wires);E;E = Hash_next(S->s_wires,E)) {
    GWire *w = (GWire*) HashElem_obj(E);
    GWireNode *n;

    w = w->driver;
    for (n = w->nodes;n;n = n->out) {
      int x = n->x;
      int y = n->y;
      ob_touch(n);
      n->x = rotateX(x - centX,y - centY,rdir) + centX;
      n->y = rotateY(x - centX,y - centY,rdir) + centY;
    }

    ob_touch(w);
    w->orient = (w->orient + 4 + rdir) % 4;
    w = wire_drivee(w);
    ob_touch(w);
    w->orient = (w->orient + 4 + rdir) % 4;
  }
  for (E = Hash_first(S->s_edgeWires);E;E = Hash_next(S->s_edgeWires,E)) {
    GWire *w = (GWire*) HashElem_obj(E);
    GWireNode *n;

    w = w->driver;
    for (n = w->nodes;n;n = n->out) {
      int x = n->x;
      int y = n->y;
      ob_touch(n);
      n->x = rotateX(x - centX,y - centY,rdir) + centX;
      n->y = rotateY(x - centX,y - centY,rdir) + centY;
    }

    ob_touch(w);
    w->orient = (w->orient + 4 + rdir) % 4;
    w = wire_drivee(w);
    ob_touch(w);
    w->orient = (w->orient + 4 + rdir) % 4;
  }

  for (E = Hash_first(S->s_gates);E;E = Hash_next(S->s_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    (*g->typeinfo->Rotate)(g,centX,centY,rdir);
  }

  /*
   * Compute new center after rotation and nudge it back to the before
   * rotation center position.  This prevents gates from "walking" as
   * they are rotated.
   */
  subcircuitBBX(S->s_gates, S->s_wires, 0,
		&minX, &minY, &maxX, &maxY, 0);
  newcentX = (minX + maxX)/2;
  newcentY = (minY + maxY)/2;
  if (newcentX != centX || newcentY != centY) {
    sel_move(es,centX-newcentX,centY-newcentY);
  }
}

void sel_move(EditState *es,int dx,int dy)
{
  HashElem *E;
  SHash *GH;
  NHash *WH,*EWH;

  if (!TkGate.circuit->mg_selection) return;

  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));
    return;
  }
  if (TkGate.circuit->mg_selection->s_hasAnchored) {
    message(0,msgLookup("err.gatanchor"));		/* Gate(s) are anchored and can not be moved. */
    return;
  }

  SetModified(MF_NET|MF_GATE);
  ob_touch(TkGate.circuit);

  GH = TkGate.circuit->mg_selection->s_gates;
  WH = TkGate.circuit->mg_selection->s_wires;
  EWH = TkGate.circuit->mg_selection->s_edgeWires;

  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    ob_touch(g);
    g->xpos += dx;
    g->ypos += dy;

  }

  for (E = Hash_first(WH);E;E = Hash_next(WH,E)) {
    GWire *w = (GWire*) HashElem_obj(E);
    GWireNode *n;

    for (n = w->driver->nodes;n;n = n->out) {
      ob_touch(n);
      n->x += dx;
      n->y += dy;
    }
  }

  for (E = Hash_first(EWH);E;E = Hash_next(EWH,E)) {
    GWire *w = (GWire*) HashElem_obj(E);
    wire_move(w->nodes,dx,dy,FULL);
  }
}

void sel_draw(EditState *es)
{
  HashElem *E;
  SHash *GH;
  NHash *WH,*EWH;

  if (!TkGate.circuit->mg_selection) return;

  GH = TkGate.circuit->mg_selection->s_gates;
  WH = TkGate.circuit->mg_selection->s_wires;
  EWH = TkGate.circuit->mg_selection->s_edgeWires;

  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    gate_draw(g,GD_NOWIRE);
  }

  for (E = Hash_first(WH);E;E = Hash_next(WH,E)) {
    GWire *w = (GWire*) HashElem_obj(E);
    GWire_draw(w->driver);
  }

  for (E = Hash_first(EWH);E;E = Hash_next(EWH,E)) {
    GWire *w = (GWire*) HashElem_obj(E);
    GWire_draw(w->driver);
  }
}

void sel_dropFixup(EditState *es)
{
  HashElem *E;
  NHash *EWH;

  if (!TkGate.circuit->mg_selection) return;

  EWH = TkGate.circuit->mg_selection->s_edgeWires;

  for (E = Hash_first(EWH);E;E = Hash_next(EWH,E)) {
    GWire *w = (GWire*) HashElem_obj(E);

    GWire_draw(w->driver);
    GWire_snap(w->driver);
    GWire_draw(w->driver);
  }
}

void sel_copy(EditState *es)
{
  GModuleDef *m = es->env;
  GModuleDef *cbm; 

  if (TkGate.circuit->cut_buffer) delete_GCutBuffer(TkGate.circuit->cut_buffer);

  ob_touch(TkGate.circuit);

  TkGate.circuit->cut_buffer = new_GCutBuffer();
  cbm = TkGate.circuit->cut_buffer->cb_buf;

  if (hdl_isactive) {
    DoTcl("HdlEditor::dumpSelection");
    cbm->m_type = MT_TEXTHDL;
    GModuleDef_saveText(cbm, TkGate.tcl->result);
  } else {
    GModuleDef_copyInto(cbm, m, 0,0,1,0);
    GCutBuffer_computeBounds(TkGate.circuit->cut_buffer);
    TkGate.circuit->cut_buffer->cb_dx = 0;
    TkGate.circuit->cut_buffer->cb_dy = 0;
    TkGate.circuit->cut_buffer->cb_lastTarget = es->env;
  }

  sel_updateMenuState();
}

void sel_copyAppend(EditState *es)
{
  GModuleDef *m = es->env;
  GModuleDef *cbm; 

  ob_touch(TkGate.circuit);

  if (!TkGate.circuit->cut_buffer) {
    printf("bad copyappend(1)\n");
    return;
  }

  cbm = TkGate.circuit->cut_buffer->cb_buf;

  if (!hdl_isactive) {
    printf("bad copyappend(2)\n");
    return;
  }
  if (!hdl_isactive) {
    printf("bad copyappend(3)\n");
    return;
  }

  ob_touch(TkGate.circuit->cut_buffer);
  DoTcl("HdlEditor::dumpSelection");

  ob_touch(cbm);
  cbm->m_type = MT_TEXTHDL;
  GModuleDef_allocText(cbm, strlen(cbm->m_text) + strlen(TkGate.tcl->result) + 1);
  strcat(cbm->m_text, TkGate.tcl->result);

  sel_updateMenuState();
}

void sel_kill(EditState *es)
{
  GModuleDef *m = es->env;

  if (hdl_isactive) {
    DoTcl("HdlEditor::isselection2");
    if (*TkGate.tcl->result != '1') return;
    sel_copy(es);
    DoTcl("HdlEditor::doDelete 0");
  } else {
    sel_copy(es);
    sel_delete(es);
    ob_touch(TkGate.circuit->cut_buffer);
    TkGate.circuit->cut_buffer->cb_dx = 0;
    TkGate.circuit->cut_buffer->cb_dy = 0;
  }
}

void sel_killAppend(EditState *es)
{
  GModuleDef *m = es->env;


  sel_copyAppend(es);

  if (hdl_isactive) {
    DoTcl("HdlEditor::doDelete 0");
  } else {
    sel_delete(es);
    ob_touch(TkGate.circuit->cut_buffer);
    TkGate.circuit->cut_buffer->cb_dx = 0;
    TkGate.circuit->cut_buffer->cb_dy = 0;
  }
}

void sel_clearDelta()
{
  if (TkGate.circuit->cut_buffer) {
    ob_touch(TkGate.circuit->cut_buffer);
    TkGate.circuit->cut_buffer->cb_dx = 0;
    TkGate.circuit->cut_buffer->cb_dy = 0;
  }
}

/*****************************************************************************
 *
 * Yank data from the cut buffer into an HDL module.
 *
 *****************************************************************************/
void sel_hdlyank(EditState *es)
{
  GModuleDef *cbm;

  if (!TkGate.circuit->cut_buffer) return;

  cbm = TkGate.circuit->cut_buffer->cb_buf;

  if (hdl_isactive) {
    DoTclL("HdlEditor::insert",cbm->m_text,NULL);
  } else {
    char fileName[STRMAX];
    getSimTempFile(fileName);
    sel_writeToFile(fileName);
    DoTcl("HdlEditor::insertFile %s",fileName);
    unlink(fileName);
  }
}

/*****************************************************************************
 *
 * Yank data from the cut buffer into a netlist module.
 *
 *****************************************************************************/
void sel_yank(EditState *es)
{
  GModuleDef *cbm;

  if (!TkGate.circuit->cut_buffer) return;

  cbm = TkGate.circuit->cut_buffer->cb_buf;

  if (TkGate.ed->mark_vis) {
    ob_touch(TkGate.circuit->cut_buffer);
    TkGate.circuit->cut_buffer->cb_dx = TkGate.ed->tx - TkGate.circuit->cut_buffer->cb_ctrx;
    TkGate.circuit->cut_buffer->cb_dy = TkGate.ed->ty - TkGate.circuit->cut_buffer->cb_ctry;
    mark_unpost();
  }

  sel_clear(es,1);

  if (hdl_isactive) {
    GGateInfo *GI = GGateInfo_lookup("COMMENT");
    EditState **pes = &TkGate.circuit->es;
    GCElement *g;
    char *text = cbm->m_text;

    if (GI) {
      const char *argv[2];
      argv[0] = "-nodialog";
      argv[1] = "1";

      g = (*GI->MakeFunction)(pes,es->env,GI->Code,TkGate.ed->tx,TkGate.ed->ty,0,0,0,argv,2);
      if (g) {
	char *p,*q;

	p = text;
	while ((q = strchr(p,'\n'))) {
	  *q = 0;
	  Comment_addLine(g,p);
	  *q = '\n';
	  p = q+1;
	}
	Comment_addLine(g,p);


	gate_draw(g,0);
	sel_appendGate(*pes,g,1);
	sel_finish(*pes);
	ob_touch(TkGate.circuit);
	TkGate.circuit->select = g;
	scrollbar_bbx_update();
	SynchronizeInterface();
      }
    }
  } else {
    ob_touch(TkGate.circuit->cut_buffer);
    if (TkGate.circuit->cut_buffer->cb_lastTarget == es->env) {
      TkGate.circuit->cut_buffer->cb_dx += 20;
      TkGate.circuit->cut_buffer->cb_dy += 20;
    }
    TkGate.circuit->cut_buffer->cb_lastTarget = es->env;

    GModuleDef_copyInto(es->env, cbm, TkGate.circuit->cut_buffer->cb_dx,TkGate.circuit->cut_buffer->cb_dy,0,1);
    if (sel_selectMarked(es)) {
      EditState_setMode(MODE_MOVESEL);
    }

    if (GModuleDef_hasSubModules(cbm))
      SetModified(MF_NET|MF_GATE|MF_MODULE);
    else
      SetModified(MF_NET|MF_GATE);
  }

  sel_updateMenuState();
}

void sel_delete(EditState *es)
{
  if (sel_num(es) == 0) return;

  SetModified(MF_NET|MF_GATE);

  ob_touch(TkGate.circuit);

  /*
   * A single gate was selected.
   */
  if (sel_num(es) == 1) {

    if (GCElement_isModule(TkGate.circuit->select))
      SetModified(MF_MODULE);

    gate_delete(TkGate.circuit->select,es->env,1);

    delete_GSelection(TkGate.circuit->mg_selection);
    TkGate.circuit->mg_selection = 0;
    TkGate.circuit->last = TkGate.circuit->select = 0;

    return;
  }

  if (TkGate.circuit->mg_selection) {
    HashElem *gl;
    SHash *H = TkGate.circuit->mg_selection->s_gates;
    NHash *delGates = new_NHash();

    for (gl = Hash_first(H);gl;gl = Hash_next(H,gl)) {
      GCElement *g = (GCElement*) HashElem_obj(gl);
      if (g->typeinfo->Code != GC_JOINT)
	NHash_insert(delGates,(int)g,g);
    }

    for (gl = Hash_first(delGates);gl;gl = Hash_next(delGates,gl)) {
      GCElement *g = (GCElement*) HashElem_obj(gl);
      if (GCElement_isModule(g))
	SetModified(MF_MODULE);

      gate_delete(g,es->env,0);
    }

    delete_NHash(delGates);

    delete_GSelection(TkGate.circuit->mg_selection);
    TkGate.circuit->mg_selection = 0;
  }
  sel_updateMenuState();
  TkGate.circuit->last = TkGate.circuit->select = 0;

  FlagRedraw();
}

void sel_anchor(EditState *es,int isSet)
{
  HashElem *E;
  SHash *GH;

  if (!TkGate.circuit->mg_selection) return;

  ob_touch(TkGate.circuit->mg_selection);
  TkGate.circuit->mg_selection->s_hasAnchored = (isSet != 0);

  GH = TkGate.circuit->mg_selection->s_gates;
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    ob_touch(g);
    g->anchored = (isSet != 0);
  }
}

void sel_setTech(EditState *es,const char *tech)
{
  HashElem *E;
  SHash *GH;

  if (!TkGate.circuit->mg_selection) return;

  GH = TkGate.circuit->mg_selection->s_gates;
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    if (g->tech && g->typeinfo->num_delays > 0) {
      ob_free(g->tech);
      ob_touch(g);
      g->tech = ob_strdup(tech);
    }

  }
}


void sel_alignHorz(EditState *es)
{
  HashElem *E;
  SHash *GH;
  int y,n;
    
  if (!TkGate.circuit->mg_selection) return;

  n = y = 0;
  GH = TkGate.circuit->mg_selection->s_gates;
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    y += g->ypos;
    n++;
  }
  if (n < 2) return;
  y /= n;

  n = 0;
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (!g->anchored && y != g->ypos)
      gate_moveObject(g,0,y-g->ypos);
    if (g->anchored) n++;
  }
  if (n) message(0,msgLookup("err.gatanchor"));		/* Gate(s) are anchored and can not be moved. */
}

void sel_alignVert(EditState *es)
{
  HashElem *E;
  SHash *GH;
  int x,n;
    
  if (!TkGate.circuit->mg_selection) return;

  n = x = 0;
  GH = TkGate.circuit->mg_selection->s_gates;
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    x += g->xpos;
    n++;
  }
  if (n < 2) return;
  x /= n;

  n = 0;
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (!g->anchored && x != g->xpos)
      gate_moveObject(g,x-g->xpos,0);
    if (g->anchored) n++;
  }
  if (n) message(0,msgLookup("err.gatanchor"));		/* Gate(s) are anchored and can not be moved. */
}

/*****************************************************************************
 *
 * Write the contents of the selection buffer to a file.
 *
 *****************************************************************************/
int sel_writeToFile(const char *fileName)
{
  HashElem *E;
  SHash *GH;
  FILE *f;

  GH = TkGate.circuit->cut_buffer->cb_buf->m_gates;
    
  f = fopen(fileName,"w");
  if (!f) return -1;

  VerilogWriteInit();
  for (E = Hash_first(GH);E;E = Hash_next(GH,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    if (g->typeinfo->VerSave)
      (*g->typeinfo->VerSave)(f,g);
  }

  fclose(f);

  return 0;
}

