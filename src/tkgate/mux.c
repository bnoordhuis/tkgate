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

    Last edit by hansen on Fri May  1 20:41:18 2009
****************************************************************************/
#include "tkgate.h"

#define MUX_IN 0
#define MUX_SEL 1
#define MUX_OUT 2

static void Mux_WriteCellDef(FILE *f,GCellSpec *gcs);


/*
 * Some of these are shared by the Demux element.  These have a MuxDemux_ prefix. 
 */
void Mux_Delete(GCElement *g,GModuleDef *env,int drawp);
void Mux_Draw(GCElement *g,int md);
void Mux_Rotate(GCElement *g, int centX, int centY,int rdir);
GCElement *Mux_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char**,int);
void Mux_PSWrite(GPrint *P,GModLayout*,GCElement *g);
void MuxDemux_VerSave(FILE*,GCElement*);
void Mux_SetProp(GCElement*,const char*,const void*);
void Mux_AddInput(EditState *es,GCElement *g);
int MuxDemux_EditProps(GCElement *g,int isLoadDialog);
GCElement *MuxDemux_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
void Mux_VersionDelta(GCElement*,Version*); 
GWireNode *Mux_wireSnap(GCElement *g,GWire *w,int *mod,int retry);

void Demux_adjustWires(GCElement *g);
void Decoder_adjustWires(GCElement *g);

static iconDimensions mux_iconDims[] = {
  {0,  0,  59, 29, 30, 15},
  {60, 0,  29, 59, 15, 30},
  {29, 60, 59, 29, 30, 12},
  {0,  29, 29, 59, 12, 30},
};
static int mux_iconBoldOffset = 89;

GPadLoc mux_in_loc[] = {
	{-30,-16,30,-16,D_UP},
	{-16,-30,-16,30,D_LEFT},
	{-30,16,30,16,D_DOWN},
	{16,-30,16,30,D_RIGHT}};

GPadLoc mux_out_loc[] = {
	{0,13,0,13,D_DOWN},
	{13,0,13,0,D_RIGHT},
	{0,-13,0,-13,D_UP},
	{-13,0,-13,0,D_LEFT}};

GPadLoc mux_sel_loc[] = {
	{-23,0,-23,0,D_LEFT},
	{0,23,0,23,D_DOWN},
	{23,0,23,0,D_RIGHT},
	{0,-23,0,-23,D_UP},
};

GPadLoc mux_sel_altloc[] = {
	{23,0,23,0,D_RIGHT},
	{0,-23,0,-23,D_UP},
	{-23,0,-23,0,D_LEFT},
	{0,23,0,23,D_DOWN},
};

static char *psMux[] = {
  "%",
  "% x y r mux",
  "%",
  "/mux {",
  "    dup /mrot exch def",
  "    startgate",
  "    8 rfont",
  "    -29.5 15.5 moveto",
  "    29.5 15.5 lineto",
  "    16.5 -12.5 lineto",
  "    -16.5 -12.5 lineto",
  "    closepath stroke",
  "    dup					% n n",
  "    1 add 58 exch div			% n d1",
  "    2 copy mul				% n d1 dn",
  "    mrot -90 eq mrot -180 eq or {",
  "      3 -1 roll 1 sub 50 string cvs exch (0) exch	% d1 (n) (0) dn",
  "      -29 add 7 rCT				% d1",
  "      exch -29 add 7 rCT",
  "    } {",
  "      3 -1 roll 1 sub 50 string cvs exch	% d1 (n) dn",
  "      -29 add 7 rCT				% d1",
  "      (0) exch -29 add 7 rCT",
  "    } ifelse",
  "    grestore",
  "} def",
  0
};

GGateInfo gate_mux_info = {
  0,
  "MUX",
  "mux",0x0,
  "mux",psMux,
  0,2,

 { {"m",	{"gm.msi",0},		{"gm.msi.21mux",0,"mux",100},		"gat_make MUX "},
   {"M 2",	{0,0},			{0,0,0,0},				"gat_make MUX -pins I=2"},
   {"M 3",	{0,0},			{0,0,0,0},				"gat_make MUX -pins I=3"},
   {"M 4",	{"gm.msi",0},		{"gm.msi.41mux",0,"mux",200},		"gat_make MUX -pins I=4"},
   {"M 5",	{0,0},			{0,0,0,0},				"gat_make MUX -pins I=5"},
   {"M 6",	{0,0},			{0,0,0,0},				"gat_make MUX -pins I=6"},
   {"M 7",	{0,0},			{0,0,0,0},				"gat_make MUX -pins I=7"},
   {"M 8",	{"gm.msi",0},		{"gm.msi.81mux",0,"mux",300},		"gat_make MUX -pins I=8"},
   {"M 9",	{0,0},			{0,0,0,0},				"gat_make MUX -pins I=9"},
   {0}},

  mux_iconDims,

  3,{{"I",IN,1,2,mux_in_loc,1},
       {"S",IN,1,1,mux_sel_loc,0},
       {"Z",OUT,1,1,mux_out_loc,0}},
  {{23,8,LJ},{0,40,CT},{-23,0,RJ},{0,-33,CT}},
  {1},
  
  {"Dsz","Diz",0},

  Mux_Make,
  Mux_WriteCellDef,
  Generic_Init,
  Mux_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Mux_Draw,
  Generic_Move,
  MuxDemux_Copy,
  Mux_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Mux_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Mux_PSWrite,
  MuxDemux_EditProps,
  MuxDemux_VerSave,
  Mux_SetProp,
  Mux_VersionDelta,
  Mux_wireSnap
};


/*
 * Adjust the wires on a mux to conform to the current setting of data_order
 */
static void Mux_adjustWiresData(GCElement *g)
{
  int N,L,i;
  double dx,dy,wx,wy;
  GWire *w;
  GPadLoc *pd = &g->typeinfo->Pad[MUX_IN].Loc[g->orient];


  wx = wy = dx = dy = 0;
  N = wire_numOnPad(g->wires[MUX_IN]);
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
  
  for (i = 0,w = g->wires[MUX_IN];w;i++, w = w->next) {
    wx += dx;
    wy += dy;
    wire_move(w->nodes,(int)wx-w->nodes->x,(int)wy-w->nodes->y,VERTICAL|HORIZONTAL);
  }
}

/*
 * Adjust the wires on a mux to conform to the current setting of select_side
 */
static void Mux_adjustWiresSelector(GCElement *g)
{
  int wx,wy;
  GPadLoc *pd = &g->typeinfo->Pad[MUX_SEL].Loc[g->orient];
  GWire *w = g->wires[MUX_SEL];
  GWire *ow = wire_other(w);

  if (g->u.mux.select_side)
    pd = &mux_sel_altloc[g->orient];

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
static void Mux_adjustWires(GCElement *g)
{
  Mux_adjustWiresData(g);
  Mux_adjustWiresSelector(g);
}

GCElement *Mux_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  int nbits;
  struct celemnt *g;
  struct wire *w;

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
	if (sscanf(Pins,"%[^=]=%d",buf,&N) == 2 && strcmp(buf,g->typeinfo->Pad[i].Name) == 0)
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

    Mux_adjustWires(g);

    for (i = 0,w = g->wires[MUX_IN];w;w = w->next,i++);

    nbits = required_bits(i);
    net_setSize(g->wires[MUX_SEL]->net,nbits);
  }

  return g;
}

GCElement *MuxDemux_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng = Generic_Copy(M,g,x,y,flags);

  ob_touch(ng);
  ng->u.mux.select_side = g->u.mux.select_side;
  ng->u.mux.data_order = g->u.mux.data_order;

  return ng;
}


void Mux_AddInput(EditState *es,GCElement *g)
{
  int i;
  int N = GCElement_numPads(g);

  for (i = 0;i < N;i++)
    if (GCElement_getPadDir(g,i) == IN && g->typeinfo->Pad[i].CanAdd)
      break;
  
  if (i == N)
    return;
  
  if (es) SetModified(MF_NET|MF_GATE);
  
  gate_draw(g,GD_NORMAL);
  wire_addToGate(g,i,es->env,0);
  Mux_adjustWires(g);
  gate_draw(g,GD_NORMAL);
}


void Mux_Draw_Special(GCElement *g)
{
  GWire *w,*w0;
  char n[STRMAX];
  int i;

  if (!(w0 = g->wires[MUX_IN])) return;
  for (i = 0,w = w0;w->next;w = w->next,i++);
  sprintf(n,"%d",i);

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);

  switch (g->orient) {
  case 0 :
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x,w0->nodes->y + 2,
		   AtTop|BetweenLeftAndRight,"0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x,w->nodes->y + 2,
		   AtTop|BetweenLeftAndRight,n);
    break;
  case 1 :
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x + 3,w0->nodes->y,
		   BetweenTopAndBottom|AtLeft, "0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x + 3,w->nodes->y,
		   BetweenTopAndBottom|AtLeft,n);
    break;
  case 2 :
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x,w0->nodes->y - 2,
		   AtBottom|BetweenLeftAndRight,"0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x,w->nodes->y - 2,
		   AtBottom|BetweenLeftAndRight,n);
    break;
  case 3:
    dce_DrawString(TkGate.instGC,
		   w0->nodes->x - 2,w0->nodes->y,
		   BetweenTopAndBottom|AtRight,"0");
    dce_DrawString(TkGate.instGC,
		   w->nodes->x - 2,w->nodes->y,
		   BetweenTopAndBottom|AtRight,n);
    break;
  }
}

void Mux_Delete(GCElement *g,GModuleDef *env,int drawp)
{
  Generic_Delete(g,env,drawp);
}

void Mux_Draw(GCElement *g,int md)
{
  Generic_Draw(g,md);
  Mux_Draw_Special(g);
}

void Mux_Rotate(GCElement *g, int centX, int centY,int rdir)
{
  int x = g->xpos;
  int y = g->ypos;

  ob_touch(g);
  g->xpos = rotateX(x - centX,y - centY,rdir) + centX;
  g->ypos = rotateY(x - centX,y - centY,rdir) + centY;
  g->orient = (g->orient + 4 + rdir) % 4;

  Mux_adjustWires(g);
}

void Mux_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
    int NIn;
    struct wire *w;

    Generic_PSLabels(P,g);

    for (NIn = 0, w = g->wires[MUX_IN];w;w = w->next) NIn++;

    fprintf(P->p_f,"%d %d %d %d %s\n",
	NIn,g->xpos,g->ypos,-g->orient*90,g->typeinfo->psprint);
}

void MuxDemux_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);

  fprintf(f," /ss:%d /do:%d",g->u.mux.select_side,g->u.mux.data_order);

  fprintf(f,"\n");
}

int MuxDemux_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_dataOrder %d",g->u.mux.data_order);
    DoTcl("set ::edgat_selectSide %d",g->u.mux.select_side);
  } else {
    const char *p;
    int new_data_order = g->u.mux.data_order;
    int new_select_side = g->u.mux.select_side;

    if ((p = Tcl_GetVar(tcl,"edgat_dataOrder",TCL_GLOBAL_ONLY))) {
      sscanf(p,"%d",&new_data_order);
    }
    if ((p = Tcl_GetVar(tcl,"edgat_selectSide",TCL_GLOBAL_ONLY))) {
      sscanf(p,"%d",&new_select_side);
    }

    if (g->u.mux.data_order != new_data_order || g->u.mux.select_side != new_select_side) {
      ob_touch(g);
      gate_draw(g,0);
      g->u.mux.data_order = new_data_order;
      g->u.mux.select_side = new_select_side;

      if (strcmp(g->typeinfo->name,"MUX") == 0)
	Mux_adjustWires(g);
      else if (strcmp(g->typeinfo->name,"DEMUX") == 0)
	Demux_adjustWires(g);
      else if (strcmp(g->typeinfo->name,"DECODER") == 0)
	Decoder_adjustWires(g);


      gate_draw(g,0);
    }
  }

  return 0;
}

void Mux_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/ss") == 0) g->u.mux.select_side = *((int*)value);
  if (strcmp(prop,"/do") == 0) g->u.mux.data_order = *((int*)value);

  if (g->wires[MUX_SEL])
    g->wires[MUX_SEL]->orient = (g->orient + 2 + (g->u.mux.select_side ? 2 : 0) ) % 4;
}

/*
 * This function is called when reading a save file from a previous version of tkgate.
 * Default pin placement of muxes changed in version 2.0.  This function sets flags on
 * muxes to make it compatable with the new vesion, but only if the save file is older
 * than 2.0.
 */
void Mux_VersionDelta(GCElement *g,Version *V)
{
  Version x1 = {"1.8.7", 1, 8, 7};

  if (VersionCmp(V,&x1) >= 0)
    return;

  switch (g->orient) {
  case D_RIGHT :
    g->u.mux.select_side = 0;
    g->u.mux.data_order = 0;
    g->wires[MUX_SEL]->orient = D_LEFT;
    break;
  case D_UP :
    g->u.mux.select_side = 1;
    g->u.mux.data_order = 1;
    g->wires[MUX_SEL]->orient = D_DOWN;
    break;
  case D_LEFT :
    g->u.mux.select_side = 0;
    g->u.mux.data_order = 1;
    g->wires[MUX_SEL]->orient = D_RIGHT;
    break;
  case D_DOWN :
    g->u.mux.select_side = 1;
    g->u.mux.data_order = 0;
    g->wires[MUX_SEL]->orient = D_UP;
    break;
  }
}

GWireNode *Mux_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;

  if (posongate(w,w->gate,&p,&n) == 0) {
    GPadLoc *pd = &g->typeinfo->Pad[p].Loc[g->orient];
    if (p == MUX_SEL && g->u.mux.select_side)
      pd = &mux_sel_altloc[g->orient];
  
    *mod = wire_force(w,pd->dir,retry);
  }
  return w->nodes;
}

/*****************************************************************************
 *
 * Generate primitive cell definition for muxes
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Mux_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int multiPad = gcs->gc_multiPad;
  int numBits = gcs->gc_numBits;
  int selNumBits = required_bits(gcs->gc_multiPad);
  int inv = (*gcs->gc_invSpec == 'N');
  PrimParm primParm;

  PrimParm_init(&primParm);
  PrimParm_intSet(&primParm,"NUMIN",multiPad);
  PrimParm_rangeSet(&primParm,"IZ_RANGE",numBits);
  PrimParm_intSet(&primParm,"IZ_BITS",numBits);
  PrimParm_rangeSet(&primParm,"S_RANGE",selNumBits);
  PrimParm_intSet(&primParm,"S_BITS",selNumBits);
  PrimParm_invSet(&primParm,"invZ",inv);
  Primitive_write(f,"mux",gcs,&primParm);
}

void init_mux()
{
  Pixmap P;

  P = Pixmap_registerFromFile("mux","mux.b");
  gateinfo_iconInit(&gate_mux_info,P,mux_iconDims,mux_iconBoldOffset);
  RegisterGate(&gate_mux_info);
}
