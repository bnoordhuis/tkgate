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

    Last edit by hansen on Mon Jan 19 18:13:48 2009
****************************************************************************/
#include "tkgate.h"

#define ARSHIFT_IN 0
#define ARSHIFT_OUT 1
#define ARSHIFT_SHIFT 2

void Shift_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions arshift_iconDims[] = {
  {0, 21, 75, 20, 36, 9},
  {76, 0, 20, 75, 10, 37},
  {0,  0, 75, 20, 36, 9},
  {97, 0, 20, 75, 10, 37},
};
static int arshift_iconBoldOffset = 76;


GPadLoc arshifter_in_loc[] = {
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT},
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT}};

GPadLoc arshifter_out_loc[] = {
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT}};

GPadLoc arshifter_shift_loc[] = {
	{-37,0,-37,0,D_LEFT},
	{0,38,0,38,D_DOWN},
	{39,0,39,0,D_RIGHT},
	{0,-38,0,-38,D_UP}};

static char *psArshifter[] = {
  "%",
  "% A Arshifter",
  "%",
  "/psarshift {",
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
  "  -7 2 moveto",
  "  3 2 lineto",
  "  3 5 lineto",
  "  9 0 lineto",
  "  3 -5 lineto",
  "  3 -2 lineto",
  "  -7 -2 lineto",
  "  closepath fill",
  "  -9 2 moveto -11 2 lineto",
  "  -11 -2 lineto -9 -2 lineto",
  "  closepath fill",
  "  -9 3 moveto",
  "  -9 6 lineto",
  "  -7 6 lineto",
  "",
  "  -7 4 lineto",
  "  -8 4 lineto",
  "  -6 3 lineto",
  "  -4 4 lineto",
  "  -5 4 lineto",
  "",
  "  -5 8 lineto",
  "  -11 8 lineto",
  "  -11 3 lineto",
  "  closepath fill",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_arshifter_info = {
  0,
  "ARSHIFT",
  "arshift",0x0,
  "psarshift",psArshifter,
  -1,1,

  {{"S A",	{"gm.alu",0},		{"gm.alu.arshift",0,"shift",300},	"gat_make ARSHIFT"},
   {0}},

  arshift_iconDims,

  3,{{"I",IN,8,1,arshifter_in_loc},
       {"Z",OUT,8,1,arshifter_out_loc},
       {"S",IN,3,1,arshifter_shift_loc}},
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

void Shift_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int numBits = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  int shiftSelBits = required_bits(numBits);
  const char *op = "arshift";
  PrimParm primParm;

  if (strcmp(gi->name,"RSHIFT") == 0)
    op = "rshift";
  else if (strcmp(gi->name,"LSHIFT") == 0)
    op = "lshift";

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"IZ_RANGE",numBits);
  PrimParm_rangeSet(&primParm,"S_RANGE",shiftSelBits);
  PrimParm_invSet(&primParm,"invZ",(*invSpec == 'N'));
  Primitive_write(f,op,gcs,&primParm);
}

void init_arshift()
{
  Pixmap P;

  P = Pixmap_registerFromFile("arshift","arshift.b");
  gateinfo_iconInit(&gate_arshifter_info,P,arshift_iconDims,arshift_iconBoldOffset);
  RegisterGate(&gate_arshifter_info);
}
