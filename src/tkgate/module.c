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

    Last edit by hansen on Wed Mar 18 05:15:53 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "tkgate.h"

GModuleDef *new_GModuleDef(const char *name)
{
  GModuleDef *M;

  M = (GModuleDef*) ob_malloc(sizeof(GModuleDef),"GModuleDef");

  M->m_name = ob_strdup(name);
  M->m_fileName = 0;
  M->m_libName = 0;
  M->m_type = MT_NETLIST;
  M->m_lineno = 0;
  M->m_isLib = 0;
  M->m_protData = 0;
  M->m_protIntf = 0;
  M->m_protEdit = 0;
  M->m_hidden = 0;
  M->m_used = 0;
  M->m_isModload = 0;
  M->m_isModsave = 0;
  M->m_isSpecial = 0;
  M->m_wires = NULL;
  M->m_interface = 0;
  M->m_altInterface = 0;
  M->m_gates = new_SHash();
  M->m_nets = new_SHash();
  M->m_pptype = PPT_SCALED;
  M->m_text = 0;
  M->m_textSize = 0;
  M->m_curLine = 0;
  M->m_curChar = 0;
  M->m_needScan = 1;
  M->m_parmPorts = 0;

  /*
   * If the module name begins with a '<' we assume this is a special
   * module such as the cut buffer, interface list, or interface edit
   * buffer.  Special modules do not appear in the list of modules and
   * are only used internally.
   */
  if (*name == '<')
    M->m_isSpecial = 1;

  return M;
}

void delete_GModuleDef(GModuleDef *M)
{
  ob_touch(M);
  env_clear(M);
  ob_free(M->m_name);
  delete_SHash(M->m_gates);
  delete_SHash(M->m_nets);
  if (M->m_text) ob_free(M->m_text);
  if (M->m_parmPorts) delete_SHash(M->m_parmPorts);
  ob_free(M);
}

/*****************************************************************************
 *
 * Set the name of the library this module is from and mark it as a library module.
 *
 *****************************************************************************/
void GModuleDef_setLibrary(GModuleDef *M,const char *name)
{
  ob_touch(M);
  M->m_libName = ob_strdup(name);
  M->m_isLib = 1;
}

void GModuleDef_setFileName(GModuleDef *M,const char *name)
{
  ob_touch(M);
  M->m_fileName = ob_strdup(name);
}

void GModuleDef_claim(GModuleDef *M)
{
  ob_touch(M);
  M->m_libName = 0;
  M->m_isLib = 0;
}


GNet *GModuleDef_findNet(GModuleDef *M,const char *name)
{
  return (GNet*) SHash_find(M->m_nets,name);
}

GCElement *GModuleDef_findGate(GModuleDef *M,const char *name)
{
  return (GCElement*) SHash_find(M->m_gates,name);
}

static GWireNode *replicate_wirenodes(GWireNode *sn,int dx,int dy)
{
  GWireNode *cn;

  if (!sn) return 0;

  cn = new_GWireNode();

  cn->x = sn->x + dx;
  cn->y = sn->y + dy;
  cn->showSize = sn->showSize;
  cn->isLabeled = sn->isLabeled;
  cn->labelSide = sn->labelSide;
  cn->offset = sn->offset;

  cn->out = replicate_wirenodes(sn->out,dx,dy);

  if (cn->out) {
    ob_touch(cn->out);
    cn->out->in = cn;
  }

  return cn;
}

static void copyWireProps(GWire *nw,GWire *w)
{
  ob_touch(nw);
  nw->orient = w->orient;
  nw->nidx = w->nidx;
  nw->xanchor = w->xanchor;
  nw->yanchor = w->yanchor;
}

static GWire *replicate_wire(GWire *w,GModuleDef *M,int dx,int dy)
{
  GWire *nw,*nw2,*ow;
  GWireNode *n;

  ow = wire_other(w);

  nw = wire_newend(M,0,0);
  nw2 = wire_newend(M,0,0);

  ob_touch(nw);
  ob_touch(nw2);

  n = replicate_wirenodes(w->driver->nodes,dx,dy);

  nw->driver = nw;
  nw2->driver = nw;
  nw->wtype = DRIVER;
  nw2->wtype = DRIVEE;

  nw->nodes = n;
  while (n->out) n = n->out;
  nw2->nodes = n;

  nw->nodes->end = nw;
  nw2->nodes->end = nw2;

  if (w->driver != w) {
    GWire *xw = nw;
    nw = nw2;
    nw2 = xw;
  }

  copyWireProps(nw,w);
  copyWireProps(nw2,ow);

  return nw;
}

static GWire *replicate_attachment(GWire *w,GCElement *ng,NHash *whash)
{
  GWire *nw;

  if (!w) return 0;

  nw = (GWire*) NHash_find(whash,(int)w);

  ob_touch(nw);

  if (!nw) {
    logError(ERL_WARN,"Could not replicate attachment.");
    return 0;
  }

  nw->next = replicate_attachment(w->next,ng,whash);

  nw->offset.num = w->offset.num;
  nw->offset.den = w->offset.den;
  nw->WireDir = w->WireDir;
  nw->PadNum = w->PadNum;
  nw->invert = w->invert;
  nw->gate = ng;
  if (w->name) nw->name = ob_strdup(w->name);

  return nw;
}

/*****************************************************************************
 * Paste circuit in S into D.  Generally, S is a cut buffer and D is the
 * current module (or vice versa).  The coordinates of the circuit in S
 * is adjusted by the offset (dx, dy) as it is pasted.  If selOnly is
 * set, then only seleced circuit elements are copied.  If selDst is
 * set, then the pasted circuit elements are left as the current selection.
 *****************************************************************************/
void GModuleDef_copyInto(GModuleDef *D,GModuleDef *S,int dx,int dy,int selOnly,int selDst)
{
  HashElem *E;
  NHash *ghash = new_NHash();		/* Map from source gates to destination gates */
  NHash *whash = new_NHash();		/* Map from source wires to destination wires */
  NHash *nhash = new_NHash();		/* Map from source nets to destination nets */
  NHash *rhash = new_NHash();		/* Map from destination nets to destination root wire */
  GWire *w;
  int i;

  /*
    Create maps for gates, wires and nets
   */
  for (E = Hash_first(S->m_gates);E;E = Hash_next(S->m_gates,E)) {
    GCElement *g,*ng;
    int N;

    g = (GCElement*) HashElem_obj(E);
    N = GCElement_numPads(g);
    if (selOnly && !g->selected)
      continue;

    ng = (*g->typeinfo->CopyGate)(D,g,g->xpos+dx,g->ypos+dy,REP_NOWIRES);
    NHash_insert(ghash,(int)g,ng);

    ob_touch(ng);

    if (selDst) ng->selected = 1;

    for (i = 0;i < N;i++) {
      for (w = g->wires[i];w;w = w->next) {
	if (!NHash_find(whash,(int)w)) {
	  GWire *ow = wire_other(w);
	  GWire *nw = replicate_wire(w,D,dx,dy);
	  GWire *onw = wire_other(nw);

	  NHash_insert(whash,(int)w,nw);
	  NHash_insert(whash,(int)ow,onw);
	}

	if (!NHash_find(nhash,(int)w->net)) {
	  GNet *n = w->net;
	  GNet *nn = new_GNet(n->n_signame,D);

	  ob_touch(nn);

	  nn->n_driver = (GWire*) NHash_find(whash,(int)w);
	  nn->n_nbits = n->n_nbits;
	  nn->n_show_name = n->n_show_name;

	  NHash_insert(nhash,(int)w->net,nn);
	}
      }
    }
  }

  /*
   * Set nets of copied wires
   */
  for (E = Hash_first(whash);E;E = Hash_next(whash,E)) {
    GWire *sw = (GWire*) NHashElem_key(E);
    GWire *dw = (GWire*) HashElem_obj(E);
    GNet *net = (GNet*) NHash_find(nhash,(int)sw->net);
    wire_setNet(dw,net);
  }

  /*
   * Attach wires to copied gates
   */
  for (E = Hash_first(ghash);E;E = Hash_next(ghash,E)) {
    GCElement *g = (GCElement*) NHashElem_key(E);
    GCElement *ng = (GCElement*) HashElem_obj(E);
    int N = GCElement_numPads(g);
    int i;

    ob_touch(ng);
    for (i = 0;i < N;i++)
      ng->wires[i] = replicate_attachment(g->wires[i],ng,whash);
  }


  /*
   * Set i/o gates and finalize nets
   */
  for (E = Hash_first(nhash);E;E = Hash_next(nhash,E)) {
    GNet *n = (GNet*) NHashElem_key(E);
    GNet *nn = (GNet*) HashElem_obj(E);

    if (n->n_ionet)  {
      ob_touch(nn);
      nn->n_ionet = (GCElement*) NHash_find(ghash,(int)n->n_ionet);
    }

    wire_finalizeNet(nn->n_driver);
  }

  /*
   * Check for partitioned nets and make the partitions separate nets
   * if any are found.
   *
   */
  for (E = Hash_first(whash);E;E = Hash_next(whash,E)) {
    GWire *dw = (GWire*) HashElem_obj(E);
    GNet *net = dw->net;
    GWire *rw = (GWire*) NHash_find(rhash,(int)net);
    GWire *dw_r;

    if (!rw) {
      dw_r = rw = wire_sigroot(dw);
      NHash_insert(rhash,(int)net,rw);
    } else
      dw_r = wire_sigroot(dw);
#if 0
    printf("PCHECK: %s",net->n_signame);
    printf(" dw:{%d}(%x)",dw->nidx,dw);
    printf(" dw_r:{%d}(%x)",dw_r->nidx,dw_r);
    printf(" rw:{%d}(%x)",rw->nidx,rw);
    printf(" nd:{%d}(%x)",net->driver->nidx,net->driver);
    printf("\n");
#endif
    if (rw != dw_r) {
      GNet *nn = new_GNet(net->n_signame,D);
#if 0
      printf("  nn:%s net:%s\n",nn->n_signame,net->n_signame);
#endif
      ob_touch(nn);
      nn->n_driver = dw_r;
      nn->n_nbits = net->n_nbits;
      nn->n_show_name = net->n_show_name;

      wire_setNet(dw_r,nn);

      wire_finalizeNet(dw_r);
      wire_finalizeNet(net->n_driver);
    }
  }

  delete_NHash(rhash);
  delete_NHash(nhash);
  delete_NHash(whash);
  delete_NHash(ghash);
}

int GModuleDef_numHdlLines(GModuleDef *M)
{
  char *c;
  int numLines = 1;

  if (GModuleDef_getType(M) == MT_NETLIST) return 0;

  for (c = M->m_text;*c;c++)
    if (*c == '\n')
	numLines++;

  return numLines;
}

void GModuleDef_getBBX(GModuleDef *M,TargetDev_e target, int *minX,int *maxX,int *minY,int *maxY)
{
  if (GModuleDef_getType(M) == MT_NETLIST) {
    HashElem *E;
    GWireList *wl;
    int is_first = 1;

    *minX = *maxX = *minY = *maxY = 0;

    for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
      int g_minX,g_minY,g_maxX,g_maxY;

      GCElement *g = (GCElement*) HashElem_obj(E);

      gate_getbbx(g, target, &g_minX,&g_minY,&g_maxX,&g_maxY);
      if (is_first || g_minX < *minX) *minX = g_minX;
      if (is_first || g_minY < *minY) *minY = g_minY;
      if (is_first || g_maxX > *maxX) *maxX = g_maxX;
      if (is_first || g_maxY > *maxY) *maxY = g_maxY;
      is_first = 0;
    }

    for (wl = M->m_wires;wl;wl = wl->wl_next) {
      GWire *w = wl->wl_wire;
      GWireNode *n;

      if (!w->nodes->out) continue;

      for (n = w->nodes;n;n = n->out) {
	if (n->x < *minX) *minX = n->x;
	if (n->y < *minY) *minY = n->y;
	if (n->x > *maxX) *maxX = n->x;
	if (n->y > *maxY) *maxY = n->y;
      }
    }
  } else {
    extern HtmlFont hdl_font;
    char *c,*s;
    int maxLen = 0;
    int numLines = 1;
    int lineLen = 0;

    s = M->m_text;
    for (c = M->m_text;*c;c++) {
      if (*c == '\n') {
	numLines++;
	lineLen = PSStringWidth(&hdl_font,s,c-s);
	if (lineLen > maxLen) maxLen = lineLen;
	s = c+1;
      }
    }

    *minX = *minY = 0;
    *maxX = maxLen;
    *maxY = numLines*(int)(1.2*hdl_font.size);
  }
}

static GModuleDef *GModuleDef_isRecursive_aux(GModuleDef *M,NHash *mhash)
{
  HashElem *E;
  GModuleDef *R;

  if (NHash_find(mhash,(int)M))
    return M;
  NHash_insert(mhash,(int)M,(void*)1);

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (GCElement_isModule(g)) {
      GModuleDef *C = env_findModule(g->u.block.moduleName);
      if (C) {
	R = GModuleDef_isRecursive_aux(C,mhash);
	if (R) return R;
      }
    }
  }

  NHash_remove(mhash,(int)M);

  return 0;
}


/*
 * Check to see if there are any recursive loops in M.  Returns null if
 * no loops are found, and returns the offending module if one is found.
 */
GModuleDef *GModuleDef_isRecursive(GModuleDef *M)
{ 
  NHash *mhash = new_NHash();		/* set of modules we have seen so far. */
  GModuleDef *R;

  R = GModuleDef_isRecursive_aux(M,mhash);

  delete_NHash(mhash);

  return R;
}

/*****************************************************************************
 *
 * Returns non-zero if M contains any module instances.
 *
 *****************************************************************************/
int GModuleDef_hasSubModules(GModuleDef *M)
{
  HashElem *E;

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (GCElement_isModule(g)) {
      return 1;
    }
  }

  return 0;
}


int GModuleDef_isDataProtected(GModuleDef *M)
{
  return ((M)->m_protData || (M)->m_isLib);
}

int GModuleDef_isIntfProtected(GModuleDef *M)
{
  return M->m_protIntf || M->m_isLib || (M->m_protEdit && !editstate_isInterfaceMode());
}

void GModuleDef_flush(GModuleDef *M)
{
  env_clear(M);
  if (M->m_text) {
    ob_free(M->m_text);
    M->m_text = ob_strdup("");
  }
}

void GModuleDef_getPropsToTcl(GModuleDef *M, Tcl_Interp *tcl,char *aname)
{
  static char *modTypes[] = {"netlist", "hdl"};
  char shortfile[STRMAX],*p;

  if (M->m_fileName) {
    p = strrchr(M->m_fileName,'/');
    if (p)
      strcpy(shortfile,p+1);
    else
      strcpy(shortfile,M->m_fileName);
  } else
    strcpy(shortfile,"<new>");
      
    
  Tcl_SetVar2(tcl,aname,"file",(M->m_fileName ? M->m_fileName : "") ,TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"shortfile",shortfile,TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"type",modTypes[M->m_type],TCL_GLOBAL_ONLY);
  if (M->m_interface) {
    if (GCElement_getType(M->m_interface) == GC_SYMBLOCK) 
      Tcl_SetVar2(tcl,aname,"itype","symbol",TCL_GLOBAL_ONLY);
    else
      Tcl_SetVar2(tcl,aname,"itype","block",TCL_GLOBAL_ONLY);
  } else
    Tcl_SetVar2(tcl,aname,"itype","block",TCL_GLOBAL_ONLY);

  Tcl_SetVar2(tcl,aname,"prot",binstr(M->m_protData),TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"protint",binstr(M->m_protIntf),TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"proted",binstr(M->m_protEdit),TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"inuse","1",TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"modload",binstr(M->m_isModload),TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"modsave",binstr(M->m_isModsave),TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"libname",(M->m_isLib ? M->m_libName : ""),TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"wantclaim","0",TCL_GLOBAL_ONLY);
  Tcl_SetVar2(tcl,aname,"ppos",(M->m_pptype == PPT_FIXED ? "fix" : "scale"),TCL_GLOBAL_ONLY);
}

void GModuleDef_setPropsFromTcl(GModuleDef *M, Tcl_Interp *tcl,char *aname)
{
  const char *r;

  ob_touch(M);
  r = Tcl_GetVar2(tcl,aname,"prot",TCL_GLOBAL_ONLY);
  M->m_protData = strbin(r);
  r = Tcl_GetVar2(tcl,aname,"protint",TCL_GLOBAL_ONLY);
  M->m_protIntf = strbin(r);
  r = Tcl_GetVar2(tcl,aname,"proted",TCL_GLOBAL_ONLY);
  M->m_protEdit = strbin(r);
  Tcl_GetVar2(tcl,aname,"wantclaim",TCL_GLOBAL_ONLY);
  r = Tcl_GetVar2(tcl,aname,"ppos",TCL_GLOBAL_ONLY);

  if (strcmp(r,"fix") == 0) {
    M->m_pptype = PPT_FIXED;
  } else
    M->m_pptype = PPT_SCALED;
}

void GModuleDef_clearParmPorts(GModuleDef *M)
{
  if (M->m_parmPorts)
    SHash_flush(M->m_parmPorts);
}

void GModuleDef_addParmPort(GModuleDef *M,const char *name,const char *value)
{
  if (!M->m_parmPorts) {
    ob_touch(M);
    M->m_parmPorts = new_SHash();
  }

  SHash_insert(M->m_parmPorts, name, ob_strdup(value));
}

/*****************************************************************************
 *
 * Scan an HDL module to find ports, wires and instances.
 *
 * Parameters:
 *      M		Module to be scanned.
 *
 * Contact simulator to scan an HDL module for syntactical correctness and to
 * retrieve basic data about the module.
 *
 *****************************************************************************/
int GModuleDef_scanHDLModule(GModuleDef *M)
{
  char cmd[STRMAX],tempFile[STRMAX],data[STRMAX];
  const char *r;
  FILE *p,*f;
  int has_module_mismatch = 0;
  int count = 0;


  r = Tcl_GetVar(TkGate.tcl,"simExec", TCL_GLOBAL_ONLY);
  if (!r) return -1;

  M->m_needScan = 0;

  getSimTempFile(tempFile);

  sprintf(cmd,"%s -sq %s -D %u",r,tempFile,computestrhash(tempFile));

  f = fopen(tempFile, "w");
  if (!f) goto abortExit;

  VerilogSavePlainModule(f,M);
  fclose(f);

  p = popen(cmd, "r");
  if (!p) goto abortExit;

  env_clear(M);
  GModuleDef_clearParmPorts(M);

  while (fgets(data, STRMAX, p)) {
    char name[STRMAX],iname[STRMAX],msg[STRMAX];
    int msb,lsb,n;

    if (sscanf(data," module %s",name) == 1) {
      if (strcmp(name,M->m_name) != 0)
	has_module_mismatch = 1;
      count++;
    } else if (sscanf(data," line %d",&n) == 1) {
      M->m_lineno = n;
    } else if (sscanf(data," instance %s %s",name,iname) == 2) {
	GGateInfo *gi = GGateInfo_lookup("MODULE");
	GCElement *g;

	if ((g = (*gi->MakeFunction)(0,M,gi->Code,0,0,0,iname,1,0,0)))
	  g->u.block.moduleName = ob_strdup(name);
    } else if (sscanf(data," input %s %d:%d",name,&msb,&lsb) == 3) {
      GNet *n = new_GNet(name,M);
      net_setDType(n,NT_INPUT);
      net_setSize(n,msb-lsb+1);
    } else if (sscanf(data," output %s %d:%d",name,&msb,&lsb) == 3) {
      GNet *n = new_GNet(name,M);
      net_setDType(n,NT_OUTPUT);
      net_setSize(n,msb-lsb+1);
    } else if (sscanf(data," inout %s %d:%d",name,&msb,&lsb) == 3) {
      GNet *n = new_GNet(name,M);
      net_setDType(n,NT_INOUT);
      net_setSize(n,msb-lsb+1);
    } else if (sscanf(data," wire %s %d:%d",name,&msb,&lsb) == 3) {
      GNet *n = new_GNet(name,M);
      net_setDType(n,NT_WIRE);
      net_setSize(n,msb-lsb+1);
    } else if (sscanf(data," reg %s %d:%d",name,&msb,&lsb) == 3) {
      GNet *n = new_GNet(name,M);
      net_setDType(n,NT_REG);
      net_setSize(n,msb-lsb+1);
    } else if (sscanf(data," parmport %s %[^\n]",name,msg) == 2) {
      GModuleDef_addParmPort(M,name,msg);
    } else if (sscanf(data," endmodule %s",name) == 1) {
    } else if (sscanf(data," error %s %d %[^\n]",name,&n,msg) == 3) {
    } else if (sscanf(data," warning %s %d %[^\n]",name,&n,msg) == 3) {
    }
  }

  pclose(p);

  if (count > 1)
    message(MC_MSGLOG|MC_URGENT,msgLookup("err.modtoomany"),M->m_name);
  else if (has_module_mismatch)
    message(MC_MSGLOG|MC_URGENT,msgLookup("err.modmismatch"),M->m_name);

  return 0;

 abortExit:
  unlink(tempFile);
  return -1;
}

/*****************************************************************************
 *
 * Make the empty module D a copy of module S.
 *
 *****************************************************************************/
void GModuleDef_copy(GModuleDef *D,GModuleDef *S)
{
  GModuleDef_copyInto(D,S,0,0,0,0);

  if (S->m_type == MT_TEXTHDL) {

    D->m_type = MT_TEXTHDL;
    D->m_lineno  = S->m_lineno;
    if (D->m_text) ob_free(D->m_text);
    GModuleDef_saveText(D,S->m_text);
    D->m_curLine = S->m_curLine;
    D->m_curChar = S->m_curChar;
    D->m_needScan = 1;

    if (S->m_parmPorts) {
      HashElem *he;

      D->m_parmPorts = new_SHash();
      for (he = Hash_first(S->m_parmPorts);he;he = Hash_next(S->m_parmPorts,he))
	SHash_insert(D->m_parmPorts, SHashElem_key(he), ob_strdup((char*)HashElem_obj(he)));
    }


    hdl_replaceName(D,D->m_name);
  }
}

/*****************************************************************************
 *
 * Save text as the HDL text for a module.  Allocate new memory only if
 * necessary. 
 *
 *****************************************************************************/
void GModuleDef_saveText(GModuleDef *M,const char *text)
{
  int size = strlen(text) + 1;

  if (M->m_text) {
    if (M->m_textSize >= size) {
      ob_touch(M->m_text);
      strcpy(M->m_text, text);
      return;
    } else
      ob_free(M->m_text);
  }
  ob_touch(M);
  M->m_text = ob_strdup(text);
  M->m_textSize = size;
}

/*****************************************************************************
 *
 * Make sure the text has space for at least n bytes in it.  Any existing
 * text will be preserved.
 *
 *****************************************************************************/
void GModuleDef_allocText(GModuleDef *M,int n)
{
  if (M->m_text) {
    if (M->m_textSize >= n) return;

    ob_touch(M);
    M->m_text = (char*) ob_realloc(M->m_text, n);
  } else {
    ob_touch(M);
    M->m_text = (char*) ob_malloc(n,"char*");
  }
  M->m_textSize = n;
}


void GModuleDef_listPorts(GModuleDef *M)
{
  GCElement *g = M->m_interface;

  DoTcl("PortList::flush");
  if (g) {
    int i;
    int N = GCElement_numPads(g);
    GWire *w;

    for (i = 0;i < N;i++) {
      int dir = GCElement_getPadDir(g,i);
      char *type = "";

      switch (dir) {
      case IN  : type = "in"; break;
      case OUT : type = "out"; break;
      case TRI : type = "inout"; break;
      }

      for (w = g->wires[i];w;w = w->next) {
	DoTcl("PortList::add %s -bits %d -type %s",w->name,w->net->n_nbits,type);
      }
    }
  }

}

void GModuleDef_listNets(GModuleDef *M)
{
  HashElem *nl;

  DoTcl("NetList::flush");
  if (editstate_getInterfaceMode() == INTFMODE_NONE) {
    for (nl = Hash_first(M->m_nets);nl;nl = Hash_next(M->m_nets,nl)) {
      GNet *n = (GNet*) HashElem_obj(nl);
      DoTcl("NetList::add %s -hidden %d -bits %d -type %s",n->n_signame,!n->n_show_name,n->n_nbits,GNet_getDTypeStr(n));
    }
  }
}

/*****************************************************************************
 *
 * Test to see if we have selected a wire label.
 *
 *****************************************************************************/
int GModuleDef_grabLabel(GModuleDef *m,int x,int y,GrabbedLabel *gl)
{
  GWireList *wl;

  for (wl = m->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    GNet *net = w->net;
    GWireNode *n;
    int width = 0;

    if (!net->n_show_name) continue;

    for (n = w->nodes;n;n = n->out) {
      int label_x,label_y,label_p;

      if (!n->isLabeled) continue;

      if (width == 0)
	width = GNet_getLabelWidth(net);
      GWireNode_getLabelPos(n,net,&label_x,&label_y,&label_p);

      if (label_p & AtLeft) {
	if (x < label_x) continue;
	if (x > label_x + width) continue;
      } else {
	if (x < label_x - width) continue;
	if (x > label_x) continue;
      }

      if (label_p & AtBottom) {
	if (y > label_y) continue;
	if (y < label_y - 12) continue;
      } else {
	if (y > label_y + 12) continue;
	if (y < label_y) continue;
      }
      
      /*
       * We found matching net so lets grab it.
       */
      ob_suggest_name("MoveLabel");
      net_unselect(1);
      EditState_unselectGate(TkGate.circuit->es);
      GNet_draw(net);
      net_select(net,0);
      GrabbedLabel_set(net,label_x - x,label_y - y,label_p);
      ob_touch(n);
      n->isLabeled = 0;
      n->labelSide = 0;
      GNet_draw(net);
      GrabbedLabel_draw(x,y);

      return 1;
    }
  }

  return 0;
}
