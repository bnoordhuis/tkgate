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

    Last edit by hansen on Fri Jan 30 20:27:13 2009
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "tkgate.h"

static char *psFrame[] = {
  "%",
  "% An frame gate",
  "%",
  "/psframe {",
  "  gsave",
  "  translate",
  "  pbox",
  "  [3 3] 0 setdash",
  "  stroke",
  "  [] 0 setdash",
  "  1 setgray 2 setlinewidth",
  "  10 0 moveto 10 add 0 rlineto stroke",
  "  0 setgray",
  "  grestore",
  "} bind def",
  0
};

GCElement *Frame_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char**,int);
void Frame_Delete(GCElement *g,GModuleDef *env,int drawp);
void Frame_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd);
int Frame_HitDistance(GCElement *g,int x,int y);
void Frame_Draw(GCElement *g,int md);
void Frame_PSWrite(GPrint *P,GModLayout *L,GCElement *g);
void Frame_VerSave(FILE *f,GCElement *g);
int Frame_EditProps(GCElement *g,int isLoadDialog);
void Frame_SetProp(GCElement*,const char*,const void*);
GCElement *Frame_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned);
void Frame_Move(GCElement *g,int x,int y);
void Frame_Rotate(GCElement *g,int centX, int centY,int rdir);

GGateInfo gate_frame_info = {
  0,
  "FRAME",
  "frame",0x0,
  "psframe",psFrame,
  -1,-1,

  {{"F",	{0,0},			{"gm.frame",0,0},		"gat_make FRAME"},
   {0}},
  0,

  0,{{0}},
  {{0,-12,CT},{12,0,LJ},{0,-12,CT},{12,0,LJ}},	/* Are these even used? */
  {0,0,0,0,1},
  
  {0},

  Frame_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Frame_Delete,
  Frame_GetExtents,
  Frame_HitDistance,
  Frame_Draw,
  Frame_Move,
  Frame_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Frame_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Frame_PSWrite,
  Frame_EditProps,
  Frame_VerSave,
  Frame_SetProp
};

GCElement *Frame_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char **options,int nOptions)
{
  GCElement *g;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWire,options,nOptions)))
    return NULL;

  ob_touch(g);

  g->u.frame.width = g->u.frame.height = 100;
  g->u.frame.text = ob_strdup("");
  g->u.frame.html = 0;

  return g;
}

static void Frame_flushHtml(GCElement *g)
{
  if (g->u.frame.html) {
    delete_Html(g->u.frame.html);
    g->u.frame.html = 0;
  }
}

void Frame_Delete(GCElement *g,GModuleDef *M,int drawp)
{
  if (M)     gate_remove(M,g);
  if (drawp) gate_draw(g,0);

  if (g->u.frame.text)
    ob_free(g->u.frame.text);

  Frame_flushHtml(g);
}

void Frame_Move(GCElement *g,int x,int y)
{
  ob_touch(g);

  if (g->top && g->left) {
    if (g->u.frame.height - y < MINSIZE)
      y = g->u.frame.height - MINSIZE;
    g->u.frame.height -= y;
    g->ypos += y;

    if (g->u.frame.width - x < MINSIZE)
      x = g->u.frame.width - MINSIZE;
    g->u.frame.width -= x;
    g->xpos += x;

  } else if (g->bottom && g->left) {
    if (g->u.frame.height + y < MINSIZE)
      y = MINSIZE - g->u.frame.height;
    g->u.frame.height += y;

    if (g->u.frame.width - x < MINSIZE)
      x = g->u.frame.width - MINSIZE;
    g->u.frame.width -= x;
    g->xpos += x;

  } else if (g->top && g->right) {
    if (g->u.frame.height - y < MINSIZE)
      y = g->u.frame.height - MINSIZE;
    g->u.frame.height -= y;
    g->ypos += y;

    if (g->u.frame.width + x < MINSIZE)
      x = MINSIZE - g->u.frame.width;
    g->u.frame.width += x;
  } else if (g->bottom && g->right) {
    if (g->u.frame.height + y < MINSIZE)
      y = MINSIZE - g->u.frame.height;
    g->u.frame.height += y;

    if (g->u.frame.width + x < MINSIZE)
      x = MINSIZE - g->u.frame.width;
    g->u.frame.width += x;
  } else {
    g->xpos += x;
    g->ypos += y;
  }
}


void Frame_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  *minx = g->xpos - 2;
  *miny = g->ypos - 2;
  *maxx = g->xpos + g->u.frame.width + 2;
  *maxy = g->ypos + g->u.frame.height + 2;

  if (bd) *bd = 20;
}

/*
  Frames can only be selected on edges or corners
 */
int Frame_HitDistance(GCElement *g,int x,int y)
{
  ob_touch(g);

  g->left = 0;
  g->right = 0;
  g->top = 0;
  g->bottom = 0;

  if ((x > g->xpos - 10) &&
      (x < g->xpos + g->u.frame.width + 10) &&
      (y > g->ypos - 10) &&
      (y < g->ypos + g->u.frame.height + 10)) {
    if (x < g->xpos + 10) {
      g->left = 1;
    }
    if (x > g->xpos + g->u.frame.width - 10) {
      g->right = 1;
    }
    if (y < g->ypos + 10) {
      g->top = 1;
    }
    if (y > g->ypos + g->u.frame.height - 10) {
      g->bottom = 1;
    }

    if (g->left || g->right || g->top || g->bottom)
      return GATERANGE-1;
  }
  return NOHIT; 			/* out of range */
}


GCElement *Frame_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  ng = Generic_Copy(M,g,x,y,flags);

  ob_touch(ng);

  ng->u.frame.width = g->u.frame.width;
  ng->u.frame.height = g->u.frame.height;
  ng->u.frame.text = g->u.frame.text ? ob_strdup(g->u.frame.text) : 0;
  ng->u.frame.html = 0;

  return ng;
}

static void Frame_buildHtml(GCElement *g)
{
  Html *h;

  if (g->u.frame.html
      && g->u.frame.html->h_zoom == TkGate.circuit->zoom_factor
      && g->u.frame.html->h_locale == TkGate.circuit->c_locale)
    return;

  ob_begin_framef("-BuildHtml",FF_TRANSPARENT);
  ob_touch(g);

  Frame_flushHtml(g);

  if (*g->u.frame.text) {
    h = new_Html(TD_X11);
    Html_addLine(h, g->u.frame.text);
    Html_format(h);
    g->u.frame.html = h;
  } else
    g->u.frame.html = 0;

  ob_end_frame();
}

void Frame_Draw(GCElement *g,int md)
{
  Frame_buildHtml(g);

  if (g->selected)
    XSetLineAttributes(TkGate.D,TkGate.frameGC,2,LineOnOffDash,CapButt,JoinMiter);

  if (!g->u.frame.html) {
    ZDrawRectangle(TkGate.D,TkGate.W,TkGate.frameGC,
		   ctow_x(g->xpos),ctow_y(g->ypos),
		   g->u.frame.width,g->u.frame.height);
  } else {
    XPoint points[6];
    int x = ctow_x(g->xpos);
    int y = ctow_y(g->ypos);
    int w,h;

    w = g->u.frame.html->h_width;
    h = g->u.frame.html->h_height;
    Html_draw(g->u.frame.html,g->xpos+15,g->ypos-h/2);

    if (w+20 < g->u.frame.width) {
      points[0].x = x+w+20;
      points[0].y = y;
      points[1].x = x+g->u.frame.width;
      points[1].y = y;
      points[2].x = x+g->u.frame.width;
      points[2].y = y+g->u.frame.height;
      points[3].x = x;
      points[3].y = y+g->u.frame.height;
      points[4].x = x;
      points[4].y = y;
      points[5].x = x+10;
      points[5].y = y;
      ZDrawLines(TkGate.D,TkGate.W,TkGate.frameGC,points,6,CoordModeOrigin);
    } else {
      points[0].x = x+g->u.frame.width;
      points[0].y = y+8;
      points[1].x = x+g->u.frame.width;
      points[1].y = y+g->u.frame.height;
      points[2].x = x;
      points[2].y = y+g->u.frame.height;
      points[3].x = x;
      points[3].y = y;
      points[4].x = x+10;
      points[4].y = y;
      ZDrawLines(TkGate.D,TkGate.W,TkGate.frameGC,points,5,CoordModeOrigin);
    }


  }

  if (g->selected)
    XSetLineAttributes(TkGate.D,TkGate.frameGC,1,LineOnOffDash,CapButt,JoinMiter);
}

void Frame_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  int width = 0;
  int height = 0;

  if (*g->u.frame.text) {
    Html *h = new_Html(TD_PRINT);
    Html_addLine(h,g->u.frame.text);
    Html_format(h);
    width = h->h_width;
    height = h->h_height;
    fprintf(P->p_f,"%d %d %d %d %d psframe\n",width,g->u.frame.width,g->u.frame.height,g->xpos,g->ypos);
    Html_psPrint(h,P,g->xpos+15,g->ypos-height/2);
    delete_Html(h);
  } else
    fprintf(P->p_f,"0 %d %d %d %d psframe\n",g->u.frame.width,g->u.frame.height,g->xpos,g->ypos);
}

void Frame_VerSave(FILE *f,GCElement *g)
{
  char buf[STRMAX],buf2[STRMAX];
  Encoder *encoder = Circuit_getSaveFileEncoder(TkGate.circuit);

  fprintf(f,"  //: frame %s",g->ename);
  VerilogBasicGateComment(f,g,0);

  recodeText(encoder,buf,STRMAX,g->u.frame.text);
  fprintf(f," /wi:%d /ht:%d /tx:\"%s\"\n",g->u.frame.width,g->u.frame.height,
	  quoteChars(buf2,buf,"\"\\"));
}

int Frame_EditProps(GCElement *g,int isLoadDialog)
{
  char buf[STRMAX];

  Generic_EditProps(g,isLoadDialog);
  if (isLoadDialog) {
    quoteChars(buf,g->u.frame.text,"\"[]\\");
    DoTcl("set ::edgat_frameLabel \"%s\"",buf);
  } else {
    strcpy(buf,Tcl_GetVar(TkGate.tcl,"edgat_frameLabel",TCL_GLOBAL_ONLY));

    ob_touch(g);

    if (g->u.frame.text)
      ob_free(g->u.frame.text);
    g->u.frame.text = ob_strdup(buf);
    Frame_flushHtml(g);
  }

  return 0;
}

void Frame_SetProp(GCElement *g,const char *prop,const void *value)
{
  ob_touch(g);

  if (strcmp(prop,"/wi") == 0) {
    int n = *(int*)value;
    g->u.frame.width = n;
  } else if (strcmp(prop,"/ht") == 0) {
    int n = *(int*)value;
    g->u.frame.height = n;
  } else if (strcmp(prop,"/tx") == 0) {
    if (g->u.frame.text)
      ob_free(g->u.frame.text);
    g->u.frame.text = ob_strdup((char*)value);;
    Frame_flushHtml(g);
  }
}

void Frame_Rotate(GCElement *g,int centX, int centY,int rdir)
{
  int x = g->xpos + g->u.frame.width/2;
  int y = g->ypos + g->u.frame.height/2;
  int nx,ny,t;

  ob_touch(g);
  nx = rotateX(x - centX,y - centY, rdir) + centX;
  ny = rotateY(x - centX,y - centY, rdir) + centY;

  t = g->u.frame.width;
  g->u.frame.width = g->u.frame.height;
  g->u.frame.height = t;

  g->xpos = nx - g->u.frame.width/2;
  g->ypos = ny - g->u.frame.height/2;

}

void init_frame()
{
  RegisterGate(&gate_frame_info);
}
