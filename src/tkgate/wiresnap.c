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
/*##########################################################################*
 *
 *	Functions in this file are used to adjust wires to make them more
 *	astheticlly pleasing.  Specifically, kinks in wires are removed and
 *	directions of wires entering gates are checked.
 *
 *
 *	Major functions in this module are:
 *
 *	GWire_snap(GWire*)
 *	wire_snapgate(GCElement*,int)
 *##########################################################################*/
#include "tkgate.h"

#define crosspoint(x1,x2,x) (((x >= x1) && (x <= x2)) || ((x >= x2) && (x <= x1)))

#define vertical(n,m) (((n)->x == (m)->x) && ((n)->y != (m)->y))
#define horizontal(n,m) (((n)->x != (m)->x) && ((n)->y == (m)->y))
#define zerolength(n,m) (((n)->x == (m)->x) && ((n)->y == (m)->y))

/*****************************************************************************
 *
 * Snap all of the wires on a pad
 *
 * Parameters:
 *    w			Wire representing the head wire of a pad 
 *
 *****************************************************************************/
static void wire_snaplist(GWire *w,int doRedraw)
{
  for (;w;w = w->next) {
    if (doRedraw) GWire_draw(w->driver);
    GWire_snap(w->driver);
    if (doRedraw) GWire_draw(w->driver);
  }
}

/*****************************************************************************
 *
 * Force a wire to leave a gate in a specified direction
 *
 * Parameters:
 *    w			Wire to be forced
 *    d			Direction to force it to.
 *    retry		This is a retry of an earlier failed attempt
 *
 * Returns: 		Magic number.
 *
 *****************************************************************************/
int wire_force(GWire *w,int d,int retry)
{
  int mod,od;
  GWireNode *n,*on;

  mod = 0;
  n = w->nodes;
  on = (n->out ? n->out : n->in);
  if (zerolength(n,on)) {
    mod = 7;
    switch (d) {
    case 0 :
      wire_move(on,retry ? 5 : 10,0,VERTICAL | HORIZONTAL);
      break;
    case 1 :
      wire_move(on,0,retry ? -5 : -10,VERTICAL | HORIZONTAL);
      break;
    case 2 :
      wire_move(on,retry ? -5 : -10,0,VERTICAL | HORIZONTAL);
      break;
    case 3 :
      wire_move(on,0,retry ? 5 : 10,VERTICAL | HORIZONTAL);
      break;
    }
  }
  else
    if (d != wireorient(w->nodes,0)) {
      mod = 8;
      n = new_GWireNode();
      ob_touch(n);
      n->out = new_GWireNode();
      ob_touch(n->out);
      n->out->in = n;

      n->x = n->out->x = w->nodes->x;
      n->y = n->out->y = w->nodes->y;

      if (w->nodes->out) {
	n->out->out = w->nodes->out;
	n->out->out->in = n->out;
	n->in = w->nodes;
	ob_touch(n->in);
	n->in->out = n;
	on = n;
	n = n->out;
	if (n->out && n->out->out)
	  od = wireorient(w->nodes,3);
	else
	  od = 0;
      } else {
	n->in = w->nodes->in;
	ob_touch(n->in);
	n->in->out = n;
	n->out->out = w->nodes;
	ob_touch(w->nodes);
	w->nodes->in = n->out;
	on = n;
	n = n->in;
	if (n && n->in)
	  od = wireorient(w->nodes,3);
	else
	  od = 0;
      }

      switch (d) {
      case 0 :
	wire_move(on,w->nodes->x + 10 - on->x,0,HORIZONTAL);
	if (od == 1)
	  wire_move(n,0,retry ? -7 : -15,VERTICAL);
	else
	  wire_move(n,0,retry ? 7 : 15,VERTICAL);
	break;
      case 1 :
	wire_move(on,0,w->nodes->y - 10 - on->y,VERTICAL);
	if (od == 2)
	  wire_move(n,retry ? -7 : -15,0,HORIZONTAL);
	else
	  wire_move(n,retry ? 7 : 15,0,HORIZONTAL);
	break;
      case 2 :
	wire_move(on,w->nodes->x - 10 - on->x,0,HORIZONTAL);
	if (od == 1)
	  wire_move(n,0,retry ? -7 : -15,VERTICAL);
	else
	  wire_move(n,0,retry ? 7 : 15,VERTICAL);
	break;
      case 3 :
	wire_move(on,0,w->nodes->y + 10 - on->y,VERTICAL);
	if (od == 2)
	  wire_move(n,retry ? -7 : -15,0,HORIZONTAL);
	else
	  wire_move(n,retry ? 7 : 15,0,HORIZONTAL);
	break;
      }
    }

  return mod;
}

/*
  Does wire_snap on all wires connected to a gate.
  */
void wire_snapgate(GCElement *g,int doRedraw)
{
  int i;
  int N = GCElement_numPads(g);

  for (i = 0;i < N;i++)
    wire_snaplist(g->wires[i],doRedraw);
}

/*
  This function makes sure that the wire end 'w' enters the gate it is
  connected to from the right direction.  If not, it calls the appropriate
  function to adjust the wire.
  */
GWireNode *wire_snapendgate(GWire *w,int *mod,int retry)
{
  GCElement *g = w->gate;
  GGateInfo *gi = g->typeinfo;

#if 0
  printf("wire_snapendgate on %s::%s\n",w->gate->typeinfo->name,w->gate->ename);
#endif

  if (gi->WireSnap)
    return (*gi->WireSnap)(g,w,mod,retry);
  else
    return Generic_wireSnap(g,w,mod,retry);
}

/*****************************************************************************
 *
 * If the destination node is unlabeled, copy label props. from the source.
 *
 * Parameters:
 *      dn 		Destination node
 *      sn 		Source node
 *
 *****************************************************************************/
static void GWireNode_suggestLabelProps(GWireNode *dn,GWireNode *sn)
{
  if (!dn) return;
  if (dn->isLabeled) return;
  if (!sn->isLabeled) return;

  dn->isLabeled = 1;
  dn->labelSide = sn->labelSide;
  dn->offset = sn->offset;
}


/*****************************************************************************
 *
 * Snaps out short segments of wire with mobile nodes. 
 *
 * Parameters:
 *      n			Wire node to snap
 *      mod			
 *      retry
 *
 *****************************************************************************/
static int wire_snap_aux(GWireNode *n,int mod,int retry)
{
  GWireNode *t;

  if (!n || !n->out) 
    return mod;

    /* Make sure that wires arround a joint look OK */
  if (n->end && n->end->gate) {
    n = wire_snapendgate(n->end,&mod,retry);
  } else
    if (n->out->end && n->out->end->gate) {
      n = wire_snapendgate(n->out->end,&mod,retry);
      n = n->in;
    }

  if (abs(n->x - n->out->x) + abs(n->y - n->out->y) >= SNAPSIZE)
    return wire_snap_aux(n->out,mod,retry);

  if (n->out) ob_touch(n->out);
  if (n->in) ob_touch(n->in);


  if (n->in && n->out->out && (n->x == n->out->x) && (n->y == n->out->y)) {
    struct wirenode *nt;

    nt = n;
    n->in->out = n->out->out;
    n->out->out->in = n->in;
    n = n->in->out;
    GWireNode_suggestLabelProps(n,nt->out);
    GWireNode_suggestLabelProps(n,nt);
    delete_GWireNode(nt->out);
    delete_GWireNode(nt);
  } else {
    if ((!anchoredp(n->out)) && !(n->out->out && anchoredp(n->out->out)) && n->in) {
      if (!n->out->out) {
	n->out->x = n->x;
	n->out->y = n->y;
	n->in->out = n->out;
	n->out->in = n->in;
	GWireNode_suggestLabelProps(n->in,n);
	delete_GWireNode(n);
	return 1;
      } else {
	if (n->in) {
	  if ((n->y == n->out->y) && (n->x != n->out->x)) {	/* General case snap */
	    ob_touch(n->out->out);
	    n->out->out->x = n->x;
	  } else {
	    if ((n->x == n->out->x) && (n->y != n->out->y)) {
	      ob_touch(n->out->out);
	      n->out->out->y = n->y;
	    }
	  }

	  n->in->out = n->out->out;
	  ob_touch(n->out->out);
	  n->out->out->in = n->in;
	  t = n;
	  n = n->in;
	  GWireNode_suggestLabelProps(n,t);
	  GWireNode_suggestLabelProps(n,t->out);
	  delete_GWireNode(t->out);
	  delete_GWireNode(t);
	  mod = 2;
	} else if (!anchoredp(n)) {
	  n->out->end = n->end;
	  ob_touch(n->end);
	  n->end->nodes = n->out;
	  n->out->in = NULL;
	  GWireNode_suggestLabelProps(n->in,n);
	  delete_GWireNode(n);
	  return 3;
	}
      }
    } else
      if (n->out->out) {
	if (!anchoredp(n) && (n->in ? !anchoredp(n->in) : 0) ) {
	  if ((n->in->y == n->y) && (n->in->x != n->x))
	    n->in->y = n->out->y;
	  else
	    if ((n->in->x == n->x) && (n->in->y != n->y))
	      n->in->x = n->out->x;
	  ob_touch(n->out->out);
	  n->in->out = n->out->out;
	  n->out->out->in = n->in;
	  t = n->in;
	  GWireNode_suggestLabelProps(t,n);
	  GWireNode_suggestLabelProps(t,n->out);
	  delete_GWireNode(n->out);
	  delete_GWireNode(n);
	  n = t;
	  mod = 4;
	}  else
	  if (!n->in && !anchoredp(n)) {
	    ob_touch(n);
	    n->x = n->out->x;
	    n->y = n->out->y;
	    t = n->out;
	    n->out = n->out->out;
	    ob_touch(n->out);
	    n->out->in = n;
	    GWireNode_suggestLabelProps(n->in,t);
	    delete_GWireNode(t);
	    mod = 5;
	  }
      }
  }
  return wire_snap_aux(n->out,mod,retry);
}

/*
  Return TRUE if the segments [n1 : n1->out] and [n2 : n2->out] are crossed.
*/
static int GWireNode_iscrossed(GWireNode *n1,GWireNode *n2)
{
  if (horizontal(n1,n1->out) && vertical(n2,n2->out))
    return crosspoint(n1->x,n1->out->x,n2->x) && crosspoint(n2->y,n2->out->y,n1->y);
  else if (horizontal(n2,n2->out) && vertical(n1,n1->out))
    return crosspoint(n2->x,n2->out->x,n1->x) && crosspoint(n1->y,n1->out->y,n2->y);
  else
    return 0;
}

/*****************************************************************************
 *
 * Remove a loop between node c and node n
 *
 * Parameters:
 *      n		Initial node on the wire to check for loops
 *
 *****************************************************************************/
static void GWireNode_removecross(GWireNode *c,GWireNode *n)
{
  ob_touch(n);

  if (horizontal(c,c->out))
    n->y = c->y;
  else
    n->x = c->x;

  ob_touch(n->in);
  n->in->out = NULL;
  GWireNode_freenodelist(c->out);

  ob_touch(c);
  c->out = n;
  n->in = c;
}

/*****************************************************************************
 *
 * Detects and removes loops from a wire.
 *
 * Parameters:
 *      n		Initial node on the wire to check for loops
 *
 *****************************************************************************/
static int GWireNode_unloopy(GWireNode *n)
{
  GWireNode *c,*start;
  int removed;

  removed = 0;
  start = n;
  for (n = n->out;n && n->out;n = n->out)
    for (c = start;c->out != n;c = c->out)
      if (GWireNode_iscrossed(c,n)) {
	GWireNode_removecross(c,n);
	removed = 1;
	break;
      }

  return removed;
}

/*
  Try to fix wire up to five times.  If we still can't get it right report
  what went wrong.
*/
void GWire_snap(GWire *w)
{
  GWireNode *n;
  int r[5];
  int i;

  for (i = 0;i < 5;i++) {
    n = w->nodes;
    GWireNode_unloopy(n);
    r[i] = wire_snap_aux(w->nodes,0, (i>3));
    if (r[i] == 0) return;
  }

  logError(ERL_ERROR,"Wire Snap failed %d %d %d %d %d in wire_snap.",
	   r[0],r[1],r[2],r[3],r[4]);

}

