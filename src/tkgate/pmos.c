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

    Last edit by hansen on Mon Jan 19 18:17:11 2009
****************************************************************************/
#include "tkgate.h"

#define PMOS_IN 0
#define PMOS_OUT 1

void Mos_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions pmos_iconDims[] = {
  {0, 0, 14, 18, 7, 9},
  {0, 19, 18, 14, 9, 7},
  {19, 15, 14, 18, 7, 7},
  {15, 0, 18, 14, 7, 7},
};
static int pmos_iconBoldOffset = 34;

GPadLoc pmos_data_loc[] ={		/* Tr. data */
 {6,-8,6,-8,D_UP},
 {-7,-8,-7,-8,D_LEFT},
 {-8,8,-8,8,D_DOWN},
 {8,7,8,7,D_RIGHT}};

GPadLoc pmos_out_loc[] = {		/* Tr. out  */
 {6,9,6,9,D_DOWN},
 {8,-8,8,-8,D_RIGHT},
 {-8,-7,-8,-7,D_UP},
 {-7,7,-7,7,D_LEFT}};

GPadLoc pmos_gate_loc[] = {		/* Tr. gate */
 {-8,0,-8,0,D_LEFT},
 {0,7,0,7,D_DOWN},
 {7,0,7,0,D_RIGHT},
 {0,-8,0,-8,D_UP}};

static char *psPmos[] = {
  "%",
  "% An PMOS gate",
  "%",
  "/pspmos {",
  "  [[0 1][1 -1][-1 0][1 1]] adjstartgate",
  "  -8 9 moveto",
  "  -8 -8 lineto stroke",
  "  6.5 8.5 moveto",
  "  -4 8.5  lineto",
  "  -4 -7.5 lineto",
  "  6.5 -7.5 lineto stroke",
  "  closepath fill",
  "  -3.5 8.5 moveto",
  "  -0.5 11 lineto",
  "  -0.5 6 lineto",
  "  closepath fill",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_pmos_info = {
  0,
  "PMOS",
  "pmos",0x0,
  "pspmos",psPmos,
  -1,0,

  {{"Ctl-t p",	{"gm.gate",0},		{"gm.gate.pmos",0,"tr"},		"gat_make PMOS"},
   {0}
  },
  pmos_iconDims,

  3,{
    {"Z",OUT,1,1,pmos_out_loc,0},
    {"S",IN,1,1,pmos_data_loc,0},
    {"G",IN,1,1,pmos_gate_loc,0}},
  {{10,4,LJ},{0,-12,CT},{-10,4,RJ},{0,18,CT}},
  {1},
  
  {"Diz","Dgz",0},

  Generic_Make,
  Mos_WriteCellDef,
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
  Generic_VerSave,
};

void init_pmos()
{
  Pixmap P;

  P = Pixmap_registerFromFile("pmos","pmos.b");
  gateinfo_iconInit(&gate_pmos_info,P,pmos_iconDims,pmos_iconBoldOffset);
  RegisterGate(&gate_pmos_info);
}
