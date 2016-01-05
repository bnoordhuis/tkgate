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

    Last edit by hansen on Mon Jan 19 18:16:29 2009
****************************************************************************/
#include "tkgate.h"

#define LSHIFT_IN 0
#define LSHIFT_OUT 1
#define LSHIFT_SHIFT 2

void Shift_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions lshift_iconDims[] = {
  {0,  0, 75, 20, 36, 9},
  {97, 0, 20, 75, 10, 37},
  {0, 21, 75, 20, 36, 9},
  {76, 0, 20, 75, 10, 37},
};
static int lshift_iconBoldOffset = 76;


GPadLoc lshifter_in_loc[] = {
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT},
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT}};

GPadLoc lshifter_out_loc[] = {
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT}};

GPadLoc lshifter_shift_loc[] = {
	{-37,0,-37,0,D_LEFT},
	{0,38,0,38,D_DOWN},
	{39,0,39,0,D_RIGHT},
	{0,-38,0,-38,D_UP}};

static char *psLshifter[] = {
  "%",
  "% A Lshift",
  "/pslshift {",
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
  "  7 2 moveto",
  "  -3 2 lineto",
  "  -3 5 lineto",
  "  -9 0 lineto",
  "  -3 -5 lineto",
  "  -3 -2 lineto",
  "  7 -2 lineto",
  "  closepath fill",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_lshifter_info = {
  0,
  "LSHIFT",
  "lshift",0x0,
  "pslshift",psLshifter,
  -1,1,

  {{"S L",	{"gm.alu",0},		{"gm.alu.lshift",0,"shift",100},	"gat_make LSHIFT"},
   {0}},


  lshift_iconDims,

  3,{{"I",IN,8,1,lshifter_in_loc},
       {"Z",OUT,8,1,lshifter_out_loc},
       {"S",IN,3,1,lshifter_shift_loc}},
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

void init_lshift()
{
  Pixmap P;

  P = Pixmap_registerFromFile("shift","shift.b");
  gateinfo_iconInit(&gate_lshifter_info,P,lshift_iconDims,lshift_iconBoldOffset);
  RegisterGate(&gate_lshifter_info);
}
