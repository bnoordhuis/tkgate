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

    Last edit by hansen on Mon Jan 19 18:14:05 2009
****************************************************************************/
#include "tkgate.h"

#define CLOCK_OUT 0
#define CLOCKPARAMXOFFSET 0
#define CLOCKPARAMYOFFSET 20

/* Defines local to this file */
#define F_PW 0x1
#define F_PHI 0x2
#define F_DW 0x4

/* Default clock parameters */
#define CLOCK_OMEGA	100
#define CLOCK_PHI    	0
#define CLOCK_DUTY	50

static void Clock_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions clock_iconDims[] = {
  {0, 0, 27, 25, 13, 12},
  {27, 0, 25, 26, 12, 14},
  {26, 27, 27, 25, 12, 13},
  {0, 26, 25, 26, 15, 14},
};
static int clock_iconBoldOffset = 53;

GCElement *Clock_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char**,int);
void Clock_Draw(GCElement *g,int md);
GCElement *Clock_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
int Clock_EditProps(GCElement *g,int isLoadDialog);
void Clock_PSWrite(GPrint *P,GModLayout*,GCElement *g);
void Clock_VerSave(FILE*,GCElement*);
void Clock_SetProp(GCElement*,const char*,const void*);

GPadLoc clock_out_loc[] = {
	{13,0,13,0,D_RIGHT},
	{0,-15,0,-15,D_UP},
	{-13,-1,-13,-1,D_LEFT},
	{-3,12,-3,12,D_DOWN}};

static char *psClock[] = {
  "%",
  "% dw phi pw x y r clockgate",
  "%",
  "/psclock_base {",
  "  [[-1 0][0 0][1 -1][-3 -2]] adjstartgate",
  "  -8 -12 moveto",
  "  16 0 lineto",
  "  -8 12 lineto",
  "  closepath",
  "  stroke",
  "  -6 -4 moveto",
  "  -4 -4 lineto",
  "  -4 4 lineto",
  "  0 4 lineto",
  "  0 -4 lineto",
  "  2 -4 lineto",
  "  stroke",
  "  grestore",
  "} def",
  "/psclock {",
  "  3 copy psclock_base",
  "  startnorotgate",
  "  0 -20 translate",
  "  0 0 moveto",
  "  8 rfont",
  "  (P = ) show",
  "  20 string cvs show",
  "  ( ) show",
  "  8 syfont",
  "  (f) show ",
  "  8 rfont",
  "  ( = ) show",
  "  20 string cvs show",
  "  ( D = ) show",
  "  20 string cvs show",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_clock_info = {
  GC_CLOCK,
  "CLOCK",
  "clock",0x0,
  "psclock",psClock,
  -1,-1,

  {{"c",	{"gm.io",0},		{"gm.io.clock",0,"x",100},	"gat_make CLOCK"},
   {0}},

  clock_iconDims,

  1,{{"Z",OUT,1,1,clock_out_loc}},
  {{0,-17,CT},{17,0,LJ},{0,-17,CT},{17,0,LJ}},
  {1},
  
  {0},

  Clock_Make,
  Clock_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Clock_Draw,
  Generic_Move,
  Clock_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Clock_PSWrite,
  Clock_EditProps,
  Clock_VerSave,
  Clock_SetProp
};

void gate_drawclockparams(GCElement *g)
{
  char b[STRMAX];
  int x,y;
  
  x = g->xpos + CLOCKPARAMXOFFSET;
  y = g->ypos + CLOCKPARAMYOFFSET;

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);
  
  sprintf(b,"f=%d ",g->u.clock.omega);
  x = RelPosDrawString(TkGate.W,TkGate.stextXF[TkGate.circuit->zoom_factor],TkGate.instGC,x,y,b,AtLeft);
  
  sprintf(b,"p=%d ",g->u.clock.phi);
  x = RelPosDrawString(TkGate.W,TkGate.stextXF[TkGate.circuit->zoom_factor],TkGate.instGC,x,y,b,AtLeft);
  
  sprintf(b,"dw=%d ",g->u.clock.duty);
  x = RelPosDrawString(TkGate.W,TkGate.stextXF[TkGate.circuit->zoom_factor],TkGate.instGC,x,y,b,AtLeft);
}

GCElement *Clock_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char **options,int nOptions)
{
  GCElement *g;
  const char *LPW,*LPhi,*LDW;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWire,options,nOptions)))
    return NULL;

  LPW  = seekOption("-pw",options,nOptions);
  LPhi = seekOption("-phi",options,nOptions);
  LDW  = seekOption("-dw",options,nOptions);

  if (!LPW || sscanf(LPW,"%hd",&g->u.clock.omega) != 1)
    g->u.clock.omega = CLOCK_OMEGA;

  if (!LPhi || sscanf(LPhi,"%hd",&g->u.clock.phi) != 1)
    g->u.clock.phi = CLOCK_PHI;

  if (!LDW || sscanf(LDW,"%hd",&g->u.clock.duty) != 1)
    g->u.clock.duty = CLOCK_DUTY;
  
  return g;
}


void Clock_Draw(GCElement *g,int md)
{
  Generic_Draw(g,md);
  gate_drawclockparams(g);
}

GCElement *Clock_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  ng = Generic_Copy(M,g,x,y,flags);

  ng->u.clock.omega = g->u.clock.omega;
  ng->u.clock.phi = g->u.clock.phi;
  ng->u.clock.duty = g->u.clock.duty;

  return ng;
}

void Clock_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  Generic_PSLabels(P,g);
  
  fprintf(P->p_f,"%d %d %d %d %d %d %s\n",
	  g->u.clock.duty,g->u.clock.phi,g->u.clock.omega,
	  g->xpos,g->ypos,-g->orient*90,g->typeinfo->psprint);
}

int Clock_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::ecpOmega %d",g->u.clock.omega);
    DoTcl("set ::ecpPhi %d",g->u.clock.phi);
    DoTcl("set ::ecpDuty %d",g->u.clock.duty);
  } else {
    const char *p;
    if ((p = Tcl_GetVar(tcl,"ecpOmega",TCL_GLOBAL_ONLY)))
      sscanf(p,"%hd",&g->u.clock.omega);
    if ((p = Tcl_GetVar(tcl,"ecpPhi",TCL_GLOBAL_ONLY)))
      sscanf(p,"%hd",&g->u.clock.phi);
    if ((p = Tcl_GetVar(tcl,"ecpDuty",TCL_GLOBAL_ONLY)))
      sscanf(p,"%hd",&g->u.clock.duty);
  }

  return 0;
}

void Clock_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);
  fprintf(f," /omega:%d /phi:%d /duty:%d",g->u.clock.omega,g->u.clock.phi,g->u.clock.duty);
  fprintf(f,"\n");
}

void Clock_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/omega") == 0) {
    int n = *((int*)value);
    g->u.clock.omega = n;
  } else if (strcmp(prop,"/phi") == 0) {
    int n = *((int*)value);
    g->u.clock.phi = n;
  } else if (strcmp(prop,"/duty") == 0) {
    int n = *((int*)value);
    g->u.clock.duty = n;
  }
}

static void Clock_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  const char *invSpec = gcs->gc_invSpec;
  int period = gcs->gc_parms[0];
  int shift  = gcs->gc_parms[1]*period/100;
  int duty   = gcs->gc_parms[2];
  int on_time, off_time;
  int on_value = 1;
  int start_value;
  int phase_wait;
  PrimParm primParm;

  if (period < 2) period = 2;
  if (duty <= 0 || duty >= 100) duty = 50;
  if (shift < 0) shift = 0;
  off_time = period*duty/100;
  on_time = period - off_time;
  shift %= period;
  if (on_time <= 0) {
    on_time = 1;
    off_time--;
  }
  if (off_time <= 0) {
    off_time = 1;
    on_time--;
  }

  if (shift < off_time) {
    start_value = 0;
    phase_wait = off_time-shift;
  } else {
    start_value = 1;
    phase_wait = period-shift;
  }
  

  PrimParm_init(&primParm);
  PrimParm_intSet(&primParm,"INITIAL",start_value);
  PrimParm_intSet(&primParm,"PHASE",phase_wait);
  PrimParm_intSet(&primParm,"OFF_TIME",off_time);
  PrimParm_intSet(&primParm,"ON_TIME",on_time);
  PrimParm_invSet(&primParm,"invZ",(*invSpec == 'N'));
  Primitive_write(f,"clock",gcs,&primParm);
}

void init_clock()
{
  Pixmap P;

  P = Pixmap_registerFromFile("clock","clock.b");
  gateinfo_iconInit(&gate_clock_info,P,clock_iconDims,clock_iconBoldOffset);
  RegisterGate(&gate_clock_info);
}
