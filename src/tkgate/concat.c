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

    Last edit by hansen on Mon Jan 19 18:14:18 2009
****************************************************************************/
#include "tkgate.h"

/*
 * Flags for Concat_writeWireNums
 */
#define WWN_OUTFIRST 0x0
#define WWN_INFIRST 0x1
#define WWN_REVIN 0x2



#define CONCAT_WIRESPACE	10
#define CONCAT_HOOK		5

#define CONCAT_T_AUTO		0
#define CONCAT_T_FIXED		1
#define CONCAT_T_TRAN		2

#define AUTOCHECK_TRIES		500

void Concat_Draw(GCElement *g,int md);
void Concat_AddInput(EditState *es,GCElement*);
void Concat_RemovePort(EditState *es,GCElement*,GWire*);
GCElement *Concat_Make(EditState **es,GModuleDef *env,int GType,
		       int x,int y,int r,const char *Name,int noWires,const char**,int);
void Concat_VerSave(FILE *f,GCElement *g);
void Concat_SetProp(GCElement *g,const char *prop,const void *value);
int Concat_EditProps(GCElement *g,int isLoadDialog);

void Concat_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd);
int Concat_HitDistance(GCElement *g,int x,int y);
void Concat_PSWrite(GPrint *P,GModLayout*,GCElement *g);
GCElement *Concat_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);

static int xBitLab[] = {0,0,0,0};
static int yBitLab[] = {0,8,0,0};
static int jBitLab[] = {RJ,RJ,LJ,RJ};

GPadLoc concat_in_loc[] =	{
	{-5,-11,-5,11,D_LEFT},
	{-11,5,11,5,D_DOWN},
	{5,11,5,-11,D_RIGHT},
	{11,-5,-11,-5,D_UP}};

GPadLoc concat_out_loc[] = {
	{1,0,1,0,D_RIGHT},
	{0,-1,0,-1,D_UP},
	{-1,0,-1,0,D_LEFT},
	{0,1,0,1,D_DOWN}};

static char *psConcat[] = {
  "%",
  "% [() ...] l x y o concat",
  "%",
  "/psconcat {",
  "  dup 5 1 roll",
  "  startgate",
  "  dup 2 div -5 exch moveto",
  "  5 0 rlineto",
  "  dup neg 0 exch rlineto",
  "  -5 0 rlineto",
  "  stroke",
  "  8 rfont",
  "  exch -180 eq {",
  "    180 rotate",
  "    2 div neg 6 exch 12 add moveto",
  "    {",
  "      gsave show grestore",
  "      0 10 rmoveto",
  "    } forall",
  "  } {",
  "    2 div -6 exch 8 sub moveto",
  "    {",
  "      gsave dup stringwidth pop neg 0 rmoveto show grestore",
  "      0 -10 rmoveto",
  "    } forall",
  "  } ifelse",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_concat_info = {
  GC_CONCAT,
  "CONCAT",
  "concat",0x0,
  "psconcat",psConcat,
  -1,-1,

  {{"w",	{"gm.io",0},	{"gm.io.merge",0,0,500},	"gat_make CONCAT"},
   {0}},

  0,

  2,{{"I",IN,1,2,concat_in_loc,1},{"Z",OUT,2,1,concat_out_loc,0}},
  {{5,-20,LJ},{-5,-5,RJ},{-5,-20,RJ},{-7,15,RJ}},
  {1,1,0,1},

  {0},
  
  Concat_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Concat_GetExtents,
  Concat_HitDistance,
  Concat_Draw,
  Generic_Move,
  Concat_Copy,
  Concat_AddInput,
  Concat_AddInput,
  Err_AddInOut,
  Generic_Rotate,
  Concat_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Concat_PSWrite,
  Concat_EditProps,
  Concat_VerSave,
  Concat_SetProp
};

static void Concat_adjustWires(GCElement *g)
{
  int N,L,i,wx,wy,dx,dy;
  GWire *w;
  int d = g->u.cat.portDir ? -1 : 1;

  wx = wy = dx = dy = 0;

  N = wire_numOnPad(g->wires[CONCAT_IN]);
  L = CONCAT_WIRESPACE*(N+1);
  
  
  switch (g->orient) {
  case 0 :
    wx = g->xpos - CONCAT_HOOK;
    wy = g->ypos + d*L/2;
    dx = 0;
    dy = -d*CONCAT_WIRESPACE;
    break;
  case 1 :
    wx = g->xpos + d*L/2;
    wy = g->ypos + CONCAT_HOOK;
    dx = -d*CONCAT_WIRESPACE;
    dy = 0;
    break;
  case 2 :
    wx = g->xpos + CONCAT_HOOK;
    wy = g->ypos - d*L/2;
    dx = 0;
    dy = d*CONCAT_WIRESPACE;
    break;
  case 3 :
    wx = g->xpos - d*L/2;
    wy = g->ypos - CONCAT_HOOK;
    dx = d*CONCAT_WIRESPACE;
    dy = 0;
    break;
  }
  
  for (i = 0,w = g->wires[CONCAT_IN];w;i++, w = w->next) {
    wx += dx;
    wy += dy;
    wire_move(w->nodes,wx-w->nodes->x,wy-w->nodes->y,VERTICAL|HORIZONTAL);
  }
}

GCElement *Concat_Make(EditState **es,GModuleDef *env,int GType,
		       int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GWire *w;
  GCElement *g;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions)))
    return NULL;

  ob_touch(g);
  g->u.cat.portDir = 0;
  g->u.cat.drivePort = -1;
  g->u.cat.type = CONCAT_T_AUTO;

  if (es && (g->orient & 0x2)) g->u.cat.portDir = 1;


  if (!noWires) {
    w = wire_drivee(g->wires[CONCAT_OUT]);
    net_setSize(w->net,2);

    Concat_adjustWires(g);
  }

  return g;
}

GCElement *Concat_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *c_g = Generic_Copy(M,g,x,y,flags);
  ob_touch(c_g);
  c_g->u.cat.portDir = g->u.cat.portDir;
  c_g->u.cat.drivePort = g->u.cat.drivePort;
  c_g->u.cat.type = g->u.cat.type;

  return c_g;
}

void Concat_GetExtents(GCElement *g,TargetDev_e target, int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  int N = wire_numOnPad(g->wires[CONCAT_IN]);
  int L = (N > 0) ? (CONCAT_WIRESPACE*(N+1)) : 2*CONCAT_WIRESPACE;
  int gx = g->xpos;
  int gy = g->ypos;

  if (bd) *bd = 0;


  switch (g->orient) {
  case 0 :
  case 2 :
    *minx = gx - 5;
    *maxx = gx + 5;
    *miny = gy-L/2-5;
    *maxy = gy+L/2-5;
    break;
  case 1 :
  case 3 :
    *minx = gx-L/2-5;
    *maxx = gx+L/2-5;
    *miny = gy - 5;
    *maxy = gy + 5;
    break;
  }
}

int Concat_HitDistance(GCElement *g,int x,int y)
{
  int N = wire_numOnPad(g->wires[CONCAT_IN]);
  int L = (N > 0) ? (CONCAT_WIRESPACE*(N+1)) : 2*CONCAT_WIRESPACE;
  int gx = g->xpos;
  int gy = g->ypos;

  switch (g->orient) {
  case 0 :
  case 2 :
    if (y >= gy-L/2-5 && y <= gy+L/2+5) y = gy;
    break;
  case 1 :
  case 3 :
    if (x >= gx-L/2-5 && x <= gx+L/2+5) x = gx;
    break;
  }
  return distance(x,y,gx,gy);
}

void Concat_Draw(GCElement *g,int md)
{
  int N = wire_numOnPad(g->wires[CONCAT_IN]);
  int L;
  int x,y;
  GWire *w;
  int lsb;
  int iod;
  int atype;
  
  x = ctow_x(g->xpos);
  y = ctow_y(g->ypos);

  L = (N > 0) ? (CONCAT_WIRESPACE*(N+1)) : 2*CONCAT_WIRESPACE;

  switch (g->orient) {
  case 0 :
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y+L/2,x,y-L/2);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y+L/2+1,x-CONCAT_HOOK,y+L/2+1);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y-L/2-1,x-CONCAT_HOOK,y-L/2-1);
    if (g->selected) {
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x-1,y+L/2,x-1,y-L/2);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y+L/2+2,x-CONCAT_HOOK,y+L/2+2);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y-L/2-2,x-CONCAT_HOOK,y-L/2-2);
    }
    break;
  case 1 :
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2,y,x-L/2,y);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2+1,y,x+L/2+1,y+CONCAT_HOOK);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x-L/2-1,y,x-L/2-1,y+CONCAT_HOOK);
    if (g->selected) {
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2,y+1,x-L/2,y+1);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2+2,y,x+L/2+2,y+CONCAT_HOOK);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x-L/2-2,y,x-L/2-2,y+CONCAT_HOOK);
    }
    break;
  case 2 :
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y+L/2,x,y-L/2);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y+L/2+1,x+CONCAT_HOOK,y+L/2+1);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y-L/2-1,x+CONCAT_HOOK,y-L/2-1);
    if (g->selected) {
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+1,y+L/2,x+1,y-L/2);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y+L/2+2,x+CONCAT_HOOK,y+L/2+2);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x,y-L/2-2,x+CONCAT_HOOK,y-L/2-2);
    }
    break;
  case 3 :
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2,y,x-L/2,y);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2+1,y,x+L/2+1,y-CONCAT_HOOK);
    ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x-L/2-1,y,x-L/2-1,y-CONCAT_HOOK);
    if (g->selected) {
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2,y-1,x-L/2,y-1);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x+L/2+2,y,x+L/2+2,y-CONCAT_HOOK);
      ZDrawLine(TkGate.D,TkGate.W,TkGate.instGC,x-L/2-2,y,x-L/2-2,y-CONCAT_HOOK);
    }
    break;
  }

  gate_drawWires(g,md);

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);

  if (g->u.cat.drivePort == CONCAT_OUT)
    iod = OUT;
  else if (g->u.cat.drivePort == CONCAT_IN)
    iod = IN;
  else {
    iod = ANY;
  }

  if (g->u.cat.type == CONCAT_T_AUTO) {
    atype = IODT_PLAIN;
  } else if (g->u.cat.type == CONCAT_T_FIXED)
    atype = IODT_BOXED;
  else {
    atype = IODT_BOXED;
    iod = TRI;
  }

  DrawPinIOMark(g->wires[CONCAT_OUT],
		((g->typeinfo->Pad[CONCAT_OUT].Loc[0].dir + g->orient) % 4), iod,atype);

  lsb = 0;
  for (w = g->wires[CONCAT_IN];w;w = w->next) {
    char buf[STRMAX];

    if (w->net->n_nbits == 1)
      sprintf(buf,"%d",lsb);
    else
      sprintf(buf,"%d:%d",lsb+w->net->n_nbits-1,lsb);
    lsb += w->net->n_nbits;

    x = w->nodes->x + xBitLab[g->orient];
    y = w->nodes->y + yBitLab[g->orient];
	  
    dce_DrawString(TkGate.instGC,x,y,jBitLab[g->orient],buf);
  }

  if (g->ename && g->show_name)
    gate_drawgatename(g,g->ename);
}

void Concat_AddInput(EditState *es,GCElement *g)
{
  GWire *e1,*e2;

  gate_draw(g,0);

  wire_new(es->env,&e1,&e2);

  ob_touch(g);
  ob_touch(e1->nodes);
  ob_touch(e2->nodes);

  e1->nodes->x = e2->nodes->x = 0;
  e1->nodes->y = e2->nodes->y = 0;

  g->wires[CONCAT_IN] = wire_append(g->wires[CONCAT_IN],e1);
  e1->gate = g;
  e1->orient = g->typeinfo->Pad[CONCAT_IN].Loc[g->orient].dir;

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
  Concat_adjustWires(g);

  gate_draw(g,0);
}

void Concat_RemovePort(EditState *es,GCElement *g,GWire *w)
{
  Generic_RemovePort(es,g,w);
  Concat_adjustWires(g);
}

int Concat_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_catdir %d",g->u.cat.portDir);

    switch (g->u.cat.type) {
    case CONCAT_T_FIXED : DoTcl("set ::edgat_sigdir %d",g->u.cat.drivePort); break;
    case CONCAT_T_AUTO  : DoTcl("set ::edgat_sigdir 2"); break;
    case CONCAT_T_TRAN  : DoTcl("set ::edgat_sigdir 3"); break;
    }
  } else {
    const char *p;
    if ((p = Tcl_GetVar(tcl,"edgat_catdir",TCL_GLOBAL_ONLY))) {
      int eb;

      if (sscanf(p,"%d",&eb) == 1 && eb != g->u.cat.portDir) {
	ob_touch(g);
	gate_draw(g,0);
	g->u.cat.portDir = eb;
	Concat_adjustWires(g);
	gate_draw(g,0);
      }
    }
    if ((p = Tcl_GetVar(tcl,"edgat_sigdir",TCL_GLOBAL_ONLY))) {
      int sd;
      int newtype = g->u.cat.type;
      int newdrive = g->u.cat.drivePort;

      if (sscanf(p,"%d",&sd) == 1) {
	switch (sd) {
	case 0 : 
	case 1 :
	  newtype = CONCAT_T_FIXED; newdrive = sd;
	  break;
	case 2 :
	  newtype = CONCAT_T_AUTO;
	  if (g->u.cat.type != CONCAT_T_AUTO)
	    newdrive = -1;
	  break;
	case 3 :
	  newtype = CONCAT_T_TRAN;
	  break;
	}
      }

      if (newtype != g->u.cat.type || newdrive != g->u.cat.drivePort) {
	ob_touch(g);
	gate_draw(g,GD_NOWIRE);
	g->u.cat.type = newtype;
	g->u.cat.drivePort = newdrive;
	gate_draw(g,GD_NOWIRE);
      }
    }
  }

  return 0;
}


void Concat_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/dr") == 0) {
    int n = *((int*)value);
    ob_touch(g);
    g->u.cat.portDir = n;
  } else if (strcmp(prop,"/tp") == 0) {
    int n = *((int*)value);
    ob_touch(g);
    g->u.cat.type = n;
  } else if (strcmp(prop,"/drp") == 0) {
    int n = *((int*)value);
    ob_touch(g);
    g->u.cat.drivePort = n;
  }
}

int Concat_GateParmList(FILE *f,GCElement *g)
{
  GWire *outw = g->wires[CONCAT_OUT];
  GWire *w;
  int first_pin;

  first_pin = 1;
  fprintf(f," ({");

  for (w = g->wires[CONCAT_IN];w;w = w->next) {
    if (!first_pin)
      fprintf(f,", ");
    first_pin = 0;

    fprintf(f,"%s",w->net->n_signame);
  }
  fprintf(f,"}, %s);",outw->net->n_signame);
  return 0;
}

/*****************************************************************************
 *
 * Write the list of ports on the 'multi-port' pad in the correct order. 
 *
 * Parameters:
 *     f	File on which to write
 *     g	Device from which ports should be found.
 *
 * Writes the ports on the CONCAT_IN pad to the output file.
 *
 *****************************************************************************/
static void Concat_writeMPorts(FILE *f,GCElement *g)
{
  int first_pin = 1;
  GWire **wires; 
  GWire *w;
  int n = 0;
  int i;

  for (w = g->wires[CONCAT_IN];w;w = w->next) n++;
  wires = (GWire**)malloc(sizeof(GWire*)*n);
  for (w = g->wires[CONCAT_IN], i = 0;w;w = w->next, i++) wires[i] = w;

  for (i = n-1;i >= 0;i--) {
    if (!first_pin) fprintf(f,", ");
    first_pin = 0;
    fprintf(f,"%s",wires[i]->net->n_signame);
  }
  free(wires);
}

/*****************************************************************************
 *
 * Write the wire numbers for the ports on a concat gate. 
 *
 *****************************************************************************/
static void Concat_writeWireNums(FILE *f,GCElement *g,unsigned flags)
{
  GWire **wires; 
  GWire *w;
  int n = 0;
  int i;

  for (w = g->wires[CONCAT_IN];w;w = w->next) n++;
  wires = (GWire**)malloc(sizeof(GWire*)*n);

  for (w = g->wires[CONCAT_IN],i=0;w;w = w->next, i++) wires[i] = w;

  fprintf(f," /w:[");

  if (!(flags & WWN_INFIRST))
    fprintf(f," %d",g->wires[CONCAT_OUT]->nidx);

  if ((flags & WWN_REVIN)) {
    for (i = 0;i < n;i++) 
      fprintf(f," %d",wires[i]->nidx);
  } else {
    for (i = n-1;i >= 0;i--) 
      fprintf(f," %d",wires[i]->nidx);
  }

  if ((flags & WWN_INFIRST))
    fprintf(f," %d",g->wires[CONCAT_OUT]->nidx);

  fprintf(f," ]");

  free(wires);
}


/*****************************************************************************
 *
 * Save concat device in verilog format
 *
 * Paramaters:
 *    f		File to write to.
 *    g		Device to write (must be concat)
 *
 *****************************************************************************/
void Concat_VerSave(FILE *f,GCElement *g)
{
  int nbits = g->wires[CONCAT_OUT]->net->n_nbits;
  int did_output = 0;

  if (g->u.cat.type == CONCAT_T_TRAN) {
    /* fall through to !did_output block */
  } else if (g->u.cat.drivePort == CONCAT_IN) {
    fprintf(f,"  assign {");
    Concat_writeMPorts(f,g);
    fprintf(f,"} = %s; //: CONCAT %s ",g->wires[CONCAT_OUT]->net->n_signame,g->ename); 
    VerilogBasicGateComment(f,g,VGC_NOWIRES);
    Concat_writeWireNums(f,g,WWN_INFIRST);
    fprintf(f," /dr:%d /tp:%d /drp:%d",g->u.cat.portDir,g->u.cat.type,g->u.cat.drivePort);
    fprintf(f,"\n");
    did_output = 1;
  } else if (g->u.cat.drivePort == CONCAT_OUT) {
    fprintf(f,"  assign %s = {",g->wires[CONCAT_OUT]->net->n_signame);
    Concat_writeMPorts(f,g);
    fprintf(f,"}; //: CONCAT %s ",g->ename); 
    VerilogBasicGateComment(f,g,VGC_NOWIRES);
    Concat_writeWireNums(f,g,WWN_OUTFIRST);
    fprintf(f," /dr:%d /tp:%d /drp:%d",g->u.cat.portDir,g->u.cat.type,g->u.cat.drivePort);
    fprintf(f,"\n");
    did_output = 1;
  }

  if (!did_output) {
    fprintf(f,"  tran %s[%d:%d]",g->ename,nbits-1,0);
    Concat_GateParmList(f,g);
    VerilogBasicGateComment(f,g,VGC_NOWIRES|VGC_DOMARK);
    fprintf(f," /dr:%d /tp:%d /drp:%d",g->u.cat.portDir,g->u.cat.type,g->u.cat.drivePort);
    Concat_writeWireNums(f,g,WWN_INFIRST|WWN_REVIN);
    fprintf(f,"\n");
  }
}

void Concat_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  int N = wire_numOnPad(g->wires[CONCAT_IN]);
  int len = (N > 0) ? (CONCAT_WIRESPACE*(N+1)) : 2*CONCAT_WIRESPACE;
  char buf[STRMAX];
  int *wire_nbits;
  int nwires = 0;
  int nbits = 0;
  int ncount;
  GWire *w;

  Generic_PSLabels(P,g);

  /*
  ** We need to collect bit-widths of wires into an array, since we don't
  ** know it which order we will be going through them.
  */
  for (w = g->wires[CONCAT_IN];w;w = w->next)
  {
    nbits += w->net->n_nbits;
    nwires++;
  }

  wire_nbits = malloc(nwires * sizeof(int));

  nwires = 0;
  for (w = g->wires[CONCAT_IN];w;w = w->next) {
    wire_nbits[nwires++] = w->net->n_nbits;
  }

  /*
  ** Now use this information to print the wires.
  */
  fprintf(P->p_f,"[\n");
  if (g->u.cat.portDir) {
    int lsb = 0;
    for (ncount = 0; ncount < nwires; ncount++) {
      if (wire_nbits[ncount] == 1)
	sprintf(buf,"%d",lsb);
      else
	sprintf(buf,"%d:%d",lsb+wire_nbits[ncount]-1,lsb);
      fprintf(P->p_f,"  (%s)\n",buf);
      lsb += wire_nbits[ncount];
    }
  } else {
    int msb = nbits-1;
    int ncount = nwires;
    for (ncount = nwires - 1;ncount >= 0; ncount--) {
      if (wire_nbits[ncount] == 1)
	sprintf(buf,"%d",msb);
      else
	sprintf(buf,"%d:%d",msb,msb-wire_nbits[ncount]+1);
      fprintf(P->p_f,"  (%s)\n",buf);
      msb -= wire_nbits[ncount];
    }
  }

  fprintf(P->p_f,"] %d %d %d %d psconcat\n",len,g->xpos,g->ypos,-g->orient*90);
  free(wire_nbits);
}

/*****************************************************************************
 *
 * Return the port number of the driver (or -1 if not set)
 *
 *****************************************************************************/
static int concat_isDriver(GCElement *g,GWire *w)
{
  switch (g->u.cat.type) {
  case CONCAT_T_AUTO :
    if (g->u.cat.newDrivePort == CONCAT_IN)
      return w != g->wires[CONCAT_OUT];
    else if (g->u.cat.newDrivePort == CONCAT_OUT)
      return w == g->wires[CONCAT_OUT];
    return 0;
  case CONCAT_T_FIXED :
    if (g->u.cat.drivePort == CONCAT_IN)
      return w != g->wires[CONCAT_OUT];
    else if (g->u.cat.drivePort == CONCAT_OUT)
      return w == g->wires[CONCAT_OUT];
    return 0;
  case CONCAT_T_TRAN :
    return 0;
  }

  return 0;
}


/*****************************************************************************
 *
 * Test wires on net for drivers
 *
 * Parameters:
 *     net	Net that we are testing.
 *
 * Returns:	Non-zero if the net has a driver 
 *
 *****************************************************************************/
static int concat_hasDriver(GWire *w)
{
  int p,n;

  w = wire_other(w);
  if (w->gate) {
    GCElement *g = w->gate;
    int i;

    switch (g->typeinfo->Code) {
    case GC_CONCAT :
      return concat_isDriver(g,w);
    case GC_JOINT :
      for (i = 0;i < 4;i++)
	if (g->wires[i] && g->wires[i] != w) {
	  if (concat_hasDriver(g->wires[i]))
	    return 1;
	}
      break;
    case GC_TAP :
      if (w == g->wires[TAP_IN]) {
	if (concat_hasDriver(g->wires[TAP_OUT]))
	  return 1;
      } else if (w == g->wires[TAP_OUT]) {
	if (concat_hasDriver(g->wires[TAP_IN]))
	  return 1;
      } else
	return 1;
      break;
    case GC_BLOCK :
      if (posongate(w,w->gate,&p,&n) == 0) {
	if (w->gate->typeinfo->Pad[p].iotype == OUT)
	  return 1;
      }
      break;
    default :
      if (posongate(w,w->gate,&p,&n) == 0) {
	if (w->gate->typeinfo->Pad[p].iotype == OUT || w->gate->typeinfo->Pad[p].iotype == TRI)
	  return 1;
      }
      break;
    }
  }

  return 0;
}


/*****************************************************************************
 *
 * Scan module and update direction on any auto concat elements.
 *
 * Parameters
 *    M		Module to scan
 *    doDraw	Redraw any concat elements that changed.
 *
 * Note: although we modify the direction member(s) on auto concat elements,
 * we do not consider this an ob_touch() since we can easily recompute it.
 *
 *****************************************************************************/
void Concat_updateAutos(GModuleDef *M,int doDraw)
{
  HashElem *he;
  int count = 0;
  GCElement **concats = 0;
  int i,j;

#if 0
  printf("Concat_updateAutos\n");
#endif


  /*
   * Count the AUTO wire merge devices and reset the new drive ports
   * to unknown.
   */
  for (he = Hash_first(M->m_gates);he;he = Hash_next(M->m_gates,he)) {
    GCElement *g = HashElem_obj(he);

    /*
     * If gate is not a concat with auto directioning, ignore it.
     */
    if (g->typeinfo != &gate_concat_info) continue;
    if (g->u.cat.type != CONCAT_T_AUTO) continue;

    g->u.cat.newDrivePort = -1;
    count++;
  }
  if (!count) return;

  /*
   * Build temporary array for concat elements and reset counter
   */
  concats = (GCElement**) malloc(sizeof(GCElement*)*count);
  count = 0;

  /*
   * Collect the wiremerge devices in the array.
   */
  for (he = Hash_first(M->m_gates);he;he = Hash_next(M->m_gates,he)) {
    GCElement *g = HashElem_obj(he);

    /*
     * If gate is not a concat with auto directioning, ignore it.
     */
    if (g->typeinfo != &gate_concat_info) continue;
    if (g->u.cat.type != CONCAT_T_AUTO) continue;
    concats[count++] = g;
  }

  /*
   * Cycle through wire merge devices up to AUTOCHECK_TRIES times to
   * set the direction.
   */
  for (j = 0;j < AUTOCHECK_TRIES;j++) {
    int changed = 0;
    GWire *w;

#if 0
    printf("  try %d\n",j);
#endif

    for (i = 0;i < count;i++) {
      GCElement *g = concats[i];
      int in_has_driver = 0;
      int out_has_driver = 0;
      int new_port;

      for (w = g->wires[CONCAT_IN];w;w = w->next) {
	if (concat_hasDriver(w))
	  in_has_driver = 1;
      }
      if (concat_hasDriver(g->wires[CONCAT_OUT]))
	out_has_driver = 1;

      if (in_has_driver && !out_has_driver) {
	new_port = CONCAT_OUT;
      } else if (out_has_driver && !in_has_driver) {
	new_port = CONCAT_IN;
      } else if (out_has_driver && in_has_driver) {
	new_port = -1;
      } else {
	new_port = -1;
      }

#if 0
      printf("    %s(in:%d out:%d): %d => %d\n",
	     g->ename, 
	     in_has_driver, out_has_driver,
	     g->u.cat.newDrivePort, new_port);
#endif

      if (g->u.cat.newDrivePort != new_port) {
	g->u.cat.newDrivePort = new_port;
	changed = 1;
      }
    }
    /*
     * If nothing could be updated, end loop.
     */
    if (!changed) break;
  }

  for (i = 0;i < count;i++) {
    GCElement *g = concats[i];
    if (g->u.cat.drivePort != g->u.cat.newDrivePort) {
      if (doDraw)
	gate_draw(g,GD_NOWIRE);
      g->u.cat.drivePort = g->u.cat.newDrivePort;
      if (doDraw)
	gate_draw(g,GD_NOWIRE);
    }
  }

  free(concats);
}

void init_concat()
{
  RegisterGate(&gate_concat_info);
}
