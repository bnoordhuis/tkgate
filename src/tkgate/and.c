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

    Last edit by hansen on Sat Feb 21 17:45:19 2009
****************************************************************************/
#include "tkgate.h"

#define AOX_WIRESPACE	5	/* Space between ports when using extender bars */
#define AOX_HOOK	5

#define AND_IN 0
#define AND_OUT 1

/*
 * AOX=AND/OR/XOR specific operations
 */
GCElement *AOX_Make(EditState **,GModuleDef *,int,int,int,int,const char *,int,const char**,int);
void AOX_Draw(GCElement *g,int md);
void AOX_AddInput(EditState *es,GCElement*);
void AOX_VerSave(FILE*,GCElement*);
void AOX_SetProp(GCElement*,const char*,const void*);
int AOX_EditProps(GCElement *g,int isLoadDialog);
void AOX_PSWrite(GPrint *P,GModLayout*,GCElement *g);
void AOX_RemovePort(EditState *es,GCElement *g,GWire *);
GCElement *AOX_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
void AOX_Rotate(GCElement *g,int centX, int centY,int rdir);
void AOX_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions and_iconDims[] = {
  {0, 0, 20, 15, 10, 7},
  {21, 0, 15, 20, 7, 9},
  {16, 21, 20, 15, 9, 7},
  {0, 16, 15, 20, 7, 10},
};
static int and_iconBoldOffset = 37;

GPadLoc and_out_loc[] ={		/* Gate output */
 {10,0,10,0,D_RIGHT},
 {0,-10,0,-10,D_UP},
 {-10,0,-10,0,D_LEFT},
 {0,10,0,10,D_DOWN}};

GPadLoc and_in_loc[] = {		/* Gate input */
 {-11,-7,-11,7,D_LEFT},
 {-7,11,7,11,D_DOWN},
 {11,-7,11,7,D_RIGHT},
 {-7,-11,7,-11,D_UP}};

static char *psAnd[] = {
  "%",
  "% An AND gate",
  "%",
  "/psand {",
  "  startgate",
  "  2.5 7 moveto",
  "  -10.5 7 lineto",
  "  -10.5 -7 lineto",
  "  2.5 -7 lineto",
  "  2.5 0 7 -90 90 arc",
  "  closepath",
  "  stroke",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_and_info = {
  0,
  "AND",
  "and:nand",0x2,
  "psand",psAnd,
  0,0,

  {{"a",	{"gm.gate",0},		{"gm.gate.and",0,0,100},	"gat_make AND"},
   {"A",	{"gm.gate",0},		{"gm.gate.nand",3,0,101},	"gat_make AND -invert Z"},
   {"Ctl-r a",	{"gm.rgate",0},		{"gm.rgate.and",0,0,100},	"gat_make AND -pins I=1"},
   {"Ctl-r A",	{"gm.rgate",0},		{"gm.rgate.nand",3,0,101},	"gat_make AND -pins I=1 -invert Z"},
   {0}},

  and_iconDims,

  2,{{"I",IN,1,2,and_in_loc,1},{"Z",OUT,1,1,and_out_loc,0}},
  {{0,-12,CT},{12,0,LJ},{0,-12,CT},{12,0,LJ}},
  {1},

  {"Diz",0},
  
  AOX_Make,
  AOX_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  AOX_Draw,
  Generic_Move,
  AOX_Copy,
  AOX_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  AOX_Rotate,
  AOX_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  AOX_PSWrite,
  AOX_EditProps,
  AOX_VerSave,
  AOX_SetProp,
  0,
  0
};

GCElement *AOX_Make(EditState **es,GModuleDef *env,int GType,
			int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions);

  ob_touch(g);
  g->u.basic.extbar = TkGate.circuit->useExtBars;

  return g;
}

/*
 * Get coordinates for extender bars.  The is_draw flag should be '1' if coordinates
 * are for screen use, and 0 if coordinates are for postscript use.
 */
void AOX_extBarCoords(GCElement *g,int is_draw,int _x1[2],int _y1[2],int _x2[2],int _y2[2],int *b_dx,int *b_dy)
{
  int N = wire_numOnPad(g->wires[AND_IN]);
  int D = (N+1)*AOX_WIRESPACE;						/* Total length required */
  int x1 = g->xpos + g->typeinfo->Pad[AND_IN].Loc[g->orient].x1;
  int y1 = g->ypos + g->typeinfo->Pad[AND_IN].Loc[g->orient].y1;
  int x2 = g->xpos + g->typeinfo->Pad[AND_IN].Loc[g->orient].x2;
  int y2 = g->ypos + g->typeinfo->Pad[AND_IN].Loc[g->orient].y2;

  if (is_draw) {
    x1 = ctow_x(x1);
    y1 = ctow_y(y1);
    x2 = ctow_x(x2);
    y2 = ctow_y(y2);
  }

  switch (g->orient) {
  case D_RIGHT :
    D = (D-(y2-y1-1));		/* distance to add */
    _x1[0] = x1+1;		_y1[0] = y1-1;
    _x2[0] = x1+1;		_y2[0] = y1-D/2;
    _x1[1] = x1+1;		_y1[1] = y2+1;
    _x2[1] = x1+1;		_y2[1] = y2+D-(D/2);
    *b_dx = 1;			*b_dy = 0;
    break;
  case D_LEFT : 
    D = (D-(y2-y1-1));		/* distance to add */
    _x1[0] = x1-1;		_y1[0] = y1-1;
    _x2[0] = x1-1;		_y2[0] = y1-D/2;
    _x1[1] = x1-1;		_y1[1] = y2+1;
    _x2[1] = x1-1;		_y2[1] = y2+D-(D/2);
    *b_dx = -1;			*b_dy = 0;
    break;
  case D_UP :
    D = (D-(x2-x1-1));		/* distance to add */
    _x1[0] = x1-1;		_y1[0] = y1-1;
    _x2[0] = x1-D/2;		_y2[0] = y1-1;
    _x1[1] = x2+1;		_y1[1] = y1-1;
    _x2[1] = x2+D-(D/2);	_y2[1] = y1-1;
    *b_dx = 0;			*b_dy = -1;
    break;
  case D_DOWN :
    D = (D-(x2-x1-1));		/* distance to add */
    _x1[0] = x1-1;		_y1[0] = y1+1;
    _x2[0] = x1-D/2;		_y2[0] = y1+1;
    _x1[1] = x2+1;		_y1[1] = y1+1;
    _x2[1] = x2+D-(D/2);	_y2[1] = y1+1;
    *b_dx = 0;			*b_dy = 1;
    break;
  }
}

void AOX_Draw(GCElement *g,int md)
{
  int N = wire_numOnPad(g->wires[AND_IN]);

  Generic_Draw(g,md);

  if (N > 2 && g->u.basic.extbar) {
    int x1[2],y1[2],x2[2],y2[2],b_dx,b_dy,i;

    AOX_extBarCoords(g,1,x1,y1,x2,y2,&b_dx,&b_dy);

    for (i = 0;i < 2;i++) {
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x1[i],y1[i],x2[i],y2[i]);
      if (g->selected)
	ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x1[i]+b_dx,y1[i]+b_dy,x2[i]+b_dx,y2[i]+b_dy);
    }
  }
}

static void AOX_adjustWires(GCElement *g)
{
  int N,L,i,wx,wy,dx,dy;
  GWire *w;
  int d = 1;

  wx = wy = dx = dy = 0;

  N = wire_numOnPad(g->wires[AND_IN]);
  L = AOX_WIRESPACE*(N+1);
  
  
  switch (g->orient) {
  case 0 :
    wx = g->xpos + g->typeinfo->Pad[AND_IN].Loc[g->orient].x1;
    wy = g->ypos - d*L/2;
    dx = 0;
    dy = d*AOX_WIRESPACE;
    break;
  case 1 :
    wx = g->xpos - d*L/2;
    wy = g->ypos + g->typeinfo->Pad[AND_IN].Loc[g->orient].y1;
    dx = d*AOX_WIRESPACE;
    dy = 0;
    break;
  case 2 :
    wx = g->xpos + g->typeinfo->Pad[AND_IN].Loc[g->orient].x1;
    wy = g->ypos + d*L/2;
    dx = 0;
    dy = -d*AOX_WIRESPACE;
    break;
  case 3 :
    wx = g->xpos + d*L/2;
    wy = g->ypos + g->typeinfo->Pad[AND_IN].Loc[g->orient].y1;
    dx = -d*AOX_WIRESPACE;
    dy = 0;
    break;
  }
  
  for (i = 0,w = g->wires[AND_IN];w;i++, w = w->next) {
    wx += dx;
    wy += dy;
    wire_move(w->nodes,wx-w->nodes->x,wy-w->nodes->y,VERTICAL|HORIZONTAL);
  }
}


void AOX_AddInput(EditState *es,GCElement *g)
{
  if (!g->u.basic.extbar)
    Generic_AddInput(es,g);		/* Legacy gate, use generic add function */ 
  else {
    GWire *e1,*e2;

    gate_draw(g,0);

    wire_new(es->env,&e1,&e2);

    ob_touch(g);
    ob_touch(e1->nodes);
    ob_touch(e2->nodes);
    ob_touch(e1);
    ob_touch(e2);

    e1->nodes->x = e2->nodes->x = 0;
    e1->nodes->y = e2->nodes->y = 0;

    g->wires[AND_IN] = wire_append(g->wires[AND_IN],e1);
    e1->gate = g;
    e1->orient = g->typeinfo->Pad[AND_IN].Loc[g->orient].dir;

    switch (e1->orient) {
    case D_RIGHT :
      e2->nodes->x += TKGATE_STUBLEN;
      break;
    case D_UP :
      e2->nodes->y -= TKGATE_STUBLEN;
      break;
    case D_LEFT :
      e2->nodes->x -= TKGATE_STUBLEN;
      break;
    case D_DOWN :
      e2->nodes->y += TKGATE_STUBLEN;
      break;
    }

    wire_finalizeNet(e1);
    AOX_adjustWires(g);

    gate_draw(g,0);
  }
}

void AOX_RemovePort(EditState *es,GCElement *g,GWire *w)
{
  Generic_RemovePort(es,g,w);
  AOX_adjustWires(g);
}


int AOX_GateParmList(FILE *f,GCElement *g)
{
  GGateInfo *gi = g->typeinfo;
  GWire *w;
  int i,j;
  int first_pin;
  int nbits = g->wires[AND_OUT]->net->n_nbits;

  first_pin = 1;
  fprintf(f," (");
  for (i = 0;i < gi->NumPads;i++) {
    for (w = g->wires[i], j=0;w;w = w->next, j++) {
      char *inv = "";

      if (!first_pin)
	fprintf(f,", ");
      first_pin = 0;

      if (gi->Pad[i].iotype == IN && w->invert)
	inv = "!";

      if (gi->Pad[i].CanAdd)
	fprintf(f,".%s%d",gi->Pad[i].Name,j);
      else
	fprintf(f,".%s",gi->Pad[i].Name);
      if (w->net->n_signame) {
	fprintf(f,"(%s",inv);
	if (w->net->n_nbits == 1 && nbits > 1)
	  fprintf(f,"{%d{%s}}",nbits,w->net->n_signame);
	else
	  fprintf(f,"%s",w->net->n_signame);
	fprintf(f,")");
      } else
	fprintf(f,"(%sw%x)",inv,(unsigned)w->net);
    } 
  }
  fprintf(f,");");
  return 0;
}

void AOX_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  AOX_GateParmList(f,g);
  VerilogBasicGateComment(f,g,1);

  if (g->u.basic.extbar != TkGate.circuit->useExtBars)
    fprintf(f," /eb:%d",g->u.basic.extbar);

  fprintf(f,"\n");
}

void AOX_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/eb") == 0) {
    int n = *((int*)value);
    g->u.basic.extbar = n;
  }
}

void AOX_PSWrite(GPrint *P,GModLayout *l,GCElement *g)
{
  int N = wire_numOnPad(g->wires[AND_IN]);

  Generic_PSWrite(P,l,g);

  if (N > 2 && g->u.basic.extbar) {
    int x1[2],y1[2],x2[2],y2[2],b_dx,b_dy,i;

    AOX_extBarCoords(g,0,x1,y1,x2,y2,&b_dx,&b_dy);
    
    for (i = 0;i < 2;i++)
      fprintf(P->p_f,"gsave %d %d %d %d moveto lineto stroke grestore\n",x1[i],y1[i],x2[i],y2[i]);
  }
}

GCElement *AOX_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng = Generic_Copy(M,g,x,y,flags);

  ob_touch(ng);
  ng->u.basic.extbar = g->u.basic.extbar;

  return ng;
}

int AOX_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_extbar %d",g->u.basic.extbar);
  } else {
    const char *p;
    if ((p = Tcl_GetVar(tcl,"edgat_extbar",TCL_GLOBAL_ONLY))) {
      int eb;

      sscanf(p,"%d",&eb);

      if (eb != g->u.basic.extbar) {
	ob_touch(g);
	g->u.basic.extbar = eb;
	gate_draw(g,0);
	if (eb)
	  AOX_adjustWires(g);
	else
	  wire_setPadSpacing(g,AND_IN);
	gate_draw(g,0);
      }
    }
  }

  return 0;
}

void AOX_Rotate(GCElement *g,int centX, int centY,int rdir)
{
  Generic_Rotate(g,centX,centY,rdir);
#if 0
  if (g->u.basic.extbar)
    AOX_adjustWires(g);
  else
    wire_setPadSpacing(g,AND_IN);
#endif
}

/*****************************************************************************
 *
 * Generate primitive cell definition for AND, OR and XOR gates.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
void AOX_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int multiPad = gcs->gc_multiPad;
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  PrimParm primParm;
  const char *type = "and";

  if (multiPad == 1) {
    if (strcmp(gi->name,"OR") == 0)
      type = "ror";
    else if (strcmp(gi->name,"XOR") == 0)
      type = "rxor";
    else
      type = "rand";
  } else {
    if (strcmp(gi->name,"OR") == 0)
      type = "or";
    else if (strcmp(gi->name,"XOR") == 0)
      type = "xor";
    else
      type = "and";
  }

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"Z_RANGE", (multiPad ==1) ? 1 : numBit);
  PrimParm_rangeSet(&primParm,"I_RANGE", numBit);
  PrimParm_invSet(&primParm,"invZ", (*invSpec == 'N'));
  PrimParm_intSet(&primParm,"NUMIN", multiPad);
  Primitive_write(f,type,gcs,&primParm);
}

void init_and()
{
  Pixmap P;

  P = Pixmap_registerFromFile("and","and.b");
  gateinfo_iconInit(&gate_and_info,P,and_iconDims,and_iconBoldOffset);
  RegisterGate(&gate_and_info);
}
