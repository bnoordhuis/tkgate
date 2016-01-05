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
/*###########################################################################*
 *
 * Functions for creating wires and adding wires to circuit elements are
 * defined in this source module.
 *
 *###########################################################################*/
#include "tkgate.h"

/*****************************************************************************
 *
 * Counts the number of wires on a terminal
 *
 * Parameters:
 *    w			Head of wire list.
 *
 * Returns:		Number of wires in wire list.
 *
 *****************************************************************************/
int wire_count(GWire *w)
{
  if (!w)
    return 0;
  else
    return 1 + wire_count(w->next);
}

/*****************************************************************************
 *
 * Links a wire into the wire list
 *
 * Parameters:
 *    wl		Wire list
 *    w			Wire to be linked
 *
 * Returns:		Modified wire list.
 *
 *****************************************************************************/
GWireList *wire_link(GWireList *wl,GWire *w)
{
  GWireList *new_wl;

  new_wl = (GWireList *) ob_malloc(sizeof(GWireList),"GWireList");
  ob_touch(new_wl);
  new_wl->wl_next = wl;
  new_wl->wl_wire = w;
  return new_wl;
}

/*****************************************************************************
 *
 * Unlinks a wire from the wire list
 *
 * Parameters:
 *    wl		Wire list
 *    w			Wire to be unlinked
 *
 * Returns:		Modified wire list.
 *
 *****************************************************************************/
GWireList *wire_unlink(GWireList *wl,GWire *w)
{
  GWireList *rest;

  if (wl == NULL)
    return NULL;

  if (wl->wl_wire != w) {
    ob_touch(wl);
    wl->wl_next = wire_unlink(wl->wl_next,w);
    return wl;
  } else {
    rest = wl->wl_next;
    ob_free(wl);

    return rest;
  }
}

/*****************************************************************************
 *
 * Creates a new endpoint of a wire
 * 
 * Parameters:
 *    M			Module in which to create new wire end.
 *    net		Net to which new wire end will belong
 *    doNodes		Non-zero if wire nodes should also be created.
 *
 * Returns:		New wire object. 
 *
 *****************************************************************************/
GWire *wire_newend(GModuleDef *M,GNet *net,int doNodes)
{
  GWire *w;

  w = (GWire *) ob_malloc(sizeof(GWire),"GWire");
  ob_touch(w);

  w->net = 0;
  w->name = NULL;
  w->xanchor = 0;
  w->yanchor = 0;
  w->orient = 0; 
  w->wtype = 0;
  w->PadNum = 0;
  w->WireDir = 0;
  w->invert = 0;
  w->gate = NULL;
  w->nodes = NULL;
  w->driver = NULL;
  w->next = NULL;
  w->nidx = 0;
  w->cpath = 0;
  w->offset.num = w->offset.den = 0;

  if (net)
    wire_setNet(w,net);

  if (doNodes) {
    w->nodes = new_GWireNode();
    ob_touch(w->nodes);
    w->nodes->end = w;
  }

  if (M) {
    ob_touch(M);
    M->m_wires = wire_link(M->m_wires,w);
  }

  return w;
}

/*****************************************************************************
 *
 * Creates a new wire node (Should be in nodes)
 *
 * Returns:		New wire node object.
 *
 *****************************************************************************/
GWireNode *new_GWireNode()
{
  GWireNode *n;

  n = (GWireNode *) ob_malloc(sizeof(GWireNode),"GWireNode");

  n->out = NULL;
  n->in = NULL;
  n->end = NULL;
  n->stype = UNSELECTED;
  n->mark = 0;
  n->showSize = 0;
  n->isLabeled = 0;
  n->labelSide = 0;
  n->offset = 50;

  return n;
}

/*****************************************************************************
 *
 *   Set/reset the net of a wire.  Update reference counts,
 *   and free net if this is the last reference to the old
 *   net of w.
 *
 * Parameters:
 *    w			Wire on which to set the net.
 *    net		Net to be assigned to the wire w.
 *
 *****************************************************************************/
void wire_setNet(GWire *w,GNet *net)
{
  ob_touch(w);				/* JPH 121603 */
  if (net) {
    ob_touch(net);			/* JPH 121603 */
    net->n_refs++;
  }
  if (w->net) {
    ob_touch(w->net);			/* JPH 121603 */
    if (--w->net->n_refs <= 0)
      delete_GNet(w->net);
  }
  w->net = net;
}

/*****************************************************************************
 *
 * Create a new wire segment on an existing net.
 *
 * Parameters
 *    M			Module in which to create wires.
 *    net		Net on which to create segment.
 *    *e1		Return for End 1 of new segment
 *    *e2		Return for End 2 of new segment
 *
 * Returns:		Non-zero on error.
 *
 *****************************************************************************/
int wire_newNetSegment(GModuleDef *M,GNet *net,GWire **e1,GWire **e2)
{
  GWire *end1,*end2;			/* end1 is output end, end2 is input end */

  end1 = wire_newend(M,net,1);
  end2 = wire_newend(M,net,1);

  ob_touch(end1);
  ob_touch(end2);
  ob_touch(end1->nodes);
  ob_touch(end2->nodes);
  end1->nodes->out = end2->nodes;
  end2->nodes->in = end1->nodes;

  end1->driver = end1;
  end1->wtype = DRIVER;
  end2->driver = end1;
  end2->wtype = DRIVEE;

  if (e1) *e1 = end1;
  if (e2) *e2 = end2;

  return 0;
}

/*****************************************************************************
 *
 * Create a new wire segment on a new net.
 *
 * Parameters:
 *    M			Module in which to create new wires/net
 *    *e1		Return for End 1 of wire
 *    *e2		Return for End 2 of wire
 *
 *****************************************************************************/
int wire_new(GModuleDef *M,GWire **e1,GWire **e2)
{
  GNet *net;

  net = new_GNet(0,M);				/* Create a whole new net */
  wire_newNetSegment(M,net,e1,e2);		/* Create wire segments on net */ 
  if (e1)
    (*e1)->nodes->showSize = 1;
  

  return 0;
}

/*****************************************************************************
 *
 * Free a wire and decrement reference count on its net
 *
 * Parameters:
 *    w			Wire to be freed.
 *
 *****************************************************************************/
void wire_free(GWire *w)
{
  if (w->net) net_decref(w->net);
  ob_free(w);
}

/*****************************************************************************
 *
 * Free a wire node.
 *
 * Parameters:
 *    n			Wire node to be freed.
 *
 *****************************************************************************/
void delete_GWireNode(GWireNode *n)
{
  ob_free(n);
}

/*****************************************************************************
 *
 * Evenly space wires on a pad of a gate along that pad.
 *
 * Parameters:
 *    g			Gate on which to space wires
 *    p			Pad # on gate.
 *
 *****************************************************************************/
void wire_setPadSpacing(GCElement *g,int p)
{
  int n,i,dy,dx;
  GWire *q;
  GWire *w = g->wires[p];
  int x1 = g->xpos + g->typeinfo->Pad[p].Loc[g->orient].x1;
  int y1 = g->ypos + g->typeinfo->Pad[p].Loc[g->orient].y1;
  int x2 = g->xpos + g->typeinfo->Pad[p].Loc[g->orient].x2;
  int y2 = g->ypos + g->typeinfo->Pad[p].Loc[g->orient].y2;

  n = wire_count(w) + 1;
  dx = (1000*(x2 - x1)) / n;
  dy = (1000*(y2 - y1)) / n;
  for (i = 1, q = w;q;i++, q = q->next) {
    int x = x1 + (dx * i)/1000;
    int y = y1 + (dy * i)/1000;
    wire_move(q->nodes,x-q->nodes->x,y-q->nodes->y,FULL);
  }
}

/*****************************************************************************
 *
 * Place a wires in the appropriate positions relative to the gates's position.
 *
 * Parameters:
 *    p		Pad # on gate
 *    g		Gate on which wire is attached.
 *    w		Wire to be placed.
 *    inout	I/O direction of wire
 *
 *****************************************************************************/
void wire_placement(int p,GCElement *g,GWire *w,int inout)
{
  ob_touch(w);
  w->orient = g->typeinfo->Pad[p].Loc[g->orient].dir;
  if (inout == IN) {
    ob_touch(w->nodes->in);
    w->nodes->in->x = w->nodes->x;
    w->nodes->in->y = w->nodes->y;
    switch (w->orient) { /* Danger */
    case D_RIGHT :
      w->nodes->in->x += TKGATE_STUBLEN;
      break;
    case D_UP :
      w->nodes->in->y -= TKGATE_STUBLEN;
      break;
    case D_LEFT :
      w->nodes->in->x -= TKGATE_STUBLEN;
      break;
    case D_DOWN :
      w->nodes->in->y += TKGATE_STUBLEN;
      break;
    }
  } else {
    ob_touch(w->nodes->out);
    w->nodes->out->x = w->nodes->x;
    w->nodes->out->y = w->nodes->y;
    switch (w->orient) {
    case D_RIGHT :
      w->nodes->out->x += TKGATE_STUBLEN;
      break;
    case D_UP :
      w->nodes->out->y -= TKGATE_STUBLEN;
      break;
    case D_LEFT :
      w->nodes->out->x -= TKGATE_STUBLEN;
      break;
    case D_DOWN :
      w->nodes->out->y += TKGATE_STUBLEN;
      break;
    }
  }
#ifdef DEBUGADD
  printf("end place\n");
#endif
}

/*****************************************************************************
 *
 * Append wire to end of the pad list
 *
 * Parameters:
 *    wl		Wire at head of pad list.
 *    w			Wire to be appended.
 *
 * Returns:		New wire pad list
 *
 *****************************************************************************/
GWire *wire_append(GWire *wl,GWire *w)
{
  GWire *l;

  if (!wl) return w;

  for (l = wl;l->next;l = l->next);
  ob_touch(l);
  l->next = w;

  return wl;
}


/*****************************************************************************
 *
 * Helping function to add a wire to a gate and set initial placement.
 *
 * Parameters:
 *    g			Gate on which to attach wire
 *    e1		End 1 of wire
 *    e2		End 2 of wire
 *    p			Pad # on gate
 *    dir               I/O direction of wire
 *    invertp		Non-zero if we should attach an inverter.
 *
 *****************************************************************************/
static void wire_gateadd(GCElement *g,GWire *e1,GWire *e2,int p,int dir,int invertp)
{
  GWire *w;

#ifdef DEBUGADD
  printf("wire_gate_add(%p)\n",p);
#endif
  net_setSize(e1->net,g->typeinfo->Pad[p].Size);
  net_setSize(e2->net,g->typeinfo->Pad[p].Size);

  switch (dir) {
  case IN :
    w = e2;
    ob_touch(w);
    w->gate = g;
    ob_touch(g);
    g->wires[p] = wire_append(g->wires[p],w);
    wire_placement(p,g,w,IN);
    wire_setPadSpacing(g,p);
    break;
  case TRI :
  case OUT :
    w = e1;
    ob_touch(w);
    w->gate = g;
    w->invert = invertp;
    ob_touch(g);
    g->wires[p] = wire_append(g->wires[p],w);
    wire_placement(p,g,w,OUT);
    wire_setPadSpacing(g,p);
    break;
  default :
    printf("UNKNOWN pin type\n");
    abort();
    break;
  }
}

/*****************************************************************************
 *
 * Adds a wire to a gate
 *
 * Parameters:
 *    g			Gate on which to add wire.
 *    p			Pad # on which to add wire.
 *    M			Module in which gate is defined.
 *    invertp		Non-zero to add inverter to wire.
 *  
 * Returns:		Non-zero on error.
 *
 *****************************************************************************/
int wire_addToGate(GCElement *g,int p,GModuleDef *M,int invertp)
{
  GWire *e1,*e2;

  wire_new(M,&e1,&e2);
  wire_gateadd(g,e1,e2,p,g->typeinfo->Pad[p].iotype,invertp);
  wire_finalizeNet(e1);

  return 0;
}
