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

    Last edit by hansen on Mon Jan 19 18:16:51 2009
****************************************************************************/
#include "tkgate.h"

#define NMOS_IN 0
#define NMOS_OUT 1

void Mos_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions nmos_iconDims[] = {
  {0, 0, 14, 18, 7, 7},
  {0, 19, 18, 14, 7, 7},
  {19, 15, 14, 18, 7, 9},
  {15, 0, 18, 14, 9, 7},
};
static int nmos_iconBoldOffset = 34;

GPadLoc nmos_out_loc[] ={		/* Tr. out */
 {6,-8,6,-8,D_UP},
 {-7,-8,-7,-8,D_LEFT},
 {-8,8,-8,8,D_DOWN},
 {8,7,8,7,D_RIGHT}};

GPadLoc nmos_data_loc[] = {		/* Tr. data */
 {6,9,6,9,D_DOWN},
 {8,-8,8,-8,D_RIGHT},
 {-8,-7,-8,-7,D_UP},
 {-7,7,-7,7,D_LEFT}};

GPadLoc nmos_gate_loc[] = {		/* Tr. gate */
 {-8,0,-8,0,D_LEFT},
 {0,7,0,7,D_DOWN},
 {7,0,7,0,D_RIGHT},
 {0,-8,0,-8,D_UP}};

static char *psNmos[] = {
  "%",
  "% An NMOS gate",
  "%",
  "/psnmos {",
  "  [[0 0][0 -1][-1 1][0 -1]] adjstartgate",
  "  -8 8 moveto",
  "  -8 -9 lineto stroke",
  "  6.5 7.5 moveto",
  "  -4 7.5  lineto",
  "  -4 -8.5 lineto",
  "  6.5 -8.5 lineto stroke",
  "  6 -8.5 moveto",
  "  2 -11 lineto",
  "  2 -6 lineto",
  "  closepath fill",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_nmos_info = {
  0,
  "NMOS",
  "nmos",0x0,
  "psnmos",psNmos,
  -1,0,

  {{"Ctl-t n",	{"gm.gate",0},		{"gm.gate.nmos",0,"tr"},		"gat_make NMOS"},
   {0}
  },

  nmos_iconDims,

  3,{
    {"Z",OUT,1,1,nmos_out_loc,0},
    {"S",IN,1,1,nmos_data_loc,0},
    {"G",IN,1,1,nmos_gate_loc,0}},
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

/*****************************************************************************
 *
 * Generate primitive cell definition for Mos gates.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
void Mos_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int numBit = gcs->gc_numBits;
  const char *op =0;
  int inv = (*gcs->gc_invSpec == 'N');
  PrimParm primParm;

  if (strcmp(gi->name,"PMOS") == 0)
    op = "pmos_trans";
  else
    op = "nmos_trans";

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"SGZ_RANGE",numBit);
  PrimParm_invSet(&primParm,"invZ",inv);
  Primitive_write(f,op,gcs,&primParm);
}

void init_nmos()
{
  Pixmap P;

  P = Pixmap_registerFromFile("nmos","nmos.b");
  gateinfo_iconInit(&gate_nmos_info,P,nmos_iconDims,nmos_iconBoldOffset);
  RegisterGate(&gate_nmos_info);
}
