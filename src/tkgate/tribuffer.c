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

#define TRIBUFFER_IN 0
#define TRIBUFFER_OUT 1
#define TRIBUFFER_ENABLE 2

static void BufIf_WriteCellDef(FILE *f,GCellSpec *gcs);

extern iconDimensions buf_iconDims[];
extern int buf_iconBoldOffset;

GPadLoc bufif_in_loc[] = {
	{-6,0,-6,0,D_LEFT},
	{0,6,0,6,D_DOWN},
	{6,0,6,0,D_RIGHT},
	{0,-6,0,-6,D_UP}};

GPadLoc bufif_out_loc[] = {
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-10,0,-10,0,D_LEFT},
	{0,10,0,10,D_DOWN}};

GPadLoc bufif_enable_loc[] = {
	{2,-5,2,-5,D_UP},
	{-5,-2,-5,-2,D_LEFT},
	{-2,5,-2,5,D_DOWN},
	{5,2,5,2,D_RIGHT}};


GGateInfo gate_bufif_info = {
  0,
  "BUFIF",
  "bufif1:notif1:bufif0:notif0",0x5 ,
  "psbuf",0,
  -1,0,

  {{"t",	{"gm.gate",0},		{"gm.gate.tribuf",0,0,500},	"gat_make BUFIF"},
   {0}
  },

  buf_iconDims,

  3,{{"Z",TRI,1,1,bufif_out_loc},
     {"I",IN,1,1,bufif_in_loc},
     {"E",IN,1,1,bufif_enable_loc}},
  {{0,18,CT},{10,0,LJ},{0,-12,CT},{-8,8,RJ}},
  {1},
  
  {"Dez","Diz",0},

  Generic_Make,
  BufIf_WriteCellDef,
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
 * Generate primitive cell definition for bufifs.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void BufIf_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  PrimParm primParm;

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"IZ_RANGE",numBit);
  PrimParm_intSet(&primParm,"IZ_BITS",numBit);
  PrimParm_invSet(&primParm,"invZ",(*invSpec == 'N'));
  Primitive_write(f,"tribuf",gcs,&primParm);
}

void init_bufif()
{
  Pixmap P;

  P = Pixmap_find("buf");
  gateinfo_iconInit(&gate_bufif_info,P,buf_iconDims,buf_iconBoldOffset);

  RegisterGate(&gate_bufif_info);
}
