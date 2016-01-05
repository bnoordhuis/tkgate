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

    Last edit by hansen on Mon Jan 19 18:17:05 2009
****************************************************************************/
#include "tkgate.h"

#define LOGICOUT_IN 0

extern iconDimensions in_iconDims[];
extern int in_iconBoldOffset;

GPadLoc out_in_loc[] = {
	{3,0,3,0,D_LEFT},
	{0,-3,0,-3,D_DOWN},
	{-3,0,-3,0,D_RIGHT},
	{0,3,0,3,D_UP}};

static char *psOut[] = {
  "%",
  "% An output pin",
  "%",
  "/psout {",
  "  startgate",
  "  -6 -3 moveto",
  "  3 0 lineto",
  "  -6 3 lineto",
  "  stroke",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_out_info = {
  GC_LOGICOUT,
  "OUT",
  "output",0x0,
  "psout",psOut,
  -1,-1,

  {{"{",	{"gm.mod",2},		{"gm.mod.out",7,0,300},	"gat_make OUT"},
   {0}},

  in_iconDims,		/* Same dimensions as 'in' */

  1,{{"I",IN,1,1,out_in_loc}},
  {{8,4,LJ},{0,-7,CT},{-8,4,RJ},{0,16,CT}},
  {1,1,1},
  
  {0},

  InOut_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  InOut_Draw,
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
  InOut_PSWrite,
  Generic_EditProps,
  InOut_VerSave,
};

void init_out()
{
  Pixmap P;

  P = Pixmap_find("inout");
  gateinfo_iconInit(&gate_out_info,P,in_iconDims,in_iconBoldOffset);

  RegisterGate(&gate_out_info);
}
