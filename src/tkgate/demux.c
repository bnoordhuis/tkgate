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

    Last edit by hansen on Mon Jan 19 18:14:35 2009
****************************************************************************/
#include "tkgate.h"

#define DEMUX_F 0
#define DEMUX_S 1
#define DEMUX_EN 2
#define DEMUX_OUT 3

void Demux_Delete(GCElement *g,GModuleDef *env,int drawp);
void Demux_Draw(GCElement *g,int md);
GCElement *Demux_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char**,int);
void Demux_PSWrite(GPrint *P,GModLayout*,GCElement *g);
void Demux_Rotate(GCElement *g, int centX, int centY,int rdir);
GCElement *MuxDemux_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
void Demux_SetProp(GCElement*,const char*,const void*);
int MuxDemux_EditProps(GCElement *g,int isLoadDialog);
GWireNode *Demux_wireSnap(GCElement *g,GWire *w,int *mod,int retry);
void Demux_AddOutput(EditState *es,GCElement *g);
void Demux_VerSave(FILE *f,GCElement *g);
static void Demux_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions demux_iconDims[] = {
  {29, 60, 59, 29, 30, 12},
  {0,  29, 29, 59, 12, 30},
  {0,  0,  59, 29, 30, 15},
  {60, 0,  29, 59, 15, 30},
};
static int demux_iconBoldOffset = 89;

GPadLoc demux_out_loc[] = {
	{-30,16,30,16,D_DOWN},
	{16,-30,16,30,D_RIGHT},
	{-30,-16,30,-16,D_UP},
	{-16,-30,-16,30,D_LEFT},
};

GPadLoc demux_f_loc[] = {
	{0,-13,0,-13,D_UP},
	{-13,0,-13,0,D_LEFT},
	{0,13,0,13,D_DOWN},
	{13,0,13,0,D_RIGHT},
};

GPadLoc demux_s_loc[] = {
	{-24,0,-24,0,D_LEFT},
	{0,22,0,22,D_DOWN},
	{22,0,22,0,D_RIGHT},
	{0,-24,0,-24,D_UP},
};

GPadLoc demux_en_loc[] = {
	{22,0,22,0,D_RIGHT},
	{0,-24,0,-24,D_UP},
	{-24,0,-24,0,D_LEFT},
	{0,22,0,22,D_DOWN},
};

static char *psDemux[] = {
  "%",
  "% x y r demux",
  "%",
  "/demux {",
  "    startgate",
  "    8 rfont",
  "    (demux) 0 5 rCT",
  "    -16.5 12.5 moveto",
  "    16.5 12.5 lineto",
  "    29.5 -15.5 lineto",
  "    -29.5 -15.5 lineto",
  "    closepath stroke",
  "    dup					% n n",
  "    1 add 58 exch div			% n d1",
  "    2 copy mul				% n d1 dn",
  "    3 -1 roll 1 sub 50 string cvs exch	% d1 (n) dn",
  "    -29 add -12 rCT				% d1",
  "    (0) exch -29 add -12 rCT",
  "    grestore",
  "} def",
  0
};

GGateInfo gate_demux_info = {
  0,
  "DEMUX",
  "demux",0x0,
  "demux",psDemux,
  3,0,

 { {"U 2",	{"gm.msi",0},		{"gm.msi.12demux",0,"xdemux",100},	"gat_make DEMUX "},
   {"U 3",	{0,0},			{0,0,0,0},				"gat_make DEMUX -pins Z=3"},
   {"U 4",	{"gm.msi",0},		{"gm.msi.14demux",0,"xdemux",200},	"gat_make DEMUX -pins Z=4"},
   {"U 5",	{0,0},			{0,0,0,0},				"gat_make DEMUX -pins Z=5"},
   {"U 6",	{0,0},			{0,0,0,0},				"gat_make DEMUX -pins Z=6"},
   {"U 7",	{0,0},			{0,0,0,0},				"gat_make DEMUX -pins Z=7"},
   {"U 8",	{"gm.msi",0},		{"gm.msi.18demux",0,"xdemux",300},	"gat_make DEMUX -pins Z=8"},
   {"U 9",	{0,0},			{0,0,0,0},				"gat_make DEMUX -pins Z=9"},
   {0}},

  demux_iconDims,

  4,{{"F",IN,1,1,demux_f_loc,0},
       {"S",IN,1,1,demux_s_loc,0},
       {"E",IN,1,1,demux_en_loc,0},
       {"Z",OUT,1,2,demux_out_loc,1}},
  {{23,0,LJ},{0,40,CT},{-23,8,RJ},{0,-33,CT}},
  {1},
  
  {"Dez","Dfz","Dsz",0},

  Demux_Make,
  Demux_WriteCellDef,
  Generic_Init,
  Demux_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Demux_Draw,
  Generic_Move,
  MuxDemux_Copy,
  Err_AddInput,
  Demux_AddOutput,
  Err_AddInOut,
  Demux_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Demux_PSWrite,
  MuxDemux_EditProps,
  Demux_VerSave,
  Demux_SetProp,

  0,
  Demux_wireSnap
};

/*
 * Adjust the wires on a mux to conform to the current setting of data_order
 */
static void Demux_adjustWiresData(GCElement *g)
{
  int N,L,i;
  double dx,dy,wx,wy;
  GWire *w;
  GPadLoc *pd = &g->typeinfo->Pad[DEMUX_OUT].Loc[g->orient];


  wx = wy = dx = dy = 0;
  N = wire_numOnPad(g->wires[DEMUX_OUT]);
  L = iabs(pd->x1-pd->x2)  + iabs(pd->y1-pd->y2); 	/* Length of pad (assumed non-diagonal) */

  wx = g->xpos + pd->x1;
  wy = g->ypos + pd->y1;

  switch (g->orient) {
  case 0 :
    dx = (double)L/(double)(N+1);
    if (g->u.mux.data_order) {
      wx = g->xpos + pd->x2;
      dx = -dx;
    }
    break;
  case 1 :
    dy = (double)L/(double)(N+1);
    if (!g->u.mux.data_order) {
      wy = g->ypos + pd->y2;
      dy = -dy;
    }
    break;
  case 2 :
    dx = (double)L/(double)(N+1);
    if (!g->u.mux.data_order) {
      wx = g->xpos + pd->x2;
      dx = -dx;
    }
    break;
  case 3 :
    dy = (double)L/(double)(N+1);
    if (g->u.mux.data_order) {
      wy = g->ypos + pd->y2;
      dy = -dy;
    }
    break;
  }
  
  for (i = 0,w = g->wires[DEMUX_OUT];w;i++, w = w->next) {
    wx += dx;
    wy += dy;
    wire_move(w->nodes,(int)wx-w->nodes->x,(int)wy-w->nodes->y,VERTICAL|HORIZONTAL);
  }
}

/*
 * Adjust the wires on a mux to conform to the current setting of select_side
 */
static void Demux_adjustWiresSelector(GCElement *g,int port)
{
  int wx,wy;
  GPadLoc *pd = &g->typeinfo->Pad[port].Loc[g->orient];
  GWire *w = g->wires[port];
  GWire *ow = wire_other(w);

  if (port == DEMUX_EN) {
    if (g->u.mux.select_side)
      pd = &demux_s_loc[g->orient];
    else
      pd = &demux_en_loc[g->orient];
  } else {
    if (g->u.mux.select_side)
      pd = &demux_en_loc[g->orient];
    else
      pd = &demux_s_loc[g->orient];
  }

  wx = g->xpos + pd->x2;
  wy = g->ypos + pd->y2;

  ob_touch(w);
  wire_move(w->nodes,wx-w->nodes->x,wy-w->nodes->y,VERTICAL|HORIZONTAL);

  /*
   * If wire end is not attached, compute mirror image.  Otherwise use brute force
   * to fix the wire.
   */
  if (w->orient != pd->dir) {
    w->orient = pd->dir;
    if (!ow->gate) {
      int cx = w->nodes->x;
      int cy = w->nodes->y;
      GWireNode *n;


      for (n = w->driver->nodes;n;n = n->out) {
	ob_touch(n);
	if (g->orient == 0 || g->orient == 2)
	  n->x = cx - (n->x - cx);
	else
	  n->y = cy - (n->y - cy);
      }
    } else
      GWire_snap(w->driver);
  }
}

/*
 * Adjust the wires on a mux.
 */
void Demux_adjustWires(GCElement *g)
{
  Demux_adjustWiresData(g);
  Demux_adjustWiresSelector(g,DEMUX_EN);
  Demux_adjustWiresSelector(g,DEMUX_S);
}


GCElement *Demux_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  int nbits;
  GCElement *g;
  GWire *w;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,1,options,nOptions)))
    return NULL;

  g->u.mux.select_side = 0;
  g->u.mux.data_order = 0;

  if (!noWires) {
    int i,j;
    const char *Invert,*Pins;
    int N = GCElement_numPads(g);

    
    Invert = seekOption("-invert",options,nOptions);
    Pins = seekOption("-pins",options,nOptions);

    for (i = 0;i < N;i++) {
      int Num = 0;

      if (Pins) {
	char buf[STRMAX];
	int N;
	if (sscanf(Pins,"%[^=]=%d",buf,&N) == 2 && strcmp(buf,GCElement_getPadName(g,i)) == 0)
	  Num = N;
      }
      if (!Num)
	Num = GCElement_getPadNum(g,i);

      for (j = 0;j < Num;j++) {
	if (Invert && strcmp(Invert,GCElement_getPadName(g,i)) == 0)
	  wire_addToGate(g,i,env,1);
	else
	  wire_addToGate(g,i,env,0);
      }
    }

    Demux_adjustWires(g);


    for (i = 0,w = g->wires[DEMUX_OUT];w;w = w->next,i++);

    nbits = required_bits(i);
    net_setSize(g->wires[DEMUX_S]->net,nbits);
  }

  return g;
}

void Demux_Draw_Special(GCElement *g)
{
  GWire *w,*w0;
  char n[STRMAX];
  int i;

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textXF[TkGate.circuit->zoom_factor]->fid);

  if (!(w0 = g->wires[DEMUX_OUT])) return;
  for (i = 0,w = w0;w->next;w = w->next,i++);
  sprintf(n,"%d",i);

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);

  switch (g->orient) {
  case 0 :
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x,w0->nodes->y - 2,
		   AtBottom|BetweenLeftAndRight,"0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x,w->nodes->y - 2,
		   AtBottom|BetweenLeftAndRight,n);
    break;
  case 1 :
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x - 2,w0->nodes->y,
		   BetweenTopAndBottom|AtRight,"0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x - 2,w->nodes->y,
		   BetweenTopAndBottom|AtRight,n);
    break;
  case 2 :
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x,w0->nodes->y + 2,
		   AtTop|BetweenLeftAndRight,"0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x,w->nodes->y + 2,
		   AtTop|BetweenLeftAndRight,n);
    break;
  case 3:
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x + 3,w0->nodes->y,
		   BetweenTopAndBottom|AtLeft, "0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x + 3,w->nodes->y,
		   BetweenTopAndBottom|AtLeft,n);
    break;
  }
}


void Demux_Delete(GCElement *g,GModuleDef *env,int drawp)
{
  Generic_Delete(g,env,drawp);
}

void Demux_Draw(GCElement *g,int md)
{
  Generic_Draw(g,md);
  Demux_Draw_Special(g);
}


void Demux_Rotate(GCElement *g, int centX, int centY,int rdir)
{
  int x = g->xpos;
  int y = g->ypos;

  ob_touch(g);
  g->xpos = rotateX(x - centX,y - centY,rdir) + centX;
  g->ypos = rotateY(x - centX,y - centY,rdir) + centY;
  g->orient = (g->orient + 4 + rdir) % 4;

  Demux_adjustWires(g);
}


void Demux_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
    int N;
    struct wire *w;

    Generic_PSLabels(P,g);

    for (N = 0, w = g->wires[DEMUX_OUT];w;w = w->next) N++;

    fprintf(P->p_f,"%d %d %d %d %s\n",
	N,g->xpos,g->ypos,-g->orient*90,g->typeinfo->psprint);
}


void Demux_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/ss") == 0) g->u.mux.select_side = *((int*)value);
  if (strcmp(prop,"/do") == 0) g->u.mux.data_order = *((int*)value);

  if (g->wires[DEMUX_EN])
    g->wires[DEMUX_EN]->orient = (g->orient + 2 + (g->u.mux.select_side ? 2 : 0) ) % 4;
}

GWireNode *Demux_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;

  if (posongate(w,w->gate,&p,&n) == 0) {
    GPadLoc *pd = &g->typeinfo->Pad[p].Loc[g->orient];
    if (p == DEMUX_EN && g->u.mux.select_side)
      pd = &demux_s_loc[g->orient];
    if (p == DEMUX_S && g->u.mux.select_side)
      pd = &demux_en_loc[g->orient];
  
    *mod = wire_force(w,pd->dir,retry);
  }
  return w->nodes;
}

void Demux_AddOutput(EditState *es,GCElement *g)
{
  int i;
  int N = GCElement_numPads(g);

  for (i = 0;i < N;i++)
    if (GCElement_getPadDir(g,i) == OUT && GCElement_getPadCanAdd(g,i))
      break;
  
  if (i == N)
    return;
  
  if (es) SetModified(MF_GATE|MF_NET);
  
  gate_draw(g,GD_NORMAL);
  wire_addToGate(g,i,es->env,0);
  Demux_adjustWires(g);
  gate_draw(g,GD_NORMAL);
}

void Demux_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);

  fprintf(f," /ss:%d /do:%d",g->u.mux.select_side,g->u.mux.data_order);

  fprintf(f,"\n");
}

/*****************************************************************************
 * 
 * Generate primitive cell definition for demuxes.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Demux_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int multiPad = gcs->gc_multiPad;
  int selBits = required_bits(gcs->gc_multiPad);
  int inBits = gcs->gc_numBits;
  PrimParm primParm;
  int i; 

  PrimParm_init(&primParm);
  PrimParm_intSet(&primParm,"NUMOUT",multiPad);
  PrimParm_rangeSet(&primParm,"OUT_RANGE",multiPad);
  PrimParm_rangeSet(&primParm,"S_RANGE",selBits);
  PrimParm_intSet(&primParm,"S_BITS",selBits);
  PrimParm_rangeSet(&primParm,"F_RANGE",inBits);
  PrimParm_intSet(&primParm,"F_BITS",inBits);

  if (!*gcs->gc_invSpec)
    PrimParm_invSet(&primParm,"invZ",0);
  else if (!gcs->gc_invSpec[1]) {
    PrimParm_invSet(&primParm,"invZ",(*gcs->gc_invSpec == 'N'));
  } else {
    char *s = PrimParm_get(&primParm,"invZ");
    sprintf(s,"%d'b",multiPad);
    s += strlen(s); 
    for (i = 0;i < multiPad;i++) {
      *s++ = (gcs->gc_invSpec[i] == 'N') ? '1' : '0';
    }
    sprintf(s," ^");
  }

  Primitive_write(f,"demux",gcs,&primParm);
}


void init_demux()
{
  Pixmap P;

  P = Pixmap_registerFromFile("demux","demux.b");
  gateinfo_iconInit(&gate_demux_info,P,demux_iconDims,demux_iconBoldOffset);
  RegisterGate(&gate_demux_info);
}
