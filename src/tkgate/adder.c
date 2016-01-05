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

    Last edit by hansen on Mon Jan 19 18:13:37 2009
****************************************************************************/
#include "tkgate.h"

#define ADDER_A 0
#define ADDER_B 1
#define ADDER_S 2
#define ADDER_CI 3
#define ADDER_CO 4

static void Adder_WriteCellDef(FILE *f,GCellSpec *gcs);


static iconDimensions add_iconDims[] = {
  {0,  0,  59, 28, 29, 15},
  {60, 0,  28, 59, 15, 29},
  {29, 60, 59, 28, 28, 12},
  {0,  29, 28, 59, 12, 29},
};
static int add_iconBoldOffset = 89;

GPadLoc adder_A_loc[] = {
	{-16,-16,-16,-16,D_UP},
	{-16,16,-16,16,D_LEFT},
	{16,16,16,16,D_DOWN},
	{16,-16,16,-16,D_RIGHT}};

GPadLoc adder_B_loc[] = {
	{16,-16,16,-16,D_UP},
	{-16,-16,-16,-16,D_LEFT},
	{-16,16,-16,16,D_DOWN},
	{16,16,16,16,D_RIGHT}};

GPadLoc adder_Ci_loc[] = {
	{24,-2,24,-2,D_RIGHT},
	{-2,-24,-2,-24,D_UP},
	{-24,2,-24,2,D_LEFT},
	{2,24,2,24,D_DOWN}};

GPadLoc adder_Co_loc[] = {
	{-24,-2,-24,-2,D_LEFT},
	{-2,24,-2,24,D_DOWN},
	{24,2,24,2,D_RIGHT},
	{2,-24,2,-24,D_UP}};


GPadLoc adder_S_loc[] = {
	{0,13,0,13,D_DOWN},
	{13,0,13,0,D_RIGHT},
	{0,-13,0,-13,D_UP},
	{-13,0,-13,0,D_LEFT}};

static char *psAdder[] = {
  "%",
  "% x y r adder",
  "%",
  "/psadder {",
  "  startgate",
  "  -30 15.5 moveto",
  "  -5 15.5 lineto",
  "  0 10 lineto",
  "  5 15.5 lineto",
  "  30 15.5 lineto",
  "  16 -12.5 lineto",
  "  -16 -12.5 lineto",
  "  closepath stroke",
  "  -3 0 moveto 3 0 lineto stroke",
  "  0 3 moveto 0 -3 lineto stroke",
  "  19 2 2 0 360 arc closepath fill",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_add_info = {
  0,
  "ADD",
  "add",0x0,
  "psadder",psAdder,
  -1,2,

  {{"+",	{"gm.alu",0},		{"gm.alu.add",0,0,100},	"gat_make ADD"},
   {0}},


  add_iconDims,

  5,{{"A",IN,8,1,adder_A_loc},
       {"B",IN,8,1,adder_B_loc},
       {"S",OUT,8,1,adder_S_loc},
       {"CI",IN,1,1,adder_Ci_loc},
       {"CO",OUT,1,1,adder_Co_loc}},
  {{24,10,LJ},{8,-24,LJ},{-24,-8,RJ},{-12,24,RJ}},
  {1},

  {"Dab_s","Dab_co","Dci_s","Dci_co",0},
  
  Generic_Make,
  Adder_WriteCellDef,
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
  Nop_SimHitFunc,
  Generic_PSWrite,
  Generic_EditProps,
  Generic_VerSave
};

/*****************************************************************************
 * 
 * Generate primitive cell definition for adders.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Adder_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  PrimParm primParm;
  int invS = 0,invCO = 0;


  if (*invSpec) {
    if (*invSpec == 'N')
      invS = 1;
    if (!invSpec[1] || invSpec[1] == 'N')
      invCO = 0;
  }

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"ABS_RANGE",numBit);
  PrimParm_invSet(&primParm,"invS",invS);
  PrimParm_invSet(&primParm,"invCO",invCO);
  Primitive_write(f,"adder",gcs,&primParm);
}

void init_adder()
{
  Pixmap P;

  P = Pixmap_registerFromFile("add","add.b");
  gateinfo_iconInit(&gate_add_info,P,add_iconDims,add_iconBoldOffset);
  RegisterGate(&gate_add_info);
}
