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
****************************************************************************/
#include "tkgate.h"

#define TRI_OUT 0

iconDimensions inout_iconDims[] = {
  {0,  8, 13,  7, 7, 3},
  {24, 0,  7, 13, 3, 7},
  {0,  8, 13,  7, 7, 3},
  {24, 0,  7, 13, 3, 7},
};
int inout_iconBoldOffset = 16;

GPadLoc inout_out_loc[] = {
	{6,0,6,0,D_RIGHT},
	{0,-8,0,-8,D_UP},
	{-8,0,-8,0,D_LEFT},
	{0,6,0,6,D_DOWN}};

static char *psTriio[] = {
  "/triiogate {",
  "  startgate",
  "  0 -3 moveto",
  "  6 0 lineto",
  "  0 3 lineto",
  "  -6 0 lineto",
  "  closepath",
  "  stroke",
  "  grestore",
  "} bind def",
  0
};

struct gateinfo gate_inout_info = {
  GC_LOGICTRI,
  "INOUT",
  "inout",0x0,
  "triiogate",psTriio,
  -1,-1,

  {{"=",	{"gm.mod",2},		{"gm.mod.inout",11,0,400},	"gat_make INOUT"},
   {0}},

  inout_iconDims,

  1,{{"Z",TRI,1,1,inout_out_loc}},
  {{-10,4,RJ},{0,15,CT},{10,4,LJ},{0,-10,CT}},
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
  InOut_VerSave
};

void init_inout()
{
  Pixmap P;

  P = Pixmap_find("inout");
  gateinfo_iconInit(&gate_inout_info,P,inout_iconDims,inout_iconBoldOffset);

  RegisterGate(&gate_inout_info);
}
