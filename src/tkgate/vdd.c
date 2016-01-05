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

static iconDimensions vdd_iconDims[] = {
  {0,  0, 35, 8, 19, 4},
  {36, 0, 8, 35, 4, 15},
  {8, 36, 35, 8, 15, 3},
  {0,  9, 8, 35, 3, 19},
};
static int vdd_iconBoldOffset = 45;

GPadLoc vdd_out_loc[] = {
  {-11, 0, -11, 0, D_DOWN},
  {0, 11, 0, 11, D_RIGHT},
  {11, 0, 11, 0, D_UP},
  {0, -11, 0, -11, D_LEFT}
};


static char *psVdd[] = {
  "%",
  "% x y r vdd",
  "%",
  "/psvdd {",
  "  startgate",
  "  -19 0 moveto -3 0 lineto stroke",
  "  10 rfont",
  "  -1 -4 moveto (V) show",
  "  8 rfont",
  "  0 -3 rmoveto (dd) show",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_vdd_info = {
  0,
  "VDD",
  "supply1",0x0,
  "psvdd",psVdd,
  -1,-1,

  {{"v",	{"gm.io",0},	{"gm.io.vdd",0,0,400},	"gat_make VDD"},
   {0}},

  vdd_iconDims,

  1,{{"Z",OUT,1,1,vdd_out_loc}},
  {{5,15,LJ},{5,15,LJ},{5,15,LJ},{5,15,LJ}},
  {1,1,1},
  
  {0},

  Generic_Make,
  Nop_WriteCellDef,
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
  InOut_VerSave    
};

void init_vdd()
{
  Pixmap P;

  P = Pixmap_registerFromFile("vdd","vdd.b");
  gateinfo_iconInit(&gate_vdd_info,P,vdd_iconDims,vdd_iconBoldOffset);
  RegisterGate(&gate_vdd_info);
}
