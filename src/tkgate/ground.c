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

    Last edit by hansen on Mon Jan 19 18:15:49 2009
****************************************************************************/
#include "tkgate.h"

static iconDimensions ground_iconDims[] = {
  {0, 0, 11, 11, 5, 5},
  {12, 0, 11, 11, 5, 5},
  {12, 12, 11, 11, 5, 5},
  {0, 12, 11, 11, 5, 5},
};
static int ground_iconBoldOffset = 24;

GPadLoc ground_out_loc[] = {
  {0, -6, 0, -6, D_UP},
  {-6, 0, -6, 0, D_LEFT},
  {0, 6, 0, 6, D_DOWN},
  {6, 0, 6, 0, D_RIGHT},
};

static char *psGround[] = {
  "%",
  "% x y r ground",
  "%",
  "/psground {",
  "  startgate",
  "  -4 6 moveto 4 6 lineto stroke",
  "  -3 4 moveto 3 4 lineto stroke",
  "  -2 2 moveto 2 2 lineto stroke",
  "  -1 0 moveto 1 0 lineto stroke",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_ground_info = {
  0,
  "GROUND",
  "supply0",0x0,
  "psground",psGround,
  -1,-1,

  {{"g",	{"gm.io",0},	{"gm.io.ground",0,0,300},	"gat_make GROUND"},
   {0}},

  ground_iconDims,

  1,{{"Z",OUT,1,1,ground_out_loc}},
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

void init_ground()
{
  Pixmap P;

  P = Pixmap_registerFromFile("ground","ground.b");
  gateinfo_iconInit(&gate_ground_info,P,ground_iconDims,ground_iconBoldOffset);
  RegisterGate(&gate_ground_info);
}
