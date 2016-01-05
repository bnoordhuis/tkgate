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

    Last edit by hansen on Mon Jan 19 18:17:52 2009
****************************************************************************/
#include "tkgate.h"

#define RSHIFT_IN 0
#define RSHIFT_OUT 1
#define RSHIFT_SHIFT 2

void Shift_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions rshift_iconDims[] = {
  {0, 21, 75, 20, 36, 9},
  {76, 0, 20, 75, 10, 37},
  {0,  0, 75, 20, 36, 9},
  {97, 0, 20, 75, 10, 37},
};
static int rshift_iconBoldOffset = 76;

GPadLoc rshifter_in_loc[] = {
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT},
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT}};

GPadLoc rshifter_out_loc[] = {
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT}};

GPadLoc rshifter_shift_loc[] = {
	{-37,0,-37,0,D_LEFT},
	{0,38,0,38,D_DOWN},
	{39,0,39,0,D_RIGHT},
	{0,-38,0,-38,D_UP}};

static char *psRshifter[] = {
  "%",
  "% A Rshifter",
  "/psrshift {",
  "  180 sub neg 360 mod neg",
  "  pslshift",
  "} def",
  0
};

GGateInfo gate_rshifter_info = {
  0,
  "RSHIFT",
  "rshift",0x0,
  "psrshift",psRshifter,
  -1,1,

  {{"S R",	{"gm.alu",0},		{"gm.alu.rshift",0,"shift",200},	"gat_make RSHIFT"},
   {0}},


  rshift_iconDims,

  3,{{"I",IN,8,1,rshifter_in_loc},
       {"Z",OUT,8,1,rshifter_out_loc},
       {"S",IN,3,1,rshifter_shift_loc}},
  {{12,-12,LJ},{12,-12,LJ},{12,-12,LJ},{12,-12,LJ}},
  {1},

  {"Dsz","Diz",0},
  
  Generic_Make,
  Shift_WriteCellDef,
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

void init_rshift()
{
  Pixmap P;

  P = Pixmap_find("shift");
  gateinfo_iconInit(&gate_rshifter_info,P,rshift_iconDims,rshift_iconBoldOffset);
  RegisterGate(&gate_rshifter_info);
}
