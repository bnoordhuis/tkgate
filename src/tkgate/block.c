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

    Last edit by hansen on Sun Feb 22 16:55:03 2009
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tkgate.h"

#define BLOCK_STUBLEN	15


#define NUMSQRS 10
#define NUMSTEPS 15

#define on_edge(x,y,bx1,by1,bx2,by2) (bx1 == bx2 ? \
   ((abs((bx1)-(x)) < 10) && ((y) > (by1)) && ((y) < (by2))) : \
   ((abs((by1)-(y)) < 10) && ((x) > (bx1)) && ((x) < (bx2))))

GPadLoc block_right_loc[]  = {{0,0,0,0,D_RIGHT}};
GPadLoc block_top_loc[]    = {{0,0,0,0,D_UP}};
GPadLoc block_left_loc[]   = {{0,0,0,0,D_LEFT}};
GPadLoc block_bottom_loc[] = {{0,0,0,0,D_DOWN}};

extern int baderp;
extern int startrekp;
extern int GlobalElementCount;

GCElement *Block_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char **options,int nOptions);
int Block_HitDistance(GCElement *g,int x,int y);
void Block_Init(GCElement *g);
void Block_Move(GCElement *g,int x,int y);
int Block_SimHitFunc(EditState*,GCElement*);

void Block_Delete(GCElement *g,GModuleDef *env,int drawp);
void Block_Draw(GCElement *g,int md);
void Block_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd);
GCElement *Block_Copy(GModuleDef *,GCElement *g,int x,int y,unsigned flags);
int Block_EditProps(GCElement *g,int isLoadDialog);
void Block_VerSave(FILE*,GCElement*);
void Block_Rotate(GCElement*,int,int,int rdir);
int Block_HitDistance(GCElement*g,int,int);
void Block_AddInput(EditState *es,GCElement *g);
void Block_AddOutput(EditState *es,GCElement *g);
void Block_AddTri(EditState *es,GCElement *g);
void Block_ChangePin(EditState *es,GCElement *g);
void Block_PSWrite(GPrint *P,GModLayout*,GCElement *g);
GWireNode *Block_wireSnap(GCElement *g,GWire *w,int *mod,int retry);

void Block_setParmPort(GCElement *g,const char *parmName,const char *newValue);
void Block_unsetParmPort(GCElement *g,const char *parmName);
const char *Block_getParmPort(GCElement *g,const char *name);


/*
     ANY TRI IN  OUT
     <>  <>  <   >	Right
     ^v  ^v  v   ^      Up
     <>  <>  >   <	Left
     ^v  ^v  v   ^	Down
*/

GGateInfo gate_block_info = {
  GC_BLOCK,
  "MODULE",
  "block",0x0,
  "barf",0,
  -1,-1,

  {{"B",	{"gm.mod",2},		{"gm.mod.inst",0,0,100},	"gat_make MODULE"},
   {0}},

  0,

  12,{{"Ti",IN,0,0,block_top_loc},
	{"Li",IN,0,0,block_left_loc},
	{"Bi",IN,0,0,block_bottom_loc},
	{"Ri",IN,0,0,block_right_loc},
	{"To",OUT,0,0,block_top_loc},
	{"Lo",OUT,0,0,block_left_loc},
	{"Bo",OUT,0,0,block_bottom_loc},
	{"Ro",OUT,0,0,block_right_loc},
	{"Tt",TRI,0,0,block_top_loc},
	{"Lt",TRI,0,0,block_left_loc},
	{"Bt",TRI,0,0,block_bottom_loc},
	{"Rt",TRI,0,0,block_right_loc}},
  {{0,0,CT},{0,0,CT},{0,0,CT},{0,0,CT}},
  {0,1,0,0,1},

  {0},
  
  Block_Make,
  Nop_WriteCellDef,
  Block_Init,
  Block_Delete,
  Block_GetExtents,
  Block_HitDistance,
  Block_Draw,
  Block_Move,
  Block_Copy,
  Block_AddInput,
  Block_AddOutput,
  Block_AddTri,
  Block_Rotate,
  Err_RemovePort,
  Block_ChangePin,
  Nop_SimInitFunc,
  Block_SimHitFunc,
  Block_PSWrite,
  Block_EditProps,
  Block_VerSave,
  0,
  0,
  Block_wireSnap
};


/*
   Replicate the wire list of a block
*/
static GWire *block_repwirelist(GModuleDef *M,GCElement *g,GWire *w,int nx,int ny,int x,int y)
{
  GWire *nw;
  GWire *cw;
  GWire *oew;

  nw = NULL;
  for (;w;w = w->next) {
    if (M) {
      GWire *w1,*w2;

      wire_new(M,&w1,&w2);
      cw = w->nodes->in ? w2 : w1;
      oew = (!w->nodes->in) ? w2 : w1;
    } else {
      GWire *w1,*w2;

      wire_new(NULL,&w1,&w2);
      cw = w->nodes->in ? w2 : w1;
      oew = (!w->nodes->in) ? w2 : w1;
    }
    ob_touch(cw);
    ob_touch(cw->nodes);
    ob_touch(cw->net);
    ob_touch(oew);

    cw->orient = w->orient;
    cw->gate = g;

    cw->net->n_dtype = w->net->n_dtype;
    net_setSize(cw->net,w->net->n_nbits);

    cw->offset = w->offset;
    oew->wtype = cw->wtype = w->wtype;
    cw->nodes->x = w->nodes->x + nx - x;
    cw->nodes->y = w->nodes->y + ny - y;
    cw->next = nw;
    nw = cw;
    cw->name = ob_strdup(w->name);

    block_canonicalizewire(g,cw);

    cw = w->nodes->out ? cw->nodes->out->end : cw->nodes->in->end;
    ob_touch(cw->nodes);

    if (cw->nodes->x == nw->nodes->x)
      cw->nodes->y = nw->nodes->y > cw->nodes->y ? nw->nodes->y - BLOCK_STUBLEN : nw->nodes->y + BLOCK_STUBLEN;
    else
      cw->nodes->x = nw->nodes->x > cw->nodes->x ? nw->nodes->x - BLOCK_STUBLEN : nw->nodes->x + BLOCK_STUBLEN;

    wire_finalizeNet(cw);
  }
  return nw;
}

static void block_replicatewires(GCElement *ng,GCElement *g,GModuleDef *M)
{
  int N = GCElement_numPads(g);
  int i;

  ob_touch(ng);
  for (i = 0;i < N;i++)
    ng->wires[i] =  block_repwirelist(M,ng,g->wires[i],
				      ng->xpos,ng->ypos,g->xpos,g->ypos);
}

/*
Creates a duplicated copy of block g.  (Assuming g is a block)
*/
GCElement *Block_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  ng = gate_new(x,y,g->orient,g->typeinfo->Code);
  if (M)
    gate_add(M,ng);

  ob_touch(ng);
  ng->u.block.gheight = g->u.block.gheight;
  ng->u.block.gwidth = g->u.block.gwidth;
  ng->orient = g->orient;

  if (g->ename)
    gate_setName(ng,g->ename,M);
  ng->u.block.moduleName = ob_strdup(g->u.block.moduleName);
  ng->enumb = GlobalElementCount++;
  ng->show_name = g->show_name;

  if (!(flags & REP_NOWIRES))
    block_replicatewires(ng,g,M);

  return ng;
}

GCElement *Block_Make(EditState **es,GModuleDef *PM,int GType,
		      int x,int y,int r,const char *Name,int noWire,
		      const char **options,int nOptions)
{
  GCElement *g,*copy;
  const char *Func,*Size,*DoInit;
  int W,H;

  if (!(g = Generic_Make(es,PM,GType,x,y,r,Name,noWire,options,nOptions)))
    return NULL;

  Func = seekOption("-func",options,nOptions);		/* The block function */
  Size = seekOption("-size",options,nOptions);		/* The initial size */
  DoInit = seekOption("-doinit",options,nOptions);	/* Should we automatically copy bock description? */


  ob_touch(g);

  if (Func) {
    g->u.block.moduleName = ob_strdup(Func);

    /*
     * This option is used by the drag-and-drop module creation feature.
     */
    if (DoInit && *DoInit == '1') {
      GModuleDef *M;

      env_checkname(g);
      M = env_findModule(g->u.block.moduleName); 

      copy = env_getInterface(g);			/* Get copy of the block description */

      if (copy) {
	if (GCElement_getType(copy) == GC_SYMBLOCK) {
	  g = SymBlock_convert(g,copy->u.block.symbol,PM,1);
	} else {					/* Standard block */
	  g->u.block.gwidth = copy->u.block.gwidth;
	  g->u.block.gheight = copy->u.block.gheight;
	  g->xpos -= g->u.block.gwidth/2;
	  g->ypos -= g->u.block.gheight/2;
	  block_replicatewires(g,copy,(*es)->env);
	}
      }
    }
  } else if (es) {
    GGateInfo *gi = g->typeinfo;
    int ok;
    const char *temp;
    GModuleDef *M;

    g->selected = 1;
    g->top = 0;
    g->bottom = 0;
    g->right = 0;
    g->left = 0;

    g->u.block.moduleName = ob_strdup("");
    gate_draw(g,GD_NORMAL);

    Tcl_SetVar(TkGate.tcl,"edgat_newBlock","1",TCL_GLOBAL_ONLY);
    (*gi->EditProps)(g,1);
    DoTcl("EditGate::post MODULE");
    if ((temp = Tcl_GetVar(TkGate.tcl,"edgat_ok",TCL_GLOBAL_ONLY)) && sscanf(temp,"%d",&ok) == 1 && ok)
      (*gi->EditProps)(g,0);
    else {
      gate_delete(g,(*es)->env,1);
      return 0;
    }
    Tcl_SetVar(TkGate.tcl,"edgat_newBlock","0",TCL_GLOBAL_ONLY);
  
    env_checkname(g);

    M = env_findModule(g->u.block.moduleName); 

    copy = env_getInterface(g);
    if (copy) {
      if (GCElement_getType(copy) == GC_SYMBLOCK) {
	g = SymBlock_convert(g,copy->u.block.symbol,PM,1);
      } else {						/* Standard block */
	g->u.block.gwidth = copy->u.block.gwidth;
	g->u.block.gheight = copy->u.block.gheight;
	g->xpos -= g->u.block.gwidth/2;
	g->ypos -= g->u.block.gheight/2;
	block_replicatewires(g,copy,(*es)->env);
      }
    }
    gate_draw(g,GD_NORMAL);

    if (PM->m_isSpecial)
      SetModified(MF_MODULE|MF_SYNCONLY);
    else
      SetModified(MF_MODULE);
  } else {
    g->u.block.moduleName = 0;
  }

  if (Size && sscanf(Size,"%dx%d",&W,&H) == 2) {
    g->u.block.gwidth = W;
    g->u.block.gheight = H;
  }
 
  return g;
}

GWire *block_hitPort(GCElement *g,int x,int y)
{
  GWire *best_w = 0;
  int best_dist = MAXPORTRANGE+1;
  int N = GCElement_numPads(g);
  int i;

  for (i = 0;i < N;i++) {
    GWire *w;

    for (w = g->wires[i];w;w = w->next) {
      int d = distance(x,y,w->nodes->x,w->nodes->y);

      if (d < best_dist) {
	best_dist = d;
	best_w = w;
      }
    }
  }

  if (best_w) {
    /*
     * If a corner is closer than a port, don't select the port.
     */
    if (distance(g->xpos                    ,g->ypos                     ,x,y) < best_dist) best_w = 0;
    if (distance(g->xpos + g->u.block.gwidth,g->ypos                     ,x,y) < best_dist) best_w = 0;
    if (distance(g->xpos                    ,g->ypos + g->u.block.gheight,x,y) < best_dist) best_w = 0;
    if (distance(g->xpos + g->u.block.gwidth,g->ypos + g->u.block.gheight,x,y) < best_dist) best_w = 0;
  }

  return best_w;
}

int Block_HitDistance(GCElement *g,int x,int y)
{
  if ((x > g->xpos - 10) &&
      (x < g->xpos + g->u.block.gwidth + 10) &&
      (y > g->ypos - 10) &&
      (y < g->ypos + g->u.block.gheight + 10)) {

    return GATERANGE-1;		/* Hit on smaller objects if overlaping */
  } else
    return NOHIT;		/* Out of range */
}

/*****************************************************************************
 *
 * Check to see if (x,y) is on the edge of g, ad return the edge.
 *
 * Parameters:
 *      g		Gate to test
 *      x,y		Coordinates to test.
 *
 *****************************************************************************/
int Block_HitEdge(GCElement *g,int x,int y)
{
  int dx_left = x-g->xpos;
  int dx_right = x-(g->xpos + g->u.block.gwidth);
  int dy_top = y-g->ypos;
  int dy_bottom = y-(g->ypos + g->u.block.gheight);

  dx_left = abs(dx_left);
  dx_right = abs(dx_right);
  dy_top = abs(dy_top);
  dy_bottom = abs(dy_bottom);

  /*
   * Ensure that (x,y) is inside the box 
   */
  if (x < g->xpos || x > g->xpos+g->u.block.gwidth)
    return PSIDE_UNKNOWN;
  if (y < g->ypos || y > g->ypos+g->u.block.gheight)
    return PSIDE_UNKNOWN;

  if (dx_left <= 10 && dx_left <= dy_top && dx_left <= dy_bottom)
    return PSIDE_LEFT;
  else if (dx_right <= 10 && dx_right <= dy_top && dx_right <= dy_bottom)
    return PSIDE_RIGHT;
  else if (dy_top <= 10)
    return PSIDE_TOP;
  else if (dy_bottom <= 10)
    return PSIDE_BOTTOM;

  return PSIDE_UNKNOWN;
}

int Block_HitComponent(GCElement *g,int x,int y)
{
  GWire *w;

  ob_touch(g);
  g->left = 0;
  g->right = 0;
  g->top = 0;
  g->bottom = 0;

  w = block_hitPort(g,x,y); 
  if (w && !TkGate.circuit->wsel) {

    TkGate.circuit->wsel = w; 
    return GATERANGE-2;
  }

  ob_touch(g);
  g->left = 0;
  g->right = 0;
  g->top = 0;
  g->bottom = 0;

  if ((x > g->xpos - 10) &&
      (x < g->xpos + g->u.block.gwidth + 10) &&
      (y > g->ypos - 10) &&
      (y < g->ypos + g->u.block.gheight + 10)) {
    if (x < g->xpos + 10) {
      g->left = 1;
    }
    if (x > g->xpos + g->u.block.gwidth - 10) {
      g->right = 1;
    }
    if (y < g->ypos + 10) {
      g->top = 1;
    }
    if (y > g->ypos + g->u.block.gheight - 10) {
      g->bottom = 1;
    }
    return GATERANGE-1;		/* Hit on smaller objects if overlaping */
  } else
    return NOHIT;		/* Out of range */
}

void Block_Init(GCElement *g)
{
  ob_touch(g);
  g->u.block.gwidth = MINSIZE;
  g->u.block.gheight = MINSIZE;
}

/*
    Draw the names and I/O direction arrows for one 'pad' of
    a logic block.
*/
void block_drawnamelist(GWire *w,int s,int d,int iod)
{
  int p,x,y;
  XFontStruct *F;

#if 0
  printf("block_drawnamelist(%s, %d, %d, %d)\n",w ? w->name : "(nil)",s,d,iod);
#endif

  if (s) {
    XSetFont(TkGate.D,TkGate.modportGC, TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
    F = TkGate.stextbXF[1];
  } else {
    XSetFont(TkGate.D,TkGate.modportGC, TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);
    F = TkGate.stextXF[1];
  }


  for (;w;w = w->next) {
    if (w->name) {
      block_getwirexy(w,d,&x,&y,&p);
      RelPosDrawString(TkGate.W,F,TkGate.modportGC,x,y,w->name,p);
    }
    DrawPinIOMark(w,d,iod,IODT_PORT);
  }
}

void Block_Delete(GCElement *g,GModuleDef *PM,int drawp)
{
  Generic_Delete(g,PM,drawp);
}

void Block_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  *minx = g->xpos;
  *miny = g->ypos;
  *maxx = g->xpos + g->u.block.gwidth;
  *maxy = g->ypos + g->u.block.gheight;

  if (bd) *bd = 30;
}

/* Draw a logic block */
void Block_Draw(GCElement *g,int md)
{
  int N = GCElement_numPads(g);
  int i;
  
  ZDrawRectangle(TkGate.D,TkGate.W,TkGate.moduleGC,
		 ctow_x(g->xpos),ctow_y(g->ypos),
		 g->u.block.gwidth,g->u.block.gheight);

  for (i = 0;i < N;i++) {
    GWire *w = g->wires[i];

    if (md != GD_NOWIRE)
      wire_drawlist(w,(md == GD_NOINWIRE) ? g : 0);
    block_drawnamelist(g->wires[i],g->selected,
		       g->typeinfo->Pad[i].Loc[0].dir,
		       g->typeinfo->Pad[i].iotype);
  }
  
  if (g->selected) {
    ZDrawRectangle(TkGate.D,TkGate.W,TkGate.moduleGC,
		   ctow_x(g->xpos+1),ctow_y(g->ypos+1),
		   g->u.block.gwidth-2,g->u.block.gheight-2);
  }

  
  if (g->ename && g->show_name) {
    char B[STRMAX];

    if (g->selected)
      XSetFont(TkGate.D,TkGate.moduleGC, TkGate.stextbXF[TkGate.circuit->zoom_factor]->fid);
    else
      XSetFont(TkGate.D,TkGate.moduleGC, TkGate.stextXF[TkGate.circuit->zoom_factor]->fid);

    sprintf(B,"(%s)",g->ename);
    dce_DrawString(TkGate.moduleGC,g->xpos + (g->u.block.gwidth / 2),
		   g->ypos + (g->u.block.gheight / 2)+fontheight(TkGate.stextXF[1]),
		   BetweenLeftAndRight | BetweenTopAndBottom,B);
  }
  if (g->u.block.moduleName) {
    if (g->selected)
      XSetFont(TkGate.D,TkGate.moduleGC, TkGate.textbXF[TkGate.circuit->zoom_factor]->fid);
    else
      XSetFont(TkGate.D,TkGate.moduleGC, TkGate.textXF[TkGate.circuit->zoom_factor]->fid);


    dce_DrawString(TkGate.moduleGC,g->xpos + (g->u.block.gwidth / 2),
		   g->ypos + (g->u.block.gheight / 2),
		   BetweenLeftAndRight | BetweenTopAndBottom,
		   g->u.block.moduleName);
  }
}

/* Move a block */
void Block_Move(GCElement *g,int x,int y)
{
  int ports_fixed = block_isFixedPort(g);
  int N = GCElement_numPads(g);
  int i;
  int min_width = MINSIZE;
  int min_height = MINSIZE;

  if (g->top || g->bottom || g->left || g->right || TkGate.circuit->wsel) {
    if (block_isIntfProtected(g)) {
      message(0,msgLookup("err.protintf"),g->u.block.moduleName);
      return;
    }
  }

  if (TkGate.circuit->wsel) {
    int pad = block_getPad(g,TkGate.circuit->wsel);

    switch (pad) {
    case BLOCK_TIN :
    case BLOCK_TOUT :
    case BLOCK_TTRI :
    case BLOCK_BIN :
    case BLOCK_BOUT :
    case BLOCK_BTRI :
      if (TkGate.circuit->wsel->nodes->x + x < g->xpos + 3)
	x = g->xpos + 3 - TkGate.circuit->wsel->nodes->x;
      if (TkGate.circuit->wsel->nodes->x + x > g->xpos + g->u.block.gwidth - 3)
	x = g->xpos + g->u.block.gwidth - 3 - TkGate.circuit->wsel->nodes->x;
      y = 0;
      break;
    case BLOCK_LIN :
    case BLOCK_LOUT :
    case BLOCK_LTRI :
    case BLOCK_RIN :
    case BLOCK_ROUT :
    case BLOCK_RTRI :
      if (TkGate.circuit->wsel->nodes->y + y < g->ypos + 3)
	y = g->ypos + 3 - TkGate.circuit->wsel->nodes->y;
      if (TkGate.circuit->wsel->nodes->y + y > g->ypos + g->u.block.gheight - 3)
	y = g->ypos + g->u.block.gheight - 3 - TkGate.circuit->wsel->nodes->y;
      x = 0;
      break;
    default :
      return;
      break;
    }
    wire_move(TkGate.circuit->wsel->nodes,x,y,FULL);
    block_setWireEnd(g,TkGate.circuit->wsel,pad);
    SetModified(MF_INTERFACE);
    return;
  }

  ob_touch(g);

  /*
   * If fixed ports, compute minimum size.
   */
  if (ports_fixed) {
    for (i = 0;i < N;i++) {
      GWire *w;
      for (w = g->wires[i];w;w = w->next) {
	block_setWireEnd(g,w,i);

	if (g->typeinfo->Pad[i].Loc->dir == D_LEFT || g->typeinfo->Pad[i].Loc->dir == D_RIGHT) {
	  int height = w->nodes->y - g->ypos + 15;
	  if (height > min_height) min_height = height;
	} else {
	  int width = w->nodes->x - g->xpos + 15;
	  if (width > min_width) min_width = width;
	}
      }
    }
  }

  if (g->top) {
    if (g->u.block.gheight - y < min_height)
      y = g->u.block.gheight - min_height;
    g->u.block.gheight -= y;
    g->ypos += y;
    for (i = 0;i < N;i++) {
      if (g->typeinfo->Pad[i].Loc->dir == D_LEFT || g->typeinfo->Pad[i].Loc->dir == D_RIGHT)
	block_scalewirelist(g->wires[i],g,1,ports_fixed);
      if (g->typeinfo->Pad[i].Loc->dir == D_UP)
	block_movewirelist(g->wires[i],0,y);
    }

    SetModified(MF_INTERFACE|MF_FORCE);
  } else if (g->bottom) {
    if (g->u.block.gheight + y < min_height)
      y = min_height - g->u.block.gheight;
    g->u.block.gheight += y;
    
    for (i = 0;i < N;i++) {
      if (g->typeinfo->Pad[i].Loc->dir == D_LEFT || g->typeinfo->Pad[i].Loc->dir == D_RIGHT)
	block_scalewirelist(g->wires[i],g,1,ports_fixed);
      if (g->typeinfo->Pad[i].Loc->dir == D_DOWN)
	block_movewirelist(g->wires[i],0,y);
    }

    SetModified(MF_INTERFACE|MF_FORCE);
  }
  
  if (g->left) {
    if (g->u.block.gwidth - x < min_width)
      x = g->u.block.gwidth - min_width;
    g->u.block.gwidth -= x;
    g->xpos += x;

    for (i = 0;i < N;i++) {
      if (g->typeinfo->Pad[i].Loc->dir == D_UP || g->typeinfo->Pad[i].Loc->dir == D_DOWN)
	block_scalewirelist(g->wires[i],g,0,ports_fixed);
      if (g->typeinfo->Pad[i].Loc->dir == D_LEFT)
	block_movewirelist(g->wires[i],x,0);
    }

    SetModified(MF_INTERFACE|MF_FORCE);
  } else if (g->right) {
    if (g->u.block.gwidth + x < min_width)
      x = min_width - g->u.block.gwidth;
    g->u.block.gwidth += x;
    
    for (i = 0;i < N;i++) {
      if (g->typeinfo->Pad[i].Loc->dir == D_UP || g->typeinfo->Pad[i].Loc->dir == D_DOWN)
	block_scalewirelist(g->wires[i],g,0,ports_fixed);
      if (g->typeinfo->Pad[i].Loc->dir == D_RIGHT)
	block_movewirelist(g->wires[i],x,0);
    }

    SetModified(MF_INTERFACE|MF_FORCE);
  } else if  (!g->top && !g->bottom) {
    for (i = 0;i < N;i++)
      block_movewirelist(g->wires[i],x,y);
    g->xpos += x;
    g->ypos += y;
  }

}

void Block_AddInput(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.oldportact"));
}

void Block_AddTri(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.oldportact"));
}

void Block_AddOutput(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.oldportact"));
}

void Block_ChangePin(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.oldportact"));
}

void Block_PSDrawWireName(GPrint *P,GWire *w,int Dir,int IODir)
{
  if (w->name) {
    HtmlFont font[1];
    int x,y,p;
    Icon *arrow;

    block_getwirexy(w,Dir,&x,&y,&p);
    PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),x,y,w->name,p);
    GetPinIOMark(w,Dir,IODir,IODT_PORT,&x,&y,&arrow);
    switch (IODir) {
    case IN :
      fprintf(P->p_f,"%d %d %d arrow\n",x,y,(Dir+2)%4);
      break;
    case OUT :
      fprintf(P->p_f,"%d %d %d arrow\n",x,y,Dir);
      break;
    case TRI :
    case ANY :
    default:
      fprintf(P->p_f,"%d %d %d tri\n",x,y,Dir%2);
      break;
    }
  }
}

void Block_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  int N = GCElement_numPads(g);
  int i;
  GWire *w;
  HtmlFont font[1];

  
  
  fprintf(P->p_f,"%d %d %d %d box\n",g->xpos,g->ypos,
	  g->u.block.gwidth,g->u.block.gheight);
  
  for (i = 0;i < N;i++)
    for (w = g->wires[i];w;w = w->next)
      Block_PSDrawWireName(P,w,g->typeinfo->Pad[i].Loc[0].dir,
			   g->typeinfo->Pad[i].iotype);
  
  if (g->ename && g->show_name) {
    char B[STRMAX];
    int h;

    if (TkGate.tcl)
      h = fontheight(TkGate.stextXF[1]);
    else
      h = 8;

    sprintf(B,"(%s)",g->ename);
    PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),
	       g->xpos + (g->u.block.gwidth / 2),
	       g->ypos + (g->u.block.gheight / 2)+h,
	       B,BetweenLeftAndRight | BetweenTopAndBottom);
  }
  if (g->u.block.moduleName) {
    PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,10),
	       g->xpos + (g->u.block.gwidth / 2),
	       g->ypos + (g->u.block.gheight / 2),
	       g->u.block.moduleName,
	       BetweenLeftAndRight | BetweenTopAndBottom);
		
  }
}

void Block_EditParmProps(GCElement *g, int isLoadDialog)
{
  GModuleDef *m = env_findModule(g->u.block.moduleName); 

  if (isLoadDialog) {
    Tcl_UnsetVar(TkGate.tcl,"edgat_modParms",TCL_GLOBAL_ONLY);
    if (m && m->m_parmPorts) {
      HashElem *he;
      for (he = Hash_first(m->m_parmPorts);he;he = Hash_next(m->m_parmPorts,he)) {
	const char *parmName = SHashElem_key(he);
	const char *value = Block_getParmPort(g,parmName);
	if (!value)
	  value = (const char*)HashElem_obj(he);

	Tcl_SetVar2(TkGate.tcl,"edgat_modParms",parmName,value,TCL_GLOBAL_ONLY);
      }
    }
  } else {
    if (m && m->m_parmPorts) {
      HashElem *he;
      for (he = Hash_first(m->m_parmPorts);he;he = Hash_next(m->m_parmPorts,he)) {
	const char *parmName = SHashElem_key(he);
	const char *defaultValue = (char*)HashElem_obj(he);
	const char *newValue = Tcl_GetVar2(TkGate.tcl,"edgat_modParms",parmName,TCL_GLOBAL_ONLY);

	if (newValue && strcmp(defaultValue,newValue) != 0)
	  Block_setParmPort(g,parmName,newValue);
	else
	  Block_unsetParmPort(g,parmName);
      }
    }
  }
}


int Block_EditProps(GCElement *g,int isLoadDialog)
{
  Tcl_Interp *tcl = TkGate.tcl;

  if (isLoadDialog) {
    Tcl_SetVar(TkGate.tcl,"edgat_blockFunc",
	       g->u.block.moduleName,TCL_GLOBAL_ONLY);
    Generic_EditProps(g,isLoadDialog);
    Block_EditParmProps(g, isLoadDialog);
  } else {
    char buf[STRMAX];
    const char *p;

    Generic_EditProps(g,isLoadDialog);
    if ((p = Tcl_GetVar(tcl,"edgat_blockFunc",TCL_GLOBAL_ONLY)) && *p && strcmp(p,g->u.block.moduleName) != 0) {
      int i;
      ob_touch(g);
      ob_free(g->u.block.moduleName);
      pickValidName(buf,p,"func",0);
      g->u.block.moduleName = ob_strdup(buf);
      for (i = 0;i < g->u.block.numModParms;i++)
	ob_free(g->u.block.modParms[i]);
      g->u.block.numModParms = 0;
      if (g->u.block.modParms) ob_free(g->u.block.modParms);
      g->u.block.modParms = 0;
    } else
      Block_EditParmProps(g, isLoadDialog);
    SetModified(MF_INTERFACE);
  }

  return 0;
}

/*****************************************************************************
 *
 * Save module parameter data for a module instance.
 *
 * Parameters:
 *     f		File in which to save module parameter data
 *     g		Module instance from which to save module parameter data.
 *
 *****************************************************************************/
void Block_VerSaveModParms(FILE *f,GCElement *g)
{
  GModuleDef *m = env_findModule(g->u.block.moduleName); 
  HashElem *he;
  int n = 0;

  if (!m || !m->m_parmPorts || g->u.block.numModParms == 0) return;

  for (he = Hash_first(m->m_parmPorts);he;he = Hash_next(m->m_parmPorts,he)) {
    char buf[STRMAX];
    const char *parmName = SHashElem_key(he);
    const char *defaultValue = (char*)HashElem_obj(he);
    const char *value = Block_getParmPort(g,parmName);
    if (!value || strcmp(value,defaultValue) == 0) continue;

    if (n == 0)
      fprintf(f," #(");
    else
      fprintf(f,", ");

    fprintf(f,".%s(",parmName);
    if (isVerilogConstant(value)) {
      fprintf(f,"%s",value);
    } else if (*value == '"') {
      int l = strlen(value);
      /*If the string is already enclosed in quotes, then don't requote it. */
      if (l == 0)
	fprintf(f,"\"\"");
      else if (l == 1)
	fprintf(f,"\"%s\"",value);
      else if (value[0] == '"' && value[l-1] == '"' && value[l-2] != '\\') {
	char buf2[STRMAX];
	strcpy(buf2,value+1);
	buf2[l-2] = 0;
	sprintf(buf,quoteChars(buf,buf2,"\"\\"));
	fprintf(f,"\"%s\"",buf);
      } else
	fprintf(f,"\"%s\"",quoteChars(buf,value,"\"\\"));
    } else {
      fprintf(f,"\"%s\"",quoteChars(buf,value,"\"\\"));
    }
    fprintf(f,")");
    n++;
  }

  if (n > 0)
    fprintf(f,")");
}

/*****************************************************************************
 *
 * Save data for a module instance.
 *
 * Parameters:
 *     f		File in which to save module instance
 *     g		Module instance to be saved
 *
 *****************************************************************************/
void Block_VerSave(FILE *f,GCElement *g)
{
  int N = GCElement_numPads(g);
  GWire *w;
  int i,j;
  int first_pin;
  static char dirchar[] = "?=><";

  fprintf(f,"  %s",g->u.block.moduleName);
  Block_VerSaveModParms(f,g);
  fprintf(f," %s",g->ename);

  first_pin = 1;
  fprintf(f," (");
  for (i = 0;i < N;i++) {
    for (w = g->wires[i], j=0;w;w = w->next, j++) {
      if (first_pin)
	first_pin = 0;
      else
	fprintf(f,", ");
      fprintf(f,".%s(%s)",w->name,w->net->n_signame);
    }
  }
  fprintf(f,");");

  fprintf(f,"   //: @(%d, %d) /sz:(%d, %d)",g->xpos,g->ypos,g->u.block.gwidth,g->u.block.gheight);

  if (g->orient != 0)
    fprintf(f," /R:%d",g->orient);

  if (!g->show_name)
    fprintf(f," /sn:%d",g->show_name);
  if (g->anchored)
    fprintf(f," /anc:1");

  fprintf(f," /p:[");
  for (i = 0;i < N;i++) {
    for (w = g->wires[i], j=0;w;w = w->next, j++) {
      fprintf(f," %s%d%c%d",
	      GCElement_getPadName(g,i),j,
	      dirchar[GCElement_getPadDir(g,i)],
	      w->nidx);
    } 
  }
  fprintf(f," ]");

  fprintf(f,"\n");
}

void block_scalewirelist(GWire *w,GCElement *g,int isY,int portsFixed)
{
  for (;w;w = w->next) {
    int dx = 0;
    int dy = 0;

    if (portsFixed) {
      if (isY) {
	dy = w->offset.num  + g->ypos - w->nodes->y;
	w->offset.den = g->u.block.gheight;
      } else {
	dx = w->offset.num + g->xpos - w->nodes->x;
	w->offset.den = g->u.block.gwidth;
      }
    } else {
      if (isY) {
	dy = (g->u.block.gheight*w->offset.num)/w->offset.den+g->ypos-w->nodes->y;
      } else {
	dx = (g->u.block.gwidth*w->offset.num)/w->offset.den+g->xpos-w->nodes->x;
      }
    }

    wire_move(w->nodes,dx,dy,FULL);
  }
}


void block_movewirelist(GWire *w,int dx,int dy)
{
  for (;w;w = w->next) {
    GWire *ow = wire_other(w);
    GCElement *g = w->gate;

    if (ow->gate && ow->gate == g) {
      if (w->driver == w) {
	GWireNode *n;
      
	for (n = w->driver->nodes;n;n = n->out) {
	  ob_touch(n);
	  n->x += dx;
	  n->y += dy;
	}
      }
    } else
      wire_move(w->nodes,dx,dy,FULL);
  }
}

int block_getPortDir(GCElement *g,GWire *w)
{
  int N = GCElement_numPads(g);
  int i;

  for (i = 0;i < N;i++) {
    GWire *bw;

    for (bw = g->wires[i];bw;bw = bw->next)
      if (bw == w)
	return GCElement_getPadDir(g,i);
  }
  return 0;
}

int block_getPad(GCElement *g,GWire *w)
{
  int N = GCElement_numPads(g);
  int i;

  for (i = 0;i < N;i++) {
    GWire *bw;

    for (bw = g->wires[i];bw;bw = bw->next)
      if (bw == w)
	return i;
  }
  return -1;
}

/*
 * Try to guess a name for a port with the specified characteristics.
 */
void guessPortName(char *buf,GCElement *g,int orient,int dir,int nbits)
{
  int N = GCElement_numPads(g);
  GWire *w =0;
  int i;
  GWire *best_w = 0;	/* Best matching wire so far */
  int best_score = 0;

  *buf = 0;

  for (i = 0;i < N;i++) {
    for (w = g->wires[i];w;w = w->next) {
      int score = 0;
      int n;

      if (g->typeinfo->Pad[i].iotype != dir) continue;		/* Must be at least same I/O dir */
      if (!w->name) continue;

      if (w->orient == orient) score += 5000;			/* Same orientation */
      if (w->net->n_nbits == nbits) score += 1000;		/* Same bit size */

      if (sscanf(w->name,"%*[A-Za-z_]%d",&n) == 1)
	score += n + 500;
      else
	score += *w->name;

      if (score > best_score) {
	best_w = w;
	best_score = score;
      }
    }
  }

  /*
   * We found a best similar port, use it to guess a name.
   */
  if (best_w) {
    DoTcl("PortEntry::generateNextNameFrom %s",best_w->name);
    if (*TkGate.tcl->result)
      strcpy(buf,TkGate.tcl->result);
  }

  if (!*buf) {
    switch (dir) {
    case IN :
      strcpy(buf,"I0");
      break;
    case OUT :
      strcpy(buf,"IO0");
      break;
    case TRI :
    default :
      strcpy(buf,"Z0");
      break;
    }
  }
}

/*****************************************************************************
 *
 * Set the name of port 'w' on block 'g'. 
 *
 *****************************************************************************/
int block_setPortName(GCElement *g,GWire *w,EditState *es)
{
  static char Buf[STRMAX];
  int x,y;
  int dir;
  int nbits;
  const char *sdir = "";
  const char *sigName;
  const char *portName;
  int ok;

  if (block_isIntfProtected(g)) {
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return 0;
  }

  dir = block_getPortDir(w->gate,w);
  sdir = iotypeStr(dir);

  Tcl_SetVar(TkGate.tcl,"edport_sig",w->net->n_signame,TCL_GLOBAL_ONLY);
  if (w->name)
    Tcl_SetVar(TkGate.tcl,"edport_port",w->name,TCL_GLOBAL_ONLY);
  else {
    guessPortName(Buf,g,w->orient,dir,1);
    Tcl_SetVar(TkGate.tcl,"edport_port",Buf,TCL_GLOBAL_ONLY);
  }

  Tcl_SetVar(TkGate.tcl,"edport_type",sdir,TCL_GLOBAL_ONLY);
  sprintf(Buf,"%d",w->net->n_nbits);
  Tcl_SetVar(TkGate.tcl,"edport_bits",Buf,TCL_GLOBAL_ONLY);

  x = w->nodes->x;
  y = w->nodes->y;

  if (editstate_getInterfaceMode() == INTFMODE_NONE)
    DoTcl("PortEdit::post [offsetgeometry . %d %d] -ismodule 1",ctow_x(x+125),ctow_y(y+50));
  else
    DoTcl("PortEdit::post [offsetgeometry . %d %d] -ismodule 1 -showsig 0",ctow_x(x+125),ctow_y(y+50));

  if (strcmp(TkGate.tcl->result,"1") == 0) {
    const char *numBits;
    int new_dir;


    sigName = Tcl_GetVar(TkGate.tcl,"edport_sig",TCL_GLOBAL_ONLY);
    portName = Tcl_GetVar(TkGate.tcl,"edport_port",TCL_GLOBAL_ONLY);
    numBits = Tcl_GetVar(TkGate.tcl,"edport_bits",TCL_GLOBAL_ONLY);
    sdir = Tcl_GetVar(TkGate.tcl,"edport_type",TCL_GLOBAL_ONLY);

    if (!sigName || sscanf(numBits,"%d",&nbits) != 1)
      nbits = w->net->n_nbits;

    new_dir = strIOType(sdir);
    if (new_dir < 0) new_dir = dir;

    /*
     * Change direction 1 or 2 times to make direction match.
     */
    if (new_dir != dir) {
      block_changedir(w->nodes,es);
      dir = block_getPortDir(w->gate,w);
      if (dir != new_dir) {
	block_changedir(w->nodes,es);
	dir = block_getPortDir(w->gate,w);
      }
    }

    SetModified(MF_INTERFACE);

    ok = 1;
  } else {
    sigName = w->net->n_signame;
    portName = w->name ? w->name : DEFAULT_PORT_NAME;
    nbits = w->net->n_nbits;

    ok = 0;
  }
  

  Block_Draw(w->gate,0);
  if (strcmp(w->net->n_signame,sigName) != 0) net_rename(w->net,sigName,GNet_getShowName(w->net));

  if (strcmp(portName,DEFAULT_PORT_NAME) == 0 || !*portName) {
    guessPortName(Buf,g,w->orient,dir,nbits);
    portName = Buf;
  }
  pickValidName(Buf,portName,"P",0);

  ob_touch(w);
  w->name = ob_strdup(Buf);
  net_setSize(w->net,nbits);
  Block_Draw(w->gate,0);

  SynchronizeInterface();

  return ok;
}

void block_getwirexy(GWire *w,int d,int *x,int *y,int *p)
{
  int h;
  
  if (TkGate.tcl)
    h = fontheight(TkGate.stextXF[1]);
  else
    h = 8;

  switch (d) {
  case D_UP :
    *x = w->nodes->x;
    *y = w->nodes->y + (7*h)/8;
    *p = BetweenLeftAndRight | BetweenTopAndBottom;
    break;
  case D_LEFT :
    *x = w->nodes->x + 4;
    *y = w->nodes->y + 3;
    *p = AtLeft | BetweenTopAndBottom;
    break;
  case D_DOWN :
    *x = w->nodes->x;
    *y = w->nodes->y - (2*h)/3;
    *p = BetweenLeftAndRight | BetweenTopAndBottom;
    break;
  case D_RIGHT :
    *x = w->nodes->x - 4;
    *y = w->nodes->y + 3;
    *p = AtRight | BetweenTopAndBottom;
    break;
  }
}

/* Used by box exploder */
static void propogate(int ux[NUMSQRS],int uy[NUMSQRS],int lx[NUMSQRS],int ly[NUMSQRS],int n)
{
  int i;

  for (;n;n--)
    for (i = NUMSQRS-1;i;i--) {
      ux[i-1] = ux[i];
      uy[i-1] = uy[i];
      lx[i-1] = lx[i];
      ly[i-1] = ly[i];
    }
}

/* Do explosion look on box */
void block_explode(GCElement *g)
{
  int ux[NUMSQRS],uy[NUMSQRS],lx[NUMSQRS],ly[NUMSQRS],n;
  int bd;

#if 1
  (*g->typeinfo->GetExtents)(g,TD_X11,&ux[NUMSQRS-1],&uy[NUMSQRS-1],&lx[NUMSQRS-1],&ly[NUMSQRS-1],&bd);
#else
  ux[NUMSQRS-1] = ctow_x(g->xpos);
  uy[NUMSQRS-1] = ctow_y(g->ypos);
  lx[NUMSQRS-1] = ux[NUMSQRS-1] + g->u.block.gwidth;
  ly[NUMSQRS-1] = uy[NUMSQRS-1] + g->u.block.gheight;
#endif

  propogate(ux,uy,lx,ly,NUMSQRS-1);
  for (n = NUMSTEPS;n;n--) {
    XClearArea(TkGate.D,TkGate.W,ux[0],uy[0],lx[0]-ux[0],ly[0]-uy[0],0);
    propogate(ux,uy,lx,ly,1);
    ux[NUMSQRS-1] -= ux[NUMSQRS-1] / n;
    uy[NUMSQRS-1] -= uy[NUMSQRS-1] / n;
    lx[NUMSQRS-1] += (TkGate.width - lx[NUMSQRS-1]) / n;
    ly[NUMSQRS-1] += (TkGate.height - ly[NUMSQRS-1]) / n;
    box(wtoc_x(ux[NUMSQRS-1]),wtoc_y(uy[NUMSQRS-1]),
	lx[NUMSQRS-1]-ux[NUMSQRS-1],ly[NUMSQRS-1]-uy[NUMSQRS-1]);
    XFlush(TkGate.D);
    usleep(1);
  }
  FlagRedraw();
}

void block_setWireEnd(GCElement *g,GWire *w,int pad)
{
  GWireNode *n = w->nodes;

  ob_touch(w);
  switch (pad) {
  case BLOCK_TIN :
  case BLOCK_TOUT :
  case BLOCK_TTRI :
    w->offset.num = n->x - g->xpos;
    w->offset.den = g->u.block.gwidth;
    w->orient = RIGHT;
    break;
  case BLOCK_LIN :
  case BLOCK_LOUT :
  case BLOCK_LTRI :
    w->offset.num = n->y - g->ypos;
    w->offset.den = g->u.block.gheight;
    w->orient = DOWN;
    break;
  case BLOCK_BIN :
  case BLOCK_BOUT :
  case BLOCK_BTRI :
    w->offset.num = n->x - g->xpos;
    w->offset.den = g->u.block.gwidth;
    w->orient = LEFT;
    break;
  case BLOCK_RIN :
  case BLOCK_ROUT :
  case BLOCK_RTRI :
    w->offset.num = n->y - g->ypos;
    w->offset.den = g->u.block.gheight;
    w->orient = UP;
    break;
  default :
    logError(ERL_ERROR,"Illegal pad number for wire on block.");
  }
}

void addbendnode(GWireNode *n)
{
  GWireNode *nn;

  nn = new_GWireNode();
  ob_touch(nn);
  ob_touch(n);

  nn->x = n->x;
  nn->y = n->y;
  if (n->in) {
    ob_touch(nn->in);

    nn->in = n->in;
    nn->in->out = nn;
    n->in = nn;
    nn->out = n;
  } else {
    ob_touch(nn->out);

    nn->out = n->out;
    nn->out->in = nn;
    n->out = nn;
    nn->in = n;
  }
}

static int quadrent(GCElement *g,int x,int y)
{
  if ((y - g->ypos)*g->u.block.gwidth > (x - g->xpos)*g->u.block.gheight) {
    if ((g->ypos+g->u.block.gheight-y)*g->u.block.gwidth >
	(x - g->xpos)*g->u.block.gheight) {
      return D_LEFT;
    } else {
      return D_DOWN;
    }
  } else {
    if ((g->ypos+g->u.block.gheight-y)*g->u.block.gwidth >
	(x - g->xpos)*g->u.block.gheight) {
      return D_UP;
    } else {
      return D_RIGHT;
    }
  }
}

static int nquadrent(GCElement *g,GWireNode *n)
{
  return quadrent(g,n->x,n->y);
}

int block_connect(GCElement *g,GWireNode *n,int iotype)
{
  int p,q;

  ob_touch(n->end);
  n->end->gate = g;
  
  switch (p = nquadrent(g,n)) {
  case D_UP :
    q = BLOCK_TIN;
    break;
  case D_DOWN :
    q = BLOCK_BIN;
    break;
  case D_LEFT :
    q = BLOCK_LIN;
    break;
  case D_RIGHT :
    q = BLOCK_RIN;
    break;
  default :
    return -1;
  }
  
  switch (iotype) {
  case IN :
    break;
  case OUT :
    q += 4;
    break;
  case TRI :
    q += 8;
    break;
  }

  ob_touch(g);
  g->wires[q] = block_addwire(n->end,g->wires[q]);
  block_setWireEnd(g,n->end,q);

  SetModified(MF_NET|MF_INTERFACE);
  
  switch (p) {
  case D_UP :
    wire_move(n,0,g->ypos - 1 - n->y,n->stype);
    if (!(wireorient(n,0) & 1))
      addbendnode(n);
    break;
  case D_LEFT :
    wire_move(n,g->xpos - 1 - n->x,0,n->stype);
    if (wireorient(n,0) & 1)
      addbendnode(n);
    break;
  case D_DOWN :
    wire_move(n,0,(g->ypos + g->u.block.gheight+1) - n->y,n->stype);
    if (!(wireorient(n,0) & 1))
      addbendnode(n);
    break;
  case D_RIGHT :
    wire_move(n,(1 + g->xpos + g->u.block.gwidth) - n->x,0,n->stype);
    if (wireorient(n,0) & 1)
      addbendnode(n);
    break;
  default :
    return -1;
  }

  wire_finalizeNet(n->end);

  return p;
}

GWire *block_addwire(GWire *w,GWire *wl)
{
  ob_touch(w);
  w->next = wl;
  return w;
}

/*
 * Primitive function to disconnect wire
 */
void block_deletewire(GWire *w)
{
  int n,p;
  GCElement *g;
  static int xdel[] = {3,0,-3,0};
  static int ydel[] = {0,-3,0,3};
  
  posongate(w,w->gate,&p,&n);
  g = w->gate;

  if (block_isIntfProtected(g)) {
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return;
  }

  ob_touch(g);
  ob_touch(w);
  ob_touch(w->nodes);

  gate_draw(g,GD_NOWIRE);
  ob_free(w->name);
  w->name = NULL;
  w->gate = NULL;
  
  g->wires[p] = wire_unattach(w,g->wires[p]);
  w->nodes->y += ydel[g->typeinfo->Pad[p].Loc->dir];
  w->nodes->x += xdel[g->typeinfo->Pad[p].Loc->dir];
  
  gate_draw(g,GD_NOWIRE);

  SetModified(MF_NET|MF_INTERFACE);
}

/*
 * Disconnect wire from block and do any necessary cleanup
 */
void block_cutoffwire(GWire *w,EditState *es)
{
  GCElement *g = w->gate;

  if (block_isIntfProtected(g)) {
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return;
  }
  
  GNet_draw(w->net);
  block_deletewire(w);
  wire_nuke(w,0,es->env);
}

/*****************************************************************************
 *
 * Get the properties for a new port to be added to a block.
 *
 *****************************************************************************/
int block_getNewPortProperties(GCElement *g,int x,int y,int orient,
			       char *name,int *iodir,int *nbits)
{
  char buf[STRMAX];

  sprintf(buf,"%d",*nbits);
  guessPortName(name,g,orient,*iodir,*nbits);

  Tcl_SetVar(TkGate.tcl,"edport_sig","",TCL_GLOBAL_ONLY);
  Tcl_SetVar(TkGate.tcl,"edport_port",name,TCL_GLOBAL_ONLY);
  Tcl_SetVar(TkGate.tcl,"edport_type",iotypeStr(*iodir),TCL_GLOBAL_ONLY);
  Tcl_SetVar(TkGate.tcl,"edport_bits",buf,TCL_GLOBAL_ONLY);

  DoTcl("PortEdit::post [offsetgeometry . %d %d] -ismodule 1 -showsig 0",
	ctow_x(x+125),ctow_y(y+50));

  if (strcmp(TkGate.tcl->result,"1") == 0) {
    const char *numBits;
    const char *sigName;
    const char *portName;
    const char *sdir;

    sigName = Tcl_GetVar(TkGate.tcl,"edport_sig",TCL_GLOBAL_ONLY);
    portName = Tcl_GetVar(TkGate.tcl,"edport_port",TCL_GLOBAL_ONLY);
    numBits = Tcl_GetVar(TkGate.tcl,"edport_bits",TCL_GLOBAL_ONLY);
    sdir = Tcl_GetVar(TkGate.tcl,"edport_type",TCL_GLOBAL_ONLY);
    
    strcpy(name,portName);
    sscanf(numBits,"%d",nbits);
    *iodir = strIOType(sdir);

    return 1;
  } else
    return 0;
}


void block_newport(EditState *es,int iodir)
{
  GCElement *g = TkGate.circuit->select;
  char portName[STRMAX];
  int nbits;
  int orient;
  GWire *w;
  int p;

  if (!block_edgehit(g,TkGate.ed->tx,TkGate.ed->ty)) return;
  if (g && !(TkGate.circuit->select->typeinfo->Code == GC_BLOCK)) return;

  if (block_isIntfProtected(g)) {
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return;
  }

  orient = quadrent(g,TkGate.ed->tx,TkGate.ed->ty);
  nbits = 1;
  if (!block_getNewPortProperties(g,TkGate.ed->tx,TkGate.ed->ty,orient,
				  portName,&iodir,&nbits))
    return;
   
  wire_new(es->env,0,&w);
  p = block_attach(es->env,g,w,w->driver,TkGate.ed->tx,TkGate.ed->ty,NULL,iodir);

  if (p >= 0) {
    pickValidName(portName,portName,"P",0);

    ob_touch(w);
    w->name = ob_strdup(portName);
    net_setSize(w->net,nbits);

    GWire_draw(w->driver);
    DrawPinIOMark(w,p,iodir,IODT_PORT);

    SetModified(MF_INTERFACE|MF_NET);
    SynchronizeInterface();
  }
  FlagRedraw();
}

void block_setdir(GWireNode *n,EditState *es,int dir)
{
  GCElement *g;
  int p,x;
  
  if (!n || !n->end || !n->end->gate) return;

  if (n->end->gate->typeinfo->Code != GC_BLOCK) {
    message(1,msgLookup("err.nopin"));			/* Can't change selected pin. */
    return;
  }
  g = n->end->gate;

  if (block_isIntfProtected(g)) {
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return;
  }

  ob_touch(g);
  
  posongate(n->end,g,&p,&x);
  
  DrawPinIOMark(n->end,g->typeinfo->Pad[p].Loc[0].dir,
		g->typeinfo->Pad[p].iotype,IODT_PORT);
  g->wires[p] = wire_unattach(n->end,g->wires[p]);

  p = p % 4;
  if (dir == OUT)
    p += 4;
  else if (dir == TRI)
    p += 8;
  
  DrawPinIOMark(n->end,g->typeinfo->Pad[p].Loc[0].dir,
		g->typeinfo->Pad[p].iotype,IODT_PORT);
  g->wires[p] = block_addwire(n->end,g->wires[p]);
  
  SetModified(MF_INTERFACE);
  SynchronizeInterface();
}

void block_changedir(GWireNode *n,EditState *es)
{
  GCElement *g;
  int p,x;
  
  if (!n || !n->end || !n->end->gate) return;

  if (n->end->gate->typeinfo->Code != GC_BLOCK) {
    message(1,msgLookup("err.nopin"));			/* Can't change selected pin. */
    return;
  }
  g = n->end->gate;

  if (block_isIntfProtected(g)) {
    message(0,msgLookup("err.protintf"),g->u.block.moduleName);
    return;
  }

  ob_touch(g);
  
  posongate(n->end,g,&p,&x);
  
  DrawPinIOMark(n->end,g->typeinfo->Pad[p].Loc[0].dir,
		g->typeinfo->Pad[p].iotype,IODT_PORT);
  g->wires[p] = wire_unattach(n->end,g->wires[p]);
  
  p = (p + 4) % GCElement_numPads(g);
  
  DrawPinIOMark(n->end,g->typeinfo->Pad[p].Loc[0].dir,
		g->typeinfo->Pad[p].iotype,IODT_PORT);
  g->wires[p] = block_addwire(n->end,g->wires[p]);
  
  SetModified(MF_INTERFACE);
  SynchronizeInterface();
}

int block_attach(GModuleDef *env,GCElement *g,GWire *w1,GWire *w2,
	     int x,int y,const char *name,int iotype)
{
  int p;

  ob_touch(w1);
  ob_touch(w1->nodes);
  ob_touch(w2->nodes);

  w1->nodes->x = x;
  w1->nodes->y = y;
  w1->gate = g;

  p = -1;
  switch (nquadrent(g,w1->nodes)) {
  case D_UP :
    w1->nodes->y = g->ypos - 1;
    w2->nodes->y = w1->nodes->y - BLOCK_STUBLEN;
    w1->nodes->x = w2->nodes->x = x;
    break;
  case D_LEFT :
    w1->nodes->x = g->xpos-1;
    w2->nodes->x = w1->nodes->x - BLOCK_STUBLEN;
    w1->nodes->y = w2->nodes->y = y;
    break;
  case D_DOWN :
    w1->nodes->y = g->ypos + g->u.block.gheight + 1;
    w2->nodes->y = w1->nodes->y + BLOCK_STUBLEN;
    w1->nodes->x = w2->nodes->x = x;
    break;
  case D_RIGHT :
    w1->nodes->x = g->xpos + g->u.block.gwidth + 1;
    w2->nodes->x = w1->nodes->x + BLOCK_STUBLEN;
    w1->nodes->y = w2->nodes->y = y;
    break;
  default :
    return -1;
  }
  p = block_connect(g,w1->nodes,iotype);
  SetModified(MF_INTERFACE);
  
  if (name && w1) w1->name = ob_strdup(name);
  
  return p;
}

int block_edgehit(GCElement *g,int x,int y)
{
  int bx,by,bw,bh;
  int e1,e2,e3,e4;

  bx = g->xpos;
  by = g->ypos;
  bw = g->u.block.gwidth;
  bh = g->u.block.gheight;

  e1 = on_edge(x,y,bx+2,by,bx+bw-2,by);
  e2 = on_edge(x,y,bx,by+2,bx,by+bh-2);
  e3 = on_edge(x,y,bx+bw,by+2,bx+bw,by+bh-2);
  e4 = on_edge(x,y,bx+2,by+bh,bx+bw-2,by+bh);

  return e1 || e2 || e3 || e4;
}

void block_canonicalizewire(GCElement *g,GWire *w)
{
  GWireNode *n;

  n = w->nodes->out ? w->nodes->out : w->nodes->in;

  ob_touch(n);
  ob_touch(w->nodes);

  switch (w->orient) {
  case 0 :
    w->nodes->x = g->xpos + g->u.block.gwidth + 1;
    n->x = w->nodes->x + BLOCK_STUBLEN;
    n->y = w->nodes->y;
    break;
  case 1 :
    w->nodes->y = g->ypos - 1;
    n->y = w->nodes->y - BLOCK_STUBLEN;
    n->x = w->nodes->x;
    break;
  case 2 :
    w->nodes->x = g->xpos - 1;
    n->x = w->nodes->x - BLOCK_STUBLEN;
    n->y = w->nodes->y;
    break;
  case 3 :
    w->nodes->y = g->ypos + g->u.block.gheight + 1;
    n->y = w->nodes->y + BLOCK_STUBLEN;
    n->x = w->nodes->x;
    break;
  }
}

char *block_reportname(GCElement *g)
{
  static char B[STRMAX];

  if (g->ename)
    sprintf(B,"block %s",g->ename);
  else
    sprintf(B,"unnamed %s block",g->u.block.moduleName);
  return B;
}

int Block_SimHitFunc(EditState *es,GCElement *g)
{
  EditState_selectGate(es,TkGate.ed->tx,TkGate.ed->ty);
  return 1;
}

int block_isDataProtected(GCElement *g)
{
  GModuleDef *M = env_findModule(g->u.block.moduleName);
  if (M)
    return GModuleDef_isDataProtected(M);
  return 0;
}

int block_isIntfProtected(GCElement *g)
{
  GModuleDef *M = env_findModule(g->u.block.moduleName);
  if (M)
    return GModuleDef_isIntfProtected(M);
  return 0;
}

int block_isFixedPort(GCElement *g)
{
  GModuleDef *M = env_findModule(g->u.block.moduleName);
  if (M)
    return (M->m_pptype == PPT_FIXED);
  return 0;
}

void Block_Rotate(GCElement *g,int centX, int centY,int rdir)
{
  int x = g->xpos + g->u.block.gwidth/2;
  int y = g->ypos + g->u.block.gheight/2;
  int nx,ny,t,i,j;

  ob_touch(g);
  nx = rotateX(x - centX,y - centY, rdir) + centX;
  ny = rotateY(x - centX,y - centY, rdir) + centY;

  t = g->u.block.gwidth;
  g->u.block.gwidth = g->u.block.gheight;
  g->u.block.gheight = t;

  g->xpos = nx - g->u.block.gwidth/2;
  g->ypos = ny - g->u.block.gheight/2;
  g->orient = (g->orient + 4 + rdir) % 4;
  
  /*
   * We need to shift the pads that the wires are attached to.
   */
  if (rdir > 0) {
    for (j = 0;j < 3;j++) {
      int offset = 4*j;
      GWire *tw;

      tw = g->wires[offset+3];
      for (i = 2;i >= 0;i--) {
	g->wires[offset+i+1] = g->wires[offset+i];
      }
      g->wires[offset] = tw;
    }
  } else {
    for (j = 0;j < 3;j++) {
      int offset = 4*j;
      GWire *tw;

      tw = g->wires[offset];
      for (i = 0;i <= 2;i++) {
	g->wires[offset+i] = g->wires[offset+i+1];
      }
      g->wires[offset+3] = tw;
    }
  }

  SetModified(MF_INTERFACE);
  SynchronizeInterface();
}

void wire_blockforce(GWire *w,int p,int retry)
{
  switch (p) {
  case BLOCK_TIN :
  case BLOCK_TOUT :
    wire_force(w,1,retry);
    break;
  case BLOCK_BIN :
  case BLOCK_BOUT :
    wire_force(w,3,retry);
    break;
  case BLOCK_LIN :
  case BLOCK_LOUT :
    wire_force(w,2,retry);
    break;
  case BLOCK_RIN :
  case BLOCK_ROUT :
    wire_force(w,0,retry);
    break;
  }
}

GWireNode *Block_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;

  if (posongate(w,w->gate,&p,&n) != 0)
    return w->nodes;
  wire_blockforce(w,p,retry);
  return w->nodes;
}

/*****************************************************************************
 *
 * Set a parameter port for an instance of a module.
 *
 * Parameters:
 *      g		Instance on which to set value
 *      name		Name of parameter
 *      value		New value of parameter
 *
 * Parameters are stored internally as an array of name/value pairs with the
 * syntax "name=value".
 *
 *****************************************************************************/
void Block_setParmPort(GCElement *g,const char *name,const char *value)
{
  char buf[STRMAX];
  int i;

  if (!name) {
    yyerror("missing parameter name on instance '%s'.",g->ename);
    return;
  }

  /*
   * If 'name' is already a parameter, just change its value.
   */
  for (i = 0;i < g->u.block.numModParms;i++) {
    int len = strlen(name);
    if (strncmp(g->u.block.modParms[i],name,len) == 0 && g->u.block.modParms[i][len] == '=') {
      if (strcmp(g->u.block.modParms[i]+len+1,value) != 0) {
	ob_free(g->u.block.modParms[i]);
	sprintf(buf,"%s=%s",name,value);
	ob_touch(g->u.block.modParms);
	g->u.block.modParms[i] = ob_strdup(buf);
	return;
      }
    }
  }

  /*
   * Add the parameter as a new parameter
   */
  ob_touch(g);

  if (g->u.block.numModParms == 0) {
    g->u.block.modParms = (char**) ob_malloc(sizeof(char*)*8,"char**");
  } else if ((g->u.block.numModParms % 8) == 0) {
    g->u.block.modParms = (char**) ob_realloc(g->u.block.modParms,sizeof(char*)*(g->u.block.numModParms+8));
  }

  sprintf(buf,"%s=%s",name,value);
  ob_touch(g->u.block.modParms);
  g->u.block.modParms[g->u.block.numModParms++] = ob_strdup(buf);
}

void Block_unsetParmPort(GCElement *g,const char *name)
{
  int i;
  for (i = 0;i < g->u.block.numModParms;i++) {
    int len = strlen(name);
    if (strncmp(g->u.block.modParms[i],name,len) == 0 && g->u.block.modParms[i][len] == '=') {
      ob_touch(g);
      ob_touch(g->u.block.modParms);
      ob_free(g->u.block.modParms[i]);
      g->u.block.modParms[i] = g->u.block.modParms[--g->u.block.numModParms];
    }
  }
}

const char *Block_getParmPort(GCElement *g,const char *name)
{
  int i;
  for (i = 0;i < g->u.block.numModParms;i++) {
    int len = strlen(name);
    if (strncmp(g->u.block.modParms[i],name,len) == 0 && g->u.block.modParms[i][len] == '=') {
      return g->u.block.modParms[i]+len+1;
    }
  }
  return 0;
}

/*****************************************************************************
 *
 * Get all wires from a pad and put them into an array.
 *
 * Parameters:
 *      g		Block or symbol block instance
 *      p		Pad number to extract (or -1 for all)
 *      wa		Array of wires for return
 *      n		Allocated length of array
 *
 *****************************************************************************/
int block_getPadWires(GCElement *g,int p,GWire **wa,int n)
{
  GWire *w;
  int count = 0;
  int isSymbol = (GCElement_getType(g) == GC_SYMBLOCK);

  if (p < 0) {
    int np = GCElement_numPads(g);
    for (p = 0;p < np;p++) {
      for (w = g->wires[p];w;w = w->next) {
	if (count < n) {
	  wa[count] = w;
	  if (isSymbol) {
	    const char *name = GCElement_getPadName(g,p);
	    ob_touch(w);
	    w->name = ob_strdup(name);
	  }
	}
	count++;
      }
    }
  } else {
    for (w = g->wires[p];w;w = w->next) {
      if (count < n) {
	wa[count] = w;
	if (isSymbol) {
	  const char *name = GCElement_getPadName(g,p);
	  ob_touch(w);
	  w->name = ob_strdup(name);
	}
      }
      count++;
    }
  }

  return count;
}

/*****************************************************************************
 *
 * Comparison function for qsort()ing array of wires by name
 *
 *****************************************************************************/
static int wireNameCmp(const void *_w1,const void *_w2)
{
  GWire *w1 = *(GWire**)_w1;
  GWire *w2 = *(GWire**)_w2;

  if (!w1->name || !w2->name) return 0;

  return strcmp(w1->name,w2->name);
}

int block_isIsomorphic_symblockports(GCElement *g1,GCElement *g2)
{
  return 0;
}

int block_isIsomorphic_blockports(GCElement *g1,GCElement *g2)
{
  int i,j,n;

  /*
   * Now check port positions
   */
  n = GCElement_numPads(g1);
  for (i = 0;i < n;i++) {
    int k = 4*(i/4) + ((i+g1->orient)%4);
    int pd1 = GCElement_getPadDir(g1,k);
    int pd2 = GCElement_getPadDir(g2,i);
    const char *pn1 = GCElement_getPadName(g1,k);
    const char *pn2 = GCElement_getPadName(g2,i);
    GWire *pw1[1024];
    GWire *pw2[1024];
    int npw1 = block_getPadWires(g1,k,pw1,1024);
    int npw2 = block_getPadWires(g2,i,pw2,1024);
    
    if (pd1 != pd2) return 0; /* Pad directions differ */
    if (npw1 != npw2) return 0; /* # wires on pad differ */

    qsort(pw1,npw1,sizeof(GWire*),wireNameCmp);
    qsort(pw2,npw2,sizeof(GWire*),wireNameCmp);

    for (j = 0;j < npw1;j++) {
      GWire *w1 = pw1[j];
      GWire *w2 = pw2[j];
      int x1,y1,x2,y2,cx,cy;

      if (w1->name && w2->name && strcmp(w1->name,w2->name) != 0) {
#if 0
	printf("port %s != %s\n",w1->name,w2->name);
#endif
	return 0;
      }

      if (GNet_getNBits(w1->net) != GNet_getNBits(w2->net)) {
#if 0
	printf("port %d != %d\n",GNet_getNBits(w1->net), GNet_getNBits(w2->net));
#endif
	return 0;
      }

      x1 = w1->nodes->x - g1->xpos - g1->u.block.gwidth/2;
      y1 = w1->nodes->y - g1->ypos - g1->u.block.gheight/2;

      cx = w2->nodes->x - g2->xpos - g2->u.block.gwidth/2;
      cy = w2->nodes->y - g2->ypos - g2->u.block.gheight/2;

      x2 = rotateX(cx,cy,g1->orient);
      y2 = rotateY(cx,cy,g1->orient);

      if (x1 != x2 || y1 != y2) {
#if 0
	printf("port %s@(%d,%d)  !=  %s@(%d,%d)\n",
	       w1->name,x1,y1,w2->name,x2,y2);
#endif
	return 0;
      }
    }
  }

  return 1;
}


/*****************************************************************************
 *
 * Test to see if g1 and g2 are isomorphic.
 *
 * Parameters:
 *       g1,g2		Elements to be compared
 *
 *****************************************************************************/
int block_isIsomorphic(GCElement *g1,GCElement *g2)
{
  int i,j,n;

  /*
   * Types must match
   */
  if (GCElement_getType(g1) != GCElement_getType(g2))
    return 0;


  if (GCElement_getType(g1) == GC_BLOCK) {
    /*
     * Normal blocks must match width/height (may be rotated though)
     */
    int width = g2->u.block.gwidth;
    int height = g2->u.block.gheight;
    
    if (((g1->orient+4-g2->orient) % 2) == 1) {
      int swap = width;
      width = height;
      height = swap;
    }

    if (g1->u.block.gheight != height)
      return 0;
    if (g1->u.block.gwidth != width)
      return 0;
  } else {
    /*
     * Symbol blocks must match symbol data
     */
    if (g1->u.block.symbol != g2->u.block.symbol)
      return 0;
  }

  /*
   * Check number of pads
   */
  if (GCElement_numPads(g1) != GCElement_numPads(g2))
    return 0;

  if (GCElement_getType(g1) == GC_BLOCK)
    return block_isIsomorphic_blockports(g1,g2);
  else
    return block_isIsomorphic_symblockports(g1,g2);
}

static int findArrayPort(GWire **pw,int npw,const char *name)
{
  int i;

  for (i = 0;i < npw;i++)
    if (pw[i] && strcmp(pw[i]->name,name) == 0)
      return i;

  return -1;
}

/*****************************************************************************
 *
 * Updates the interface of a module instance with the registered interface.
 *
 * Paramaters:
 *      g		module instance to be updated
 *      m		parent module of g
 *
 *****************************************************************************/
void block_updateInterface(GCElement *g,GModuleDef *m)
{
  GCElement *ug = env_getInterface(g);	/* Get current interface */ 
  GWire *pw[1024];
  int npw,n;
  GWire *w;
  int i;
  int draw_p = (TkGate.circuit->es->env == m);
  int px,py;
  NHash *resizeTable = 0;

  /*
   * Nothing to update to.
   */
  if (!ug) return;

  /*
   * Don't update if the existing interface matches.
   */
  if (block_isIsomorphic(g,ug)) {
    return;
  }

  if (draw_p)
    gate_draw(g,GD_NOWIRE);  

  /*
   * Get all of the ports on the old interface
   */
  npw = block_getPadWires(g,-1,pw,1024);
  qsort(pw,npw,sizeof(GWire*),wireNameCmp);


  ob_touch(g);

  /*
   * Detach all of the current wires 
   */
  n = GCElement_numPads(g);
#if 0
  printf("detaching gate at (%d,%d)\n",g->xpos,g->ypos); 
#endif
  for  (i = 0;i < n;i++) {
#if 0
    printf("  detach %s: (%d, %d)\n", g->wires[i]->name, g->wires[i]->nodes->x, g->wires[i]->nodes->y);
#endif
    g->wires[i] = 0;
  }
  for  (i = 0;i < npw;i++) {
    ob_touch(pw[i]);
    pw[i]->gate = 0;
    pw[i]->next = 0;
  }


  /*
   * Update core block parameters
   */
  g->typeinfo = ug->typeinfo;
  g->u.block = ug->u.block;
  if (GCElement_getType(g) == GC_SYMBLOCK) {
    GModSymbol_attach(g->u.block.symbol);
  }



  /*
   * Swap block size for rotation if necessary and get the relative center point (px,py)
   */
  if (GCElement_getType(ug) == GC_BLOCK) {
    g->xpos += g->u.block.gwidth/2;
    g->ypos += g->u.block.gheight/2;

    if ((g->orient % 2) != 0) {
      int swap = g->u.block.gwidth;
      g->u.block.gwidth = g->u.block.gheight;
      g->u.block.gheight = swap;
      px = ug->u.block.gheight/2;
      py = ug->u.block.gwidth/2;
    } else {
      px = ug->u.block.gwidth/2;
      py = ug->u.block.gheight/2;
    }
    g->xpos -= g->u.block.gwidth/2;
    g->ypos -= g->u.block.gheight/2;
  } else {
    px = py = 0;
  }


  /*
   * Reattach ports
   */
#if 0
  printf("attaching gate at (%d,%d)\n",g->xpos,g->ypos); 
#endif
  n = GCElement_numPads(ug);
  for  (i = 0;i < n;i++) {
    /*
     * Get pad and direction information.
     */
    GPadInfo *pad = &ug->typeinfo->Pad[i];
    int dir = GCElement_getPadDir(ug,i);
    int iodir = pad->iotype;
    int j = 4*(i/4) + ((i+g->orient)%4);

    /*
     * Do action for all ports on the canonical interface
     */
    for (w = ug->wires[i];w;w = w->next) {
      int p = findArrayPort(pw,npw,w->name);			/* Find wire on original block */
      int orient = (g->orient + w->orient) % 4;			/* Required orientation */
      int x,y;

      if (GCElement_getType(ug) == GC_BLOCK) {
	int cx = w->nodes->x - ug->xpos - ug->u.block.gwidth/2;	/* x-position relative to center */
	int cy = w->nodes->y - ug->ypos - ug->u.block.gheight/2;/* y-position relative to center */

	x = g->xpos + rotateX(cx,cy,g->orient) + px;
	y = g->ypos + rotateY(cx,cy,g->orient) + py;
      } else {
	int cx = w->nodes->x - ug->xpos;
	int cy = w->nodes->y - ug->ypos;

	x = g->xpos + rotateX(cx,cy,g->orient);
	y = g->ypos + rotateY(cx,cy,g->orient);
      }
#if 0
      printf("  attach %s: (%d, %d)\n", ug->wires[i]->name, x, y);
#endif

      if (p < 0) {
	GWire *nw;
	wire_new(m,0,&nw);
	p = block_attach(m,g,nw,nw->driver,x,y,w->name,iodir);
	net_setSize(nw->net,GNet_getNBits(w->net));
	if (draw_p)
	  GWire_draw(nw->driver);
      } else {
	if (draw_p)
	  GWire_draw(pw[p]->driver);
	
	if (((orient+4-pw[p]->orient) % 2) == 1) {
	  GWire_insertNode(pw[p]);
	}

	pw[p]->orient = orient;
	pw[p]->next = g->wires[j];
	g->wires[j] = pw[p];
	pw[p]->gate = g;

	wire_moveto(pw[p]->nodes,x,y);
	GWire_snap(pw[p]->driver);

	if (GNet_getNBits(pw[p]->net) != GNet_getNBits(w->net)) {
	  if (!resizeTable)
	    resizeTable = new_PHash_noob();
	  PHash_insert(resizeTable,pw[p]->net,(void*)GNet_getNBits(w->net));
	}

	if (draw_p)
	  GWire_draw(pw[p]->driver);

	pw[p] = 0;
      }
    }
  }

  /*
   * Delete wires that are no longer used.
   */
  for (i = 0;i < npw;i++)
    if (pw[i]) {
      if (!wire_shorten(pw[i],m,draw_p))
	GWire_draw(pw[i]->driver);
    }

  /*
   * Update sizes of any nets that have changed size
   */
  if (resizeTable) {
    HashElem *he;

    for (he = Hash_first(resizeTable);he;he = Hash_next(resizeTable,he)) {
      GNet *n = (GNet*)PHashElem_key(he);
      int size = (unsigned)HashElem_obj(he);
      GNet_draw(n);
      net_setSize(n,size);
      GNet_draw(n);
    }
    delete_PHash(resizeTable);
  }


  if (draw_p)
    gate_draw(g,GD_NOWIRE);  
}

void init_block()
{
  /*****************************************************************************
   * Register gate data.
   *****************************************************************************/
  RegisterGate(&gate_block_info);
}
