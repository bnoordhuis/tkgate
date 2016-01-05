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
#ifndef __wires_h
#define __wires_h

#define NUMPOS 39

/* Split node directions */
#define IN_DIR 42
#define OUT_DIR 7

/* wire_makestraight conditions */
#define OK 42
#define IMMOBILE 0

/* Wire selction modes (These are bit masks) */
#define UNSELECTED	0x0
#define HORIZONTAL	0x1
#define VERTICAL	0x2
#define FULL		0x3
#define NOSTRAIGHTEN	0x4

#define DRIVER 42
#define DRIVEE 0
#define UNKNOWN 8

/* Hit Operations */
#define F_GATE 0x01
#define F_WIRE 0x02
#define F_WIREEND 0x12
#define F_WIRENOEND 0x22

#define D_RIGHT 0
#define D_UP 1
#define D_LEFT 2
#define D_DOWN 3

/*
   A "GWire" is really an end point of a wire segment.  It might have been
   more appropriate to call it a port. 
*/
struct wire {
  GNet	*net;			/* Net this wire is a part of */
  char *name;			/* Port name (if connected to a block) */
  int nidx;			/* Index within net */
  unsigned invert : 1;		/* Inverter at this end? */
  unsigned xanchor : 1;		/* X coordinate is anchored */
  unsigned yanchor : 1;		/* Y coordinate is anchored */
  unsigned cpath : 1;		/* Is this seg. on displayed path? */
  short orient;			/* Orientation: UP, RIGHT, DOWN, LEFT */
  struct {
    short num,den;		/* Offset of wire position (as a fraction) */
  } offset;
  char wtype;			/* DRIVER/DRIVEE tag */
  short PadNum;			/* Pad number (computed before simulation) */
  short WireDir;		/* Direction  (computed before simulation) */
  GCElement *gate;		/* Gate connected to this wire end */
  GWireNode *nodes;		/* Nodes on this wire */
  GWire *driver;		/* Driving end of wire */
  GWire *next;			/* Next wire in global list */
};

struct wirenode {
  int x,y;			/* Node position */
  char stype;			/* What kind of node selection */
  unsigned showSize : 1;	/* Show wire size on this link? */
  unsigned mark : 1;		/* Has this node been touched */
  unsigned isLabeled : 1;	/* Non-zero if this node is labeled */
  unsigned labelSide : 1;	/* Side label is on (0=left/top, 1=right/bottom) */
  unsigned offset : 8;		/* Offset of label */

  GWireNode *out;		/* Output nodes */
  GWireNode *in;		/* Input nodes */
  GWire *end;			/* End of wire */
};

GWireNode *new_GWireNode();
void delete_GWireNode(GWireNode*);
GWire *wirenode_driver(GWireNode*);
GWire *wirenode_drivee(GWireNode*);
void GWireNode_displaySize(GWireNode *n,GNet *net);
GWireNode *wirenode_cutsegment(int,int,GWireNode *,GModuleDef *);
GWireNode *wirenode_cutcorner(GWireNode *,GModuleDef *);
void GWireNode_freenodelist(GWireNode *n);

void wire_free(GWire*);
GWire *wire_unattach(GWire *w,GWire *wl);
GWire *wire_driver(GWire*);
GWire *wire_drivee(GWire*);
GWire *wire_other(GWire *w);
GWire *wire_append(GWire *wl,GWire *w);
int wire_addToGate(GCElement *g,int p,GModuleDef *mdef,int invertp);
int wire_new(GModuleDef *mdef,GWire **e,GWire **e2);
int wire_newNetSegment(GModuleDef *mdef,GNet*,GWire **e,GWire **e2);
GWireList *wire_unlink(GWireList *wl,GWire *w);
GWireList *wire_link(GWireList *wl,GWire *w);
char *wire_removeName(GWire*);
void wire_setName(GWire*,char*);
GWire *wire_sigroot(GWire*);
GWire *wire_newend(GModuleDef *M,GNet*,int doNode);
void wire_setNet(GWire *w,GNet *net);
void wire_move(GWireNode *n,int dx,int dy,int type);
void wire_moveto(GWireNode *n,int x,int y);
GWireNode *wire_iohit(int,int,GWireList*);
GWireNode *wire_hitanynode(int,int,GWireList*);
GWireNode *wire_hitall(int,int,GWireList*);
GWireNode *wire_hit(int,int,GWireList*);
GWire *wire_endhit(GWire*,GWireList*);
GWireNode *wire_hit_other(GWire*,GWireList*);
void wire_snapgate(GCElement *g,int doRedraw);
void GWire_snap(GWire *w);
GWire *wire_connect(GModuleDef *env,GWire *w1,GWire *w2);
GWireNode *wire_makecorner(GWireNode *n,int tx,int ty);
int wire_shorten(GWire*,GModuleDef*,int drawp);
int wire_nuke(GWire*,int,GModuleDef*);
void wire_drawnet(GWire *w);
void GWire_draw(GWire *);
void wire_deletezeronodes(GWireNode *n);
void wire_finalizeNet(GWire *w);
void wire_cut(int x,int y,GWireNode *n,GModuleDef *env);
void wire_makestraightaux(GWireNode *n,int wt,int ox,int oy,int nx,int ny);
void wire_setPadSpacing(GCElement *g,int p);
void wire_deletenode(GWireNode *n);
int wire_force(GWire *w,int d,int retry);
void wire_drawlist(GWire *w,GCElement *g);
GWire *wire_findClosest(GWire*,int,int);
void wire_addstub(EditState *es,int x,int y);
int GWire_pickProbePosition(GWire *w,int *x,int *y);
int GWire_getNetWires(GWire *w,GWire **wlist,unsigned which);
void GWire_updateLabels(GWire *w);
void GWire_insertNode(GWire*w);

void GWireNode_getLabelPos(GWireNode *n,GNet *net,int *x,int *y,int *p);

void joint_connect(GCElement *g,GWireNode *n);
void joint_make(int x,int y,GWire *w1,GWire *w2,GWire *w3,GWire *w4,EditState *es);
void joint_fixwires(GCElement *j,GWire *w,int retry);
int joint_dejoint(GWireNode *n,GModuleDef *env,int drawp);
void joint_addstub(GCElement *g,EditState *es);

GCElement *join_wires(GWire *w,GWireNode *n,EditState *es);
void join_wirereverse(GWire *w);
void join_treereverse(GWire *w);
void join_treereverse_aux(GWire *w);

void tap_transmute(GWire *branch,EditState *es);


void setwiresize(EditState *es);
void SignalNamePos(GWireNode *n1,GWireNode *n2,int *x,int *y,int *p,int *q);

int wireorient(GWireNode *n,int d);
void createwire(GModuleDef *e,GCElement *g,int p,int invertp);

void wire_dump(EditState *es);
int wire_numOnPad(GWire *w);

GWireNode *wire_nodehit(int x,int y,GWireNode *n,int includeend,int range,int corneronly);
GWireNode *wire_splitnode(GWireNode *n,int dir);
GWire *wire_endhit(GWire *w,GWireList *wl);
GCElement *wire_drivinggate(GWire *w);
void wire_rePort(GWire *w,int wx,int wy,int dir);


#endif


