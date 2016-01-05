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

    Last edit by hansen on Sat Feb 21 16:44:18 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "tkgate.h"

void init_and();
void init_or();
void init_xor();
void init_buffer();
void init_bufif();
void init_adder();
void init_mult();
void init_divide();
void init_register();
void init_ff();
void init_clock();
void init_switch();
void init_ground();
void init_vdd();
void init_in();
void init_out();
void init_inout();
void init_dip();
void init_mux();
void init_demux();
void init_decoder();
void init_tap();
void init_concat();
void init_lshift();
void init_rshift();
void init_arshift();
void init_roll();
void init_joint();
void init_block();
void init_symblock();
void init_ram();
void init_rom();
void init_nmos();
void init_pmos();
void init_comment();
void init_frame();
void init_led();
void init_jkff();
void init_script();

MakeMenuData *makeMenuData = 0;

extern NHash *GateIdxHash;

extern int Marked;

GWire *wire_root(GWireNode *n,int size);

const char *seekOption(const char *opt,const char **optList,int nOpts)
{
  int i;

  for (i = 0;i < nOpts;i++)
    if (strcmp(opt,optList[i]) == 0 && i+1 < nOpts)
      return optList[i+1];
  return 0;
}


/*
   Build the standard pin connection portion of a gate description
*/
#if 0
void buildStdPins(char *p,GSimState *ss, GCElement *g)
{
  struct wire *w;
  int i;
  int N = GCElement_numPads(g);

  for (i = 0;i < N;i++) {
    strcpy(p," ( "); p += 3;
    for (w = g->wires[i];w;w = w->next) {
      sprintf(p,"%s%d ",w->invert ? "~" : "",simnet_id(ss,w->driver));
      p += strlen(p);
    }
    strcpy(p," ) "); p += 3;
  }
}
#endif

GCElement *Generic_Make(EditState **es,GModuleDef *PM,int GType,
			int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  int i,j;
  GCElement *g;
  const char *Invert,*Pins;
  Invert = seekOption("-invert",options,nOptions);
  Pins = seekOption("-pins",options,nOptions);

  if (es && *es) {
    if (!(*es)->env->m_isSpecial)
      SetModified(MF_NET|MF_GATE);
  }

  g = gate_new(x,y,r,GType);
  ob_touch(g);
  gate_setName(g,Name,PM);
  gate_add(PM,g);

  if (!noWires) {
    int N = GCElement_numPads(g);
    for (i = 0;i < N;i++) {
      int Num = 0;

      if (Pins) {
	char buf[STRMAX];
	int N;
	if (sscanf(Pins,"%[^=]=%d",buf,&N) == 2 && strcmp(buf,g->typeinfo->Pad[i].Name) == 0)
	  Num = N;
      }
      if (!Num)
	Num = g->typeinfo->Pad[i].Num;

      for (j = 0;j < Num;j++) {
	if (Invert && strcmp(Invert,g->typeinfo->Pad[i].Name) == 0)
	  wire_addToGate(g,i,PM,1);
	else
	  wire_addToGate(g,i,PM,0);
      }
    }
  }
  return g;
}

void Generic_Init(GCElement*g)
{
}

void Generic_Delete(GCElement *g,GModuleDef *M,int drawp)
{
  if (drawp) gate_draw(g,GD_NOWIRE);
  gate_cutOffWires(g,M,drawp);
  if (M)     gate_remove(M,g);
}

void Generic_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  GGateInfo *gi = g->typeinfo;
  iconDimensions *id = 0;

  if (bd) *bd = 10;

  if (!gi->dim) {
    *minx = g->xpos - 5;
    *miny = g->ypos - 5;
    *maxx = g->xpos + 5;
    *maxy = g->ypos + 5;
    logError(ERL_ERROR,"Generic_GetExtents called on gate with no dimensions.");
    return;
  }
  
  if (gi->Flags.CanRot && !gi->Flags.single_icon)
    id = &gi->dim[g->orient];
  else
    id = &gi->dim[0];

  *minx = g->xpos - id->ox;
  *miny = g->ypos - id->oy;
  *maxx = g->xpos - id->ox + id->w;
  *maxy = g->ypos - id->oy + id->h;
}

void Generic_Draw(GCElement *g,int md)
{
  mk_gate(g->xpos,g->ypos,g->typeinfo,g->orient,g->selected);
  gate_drawWires(g,md);

  if (g->ename && g->show_name)
    gate_drawgatename(g,g->ename);
}

int Generic_HitDistance(GCElement *g,int x,int y)
{
  int minx,miny,maxx,maxy;
  int d;

  if (g->typeinfo->GetExtents)
    (*g->typeinfo->GetExtents)(g,TD_X11,&minx,&miny,&maxx,&maxy,0);
  else
    Generic_GetExtents(g,TD_X11,&minx,&miny,&maxx,&maxy,0);

  d = distance(x,y,g->xpos,g->ypos);
  if (x >= minx  && x <= maxx && y >= miny && y <= maxy && d > GATERANGE)
    d = GATERANGE-1;
  return d;
}

void Generic_Move(GCElement *g,int x,int y)
{
  int i;
  struct wire *w;
  int N = GCElement_numPads(g);

  ob_touch(g);
  g->xpos += x;
  g->ypos += y;
  for (i = 0;i < N;i++)
    for (w = g->wires[i];w;w = w->next)
      wire_move(w->nodes,x,y,FULL);
}

static void replicateGateWires(GCElement *ng,GCElement *g,GModuleDef *M)
{
  int i;
  GWire *w;
  int N = GCElement_numPads(g);

  ob_touch(ng);

  /*
   * Clear an existing wires on ng
   */
  for (i = 0;i < N;i++)
    ng->wires[i] = 0;

  for (i = 0;i < N;i++) {
    for (w = g->wires[i];w;w = w->next) {
      GWire *nw,*nw2;

      wire_new(M,&nw,&nw2);
      ob_touch(nw);

      ng->wires[i] = wire_append(ng->wires[i],nw);
      nw->gate = ng;

      ob_touch(nw->net);
      nw->net->n_nbits = w->net->n_nbits;
      nw->orient = w->orient;
      nw->offset.num = w->offset.num;
      nw->offset.den = w->offset.den;
      nw->invert = w->invert;

      ob_touch(nw->nodes);
      nw->nodes->x = w->nodes->x + (ng->xpos-g->xpos);
      nw->nodes->y = w->nodes->y + (ng->ypos-g->ypos);

      ob_touch(nw->nodes->out);
      nw->nodes->out->x = nw->nodes->x;
      nw->nodes->out->y = nw->nodes->y;

      if (w->name) nw->name = ob_strdup(w->name);

      switch (nw->orient) {
      case D_UP :
	nw->nodes->out->y -= TKGATE_STUBLEN;
	break;
      case D_DOWN :
	nw->nodes->out->y += TKGATE_STUBLEN;
	break;
      case D_LEFT :
	nw->nodes->out->x -= TKGATE_STUBLEN;
	break;
      case D_RIGHT :
	nw->nodes->out->x += TKGATE_STUBLEN;
	break;
      }
    }
  }

  for (i = 0;i < N;i++) {
    for (w = ng->wires[i];w;w = w->next) {
      wire_finalizeNet(w);
    }
  }
}

GCElement *Generic_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;
  char *name = 0;
  int i;

  ng = gate_new(x,y,g->orient,g->typeinfo->Code);
  ob_touch(ng);

  if (GModuleDef_findGate(M,g->ename))
    name = 0;
  else
    name = g->ename;
  gate_setName(ng,name,M);
  ng->show_name = g->show_name;

  ng->tech = ob_strdup(g->tech);
  ng->cust_delay = g->cust_delay;
  ob_touch(ng->delays);
  for (i = 0;g->typeinfo->delayNames[i];i++)
    ng->delays[i] = g->delays[i];

  if (!(flags & REP_NOWIRES))
    replicateGateWires(ng,g,M);

  return ng;
}

void Generic_AddInput(EditState *es,GCElement *g)
{
  int i;
  int N = GCElement_numPads(g);
  
  for (i = 0;i < N;i++)
    if (GCElement_getPadDir(g,i) == IN && GCElement_getPadCanAdd(g,i))
      break;
  
  if (i == N)
    return;
  
  if (es) SetModified(MF_GATE|MF_NET);
  
  gate_draw(g,GD_NORMAL);
  
  wire_addToGate(g,i,es->env,0);
  gate_draw(g,GD_NORMAL);
}

void Generic_AddOutput(EditState *es,GCElement *g)
{
  int i;
  int N = GCElement_numPads(g);
  
  for (i = 0;i < N;i++)
    if (GCElement_getPadDir(g,i) == OUT && GCElement_getPadCanAdd(g,i))
      break;
  
  if (i == N)
    return;
  
  if (es) SetModified(MF_GATE|MF_NET);
  
  gate_draw(g,GD_NORMAL);
  
  wire_addToGate(g,i,es->env,0);
  gate_draw(g,GD_NORMAL);
}

void Generic_RemovePort(EditState *es,GCElement *g,GWire *dw)
{
  int i,j;
  GWire *w;
  int isfound = 0;
  int N = GCElement_numPads(g);

  ob_touch(g);

  for (i = 0;i < N && !isfound;i++) {
    if (g->wires[i] == dw) {
      g->wires[i] = dw->next;
      isfound = 1;
    } else {
      for (w = g->wires[i], j = 0;w && w->next;w = w->next, j++) {
	if (w->next == dw) {
	  ob_touch(w);
	  w->next = dw->next;
	  isfound = 1;
	  break;
	}
      }
    }
  }

  if (!isfound) return;

  /*
   * Finish up wire destruction.
   */
  ob_touch(dw);
  dw->gate = 0;
  wire_setPadSpacing(g,i);
  wire_nuke(dw,0,TkGate.circuit->es->env);
}

void Err_RemovePort(EditState *es,GCElement *g,GWire *w)
{
}

void Err_AddInput(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.badinadd"));			/* Can't add any more inputs to selected gate. */
}

void Err_AddOutput(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.badoutadd"));		/* Can't add any more outputs to selected gate. */
}

void Err_AddInOut(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.badinoutadd"));		/* Can't add any more inout pins to selected gate. */
}

void Err_ChangePin(EditState *es,GCElement *g)
{
  message(1,msgLookup("err.badpinchg"));		/* Can't change pin types on selected gate. */
}

void Generic_DrawGateLabel(GPrint *P,GCElement *g,const char *text)
{
  HtmlFont font[1];

  PSDrawText(P,HtmlFont_init(font,FF_HELVETICA,FP_ROMAN,8),
	     g->xpos+g->typeinfo->lpos[g->orient].x,
	     g->ypos+g->typeinfo->lpos[g->orient].y,
	     text,g->typeinfo->lpos[g->orient].just);
}

void Generic_PSLabels(GPrint *P,GCElement *g)
{
  if (g->show_name && g->ename)
    Generic_DrawGateLabel(P,g,g->ename);
}

void Generic_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  Generic_PSLabels(P,g);

  fprintf(P->p_f,"%d %d %d %s\n",g->xpos,g->ypos,-g->orient*90,
	  g->typeinfo->psprint);
}

void Generic_editPropsDLWires(GCElement *g)
{
  Tcl_Interp *tcl = TkGate.tcl;
  int N = GCElement_numPads(g);
  char buf[8*STRMAX],*p;
  GWire *w;
  int i,j;

  p = buf;
  *p = 0;
  for (i = 0;i < N;i++) {
    char *dir = 0;
    switch (GCElement_getPadDir(g,i)) {
    case IN  : dir = "in"; break;
    case OUT : dir = "out"; break;
    case TRI : dir = "bidir"; break;
    }
    for (w = g->wires[i], j = 0;w;w = w->next, j++) {
      char port[STRMAX];

      /*
       * Special hack to hide the "I" port on a wire-tap element.
       */
      if (GCElement_getType(g) == GC_TAP && i == 1) continue;

      if (GCElement_getPadCanAdd(g,i))
	sprintf(port,"%s%d",GCElement_getPadName(g,i),j);
      else
	strcpy(port,GCElement_getPadName(g,i));
      p += sprintf(p," { \"%s\"",w->net->n_signame ? w->net->n_signame : "*unnamed*");
      if (w->name)
	p += sprintf(p," \"%s\"",w->name);
      else
	p += sprintf(p," \"%s\"",port);
      p += sprintf(p," \"%s\" %d ",dir,w->net->n_nbits);

      if (w->name) {
	switch (*port) {
	case 'T' : p += sprintf(p,"top"); break;
	case 'B' : p += sprintf(p,"bottom"); break;
	case 'L' : p += sprintf(p,"left"); break;
	case 'R' : p += sprintf(p,"right"); break;
	}
      }

      p += sprintf(p,"}");
    }
  }
  *p = 0;

  Tcl_SetVar(tcl,"edgat_wires",buf,TCL_GLOBAL_ONLY);
}

/*
   Process any wire operations on 'g'.  The wop list is a list of
   elements having one of the forms:

      {add I}			Add a wire to port group I
      {delete I2}		Delete 2nd wire of port group I
      {edit Pold w Pnew io n}	Edit port.  Pold is old port name, w is signal name,
      				Pnew is new port name, io is the I/O type,
				and n is the number of bits.
*/
void Generic_editPropsULWires(GCElement *g)
{
  Tcl_Interp *tcl = TkGate.tcl;
  char port[128],name[STRMAX],_newport[128],dir[128];
  const char *p;
  int bits;
  int n,i,j;
  const char *wops = Tcl_GetVar(tcl,"edgat_wops",TCL_GLOBAL_ONLY);

  if (g->typeinfo->Code == GC_TAP) return;	/* Can't edit tap ports */

  ob_touch(g);

  SetModified(MF_NET|MF_GATE);
  for (p = wops+strcspn(wops,"{");*p;p += strcspn(p,"{")) {
    if (sscanf(p,"{add %[A-Za-z]",port) == 1) {
      GGateInfo *gi = g->typeinfo;
      int N = GCElement_numPads(g);
      int pdir = ANY;

      for (i = 0;i < N;i++)
	if (strcmp(gi->Pad[i].Name,port) == 0) {
	  pdir = gi->Pad[i].iotype;
	  break;
	}

      switch (pdir) {
      case TRI :
	(*g->typeinfo->AddTri)(TkGate.circuit->es,g);
	break;
      case OUT :
	(*g->typeinfo->AddOutput)(TkGate.circuit->es,g);
	break;
      case IN :
      default :
	(*g->typeinfo->AddInput)(TkGate.circuit->es,g);
	break;
      }
    } else if (sscanf(p,"{delete %[A-Za-z]%d",port,&n) == 2) {
      int N = GCElement_numPads(g);
      GWire *w; 

      for (i = 0;i < N;i++) {
	const char *padName = GCElement_getPadName(g,i);
	if (strcmp(padName,port) == 0)
	  break;
      }

      if (i < N) {
	GWire *dw = 0;

	/*
	 * Find wire to be deleted.
	 */
	for (w = g->wires[i], j = 0;w;w = w->next, j++) {
	  if (j == n) {
	    dw = w;
	    break;
	  }
	}

	(*g->typeinfo->RemovePort)(TkGate.circuit->es,g,dw);
      }
    } else if (sscanf(p,"{edit %s %s %s %s %d",port,name,_newport,dir,&bits) == 5) {
      int N = GCElement_numPads(g);
      GWire *w = 0;
      GWire *r;

#if 0
      printf("{edit %s %s %s %s %d}\n",port,name,_newport,dir,bits);
#endif

      if (*name == '*') *name = 0;	/* Use default signal name */

      if (g->typeinfo->Code == GC_BLOCK) {
	char *q;
	char *oldport = port;
	char *newport = _newport;

	/*
	 * Trim off the leading "??:"
	 */
	q = strchr(oldport,':');
	if (q) oldport = q+1;
	q = strchr(newport,':');
	if (q) newport = q+1;

	/*
	 * Search for the named port.
	 */
	for (i = 0;i < N;i++) {
	  for (w = g->wires[i];w;w = w->next)
	    if (strcmp(w->name,oldport) == 0)
	      goto gotw;
	}
      gotw:

	if (strcmp(oldport,newport) != 0 && w) {
	  if (w->name) ob_free(w->name);
	  ob_touch(w);
	  w->name = ob_strdup(newport);
	}
      } else {
	int N = GCElement_numPads(g);
	char *q;
	
	/*
	 * Separate the port name into the base name 'port' and the
	 * quantifier index.
	 */
	q = port + strcspn(port,"0123456789");
	if (sscanf(q,"%d",&n) == 1)
	  *q = 0;
	else
	  n = 0;

	/*
	 * Search for the wire associated with the port, the
	 * selected wire will be in 'w'.
	 */
	for (i = 0;i < N;i++) {
	  const char *padName = GCElement_getPadName(g,i);
	  if (strcmp(padName,port) == 0)
	    break;
	}
	for (w = g->wires[i], j = 0;j < n;w = w->next, j++);
      }

      /*
       * Apply edits to wire w if found.
       */
      if (w) {
	r = wire_root(w->nodes,0);
	net_setSize(r->net,bits);
	if (strcmp(w->net->n_signame,name) != 0)
	  net_rename(w->net,name,GNet_getShowName(w->net));
      }
    } else {
      printf("FAILED: %s\n",p);
    }
    p++;
  }
}

void Generic_editPropsDLDelay(GCElement *g)
{
  char buf[STRMAX],*p;
  GGateInfo *gi = g->typeinfo;
  GDelayDef *dd;
  int i;

  if (!gi->delayNames[0]) {
    DoTcl("set ::edgat_dtype none");
    return;
  }

  p = buf;
  *p = 0;
  for (i = 0;gi->delayNames[i];i++) {
    p += sprintf(p," \"%s\"",gi->delayNames[i]);
    DoTcl("set ::edgat_techdelay(%s) 5",gi->delayNames[i]);
    DoTcl("set ::edgat_delayvalue(%s) %d",gi->delayNames[i],g->delays[i]);
  }

  DoTcl("set ::edgat_tech %s",g->tech);
  DoTcl("set ::edgat_delays { %s }",buf);
  if (g->cust_delay) {
    DoTcl("set ::edgat_dtype cust");
  } else {
    DoTcl("set ::edgat_dtype tech");
  }

  if (gi->delay_defs) {
    p = buf;
    *p = 0;
    for (dd = gi->delay_defs;dd;dd = dd->dd_next) {
      p += sprintf(p," %s",dd->dd_tech ? dd->dd_tech : TKGATE_DEFAULT_TECH);
    }

    DoTcl("set ::edgat_techList { %s }",buf);
  } else {
    /* NOTE: This branch is only called when tkgate is improperly configured */
    DoTcl("set ::edgat_techList { default }");
  }
}

void Generic_editPropsULDelay(GCElement *g)
{
  GGateInfo *gi = g->typeinfo;
  int i;
  const char *dtype,*tech;

  ob_touch(g);

  for (i = 0;gi->delayNames[i];i++) {
    char buf[STRMAX];
    const char *r;
    int value;

    sprintf(buf,"edgat_delayvalue(%s)",gi->delayNames[i]);
    r = Tcl_GetVar(TkGate.tcl,buf,TCL_GLOBAL_ONLY);
    if (r && sscanf(r,"%d",&value) == 1) {
      if (value < 1) value = 1;
      ob_touch(g->delays);
      g->delays[i] = value;
    }
  }

  dtype = Tcl_GetVar(TkGate.tcl,"edgat_dtype",TCL_GLOBAL_ONLY);
  tech = Tcl_GetVar(TkGate.tcl,"edgat_tech",TCL_GLOBAL_ONLY);

  if (!dtype) return;
  if (strcmp(dtype,"tech") == 0) {
    if (g->tech) ob_free(g->tech);
    g->tech = ob_strdup(tech);
    g->cust_delay = 0;
  } else if (strcmp(dtype,"cust") == 0) {
    g->cust_delay = 1;
  }
}

void Generic_editPropsDLBasics(GCElement *g)
{
  int x,y,rw,rh;
  int dbw = 440;		/* Estimated width of dialog box */
  int dbh = 410;		/* Estimated height of dialog box */

#if 0
  printf("Generic_editPropsDLBasics(%s)\n",g->typeinfo->name);
#endif
  Tcl_SetVar(TkGate.tcl,"edgat_type",g->typeinfo->name,TCL_GLOBAL_ONLY);
  Tcl_SetVar(TkGate.tcl,"edgat_name",(g->ename?g->ename:""),TCL_GLOBAL_ONLY);
  DoTcl("set ::edgat_hideName %d",!g->show_name);
  DoTcl("set ::edgat_anchor %d",g->anchored);
  DoTcl("set ::edgat_cpbreak %d",g->cpath_cut);
  DoTcl("set ::edgat_xpos %d",g->xpos);
  DoTcl("set ::edgat_ypos %d",g->ypos);

  rh = HeightOfScreen(TkGate.S);
  rw = WidthOfScreen(TkGate.S);
  x = ctow_x(g->xpos-50);
  y = ctow_y(g->ypos-50);

  DoTcl("offsetgeometry . %d %d",x,y);
  sscanf(TkGate.tcl->result,"+%d+%d",&x,&y);

  if (x < 25) x = 25;
  if (y < 25) y = 25;
  if (x + dbw > rw-25) x = rw-25-dbw;
  if (y + dbh > rh-50) y = rh-50-dbh;

  DoTcl("set ::edgat_geom +%d+%d",x,y);
}

void Generic_editPropsULBasics(GCElement *g)
{
  Tcl_Interp *tcl = TkGate.tcl;
  const char *name = Tcl_GetVar(tcl,"edgat_name",TCL_GLOBAL_ONLY);
  const char *hide = Tcl_GetVar(tcl,"edgat_hideName",TCL_GLOBAL_ONLY);
  const char *anchor = Tcl_GetVar(tcl,"edgat_anchor",TCL_GLOBAL_ONLY);
  const char *cpcut = Tcl_GetVar(tcl,"edgat_cpbreak",TCL_GLOBAL_ONLY);
  const char *xpos = Tcl_GetVar(tcl,"edgat_xpos",TCL_GLOBAL_ONLY);
  const char *ypos = Tcl_GetVar(tcl,"edgat_ypos",TCL_GLOBAL_ONLY);
  int n,x,y;

  SetModified(MF_NET|MF_GATE);

  ob_touch(g);
  
  if (*name)
    gate_setName(g,name,TkGate.circuit->es->env);

  if (sscanf(hide,"%d",&n) == 1)
    g->show_name = !n;

  if (sscanf(anchor,"%d",&n) == 1)
    g->anchored = n;

  if (sscanf(cpcut,"%d",&n) == 1)
    g->cpath_cut = n;

  if (xpos && ypos && sscanf(xpos,"%d",&x) == 1 && sscanf(ypos,"%d",&y) == 1) {
    if (x != g->xpos || y != g->ypos) {
      int a = g->anchored;
      g->anchored = 0;
      gate_moveTo(g,x,y);
      g->anchored = a;
    }
  }
}

int Generic_EditProps(GCElement *g,int isLoadDialog)
{
  if (isLoadDialog) {
    Generic_editPropsDLBasics(g);
    Generic_editPropsDLWires(g);
    Generic_editPropsDLDelay(g);
  } else {
    ob_touch(g);
    Generic_editPropsULWires(g);
    Generic_editPropsULBasics(g);
    Generic_editPropsULDelay(g);

    FlagRedraw();
  }
  return 0;
}

void Generic_VerSave(FILE *f,GCElement *g)
{
  VerilogBasicGateCall(f,g);
  VerilogBasicGateParmList(f,g);
  VerilogBasicGateComment(f,g,1);
  fprintf(f,"\n");
}

/*
 * Rotate a gate 90 degree counter-clockwise around (x,y) and adjust fixed
 * wire positions if necessary.
 */
void Generic_Rotate(GCElement *g, int centX, int centY,int rdir)
{
  int N = GCElement_numPads(g);
  int x = g->xpos;
  int y = g->ypos;
  int i,j;

  ob_touch(g);
  g->xpos = rotateX(x - centX,y - centY,rdir) + centX;
  g->ypos = rotateY(x - centX,y - centY,rdir) + centY;
  g->orient = (g->orient + 4 + rdir) % 4;

  for (i = 0;i < N;i++) {
    GWire *w = g->wires[i];
    GPadLoc *L = GCElement_getPadLoc(g,i,g->orient);
    int x1 = L->x1 + g->xpos;
    int y1 = L->y1 + g->ypos;
    int x2 = L->x2 + g->xpos;
    int y2 = L->y2 + g->ypos;

    for (j = 0;w;w = w->next, j++) {
      if (x1 == x2 && y1 == y2) {
	wire_move(w->nodes,x1-w->nodes->x,y1-w->nodes->y,VERTICAL|HORIZONTAL);
      }
    }
  }
}

GWireNode *Generic_wireSnap(GCElement *g,GWire *w,int *mod,int retry)
{
  int p,n;

  if (posongate(w,w->gate,&p,&n) == 0) {
    GPadLoc *loc = GCElement_getPadLoc(g,p,w->gate->orient);
    *mod = wire_force(w,loc->dir,retry);
  }
  return w->nodes;
}

void Nop_SimInitFunc(EditState *ss,GCElement *g,const char *path)
{
}

int Nop_SimHitFunc(EditState *ss,GCElement *g)
{
  return 0;
}


int Nop_VerSave(FILE *f,GCElement *g)
{
  return 0;
}

GCElement *Nop_Copy(EditState *es,GCElement *g,int x,int y,unsigned flags)
{
  return 0;
}

void gateinfo_iconInit(GGateInfo *gi,Pixmap P,iconDimensions *id,int boldOffset)
{
  int i;

  for (i = 0;i < 4;i++) {
    gi->icon[i] = new_Icon(P,id[i].x,id[i].y,id[i].w,id[i].h,id[i].ox,id[i].oy);
    gi->icon[i+4] = new_Icon(P,id[i].x,id[i].y+boldOffset,id[i].w,id[i].h,id[i].ox,id[i].oy);
  }
}

void gateinfo_altIconInit(GGateInfo *gi,Pixmap P,iconDimensions *id,int boldOffset)
{
  int i;

  for (i = 0;i < 4;i++) {
    gi->altIcon[i] = new_Icon(P,id[i].x,id[i].y,id[i].w,id[i].h,id[i].ox,id[i].oy);
    gi->altIcon[i+4] = new_Icon(P,id[i].x,id[i].y+boldOffset,id[i].w,id[i].h,id[i].ox,id[i].oy);
  }
}

void gateinfo_1iconInit(GGateInfo *gi,Pixmap P,iconDimensions *id,int boldOffset)
{
  int i;
  Icon *pid,*bid;

  pid = new_Icon(P,id->x,id->y,id->w,id->h,id->ox,id->oy);
  bid = new_Icon(P,id->x,id->y+boldOffset,id->w,id->h,id->ox,id->oy);

  for (i = 0;i < 4;i++) {
    gi->icon[i] = pid;
    gi->icon[i+4] = bid;
  }
}

static int keymenuent_compare(const void *pA,const void *pB)
{
  GKeyMenuEnt **A = (GKeyMenuEnt **) pA;
  GKeyMenuEnt **B = (GKeyMenuEnt **) pB;
  int x;

  x = strcmp((*A)->entry.gtag,(*B)->entry.gtag);
  if (x == 0) return (*A)->entry.order-(*B)->entry.order;
  return x;
}

/*****************************************************************************
 *
 * Find or create a top-level entry in the "Make" menu.
 *
 * Parameters:
 *    name		Name of message tag for menu entry (or NULL)
 *
 *****************************************************************************/
BuildMenuRoot *findAddRootMakeMenu(char *name)
{
  BuildMenuRoot *bmr;

  if (name) {
    bmr = (BuildMenuRoot *) SHash_find(makeMenuData->rmap,name);
    if (!bmr) {
      char buf[STRMAX];

      bmr = (BuildMenuRoot*) ob_malloc(sizeof(BuildMenuRoot),"BuildRootMenu");
      sprintf(buf,"root%d",makeMenuData->numRoot);
      bmr->mname = ob_strdup(buf);
      SHash_insert(makeMenuData->rmap,name,bmr);
      bmr->count = 0;
      bmr->kme = (GKeyMenuEnt**) ob_malloc(sizeof(GKeyMenuEnt*)*MAKEENTRYMAX,"GKeyMenuEnt*[]");
      makeMenuData->roots[makeMenuData->numRoot++] = bmr;
    }
  } else {
    bmr = (BuildMenuRoot*) ob_malloc(sizeof(BuildMenuRoot),"BuildRootMenu");
    bmr->count = 0;
    bmr->mname = 0;
    bmr->kme = (GKeyMenuEnt**) ob_malloc(sizeof(GKeyMenuEnt*)*MAKEENTRYMAX,"GKeyMenuEnt*[]");
    makeMenuData->roots[makeMenuData->numRoot++] = bmr;
  }

  return bmr;
}

char *tcl_sanitize(char *buf,char *s)
{
  char *p = buf;

  while (*s) {
    if (strchr("\\{}\"[]",*s))
      *p++ = '\\';
    *p++ = *s++;
  }
  *p = 0;

  return buf;
}

void buildMakeMenuData()
{
  HashElem *E;
  int i,j;

  if (makeMenuData) return;

  makeMenuData = (MakeMenuData*) ob_malloc(sizeof(MakeMenuData),"MakeMenuData");
  makeMenuData->rmap = new_SHash();
  makeMenuData->numRoot = 0;

  /*
   * Create the standard root menus.
   */
  findAddRootMakeMenu("gm.io");
  findAddRootMakeMenu("gm.gate");
  findAddRootMakeMenu("gm.rgate");
  findAddRootMakeMenu("gm.msi");
  findAddRootMakeMenu("gm.alu");
  findAddRootMakeMenu("gm.mem");
  findAddRootMakeMenu("gm.mod");

  /*
   * First Pass - standard entries
   * Register all root names
   * Create shortcuts
   * 
   */
  for (E = Hash_first(GateIdxHash);E;E = Hash_next(GateIdxHash,E)) {
    GGateInfo *gi = (GGateInfo*) HashElem_obj(E);

    for (i = 0;gi->cmds[i].command;i++) {
      GKeyMenuEnt *kme = &gi->cmds[i];

      if (!kme->entry.gtag) kme->entry.gtag = "--";

      if (kme->root.name) {
	BuildMenuRoot *bmr = findAddRootMakeMenu(kme->root.name);
	assert(bmr->count < MAKEENTRYMAX);
	bmr->kme[bmr->count++] = kme;
      }
    }
  }

  /*
   * Second Pass - direct top-level entries (e.g., Comment, Frame)
   * Create shortcuts
   * 
   */
  for (E = Hash_first(GateIdxHash);E;E = Hash_next(GateIdxHash,E)) {
    GGateInfo *gi = (GGateInfo*) HashElem_obj(E);

    for (i = 0;gi->cmds[i].command;i++) {
      GKeyMenuEnt *kme = &gi->cmds[i];
      if (!kme->root.name && kme->entry.name) {
	BuildMenuRoot *bmr = findAddRootMakeMenu(0);
	bmr->count = 1;
	bmr->kme[0] = kme;
      }
    }
  }


  for (j = 0;j < makeMenuData->numRoot;j++) {
    BuildMenuRoot *bmr = makeMenuData->roots[j];
    if (bmr->count == 0) continue;
    qsort(bmr->kme,bmr->count,sizeof(GKeyMenuEnt*),keymenuent_compare);
  }

#if 0
  /* Debug dump of menu */
  printf("MENU\n");
  for (j = 0;j < makeMenuData->numRoot;j++) {
    BuildMenuRoot *bmr = makeMenuData->roots[j];

    if (bmr->count == 0) continue;

    if (bmr->mname) {
      printf("  %s : %s\n",bmr->mname,bmr->kme[0]->root.name);
      for (i = 0;i < bmr->count;i++) {
	printf("     %s %s\n",bmr->kme[i]->entry.name,bmr->kme[i]->entry.gtag);
      }
    } else {
      printf("  -- : %s\n",bmr->kme[0]->entry.name);
    }
  }
#endif
}

/*****************************************************************************
 *
 * Parse a keyboard sequence for creating a gate.
 *
 * Parameters:
 *    keys[]		Array of generated key event names
 *    key_seq		Abbreviated key sequence
 *
 * Returns:		Length of key sequence.
 *
 *****************************************************************************/
int parseKeys(char keys[5][128],const char *key_seq)
{
  char buf[STRMAX],*T;
  int n = 0;

  strcpy(buf,key_seq);
  for (T = strtok(buf," ");T;T = strtok(0," ")) {
    if (strncmp(T,"Ctl-",4) == 0) {
      sprintf(keys[n++],"Control-%s",T+4);
    } else if (!T[1] && isalnum(T[0])) {
      sprintf(keys[n++],"KeyPress-%s",T);
    } else if (T[1]) {
      sprintf(keys[n++],"%s",T);
    } else {
      char *ks = 0;

      switch (T[0]) {
      case '+' : ks = "plus"; break;
      case '-' : ks = "minus"; break;
      case '*' : ks = "asterisk"; break;
      case '=' : ks = "equal"; break;
      case '/' : ks = "slash"; break;
      case '{' : ks = "braceleft"; break;
      case '}' : ks = "braceright"; break;
      case '[' : ks = "bracketleft"; break;
      case ']' : ks = "bracketright"; break;
      case '@' : ks = "at"; break;
      }
      assert(ks);
      if (ks) strcpy(keys[n++],ks);
    }
    if (n == 5) break;
  } 

  return n;
}

/*****************************************************************************
 *
 *****************************************************************************/
void makeMakeShortcuts()
{
  int i;
  HashElem *E;

  for (E = Hash_first(GateIdxHash);E;E = Hash_next(GateIdxHash,E)) {
    GGateInfo *gi = (GGateInfo*) HashElem_obj(E);

    for (i = 0;gi->cmds[i].command;i++) {
      GKeyMenuEnt *kme = &gi->cmds[i];
      char keys[5][128];
      int n;

      if (kme->accel_label_only || !kme->key_seq) continue;

      n = parseKeys(keys,kme->key_seq);
      if (n == 1) {
	DoTcl("KeyBinding::new <%s> { action Make { %s } } -perm 1",keys[0],kme->command);
      } else {
	DoTcl("KeyBinding::new <%s><%s> { action Make { %s } } -perm 1",keys[0],keys[1],kme->command);
      }
    }
  }  
}


void makeMakeMenu(const char *m)
{
  int did_root_sep = 0;
  char buf[STRMAX];
  int i,j;

  DoTcl("menu %s",m);	/* Create the menu */

  for (j = 0;j < makeMenuData->numRoot;j++) {
    BuildMenuRoot *bmr = makeMenuData->roots[j];
    
    if (bmr->count == 0) continue;

    if (bmr->mname) {
      char *cur_gtag = 0;


      DoTcl("%s add cascade -label [m %s] -menu %s.%s -underline [ul %d]",
	    m,bmr->kme[0]->root.name,m,bmr->mname,bmr->kme[0]->root.ul);
      DoTcl("menu %s.%s -tearoff 0",m,bmr->mname);
      for (i = 0;i < bmr->count;i++) {
	if (cur_gtag && strcmp(cur_gtag,bmr->kme[i]->entry.gtag) != 0)
	  DoTcl("%s.%s add separator",m,bmr->mname);
	cur_gtag = bmr->kme[i]->entry.gtag;
	DoTcl("%s.%s add command -label [m %s] -underline [ul %d] -command { action Make { %s } } -accelerator \"%s\"",
	      m,bmr->mname,bmr->kme[i]->entry.name,bmr->kme[i]->entry.ul,bmr->kme[i]->command,tcl_sanitize(buf,bmr->kme[i]->key_seq));
      }
    } else {
      if (!did_root_sep)
	DoTcl("%s add separator",m);
      did_root_sep = 1;
      DoTcl("%s add command -label [m %s] -underline [ul %d] -command { action Make { %s } } -accelerator \"%s\"",
	    m,bmr->kme[0]->entry.name,bmr->kme[0]->entry.ul,bmr->kme[0]->command,tcl_sanitize(buf,bmr->kme[0]->key_seq));
    }
  }

#if 0
  /*
   * Free temporary memory
   */
  for (E = Hash_first(H);E;E = Hash_next(H,E)) {
    BuildMenuRoot *bmr = (BuildMenuRoot*) HashElem_obj(E);
    ob_free(bmr->mname);
    ob_free(bmr->kme);
    ob_free(bmr);
  }
  delete_SHash(H);
#endif
}


/*****************************************************************************
 *
 * Generate primitive cell definition 
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
void Generic_WriteCellDef(FILE *f,GCellSpec *gcs)
{
  GCellSpec_writeBeginModule(f,gcs);
  fprintf(f,"  // Definition pending\n");
  GCellSpec_writeEndModule(f,gcs);
}

/*****************************************************************************
 *
 * Place holder for null primitive cell definition generator
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
void Nop_WriteCellDef(FILE *f,GCellSpec *gcs)
{
}

void init_gates()
{
  init_and();			/*  */
  init_or();			/*  */
  init_xor();			/*  */
  init_buffer();		/*  */
  init_bufif();			/* depends on 'buffer' */
  init_adder();			/*  */
  init_mult();			/*  */
  init_divide();		/*  */
  init_register();		/*  */
  init_ff();			/*  */
  init_clock();			/*  */
  init_switch();		/*  */
  init_ground();		/*  */
  init_vdd();			/*  */
  init_in();			/*  */
  init_out();			/* depends on 'in' */
  init_inout();			/* depends on 'in' */
  init_dip();			/*  */
  init_mux();			/*  */
  init_demux();			/*  */
  init_decoder();		/*  */
  init_tap();			/*  */
  init_concat();		/*  */
  init_lshift();		/*  */
  init_rshift();		/*  */
  init_arshift();		/*  */
  init_roll();			/*  */
  init_joint();			/*  */
  init_block();			/*  */
  init_symblock();		/*  */
  init_ram();			/*  */
  init_rom();			/*  */
  init_nmos();			/*  */
  init_pmos();			/*  */
  init_comment();		/*  */
  init_frame();			/*  */
  init_led();			/*  */
  init_jkff();			/*  */
  init_script();		/*  */

  buildMakeMenuData();
}
