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

    Last edit by hansen on Sat Sep 26 18:35:12 2009
****************************************************************************/
#include "tkgate.h"

#define MAXDEPTH	128

extern GScope *Scope;

int security_exec_level;

static char *unitStrings[] = {"s","ms","us","ns","ps","fs"};

/*****************************************************************************
 *
 * Convert a character to a value.
 *
 * Paramaters:
 *     c		Character to convert
 *
 * Returns:		Value corresponding to c or -1 if it was an unknown
 *
 *****************************************************************************/
static int charToSym(char c)
{
  if (isdigit(c))
    return c - '0';
  else if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  else if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  else
    return -1;
}


static void parse_verilog_bits(unsigned *Svalue,unsigned *Svalid,const char *A)
{
  int d = strlen(A);
  int i;

  *Svalue = 0;
  *Svalid = ~0;

  for (i = 0;i < d;i++) {
    int digitVal = charToSym(A[d-i-1]);

    if (digitVal < 0)
      *Svalid &= ~(1 << i);

    *Svalue |= digitVal << i;
  }

  if (d < TKGATE_WORDSIZE && charToSym(A[0]) < 0) {
    *Svalid &= ((1 << d)-1);
  }
}

void parse_verilog_hex(unsigned *Svalue,unsigned *Svalid,const char *A)
{
  int len = strlen(A);		/* Length of string */
  int i;

  *Svalue = 0;
  *Svalid = 0;

  for (i = 0;i < len;i++) {
    int digitValue, digitValid;

    if (A[i] == '(') {
      digitValue = 0;
      digitValid = 0;
      for (i++;A[i] != ')';i++) {
	int d = charToSym(A[i]);
	int v = (d < 0) ? 0 : 1;
	if (d < 0) d = 0;
	digitValue = (digitValue << 1) | d;
	digitValid = (digitValid << 1) | v;
      }
    } else {
      digitValue = charToSym(A[i]);
      digitValid = (digitValue < 0) ? 0 :0xf;
      if (digitValue < 0) digitValue = 0;
    }

    /*
     * If the initial digit is not unknown, make all bits above the 
     * specified bits valid.
     */
    if (i == 0 && digitValid == 0xf)
      *Svalid = ~0;

    *Svalid = (*Svalid << 4) | digitValid;
    *Svalue = (*Svalue << 4) | digitValue;
  }

  /*  printf("parse_verilog_hex: [%s] valid=%x value=%x\n",A,*Svalid,*Svalue);*/
}

static void parse_verilog_oct(unsigned *Svalue,unsigned *Svalid,const char *A)
{
  int len = strlen(A);		/* Length of string */
  int i;

  *Svalue = 0;
  *Svalid = 0;

  for (i = 0;i < len;i++) {
    int digitValue, digitValid;

    if (A[i] == '(') {
      digitValue = 0;
      digitValid = 0;
      for (i++;A[i] != ')';i++) {
	int d = charToSym(A[i]);
	int v = (d < 0) ? 0 : 1;
	if (d < 0) d = 0;
	digitValue = (digitValue << 1) | d;
	digitValid = (digitValid << 1) | v;
      }
    } else {
      digitValue = charToSym(A[i]);
      digitValid = (digitValue < 0) ? 0 :0x7;
      if (digitValue < 0) digitValue = 0;
    }

    /*
     * If the initial digit is not unknown, make all bits above the 
     * specified bits valid.
     */
    if (i == 0 && digitValid == 0x7)
      *Svalid = ~0;

    *Svalid = (*Svalid << 3) | digitValid;
    *Svalue = (*Svalue << 3) | digitValue;
  }

  /*  printf("parse_verilog_hex: [%s] valid=%x value=%x\n",A,*Svalid,*Svalue);*/
}

/*****************************************************************************
 *
 * Parse a verilog-style constant
 *
 * Parameters:
 *      value		String representation of constant to be parsed
 *      *ivalue		Returned integer representation of value (up to 32-bits)
 *      *ivalid		Returned mask of valid bits
 *      *nbits		Returned number of bits
 *
 * Returns:		Non-zero if consant was invalid, 0 if valid.
 *
 * Parse a verilog-style constant as received from the simulator.
 * Only 'b' and 'h' types are used, with 'b' types being used only
 * for single-bit values.
 *
 *****************************************************************************/ 
int parse_verilog_constant(char *value,unsigned *ivalue,unsigned *ivalid,int *nbits)
{
  const char *p;
  char c;

  if (*value == 'x') {
    *ivalue = 0;
    *ivalid = 0;
    return 0;
  }

  *nbits = 0;
  if (sscanf(value,"%d'%c",nbits,&c) == 2) {
    p = strchr(value,'\'')+2;
  } else if (sscanf(value,"'%c",&c) == 1) {
    nbits = 0;
    p = strchr(value,'\'')+2;
  } else {
    nbits = 0;
    p = value;
    c = 'i';
  }

  switch (c) {
  case 'b' :
    parse_verilog_bits(ivalue,ivalid,p);
    break;
  case 'h' :
    parse_verilog_hex(ivalue,ivalid,p);
    break;
  case 'o' :
    parse_verilog_oct(ivalue,ivalid,p);
    break;
  case 'd' :
  case 'i' :
    *ivalid = ~0;
    if (sscanf(p,"%u",ivalue) != 1)
      return -1;
    break;
  default:
    return -1;
    break;
  }
  return 0;
}

void sendSimCmd(char *fmt,...)
{
  char buf[STRMAX];
  va_list ap;

  va_start(ap,fmt);
  vsprintf(buf,fmt,ap);
  va_end(ap);

  DoTclL("Simulator::cmdSend",buf,NULL);
}

GSimSwitch *new_GSimSwitch(char *wname,char *gname,GCElement *g)
{
  GSimSwitch *ss = (GSimSwitch*)ob_malloc(sizeof(GSimSwitch),"GSimSwitch");

  ss->name = ob_strdup(wname);
  ss->gname = ob_strdup(gname);
  ss->gate = g;
  ss->state = (unsigned*)ob_malloc(sizeof(unsigned),"unsigned");
  *ss->state = g->u.sw.dipval;

  return ss;
}

GSimLed *new_GSimLed(char *wname,char *gname,GCElement *g)
{
  GSimLed *ss = (GSimLed*)ob_malloc(sizeof(GSimLed),"GSimLed");

  ss->name = ob_strdup(wname);
  ss->gname = ob_strdup(gname);
  ss->gate = g;

  return ss;
}

GSimModule *new_GSimModule(GModuleDef *M,GCElement *g,GSimModule *P)
{
  GSimModule *sM = (GSimModule*) ob_malloc(sizeof(GSimModule),"GSimModule");

  sM->mod = M;
  sM->inst = g;
  sM->parent = P;

  sM->probes = new_SHash();
  sM->switches = new_SHash();
  sM->leds = new_SHash();
  sM->children = new_SHash();

  return sM;
}

GNet *sim_findNet(const char *name)
{
  char buf[STRMAX],*T;
  char *ptr[MAXDEPTH];
  GModuleDef *M = TkGate.circuit->root_mod;
  GNet *n = 0;
  int N,i;

  strcpy(buf,name);
  for (T = strtok(buf,"."), N = 0;T;T = strtok(0,"."), N++)
    ptr[N] = T;

  for (i = 0;i < N;i++) {
    if (i != (N-1)) {
      GCElement *g = GModuleDef_findGate(M,ptr[i]);

      if (g && GCElement_isModule(g))
	M = env_findModule(g->u.block.moduleName);
      else
	return 0;
    } else
      n = GModuleDef_findNet(M,ptr[i]); 
  }

  return n;
}

GCElement *sim_findGate(const char *name)
{
  char buf[STRMAX],*T;
  char *ptr[MAXDEPTH];
  GModuleDef *M = TkGate.circuit->root_mod;
  int N,i;
  GCElement *g = 0;

  strcpy(buf,name);
  for (T = strtok(buf,"."), N = 0;T;T = strtok(0,"."), N++)
    ptr[N] = T;

  for (i = 0;i < N;i++) {
    g = GModuleDef_findGate(M,ptr[i]);
    if (i == (N-1))
      break;
    else if (g && GCElement_isModule(g))
      M = env_findModule(g->u.block.moduleName);
    else
      return 0;
  }
  return g;
}

GModuleDef *sim_findContainingMod(const char *path)
{
  char buf[STRMAX],*p;
  GModuleDef *M;

  strcpy(buf,path);
  if ((p = strrchr(buf,'.'))) {
    GCElement *mg;
    *p = 0;
    mg = sim_findGate(buf);
    if (mg)
      M = env_findModule(mg->u.block.moduleName);
    else
      M = 0;
  } else
    M = TkGate.circuit->root_mod;

  return M;
}

void getSimTempFile(char *buf)
{
  int fd;

  strcpy(buf,"/tmp/tkgate.XXXXXX");
  fd = mkstemp(buf);
  close(fd);
}

/*****************************************************************************
 *
 * Initialize a SimInterface (Simulation Interface) object
 *
 * Parameters:
 *     si		Object to initialize.
 *
 *****************************************************************************/
void SimInterface_init(SimInterface *si)
{
  si->active = 0;
  *si->simFileName = 0;
  si->sim_root = 0;
  si->area = 0;
  si->staticPower = 0;
}

char *GSimModule_getTreePath(GSimModule *M,char *s)
{
  if (!M->parent) {
    return s + sprintf(s,"<%s>",M->mod->m_name);
  }

  s = GSimModule_getTreePath(M->parent,s);
  return s + sprintf(s,"/%s<%s>",M->inst->ename,M->mod->m_name);
}

char *GSimModule_getPathPrefix(GSimModule *M,char *s)
{
  if (!M->parent) {
    return s + sprintf(s,"%s.",M->mod->m_name);
  }

  s = GSimModule_getPathPrefix(M->parent,s);
  return s + sprintf(s,"%s.",M->inst->ename);
}

void GSimModule_getNetPathName(GSimModule *M,GNet *n,char *buf)
{
  char *s;

  s = GSimModule_getPathPrefix(M,buf);
  strcpy(s,n->n_signame);
}


/*
  The the full path of a gate g in the current module.
 */
void GSimModule_getFullPath(GSimModule *M,GCElement *g,char *buf)
{
  char *s;

  s = GSimModule_getPathPrefix(M,buf);
  if (g)
    strcpy(s,g->ename);
}

GSimProbe *new_GSimProbe(const char *name,GNet *net,int x,int y,GSimModule *sM)
{
  GSimProbe *P = (GSimProbe *) ob_malloc(sizeof(GSimProbe),"GSimProbe");

  P->name = ob_strdup(name);
  P->net = net;
  P->x = x;
  P->y = y;
  P->ss = sM;

  return P;
}

void delete_GSimProbe(GSimProbe *P)
{
  ob_free(P->name);
  ob_free(P);
}

void GSimProbe_draw(GSimProbe *P)
{
  static Icon *probe_icon = 0;

  if (!probe_icon) {
    int w,h,x,y;
    Pixmap pm = Pixmap_registerFromFileWithParms("probe","probe.b",&w,&h,&x,&y);
    probe_icon = new_Icon(pm,0,0,w,h,x,y);
  }
  Icon_draw(TkGate.D,TkGate.W,TkGate.toolGC,ctow_x(P->x),ctow_y(P->y),probe_icon);
  dce_DrawString(TkGate.toolGC,P->x+20,P->y-20,BetweenTopAndBottom|AtLeft,P->name);
}

const char *SimProbe_localName(GSimProbe *P)
{
  const char *x = strrchr(P->name,'.');
  if (x)
    return x+1;
  else
    return P->name;
}

/*****************************************************************************
 *
 * Lookup up a gate from its full path.
 *
 * Parameters:
 *     si		Simulation interface to search
 *     name		Full path name of gate
 *     *r_M		Return for module gate was found in.
 *     *r_g		Return for gate that was found
 *     *r_ss		Return for GSimSwitch if gate was a switch/dip.
 *
 * Returns:		Zero on success, negative on failure.
 *
 *  Lookup the gate named 'name' where 'name' is a full path gate name.  Returns the
 *  simulation module the gate is in, the GCElement structure and the correspoinding
 *  GSimSwitch for the specified gate.  Only non-NULL return arguments will be assigned.
 *  The function returns non-zero if the specified gate is not found.
 *
 *****************************************************************************/
int SimInterface_lookupGate(SimInterface *si,const char *name,GSimModule **r_M,
			    GCElement **r_g,GSimSwitch **r_ss)
{
  char buf[STRMAX],*T;
  char *ptr[MAXDEPTH];
  GSimModule *M;
  GCElement *g;
  GSimSwitch *ss;
  int i,N;

  M = si->sim_root;

  strcpy(buf,name);
  for (T = strtok(buf,"."), N = 0;T;T = strtok(0,"."), N++)
    ptr[N] = T;

  /*
   * Find parent GSimModule of specified gate.
   */
  for (i = 1;i < N-1;i++) {
    M = (GSimModule*)SHash_find(M->children,ptr[i]);
    if (!M) return -1;
  }

  /*
   * Find the actual gate.
   */
  g = GModuleDef_findGate(M->mod,ptr[N-1]);
  if (!g) {
    return -1;
  }

  /*
   * Find the GSimSwitch if available.
   */
  ss = (GSimSwitch*) SHash_find(M->switches,g->ename);

  if (r_M) *r_M = M;
  if (r_g) *r_g = g;
  if (r_ss) *r_ss = ss;

  return 0;
}

/******************************************************************************
 * Lookup the net named 'name' where 'name' is a full path net name.  Returns the
 * simulation module the net is in, and the wire.
 *****************************************************************************/
int SimInterface_lookupWire(SimInterface *si,const char *name,GSimModule **r_M,GWire **r_w,GNet **r_n)
{
  char buf[STRMAX],*T;
  char *ptr[MAXDEPTH];
  GSimModule *M;
  GNet *n;
  int i,N;

  M = si->sim_root;

  strcpy(buf,name);
  for (T = strtok(buf,"."), N = 0;T;T = strtok(0,"."), N++)
    ptr[N] = T;

  /*
   * Find parent GSimModule of specified wire.
   */
  for (i = 1;i < N-1;i++) {
    M = (GSimModule*)SHash_find(M->children,ptr[i]);
    if (!M) return -1;
  }

  if (GModuleDef_getType(M->mod) == MT_NETLIST) {
    /*
     * Find the actual net.
     */
    n = GModuleDef_findNet(M->mod,ptr[N-1]);
    if (!n) return -1;

    *r_M = M;
    *r_w = n->n_driver;
    if (r_n) *r_n = n;
  } else {
    /*
     * This is an HDL module.  Do not set a wire.
     */
    n = GModuleDef_findNet(M->mod,ptr[N-1]);
    if (!n) return -1;
    *r_M = M;
    *r_w = 0;
    if (r_n) *r_n = n;
  }

  return 0;
}

void SimInterface_changeCurrentModule(GSimModule *new_sm,GSimModule *old_sm)
{
  char buf[STRMAX];
  HashElem *E;

  if (old_sm) {
    for (E = Hash_first(old_sm->leds);E;E = Hash_next(old_sm->leds,E)) {
      GSimLed *sl = (GSimLed*) HashElem_obj(E);

      GSimModule_getNetPathName(old_sm,sl->gate->wires[0]->net, buf);
      sendSimCmd("$unprobe %s $led:%s",buf,sl->gate->ename);
    }
    for (E = Hash_first(new_sm->switches);E;E = Hash_next(new_sm->switches,E)) {
      GSimSwitch *ss = (GSimSwitch*) HashElem_obj(E);

      GSimModule_getNetPathName(new_sm,ss->gate->wires[0]->net, buf);
      sendSimCmd("$unprobe %s $switch:%s",buf,ss->gate->ename);
    }
 }

  if (new_sm) {
    for (E = Hash_first(new_sm->switches);E;E = Hash_next(new_sm->switches,E)) {
      GSimSwitch *ss = (GSimSwitch*) HashElem_obj(E);
      ss->gate->u.sw.dipval = *ss->state;

      GSimModule_getNetPathName(new_sm,ss->gate->wires[0]->net, buf);
      sendSimCmd("$probe %s $switch:%s",buf,ss->gate->ename);
    }
    for (E = Hash_first(new_sm->leds);E;E = Hash_next(new_sm->leds,E)) {
      GSimLed *sl = (GSimLed*) HashElem_obj(E);

      GSimModule_getNetPathName(new_sm,sl->gate->wires[0]->net, buf);
      sendSimCmd("$probe %s $led:%s",buf,sl->gate->ename);
    }

    GSimModule_getTreePath(new_sm, buf);
    DoTcl("BlockTree::setActive %s", buf);
  }


}

void SimInterface_updateNetlistProbes(GSimModule *SM)
{
  HashElem *E;

  for (E = Hash_first(SM->probes);E;E = Hash_next(SM->probes,E)) {
    GSimProbe *P = (GSimProbe*) HashElem_obj(E);
    DoTcl("NetList::configureitem %s -probe 1",SimProbe_localName(P));
  }
}

static GSimModule *SimInterface_buildSimMods_aux(GModuleDef *M,GCElement *pg,GSimModule *P)
{
  GSimModule *sM = new_GSimModule(M,pg,P);
  HashElem *E;

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    GSimModule *csM;
    GModuleDef *cM;
    char netPath[STRMAX],componentPath[STRMAX];

    GSimModule_getFullPath(sM,g,componentPath);

    (*g->typeinfo->SimInitFunc)(TkGate.circuit->es,g,componentPath);

    switch (g->typeinfo->Code) {
    case GC_SYMBLOCK :
    case GC_BLOCK :
      cM = env_findModule(g->u.block.moduleName);
      if (cM) {
	csM = SimInterface_buildSimMods_aux(cM,g,sM);
	SHash_insert(sM->children,g->ename,csM);
      }
      break;
    case GC_SWITCH :
    case GC_DIP :
      GSimModule_getNetPathName(sM,g->wires[0]->net,netPath);
      SHash_insert(sM->switches,g->ename,new_GSimSwitch(netPath,componentPath,g));
      break;
    case GC_LED :
      GSimModule_getNetPathName(sM,g->wires[0]->net,netPath);
      SHash_insert(sM->leds,g->ename,new_GSimSwitch(netPath,componentPath,g));
      break;
    }
  }

  return sM;
}

static void SimInterface_buildSimMods(SimInterface *si)
{
  si->sim_root = SimInterface_buildSimMods_aux(TkGate.circuit->root_mod,0,0);
}

void SimInterface_drawProbes(SimInterface *si)
{
  GSimModule *sM = TkGate.circuit->es->smod;
  HashElem *E;

  if (!sM) return;

  for (E = Hash_first(sM->probes);E;E = Hash_next(sM->probes,E)) {
    GSimProbe *P = (GSimProbe*) HashElem_obj(E);
    GSimProbe_draw(P);
  }
}

int SimInterface_probeExists(SimInterface *si,GSimModule *sM,const char *name)
{
  return SHash_find(sM->probes,name) != 0;
}

/*****************************************************************************
 *
 * Add a probe in response to request from simulator
 *
 *****************************************************************************/
void SimInterface_addProbe(SimInterface *si,const char *name,int nbits)
{
  GSimProbe *P;
  GSimModule *sM;
  GWire *w;
  int visible;
  int x,y;

  if (GScope_findTrace(Scope, name)) return;
  if (SimInterface_lookupWire(si,name,&sM,&w,0) != 0) return;
  visible = (sM == TkGate.circuit->es->smod);

  GScope_addTrace(Scope,name,name,nbits,TkGate.sortTraces);

  if (GModuleDef_getType(sM->mod) == MT_NETLIST) {
    GWire_pickProbePosition(w,&x,&y);
    P = new_GSimProbe(name,w->net,x,y,sM);
    SHash_insert(sM->probes,name,P);

    if (visible) {
      GSimProbe_draw(P);
      DoTcl("NetList::configureitem %s -probe 1",SimProbe_localName(P));
    }
  } else {
    P = new_GSimProbe(name,w->net,x,y,sM);
    SHash_insert(sM->probes,name,P);

    if (visible) {
      DoTcl("NetList::configureitem %s -probe 1",SimProbe_localName(P));
    }
  }
}

/*****************************************************************************
 *
 * Remove a probe in response to request from simulator
 *
 *****************************************************************************/
void SimInterface_delProbe(SimInterface *si,const char *name)
{
  GSimProbe *P;
  GSimModule *sM;
  GWire *w;
  int visible;

  if (!GScope_findTrace(Scope, name)) return;
  if (SimInterface_lookupWire(si,name,&sM,&w,0) != 0) return;
  visible = (sM == TkGate.circuit->es->smod);

  P = SHash_find(sM->probes,name);
  if (!P) return;

  if (GModuleDef_getType(sM->mod) == MT_NETLIST) {

    if (visible) {
      GSimProbe_draw(P);
      DoTcl("NetList::configureitem %s -probe 0",SimProbe_localName(P));
    }
  } else {
    if (visible) {
      DoTcl("NetList::configureitem %s -probe 0",SimProbe_localName(P));
    }
  }

  SHash_remove(sM->probes,name);
  delete_GSimProbe(P);

  GScope_deleteTrace(Scope,name);
}

/*****************************************************************************
 *
 * Add or delete a probe in a circuit in response to interface request
 *
 *****************************************************************************/
void SimInterface_addDelProbe(SimInterface *si,GSimModule *sM,const char *name,GWire *w,GWireNode *n,int x,int y)
{
  GSimProbe *P;
  int visible = (sM == TkGate.circuit->es->smod);

  if (GModuleDef_getType(sM->mod) != MT_NETLIST) {
    logError(ERL_ERROR,"SimInterface_addDelProbe called on HDL module.");
    return;
  }

  if ((P = SHash_find(sM->probes,name))) {	/* If probe exists, delete it */
    if (visible)
      GSimProbe_draw(P);
    SHash_remove(sM->probes,name);
    GScope_deleteTrace(Scope,name);
    sendSimCmd("$unprobe %s",name);
    DoTcl("NetList::configureitem %s -probe 0",SimProbe_localName(P));
    delete_GSimProbe(P);
  } else {					/* else add it */
    GScope_addTrace(Scope,name,name,w->net->n_nbits,TkGate.sortTraces);
    GWire_pickProbePosition(w,&x,&y);
    P = new_GSimProbe(name,w->net,x,y,sM);
    SHash_insert(sM->probes,name,P);
    if (visible)
      GSimProbe_draw(P);
    sendSimCmd("$probe %s",name);
    DoTcl("NetList::configureitem %s -probe 1",SimProbe_localName(P));
  }
}

void SimInterface_addDelHDLProbe(SimInterface *si,GSimModule *sM,const char *name,GNet *net)
{
  GSimProbe *P;

  if (GModuleDef_getType(sM->mod) != MT_TEXTHDL) {
    logError(ERL_ERROR,"SimInterface_addDelHDLProbe called on non-HDL module.");
    return;
  }

  if ((P = SHash_find(sM->probes,name))) {	/* If probe exists, delete it */
    SHash_remove(sM->probes,name);
    GScope_deleteTrace(Scope,name);
    sendSimCmd("$unprobe %s",name);
    DoTcl("NetList::configureitem %s -probe 0",SimProbe_localName(P));
    delete_GSimProbe(P);
  } else {
    P = new_GSimProbe(name,0,0,0,sM);
    GScope_addTrace(Scope,name,name,GNet_getNBits(net),TkGate.sortTraces);
    SHash_insert(sM->probes,name,P);
    sendSimCmd("$probe %s",name);
    DoTcl("NetList::configureitem %s -probe 1",SimProbe_localName(P));
  }
}

void SimInterface_setLed(SimInterface *si,char *gname,char *value)
{
  GCElement *g;
  unsigned ivalue,ivalid;
  int n;

  g = sim_findGate(gname);
  if (!g)
    g = GModuleDef_findGate(TkGate.circuit->es->smod->mod, gname);
  if (!g) return;

  parse_verilog_constant(value,&ivalue,&ivalid,&n);

#if 0
  if (g->u.led.ltype == 1) {
    printf("[%s] -> valid:%02x   value:%02x\n",value,ivalid,ivalue);
  }
#endif

  if (g->u.led.value != ivalue || g->u.led.valid != ivalid) {
    gate_draw(g,GD_NOWIRE);
    g->u.led.value = ivalue;
    g->u.led.valid = ivalid;
    gate_draw(g,GD_NOWIRE);
  }
}

/*****************************************************************************
 *
 * Set the state of a switch.  This method is used in response to the
 * "tell $switch" notice from simulator.  It is used to ensure switch changes
 * from a simulation script cause switches to be updated properly.
 *
 *****************************************************************************/
void SimInterface_setSwitch(SimInterface *si,char *gname,char *value)
{
  EditState *es = TkGate.circuit->es;
  GCElement *g;
  unsigned ivalue,ivalid;
  int n;

  g = sim_findGate(gname);
  if (!g)
    g = GModuleDef_findGate(TkGate.circuit->es->smod->mod, gname);
  if (!g) return;

  parse_verilog_constant(value,&ivalue,&ivalid,&n);

  if (g->u.sw.dipval != ivalue) {
    GSimSwitch *ss = SHash_find(es->smod->switches,g->ename);

    gate_draw(g,0);
    *ss->state = g->u.sw.dipval = ivalue;
    gate_draw(g,0);
  }
}


/*****************************************************************************
 *
 * Hit an object while in simulation mode.
 *
 * Parameters:
 *     si		Simulator interface
 *     x,y		Coordinates of hit
 *     isDoubleClick	Non-zero if this is a double click.
 *
 *****************************************************************************/
void SimInterface_hit(SimInterface *si,int x,int y,int isDoubleClick)
{
  GCElement *g = 0;
  GWireNode *n = 0;
  EditState *es = TkGate.circuit->es;

  EditState_unselectAll(es);

  if ((g = gate_hit(TkGate.circuit->es->env,x,y))) {
    if (g->typeinfo->Code == GC_JOINT) {
      int i;

      for (i = 0;i < 4;i++)
	if (g->wires[i]) {
	  n = g->wires[i]->nodes;
	  break;
	}
    } else {
      if ((*g->typeinfo->SimHitFunc)(es,g))
	return;
      else
	n = wire_iohit(x,y,TkGate.circuit->es->env->m_wires);
    }
  } else
    n = wire_iohit(x,y,TkGate.circuit->es->env->m_wires);

  if (n) {
    char buf[STRMAX];
    GWire *w = wirenode_driver(n);

    GSimModule_getNetPathName(es->smod, w->net,buf);

    net_select(w->net,1);
    TkGate.circuit->wnsel = n;
    TkGate.circuit->wsel = wirenode_driver(n);

    if (isDoubleClick) {
      DoTcl("Simulator::hideValue");
      SimInterface_addDelProbe(si,TkGate.circuit->es->smod,buf,w,n,x,y);
    } else {
      DoTcl("Simulator::showValue %s",buf);
    }
  }
}

void SimInterface_hitRelease(SimInterface *si)
{
  DoTcl("Simulator::hideValue");
}

void SimInterface_send(SimInterface *si,const char *command,...)
{
}

static void GSimModule_initSwitches(GSimModule *M)
{
  HashElem *E;

  for (E = Hash_first(M->switches);E;E = Hash_next(M->switches,E)) {
    GSimSwitch *ss = (GSimSwitch*) HashElem_obj(E);
    int nbits = ss->gate->wires[0]->net->n_nbits;
    DoTcl("tkg_simNetSet %s %d'h%x",ss->name,nbits,*ss->state);
  }

  for (E = Hash_first(M->children);E;E = Hash_next(M->children,E)) {
    GSimModule *cM = (GSimModule*) HashElem_obj(E);
    GSimModule_initSwitches(cM);
  }
}

static void GSimModule_initClocks(GSimModule *M,char *name)
{
  HashElem *E;
  char *nend = name + strlen(name);


  for (E = Hash_first(M->mod->m_gates);E;E = Hash_next(M->mod->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    if (GCElement_getType(g) == GC_CLOCK) {
      sprintf(nend,".%s",g->ename);
      sendSimCmd("$regclock %s.Z",name);
    }
  }



  for (E = Hash_first(M->children);E;E = Hash_next(M->children,E)) {
    GSimModule *cM = (GSimModule*) HashElem_obj(E);
    sprintf(nend,".%s",cM->inst->ename);
    GSimModule_initClocks(cM,name);
  }
}

static void GSimModule_initMemories(GSimModule *M,char *name)
{
  HashElem *E;
  char *nend = name + strlen(name);

  for (E = Hash_first(M->mod->m_gates);E;E = Hash_next(M->mod->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);

    if (GCElement_getType(g) == GC_ROM || GCElement_getType(g) == GC_RAM) {
      sprintf(nend,".%s",g->ename);

      if (g->u.mem.memfile && *g->u.mem.memfile != 0)
	sendSimCmd("$memload %s %s",g->u.mem.memfile,name);
    }
  }
  for (E = Hash_first(M->children);E;E = Hash_next(M->children,E)) {
    GSimModule *cM = (GSimModule*) HashElem_obj(E);
    sprintf(nend,".%s",cM->inst->ename);
    GSimModule_initMemories(cM,name);
  }
}


static void SimInterface_initClocks(SimInterface *si)
{
  char name[STRMAX];

  sprintf(name,"%s",si->sim_root->mod->m_name);
  GSimModule_initClocks(si->sim_root,name);
}

static void SimInterface_initMemories(SimInterface *si)
{
  char name[STRMAX];

  sprintf(name,"%s",si->sim_root->mod->m_name);
  GSimModule_initMemories(si->sim_root,name);
}

/*****************************************************************************
 *
 * Add the probes that where saved from the previous invocation.
 *
 *****************************************************************************/
static void SimInterface_addSavedProbes(SimInterface *si)
{
  List *pNames = TkGate.circuit->c_probes;
  ListElem *le;
  int x;

  /*
   * Temporarily disable trace sorting.
   */
  x = TkGate.sortTraces;
  TkGate.sortTraces = 0;

  for (le = List_first(pNames);le;le = List_next(pNames,le)) {
    const char *name = ListElem_obj(le);
    GSimModule *sM;
    GWire *w;
    GNet *net;

    if (SimInterface_lookupWire(si,name,&sM,&w,&net) != 0) {
      continue;
    }

    sendSimCmd("$probe %s",name);
    SimInterface_addProbe(si,name,GNet_getNBits(net));
  }

  TkGate.sortTraces = x;
}

/*****************************************************************************
 *
 * Do post-scan simulator initialization
 *
 * Parameters:
 *     si		Simulator interface.
 *
 * Called after simulator has loaded circuit and reported that it is ready
 * to start simulation.
 *
 *****************************************************************************/
static void SimInterface_startUp(SimInterface *si)
{
  int i;
  const char *global_script;

  DoTcl("Simulator::setupSecurity");

  if (si->no_scope) {
    DoTcl("tkg_altStartup");
    return;
  }

  DoTcl("Scope::post");
  GSimModule_initSwitches(si->sim_root);
  SimInterface_initClocks(si);
  SimInterface_initMemories(si);
  //unlink(si->simFileName);
  *si->simFileName = 0;

  SimInterface_changeCurrentModule(TkGate.circuit->es->smod,0);

  global_script = Tcl_GetVar(TkGate.tcl,"tkg_simInitScript",TCL_GLOBAL_ONLY);
  if (global_script && *global_script) {
    DoTclL("ScriptMgr::load",global_script,NULL);
  }

  for (i = 0;i < TkGate.circuit->numInitScripts;i++) {
    DoTclL("ScriptMgr::load",TkGate.circuit->initScripts[i],NULL);
  }

  BrkPtTable_sendAll(TkGate.circuit->c_breakpoints);

  if (TkGate.saveTraces)
    SimInterface_addSavedProbes(si);

  if (TkGate.circuit->simAutoStart) {
    DoTcl("tkg_simRun");
    //    sendSimCmd("$go");
  }
}

/*****************************************************************************
 *
 * Execute a tcl command under the current security policy.
 *
 *****************************************************************************/
int SimInterface_doSanitizedTcl(char *cmd)
{
  if (security_exec_level == 2) {
    DoTcl("%s",cmd);
    return 0;
  } else if (security_exec_level == 1) {
    char cmdName[STRMAX];

    /* commands can not include a '[' character */
    if (strchr(cmd,'['))
      return -1;

    if (sscanf(cmd,"%s",cmdName) != 1) return -1;


    DoTcl("VPD::isallowed %s",cmdName);
    if (*TkGate.tcl->result != '1') return -1;

    DoTcl(cmd);
    return 0;
  }

  return 0;
}

/*****************************************************************************
 *
 * Decode and report an error message from the simulator
 *
 * Parameters:
 *      si		Simulator interface object
 *      level		Level of error (0=warning, 1=error)
 *      msg		Encoded error message.
 *
 *****************************************************************************/
int SimInterface_reportError(SimInterface *si,int level,const char *msg)
{
  char etype[STRMAX],text[STRMAX];
  char buf[STRMAX];

  if (sscanf(msg,"%*s %s %[^\n]",etype,text) != 2)
    return -1;

  if (strcmp(etype,"file") == 0) {
    Error_report(msg);
  } else if (strcmp(etype,"run") == 0) {
    char timeStr[STRMAX],msg[STRMAX];
    if (sscanf(text,"%s : %[^\n]",timeStr,msg) == 2) {
      char *p = buf;
      Error_decode(msg);
      p += sprintf(p,msgLookup("err.sim.run"), timeStr);
      sprintf(p," %s",msg);
      message(MC_MSGLOG|MC_URGENT,"%s",buf);
    } else {
      message(MC_MSGLOG|MC_URGENT,"[BADLY FORMATTED MESSAGE] %s",text);
    }
  } else if (strcmp(etype,"command") == 0) {
    Error_decode(text);
    sprintf(buf,"%s: %s",msgLookup("err.sim.cmd"),text);
    message(MC_MSGLOG|MC_URGENT,"%s",buf);
  } else {
    Error_decode(text);
    sprintf(buf,"Unknown Error Type: %s",text);
    message(MC_MSGLOG|MC_URGENT,"%s",buf);
  }

  return 0;
}

/*****************************************************************************
 *
 *
 *
 *****************************************************************************/
void SimInterface_setSimMods(SimInterface *si)
{
  List L;
  EditState *es;
  ListElem *E;

  List_init(&L);

  for (es = TkGate.circuit->es;es;es = es->parent)
    List_addToHead(&L,es);

  E = List_first(&L);
  es = (EditState*) ListElem_obj(E);
  es->smod = si->sim_root;
  for (E = List_next(&L,E);E;E = List_next(&L,E)) {
    es = (EditState*) ListElem_obj(E);
    assert(es->inst);
    /*    printf("smod = %s\n",es->inst->ename);*/
    es->smod = (GSimModule*)SHash_find(es->parent->smod->children,es->inst->ename);
  }

  List_uninit(&L);
}

/*****************************************************************************
 *
 * Show simulator startup message.
 *
 *****************************************************************************/
void SimInterface_showStartMsg()
{
  time_t now;
  char *nowstr,*p; 

  time(&now);
  nowstr = ctime(&now);
  if (nowstr) {
    p = strrchr(nowstr,'\n');
    if (p) *p = 0;
    DoTcl("InfoPanel::log \"Starting simulator at %s.\" -noshow 1 -color blue",nowstr);
  } else
    DoTcl("InfoPanel::log \"Starting simulator at unknown time.\" -noshow 1 -color blue");
}

/*****************************************************************************
 *
 * Start the simulator
 *
 * Parameters:
 *     si		Simulation interface to use.
 *
 *****************************************************************************/
void SimInterface_begin(SimInterface *si)
{
  EditState *es;
  Circuit *c = TkGate.circuit;

  SimInterface_showStartMsg();
  editstate_makeRootAtTop(&c->es);
  es = c->es;

  getSimTempFile(si->simFileName);				/* Get a temp file for circuit */

  if (VerilogWriteModules(si->simFileName,VSO_ALLMODS|VSO_NOHDLCHECK) != 0) {	/* Save circuit to send to simulator */
    /* Could not save temporary file '%s' for simulator (disc full?) */
    message(1,msgLookup("err.sim.badtmp"),si->simFileName);
    return;
  }

  SimInterface_buildSimMods(si);				/* Build the simulator hierarchy */

  /*
     Check to see if the top module on the module stack is really the root
     circuit.  If not, switch to the root circuit.
   */
  for (es = c->es;es->parent;es = es->parent);
  if (c->root_mod != es->env) {
    while (c->es)
      editstate_pop(&c->es);
    editstate_push(&c->es,c->root_mod,0);
    editstate_setCurrent(c->es);

    /* Simulation mode requires root module at top of edit stack. */
    message(0,msgLookup("msg.sim.chgtoroot"));
  }

  /*
     Pop modules that don't have a concrete gate associated with them.
   */
  if (c->es->parent && !c->es->inst) {
    while (c->es->parent && !c->es->inst)
      editstate_pop(&c->es);
    editstate_setCurrent(c->es);
    message(0,msgLookup("msg.sim.nogatemod"));	/* Simulation mode requires concrete module stack. */
  }

  SimInterface_setSimMods(si);

  /* Start simulator with specified file */
  switch (c->c_tvMode) {
  case TV_NONE :
    DoTcl("tkg_startSim %s none",
	  si->simFileName,
	  c->c_startup,SimInterface_unitsToStr(c->c_startupUnits)
	  );
    break;
  case TV_AFTER :
    DoTcl("tkg_startSim %s %g%s",
	  si->simFileName,
	  c->c_startup,SimInterface_unitsToStr(c->c_startupUnits)
	  );
    break;
  case TV_ALL :
  default:
    DoTcl("tkg_startSim %s 0ns", si->simFileName);
    break;
  }

  si->active = 1;

  FlagRedraw();
}

/*
  Missing cleanup code for simulator module stuff
 */
void SimInterface_end(SimInterface *si)
{
  GScope_saveProbes(Scope);

  si->active = 0;
  //  if (*si->simFileName)
  //unlink(si->simFileName);
  DoTcl("tkg_endSim");
  DoTcl("Scope::unpost");
#if 0
  SimInterface_flushBreaks(si);
#endif
}

static int SimInterface_findPathToModule_aux(GCElement **path,
					     int depth,
					     GModuleDef *p,
					     GModuleDef *targetModule)
{
  HashElem *e;

  /*
   * Found a path to the target module
   */
  if (p == targetModule)
    return depth;

  for (e = Hash_first(p->m_gates);e;e = Hash_next(p->m_gates,e)) {
    GCElement *g = (GCElement*) HashElem_obj(e);

    if (GCElement_isModule(g) && g->u.block.moduleName && *g->u.block.moduleName) {
      GModuleDef *child = env_findModule(g->u.block.moduleName);
      if (child) {
	int r = SimInterface_findPathToModule_aux(path,depth+1,child,targetModule);
	if (r > 0) {
	  path[depth] = g;
	  return r;	/* Found a path */
	}
      }
    }
  }

  /*
   * Did not find a path.
   */
  return -1;
}

int SimInterface_findPathToModule(char *path,GModuleDef *m)
{
  GCElement *mpath[MODULE_PATH_MAX];
  char *p;
  int n,i;
  
  n = SimInterface_findPathToModule_aux(mpath, 0, TkGate.circuit->root_mod, m);

  if (n <= 0)
    return -1;

  sprintf(path,"/<%s>",GModuleDef_getName(TkGate.circuit->root_mod));
  p = path+strlen(path);

  for (i = 0;i < n;i++) {
    GModuleDef *pathMod = env_findModule(mpath[i]->u.block.moduleName);
    sprintf(p,"/%s<%s>",mpath[i]->ename,GModuleDef_getName(pathMod));
    p += strlen(p);
  }

  return 0;
}

void SimInterface_navigateToModule(EditState **es,const char *path)
{
  char pathbuf[STRMAX],*t;
  char name[STRMAX];
  GModuleDef *M = TkGate.circuit->root_mod;

  if (strncmp(path,"/<",2) != 0) {
#if 1 
      message(1,msgLookup("err.nojump"));
#else
    M = env_findModule(path);
    if(!M) {
      message(1,msgLookup("err.nojump"));
      return;
    }

    if (SimInterface_findPathToModule(pathbuf, M) < 0) {
      message(1,msgLookup("err.nojump"));
      return;
    }

    if (*pathbuf != '/') {
      message(1,msgLookup("err.nojump"));
      return;
    }

    //
    // Try again with full path
    //
    SimInterface_navigateToModule(es,pathbuf);
#endif
    return;
  }

  /*
   * Pop up to root module
   */
  while (*es && (*es)->env != M)
    editstate_pop(es);

  strcpy(pathbuf,path);
  t = strtok(pathbuf,"/");
  for (t = strtok(0,"/");t;t = strtok(0,"/")) {
    if (sscanf(t,"%[^<]",name) == 1) {
      GCElement *g = GModuleDef_findGate(M,name);

      if (GCElement_isModule(g) && g->u.block.moduleName && *g->u.block.moduleName) {
	M = env_findModule(g->u.block.moduleName);
	if (M)
	  editstate_push(es,M,g);
      }
    }
  }
  editstate_setCurrent(*es);

  //  DoTcl("BlockTree::setActive %s",path);

  FlagRedraw();
}

const char* SimInterface_unitsToStr(int u)
{
  if (u >= 0 && u < NUM_UNITS) 
    return unitStrings[u];
  else
    return "bs";
}

int SimInterface_strToUnits(const char *s)
{
  int u;
  
  for (u = 0;u < NUM_UNITS;u++)
    if (strcmp(s,unitStrings[u]) == 0)
      return u;

  return -1;
}

/*****************************************************************************
 *
 * Convert a time in epochs to a time with units after it using the timescale
 * setting returned from the simulator.
 *
 *****************************************************************************/
char *SimInterface_formatTime(SimInterface *si, char *buf,simtime_t t)
{
  double n = (double)t*si->si_tsmult/(double)si->si_precision;
  int units = si->si_units;


  while (n > 1000 && units > 0) {
    n /= 1000;
    units--;
  }

  sprintf(buf,"%g%s",n,SimInterface_unitsToStr(units));

  return buf;
}

/*****************************************************************************
 *
 * Commands received from the simulator are processed here.
 *
 * Parameters:
 *      si		Simulator interface object
 *      C		Command to be processed
 *
 * Commands:
 *    comment		Ignore any text on this line
 *    echo		Echo text on this line to the log
 *    ok		Indicates circuit was read and is ready to be simulated
 *    error_exit	Indicates there was an error in the circuit while reading
 *    showprobe         Request from simulator to place a probe
 *    hideprobe         Request from simulator to hide a probe
 *    valueof		Indicates the value of a signal changed
 *    tell		Tell a listener that state has changed on a monitored net.
 *    netdelay		-currently not used-
 *    cpath		-currently not used-
 *    cdone		-currently not used-
 *    stats		Indicates statistics about a circuit.
 *    badscript		Indicates an error occured in a simulation script.
 *    endscript		Indicates a script has completed
 *    break		Indicates that simulation has stopped at a breakpoint.
 *    stop		Indicates that simulation has stopped.
 *    post		Requests that a VPD be posted
 *    exec		Requests that a tcl command be executed
 *    error		Reports an error 
 *    warning		Reports a warning 
 *
 *****************************************************************************/
int SimInterface_command(SimInterface *si,const char *C)
{
  char buf[STRMAX],buf2[STRMAX],buf3[STRMAX],buf4[STRMAX];
  int a1,a2;
  simtime_t t = 0;
  unsigned u1,u2,u3,u4;

  if (strncmp(C,"stop",4) == 0) {
    *buf = 0;
  }

  while (*C == ' ')C++;

  if (strncmp(C,"comment",7) == 0) {				/* Ignore comments from simulator */
    return 0;
  } else if (sscanf(C,"echo %[^\n]",buf) == 1) {
    DoTcl("InfoPanel::log \"%s\"",quoteChars(buf2,buf,TCL_SPECIALCHARS));
    return 0;
  } else if (strncmp(C,"echo",4) == 0) {
    DoTcl("InfoPanel::log \"\"");
    return 0;
  } else if (sscanf(C," zoom %d",&a1) == 1) {			/* Set zoom factor */ 
    DoTcl(".scope.main.frame.canvas setzoom %d",-a1);
  } else if (sscanf(C,"ok %d %s / %d %s",&a1,buf,&a2,buf2) == 4) { /* Simulator loaded file and is ready to go */
    si->si_tsmult = a1;
    si->si_units = SimInterface_strToUnits(buf);
    si->si_precision = Timescale_parse(a1,buf)/Timescale_parse(a2,buf2);
    if (si->si_precision == 0) si->si_precision = 1;
    Error_close();
    SimInterface_startUp(si);
  } else if (strncmp(C,"error_exit",10) == 0) {			/* The simulator exited on an error */
    DoTcl("ErrBox::hadFatalErrors");
    Error_close();
    tkgate_setMajorMode(MM_EDIT);
  } else if (sscanf(C," showprobe %s %d", buf, &a1) == 2) {	/* Probe set from script */ 
    SimInterface_addProbe(si,buf,a1);
  } else if (sscanf(C," hideprobe %s", buf) == 1) {		/* Probe hidden from script */ 
    SimInterface_delProbe(si,buf);
  } else if (sscanf(C," valueof %s %s @ %llu",			/* The value of a net has changed */ 
		    buf,buf2,&t) == 3) {
    if (GScope_findTrace(Scope,buf)) {
      Scope_stepTo(t);
      Scope_setValue(buf,buf2);
    }
  } else if (sscanf(C," tell $queue %s %s @ %llu",buf,buf2,&t) == 3) {	/* The value of a net has been requested */ 
    DoTclL("VPD::qdata",buf,buf2,NULL);
  } else if (sscanf(C," tell $show %s %s",buf,buf2) == 2) {	/* The value of a net has been requested */ 
    sprintf(buf3,"%s=%s",buf,buf2);
    Tcl_SetVar(TkGate.tcl,"tkg_simDisplayedVal",
	       buf3,TCL_GLOBAL_ONLY);
  } else if (sscanf(C," tell $led:%s %*s %s @ %llu",buf,buf2,&t) == 3) {	/* Set value of an led */
    SimInterface_setLed(si,buf,buf2);
  } else if (sscanf(C," tell $switch:%s %*s %s @ %llu",buf,buf2,&t) == 3) {	/* Set value of an led */
    SimInterface_setSwitch(si,buf,buf2);
  } else if (sscanf(C," netdelay %s %d %d",buf,&a1,&a2) == 3) {	/* Net delay values */
    cpath_registerNetDelay(buf,a1,a2);
  } else if (sscanf(C," cpath %d %[^\n]",&a1,buf) == 2) {	/* Critical path */
    DoTcl("tkg_cpathAdd %d {%s}",a1,buf);
  } else if (strncmp(C,"cdone",5) == 0) {			/* End of critical path data */
    DoTcl("tkg_cpathEnd");
  } else if (sscanf(C," stats area=%d static_power=%d",		/* Circuit statistics */ 
		    &si->area,&si->staticPower) == 2) {
    message(0,"Estimated area=%d.",si->area,si->staticPower);
  } else if (sscanf(C," badscript %s",buf) == 1) {		/* Report breakpoint/script syntax error */
    if (sscanf(buf,"break:%d",&a1) == 1)
      BrkPtTable_error(TkGate.circuit->c_breakpoints,a1);
    else if (sscanf(buf,"script:%d",&a1) == 1) {
      DoTcl("ScriptMgr::setState %d -1",a1);
      DoTcl("ScriptMgr::viewScriptFile script:%d",a1);
    }
  } else if (sscanf(C," endscript %s",buf) == 1) {		/* Report script termination */
    if (sscanf(buf,"script:%d",&a1) == 1)
      DoTcl("ScriptMgr::setState %d 1",a1);
  } else if (sscanf(C," break %d %s",&a1,buf) == 2) {		/* Simulator hit a breakpoint */ 
    BrkPtTable_activate(TkGate.circuit->c_breakpoints,a1,buf);
  } else if (sscanf(C," time @ %llu",&t) == 1) {		/* Update current time */
  } else if (sscanf(C," go @ %llu",&t) == 1) {			/* Simulator is in run mode */
    DoTcl("tkg_runLogo");
    DoTcl("Simulator::startTimeUpdates");
    DoTcl("set ::simulatorState go");
  } else if (sscanf(C," stop @ %llu",&t) == 1) {		/* Simulator is in pause mode */
    if (si->no_scope) return 0;

    if (Scope->s_time != t)
      DoTcl("tkg_stepLogo");
    DoTcl("Simulator::stopTimeUpdates");
    DoTcl("tkg_pauseLogo");
    Scope_stepTo(t);
    DoTcl("set ::simulatorState stop");
  } else if (sscanf(C," readystop @ %llu",&t) == 1) {		/* Simulator is in paused and waiting for a net change  */
    if (si->no_scope) return 0;

    if (Scope->s_time != t)
      DoTcl("tkg_stepLogo");
    DoTcl("Simulator::stopTimeUpdates");
    DoTcl("tkg_pauseLogo");
    Scope_stepTo(t);
    DoTcl("set ::simulatorState readystop");
  } else if (sscanf(C," post %[^\n]",buf) == 1) {		/* Post a VPD */
    int numArgs;
    const char **args;

    if (Tcl_SplitList(TkGate.tcl, buf, &numArgs, &args) == TCL_OK) {
      DoTclV("VPD::post",numArgs,args);
      Tcl_Free((char*)args);
    }
  } else if (sscanf(C," netinfo %s %*s [%u:%u] [%u:%u]",buf,&u1,&u2,&u3,&u4) == 5) {		/*  */
    DoTcl("MemView::configMemory %s %d %d %d",buf,u1-u2+1,u3,u4);
  } else if (sscanf(C," memory %s %u : %[^\n]",buf,&u1,buf2) == 3) {		/*  */
    DoTcl("MemView::setData %s %u %s",buf,u1,buf2);
  } else if (sscanf(C," memory-addr %s %u %s %s @ %llu",buf,&u1,buf2,buf3,&t) == 5) {	/*  */
    DoTcl("MemView::setAccessAddr %s %u %s %s %llu",buf,u1,buf2,buf3,t);
  } else if (sscanf(C," exec %[^\n]",buf) == 1) {		/* Create a tty window */
    if (SimInterface_doSanitizedTcl(buf) < 0) {
      message(MC_MSGLOG|MC_URGENT,msgLookup("err.protexec"),buf);
    }
  } else if (sscanf(C," warning %s",buf) == 1) {		/* An error in a simulator input file. */
    SimInterface_reportError(si,0,C);
  } else if (sscanf(C," error %s",buf) == 1) {			/* An error in a simulator input file. */
    SimInterface_reportError(si,1,C);
  } else if (sscanf(C," scripterror %s %s %s : %[^\n]",buf,buf2,buf3,buf4) == 4) { /* An error in a script file. */
    Error_decode(buf4);
    DoTclL("ScriptMgr::reportError",buf,buf2,buf3,buf4,NULL);
  } else if (sscanf(C," embeddedscript begin %s %u",buf,&u1) == 2) {  /* Beginning of an embedded script. */
    Error_scriptBegin(buf,u1);
  } else if (sscanf(C," embeddedscript end %s %u",buf,&u1) == 2) {  /* End of an embedded script. */
    Error_scriptEnd(buf,u1);
  }

  if (t > 0) {
    DoTcl("set ::currentSimTime [gat_formatTime %llu]",t);
  }

  return 0;
}

 
