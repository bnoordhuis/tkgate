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
****************************************************************************/

/*****************************************************************************
 *    Functions that change the structure of a wire are defined here.  This
 *    include routines for cutting and attaching wires.
 *
 *   wireorient(n,d)		Get orientation of a wire
 *   wire_root_ok(w)		Test if this wire can be a root.
 *   wire_choose_root(w,rw)     Choose best of two wires as root
 *   wire_sigroot(w)		Choose best wire for root from connected tree.
 *   setIoNet(net,g)		Mark a net as an I/O for element g
 *   wire_findIdx(w,d)		Find the the wire with index d
 *   wire_chooseUniqueNet(dw)	Choose a unique net for a newly merged wire.
 *   wire_replaceNet(w,n)	Replace the net of a wire.
 *   GWire_sizeisdisplayed(w)	Returns true if w has any segments displaying the size 
 *   wire_finalizeNet(w)	Do all finalization of a wire after it has been modified
 *   wire_findClosest(w,x,y)	Find other wire end closest to w at (x,y)
 *   wire_solder(n1,n2)		Connect two nodes together
 *   wire_connect(M,w1,w2)	Connect two wires together
 *   wire_changedriver(n,w)	Change the driver of a node to w
 *   GWireNode_setends(n)	Set the ends of a wire.
 *   wirenode_cutcorner(n,M)    Cut a wire at a corner.
 *   wirenode_cutsegment(n,M)	Cut a wire in the middle of a segment.
 *   wire_cut(x,y,n,M)		Cut the wire at node n at position (x,y)
 *   wire_shorten(w,M,dp)	Shorten a wire after a cut
 *   wire_nuke(w,dp,M)		Try to get rid of a wire if it is not attached to anything
 *   wire_trash(n,M,dp)		Trash a wire
 *   ...more...
 *
 *   Helping functions for primary functions above:
 *
 *   wire_sigroot_aux(w,rw)     
 *   wire_replaceNetAux(w,net)     
 *   wire_trashin(n,M,dp)	Trash a wire in the "in" direction.
 *   wire_trashout(n,M,dp)	Trash a wire in the "out" direction.
 *
 *****************************************************************************/
#include "tkgate.h"

#define DEBUG_CUT 0


/* Wire snap cases */
#define HH 0x3
#define VV 0xC
#define HV 0x6
#define VH 0x9
#define HZ 0x1
#define ZH 0x2
#define VZ 0x4
#define ZV 0x8

/*
  Return the orientation of the segment 'd' in from end 'n'.
  */
int wireorient(GWireNode *n,int d)
{
  int r;		/* Direction of wire (in/out) */
  GWireNode *on;	/* Other end of segment */

  r = (n->in != NULL);

  for (;d;d--)
    if (r)
      n = (n->in ? n->in : n);
    else
      n = (n->out ? n->out : n);

  on = (r ? (n->in ? n->in : n) : (n->out ? n->out : n));

  if (n->x == on->x) {
    if (n->y < on->y)
      return 3;
    else
      if (n->y > on->y)
	return 1;
      else
	return -1;
  } else
    if (n->x < on->x)
      return 0;
    else
      return 2;
}

/*
 * return non-zero if this wire is a valid root wire.
 */
static int wire_root_ok(GWire *w)
{
  if (!w->gate) return 1;
  if (w->gate->typeinfo->Code != GC_JOINT && w->gate->typeinfo->Code != GC_TAP) return 1;
  if (w->gate->typeinfo->Code == GC_TAP && w == w->gate->wires[TAP_TAP]) return 1;

  return 0;
}

/*
 * Compare w and *root_w to determine which is the better
 * root representative.  If w is better than *root_w, then
 * *root_w is set to w.
 */
static void wire_choose_root(GWire *w,GWire **root_w)
{
  if (!wire_root_ok(w)) return;		/* w must at least be a valid root */

  if (!*root_w) {			/* If no current root, then make w the root */
    *root_w = w;
    return;
  } 

  /*
   * Ideally, the root should be a driver of a segment.  If both w and
   * *root_w have the same driver status, then use the address to determine
   * the representative.
   */
  if ((w->driver == w && (*root_w)->driver == *root_w)
      || (w->driver != w && (*root_w)->driver != *root_w)) {
    if (w < *root_w) *root_w = w;
  } else if (w->driver == w && (*root_w)->driver != *root_w) {
    *root_w = w;
  }
}

/*
 * Recursively scan all wires in the tree starting at w and choose
 * a representative wire. The representative wire choosen for any
 * wire on a net is the same for all wires on the net.
 */
static void wire_sigroot_aux(GWire *w,GWire **root_w)
{
  int i;

  wire_choose_root(w,root_w);
  if (!w->gate) return;

  switch (w->gate->typeinfo->Code) {
  case GC_JOINT :
    for (i=0;i<4;i++)
      if (w->gate->wires[i] && w->gate->wires[i] != w) {
	GWire *xw = w->gate->wires[i];
	wire_choose_root(xw,root_w);
	wire_sigroot_aux(wire_other(xw),root_w);
      }
    break;
  case GC_TAP :
    if (w->gate->wires[TAP_OUT] == w) {
      GWire *xw = w->gate->wires[TAP_IN];
      wire_choose_root(xw,root_w);
      wire_sigroot_aux(wire_other(xw),root_w);
    }
    if (w->gate->wires[TAP_IN] == w) {
      GWire *xw = w->gate->wires[TAP_OUT];
      wire_choose_root(xw,root_w);
      wire_sigroot_aux(wire_other(xw),root_w);
    }
    break;
  }
}

/*
   Return the representative wire end with the signal name for w.
   The represent wire end must not be connected to a joint or
   the IN/OUT ports of a tap.
*/
GWire *wire_sigroot(GWire *w)
{
  GWire *root_w = 0;
  
  wire_sigroot_aux(w,&root_w);
  wire_sigroot_aux(wire_other(w),&root_w);

  if (!root_w) {
    logError(ERL_ERROR,"failed to find valid wire root.");
    return w;
  }

  return root_w;
}


/*****************************************************************************
 *
 * Helping function to search a net through its wire for an I/O net
 *
 *****************************************************************************/
GCElement *GWire_findIoGate_aux(GWire *w)
{
  int i;
  GCElement *g = 0;

  if (!w->gate) return 0;

  /*
   * Found an I/O gate.
   */
  if (w->gate->typeinfo->Flags.IsIOnet)
    return w->gate;

  switch (w->gate->typeinfo->Code) {
  case GC_JOINT :
    for (i=0;i<4;i++)
      if (w->gate->wires[i] && w->gate->wires[i] != w) {
	GWire *xw = w->gate->wires[i];
	g = GWire_findIoGate_aux(wire_other(xw));
	if (g) return g;
      }
    break;
  case GC_TAP :
    if (w->gate->wires[TAP_OUT] == w) {
      GWire *xw = w->gate->wires[TAP_IN];

      g = GWire_findIoGate_aux(wire_other(xw));
      if (g) return g;
    }
    if (w->gate->wires[TAP_IN] == w) {
      GWire *xw = w->gate->wires[TAP_OUT];

      g = GWire_findIoGate_aux(wire_other(xw));
      if (g) return g;
    }
    break;
  }

  return 0;
}

/*****************************************************************************
 *
 * Search a net through its wire for an I/O net
 *
 *****************************************************************************/
GCElement *GWire_findIoGate(GWire *w)
{
  GCElement *g;

  g = GWire_findIoGate_aux(w);
  if (g) return g;

  return GWire_findIoGate_aux(wire_other(w));
}

/*****************************************************************************
 *
 * Attempt to set the I/O gate of net.  If g is not an I/O net, then it does
 * nothing.
 *
 *****************************************************************************/
static void setIoNet(GNet *net,GCElement *g)
{
  if (!g) return;
  if (g->typeinfo->Flags.IsIOnet) {
    ob_touch(net);
    net->n_ionet = g;
  }
}

/*****************************************************************************
 *
 * Helping function to replace the net of a wire.
 *
 *****************************************************************************/
static void wire_replaceNetAux(GWire *w,GNet *net)
{
  GWire *sw;
  int i;

  sw = w;
  w = wire_drivee(w);
  if (w == sw) {
    join_wirereverse(w);
    sw = wire_driver(w);
    w  = wire_drivee(w);
  }

  wire_setNet(sw,net);
  if (net) setIoNet(net,sw->gate);
  wire_setNet(w,net);
  if (net) {
    ob_touch(sw);
    ob_touch(w);
    ob_touch(net);
    setIoNet(net,w->gate);
    sw->nidx = net->n_wnum++;
    w->nidx = net->n_wnum++;
  }

#if 0
  if (net)
    printf("   wire_replaceNetAux(%s) [0x%x]=%d [0x%x]=%d\n",
	   net->n_signame,sw,sw->nidx,w,w->nidx);
#endif

  if (w->gate)
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      if (net) {
	ob_touch(net);
	ob_touch(w->gate);
	w->gate->u.joint.gidx = net->n_gnum++;
      }
      for (i=0;i<4;i++)
	if (w->gate->wires[i] && w->gate->wires[i] != w)
	  wire_replaceNetAux(w->gate->wires[i],net);
      break;
    case GC_TAP :
      if (w->gate->wires[TAP_TAP] == w) break;
      if (net) {
	ob_touch(net);
	ob_touch(w->gate);
	w->gate->u.tap.gidx = net->n_gnum++;
      }
      if (w->gate->wires[TAP_IN] == w)
	wire_replaceNetAux(w->gate->wires[TAP_OUT],net);
      if (w->gate->wires[TAP_OUT] == w)
	wire_replaceNetAux(w->gate->wires[TAP_IN],net);
      break;
    }
}

GWire *wire_findIdx(GWire *w,int d)
{
  GWire *xw = 0;
  int i;

  if (w->nidx == d) return w;
  w = wire_drivee(w);
  if (w->nidx == d) return w;

  if (w->gate)
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      for (i=0;i<4;i++)
	if (w->gate->wires[i] && w->gate->wires[i]->net && w->gate->wires[i] != w) {
	  xw = wire_findIdx(w,d);
	  if (xw) break;
	}
      break;
    case GC_TAP :
      if (w->gate->wires[TAP_OUT] == w)
	xw = wire_findIdx(w->gate->wires[TAP_IN],d);
      if (w->gate->wires[TAP_IN] == w)
	xw = wire_findIdx(w->gate->wires[TAP_OUT],d);
      break;
    }

  return xw;
}

/*
   Chooses a unique net for a collection of connected wires,
   clears the 'net' field for all wires in the collection,
   deletes all but one of the 'net's, and returns a unique
   net.  A named net will take precidence over unnamed nets
   in selecting a net to return.
*/
GNet *wire_chooseUniqueNet(GWire *dw)
{
  GNet *net = 0;
  GWire *w;
  int i;

  w = wire_drivee(dw);

  net = net_pickOne(dw->net,w->net,0);
  net_incref(net);
  wire_setNet(dw,0);
  wire_setNet(w,0);

  if (w->gate)
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      for (i=0;i<4;i++)
	if (w->gate->wires[i] && w->gate->wires[i]->net)
	  net = net_pickOne(net,wire_chooseUniqueNet(w->gate->wires[i]),1);
      break;
    case GC_TAP :
      if (w != w->gate->wires[TAP_TAP]) {
	if (w->gate->wires[TAP_OUT]->net)
	  net = net_pickOne(net,wire_chooseUniqueNet(w->gate->wires[TAP_OUT]),1);
	if (w->gate->wires[TAP_IN]->net)
	  net = net_pickOne(net,wire_chooseUniqueNet(w->gate->wires[TAP_IN]),1);
      }
      break;
    }
  return net;
}

/*
   Replace the net on all connected components of w.  The old
   net is *not* deleted.
*/
void wire_replaceNet(GWire *w,GNet *net)
{
  w = wire_sigroot(w);
  wire_replaceNetAux(w,net);
}

static int GWire_sizeisdisplayedonnode(GWire *w)
{
  GWireNode *n;

  w = wire_driver(w);

  for (n = w->nodes;n;n = n->out)
    if (n->showSize)
      return 1;

  return 0;
}

static int GWire_sizeisdisplayed_aux(GWire *w,int isFirst)
{
  int i;

  w = wire_other(w);

  if (!isFirst) {
    if (GWire_sizeisdisplayedonnode(w))
      return 1;
  }

  if (w->gate) {
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      for (i=0;i<4;i++)
	if (w->gate->wires[i] && (w->gate->wires[i] != w))
	  GWire_sizeisdisplayed_aux(w->gate->wires[i],0);
      break;
    case GC_TAP :
      if (w == w->gate->wires[TAP_OUT])
	GWire_sizeisdisplayed_aux(w->gate->wires[TAP_IN],0);
      else if (w == w->gate->wires[TAP_IN])
	GWire_sizeisdisplayed_aux(w->gate->wires[TAP_OUT],0);
      break;
    }
  }
  return 0;
}

int GWire_sizeisdisplayed(GWire *w)
{
  GWire *o_w = wire_other(w);

  if (GWire_sizeisdisplayed_aux(w,0))
    return 1;
  if (GWire_sizeisdisplayed_aux(o_w,1))
    return 1;

  return 0; 
}

/*****************************************************************************
 *
 * Do final processing on a wire after it has been touched by editing functions.
 *
 * Parameters:
 *      w		A wire on the net to be finalized.
 *
 * Choose a final net for a wire.  If there is more than one net on
 * a collection of wires choose one and delete the others.
 *
 *****************************************************************************/
void wire_finalizeNet(GWire *w)
{
  GNet *net;

#if 0
  printf("wire_finalizeNet(0x%x (%s))\n",w,w->net->n_signame);
#endif

  w = wire_sigroot(w);
  net = wire_chooseUniqueNet(w);

  ob_touch(net);
  net->n_finalized = 0;
  net->n_wnum = 0;
  net->n_gnum = 0;
  net->n_ionet = 0;
  wire_replaceNetAux(w,net);
  net->n_driver = w;

  net->n_finalized = 1;
  net_decref(net);

  /*
   * Make sure at least one wire segment is marked to display the bit size.
   */
  if (!GWire_sizeisdisplayed(net->n_driver))
    net->n_driver->nodes->showSize = 1;

  /*
   * Make sure name is visible if necessary.
   */
  GNet_checkNameVisibility(net,0);
}

/*****************************************************************************
 *
 * Find wire end closest to (x,y) on net rooted at w.
 *
 * Parameters:
 *      w		Wire on which to search
 *      x,y		Target coordinate pair
 *
 *****************************************************************************/
GWire *wire_findClosest(GWire *w,int x,int y)
{
  GWire *ow = 0;
  GWire *xw = 0;
  GWire *bw;
  int i;

  if (!w) return 0;

  ow = wire_other(w);
  bw = w;
  if (wdist(ow,x,y) < wdist(bw,x,y))
    bw = ow;

  if (ow->gate) {
    GCElement *g = ow->gate;
    switch (g->typeinfo->Code) {
    case GC_JOINT :
      for (i = 0;i < 4;i++) 
	if (g->wires[i] != ow) {
	  xw = wire_findClosest(g->wires[i],x,y);
	  if (xw && wdist(xw,x,y) < wdist(bw,x,y))
	    bw = xw;
	}
      break;
    case GC_TAP :
      if (g->wires[TAP_IN] == ow) {
	xw = wire_findClosest(g->wires[TAP_OUT],x,y);
      } else if (g->wires[TAP_OUT] == ow) {
	xw = wire_findClosest(g->wires[TAP_IN],x,y);
      } else
	xw = 0;
      if (xw && wdist(xw,x,y) < wdist(bw,x,y))
	bw = xw;
      break;
    }
  } 

  return bw;
}


/*
   Low-level routine for connecting two wire nodes n1 and n2
*/
void wire_solder(GWireNode *n1,GWireNode *n2)
{
  int t;

  ob_touch(n1);
  ob_touch(n2);

  if (n1->out || n2->in) printf("Ack!\n");
  t = horizontal(n1,n1->in) | (horizontal(n2,n2->out) <<1) |
    (vertical(n1,n1->in)<<2) | (vertical(n2,n2->out)<<3);

  switch (t) {
  case HH :
  case VV :
    if ((n1->x != n2->x) || (n1->y != n2->y)) { /* Couldn't move a node */
      n1->out = n2;				  /* Don't remove bend */
      n2->in = n1;
      debugprint("*click* (case 1)\n");
    } else {					  /* General case */
      ob_touch(n1->in);
      ob_touch(n2->out);

      n1->in->out = n2->out;
      n2->out->in = n1->in;
      delete_GWireNode(n1);
      delete_GWireNode(n2);
      debugprint("*click* (case 2)\n");
    }
    break;
  case HV :
  case VH :
    ob_touch(n2->out);

    n1->out = n2->out;
    n2->out->in = n1;
    delete_GWireNode(n2);
    debugprint("*click* (case 3)\n");
    break;
  case ZH :
  case ZV :
    if (n1->in->in) {
      ob_touch(n1->in);

      n1->in->out = NULL;
      debugprint("*subclick* (case 4)\n");
      wire_solder(n1->in,n2);
      delete_GWireNode(n1);
    } else {
      ob_touch(n2->out);

      n1->out = n2->out;
      n2->out->in = n1;
      delete_GWireNode(n2);
      logError(ERL_ERROR,"Out of wire case 4 in wire_solder.");
    }
    break;
  case HZ :
  case VZ :
    if (n2->out->out) {
      ob_touch(n2->out);

      n2->out->in = NULL;
      debugprint("*subclick* (case 5)\n");
      wire_solder(n1,n2->out);
      delete_GWireNode(n2);
    } else {
      ob_touch(n2->out);

      n1->out = n2->out;
      n2->out->in = n1;
      logError(ERL_ERROR,"Out of wire case 5 in wire_solder.");
    }
    break;
  default :
    n1->out = n2;
    n2->in = n1;
    logError(ERL_FATAL,"Weird connection in wire_solder.");
  }
}

/*
   Connects two wires into a single wire.
*/
GWire *wire_connect(GModuleDef *M,GWire *w1,GWire *w2)
{
  GWire *nw;
  GWireNode *n1,*n2;

  if (w1->net->n_nbits != w2->net->n_nbits) return 0;

  if (w1->nodes->out)
    join_treereverse(w1);
  if (w2->nodes->in)
    join_treereverse(w2);

  n1 = w1->nodes;
  n2 = w2->nodes;

  ob_touch(w1->nodes);
  ob_touch(w2->nodes);

  w1->nodes->stype = FULL;
  w2->nodes->stype = FULL;

  if (!anchoredp(n1->in)) {
    wire_move(w1->nodes,w2->nodes->x - w1->nodes->x,
	      w2->nodes->y - w1->nodes->y,w1->nodes->stype);
  } else {
      wire_move(w2->nodes,w1->nodes->x - w2->nodes->x,
		w1->nodes->y - w2->nodes->y,w2->nodes->stype);
  }
  w1->nodes->end = NULL;
  w2->nodes->end = NULL;

  nw = wire_drivee(w2);
  ob_touch(nw);
  nw->driver = wire_driver(w1);
  nw = nw->driver;

  wire_solder(w1->nodes,w2->nodes);

  wire_finalizeNet(nw);

  ob_touch(M);
  M->m_wires = wire_unlink(M->m_wires,w1);
  M->m_wires = wire_unlink(M->m_wires,w2);

  wire_free(w1);
  wire_free(w2);

  return nw->driver;
}


/*****************************************************************************
 *
 * Changes the driver of a node 
 *
 * Parameters:
 *      n		Node of wire on which to make change
 *      w		New driving wire
 *
 *****************************************************************************/
void wire_changedriver(GWireNode *n,GWire *w)
{
  if (n == NULL) return;

  if (n->end) {
    ob_touch(n->end);
    n->end->driver = w;
  } else
    wire_changedriver(n->out,w);
}

void GWireNode_freeOut(GWireNode *n)
{
  if (!n) return;
  GWireNode_freeOut(n->out);
  delete_GWireNode(n);
}

void GWireNode_setends(GWireNode *n)
{
  GWire *w;

  w = wirenode_drivee(n);
  ob_touch(w);
  w->driver = wirenode_driver(n);
  ob_touch(w->driver);
  w->driver->driver = w->driver;

  w->wtype = DRIVEE;
  w->driver->wtype = DRIVER;
}

void GWire_pickRenameWire(GWire *w1,GWire *w2,GModuleDef *M)
{
  GNet *net = new_GNet_compatable(0,w1->net,M);
  GCElement *g1 = GWire_findIoGate(w1);
  GCElement *g2 = GWire_findIoGate(w2);

  if (g1 && !g2)
    wire_replaceNet(w2,net);
  else
    wire_replaceNet(w1,net);
}


/*
   Called when the wire clippers are used to cut a wire on a corner.
*/
GWireNode *wirenode_cutcorner(GWireNode *n,GModuleDef *M)
{
  GWireNode *cn;
  GWire *w = wirenode_driver(n);

  cn = new_GWireNode();
  ob_touch(cn);
  cn->in = n->in;
  ob_touch(cn->in);
  cn->in->out = cn;
  cn->out = NULL;
  ob_touch(n);
  n->in = NULL;

  cn->end = wire_newend(M,w->net,0);
  ob_touch(cn->end);
  cn->end->nodes = cn;

  n->end = wire_newend(M,w->net,0);
  ob_touch(n->end);
  n->end->nodes = n;

  cn->x = n->x;
  cn->y = n->y;

  GWireNode_setends(n);
  GWireNode_setends(cn);

  /*
   * Decide which net to rename.
   */
  GWire_pickRenameWire(n->end,cn->end,M);

  return cn;
}


GWireNode *wirenode_cutsegment(int x,int y,GWireNode *n,GModuleDef *M)
{
  GWireNode *cn1,*cn2;
  GWire *w = wirenode_driver(n);

  cn1 = new_GWireNode();
  cn2 = new_GWireNode();	
  
  ob_touch(cn1);
  ob_touch(cn2);
  if (n->x == n->out->x) {
    cn1->x = cn2->x = n->x;
    cn1->y = cn2->y = y;
  } else {
    cn1->x = cn2->x = x;
    cn1->y = cn2->y = n->y;
  }
  
  cn1->in = n;
  cn2->out = n->out;
  n->out = cn1;
  ob_touch(cn2->out);
  cn2->out->in = cn2;

  cn1->end = wire_newend(M,w->net,0);
  cn2->end = wire_newend(M,w->net,0);

  ob_touch(cn1->end);
  ob_touch(cn2->end);

  cn1->end->nodes = cn1;
  cn2->end->nodes = cn2;
  GWireNode_setends(cn1);
  GWireNode_setends(cn2);

  /*
   * Decide which side to rename.
   */  

  GWire_pickRenameWire(cn1->end,cn2->end,M);

  return cn2;
}

static int verify_addr(void *p)
{
  return (p) != (void*)0xa7a7a7a7;
}

static void verify_nodes(const char *place,GWireNode *n)
{
  if (!verify_addr(n->in)) {
    printf("%s: bad wire node in\n",place);
    return;
  }
  if (!verify_addr(n->out)) {
    printf("%s: bad wire node in\n",place);
    return;
  }
  if (n->out)
    verify_nodes(place,n->out);
}

static void verify_wires(const char *place,GWire *w)
{
  if (!verify_addr(w->net)) {
    printf("%s: bad wire net\n",place);
    return;
  }
  verify_nodes(place,w->nodes);
  if (w->next)
    verify_wires(place,w->next);
}

static void verify_net(const char *place,GNet *n)
{
  if (!verify_addr(n->n_signame)) {
    printf("%s: bad net\n",place);
    return;
  }
  verify_wires(place, n->n_driver);
}

/*****************************************************************************
 *
 * Cut wire near node an at (x,y).
 *
 * Parameters:
 *      x,y          Position at which cut is requested
 *      n            Wire node at which to cut
 *      M            Module that wire is in.
 *
 * NOTE: Assumes that the node has been marked with the type of cut that is
 * to take place using wire_hit, wire_iohit, etc.  The type of cut is stored
 * in the "stype" field of the node and can be  FULL (for a cut at a corner),
 * HORIZONTAL for a cut on a horizontal segment, or VERTICAL for a cut on a
 * vertical segment.
 *
 *****************************************************************************/
void wire_cut(int x,int y,GWireNode *n,GModuleDef *M)
{
  GWireNode *cn;
  GWire *w,*nw;
  int nbits;
  GNet *na = 0, *nb = 0;

  w = wirenode_driver(n);
  nbits = w->net->n_nbits;

  switch (n->stype) {
  case FULL :
    if (n->in && n->out) {
      GNet_draw(w->net);

      cn = wirenode_cutcorner(n,M);

      na = wirenode_driver(n)->net;
      nb = wirenode_driver(cn)->net;
      net_incref(na);
      net_incref(nb);

      if (!wire_shorten(n->end,M,0)) {
	nw = wirenode_drivee(n);
	net_setSize(nw->net,nbits);
	nw = wirenode_driver(n);
	net_setSize(nw->net,nbits);
	wire_finalizeNet(nw);
      }

      if (!wire_shorten(cn->end,M,0)) {
	nw = wirenode_driver(cn);
	net_setSize(nw->net,nbits);
	wire_finalizeNet(nw);
      }
    } else
      if (n->end->gate && (n->end->gate->typeinfo->Code == GC_BLOCK)) {
	GCElement *g = n->end->gate;
	if (block_isIntfProtected(g)) {
	  message(0,msgLookup("err.protintf"),g->u.block.moduleName);
	  return;
	}
	
	SetModified(MF_INTERFACE);

	GNet_draw(w->net);
	na = w->net;
	net_incref(na);
	w = n->end;
	block_deletewire(w);
	wire_nuke(w,0,M);
      }
    break;
  case VERTICAL :
  case HORIZONTAL :
    {
      GWire *w1,*w2;

      GNet_draw(w->net);

      cn = wirenode_cutsegment(x,y,n,M);

      na = wirenode_driver(n)->net;
      nb = wirenode_driver(cn)->net;
      net_incref(na);
      net_incref(nb);
      //    printf("(3) na-sig=%p\n",na->n_signame);


#if DEBUG_CUT
      printf("@5(n=%s): %d\n",n->out->end->net->n_signame,n->out->end->net->n_refs);
      printf("@5(cn=%s): %d\n",cn->end->net->n_signame,cn->end->net->n_refs);
#endif
      w1 = n->out->end;
      if (!wire_shorten(w1,M,0)) {
#if 0
	nw = wirenode_drivee(n);
	net_setSize(nw->net,nbits);
	nw = wirenode_driver(n);
	net_setSize(nw->net,nbits);
	wire_finalizeNet(nw);
#else
	net_setSize(w1->net,nbits);
	wire_finalizeNet(w1);
#endif
      } else {
#if DEBUG_CUT
	printf("ACK 1\n");
#endif
      }

      w2 = cn->end;
      if (!wire_shorten(w2,M,0)) {
	cn = w2->nodes;
	nw = wirenode_driver(cn);
	net_setSize(nw->net,nbits);
	wire_finalizeNet(nw);
      } else {
#if DEBUG_CUT
	printf("ACK 2\n");
#endif
      }
    }
    break;
  }

  /*
   * We create temp. references to these nets so we don't delete them until
   * we have a chance to check the reference count.  If there is only 1
   * reference, then that is just the temp. reference and the net will be
   * deleted as soon as we decrement the ref. count
   */
  if (na && na->n_refs > 1) GNet_draw(na);
  if (nb && nb->n_refs > 1) GNet_draw(nb);
  if (na) net_decref(na);
  if (nb) net_decref(nb);
}

/*
 * Shortens a newly cut wire, totally deletes the wire if it is no longer
 * connected to anything other than a joint. Returns '1' if the wire was
 * deleted, '0' if the wire was retained.
 */
int wire_shorten(GWire *w,GModuleDef *M,int drawp)
{
  GWireNode *n,*on;	/* node and "other" node on segment in question */

  if (drawp) GWire_draw(w->driver);
  if (wire_nuke(w,drawp,M)) {
    //    printf("ws: nuke1\n");
    return 1;
  }

  n = w->nodes;
  on = (n->in ? n->in : n->out);

  if ((abs(n->y - on->y) < 10) && (abs(n->x - on->x) < 10)) {
    if (on->in && on->out) {
      ob_touch(on);
      on->end = n->end;
      ob_touch(on->end);
      on->end->nodes = on;
      if (on->out == n)
	on->out = NULL;
      else
	on->in = NULL;
      //      printf("ws: del\n");
      delete_GWireNode(n);
    }
  } else {
    ob_touch(n);
    if (n->x == on->x) {
      n->y += 5*(n->y < on->y) - 5*(n->y > on->y);	/* Zero if n->y == on->y */
    }
    else {
      n->x += 5*(n->x < on->x) - 5*(n->x > on->x);
    }
  }
  //  printf("ws: ok\n");
  return 0;
}

static int wire_trashin(GWireNode *n,GModuleDef *M,int drawp)
{
  int trashp;

  if (!n) return 0;

  if (!n->in) {
    if (n->end && n->end->gate) {
      switch (n->end->gate->typeinfo->Code) {
      case GC_JOINT :
	return joint_dejoint(n,M,drawp);
      default :
	return 0;
      }
    } else {
      if (M) {
	ob_touch(M);
	M->m_wires = wire_unlink(M->m_wires,n->end);
      }
      wire_free(n->end);
      delete_GWireNode(n);
      return 1;
    }
  } else {
    if ((trashp = wire_trashin(n->in,M,drawp)))
      delete_GWireNode(n);
    return trashp;
  };
}

static int wire_trashout(GWireNode *n,GModuleDef *M,int drawp)
{
  int trashp;
  
  if (!n) return 0;
  
  if (!n->out) {
    if (n->end && n->end->gate) {
      switch (n->end->gate->typeinfo->Code) {
      case GC_JOINT :
	return joint_dejoint(n,M,drawp);
      default :
	return 0;
      } 
    } else {
      if (M) {
	ob_touch(M);
	M->m_wires = wire_unlink(M->m_wires,n->end);
      }
      wire_free(n->end);
      delete_GWireNode(n);
      return 1;
    }
  } else {
    if ((trashp = wire_trashout(n->out,M,drawp)))
      delete_GWireNode(n);
    return trashp;
  }
}

static int wire_trash(GWireNode *n,GModuleDef *M,int draw)
{
  if (n->in)
    return wire_trashin(n,M,draw);
  else
    return wire_trashout(n,M,draw);
}

/*
 * Nukes a wire if it is not attached to anything.  Returns 1 if the wire
 * was nuked, 0 otherwise.
 */
int wire_nuke(GWire *w,int draw,GModuleDef *M)
{
  GWire *ow;
  GWireNode *freeNodes = 0;

  if (wire_trash(w->nodes,M,draw)) {
    ob_touch(M);
    M->m_wires = wire_unlink(M->m_wires,w);
    wire_free(w);
    return 1;
  }
  return 0;
}

GWireNode *wire_makecorner(GWireNode *n,int tx,int ty)
{
  GWireNode *nn;
  GWire *dn;

  if (!n->out)
    return NULL;
  
  if ((n->stype != HORIZONTAL) && (n->stype != VERTICAL))
    return NULL;

  dn = wirenode_driver(n);
  GWire_draw(dn);
  nn = new_GWireNode();
  ob_touch(nn);
  nn->out = new_GWireNode();
  ob_touch(nn->out);
  nn->out->in = nn;
  
  if (n->x == n->out->x) {
    nn->x = nn->out->x = n->x;
    nn->y = nn->out->y = ty;
  } else {
    nn->y = nn->out->y = n->y;
    nn->x = nn->out->x = tx;
  }
  
  nn->out->out = n->out;
  ob_touch(n->out);
  n->out->in = nn->out;
  nn->in = n;
  n->out = nn;
  
  nn->stype = nn->out->stype = HORIZONTAL | VERTICAL;
  
  GWire_draw(dn);
  
  return n->in ? nn : nn->out;
}

GWire *wire_unattach(GWire *w,GWire *wl)
{
  if (wl) {
    if (w == wl)
      return w->next;
    else {
      ob_touch(wl);
      wl->next = wire_unattach(w,wl->next);
      return wl;
    }
  } else
    return NULL;
}

void wire_deletenode(GWireNode *n)
{
  if (n->out && !n->in) {
    ob_touch(n->end);
    ob_touch(n->out);
    n->end->nodes = n->out;
    n->out->end = n->end;
    n->out->in = NULL;
    n->out->stype = n->stype;
  } else
    if (n->in && !n->out) {
      ob_touch(n->end);
      ob_touch(n->in);
      n->end->nodes = n->in;
      n->in->end = n->end;
      n->in->out = NULL;
      n->in->stype = n->stype;
    } else {
      ob_touch(n->out);
      ob_touch(n->in);
      n->out->in = n->in;
      n->in->out = n->out;
      n->out->stype = n->stype;
    }
  delete_GWireNode(n);
}

void wire_deletezeronodes(GWireNode *n)
{
  if (n && n->out) {
    if (n->x == n->out->x && n->y == n->out->y) {
      GWireNode *nn;

      nn = n->out;
      wire_deletenode(n);
      wire_deletezeronodes(nn);
    } else
      wire_deletezeronodes(n->out);
  }
}

void GWireNode_freenodelist(GWireNode *n)
{
  if (n) {
    GWireNode_freenodelist(n->out);
    delete_GWireNode(n);
  }
}

/*
  Evaluate quality of cutting segment between n1 and n2 at point (x,y).
  Lower values are better
 */
int cutscore(GWireNode *n1,GWireNode *n2,int x,int y)
{
  if (!n1 || !n2) return 1000;

  if (n1->x == n2->x) {
    if (n1->y < n2-> y) {
      if (y < n1->y || y > n2->y) return 1000;
    } else {
      if (y < n2->y || y > n1->y) return 1000;
    }
    return abs(x-n1->x);
  } else {
    if (n1->x < n2->x) {
      if (x < n1->x || x > n2->x) return 1000;
    } else {
      if (x < n2->x || x > n1->x) return 1000;
    }
    return abs(y-n1->y);
  }
}

void wire_dump(EditState *es)
{
  GWireList *l;
  GWire *lw;
  GWireNode *ln;

  printf("WIRES:\n");
  for (l = es->env->m_wires;l;l = l->wl_next) {
    lw = l->wl_wire;
    if (!lw->nodes->out) continue;
    printf("  **  %s: ",lw->net->n_signame);
    for (ln = lw->nodes;ln;ln = ln->out)
      printf(" @%p:(%d %d)",ln,ln->x,ln->y);
    printf("\n");
  }
}


/*
 * Adds a new wire stub at position (x,y).  
 */
void wire_addstub(EditState *es,int x,int y)
{
  GWire *new_w1,*new_w2,*cut_w1,*cut_w2;
  GWireNode *n1 = 0;
  GWireNode *n2 = 0;
  int cs1,cs2;
  GWire *w;
  GWire *jw[4];					/* Attachment points for joint (0=right, 1=up, 2=left, 3=down) */

  jw[0] = jw[1] = jw[2] = jw[3] = 0;

  n1 = wire_hitall(x,y,es->env->m_wires);
  if (!n1) { return; }
  w = wirenode_driver(n1);

  if (distance(x,y,n1->x,n1->y) < MAXWIRERANGE) {	/* Attach to corner */
    if (!n1->in || !n1->out) {
      return;
    }

    x = n1->x;
    y = n1->y;

    GNet_draw(w->net);

    cut_w1 = wire_newend(es->env,w->net,1);
    cut_w2 = wire_newend(es->env,w->net,1);
    wire_newNetSegment(es->env,w->net,&new_w1,&new_w2);		/* The new wire to attach */

    ob_touch(cut_w1->nodes);
    cut_w1->nodes->in = n1->in;

    ob_touch(n1->in);
    n1->in->out = cut_w1->nodes;

    ob_touch(cut_w2->nodes);
    cut_w2->nodes->out = n1->out;

    ob_touch(n1->out);
    n1->out->in = cut_w2->nodes;

    ob_touch(new_w1->nodes);
    new_w1->nodes->x = x;
    new_w1->nodes->y = y;

    ob_touch(new_w2->nodes);
    new_w2->nodes->x = x;
    new_w2->nodes->y = y;

    cut_w1->nodes->x = x;
    cut_w1->nodes->y = y;
    cut_w2->nodes->x = x;
    cut_w2->nodes->y = y;
    
    if (n1->x > n1->in->x)
      jw[2] = cut_w1;
    else if (n1->x < n1->in->x)
      jw[0] = cut_w1;
    else if (n1->y > n1->in->y)
      jw[1] = cut_w1;
    else if (n1->y < n1->in->y)
      jw[3] = cut_w1;
    else {
      jw[0] = cut_w1;				/* Should never happen */
      logError(ERL_ERROR,"bogus w1 cut: (%d,%d) (%d,%d)\n",n1->x,n1->y,n1->in->x,n1->in->y);
    }

    if (n1->x > n1->out->x && !jw[2])
      jw[2] = cut_w2;
    else if (n1->x < n1->out->x && !jw[0])
      jw[0] = cut_w2;
    else if (n1->y > n1->out->y && !jw[1])
      jw[1] = cut_w2;
    else if (n1->y < n1->out->y && !jw[3])
      jw[3] = cut_w2;
    else {					/* Should never happen */
      logError(ERL_ERROR,"bogus w2 cut: (%d,%d) (%d,%d)\n",n1->x,n1->y,n1->out->x,n1->out->y);
      if (!jw[0])
	jw[0] = cut_w2;
      else
	jw[2] = cut_w2;
    }

    if (!jw[0]) {
      jw[0] = new_w1;
      new_w2->nodes->x += 12;
    } else if (!jw[1]) {
      jw[1] = new_w1;
      new_w2->nodes->y -= 12;
    } else if (!jw[2]) {
      jw[2] = new_w1;
      new_w2->nodes->x -= 12;
    } else if (!jw[3]) {
      jw[3] = new_w1;
      new_w2->nodes->y += 12;
    }

    delete_GWireNode(n1);
  } else {						/* Attach to segment */
    cs1 = cutscore(n1,n1->out,x,y);
    cs2 = cutscore(n1,n1->in,x,y);

    if (cs1 < cs2) {
      n2 = n1->out;
    } else {
      n1 = n1->in;
      n2 = n1->out;
    }
    if (!n2) return;

    GNet_draw(w->net);

    cut_w1 = wire_newend(es->env,w->net,1);
    cut_w2 = wire_newend(es->env,w->net,1);
    wire_newNetSegment(es->env,w->net,&new_w1,&new_w2);		/* The new wire to attach */

    ob_touch(n1);
    n1->out = cut_w1->nodes;

    ob_touch(cut_w1->nodes);
    cut_w1->nodes->in = n1;

    ob_touch(n2);
    n2->in = cut_w2->nodes;

    ob_touch(cut_w2->nodes);
    cut_w2->nodes->out = n2;

    ob_touch(new_w2->nodes);

    if (n1->x == n2->x) {
      x = n1->x;

      jw[0] = new_w1;

      new_w2->nodes->x = x + 12;
      new_w2->nodes->y = y;

      if (n1->y < n2->y) {
	jw[1] = cut_w1;
	jw[3] = cut_w2;
      } else {
	jw[1] = cut_w2;
	jw[3] = cut_w1;
      }
    } else if (n1->y == n2->y) {
      y = n1->y;

      jw[1] = new_w1;

      new_w2->nodes->x = x;
      new_w2->nodes->y = y - 12;

      if (n1->x < n2->x) {
	jw[2] = cut_w1;
	jw[0] = cut_w2;
      } else {
	jw[2] = cut_w2;
	jw[0] = cut_w1;
      }
    }

    ob_touch(new_w1->nodes);
    ob_touch(cut_w1->nodes);
    ob_touch(cut_w2->nodes);
    new_w1->nodes->x = x;
    new_w1->nodes->y = y;
    cut_w1->nodes->x = x;
    cut_w1->nodes->y = y;
    cut_w2->nodes->x = x;
    cut_w2->nodes->y = y;

  }

  if (jw[0]) { jw[0]->nodes->x += 2; jw[0]->orient = 0; }
  if (jw[1]) { jw[1]->nodes->y -= 2; jw[1]->orient = 1; }
  if (jw[2]) { jw[2]->nodes->x -= 2; jw[2]->orient = 2; }
  if (jw[3]) { jw[3]->nodes->y += 2; jw[3]->orient = 3; }
  joint_make(x,y,jw[0],jw[1],jw[2],jw[3],es);

  ob_touch(new_w2);
  new_w2->orient = (new_w1->orient + 2) % 4;

  GWireNode_setends(cut_w1->nodes);
  GWireNode_setends(cut_w2->nodes);
  GWireNode_setends(new_w1->nodes);

  wire_finalizeNet(new_w1);
  GNet_draw(w->net);
}

int wire_numOnPad(GWire *w)
{
  if (!w) 
    return 0;
  else
    return 1 + wire_numOnPad(w->next);
}

GWire *wire_root(GWireNode *n)
{
  GWire *w;
  int i;

  w = wirenode_driver(n);
  if (w->gate)
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      for (i=0;i<4;i++)
	if (w->gate->wires[i] &&
	    (w->gate->wires[i]->nodes->in))
	  return wire_root(w->gate->wires[i]->nodes);
      break;
    case GC_TAP :
      if (w->gate->wires[TAP_TAP] != w)
	return wire_root(w->gate->wires[TAP_IN]->nodes);
      break;
    }
  return w;
}

/*
 * Set the size of the selected wire to the current wire size
 */
void setwiresize(EditState *es)
{
  GWire *r;
  int size;

  ob_touch(TkGate.circuit);

  if (!(TkGate.circuit->wnsel = wire_hit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires)))
    TkGate.circuit->wnsel = wire_iohit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires);

  if (TkGate.circuit->wnsel) {
    const char *str_size = Tcl_GetVar(TkGate.tcl,"tkg_bitWidth",TCL_GLOBAL_ONLY);

    size = 1;
    assert(str_size);
    sscanf(str_size,"%d",&size);

    TkGate.circuit->wsel = wirenode_driver(TkGate.circuit->wnsel);
    r = wire_root(TkGate.circuit->wnsel);
    GNet_draw(r->net);
    net_setSize(r->net,size);
    GNet_draw(r->net);
    SetModified(MF_NET);
  }
}


static int GWire_getNetWires_aux(GWire *w,GWire **wlist,unsigned which,int isFirst)
{
  int count = 0;
  int i;

  w = wire_other(w);

  if (!isFirst) {
    if ((which & GW_DRIVER))
      wlist[count++] = wire_driver(w);
    if ((which & GW_DRIVEE))
      wlist[count++] = wire_drivee(w);
  }

  if (w->gate) {
    switch (w->gate->typeinfo->Code) {
    case GC_JOINT :
      for (i=0;i<4;i++)
	if (w->gate->wires[i] && (w->gate->wires[i] != w))
	  count += GWire_getNetWires_aux(w->gate->wires[i],wlist+count,which,0);
      break;
    case GC_TAP :
      if (w == w->gate->wires[TAP_OUT])
	count += GWire_getNetWires_aux(w->gate->wires[TAP_IN],wlist+count,which,0);
      else if (w == w->gate->wires[TAP_IN])
	count += GWire_getNetWires_aux(w->gate->wires[TAP_OUT],wlist+count,which,0);
      break;
    }
  }
  return count;
}

int GWire_getNetWires(GWire *w,GWire **wlist,unsigned which)
{
  GWire *o_w;
  int count = 0;

  if (!w) return 0;

  o_w = wire_other(w);

  count = GWire_getNetWires_aux(w,wlist,which,0);
  count += GWire_getNetWires_aux(o_w,wlist+count,which,1);

  return count; 
}

void GWire_insertNode(GWire*w)
{
  GWireNode *n = new_GWireNode();

  ob_touch(w->nodes);
  ob_touch(w);

  if (!w->nodes->in) {
    w->nodes->in = n;
    n->out = w->nodes;
  } else {
    w->nodes->out = n;
    n->in = w->nodes;
  }

  n->x = w->nodes->x;
  n->y = w->nodes->y;

  w->nodes->end = 0;
  n->end = w;
  w->nodes = n;

}
