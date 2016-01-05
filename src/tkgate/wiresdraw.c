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
/*
    One of four main modules pertaining to wires.  COntaines functions for moving
    and drawing wires.  Basic routines to maintain horizontal and vertical wires
    being moved are defined here.


*/
#include <stdlib.h>
#include <stdio.h>
#include "tkgate.h"

/*****************************************************************************
 *
 * Tweek point to compensate for X11 server differences.
 *
 * Parameters:
 *      p		Coordinate to be adjusted
 *      q		Next point in line segment	
 *
 * Tweek end point p (where q is the adjacent point) to compensate for
 * X server differences.  It is assumed that p and q are non-heap
 * objects and do not require being saved with ob_touch()
 *
 *****************************************************************************/
static void tweekPoint(XPoint *p,XPoint *q)
{
  if (p->x == q->x) {
    if (p->y > q->y) {
      p->y += TKGATE_WIRETWEEK_BOTTOM;
    } else if (p->y < q->y) {
      p->y += TKGATE_WIRETWEEK_TOP;
    }
  } else if (p->y == q->y) {
    if (p->x > q->x) {
      p->x += TKGATE_WIRETWEEK_RIGHT;
    } else if (p->x < q->x) {
      p->x += TKGATE_WIRETWEEK_LEFT;
    }
  }
}

/*****************************************************************************
 *
 * Get the position of the net node label 
 *
 * Parameters:
 *      n		Node on which to get label position.
 *      net		Net of the node.
 *      *x,*y		Return coordinates for node.
 *      *p		Positioning
 *
 *****************************************************************************/
void GWireNode_getLabelPos(GWireNode *n,GNet *net,int *x,int *y,int *p)
{
  //  n->showSide
  if (n->out) {
    if (n->y != n->out->y) {
      /* Vertical segment */
      if (n->labelSide) {
	*x = n->x - 3;
	*y = n->y + (n->out->y - n->y)*n->offset/100;
	*p = AtRight|AtBottom;
      } else {
	*x = n->x + 3;
	*y = n->y + (n->out->y - n->y)*n->offset/100;
	*p = AtLeft|AtBottom;
      }
    } else if (n->x != n->out->x) {
      /* Horizontal segment */
      if (n->labelSide) {
	*y = n->y;
	*x = n->x + (n->out->x - n->x)*n->offset/100;
	*p = AtLeft|AtTop;
      } else {
	*y = n->y;
	*x = n->x + (n->out->x - n->x)*n->offset/100;
	*p = AtLeft|AtBottom;
      }
    } else {
      /* Zero-length segment */
      *x = n->x;
      *y = n->y;
      *p = AtLeft|AtBottom;
    }
  } else {
    /* Shouldn't be possible, but just in case */
    *x = n->x;
    *y = n->y;
    *p = AtLeft|AtBottom;
  }
}

/*****************************************************************************
 *
 * Display the label at a node.
 *
 * Parameters:
 *      n		Node on which to display label
 *      net		Net of the node.
 *
 *****************************************************************************/
static void GWireNode_displayLabel(GWireNode *n,GNet *net)
{
  char label[STRMAX];
  GC gc;

  if (net == TkGate.circuit->nsel) {
    if (net->n_nbits > 1)
      gc = TkGate.selBusGC;
    else
      gc = TkGate.selWireGC;
  } else {
    if (net->n_nbits > 1)
      gc = TkGate.busGC;
    else
      gc = TkGate.wireGC;
  }


  if (GNet_getDisplayLabel(net, label, STRMAX, DLM_GET_VISIBLE) > 0) {
    int x,y,p;

    GWireNode_getLabelPos(n,net,&x,&y,&p);
    dce_DrawString(gc,x,y,p,label);
  }
}


/*****************************************************************************
 *
 * Display the size of a wire with a slant and a number of bits
 *
 * Parameters:
 *      n		Wirenode at which to display wire size
 *      net		Net for this wirenode (or NULL to search for the net)
 *
 *
 *****************************************************************************/
void GWireNode_displaySize(GWireNode *n,GNet *net)
{
  GWireNode *n2 = n->out;

  //
  // No implied next node.
  //
  if (!n2) return;

  //
  // If no net was given, search for the net.
  //
  if (!net) {
    GWire *w = wirenode_driver(n);
    net = w->net;
  }

  if (net->n_nbits > 1) {
    int x1 = n->x;
    int y1 = n->y;
    int x2 = n2->x;
    int y2 = n2->y;
    char ns[30];
    int dx,dy,x,y;

    dx = (x2 - x1);
    dy = (y2 - y1);

    if (dx*dx+dy*dy < (15*15))
      return;


    x = x1 + dx/2;
    y = y1 + dy/2;

    sprintf(ns,"%d",net->n_nbits);

    if (x1 == x2) {
      Icon_draw(TkGate.D,TkGate.W,TkGate.busGC,ctow_x(x+TKGATE_BUSW_VERT-1),ctow_y(y),SIZEHASH);
      dce_DrawString(TkGate.busGC,x+8+TKGATE_BUSW_VERT,y,AtLeft|BetweenTopAndBottom,ns);
    } else {
      Icon_draw(TkGate.D,TkGate.W,TkGate.busGC,ctow_x(x),ctow_y(y+TKGATE_BUSW_HORZ-1),SIZEHASH);
      dce_DrawString(TkGate.busGC,x,y-3+TKGATE_BUSW_HORZ,BetweenLeftAndRight|AtBottom,ns);
    }

  }
}

/*
    Draw end of wire
*/
void wire_drawend(GWireNode *n,GWireNode *n2,int s,short *x,short *y)
{
  static int xval[] = {0,0,-1,1};
  static int yval[] = {-1,1,0,0};
  int d,inv;

  d = (n->x == n2->x) ? (n->y <= n2->y) : 2 + (n->x <= n2->x);
  inv = n->end->invert != 0;

  if (inv)
    Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,
	      ctow_x(n->x + 2*xval[d]),ctow_y(n->y + 2*yval[d]),
	      INVERTER);

  if (n->end && n->end->gate && n->end->gate->cpath_cut) {
    int x1,y1,x2,y2;
    int r;

    x1 = x2 = ctow_x(n->x + xval[d]*5);
    y1 = y2 = ctow_y(n->y + yval[d]*5);

    r = (d + 2) % 4;

    x1 += xval[r]*5;
    x2 -= xval[r]*5;
    y1 += yval[r]*5;
    y2 -= yval[r]*5;

    ZDrawLine(TkGate.D, TkGate.W, TkGate.instGC, x1,y1, x2,y2);
  }

  *x += 5*xval[d]*inv;
  *y += 5*yval[d]*inv;
}

/*****************************************************************************
 *
 * Draw all wires driving a gate
 *
 *****************************************************************************/
void wire_drawlist(GWire *w,GCElement *g)
{
  GCElement *dg;

  for (;w;w = w->next) {
    dg = wire_drivinggate(w);
    if ((!g) || (!dg) || g == dg)
      GWire_draw(w->driver);
  }
}

/*****************************************************************************
 *
 * Draw a single wire
 *
 * Parameters:
 *      w		Wire to draw
 *
 * A wire is a single sequence of verticies between to connection points.  A
 * connection point can be either a gate or a joint.
 *
 *****************************************************************************/
void GWire_draw(GWire *w)
{
  GWire *w1,*w2;
  GWireNode *n;
  XPoint p[1024];
  int k;

  w1 = wire_driver(w);
  w2 = wire_other(w1);

  for (k = 0, n = w1->nodes;n;k++, n = n->out) {
    p[k].x = ctow_x(n->x);
    p[k].y = ctow_y(n->y);

    if (n->showSize)
      GWireNode_displaySize(n,w1->net);

    if (n->isLabeled)
      GWireNode_displayLabel(n,w1->net);
  }

  wire_drawend(w1->nodes,w1->nodes->out,0,&p[0].x,&p[0].y);
  wire_drawend(w2->nodes,w2->nodes->in,0,&p[k-1].x,&p[k-1].y);

  if (w1->net->n_nbits > 1) {
    GC gc = (w1->net == TkGate.circuit->nsel) ? TkGate.selBusGC : TkGate.busGC;

    tweekPoint(&p[0],&p[1]);
    tweekPoint(&p[k-1],&p[k-2]);

    ZDrawLines(TkGate.D,TkGate.W,gc,p,k,CoordModeOrigin);
  } else {
    GC gc = (w1->net == TkGate.circuit->nsel) ? TkGate.selWireGC : TkGate.wireGC;

    if (w1->net == TkGate.circuit->nsel) {
      tweekPoint(&p[0],&p[1]);
      tweekPoint(&p[k-1],&p[k-2]);
    }
    ZDrawLines(TkGate.D,TkGate.W,gc,p,k,CoordModeOrigin);
  }

  if (w1->cpath && TkGate.cpath_flashState) {
    int i;

    for (i = 1;i < k;i++)
      cpath_draw(p[i-1].x,p[i-1].y,p[i].x,p[i].y);
  }
}

static void wire_drawnet_aux(GWire *w,int isFirst)
{
  int i;

  w = wire_other(w);

  if (!isFirst) {
    GWire_draw(w);
    if (w->gate && w->gate->typeinfo->Flags.WireProps) gate_draw(w->gate,GD_NOWIRE);
  }

  if (w->gate) {
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      gate_draw(w->gate,GD_NOWIRE);
      for (i=0;i<4;i++)
	if (w->gate->wires[i] && (w->gate->wires[i] != w))
	  wire_drawnet_aux(w->gate->wires[i],0);
      break;
    case GC_TAP :
      if (w == w->gate->wires[TAP_OUT])
	wire_drawnet_aux(w->gate->wires[TAP_IN],0);
      else if (w == w->gate->wires[TAP_IN])
	wire_drawnet_aux(w->gate->wires[TAP_OUT],0);
      break;
    case GC_LED :
      gate_draw(w->gate,GD_NOWIRE);
      break;
    }
  }
}

/*****************************************************************************
 *
 * Draw all wires associate with a net
 *
 * Parameters:
 *      w		Representative wire on the net
 *
 *****************************************************************************/
void wire_drawnet(GWire *w)
{
  GWire *o_w;

  if (!w) return;

  o_w = wire_other(w);

  wire_drawnet_aux(w,0);
  wire_drawnet_aux(o_w,1);

  if (w->gate && w->gate->typeinfo->Flags.WireProps) gate_draw(w->gate,GD_NOWIRE);
}
