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

    Last edit by hansen on Sun Feb  1 15:02:21 2009
****************************************************************************/
#include <ctype.h>
#include "thyme.h"

#define DEBUG 0

static ModuleInst *Circuit_buildNets(Circuit *c,ModuleDecl *m,MIInstance *mi,ModuleInst *parent,char *path);
static void Circuit_buildHier(Circuit *c,ModuleInst *mi,ModuleInst *parent,char *path);

/*****************************************************************************
 *
 * Initialize the circuit data structure
 *
 * Parameters:
 *      c		Memory to initialize with circuit data structure.
 *
 *****************************************************************************/
void Circuit_init(Circuit *c)
{
  SHash_init(&c->c_nets);
  NHash_init(&c->c_triggers);
  SHash_init(&c->c_channels);
  SHash_init(&c->c_moduleInsts);
  SHash_init(&c->c_dynamicModules);
  c->c_evQueue = new_EvQueue(c);
  c->c_root = 0;
}

/*****************************************************************************
 *
 * Get the scope for the next higher context, or null if this is a top-level
 * scope.
 *
 *****************************************************************************/
Scope *Circuit_getUpScope(Circuit *c,Scope *s)
{
  char path[STRMAX],*p;
  ModuleInst *mi;

  strcpy(path,s->s_path);
  if (!(p = strrchr(path,'.')) || p == path)
    return 0;

  *p = 0;

  mi = (ModuleInst*) SHash_find(&c->c_moduleInsts,path);
  if (!mi) return 0;
  
  return &mi->mc_scope;
}


/*****************************************************************************
 *
 * Do a register variable initialization.
 *
 *****************************************************************************/
static void Circuit_buildNets_reginit(Circuit *c,ModuleDecl *m,
					 ModuleInst *mi,MIAssign *mia,
					MIInstance *mid)
{
  Value *s;
  Scope *scope;
  const char *name;
  Net *reg;

  if (Expr_type(mia->mia_lhs) != E_LITERAL) {
    errorModule(mi->mc_mod,ModuleItem_getPlace(mia),ERR_IE_BADEXP,"'non-literal initialization'");
    return;
  }

  name = Expr_getLitName(mia->mia_lhs);

  scope = ModuleInst_getScope(mi);
  s = Expr_parmEval(mia->mia_rhs,scope,0);
  if (!s) return;

  reg = Scope_findNet(scope, name, SDF_LOCAL_ONLY);
  if (!reg) {
    errorModule(mi->mc_mod,ModuleItem_getPlace(mia),ERR_IE_BADVAR);
    return;
  }

  Net_set(reg, s);
}

/*****************************************************************************
 *
 * Declare a module parameter
 *
 * Paramaters:
 *     c		Circuit we are building.
 *     m		Module declaration.
 *     mi		Current module context.
 *     mip		Module parameter declaration.
 *     mid		Module instance declaration
 *
 *****************************************************************************/
static void Circuit_buildNets_parameter(Circuit *c,ModuleDecl *m,
					 ModuleInst *mi,MIParameter *mip,
					MIInstance *mid)
{
  Value *s;
  Expr *e;
  Scope *scope;

  e = mid ? MIInstance_findParm(mid,mip->mip_name, mip->mip_ppPos) : 0;
  if (e && mip->mip_ppPos == -1) {
    errorModule(mi->mc_mod,ModuleItem_getPlace(mip),ERR_NOTPPORT,mip->mip_name);
    e = 0;
  }

  if (!e) e = mip->mip_expr;

  scope = ModuleInst_getScope(mi);
  if (mip->mip_ppPos >= 0)
    scope = Circuit_getUpScope(c,scope);
  s = Expr_parmEval(e,scope,0);

#if 0
  {
    printf("parameter eval: "); 
    Expr_print(e,stdout);
    printf(" = ");
    if (s)
      Value_print(s,stdout);
    else
      printf("null");
    printf(" | scope=%s m=%s mi=%s mid=%s pp=%d parent=%x\n",
	   scope ? scope->s_path : "(null)",
	   m->m_name,
	   mi->mc_path,
	   mid ? mid->mii_name : "(null)",
	   mip->mip_ppPos,
	   mi->mc_parent
	   );
  }
#endif

  if (!s) return;

  if (ModuleInst_findNet(mi,mip->mip_name)) {
    errorModule(mi->mc_mod,ModuleItem_getPlace(mip),ERR_REDEFP,mip->mip_name);
    return;
  }

  ModuleInst_defParm(mi,mip->mip_name,s);

#if DEBUG
  printf("parameter %s%s = ",mi->mc_path,mip->mip_name);
  Value_print(s,stdout);
  printf("\n");
#endif    
}

/*****************************************************************************
 *
 * Process a net/reg declaration statement.
 *
 * Parameters:
 *     c		Circuit in which to declare nets
 *     m		Module declaration in which net declaration is contained
 *     mi		Current module instance
 *     mid		Net declaration to process
 *
 *****************************************************************************/
void Circuit_declareNet(Circuit *c,Scope *scope,NetDecl *nd,ModuleDecl *m,Place *place)
     //static void Circuit_buildNets_netdecl(Circuit *c,ModuleDecl *mggg,ModuleInst *mi,MINetDecl *mid)
{
  unsigned msb,lsb,beginAddr,endAddr;
  int has_mem = 0;
  char buf[STRMAX];
  Net *n;

  /*
   * Get the bit range declaration if there is one, or use [0:0] if not.
   */
  if (nd->n_range) {
    if (VRange_parmEvalMsb(nd->n_range,scope,&msb) < 0
	|| VRange_parmEvalLsb(nd->n_range,scope,&lsb) < 0) {
      return;
    }

    if (lsb != 0 || msb < lsb) {
      char range[256];
      sprintf(range,"%d:%d",msb,lsb);
      errorModule(m,place,ERR_BADRANGE,range,nd->n_name);
    }
  } else
    msb = lsb = 0;

  /*
   * If there is an address range, parse it.
   */
  if (nd->n_addrRange) {
    if (VRange_parmEvalMsb(nd->n_addrRange,scope,&beginAddr) < 0
	|| VRange_parmEvalLsb(nd->n_addrRange,scope,&endAddr) < 0) {
      return;
    }

    /* order does not mater, swap to make start the lower number */
    if (endAddr < beginAddr) { unsigned t = beginAddr; beginAddr = endAddr; beginAddr = t; }
    has_mem = 1;

    if (!(nd->n_type & NT_P_REG)) {
      errorModule(m,place,ERR_NOTREG);
      return;
    }
  }

  /*
   * Make sure we are not redefining a net.  This test is probably redundant and not
   * possible to get.
   */
  if (Scope_findNet(scope,nd->n_name,SDF_LOCAL_ONLY)) {
    errorModule(m,place,ERR_REDEF,nd->n_name);
    return;
  }
  sprintf(buf,"%s.%s",scope->s_path,nd->n_name);

  /*
   * Create the net or memory.
   */
  if (has_mem)
    n = new_Net_memory(buf,msb,lsb,beginAddr,endAddr);
  else
    n = new_Net(buf,nd->n_type,msb,lsb);

  /*
   * Define it.
   */
  Scope_defNet(scope,nd->n_name,n);
}

/*****************************************************************************
 *
 * Bind internal and external ports of an instance
 *
 * Parameters:
 *      mi			Module instance on which to bind ports
 *      parent_ctx		Module containing mi
 *      subM			Definition of module instance mi
 *      subM_ctx		Context of module instance mi
 *      iPorts			Returned nets for internal ports (inside subM_ctx)
 *      ePorts			Returned nets for external ports (inside parent_ctx)
 *      n			Number of ports
 *
 * Returns:			Non-zero if succesful.
 *
 *****************************************************************************/
static int Circuit_bindPorts(MIInstance *mi,ModuleInst *parent_ctx,ModuleDecl *subM,ModuleInst *subM_ctx,Net **iPorts,Expr **ePorts,int n)
{
  ListElem *le;
  NameExpr *ne;
  int pnum = 0;
  int ok = 1;
  int i;

  for (i = 0;i < n;i++) {
    iPorts[i] = 0;
    ePorts[i] = 0;
  }

  ne = (NameExpr*) ((List_numElems(mi->mii_ports) > 0) ? ListElem_obj(List_first(mi->mii_ports)) : 0);
  if (ne && ne->ne_name) {
    /*****************************************************************************
     * Associate ports by name
     *****************************************************************************/
    SHash donePorts;

    SHash_init(&donePorts);

    for (le = List_first(mi->mii_ports);le;le = List_next(mi->mii_ports,le)) {
      NetDecl *nd;
      Net *net;

      ne = (NameExpr*) ListElem_obj(le);
      if (!ne->ne_name) {
	errorModule(parent_ctx->mc_mod,ModuleItem_getPlace(mi),ERR_PORTMIX,mi->mii_instName,subM->m_name);
	ok = 0;
	continue;
      }

      nd = ModuleDecl_findNet(subM,ne->ne_name);
      net = ModuleInst_findNet(subM_ctx,ne->ne_name);

      if (!nd || !net) {
	errorNet(parent_ctx->mc_mod,ne->ne_name,ModuleItem_getPlace(mi),ERR_PORTNOTDEF,ne->ne_name,
		 mi->mii_instName,subM->m_name);
	ok = 0;
	continue;
      }
      if (SHash_find(&donePorts,ne->ne_name)) {
	errorNet(parent_ctx->mc_mod,ne->ne_name,ModuleItem_getPlace(mi),ERR_MULTCONN,
		 ne->ne_name,mi->mii_instName,subM->m_name);
	ok = 0;
	continue;
      }

      iPorts[pnum] = net;
      ePorts[pnum] = ne->ne_expr;
      pnum++;

      SHash_insert(&donePorts,ne->ne_name, nd);
    }

    /*****************************************************************************
     * Check for any unconnected ports left in the module
     *****************************************************************************/
    for (le = List_first(&subM->m_ports);le;le = List_next(&subM->m_ports,le)) {
      const char *portName = (const char*)ListElem_obj(le);
      if (!SHash_find(&donePorts,portName)) {
	errorNet(parent_ctx->mc_mod,portName,ModuleItem_getPlace(mi),ERR_NOCONN,
		 portName,mi->mii_instName,subM->m_name);
	ok = 0;
      }
    }



    SHash_uninit(&donePorts);
  } else {
    /*****************************************************************************
     * Associate ports by position
     *****************************************************************************/
    if (List_numElems(mi->mii_ports) != List_numElems(&subM->m_ports)) {
      errorModule(parent_ctx->mc_mod,ModuleItem_getPlace(mi),ERR_PORTCOUNT,mi->mii_instName,subM->m_name);
      ok = 0;
    } else {
      ListElem *le2;
      for (le = List_first(mi->mii_ports), le2 = List_first(&subM->m_ports);
	   le && le2;
	   le = List_next(mi->mii_ports,le), le2 = List_next(&subM->m_ports,le2)) {

	const char *portName = (const char*)ListElem_obj(le2);
	NetDecl *nd = ModuleDecl_findNet(subM,portName);
	Net *n = ModuleInst_findNet(subM_ctx,portName);

	ne = (NameExpr*) ListElem_obj(le);

	if (!nd || !n) {
	  errorNet(parent_ctx->mc_mod,portName,ModuleItem_getPlace(mi),ERR_NOCONN,portName,mi->mii_instName,subM->m_name);
	  ok = 0;
	  continue;
	}

	iPorts[pnum] = n;
	ePorts[pnum] = ne->ne_expr;
	pnum++;
      }
    }
  }

  return ok;
}

/*****************************************************************************
 *
 * Merge nets
 *
 * Parameters:
 *     c		Circuit in which to perform merge
 *     eNet		Net to be retained after merge
 *     eCtx		Context of eNet
 *     iNet		Net to be eliminated after merge
 *     iCtx		Context of iNet
 *     
 * This function is used to elaborate modules ports when the internal and external
 * connections are compatable.
 *
 *****************************************************************************/
static void Circuit_mergeNets(Circuit *c,Net *eNet,ModuleInst *eCtx,Net *iNet,ModuleInst *iCtx)
{
  const char *shortName = strrchr(iNet->n_name,'.');

  if (shortName && *shortName) 
    shortName++;
  else
    shortName = iNet->n_name;

  SHash_replace(&c->c_nets,iNet->n_name,eNet);
  Scope_replaceLocalNet(ModuleInst_getScope(iCtx),shortName,eNet);

  delete_Net(iNet);
}

/*****************************************************************************
 *
 * Make an implicit 'assign' for values beinging passed as a module input.
 * 
 * Parameters:
 *     c		Circuit being elaborated
 *     iNet		Internal net of port
 *     iCtx		Internal module instance
 *     expr		External expression for port
 *     eCtx		External module instance
 *     codeBlock	CodeBlock to use.
 *
 * This function is used to elaborate module inputs that can not be implemented
 * as direct connections.  For example, in:
 *
 *   foo f1(.z(w1), .a(w3 & w4), .b(w5));
 *
 * the input to port a is an expression.  The port is treated as if it where an
 * assign statement for the form:
 *
 *   assign top.f1.a = top.w3 & top.w4;
 *
 *****************************************************************************/
static void Circuit_makeInAssign(Circuit *c,Net *iNet,ModuleInst *iCtx,Expr *expr,ModuleInst *eCtx,CodeBlock *codeBlock)
{
  VGThread *thread = new_VGThread(codeBlock,CodeBlock_size(codeBlock),eCtx,0);
  int lsize = Net_nbits(iNet);
  int rsize = Expr_getBitSize(expr, ModuleInst_getScope(eCtx));
  int size = imax(lsize,rsize);
  unsigned top_bc;
  Trigger *trigger;
  Value *rhs_ret;
  int driver_id;

  top_bc = CodeBlock_size(codeBlock);
  rhs_ret = Expr_generate(expr,size,ModuleInst_getScope(eCtx), codeBlock);
  if (!rhs_ret) return;

  driver_id = Net_addDriver(iNet);
  BCWireAsgnD_init(CodeBlock_nextEmpty(codeBlock),iNet,driver_id,0,rhs_ret,0,rsize,0);

  trigger = Expr_getDefaultTrigger(expr, ModuleInst_getScope(eCtx));
  BCTrigger_init(CodeBlock_nextEmpty(codeBlock),trigger);

  BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);

  List_addToTail(&eCtx->mc_threads, thread);
}

/*****************************************************************************
 *
 * Make an implicit 'assign' for values beinging passed as a module output.
 * 
 * Parameters:
 *     c		Circuit being elaborated
 *     expr		External expression for port
 *     eCtx		External module instance
 *     iNet		Internal net of port
 *     iCtx		Internal module instance
 *     codeBlock	CodeBlock to use.
 *
 * This function is used to elaborate module inputs that can not be implemented
 * as direct connections.  For example, in:
 *
 *   foo f1(.z({w1,w2}), .a(w3 & w4), .b(w5));
 *
 * The output from port z has is a concatenation expression.  The port is treated 
 * as if it where an assign statement for the form:
 *
 *   assign {top.w1,top.w2} = top.f1.z;
 *
 *****************************************************************************/
static void Circuit_makeOutAssign(Circuit *c,Expr *expr,ModuleInst *eCtx,Net *iNet,
				  ModuleInst *iCtx,CodeBlock *codeBlock)
{
  VGThread *thread = new_VGThread(codeBlock,CodeBlock_size(codeBlock),eCtx,0);
  unsigned top_bc;
  Trigger *trigger;
  Value *rhs_ret;
  List lhs_list;		/* List of left-hand side nets */
  ListElem *le;
  unsigned base_bit = 0;

  top_bc = CodeBlock_size(codeBlock);
  trigger = Circuit_getNetTrigger(c, iNet, TT_EDGE);

  //  BCTrigger_init(CodeBlock_nextEmpty(codeBlock),trigger);
  rhs_ret = &iNet->n_data.value;

  List_init(&lhs_list);
  Expr_expandConcat(expr, ModuleInst_getScope(eCtx), &lhs_list);

  for (le = List_first(&lhs_list);le;le = List_next(&lhs_list,le)) {
    Expr *lhs_e = (Expr*) ListElem_obj(le);
    Net *n;
    Value *nLsb;
    unsigned lhs_size;
    int driver_id;

    if (Expr_lhsGenerate(lhs_e,ModuleInst_getScope(eCtx),codeBlock,&n,&nLsb,&lhs_size,0) < 0) {
      errorModule(eCtx->mc_mod,Place_getCurrent(),ERR_BADOUT);
      return;
    }

    driver_id = Net_addDriver(n);
    BCWireAsgnD_init(CodeBlock_nextEmpty(codeBlock),n,driver_id,nLsb,rhs_ret,base_bit,lhs_size,0);

    base_bit += Expr_getBitSize(lhs_e, ModuleInst_getScope(eCtx));
  }
  BCTrigger_init(CodeBlock_nextEmpty(codeBlock),trigger);

  BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);

  List_addToTail(&eCtx->mc_threads, thread);
}

/*****************************************************************************
 *
 * Return non-zero if internal port iNet and external port eNet can be merged
 * into a single net.
 *
 * Parameters:
 *     iNet		Net for internal port.
 *     eNet		Expression for external port.
 *     eCtx		Module instance eNet is in.
 *
 *****************************************************************************/
static int mergablePort(Net *iNet,Expr *eNet,ModuleInst *eCtx)
{
  Net *en;

  if (!(Net_getType(iNet) & NT_P_WIRE)) return 0;	/* Internal is not a net */
  if (Expr_type(eNet) != E_LITERAL) return 0;		/* External is an expression */
  en = ModuleInst_findNet(eCtx,Expr_getLitName(eNet));		
  if (!en) return 0;					/* Can't find external net */
  if (Net_nbits(iNet) != Net_nbits(en)) return 0;	/* Bit size does not match */
  if (!(Net_getType(en) & NT_P_WIRE) && 		/* External is a register, but */
      !(Net_getType(iNet) & NT_P_INPUT)) return 0;	/*   not an input port */ 

  return 1;
}

/*****************************************************************************
 *
 * Expand a module instance
 *
 * Paramaters:
 *     c		Circuit in which to expand instance
 *     m		Module to expand
 *     mi		Module instance
 *     mid		Module instance declaration
 *     path		Path of containing module
 *     codeBlock	CodeBlock for generating parameter expressions
 *
 *****************************************************************************/
static int Circuit_buildHier_instance(Circuit *c,ModuleDecl *m,ModuleInst *mi,MIInstance *mid,
				       ModuleInst *parent,char *path,CodeBlock *codeBlock)
{
  char *pathTail = strend(path);		/* Save old tail of path */
  ModuleDecl *subM;
  ModuleInst *subM_ctx;

  subM = VGSim_findModule(&vgsim, mid->mii_name);
  if (!subM) {
    errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_MODUNDEF,mid->mii_name);
    return 1;
  }
  sprintf(pathTail,".%s", mid->mii_instName);

  subM_ctx = Circuit_buildNets(c,subM,mid,parent,path);

  /*
   * Check for parameter port consistency.
   */
  if (List_numElems(mid->mii_parms) > 0) {
    NameExpr *ne = (NameExpr*) ListElem_obj(List_first(mid->mii_parms));
    ListElem *ple;

    if (!ne->ne_name) {
      /*
       * Must not mix types
       */
      for (ple = List_first(mid->mii_parms);ple;ple = List_next(mid->mii_parms, ple)) {
	ne = (NameExpr*) ListElem_obj(ple);
	if (ne->ne_name) {
	  errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_PARAMMIX,mid->mii_instName,
		      subM->m_name);
	  break;
	}
      }

      /*
       * Instance does not explicitly named parameters.  Need match number only.
       */
      if (List_numElems(mid->mii_parms) > List_numElems(&subM->m_parmPorts)) {
	errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_TOOMANYPP,mid->mii_instName);
      } else if (List_numElems(mid->mii_parms) < List_numElems(&subM->m_parmPorts)) {
	errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_TOOFEWPP,mid->mii_instName);
      }
    } else {
      /*
       * Every parameter name used in instance must be valid and not duplicated.
       */
      for (ple = List_first(mid->mii_parms);ple;ple = List_next(mid->mii_parms, ple)) {
	ne = (NameExpr*) ListElem_obj(ple);
	if (!ne->ne_name) {
	  errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_PARAMMIX,mid->mii_instName,
		      subM->m_name);
	  break;
	}
	if (!ModuleDecl_isParm(subM,ne->ne_name))
	  errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_NOTPPORT,ne->ne_name);
      }
    }
  }

  if (List_numElems(mid->mii_ports) > 0 || List_numElems(&subM->m_ports) > 0) {
    /*****************************************************************************
     * Determine the correct association between internal and external ports.  We
     * know there is at least one port and that either all ports are associate by
     * name or all are associated by position.  Check the first instance port for
     * a name to see which case we are doing.
     *****************************************************************************/
    int n = imax(List_numElems(mid->mii_ports),List_numElems(&subM->m_ports));
    Net **iPorts = (Net**) malloc(sizeof(Net*)*n);
    Expr **ePorts = (Expr**) malloc(sizeof(Expr*)*n);
    int i;

    Circuit_bindPorts(mid,mi,subM,subM_ctx,iPorts,ePorts,n);

    for (i = 0;i < n;i++) {
      if (!iPorts[i] || !ePorts[i])	/* Error messages for these cases have already been generated */
	continue;

      if (mergablePort(iPorts[i],ePorts[i],mi)) {
	/*****************************************************************************
	 * Port connection is a direct connect candidate.
	 *****************************************************************************/
	Net *eNet = ModuleInst_findNet(mi,Expr_getLitName(ePorts[i]));

	if (!eNet) {
	  errorModule(mi->mc_mod,ModuleItem_getPlace(mid),ERR_IE_NONET,Expr_getLitName(ePorts[i]));
	  continue;
	}
	Circuit_mergeNets(c,eNet,mi,iPorts[i],subM_ctx);
      } else {
	const char *portName = Net_getLocalName(iPorts[i]);

	/*****************************************************************************
	 * Explicit port function is required.
	 *****************************************************************************/
	switch ((Net_getType(iPorts[i]) & NT_P_IO_MASK)) {
	case NT_P_INPUT :
	  Circuit_makeInAssign(c,iPorts[i],subM_ctx,ePorts[i],mi,codeBlock);

	  if (Net_nbits(iPorts[i]) != Expr_getBitSize(ePorts[i],ModuleInst_getScope(mi))) {
	    errorNet(mi->mc_mod,portName,ModuleItem_getPlace(mid),WRN_INPORT,portName);
	  }

	  break;
	case NT_P_OUTPUT :
	  Circuit_makeOutAssign(c,ePorts[i],mi,iPorts[i],subM_ctx,codeBlock);

	  if (Net_nbits(iPorts[i]) != Expr_getBitSize(ePorts[i],ModuleInst_getScope(mi))) {
	    errorNet(mi->mc_mod,portName,ModuleItem_getPlace(mid),WRN_OUTPORT,portName);
	  }

	  break;
	case NT_P_INOUT :
	  errorNet(mi->mc_mod,portName,ModuleItem_getPlace(mid),ERR_BADINOUT,portName);
	  break;
	default :
	  errorNet(mi->mc_mod,portName,ModuleItem_getPlace(mid),ERR_BADPORTTYPE,portName);
	  break;
	}
      }
    }
  }

  Circuit_buildHier(c,subM_ctx,parent,path);
  *pathTail = 0;

  return 0;
}

/*****************************************************************************
 *
 * Do final actions to generate a module instance
 *
 * Parameters:
 *      c			Circuit to we are building
 *      mi			Module instance
 *      codeBlock		Code block to use 
 *
 * This function should be called after generating all bytecode for the module
 * definition.  This function will generate any specify task calls (e.g., $setup,
 * $hold), close the codeblock for the module, and create and register all threads
 * used by this module instance.
 *
 *****************************************************************************/
void Circuit_finishModuleInst(Circuit *c, ModuleInst *mi, CodeBlock *codeBlock)
{
  EvQueue *Q = Circuit_getQueue(c);
  ModuleDecl *m = mi->mc_mod;
  ListElem *le;

  if (ModuleDecl_getSpecify(mi->mc_mod))
    Specify_generateTasks(ModuleDecl_getSpecify(mi->mc_mod),mi,codeBlock);

  CodeBlock_close(codeBlock);
  ModuleInst_setCodeBlock(mi,codeBlock);

  /*
   * If we saw any errors this time through we record them.
   */
  m->m_errorsDone = 1;

  /*
   * Queue all threads from this module for execution.
   */
  for (le = List_first(&mi->mc_threads);le;le = List_next(&mi->mc_threads,le)) {
    VGThread *t = (VGThread*) ListElem_obj(le);

    EvQueue_enqueueAfter(Q,new_EvThread(t),0);
    VGThread_start(t);
  }
}


/*****************************************************************************
 *
 * This is the workhorse function for building the circuit
 *
 * Parameters:
 *     c		Circuit in which to expand the module.
 *     mi		Module instance that we are in.
 *     parent		Parent module
 *     path		Path prefix for expanding sub-modules.
 *
 *****************************************************************************/
static void Circuit_buildHier(Circuit *c,ModuleInst *mi,ModuleInst *parent,char *path)
{
  CodeBlock *codeBlock = new_CodeBlock(mi);
  ModuleDecl *m = mi->mc_mod;
  int error_count = 0;
  ListElem *le;
  HashElem *he;
  SHash *mi_tasks = &mi->mc_scope.s_tasks;

  /*
   * If there are any path delay statements, use special compilation code to handle
   * the object as a specify module.
   */
  if (ModuleInst_isPathDelayMod(mi)) {
    Circuit_buildPathDelayMod(c,mi,parent,path);
    return;
  }

  for (he = Hash_first(mi_tasks);he; he = Hash_next(mi_tasks,he)) {
    UserTask *ut = (UserTask*) HashElem_obj(he);
    UserTask_generate(ut, codeBlock);
  }

  for (le = List_first(&m->m_items);le; le = List_next(&m->m_items,le)) {
    ModuleItem *item = (ModuleItem*) ListElem_obj(le);

    switch (item->mi_type) {
    case IC_PARAMETER :
    case IC_NETDECL :
    case IC_REGINIT :
      break;
    case IC_ASSIGN :
    case IC_ALWAYS :
    case IC_INITIAL :
    case IC_GATE :
      {
	VGThread *t;
	t = ModuleItem_generate(item,mi,codeBlock);
	if (t) ModuleInst_addThread(mi, t);
      }
      break;
    case IC_INSTANCE :
      error_count += Circuit_buildHier_instance(c,m,mi,(MIInstance*)item,parent,path,codeBlock);
      break;
    }
  }

  Circuit_finishModuleInst(c,mi,codeBlock);
}

/*****************************************************************************
 *
 * Process parameter and net/reg declarations for a module instance.
 *
 * Parameters:
 *     c	Circuit in which to expand the module.
 *     m	Module to be expanded.
 *     mid	Module instance declartion
 *     parent	Parent module instance.
 *     path	Path prefix for expanding sub-modules.
 *
 * Returns:	ModuleInst object
 *
 *****************************************************************************/
static ModuleInst *Circuit_buildNets(Circuit *c,ModuleDecl *m,MIInstance *mid,ModuleInst *parent,char *path)
{
  ModuleInst *mi;
  ListElem *le;
  HashElem *he;

  mi = new_ModuleInst(m,c,parent,path);
  SHash_insert(&c->c_moduleInsts,path,mi);
  
  for (he = Hash_first(&m->m_tasks);he; he = Hash_next(&m->m_tasks,he)) {
    UserTaskDecl *utd = (UserTaskDecl*) HashElem_obj(he);
    UserTask *ut = new_UserTask(utd, ModuleInst_getScope(mi));

    ModuleInst_defineTask(mi,UserTaskDecl_getName(utd),ut);
  }

#if 0
  printf("bn: %s parent: %s  path: %s\n",
	 mid ? mid->mii_name : "(null)",
	 parent ? parent->mc_path : "(null)",path);
#endif

  for (le = List_first(&m->m_items);le; le = List_next(&m->m_items,le)) {
    ModuleItem *item = (ModuleItem*) ListElem_obj(le);

    switch (item->mi_type) {
    case IC_PARAMETER :
      Circuit_buildNets_parameter(c,m,mi,(MIParameter*)item,mid);
      break;
    case IC_REGINIT :
      Circuit_buildNets_reginit(c,m,mi,(MIAssign*)item,mid);
      break;
    case IC_NETDECL :
      {
	MINetDecl *mid = (MINetDecl*)item;
	NetDecl *nd = mid->mid_decl;

	/*
	 * Internal nets for path delay modules are defered until later
	 */
	if (ModuleInst_isPathDelayMod(mi) && (NetDecl_getType(nd) & NT_P_IO_MASK) == 0) break;

	Circuit_declareNet(c,
			   ModuleInst_getScope(mi),
			   nd,
			   m,ModuleItem_getPlace(item));
      }
      break;
    case IC_ASSIGN :
    case IC_ALWAYS :
    case IC_INITIAL :
    case IC_INSTANCE :
    case IC_GATE :
      break;
    }
  }

  return mi;
}

/*****************************************************************************
 *
 * Build the instantiated circuit.
 *
 * Parameters:
 *     c		Circuit to be built
 *     m		Top-level module to build
 *
 *****************************************************************************/
void Circuit_build(Circuit *c,ModuleDecl *m)
{
  char path[STRMAX*2];

  strcpy(path,m->m_name);
  c->c_root = Circuit_buildNets(c,m,0,0,path);
  Circuit_buildHier(c,c->c_root,0,path);
}

/*****************************************************************************
 *
 * Sort the threads scheduled at the current time (normally 0) to ensure
 * proper initilization.
 *
 *****************************************************************************/
void Circuit_sortThreads(Circuit *c)
{
  /*may not be necessary*/
#if 0
  Event *e = c->c_evQueue->eq_wheelHead[0];

  for (;e;e = e->ev_base.eb_next) {
    switch (Event_getType(e)) {
    case EV_UNKNOWN : printf("unknown\n"); break;
    case EV_THREAD :
      {
	VGThread *t = e->ev_thread.et_thread;
	ModuleItem *mi = VGThread_getMItem(t);
	
	if (mi) {
	  printf("thread %p - %d\n",t,ModuleItem_getType(mi));
	  ModuleItem_print(mi,stdout);
	} else
	  printf("thread %p - null\n",t);
      }
      break;
    case EV_NET : printf("net\n"); break;
    case EV_CONTROL : printf("control\n"); break;
    case EV_DRIVER : printf("driver\n"); break;
    case EV_STROBE : printf("strobe\n"); break;
    case EV_PROBE : printf("probe\n"); break;
    }
  }
#endif
}


/*****************************************************************************
 *
 * Install a script module into the circuit.
 *
 * Parameters:
 *     c		Circuit to be built
 *     m		Top-level module to build
 *
 *****************************************************************************/
void Circuit_installScript(Circuit *c,ModuleDecl *m,DynamicModule *dm)
{
  static int count = 0;
  char scriptRootName[STRMAX];
  ModuleInst *mi;
  ListElem *le;
  
  sprintf(scriptRootName,"%%script%d",count++);
  mi = new_ModuleInst(m,c,0,scriptRootName);
  DynamicModule_setModuleInst(dm,mi);

  Scope_setPeer(ModuleInst_getScope(mi),ModuleInst_getScope(c->c_root));
  mi->mc_peer = c->c_root;
  
  for (le = List_first(&m->m_items);le; le = List_next(&m->m_items,le)) {
    ModuleItem *item = (ModuleItem*) ListElem_obj(le);

    if (item->mi_type == IC_NETDECL) {
      //      Circuit_buildNets_netdecl(c,m,mi,(MINetDecl*)item);
      Circuit_declareNet(c,ModuleInst_getScope(mi),
      			 ((MINetDecl*)item)->mid_decl,
      			 m,ModuleItem_getPlace(item));
    }
  }

  Circuit_buildHier(c,mi,0,scriptRootName);
}

/*****************************************************************************
 *
 * Check a circuit for potential problems
 *
 * Parameters:
 *      c		Circuit to be checked
 *
 * This function does a final check for errors/warnings in the circuit.  The
 * major check is for floating wires (wires with no driving signal).  When
 * reporting floating nets, we need to be sure to report them only once for
 * each module type, and not for each module instance.  We do this by reporting
 * the error in the first instance of a type that we find.
 *
 *****************************************************************************/
 void Circuit_check(Circuit *c)
{
  HashElem *he;
  SHash reported;

  SHash_init(&reported);


  for (he = Hash_first(&c->c_nets);he;he = Hash_next(&c->c_nets, he)) {
    Net *n = (Net*) HashElem_obj(he);

    //
    // Ignore entries that are parameters, or not the primary name for a net.
    //
    if ((Net_getType(n) & NT_P_PARAMETER)) continue;
    if (strcmp(SHashElem_key(he), n->n_name) != 0) continue;


    //
    // Check for floaters only on nets
    //
    if ((Net_getType(n) & NT_P_WIRE)) {
      if ( (n->n_flags & NA_INPATHDMOD) ) continue;


      if (n->n_numDrivers == 0) {
	char instname[STRMAX];
	ModuleInst *m;
	char *localName;

	strcpy(instname,n->n_name);
	localName = strrchr(instname,'.');
	if (localName) *localName++ = 0;
	
	m = Circuit_findModuleInst(c,instname);

	if (m) {
	  char buf[STRMAX];

	  sprintf(buf,"%s.%s",ModuleDecl_getName(m->mc_mod),localName);
	  if (!SHash_find(&reported,buf)) {
	    m->mc_mod->m_errorsDone = 0;
	    errorNet(m->mc_mod,n->n_name,&m->mc_mod->m_place,WRN_FLOATNET,localName);
	    SHash_insert(&reported,buf,n);
	  }
	}
      }
    }
  }

  SHash_uninit(&reported);
}

/*****************************************************************************
 *
 * Begin simulating a circuit
 *
 * Parameters:
 *     c		Circuit to simulate
 *
 *****************************************************************************/
void Circuit_run(Circuit *c)
{
  if (vgsim.vg_interactive)
    EvQueue_interactiveMainEventLoop(c->c_evQueue);
  else
    EvQueue_mainEventLoop(c->c_evQueue);
}

/*****************************************************************************
 *
 * Get a trigger for the specified posesges and negedges lists.
 *
 * Parameters:
 *     c		Circuit to use.
 *     posedges		List of Net* that trigger on a posedge
 *     negedges		List of Net* that trigger on a negedge
 *
 *****************************************************************************/
Trigger *Circuit_getTrigger(Circuit *c,List *posedges,List *negedges)
{
  unsigned hc = Trigger_sortAndGetHashCode(posedges,negedges);
  Trigger *tlist,*r;

  tlist = NHash_find(&c->c_triggers,hc);
  r = Trigger_getTrigger(&tlist,posedges,negedges);
  NHash_replace(&c->c_triggers,hc,tlist);

  return r;
}

/*****************************************************************************
 *
 * Get a trigger for a single net
 *
 * Parameters:
 *     c		Circuit to use.
 *     n		Net to trigger on.
 *     tt		Type of edge to trigger on.
 *
 *****************************************************************************/
Trigger *Circuit_getNetTrigger(Circuit *c,Net *n,transtype_t tt)
{
  Trigger *trigger;
  List posedges, negedges;

  if (tt == TT_NONE) return 0;

  List_init(&posedges);
  List_init(&negedges);

  if (tt == TT_POSEDGE || tt == TT_EDGE)
    List_addToTail(&posedges, n);

  if (Net_nbits(n) == 1 && (tt == TT_NEGEDGE || tt == TT_EDGE))
    List_addToTail(&negedges, n);

  trigger = Circuit_getTrigger(c,&posedges,&negedges);

  List_uninit(&posedges);
  List_uninit(&negedges);

  return trigger;
}

/*****************************************************************************
 *
 * Find the named module instance in the global circuit table.
 *
 * Parameters:
 *     c		Circuit to search
 *     name		Name of module instance to look for.
 *
 *
 *****************************************************************************/
ModuleInst *Circuit_findModuleInst(Circuit *c, const char *name)
{
  ModuleInst *mi;

  mi = (ModuleInst*)SHash_find(&c->c_moduleInsts, name);
  if (!mi) {
    char *fullName = (char*) malloc(strlen(c->c_root->mc_path)+strlen(name)+2);
    sprintf(fullName,"%s.%s",c->c_root->mc_path,name);
    mi = (ModuleInst*)SHash_find(&c->c_moduleInsts, fullName);
    free(fullName);
  }

  return mi;
}

/*****************************************************************************
 *
 * Find the named net in the global circuit table.
 *
 * Parameters:
 *     c		Circuit to search
 *     name		Name of net to look for.
 *
 *
 *****************************************************************************/
Net *Circuit_findNet(Circuit *c,const char *name)
{
  return (Net*) SHash_find(&c->c_nets,name);
}

/*****************************************************************************
 *
 * Find the named memory net in the global circuit table.
 *
 * Parameters:
 *     c		Circuit to search
 *     name		Name of net to look for.
 *
 * If 'name' specifies an existing net, then that net is returned if it is a
 * memory or null is returned if it is a non-memory.  If 'name' specifies a
 * module, the module is scanned for memory nets.  If the module countains
 * exactly one memory net, that net is returned.
 *
 *****************************************************************************/
Net *Circuit_findMemoryNet(Circuit *c,const char *name)
{
  Net *net = Circuit_findNet(c,name);
  ModuleInst *m;
  HashElem *he;
  SHash *nets;

  if (net && (Net_getType(net) & NT_P_MEMORY)) {
    return net;
  }

  net = 0;
  m = Circuit_findModuleInst(c, name);
  if (!m) return 0;  

  nets = &m->mc_scope.s_nets;
  for (he = Hash_first(nets);he;he = Hash_next(nets, he)) {
    Net *n = (Net*) HashElem_obj(he);

    if ((Net_getType(n) & NT_P_MEMORY)) {
      if (net) return 0;	/* There is more than 1 memory */
      net = n;
    }
  }

  return net;
}

/*****************************************************************************
 *
 * Read memory(ies) into a circuit.
 *
 * Parameters:
 *     c
 *     fileName
 *     net
 *     start
 *     stop
 *     flags
 *
 * Data files can contain lines of the following form:
 *
 * @addr				Set current address
 * data1 data2 data3 ...		Assign data from current address
 * addr/data1 data2 data3...		Assign data from specified address
 * memory name				Switch the active memory
 * @memory name				Switch the active memory (alternate syntax)
 * @radix 2/8/10/16			Set the radix for data values
 *
 * The memory file syntax is designed to support either old-style tkgate memory files,
 * or Verilog format memory files.
 *
 *****************************************************************************/
void Circuit_readMemory(Circuit *c, const char *fileName, Net *net, unsigned start, unsigned stop, unsigned flags)
{
  FILE *f;
  char buf[STRMAX],name[STRMAX];
  char *data,*t;
  Memory *m = 0;
  Value *v = 0;
  unsigned addr = start;
  char c_slash;

  if (!(f = openInPath(fileName))) {
    errorRun(ERR_MEMFILE,fileName);
    return;
  }

  while (fgets(buf,STRMAX,f)) {
    if (!m && net) {
      if (!(Net_getType(net) & NT_P_MEMORY)) {
	errorRun(ERR_NOTMEM,Net_getName(net));
	break;
      }
      m = Net_getMemory(net);
      if (v) delete_Value(v);
      v = new_Value(Memory_dataNBits(m));
      addr = 0;
    }

    data = 0;
    if (sscanf(buf," @ %x",&addr) == 1) {
      if (!m) break;
      /* Change of current address */
    } else if (sscanf(buf," @ memory %s",name) == 1 || sscanf(buf," memory %s",name) == 1) {
      net = Circuit_findMemoryNet(c, name);
      m = 0;
      if (!net) {
	errorRun(ERR_NOTMEM,name);
      }
    } else if (sscanf(buf," %*x %c",&c_slash) == 1 && c_slash == '/') {
      if (!m) break;
      sscanf(buf,"%x",&addr);
      data = strchr(buf,'/');
      if (data) data++;
    } else if (sscanf(buf," %s",name) == 1 && *name == '#') {
      continue; /* comment line */
    } else if (sscanf(buf," %s",name) <= 0) {
      continue; /* empty line */
    } else {
      if (!m) break;
      data = buf;
    }

    if (data) {
      if (!m) {
	errorRun(ERR_NOMEM);
	break;
      }

      for (t = strtok(data," \t\n");t;t = strtok(0," \t\n")) {
	if ((flags & SF_HEX))
	  Value_convertHex(v,t,Memory_dataNBits(m));
	else if ((flags & SF_BIN))
	  Value_convertBits(v,t,Memory_dataNBits(m));
	else if ((flags & SF_OCT))
	  Value_convertOct(v,t,Memory_dataNBits(m));
	else if ((flags & SF_DEC))
	  Value_convertDec(v,t,Memory_dataNBits(m));
	else
	  Value_convertHex(v,t,Memory_dataNBits(m));

	Memory_put(m, addr, v);
	addr++;
      }
    }
  }

  if (v) delete_Value(v);
  fclose(f);
}

/*****************************************************************************
 *
 * Writes memory(ies) to a file.
 *
 * Parameters:
 *     c			Pointer to circuit
 *     fileName			File name to write to
 *     net			Net of target memory (or NULL for all memories)
 *     start			Starting address (if single memory) 
 *     stop			Stopping address (if single memory) 
 *     flags			Options
 *
 *****************************************************************************/
int Circuit_writeMemory(Circuit *c, const char *fileName, Net *net, unsigned start, unsigned stop, unsigned flags)
{
  FILE *f;
  Memory *m = 0;

  /*
   * Trying to write something that is not a memory.
   */
  if (net) {
    if (!(Net_getType(net) & NT_P_MEMORY)) {
      errorRun(ERR_NOTMEM,Net_getName(net));
      return 0;
    }
    m = Net_getMemory(net);
  }

  if (!(f = fopen(fileName,"w")))
    return -1;

  if (m) {
    /*
     * Dump one memory.
     */
    Memory_dump(m,f,flags,start,stop);
  } else {
    HashElem *he;

    /*
     * Dump all memories.
     */
    for (he = Hash_first(&c->c_nets);he;he = Hash_next(&c->c_nets,he)) {
      Net *n = (Net*)HashElem_obj(he);

      m = Net_getMemory(n);

      if ((Net_getType(n) & NT_P_MEMORY)) {
	fprintf(f,"\n@memory %s\n",Net_getName(n));
	Memory_dump(m,f,flags,0,~0);
      }
    }
  }

  fclose(f);
  return 0;
}


/*****************************************************************************
 *
 * Find (or create if necessary) the named communication channel.
 *
 * Parameters:
 *      c		Circuit in which to find/create the channel 
 *      name		Name of the channel.
 *
 *****************************************************************************/
Channel *Circuit_getChannel(Circuit *c, const char *name)
{
  Channel *channel;

  channel = (Channel*) SHash_find(&c->c_channels,name);
  if (!channel) {
    channel = new_Channel(name);
    SHash_insert(&c->c_channels,name,channel);
  }

  return channel;
}


/*****************************************************************************
 *
 * Unload a dynamically loaded module
 *
 * Parameters:
 *      c		Circuit object 
 *      dm		Dynamic module object
 *
 *****************************************************************************/
void Circuit_unloadDynamicModule(Circuit *c,DynamicModule *dm)
{
  ListElem *le;

#if 0
  vgio_echo("unloading dynamic module %s.\n",dm->dm_name);
#endif
  for (le = List_first(&dm->dm_threads);le;le = List_next(&dm->dm_threads,le)) {
    VGThread *t = (VGThread*)ListElem_obj(le);

    VGThread_kill(t);
  }
}

void Circuit_disableDynamicModule(Circuit *c,DynamicModule *dm)
{
  ListElem *le;

  for (le = List_first(&dm->dm_threads);le;le = List_next(&dm->dm_threads,le)) {
    VGThread *t = (VGThread*)ListElem_obj(le);

    VGThread_disable(t);
  }
}

void Circuit_enableDynamicModule(Circuit *c,DynamicModule *dm)
{
  ListElem *le;

  for (le = List_first(&dm->dm_threads);le;le = List_next(&dm->dm_threads,le)) {
    VGThread *t = (VGThread*)ListElem_obj(le);
    int wasActive = VGThread_isActive(t);

    VGThread_enable(t);
    if (VGThread_isActive(t) && !wasActive)
      VGThread_exec(t);
  }
}

/*****************************************************************************
 *
 * Create a dynamic module object
 *
 * Parameters:
 *      name		Name of the dynamic module
 *
 *****************************************************************************/
DynamicModule *new_DynamicModule(const char *name)
{
  DynamicModule *dm = (DynamicModule*) malloc(sizeof(DynamicModule));
  dm->dm_name = strdup(name);
  dm->dm_aliveThreads = 0;
  List_init(&dm->dm_mitems);
  List_init(&dm->dm_threads);
  return dm;
}

/*****************************************************************************
 *
 * Delete a dynamic module object
 *
 * Parameters:
 *      dm		Dynamic module object to delete
 *
 *****************************************************************************/
void delete_DynamicModule(DynamicModule *dm)
{
  free(dm->dm_name);
  List_uninit(&dm->dm_mitems);
  List_uninit(&dm->dm_threads);
  free(dm);
}

/*****************************************************************************
 *
 * Receive notification that a thread associated with this module has terminated.
 *
 *****************************************************************************/
void DynamicModule_killNotify(DynamicModule *dm)
{
  // EvQueue *Q = Circuit_getQueue(&vgsim.vg_circuit);
  //  vgio_printf("comment dynamic module [%s] terminated at %llu\n",dm->dm_name,EvQueue_getCurTime(Q));

  vgio_printf("endscript %s\n",dm->dm_name);
}


/*****************************************************************************
 *
 * Add a module item to a dynamic module
 *
 * Parameters:
 *      dm		Dynamic module object
 *      mi		Module item to add
 *
 *****************************************************************************/
void DynamicModule_addMItem(DynamicModule *dm,ModuleItem *mi)
{
  List_addToTail(&dm->dm_mitems,mi);
  ModuleItem_setDynamicModule(mi,dm);
}

/*****************************************************************************
 *
 * Add a thread to a dynamic module
 *
 * Parameters:
 *      dm		Dynamic module object
 *      t		Thread to add
 *
 *****************************************************************************/
void DynamicModule_addThread(DynamicModule *dm,VGThread *t)
{
  List_addToTail(&dm->dm_threads,t);
  dm->dm_aliveThreads++;
}

