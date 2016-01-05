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

    Last edit by hansen on Fri May  1 20:41:14 2009
****************************************************************************/
#include "tkgate.h"

#define DECODER_IN 0
#define DECODER_EN 1
#define DECODER_OUT 2

static void Decoder_Delete(GCElement *g,GModuleDef *env,int drawp);
static void Decoder_Draw(GCElement *g,int md);
static GCElement *Decoder_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char**,int);
GCElement *MuxDemux_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
static void Decoder_PSWrite(GPrint *P,GModLayout*,GCElement *g);
static void Decoder_Rotate(GCElement *g, int centX, int centY,int rdir);
static void Decoder_SetProp(GCElement*,const char*,const void*);
int MuxDemux_EditProps(GCElement *g,int isLoadDialog);
static void Decoder_VersionDelta(GCElement*,Version*); 
static GWireNode *Decoder_wireSnap(GCElement *g,GWire *w,int *mod,int retry);
static void Decoder_AddOutput(EditState *es,GCElement *g);
void MuxDemux_VerSave(FILE*,GCElement*);
static void Decoder_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions decoder_iconDims[] = {
  {29, 60, 59, 29, 30, 12},
  {0,  29, 29, 59, 12, 30},
  {0,  0,  59, 29, 30, 15},
  {60, 0,  29, 59, 15, 30},
};
static int decoder_iconBoldOffset = 89;

GPadLoc decoder_out_loc[] = {
	{-30,16,30,16,D_DOWN},
	{16,-30,16,30,D_RIGHT},
	{-30,-16,30,-16,D_UP},
	{-16,-30,-16,30,D_LEFT},
};

GPadLoc decoder_in_loc[] = {
	{0,-13,0,-13,D_UP},
	{-13,0,-13,0,D_LEFT},
	{0,13,0,13,D_DOWN},
	{13,0,13,0,D_RIGHT},
};

GPadLoc decoder_en_loc[] = {
	{-24,0,-24,0,D_LEFT},
	{0,22,0,22,D_DOWN},
	{22,0,22,0,D_RIGHT},
	{0,-24,0,-24,D_UP},
};

GPadLoc decoder_en_altloc[] = {
	{22,0,22,0,D_RIGHT},
	{0,-24,0,-24,D_UP},
	{-24,0,-24,0,D_LEFT},
	{0,22,0,22,D_DOWN},
};

static char *psDecoder[] = {
  "%",
  "% x y r decoder",
  "%",
  "/decoder {",
  "    startgate",
  "    8 rfont",
  "    (dec) 0 5 rCT",
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

GGateInfo gate_decoder_info = {
  0,
  "DECODER",
  "decoder",0x0,
  "decoder",psDecoder,
  2,2,

 { {"D 2",	{"gm.msi",0},		{"gm.msi.12dec",0,"xdecode",100},	"gat_make DECODER "},
   {"D 3",	{0,0},			{0,0,0,0},				"gat_make DECODER -pins Z=3"},
   {"D 4",	{"gm.msi",0},		{"gm.msi.14dec",0,"xdecode",200},	"gat_make DECODER -pins Z=4"},
   {"D 5",	{0,0},			{0,0,0,0},				"gat_make DECODER -pins Z=5"},
   {"D 6",	{0,0},			{0,0,0,0},				"gat_make DECODER -pins Z=6"},
   {"D 7",	{0,0},			{0,0,0,0},				"gat_make DECODER -pins Z=7"},
   {"D 8",	{"gm.msi",0},		{"gm.msi.18dec",0,"xdecode",300},	"gat_make DECODER -pins Z=8"},
   {"D 9",	{0,0},			{0,0,0,0},				"gat_make DECODER -pins Z=9"},
   {0}},

  decoder_iconDims,

  3,{{"I",IN,1,1,decoder_in_loc,0},
       {"E",IN,1,1,decoder_en_loc,0},
       {"Z",OUT,1,2,decoder_out_loc,1}},
  {{23,0,LJ},{0,40,CT},{-23,8,RJ},{0,-33,CT}},
  {1},
  
  {"Dez","Diz",0},

  Decoder_Make,
  Decoder_WriteCellDef,
  Generic_Init,
  Decoder_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Decoder_Draw,
  Generic_Move,
  MuxDemux_Copy,
  Err_AddInput,
  Decoder_AddOutput,
  Err_AddInOut,
  Decoder_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Decoder_PSWrite,
  MuxDemux_EditProps,
  MuxDemux_VerSave,
  Decoder_SetProp,

  Decoder_VersionDelta,
  Decoder_wireSnap
};

/*
 * Adjust the wires on a mux to conform to the current setting of data_order
 */
static void Decoder_adjustWiresData(GCElement *g)
{
  int N,L,i;
  double dx,dy,wx,wy;
  GWire *w;
  GPadLoc *pd = &g->typeinfo->Pad[DECODER_OUT].Loc[g->orient];


  wx = wy = dx = dy = 0;
  N = wire_numOnPad(g->wires[DECODER_OUT]);
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
  
  for (i = 0,w = g->wires[DECODER_OUT];w;i++, w = w->next) {
    wx += dx;
    wy += dy;
    wire_move(w->nodes,(int)wx-w->nodes->x,(int)wy-w->nodes->y,VERTICAL|HORIZONTAL);
  }
}

/*
 * Adjust the wires on a mux to conform to the current setting of select_side
 */
static void Decoder_adjustWiresSelector(GCElement *g)
{
  int wx,wy;
  GPadLoc *pd = &g->typeinfo->Pad[DECODER_EN].Loc[g->orient];
  GWire *w = g->wires[DECODER_EN];
  GWire *ow = wire_other(w);

  if (g->u.mux.select_side)
    pd = &decoder_en_altloc[g->orient];

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
void Decoder_adjustWires(GCElement *g)
{
  Decoder_adjustWiresData(g);
  Decoder_adjustWiresSelector(g);
}


static GCElement *Decoder_Make(EditState **es,GModuleDef *env,int GType,
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

    Decoder_adjustWires(g);


    for (i = 0,w = g->wires[DECODER_OUT];w;w = w->next,i++);

    nbits = required_bits(i);
    net_setSize(g->wires[DECODER_IN]->net,nbits);
  }

  return g;
}

static void Decoder_Draw_Special(GCElement *g)
{
  GWire *w,*w0;
  char n[STRMAX];
  int i;

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textXF[TkGate.circuit->zoom_factor]->fid);

  if (!(w0 = g->wires[DECODER_OUT])) return;
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


static void Decoder_Delete(GCElement *g,GModuleDef *env,int drawp)
{
  Generic_Delete(g,env,drawp);
}

static void Decoder_Draw(GCElement *g,int md)
{
  Generic_Draw(g,md);
  Decoder_Draw_Special(g);
}


static void Decoder_Rotate(GCElement *g, int centX, int centY,int rdir)
{
  int x = g->xpos;
  int y = g->ypos;

  ob_touch(g);
  g->xpos = rotateX(x - centX,y - centY,rdir) + centX;
  g->ypos = rotateY(x - centX,y - centY,rdir) + centY;
  g->orient = (g->orient + 4 + rdir) % 4;

  Decoder_adjustWires(g);
}


static void Decoder_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
    int N;
    struct wire *w;

    Generic_PSLabels(P,g);

    for (N = 0, w = g->wires[DECODER_OUT];w;w = w->next) N++;

    fprintf(P->p_f,"%d %d %d %d %s\n",
	N,g->xpos,g->ypos,-g->orient*90,g->typeinfo->psprint);
}


static void Decoder_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/ss") == 0) g->u.mux.select_side = *((int*)value);
  if (strcmp(prop,"/do") == 0) g->u.mux.data_order = *((int*)value);

  if (g->wires[DECODER_EN])
    g->wires[DECODER_EN]->orient = (g->orient + 2 + (g->u.mux.select_side ? 2 : 0) ) % 4;
}

static void Decoder_VersionDelta(GCElement *g,Version *V)
{
  Version x1 = {"1.8.7", 1, 8, 7};

  if (VersionCmp(V,&x1) >= 0)
    return;

  switch (g->orient) {
  case D_RIGHT :
    g->u.mux.select_side = 0;
    g->u.mux.data_order = 0;
    g->wires[DECODER_EN]->orient = D_LEFT;
    break;
  case D_UP :
    g->u.mux.select_side = 1;
    g->u.mux.data_order = 1;
    g->wires[DECODER_EN]->orient = D_UP;
    break;
  case D_LEFT :
    g->u.mux.select_side = 0;
    g->u.mux.data_order = 1;
    g->wires[DECODER_EN]->orient = D_RIGHT;
    break;
  case D_DOWN :
    g->u.mux.select_side = 1;
    g->u.mux.data_order = 0;
    g->wires[DECODER_EN]->orient = D_DOWN;
    break;
  }
}

static GWireNode *Decoder_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;

  if (posongate(w,w->gate,&p,&n) == 0) {
    GPadLoc *pd = &g->typeinfo->Pad[p].Loc[g->orient];
    if (p == DECODER_EN && g->u.mux.select_side)
      pd = &decoder_en_altloc[g->orient];
  
    *mod = wire_force(w,pd->dir,retry);
  }
  return w->nodes;
}

static void Decoder_AddOutput(EditState *es,GCElement *g)
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
  Decoder_adjustWires(g);
  gate_draw(g,GD_NORMAL);
}

/*****************************************************************************
 * 
 * Generate primitive cell definition for decoderes.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Decoder_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int multiPad = gcs->gc_multiPad;
  int numBits = required_bits(gcs->gc_multiPad);
  PrimParm primParm;
  int i; 

  PrimParm_init(&primParm);
  PrimParm_intSet(&primParm,"NUMOUT",multiPad);
  PrimParm_rangeSet(&primParm,"OUT_RANGE",multiPad);
  PrimParm_rangeSet(&primParm,"I_RANGE",numBits);
  PrimParm_intSet(&primParm,"I_BITS",numBits);

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

  Primitive_write(f,"decoder",gcs,&primParm);
}


void init_decoder()
{
  Pixmap P;

  P = Pixmap_registerFromFile("decoder","decoder.b");
  gateinfo_iconInit(&gate_decoder_info,P,decoder_iconDims,decoder_iconBoldOffset);
  RegisterGate(&gate_decoder_info);
}
