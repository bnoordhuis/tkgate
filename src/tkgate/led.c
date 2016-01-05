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

    Last edit by hansen on Sat Sep 26 14:30:18 2009
****************************************************************************/
#include "tkgate.h"

#define LED_IN 0

#define MAXDIGITS	12

#define LT_BASIC	0
#define LT_BAR		1
#define LT_HEX		2
#define LT_DEC		3
#define LT_SEG		4

static iconDimensions led_iconDims[] = {
  {0,  0,  13, 14, 6, 7},
  {14, 0,  14, 13, 7, 6},
  {15, 14, 13, 14, 6, 6},
  {0,  15, 14, 13, 6, 6},
};
static int led_iconBoldOffset = 29;

GCElement *Led_Make(EditState **,GModuleDef *,int,int,int,int,const char *,int,const char**,int);
void Led_GetExtents(GCElement *g,TargetDev_e,int *minx,int *miny,int *maxx,int *maxy,int *bd);
void Led_Draw(GCElement *g,int md);
void Led_VerSave(FILE*,GCElement*);
void Led_SetProp(GCElement*,const char*,const void*);
int Led_EditProps(GCElement *g,int isLoadDialog);
void Led_PSWrite(GPrint *P,GModLayout*,GCElement *g);
GCElement *Led_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);

GPadLoc led_in_loc[] = {		/* Control line */
 {0,7,0,7,D_DOWN},
 {7,0,7,0,D_RIGHT},
 {0,-7,0,-7,D_UP},
 {-7,0,-7,0,D_LEFT}};

static char *psLed[] = {
  "%",
  "% An LED (bit)",
  "%",
  "/psled_bit {",
  "  startgate",
  "  -5 -7 moveto",
  "  5 -7 lineto",
  "  5 0 lineto",
  "  0 0 5 0 180 arc",
  "  closepath",
  "  gsave .8 setgray fill grestore",
  "  stroke",
  "  grestore",
  "} bind def",
  "%",
  "% Single bar graph segment",
  "%",
  "/psled_barseg {",
  "  gsave",
  "  3 -13 translate",
  "  3 10 pbox ",
  "  gsave .8 setgray fill grestore",
  "  stroke",
  "  grestore",
  "} def",
  "%",
  "% An LED (bar graph)",
  "%",
  "/psled_bar {",
  "  startgate",
  "  neg 0 0 4 2 roll box",
  "  1 1 3 -1 roll { psled_barseg 5 0 translate } for",
  "  grestore",
  "} bind def",
  "%",
  "% Single 7-segment digit",
  "%",
  "/psled_7segdig {",
  "  gsave",
  "  2 setlinewidth",
  "  0 -27 translate",
  "  5 0 17 0 line",
  "  5 11 17 11 line",
  "  5 22 17 22 line",
  "",
  "  4 1 4 10 line",
  "  18 1 18 10 line",
  "  4 12 4 21 line",
  "  18 12 18 21 line",
  "  grestore",
  "} def",
  "%",
  "% An LED (7 segment)",
  "%",
  "/psled_7seg {",
  "  startgate",
  "  neg 0 0 4 2 roll box",
  "  1 1 3 -1 roll { psled_7segdig 20 0 translate } for",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_led_info = {
  GC_LED,
  "LED",
  "led",0x0,
  "psled",psLed,
  -1,0,

 { {"l",	{"gm.io",0},		{"gm.io.led",0,"led",100},	"gat_make LED -type basic"},
   {"L 1",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 1"},
   {"L 2",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 2"},
   {"L 3",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 3"},
   {"L 4",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 4"},
   {"L 5",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 5"},
   {"L 6",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 6"},
   {"L 7",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 7"},
   {"L 8",	{"gm.io",0},		{"gm.io.ledbar",0,"led",200},	"gat_make LED -type bar -bits 8"},
   {"L 9",	{0,0},			{0,0,0,0},			"gat_make LED -type bar -bits 9"},
   {"L h",	{"gm.io",0},		{"gm.io.ledhex",0,"led",300},	"gat_make LED -type hex -bits 8"},
   {"L d",	{"gm.io",0},		{"gm.io.leddec",0,"led",400},	"gat_make LED -type dec -bits 8"},
   {"L s",	{"gm.io",0},		{"gm.io.ledseg",0,"led",500},	"gat_make LED -type seg -bits 7"},
   {0}},

  led_iconDims,

  1,{{"I",IN,1,1,led_in_loc,0}},
  {{0,-12,CT},{-12,3,RJ},{0,18,CT},{12,3,LJ}},
  {1,1,0,0,0},
  
  {0},

  Led_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Generic_Delete,
  Led_GetExtents,
  Generic_HitDistance,
  Led_Draw,
  Generic_Move,
  Led_Copy,
  Generic_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Led_PSWrite,
  Led_EditProps,
  Led_VerSave,
  Led_SetProp
};


Icon *inled_icon[8];	/* Special icons for 'light' part of led. */
Icon *seg_icon[7];	/* Icons for 7-segment display */

GCElement *Led_Make(EditState **es,GModuleDef *env,int GType,
			int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions);
  const char *LType,*NBits;
  int n;

  ob_touch(g);

  g->u.led.ltype = LT_BASIC;
  g->u.led.value = 0;
  g->u.led.valid = ~0;
  n = 1;

  LType = seekOption("-type",options,nOptions);
  NBits = seekOption("-bits",options,nOptions);

  if (LType) {
    if (strcmp(LType,"basic") == 0)
      g->u.led.ltype = LT_BASIC;
    else if (strcmp(LType,"hex") == 0)
      g->u.led.ltype = LT_HEX;
    else if (strcmp(LType,"dec") == 0)
      g->u.led.ltype = LT_DEC;
    else if (strcmp(LType,"bar") == 0)
      g->u.led.ltype = LT_BAR;
    else if (strcmp(LType,"seg") == 0)
      g->u.led.ltype = LT_SEG;
  }

  if (NBits) {
    sscanf(NBits,"%d",&n);
    if (n < 1) n = 1;
    if (n > 32) n = 32;
  }

  if (!noWires)
    net_setSize(g->wires[LED_IN]->net,n);

  return g;
}

/*
 * Returns number of decimal digits required to display an n-bit number.
 */
static unsigned decDigits(unsigned n)
{

  unsigned m;

  if (n < 32)
    m = (1 << n) - 1;
  else
    m = 0xffffffff;

  n = 0;
  while (m > 0) {
    m /= 10;
    n++;
  }
  if (!n) n = 1;

  return n;
} 

/*
 * Returns number of 7-seg digits.
 */
static unsigned numDigits(GCElement *g)
{
  unsigned n = g->wires[LED_IN]->net->n_nbits;

  switch (g->u.led.ltype) {
  case LT_HEX : 
  default :
    n = (n+3)/4;
    break;
  case LT_SEG : 
    n = (n+6)/7;
    break;
  case LT_DEC : 
    n = decDigits(n);
    break;
  }

  return n;
}

/*
 *    _1
 *  2| |3
 *    -4
 *  5| |6
 *    _7
 */
static void getLedSegments(GCElement *g,unsigned *seg_value,unsigned *seg_valid,unsigned *n)
{
#define S(n) (1 << (n-1))
  static unsigned seg_data[] = {
    S(1)|S(2)|S(3)|S(5)|S(6)|S(7),		/* 0 */
    S(3)|S(6),					/* 1 */
    S(1)|S(3)|S(4)|S(5)|S(7),			/* 2 */ 
    S(1)|S(3)|S(4)|S(6)|S(7),			/* 3 */ 
    S(2)|S(4)|S(3)|S(6),			/* 4 */
    S(1)|S(2)|S(4)|S(6)|S(7),			/* 5 */
    S(1)|S(2)|S(4)|S(5)|S(6)|S(7),		/* 6 */
    S(1)|S(3)|S(6),				/* 7 */
    S(1)|S(2)|S(3)|S(4)|S(5)|S(6)|S(7),		/* 8 */
    S(1)|S(2)|S(3)|S(4)|S(6),			/* 9 */
    S(1)|S(2)|S(3)|S(4)|S(5)|S(6),		/* A */
    S(2)|S(4)|S(5)|S(6)|S(7),			/* b */
    S(1)|S(2)|S(5)|S(7),			/* C */
    S(3)|S(4)|S(5)|S(6)|S(7),			/* d */
    S(1)|S(2)|S(4)|S(5)|S(7),			/* E */
    S(1)|S(2)|S(4)|S(5),			/* F */
  };
#undef S
  int nb = g->wires[LED_IN]->net->n_nbits;
  unsigned valid = g->u.led.valid;
  unsigned value = g->u.led.value;
  int i;

  /*
   * 0 extend if necessary
   */
  if (nb < 32) {
    value &= (1<<nb)-1;
    valid |= ~((1<< nb)-1);
  }

  *n = numDigits(g);

  i = *n - 1;
  switch (g->u.led.ltype) {
  case LT_HEX : 
  default :
    for (i = *n-1;i >= 0;i--) {
      unsigned a = value & 0xf;
      unsigned b = valid & 0xf;

      if (b == 0xf) {
	seg_value[i] = seg_data[a];
	seg_valid[i] = 0x7f;
      } else {
	seg_value[i] = 0;
	seg_valid[i] = 0;
      }

      valid >>= 4;
      value >>= 4;
    }
    break;
  case LT_SEG : 
    for (i = *n-1;i >= 0;i--) {
      unsigned a = value & 0x7f;
      unsigned b = valid & 0x7f;

      seg_value[i] = a;
      seg_valid[i] = b;

      valid >>= 7;
      value >>= 7;
    }
    break;
  case LT_DEC : 
    if (valid == 0xffffffff) {
      for (i = *n-1;i >= 0;i--) {
	unsigned a = value % 10;

	seg_value[i] = seg_data[a];
	seg_valid[i] = 0x7f;

	value /= 10;
      }
    } else {
      for (i = *n-1;i >= 0;i--) {
	seg_value[i] = 0;
	seg_valid[i] = 0;
      }
    }
    break;
  }
}

void Led_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  switch (g->u.led.ltype) {
  case LT_BASIC : 
  default :
    Generic_GetExtents(g,target,minx,miny,maxx,maxy,bd);
    break;
  case LT_BAR : 
    {
      int x = 0,y = 0,w,h,n;

      n = g->wires[LED_IN]->net->n_nbits;
      h = 15;
      w = 3 + 5*n;

      switch (g->orient) {
      case D_RIGHT :
	x = g->xpos - w/2;
	y = g->ypos - 9;
	break;
      case D_UP :
	x = g->xpos - w + 6;
	y = g->ypos - 7 ;
	break;
      case D_LEFT :
	x = g->xpos - w/2;
	y = g->ypos - 6;
	break;
      case D_DOWN :
	x = g->xpos - 6 ;
	y = g->ypos - 7;
	break;
      }

      if (bd) *bd = 10;
      if (minx) *minx = x;
      if (miny) *miny = y;
      if (maxx) *maxx = x + w;
      if (maxy) *maxy = y + h;
      break;
    }
  case LT_HEX : 
  case LT_SEG : 
  case LT_DEC : 
    {
      int x = 0,y = 0,w,h;
      unsigned n;

      n = numDigits(g);
      h = 31;
      w = 3 + 20*n;

      switch (g->orient) {
      case D_RIGHT :
	x = g->xpos - w/2;
	y = g->ypos - h + 6;
	break;
      case D_UP :
	x = g->xpos - w + 6;
	y = g->ypos - h/2 ;
	break;
      case D_LEFT :
	x = g->xpos - w/2;
	y = g->ypos - 6;
	break;
      case D_DOWN :
	x = g->xpos - 6;
	y = g->ypos - h/2;
	break;
      }

      if (bd) *bd = 10;
      if (minx) *minx = x;
      if (miny) *miny = y;
      if (maxx) *maxx = x + w;
      if (maxy) *maxy = y + h;
      break;
    }

  }
}

void led_drawname(GCElement *g,char *ename,int ax,int ay,int w,int h)
{
  int x,y;

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textXF[TkGate.circuit->zoom_factor]->fid);

  x = ax+w/2;

  if (g->orient == 2) {
    if (g->u.led.ltype != LT_BAR)
      y = ay+h+22;
    else
      y = ay+h+12;
  } else
    y = ay-5;

  dce_DrawString(TkGate.instGC,x,y,CT,ename);
}

void led_PSdrawname(GPrint *P,GCElement *g,char *ename,int ax,int ay,int w,int h)
{
  int x,y;
  HtmlFont font[1];

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textXF[TkGate.circuit->zoom_factor]->fid);

  x = ax+w/2;

  if (g->orient == 2) {
    if (g->u.led.ltype != LT_BAR)
      y = ay+h+22;
    else
      y = ay+h+12;
  } else
    y = ay-5;

  PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),
	     x,y,ename,BetweenLeftAndRight|AtBaseline);
}

static void Led_Draw_basic(GCElement *g,int md)
{
  int idx;

  Generic_Draw(g,md);

  if (!(g->u.led.valid & 0x1)) {
    Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledunknown_pixel);
  } else if (g->u.led.value) {
    Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledon_pixel);
  } else {
    Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledoff_pixel);
  }

  idx = g->orient;
  if (g->selected) idx += 4;
  Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(g->xpos),ctow_y(g->ypos),inled_icon[idx]);

  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.inst_pixel);
}

static void Led_Draw_bar(GCElement *g,int md)
{
  int i,n,x,y,x2,y2,w,h,dx;

  n = g->wires[LED_IN]->net->n_nbits;

  Led_GetExtents(g,TD_X11,&x,&y,&x2,&y2,0);
  w = x2-x; 
  h = y2-y; 

  if (g->show_name && g->ename)
    led_drawname(g,g->ename,x,y,w,h);

  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x),ctow_y(y),w,h);
  if (g->selected)
    ZDrawRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x)+1,ctow_y(y)+1,w-2,h-2);

  x += 3;
  y += 3;

  /*
   * Draw all of the unknown bits
   */
  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledunknown_pixel);
  dx = 0;
  for (i = 0;i < n;i++) {
    if (!(g->u.led.valid & (1 << (n-i-1)))) {
      ZFillRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx),ctow_y(y),3,10);
    }
    dx += 5;
  }

  /*
   * Draw all of the on bits
   */
  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledon_pixel);
  dx = 0;
  for (i = 0;i < n;i++) {
    if ((g->u.led.valid & (1 << (n-i-1)) && (g->u.led.value & (1 << (n-i-1))))) {
      ZFillRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx),ctow_y(y),3,10);
    }
    dx += 5;
  }

  /*
   * Draw all of the off bits
   */
  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledoff_pixel);
  dx = 0;
  for (i = 0;i < n;i++) {
    if ((g->u.led.valid & (1 << (n-i-1)) && !(g->u.led.value & (1 << (n-i-1))))) {
      ZFillRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx),ctow_y(y),3,10);
    }
    dx += 5;
  }

  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.inst_pixel);
  gate_drawWires(g,md);
}


void Led_Draw_7segs(GCElement *g,int md)
{
  int i,j,x,y,x2,y2,w,h,dx;
  unsigned seg_value[MAXDIGITS];
  unsigned seg_valid[MAXDIGITS];
  unsigned n;
  char *seg_type = 0;

  Led_GetExtents(g,TD_X11,&x,&y,&x2,&y2,0);
  w = x2-x; 
  h = y2-y; 

  if (g->show_name && g->ename)
    led_drawname(g,g->ename,x,y,w,h);

  getLedSegments(g,seg_value,seg_valid,&n);

  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x),ctow_y(y),w,h);
  if (g->selected)
    ZDrawRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+1),ctow_y(y+1),w-2,h-2);

  dx = 0;
  for (i = 0;i < n;i++, dx += 20)
    ZDrawRectangle(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx+3),ctow_y(y+3),17,25);


  /*
    Draw the type indicator for 7-seg LEDs
  */
  switch (g->u.led.ltype) {
  case LT_HEX : 
    seg_type = "16";
    break;
  case LT_SEG : 
    seg_type = "D";
    break;
  case LT_DEC : 
    seg_type = "10";
    break;
  }

  if (seg_type) {
    if (g->selected)
      XSetFont(TkGate.D,TkGate.instGC,TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
    else
      XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);
    dce_DrawString(TkGate.instGC,x+w/2+5,y+40,LJ,seg_type);
  }

  dx = 0;
  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledunknown_pixel);
  for (i = 0;i < n;i++, dx += 20) {
    for (j = 0;j < 7;j++) {
      if (!(seg_valid[i] & (1<<j)))
	Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx+5),ctow_y(y+5),seg_icon[j]);
    }
  }

  dx = 0;
  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledon_pixel);
  for (i = 0;i < n;i++, dx += 20) {
    for (j = 0;j < 7;j++) {
      if ((seg_valid[i] & (1<<j)) && (seg_value[i] & (1<<j)))
	Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx+5),ctow_y(y+5),seg_icon[j]);
    }
  }

  dx = 0;
  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.ledoff_pixel);
  for (i = 0;i < n;i++, dx += 20) {
    for (j = 0;j < 7;j++) {
      if ((seg_valid[i] & (1<<j)) && !(seg_value[i] & (1<<j)))
	Icon_draw(TkGate.D,TkGate.W,TkGate.instGC,ctow_x(x+dx+5),ctow_y(y+5),seg_icon[j]);
    }
  }

  Tkg_changeColor(TkGate.instGC, GXxor, TkGate.inst_pixel);
  gate_drawWires(g,md);
}


void Led_Draw(GCElement *g,int md)
{
  if (tkgate_currentMode() != MM_SIMULATE) {
    g->u.led.valid = 0xffffffff;
    g->u.led.value = 0;
  }

  switch (g->u.led.ltype) {
  case LT_BASIC : 
    Led_Draw_basic(g,md);
    break;
  case LT_BAR :
    Led_Draw_bar(g,md);
    break;
  default :
    Led_Draw_7segs(g,md);
    break;
  }
}

GCElement *Led_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  ng = Generic_Copy(M,g,x,y,flags);
  ob_touch(ng);

  ng->u.led.ltype = g->u.led.ltype;
  ng->u.led.value = g->u.led.value;
  ng->u.led.valid = g->u.led.value;

  return ng;
}


void Led_VerSave(FILE *f,GCElement *g)
{
  fprintf(f,"  //: %s %s (%s)"
	  ,g->typeinfo->name
	  ,g->ename
	  ,g->wires[0]->net->n_signame);
  VerilogBasicGateComment(f,g,0);
  fprintf(f," /type:%d",g->u.led.ltype);
  fprintf(f,"\n");

#if 0
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);
  fprintf(f," /type:%d",g->u.led.ltype);
  fprintf(f,"\n");
#endif
}

int Led_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    DoTcl("set ::edgat_ledType %d",g->u.led.ltype);
  } else {
    const char *p;

    if ((p = Tcl_GetVar(tcl,"edgat_ledType",TCL_GLOBAL_ONLY))) {
      ob_touch(g);
      sscanf(p,"%d",&g->u.led.ltype);
    }
  }
  return 0;
}

void Led_SetProp(GCElement *g,const char *prop,const void *value)
{
  if (strcmp(prop,"/type") == 0) {
    int n = *((int*)value);
    ob_touch(g);
    g->u.led.ltype = n;
  }
}

void Led_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  int n;
  int x,y,x2,y2,w,h;
  HtmlFont font[1];

  Led_GetExtents(g,TD_PRINT,&x,&y,&x2,&y2,0);
  w = x2-x; 
  h = y2-y; 

  switch (g->u.led.ltype) {
  case LT_BASIC : 
  default :
    Generic_PSLabels(P,g);
    fprintf(P->p_f,"%d %d %d psled_bit\n",g->xpos,g->ypos,-g->orient*90);
    break;
  case LT_BAR : 
    if (g->show_name && g->ename)
      led_PSdrawname(P,g,g->ename,x,y,w,h);

    n = g->wires[LED_IN]->net->n_nbits;
    fprintf(P->p_f,"%d %d %d %d %d 0 psled_bar\n",n,w,h,x,y);
    break;
  case LT_HEX : 
  case LT_SEG : 
  case LT_DEC : 
    if (g->show_name && g->ename)
      led_PSdrawname(P,g,g->ename,x,y,w,h);
    n = numDigits(g);
    fprintf(P->p_f,"%d %d %d %d %d 0 psled_7seg\n",n,w,h,x,y);
    break;
  }

  switch (g->u.led.ltype) {
  case LT_HEX : 
    PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),x+w/2+3,y+30,"16",AtLeft|AtTop);
    break;
  case LT_SEG : 
    PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),x+w/2+3,y+30,"D",AtLeft|AtTop);
    break;
  case LT_DEC : 
    PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),x+w/2+3,y+30,"10",AtLeft|AtTop);
    break;
  }
}


void init_led()
{
  Pixmap P;
  int i;
  char buf1[STRMAX],buf2[STRMAX];
  iconDimensions *id = led_iconDims;

  P = Pixmap_registerFromFile("led","led.b");
  gateinfo_iconInit(&gate_led_info,P,led_iconDims,led_iconBoldOffset);
  RegisterGate(&gate_led_info);

  for (i = 0;i < 7;i++) {
    sprintf(buf1,"ledseg%d",i+1);
    sprintf(buf2,"ledseg%d.b",i+1);
    P = Pixmap_registerFromFile(buf1,buf2);
    seg_icon[i] = new_Icon(P,0,0,13,21,0,0);
  }

  /*
   * Load special pixmap for leds.
   */
  P = Pixmap_registerFromFile("ledinside","ledinside.b");
  for (i = 0;i < 4;i++) {
    inled_icon[i] = new_Icon(P,id[i].x,id[i].y,id[i].w,id[i].h,id[i].ox,id[i].oy);
    inled_icon[i+4] = new_Icon(P,id[i].x,id[i].y+led_iconBoldOffset,id[i].w,id[i].h,id[i].ox,id[i].oy);
  }

}
