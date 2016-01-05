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

    Last edit by hansen on Tue Feb  3 15:33:02 2009
****************************************************************************/
#include "tkgate.h"

static void FF_WriteCellDef(FILE *f,GCellSpec *gcs);
static GCElement *FF_Make(EditState **es,GModuleDef *env,int GType,
		   int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions);
static void FF_Draw(GCElement *g,int md);
static int FF_EditProps(GCElement *g,int isLoadDialog);
static void FF_VerSave(FILE *f,GCElement *g);
static GCElement *FF_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned);
static void FF_SetProp(GCElement *g,const char *prop,const void *value);
static void FF_PSWrite(GPrint *gp,GModLayout*,GCElement *g);
static GWireNode *FF_wireSnap(GCElement *g,GWire *w,int *mod,int retry);
static void FF_Rotate(GCElement *g, int centX, int centY,int rdir);

static iconDimensions ff_iconDims[] = {
  {0,  0, 31, 31, 15, 15},
  {32, 0, 31, 31, 15, 15},
  {32, 32, 31, 31, 15, 15},
  {0,  32, 31, 31, 15, 15},
};
static int ff_iconBoldOffset = 64;

GPadLoc ff_in_loc[] = {
	{-16,0,-16,0,D_LEFT},
	{0,16,0,16,D_DOWN},
	{16,0,16,0,D_RIGHT},
	{0,-16,0,-16,D_UP},
};

GPadLoc ff_out_loc[] = {
	{16,-5,16,-5,D_RIGHT},
	{-5,-16,-5,-16,D_UP},
	{-16,5,-16,5,D_LEFT},
	{5,16,5,16,D_DOWN},
};

GPadLoc ff_nout_loc[] = {
	{16,5,16,5,D_RIGHT},
	{5,-16,5,-16,D_UP},
	{-16,-5,-16,-5,D_LEFT},
	{-5,16,-5,16,D_DOWN},
};

GPadLoc ff_ck_loc[] = {
	{0,16,0,16,D_DOWN},
	{16,0,16,0,D_RIGHT},
	{0,-16,0,-16,D_UP},
	{-16,0,-16,0,D_LEFT},
};

GPadLoc ff_en_loc[] = {
	{5,-16,5,-16,D_UP},
	{-16,-5,-16,-5,D_LEFT},
	{-5,16,-5,16,D_DOWN},
	{16,5,16,5,D_RIGHT},
};

GPadLoc ff_clr_loc[] = {
	{-5,-16,-5,-16,D_UP},
	{-16,5,-16,5,D_LEFT},
	{5,16,5,16,D_DOWN},
	{16,-5,16,-5,D_RIGHT},
};

/***************************************************************************************/
/* Reverse locations */
/***************************************************************************************/
struct locate rff_in_loc[] = {
	{16,0,16,0,D_RIGHT},
	{0,-16,0,-16,D_UP},
	{-16,0,-16,0,D_LEFT},
	{0,16,0,16,D_DOWN},
};

struct locate rff_out_loc[] = {
	{-16,-5,-16,-5,D_LEFT},
	{-5,16,-5,16,D_DOWN},
	{16,5,16,5,D_RIGHT},
	{5,-16,5,-16,D_UP},
};

struct locate rff_nout_loc[] = {
	{-16,5,-16,5,D_LEFT},
	{5,16,5,16,D_DOWN},
	{16,-5,16,-5,D_RIGHT},
	{-5,-16,-5,-16,D_UP},
};

static char *psFlipFlop[] = {
  "%",
  "% A flip-flop",
  "/psflipflop {",
  "  [[0 0][0 0][0 0][0 0]] adjstartgate",
  "  -15.5 -15.5 moveto",
  "  -15.5 15.5 lineto",
  "  15.5 15.5 lineto",
  "  15.5 -15.5 lineto",
  "  closepath stroke",
  "  -5 -15.5 moveto",
  "  0 -10.5 lineto",
  "  5 -15.5 lineto",
  "  stroke",
  "  7 rfont",
  "  (_C) -8 6 prshow",
  "  (_E) 2 6 prshow",
  "  {",
  "    (Q) -14 2 prshow",
  "    (_Q) -14 -8 prshow",
  "    (D) 8 -3 prshow",
  "  } {",
  "    (Q) 8 2 prshow",
  "    (_Q) 8 -8 prshow",
  "    (D) -14 -3 prshow",
  "  } ifelse",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_ff_info = {
  GC_FLIPFLOP,
  "FF",
  "ff",0x0,
  "psflipflop",psFlipFlop,
  -1,0,

  {{"f",	{"gm.mem",5},		{"gm.mem.ff",0,0,100},	"gat_make FF"},
   {"q",	{"gm.mem",5},		{"gm.mem.rff",0,0,100},	"gat_make FF -reverse 1"},

   {0}},


  ff_iconDims,

  6,{{"Q",OUT,1,1,ff_out_loc},
     {"_Q",OUT,1,1,ff_nout_loc},
     {"D",IN,1,1,ff_in_loc},
     {"EN",IN,1,1,ff_en_loc},
     {"CLR",IN,1,1,ff_clr_loc},
     {"CK",IN,1,1,ff_ck_loc}},
  {{-17,15,RJ},{17,15,LJ},{17,15,LJ},{-17,15,RJ}},
  {1},

  {"Dsetup","Dhold","Dck_q",0},
  
  FF_Make,
  FF_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  FF_Draw,
  Generic_Move,
  FF_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  FF_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  FF_PSWrite,
  FF_EditProps,
  FF_VerSave,
  FF_SetProp,
  0,
  FF_wireSnap
};

/*****************************************************************************
 *
 * Make the GGateInfo structure reflect a normal flip-flop
 *
 *****************************************************************************/
static void FF_normal_gateinfo()
{
  gate_ff_info.Pad[0].Loc = ff_out_loc;
  gate_ff_info.Pad[1].Loc = ff_nout_loc;
  gate_ff_info.Pad[2].Loc = ff_in_loc;
}

/*****************************************************************************
 *
 * Make the GGateInfo structure reflect a mirror flip-flop
 *
 *****************************************************************************/
static void FF_reverse_gateinfo()
{
  gate_ff_info.Pad[0].Loc = rff_out_loc;
  gate_ff_info.Pad[1].Loc = rff_nout_loc;
  gate_ff_info.Pad[2].Loc = rff_in_loc;
}

static GCElement *FF_Make(EditState **es,GModuleDef *env,int GType,
			int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g;
  const char *revOpt;
  int isMirror = 0;

  revOpt = seekOption("-reverse",options,nOptions);
  if (revOpt && *revOpt == '1')
    isMirror = 1;

  if (isMirror)
    FF_reverse_gateinfo();

  g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions);
  ob_touch(g);
  g->u.ff.mirror = isMirror;

  if (isMirror)
    FF_normal_gateinfo();

  return g;
}

static void FF_Draw(GCElement *g,int md)
{
  int x = g->xpos;
  int y = g->ypos;
  GGateInfo *gi = g->typeinfo;
  int idx = g->orient;
  int selected = g->selected;

  if (selected) idx += 4;

  if (g->u.ff.mirror)
    Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x),ctow_y(y),gi->altIcon[idx]);
  else
    Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x),ctow_y(y),gi->icon[idx]);

  gate_drawWires(g,md);

  if (g->ename && g->show_name)
    gate_drawgatename(g,g->ename);
}

static int FF_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_ffmirror %d",g->u.ff.mirror);
  } else {
    const char *p;
    int mirror = g->u.ff.mirror;

    if ((p = Tcl_GetVar(tcl,"edgat_ffmirror",TCL_GLOBAL_ONLY)))
      sscanf(p,"%d",&mirror);

    if (g->u.ff.mirror != mirror) {
      int p;

      g->u.ff.mirror = mirror;

      if (mirror)
	FF_reverse_gateinfo();

      for (p = 0;p < 3;p++) {
	GPadLoc *pd = &g->typeinfo->Pad[p].Loc[g->orient];

	wire_rePort(g->wires[p],g->xpos+pd->x2,g->ypos+pd->y2,pd->dir);
      }

      if (mirror)
	FF_normal_gateinfo();
    }

    ob_touch(g);
  }

  return 0;
}

static void FF_Rotate(GCElement *g, int centX, int centY,int rdir)
{
  if (g->u.ff.mirror)
    FF_reverse_gateinfo();

  Generic_Rotate(g,centX,centY,rdir);

  if (g->u.ff.mirror)
    FF_normal_gateinfo();
}


static void FF_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);
  fprintf(f," /mi:%d",g->u.ff.mirror);
  fprintf(f,"\n");
}

static GCElement *FF_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  if (g->u.ff.mirror)
    FF_reverse_gateinfo();

  ng = Generic_Copy(M,g,x,y,flags);
  ob_touch(ng);
  ng->u.ff.mirror = g->u.ff.mirror;

  if (g->u.ff.mirror)
    FF_normal_gateinfo();

  return ng;
}

static void FF_SetProp(GCElement *g,const char *prop,const void *value)
{
  int i;

  if (strcmp(prop,"/mi") == 0) g->u.ff.mirror = *((int*)value);

  if (g->u.ff.mirror) {
    for (i = 0;i < 3;i++) {
      if (g->wires[i])
	g->wires[i]->orient = (g->wires[i]->orient + 2) % 4;
    }
  }
}

static void FF_PSWrite(GPrint *gp,GModLayout *mli,GCElement *g)
{
  Generic_PSLabels(gp,g);

  fprintf(gp->p_f,"%s %d %d %d %s\n",
	  g->u.ff.mirror ? "true" : "false",
	  g->xpos,g->ypos,-g->orient*90,
	  g->typeinfo->psprint);
}

static GWireNode *FF_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;

  if (posongate(w,w->gate,&p,&n) == 0) {
    GPadLoc *pd;

    if (g->u.ff.mirror)
      FF_reverse_gateinfo();

    pd = &g->typeinfo->Pad[p].Loc[g->orient];

    if (g->u.ff.mirror)
      FF_normal_gateinfo();

  
    *mod = wire_force(w,pd->dir,retry);
  }
  return w->nodes;
}

/*****************************************************************************
 *
 * Generate primitive cell definition for flip-flops.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void FF_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  int invQ = 0, invNQ = 1;
  PrimParm primParm;

  if (*invSpec) {
    if (*invSpec == 'N')
      invQ = 1;
    if (!invSpec[1] || invSpec[1] == 'N')
      invNQ = 0;
  }

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"DQ_RANGE",numBit);
  PrimParm_intSet(&primParm,"DQ_BITS",numBit);
  PrimParm_invSet(&primParm,"invQ",invQ);
  PrimParm_invSet(&primParm,"invNQ",invNQ);
  Primitive_write(f,"flipflop",gcs,&primParm);
}

void init_ff()
{
  Pixmap P;

  P = Pixmap_registerFromFile("flipflop","flipflop.b");
  gateinfo_iconInit(&gate_ff_info,P,ff_iconDims,ff_iconBoldOffset);

  P = Pixmap_registerFromFile("rflipflop","rflipflop.b");
  gateinfo_altIconInit(&gate_ff_info,P,ff_iconDims,ff_iconBoldOffset);

  RegisterGate(&gate_ff_info);
}
