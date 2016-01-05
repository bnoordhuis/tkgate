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
****************************************************************************/
#include "thyme.h"

/*****************************************************************************
 *
 * Create a new declaration scope object
 *
 * Parameter:
 *      s		Scope object to be initialized
 *      parent		Parent scope
 *
 *****************************************************************************/
void ScopeDecl_init(ScopeDecl *s,ScopeDecl *parent)
{
  s->sd_parent = parent;
  SHash_init(&s->sd_nets);
}

/*****************************************************************************
 *
 * Uninitialize a declaration scope object
 *
 * Parameter:
 *      s		Scope object to be uninitialized
 *
 *****************************************************************************/
void ScopeDecl_uninit(ScopeDecl *s)
{
  SHash_uninit(&s->sd_nets);
}


Scope *new_Scope(const char *path,Scope *parent,ModuleInst *mi)
{
  Scope *s = (Scope*) malloc(sizeof(Scope));

  Scope_init(s,path,parent,mi);

  return s;
}

void delete_Scope(Scope *s)
{
  Scope_uninit(s);
  free(s);
}

void Scope_init(Scope *s,const char *path,Scope *parent,ModuleInst *mi)
{
  s->s_path = path ? strdup(path) : 0;
  s->s_parent = parent;
  s->s_instance = mi;
  s->s_peer = 0;
  SHash_init(&s->s_nets);
  SHash_init(&s->s_tasks);
}

void Scope_uninit(Scope *s)
{
  if (s->s_path) free(s->s_path);
  SHash_uninit(&s->s_nets);
  SHash_uninit(&s->s_tasks);
}

void Scope_setPeer(Scope *s,Scope *peer)
{
  s->s_peer = peer;
}

Net *Scope_findNet(Scope *s,const char *name,unsigned flags)
{
  Circuit *c = &vgsim.vg_circuit;		/* Top-level circuit */
  Net *n = 0;

  /*
   * If scope is null, look only for fully qualified names.
   */
  if (!s)
    return Circuit_findNet(c,name);

  /*
   * Look for 'name' in local name table.
   */
  n = (Net*) SHash_find(&s->s_nets,name);
  if (n) return n;

  /*
   * Look for 'name' in parent scope
   */
  if (s->s_parent && !(flags & SDF_LOCAL_ONLY))
    n = Scope_findNet(s->s_parent,name,flags);
  if (n) return n;

  /*
   * Look for 'name' in peer module.
   */
  if (s->s_peer)
    return  Scope_findNet(s->s_peer,name,flags);

  /*
   * Look for 'name' as a fully qualified net name.
   */
  n = Circuit_findNet(c,name);
  if (n) return n;

  if (strchr(name,'.')) {
    char *tempName = (char*)malloc(strlen(s->s_path)+strlen(name)+2);
    sprintf(tempName,"%s.%s",s->s_path,name);
    n = Circuit_findNet(c,tempName);
    free(tempName);
  }

  return n;
}

void Scope_defNet(Scope *s,const char *name,Net *n)
{
  Circuit *c = &vgsim.vg_circuit;		/* Top-level circuit */

  SHash_insert(&s->s_nets,name,n);
  SHash_insert(&c->c_nets,n->n_name,n);
}

void Scope_replaceLocalNet(Scope *s,const char *name,Net *n)
{
  SHash_replace(&s->s_nets,name,n);
}


UserTask *Scope_findTask(Scope *s,const char *name)
{
  return (UserTask*)SHash_find(&s->s_tasks,name);
}

int Scope_defineTask(Scope *s,const char *name,UserTask *ut)
{
  if (Scope_findTask(s,name)) return -1;

  SHash_insert(&s->s_tasks,name,ut);

  return 0;
}

Value *Scope_findParm(Scope *scope,const char *name)
{
  Net *n = (Net*) Scope_findNet(scope,name,SDF_LOCAL_ONLY);
  if (!n) return 0;
  if (!(Net_getType(n) & NT_P_PARAMETER)) return 0;

  return Net_getValue(n);
}

/*****************************************************************************
 *
 * Find a net in a declaration scope object
 *
 * Parameter:
 *      s		Scope object to be uninitialized
 *      name		Name of net to look for
 *      flags		Flags modifying net lookup
 *
 *****************************************************************************/
NetDecl *ScopeDecl_findNet(ScopeDecl *s,const char *name,unsigned flags)
{
  NetDecl *n;

  n = (NetDecl*) SHash_find(&s->sd_nets,name);
  if (!n && s->sd_parent && !(flags & SDF_LOCAL_ONLY))
    n = ScopeDecl_findNet(s->sd_parent,name,flags); 

  return n;
}

/*****************************************************************************
 *
 * Define a new net in a declaration scope
 *
 * Parameter:
 *      s		Scope object to be uninitialized
 *      n		Net declaration object to add
 *
 *****************************************************************************/
void ScopeDecl_defNet(ScopeDecl *s,NetDecl*n)
{
  SHash_insert(&s->sd_nets,n->n_name,n);
}

/*****************************************************************************
 *
 * Create a new module declaration object
 *
 * Parameter:
 *      name		Name of the module
 *
 * Returns:		Newly create module object.
 *
 *****************************************************************************/
ModuleDecl *new_ModuleDecl(const char *name)
{
  ModuleDecl *m = (ModuleDecl*) malloc(sizeof(ModuleDecl));

  m->m_name = name ? strdup(name) : 0;
  m->m_errorsDone = 0;
  m->m_specify = 0;
  List_init(&m->m_ports);
  List_init(&m->m_parmPorts);
  List_init(&m->m_items);
  ScopeDecl_init(&m->m_scope,0);
  SHash_init(&m->m_tasks);
  m->m_faninnodes = 0;

  m->m_timescale = currentTimescale;

  return m;
}

/*****************************************************************************
 *
 * Delete a module declaration object and reclaim all memory.
 *
 * Parameter:
 *      m		Module declaration to be destroyed.
 *
 *****************************************************************************/
void delete_ModuleDecl(ModuleDecl *m)
{
  free(m->m_name);
  List_uninit(&m->m_ports);
  List_uninit(&m->m_parmPorts);
  ScopeDecl_uninit(&m->m_scope);
  SHash_uninit(&m->m_tasks);
  if (m->m_specify) delete_Specify(m->m_specify);
  free(m);
}

/*****************************************************************************
 *
 * Add a port to a module declaration
 *
 * Parameter:
 *      m		Module declaration to which to add port
 *      name		Name of port to add
 *
 *****************************************************************************/
void ModuleDecl_addPort(ModuleDecl *m,const char *name)
{
  List_addToTail(&m->m_ports,strdup(name));
}

/*****************************************************************************
 *
 * Define a 'parameter' in a module
 *
 * Parameter:
 *      m		Module in which to define parameter
 *      name		Name of the parameter
 *      e		Expression for the parameter
 *      isPort		Non-zero if this is a parameter port.
 *
 *****************************************************************************/
void ModuleDecl_defineParm(ModuleDecl *m,const char *name,Expr *e, int isPort)
{
  MIParameter *mip = new_MIParameter(name,e);

  List_addToTail(&m->m_items, mip);

  if (isPort) {
    mip->mip_ppPos = List_numElems(&m->m_parmPorts);
    List_addToTail(&m->m_parmPorts,strdup(name));
  }
}

int ModuleDecl_isParm(ModuleDecl *m,const char *name)
{
  ListElem *le;

  for (le = List_first(&m->m_parmPorts);le;le = List_next(&m->m_parmPorts,le)) {
    const char *portName = (const char*)ListElem_obj(le);
    if (strcmp(name,portName) == 0) return 1;
  }
  return 0;
}

/*****************************************************************************
 *
 * Print basic information about a module definition.
 *
 * Parameters:
 *     m		Module to display.
 *
 *****************************************************************************/
void ModuleDecl_printData(ModuleDecl *m)
{
  ListElem *le;

  printf("module %s\n",ModuleDecl_getName(m));
  printf("  line %d\n",m->m_place.p_lineNo);

  for (le = List_first(&m->m_items);le;le = List_next(&m->m_items,le)) {
    ModuleItem *mi = (ModuleItem*) ListElem_obj(le);

    switch (mi->mi_type) {
    case IC_PARAMETER :
      {
	MIParameter *mip = (MIParameter*) mi;
	ListElem *le2;
	for (le2 = List_first(&m->m_parmPorts);le2;le2 = List_next(&m->m_parmPorts,le2)) {
	  char *parmPort = (char*) ListElem_obj(le2);
	  if (strcmp(parmPort,mip->mip_name) == 0) break;
	}
	/* If parameter is parmport ... */
	if (le2) {
	  Value *v = Expr_parmEval(mip->mip_expr,0,0);
	  if (v) {
	    char buf[STRMAX];
	    Value_getstr(v,buf);
	    printf("  parmport %s %s\n",mip->mip_name,buf);
	  } else
	    printf("  parmport %s 0\n",mip->mip_name);
	}

	      
      }
      break;
    case IC_NETDECL :
      {
	NetDecl *nd = mi->mi_netdec.mid_decl;
	int rangeok = 1;
	char typeName[STRMAX],rangeExpr[STRMAX];
	VRange *range;

	if ((NetDecl_getType(nd) & NT_P_IO_MASK)) {
	  NT_getStr((NetDecl_getType(nd) & NT_P_IO_MASK)|NT_P_WIRE, typeName);
	} else if ((NetDecl_getType(nd) & NT_P_WIRE)) {
	  strcpy(typeName,"wire");
	} else if ((NetDecl_getType(nd) & NT_P_REG)) {
	  strcpy(typeName,"reg");
	} else
	  break;	/* ignore this type */

	range = NetDecl_getRange(nd);
	if (VRange_getDirect(range,rangeExpr) < 0)
	  rangeok = 0;

	printf("  %s %s %s\n",typeName, NetDecl_getName(nd),rangeExpr);
	if (!rangeok) {
	    errorFile(ModuleItem_getPlace(mi),ERR_BADPRTRANGE,NetDecl_getName(nd));
	}
      }
      break;
    case IC_INSTANCE :
      printf("  instance %s %s\n",mi->mi_inst.mii_name, mi->mi_inst.mii_instName);
      break;
    default :
      break;
    }
  }

  printf("endmodule %s\n",ModuleDecl_getName(m));
}

/*****************************************************************************
 *
 * Print a module declaration (for debugging purposes)
 *
 * Parameters:
 *      m		Module declaration to print
 *      f		File in which to print declaration
 *
 *****************************************************************************/
void ModuleDecl_print(ModuleDecl *m,FILE *f)
{
  fprintf(f,"module %s",m->m_name);

  /*
   * Display the parameter ports
   */
  if (List_numElems(&m->m_parmPorts) > 0) {
    ListElem *E;
    int notFirst = 0;

    fprintf(f," #(");
    for (E = List_first(&m->m_parmPorts);E;E = List_next(&m->m_parmPorts,E)) {
      const char *name = (const char*) ListElem_obj(E);
      if (notFirst) fprintf(f,", ");
      notFirst = 1;
      fprintf(f,"%s",name);
    }
    fprintf(f,")");
  }

  /*
   * Display the ports 
   */
  if (List_numElems(&m->m_ports) > 0) {
    ListElem *E;
    int notFirst = 0;

    fprintf(f," (");
    for (E = List_first(&m->m_ports);E;E = List_next(&m->m_ports,E)) {
      const char *name = (const char*) ListElem_obj(E);
      if (notFirst) fprintf(f,", ");
      notFirst = 1;
      fprintf(f,"%s",name);
    }
    fprintf(f,")");
  }
  fprintf(f,";\n");


  /*
   * Display the items.
   */
  if (List_numElems(&m->m_items) > 0) {
    ListElem *E;

    for (E = List_first(&m->m_items);E;E = List_next(&m->m_items,E)) {
      ModuleItem *mi = (ModuleItem*) ListElem_obj(E);

      if (mi->mi_type != IC_NETDECL && mi->mi_type != IC_PARAMETER)
	fprintf(f,"\n");

      ModuleItem_print(mi,f);
    }
  }

  fprintf(f,"\n");
  fprintf(f,"endmodule\n");
}

NetDecl *ModuleDecl_findNet(ModuleDecl *m,const char *name)
{
  return ScopeDecl_findNet(&m->m_scope,name,0);
}

/*****************************************************************************
 *
 * Add a net declaration to a module.
 *
 * Parameters:
 *     m		Module declaration object
 *     n		Net declaration object
 *
 *****************************************************************************/
void ModuleDecl_defNet(ModuleDecl *m,NetDecl*n)
{
  ScopeDecl_defNet(&m->m_scope,n);
  List_addToTail(&m->m_items, new_MINetDecl(n));
}

/*****************************************************************************
 *
 * Create the specify data object for a module if not already created.
 *
 * Parameters:
 *     m		Module declaration object
 *
 *****************************************************************************/
void ModuleDecl_makeSpecify(ModuleDecl *m)
{
  if (!m->m_specify)
    m->m_specify = new_Specify(m);
}

int ModuleDecl_numPorts(ModuleDecl *m,nettype_t ptype)
{
  ListElem *le;
  int n = 0;

  ptype &= NT_P_IO_MASK;

  for (le = List_first(&m->m_ports);le;le = List_next(&m->m_ports,le)) {
    char *name = (char*)ListElem_obj(le);
    NetDecl *nd = (NetDecl*)ModuleDecl_findNet(m,name);
    if (nd && ((NetDecl_getType(nd) & NT_P_IO_MASK) == ptype))
      n++;
  }

  return n;
}

UserTaskDecl *ModuleDecl_findTask(ModuleDecl *m,const char *name)
{
  return (UserTaskDecl*)SHash_find(&m->m_tasks,name);
}

int ModuleDecl_defineTask(ModuleDecl *m,const char *name,UserTaskDecl *utd)
{
  if (ModuleDecl_findTask(m,name)) return -1;

  SHash_insert(&m->m_tasks,name,utd);

  return 0;
}

/*****************************************************************************
 *
 * Make an array of strings from the keys of the elements in a hash tabe
 *
 *****************************************************************************/
static char **getHashKeys(SHash *H)
{
  char **keys = malloc(sizeof(char*)*(Hash_numElems(H)+1));
  HashElem *he;
  int i = 0;

  for (he = Hash_first(H);he;he = Hash_next(H,he)) {
    const char *name = SHashElem_key(he);
    keys[i++] = strdup(name);
  }
  keys[i] = 0;

  return keys;
}

#if 0
static void printHList(SHash *H)
{
  HashElem *he;
  int is_first = 1;

  for (he = Hash_first(H);he;he = Hash_next(H,he)) {
    const char *name = SHashElem_key(he);
    if (!is_first) printf(", ");
    printf("%s",name);
    is_first = 0;
  }
}
#endif

/*****************************************************************************
 *
 * Sort the itemlist in a module to be in topological order (used for path-delay modules)
 *
 * Parameters:
 *      m		Module to be sorted
 *
 *****************************************************************************/
int ModuleDecl_makeFaninTree(ModuleDecl *m)
{
  ListElem *le;
  int i;

#if 0
  printf("ModuleDecl_makeFaninTree(%s)\n",m->m_name);
#endif
  m->m_faninnodes = new_SHash();


  for (le = List_first(&m->m_items);le; le = List_next(&m->m_items,le)) {
    ModuleItem *item = (ModuleItem*) ListElem_obj(le);

    switch (item->mi_type) {
    default :
#if 0
      printf("   mitem\n");
#endif
      break;
    case IC_ASSIGN :
      {
	MIAssign *a = (MIAssign*) item;
	FaninNode *fn = new_FaninNode(item);
	SHash outH,inH;

	SHash_init(&outH);
	SHash_init(&inH);

	Expr_getStaticReaders(a->mia_lhs, &outH);
	Expr_getStaticReaders(a->mia_rhs, &inH);

	fn->fn_out = getHashKeys(&outH);
	fn->fn_in = getHashKeys(&inH);

	for (i = 0;fn->fn_out[i];i++) {
	  if (!SHash_find(m->m_faninnodes,fn->fn_out[i]))
	    SHash_insert(m->m_faninnodes,fn->fn_out[i],fn);
	  else
	    errorModule(m, ModuleItem_getPlace(fn->fn_item),ERR_PATHDREASSGN,fn->fn_out[i]);
	}

#if 0
	printf("   assign ");
	printHList(&outH);
	printf(" = ");
	printHList(&inH);
	printf("\n");
#endif
	

	SHash_uninit(&inH);
	SHash_uninit(&outH);
      }
      break;
    case IC_GATE :
      {
	MIGate *g = (MIGate*) item;
	FaninNode *fn = new_FaninNode(item);
	SHash outH,inH;
	ListElem *le;
	int i;
	int N = List_numElems(g->mig_ports);

	SHash_init(&outH);
	SHash_init(&inH);

	for (i = 1, le = List_first(g->mig_ports);le;i++, le = List_next(g->mig_ports,le)) {
	  Expr *port = (Expr*)ListElem_obj(le);

	  if ((i == 1) || (g->mig_desc->gd_gac == GAC_MULTOUT && i != N))
	    Expr_getStaticReaders(port, &outH);
	  else
	    Expr_getStaticReaders(port, &inH);
	}

	fn->fn_out = getHashKeys(&outH);
	fn->fn_in = getHashKeys(&inH);

	for (i = 0;fn->fn_out[i];i++) {
	  if (!SHash_find(m->m_faninnodes,fn->fn_out[i]))
	    SHash_insert(m->m_faninnodes,fn->fn_out[i],fn);
	  else
	    errorModule(m, ModuleItem_getPlace(fn->fn_item),ERR_PATHDREASSGN,fn->fn_out[i]);
	}

#if 0
	printf("   gate ");
	printHList(&outH);
	printf(" = ");
	printHList(&inH);
	printf("\n");
#endif
	SHash_uninit(&inH);
	SHash_uninit(&outH);
      }
      break;
    }
  }

  return 0;
}

void ModuleDecl_clearFaninFlags(ModuleDecl *m)
{
  HashElem *he;
  int i;

  for (he = Hash_first(m->m_faninnodes);he;he = Hash_next(m->m_faninnodes,he)) {
    FaninNode *fn = (FaninNode *)HashElem_obj(he);
    fn->fn_flag = 0;
    if (fn->fn_outNets) {
      for (i = 0;fn->fn_out[i];i++)
	fn->fn_outNets[i] = 0;
    }
    if (fn->fn_inNets) {
      for (i = 0;fn->fn_in[i];i++)
	fn->fn_inNets[i] = 0;
    }
  }
}

UserTask *ModuleInst_findTask(ModuleInst *mi,const char *name)
{
  return Scope_findTask(ModuleInst_getScope(mi),name);
}

int ModuleInst_defineTask(ModuleInst *mi,const char *name,UserTask *ut)
{
  return Scope_defineTask(ModuleInst_getScope(mi),name,ut);
}


/*****************************************************************************
 *
 * Create a new net declaration object
 *
 * Parameters:
 *     name		Name of the net
 *     wtype		Type of the net WIRE, REG, etc.
 *     range		Bit-range of the net (uninstantiated)
 *     addrRange	Address range if this is a memory (or null otherwise)
 *     place		Place where net is declared
 *
 *****************************************************************************/
NetDecl *new_NetDecl(const char *name,int wtype,VRange *range,VRange *addrRange,Place *place)
{
  NetDecl *n = (NetDecl *) malloc(sizeof(NetDecl));

  n->n_name = strdup(name);
  n->n_type = wtype;
  n->n_range = range;
  n->n_addrRange = addrRange;
  if (place)
    Place_copy(&n->n_place, place);
  else
    Place_init(&n->n_place, "");

  return n;
}

ModuleInst *new_ModuleInst(ModuleDecl *md,Circuit *c,ModuleInst *parent,const char *path)
{
  ModuleInst *mc = (ModuleInst*) malloc(sizeof(ModuleInst));

  ModuleInst_init(mc,md,c,parent,path);

  return mc;
}

void delete_ModuleInst(ModuleInst *mc)
{
  ModuleInst_uninit(mc);
  free(mc);
}

void ModuleInst_init(ModuleInst *mi,ModuleDecl *md,Circuit *c,ModuleInst *parent,const char *path)
{
  mi->mc_path = strdup(path);
  mi->mc_peer = 0;
  mi->mc_mod  = md;
  mi->mc_circuit = c;
  mi->mc_parent = parent;
  List_init(&mi->mc_threads);
  Scope_init(&mi->mc_scope,path,0,mi);
}

void ModuleInst_uninit(ModuleInst *mc)
{
  free(mc->mc_path);
  Scope_uninit(&mc->mc_scope);
}

Value *ModuleInst_findParm(ModuleInst *mc,const char *name)
{
  return Scope_findParm(&mc->mc_scope,name);
}


void ModuleInst_defParm(ModuleInst *mc,const char *name,Value *value)
{
  char fullName[2*STRMAX];
  Net *n;

  sprintf(fullName,"%s.%s",mc->mc_path,name);
  n = new_Net(fullName,NT_PARAMETER,Value_nbits(value)-1,0);
  Value_copy(Net_getValue(n),value);

  Scope_defNet(ModuleInst_getScope(mc),name,n);
}


/*****************************************************************************
 *
 * Find a named net in the context of a module instance
 *
 * Parameters:
 *     mc		Current module context
 *     name		Name of net
 *
 * The following rules are used to lookup the net name:
 *
 *     1) Look for 'name' in the local net table.
 *     2) If the module has a peer, look in the peer module.  For modules with
 *        peers, this is the final search step.
 *     3) Look for 'name' in the circuit net table for fully qualified names
 *        such as 'top.foo.n'.
 *     4) Treat 'name' as a relative name such as 'foo.n'.  This is done by
 *        prepending the path for the current module and searching again as
 *        a fully qualified path.
 *
 *****************************************************************************/
Net *ModuleInst_findNet(ModuleInst *mc,const char *name)
{
  return Scope_findNet(ModuleInst_getScope(mc), name, 0);
}

/*****************************************************************************
 *
 * Define a net
 *
 * Parameters:
 *     mc		Current module context
 *     name		Local name of net
 *     n		Net object with full path name
 *
 *****************************************************************************/
void ModuleInst_defNet(ModuleInst *mc,const char *name,Net *n)
{
  Scope_defNet(ModuleInst_getScope(mc), name, n);
}

/*****************************************************************************
 *
 * Lookup the local name of a net
 *
 * Parameters:
 *     mi		Current module context
 *     n		Net object
 *
 *****************************************************************************/
const char *ModuleInst_findLocalNetName(ModuleInst *mi,Net *n)
{
  HashElem *he;
  SHash *nets = &mi->mc_scope.s_nets;

  for (he = Hash_first(nets);he;he = Hash_next(nets,he)) {
    Net *mi_n = HashElem_obj(he);
    if (mi_n == n)
      return SHashElem_key(he);
  }
  return 0;
}

FaninNode *new_FaninNode(ModuleItem *item)
{
  FaninNode *fn = (FaninNode*) malloc(sizeof(FaninNode));

  fn->fn_item = item;
  fn->fn_out = 0;
  fn->fn_outNets = 0;
  fn->fn_in = 0;
  fn->fn_inNets = 0;
  fn->fn_flag = 0;

  return fn;
}

