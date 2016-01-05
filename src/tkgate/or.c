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

    Last edit by hansen on Mon Jan 19 18:16:59 2009
****************************************************************************/
#include "tkgate.h"

GCElement *AOX_Make(EditState **,GModuleDef *,int,int,int,int,const char *,int,const char**,int);
void AOX_Draw(GCElement *g,int md);
void AOX_AddInput(EditState *es,GCElement*);
void AOX_VerSave(FILE*,GCElement*);
void AOX_SetProp(GCElement*,const char*,const void*);
int AOX_EditProps(GCElement *g,int isLoadDialog);
void AOX_PSWrite(GPrint *P,GModLayout*,GCElement *g);
void AOX_RemovePort(EditState *es,GCElement *g,GWire *);
GCElement *AOX_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
void AOX_WriteCellDef(FILE *f,GCellSpec *gcs);

static iconDimensions or_iconDims[] = {
  {0, 0, 21, 15, 11, 7},
  {22, 0, 15, 21, 7, 9},
  {16, 22, 21, 15, 9, 7},
  {0, 16, 15, 21, 7, 11},
};
static int or_iconBoldOffset = 38;

extern GPadLoc and_out_loc[];
extern GPadLoc and_in_loc[];

static char *psOr[] = {
  "%",
  "% An OR gate",
  "%",
  "/psor {",
  "  startgate",
  "  -10 -7 moveto",
  "  -2 7 14 -90 -30 arc",
  "  -2 -7 14 30 90 arc",
  "  14 theight neg 11 sub 0 14 30 -30 arcn",
  "  closepath",
  "  stroke",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_or_info = {
  0,
  "OR",
  "or:nor",0x2,
  "psor",psOr,
  0,0,
  
  {{"o",	{"gm.gate",0},		{"gm.gate.or",0,0,200},	"gat_make OR"},
   {"O",	{"gm.gate",0},		{"gm.gate.nor",0,0,201},	"gat_make OR -invert Z"},
   {"Ctl-r o",	{"gm.rgate",0},		{"gm.rgate.or",0,0,200},	"gat_make OR -pins I=1"},
   {"Ctl-r O",	{"gm.rgate",0},		{"gm.rgate.nor",0,0,201},	"gat_make OR -pins I=1 -invert Z"},
   {0}},

  or_iconDims,

  2,{{"I",IN,1,2,and_in_loc,1},{"Z",OUT,1,1,and_out_loc,0}},
  {{0,-12,CT},{12,0,LJ},{0,-12,CT},{12,0,LJ}},
  {1},
  {"Diz",0},
  
  AOX_Make,
  AOX_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  AOX_Draw,
  Generic_Move,
  AOX_Copy,
  AOX_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  AOX_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  AOX_PSWrite,
  AOX_EditProps,
  AOX_VerSave,
  AOX_SetProp
};

void init_or()
{
  Pixmap P;

  P = Pixmap_registerFromFile("or","or.b");
  gateinfo_iconInit(&gate_or_info,P,or_iconDims,or_iconBoldOffset);
  RegisterGate(&gate_or_info);
}
