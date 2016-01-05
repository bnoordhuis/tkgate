/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

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
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include "tkgate.h"

static SHash *cellsUsed;	/* Table of all built-in cells used */


/*
 * Return a string representing a port type from a gate type code.  If the gate is
 * not a module port then the null string is returned.
 */
const char *iotypeStr(int code)
{
  switch (code) {
  case IN :
    return "in";
  case OUT :
    return "out";
  case TRI :
    return "inout";
  }
  return "unknown";
}

/*
 * Return a verilog string representing a port type from a gate type code.  If the gate is
 * not a module port then the null string is returned.
 */
const char *verIotypeStr(int code)
{
  switch (code) {
  case IN :
    return "input";
  case OUT :
    return "output";
  case TRI :
    return "inout";
  }
  return "unknown";
}

/*
 * Transform a string for an iotype to a code value.
 */
int strIOType(const char *code)
{
  if (strcmp(code,"in") == 0)
    return IN;
  else if (strcmp(code,"out") == 0)
    return OUT;
  else if (strcmp(code,"inout") == 0)
    return TRI;
  else
    return -1;
}

/*****************************************************************************
 *
 * Generate the gate type name for gate g of type gi 
 *
 * Parameters:
 *    gi		Type info for gate
 *    g			The gate for which we are generating a name.
 *
 *****************************************************************************/
char *getTypeName(GGateInfo *gi,GCElement *g)
{
  static char buf[32];
  unsigned imask = 0;
  unsigned M = gi->vmask;
  int i,j;
  char *p;

  if (!M) return gi->vnames;

  j = 0;
  for (i = 0;i < 32 && M;i++) {
    unsigned Q = (1 << i);
    if ((M & Q)) {
      M &= ~Q;
      if (g->wires[i]->invert)
	imask |= (1 << j);
      j++;
    }
  }

  p = gi->vnames;
  for (;imask;imask--) {
    p += strcspn(p,":");
    if (*p == ':') p++;
  }

  for (i = 0;i < 31 && *p && *p != ':';)
    buf[i++] = *p++;
  buf[i] = 0;

  return buf;
}

int VerilogBasicGateCall(FILE *f,GCElement *g)
{
  static int gateID = 0;
  char typeName[STRMAX];

  GGateInfo_encodeTypeName(typeName,g);
  SHash_insert(cellsUsed,typeName,0);

  fprintf(f,"  %s",typeName);

  /*
   * If there are delay parameters generate the values.
   */
  if (g->typeinfo->delayNames[0]) {
    int delays[MAXDELAYS];
    GDelayDef *dd;
    int i;

    if (g->cust_delay) {
      for (i = 0;g->typeinfo->delayNames[i];i++) {
	delays[i] = g->delays[i];
      }
    } else {
      dd = GDelayDef_findTech(g->typeinfo->delay_defs, g->tech);
      if (!dd || GDelayDef_getDelays(dd, g, delays) < 0) {
	for (i = 0;i < MAXDELAYS;i++)
	  delays[i] = 1;
      }
    }

    fprintf(f," #(");
    for (i = 0;g->typeinfo->delayNames[i];i++) {
      if (i > 0) fprintf(f,", ");
      fprintf(f,"%d",delays[i]);
    }
    fprintf(f,")");
  }

  if (g->ename)
    fprintf(f," %s",g->ename);
  else {
    fprintf(f," g%d",gateID++);
    logError(ERL_WARN,"Saved gate with no name.");
  }
  return 0;
}

int VerilogBasicGateParmList(FILE *f,GCElement *g)
{
  GGateInfo *gi = g->typeinfo;
  GWire *w;
  int i,j;
  int first_pin;

  first_pin = 1;
  fprintf(f," (");
  for (i = 0;i < gi->NumPads;i++) {
    for (w = g->wires[i], j=0;w;w = w->next, j++) {
      char *inv = "";

      if (!first_pin)
	fprintf(f,", ");
      first_pin = 0;

      if (gi->Pad[i].iotype == IN && w->invert)
	inv = "~";

      if (gi->Pad[i].CanAdd)
	fprintf(f,".%s%d",gi->Pad[i].Name,j);
      else
	fprintf(f,".%s",gi->Pad[i].Name);
      if (w->net->n_signame)
	fprintf(f,"(%s%s)",inv,w->net->n_signame);
      else
	fprintf(f,"(%sw%x)",inv,(unsigned)w->net);
    } 
  }
  fprintf(f,");");
  return 0;
}

void VerilogBasicGateComment(FILE *f,GCElement *g,unsigned flags)
{
  GGateInfo *gi = g->typeinfo;
  GWire *w;
  int i,j;

  if ((flags & VGC_DOMARK))
    fprintf(f,"   //:");
  fprintf(f," @(%d,%d)",g->xpos,g->ypos);
  if (!g->show_name)
    fprintf(f," /sn:0");
  if (g->orient != 0)
    fprintf(f," /R:%d",g->orient);
  if (g->anchored != 0)
    fprintf(f," /anc:1");
  if (g->cpath_cut != 0)
    fprintf(f," /cpc:1");

  if (g->cust_delay) {
    int i;

    fprintf(f," /delay:\"");
    for (i = 0;gi->delayNames[i];i++) {
      fprintf(f," %d",g->delays[i]);
    }
    fprintf(f,"\"");
  } else {
    if (strcmp(g->tech,TKGATE_DEFAULT_TECH) != 0)
      fprintf(f," /tech:%s",g->tech);
  }

  if (!(flags & VGC_NOWIRES)) {
    if (gi->NumPads > 0) {
      fprintf(f," /w:[");
      for (i = 0;i < gi->NumPads;i++) {
	for (w = g->wires[i], j=0;w;w = w->next, j++) {
	  fprintf(f," %d",w->nidx);
	} 
      }
      fprintf(f," ]");
    }
  }
}

static void VerilogSaveWire(FILE *f,GWire *w,int doComment)
{
  GWireNode *n;
  GNet *net = w->net;

  if (doComment)
    fprintf(f,"//:");

  fprintf(f," {%d}",w->nidx);

  n = w->nodes;

  if (n->out) {
    for (;n;n = n->out) {
      fprintf(f,"(");

      if (n->showSize && net->n_nbits > 1)
	fprintf(f,"#:");
      if (n->isLabeled)
	fprintf(f,"%d:",n->labelSide ? -n->offset : n->offset);
      fprintf(f,"%d,%d)",n->x,n->y);
      w = n->end;
    }
  } else {
    for (;n;n = n->in) {
      fprintf(f,"(");
      if (n->in && n->in->showSize && w->net->n_nbits > 1)
	fprintf(f,"#:");
      if (n->in && n->in->isLabeled)
	fprintf(f,"%d:",n->in->labelSide ? -n->in->offset : n->in->offset);
      fprintf(f,"%d,%d)",n->x,n->y);
      w = n->end;
    }
  }

  if (w) {
    GCElement *g;
    GGateInfo *gi;
    int i;

    fprintf(f,"{%d}",w->nidx);
    fprintf(f,"\n");

    g = w->gate;
    if (!g) return;
    gi = w->gate->typeinfo;

    switch (gi->Code) {
    case GC_JOINT :
      for (i = 0;i < gi->NumPads;i++)
	if (g->wires[i] && g->wires[i] != w)
	  VerilogSaveWire(f,g->wires[i],1);
      break;
    case GC_TAP :
      if (g->wires[TAP_OUT] == w)
	VerilogSaveWire(f,g->wires[TAP_IN],1);
      if (g->wires[TAP_IN] == w)
	VerilogSaveWire(f,g->wires[TAP_OUT],1);
      break;
    }
  }
}

static void ClearNetMarks(GModuleDef *M)
{
  HashElem *nl;

  for (nl = Hash_first(M->m_nets);nl;nl = Hash_next(M->m_nets,nl)) {
    GNet *n = (GNet*) HashElem_obj(nl);
    n->n_mark = 0;
  }
}

static void VerilogSaveOneNet(FILE *f,GNet *n)
{
  if (n->n_ionet) {
    switch (n->n_ionet->typeinfo->Code) {
    case GC_SWITCH :
    case GC_DIP :
      fprintf(f,"reg ");
      break;
    default :
      fprintf(f,"%s ",n->n_ionet->typeinfo->vnames);
      break;
    }
  } else {
    fprintf(f,"%s ",GNet_getVType(n));
  }

  if (n->n_nbits > 1)
    fprintf(f,"[%d:0] ",n->n_nbits-1);

  if (n->n_signame)
    fprintf(f,"%s;    ",n->n_signame);
  else
    fprintf(f,"w%x;    ",(unsigned)n);

  fprintf(f,"//:");
  if (!n->n_show_name)
    fprintf(f," /sn:0");

  VerilogSaveWire(f,wire_sigroot(n->n_driver),0);
}

static void VerilogSaveNets(FILE *f,GModuleDef *M)
{
  HashElem *gl;
  HashElem *nl;

  /* Unmark all gates */
  for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
    GCElement *g = (GCElement*)HashElem_obj(gl);
    g->mark = 0;
  }

  /*
     Make sure all nets are finalized.
   */
#if 0
  for (wl = M->m_wires;wl;wl = wl->wl_next) {
    wire_finalizeNet(wl->wl_wire);
  }
#endif

  for (nl = Hash_first(M->m_nets);nl;nl = Hash_next(M->m_nets,nl)) {
    GNet *n = (GNet*) HashElem_obj(nl);

    if (n->n_ionet)
      VerilogSaveOneNet(f,n);
  }
  for (nl = Hash_first(M->m_nets);nl;nl = Hash_next(M->m_nets,nl)) {
    GNet *n = (GNet*) HashElem_obj(nl);

    if (!n->n_ionet)
      VerilogSaveOneNet(f,n);
  }
  fprintf(f,"//: enddecls\n\n");
}

/*
   Save the parameter list for a module.
*/
static void VerilogSaveModDecl(FILE *f,GModuleDef *M)
{
  HashElem *gl;
  int did_first = 0;

  if (GModuleDef_isTop(M)) {
    if (M == TkGate.circuit->root_mod)
      fprintf(f,";    //: root_module\n");
    else
      fprintf(f,";\n");
    return;
  }

  fprintf(f,"(");
  for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);
    switch (g->typeinfo->Code) {
    case GC_LOGICIN :
    case GC_LOGICOUT :
    case GC_LOGICTRI :
      if (did_first) fprintf(f,", ");
      did_first = 1;
      fprintf(f,"%s",g->wires[0]->net->n_signame); break;
      break;
    }
  }
  fprintf(f,");\n");
}

/*
 * Copy up to N characters from p up to the next end of string or newline.
 * reposition p after the newline.  Returns the number of characters copied
 * or -1 if p is at the end of the string.
 */
static int copyline(char *buf,int N,const char **p)
{
  int i;

  for (i = 0;i < N;i++, (*p)++) {
    if (*(*p) == 0) {
      buf[i] = 0;
      if (i == 0)
	return -1;
      else
	return i;
    }
    if (*(*p) == '\n') {
      buf[i] = 0;
      (*p)++;
      return i;
    }

    buf[i] = *(*p);
  }

  buf[N-1] = 0;
  return N;
}

/******************************************************************************
 *
 * Save a module symbol
 *
 * Parameter:
 *      f		File to which to write symbol
 *      ms		Module symbol to save
 *
 * Saves a module symbol description including normal and selected icons and
 * port positions.  Only symbols relevent to instances in the file being saved
 * are written.  The ID numbers for symbols are considered local to each
 * particular save file.  ID numbers may or may not be duplicated in different
 * files.
 *
 ******************************************************************************/
static void VerilogSaveModSymbol(FILE *f,GModSymbol *ms)
{
  char buf[STRMAX];
  char buf2[STRMAX];
  const char *data,*p;
  int x,y;
  int i;

  if (!ms) return;

  fprintf(f,"\n");
  fprintf(f,"//: /symbolBegin: %u\n",(unsigned)ms);

  /*
   * Generate the normal icon data
   */
  data = GModSymbol_getNormalIcon(ms,&x,&y);
  fprintf(f,"//: /iconBegin normal %d %d %d\n",
	  strlen(data),x,y);
  p = data;
  while (copyline(buf,STRMAX,&p) >= 0) {
    fprintf(f,"//: /data \"%s\"\n",quoteChars(buf2,buf,"\"\\"));
  }
  fprintf(f,"//: /iconEnd\n");

  /*
   * Generate the select icon data
   */
  data = GModSymbol_getSelectIcon(ms,&x,&y);
  fprintf(f,"//: /iconBegin select %d %d %d\n",
	  strlen(data),x,y);
  p = data;
  while (copyline(buf,STRMAX,&p) >= 0) {
    fprintf(f,"//: /data \"%s\"\n",quoteChars(buf2,buf,"\"\\"));
  }
  fprintf(f,"//: /iconEnd\n");

  /*
   * Generate the port data
   */
  for (i = 0;i < GModSymbol_numPorts(ms);i++) {
    GSymPort *msp = GModSymbol_getPort(ms,i);
    fprintf(f,"//: /port %s %s",
	    verIotypeStr(msp->msp_type),
	    msp->msp_name);
    if (msp->msp_size != 1)
      fprintf(f,"[%d:0]",msp->msp_size-1);

    fprintf(f," @(%d, %d) /r:%d\n",
	    msp->msp_x,
	    msp->msp_y,
	    msp->msp_orient);
  }
  fprintf(f,"//: /symbolEnd\n");
}

/*
   Save comment for block descriptor.
*/
static void VerilogSaveModInterface(FILE *f,GModuleDef *M)
{
  GCElement *g = M->m_interface;
  GGateInfo *gi;
  GWire *w;
  int i,j;
  static char dirchar[] = "?=><";

  if (!g) return;

  if (GCElement_getType(g) == GC_SYMBLOCK) {
    fprintf(f,"//: /symbol:%u\n",(unsigned)g->u.block.symbol);
  }

  gi = g->typeinfo;
  fprintf(f,"//: interface ");
  fprintf(f," /sz:(%d, %d) /bd:[",g->u.block.gwidth,g->u.block.gheight);
  for (i = 0;i < gi->NumPads;i++) {
    for (w = g->wires[i], j=0;w;w = w->next, j++) {
      fprintf(f," %s%d%c",
	      gi->Pad[i].Name,j,
	      dirchar[gi->Pad[i].iotype]);
      if (w->net->n_nbits == 1)
	fprintf(f,"%s",w->name);
      else
	fprintf(f,"%s[%d:0]",w->name,w->net->n_nbits-1);
      fprintf(f,"(%d/%d)",w->offset.num,w->offset.den);
    } 
  }
  fprintf(f," ]");
  fprintf(f," /pd: %d",M->m_protData);
  fprintf(f," /pi: %d",M->m_protIntf);
  fprintf(f," /pe: %d",M->m_protEdit);
  fprintf(f," /pp: %d",M->m_pptype);

  fprintf(f,"\n");
}

/*****************************************************************************
 *
 * Save symbols used in this circuit
 *
 * Parameters:
 *      f		File to write symbols to
 *      flags		Options
 *
 *****************************************************************************/
static void VerilogSaveSymbols(FILE *f,vsaveopts_t flags)
{
  HashElem *he;

  /*
   * Clear the mark on all symbols
   */
  for (he = GModSymbol_first();he;he = GModSymbol_next(he)) {
    GModSymbol *ms = (GModSymbol*) HashElem_obj(he);
    ms->ms_mark = 0;
  }


  for (he = Hash_first(TkGate.circuit->moduleTable);he;he = Hash_next(TkGate.circuit->moduleTable,he)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(he);
    HashElem *he2;

    if (M->m_isLib && !(flags & VSO_LIBRARY)) continue;

    if (M->m_interface && GCElement_getType(M->m_interface) == GC_SYMBLOCK) {
      GModSymbol *ms = GCElement_getSymbol(M->m_interface);
      ms->ms_mark = 1;
    }


    for (he2 = Hash_first(M->m_gates);he2;he2 = Hash_next(M->m_gates,he2)) {
      GCElement *g = (GCElement*) HashElem_obj(he2);
      if (GCElement_getType(g) == GC_SYMBLOCK) {
	GModSymbol *ms = GCElement_getSymbol(g);
	ms->ms_mark = 1;
      }
    }
  }


  /*
   * Save only marked symbols
   */
  for (he = GModSymbol_first();he;he = GModSymbol_next(he)) {
    GModSymbol *ms = (GModSymbol*) HashElem_obj(he);
    if (ms->ms_mark) {
      VerilogSaveModSymbol(f,ms);
    }
  }
}


/*
 * Save any flags this module might have
 */
static void VerilogSaveModFlags(FILE *f,GModuleDef *M)
{
  if (M->m_protData)
    fprintf(f,"//: property prot_data=%d\n",M->m_protData);
  if (M->m_protIntf)
    fprintf(f,"//: property prot_intf=%d\n",M->m_protIntf);
  if (M->m_pptype != PPT_SCALED)
    fprintf(f,"//: property pptype=%d\n",M->m_pptype);
}

static void VerilogSaveGates(FILE *f,GModuleDef *M)
{
  HashElem *gl;

  for (gl = Hash_first(M->m_gates);gl;gl = Hash_next(M->m_gates,gl)) {
    GCElement *g = (GCElement*) HashElem_obj(gl);
    assert(g->typeinfo->VerSave);
    (*g->typeinfo->VerSave)(f,g);
  }
  fprintf(f,"\n");
}

static void VerilogSaveHdlText(FILE *f,GModuleDef *M)
{
  Encoder *encoder = Circuit_getSaveFileEncoder(TkGate.circuit);

  if (M->m_text && strlen(M->m_text) > 0) {
    char *last = M->m_text + strlen(M->m_text) - 1;
    char *buf =  recodeTextP(encoder,M->m_text);

    fprintf(f,"%s",buf);
    if (*last != '\n')
      fprintf(f,"\n");

    free(buf);
  }
}

static void VerilogRegisterHdlCells(GModuleDef *M)
{
  HashElem *e;

  for (e = Hash_first(M->m_gates);e;e = Hash_next(M->m_gates,e)) {
    GCElement *g = (GCElement*) HashElem_obj(e);
    const char *prefix = TkGate.circuit->c_gatePrefix;

    if (strncmp(g->u.block.moduleName,prefix,strlen(prefix)) == 0)
      SHash_insert(cellsUsed,g->u.block.moduleName,0);
  }
}


void VerilogSaveModule(FILE *f,GModuleDef *M)
{
  ClearNetMarks(M);
  Concat_updateAutos(M,0);

  if (M->m_type == MT_NETLIST) {
    fprintf(f,"//: /netlistBegin %s\n",M->m_name);
    fprintf(f,"module %s",M->m_name);

    VerilogSaveModDecl(f,M);
    if (!GModuleDef_isTop(M))
      VerilogSaveModInterface(f,M);
    VerilogSaveModFlags(f,M);
    VerilogSaveNets(f,M);
    VerilogSaveGates(f,M);

    fprintf(f,"endmodule\n");
    fprintf(f,"//: /netlistEnd\n\n");
  } else {
    fprintf(f,"//: /hdlBegin %s\n",M->m_name);
    VerilogSaveModInterface(f,M);
    VerilogSaveModFlags(f,M);
    if (GModuleDef_isTop(M)) {
      fprintf(f,"//: root_module\n");
    }
    fprintf(f,"//: enddecls\n");
    VerilogRegisterHdlCells(M);
    VerilogSaveHdlText(f,M);
    fprintf(f,"//: /hdlEnd\n\n");

    fprintf(f,"\n");
    Timescale_save(&TkGate.circuit->c_timescale, f);
    fprintf(f,"\n");
  }
}

void VerilogSavePlainModule(FILE *f,GModuleDef *M)
{
  ClearNetMarks(M);
  Concat_updateAutos(M,0);


  if (M->m_type == MT_NETLIST) {
    fprintf(f,"module %s",M->m_name);

    VerilogSaveModDecl(f,M);
    VerilogSaveModFlags(f,M);
    VerilogSaveNets(f,M);
    VerilogSaveGates(f,M);

    fprintf(f,"endmodule\n");
  } else {
    VerilogSaveHdlText(f,M);
  }
}

/*****************************************************************************
 *
 * Do initialization required before writing verilog file
 *
 *****************************************************************************/
void VerilogWriteInit()
{
  if (!cellsUsed)
    cellsUsed = new_SHash_noob();
  else
    SHash_flush(cellsUsed);
}

/*****************************************************************************
 *  Raw save of file in verilog format.
 *****************************************************************************/
int VerilogWriteModules(const char *name,vsaveopts_t flags)
{
  FILE *f;
  HashElem *E;
  int i;
  int error_status;
  char buf[STRMAX];
  Circuit *c = TkGate.circuit;

  VerilogWriteInit();

  if (!(flags & VSO_NOHDLCHECK)) {
    if (hdl_checkSave(0) < 0)			/* Make sure hdl is up to date */
      return ECANCELED;
  }

  if (!(f = fopen(name,"w"))) {
    return ENOENT;
  }

  fprintf(f,"//: version \"%s\"\n",VERSIONS[0].vd_name);
  fprintf(f,"//: property encoding = \"%s\"\n",c->c_encoding);
  fprintf(f,"//: property locale = \"%s\"\n",c->c_locale->l_code);
  fprintf(f,"//: property prefix = \"%s\"\n",c->c_gatePrefix);

  if (c->title && *c->title)
    fprintf(f,"//: property title = \"%s\"\n",quoteChars(buf,c->title,"\"\\"));


  if (c->simClockMode)
    fprintf(f,"//: property simClockMode = 1\n");
  if (c->simClockName)
    fprintf(f,"//: property simClockName = \"%s\"\n",c->simClockName);

  if (!c->useExtBars)
    fprintf(f,"//: property useExtBars = 0\n");

  if (!c->showSwitchNets)
    fprintf(f,"//: property showSwitchNets = 0\n");

  if (c->simAutoStart)
    fprintf(f,"//: property simAutoStart = 1\n");

  if (c->discardChanges)
    fprintf(f,"//: property discardChanges = 1\n");

  if (strcmp(TkGate.defaultTech,TKGATE_DEFAULT_TECH) != 0)
    fprintf(f,"//: property technology = %s\n",TkGate.defaultTech);

  fprintf(f,"//: property timingViolationMode = %d\n",c->c_tvMode);
  fprintf(f,"//: property initTime = \"%g %s\"\n",c->c_startup,
	  Timescale_codeToUnits(c->c_startupUnits));

  if (c->numInitScripts > 0) {
    for (i = 0;i < c->numInitScripts;i++)
      fprintf(f,"//: script \"%s\"\n",quoteChars(buf,c->initScripts[i],"\"\\"));
    fprintf(f,"\n");
  }    

  for (E = Hash_first(TkGate.libraries);E;E = Hash_next(TkGate.libraries,E)) {
    const char *name = SHashElem_key(E);
    fprintf(f,"//: require \"%s\"\n",name);
  }

  fprintf(f,"\n");
  Timescale_save(&c->c_timescale, f);
  fprintf(f,"\n");

  VerilogSaveSymbols(f,flags);

  /*
   * Save the root module first, then save all other modules.
   */
  VerilogSaveModule(f,c->root_mod);
  for (E = Hash_first(c->moduleTable);E;E = Hash_next(c->moduleTable,E)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(E);
    if (((flags & VSO_LIBRARY) || !M->m_isLib) && M != c->root_mod)
      VerilogSaveModule(f,M);
  }

  /*
   * Generate built-in definitions for basic gates used.
   */
  if (flags & VSO_BUILTINS) {
    fprintf(f,"\n");
    Timescale_save(&c->c_timescale, f);
    fprintf(f,"\n");

    for (E = Hash_first(cellsUsed);E;E = Hash_next(cellsUsed,E)) {
      GCellSpec gcs;
      const char *cellName = (const char*) SHashElem_key(E);
      if (GGateInfo_decodeTypeName(cellName,&gcs)) {
	fprintf(f,"\n");
	(*gcs.gc_info->WriteCellDef)(f,&gcs);
      }
    }
  }
  SHash_flush(cellsUsed);


  error_status = ferror(f);
  fclose(f);

  return error_status;
}

/*****************************************************************************
 * Copy a file and return 0 on success or non-zero on failure.
 *****************************************************************************/
static int copy_file(const char *src,const char *dst)
{
  FILE *inf = 0,*outf = 0;
  int r = 0;
  int c;

  inf = fopen(src,"r");
  if (inf)
    outf = fopen(dst,"w");

  if (inf && outf) {
    while ((c = fgetc(inf)) != EOF) {
      if (fputc(c,outf) == EOF) break;
    }

    if (ferror(inf)) r = -1;
    if (ferror(outf)) r = -1;
  } else
    r = -1;

  if (inf)  fclose(inf);
  if (outf) fclose(outf);
  return r;
}

/*****************************************************************************
 *
 * Check a file to see if it is OK by invoking tkgate in verify mode on the
 * file.
 *
 *****************************************************************************/
static int check_file(const char *name)
{
  char cmd[STRMAX];
  int pid = 0,status = 0;

#if 0
  /* Mess with the file to simulate corruption. */
  sprintf(cmd,"xemacs %s",name);
  system(cmd);
#endif

  sprintf(cmd,"%s/libexec/tkgate",TkGate.homedir);
  if (!(pid = fork())) {
    execl(cmd,cmd,"-Vq",name,0);
    exit(1);
  }

  waitpid(pid,&status,0);

  return status;
}


/*****************************************************************************
 *
 * Save a file without confirmation check.
 *
 *****************************************************************************/
int VerilogQuickSave(const char *name,vsaveopts_t saveFlags)
{
  int r;

  r = VerilogWriteModules(name,saveFlags);
  if (r == ECANCELED) return ECANCELED;

  if (r != 0) {
    message(1,msgLookup("err.badsave"),name);
    return r;
  }
  if (check_file(name))
    message(1,msgLookup("err.corruptnewsave"),name);
  else {
    message(0,msgLookup("msg.save"),name);
    ClearModified();
  }
  return 0;
}

/*****************************************************************************
 *
 * Make sure we have permission to write the file.
 *
 *****************************************************************************/
int isWritable(const char *name)
{
  char dirName[STRMAX],*p;
  struct stat sb;
  int gid = getgid();
  int uid = getuid();

  /*
   * If the file exists, look at its permissions
   */
  if (stat(name, &sb) == 0) {
    unsigned mode = sb.st_mode & 7;
    
    if (uid == sb.st_uid)
      mode |= (sb.st_mode >> 6) & 7;
    if (gid == sb.st_gid)
      mode |= (sb.st_mode >> 3) & 7;

    if ((mode & 2))
      return 1;
    else
      return 0;
  }

  /*
   * If the file does not exist, check its directory
   */
  if (strlen(name) > STRMAX) return 0;
  strcpy(dirName, name);

  p = strrchr(dirName,'/');
  if (!p)
    strcpy(dirName,".");
  else {
    *p = 0;
    while (p > dirName && p[-1] == '/')
      *--p = 0;
    if (p == dirName)
      strcpy(dirName,"/");
  }


  if (stat(dirName, &sb) == 0) {
    unsigned mode = sb.st_mode & 7;
    
    if (uid == sb.st_uid)
      mode |= (sb.st_mode >> 6) & 7;
    if (gid == sb.st_gid)
      mode |= (sb.st_mode >> 3) & 7;

    if ((mode & 2))
      return 1;
    else
      return 0;
  }

  return 0;
}

/*****************************************************************************
 *
 * Safely save a file in verilog format.
 *
 *****************************************************************************/
int VerilogSave(const char *name)
{
  char tempName1[STRMAX],tempName2[STRMAX];
  extern int doBackupOnSave;
  extern int doSafeSave;
  int file_exists = 0;
  vsaveopts_t saveFlags = 0;
  FILE *f;


  if (!*name) return -1;

  if (!isWritable(name)) {
    message(1,msgLookup("err.notwritable"),name);
    return 0;
  }

  if (TkGate.tcl) {
    const char *r;
    r = Tcl_GetVar(TkGate.tcl,"tkg_saveWithCells",TCL_GLOBAL_ONLY);
    if (r && *r == '1')
      saveFlags |= VSO_BUILTINS;
  }


  /*
   * Check to see if we are overwriting an existing file.
   */
  file_exists = ((f = fopen(name,"r")) != 0);
  if (f) fclose(f);


  /*
   * If we are overwriting an existing file and safe saving is enabled,
   * we will first write the circuit to a temporary file, check the file
   * to see if it was saved correctly, and only then move it to the
   * target save file.
   *
   */
  if (file_exists) {
    /*
     * If this is the first time we have saved and the file exists, make a backup. 
     */
    if (doBackupOnSave) {
      char buf[STRMAX];
      char *p;

      strcpy(buf,name);
      p = buf+strlen(buf);
      if (p[-1] != '~') {	/* Don't create backup when editing a backup file */
	strcpy(p,"~");
	if (copy_file(name,buf) != 0)
	  message(1,msgLookup("err.backupfail"),buf);
      }
      doBackupOnSave = 0;
    }

    if (doSafeSave) {
      int r1 = 0,r2 = 0;

      sprintf(tempName1,"%s#save-%d#",name,getpid());
      sprintf(tempName2,"%s#backup-%d#",name,getpid());

      r1 = copy_file(name,tempName2);
      if (r1 == 0) {
	r2 = VerilogWriteModules(tempName1,saveFlags);
	if (r2 == ECANCELED) return ECANCELED;
      }

      if (r1 != 0 || r2 != 0) {
	unlink(tempName1);
	unlink(tempName2);
	DoTcl("yesno [format [m err.nosafesave] %s]",name);
	if (strcmp(TkGate.tcl->result,"yes") == 0)
	  VerilogQuickSave(name,saveFlags);
	return 0;
      }

      if (check_file(tempName1)) {
	unlink(tempName2);
	message(1,msgLookup("err.corruptsave"),name,tempName1);
	return 0;
      }

      if (rename(tempName1,name) != 0) {
	message(1,msgLookup("err.badrename"),tempName1,name);
	return 0;
      }

      unlink(tempName2);
      message(0,msgLookup("msg.save"),name);
      ClearModified();
    } else
      VerilogQuickSave(name,saveFlags);
    return 0;
  } else {
    VerilogQuickSave(name,saveFlags);
    return 0;
  }
}

void GCellSpec_writeBeginModule(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int N = gi->NumPads;
  int multiPad = gcs->gc_multiPad;
  int i,j;
  int isfirst = 1;

  fprintf(f,"//: /builtinBegin\n");
  fprintf(f,"module %s",gcs->gc_name);

  /*
   * Generate delay port declaration list
   */
  if (gi->delayNames[0]) {
    fprintf(f," #(");
    for (i = 0;gi->delayNames[i];i++) {
      if (i > 0) fprintf(f,", ");
      fprintf(f,".%s(1)",gi->delayNames[i]);
    }
    fprintf(f,")");
  }

  /*
   * Generate port declaration list
   */
  fprintf(f," (");
  for (i = 0;i < N;i++) {
    if (!isfirst) fprintf(f,", ");
    isfirst = 0;

    if (gi->Pad[i].CanAdd) {
      for (j = 0;j < multiPad;j++) {
	if (j > 0) fprintf(f,", ");
	fprintf(f,"%s%d",gi->Pad[i].Name,j);
      }
    } else
      fprintf(f,"%s",gi->Pad[i].Name);
  }
  fprintf(f,");\n");
}

void GCellSpec_writePortDecls(FILE *f,GCellSpec *gcs)
{
  GGateInfo *gi = gcs->gc_info;
  int N = gi->NumPads;
  int multiPad = gcs->gc_multiPad;
  int numBits = gcs->gc_numBits;
  int i,j;
  char bitSpec[128];

  if (numBits > 1)
    sprintf(bitSpec," [%d:0]",numBits-1);
  else
    *bitSpec = 0;

  for (i = 0;i < N;i++) {
    const char *iotype = verIotypeStr(gi->Pad[i].iotype);

    if (gi->Pad[i].CanAdd) {
      for (j = 0;j < multiPad;j++) {
	fprintf(f,"%s%s %s%d;\n",iotype,bitSpec,gi->Pad[i].Name,j);
      }
    } else
      fprintf(f,"%s%s %s;\n",iotype,bitSpec,gi->Pad[i].Name);
  }
}

void GCellSpec_writeEndModule(FILE *f,GCellSpec *gcs)
{
  fprintf(f,"\n");
  fprintf(f,"endmodule\n");
  fprintf(f,"//: /builtinEnd\n\n");
}
