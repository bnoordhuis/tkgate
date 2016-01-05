/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

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

    Last edit by hansen on Mon Feb  2 23:19:11 2009
****************************************************************************/
#include "tkgate.h"


#define REG_Q 0
#define REG_D 1
#define REG_EN 2
#define REG_CLR 3
#define REG_CK 4

static void Register_WriteCellDef(FILE *f,GCellSpec *gcs);


static iconDimensions reg_iconDims[] = {
  {0,  0, 75, 20, 36, 9},
  {97, 0, 20, 75, 10, 37},
  {0, 21, 75, 20, 36, 9},
  {76, 0, 20, 75, 10, 37},
};
static int reg_iconBoldOffset = 76;

GPadLoc register_in_loc[] = {
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT},
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT}};

GPadLoc register_out_loc[] = {
	{0,11,0,11,D_DOWN},
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-11,0,-11,0,D_LEFT}};

GPadLoc register_ck_loc[] = {
	{-37,0,-37,0,D_LEFT},
	{0,38,0,38,D_DOWN},
	{39,0,39,0,D_RIGHT},
	{0,-38,0,-38,D_UP}};

GPadLoc register_en_loc[] = {
	{39,5,39,5,D_RIGHT},
	{5,-38,5,-38,D_UP},
	{-37,-5,-37,-5,D_LEFT},
	{-5,38,-5,38,D_DOWN}};

GPadLoc register_clr_loc[] = {
	{39,-5,39,-5,D_RIGHT},
	{-5,-38,-5,-38,D_UP},
	{-37,5,-37,5,D_LEFT},
	{5,38,5,38,D_DOWN}};


static char *psRegister[] = {
  "%",
  "% A Register",
  "/register {",
  "  [[0 0][-1 1][2 1][0 -1]] adjstartgate",
  "  -36.5 -10.5 moveto",
  "  -36.5 9.5 lineto",
  "  38.5 9.5 lineto",
  "  38.5 -10.5 lineto",
  "  closepath stroke",
  "  -36.5 -7 moveto",
  "  -30 0 lineto",
  "  -36.5 7 lineto stroke",
  "  7 rfont",
  "  (_CLR) dup stringwidth pop neg 36 add 0 prshow",
  "  (_EN) dup stringwidth pop neg 36 add -9 prshow",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_register_info = {
  GC_REGISTER,
  "REG",
  "register",0x0,
  "register",psRegister,
  -1,0,

  {{"r",	{"gm.mem",5},		{"gm.mem.reg",0,0,100},	"gat_make REG"},
   {0}},


  reg_iconDims,

  5,{{"Q",OUT,8,1,register_out_loc},
     {"D",IN,8,1,register_in_loc},
     {"EN",IN,1,1,register_en_loc},
     {"CLR",IN,1,1,register_clr_loc},
     {"CK",IN,1,1,register_ck_loc}},
  {{0,3,CT},{13,-5,LJ},{0,3,CT},{13,-5,LJ}},
  {1},

  {"Dsetup","Dhold","Dck_q",0},
  
  Generic_Make,
  Register_WriteCellDef,
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
 * Generate primitive cell definition for registers.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Register_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  int invQ = 0;
  PrimParm primParm;

  if (*invSpec == 'N')
    invQ = 1;

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"DQ_RANGE",numBit);
  PrimParm_intSet(&primParm,"DQ_BITS",numBit);
  PrimParm_invSet(&primParm,"invQ",invQ);
  Primitive_write(f,"register",gcs,&primParm);
}

void init_register()
{
  Pixmap P;

  P = Pixmap_registerFromFile("reg","reg.b");
  gateinfo_iconInit(&gate_register_info,P,reg_iconDims,reg_iconBoldOffset);
  RegisterGate(&gate_register_info);
}
