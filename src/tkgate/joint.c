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

    Last edit by hansen on Mon Jan 19 18:16:15 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tkgate.h"

#define JOINT_RIGHT 0
#define JOINT_TOP 1
#define JOINT_LEFT 2
#define JOINT_BOTTOM 3

void GWireNode_setends(GWireNode *n);

GCElement *Joint_Make(EditState **es,GModuleDef *M,int GType,
		      int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions);

void Joint_Draw(GCElement *g,int md);

void Joint_Delete(GCElement *g,GModuleDef *M,int drawp);
void Joint_Init(GCElement *g);
void Joint_VerSave(FILE *f,GCElement *g);
void Joint_Rotate(GCElement *g, int centX, int centY, int rdir);
GWireNode *Joint_wireSnap(GCElement *g,GWire *w,int *mod,int retry);

static iconDimensions joint_iconDims[] = {
  {0, 0, 3, 3, 1, 1},
};
static int joint_iconBoldOffset = 4;

static char *psJoint[] = {
  "%",
  "% A Solder Joint",
  "%",
  "/joint {",
  "  startgate",
  "  0 0 2 0 360 arc",
  "  closepath",
  "  fill",
  "  grestore",
  "} bind def",
  0
};

GPadLoc joint_L_loc[] = {
  {-1,0,-1,0,D_LEFT}
};

GPadLoc joint_R_loc[] = {
  {1,0,1,0,D_RIGHT}
};

GPadLoc joint_T_loc[] = {
  {0,-1,0,-1,D_UP}
};

GPadLoc joint_B_loc[] = {
  {0,1,0,1,D_DOWN}
};


GGateInfo gate_joint_info = {
  GC_JOINT,
  "JOINT",
  "joint",0x0,
  "joint",psJoint,
  -1,-1,

  {{0}},
  joint_iconDims,
  
  /*
     The order here is critical (only for joints).  It should correspond
     to the direction of the pin.  This kludge is only for this type.
     */
  4,{{"R",ANY,0,0,joint_R_loc,0},{"T",ANY,0,0,joint_T_loc,0},
       {"L",ANY,0,0,joint_L_loc,0},{"B",ANY,0,0,joint_B_loc,0}},
  {{0,0,CT},{0,0,CT},{0,0,CT},{0,0,CT}},
  {0,1},

  {0},
  
  Joint_Make,
  Nop_WriteCellDef,
  Joint_Init,
  Joint_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Joint_Draw,
  Generic_Move,
  Generic_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Joint_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Generic_PSWrite,
  Generic_EditProps,
  Joint_VerSave,
  0,
  0,
  Joint_wireSnap
};


GCElement *Joint_Make(EditState **es,GModuleDef *M,int GType,
		      int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g = Generic_Make(es,M,GType,x,y,r,Name,noWires,options,nOptions);

  ob_touch(g);
  g->show_name = 0;

  return g;
}

void Joint_Init(GCElement *g)
{
  ob_touch(g);
  g->u.joint.driver = -1;
}

void Joint_Delete(GCElement *g,GModuleDef *M,int drawp)
{
}

void Joint_Draw(GCElement *g,int md)
{
  mk_gate(g->xpos,g->ypos,g->typeinfo,g->orient,g->selected);
  gate_drawWires(g,md);
}


/**********************************************************************
 *
 *  Special joint handling in wires follows....
 *
 **********************************************************************/

extern int debugmode;

/*
   Return the position of the free position on a joint, or
   -1 of there are no free positions.
*/
int joint_free(GCElement *g)
{
  int i;

  for (i = 0;i < 4;i++)
    if (!g->wires[i])
      return i;

  return -1;
}

/*
   Return any wire of a joint, or NULL if there are none.
*/
GWire *joint_any(GCElement *g)
{
 int i;

  for (i = 0;i < 4;i++)
    if (g->wires[i])
      return g->wires[i];
 return 0;
}


/*
   Connect wire at node n to joint g.  It is assumed that the
   wires have been erased and are not currently visible.
*/
void joint_connect(GCElement *g,GWireNode *n)
{
  GWireNode *nn;
  int i,dx,dy;
  GWire *w,*jw,*o_w;
  
  message(0,"trying joint_connect.");
  if (n->in) {
    nn = n->in;
    if (g->u.joint.driver >= 0) {
      if (n->end) {
	join_treereverse(n->end);
	nn = n->out;
      } else {
	message(0,"Connection refused because of bogosity.");
	return;
      }
    }
  } else
    nn = n->out;

  w = wirenode_driver(n);
  jw = joint_any(g);

  if (!net_connectOK(w->net,jw->net,0)) return;

  if ((i = joint_free(g)) < 0)
    return;

  dx = g->xpos - n->x;
  dy = g->ypos - n->y;

  ob_touch(n);
  ob_touch(n->end);
  if (n->x == nn->x) {
    if (n->y > nn->y) {
      dy -= 2;
      n->end->orient = RIGHT;
    } else {
      dy += 2;
      n->end->orient = LEFT;
    }
  } else {
    if (n->x > nn->x) {
      dx -= 2;
      n->end->orient = DOWN;
    } else {
      dx += 2;
      n->end->orient = UP;
    }
  }

  if (n->end && (o_w = wire_other(n->end)) && !o_w->gate) {
    n->x += dx;
    n->y += dy;
  } else
    wire_move(n,dx,dy,n->stype);

  ob_touch(g);
  ob_touch(n->end);
  g->wires[i] = n->end;
  n->end->gate = g;
  if (n->in)
    g->u.joint.driver = i;
}

/*
   Create a new joint to merge wires w1 through w4.
*/
void joint_make(int x,int y,GWire *w1,GWire *w2,GWire *w3,GWire *w4,EditState *es)
{
  GCElement *j;
  int i;
  
  j = gate_new(x,y,0,GC_JOINT);
  ob_touch(j);
  j->wires[0] = w1;
  j->wires[1] = w2;
  j->wires[2] = w3;
  j->wires[3] = w4;

  for (i = 0;i < 4;i++) {
    GWire *lw = j->wires[i];
    if (lw) {
      ob_touch(lw);
      lw->gate = j;
      if (lw->nodes->in)
	j->u.joint.driver = i;
    }
  }
  
  gate_add(es->env,j);
}

/*
    Returns the orientation of a wire on a joint.
*/
int joint_wiredir(GWire *w)
{
  if (w->nodes->x > w->gate->xpos)
    return JOINT_RIGHT;
  else if (w->nodes->x < w->gate->xpos)
    return JOINT_LEFT;
  else if (w->nodes->y < w->gate->ypos)
    return JOINT_TOP;
  else
    return JOINT_BOTTOM;
}

static void joint_fixwires_aux(GCElement *j,GWireNode *n,int td)
{
  int x,y,nx,ny;

  nx = ny = 0;
  x = n->x;
  y = n->y;

  ob_touch(n->end);
  switch (td) {
  case JOINT_RIGHT :	/* --> */
    ny = j->ypos;
    nx = j->xpos + 2;
    n->end->orient = UP;    /* These directions are confusing, but correct */
    break;
  case JOINT_TOP :	/*  ^  */
    ny = j->ypos - 2;
    nx = j->xpos;
    n->end->orient = RIGHT;
    break;
  case JOINT_LEFT :	/* <-- */
    ny = j->ypos;
    nx = j->xpos - 2;
    n->end->orient = DOWN;
    break;
  case JOINT_BOTTOM :	/*  v  */
    ny = j->ypos + 2;
    nx = j->xpos;
    n->end->orient = LEFT;
    break;
  }

  if (td != -1) {
    wire_move(n,nx-x,ny-y,VERTICAL | HORIZONTAL);
  }
}

/*
  Correct the wire so that the end enters the joint in direction 'd'.

  0:
  |
#--+

  1:
  +----
  |
#

  2:

  +---#
  |

  3:

#
  |
  +---
  */
static void joint_correct(GWireNode *n,int d)
{
  GWireNode *nn;

  nn = new_GWireNode();
  ob_touch(n);
  ob_touch(nn);

  nn->x = n->x;
  nn->y = n->y;

  switch (d) {
  case 0 :
    wire_move(n,10,0,VERTICAL | HORIZONTAL);
    break;
  case 1 :
    wire_move(n,0,-10,VERTICAL | HORIZONTAL);
    break;
  case 2 :
    wire_move(n,-10,0,VERTICAL | HORIZONTAL);
    break;
  case 3 :
    wire_move(n,0,10,VERTICAL | HORIZONTAL);
    break;
  }

  if (n->out) {
    n->in = nn;
    nn->out = n;
  } else {
    n->out = nn;
    nn->in = n;
  }
  nn->end = n->end;
  n->end = NULL;

  ob_touch(nn->end);
  nn->end->nodes = nn;	
}

/*
  Fixes the coordinates of wire 'w' arround the joint 'j'.  If two wires want to be on
  the same position arround the joint, we need to edit the wires until they don't.

       1
       |
   2 --+-- 0
       |
       3

*/
#if 1
void joint_fixwires(GCElement *j,GWire *w,int retry)
{
  int i,k,td;
  GWireNode *n;
  int pos[4];
  GWire *wa[4];

  while ((td = wireorient(w->nodes,0)) == -1) {
    if (debugmode) printf("joint_fixwires() Removing zero node\n");
    if (w->nodes->in ? !w->nodes->in->in : !w->nodes->out->out) {
      if (debugmode) printf("Punting fixwires!\n");
      return;
    }
    wire_deletenode(w->nodes);
  }

  for (i = 0;i < 4;i++) pos[i] = 0;	/* Flag all positions as unused */

  for (i = 0;i < 4;i++)
    if (j->wires[i] && ((n = j->wires[i]->nodes) != w->nodes)) {
      td = wireorient(n,0);
      joint_fixwires_aux(j,n,td);
      if (td == -1) {
	td = joint_wiredir(n->end);
      }
      pos[td] = 1;
    }

  td = wireorient(w->nodes,0);
  if (td == -1) {
    logError(ERL_WARN,"Wierd place for 0-wire 1 in joint_fixwires.");
    return;
  }

  if (pos[td]) {
    td = wireorient(w->nodes,1);
    if (td == -1) {
      td = (wireorient(w->nodes,0) + 1) & 3;
    }
    if (!pos[td])
      joint_correct(w->nodes,td);
    else {
      td = (wireorient(w->nodes,0) + 2) & 3;
      if (pos[td])
	joint_correct(w->nodes,(((td = wireorient(w->nodes,1))+2)&3));
      else {
	joint_fixwires_aux(j,w->nodes,td);
	wire_force(w,td,retry);
      }
    }
  }
  if (debugmode) printf("Fixing direction %d\n",td);
  joint_fixwires_aux(j,w->nodes,td);

  ob_touch(j);
  for (i = 0;i < 4;i++) {
    wa[i] = j->wires[i];
    j->wires[i] = 0;
  }

  for (i = 0;i < 4;i++) {
    if (!wa[i]) continue;

    td = wireorient(wa[i]->nodes,0);
    if (td < 0 || j->wires[td]) {
      for (k = 0;k < 4;k++)
	if (!j->wires[k]) {
	  td = k;
	  break;
	}
    }

    ob_touch(j);
    ob_touch(wa[i]);
    j->wires[td] = wa[i];
    wa[i]->orient = j->typeinfo->Pad[td].Loc[0].dir;
  }
}
#endif

/*
    Unhooks n from a joint and possibly deletes the joint.
*/
int joint_dejoint(GWireNode *n,GModuleDef *M,int drawp)
{
  struct celemnt *j;
  struct wire *wi,*wo;
  int num,i,k;

  k = 0;

  j = n->end->gate;
  if ((!j) || (j->typeinfo->Code != GC_JOINT))
    logError(ERL_FATAL,"Called on non-joint in joint_dejoint.");

  for (i = 0,num = 0;i < 4;i++)
    if (j->wires[i]) {
      num++;
      if (j->wires[i]->nodes == n) k = i;
    }

  switch (num) {
  case 0 :
  case 1 :
  case 2 :
    logError(ERL_WARN,"Joint with too few wires in joint_dejoint.");
    return 0;
  case 3 :
    wi = wo = NULL;
    for (i = 0;i < 4;i++) {
      if (j->wires[i] && j->wires[i]->nodes !=n) {
	if (wo) {
	  wi = j->wires[i];
	  if (drawp) GWire_draw(wi->driver);
	  ob_touch(wi);
	  wi->gate = NULL;
	} else {
	  wo = j->wires[i];
	  if (drawp) GWire_draw(wo->driver);
	  ob_touch(wo);
	  wo->gate = NULL;
	}
      }
      ob_touch(j);
      j->wires[i] = NULL;
    }
    if (!(wi && wo))
      logError(ERL_FATAL,"Bad formation in joint_dejoint.");

    if (wi->driver->gate||wire_drivee(wi)->gate||
	wo->driver->gate||wire_drivee(wo)->gate) {
      GWire *rw;

      ob_touch(wi->nodes);
      ob_touch(wo->nodes);
      wi->nodes->x = j->xpos;
      wi->nodes->y = j->ypos;
      wo->nodes->x = j->xpos;
      wo->nodes->y = j->ypos;
      rw = wire_connect(M,wi,wo);
      if (rw && drawp) GWire_draw(rw->driver);
    } else
      if (!(wire_nuke(wi,0,M)&&wire_nuke(wo,0,M)))
	logError(ERL_FATAL,"Wires can't be nuked in joint_dejoint.");

    if (M) M->m_wires = wire_unlink(M->m_wires,n->end);
    wire_free(n->end);
    delete_GWireNode(n);

    if (drawp) gate_draw(j,0);
    gate_remove(M,j);
    ob_free(j);

    return 1;
  case 4 :
    if (j->wires[k]->driver != j->wires[k]) {
      join_treereverse_aux(j->wires[((k+1)%4)]);
    }

    ob_touch(j);
    j->wires[k] = NULL;

    wire_finalizeNet(j->wires[((k+1)%4)]);

    if (M) {
      ob_touch(M);
      M->m_wires = wire_unlink(M->m_wires,n->end);
    }
    wire_free(n->end);
    delete_GWireNode(n);

    return 1;
  }
  return 0;
}

/*
  A wire leading to a splice has been cut, we need to decide if we should
  nuke this splice or not.  A splice will be nuked if all other wires leading to
  it are not attached to anything.
*/
int joint_desplice(GWireNode *n,GModuleDef *M)
{
  GCElement *g;
  GWire *w1,*w2,*we1,*we2;

  g = n->end->gate;
  switch (g->typeinfo->Code) {
  case GC_TAP :
    if (n->end == g->wires[TAP_IN]) {
      w1 = g->wires[TAP_OUT];
      w2 = g->wires[TAP_TAP];
      we1 = wire_drivee(w1);
      we2 = wire_drivee(w2);
    } else if (n->end == g->wires[TAP_OUT]) {
      w1 = g->wires[TAP_IN];
      w2 = g->wires[TAP_TAP];
      we1 = w1->driver;
      we2 = wire_drivee(w2);
    } else if (n->end == g->wires[TAP_TAP]) {
      w1 = g->wires[TAP_OUT];
      w2 = g->wires[TAP_IN];
      we1 = wire_drivee(w1);
      we2 = w2->driver;
    } else {
      logError(ERL_WARN,"Wire not found on splice in joint_desplice.");
      return 0;
    }
    if ((!we1->gate)&&(!we2->gate)) {
      int N = GCElement_numPads(g);
      int i;

      gate_unattachwirelist(w1,M,1);
      gate_unattachwirelist(w2,M,1);

      ob_touch(g);
      for (i = 0;i < N;i++)
	g->wires[i] = NULL;
      gate_draw(g,0);
      gate_remove(M,g);
      ob_free(g);
      ob_touch(M);
      M->m_wires = wire_unlink(M->m_wires,n->end);
      wire_free(n->end);
      delete_GWireNode(n);
      return 1;
    }
    return 0;
  default :
    logError(ERL_WARN,"Unknown splice element found in joint_desplice.");
    return 0;
  }
}

void Joint_VerSave(FILE *f,GCElement *g)
{
  int i;
  GWire *w;
  int N = GCElement_numPads(g);

  w = 0;

  /*
     Gate any pin on the joint.
   */
  for (i = 0;i < N;i++)
    if ((w = g->wires[i])) break;

  fprintf(f,"  //: joint %s (%s) @(%d, %d)",g->ename,w->net->n_signame,g->xpos,g->ypos);
  if (g->anchored)
    fprintf(f," /anc:1");
    

  fprintf(f," /w:[");
  for (i = 0;i < N;i++)
    if ((w = g->wires[i]))
      fprintf(f," %d",w->nidx);
    else
      fprintf(f," -1");
  fprintf(f," ]\n");
}

/*
  Add a stub to a joint with three wires.
 */
void joint_addstub(GCElement *g,EditState *es)
{
  int idx;
  GWire *new_w1,*new_w2;
  GWire *w = 0;			/* Any existing wire on joint */
  int x1,y1,x2,y2;

  /*
   * Find position 
   */
  for (idx = 0;idx < 4;idx++)
    if (!g->wires[idx]) break;

  if (g->wires[0])
    w = g->wires[0];
  else
    w = g->wires[1];

  if (idx >= 4) return;

  wire_newNetSegment(es->env,w->net,&new_w1,&new_w2);

  GNet_draw(w->net);

  ob_touch(g);
  ob_touch(new_w1);
  ob_touch(new_w2);

  g->wires[idx] = new_w1;
  new_w1->gate = g;
  new_w1->orient = idx;
  new_w2->orient = (idx+2)%4;

  x1 = g->xpos + (idx == 0)*2 + (idx == 2)*-2;
  y1 = g->ypos + (idx == 3)*2 + (idx == 1)*-2;
  x2 = g->xpos + (idx == 0)*12 + (idx == 2)*-12;
  y2 = g->ypos + (idx == 3)*12 + (idx == 1)*-12;

  ob_touch(new_w1->nodes);
  ob_touch(new_w2->nodes);

  new_w1->nodes->x = x1;
  new_w1->nodes->y = y1;
  new_w2->nodes->x = x2;
  new_w2->nodes->y = y2;

  GWireNode_setends(new_w1->nodes);
  wire_finalizeNet(new_w1);
  GNet_draw(w->net);
}

void Joint_Rotate(GCElement *g, int centX, int centY,int rdir)
{
  int x = g->xpos;
  int y = g->ypos;

  ob_touch(g);
  g->xpos = rotateX(x -centX,y - centY, rdir) + centX;
  g->ypos = rotateY(x -centX,y - centY, rdir) + centY;
}

GWireNode *Joint_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  joint_fixwires(w->gate,w,retry);
  return w->nodes;
}

void init_joint()
{
  Pixmap P;

  P = Pixmap_registerFromFile("joint","joint.b");
  gateinfo_1iconInit(&gate_joint_info,P,joint_iconDims,joint_iconBoldOffset);
  RegisterGate(&gate_joint_info);
}

