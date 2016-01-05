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
****************************************************************************/
#include "tkgate.h"

#define CENTER_JUST	(BetweenTopAndBottom|BetweenLeftAndRight)

#define DIP_OUT 0

extern int SimulatorActive;

GCElement *Dip_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char**,int);

void Dip_Draw(GCElement *g,int md);
int Dip_EditProps(GCElement *g,int isLoadDialog);
void Dip_PSWrite(GPrint *P,GModLayout*,GCElement *g);
int Dip_SimHitFunc(EditState *ss,GCElement *g);
void Dip_SimInitFunc(EditState *es,GCElement *g,const char *path);
GCElement *SwitchDip_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned);
void SwitchDip_VerSave(FILE *f,GCElement *g);
void SwitchDip_SetProp(FILE *f,GCElement *g);

static iconDimensions dip_iconDims[] = {
  {0, 0, 75, 20, 37, 10},
};
static int dip_iconBoldOffset = 21;


GPadLoc dip_out_loc[] = {
  {0,10,0,10,D_DOWN},
  {38,0,38,0,D_RIGHT},
  {0,-11,0,-11,D_UP},
  {-38,0,-38,0,D_LEFT},
};

static char *psDip[] = {
  "%",
  "% (v) x y r dip",
  "%",
  "/psdip {",
  "  dup 4 1 roll",
  "  startnorotgate",
  "  3.5 0.5 translate",
  "  -42 -10 moveto",
  "  34 -10 lineto",
  "  34 10 lineto",
  "  -42 10 lineto",
  "  closepath stroke",
  "  8 rfont",
  "  1 -1 scale",
  "  -180 eq { 38 5 LJ } { -4 -12 CT } ifelse",
  "  1 -1 scale",
  "  5 rfont",
  "  -38 3 moveto (ON) show",
  "  -38 -6 moveto (OFF) show",
  "  -24 7 25 {-5 ds} for",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_dip_info = {
  GC_DIP,
  "DIP",
  "dip",0x0,
  "psdip",psDip,
  -1,-1,

  {{"d",	{"gm.io",0},	{"gm.io.dip",0,0,200},	"gat_make DIP"},
   {0}},

  dip_iconDims,

  1,{{"Z",OUT,8,1,dip_out_loc}},
  {{-42,0,RJ},{-35,23,LJ},{-35,23,LJ},{-35,23,LJ}},
  {1,0,1,0,0,1},

  {0},

  Dip_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Dip_Draw,
  Generic_Move,
  SwitchDip_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Dip_SimInitFunc,
  Dip_SimHitFunc,
  Dip_PSWrite,
  Dip_EditProps,
  SwitchDip_VerSave,
  SwitchDip_SetProp
};

GCElement *Dip_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g;
  const char *DefV;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions)))
    return NULL;

  DefV = seekOption("-value",options,nOptions);
  if (!DefV || sscanf(DefV,"%d",&g->u.sw.dipval) != 1)
    g->u.sw.dipval = 0;

  g->u.sw.perm_dipval = g->u.sw.dipval;

  g->u.sw.showNet = TkGate.circuit->showSwitchNets;

  return g;
}

void Dip_Draw(GCElement *g,int md)
{
  char s[STRMAX];
  char compositeName[STRMAX];
  int vox,voy;
  
  mk_gate(g->xpos,g->ypos,g->typeinfo,g->orient,g->selected);
  gate_drawWires(g,md);

  SwitchDip_getCompositeName(g, compositeName);
  if (*compositeName)
    gate_drawgatename(g,compositeName);

  if (g->wires[DIP_OUT]) {
    unsigned mask = nmask(g->wires[DIP_OUT]->net->n_nbits);
    unsigned dipValue;

    if (tkgate_currentMode() == MM_SIMULATE)
      dipValue = g->u.sw.dipval;
    else
      dipValue = g->u.sw.perm_dipval;

    sprintf(s,"%x",dipValue & mask);
  
    vox = (g->orient == 2) ?  43 : 0;
    voy = (g->orient == 2) ?  5 : -15;

    if (g->selected)
      XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);  
    else
      XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);  
    dce_DrawString(TkGate.instGC,g->xpos+vox,g->ypos+voy,
		   CENTER_JUST,s);
  }
}

void Dip_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  Generic_PSLabels(P,g);

  fprintf(P->p_f,"(%x) %d %d %d %s\n",
	  g->u.sw.perm_dipval & nmask(g->wires[DIP_OUT]->net->n_nbits),
	  g->xpos,g->ypos,-g->orient*90,g->typeinfo->psprint);
}

int Dip_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_dip %x",g->u.sw.perm_dipval);
    DoTcl("set ::edgat_shownet %d",g->u.sw.showNet);
  } else {
    const char *p;

    ob_touch(g);

    if ((p = Tcl_GetVar(tcl,"edgat_dip",TCL_GLOBAL_ONLY)))
      if (sscanf(p,"%x",&g->u.sw.perm_dipval) != 1)
	message(1,msgLookup("err.badhex"),p);	/* "Illegal hex value '%s' ignored." */
    g->u.sw.dipval = g->u.sw.perm_dipval;

    if ((p = Tcl_GetVar(tcl,"edgat_shownet",TCL_GLOBAL_ONLY)))
      sscanf(p,"%d",&g->u.sw.showNet);
  }

  return 0;
}

void Dip_SimInitFunc(EditState *es,GCElement *g,const char *path)
{
  g->u.sw.dipval = g->u.sw.perm_dipval;
}

int Dip_SimHitFunc(EditState *es,GCElement *g)
{
  GSimSwitch *ss = SHash_find(es->smod->switches,g->ename);

  DoTcl("Simulator::dipEntry %s %x %d %d",
	ss->gname,g->u.sw.dipval,
	ctow_x(g->xpos),ctow_y(g->ypos));

  return 1;
}

void init_dip()
{
  Pixmap P;

  P = Pixmap_registerFromFile("dip","dip.b");
  gateinfo_1iconInit(&gate_dip_info,P,dip_iconDims,dip_iconBoldOffset);
  RegisterGate(&gate_dip_info);
}
