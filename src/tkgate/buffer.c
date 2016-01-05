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

    Last edit by hansen on Mon Jan 19 18:13:58 2009
****************************************************************************/
#include "tkgate.h"

#define BUFFER_IN 0
#define BUFFER_OUT 1

static void Buffer_WriteCellDef(FILE *f,GCellSpec *gcs);


iconDimensions buf_iconDims[] = {
  {0, 0, 15, 15, 5, 7},
  {16, 0, 15, 15, 7, 9},
  {16, 16, 15, 15, 9, 7},
  {0,16, 15, 15, 7, 5},
};
int buf_iconBoldOffset = 32;

GPadLoc buffer_in_loc[] = {
	{-6,0,-6,0,D_LEFT},
	{0,6,0,6,D_DOWN},
	{6,0,6,0,D_RIGHT},
	{0,-6,0,-6,D_UP}};

GPadLoc buffer_out_loc[] = {
	{10,0,10,0,D_RIGHT},
	{0,-10,0,-10,D_UP},
	{-10,0,-10,0,D_LEFT},
	{0,10,0,10,D_DOWN}};


char *psBuffer[] = {
  "%",
  "% A Buffer",
  "/psbuf {",
  "  startgate",
  "  -5.5 -7.5 moveto",
  "  9.5 0 lineto",
  "  -5.5 7.5 lineto",
  "  closepath",
  "  stroke",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_buf_info = {
  0,
  "BUF",
  "buf:not",0x2,
  "psbuf",psBuffer,
  -1,0,

  {{"b",	{"gm.gate",0},		{"gm.gate.buf",0,0,400},	"gat_make BUF"},
   {"I",	{"gm.gate",0},		{"gm.gate.inv",0,0,401},	"gat_make BUF -invert Z"},
   {0}},

  buf_iconDims,

  2,{{"I",IN,1,1,buffer_in_loc},
       {"Z",OUT,1,1,buffer_out_loc}},
  {{0,-12,CT},{12,0,LJ},{0,-12,CT},{12,0,LJ}},
  {1},
  
  {"Diz",0},

  Generic_Make,
  Buffer_WriteCellDef,
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
* Generate primitive cell definition for buffer.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Buffer_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  int numBit = gcs->gc_numBits;
  const char *invSpec = gcs->gc_invSpec;
  PrimParm primParm;

  PrimParm_init(&primParm);
  PrimParm_rangeSet(&primParm,"IZ_RANGE",numBit);
  PrimParm_invSet(&primParm,"invZ",(*invSpec == 'N'));
  Primitive_write(f,"buf",gcs,&primParm);
}

void init_buffer()
{
  Pixmap P;
  P = Pixmap_registerFromFile("buf","buf.b");
  gateinfo_iconInit(&gate_buf_info,P,buf_iconDims,buf_iconBoldOffset);
  RegisterGate(&gate_buf_info);
}
