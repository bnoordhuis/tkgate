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

    Last edit by hansen on Thu Jul 27 00:25:37 2006
****************************************************************************/
#include <ctype.h>
#include "thyme.h"

/*****************************************************************************
 *
 * Compare literal strings in Expr* format.
 *
 * Parameters:
 *      vA		Pointer to Expr*
 *      vB		Pointer to Expr*
 *
 *****************************************************************************/
#if 0
static int cmpExprStr(const void *vA, const void *vB)
{
  Expr *A = *(Expr**)vA;
  const char *B = *(const char**)vB;

  if (Expr_type(A) == LITERAL) return -1;
  return strcmp(Expr_getLitName(A), B);
}
#endif

/*****************************************************************************
 *
 * For each path delay specification, compute the actual delay value.
 *
 * Parameters:
 *      c		Circuit object
 *      mi		Module instance object
 *
 *****************************************************************************/
static int Circuit_dpath_computeDelays(Circuit *c,ModuleInst *mi)
{
  Specify *spec = ModuleDecl_getSpecify(ModuleInst_getModDecl(mi));
  ListElem *le;

  /*
   * Compute values of delay parameters
   */
  for (le = List_first(Specify_getStats(spec));le;le = List_next(Specify_getStats(spec),le)) {
    SpecifyStat *ss = (SpecifyStat*)ListElem_obj(le);
    unsigned delay;
    Expr_parmEvalI(ss->ss_delay, ModuleInst_getScope(mi), &delay, PEF_SPECPARM);
    ss->ss_idelay = delay;
  }

  return 0;
}

/*****************************************************************************
 *
 * Mark all nets as being part of a path delay module and extract the ports.
 *
 * Parameters:
 *      c		Circuit object
 *      mi		Module instance object
 *      inset		Return for input ports
 *      outset		Return for output ports
 *
 *****************************************************************************/
static int Circuit_dpath_checkPorts(Circuit *c,
						ModuleInst *mi,
						SHash *inset,
						SHash *outset)
{
  ModuleDecl *m = mi->mc_mod;
  HashElem *he;

  /*
   * Generate all of the input handlers
   */
  for (he = Hash_first(ModuleDecl_getNets(m));he;he = Hash_next(ModuleDecl_getNets(m),he)) {
    NetDecl *netdecl = (NetDecl*)HashElem_obj(he);
    Net *net = ModuleInst_findNet(mi, NetDecl_getName(netdecl));

    if (!net) continue;	/* not an I/O net */

    net->n_flags |= NA_INPATHDMOD;

    switch (NetDecl_getType(netdecl) & NT_P_IO_MASK) {
    case NT_P_INPUT :
      SHash_insert(inset, NetDecl_getName(netdecl), net);
      break;
    case NT_P_OUTPUT :
      SHash_insert(outset, NetDecl_getName(netdecl), net);
      break;
    case NT_P_INOUT :
      errorModule(mi->mc_mod,NetDecl_getPlace(netdecl),ERR_PATHDINOUT);
      return -1;
    default :
      break;
    }
  }

  return 0;
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
static Net *Circuit_dpath_findAddNet(Circuit *c,Scope *port_scope,NetDecl *nd,ModuleDecl *m,const char *outName)
{
  Place *place = NetDecl_getPlace(nd);
  unsigned msb,lsb;
  char name[STRMAX],localName[STRMAX];
  Net *n;


  /*
   * Figure out the name we are going to use for this net
   */
  if (strcmp(NetDecl_getName(nd), outName) == 0) {
    sprintf(name,"%s.%s",port_scope->s_path,NetDecl_getName(nd));
    sprintf(localName,"%s",NetDecl_getName(nd));
  } else {
    sprintf(name,"%s.%s$%s",port_scope->s_path,outName,NetDecl_getName(nd));
    sprintf(localName,"%s$%s",outName,NetDecl_getName(nd));
  }

  /*
   * If the net has already been declared, then just return it, but make sure it
   * has been declared as a register
   */
  if ((n = Scope_findNet(port_scope,nd->n_name,SDF_LOCAL_ONLY))) {
    return n;
  }

  /*
   * Get the bit range declaration if there is one, or use [0:0] if not.
   */
  if (nd->n_range) {
    if (VRange_parmEvalMsb(nd->n_range,port_scope,&msb) < 0
	|| VRange_parmEvalLsb(nd->n_range,port_scope,&lsb) < 0) {
      return 0;
    }

    if (lsb != 0 || msb < lsb) {
      char range[256];
      sprintf(range,"%d:%d",msb,lsb);
      errorModule(m,place,ERR_BADRANGE,range,NetDecl_getName(nd));
    }
  } else
    msb = lsb = 0;

  /*
   * If there is an address range, then this is a memory which we do not
   * support in path-delay modules.  After issuing an error, we go ahead
   * and ignore the address range.
   */
  if (nd->n_addrRange) {
    nd->n_addrRange = 0;
    errorModule(m,place,ERR_PATHDMEM, NetDecl_getName(nd));
  }

  /*
   * Create the actual net, and define it.
   */
  n = new_Net(name,nd->n_type,msb,lsb);
  Scope_defNet(port_scope,nd->n_name,n);
  n->n_type &= ~NT_P_WIRE;
  n->n_type |= NT_P_REG;

  return n;
}

/*****************************************************************************
 *
 * Make/find nets for a FaninNode object in a path-delay module
 *
 * Parameters:
 *      c		Circuit we are building
 *      m		Module we are building
 *      scope		Scope for the module m
 *      port_scope	Temporary scope for the current output
 *      outName		Name of output we are building
 *      fn		Node to which we are attaching nets.
 *
 * Instantiate the nets for a single fanin node in a path-delay module.  If the
 * required net already exists in the port_scope, then that net is used, otherwise
 * the net is created.
 *
 *****************************************************************************/
static void Circuit_dpath_makePortNets(Circuit *c, ModuleDecl *m, Scope *scope, 
				       Scope *port_scope, const char *outName, 
				       FaninNode *fn)
{
  int i;

  /*
   * Create the net arrays for ports if they have not already been created
   */
  if (!fn->fn_outNets) {
    for (i = 0;fn->fn_out[i];i++);
    fn->fn_outNets = (Net**) malloc(sizeof(Net*)*(i+1));
  }
  if (!fn->fn_inNets) {
    for (i = 0;fn->fn_in[i];i++);
    fn->fn_inNets = (Net**) malloc(sizeof(Net*)*(i+1));
  }

  /*
   * Create nets for outputs
   */
  for (i = 0;fn->fn_out[i];i++) {
    NetDecl *nd = ModuleDecl_findNet(m, fn->fn_out[i]);
    fn->fn_outNets[i] = Circuit_dpath_findAddNet(c, port_scope,nd,m,outName);
  }

  /*
   * Create nets for inputs
   */
  for (i = 0;fn->fn_in[i];i++) {
    NetDecl *nd = ModuleDecl_findNet(m, fn->fn_in[i]);
    fn->fn_inNets[i] = Circuit_dpath_findAddNet(c, port_scope,nd,m,outName);
  }
}

static int Circuit_dpath_generate_gate(Circuit *c, ModuleInst *mi, Scope *port_scope, CodeBlock *codeBlock, MIGate *mig)
{
  /* UNIMPLEMENTED */
  errorFile(ModuleItem_getPlace(mig),ERR_IE_UNIMP);
  return 0;
}

/*****************************************************************************
 *
 * Generate the bytecode for an "assign" in a path-delay module
 *
 * Parameters:
 *      c		Circuit we are building
 *      mi		Module instance we are building
 *      port_scope	Scope for the current output
 *      codeBlock	Code block in which to write bytecode
 *      mia		Module instance for the assign
 *
 *****************************************************************************/
static int Circuit_dpath_generate_assign(Circuit *c, ModuleInst *mi, Scope *port_scope,
					 CodeBlock *codeBlock, MIAssign *mia)
{
  int lsize;			/* Base bit size of left-hand side */
  int rsize;			/* Base bit size of right-hand side */
  int size;			/* Bit size for assignment operation */
  unsigned top_bc;		/* Branch position of top of statement */
  Value *rhs_ret;		/* Return value of right-hand side */
  List lhs_list;		/* List of left-hand side nets */
  ListElem *le;			/* Iterator for left-hand-side components */
  int base_bit = 0;		/* Base bit from which to get data for left-hand side */

  lsize = Expr_getBitSize(mia->mia_lhs, port_scope);
  rsize = Expr_getBitSize(mia->mia_rhs, port_scope);
  size = imax(lsize,rsize);

  /*
   * Test to see if getBitSize generated an error, abort generation if error was found.
   */
  if (lsize == 0 || rsize == 0)
    return -1;

  /*
   * Generate bytecode for right-hand side.
   */
  top_bc = CodeBlock_size(codeBlock);
  rhs_ret = Expr_generate(mia->mia_rhs,size, port_scope, codeBlock);
  if (!rhs_ret) return -1;

  /*
   * Expand any left-hand-side concatenation.
   */
  List_init(&lhs_list);
  Expr_expandConcat(mia->mia_lhs, port_scope, &lhs_list);

  /*
   * Find the delay for the assignment.
   */
  if (mia->mia_bcond) {
    /* Not allowed */
    errorModule(ModuleInst_getModDecl(mi), ModuleItem_getPlace(mia), ERR_PATHDCOND);
    return -1;
  }

  for (le = List_first(&lhs_list);le;le = List_next(&lhs_list,le)) {
    Expr *lhs_e = (Expr*) ListElem_obj(le);
    Net *n;
    Value *nLsb;
    unsigned lhs_size;

    if (Expr_lhsGenerate(lhs_e,port_scope, codeBlock,&n,&nLsb,&lhs_size,0) < 0) {
      errorFile(ModuleItem_getPlace(mia),ERR_BADASGNLHS);
      return -1;
    }

    if ((NetDecl_getType(n) & NT_P_REG)) {
      BCAsgn_init(CodeBlock_nextEmpty(codeBlock),n,nLsb,rhs_ret,base_bit,lhs_size);
    } else {
      int driver_id = Net_addDriver(n);
      BCWireAsgnD_init(CodeBlock_nextEmpty(codeBlock),n,driver_id,nLsb,rhs_ret,base_bit,lhs_size,0);
    }

#if 0
    printf(" assigning %s from [%d +: %d] of size %d\n",n->n_name,base_bit,lhs_size,size);
#endif

    base_bit += Expr_getBitSize(lhs_e, port_scope);
  }

  return 0;
}


/*****************************************************************************
 *
 * Recursively generate the tree for a net.
 *
 * Parameters:
 *      c		Circuit we are building
 *      mi		Module instance that we are building
 *      parent		Parent module instance
 *      scope		Scope in which to declare variables
 *	codeBlock	Block in which to write bytecode
 *      parent_fanin	Parent "fanin"
 *      outName		Name of output for which we are generating the tree
 *      netName		Name of net to generate
 *
 *****************************************************************************/
static int Circuit_dpath_outputtree(Circuit *c,
						ModuleInst *mi,
						ModuleInst *parent,
						Scope *port_scope, 
						CodeBlock *codeBlock,
						FaninNode *parent_fanin,
						const char *outName,
						const char *netName)
{
  ModuleDecl *m = ModuleInst_getModDecl(mi);
  FaninNode *fn = ModuleDecl_findFanin(m,netName);
  NetDecl *net = ModuleDecl_findNet(m, netName);
  int ok = 0;
  int i;

  /* No fanin found, this better be an input */
  if (!fn) {

    if (!net) {
      Place *place = parent_fanin ? ModuleItem_getPlace(parent_fanin->fn_item) : ModuleDecl_getPlace(m);
      errorModule(m, place, ERR_NOTDEF, netName);
      return -1;
    }

    if (!(NetDecl_getType(net) & NT_P_INPUT)) {
      Place *place = parent_fanin ? ModuleItem_getPlace(parent_fanin->fn_item) : NetDecl_getPlace(net);
      errorModule(m, place, ERR_PATHDNOTIN, netName);
      return -1;
    }

    return 0;	/* This is an input, do nothing. */
  }

  /*
   * Complain about hierarchical variables.
   */
  if (strchr(netName, '.') != 0) {
    errorModule(m, ModuleItem_getPlace(parent_fanin->fn_item), ERR_BADHIER, netName);
  }

  /*
   * Use bit 1 to indicate that we are currently recursing on this net
   */
  if ((fn->fn_flag & 2)) {
    errorModule(m, ModuleItem_getPlace(fn->fn_item), ERR_PATHDLOOP, netName);
    return -1;
  }

  /*
   * Use bit 0 to indicate that we have already generated this item
   */
  if ((fn->fn_flag & 1)) {
    return 0;
  }
  fn->fn_flag = 3;

  /*
   * Figure out the nets to use on the ports of this module item
   */
  Circuit_dpath_makePortNets(c, m, ModuleInst_getScope(mi), port_scope, outName, fn);

  /*
   * Recursively build the inputs to this item
   */
  for (i = 0;fn->fn_in[i];i++) {
    int r = Circuit_dpath_outputtree(c, mi, parent, port_scope, codeBlock, fn, outName, fn->fn_in[i]);
    if (r < 0) ok = -1;
  }

  switch (ModuleItem_getType(fn->fn_item)) {
  case IC_ASSIGN :
    Circuit_dpath_generate_assign(c, mi, port_scope, codeBlock, (MIAssign*)fn->fn_item);
    break;
  case IC_GATE :
    Circuit_dpath_generate_gate(c, mi, port_scope, codeBlock, (MIGate*)fn->fn_item);
    break;
  default:
    errorModule(m, ModuleItem_getPlace(fn->fn_item), ERR_IE_BADSTATE, "Circuit_dpath_outputtree");
    break;
  }

  fn->fn_flag = 1;	
  
  return ok;
}

static void Circuit_dpath_makeInputHandlers(Circuit *c, ModuleInst *mi, CodeBlock *codeBlock,
					    Scope *port_scope, Scope *scope,const char *outName)
{
  ModuleDecl *m = ModuleInst_getModDecl(mi);
  Specify *spec = ModuleDecl_getSpecify(m);
  ListElem *le,*le2;

  for (le = List_first(&m->m_ports);le;le = List_next(&m->m_ports,le)) {
    char *name = (char*) ListElem_obj(le);
    unsigned top_bc;
    VGThread *thread;
    Trigger *trigger;
    
    Net *pn,*n;
    List spec_stats;


    List_init(&spec_stats);

    pn = Scope_findNet(port_scope, name, SDF_LOCAL_ONLY);
    if (!pn) continue;					/* Port is not in support */
    if (strchr(Net_getName(pn),'$') == 0) continue;	/* Port in not an input */
    n = Scope_findNet(scope, name, SDF_LOCAL_ONLY);
    if (!n) continue;

    /*
     * Create the thread and create the initial trigger
     */
    top_bc = CodeBlock_size(codeBlock);
    thread = new_VGThread(codeBlock,top_bc,mi, 0);
    ModuleInst_addThread(mi, thread);
    trigger = Circuit_getNetTrigger(c,n,TT_EDGE);



    /*
     * Get the relevent delay statements
     */
    Specify_getDelays(spec,name,outName,&spec_stats);

    /*
     * Generate the assignment(s) 
     */
    switch (List_numElems(&spec_stats)) {
    case 0 :
      /*
       * always @(a) z$a <= a;
       */
      BCAsgn_init(CodeBlock_nextEmpty(codeBlock), pn, 0, Net_getValue(n), 0, Net_nbits(n));
      break;
    case 1 :
      /*
       * always @(a) z$a <= #delay a;
       */
      {
	SpecifyStat *ss = ListElem_obj(List_first(&spec_stats));
	if (!ss->ss_cond) {
	  BCNbAsgnD_init(CodeBlock_nextEmpty(codeBlock), pn, 0, Net_getValue(n), 0, Net_nbits(n), ss->ss_idelay);
	  break;
	}
      }
      /* fall through if there are conditions */
    default : 
      /*
       * always @(a)
       *   begin
       *     if (a) z$a <= #delay1 a;
       *     else if (!a) z$a <= #delay2 a;
       *     else z$a <= a;
       *   end
       */
      {
	int did_universal = 0;
	List gotoList;
	unsigned last_goto = 0;

	List_init(&gotoList);

	//	BCDebugPrint_init(CodeBlock_nextEmpty(codeBlock),"BEGIN conditional delay\n");

	for (le2 = List_first(&spec_stats);le2;le2 = List_next(&spec_stats,le2)) {
	  SpecifyStat *ss = ListElem_obj(le2);
	  if (ss->ss_cond) {
	    Value *cond;

	    if (last_goto != 0)
	      BCGoto_setOffset((BCGoto*)CodeBlock_get(codeBlock,last_goto), CodeBlock_size(codeBlock));

	    cond = Expr_generateS(ss->ss_cond, scope, codeBlock);
	    last_goto = CodeBlock_size(codeBlock);
	    BCGoto_init(CodeBlock_nextEmpty(codeBlock), cond, 1, codeBlock, 0);
	    BCNbAsgnD_init(CodeBlock_nextEmpty(codeBlock), pn, 0, Net_getValue(n), 0, Net_nbits(n), ss->ss_idelay);
	    List_addToTail(&gotoList, CodeBlock_get(codeBlock,CodeBlock_size(codeBlock)));
	    BCGoto_init(CodeBlock_nextEmpty(codeBlock), 0, 0, codeBlock, 0);
	  } else {
	    did_universal = 1;

	    if (last_goto != 0)
	      BCGoto_setOffset((BCGoto*)CodeBlock_get(codeBlock,last_goto), CodeBlock_size(codeBlock));
	    BCNbAsgnD_init(CodeBlock_nextEmpty(codeBlock), pn, 0, Net_getValue(n), 0, Net_nbits(n), ss->ss_idelay);

	    break;
	  }
	}
	if (!did_universal) {
	  if (last_goto != 0)
	    BCGoto_setOffset((BCGoto*)CodeBlock_get(codeBlock,last_goto), CodeBlock_size(codeBlock));
	  BCNbAsgnD_init(CodeBlock_nextEmpty(codeBlock), pn, 0, Net_getValue(n), 0, Net_nbits(n), 0);
	}

	for (le2 = List_first(&gotoList);le2;le2 = List_next(&gotoList,le2)) {
	  BCGoto *g = ListElem_obj(le2);
	  BCGoto_setOffset(g, CodeBlock_size(codeBlock));
	}


	List_init(&gotoList);
      }
      break;
    }

    //
    // Wait for trigger condition and go back to do input loop again.
    //
    BCTrigger_init(CodeBlock_nextEmpty(codeBlock),trigger);
    BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);

    List_uninit(&spec_stats);
  }
}

/*****************************************************************************
 *
 * Get the trigger to use with an output port tree.
 *
 * Parameters:
 *      c		Circuit we are building
 *      mi		Module instance we are building
 *      scope		Scope to use for local variable lookups
 *
 *****************************************************************************/
static Trigger *Circuit_dpath_getPortTrigger(Circuit *c, ModuleInst *mi, Scope *scope)
{
  ModuleDecl *m = ModuleInst_getModDecl(mi);
  Trigger *trigger;
  List posedges, negedges;
  ListElem *le;

  List_init(&posedges);
  List_init(&negedges);

  for (le = List_first(&m->m_ports);le;le = List_next(&m->m_ports,le)) {
    char *name = (char*) ListElem_obj(le);
    Net *n;

    n = Scope_findNet(scope, name, SDF_LOCAL_ONLY);
    if (!n) continue;					/* Port is not in support */
    if (strchr(Net_getName(n),'$') == 0) continue;	/* Port in not an input */

    List_addToTail(&posedges, n);
    List_addToTail(&negedges, n);
  }

  trigger = Circuit_getTrigger(c, &posedges, &negedges);

  List_uninit(&posedges);
  List_uninit(&negedges);

  return trigger;
}

/*****************************************************************************
 *
 * Generate bytecode for a path-delay module (restrictions apply)
 *
 * Parameters:
 *     c		Circuit in which to expand the module.
 *     mi		Module instance that we are in.
 *     parent		Parent module
 *     path		Path prefix for expanding sub-modules.
 *
 * This function is called in liu of Circuit_buildHier to create the definiton
 * for modules with path delays.  Only 'assign' and basic gates are allowed in
 * 'path delay' modules.
 *
 *
 *  Basic code structure for an output 'z' that depends on inputs 'a', 'b' and 'c':
 *
 *		goto trigger
 *  top:	[code implementing function for output port 'z']
 *  trigger:	wait for @(z$a or z$b or z$c)
 *		goto top+1
 *
 *  		always @(a) z$a <= #a_to_z_delay a;
 *  		always @(b) z$b <= #b_to_z_delay b;
 *  		always @(c) z$c <= #c_to_z_delay c;
 *    
 *
 *****************************************************************************/
void Circuit_buildPathDelayMod(Circuit *c,ModuleInst *mi,ModuleInst *parent,char *path)
{
  Scope *scope = ModuleInst_getScope(mi);
  CodeBlock *codeBlock = new_CodeBlock(mi);
  SHash/*Net*/ outset;
  SHash/*Net*/ inset;
  HashElem *he;

#if 0
  /* may need to fix this for dynamic modules */
  if (ModuleItem_getDynamicModule(mia))
    DynamicModule_addThread(ModuleItem_getDynamicModule(mia), thread);
#endif


  SHash_init(&outset);
  SHash_init(&inset);

  if (Circuit_dpath_computeDelays(c,mi) < 0) goto error_cleanup;
  if (Circuit_dpath_checkPorts(c,mi,&inset,&outset) < 0) goto error_cleanup;

  /*
   * Generate bytecode for each output.  We create a temporary copy of the module scope
   * for each output we generate
   */
  for (he = Hash_first(&outset);he;he = Hash_next(&outset,he)) {
    Scope *port_scope = new_Scope(Scope_getPath(scope), Scope_getParent(scope), mi);
    const char *outName = SHashElem_key(he);
    VGThread *thread;
    Trigger *trigger;
    unsigned top_bc;
    Net *outNet;

    /*
     * Copy the net for the target output to the port scope. 
     */
    outNet = Scope_findNet(scope,outName, SDF_LOCAL_ONLY);
    Scope_defNet(port_scope, outName, outNet);

    /*
     * Clear the fanin objects.
     */
    ModuleDecl_clearFaninFlags(ModuleInst_getModDecl(mi));

    /*
     * Save top of handler code address and generate a jump to the bottom where the
     * trigger is located.  The 'goto' is a bootstap goto and is only executing during
     * initialization.
     */
    top_bc = CodeBlock_size(codeBlock);
    BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,0);

    /*
     * Generate the code implementing this output
     */
    Circuit_dpath_outputtree(c,mi,parent,port_scope, codeBlock, 0, outName, outName);

    /*
     * Get the trigger for this output port
     */
    trigger = Circuit_dpath_getPortTrigger(c, mi, port_scope);

    /*
     * Create a thread to handle the core part of this output taking the following steps
     *  	1) creating and registering a thread
     *		2) Retargeting the initial goto to point to the end of the code
     *		3) Creating a trigger on changes of the dependent inputs
     *		4) Creating a goto back to the top (after the bootstrap goto)
     */
    thread = new_VGThread(codeBlock,top_bc,mi, 0);
    ModuleInst_addThread(mi, thread);
    BCGoto_setOffset((BCGoto*)CodeBlock_get(codeBlock,top_bc),CodeBlock_size(codeBlock));
    BCTrigger_init(CodeBlock_nextEmpty(codeBlock),trigger);
    BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc+1);

    Circuit_dpath_makeInputHandlers(c, mi, codeBlock, port_scope, scope, outName);

    /*
     * Clean up the temporary scope that we created
     */
    delete_Scope(port_scope);
  }

 error_cleanup:	/* Goto used if something above failed. */
  SHash_uninit(&outset);
  SHash_uninit(&inset);

  Circuit_finishModuleInst(c,mi,codeBlock);
}
