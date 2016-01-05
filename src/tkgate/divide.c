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

    Last edit by hansen on Mon Jan 19 18:15:00 2009
****************************************************************************/
#include "tkgate.h"

#define DIVIDE_A 0
#define DIVIDE_B 1
#define DIVIDE_Q 2
#define DIVIDE_R 3

static void Div_WriteCellDef(FILE *f,GCellSpec *gcs);


static iconDimensions divide_iconDims[] = {
  {0,  0,  59, 28, 29, 15},
  {60, 0,  28, 59, 15, 29},
  {29, 60, 59, 28, 28, 12},
  {0,  29, 28, 59, 12, 29},
};
static int divide_iconBoldOffset = 89;

GPadLoc divide_A_loc[] = {
	{-16,-16,-16,-16,D_UP},
	{-16,16,-16,16,D_LEFT},
	{16,16,16,16,D_DOWN},
	{16,-16,16,-16,D_RIGHT}};

GPadLoc divide_B_loc[] = {
	{16,-16,16,-16,D_UP},
	{-16,-16,-16,-16,D_LEFT},
	{-16,16,-16,16,D_DOWN},
	{16,16,16,16,D_RIGHT}};

GPadLoc divide_Q_loc[] = {
	{10,13,10,13,D_DOWN},
	{13,-10,13,-10,D_RIGHT},
	{-10,-13,-10,-13,D_UP},
	{-13,10,-13,10,D_LEFT}};

GPadLoc divide_R_loc[] = {
	{-10,13,-10,13,D_DOWN},
	{13,10,13,10,D_RIGHT},
	{10,-13,10,-13,D_UP},
	{-13,-10,-13,-10,D_LEFT}};

static char *psDivide[] = {
  "%",
  "% x y r psdiv",
  "%",
  "/psdiv {",
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
  "  0 3 1 0 360 arc closepath fill",
  "  0 -3 1 0 360 arc closepath fill",
  "  grestore",
  "} def",
  0
};


GGateInfo gate_divide_info = {
  0,
  "DIV",
  "div",0x0,
  "psdiv",psDivide,
  -1,2,

  {{"/",	{"gm.alu",0},		{"gm.alu.div",0,0,300},	"gat_make DIV"},
   {0}},

  divide_iconDims,

  4,{{"A",IN,8,1,divide_A_loc},
       {"B",IN,8,1,divide_B_loc},
       {"Q",OUT,8,1,divide_Q_loc},
       {"R",OUT,8,1,divide_R_loc}},
  {{24,10,LJ},{8,-24,LJ},{-24,-8,RJ},{-12,24,RJ}},
  {1},
  
  {"Dab_q","Dab_r",0},

  Generic_Make,
  Div_WriteCellDef,
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
 * Generate primitive cell definition for dividers.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Div_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  int invQ = 0,invR = 0;
  PrimParm primParm;

  if (*invSpec) {
    if (*invSpec == 'N')
      invQ = 1;
    if (!invSpec[1] || invSpec[1] == 'N')
      invR = 0;
  }

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"ABQR_RANGE",numBit);
  PrimParm_invSet(&primParm,"invQ",invQ);
  PrimParm_invSet(&primParm,"invR",invR);
  Primitive_write(f,"div",gcs,&primParm);
}

#if 0
static void Div_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  int negQ = 0,negR = 0;

  if (*invSpec) {
    if (*invSpec == 'N')
      negQ = 1;
    if (!invSpec[1] || invSpec[1] == 'N')
      negR = 0;
  }

  GCellSpec_writeBeginModule(f,gcs);

  fprintf(f,"input%s A,B;\n", bitrangeSpec(numBit));
  fprintf(f,"output%s Q,R;\n", bitrangeSpec(numBit));
  if (negQ) fprintf(f,"wire%s _Q;\n", bitrangeSpec(numBit));
  if (negR) fprintf(f,"wire%s _R;\n", bitrangeSpec(numBit));

  fprintf(f,"  assign #Dab_q %sQ = A / B;\n",(negQ ? "_" : ""));
  fprintf(f,"  assign #Dab_r %sR = A %% B;\n",(negR ? "_" : ""));
  if (negQ) fprintf(f,"  assign Q = ~ _Q;\n");
  if (negR) fprintf(f,"  assign R = ~ _R;\n");

  GCellSpec_writeEndModule(f,gcs);
}
#endif

void init_divide()
{
  Pixmap P;

  P = Pixmap_registerFromFile("divide","divide.b");
  gateinfo_iconInit(&gate_divide_info,P,divide_iconDims,divide_iconBoldOffset);
  RegisterGate(&gate_divide_info);
}
