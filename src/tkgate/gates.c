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

    Last edit by hansen on Thu Feb 12 14:49:29 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>
#include "tkgate.h"


static SHash *GateTypeHash = 0;		/* Map from new style gate type names to gateinfo */
static SHash *OldGateTypeHash = 0;	/* Map from old verilog name to gateinfo */
NHash *GateIdxHash = 0;			/* Map from gate type code to gateinfo */
Icon *INVERTER;				/* Icon for inverter buble */
Icon *Mark;				/* Icon for position mark */
Icon *SIZEHASH;				/* Icon for wire bit size hash mark */
Icon *REPCOUNTMARK;			/* Icon for replicate mark */
int GlobalElementCount = 0;		/* Global counter of gate elements */

/*****************************************************************************
 *
 * Table for IO direction marks for pins on logic blocks.  Note, these arrows
 * are also shared by concat elements.
 *
 *****************************************************************************/
static Icon *iodmark[4][4];	/* Normal arrows [orient][iodir] */
static Icon *boxmark[4][4];	/* Boxed arrows [orient][iodir] */

/*****************************************************************************
 *
 * Set the name of a gate in a module
 *
 * Parameters:
 *    g			Gate on which to change/set name.
 *    name		New name for gate.
 *    M			Module in which gate is defined (null if gate is not
 *			yet in defined in a module). 
 *
 *****************************************************************************/
void gate_setName(GCElement *g,const char *name,GModuleDef *M)
{
  char buf[1024];
  SHash *H = M ? M->m_gates : 0;

  if (name && g->ename && strcmp(name,g->ename) == 0)
    return;

  pickValidName(buf,name,"g",H);

  if (g->ename) {
    SHash_remove(H,g->ename);
    ob_free(g->ename);
  }
  ob_touch(g);
  g->ename = ob_strdup(buf);
  g->show_name = (name != 0);
  if (H)
    SHash_insert(H,g->ename,g);
}

/*****************************************************************************
 *
 * Add a gate to a module.
 *
 * Parameters:
 *    M			Module in which gate will be placed
 *    g			Gate to be added.
 *
 *****************************************************************************/
void gate_add(GModuleDef *M,GCElement *g)
{
  if (!g->ename) {
    ob_touch(g);
    gate_setName(g,0,M);
    g->show_name = 0;
  }
}

/*****************************************************************************
 *
 * Remove a gate from a module.
 *
 * Parameters:
 *    M			Module in which gate is currently defined.
 *    g			Gate to remove from module.
 *
 *****************************************************************************/
void gate_remove(GModuleDef *M,GCElement *g)
{
  SHash_remove(M->m_gates,g->ename);
}

/*****************************************************************************
 *
 * Creates a new gate object.
 *
 * Parameters:
 *    x,y		Position for new gate.
 *    rot		Initial rotation for new gate.
 *    type		Type of new gate.
 *
 * Returns:		New gate object (not yet inserted into a module)
 *
 *****************************************************************************/
GCElement *gate_new(int x,int y,int rot,int type)
{
  GGateInfo *ginfo = GGateInfo_codeLookup(type);
  GCElement *g;
  int i,n;
  int N;
  
#ifdef DEBUG
  printf("gate_new(%d %d %d  NULL)\n",x,y,type);
#endif

  g = (GCElement *) ob_malloc(sizeof(GCElement)+requiredPortMemory(ginfo->NumPads),"GCElement");

  memset(g,0,sizeof(*g));

  g->typeinfo = ginfo;
  g->tech = ob_strdup(TKGATE_DEFAULT_TECH);
  g->xpos = x;
  g->ypos = y;
  g->top = 0;
  g->bottom = 0;
  g->selected = 0;
  g->orient = g->typeinfo->Flags.CanRot ? rot : 0;
  g->ename = 0;
  g->show_name = 0;
  g->anchored = 0;
  g->cust_delay = 0;
  g->isModuleInterface = 0;
  g->cpath_cut = 0;
  g->enumb = GlobalElementCount++;
  N = GCElement_numPads(g);
  for (i = 0;i < N;i++)
    g->wires[i] = NULL;
  
  if (g->typeinfo->InitGate)
    (*g->typeinfo->InitGate)(g);

  n = 0;
  for (i = 0;g->typeinfo->delayNames[i];i++)n++;
  if (n == 0) 
    g->delays = 0;
  else {
    g->delays = (int*) ob_malloc(sizeof(int)*n,"int[]");
    for (i = 0;g->typeinfo->delayNames[i];i++)
      g->delays[i] = 1;
  }

  return g;
}

/*****************************************************************************
 *
 * Unattach a wire from a gate.
 *
 * Parameters:
 *    w			Wire to detach (gate is assumed)
 *    M			Module in which wire is defined.
 *    drawp		Non-zero if we should update the display after operation.
 *
 *****************************************************************************/
void gate_unattachwire(GWire *w,GModuleDef *M,int drawp)
{
  struct wire *wd;

  ob_touch(w);
  ob_touch(w->gate);

  wd = w->driver;
  if (M && drawp) GWire_draw(wd);
  w->gate = NULL;
  if (w->name) ob_free(w->name);
  
  w->name = NULL;
#if 0
  if (!wire_trash(w->nodes,M,drawp)) {
    w->invert = 0;
    if (M && drawp) GWire_draw(wd);
  } else {
    if (M) {
      ob_touch(M);
      M->m_wires = wire_unlink(M->m_wires,w);
    }
    wire_free(w);
  }
#else
  if (!wire_nuke(w,drawp,M)) {
    w->invert = 0;
    if (M && drawp) GWire_draw(wd);
  }
#endif
}

/*****************************************************************************
 *
 * Unattach all wires from a pad of a gate.
 *
 * Parameters:
 *    w			Head of pad list on a gate
 *    M			Module in which to perform opreration
 *    drawp		Non-zero if we should update display.
 *
 *****************************************************************************/
void gate_unattachwirelist(GWire *w,GModuleDef *M,int drawp)
{
  GWire *wnext;

  for (;w;w = wnext) {
    wnext = w->next;
    gate_unattachwire(w,M,drawp);
  }
}

/*****************************************************************************
 *
 * Cut all wires off of a gate.
 *
 * Parameters:
 *    g			Gate to strip of all wires.
 *    M			Module in which gate is defined/
 *    drawp		Non-zero if we should update display.
 *
 *****************************************************************************/
void gate_cutOffWires(GCElement *g,GModuleDef *M,int drawp)
{
  int i;
  int N = GCElement_numPads(g);

  ob_touch(g);
  for (i = 0;i < N;i++) {
    gate_unattachwirelist(g->wires[i],M,drawp);
    g->wires[i] = NULL;
  }

}

/*****************************************************************************
 *
 * Delete a gate.
 *
 * Parameters:
 *    g			Gate to be deleted.
 *    M			Module in which gate is currently defined.
 *    drawp		Non-zero if we should update the display. 
 *
 * Returns:		Non-zero on success.
 *
 * Delete a gate by unattaching all wires connected to it and removing it from
 * the object list.  This function does not actually free the object.
 *
 *****************************************************************************/
int gate_delete(GCElement *g,GModuleDef *M,int drawp)
{
#if 0
  if (g->typeinfo != (void*)0xa7a7a7a7)
    printf("gate_delete(%p: %s %s)\n",g,g->typeinfo->name,g->ename);
  else {
    printf("gate_delete(%p: bogus)\n",g);
    abort();
  }
#endif

  (*g->typeinfo->DeleteGate)(g,M,drawp);
  ob_free(g);
  /* g->typeinfo = 0; */

  return 1;
}

/*****************************************************************************
 *
 * Auxilary function to do a hit test to detect a gate.
 *
 * Parameters:
 *    x,y		Position of the "hit".
 *    M			Module to search
 *
 * Returns:		The best gate for a hit at (x,y)
 *
 *****************************************************************************/
GCElement *gate_hit(GModuleDef *M,int x,int y)
{
  int threshDist = GATERANGE;
  HashElem *gl;
  GCElement *hit = 0;

  for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);
    int dist;

    dist = (*g->typeinfo->HitDistance)(g,x,y);

    if  (dist < threshDist) {
      hit = g;
      threshDist = dist;
    }
  }

  return hit;
}

/*****************************************************************************
 *
 * Displays all the gates in a module
 *
 * Parameters:
 *    M			Module to be displayed.
 *
 *****************************************************************************/
void gate_displayall(GModuleDef *M)
{
  HashElem *gl;
  GWireList *wl;

  for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
    int MinX,MinY,MaxX,MaxY;
    GCElement *g = (GCElement*) HashElem_obj(gl);

    gate_draw(g,GD_NOWIRE);
    gate_getbbx(g,TD_X11,&MinX,&MinY,&MaxX,&MaxY);
    if (MinX < TkGate.ed->min_x) TkGate.ed->min_x = MinX;
    if (MinY < TkGate.ed->min_y) TkGate.ed->min_y = MinY;
    if (MaxX > TkGate.ed->max_x) TkGate.ed->max_x = MaxX;
    if (MaxY > TkGate.ed->max_y) TkGate.ed->max_y = MaxY;
  }

  for (wl = M->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    if (w == w->driver)
      GWire_draw(w);
  }
}

/* Draws a gate (Circuit element) and the wires connected to it. */
void gate_draw(GCElement *g,int md)
{
  if (g->typeinfo->DrawGate)
    (*g->typeinfo->DrawGate)(g,md);
  else
    Generic_Draw(g,md);
}

void gate_drawWires(GCElement *g,int md)
{
  GWire *w;
  int i;
  int N = GCElement_numPads(g);

  if (md != GD_NOWIRE) {
    for (i = 0;i < N;i++)
      for (w = g->wires[i];w;w=w->next)
	if (w->nodes->out) {
	  GWire_draw(w);
	} else {
	  if ((w->driver->gate != g) && !(w->driver->gate && md == GD_NOINWIRE))
	    GWire_draw(w->driver);
	}
  }
}

void gate_drawgatename(GCElement *g,const char *name)
{
  int x,y; 
  int o;
 
  o = g->orient;
  x = g->xpos;
  y = g->ypos;

  if (g->selected)
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textbXF[TkGate.circuit->zoom_factor]->fid);
  else
    XSetFont(TkGate.D,TkGate.instGC,TkGate.textXF[TkGate.circuit->zoom_factor]->fid);
  
  dce_DrawString(TkGate.instGC,x + g->typeinfo->lpos[o].x,
		 y + g->typeinfo->lpos[o].y,
		 g->typeinfo->lpos[o].just,name);
}

/* Moves a gate and adjusts wires */
void gate_move(GCElement *g,int dx,int dy)
{
  if (GModuleDef_isDataProtected(TkGate.circuit->es->env)) {
    message(0,msgLookup("err.protdata"));
    return;
  }

  if (g->anchored) {
    message(0,msgLookup("err.gatanchor")); /* "Gate is anchored and can not be moved." */
    return;
  }

  SetModified(MF_NET|MF_GATE);

  (*g->typeinfo->MoveGate)(g,dx,dy);
}

/*****************************************************************************
 *
 * Move an object by the specified amount (specified as a relative position)
 *
 *****************************************************************************/
void gate_moveObject(GCElement *g,int dx,int dy)
{
  if (g->anchored) {
    message(0,msgLookup("err.gatanchor"));		/* "Gate is anchored and can not be moved." */
    return;
  }

  gate_draw(g,GD_NORMAL);
  gate_move(g,dx,dy);
  gate_draw(g,GD_NORMAL);
}

void gate_moveTo(GCElement *g,int x,int y)
{
  gate_move(g,x-g->xpos,y-g->ypos);
}

/* Marks a gate */
void gate_mark(GCElement *g,int select)
{
  ob_touch(g);
  g->selected = select;
}

/*
  Gets the bounding box of a gate.
*/
void gate_getbbx(GCElement *g,TargetDev_e target, int *minx,int *miny,int *maxx,int *maxy)
{
  (*g->typeinfo->GetExtents)(g, target, minx,miny,maxx,maxy,0);
}

void displayRepCount(int x,int y,int num)
{
  char ns[30];

  Icon_draw(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(x),ctow_y(y),REPCOUNTMARK);

  sprintf(ns,"%d",num);
  dce_DrawString(TkGate.toolGC,x+10,y,BetweenTopAndBottom|AtLeft,ns);
}

void gate_doReplication(EditState *es)
{
  struct celemnt *g;
  int sx,sy,ex,ey,dx,dy,h,w,n,bd,num;
  
  g = TkGate.circuit->select;
  ZDrawLine(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(TkGate.ed->sx),ctow_y(TkGate.ed->sy),ctow_x(TkGate.ed->tx),ctow_y(TkGate.ed->ty));

  (*g->typeinfo->GetExtents)(g,TD_X11,&sx,&sy,&ex,&ey,&bd);

  w = ex-sx + bd;
  h = ey-sy + bd;

  
  sx = g->xpos;
  sy = g->ypos;
  ex = TkGate.ed->tx;
  ey = TkGate.ed->ty;
  
  if (sx == ex) {
    dx = 0;
    dy = (sy > ey) ? -h : h;
    n = abs((sy-ey)/dy);
  } else if (sy == ey) {
    dx = (sx > ex) ? -w : w;
    dy = 0;
    n = abs((sx-ex)/dx);
  } else if (abs(10*(sy-ey)/(sx-ex)) > (10*h)/w) {
    dy = (sy > ey) ? -h : h;
    dx = (dy*(sx-ex))/(sy-ey);
    n = abs((sy-ey)/dy);
  } else {
    dx = (sx > ex) ?-w : w;
    dy = (dx*(sy-ey))/(sx-ex);
    n = abs((sx-ex)/dx);
  }
  
  sx += dx;
  sy += dy;
  num = 2;
  while (n--) {
    GCElement *ng;

    displayRepCount(sx,sy,num);
    ng = (*g->typeinfo->CopyGate)(es->env,g,sx,sy,0);
    gate_draw(ng,0);
    sx += dx;
    sy += dy;
    num++;
  }
}

void gate_setinvertwirelist(GWire *nw,GWire *w)
{
  for (;nw && w;w = w->next,nw = nw->next) {
    ob_touch(nw);
    nw->invert = w->invert;
  }
}

void gate_hashrepline(EditState *es,int x,int y)
{
  GCElement *g;
  int sx,sy,ex,ey,dx,dy,bd,h,w,n,num;
  
  g = TkGate.circuit->select;

  (*g->typeinfo->GetExtents)(g,TD_X11,&sx,&sy,&ex,&ey,&bd);

  w = ex-sx + bd;
  h = ey-sy + bd;
  
  sx = g->xpos;
  sy = g->ypos;
  ex = x;
  ey = y;
  
  if (sx == ex) {
    dx = 0;
    dy = (sy > ey) ? -h : h;
    n = abs((sy-ey)/dy);
  } else if (sy == ey) {
    dx = (sx > ex) ? -w : w;
    dy = 0;
    n = abs((sx-ex)/dx);
  } else if (abs(10*(sy-ey)/(sx-ex)) > (10*h)/w) {
    dy = (sy > ey) ? -h : h;
    dx = (dy*(sx-ex))/(sy-ey);
    n = abs((sy-ey)/dy);
  } else {
    dx = (sx > ex) ? -w : w;
    dy = (dx*(sy-ey))/(sx-ex);
    n = abs((sx-ex)/dx);
  }
  
  sx += dx;
  sy += dy;
  num = 2;
  while (n--) {
    displayRepCount(sx,sy,num);
    sx += dx;
    sy += dy;
    num++;
  }
}

int GGateInfo_getPadNum(GGateInfo *gi,const char *pname)
{
  char buf[STRMAX];
  int p;

  if (sscanf(pname,"%[A-Za-z]",buf) != 1)
    return -1;

  for (p = 0;p < gi->NumPads;p++)
    if (strcmp(gi->Pad[p].Name,buf) == 0)
      return p;

  return -1;
}

/*
   Transmute a gate from one type to another.

   Warning: Only certain types of transmutations can be
   performed.
*/
void gate_transmute(GCElement *g,GGateInfo *ngi)
{
  GGateInfo *ogi = g->typeinfo;
  int oldOrient,rot,i;

  assert(ngi->NumPads == ogi->NumPads);

  ob_touch(g);

  oldOrient = g->orient;
  rot = ogi->Pad[0].Loc[oldOrient].dir;
  for (i = 0;i < 4;i++) 
    if (ngi->Pad[0].Loc[i].dir == rot) {
      g->orient = i;
      break;
    }
  assert(i != 4);

  for (i = 0;i < ngi->NumPads;i++) {
    GWire *w;
    GPadLoc *oL = &ogi->Pad[i].Loc[oldOrient];
    GPadLoc *nL = &ngi->Pad[i].Loc[g->orient];

    for (w = g->wires[i];w;w = w->next) {
      ob_touch(w->nodes);
      w->nodes->x += nL->x1 - oL->x1;
      w->nodes->y += nL->y1 - oL->y1;
    }
  }

  g->typeinfo = ngi;
}

/*****************************************************************************
 *
 * Get number of pads on a gate.
 *
 * Paramaters:
 *     g		Gate to examine for pad count
 *
 * Returns:		Number of pads on g
 *
 * Return the number of pads on a gate.  For all gate types except symbol
 * modules we get this from the gate's typeinfo object.  For symbol modules,
 * we use the symbol data.
 *
 *****************************************************************************/
int GCElement_numPads(GCElement *g)
{
  extern GGateInfo gate_symblock_info;

  if (g->typeinfo == &gate_symblock_info) {
    if (g->u.block.symbol)
      return GModSymbol_numPorts(g->u.block.symbol);
    else
      return 0;
  } else
    return g->typeinfo->NumPads;
}

GPadLoc *GCElement_getPadLoc(GCElement *g,int p,int r)
{
  extern GGateInfo gate_symblock_info;

  if (g->typeinfo == &gate_symblock_info) {
    GSymPort *sp = GModSymbol_getPort(g->u.block.symbol,p);
    return &sp->msp_loc[r];
  } else
    return &g->typeinfo->Pad[p].Loc[r];
}

int GCElement_getPadDir(GCElement *g,int p)
{
  extern GGateInfo gate_symblock_info;

  if (g->typeinfo == &gate_symblock_info) {
    GSymPort *sp = GModSymbol_getPort(g->u.block.symbol,p);
    return sp->msp_type;
  } else
    return g->typeinfo->Pad[p].iotype;
}

/*****************************************************************************
 *
 * Get the name of the pth pad.
 *
 * Parameters:
 *      g		Gate in on which we are looking up the pad name.
 *      p		Index of pad
 *
 *****************************************************************************/
const char *GCElement_getPadName(GCElement *g,int p)
{
  extern GGateInfo gate_symblock_info;

  if (g->typeinfo == &gate_symblock_info) {
    GSymPort *sp = GModSymbol_getPort(g->u.block.symbol,p);
    return sp->msp_name;
  } else
    return g->typeinfo->Pad[p].Name;
}

/*****************************************************************************
 *
 * Get the wire associated with the named pad on a gate
 *
 * Parameters:
 *      g		Gate 
 *      portName	Index of pad
 *
 *****************************************************************************/
GWire *GCElement_getPort(GCElement *g, const char *portName)
{
  int n = GCElement_numPads(g);
  GWire *w;
  int i;

  if (GCElement_getType(g) == GC_BLOCK) {
    for (i = 0;i < n;i++)
      for (w = g->wires[i];w;w = w->next)
	if (strcmp(w->name,portName) == 0)
	  return w;
  } else if (GCElement_getType(g) == GC_SYMBLOCK) {
    for (i = 0;i < n;i++) {
      GSymPort *sp = GModSymbol_getPort(g->u.block.symbol,i);
      if (strcmp(sp->msp_name, portName) == 0)
	return g->wires[i];
    }
  } else {
    GGateInfo *gi = g->typeinfo;
    int k = 0;

    i = GGateInfo_getPadNum(gi,portName);
    if (sscanf(portName+strlen(gi->Pad[i].Name),"%d",&k) != 1) k = 0;

    for (w = g->wires[i];k > 0 && w;w = w->next, k--);
    return w;
  }
  return 0;
}

int GCElement_getPadCanAdd(GCElement *g,int p)
{
  extern GGateInfo gate_symblock_info;

  if (g->typeinfo == &gate_symblock_info) {
    return 0;
  } else
    return g->typeinfo->Pad[p].CanAdd;
}

/*****************************************************************************
 *
 * Set the symbol to use for a symbol block module.
 *
 * Parameters:
 *      g		Gate on which to set symbol (must be a SYMBLOCK)
 *      ms		Symbol to set for gate.
 *
 *****************************************************************************/
void GCElement_setSymbol(GCElement *g, GModSymbol *ms)
{
  ob_touch(g);
  if (g->u.block.symbol) {
    GModSymbol_detach(g->u.block.symbol);
  }
  g->u.block.symbol = ms;
  if (ms) {
    GModSymbol_attach(ms);
  }
}

/*****************************************************************************
 *
 * Lookup function for new-style gate type names.
 *
 * Parameters:
 *    name		Name to look up.
 *
 * Returns:		Gate info object corresponding to name
 *
 *****************************************************************************/
GGateInfo *GGateInfo_lookup(const char *name)
{
  return (GGateInfo*) SHash_find(GateTypeHash,name);
}

/*****************************************************************************
 *
 * Lookup function for old-style gate type names.
 *
 * Parameters:
 *    name		Name to look up.
 *
 * Returns:		Gate info object corresponding to name
 *
 *****************************************************************************/
GGateInfo *GGateInfo_vlookup(const char *name)
{
  return (GGateInfo*) SHash_find(OldGateTypeHash,name);
}

/*****************************************************************************
 *
 * Lookup function for old-style gate type variant numbers.
 *
 * Parameters:
 *    gi		Gate info for gate type.
 *    name		Name to look up.
 *
 * Returns:		Variant number corresponding to name.
 *
 *****************************************************************************/
int GGateInfo_variantNum(GGateInfo *gi,const char *func)
{
  int vn = 0;
  char *p;
  int L = strlen(func);

  for (p = gi->vnames;p && *p;vn++,p = strchr(p+1,':')) {
    if (*p == ':') p++;
    if (strncmp(p,func,L) == 0 && (p[L] == ':' || !p[L]))
      break;
  }

  return vn;
}

/*****************************************************************************
 *
 * Registers a gate type
 *
 * Parameters:
 *    gi		Gate info object to be registered.
 *
 *****************************************************************************/
void RegisterGate(GGateInfo *gi)
{
  static int code_count = 1; 
  char buf[STRMAX],*T;

  if (gi->Code <= 0)
    gi->Code = code_count++;

  if (!gi->vnames) {
    fprintf(stderr,"No verilog name for block '%s'\n",gi->name);
    exit(1);
  }
  strcpy(buf,gi->vnames);

  for (T = strtok(buf,":");T;T = strtok(0,":")) {
    SHash_insert(OldGateTypeHash,T,gi);
  }

  SHash_insert(GateTypeHash,gi->name,gi);
  NHash_insert(GateIdxHash,gi->Code,gi);
}

/*****************************************************************************
 *
 * Initialize special icons if we are running in interactive mode.
 *
 *****************************************************************************/
void init_specials()
{
  if (TkGate.tcl) {
    Pixmap P;
    Icon *ico;

    P = Pixmap_registerFromFile("misc","misc.b");
    Mark = new_Icon(P,6,28,9,9,4,4);
    SIZEHASH = new_Icon(P,16,27,10,10,4,4);
    INVERTER = new_Icon(P,0,28,5,5,2,2);
    REPCOUNTMARK = new_Icon(P,32,0,9,9,4,4);

    /*****************************************************************************
     * Register pixmaps for arrows
     *****************************************************************************/
    P = Pixmap_registerFromFile("iodarrow","iodarrow.b");

    ico = new_Icon(P,14,0,7,5,3,2);	/* <> arrow */
    iodmark[0][TRI] = iodmark[2][TRI] = ico;

    ico = new_Icon(P,14,6,5,7,2,3);	/* v^ arrow */
    iodmark[1][TRI] = iodmark[3][TRI] = ico;

    ico = new_Icon(P,22,0,5,5,2,2);   /* x mark */
    iodmark[0][ANY] = iodmark[1][ANY] = iodmark[2][ANY] = iodmark[3][ANY] = ico;

    ico = new_Icon(P,0,0,7,5,2,3);   /* < arrow */
    iodmark[0][IN] = iodmark[2][OUT] = ico;

    ico = new_Icon(P,6,8,7,5,3,2);   /* > arrow */
    iodmark[0][OUT] = iodmark[2][IN] = ico;

    ico = new_Icon(P,8,0,5,7,2,3);   /* v arrow */
    iodmark[1][IN] = iodmark[3][OUT] = ico;

    ico = new_Icon(P,0,6,5,7,2,3);   /* ^ arrow */
    iodmark[1][OUT] = iodmark[3][IN] = ico;

    

    /*****************************************************************************
     * Register pixmaps for boxed arrows
     *****************************************************************************/
    P = Pixmap_registerFromFile("boxarrow","boxarrow.b");

    ico = new_Icon(P,24,0,11,11,5,5);	/* <> arrow */
    boxmark[0][ANY] = boxmark[0][TRI] = boxmark[2][ANY] = boxmark[2][TRI] = ico;

    ico = new_Icon(P,24,12,11,11,5,5);	/* v^ arrow */
    boxmark[1][ANY] = boxmark[1][TRI] = boxmark[3][ANY] = boxmark[3][TRI] = ico;

    ico = new_Icon(P,0,0,11,11,5,5);   /* < arrow */
    boxmark[0][IN] = boxmark[2][OUT] = ico;

    ico = new_Icon(P,12,0,11,11,5,5);   /* > arrow */
    boxmark[0][OUT] = boxmark[2][IN] = ico;

    ico = new_Icon(P,12,12,11,11,5,5);   /* v arrow */
    boxmark[1][IN] = boxmark[3][OUT] = ico;

    ico = new_Icon(P,0,12,11,11,5,5);   /* ^ arrow */
    boxmark[1][OUT] = boxmark[3][IN] = ico;

  }
}

/*****************************************************************************
 *
 * Make the basic gate type info hash tables and initialize special objects
 *
 *****************************************************************************/
void init_gateHashTables()
{
  GateTypeHash = new_SHash();
  OldGateTypeHash = new_SHash();
  GateIdxHash = new_SHash();
}

/*****************************************************************************
 *
 * Lookup the gate info object from a type code
 *
 * Parameters:
 *    T			Type code to translate to a gate info object.
 *
 *****************************************************************************/
GGateInfo *GGateInfo_codeLookup(int T)
{
  return (GGateInfo*) NHash_find(GateIdxHash,T);
}

/*****************************************************************************
 *
 * Generate type name for primitive of a particular gate type.
 *
 * Parameters:
 *    buf		Buffer to write type name to.
 *    g			Gate from which to generate a type.
 *
 * Built-in tkgate modules have a special naming convension.  Here is
 * an example:
 *
 * _GGNAND3B4D5 g1 (.Z(w1), .I0({{4}{w2}}), .I1(w3), .I2(w4));
 *
 * Here _GG is the prefix for built-in gates, the next "N" means that the
 * output is inverted, "AND" is the base name of the gate, "3" indicates 
 * the number of inputs (or outputs depending on type), "B4" indicates the
 * base bit width and "D5" indicates the delay of the gate.
 *
 * The general format is:
 *
 * _GG<inv><base-name><numport>x<bits><delay><parms>
 *
 * The meanings of the parts are:
 *
 *    <inv>		Output inverter state.  A single leading "N" means
 *			that all outputs are inverted.  If this field is
 *			omitted it means that all outputs are non-inverted.
 *			For all other cases a string such as "NPPNN_" is used
 *			to indicate the inverter status of each output port. 
 *
 *    <base-name>	This is the base name of the element.  It must be one
 *			of the tkgate defined primitive names.
 *
 *    <numport>		For primitives that have a variable number of ports on
 *			one of their pads, this is the port count.  This field
 *                      is omitted if the primitive has a fixed number of ports.
 *
 *    <bits>		This is the bit width of the gate if there are multiple
 *			bit widths and the bit width is greater than 1.  Note
 *			that if the <numport> field was omitted, the "x" preceeding
 *			this field is omitted.
 *
 *    <delay>		A delay specification prefixed by "_D" if explicit values
 *			are given, by "_T" if a technology is given or omitted
 *			if from the default technology.
 *
 *    <parms>		If a gate has parameters, this string encodes the parameter
 *			values beginning with a _P.
 *
 * Examples:
 *
 *   _GGNAND3x4_D5	3-input 4-bit wide AND gate with inverted output and
 *			delay of 5.
 *
 *   _GGOR4x8		4-input 8-bit wide OR gate with standard delay.
 *
 *   _GGOR4x8_Tmytech	4-input 8-bit wide OR gate with delay from "mytech".
 *
 *   _GGADD8_D68_70_62_64	8-bit adder with custom delay parameters of 68, 70, 62 and 64.
 *
 *   _GGCLOCK_P100_0_50	Clock with pulsewidth of 100, phase of 0 and duty cycle of 50.
 *
 * Specical case examples:
 *
 *   _GGRAM8x16		Ram with 8-bit address line and 16-bit data line.
 *
 *
 *****************************************************************************/
void GGateInfo_encodeTypeName(char *buf,GCElement *g)
{
  GGateInfo *gi = g->typeinfo;
  int npad,nbit;
  char inv[STRMAX],*iq;
  unsigned inv_flags = 0; 
  char *p;
  int i;

  /*
   * Get number of bits (or -1) if this gate has constant bit size. 
   */
  if (gi->gi_bitPad >= 0) {
    GWire *w;
    nbit = 0;
    for (w = g->wires[gi->gi_bitPad];w;w = w->next)
      if (nbit < w->net->n_nbits)
	nbit = w->net->n_nbits;
  } else
    nbit = -1;

  /*
   * Get port count variable for gate (or -1 if port count is fixed)
   */
  if (gi->gi_multiPad >= 0) {
    GWire *w;
    npad = 0;
    for (w = g->wires[gi->gi_multiPad];w;w = w->next) npad++;
  } else
    npad = -1;

  /*
   * Genrate standard prefix
   */
  p = buf;
  p += sprintf(p,"%s",TkGate.circuit->c_gatePrefix);

  /*
   * Generate output inverter sequence.
   */
  iq = inv;
  inv_flags = 0;
  for (i = 0;i < gi->NumPads;i++) {
    if (gi->Pad[i].iotype != IN) {
      GWire *w;
      for (w = g->wires[i];w;w = w->next) {
	if (w->invert) {
	  *iq++ = 'N';
	  inv_flags |= 1;
	} else {
	  *iq++ = 'P';
	  inv_flags |= 2;
	}
      }
    }
  }
  *iq = 0;
  if (inv_flags == 1)
    p += sprintf(p,"N");		/* All outputs inverted, just attach an N */
  else if (inv_flags == 3)
    p += sprintf(p,"%s_",inv);		/* Mixed inverted and non-inverted outputs */

  /*
   * Generate gate base name
   */
  p += sprintf(p,"%s",gi->name);


  /*
   * Gate type specific modifictions.  Eventually this should function should be
   * made into a a gateinfo "virtual" function.
   */
  if (strcmp(gi->name,"RAM") == 0 || strcmp(gi->name,"ROM") == 0) {
    npad = g->wires[0]->net->n_nbits;	/* Address line bits */
    nbit = g->wires[1]->net->n_nbits;	/* Data line bits */
  }

  /*
   * Generate number of ports and bits
   */
  if (npad >= 0) p += sprintf(p,"%u",npad);
  if (nbit > 1) {
    if (npad >= 0)
      p += sprintf(p,"x%u",nbit);
    else
      p += sprintf(p,"%u",nbit);
  }

  /*
   * Generate delay codes
   */
#if 0
  if (g->cust_delay) {
    for (i = 0;gi->delayNames[i];i++) {
      if (i == 0)
	p += sprintf(p,"_D");
      else
	p += sprintf(p,"_");
      p += sprintf(p,"%u",g->delays[i]);
    }
  } else {
    if (strcmp(g->tech,TKGATE_DEFAULT_TECH) != 0)
      p += sprintf(p,"_T%s",g->tech);
  }
#endif

  /*
   * Gate type specific modifictions.  Eventually this should be moved into
   * a gateinfo function.
   */
  if (strcmp(gi->name,"CLOCK") == 0) {
    p += sprintf(p,"_P%u_%u_%u",g->u.clock.omega,g->u.clock.phi,g->u.clock.duty);
  }
}

/*****************************************************************************
 *
 * Decode a gate name and return non-zero if it is a built-in type.
 *
 * Parameters:
 *    name			Name of cell from verilog file
 *    gcs			Special structure with cell name decoding.
 *
 * Returns:			Non-zero if cell name was decoded.
 *
 *
 *****************************************************************************/
int GGateInfo_decodeTypeName(const char *name,GCellSpec *gcs)
{
  char baseName[STRMAX];
  char dpSpec[STRMAX];		/* Delay an parameter specifications */
  int len = strlen(TkGate.circuit->c_gatePrefix);
  int n1,n2,i;
  int has_multipad = 0;
  int what;
  char *t;

  strcpy(gcs->gc_name,name);

  /*
   * If we don't have the right prefix, quite right now.
   */
  if (strncmp(name,TkGate.circuit->c_gatePrefix,len) != 0)
    return 0;

  /*
   * Advance past prefix and look for a complex inverter specification.
   */
  name += len;
  if (sscanf(name,"%[NP]_%s",gcs->gc_invSpec,baseName) == 2) {
    name += strlen(gcs->gc_invSpec)+1;
  } else
    *gcs->gc_invSpec = 0;		/* Normal or no inverter spec */

  /* 
   * Copy prefix stripped name
   */
  if (sscanf(name,"%[A-Za-z]%dx%d_%s",baseName,&n1,&n2,dpSpec) == 4) {
  } else if (sscanf(name,"%[A-Za-z]%d_%s",baseName,&n1,dpSpec) == 3) {
    n2 = -1;
  } else if (sscanf(name,"%[A-Za-z]_%s",baseName,dpSpec) == 2) {
    n1 = n2 = -1;
  } else if (sscanf(name,"%[A-Za-z]%dx%d",baseName,&n1,&n2) == 3) {
    *dpSpec = 0;
  } else if (sscanf(name,"%[A-Za-z]%d",baseName,&n1) == 2) {
    *dpSpec = 0;
    n2 = -1;
  } else if (sscanf(name,"%[A-Za-z]",baseName) == 1) {
    *dpSpec = 0;
    n1 = n2 = -1;
  } else
    return 0;	/* Can not decode gate type name */

  /*
   * Try to look up the gate type, if we do not find it, see if it has a leading N.
   */
  gcs->gc_info = GGateInfo_lookup(baseName);
  if (!gcs->gc_info && *baseName == 'N' && !*gcs->gc_invSpec) {
    gcs->gc_info = GGateInfo_lookup(baseName+1);
    strcpy(gcs->gc_invSpec,"N");
  }
  if (!gcs->gc_info) return 0;

  /*
   * Test to see if we have a multi-port on this type
   */
  for (i = 0;i < gcs->gc_info->NumPads;i++)
    if (gcs->gc_info->Pad[i].CanAdd) {
      has_multipad = 1;
      break;
    }

  /*
   * If the gate has a "multipad" or it is a ROM or a RAM use n1 for the multi-pad size
   * and n2 for the bit size.  In all other cases there is no multipad and we use n1
   * for the number of bits.
   */
  if (has_multipad || strcmp(gcs->gc_info->name,"ROM") == 0 || strcmp(gcs->gc_info->name,"RAM") == 0) {
    gcs->gc_multiPad = n1;
    gcs->gc_numBits = n2;
  } else {
    gcs->gc_multiPad = -1;
    gcs->gc_numBits = n1;
  }
  if (gcs->gc_numBits < 0) gcs->gc_numBits = 1;

  /*
   * Initialize the delay and parameter arrays.
   */
  gcs->gc_numParms = 0;
  for (i = 0;i < MAXPARMS;i++) {
    gcs->gc_parms[i] = 1;
  }

#if 0
  gcs->gc_numDelays = 0;
  for (i = 0;gcs->gc_info->delayNames[i];i++) {
    gcs->gc_delays[i] = 1;
  }
#endif

#if 1
  /*
   * Parse delay and parameter section.
   */
  what = 0;
  for (t = strtok(dpSpec,"_");t;t = strtok(0,"_")) {
    if (isalpha(*t))
      what = *t++;

    switch (what) {
#if 0
    case 'D' :
      if (gcs->gc_numDelays >= MAXDELAYS) break;
      if (sscanf(t,"%d",&n1) == 1)
	gcs->gc_delays[gcs->gc_numDelays++] = n1;
      break;
    case 'T' :
      {
	GDelayDef *dd;

	dd = GDelayDef_findTech(gcs->gc_info->delay_defs, t);
	if (!dd) break;
	gcs->gc_numDelays = GDelayDef_getDelaysFromCell(dd, gcs, gcs->gc_delays);
      }
      break;
#endif
    case 'P' :
      if (gcs->gc_numParms >= MAXPARMS) break;
      if (sscanf(t,"%d",&n1) == 1)
	gcs->gc_parms[gcs->gc_numParms++] = n1;
      break;
    }
  }
#endif

  return 1;
}

/*****************************************************************************
 *
 * Get icon data for displaying a directional arrow.
 *
 *****************************************************************************/
void GetPinIOMark(GWire *w,int d,int iod,int atype,int *x,int *y,Icon **arrow)
{
  int dx,dy;

  *x = w->nodes->x;
  *y = w->nodes->y;

  switch (atype) {
  case IODT_PORT :
    *arrow = iodmark[d][iod];
    dx = 4;
    dy = 5;
    break;
  case IODT_PLAIN :
    *arrow = iodmark[d][iod];
    dx = 8;
    dy = 5;
    break;
  case IODT_BOXED :
    *arrow = boxmark[d][iod];
    dx = 12;
    dy = 7;
    break;
  }

  switch (d) {
  case D_UP :
    *x += dx;
    *y -= dy;
    break;
  case D_DOWN :
    *x += dx;
    *y += dy;
    break;
  case D_LEFT :
    *x -= dy;
    *y += dx;
    break;
  case D_RIGHT :
    *x += dy;
    *y += dx;
    break;
  }
}

void DrawPinIOMark(GWire *w,int d,int iod,int atype)
{
  int x,y;
  Icon *arrow;
  GC gc;

  if (atype == IODT_PORT)
    gc = TkGate.modportGC;
  else
    gc = TkGate.instGC;

  GetPinIOMark(w,d,iod,atype,&x,&y,&arrow);
  Icon_draw(TkGate.D,TkGate.W,gc,ctow_x(x),ctow_y(y),arrow);
}
