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

    Last edit by hansen on Mon Jan 19 18:18:09 2009
****************************************************************************/
/***************************************************************************
This file contains the defintion of a "symbol module instance" (SymModule).
A SymModule is a module that has a user defined bitmap as its symbol.  It
is an alternate form of a normal module instance.

Problems
  * How do we deal with a symbol, particular port arrangements, being
  modified after we have created instances of it?
  * What do we do if the symbol module definition is part of a library?
  We could load the user file first, and not yet have a symbol definition.
  * What if a user tries to change the function of a symbol module?
  * How do we handle undo/redo Pixmap data?


Handling characteristics of SymModules:
* Each SymModule has its own pointer to a GModSymbol.  Different instances of
  the same module may have different  GModSymbol definitions associated with
  them.
* Newly created SymModule instances use the GModSymbol for the corresponding 
  module definition.
*
***************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tkgate.h"

void SymBlock_Draw(GCElement *g,int md);
int SymBlock_HitDistance(GCElement *g,int x,int y);
void SymBlock_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd);
void SymBlock_Delete(GCElement *g,GModuleDef *env,int drawp);
GCElement *SymBlock_Copy(GModuleDef *,GCElement *g,int x,int y,unsigned flags);
void SymBlock_VerSave(FILE*,GCElement*);
int SymBlock_EditProps(GCElement *g,int isLoadDialog);
void SymBlock_PSWrite(GPrint *P,GModLayout*,GCElement *g);

int Block_SimHitFunc(EditState*,GCElement*);


static char *psSymBlock[] = {
  "%",
  "% A symbol block gate",
  "%",
  "/pssymblock {",
  "  startgate",
  "  1 -1 scale",
  "  {} forall translate 1 exch matrix exch image",
  "  grestore",
  "} bind def",
  0
};

GGateInfo gate_symblock_info = {
  GC_SYMBLOCK,
  "SYMMODULE",
  "symblock",0x0,
  "pssymblock",psSymBlock,
  -1,-1,

  {{0}},

  0,

  0,{{0}},
  {{0,0,CT},{0,0,CT},{0,0,CT},{0,0,CT}},
  {0,0,0,0,0,0},

  {0},
  
  Generic_Make,
  Nop_WriteCellDef,
  Generic_Init,
  SymBlock_Delete,
  SymBlock_GetExtents,
  SymBlock_HitDistance,
  SymBlock_Draw,
  Generic_Move,
  SymBlock_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Block_SimHitFunc,
  SymBlock_PSWrite,
  SymBlock_EditProps,
  SymBlock_VerSave
};


/*
 * Make a port on a gate for a module symbol
 */
void SymBlock_makePort(GCElement *g,GModSymbol *ms,GModuleDef *M,int pn)
{
  GSymPort *p = GModSymbol_getPort(ms,pn);
  GPadLoc *l = &p->msp_loc[0];
  int x = l->x1;
  int y = l->y1;
  GWire *e1,*e2;

#if 0
  printf("symbolPort %s[%d] @(%d,%d) r=%d in %s\n",
	 p->msp_name,p->msp_size,x,y,p->msp_orient,
	 M ? M->m_name : "NULL");
#endif

  wire_new(M,&e1,&e2);

  ob_touch(g);
  ob_touch(e1);
  ob_touch(e1->nodes);
  ob_touch(e2->nodes);

  net_setSize(e1->net,p->msp_size);
  g->wires[pn] = wire_append(0,e1);
  e1->orient = p->msp_orient;
  e1->gate = g;
  e1->name = ob_strdup(p->msp_name);

  e1->nodes->x = e2->nodes->x = g->xpos+x;
  e1->nodes->y = e2->nodes->y = g->ypos+y;
  switch (p->msp_orient) {
  case D_RIGHT :
    e2->nodes->x += TKGATE_STUBLEN;
    break;
  case D_LEFT :
    e2->nodes->x -= TKGATE_STUBLEN;
    break;
  case D_UP :
    e2->nodes->y -= TKGATE_STUBLEN;
    break;
  case D_DOWN :
    e2->nodes->y += TKGATE_STUBLEN;
    break;
  }
  wire_finalizeNet(e1);
}

/*
 * Convert a normal block into a SymBlock.
 *
 * Aguments:
 *    g		Block circuit element to be converted
 *    ms	Module symbol to use (or null to create default symbol)
 *    M		Module in which g is contained
 *    doWires	Non-zero if wires should be created
 */
GCElement *SymBlock_convert(GCElement *g,GModSymbol *ms,GModuleDef *M,int doWires)
{
  char *modName = g->u.block.moduleName;
  int N = GModSymbol_numPorts(ms);
  int portBytes = requiredPortMemory(N);

  ob_touch(g);

  if (!ms) ms = new_GModSymbol();

#if 0
  printf("converting %s to symblock\n",g->ename);
#endif

  /*
   * If portBytes is greater than zero, then we may need to reallocate the gate object
   * to make space for ports.
   */
  if (portBytes > 0)
    g = (GCElement *) ob_realloc(g,sizeof(GCElement)+portBytes);

  g->typeinfo = &gate_symblock_info;
  g->u.block.moduleName = modName;
  g->u.block.symbol = ms;
  GModSymbol_attach(ms);

  if (doWires) {
    int i;

    for (i = 0;i < N;i++) {
      SymBlock_makePort(g,ms,M,i);
    }
  }
#if 0
  printf("conversion complete\n");
#endif

  return g;
}

/*****************************************************************************
 *
 * Remake the ports for a gate in a module.  Normally called for the interface
 * representative when editing the interface.
 *
 * Parameters:
 *      g		Circuit element to be remade
 *      M		Module in which g is defined.
 *
 *****************************************************************************/
void SymBlock_remakePorts(GCElement *g,GModuleDef *M)
{
  GModSymbol *ms = GCElement_getSymbol(g);
  int N = GModSymbol_numPorts(ms);
  int i;

  if (!ms) return;

  gate_cutOffWires(g,M,0);

  for (i = 0;i < N;i++) {
    SymBlock_makePort(g,ms,M,i);
  }
}


void SymBlock_Draw(GCElement *g,int md)
{
  GModSymbol *ms = g->u.block.symbol;

#if 0
  printf("SymBlock_Draw(%s, %d)\n",g->ename,md);
#endif

  GModSymbol_draw(ms,g->xpos,g->ypos,g->orient,g->selected);
  gate_drawWires(g,md);

#if 0
  /*block symbol debugging*/
  {
    int minx,miny,maxx,maxy,x,y;
    char buf[STRMAX];

    SymBlock_GetExtents(g,TD_X11,&minx,&miny,&maxx,&maxy,0);
    XSetFont(TkGate.D,TkGate.instGC,TkGate.stextXF[1]);

    x = maxx+2;
    y = miny+10;

    sprintf(buf,"g=%p",g);
    RelPosDrawString(TkGate.W,TkGate.stextF[1],TkGate.instGC,x,y,buf,AtLeft);

    y+=10;
    sprintf(buf,"ms=%p",ms);
    RelPosDrawString(TkGate.W,TkGate.stextF[1],TkGate.instGC,x,y,buf,AtLeft);

    y+=10;
    sprintf(buf,"rcount=%d",ms->ms_refCount);
    RelPosDrawString(TkGate.W,TkGate.stextF[1],TkGate.instGC,x,y,buf,AtLeft);
  }
#endif
}

int SymBlock_HitDistance(GCElement *g,int x,int y)
{
  int minx,miny,maxx,maxy;

  SymBlock_GetExtents(g,TD_X11,&minx,&miny,&maxx,&maxy,0);

  if (x >= minx && x <= maxx && y >= miny && y <= maxy)
    return GATERANGE-2;
  else
    return NOHIT;
}

void SymBlock_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  GModSymbol *ms = g->u.block.symbol;

  GModSymbol_getExtents(ms,g->orient,minx,miny,maxx,maxy);

  *minx += g->xpos;
  *miny += g->ypos;
  *maxx += g->xpos;
  *maxy += g->ypos;
}

void SymBlock_Delete(GCElement *g,GModuleDef *M,int drawp)
{
  if (drawp) gate_draw(g,GD_NOWIRE);
  gate_cutOffWires(g,M,drawp);
  GModSymbol_detach(g->u.block.symbol);
  g->u.block.symbol = 0;
  if (M)
    gate_remove(M,g);
}

GCElement *SymBlock_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;

  ng = Generic_Copy(M,g,x,y,flags);
  ng->u.block.moduleName = ob_strdup(g->u.block.moduleName);
  ng->u.block.symbol = g->u.block.symbol;
  GModSymbol_attach(g->u.block.symbol);

  return ng;
}

/*****************************************************************************
 *
 * Managing function for symbol block dialog properties box
 *
 * Parameters:
 *    g			Gate that is being edited
 *    isLoadDialog	1=load values for display, 0=save modified values
 *
 *****************************************************************************/
int SymBlock_EditProps(GCElement *g,int isLoadDialog)
{
  if (isLoadDialog) {
    Generic_editPropsDLBasics(g);
    Generic_editPropsDLWires(g);
    Block_EditParmProps(g, isLoadDialog);
    Tcl_SetVar(TkGate.tcl,"edgat_dtype","none",TCL_GLOBAL_ONLY);
  } else {
    ob_touch(g);
    Generic_editPropsULWires(g);
    Generic_editPropsULBasics(g);
    Block_EditParmProps(g, isLoadDialog);

    FlagRedraw();
  }
  return 0;
}

void SymBlock_writePSSymbol(GPrint *P,GModSymbol *ms)
{
  int w = ms->ms_normal.w;
  int h = ms->ms_normal.h;
  int x,y;
  int n = w*h;
  int i,j;
  char *p;

  p = strchr(ms->ms_normal.data,'{');
  if (!p) {
    fprintf(P->p_f,"/ms%p [1 1 <00>] def\n",ms);
    return;
  }

  fprintf(P->p_f,"/ms%p [%d %d\n<",ms,w,h);

  for (i = 0;;i++) {
    unsigned code, rcode;

    while (*p && (p[0] != '0' || p[1] != 'x')) p++;
    if (!*p) break;

    sscanf(p,"0x%x",&code);

    rcode = 0;
    for (j = 0;j < 8;j++)
      if ((code & (1<<j)))
	rcode |= 1 <<(7-j);


    fprintf(P->p_f,"%02x",((~rcode) & 0xff));
    if ( ((i+1) % 32 == 0) && (i+1) < n) {
      fprintf(P->p_f,"\n");
    }
    p++;
  }

  x = ms->ms_ix[0][0];
  y = ms->ms_iy[0][0];
  fprintf(P->p_f,"> %d %d] def\n",x,y);
}

/*****************************************************************************
 *
 * Symbol block postscript generation.
 *
 *****************************************************************************/
void SymBlock_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  GModSymbol *ms = g->u.block.symbol;

  if (!PHash_find(&P->p_genSymbols,ms)) {
    PHash_insert(&P->p_genSymbols,ms,ms);

    SymBlock_writePSSymbol(P,ms);
  }

  Generic_PSLabels(P,g);
  fprintf(P->p_f,"ms%p %d %d %d %s\n",ms,g->xpos,g->ypos,-g->orient*90,
	  g->typeinfo->psprint);
}

void SymBlock_VerSave(FILE *f,GCElement *g)
{
  GWire *w;
  int i,j;
  int first_pin;
  int N = GCElement_numPads(g);
  GModSymbol *ms = g->u.block.symbol;

  fprintf(f,"  %s",g->u.block.moduleName);
  Block_VerSaveModParms(f,g);
  fprintf(f," %s",g->ename);

  first_pin = 1;
  fprintf(f," (");
  for (i = 0;i < N;i++) {
    GSymPort *msp = ms->ms_ports[i];

    for (w = g->wires[i], j=0;w;w = w->next, j++) {
      if (first_pin)
	first_pin = 0;
      else
	fprintf(f,", ");
      fprintf(f,".%s(%s)",msp->msp_name,w->net->n_signame);
    }
  }
  fprintf(f,");");

  fprintf(f,"   //: @(%d, %d) /symbol:%u",g->xpos,g->ypos,(unsigned)ms);

  if (!g->show_name) fprintf(f," /sn:%d",g->show_name);
  if (g->anchored) fprintf(f," /anc:1");

  if (N > 0) {
    fprintf(f," /w:[");
    for (i = 0;i < N;i++) {
      for (w = g->wires[i], j=0;w;w = w->next, j++) {
	fprintf(f," %d",w->nidx);
      } 
    }
    fprintf(f," ]");
  }


  fprintf(f,"\n");
}

void init_symblock()
{
  /* I don't think we need this */
  RegisterGate(&gate_symblock_info);
}
