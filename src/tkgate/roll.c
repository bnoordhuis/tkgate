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

    Last edit by hansen on Mon Jan 19 18:17:34 2009
****************************************************************************/
#include "tkgate.h"

#define ROLL_IN 0
#define ROLL_OUT 1
#define ROLL_SHIFT 2

static void Roll_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions roll_iconDims[] = {
  {0, 21, 75, 20, 36, 9},
  {76, 0, 20, 75, 10, 37},
  {0,  0, 75, 20, 36, 9},
  {97, 0, 20, 75, 10, 37},
};
static int roll_iconBoldOffset = 76;


GPadLoc roller_in_loc[] = {
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT},
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT}};

GPadLoc roller_out_loc[] = {
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT}};

GPadLoc roller_shift_loc[] = {
	{-37,0,-37,0,D_LEFT},
	{0,38,0,38,D_DOWN},
	{39,0,39,0,D_RIGHT},
	{0,-38,0,-38,D_UP}};

static char *psRoller[] = {
  "%",
  "% A Roller",
  "/psroll {",
  "  [[0 .5][-.5 .5][2 0.5][0 -.5]] adjstartgate",
  "  -31 -10.5 moveto",
  "  -37.5 -5 lineto",
  "  -37.5 4 lineto",
  "  -31 10.5 lineto",
  "  31 10.5 lineto",
  "  37.5 4 lineto",
  "  37.5 -5 lineto",
  "  31 -10.5 lineto",
  "  closepath stroke",
  "  2 setlinewidth",
  "  newpath",
  "  0 0 6 20 340 arc stroke",
  "  30 rotate",
  "  4 0 translate",
  "  -70 rotate",
  "  -1.5 0 moveto 1.5 0 lineto",
  "  0 3 lineto closepath stroke",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_roller_info = {
  0,
  "ROLL",
  "roll",0x0,
  "psroll",psRoller,
  -1,1,

  {{"S O",	{"gm.alu",0},		{"gm.alu.roll",0,"shift",400},	"gat_make ROLL"},
   {0}},
  

  roll_iconDims,

  3,{{"I",IN,8,1,roller_in_loc},
       {"Z",OUT,8,1,roller_out_loc},
       {"S",IN,3,1,roller_shift_loc}},
  {{12,-12,LJ},{12,-12,LJ},{12,-12,LJ},{12,-12,LJ}},
  {1},

  {"Dsz","Diz",0},

  Generic_Make,
  Roll_WriteCellDef,
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


static void Roll_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int numBits = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  int shiftSelBits = required_bits(numBits);
  PrimParm primParm;


  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"IZ_RANGE",numBits);
  PrimParm_intSet(&primParm,"IZ_BITS",numBits);
  PrimParm_rangeSet(&primParm,"S_RANGE",shiftSelBits);
  PrimParm_invSet(&primParm,"invZ",(*invSpec == 'N'));
  Primitive_write(f,"roll",gcs,&primParm);
}

void init_roll()
{
  Pixmap P;

  P = Pixmap_registerFromFile("roll","roll.b");
  gateinfo_iconInit(&gate_roller_info,P,roll_iconDims,roll_iconBoldOffset);
  RegisterGate(&gate_roller_info);
}
