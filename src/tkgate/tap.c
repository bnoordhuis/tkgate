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

    Last edit by hansen on Mon Jan 19 18:18:37 2009
****************************************************************************/
#include <stdlib.h>
#include "tkgate.h"


GCElement *Tap_Make(EditState **es,GModuleDef *env,int GType,
			  int x,int y,int r,const char *Name,int noWires,const char**,int);
void Tap_Draw(GCElement *g,int md);
void Tap_Init(GCElement *g);
void Tap_PSWrite(GPrint *P,GModLayout*,GCElement *g);
void Tap_Delete(GCElement *g,GModuleDef *env,int drawp);
void Tap_VerSave(FILE *f,GCElement *g);
void Tap_SetProp(GCElement *g,const char *prop,const void *value);
int Tap_EditProps(GCElement *g,int isLoadDialog);
GCElement *Tap_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
void Tap_VersionDelta(GCElement*,Version*); 
GWireNode *Tap_wireSnap(GCElement *g,GWire *w,int *mod,int retry);

#if 0
static iconDimensions tap_iconDims[] = {
  {0, 0, 3, 7, 1, 3},
  {8, 0, 7, 3, 3, 1},
  {4, 0, 3, 7, 1, 3},
  {8, 4, 7, 3, 3, 1},
};
#endif

static iconDimensions tap_iconDims[] = {
  {0, 0, 3, 7, 2, 3},
  {8, 0, 7, 3, 3, 2},
  {4, 0, 3, 7, 2, 3},
  {8, 4, 7, 3, 3, 2},
};
static int tap_iconBoldOffset = 8;

GPadLoc tap_in_loc[] = {
	{-3,0,-3,0,D_UP},
	{0,2,0,2,D_LEFT},
	{2,0,2,0,D_DOWN},
	{0,-3,0,-3,D_RIGHT}};

GPadLoc tap_out_loc[] = {
	{-3,1,-3,1,D_DOWN},
	{1,2,1,2,D_RIGHT},
	{2,-1,2,-1,D_UP},
	{-1,-3,-1,-3,D_LEFT}};
#if 0
GPadLoc tap_tap_loc[] = {
	{2,0,2,0,D_RIGHT},
	{0,-2,0,-2,D_UP},
	{-2,0,-2,0,D_LEFT},
	{0,2,0,2,D_DOWN}};
#endif
GPadLoc tap_tap_loc[] = {
	{2,0,2,0,D_RIGHT},
	{0,-2,0,-2,D_UP},
	{-2,0,-2,0,D_LEFT},
	{0,2,0,2,D_DOWN}};


static char *psTap[] = {
  "%",
  "% A Splice",
  "/pstap {",
  "  [[-2 0][0 -1][-1 0][0 -2]] adjstartgate",
  "  -4 -3 moveto",
  "  -2 -3 lineto",
  "  2 0 lineto",
  "  -2 3 lineto",
  "  -4 3 lineto",
  "  8 rfont",
  "  4 4 moveto show",
  "  closepath fill",
  "  grestore",
  "} def",
  0
};


GGateInfo gate_tap_info = {
  GC_TAP,
  "TAP",
  "tran",0x0,
  "pstap",psTap,
  -1,-1,

  {{0}},

  tap_iconDims,

  3,{{"Z",OUT,1,1,tap_tap_loc},
     {"I",TRI,8,1,tap_in_loc},
     {"D",TRI,8,1,tap_out_loc}},
  {{5,-5,LJ},{-5,-5,RJ},{-5,-5,RJ},{-5,10,RJ}},
  {1,1},
  
  {0},

  Tap_Make,
  Nop_WriteCellDef,
  Tap_Init,
  Tap_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Tap_Draw,
  Generic_Move,
  Tap_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Tap_PSWrite,
  Tap_EditProps,
  Tap_VerSave,
  Tap_SetProp,
  Tap_VersionDelta,
  Tap_wireSnap
};

GCElement *Tap_Make(EditState **es,GModuleDef *env,int GType,
			  int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g;
  const char *Side,*Range;
  
  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions)))
    return NULL;

  Side = seekOption("-side",options,nOptions);
  Range = seekOption("-range",options,nOptions);

  if (!Side || sscanf(Side,"%d",&g->u.tap.spliceside) != 1)
    g->u.tap.spliceside = 0;

  if (!Range || sscanf(Range,"%d:%d",&g->u.tap.msb,&g->u.tap.lsb) != 2) {
    if (g->wires[TAP_TAP])
      g->u.tap.msb = g->wires[TAP_TAP]->net->n_nbits - 1;
    else
      g->u.tap.msb = 0;
    g->u.tap.lsb = 0;
  }

  return g;
}

GCElement *Tap_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  ng = Generic_Copy(M,g,x,y,flags);
  ob_touch(ng);

  ng->u.tap.msb = g->u.tap.msb;
  ng->u.tap.lsb = g->u.tap.lsb;
  ng->u.tap.spliceside = g->u.tap.spliceside;

  return ng;
}

void Tap_Init(GCElement *g)
{
  ob_touch(g);
  g->u.tap.msb = g->u.tap.lsb = 0;
  g->u.tap.spliceside = 0;
}

/*
 * Deleting a tap differs in that instead of cutting off all of the wires, the
 * TAP_IN and TAP_OUT connections need to be reattached.  However, if these
 * wires are not connected to anything, then the whole structure should be
 * deleted.
 */
void Tap_Delete(GCElement *g,GModuleDef *M,int drawp)
{
  GWire *iw = g->wires[TAP_IN];
  GWire *ow = g->wires[TAP_OUT];
  GWire *o_iw;
  GWire *o_ow;
  GNet *net;

  net = iw ? iw->net : (ow ? ow->net : 0);
  if (drawp) GNet_draw(net);

  if (iw) {
    ob_touch(iw);
    iw->gate = 0;
    o_iw = wire_other(iw);
  } else
    o_iw = 0;

  if (ow) {
    ob_touch(ow);
    ow->gate = 0;
    o_ow = wire_other(ow);
  } else
    o_ow = 0;


  ob_touch(g);
  g->wires[TAP_IN] = 0;
  g->wires[TAP_OUT] = 0;

  if (iw && ow) {
    if (o_iw->gate || o_ow->gate) {
      /*
       * If either end is connected to a gate, rejoin the wires
       */
    
      ob_touch(M);
      M->m_wires = wire_unlink(M->m_wires,iw);
      M->m_wires = wire_unlink(M->m_wires,ow);

      if (iw->nodes->out && ow->nodes->out)
	join_treereverse(ow);

      if (iw->nodes->out && ow->nodes->in) {
	ob_touch(ow->nodes);
	ob_touch(iw->nodes);
	ob_touch(o_iw);
	ow->nodes->out = iw->nodes;
	iw->nodes->in = ow->nodes;
	o_iw->driver = ow->driver;
      } else if (ow->nodes->out && iw->nodes->in) {
	ob_touch(ow->nodes);
	ob_touch(iw->nodes);
	ob_touch(o_iw);
	ow->nodes->in = iw->nodes;
	iw->nodes->out = ow->nodes;
	o_ow->driver = iw->driver;
      } else
	printf("huh! at %s, %d\n",__FILE__,  __LINE__);

      iw->nodes->x = ow->nodes->x;
      iw->nodes->y = ow->nodes->y;

      ob_touch(iw->nodes);
      ob_touch(ow->nodes);
      ob_touch(net);
      ow->nodes->end = 0;
      iw->nodes->end = 0;

      GWire_snap(wirenode_driver(ow->nodes));

      if (net->n_driver == iw) {
	net->n_driver = o_iw;
      } else if (net->n_driver == ow) {
	net->n_driver = o_ow;
      }
      wire_free(iw);
      wire_free(ow);
      wire_finalizeNet(net->n_driver);
      if (drawp) GNet_draw(net);
    } else {
      /*
       * Wire is not connected to anything else, delete it entirely.
       */
#if 0
      GWire_draw(iw->driver);
      GWire_draw(ow->driver);
#endif
      wire_nuke(iw,0,M);
      wire_nuke(ow,0,M);
    }
  }
  Generic_Delete(g,M,drawp);
}

/* Used by Draw and PSDraw */
static short dx[] = {-1,0,1,0};
static short dy[] = {0,1,0,-1};

void Tap_Draw(GCElement *g,int md)
{
  int o,x,y;
  char buf[STRMAX];

  if (!g->u.tap.spliceside)
    mk_gate(g->xpos,g->ypos,g->typeinfo,g->orient,g->selected);
  else {
    mk_gate(g->xpos+5*dx[g->orient],g->ypos+5*dy[g->orient],
	    g->typeinfo,(g->orient+2)%4,g->selected);
  }

  gate_drawWires(g,md);
  
  if (!g->ename) return;
  
  
  o = (g->orient + 2*g->u.tap.spliceside) % 4;
  if (g->u.tap.spliceside) {
    x = g->xpos+5*dx[g->orient];
    y = g->ypos+5*dy[g->orient];
  } else {
    x = g->xpos;
    y = g->ypos;
  }
  
  if (g->u.tap.lsb == g->u.tap.msb)
    sprintf(buf,"%d",g->u.tap.msb);
  else
    sprintf(buf,"%d:%d",g->u.tap.msb,g->u.tap.lsb);

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);
  dce_DrawString(TkGate.instGC,x + g->typeinfo->lpos[o].x,
		 y + g->typeinfo->lpos[o].y,
		 g->typeinfo->lpos[o].just,buf);
}

void Tap_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  int O;
  char buf[STRMAX];

  O = (g->orient + (g->u.tap.spliceside ? 2 : 0)) % 4;

  Generic_PSLabels(P,g);

  if (g->u.tap.lsb == g->u.tap.msb)
    sprintf(buf,"%d",g->u.tap.msb);
  else
    sprintf(buf,"%d:%d",g->u.tap.msb,g->u.tap.lsb);

  if (!g->u.tap.spliceside)
    fprintf(P->p_f,"(%s) %d %d %d %s\n",buf,g->xpos,g->ypos,-O*90,g->typeinfo->psprint);
  else
    fprintf(P->p_f,"(%s) %d %d %d %s\n",buf,g->xpos+7*dx[g->orient],g->ypos+7*dy[g->orient],
	    -O*90,g->typeinfo->psprint);
}

void Tap_VerSave(FILE *f,GCElement *g)
{
  if (g->u.tap.msb != g->u.tap.lsb)
    fprintf(f,"  assign %s = %s[%d:%d]; //: TAP %s",
	    g->wires[TAP_TAP]->net->n_signame,
	    g->wires[TAP_IN]->net->n_signame,
	    g->u.tap.msb,g->u.tap.lsb,
	    g->ename);
  else
    fprintf(f,"  assign %s = %s[%d]; //: TAP %s",
	    g->wires[TAP_TAP]->net->n_signame,
	    g->wires[TAP_IN]->net->n_signame,
	    g->u.tap.msb,
	    g->ename);

  VerilogBasicGateComment(f,g,0);
  fprintf(f," /ss:%d",g->u.tap.spliceside);
  fprintf(f,"\n");
}

void Tap_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/ss") == 0) {
    int n = *((int*)value);
    ob_touch(g);
    g->u.tap.spliceside = n;
  }
}

int Tap_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  ob_touch(g);

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    if (g->u.tap.msb == g->u.tap.lsb)
      DoTcl("set ::edgat_tap \"%d\"",g->u.tap.msb);
    else
      DoTcl("set ::edgat_tap \"%d:%d\"",g->u.tap.msb,g->u.tap.lsb);
  } else {
    const char *p;
    int msb,lsb;

    if ((p = Tcl_GetVar(tcl,"edgat_tap",TCL_GLOBAL_ONLY))) {
      if (sscanf(p,"%d:%d",&msb,&lsb) == 2) {
	g->u.tap.msb = msb;
	g->u.tap.lsb = lsb;
      } else if (sscanf(p,"%d",&msb) == 1) {
	g->u.tap.msb = g->u.tap.lsb = msb;
      }
    }
  }
  return 0;
}

/*
 * Tweek positions of wires for versions before 1.6
 */
void Tap_VersionDelta(GCElement *g,Version *V)
{
  Version x1 = {"1.6", 1, 6, 0};
  int dx = 0,dy = 0,o;
  GWire *w = g->wires[TAP_TAP];

  if (VersionCmp(V,&x1) >= 0)
    return;

  o = wireorient(w->nodes,0);

  if (g->u.tap.spliceside)
    o = (o+2)%4;

  switch (o) {
  case 0 :
    dx = -1;
    dy = 0;
    break;
  case 1 :
    dx = 0;
    dy = -1;
    break;
  case 2 :
    dx = -1;
    dy = 0;
    break;
  case 3 :
    dx = 0;
    dy = -1;
    break;
  }

  ob_touch(w->nodes);
  w->nodes->x += dx;
  w->nodes->y += dy;
} 

GWireNode *Tap_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;
  static short dx[] = {-9,0,9,0};
  static short dy[] = {0,9,0,-9};

  if (posongate(w,w->gate,&p,&n) != 0)
    return w->nodes;

  switch (p) {
  case TAP_IN :
  case TAP_OUT :
    *mod = wire_force(w,
		      w->gate->typeinfo->Pad[p].Loc[w->gate->orient].dir,
		      retry);
    break;
  case TAP_TAP :
    if (w->gate->u.tap.spliceside !=
	(w->gate->typeinfo->Pad[p].Loc[w->gate->orient].dir !=
	 wireorient(w->nodes,0))) {

      gate_draw(w->gate,0);
      ob_touch(w->gate);
      w->gate->u.tap.spliceside = 
	w->gate->typeinfo->Pad[p].Loc[w->gate->orient].dir!=
	wireorient(w->nodes,0);

      gate_draw(w->gate,0);
      if (w->gate->u.tap.spliceside)
	wire_move(w->nodes,dx[w->gate->orient],
		  dy[w->gate->orient],VERTICAL | HORIZONTAL);
      else
	wire_move(w->nodes,-dx[w->gate->orient],
		  -dy[w->gate->orient],VERTICAL | HORIZONTAL);
    }
    break;
  }

  return w->nodes;
}


/*
  Change the joint at the end of 'branch' into a splice.
*/
void tap_transmute(GWire *branch,EditState *es)
{
  GModuleDef *env;
  GWire *in,*out;
  GCElement *g;
  int i,o;
  int tap_dx = 0,tap_dy = 0;
 
  g = branch->gate;
  env = es->env;
#ifdef JOIN_DRAW
  gate_draw(g,0);
#endif
  in = out = NULL;

  for (i = 0;i < 4;i++)
    if (g->wires[i] &&
	(g->wires[i] != branch)) {
      if (out)
	in = g->wires[i];
      else
	out = g->wires[i];
    }
  
  ob_touch(g);
  g->typeinfo = GGateInfo_codeLookup(GC_TAP);
  g->wires[TAP_IN] = in;
  g->wires[TAP_OUT] = out;
  g->wires[TAP_TAP] = branch;

  g->u.tap.msb = g->wires[TAP_TAP]->net->n_nbits - 1;
  g->u.tap.lsb = 0;
  
  o = wireorient(out->nodes,0);
  g->orient = (o=(o+1)%4);
  g->u.tap.spliceside = 
    g->typeinfo->Pad[TAP_TAP].Loc[o].dir !=
      wireorient(branch->nodes,0);

  switch (o) {
  case 0 :
    g->xpos += 3;
    wire_move(out->nodes,0,-2,FULL|NOSTRAIGHTEN);
    wire_move(in->nodes,0,1,FULL|NOSTRAIGHTEN);
    tap_dx = g->u.tap.spliceside ? -2 : 3;
    tap_dy = 0;
    break;
  case 1 :
    g->ypos -= 2;
    wire_move(out->nodes,-2,0,FULL|NOSTRAIGHTEN);
    wire_move(in->nodes,1,0,FULL|NOSTRAIGHTEN);
    tap_dx = 0;
    tap_dy = g->u.tap.spliceside ? 2 : -3;
    break;
  case 2 :
    g->xpos -= 2;
    wire_move(out->nodes,0,1,FULL|NOSTRAIGHTEN);
    wire_move(in->nodes,0,-2,FULL|NOSTRAIGHTEN);
    tap_dx = g->u.tap.spliceside ? 2 : -3;
    tap_dy = 0;
    break;
  case 3 :
    g->ypos += 3;
    wire_move(out->nodes,1,0,FULL|NOSTRAIGHTEN);
    wire_move(in->nodes,-2,0,FULL|NOSTRAIGHTEN);
    tap_dx = 0;
    tap_dy = g->u.tap.spliceside ? -2 : 3;
    break;
  }
  wire_move(branch->nodes,tap_dx,tap_dy,FULL|NOSTRAIGHTEN);


  GWire_snap(branch);
  GWire_snap(out);
  GWire_snap(in->driver);
#ifdef JOIN_DRAW
  gate_draw(g,0);
#endif
}

void init_tap()
{
  Pixmap P;

  P = Pixmap_registerFromFile("tap","tap.b");
  gateinfo_iconInit(&gate_tap_info,P,tap_iconDims,tap_iconBoldOffset);
  RegisterGate(&gate_tap_info);
}
