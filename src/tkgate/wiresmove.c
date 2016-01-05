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


#define isgatenode(_n) ((_n)->end && (_n)->end->gate)

/* 
 * Straighten out nodes
 */
int wire_netstraight(GWireNode *n,int wt,int ox,int oy,int nx,int ny)
{
  int x,y;
  GWireNode *next;
  
  if (!n) return OK;
  
  next = (wt == DRIVER ? n->out : n->in);
  x = n->x;
  y = n->y;
  if (isgatenode(n)) {
    if ((ox == nx) && (oy == ny)) return OK;
    if (next && (ox == nx) && next->y == n->y) return OK;
    if (next && (oy == ny) && next->x == n->x) return OK;

    return IMMOBILE;
  }

  ob_touch(n);
  
  switch (wire_netstraight(next,wt,x,y,x+nx-ox,y+ny-oy)) {
  case OK :
    n->x += nx - ox;
    n->y += ny - oy;
    if ((nx != ox) || (ny != oy)) n->mark = 1;
    return OK;
  case IMMOBILE :
    if ((next->end->orient % 2) == 0) {
      n->y = next->y;
      n->x += nx - ox;
      if (nx - ox) n->mark = 1;
    } else {
      n->x = next->x;
      n->y += ny - oy;
      if (ny - oy) n->mark = 1;
    }
    return OK;
  }
  logError(ERL_FATAL,"Unknown case in wire_newstraight.");
  return IMMOBILE;
}

/*
  Splits a node if we can't find a way around it
 */
GWireNode *wire_splitnode(GWireNode *n,int dir)
{
  GWireNode *newnode;

  newnode = new_GWireNode();

  ob_touch(newnode);
  newnode->mark = 1;
  switch (dir) {
  case OUT_DIR :
    newnode->out = n->out;
    newnode->in = n;
    newnode->end = NULL;
    if (n->out) {
      ob_touch(n->out);
      n->out->in = newnode;
      newnode->x = (n->x + n->out->x) / 2;
      newnode->y = (n->y + n->out->y) / 2;
    } else {
      newnode->x = n->x;
      newnode->y = n->y;
    }
    ob_touch(n);
    n->out = newnode;
    break;
  case IN_DIR :
    newnode->in = n->in;
    newnode->out = n;
    newnode->end = NULL;
    if (n->in) {
      ob_touch(n->in);
      n->in->out = newnode;
      newnode->x = (n->x + n->in->x) / 2;
      newnode->y = (n->y + n->in->y) / 2;
    } else {
      newnode->x = n->x;
      newnode->y = n->y;
    }
    ob_touch(n);
    n->in = newnode;
    break;
  }
  return newnode;
}

/* Auxilliary function for makestraight */
void wire_makestraightaux(GWireNode *n,int wt,int ox,int oy,int nx,int ny)
{
  GWireNode *n1,*n2;
  
  if (!n) return;
  
  if (wire_netstraight(n,wt,ox,oy,nx,ny) == IMMOBILE) {
    if (wt == DRIVEE && (n->out->out || isgatenode(n->out))) {
      n1 = wire_splitnode(n,OUT_DIR);
      n2 = wire_splitnode(n1,OUT_DIR);
      ob_touch(n1);
      ob_touch(n2);
      n2->x = n1->x = (n->x + ox) / 2;
      n2->y = n1->y = (n->y + oy) / 2;
    } else if (wt != DRIVEE && (n->in->in || isgatenode(n->in))) {
      n1 = wire_splitnode(n,IN_DIR);
      n2 = wire_splitnode(n1,IN_DIR);
      ob_touch(n1);
      ob_touch(n2);
      n2->x = n1->x = (n->x + ox) / 2;
      n2->y = n1->y = (n->y + oy) / 2;
    } else {
      n2 = wire_splitnode(n,wt == DRIVEE ? OUT_DIR : IN_DIR);
      ob_touch(n2);
      if (n->x == ox)
	n2->y = oy;
      else
	n2->x = ox;
    }
    if (wire_netstraight(n2,wt,ox,oy,nx,ny) == IMMOBILE)
      fprintf(stderr,"Huh? split node didn't help\n");
  }
}

/* Takes a moved node and it's old position to figure out how to move the rest of the
   network to keep the rep. invarient */
void wire_makestraight(GWireNode *n,int ox,int oy)
{
  if (n->stype == FULL) {
    wire_makestraightaux(n->out,DRIVER,ox,oy,n->x,n->y);
    wire_makestraightaux(n->in,DRIVEE,ox,oy,n->x,n->y);
  } else {
    if (n->stype & HORIZONTAL) {
      ob_touch(n->out);
      n->out->x += n->x - ox;
      if (n->x - ox) {
	ob_touch(n);
	n->mark = 1;
      }
    }
    if (n->stype & VERTICAL) {
      ob_touch(n->out);
      n->out->y += n->y - oy;
      if (n->y - oy) {
	ob_touch(n);
	n->mark = 1;
      }
    }
  }
}

/******************************************************************************
 *
 * Moves a wire
 *
 * Parameters:
 *      n		Wire node to be moved
 *      dx,dy		Vector for movement of node
 *      type		Type of movement (affects movement of connected nodes)
 *
 *****************************************************************************/
void wire_move(GWireNode *n,int dx,int dy,int type)
{
  int x,y;

  ob_touch(n);
  n->stype = type;
  x = n->x;
  y = n->y;

 
  if ((type & VERTICAL)) n->y += dy;
  if ((type & HORIZONTAL)) n->x += dx;

  if (n->x == x && n->y == y) return;	/* Didn't move */

  SetModified(MF_NET);

  if ((type & NOSTRAIGHTEN)) return;
  wire_makestraight(n,x,y);
}

void wire_moveto(GWireNode *n,int x,int y)
{
  wire_move(n,x-n->x,y-n->y,FULL);
}



GWireNode *wire_hitanynode(int x,int y,GWireList *wires)
{
  struct wirenode *N,*Close;
  int CloseD,D;
  
  if (!wires) return NULL;
  
  CloseD = distance(x,y,wires->wl_wire->nodes->x,wires->wl_wire->nodes->y);
  Close = wires->wl_wire->nodes;
  
  for (;wires;wires = wires->wl_next)
    if (wires->wl_wire->nodes->out)
      for (N = wires->wl_wire->nodes;N;N = N->out) {
	D = distance(x,y,N->x,N->y);
	if (D < CloseD) {
	  CloseD = D;
	  Close = N;
	}
      }
  
  if (CloseD < MAXWIRERANGE)
    return Close;
  else
    return NULL;
}

/* Test an individual wire's node list */
GWireNode *wire_nodehit(int x,int y,GWireNode *n,int includeend,int range,int corneronly)
{
  int testrange;
  GWireNode *test;
  
  if (!n)
    return NULL;
  
  if (includeend || !anchoredp(n))
    if ((testrange = distance(x,y,n->x,n->y))
	< (corneronly ? range : MAXWIRERANGE)) {
      if ((test = wire_nodehit(x,y,n->out,includeend,testrange,1)))
	return test;
      else {
	ob_touch(n);
	n->stype = FULL;
	return n;
      }
    }
  
  if ((n->out ? includeend || (!anchoredp(n) && !anchoredp(n->out)) : 0) &&
      !corneronly) {
    if ((n->y == n->out->y) &&
	(midpointp(x,n->x,n->out->x) && ((testrange = sqr(y - n->y)) < range))) {
      
      
      if ((test = wire_nodehit(x,y,n->out,includeend,testrange,0)))
	return test;
      else {
	ob_touch(n);
	n->stype = VERTICAL;
	return n;
      }
    }
    if ((n->x == n->out->x) &&
	(midpointp(y,n->y,n->out->y) && ((testrange = sqr(x - n->x)) < range))) {
      
      if ((test = wire_nodehit(x,y,n->out,includeend,testrange,0)))
	return test;
      else {
	ob_touch(n);
	n->stype = HORIZONTAL;
	return n;
      }
    }
  }
  return wire_nodehit(x,y,n->out,includeend,range,corneronly);
}

GWireNode *wire_hitall(int x,int y,GWireList *wires)
{
  GWireNode *test;

  if (!wires)
    return NULL;

  if ((wires->wl_wire->nodes->out ? (test = 
      wire_nodehit(x,y,wires->wl_wire->nodes,1,MAXWIRERANGE,0)):NULL))
    return test;
  else 
    return wire_hitall(x,y,wires->wl_next);
}



/* Search for a wire other than 'w' */
GWireNode *wire_hit_other(GWire *w,GWireList *wires)
{
 GWireNode *test;

 if ((!wires) || (!w))
   return NULL;
 
 if ((wires->wl_wire->nodes->out && (wires->wl_wire->driver != w->driver)) ?
     (test = wire_nodehit(w->nodes->x,w->nodes->y,wires->wl_wire->nodes,
			 1,MAXWIRERANGE,0)):NULL)
   return test;
 else 
   return wire_hit_other(w,wires->wl_next);
}

/* Test to see if theres a wire here */
GWireNode *wire_hit(int x,int y,GWireList *wires)
{
  GWireNode *test;
  
  if (!wires)
    return NULL;
  
  if ((wires->wl_wire->nodes->out ? (test = 
      wire_nodehit(x,y,wires->wl_wire->nodes,0,MAXWIRERANGE,0)):NULL))
    return test;
  else 
    return wire_hit(x,y,wires->wl_next);
}

GWireNode *wire_iohit(int x,int y,GWireList *wires)
{
  GWireNode *test;
  
  if (!wires)
    return NULL;
  
#ifdef DEBUGHIT
  printf("Wire is: %d\n",wires->wl_wire->wtype);
#endif
  /* Range was 5*5+1 */
  if ((test = wire_nodehit(x,y,wires->wl_wire->nodes,1,MAXWIRERANGE,0)))
    /* Danger */
    return test;
  else 
    return wire_iohit(x,y,wires->wl_next);
}

/* Test only at endpoints of a wire. */
GWire *wire_endhit(GWire *w,GWireList *wl)
{
  GWire *best_w = 0;
  int best_d2 = 0;

  for (;wl;wl = wl->wl_next) {
    int dx = wl->wl_wire->nodes->x - w->nodes->x;
    int dy = wl->wl_wire->nodes->y - w->nodes->y;
    int d2 = dx*dx + dy*dy;

    if (wl->wl_wire == w || anchoredp(wl->wl_wire->nodes))
      continue;

    if (!best_w || d2 < best_d2) {
      best_w = wl->wl_wire;
      best_d2 = d2;
    }
  }

  if (best_w && best_d2 < 25)
    return best_w;
  else
    return 0;

#if 0
  if (!wl)
    return NULL;
  if ((abs(wl->wl_wire->nodes->x - w->nodes->x) < 5) &&
      (abs(wl->wl_wire->nodes->y - w->nodes->y) < 5) &&
      (wl->wl_wire != w) && !anchoredp(wl->wl_wire->nodes))
    return wl->wl_wire;
  else
    return wire_endhit(w,wl->wl_next);
#endif
}

GCElement *wire_drivinggate(GWire *w)
{
  if (!w)
    return NULL;
  else
    return w->driver->gate;
}

/* Returns the driver of a wire network */
GWire *wirenode_driver(GWireNode *n)
{
  if (!n) 
    return NULL;
  if (!n->in)
    return  n->end;
  else
    return wirenode_driver(n->in);
}

/* Returns the drivee of a wire network */
GWire *wirenode_drivee(GWireNode *n)
{
  if (!n) 
    return NULL;
  if (!n->out) {
    return n->end;
  } else
    return wirenode_drivee(n->out);
}

GWire *wire_driver(GWire *w)
{
  return wirenode_driver(w->nodes);
}

GWire *wire_drivee(GWire *w)
{
  return wirenode_drivee(w->nodes);
}


GWire *wire_other(GWire *w)
{
  if (w->driver != w)
    return w->driver;
  else
    return wire_drivee(w);
}

int posongate(GWire *w,GCElement *g,int *p,int *n)
{
  int i;
  int N = GCElement_numPads(g);

  for (i = 0;i < N;i++)
    if ((*n = findwirepos(w,g->wires[i]))) {
      *p = i;
      return 0;
    }

  return -1;
}

/* Searches an i/o wire list for the number of the target wire */
int findwirepos(GWire *w,GWire *l)
{
  int p;

  if (!l)
    return 0;
  else {
    assert(l != l->next); 
    if (w == l)
      return 1;
    else
      if ((p = findwirepos(w,l->next)) != 0)
	return p + 1;
      else
	return 0;
  }
}

int GetPadPosition(GCElement *g,const char *name)
{
  int N = GCElement_numPads(g);
  int i;

  for (i = 0;i < N;i++)
    if (!strcmp(GCElement_getPadName(g,i),name))
      return i;
  return -1;
}

void wire_rePort(GWire *w,int wx,int wy,int dir)
{
  GWire *ow = wire_other(w);

  ob_touch(w);
  wire_move(w->nodes,wx-w->nodes->x,wy-w->nodes->y,VERTICAL|HORIZONTAL);

  /*
   * If wire end is not attached, compute mirror image.  Otherwise use brute force
   * to fix the wire.
   */
  if (w->orient != dir) {
    w->orient = dir;
    if (!ow->gate) {
      int cx = w->nodes->x;
      int cy = w->nodes->y;
      GWireNode *n;


      for (n = w->driver->nodes;n;n = n->out) {
	ob_touch(n);
	//	if (g->orient == 0 || g->orient == 2)
	if (dir == D_LEFT || dir == D_RIGHT)
	  n->x = cx - (n->x - cx);
	else
	  n->y = cy - (n->y - cy);
      }
    } else
      GWire_snap(w->driver);
  }
}
