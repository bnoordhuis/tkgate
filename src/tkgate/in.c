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

    Last edit by hansen on Mon Jan 19 18:16:01 2009
****************************************************************************/
#include "tkgate.h"

#define LOGICIN_OUT 0

iconDimensions in_iconDims[] = {
  {0, 0, 7, 7, 3, 3},
  {8, 0, 7, 7, 3, 3},
  {16, 0, 7, 7, 3, 3},
  {14, 8, 7, 7, 3, 3},
};
int in_iconBoldOffset = 16;

GPadLoc in_out_loc[] = {
	{2,0,2,0,D_RIGHT},
	{0,-2,0,-2,D_UP},
	{-2,0,-2,0,D_LEFT},
	{0,2,0,2,D_DOWN}};

static char *psIn[] = {
  "%",
  "% An input pin",
  "%",
  "/psin {",
  "  startgate",
  "  -6 -3 moveto",
  "  0 0 lineto",
  "  -6 3 lineto",
  "  stroke",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_in_info = {
  GC_LOGICIN,
  "IN",
  "input",0x0,
  "psin", psIn,
  -1,-1,

  {{"}",	{"gm.mod",2},		{"gm.mod.in",7,0,200},	"gat_make IN"},
   {0}},

  in_iconDims,

  1,{{"Z",OUT,1,1,in_out_loc}},
  {{-8,4,RJ},{0,16,CT},{8,4,LJ},{0,-7,CT}},
  {1,1,1},
  
  {0},

  InOut_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  InOut_Draw,
  Generic_Move,
  Generic_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  InOut_PSWrite,
  Generic_EditProps,
  InOut_VerSave
};

GCElement *InOut_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char **options,int nOptions)
{
  GCElement     *g;
  GGateInfo *gi;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWire,options,nOptions)))
    return NULL;

  gi = g->typeinfo;

  if (es) {
    Tcl_SetVar(TkGate.tcl,"edgat_newPort","1",TCL_GLOBAL_ONLY);
    net_editProps(g->wires[0]->net,x,y);
    Tcl_SetVar(TkGate.tcl,"edgat_newPort","0",TCL_GLOBAL_ONLY);
  }
  return g;
}

void InOut_Draw(GCElement *g,int md)
{
  GWire *w;

  mk_gate(g->xpos,g->ypos,g->typeinfo,g->orient,g->selected);

  gate_drawWires(g,md);

  if ((w = g->wires[0]))
    gate_drawgatename(g,w->net->n_signame);
}

void InOut_VerSave(FILE *f,GCElement *g)
{
  fprintf(f,"  //: %s %s (%s)"
	  ,g->typeinfo->name
	  ,g->ename
	  ,g->wires[0]->net->n_signame);
  VerilogBasicGateComment(f,g,0);
  fprintf(f,"\n");
}

void InOut_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  Generic_DrawGateLabel(P,g,g->wires[0]->net->n_signame);

  fprintf(P->p_f,"%d %d %d %s\n",
	  g->xpos,g->ypos,-g->orient*90,
	  g->typeinfo->psprint);
}

void init_in()
{
  Pixmap P;

  P = Pixmap_registerFromFile("inout","inout.b");
  gateinfo_iconInit(&gate_in_info,P,in_iconDims,in_iconBoldOffset);

  RegisterGate(&gate_in_info);
}
