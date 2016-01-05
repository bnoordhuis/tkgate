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

    Last edit by hansen on Sat Sep 26 18:24:54 2009
****************************************************************************/
#include <string.h>
#include "tkgate.h"

static void Rom_WriteCellDef(FILE *f,GCellSpec *gcs);


static iconDimensions rom_iconDims[] = {
  {0,  0,   34, 49, 17, 23},
  {35, 0,   49, 34, 23, 16},
  {50, 35,  34, 49, 16, 25},
  {0,  50,  49, 34, 25, 17},
};
static int rom_iconBoldOffset = 85;

GCElement *RamRom_Make(EditState **es,GModuleDef *env,int GType,
		    int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions);
int RamRom_EditProps(GCElement *g,int isLoadDialog);
void RamRom_SetProp(GCElement*,const char*,const void*);
int RamRom_SimHitFunc(EditState *es,GCElement *g);
void RamRom_VerSave(FILE *f,GCElement *g);

#define ROM_A 0
#define ROM_D 1
#define ROM_OE 2

GPadLoc rom_A_loc[] = {
  {-18, 1, -18, 1, D_LEFT},
  {1, 18, 1, 18, D_DOWN},
  {18, -1, 18, -1, D_RIGHT},
  {-1, -18, -1, -18, D_UP}
};

GPadLoc rom_D_loc[] = {
  {17, -1, 17, -1, D_RIGHT},
  {-1, -17, -1, -17, D_UP},
  {-17, 1, -17, 1, D_LEFT},
  {1, 17, 1, 17, D_DOWN}

};
GPadLoc rom_oe_loc[] = {
 {0, 26, 0, 26, D_DOWN},
 {26, 0, 26, 7, D_RIGHT},
 {0, -26, 0, -26, D_UP},
 {-26, 0, -26, 0, D_LEFT}
};

static char *psRom[] = {
  "%",
  "% x y r psrom",
  "%",
  "/psrom {",
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
  "  (_oe) -4 -23 prshow",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_rom_info = {
  GC_ROM,
  "ROM",
  "rom",0x0,
  "psrom",psRom,
  -1,-1,

  {{"u",	{"gm.mem",5},		{"gm.mem.rom",0,"mem",200},	"gat_make ROM"},
   {0}},


  rom_iconDims,

  3,{{"A",IN,8,1,rom_A_loc},
       {"D",TRI,8,1,rom_D_loc},
       {"OE",IN,1,1,rom_oe_loc}},
  {{10,-30,RJ},{10,-30,RJ},{10,-30,RJ},{10,-30,RJ}},
  {1},

  {"Dout","Dread",0},
  
  RamRom_Make,
  Rom_WriteCellDef,
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
  RamRom_EditProps,
  RamRom_VerSave,
  RamRom_SetProp
};

/*****************************************************************************
 * 
 * Generate primitive cell definition for roms.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Rom_WriteCellDef(FILE *f,GCellSpec *gcs)
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
  PrimParm_invSet(&primParm,"invD",(*invSpec == 'N'));
  Primitive_write(f,"rom",gcs,&primParm);
}

void init_rom()
{
  Pixmap P;

  P = Pixmap_registerFromFile("rom","rom.b");
  gateinfo_iconInit(&gate_rom_info,P,rom_iconDims,rom_iconBoldOffset);
  RegisterGate(&gate_rom_info);
}
