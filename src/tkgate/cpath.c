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

    Last edit by hansen on Tue Jan 20 14:49:28 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "tkgate.h"

#define CP_DELTA 3

/*
 * Maps a net name to a two-element integer array.
 */
SHash *net_delay_table = 0;

/*
 * Maps a cannonical net name to a NetAliasList
 */
SHash *net_alias_table = 0;

NetAliasList *new_NetAliasList()
{
  NetAliasList *al = (NetAliasList *) ob_malloc(sizeof(NetAliasList),"NetAliasList");

  al->length = 0;
  al->names = (char**) ob_malloc(sizeof(char*) * NETALIASLIST_STEPSIZE,"char*[]");

  return al;
}

void delete_NetAliasList(NetAliasList *al)
{
  int i;

  for (i = 0;i < al->length;i++)
    ob_free(al->names[i]);
  ob_free(al);
}

void NetAliasList_add(NetAliasList *al,const char *name)
{
  if (((al->length+1) % NETALIASLIST_STEPSIZE) == 0) {
    char **n = (char**)ob_malloc(sizeof(char*)*(al->length+1+NETALIASLIST_STEPSIZE),"char*[]");
    int i;

    for (i = 0;i < al->length;i++)
      n[i] = al->names[i];
    ob_free(al->names);
    al->names = n;
  }

  al->names[al->length++] = ob_strdup(name);
}

/*
  Clear all critical path marks
 */
void cpath_clear()
{
  HashElem *E;
  GWireList *wl;

  for (E = Hash_first(TkGate.circuit->moduleTable);E;E = Hash_next(TkGate.circuit->moduleTable,E)) {
    GModuleDef *M = (GModuleDef*)HashElem_obj(E);

    for (wl = M->m_wires;wl;wl = wl->wl_next) {
      GWire *w = wl->wl_wire;
      w->cpath = 0;
    }
  }
}

void cpath_flush()
{
  if (TkGate.cpath_pelems) {
    int i;

    for (i = 0;i < TkGate.cpath_len;i++)
      ob_free(TkGate.cpath_pelems[i]);
    ob_free(TkGate.cpath_pelems);
    TkGate.cpath_pelems = 0;
    TkGate.cpath_len = 0;
  }
}

/*
 * find the port corresponding to 'name'.  The 'name' string is
 * destroyed in the process.  
 */
GWire *cpath_findGatePort(const char *Gname,const char *Pname,int n,GModuleDef *M)
{
  int i,j;
  GCElement *g;

#if 0
  printf("   cpath_findGatePort: %s %s %d\n",Gname,Pname,n);
#endif

  g = GModuleDef_findGate(M,Gname);

  if (g) {					/* Gname is at this level */
    int N = GCElement_numPads(g);

    for (i = 0;i < N;i++) {
      if (strcmp(Pname,GCElement_getPadName(g,i)) == 0) {
	GWire *w = g->wires[i];

	for (j = 0;w && j < n;j++)
	  w = w->next;
	return w;
      }
    }
    return 0;
  } else {					/* Gname is at a different level */
    GWire *w;
    char *x;

    x = strchr(Gname,'.');
    if (!x) return 0;
    *x = 0;
    g = GModuleDef_findGate(M,Gname);
    if (!g) return 0;

    /*
     * Special case for regular (not symbol) modules.
     */
    if (g->typeinfo->Code == GC_BLOCK) {
      int N = GCElement_numPads(g);

      Gname = x+1;

      w = cpath_findGatePort(Gname,Pname,n,env_findModule(g->u.block.moduleName));
      if (!w) return 0;

      for (i = 0;i < N;i++) {
	GWire *mw;

	for (mw = g->wires[i];mw;mw = mw->next) {
	  if (strcmp(w->net->n_signame,mw->name) == 0)
	    return mw;
	} 
      }
    }

    return 0;
  }
}

GWire *cpath_findPort(const char *name,GModuleDef *M)
{
  char buf[STRMAX];
  char *Gname,*Pname,*x;
  int n;

  if (!name) return 0;

  strcpy(buf,name);
  
  Gname = buf;
  Pname = strchr(Gname,'[');
  if (!Pname) return 0;

  *Pname++ = 0;
  x = strchr(Pname,']');
  if (x) *x  = 0;

  for (x = Pname;*x && !isdigit(*x);x++);
  if (sscanf(x,"%d",&n) == 1)
    *x = 0;
  else
    n = 0;

  return cpath_findGatePort(Gname,Pname,n,M);

}

/*
 * Highlight the path from w1 to w2
 */
int cpath_highlightPath(GWire *w1,GWire *w2)
{
  GCElement *g;
  GGateInfo *gi;

  if (!w1 || w1->cpath)
    return 0;

  w1->cpath = 1;
  w1 = wire_other(w1);
  w1->cpath = 1;

  if (w1 == w2 || wire_other(w1) == w2) return 1;

  g = w1->gate;
  if (g) {
    int i;

    gi = g->typeinfo;
    switch (gi->Code) {
    case GC_JOINT :
      for (i = 0;i < 4;i++)
	if (cpath_highlightPath(g->wires[i],w2))
	  return 1;
      break;
    case GC_TAP :
      if (w1 == g->wires[TAP_IN]) {
	if (cpath_highlightPath(g->wires[TAP_OUT],w2))
	  return 1;
      } else if (w1 == g->wires[TAP_OUT]) {
	if (cpath_highlightPath(g->wires[TAP_IN],w2))
	  return 1;
      }
      break;
    }
  }

  w1->cpath = 0;
  w1 = wire_other(w1);
  w1->cpath = 0;
  return 0;
}

/*
 * A starting port has not been explicitly specified.  Try to
 * guess the initial port.
 */
GWire *cpath_guessPort(GWire *w,GModuleDef *M,int isOut)
{
  GNet *n = w->net;
  HashElem *E;

  if (!M) return 0;

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    switch (g->typeinfo->Code) {
    case GC_LOGICIN :
    case GC_LOGICOUT :
    case GC_LOGICTRI :
      if (g->wires[0]->net == n)
	return g->wires[0];
      break;
    case GC_SWITCH :
    case GC_DIP :
      if (isOut) {
	if (g->wires[0]->net == n)
	  return g->wires[0];
      }
      break;
    case GC_LED :
      if (!isOut) {
	if (g->wires[0]->net == n)
	  return g->wires[0];
      }
      break;
    case GC_REGISTER :
    case GC_FLIPFLOP :
      {
	int N = GCElement_numPads(g);
	int i;

	for (i = 0;i < N;i++) {
	  if (isOut && GCElement_getPadDir(g,i) == OUT && g->wires[i]->net == n)
	    return g->wires[i];
	  if (!isOut && GCElement_getPadDir(g,i) == IN && g->wires[i]->net == n)
	    return g->wires[i];
	}
      }
      break;
    }
  }

  return 0;
}

/*
  Display an n-element path.  Each element of the path is a string of
  the form <port1>:net:<port2>
 */
void cpath_show_aux(int n,char *pelems[])
{
  GModuleDef *M;
  GWireList *wl;
  char modPath[STRMAX];
  int i,j,l;
  NetAliasList *al;

  M = TkGate.circuit->es->env;

  editstate_getPath(TkGate.circuit->es,modPath);
  l = strlen(modPath);
  if (l > 0) {
    strcat(modPath,".");
    l++;
  }
  for (i = 0;i < n;i++) {
    char port1[STRMAX],netName[STRMAX],port2[STRMAX];
    GNet *net = 0;
    GWire *w1 = 0,*w2 = 0;

    if (sscanf(pelems[i],"<%[^>]>:%[^:]:<%[^>]>",port1,netName,port2) == 3) {
    } else if (sscanf(pelems[i],"<>:%[^:]:<%[^>]>",netName,port2) == 2) {
      *port1 = 0;
    } else if (sscanf(pelems[i],"<%[^>]>:%[^:]:<>",port1,netName) == 2) {
      *port2 = 0;
    } else if (sscanf(pelems[i],"<>:%[^:]:<>",netName) == 1) {
      *port1 = 0;
      *port2 = 0;
    }

    /*printf("cpath [%s]: <%s %s>\n",netName,port1,port2);*/

    net = 0;
    al = (NetAliasList*) SHash_find(net_alias_table,netName);
    if (al) {
      for (j = 0;!net && j < al->length;j++) {
	if (l != 0 && strncmp(modPath,al->names[j],l) != 0) {
	  continue;
	}
	net = GModuleDef_findNet(M,al->names[j]+l);
      }
    }

    if (!net) continue;			/* Net is not in this module */
    
    if (l == 0 || (*port1 && strncmp(port1,modPath,l) == 0))
      w1 = cpath_findPort(port1+l,TkGate.circuit->es->env);

    if (l == 0 || (*port2 && strncmp(port2,modPath,l) == 0))
      w2 = cpath_findPort(port2+l,TkGate.circuit->es->env);

    if (w1 == w2) continue;

    if (w1 && !w2) w2 = cpath_guessPort(w1,M,0);
    if (w2 && !w1) w1 = cpath_guessPort(w2,M,1);

#if 0
    printf("cpath_show: |%s| w1=%x  w2=%x  modPath=%s  l=%d  net=%x\n",pelems[i],w1,w2,modPath,l,net);
#endif

    if (!w1 && !w2) {				/* No port info, highligh full net */ 
      for (wl = M->m_wires;wl;wl = wl->wl_next) {
	GWire *w = wl->wl_wire;
	if (w->net != net) continue; 
	w->cpath = 1;
      }
    } else if (!w1) {				/* highlight w2 only */
      w1 = wire_driver(w2);
      w2 = wire_drivee(w1);
      w1->cpath = w2->cpath = 1;
    } else if (!w2) {				/* highlight w1 only */
      w1 = wire_driver(w1);
      w2 = wire_drivee(w1);
      w1->cpath = w2->cpath = 1;
    } else {					/* highlight path */
      cpath_highlightPath(w1,w2);
    }
  }
}

void cpath_close()
{
  cpath_flushNetDelayTable();
  DoTcl("catch { set analyOn 0 }");
  DoTcl("catch { close $simId }");
  DoTcl("catch { destroy .cpbox }");
  ClearErrorMark();
}

/*
 * Execute a command from the analyzer
 */
int cpath_command(const char *C)
{
  char buf[STRMAX],buf2[STRMAX];
  int t,a1,a2;
  extern int debugSimInterface;
  int area,staticPower;

  if (debugSimInterface) {
    printf("Analyze: %s\n",C);
    fflush(stdout);
  }

  while (*C == ' ')C++;
  if (strncmp(C,"comment",7) == 0) {			/* Ignore comments from simulator */
    return 0;
  } else if (strncmp(C,"echo",4) == 0) {
    printf("gsim: %s\n",C);
    return 0;
  } else if (strncmp(C,"ok",2) == 0) {			/* Simulator loaded file and is ready to go */
    DoTcl("tkg_cpathStart");
  } else if (strncmp(C,"error_exit",10) == 0) {		/* The simulator exited on an error */
    Error_close();
    tkgate_setMajorMode(MM_EDIT);
  } else if (sscanf(C," netalias %s %s",buf,buf2) == 2) {	/* Net alias definition */
    cpath_registerNetAlias(buf,buf2);
  } else if (sscanf(C," netdelay %s %d %d",buf,&a1,&a2) == 3) {	/* Net delay values */
    cpath_registerNetDelay(buf,a1,a2);
  } else if (sscanf(C," cpath_loop %s",buf) == 1) {		/* Combinational loop */
    DoTcl("tkg_cpathAddLoop %s",buf);
  } else if (sscanf(C," cpath %d %[^\n]",&t,buf) == 2) {	/* Critical path */
    DoTcl("tkg_cpathAdd %d {%s}",t,buf);
  } else if (strncmp(C,"cdone",5) == 0) {			/* End of critical path data */
    DoTcl("tkg_cpathEnd");
  } else if (sscanf(C," stats area=%d static_power=%d",&area,&staticPower) == 2) {/* Circuit statistics */ 
    message(0,"Estimated area=%d.",area,staticPower);
  } else if (sscanf(C," warning file %[^\n]",buf) == 1) {		/* An error in a simulator input file. */
    Error_report(C);
  } else if (sscanf(C," error file %[^\n]",buf) == 1) {		/* An error in a simulator input file. */
    Error_report(C);
  } else if (sscanf(C," simerror %[^\n]",buf) == 1) {		/* A run-time error */
    message(1,buf);
  }
  return 0;
  
}

void cpath_open()
{
  EditState *es;
  char tempName[STRMAX];

  editstate_makeRootAtTop(&TkGate.circuit->es);
  es = TkGate.circuit->es;

  getSimTempFile(tempName);

  if (VerilogWriteModules(tempName,VSO_ALLMODS|VSO_NOHDLCHECK) != 0) {	/* Save circuit to send to simulator/analyzer */
    /* Could not save temporary file '%s' for simulator (disc full?) */
    message(1,msgLookup("err.sim.badtmp"),tempName);
    return;
  }

  DoTcl("tkg_startAnalyzer %s",tempName);
}

void cpath_show(int n,const char *pelems[])
{
  int i;

  cpath_clear();
  cpath_flush();
  ClearErrorMark();

  if (n <= 0) return;

#if 0
  printf("cpath_show:");
  for (i = 0;i < n;i++)
    printf(" %s",pelems[i]);
  printf("\n");
#endif

  TkGate.cpath_len = n;
  TkGate.cpath_pelems = (char**) ob_malloc(sizeof(char*)*n,"char*[]");
  for (i = 0;i < n;i++) {
    TkGate.cpath_pelems[i] = ob_strdup(pelems[i]);
  }
  cpath_show_aux(TkGate.cpath_len,TkGate.cpath_pelems);
  FlagRedraw();
}

/*
 * Redisplay the current critical path if one is visible
 */
void cpath_reshow()
{
  if (tkgate_currentMode() != MM_ANALYZE) return;

  cpath_clear();
  cpath_show_aux(TkGate.cpath_len,TkGate.cpath_pelems);
  FlagRedraw();
}

void cpath_draw(int x1,int y1,int x2,int y2)
{
  int n,i; 
  int dx = x2-x1;
  int dy = y2-y1;
  int x,y;
  int x_off = 0,y_off = 0;

  n = (dx+dy)/CP_DELTA;
  if (n < 0) n = -n;

  /*
    Only one of dx or dy should be non-zero
   */
  if (dx > 0)
    dx = CP_DELTA;
  else if (dx < 0)
    dx = -CP_DELTA;

  if (dy > 0)
    dy = CP_DELTA;
  else if (dy < 0)
    dy = -CP_DELTA;

  /*
   * Correct for variations in X servers.
   */
  if (dx == 0) {
    x_off = TKGATE_BUSW_VERT-1;
  } else {
    y_off = TKGATE_BUSW_HORZ-1;
  }

  x = x1;
  y = y1;
  for (i = 0;i < n;i++) {
    x += dx;
    y += dy;
    Icon_draw(TkGate.D,TkGate.W,TkGate.cpathGC,x+x_off,y+y_off,SIZEHASH);
  }
}

void cpath_drawSegments()
{
  GModuleDef *M = TkGate.circuit->es->env;
  GWireList *wl;

  for (wl = M->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;

    if (w->cpath && w->nodes->out)
      GWire_draw(w);
  }
}

/*
  Called at regular intervals while cpath is displayed to 
  flash the cpath state.
 */
void cpath_flash()
{
  if (!TkGate.circuit->es) return;

  if (TkGate.flashCPath) {
    cpath_drawSegments();
    TkGate.cpath_flashState = !TkGate.cpath_flashState;
    cpath_drawSegments();
  } else if (!TkGate.cpath_flashState) {
    cpath_drawSegments();
    TkGate.cpath_flashState = 1;
    cpath_drawSegments();
  }
}

void init_cpathNetDelayTable()
{
  net_delay_table = new_SHash();
  net_alias_table = new_SHash();
}

void cpath_flushNetDelayTable()
{
  HashElem *E;

  for (E = Hash_first(net_delay_table);E;E = Hash_next(net_delay_table,E)) {
    int *d = (int*)HashElem_obj(E);
    ob_free(d);
  }
  SHash_flush(net_delay_table);

  for (E = Hash_first(net_delay_table);E;E = Hash_next(net_delay_table,E)) {
    NetAliasList *d = (NetAliasList*)HashElem_obj(E);
    delete_NetAliasList(d);
  }
  SHash_flush(net_alias_table);
}

void cpath_registerNetAlias(const char *alias_name,const char *canon_name)
{
  NetAliasList *al = (NetAliasList*)SHash_find(net_alias_table,canon_name);

#if 0
  printf("cpath_registerNetAlias: %s %s\n",alias_name,canon_name);
#endif

  if (!al) {
    al = new_NetAliasList();
    SHash_insert(net_alias_table,canon_name,al);
  }

  NetAliasList_add(al,alias_name);
}

void cpath_registerNetDelay(const char *name,int fd,int bd)
{
  int *d;

  cpath_registerNetAlias(name,name);
#if 0
  printf("cpath_registerNetDelay: %s fd=%d bd=%d\n",name,fd,bd);
#endif

  d = SHash_find(net_delay_table,name);
  if (!d) {
    d = (int*)ob_malloc(2*sizeof(int),"int[2]");
    SHash_insert(net_delay_table,name,d);
  }

  d[0] = fd;
  d[1] = bd;
}

int  cpath_getNetDelay(const char *name,int *fd,int *bd)
{
  int *d;

  d = SHash_find(net_delay_table,name);
  if (d) {
    if (fd) *fd = d[0];
    if (bd) *bd = d[1];
    return 1;
  }

  return 0;
}

void cpath_showNetDelay(GNet *n)
{
  char buf[STRMAX];
  int fd,bd;

  editstate_getPath(TkGate.circuit->es,buf);
  if (*buf) {
    char *p = buf+strlen(buf);
    sprintf(p,".%s",n->n_signame);
  } else
    strcpy(buf,n->n_signame);

  if (cpath_getNetDelay(buf,&fd,&bd))
    DoTcl("tkg_postNetDelay %s %d %d",buf,fd,bd);
}

/*
  Mouse button press while in cpath mode.
 */
void cpath_mouseDown(EditState *es)
{
  GCElement *g = gate_hit(es->env,TkGate.ed->tx,TkGate.ed->ty);
  GWireNode *wn = wire_iohit(TkGate.ed->tx,TkGate.ed->ty,es->env->m_wires);

  if (wn) {
    GWire *w = wirenode_driver(wn);

    EditState_unselectGate(es);
    cpath_showNetDelay(w->net);
    net_select(w->net,1);
  } else if (g) {
    net_unselect(1);
    EditState_selectGate(es,TkGate.ed->tx,TkGate.ed->ty);
  } else {
    EditState_unselectGate(es);
    net_unselect(1);
  }
}

void cpath_mouseUp(EditState *es)
{
  DoTcl("tkg_unpostNetDelay");
}
