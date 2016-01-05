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

    Last edit by hansen on Sat Sep 26 18:25:03 2009
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "tkgate.h"


#define RAM_A 0
#define RAM_D 1
#define RAM_WE 2
#define RAM_OE 3
#define RAM_CS 4

static void Ram_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions ram_iconDims[] = {
  {0,  0,   34, 49, 17, 23},
  {35, 0,   49, 34, 23, 16},
  {50, 35,  34, 49, 16, 25},
  {0,  50,  49, 34, 25, 17},
};
static int ram_iconBoldOffset = 85;

GPadLoc ram_A_loc[] = {
  {-18, 1, -18, 1, D_LEFT},
  {1, 18, 1, 18, D_DOWN},
  {18, -1, 18, -1, D_RIGHT},
  {-1, -18, -1, -18, D_UP}
};

GPadLoc ram_D_loc[] = {
  {17, -1, 17, -1, D_RIGHT},
  {-1, -17, -1, -17, D_UP},
  {-17, 1, -17, 1, D_LEFT},
  {1, 17, 1, 17, D_DOWN}

};
GPadLoc ram_we_loc[] = {
  {0, -24, 0, -24, D_UP},
  {-24, 0, -24, 0, D_LEFT},
  {0, 24, 0, 24, D_DOWN},
  {24, 0, 24, 0, D_RIGHT}
};
GPadLoc ram_cs_loc[] = {
 {-7, 26, -7, 26, D_DOWN},
 {26, 7, 26, 7, D_RIGHT},
 {7, -26, 7, -26, D_UP},
 {-26, -7, -26, -7, D_LEFT}
};
GPadLoc ram_oe_loc[] = {
 {7, 26, 7, 26, D_DOWN},
 {26, -7, 26, -7, D_RIGHT},
 {-7, -26, -7, -26, D_UP},
 {-26, 7, -26, 7, D_LEFT}
};

GCElement *RamRom_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions);
int RamRom_EditProps(GCElement *g,int isLoadDialog);
void RamRom_SetProp(GCElement*,const char*,const void*);
int RamRom_SimHitFunc(EditState *es,GCElement *g);
void RamRom_VerSave(FILE *f,GCElement *g);

static char *psRam[] = {
  "%",
  "% x y r ram",
  "%",
  "/psram {",
  "  startgate",
  "  -17.5 23.5 moveto",
  "  16.5 23.5 lineto",
  "  16.5 -25.5 lineto",
  "  -17.5 -25.5 lineto",
  "  closepath stroke",
  "  8 rfont",
  "  -15 -2 moveto (A) show",
  "  5 -2 moveto (D) show",
  "  6 rfont",
  "  (_cs) -11 -23 prshow",
  "  (_oe) 4 -23 prshow",
  "  (_we) -4 16 prshow",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_ram_info = {
  GC_RAM,
  "RAM",
  "ram",0x0,
  "psram",psRam,
  -1,-1,

  {{"R",	{"gm.mem",5},		{"gm.mem.ram",0,"mem",100},	"gat_make RAM"},
   {0}},


  ram_iconDims,

  5,{{"A",IN,8,1,ram_A_loc},
       {"D",TRI,8,1,ram_D_loc},
       {"WE",IN,1,1,ram_we_loc},
       {"OE",IN,1,1,ram_oe_loc},
       {"CS",IN,1,1,ram_cs_loc}},
  {{5,-30,LJ},{5,-30,LJ},{5,-30,LJ},{5,-30,LJ}},
  {1},

  {"Dout","Dread","Dwrite","Daddr_setup","Daddr_hold","Ddata_hold",0},
  
  RamRom_Make,
  Ram_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Generic_Draw,
  Generic_Move,
  Generic_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  RamRom_SimHitFunc,
  Generic_PSWrite,
  Generic_EditProps,
  RamRom_VerSave
};

GCElement *RamRom_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g;
  const char *DefV;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions)))
    return NULL;

  DefV = seekOption("-memfile",options,nOptions);

  ob_touch(g);

  if (!DefV)
    g->u.mem.memfile = 0;
  else
    g->u.mem.memfile = ob_strdup(DefV);

  return g;
}


int RamRom_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_memFile \"%s\"",g->u.mem.memfile ? g->u.mem.memfile : "");
  } else {
    const char *p;

    ob_touch(g);
    if (g->u.mem.memfile) ob_free(g->u.mem.memfile);
    g->u.mem.memfile = 0;

    if ((p = Tcl_GetVar(tcl,"edgat_memFile",TCL_GLOBAL_ONLY))) {
      if (strspn(p," ") != strlen(p))
	g->u.mem.memfile = ob_strdup(p);
    }
  }

  return 0;
}

void RamRom_SetProp(GCElement *g,const char *name,const void *value)
{
  if (strcmp(name,"/mem") == 0) {
    ob_touch(g);
    if (g->u.mem.memfile) ob_free(g->u.mem.memfile);
    g->u.mem.memfile = ob_strdup(value);
  }
}

int RamRom_SimHitFunc(EditState *es,GCElement *g)
{
  EditState_selectGate(es,TkGate.ed->tx,TkGate.ed->ty);
  return 1;
}

void RamRom_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);
  if (g->u.mem.memfile)
    fprintf(f," /mem:\"%s\"",g->u.mem.memfile);
  fprintf(f,"\n");
}

/*****************************************************************************
 * 
 * Generate primitive cell definition for rams.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 * Note on RAMS with invert on D port: Placing an inverter on the D port of
 * a RAM causes the RAM data to be stored inverted, but subsequent reads will
 * reinvert on output resulting in the originally stored number.  The only way
 * to see the difference between a RAM and an NRAM is when loading or storing
 * a memory contents file.
 *
 *****************************************************************************/
static void Ram_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int AnumBits = gcs->gc_multiPad;	/* Hack for ROM/RAM gates */
  int DnumBits = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  PrimParm primParm;

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"A_RANGE",AnumBits);
  PrimParm_rangeSet(&primParm,"D_RANGE",DnumBits);
  PrimParm_set(&primParm,"MEM_RANGE","[0:%u]",(1 << AnumBits) - 1);
  PrimParm_intSet(&primParm,"D_BITS",DnumBits);
  Primitive_write(f,"ram",gcs,&primParm);
}

void init_ram()
{
  Pixmap P;

  P = Pixmap_registerFromFile("ram","ram.b");
  gateinfo_iconInit(&gate_ram_info,P,ram_iconDims,ram_iconBoldOffset);
  RegisterGate(&gate_ram_info);
}

