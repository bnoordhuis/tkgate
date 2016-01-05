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

    Last edit by hansen on Sat Jan 24 09:02:39 2009
****************************************************************************/
#include "tkgate.h"

#define MULT_A 0
#define MULT_B 1
#define MULT_P 2

static void Mult_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions mult_iconDims[] = {
  {0,  0,  59, 28, 29, 15},
  {60, 0,  28, 59, 15, 29},
  {29, 60, 59, 28, 28, 12},
  {0,  29, 28, 59, 12, 29},
};
static int mult_iconBoldOffset = 89;

GPadLoc mult_A_loc[] = {
	{-16,-16,-16,-16,D_UP},
	{-16,16,-16,16,D_LEFT},
	{16,16,16,16,D_DOWN},
	{16,-16,16,-16,D_RIGHT}};

GPadLoc mult_B_loc[] = {
	{16,-16,16,-16,D_UP},
	{-16,-16,-16,-16,D_LEFT},
	{-16,16,-16,16,D_DOWN},
	{16,16,16,16,D_RIGHT}};

GPadLoc mult_P_loc[] = {
	{0,13,0,13,D_DOWN},
	{13,0,13,0,D_RIGHT},
	{0,-13,0,-13,D_UP},
	{-13,0,-13,0,D_LEFT}};

static char *psMult[] = {
  "%",
  "% x y r psmult",
  "%",
  "/psmult {",
  "  startgate",
  "  -30 15.5 moveto",
  "  -5 15.5 lineto",
  "  0 10 lineto",
  "  5 15.5 lineto",
  "  30 15.5 lineto",
  "  16 -12.5 lineto",
  "  -16 -12.5 lineto",
  "  closepath stroke",
  "  -3 -3 moveto 3 3 lineto stroke",
  "  -3 3 moveto 3 -3 lineto stroke",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_mult_info = {
  0,
  "MUL",
  "mult",0x0,
  "psmult",psMult,
  -1,2,

  {{"*",	{"gm.alu",0},		{"gm.alu.mult",0,0,200},	"gat_make MUL"},
   {0}},


  mult_iconDims,

  3,{{"A",IN,8,1,mult_A_loc},
       {"B",IN,8,1,mult_B_loc},
       {"P",OUT,8,1,mult_P_loc}},
  {{24,10,LJ},{8,-24,LJ},{-24,-8,RJ},{-12,24,RJ}},
  {1},

  {"Dab_p",0},
  
  Generic_Make,
  Mult_WriteCellDef,
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
 * Generate primitive cell definition for multipliers.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Mult_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  PrimParm primParm;
  int invP = 0;

  if (*invSpec == 'N')
    invP = 1;

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"AB_RANGE",(numBit+1)/2);
  PrimParm_rangeSet(&primParm,"P_RANGE",numBit);
  PrimParm_invSet(&primParm,"invP",invP);
  Primitive_write(f,"mult",gcs,&primParm);
}

void init_mult()
{
  Pixmap P;

  P = Pixmap_registerFromFile("mult","mult.b");
  gateinfo_iconInit(&gate_mult_info,P,mult_iconDims,mult_iconBoldOffset);
  RegisterGate(&gate_mult_info);
}
