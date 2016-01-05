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

    Last edit by hansen on Sun Feb 12 17:01:30 2006
****************************************************************************/
/*
   Functions in this module are used to make joints.  Joints are created
   when a wire end is dropped near the middle of another wire.
*/
#include <stdio.h>
#include "tkgate.h"

#define parallel(_n1,_n2) ((((_n1)->x == (_n1)->out->x) && ((_n2)->x == (_n2)->out->x)) \
			|| (((_n1)->y == (_n1)->out->y) && ((_n2)->y == (_n2)->out->y)))

void join_wirenodereverse(GWireNode *n)
{
  GWireNode *t;

  if (n) {
    ob_touch(n);
    t = n->out;
    n->out = n->in;
    n->in = t;
    join_wirenodereverse(n->in);
  }
}

/*
  Reverse the link direction of a single wire.
 */
void join_wirereverse(GWire *w)
{
  struct wire *wd;

  wd = wire_driver(w);
  join_wirenodereverse(wd->nodes);

  wd = wire_driver(w);
  ob_touch(wd);
  wd->driver = wd;

  w = wire_drivee(w);
  ob_touch(w);
  w->driver = wd;
}

void join_treereverse_aux(GWire *w)
{
  GWire *ow = wire_other(w);		/* Get other end */
  GCElement *g;
  int i;

  join_wirereverse(w);

  if (!(g = ow->gate)) return;		/* Other end was endpoint, we are all done */

  switch (g->typeinfo->Code) {
  case GC_JOINT :
    if (ow->driver == ow) {		/* ow used to be drivee, need to choose new drivee */
      for (i = 0;i < 4;i++)
	if (g->wires[i] && g->wires[i] != ow) {
	  join_treereverse_aux(g->wires[i]);
	  break;
	}
    } else {				/* ow now a drivee, need to undo other drivee */
      for (i = 0;i < 4;i++)
	if (g->wires[i] && g->wires[i] != ow && g->wires[i]->driver != g->wires[i]) {
	  join_treereverse_aux(g->wires[i]);
	  break;
	}
    }
    /*
     * This is just here as a consistancy check
     */
    {
      int nd = 0,ne = 0;


      for (i = 0;i < 4;i++) {
	GWire *w = g->wires[i];

	if (!w) continue;
	if (w->driver == w) nd++; else ne++;
      }
      if (ne != 1)
	logError(ERL_ERROR,"Expecting only a single drivee on joint.");
    }
    break;
  case GC_TAP :
    if (g->wires[TAP_IN] == ow)
      join_treereverse_aux(g->wires[TAP_OUT]);
    else if (g->wires[TAP_OUT] == ow)
      join_treereverse_aux(g->wires[TAP_IN]);
    break;
  }
}

/*
  Reverse the link direction of a tree of wires.  The top-level
  w should be a free end.
 */
void join_treereverse(GWire *w)
{
  if (w->gate)
    logError(ERL_ERROR,"Expecting free end in join_treereverse");
  else
    join_treereverse_aux(w);
}

/*
	Splices a corner.  After splice, w1 is a driver, and w2 is a drivee.
*/
void join_splice_corner(GWireNode *n,GWire *w1[],GWire *w2[],int dwidth,EditState *es)
{
  GWire  *wd;
  GWire *pw = wirenode_driver(n);

  w1[0] = wire_newend(es->env,pw->net,0);
  w2[0] = wire_newend(es->env,pw->net,0);

  ob_touch(n);
  ob_touch(w1[0]);
  ob_touch(w2[0]);
  ob_touch(w1[0]->nodes);

  w1[0]->nodes = n;
  w2[0]->nodes = new_GWireNode();

  ob_touch(w2[0]->nodes);
  w2[0]->nodes->x = n->x;
  w2[0]->nodes->y = n->y;
  w2[0]->nodes->in = n->in;

  ob_touch(w2[0]->nodes->in);
  w2[0]->nodes->in->out = w2[0]->nodes;

  ob_touch(n);
  n->in = NULL;
  
  w1[0]->nodes->end = w1[0];
  w2[0]->nodes->end = w2[0];
  
  w1[0]->driver = wire_driver(w1[0]);
  wd = wire_drivee(w1[0]);
  ob_touch(wd);
  wd->driver = w1[0]->driver;
  
  w2[0]->driver = wire_driver(w2[0]);
  wd = wire_drivee(w2[0]);
  ob_touch(wd);
  wd->driver = w2[0]->driver;
  net_setSize(w1[0]->net,dwidth);
  net_setSize(w2[0]->net,dwidth);
}

void join_splice_middle(GWireNode *n,GWire *w1[],GWire *w2[],int x,int y,int dwidth,EditState *es)
{
  GWire *pw = wirenode_driver(n);

  w1[0] = wire_newend(es->env,pw->net,1);
  w2[0] = wire_newend(es->env,pw->net,1);

  ob_touch(w1[0]);
  ob_touch(w2[0]);
  ob_touch(w1[0]->nodes);
  ob_touch(w2[0]->nodes);

  if ((n->x == n->out->x) && (n->y != n->out->y)) {
    w2[0]->nodes->x = w1[0]->nodes->x = n->x;
    w2[0]->nodes->y = w1[0]->nodes->y = y;
  } else
    if ((n->y == n->out->y) && (n->x != n->out->x)){
      w2[0]->nodes->x = w1[0]->nodes->x = x;
      w2[0]->nodes->y = w1[0]->nodes->y = n->y;
    } else {
      w2[0]->nodes->x = w1[0]->nodes->x = x;
      w2[0]->nodes->y = w1[0]->nodes->y = y;
      logError(ERL_WARN,"Bizzare attachment in join_splice_middle.");
    }

  ob_touch(n);
  ob_touch(n->out);

  w1[0]->nodes->out = n->out;
  n->out->in = w1[0]->nodes;

  w2[0]->nodes->in = n;
  n->out = w2[0]->nodes;

  w1[0]->driver = w1[0];
  wirenode_drivee(w1[0]->nodes)->driver = w1[0];

  w2[0]->driver = wirenode_driver(w2[0]->nodes);
  net_setSize(w1[0]->net,dwidth);
  net_setSize(w2[0]->net,dwidth);
}



void join_middle_out(GWire *w,GWireNode *n,EditState *es,int size)
{
  struct wire *nw1,*nw2;

  join_splice_middle(n,&nw1,&nw2,w->nodes->x,w->nodes->y,size,es);
  join_treereverse(nw2);
  joint_make(nw1->nodes->x,nw1->nodes->y,nw1,nw2,w,NULL,es);
  joint_fixwires(w->gate,w,0);
  wire_snapgate(w->gate,0);
#ifdef JOIN_DRAW
  gate_draw(w->gate,0);
#endif
}

void join_middle_in(GWire *w,GWireNode *n,EditState *es,int size)
{
  GWire *nw1,*nw2;

  join_splice_middle(n,&nw1,&nw2,w->nodes->x,w->nodes->y,size,es);
  joint_make(nw1->nodes->x,nw1->nodes->y,nw1,nw2,w,NULL,es);
  joint_fixwires(w->gate,w,0);
  wire_snapgate(w->gate,0);
#ifdef JOIN_DRAW
  gate_draw(w->gate,0);
#endif
}

void join_corner_out(GWire *w,GWireNode *n,EditState *es,int size)
{
  GWire *nw1,*nw2;

  join_splice_corner(n,&nw1,&nw2,size,es);
  join_treereverse(nw2);
  joint_make(nw1->nodes->x,nw1->nodes->y,nw1,nw2,w,NULL,es);
  joint_fixwires(w->gate,w,0);
  wire_snapgate(w->gate,0);
#ifdef JOIN_DRAW
  gate_draw(w->gate,0);
#endif
}

void join_corner_in(GWire *w,GWireNode *n,EditState *es,int size)
{
  GWire *nw1,*nw2;

  join_splice_corner(n,&nw1,&nw2,size,es);
  joint_make(nw1->nodes->x,nw1->nodes->y,nw1,nw2,w,NULL,es);
  joint_fixwires(w->gate,w,0);
  wire_snapgate(w->gate,0);
#ifdef JOIN_DRAW
  gate_draw(w->gate,0);
#endif
}

/*
 * Joins a wire endpoint w (selected by soldering iron), to a point
 * n on another wire.  If the join results in a bit selector being
 * created, the GCElement for it is returned.
 */
GCElement *join_wires(GWire *w,GWireNode *n,EditState *es)
{
  GWire *splicewire;
  GCElement *g = 0;

  splicewire = wirenode_driver(n);
  if (splicewire->net->n_nbits < w->net->n_nbits) return g;
 
  if ((n->stype == VERTICAL)||(n->stype == HORIZONTAL)) {
    if (!w->nodes->out) {
      join_middle_out(w,n,es,splicewire->net->n_nbits);
      if (splicewire->net->n_nbits != w->net->n_nbits) {
	tap_transmute(w,es);
	g = w->gate;
	wire_finalizeNet(splicewire);
      }
      return g;
    } else {
      join_middle_in(w,n,es,splicewire->net->n_nbits);
      if (splicewire->net->n_nbits != w->net->n_nbits) {
	tap_transmute(w,es);
	g = w->gate;
	wire_finalizeNet(splicewire);
      }
      return g;
    } 
  } else {
    if (!n->end) {
      if (!w->nodes->out) {
	if (splicewire->net->n_nbits == w->net->n_nbits) {
	  join_corner_out(w,n,es,splicewire->net->n_nbits);
	  return g;
	}
      } else {
	if (splicewire->net->n_nbits == w->net->n_nbits) {
	  join_corner_in(w,n,es,splicewire->net->n_nbits);
	  return g;
	}
      }
    }
  }
  GWire_snap(w->driver);
  return g;
}
