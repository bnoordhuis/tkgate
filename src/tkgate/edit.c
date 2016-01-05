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

    Last edit by hansen on Tue May  5 20:55:15 2009
****************************************************************************/
/*
    General purpose functions used by the editor.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include "tkgate.h"

int stepsize;

int did_interface_resize = 0;

int debugmode = 0;

/*****************************************************************************
 *
 * Draws a line in circuit space with appropriate adjustments for scaling
 * and scroll position.
 *
 *****************************************************************************/
void line(int x1,int y1,int x2,int y2)
{
  ZDrawLine(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(x1), ctow_y(y1), ctow_x(x2), ctow_y(y2));
}

/*****************************************************************************
 *
 * Draws a box in circuit space with appropriate adjustments for scaling
 * and scroll position.
 *
 *****************************************************************************/
void box(int x,int y,int w,int h)
{
  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(x),ctow_y(y),w,h);
}

/*****************************************************************************
 *
 * Displays, or erases a gate symbol on the screen.
 *
 *****************************************************************************/
void mk_gate(int x,int y,GGateInfo *gi,int rot,int selected)
{
  int idx;

  idx = rot;
  if (selected) idx += 4;

  Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x),ctow_y(y),gi->icon[idx]);
}

/*****************************************************************************
 *
 * Draws the mark symbol
 *
 *****************************************************************************/
void mark_draw()
{
  Icon_draw(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(TkGate.ed->mark_x),ctow_y(TkGate.ed->mark_y),Mark);
}

/*****************************************************************************
 *
 * Flush the mark state (as a result of screen being cleared)
 *
 *****************************************************************************/
void mark_flush()
{
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->mark_vis = 0;
}

/*****************************************************************************
 *
 * Temporarily hide the mark symbol
 *
 *****************************************************************************/
void mark_hide()
{
  if (TkGate.ed->mark_vis)
    mark_draw();
  mark_flush();
}

/*****************************************************************************
 *
 * Redraw mark is posted but not visible
 *
 *****************************************************************************/
void mark_redraw()
{
  if (TkGate.ed->mark_posted && !TkGate.ed->mark_vis) {
    mark_draw();
#if TOUCH_XGATE_ED
    ob_touch(TkGate.ed);
#endif
    TkGate.ed->mark_vis = 1;
  }
}

/*****************************************************************************
 *
 * Displays the mark on the screen and marks it as visible.
 *
 *****************************************************************************/
void mark_post()
{
  if (EditState_getMode() != MODE_MOVE ||
      TkGate.circuit->select ||
      TkGate.circuit->labelsel->net ||
      TkGate.circuit->mg_selection || 
      TkGate.circuit->wnsel ||
      editstate_isInterfaceMode() ||
      Hyperlink_isPending())
    return;

  if (TkGate.ed->mark_vis)
    mark_draw();

#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->mark_x = TkGate.ed->tx;
  TkGate.ed->mark_y = TkGate.ed->ty;
  mark_draw();
  TkGate.ed->mark_vis = 1;
  TkGate.ed->mark_posted = 1;
}

/******************************************************************************
 *
 * Removes the mark from the screen and marks it as not visible.
 *
 *****************************************************************************/
void mark_unpost()
{
  if (!TkGate.ed->mark_vis) return;
  mark_draw();
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->mark_vis = 0;
  TkGate.ed->mark_posted = 0;
}


void HandScroll_set(EditState *es)
{
  int x = TkGate.ed->rx;
  int y = TkGate.ed->ry;

  ob_suggest_name("Scroll");

  ob_touch(TkGate.ed);
  TkGate.ed->handScroll.orgSave_x = TkGate.circuit->org_x;
  TkGate.ed->handScroll.orgSave_y = TkGate.circuit->org_y;
  TkGate.ed->handScroll.setSave_x = x;
  TkGate.ed->handScroll.setSave_y = y;

}


void HandScroll_move(EditState *es)
{
  int sx = TkGate.ed->handScroll.setSave_x;
  int sy = TkGate.ed->handScroll.setSave_y;
  int x = TkGate.ed->rx;
  int y = TkGate.ed->ry;
  int ox = TkGate.ed->handScroll.orgSave_x;
  int oy = TkGate.ed->handScroll.orgSave_y;
  int dx = (x-sx)/TkGate.circuit->zoom_factor;
  int dy = (y-sy)/TkGate.circuit->zoom_factor;

  if (TkGate.smoothScroll) {
    TkGate.idle_ev.scroll_new_x = ox + dx;
    TkGate.idle_ev.scroll_new_y = oy + dy;
    FlagScrolling();
  } else {
    ob_touch(TkGate.circuit);
    TkGate.circuit->org_x = ox + dx;
    TkGate.circuit->org_y = oy + dy;
    FlagRedraw();
  }
}

void HandScroll_drop(EditState *es)
{
  int x = TkGate.ed->rx;
  int y = TkGate.ed->ry;

  HandScroll_move(es);
}

/*****************************************************************************
 *
 * Try to select a gate at the specified position.
 *
 *****************************************************************************/
void EditState_selectGate(EditState *es,int tx,int ty)
{
  GModuleDef *env = es->env;
  GCElement *g;

  g = gate_hit(env,tx,ty);

  if (!(TkGate.state & ControlMask))
    sel_clear(es,1);

  if (g) {
    int n;

    sel_appendGate(es,g,1);
    sel_refinish(es);

    ob_touch(TkGate.circuit);
    TkGate.circuit->last = 0;
    n = sel_num(es);
    TkGate.circuit->select = (n == 1) ? g : 0;
    if (n > 1) {
      EditState_setMode( MODE_MOVESEL);
    }
  }
}

/*****************************************************************************
 *
 * Unselect any selected gate or gates.
 *
 *****************************************************************************/
void EditState_unselectGate(EditState *es)
{
  if (TkGate.circuit->select) {
    gate_draw(TkGate.circuit->select,GD_NORMAL);
    gate_mark(TkGate.circuit->select,0);
    gate_draw(TkGate.circuit->select,GD_NORMAL);
  }
  ob_touch(TkGate.circuit);
  TkGate.circuit->last = NULL;
  TkGate.circuit->select = NULL;
}

/*****************************************************************************
 *
 * Do a "snap" operation on a wire.
 *
 *****************************************************************************/
void DoSnap(GWire *w)
{
  GWire_draw(w->driver);
  GWire_snap(w->driver);
  GWire_draw(w->driver);
}

/*****************************************************************************
 *
 * Update the bat cursor if we are in move mode.
 *
 *****************************************************************************/
void SetBatCursor()
{
  if (EditState_getMode() != MODE_MOVE) return;
  if (TkGate.circuit->wnsel) return;

  switch (TkGate.batc) {
  case 0 :
    wm_SetCursor(BATCURSOR1);
    break;
  case 1 :
    wm_SetCursor(BATCURSOR2);
    break;
  case 2 :
    wm_SetCursor(BATCURSOR3);
    break;
  case 3 :
    wm_SetCursor(BATCURSOR2);
    break;
  }
  TkGate.batc = (TkGate.batc + 1) % 4;
}

/*****************************************************************************
 *
 * Set the anchor end of rubber band line at the current position.
 *
 *****************************************************************************/
void setrubberband(EditState *es)
{
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->lx = TkGate.ed->sx = TkGate.ed->tx;
  TkGate.ed->lx = TkGate.ed->sy = TkGate.ed->ty;

  ZDrawLine(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(TkGate.ed->sx),ctow_y(TkGate.ed->sy),ctow_x(TkGate.ed->tx),ctow_y(TkGate.ed->ty));
}

/*****************************************************************************
 *
 * Move the free end of rubber band line to the current position.
 *
 *****************************************************************************/
void moverubberband(EditState *es)
{
  ZDrawLine(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(TkGate.ed->sx),ctow_y(TkGate.ed->sy),ctow_x(TkGate.ed->lx),ctow_y(TkGate.ed->ly));
  ZDrawLine(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(TkGate.ed->sx),ctow_y(TkGate.ed->sy),ctow_x(TkGate.ed->tx),ctow_y(TkGate.ed->ty));
}

/*****************************************************************************
 *
 * Set the anchor corner of a rubber band box at the current position.
 *
 *****************************************************************************/
void setRubberBox(EditState *es)
{
  int x = ctow_x(TkGate.ed->sx);
  int y = ctow_y(TkGate.ed->sy);
  int width = TkGate.ed->tx-TkGate.ed->sx;
  int height = TkGate.ed->ty-TkGate.ed->sy;

  if (width < 0) {
    width = -width;
    x = x - width;
  } 
  if (height < 0) {
    height = -height;
    y = y - height;
  } 

  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.toolGC,x,y,width,height);
}

/*****************************************************************************
 *
 * Set the free corner of a rubber band box to the current position.
 *
 *****************************************************************************/
void moveRubberBox(EditState *es)
{
  int x = ctow_x(TkGate.ed->sx);
  int y = ctow_y(TkGate.ed->sy);
  int lx = ctow_x(TkGate.ed->sx);
  int ly = ctow_y(TkGate.ed->sy);
  int width = TkGate.ed->tx-TkGate.ed->sx;
  int height = TkGate.ed->ty-TkGate.ed->sy;
  int lwidth = TkGate.ed->lx-TkGate.ed->sx;
  int lheight = TkGate.ed->ly-TkGate.ed->sy;

  if (width < 0) {
    width = -width;
    x = x - width;
  } 
  if (height < 0) {
    height = -height;
    y = y - height;
  } 
  if (lwidth < 0) {
    lwidth = -lwidth;
    lx = lx - lwidth;
  } 
  if (lheight < 0) {
    lheight = -lheight;
    ly = ly - lheight;
  } 

  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.toolGC,x,y,width,height);
  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.toolGC,lx,ly,lwidth,lheight);
}

/*****************************************************************************
 *
 * Move selected objects in response to mouse movement.
 *
 * Parameters:
 *
 *      es		Editstate in which to perform operation
 *
 * Move selected object(s) to the new cursor position. The effect is mode 
 * dependent and may result in moving either an object, a selection, a rubber
 * band line/box depending on context.
 *
 *****************************************************************************/
void EditState_moveobject(EditState *es)
{
  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));
    return;
  }

  if (TkGate.circuit->labelsel->net) {
    GrabbedLabel_draw(TkGate.ed->lx,TkGate.ed->ly);
    GrabbedLabel_draw(TkGate.ed->tx,TkGate.ed->ty);
    return;
  }

  switch (EditState_getMode()) {
  case MODE_MOVENULL :
    break;
  case MODE_MOVESEL :
    if (TkGate.state & ControlMask) {
      EditState_setMode( MODE_MOVENULL);
      wm_SetCursor(ARROWCURSOR);
      return;
    }

    ob_suggest_name("MultiMove");
    sel_draw(es);
    sel_move(es,TkGate.ed->tx-TkGate.ed->lx,TkGate.ed->ty-TkGate.ed->ly);
    sel_draw(es);
    break;
  case MODE_MOVE :
    mark_unpost();

    if (TkGate.circuit->wnsel) {
      if (!TkGate.circuit->wsel)
	printf("Huh? wsel is NULL\n");
      ob_suggest_name("MoveWire");
      GWire_draw(TkGate.circuit->wsel);
      wire_move(TkGate.circuit->wnsel,
		TkGate.ed->tx-TkGate.ed->lx,TkGate.ed->ty-TkGate.ed->ly,
		TkGate.circuit->wnsel->stype);
      GWire_draw(TkGate.circuit->wsel);
    } else {
      if (TkGate.circuit->select && TkGate.circuit->select->typeinfo->Flags.special_move) {
	if (!TkGate.circuit->select->anchored) {
	  sel_draw(es);
	  ob_suggest_name("MoveGate");
	  if (es->env->m_isSpecial) SetModified(MF_BEGINSPEC);
	  gate_move(TkGate.circuit->select,TkGate.ed->tx-TkGate.ed->lx,TkGate.ed->ty-TkGate.ed->ly);
	  if (es->env->m_isSpecial) SetModified(MF_ENDSPEC);
	  sel_draw(es);
	} else
	  message(0,msgLookup("err.gatanchor"));		/* "Gate is anchored and can not be moved." */

      } else if (TkGate.circuit->mg_selection) {
	if (!TkGate.circuit->mg_selection->s_hasAnchored) {
	  sel_draw(es);
	  ob_suggest_name("MoveGate");
	  sel_move(es,TkGate.ed->tx-TkGate.ed->lx,TkGate.ed->ty-TkGate.ed->ly);
	  sel_draw(es);
	} else
	  message(0,msgLookup("err.gatanchor"));		/* "Gate is anchored and can not be moved." */
      }
    }
    if (!TkGate.circuit->select && !TkGate.circuit->wnsel && !editstate_isInterfaceMode()) {
      int dx = TkGate.ed->tx-TkGate.ed->sx;
      int dy = TkGate.ed->ty-TkGate.ed->sy;

      if (dx*dx+dy*dy > SELMODETHRESH) {
	EditState_setMode( MODE_MAKESEL);
	wm_SetCursor(ARROWCURSOR);
	setRubberBox(es);
      }
    }

    mark_post();
    break;
  case MODE_MAKESEL :
    moveRubberBox(es);
    break;
  case MODE_REPLICATE :
    if (TkGate.circuit->select) {
      gate_hashrepline(es,TkGate.ed->lx,TkGate.ed->ly);
      gate_hashrepline(es,TkGate.ed->tx,TkGate.ed->ty);
      moverubberband(es);
    }
    break;
  }
}

/*****************************************************************************
 *
 * Respond to a wire being dropped on a gate.  Special things happen when the
 * gate is a joint or a block.
 *
 *****************************************************************************/
void DropWireOnGate(EditState *es,GCElement *g)
{
  extern int blockDropConnect;
  GWire *w;
  int p;

  w = TkGate.circuit->wnsel->end;

  /*
   * Wire is already connected to something.
   */
  if (w->gate) return;

  GWire_draw(TkGate.circuit->wsel->driver);


  wire_deletezeronodes(TkGate.circuit->wnsel->in ? w->driver->nodes : w->nodes);
  ob_touch(TkGate.circuit);
  TkGate.circuit->wnsel = w->nodes;
  
  switch (g->typeinfo->Code) {
  case GC_JOINT :
    joint_connect(g,TkGate.circuit->wnsel);
    GWire_draw(TkGate.circuit->wsel->driver);
    wire_finalizeNet(TkGate.circuit->wsel);
    break;
  case GC_BLOCK :
    if (blockDropConnect) {
      if (block_isIntfProtected(g)) {
	message(0,msgLookup("err.protintf"),g->u.block.moduleName);
      } else {
	p = block_connect(g,TkGate.circuit->wnsel,IN);
	if (p >= 0) {
	  GWire_snap(TkGate.circuit->wsel->driver);
	  GWire_draw(TkGate.circuit->wsel->driver);
	  DrawPinIOMark(TkGate.circuit->wnsel->end,p,TkGate.circuit->wnsel->in ? IN : OUT,IODT_PLAIN);
	  block_setPortName(g,TkGate.circuit->wnsel->end,es);
	  wire_finalizeNet(TkGate.circuit->wnsel->end);
	}
	break;
      }
    }
    /* fall through */
  default :
    GWire_snap(w->driver);
    wire_finalizeNet(w);
    GWire_draw(w->driver);
    break;
  }
}

/*****************************************************************************
 *
 * Hit an object at a location and return either the gate in g, or the wire
 * in w and n.  gbais is an amount to bias toward selecting a gate. 
 *
 *****************************************************************************/
static void hit_choose(int x,int y,int gbias,GCElement **g,GWire **w,GWireNode **n)
{
  int dg, dw, dn;

  dg = dw = dn = NOHIT;

  if (g && *g) {
    int dx = (*g)->xpos - x;
    int dy = (*g)->ypos - y;
    dg = dx*dx+dy*dy - gbias;
  }

  if (w && *w) {
    int dx = (*w)->nodes->x - x;
    int dy = (*w)->nodes->y - y;
    dw = dx*dx+dy*dy;
  }
  
  if (n && *n) {
    GWire *e1,*e2;

    int dx = (*n)->x - x;
    int dy = (*n)->y - y;

    e1 = wirenode_driver(*n);
    e2 = wirenode_drivee(*n);

    if (g && (e1->gate == *g || e2->gate == *g))
      dn = NOHIT;
    else {
      switch ((*n)->stype) {
      case FULL :
	dn = dx*dx + dy*dy;
	break;
      case VERTICAL :
	dn = dx*dx;
	break;
      case HORIZONTAL :
	dn = dy*dy;
	break;
      default :
	logError(ERL_WARN,"Unknown stype %d in hit_all.",(*n)->stype);
	break;
      }
    }
  }

  if (dn <= dw && dn <= dg) {
    if (w) *w = 0;
    if (g) *g = 0;
  } else if (dw <= dn && dw <= dg) {
    if (n) *n = 0;
    if (g) *g = 0;
  } else {
    if (n) *n = 0;
    if (w) *w = 0;
  }
}

/*****************************************************************************
 *
 * Drop a wire and determine what to do from context.
 *
 *****************************************************************************/
void DropWire(EditState *es)
{
  GWire *csel = 0;
  GWireNode *nsel = 0;
  GCElement *g;
  GWire *kw1,*kw2;

  if (!TkGate.circuit->wsel) {
    printf("Huh? wsel is NULL\n");
    return;
  }

  net_unselect(1);

  if (TkGate.circuit->wnsel->end) {
    g = gate_hit(es->env,TkGate.ed->tx,TkGate.ed->ty);
    if (TkGate.circuit->wnsel->stype == FULL) {
      nsel = wire_hit_other(TkGate.circuit->wnsel->end,es->env->m_wires);
      csel = wire_endhit(TkGate.circuit->wnsel->end,es->env->m_wires);
    }

    hit_choose(TkGate.ed->tx,TkGate.ed->ty,9,&g,&csel,&nsel);

    if (g) {
      DropWireOnGate(es,g);
    } else if (csel) {
      GWire *kw1 = wire_sigroot(TkGate.circuit->wnsel->end);
      GWire *kw2 = wire_sigroot(csel);
      GWire *rw;

      if (!net_connectOK(kw1->net,kw2->net,0)) return;
      
      GNet_draw(kw1->net);
      GNet_draw(kw2->net);
      rw = wire_connect(es->env,TkGate.circuit->wnsel->end,csel);
      GNet_draw(rw->net);
      ob_touch(TkGate.circuit);	/* JPH 121703 */
      TkGate.circuit->wsel = rw;
    } else if (nsel) {
      GCElement *tap_g;
      GNet *n1,*n2;

      kw1 = wire_sigroot(TkGate.circuit->wnsel->end);
      kw2 = wire_sigroot(wirenode_driver(nsel));

      n1 = kw1->net;
      n2 = kw2->net;

      if (!net_connectOK(n1,n2,1)) return;
      if (nsel->stype != VERTICAL && nsel->stype != HORIZONTAL && n1->n_nbits != n2->n_nbits) {
	message(0,msgLookup("err.badconbitw"));	/* Connection refused because bit widths do not match. */
	return;
      }

      GNet_draw(n1);
      GNet_draw(n2);

      net_incref(n1);
      net_incref(n2);

      tap_g = join_wires(TkGate.circuit->wnsel->end,nsel,es);

      wire_finalizeNet(TkGate.circuit->wsel);

      if (n1->n_refs > 1) { 
	wire_finalizeNet(n1->n_driver);
	GNet_draw(n1);
      }
      if (n2->n_refs > 1) {
	wire_finalizeNet(n2->n_driver);
	GNet_draw(n2);
      }

      net_decref(n1);
      net_decref(n2);

      if (tap_g)
      	gate_draw(tap_g,GD_NOWIRE);
    }
  }

  if (!(TkGate.state & Mod1Mask))
    DoSnap(TkGate.circuit->wsel);
  wire_finalizeNet(TkGate.circuit->wsel);

  if (EditState_getMode() == MODE_MOVE)
    net_select(TkGate.circuit->wsel->net,1);
}

/*****************************************************************************
 *
 * Drop an object in response to a mouse button release
 *
 * Parameters:
 *      es		Edit state in which to perform operation
 *
 *
 *****************************************************************************/
void EditState_dropobject(EditState *es)
{
  DoTcl("tkg_unpostNetDelay");

  if (TkGate.circuit->labelsel->net) {
    GrabbedLabel_draw(TkGate.ed->lx,TkGate.ed->ly);
    GNet_labelClosest(TkGate.circuit->labelsel->net,TkGate.ed->tx,TkGate.ed->ty);
    GrabbedLabel_unset();
    return;
  }

  switch (EditState_getMode()) {
  case MODE_MOVENULL :
    EditState_setMode( MODE_MOVE);
    break;
  case MODE_CUT :
    if (TkGate.startrekp)
      wm_SetCursor(TREKDELETEUP);
    else
      wm_SetCursor(OPENCUTTERS);
    break;
  case MODE_INVERT :
    wm_SetCursor(INVERTUPCURSOR);
    break;
  case MODE_MOVESEL :
    sel_dropFixup(es);
    break;
  case MODE_MOVE :
    if (!modifyOK(es,0x2)) {
      if (TkGate.batp) {
	SetBatCursor();
      } else
	wm_SetCursor(ARROWCURSOR);
      return;
    }

    if (TkGate.circuit->wnsel) {
	DropWire(es);
    } else
      if (TkGate.circuit->select) {
	if (!(TkGate.state & Mod1Mask))
	  wire_snapgate(TkGate.circuit->select,1);
      }
    if (TkGate.batp) {
      SetBatCursor();
    } else
      wm_SetCursor(ARROWCURSOR);

    mark_post();

    break;
  case MODE_DELETE :
    if (TkGate.startrekp)
      wm_SetCursor(TREKDELETEUP);
    else
      wm_SetCursor(DELETEUPCURSOR);
    break;
  case MODE_SETSIZE :
    if (!modifyOK(es,0x2)) return;
    ob_suggest_name("Resize");
    setwiresize(es);
    break;
  case MODE_REPLICATE :
    if (!modifyOK(es,0x2)) return;
    ob_append_frame("Replicate");
    if (TkGate.circuit->select)
      gate_doReplication(es);
    setEditMode(es,MODE_MOVE);
    ob_end_frame();
    break;
  case MODE_MAKESEL :
    if (!modifyOK(es,0x2)) return;
    EditState_setMode( MODE_MOVE);
    setRubberBox(es);
    if (sel_select(es))
      EditState_setMode( MODE_MOVESEL);
    break;
  default :
    break;
  }
  sel_updateMenuState();
}

/*****************************************************************************
 *
 * Unselect everything
 *
 *****************************************************************************/
void EditState_unselectAll(EditState *es)
{
  ClearErrorMark();

  mark_unpost();

  if (TkGate.circuit->select) {
    EditState_unselectGate(es);
  }
  sel_clear(es,1);
  net_unselect(1);
}

/*****************************************************************************
 *
 * Sets initial wires for an object
 *
 *****************************************************************************/
void initial_wires(GModuleDef *env,GCElement *select,int invertp)
{
  int i,j;
  int N = GCElement_numPads(select);

  for (i = 0;i < N;i++)
    for (j = 0;j < GCElement_getPadNum(select,i);j++)
      wire_addToGate(select,i,env,(select->typeinfo->Pad[i].iotype == OUT && invertp));

}

/*****************************************************************************
 *
 * Create a string with formatted information about a net for debugging
 * purposes.
 *
 *****************************************************************************/
char *PrintNet(char *p,GNet *net)
{
  char *name;

  if (!net) {
    p += sprintf(p,"NetID:    *null*\n");
    return p;
  }

  name = net->n_signame ? net->n_signame : "*none*";
  p += sprintf(p,"NetID:   0x%p     Name: %s\n",(void*)net,name);
  p += sprintf(p,"  Bits: %d    Refs: %d",net->n_nbits,net->n_refs);
  if (net->n_finalized)
    p += sprintf(p,"   Finalized: ok\n");
  else
    p += sprintf(p,"   Finalized: %d/%d\n",net->n_wnum,net->n_gnum);

  p += sprintf(p,"  Flags:");
  if (net->n_mark) p += sprintf(p," mark");
  if (net->n_ionet) p += sprintf(p," ionet");
  if (GNet_getShowName(net)) p += sprintf(p," show_name");
  p += sprintf(p,"\n");

  return p;
}

/*****************************************************************************
 *
 * Create a string with formatted information about a wire end for debugging
 * purposes.
 *
 *****************************************************************************/
char *PrintWireEnd(char *p,GWire *w)
{
  GCElement *g = w->gate;

  p += sprintf(p,"Port:    %s (0x%p) {%d}\n",w->name ? w->name : "*none*",(void*)w,w->nidx);

  if (!g)
    p += sprintf(p,"Gate:    *none*\n");
  else {
    
    if (!g->typeinfo)
      p += sprintf(p,"Gate:    typeinfo is null\n");
    else if (badaddr(g->typeinfo))
      p += sprintf(p,"Gate:    typeinfo is bad address\n");
    else if (!g->typeinfo->vnames)
      p += sprintf(p,"Gate:    typeinfo name is null\n");
    else {
      int pos,n;

      posongate(w,g,&pos,&n);
      p += sprintf(p,"Gate:    %s.%s[%d]\n",g->typeinfo->name,
	     g->typeinfo->Pad[pos].Name,n);
      if (GCElement_getType(g) == GC_BLOCK)
	p += sprintf(p,"Offset:  %d/%d\n",w->offset.num,w->offset.den);
    }
  }

  p += sprintf(p,"Orient:  %d\n",w->orient);
  p += sprintf(p,"Wtype:   %d\n",w->wtype);

  if (w == (GWire*)wire_sigroot(w))
    p += sprintf(p,"DType:  root driver\n");
  else if (w == w->driver)
    p += sprintf(p,"DType:  segment driver\n");
  else
    p += sprintf(p,"DType:  segment drivee\n");

  return p;
}

/*****************************************************************************
 *
 * Create a string with formatted information about a wire node for debugging
 * purposes.
 *
 *****************************************************************************/
char *PrintWireNode(char *p,GWireNode *n)
{
  p += sprintf(p," Pos: (%d, %d)  [ss:%d il:%d/%d off:%d] %s\n",n->x,n->y,
	       n->showSize,n->isLabeled,n->labelSide,n->offset,n->end ? "*end*" : "");

  if (n->in && n->in->out != n)
    p += sprintf(p,"    n->in is wrong\n");
  if (n->out && n->out->in != n)
    p += sprintf(p,"    n->out is wrong\n");
  if (n->end && n->in && n->out)
    p += sprintf(p,"    n->end is wrong\n");

  return p;
}

/*****************************************************************************
 *
 * Handle mouse down action while in debug mode (the wrench tool).
 *
 *****************************************************************************/
void DoDebug(EditState *es)
{
  GWireNode *n;
  GWire *W = 0;
  GCElement *g;
  GGateInfo *gi;
  char msg[1024*10];
  char *p = msg;

  *msg = 0;

  if ((g = gate_hit(es->env,TkGate.ed->tx,TkGate.ed->ty))) {
    int N = GCElement_numPads(g);
    GWire *w;
    int i;

    gi = g->typeinfo;

    p += sprintf(p,"Type: %s\n",gi->name);
    p += sprintf(p,"Name: %s (%p)\n",g->ename ? g->ename : "*none*",g);
    if (GCElement_getType(g) == GC_BLOCK) {
      p += sprintf(p,"Size: %dx%d\n",g->u.block.gwidth,g->u.block.gheight);
    }
    p += sprintf(p,"Orient: %d\n",g->orient);
    p += sprintf(p,"Pads:\n");

    for (i = 0;i < N;i++) {
      p += sprintf(p,"  %s:",GCElement_getPadName(g,i));
      for (w = g->wires[i];w;w = w->next) {
	p += sprintf(p," ");
	if (w->net->n_signame)
	  p += sprintf(p,"%s(0x%p) {%d}",w->net->n_signame,(void*)w->net,w->nidx);
	else
	  p += sprintf(p,"0x%p {%d}",(void*)w->net,w->nidx);
	if (w == (GWire*)wire_sigroot(w))
	  p += sprintf(p,"[r]");
	else if (w == w->driver)
	  p += sprintf(p,"[d]");
	else
	  p += sprintf(p,"[e]");
      }
      p += sprintf(p,"\n");
    }
  } else if ((n = wire_iohit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires))) {
    GNet *net;

    while (n->in) n = n->in;

    net = n->end->net;
    p = PrintNet(p,net);
    p = PrintWireEnd(p,n->end);
    for (;n;n = n->out) {
      p = PrintWireNode(p,n);
      W = n->end;
    }
    p = PrintWireEnd(p,W);
    if (W->net != net) {
      p += sprintf(p,"####Warning other end on different net...\n");
      p = PrintNet(p,W->net);
    }
    p += sprintf(p,"\n");
  }

  if (*msg)
    DoTcl("tkg_debugMessage {%s}",msg);
}

/*****************************************************************************
 *
 * Perform main edit window actions in response to a mouse press in Move mode.
 *
 * Parameters:
 *      es		Editstate in which to perform operation
 *
 *****************************************************************************/
static int EditState_selectobject_Move(EditState *es)
{
  /*
   * Check to see what gate we may have clicked on.
   */
  GCElement *g = gate_hit(es->env,TkGate.ed->tx,TkGate.ed->ty);
  GWireNode *n;
  GWire *w;

  if (g && GCElement_getType(g) == GC_BLOCK) {
    Block_HitComponent(g,TkGate.ed->tx,TkGate.ed->ty);
  }

  if (EditState_getMode() == MODE_MOVESEL) {
    /*
     * We already have a set of selected gates (mode MOVESEL).  If the gate we
     * clicked on is already in the selected set, and we have the control key
     * pressed, unselect it.
     */
    if (g && sel_isSelGate(g)) {
      if ((TkGate.state & ControlMask)) {
	gate_draw(g,GD_NOWIRE);
	sel_unselectGate(es,g);
	gate_draw(g,GD_NOWIRE);
      }
      return 0;
    }
    EditState_setMode( MODE_MOVE);
  }

  /*
   * If the control key is not pressed, unselect all gates.
   */
  if (!(TkGate.state & ControlMask))
    sel_clear(es,1);

#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->sx = TkGate.ed->tx;
  TkGate.ed->sy = TkGate.ed->ty;

  if (GModuleDef_grabLabel(es->env,TkGate.ed->tx,TkGate.ed->ty,TkGate.circuit->labelsel)) {
    return 0;
  }

  if (g && GCElement_getType(g) == GC_TAP) {
    /*
     * Favor tap selection over wire selection.
     */
  } else if ((n = wire_hit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires))) {
    /*
     * Standard wire hit - wire can be moved
     */
    if (!editstate_isInterfaceMode()) {
      ob_touch(TkGate.circuit);
      TkGate.circuit->wnsel = n;

      if (((!TkGate.circuit->wnsel->in)
	   || (!TkGate.circuit->wnsel->out)) && (TkGate.circuit->wnsel->stype == FULL) ) {
	/*
	 * We have a connectable wire end
	 */
	TkGate.batc = 0;
	wm_SetCursor(IRON);
      }

      TkGate.circuit->wsel = wirenode_driver(TkGate.circuit->wnsel);
      EditState_unselectGate(es);
    }
  } else if ((n = wire_iohit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires))) {

    if (!editstate_isInterfaceMode()) {
      /*
       * I/O port wire hit - wire can not be moved unless we create new segments
       */
      if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
	message(0,msgLookup("err.protdata"));
	ob_touch(TkGate.circuit);
	TkGate.circuit->wsel = wirenode_driver(TkGate.circuit->wnsel);
	EditState_unselectGate(es);
	net_select(TkGate.circuit->wsel->net,1);

	return 0;
      }

      /*
       * If button1 is pressed and we are clicking on a segment with an anchored end, we may
       * need to create a corner to grab on to.  Otherwise, assume we are right clicking on the
       * wire segment to change wire/net/segment properties.
       */
      if (TkGate.button == 1) {
	if ((n = wire_makecorner(n,TkGate.ed->tx,TkGate.ed->ty))) {
	  w = wirenode_driver(n);
	}
      } else {
	if (n) {
	  w = wirenode_driver(n);
	}
      }

      if (n && w) {
	ob_touch(TkGate.circuit);
	TkGate.circuit->wnsel = n;
	TkGate.circuit->wsel = w;
      }
	
      EditState_unselectGate(es);
    }
  }

  if (TkGate.circuit->wnsel) {
    sel_clear(es,1);
  }

  /*
   * If we selected a wire, unselect any gate selection.
   */
  if (!TkGate.circuit->wnsel) { 
    if (g && g->selected && (TkGate.state & ControlMask)) {
      gate_draw(g,GD_NOWIRE);
      sel_unselectGate(es,g);
      gate_draw(g,GD_NOWIRE);
    } else {
      EditState_selectGate(es,TkGate.ed->tx,TkGate.ed->ty);
    }
  }

  /*
   * If there is no selection, we are setting the mark and will show the current
   * direction while the mouse button is held down. 
   */
  if (!Circuit_isSelection(TkGate.circuit) && ! editstate_isInterfaceMode()) {
    TkGate.batc = 0;
    switch (EditState_getRotation()) {
    case ROT0 :
      wm_SetCursor(ARROW0);
      break;
    case ROT90 :
      wm_SetCursor(ARROW90);
      break;
    case ROT180 :
      wm_SetCursor(ARROW180);
      break;
    case ROT270 :
      wm_SetCursor(ARROW270);
      break;
    }
  }


  /*
   * If we selected a wire or a joint, mark the appropriate net as selected.
   */
  if (TkGate.circuit->wsel) {
    net_select(TkGate.circuit->wsel->net,1);
  } else if (TkGate.circuit->select && TkGate.circuit->select->typeinfo->Code == GC_JOINT) {
    GCElement *g = TkGate.circuit->select;
    int i;
    for (i = 0;i < 4;i++)
      if (g->wires[i]) {
	net_select(g->wires[i]->net,1);

	return 0;
      }
  } else
    net_unselect(1);

  /*
   * If the control key is pressed, start a rubber-band selection box.
   */
  if (TkGate.state & ControlMask) {
    EditState_setMode( MODE_MAKESEL);
    wm_SetCursor(ARROWCURSOR);
    setRubberBox(es);
  }

  return 1;
}

/*****************************************************************************
 *
 * Perform main edit window actions in response to a mouse press in Replicate mode.
 *
 * Parameters:
 *      es		Editstate in which to perform operation
 *
 *****************************************************************************/
static int EditState_selectobject_Replicate(EditState *es)
{
  if (!modifyOK(es,0)) return 0;

  ob_append_frame("Replicate");
  EditState_selectGate(es,TkGate.ed->tx,TkGate.ed->ty);
  if (TkGate.circuit->select) {
#if TOUCH_XGATE_ED
    ob_touch(TkGate.ed);
#endif
    TkGate.ed->tx = TkGate.circuit->select->xpos;
    TkGate.ed->ty = TkGate.circuit->select->ypos;
    setrubberband(es);
  }
  ob_end_frame();

  return 1;
}

/*****************************************************************************
 *
 * Perform main edit window actions in response to a mouse press in Cut mode.
 *
 * Parameters:
 *      es		Editstate in which to perform operation
 *
 *****************************************************************************/
static int EditState_selectobject_Cut(EditState *es)
{
  if (!modifyOK(es,0x2)) return 0;

  if (TkGate.startrekp)
    wm_SetCursor(TREKDELETEDN);
  else
    wm_SetCursor(CLOSEDCUTTERS);
  if ((TkGate.circuit->wnsel = wire_iohit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires))) {
    ob_suggest_name("Cut");
    SetModified(MF_NET);
    net_unselect(1);
    wire_cut(TkGate.ed->tx,TkGate.ed->ty,TkGate.circuit->wnsel,es->env);
  }
  ob_touch(TkGate.circuit);
  TkGate.circuit->wsel = NULL;
  TkGate.circuit->wnsel = NULL;

  return 1;
}

/*****************************************************************************
 *
 * Perform main edit window actions in response to a mouse press in Invert mode.
 *
 * Parameters:
 *      es		Editstate in which to perform operation
 *
 *****************************************************************************/
static int EditState_selectobject_Invert(EditState *es)
{
  if (!modifyOK(es,0)) return 0;
  wm_SetCursor(INVERTDNCURSOR);

  ob_touch(TkGate.circuit);
  if ((TkGate.circuit->wnsel = wire_iohit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires)))
    if (TkGate.circuit->wnsel->end && TkGate.circuit->wnsel->end->gate) {
      if (!TkGate.circuit->wnsel->end->gate->typeinfo->Flags.NoInvert) {
	ob_suggest_name("Invert");
	GWire_draw(TkGate.circuit->wnsel->end->driver);
	ob_touch(TkGate.circuit->wnsel->end);
	TkGate.circuit->wnsel->end->invert = !TkGate.circuit->wnsel->end->invert;
	GWire_draw(TkGate.circuit->wnsel->end->driver);
	SetModified(MF_NET);
      } else
	message(0,msgLookup("err.noinvgate"));
    }
  TkGate.circuit->wnsel = NULL;

  return 1;
}

/*****************************************************************************
 *
 * Perform main edit window actions in response to a mouse press in Delete mode.
 *
 * Parameters:
 *      es		Editstate in which to perform operation
 *
 *****************************************************************************/
static int EditState_selectobject_Delete(EditState *es)
{
  if (!modifyOK(es,0)) return 0;
  if (TkGate.startrekp)
    wm_SetCursor(TREKDELETEDN);
  else
    wm_SetCursor(DELETEDNCURSOR);
  EditState_unselectGate(es);
  EditState_selectGate(es,TkGate.ed->tx,TkGate.ed->ty);
  if (TkGate.circuit->select) {
    ob_suggest_name("Delete");
    sel_delete(es);
  }
  ob_touch(TkGate.circuit);
  TkGate.circuit->last = TkGate.circuit->select = NULL;

  return 1;
}

/*****************************************************************************
 *
 * Perform main edit window actions in response to a mouse press.
 *
 * Parameters:
 *      es		Editstate in which to perform operation
 *
 * Select an object at the current mouse position.  The selected object
 * could be a gate or a wire.
 *
 *****************************************************************************/
void EditState_selectobject(EditState *es)
{
  switch (EditState_getMode()) {
  case MODE_MOVENULL :
    break;
  case MODE_MOVESEL :
  case MODE_MOVE :
    EditState_selectobject_Move(es);
    break;
  case MODE_DEBUG :
    DoDebug(es);
    break;
  case MODE_REPLICATE :
    EditState_selectobject_Replicate(es);
    break;
  case MODE_CUT :
    EditState_selectobject_Cut(es);
    break;
  case MODE_INVERT :
    EditState_selectobject_Invert(es);
    break;
  case MODE_DELETE :
    EditState_selectobject_Delete(es);
    break;
  default :
    break;
  }

  if (TkGate.circuit->wsel && tkgate_currentMode() == MM_ANALYZE)
    cpath_showNetDelay(TkGate.circuit->wsel->net);
}

/*****************************************************************************
 *
 * Set the cursor for the current mode.
 *
 *****************************************************************************/
void setEditCursor(int mode)
{
  switch (mode) {
  case MODE_MOVE :
  default :
    if (TkGate.batp) {
      SetBatCursor();
    } else
      wm_SetCursor(ARROWCURSOR);
    break;
  case MODE_INVERT :
    wm_SetCursor(INVERTUPCURSOR);
    break;
  case MODE_CUT :
    if (TkGate.startrekp)
      wm_SetCursor(TREKDELETEUP);
    else
      wm_SetCursor(OPENCUTTERS);
    break;
  case MODE_SCROLL :
    wm_SetCursor(SCROLLCURSOR);
    break;
  case MODE_DELETE :
    if (TkGate.startrekp)
      wm_SetCursor(TREKDELETEUP);
    else
      wm_SetCursor(DELETEUPCURSOR);
    break;
  case MODE_DEBUG :
    wm_SetCursor(WRENCHCURSOR);
    break;
  case MODE_SETSIZE :
    wm_SetCursor(SIZECURSOR);
    break;
  case MODE_REPLICATE :
    wm_SetCursor(REPCURSOR);
    break;
  }

}

/*****************************************************************************
 *
 * Set the edit mode.
 *
 *****************************************************************************/
void setEditMode(EditState *es,int mode)
{
  int iomodep;
  static int lastmode = 0;

  ob_begin_frame("-Unselect");

  iomodep = 0;
  switch (EditState_getMode()) {
  case MODE_MOVENULL :
  case MODE_MOVE :
    lastmode = MODE_MOVE;
    break;
  case MODE_INVERT :
    lastmode = EditState_getMode();
    break;
  case MODE_CUT :
    lastmode = EditState_getMode();
    break;
  case MODE_SCROLL :
    lastmode = EditState_getMode();
    break;
  case MODE_DELETE :
    lastmode = EditState_getMode();
    break;
  case MODE_SETSIZE :
    lastmode = EditState_getMode();
    break;
  case MODE_DEBUG :
    lastmode = EditState_getMode();
    break;
  default :
    break;
  }

  if (lastmode == MODE_MOVE && TkGate.batp) lastmode = -1;

  if (mode == MODE_REPLICATE)
    DoTcl("set ::tkg_replicateOn 1");
  else
    DoTcl("set ::tkg_replicateOn 0");


  TkGate.batc = 0;
  EditState_setMode( mode);
  switch (mode) {
  case MODE_MOVE :
    TkGate.ed->mark_vis = 0;
    break;
  case MODE_INVERT :
    net_unselect(1);
    sel_clear(es,1);
    break;
  case MODE_CUT :
    net_unselect(1);
    sel_clear(es,1);
    break;
  case MODE_SCROLL :
    net_unselect(1);
    sel_clear(es,1);
    break;
  case MODE_DELETE :
    net_unselect(1);
    sel_clear(es,1);
    break;
  case MODE_DEBUG :
    net_unselect(1);
    break;
  case MODE_SETSIZE :
    net_unselect(1);
    sel_clear(es,1);
    break;
  case MODE_REPLICATE :
    net_unselect(1);
    break;
  default :
    net_unselect(1);
    break;
  }

  setEditCursor(mode);

  if (!iomodep) {
    mark_unpost();
    EditState_unselectGate(es);
    ob_touch(TkGate.circuit);
    TkGate.circuit->wnsel = NULL;
    TkGate.circuit->wsel = NULL;
  }
  ob_end_frame();
}

/*****************************************************************************
 *
 * Write a message to the log file.
 *
 *****************************************************************************/
void logError(int level,const char *fileName,int lineNum,const char *msg,...)
{
  FILE *f;
  struct passwd *pw;
  struct tm *tm;
  time_t clock[1];
  char buf[STRMAX],msgBuf[STRMAX],fname[STRMAX];
  char *lstr = 0;
  static char *mon[] = {
    "Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"
  };
  va_list ap;
  const char *home = 0;

  if (TkGate.tcl)
    home = Tcl_GetVar(TkGate.tcl,"tkg_gateHome",TCL_GLOBAL_ONLY);
  else
    home = "<tkgate-home>";

  va_start(ap,msg);
  vsprintf(msgBuf,msg,ap);
  va_end(ap);


  switch (level) {
  case 0 : lstr = "Warning: "; break;
  case 1 : lstr = "Error: "; break;
  case 2 : lstr = "FATAL: "; break;
  default: lstr = "Unknown: "; break;
  }

  pw = getpwuid(getuid());
  time(clock);
  tm = localtime(clock);
  sprintf(buf,"<%s> %s [%s] (%d-%s-%02d %d:%02d) %s (%s, line %d)"
	  ,TKGATE_FULL_VERSION
	  ,pw->pw_name,GetSysType()
	  ,tm->tm_mday,mon[tm->tm_mon],(tm->tm_year % 100),tm->tm_hour,tm->tm_min
	  ,msgBuf,fileName,lineNum);

  if (TkGate.errorLogFile) {
    sprintf(fname,"%s/%s",home,TkGate.errorLogFile);
    if ((f = fopen(fname,"a"))) {
      fprintf(f,"%s\n",buf);
      fclose(f);
    } else if ((f = fopen(TkGate.errorLogFile,"a"))) {
      fprintf(f,"%s\n",buf);
      fclose(f);
    }
  }

  printf("tkgate: %s\n",buf);
  switch (level) {
  case 0 :
    if (TkGate.baderp)
      printf("Oh! Look what you almost did, Miles!\n");
    else if (TkGate.startrekp)
      printf("Captain!  Romulan ship decloaking dead ahead!\n");
    break;
  case 1 :
    if (TkGate.baderp)
      printf("Oh! Look what you did, Miles!\n");
    else if (TkGate.startrekp)
      printf("We have engaged the Borg!\n");
    break;
  case 2 :
    panicSave(0);
  }
}

/*****************************************************************************
 *
 *****************************************************************************/
void UpdateModifiedIndicator()
{
  if (TkGate.tcl)
    Tcl_SetVar(TkGate.tcl,"tkg_modifiedFlag",
	       binstr(TkGate.circuit->modified_flags),TCL_GLOBAL_ONLY);
}

/*****************************************************************************
 *
 * Mark the circuit as modified.  Two types of flags are set 'modified_flags'
 * and 'sync_flags'.  The 'modified_flags' are used to indicate that there
 * are changes that have not been saved to the current save file.  The
 * 'sync_flags' are used to indicate that there have been changes to the circuit
 * data that may require 'synching' with the tcl/tk interface.  The sync flags
 * are cleared as soon as the tcl/tk interface has been updated.  Different bits
 * in the 'sync_flags' indicate what has been changed.
 *
 *****************************************************************************/
void SetModified(unsigned flags)
{
  static int inSpecial = 0;

  if (!(flags & MF_FORCE)) {
    if ((flags & MF_BEGINSPEC)) {
      inSpecial++;
      return;
    }
    if ((flags & MF_ENDSPEC)) {
      inSpecial--;
      return;
    }

    if (inSpecial)
      flags |= MF_SYNCONLY; 
  }

  if ((flags & MF_SYNCONLY)) {
    TkGate.sync_flags |= flags;
  } else {
    if (TkGate.circuit->no_set_modify) return;

    GSearchContext_clear(TkGate.circuit->search);

    ob_touch(TkGate.circuit);
    TkGate.circuit->modified_flags |= flags;
    TkGate.sync_flags |= flags;

    UpdateModifiedIndicator();
  }
}

/*****************************************************************************
 *
 * Mark the circuit as unmodified.  This is called after saving (or loading)
 * a file.
 *
 *****************************************************************************/
void ClearModified()
{
  ob_touch(TkGate.circuit);
  TkGate.circuit->modified_flags = 0;
  UpdateModifiedIndicator();
}

/*****************************************************************************
 *
 * Clear the synchronization flags.
 *
 *****************************************************************************/
void SynchronizeInterface()
{
  if (!TkGate.tcl) return;


  /*
   * Debug printing of sync flags.
   */
#if 0
  if (TkGate.sync_flags) {
    printf("SYNC:");
    if ((TkGate.sync_flags & MF_INTERFACE)) {
      printf(" interface");
    }
    if ((TkGate.sync_flags & MF_MODULE)) {
      printf(" module");
    }
    if ((TkGate.sync_flags & MF_GATE)) {
      printf(" gate");
    }
    if ((TkGate.sync_flags & MF_NET)) {
      printf(" net");
    }
    printf("\n");
  }
#endif

  if ((TkGate.sync_flags & MF_MODULE)) {
    env_updateMTCircuit();
  }

  if ((TkGate.sync_flags & MF_INTERFACE)) {
    int interface_updated = 0;

    if (editstate_getInterfaceMode() == INTFMODE_SINGLE) {
      GModuleDef *M = 0;
      GCElement *g = 0;
      SHash *H = TkGate.circuit->mid_display->m_gates;
      HashElem *E;

      for (E = Hash_first(H);E;E = Hash_next(H,E)) {
	GCElement *dg = HashElem_obj(E);

	if (GCElement_isModule(dg)) {
	  g = dg;
	  if (!interface_updated)
	    ob_begin_framef("ItfUpdate",FF_TRANSPARENT||FF_BACKGROUND);
	  interface_updated = 1;
	  M = env_findAdd(g->u.block.moduleName,0);
	}
      }

      if (M && g) {
#if 0
	printf("setInterface: M.g=%p  g=%p\n",M->m_interface,g);
#endif
	if (!interface_updated)
	  ob_begin_framef("ItfUpdate",FF_TRANSPARENT||FF_BACKGROUND);
	interface_updated = 1;
	modint_setInterface(M,g);
      }

      DoTcl("IPanel::loadCurrent");

      if (interface_updated)
	ob_end_frame();
    }
  }

  if (TkGate.sync_flags)
    Concat_updateAutos(TkGate.circuit->es->env,1);

  TkGate.sync_flags = 0;
}

