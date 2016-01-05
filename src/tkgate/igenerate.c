/****************************************************************************
    Copyright (C) 1987-2005 by Jeffery P. Hansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License,for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Last edit by hansen on Mon Feb 23 20:48:29 2009
****************************************************************************/
/****************************************************************************
 *
 * This file contains functions related to the automatic module instance
 * generation functions.
 *
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "tkgate.h"

#define IGEN_DEBUG 0

#define VPITCH  16			/* Space between ports on vertical edges */
#define HPITCH  40			/* Space between ports on horizontal edges */
#define VMARGIN 16			/* Minimum spacing before labels start on vertical side */
#define HMARGIN 20			/* Minimum spacing before labels start on horizontal side */

#define NAMEPAD 10			/* Minimum padding on sides of module name label */
    
/*
 * Map an I/O type and a module side to a block port number.
 */
int blockPortNum(int iotype,int side)
{
  return ((iotype+1) % 3)*4 + ((side+3)%4);
}

/*
 * Return a string representing the side a port is on.
 */
const char *igen_sideStr(int side)
{
  switch (side) {
  case PSIDE_TOP :
    return "top";
  case PSIDE_BOTTOM :
    return "bottom";
  case PSIDE_LEFT :
    return "left";
  case PSIDE_RIGHT :
    return "right";
  }
  return "unknown"; 
}

int igen_strToSide(const char *side)
{
  if (strcmp(side,"top") == 0)
    return PSIDE_TOP;
  else if (strcmp(side,"bottom") == 0)
    return PSIDE_BOTTOM;
  else if (strcmp(side,"left") == 0)
    return PSIDE_LEFT;
  else if (strcmp(side,"right") == 0)
    return PSIDE_RIGHT;
  else
    return -1;
}

/*
 * Create a new IGPort object.  Note that we use malloc() instead of ob_malloc() since IGPort
 * objects are only used temporarilly 
 */
IGPort *new_IGPort(const char *name,int iotype,int side,int bits,int n)
{
  IGPort *igp = (IGPort*) malloc(sizeof(IGPort));
  igp->igp_name = strdup(name);
  igp->igp_iotype = iotype;
  igp->igp_side = side;
  igp->igp_bits = bits;
  igp->igp_pos = n;
  igp->igp_offset = -1;
  igp->igp_keep = 0;
  igp->igp_x = 0; 
  igp->igp_y = 0;
  return igp;
}

void delete_IGPort(IGPort *igp)
{
  free(igp->igp_name);
  free(igp);
}


const char *igp_getName(const IGPort *igp)
{
  return igp->igp_name;
}

int igp_getSize(const IGPort *igp)
{
  return igp->igp_bits;
}

int igp_getPos(const IGPort *igp)
{
  return igp->igp_pos;
}

void igp_setPos(IGPort *igp,int pos)
{
  igp->igp_pos = pos;
}

int igp_getType(const IGPort *igp)
{
  return igp->igp_iotype;
}

int igp_getSide(const IGPort *igp)
{
  return igp->igp_side;
}

const char *igp_getTypeStr(const IGPort *igp)
{
  return iotypeStr(igp->igp_iotype);
}

const char *igp_getSideStr(const IGPort *igp)
{
  return igen_sideStr(igp->igp_side);
}

/*
 * Compare two ports for the purpose of sorting.  Ports on the same side of the module
 * are sorted first, then the name is sorted.  Names are sorted alphebetically with the
 * following exceptions:
 *
 * 1) case insensitive
 * 2) leading '_' is ignored (but will be placed after an identical name without '_')
 * 3) If a name is of the form 'AAnn' where 'AA' is a string of letters, and 'nn' is
 *    a string of numbers, then the 'nn' part will be sorted in numeric order.  For
 *    example, you will get the order: F7, F8, F9, F10, F11, F12 instead of the
 *    strict alphnumeric order: F10, F11, F12, F7, F8.
 */
static int alpha_port_compare(const void *vA,const void *vB)
{
  const IGPort *A = *(IGPort**)vA;
  const IGPort *B = *(IGPort**)vB;
  const char *nameA,*endA;
  const char *nameB,*endB;
  int numA;
  int numB;
  int negA = 0, negB = 0;
  int c;

  /*
   * Sort by side first
   */
  if (igp_getSide(A) != igp_getSide(B))
    return igp_getSide(A) - igp_getSide(B);

  /*
   * Get names and position to first character after the '_'.
   */
  nameA = igp_getName(A);
  nameB = igp_getName(B);
  if (*nameA == '_') { negA = 1; nameA++; }
  if (*nameB == '_') { negB = 1; nameB++; }

  /*
   * The 'endX' pointers point to null at the end of the string, or to the first of the
   * terminating sequence of digits. 
   */
  endA = nameA + strlen(nameA);
  endB = nameB + strlen(nameB);
  while (endA > nameA && isdigit(endA[-1])) { endA--; }
  while (endB > nameB && isdigit(endB[-1])) { endB--; }

  /*
   * If the non-numeric parts are of unequal length, then do a standard comparison.
   */
  if ( (endA-nameA) != (endB-nameB) ) {
    c = strcasecmp(nameA,nameB);
    if (c == 0) return negA - negB;
    return c;
  }

  /*
   * Compare non-numeric part.  If not an exact match then return which is greater.
   */
  c = strncasecmp(nameA,nameB,endA-nameA);
  if (c != 0)
    return c;

  if (negA != negB) {
    return negA - negB;
  }

  if (sscanf(endA,"%d",&numA) == 0)
    return 1;
  if (sscanf(endB,"%d",&numB) == 0)
    return -1;

  return numA - numB;
}

/*
 * Compare ports based on side and position.
 */
static int port_compare(const void *va,const void *vb)
{
  IGPort *A = *(IGPort**)va;
  IGPort *B = *(IGPort**)vb;

  /*
   * Sort by side first
   */
  if (igp_getSide(A) != igp_getSide(B))
    return igp_getSide(A) - igp_getSide(B);

  /*
   * Sort by offset second
   */
  return A->igp_offset - B->igp_offset;
}

void igen_sortPorts(SHash *phash)
{
  IGPort **port_list;
  int N = Hash_numElems(phash);
  int i = 0;
  HashElem *E;

  if (N == 0) return;

  port_list = malloc(N*sizeof(IGPort*));


  for (E = Hash_first(phash);E;E = Hash_next(phash,E)) {
    port_list[i++] = (IGPort*) HashElem_obj(E);
  }

  qsort(port_list,N,sizeof(IGPort*),alpha_port_compare);

  for (i = 0;i < N;i++)
    igp_setPos(port_list[i],i);

  free(port_list);
}

/*
 * Get a sorted array of IGPort elements generated from the 
 * current module interface.  The length of the array is
 * stored in N.  The returned value, if non-NULL should
 * be freed with free()
 */
IGPort **igen_getInterfacePorts(GModuleDef *M,int *N)
{
  GCElement *g = M->m_interface;
  IGPort **portList;
  int npads,numports,i,n;

  *N = 0;

  if (!g) return 0;

  /*
   * Count the total number of ports on the interface
   */
  numports = 0;
  npads = GCElement_numPads(g);
  for (i = 0;i < npads;i++) {
    GWire *w;
    for (w = g->wires[i];w;w = w->next) numports++;
  }
  if (numports == 0) return 0;
  portList = (IGPort**) malloc(sizeof(IGPort*)*numports);

  /*
   * Initialize the current port number
   */
  n = 0;

  /*
   * Collect port data.
   */
  for (i = 0;i < npads;i++) {
    /*    const char *padName = GCElement_getPadName(g,i);*/
    int iotype = GCElement_getPadDir(g,i);	/* iotype for this pad */
    GWire *w;

    /*
     * Extract the port data from the interface
     */
    for (w = g->wires[i];w;w = w->next) {
      /*      portList[n] = new_IGPort(padName,iotype,w->orient,w->net->nbits,n);*/
      portList[n] = new_IGPort(w->name,iotype,w->orient,w->net->n_nbits,n);

      if (GCElement_getType(g) == GC_BLOCK) {
	if (w->orient == PSIDE_RIGHT || w->orient == PSIDE_LEFT)
	  portList[n]->igp_offset = (g->u.block.gheight*w->offset.num)/w->offset.den;
	else
	  portList[n]->igp_offset = (g->u.block.gwidth*w->offset.num)/w->offset.den;
      } else
	portList[n]->igp_offset = 0;
      n++;
    }
  }

  /*
   * Sort ports by their position along edge
   */
  qsort(portList,numports,sizeof(portList[i]),port_compare);

  /*
   * Return the number of ports.
   */
  *N = numports;

  return portList;
}

/*
 * Get the list of ports used in module
 */
static IGPort **igen_getModulePorts(GModuleDef *M,int *N)
{
  SHash *phash = new_SHash();
  IGPort **portList;
  HashElem *E;
  int i;

  *N = 0;

  /*
   * Get ports used in module definition.
   */
  if (GModuleDef_getType(M) == MT_NETLIST) {
    for (i = 0, E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E), i++) {
      GCElement *g = (GCElement*) HashElem_obj(E);
      GWire *w = 0;
      int side = 0;
      int iotype = 0;

      switch (g->typeinfo->Code) {
      case GC_LOGICIN :
	w = g->wires[0];
	side = 2;
	iotype = IN;
	break;
      case GC_LOGICOUT :
	w = g->wires[0];
	side = 0;
	iotype = OUT;
	break;
      case GC_LOGICTRI :
	w = g->wires[0];
	side = 0;
	iotype = TRI;
	break;
      }
      if (w && !SHash_find(phash,w->net->n_signame)) {
	IGPort *igp = new_IGPort(w->net->n_signame,iotype,side,w->net->n_nbits,i++);
	SHash_insert(phash,w->net->n_signame,igp);
      }
    }
  } else if (GModuleDef_getType(M) == MT_TEXTHDL) {
    for (i = 0, E = Hash_first(M->m_nets);E;E = Hash_next(M->m_nets,E), i++) {
      GNet *n = (GNet*) HashElem_obj(E);
      int side = -1;
      int iotype = 0;

      switch (n->n_dtype) {
      case NT_INPUT :
	side = 2;
	iotype = IN;
	break;
      case NT_OUTPUT :
	side = 0;
	iotype = OUT;
	break;
      case NT_INOUT :
	side = 0;
	iotype = TRI;
	break;
      }
      if (side >= 0 && !SHash_find(phash,n->n_signame)) {
	IGPort *igp = new_IGPort(n->n_signame,iotype,side,n->n_nbits,i++);
	SHash_insert(phash,n->n_signame,igp);
      }
    }
  }

  *N = Hash_numElems(phash);

  if (*N == 0)
    return 0;

  portList = (IGPort**) malloc(sizeof(IGPort*)* *N);

  for (i = 0, E = Hash_first(phash);E;E = Hash_next(phash,E), i++) {
    portList[i] = (IGPort*) HashElem_obj(E);
  }

  /*
   * Sort ports by their position along edge
   */
  qsort(portList,*N,sizeof(portList[i]),alpha_port_compare);


  /*
   * Free up temporary memory from the hash table
   */
  delete_SHash(phash);

  return portList;
}

void igen_freePortList(IGPort **portList,int n)
{
  int i;

  for (i = 0;i < n;i++) {
    if (portList[i])
      delete_IGPort(portList[i]);
  }

  free(portList);

}


/*
 * Transfer port information from module interface to port list window. 
 */
int igen_load(GModuleDef *M,char *aname)
{
  Tcl_Interp *tcl = TkGate.tcl;
  GCElement *g = M->m_interface;
  IGPort **portList;
  char ploc[STRMAX];
  char buf[STRMAX];
  int i,n;

  if (g && GCElement_getType(g) == GC_SYMBLOCK) 
    Tcl_SetVar2(tcl,aname,"itype","symbol",TCL_GLOBAL_ONLY);
  else
    Tcl_SetVar2(tcl,aname,"itype","block",TCL_GLOBAL_ONLY);

  /*
   * Get the port list.
   */
  portList = igen_getInterfacePorts(M,&n);
  if (!portList) {
    /*
     * No ports on this instance.
     */
    Tcl_SetVar2(tcl,aname,"numports","0",TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"ok","1",TCL_GLOBAL_ONLY);
    return 0;
  }

  /*
   * Transfer data to tcl/tk side
   */
  for (i = 0;i < n;i++) {
    IGPort *igp = portList[i];

    sprintf(ploc,"port:%d",i);
    sprintf(buf,"%s %s %d %s",
	    igp_getName(igp),
	    igp_getTypeStr(igp),
	    igp_getSize(igp),
	    igp_getSideStr(igp));
    Tcl_SetVar2(tcl,aname,ploc,buf,TCL_GLOBAL_ONLY);
  }

  igen_freePortList(portList,n);

  sprintf(buf,"%d",n);
  Tcl_SetVar2(tcl,aname,"numports",buf,TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"ok","1",TCL_GLOBAL_ONLY);

  return 0;
}

int getSideLabelSizes(IGPort **ports,int numPorts,int *maxWidth,int *totalWidth)
{
  int j;

  if (maxWidth) *maxWidth = 0;
  if (totalWidth) *totalWidth = 0;
  for (j = 0;j < numPorts;j++) {
    IGPort *igp = ports[j];
    const char *name = igp_getName(igp);
    int w = GKTextWidth(TkGate.stextbXF[1],name,strlen(name));
    if (maxWidth && w > *maxWidth) *maxWidth = w;
    if (totalWidth) *totalWidth = w;
  }
  return 0;
}

/*
 * Get data pertaining to user defined symbols.
 */
int igen_getIcon(GModuleDef *M,char *aname)
{
  Tcl_Interp *tcl = TkGate.tcl;
  GCElement *g = M->m_interface;
  GModSymbol *ms = 0;
  int i;
  char buf[STRMAX];

  if (g)
    ms = g->u.block.symbol;

#if 0
  sel_clear(TkGate.circuit->es,1);
#endif

  if (ms) {
    const char *data;
    int x,y;

    data = GModSymbol_getNormalIcon(ms,&x,&y);
    Tcl_SetVar2(tcl,aname,"normalIcon",(char*)data,TCL_GLOBAL_ONLY);
    sprintf(buf,"%d %d",x,y);
    Tcl_SetVar2(tcl,aname,"normalOffset",buf,TCL_GLOBAL_ONLY);

    data = GModSymbol_getSelectIcon(ms,&x,&y);
    Tcl_SetVar2(tcl,aname,"selectIcon",(char*)data,TCL_GLOBAL_ONLY);
    sprintf(buf,"%d %d",x,y);
    Tcl_SetVar2(tcl,aname,"selectOffset",buf,TCL_GLOBAL_ONLY);

    for (i = 0;i < GModSymbol_numPorts(ms);i++) {
      char pname[STRMAX],pvalue[STRMAX];
      GSymPort *msp = GModSymbol_getPort(ms,i);
      const char *ptype = iotypeStr(msp->msp_type);

      sprintf(pname,"pplace:%d",i);
      sprintf(pvalue,"%s %d %d %s %d %d",msp->msp_name,
	      msp->msp_x,msp->msp_y,ptype,msp->msp_orient,msp->msp_size);
      Tcl_SetVar2(tcl,aname,pname,pvalue,TCL_GLOBAL_ONLY);
    }
    sprintf(buf,"%d",GModSymbol_numPorts(ms));
    Tcl_SetVar2(tcl,aname,"numPPlace",buf,TCL_GLOBAL_ONLY);
  } else {
    Tcl_SetVar2(tcl,aname,"normalIcon","",TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"normalOffset","0 0",TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"selectIcon","",TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"selectOffset","0 0",TCL_GLOBAL_ONLY);
    Tcl_SetVar2(tcl,aname,"numPPlace","0",TCL_GLOBAL_ONLY);
  }

  return 0;
}

/*
 * Store data pertaining to user defined symbols.
 */
int igen_putIcon(GModuleDef *M,char *aname,char *icon)
{
  Tcl_Interp *tcl = TkGate.tcl;
  const char *data;
  const char *r;
  GModSymbol *ms;
  GCElement *g = M->m_interface;
  int i;
  int N = 0;
  int x,y;
  int isnew = 0;

  sel_clear(TkGate.circuit->es,0);	/* caller is responsible for redisplay. */

  ms = GCElement_getSymbol(g);
  if (!ms || ms->ms_refCount > 2) {
    ms = new_GModSymbol();
    GCElement_setSymbol(g, ms);
#if IGEN_DEBUG
    printf("igen_putIcon %s %s rcount=%d [NEW]\n",aname,icon,ms->ms_refCount);
#endif
    isnew = 1;
  } else {
#if IGEN_DEBUG
    printf("igen_putIcon %s %s rcount=%d\n",aname,icon,ms->ms_refCount);
#endif
  }


  if (isnew || !icon || strcmp(icon,"normal") == 0) {
    data = Tcl_GetVar2(tcl,aname,"normalIcon",TCL_GLOBAL_ONLY);
    r = Tcl_GetVar2(tcl,aname,"normalOffset",TCL_GLOBAL_ONLY);
    if (data && r && sscanf(r,"%d %d",&x,&y) == 2)
      GModSymbol_setNormalIcon(ms,data,x,y);
  }

  if (isnew || !icon || strcmp(icon,"select") == 0) {
    data = Tcl_GetVar2(tcl,aname,"selectIcon",TCL_GLOBAL_ONLY);
    r = Tcl_GetVar2(tcl,aname,"selectOffset",TCL_GLOBAL_ONLY);
    if (data && r && sscanf(r,"%d %d",&x,&y) == 2)
      GModSymbol_setSelectIcon(ms,data,x,y);
  }

  GModSymbol_flushPorts(ms);
  r = Tcl_GetVar2(tcl,aname,"numPPlace",TCL_GLOBAL_ONLY);
  if (r) sscanf(r,"%d",&N);

  for (i = 0;i < N;i++) {
    char pname[STRMAX];

    sprintf(pname,"pplace:%d",i);
    r = Tcl_GetVar2(tcl,aname,pname,TCL_GLOBAL_ONLY);
    if (r) {
      char name[STRMAX],ptype[STRMAX];
      int x,y,orient,bsize;

      if (sscanf(r,"%s %d %d %s %d %d",
		 name,&x,&y,ptype,&orient,&bsize) == 6) {
	GModSymbol_addPort(ms,new_GSymPort(name,x,y,strIOType(ptype),orient,bsize));
      }
    }
  }

  SymBlock_remakePorts(g,TkGate.circuit->mid_mod);
  modint_syncDisplay(M);

#if 0
  printf("## icon=%s  mm=%p[%s]  md=%p[%s]  M=%p[%s]\n",icon, 
	 TkGate.circuit->mid_mod,  TkGate.circuit->mid_mod->m_name, 
	 TkGate.circuit->mid_display,  TkGate.circuit->mid_display->m_name, 
	 TkGate.circuit->es->env,TkGate.circuit->es->env->m_name);
#endif
  if (icon && strcmp(icon,"select") == 0 && TkGate.circuit->mid_display == TkGate.circuit->es->env) {
    EditState *es = TkGate.circuit->es;
    GCElement *g = modint_findDisplay(M->m_name);

    if (g) {
      sel_appendGate(es,g,0);
      sel_finish(es);
      ob_touch(TkGate.circuit);
      TkGate.circuit->select = g;
    }
  }
  

  //  SetModified(MF_INTERFACE);
  FlagRedraw();

  return 0;
}

/*****************************************************************************
 *
 * Set port position information for ports on symbol blocks. 
 *
 *****************************************************************************/
static void igen_setSymbolPortPositions(GModuleDef *M,IGPort **portList,int numPorts)
{
  GCElement *g = M->m_interface;
  GModSymbol *ms = GCElement_getSymbol(g);
  int i,j;

  if (!ms) return;		/* No position information is available */

  for (i = 0;i < numPorts;i++) {
    IGPort *p = portList[i];

    for (j = 0;j < ms->ms_numPorts;j++) {
      GSymPort *msp = ms->ms_ports[j];
      if (strcmp(p->igp_name,msp->msp_name) == 0) {
	p->igp_keep = 1;
	p->igp_x = msp->msp_x;
	p->igp_y = msp->msp_y;
      }
    }
  }
}

/*
 * Set the port position for 'igp'.
 */
static void igen_positionBlockPort(IGPort *igp,int sideCount,IGPort **ports,int n)
{
  int i;

  igp->igp_offset = -1;		/* position is unassigned */

  for (i = 0;i < n;i++) {
    if (igp->igp_side == ports[i]->igp_side) {
      if (sideCount-- == 0) {
	igp->igp_offset = ports[i]->igp_offset;
	return;
      }
    }
  }
}

/*
 * Set the positions of all ports based on the interface position.
 */
static void igen_setBlockPortPositions(GModuleDef *M,IGPort **portList,int numPorts)
{
  int numInstPorts = 0;
  IGPort **instPorts = 0;			/* Interface ports from module instance */
  int sideCount[PSIDE_MAX];			/* How many ports we have positioned on each side */
  int i;

  for (i = 0;i < PSIDE_MAX;i++) sideCount[i] = 0;

  /*
   * Get interface ports from module interface.
   */
  instPorts = igen_getInterfacePorts(M,&numInstPorts);

  for (i = 0;i < numPorts;i++) {
    int side = portList[i]->igp_side;
    igen_positionBlockPort(portList[i],sideCount[side]++,instPorts,numInstPorts);
  }

  /*
   * Free the instance ports
   */
  if (instPorts) igen_freePortList(instPorts,numInstPorts);
}

/*****************************************************************************
 *
 * Helping function for igen_generate to get the instance ports
 *
 * Parameters:
 *     M			Module for which to generate instance port list
 *     aname			Name of tcl variable with interface data
 *     keep			Non-zero if we are keeping positions
 *     *numPorts		Return for number of ports found.
 *
 *****************************************************************************/
static IGPort **igen_generate_getInstPorts(GModuleDef *M,const char *aname,int *numPorts)
{
  IGPort **portList = 0;
  const char *r;
  int i;

  /*
   * Get port count data for module
   */
  r = Tcl_GetVar2(TkGate.tcl,aname,"numports",TCL_GLOBAL_ONLY);
  if (!r || sscanf(r,"%d",numPorts) != 1)
    return 0;

  if (*numPorts > 0)
    portList = (IGPort**) malloc((*numPorts)*sizeof(IGPort*));

  /*
   * Get ports and put them in the portlist
   */
  for (i = 0;i < *numPorts;i++) {
    char pid[STRMAX],name[STRMAX],iodirstr[STRMAX],sidestr[STRMAX];
    int bits,side,iodir;

    /*
     * Get port data and create an IGPort for it in portList, and compute
     * its position.
     */
    sprintf(pid,"port:%d",i);
    r = Tcl_GetVar2(TkGate.tcl,aname,pid,TCL_GLOBAL_ONLY);
    if (!r || sscanf(r,"%s %s %d %s",name,iodirstr,&bits,sidestr) != 4) return 0;
    iodir = strIOType(iodirstr);
    side = igen_strToSide(sidestr);
    portList[i] = new_IGPort(name,iodir,side,bits,i);
  }

  return portList;
}

/*****************************************************************************
 * Helping function for igen_generate() to extend the portList to include ports
 * from the module definition.  Only ports with names not already in the port
 * list are added.
 *****************************************************************************/
static IGPort **igen_generate_getModPorts(GModuleDef *M,IGPort **portList,int *numPorts)
{
  int numModPorts;
  IGPort **modPorts = igen_getModulePorts(M,&numModPorts); 
  SHash *phash;
  int i;

  /*
   * If we have nothing to add, then just return
   */
  if (numModPorts <= 0) return portList;

  /*
   * If there are no ports in the portList, then return all of the module ports
   * we found.
   */
  if (!portList) {
    *numPorts = numModPorts;
    return modPorts;
  }

  /*****************************************************************************/
  /***** We now know we have both existing ports and new module ports	    ****/
  /***** so we must now merge them.					    ****/
  /*****************************************************************************/


  /*
   * Create a hash table to remember the names of the existing inteface ports.
   */
  phash = new_SHash();

  /*
   * Expand the portlist array for the worst case
   */
  portList = (IGPort**) realloc(portList,(*numPorts+numModPorts)*sizeof(IGPort*));

  /*
   * Record the names of already existing ports
   */
  for (i = 0;i < *numPorts;i++)
    SHash_insert(phash,portList[i]->igp_name,portList[i]);

  /*
   * Merge any module ports that were not existing instance ports
   */
  for (i = 0;i < numModPorts;i++) {
    if (!SHash_find(phash,modPorts[i]->igp_name)) {
      portList[(*numPorts)++] = modPorts[i];
      modPorts[i] = 0;
    }
  }

  /*
   * Free the module ports.
   */
  igen_freePortList(modPorts,numModPorts);

  delete_SHash(phash);

  return portList;
}

/*****************************************************************************
 *
 * Helping function for igen_generate() to set positions for ports that are
 * currently unpositioned on a side.  We assume that any unpositioned ports
 * are at the end of the list after any positioned ports.
 *
 *****************************************************************************/
void igen_generate_positionPorts(IGPort **portList,int numPorts,int side)
{
  int currentPos = -1;
  int i;
  int margin, pitch;

  if (side == PSIDE_LEFT || side == PSIDE_RIGHT) {
    margin = VMARGIN;
    pitch = VPITCH;
  } else {
    margin = HMARGIN;
    pitch = HPITCH;
  }

  for (i = 0;i < numPorts;i++) {
    IGPort *igp = portList[i];

    if (igp_getSide(igp) != side) continue;

    if (igp->igp_offset < 0) {
      if (currentPos < 0)
	currentPos = margin;
      else
	currentPos += pitch;
      igp->igp_offset = currentPos;
    } else  if (igp->igp_offset > currentPos) {
      currentPos = igp->igp_offset;
    }
  }
}

/*****************************************************************************
 *
 * Generate module symbol
 *
 * Parameters:
 *      M		Module to generate symbol for
 *      aname		Tcl array for passing interface data
 *      which		Source of port data ("int", "mod" or "modint")
 *      keep		If non-zero port positions will be kept if possible.
 *
 *****************************************************************************/
static int igen_generate_symbol(GModuleDef *M,const char *aname,const char *which,int keep)
{
  GCElement *g = M->m_interface;
  GModSymbol *ms = GCElement_getSymbol(g);
  IGPort **portList = 0;
  int numPorts = 0;
  int i,r;
  int x,y;
  int minx,miny,maxx,maxy;

  EditState_unselectGate(0);

  /*
   * We are including current interface ports
   */
  if (strcmp(which,"int") == 0 || strcmp(which,"modint") == 0) {
    portList = igen_generate_getInstPorts(M,aname,&numPorts);
  }

  /*
   * Get module ports if necessary
   */
  if (strcmp(which,"mod") == 0 || strcmp(which,"modint") == 0) {
    portList = igen_generate_getModPorts(M,portList,&numPorts);
  }

  if (keep)
    igen_setSymbolPortPositions(M,portList,numPorts);


  /*
   * Get a module symbol object for the module interface using the following rules:
   *    1) If the module does not have a symbol interface, create one.
   *    2) If the reference count of the existing interface is greater than 2, copy it.
   *    3) Otherwise, use the existing module symbol.
   */
  if (!ms) {
    ms = new_GModSymbol();
#if IGEN_DEBUG
    printf("igen_generate_symbol %s rcount=%d [NEW]\n",aname,ms->ms_refCount);
#endif
  } else if (ms->ms_refCount > 2) {
    ms = copy_GModSymbol(ms);
    GCElement_setSymbol(g, ms);
#if IGEN_DEBUG
    printf("igen_generate_symbol %s rcount=%d [COPY]\n",aname,ms->ms_refCount);
#endif
  } else {
#if IGEN_DEBUG
    printf("igen_generate_symbol %s rcount=%d [REUSE]\n",aname,ms->ms_refCount);
#endif
  }


  x = 0;
  y = 0;
  GModSymbol_getExtents(ms,0,&minx,&miny,&maxx,&maxy);
  GModSymbol_flushPorts(ms);

  for (r = 0;r < 4;r++) {
    int n = 0;
    int k = 0;
    int length;

    /*
     * Count number of ports of rotation r
     */
    for (i = 0;i < numPorts;i++) {
      IGPort *igp = portList[i];
      if (igp_getSide(igp) == r) n++;
    }

    if (n == 0) continue;

    if (r == 0 || r == 2)
      length = maxy-miny;
    else
      length = maxx-minx;

#if IGEN_DEBUG
    printf("  rotation %d: n=%d length=%d   ms_i=(%d,%d)\n",r,n,length,ms->ms_ix[0][0],ms->ms_iy[0][0]);
#endif

    for (i = 0;i < numPorts;i++) {
      IGPort *igp = portList[i];
      const char *name = igp_getName(igp);
      int iotype = igp_getType(igp);
      int rotation = igp_getSide(igp);
      int nbits = igp_getSize(igp);

      if (rotation != r) continue;

      switch (r) {
      case 0:
	x = maxx;
	y = miny + (k+1)*length/(n+1);
	break;
      case 1:
	x = minx + (k+1)*length/(n+1);
	y = miny-1;
	break;
      case 2:
	x = minx-1;
	y = miny + (k+1)*length/(n+1);
	break;
      case 3:
	x = minx + (k+1)*length/(n+1);
	y = maxy;
	break;
      }

      x += ms->ms_normal.x - ms->ms_ix[0][0];
      y += ms->ms_normal.y - ms->ms_iy[0][0];

      /*
       * Override port positions if marked as keep.
       */
      if (igp->igp_keep) {
	x = igp->igp_x;
	y = igp->igp_y;
      }

      GModSymbol_addPort(ms,new_GSymPort(name,x,y,iotype,rotation,nbits));
#if IGEN_DEBUG
      printf("    GModSymbol_addPort(ms,new_GSymPort(%s,%d,%d,%d,%d,%d))\n",name,x,y,iotype,rotation,nbits);
#endif

      k++;
    }
  }

  modint_makeSymbolInterface(M,ms);
  modint_syncDisplay(M);

  SetModified(MF_INTERFACE);
  FlagRedraw();


  igen_freePortList(portList,numPorts);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Helping function for igen_generate() to do the actual construction of the
 * new module interface.
 *
 *****************************************************************************/
void igen_generate_build(GModuleDef *M,IGPort **portList,int numPorts,int retainSize)
{
  GModuleDef *MI = TkGate.circuit->mid_mod;	/* Module interface container */
  int width = 2*HMARGIN, height = 2*VMARGIN;	/* Size of module */
  GCElement *g;
  int labelWidth;
  int maxPortLabelWidth = 0;
  int i;

  /*
   * If we are retaining the size, set the width/height form the current
   * module interface.
   */
  if (retainSize) {
    g = M->m_interface;		/* Old module description */
    width = g->u.block.gwidth;			/* Old module width */
    height = g->u.block.gheight;		/* Old module height */
  }

  /*
   * Clear the old interface and get the new empty one.
   */
  EditState_unselectAll(TkGate.circuit->es);
  modint_setInterface(M,0);
  g = M->m_interface;


  /*
   * Width of the label
   */
  labelWidth = GKTextWidth(TkGate.textbXF[1],M->m_name,strlen(M->m_name));

  /*
   * Compute the and set the new size for the module
   */
  for (i = 0;i < numPorts;i++) {
    IGPort *igp = portList[i];
    int side = igp_getSide(igp);
    int length;
    int w;

    if (side == PSIDE_LEFT || side == PSIDE_RIGHT) {
      length = igp->igp_offset + VMARGIN;
      if (length > height) height = length;

      w = GKTextWidth(TkGate.stextbXF[1],igp->igp_name,strlen(igp->igp_name));
      if (w > maxPortLabelWidth) maxPortLabelWidth = w;
    } else {
      length = igp->igp_offset + HMARGIN;
      if (length > width) width = length;
    }
  }

  if ( maxPortLabelWidth*2+labelWidth > width) {
    width = maxPortLabelWidth*2+labelWidth + 15;
  }

  g->u.block.gwidth = width;
  g->u.block.gheight = height;


  /*
   * Attach ports to the module
   */
  for (i = 0;i < numPorts;i++) {
    IGPort *igp = portList[i];
    int iotype = igp_getType(igp);
    int side = igp_getSide(igp);
    GWire *w;
    int x,y;

    wire_new(MI,0,&w);
    w->net->n_nbits = igp_getSize(igp);

    switch (side) {
    case PSIDE_LEFT :
      x = g->xpos;
      y = g->ypos + igp->igp_offset;
      block_attach(MI,g,w,w->driver,x,y,igp_getName(igp),iotype);
      break;
    case PSIDE_RIGHT :
      x = g->xpos + width;
      y = g->ypos + igp->igp_offset;
      block_attach(MI,g,w,w->driver,x,y,igp_getName(igp),iotype);
      break;
    case PSIDE_TOP :
      x = g->xpos + igp->igp_offset;
      y = g->ypos;
      block_attach(MI,g,w,w->driver,x,y,igp_getName(igp),iotype);
      break;
    case PSIDE_BOTTOM :
      x = g->xpos + igp->igp_offset;
      y = g->ypos + height;
      block_attach(MI,g,w,w->driver,x,y,igp_getName(igp),iotype);
      break;
    }
  }
}

/*****************************************************************************
 *
 * Generate an initial interface.  This is an altenative interface generator
 * function that does not need to be called from tcl.  It is used to generate
 * initial interfaces from native Verilog.
 *
 *****************************************************************************/
int igen_generate_initial(GModuleDef *M)
{
  IGPort **portList = 0;	/* Array of all ports */
  int numPorts = 0;
  int i;

  portList = igen_generate_getModPorts(M,portList,&numPorts);

  /*
   * Set positions for currently unpositioned ports on each side.
   */
  for (i = 0;i < PSIDE_MAX;i++)
    igen_generate_positionPorts(portList,numPorts,i);


  /*
   * Do the actual construction of the new module interface
   */
  igen_generate_build(M,portList,numPorts,0);
  if (portList) igen_freePortList(portList,numPorts);

  return 0;
}

/*****************************************************************************
 *
 * Generate an interface
 *
 *****************************************************************************/
int igen_generate(GModuleDef *M,int argc,const char *argv[])
{
  char *aname = 0;		/* Array with port information */
  char *which = "int";		/* Which ports to use */
  char *pos = "keep";		/* Positioning disposition */
  IGPort **portList = 0;	/* Array of all ports */
  int numPorts = 0;
  int keep = 0;
  int i;
  const char *strIType;

  /*
   * Parse command parameters
   */
  for (i = 0;i < argc;i++) {
    if (strcmp(argv[i],"-data") == 0 && i+1 < argc)
      aname = (char*) argv[++i];
    else if (strcmp(argv[i],"-which") == 0 && i+1 < argc)
      which = (char*) argv[++i];
    else if (strcmp(argv[i],"-pos") == 0 && i+1 < argc)
      pos = (char*) argv[++i];
    else
      return TCL_ERROR;
  }
  keep = pos && (strcmp(pos,"keep") == 0);		/* Keep existing positions/size */

  if (!aname) return TCL_ERROR;

  strIType = Tcl_GetVar2(TkGate.tcl,aname,"itype",TCL_GLOBAL_ONLY);
  if (strIType && strcmp(strIType,"symbol") == 0) {
    igen_generate_symbol(M,aname,which,keep);
    return TCL_OK;
  }

  /*
   * We are including current interface ports
   */
  if (strcmp(which,"int") == 0 || strcmp(which,"modint") == 0) {
    portList = igen_generate_getInstPorts(M,aname,&numPorts);
    if (keep)
      igen_setBlockPortPositions(M,portList,numPorts);
  }

  /*
   * Get module ports if necessary
   */
  if (strcmp(which,"mod") == 0 || strcmp(which,"modint") == 0) {
    portList = igen_generate_getModPorts(M,portList,&numPorts);
  }

  /*
   * Set positions for currently unpositioned ports on each side.
   */
  for (i = 0;i < PSIDE_MAX;i++)
    igen_generate_positionPorts(portList,numPorts,i);

  /*
   * Debugging code to print out the new port list.
   */
#if 0
  printf("Generated port list:\n");
  for (i = 0;i < numPorts;i++) {
    IGPort *igp = portList[i];

    printf("  %s %s[%d] @ %s",
	   igp_getTypeStr(igp),
	   igp_getName(igp),
	   igp_getSize(igp),
	   igp_getSideStr(igp));

    if (igp->igp_offset >= 0) 
      printf("+%d\n",igp->igp_offset);
    else
      printf("\n");
  }
#endif

  /*
   * Do the actual construction of the new module interface
   */
  igen_generate_build(M,portList,numPorts,keep);

  SetModified(MF_INTERFACE);

  if (editstate_getInterfaceMode() == INTFMODE_SINGLE) {
    modint_syncDisplay(M);
    FlagRedraw();
  }

  if (portList) igen_freePortList(portList,numPorts);

  return 0;
}

/*****************************************************************************
 *
 * Change the type of an interface between "block" and "symbol".
 *
 *****************************************************************************/
int igen_changetype(GModuleDef *M,int argc,const char *argv[])
{
  char *aname = 0;	/* Array with port information */
  const char *strIType;
  int i;
  int doswap = 0;

  if (GModuleDef_isIntfProtected(M)) {
    message(1,msgLookup("err.protintf"),M->m_name);
    return TCL_ERROR;
  }


  for (i = 0;i < argc;i++) {
    if (strcmp(argv[i],"-data") == 0 && i+1 < argc)
      aname = (char*) argv[++i];
  }
  if (!aname) return TCL_ERROR;

  /*
   * Check the requested type and take action according to the following rules.
   *   1) If the current interface is of the requested type, do nothing. 
   *   2) If the alternate interface is of the requested type, swap the alternate
   *      and current interface objects.
   *   3) If there is no alternate, or the alternate is not of the requested type,
   *      purge the alternate, move the current interface to the alternate and
   *      generate a new interface of the requested type.
   */
  strIType = Tcl_GetVar2(TkGate.tcl,aname,"itype",TCL_GLOBAL_ONLY);
  if (strIType && strcmp(strIType,"symbol") == 0) {
    if (M->m_interface && GCElement_getType(M->m_interface) == GC_SYMBLOCK) return TCL_OK;
    if (M->m_altInterface && GCElement_getType(M->m_altInterface) == GC_SYMBLOCK)
      doswap = 1;
  } else {
    if (M->m_interface && GCElement_getType(M->m_interface) == GC_BLOCK) return TCL_OK;
    if (M->m_altInterface && GCElement_getType(M->m_altInterface) == GC_BLOCK)
      doswap = 1;
  }


  /*
   * Perform either the required swap or generate action.
   */
  if (doswap) {
    GCElement *g1,*g2,*new_g1,*new_g2;

#if IGEN_DEBUG
    printf("igen_changetype: swap\n");
#endif

    g1 = M->m_altInterface;
    g2 = M->m_interface;


    new_g1 = (*g1->typeinfo->CopyGate)(TkGate.circuit->mid_mod,g1,0,0,0);
    new_g2 = (*g2->typeinfo->CopyGate)(TkGate.circuit->mid_altMod,g2,0,0,0);
    new_g1->show_name = 0;
    new_g2->show_name = 0;


    gate_delete(g1,TkGate.circuit->mid_altMod,0);
    gate_delete(g2,TkGate.circuit->mid_mod,0);

    ob_touch(M);
    M->m_altInterface = new_g2;
    M->m_interface = new_g1;

    SetModified(MF_INTERFACE);
    modint_syncDisplay(M);
    FlagRedraw();
  } else {
    GCElement *g,*new_g;

    g = M->m_interface;


    new_g = (*g->typeinfo->CopyGate)(TkGate.circuit->mid_altMod,g,0,0,0);
    new_g->show_name = 0;

    ob_touch(M);

    gate_delete(M->m_interface,TkGate.circuit->mid_mod,0);
    M->m_interface = 0;
    modint_setInterface(M,0);
    M->m_altInterface = new_g;

#if IGEN_DEBUG
    printf("igen_changetype: backup(%p -> %p) and generate\n",M->m_interface,M->m_altInterface);
#endif


    igen_generate(M,argc,argv);
  }


  return TCL_OK;
}

/*****************************************************************************
 *
 * Main function for manipulating module interfaces.
 *
 *****************************************************************************/
int igen_command(const char *op,const char *mod_name,int argc,const char *argv[])
{
  GModuleDef *M;
  char *aname = 0;
  char *icon = 0;
  Tcl_Interp *tcl = TkGate.tcl;

#if 0
 {
   int i;
   printf("igen %s %s",op,mod_name);
   for (i = 0;i < argc;i++)
     printf(" %s",argv[i]);
   printf("\n");
 }
#endif

  aname  = (char*)seekOption("-data",argv,argc);
  icon  =  (char*)seekOption("-icon",argv,argc);

  M = env_findModule(mod_name);
  if (!M) {
    if (aname) Tcl_SetVar2(tcl,aname,"ok","0",TCL_GLOBAL_ONLY);
    return 0;
  }

  if (icon && strcmp(op,"syncicon") == 0) {
    GCElement *g = modint_findDisplay(M->m_name);
    EditState *es = TkGate.circuit->es;

    if (es->env != TkGate.circuit->mid_display)
      return 0;

    if (strcmp(icon,"select") == 0) {
      if (g->selected) return 0;
    } else {
      if (!g->selected) return 0;
    }

    ob_begin_framef("SwitchIcon",0);
    if (g->selected) {
      sel_clear(es,1);
    } else {
      sel_appendGate(es,g,1);
      sel_finish(es);
      ob_touch(TkGate.circuit);
      TkGate.circuit->select = g;
    }
    ob_end_frame();
  } else if (strcmp(op,"load") == 0) {
    igen_load(M,aname);
  } else if (strcmp(op,"changetype") == 0) {
    igen_changetype(M,argc,argv);
  } else if (strcmp(op,"generate") == 0) {
    igen_generate(M,argc,argv);
  } else if (strcmp(op,"puticon") == 0) {
    igen_putIcon(M,aname,icon);
  } else if (strcmp(op,"geticon") == 0) {
    igen_getIcon(M,aname);
  } else {
    return -1;
  }

  return 0;
}

