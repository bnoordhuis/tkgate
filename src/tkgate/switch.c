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

    Last edit by hansen on Sat Feb 21 20:02:58 2009
****************************************************************************/
#include "tkgate.h"

#define SWITCH_OUT 0

static iconDimensions upswitch_iconDims[] = {
  {0, 0, 34, 26, 17, 13},
  {0, 0, 34, 26, 17, 13},
  {0, 0, 34, 26, 17, 13},
  {0, 0, 34, 26, 17, 13},
};

static iconDimensions dnswitch_iconDims[] = {
  {35, 0, 34, 26, 17, 13},
  {35, 0, 34, 26, 17, 13},
  {35, 0, 34, 26, 17, 13},
  {35, 0, 34, 26, 17, 13},
};
static int switch_iconBoldOffset = 27;

GCElement *Switch_Make(EditState **,GModuleDef *,int,int,int,int,const char *,int,const char**,int);
void Switch_Draw(GCElement *g,int md);
int Switch_EditProps(GCElement *g,int isLoadDialog);
int Switch_SimHitFunc(EditState*,GCElement*);
GCElement *SwitchDip_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned);
void Switch_SimInitFunc(EditState *es,GCElement *g,const char *path);
void SwitchDip_VerSave(FILE *f,GCElement *g);
void SwitchDip_SetProp(GCElement*,const char*,const void*);

GPadLoc switch_out_loc[] = {
  {17,0,17,0,D_RIGHT},
  {0,-14,0,-14,D_UP},
  {-18,0,-18,0,D_LEFT},
  {0,13,0,13,D_DOWN}};

static char *psSwitch[] = {
  "%",
  "% A Switch",
  "/switchgate {",
  "  startnorotgate",
  "  % Outer Box",
  "  4 setlinewidth",
  "  -18 -12 moveto",
  "  15 -12 lineto",
  "  15 12 lineto",
  "  1 setlinewidth",
  "  -18 12 lineto",
  "  closepath",
  "  stroke",
  "  gsave      % Inner Box",
  "  -5 -9 moveto",
  "  -5 10 lineto",
  "  -16 10 lineto",
  "  -16 -9 lineto",
  "  closepath",
  "  stroke",
  "  0.8 setgray",
  "  -5 -9 moveto",
  "  -5 10 lineto",
  "  -16 10 lineto",
  "  -16 -9 lineto",
  "  closepath",
  "  fill",
  "  grestore",
  
  "  -8 0 moveto    % Knives",
  "  -8 7 lineto",
  "  -13 7 lineto",
  "  -13 0 lineto",
  "  stroke",
  
  "  -10.5 7  moveto    % Handle",
  "  -10.5 7.5 lineto",
  "  stroke",
  "  -10.5 8 1 0 360 arc",
  "  closepath",
  "  fill",
  
  "  -7 -1 moveto    % Hinges",
  "  -7 1 lineto",
  "  -9 -1 moveto",
  "  -9 1 lineto",
  "  -12 -1 moveto",
  "  -12 1 lineto",
  "  -14 -1 moveto",
  "  -14 1 lineto",
  "  stroke",
  "  -7 5 1 0 360 arc  % Clips",
  "  closepath fill",
  "  newpath",
  "  -9 5 1 0 360 arc",
  "  closepath fill",
  "  newpath",
  "  -12 5 1 0 360 arc",
  "  closepath fill",
  "  newpath",
  "  -14 5 1 0 360 arc",
  "  closepath fill",
  "  newpath",
  "  -7 -5 1 0 360 arc",
  "  closepath fill",
  "  newpath",
  "  -9 -5 1 0 360 arc",
  "  closepath fill",
  "  newpath",
  "  -12 -5 1 0 360 arc",
  "  closepath fill",
  "  newpath",
  "  -14 -5 1 0 360 arc",
  "  closepath fill",
  "  8 rfont",
  "  -3 4 moveto",
  "  (on) show",
  "  -3 -7 moveto",
  "  (off) show",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_switch_info = {
  GC_SWITCH,
  "SWITCH",
  "switch",0x0,
  "switchgate",psSwitch,
  -1,-1,

  {{"s",	{"gm.io",0},		{"gm.io.switch",0,0,100},	"gat_make SWITCH"},
   {0}},

  upswitch_iconDims,

  1,{{"Z",OUT,1,1,switch_out_loc}},
  {{0,-17,CT},{17,0,LJ},{0,-17,CT},{17,0,LJ}},
  {1,1,1,0,0},

  {0},

  Switch_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Switch_Draw,
  Generic_Move,
  SwitchDip_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Switch_SimInitFunc,
  Switch_SimHitFunc,
  Generic_PSWrite,
  Switch_EditProps,
  SwitchDip_VerSave,
  SwitchDip_SetProp,
};

GCElement *Switch_Make(EditState **es,GModuleDef *env,int GType,
			int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions);

  ob_touch(g);
  g->u.sw.perm_dipval = 0;
  g->u.sw.dipval = 0;
  g->u.sw.showNet = TkGate.circuit->showSwitchNets;

  return g;
}

void SwitchDip_getCompositeName(GCElement *g,char *compositeName)
{
  GWire *w = g->wires[0];
  char *netName = w->net->n_signame ? w->net->n_signame : "";

  if (g->u.sw.showNet) {
    if (g->ename && g->show_name)
      sprintf(compositeName,"%s[%s]",g->ename,netName);
    else
      sprintf(compositeName,"[%s]",netName);
  } else {
    if (g->ename && g->show_name)
      sprintf(compositeName,"%s",g->ename);
    else
      *compositeName = 0;
  }
}

void Switch_Draw(GCElement *g,int md)
{
  Icon *C;
  int x,y;
  int idx = 0;
  char compositeName[STRMAX];

  if (g->u.sw.dipval) idx++;
  if (g->selected) idx += 4;

  C = g->typeinfo->icon[idx];
  x = g->xpos;
  y = g->ypos;

  Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x),ctow_y(y),C);


  gate_drawWires(g,md);

  SwitchDip_getCompositeName(g, compositeName);
  if (*compositeName)
    gate_drawgatename(g,compositeName);
}

GCElement *SwitchDip_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng = Generic_Copy(M,g,x,y,flags);

  ob_touch(ng);
  ng->u.sw.perm_dipval = g->u.sw.perm_dipval;
  ng->u.sw.dipval = g->u.sw.dipval;
  ng->u.sw.showNet = g->u.sw.showNet;

  return ng;
}

int Switch_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_switch %d",g->u.sw.perm_dipval);
    DoTcl("set ::edgat_shownet %d",g->u.sw.showNet);
  } else {
    const char *p;

    ob_touch(g);

    if ((p = Tcl_GetVar(tcl,"edgat_switch",TCL_GLOBAL_ONLY)))
      sscanf(p,"%d",&g->u.sw.perm_dipval);
    g->u.sw.dipval = g->u.sw.perm_dipval;

    if ((p = Tcl_GetVar(tcl,"edgat_shownet",TCL_GLOBAL_ONLY)))
      sscanf(p,"%d",&g->u.sw.showNet);
  }
  return 0;
}

void SwitchDip_VerSave(FILE *f,GCElement *g)
{
  fprintf(f,"  //: %s %s (%s)"
	  ,g->typeinfo->name
	  ,g->ename
	  ,g->wires[0]->net->n_signame);
  VerilogBasicGateComment(f,g,0);
  fprintf(f," /st:%d /dn:%d",g->u.sw.dipval,g->u.sw.showNet);
  fprintf(f,"\n");
}

int Switch_SimHitFunc(EditState *es,GCElement *g)
{
  GSimSwitch *ss = SHash_find(es->smod->switches,g->ename);
  gate_draw(g,0);
  *ss->state = g->u.sw.dipval = !g->u.sw.dipval;
  gate_draw(g,0);

  DoTcl("tkg_simNetSet %s 1'b%d",ss->name,g->u.sw.dipval);
  return 1;
}

void Switch_SimInitFunc(EditState *es,GCElement *g,const char *path)
{
  g->u.sw.dipval = g->u.sw.perm_dipval;
}

void SwitchDip_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/st") == 0)
    g->u.sw.dipval = g->u.sw.perm_dipval = *((int*)value);
  else if (strcmp(prop,"/dn") == 0)
    g->u.sw.showNet = *((int*)value);
}

void init_switch()
{
  Pixmap P;
  Icon *upId,*upBId,*dnId,*dnBId;
  GGateInfo *gi = &gate_switch_info;

  P = Pixmap_registerFromFile("switch","switch.b");
  
  upId = new_IconID(P,upswitch_iconDims,0);
  upBId = new_IconID(P,upswitch_iconDims,switch_iconBoldOffset);
  dnId = new_IconID(P,dnswitch_iconDims,0);
  dnBId = new_IconID(P,dnswitch_iconDims,switch_iconBoldOffset);

  gi->icon[0] = dnId;
  gi->icon[1] = upId;

  gi->icon[2] = dnId;
  gi->icon[3] = upId;

  gi->icon[4] = dnBId;
  gi->icon[5] = upBId;

  gi->icon[6] = dnId;
  gi->icon[7] = upId;

  RegisterGate(&gate_switch_info);
}
