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

    Last edit by hansen on Thu Jan 29 12:49:46 2009
****************************************************************************/
#include "thyme.h"

/*****************************************************************************
 *
 * Table of built-in primitive gate instance types.
 *
 * NOTE: It is assumed in ModuleDecl_topologicalSort that an argtype other
 * than GAC_MULTIOUT has only one output.
 *
 *****************************************************************************/
static GateDesc gateTable[] = {
  /*opcode	name		argtype		minport	m.func		o.func.*/
  {PRIMAND,	"and",		GAC_MULTIN,	2,	Value_and,	0},
  {KWOR,	"or",		GAC_MULTIN,	2,	Value_or,	0},
  {PRIMXOR,	"xor",		GAC_MULTIN,	2,	Value_xor,	0},
  {PRIMNAND,	"nand",		GAC_MULTIN,	2,	Value_and,	Value_not},
  {PRIMNOR,	"nor",		GAC_MULTIN,	2,	Value_or,	Value_not},
  {PRIMXNOR,	"xnor",		GAC_MULTIN,	2,	Value_xor,	Value_not},
  {PMOS,	"pmos",		GAC_FIXED,	3,	Value_pmos,	0},
  {NMOS,	"nmos",		GAC_FIXED,	3,	Value_nmos,	0},
  {BUFIF0,	"bufif0",	GAC_FIXED,	3,	Value_bufif0,	0},
  {BUFIF1,	"bufif1",	GAC_FIXED,	3,	Value_bufif1,	0},
  {NOTIF0,	"notif0",	GAC_FIXED,	3,	Value_bufif0,	Value_not},
  {NOTIF1,	"notif1",	GAC_FIXED,	3,	Value_bufif1,	Value_not},
  {BUF,		"buf",		GAC_MULTOUT,	2,	0,		Value_buf},
  {PRIMNOT,	"not",		GAC_MULTOUT,	2,	0,		Value_not},
};

static void MIParameter_print(MIParameter *mip, FILE *f);
static void MINetDecl_print(MINetDecl *mid, FILE *f);
static void MIAssign_print(MIAssign *mia, FILE *f);
static void MIBlock_print(MIBlock *mib, FILE *f);
static void MIInstance_print(MIInstance *mii, FILE *f);
static void MIGate_print(MIGate *mig, FILE *f);

static VGThread *MIBlock_generate(MIBlock *mib, ModuleInst *modCtx,CodeBlock *codeBlock);
static VGThread *MIAssign_generate(MIAssign *mia, ModuleInst *modCtx, CodeBlock *codeBlock);
static VGThread *MIGate_generate(MIGate *mig, ModuleInst *modCtx,CodeBlock *codeBlock);

static MItemVTable vtable_MIParameter = {
  0,
  (MIprint_f*) MIParameter_print
};
static MItemVTable vtable_MINetDecl = {
  0,
  (MIprint_f*) MINetDecl_print
};
static MItemVTable vtable_MIAssign = {
  (MIgenerate_f*) MIAssign_generate,
  (MIprint_f*) MIAssign_print
};
static MItemVTable vtable_MIBlock = {
  (MIgenerate_f*) MIBlock_generate,
  (MIprint_f*) MIBlock_print
};
static MItemVTable vtable_MIInstance = {
  0,
  (MIprint_f*) MIInstance_print
};
static MItemVTable vtable_MIGate = {
  (MIgenerate_f*) MIGate_generate,
  (MIprint_f*) MIGate_print
};


/*****************************************************************************
 *
 * Create a module item of the specified type (only basic fields are initialized)
 *
 * Parameter:
 *     mitype		Module item type code (IC_* values)
 *
 *****************************************************************************/
ModuleItem *new_ModuleItem(int mitype)
{
  ModuleItem *mi = (ModuleItem*) malloc(sizeof(ModuleItem));

  mi->mi_base.mb_type = mitype;
  Place_copy(&mi->mi_base.mb_place, Place_getCurrent());
  mi->mi_base.mb_place.p_mitem = mi;
  mi->mi_base.mb_dynMod = 0;
  mi->mi_base.mb_flags = 0;

  switch (mitype) {
  case IC_ASSIGN :    mi->mi_base.mb_vtable = &vtable_MIAssign; break;
  case IC_REGINIT :   mi->mi_base.mb_vtable = &vtable_MIAssign; break; /* Share the same vtable */
  case IC_ALWAYS :    mi->mi_base.mb_vtable = &vtable_MIBlock; break;
  case IC_INITIAL :   mi->mi_base.mb_vtable = &vtable_MIBlock; break;
  case IC_INSTANCE :  mi->mi_base.mb_vtable = &vtable_MIInstance; break;
  case IC_PARAMETER : mi->mi_base.mb_vtable = &vtable_MIParameter; break;
  case IC_NETDECL :   mi->mi_base.mb_vtable = &vtable_MINetDecl; break;
  case IC_GATE :      mi->mi_base.mb_vtable = &vtable_MIGate; break;
  }

  return mi;
}

void ModuleItem_killNotify(ModuleItem *mi)
{
  if (mi->mi_base.mb_dynMod)
    DynamicModule_killNotify(mi->mi_base.mb_dynMod);
}

MIBlock *new_MIBlock(itemcode_t btype,StatDecl *stat)
{
  MIBlock *mib = (MIBlock*) new_ModuleItem(btype);

  mib->mib_type = btype;
  mib->mib_stat = stat;

  return mib;
}

MIGate *new_MIGate(unsigned gateType, Expr *delay, const char *instName, VRange *slices, List *ports)
{
  MIGate *mig = (MIGate*) new_ModuleItem(IC_GATE);
  int i;

  mig->mig_desc = 0;
  for (i = 0;i < sizeof(gateTable)/sizeof(gateTable[0]);i++) {
    if (gateTable[i].gd_code == gateType) {
      mig->mig_desc = &gateTable[i];
      break;
    }
  }

  mig->mig_instName = instName ? strdup(instName) : 0;
  mig->mig_delay = delay;
  mig->mig_slices = slices;
  mig->mig_ports = ports;

  if (!mig->mig_desc)
    errorFile(ModuleItem_getPlace(mig), ERR_GATEUNIMP, instName);

  return mig;
}

MIAssign *new_MIAssign(Expr *lhs,Expr *rhs,Expr *bcond)
{
  MIAssign *mia = (MIAssign*) new_ModuleItem(IC_ASSIGN);

  mia->mia_lhs = lhs;
  mia->mia_rhs = rhs;
  mia->mia_bcond = bcond;

  return mia;
}

MIParameter *new_MIParameter(const char *name,Expr *expr)
{
  MIParameter *mip = (MIParameter*) new_ModuleItem(IC_PARAMETER);

  mip->mip_name = strdup(name);
  mip->mip_expr = expr;
  mip->mip_ppPos = -1;

  return mip;
}

MINetDecl *new_MINetDecl(NetDecl *nd)
{
  MINetDecl *mid = (MINetDecl*) new_ModuleItem(IC_NETDECL);

  mid->mid_decl = nd;

  return mid;
}

static void MIParameter_print(MIParameter *mip, FILE *f)
{
  fprintf(f,"  parameter %s = ",mip->mip_name);
  Expr_print(mip->mip_expr,f);
  fprintf(f,";\n");
}



/*****************************************************************************
 *
 * Create a new MIInstance object
 *
 * Parameters:
 *     name		Name of module
 *     slices		Number of slices
 *     parms		List of NameExpr objects representing parameters settings.
 *     ports		List of NameExpr objects representing ports.
 *
 *****************************************************************************/
MIInstance *new_MIInstance(const char *name, const char *instName, VRange *slices, List *parms, List *ports)
{
  MIInstance *mii = (MIInstance*) new_ModuleItem(IC_INSTANCE);

  mii->mii_name = strdup(name);
  mii->mii_instName = strdup(instName);
  mii->mii_slices = slices;
  mii->mii_parms = parms;
  mii->mii_ports = ports;

  return mii;
}

/*****************************************************************************
 *
 * Find the expression for a parameter port passed from an instance
 *
 * Parameters:
 *     mi		Instance to search
 *     name		Name of paramater
 *     ppIdx		Position of parameter
 *
 *****************************************************************************/
Expr *MIInstance_findParm(MIInstance *mi,const char *name, int ppIdx)
{
  ListElem *le;
  int iIdx = 0;

  for (le = List_first(mi->mii_parms);le;le = List_next(mi->mii_parms,le), iIdx++) {
    NameExpr *ne = (NameExpr*) ListElem_obj(le);

    if (ne->ne_name) {
      if (strcmp(ne->ne_name,name) == 0)
	return ne->ne_expr;
    } else {
      if (iIdx == ppIdx)
	return ne->ne_expr;
    }
  }

  return 0;
}

/*****************************************************************************
 *
 * Get the string for a net type keyword
 *
 *****************************************************************************/
int NT_getStr(nettype_t nt,char *buf)
{
  char *p = buf;

  if ((nt & NT_P_WIRE)) {
    /*
     * Print I/O properties
     */
    switch ((nt & NT_P_IO_MASK)) {
    case NT_P_INPUT :  p += sprintf(p,"input");  break;
    case NT_P_INOUT :  p += sprintf(p,"inout");  break;
    case NT_P_OUTPUT : p += sprintf(p,"output"); break;
    }

    /*
     * Print wire type
     */
    switch (NT_GETBASE(nt)) {
    case NT_WIRE :          if (!(nt & NT_P_IO_MASK)) p += sprintf(p,"wire");  break;
    case NT_WAND :          p += sprintf(p,"wand");            break;
    case NT_WOR :           p += sprintf(p,"wor");             break;
    case NT_TRI :           p += sprintf(p,"tri");             break;
    case NT_TRI1 :          p += sprintf(p,"tri1");            break;
    case NT_TRI0 :          p += sprintf(p,"tri0");            break;
    case NT_TRIAND :        p += sprintf(p,"triand");          break;
    case NT_TRIOR :         p += sprintf(p,"trior");           break;
    case NT_TRIREG :        p += sprintf(p,"trireg");          break;
    case NT_TRIREG_SMALL :  p += sprintf(p,"trireg (small)");  break;
    case NT_TRIREG_MEDIUM : p += sprintf(p,"trireg (medium)"); break;
    case NT_TRIREG_LARGE :  p += sprintf(p,"trireg (large)");    break;
    default :               p += sprintf(p,"illegal_net<%x>",nt);     break;
    }
  } else if ((nt & NT_P_REG)) {
    switch ((nt & (NT_P_BASE_MASK|NT_P_STRENGTH_MASK))) {
    case NT_REG :
      if ((nt & NT_P_OUTPUT))
	p += sprintf(p,"output reg");
      else
	p += sprintf(p,"reg");
      break;
    case NT_MEMORY :  p += sprintf(p,"reg");     break;
    case NT_SUPPLY0 : p += sprintf(p,"supply0");     break;
    case NT_SUPPLY1 : p += sprintf(p,"supply1");     break;

    case NT_INTEGER : p += sprintf(p,"integer");     break;
    case NT_REAL :    p += sprintf(p,"real");        break;
    case NT_TIME :    p += sprintf(p,"time");        break;
    case NT_EVENT :   p += sprintf(p,"event");       break;
    default :         p += sprintf(p,"illegal_reg<%x>",nt); break;
    }
  } else {
    p += sprintf(p,"illegal");
  }

  if ((nt & NT_P_SCALAR))
    p += sprintf(p," scalar");
  if ((nt & NT_P_VECTORED))
    p += sprintf(p," vectored");
  if ((nt & NT_P_SIGNED))
    p += sprintf(p," signed");

  if ((nt & NT_P_1_STRENGTH_MASK) && (nt & NT_P_0_STRENGTH_MASK)) {
    p += sprintf(p," (");
    switch ((nt & NT_P_0_STRENGTH_MASK)) {
    case NT_P_HIGHZ0 :  p += sprintf(p,"highz0"); break;
    case NT_P_WEAK0 :   p += sprintf(p,"weak0"); break;
    case NT_P_PULL0 :   p += sprintf(p,"pull0"); break;
    case NT_P_STRONG0 : p += sprintf(p,"strong0"); break;
    case NT_P_SUPPLY0 : p += sprintf(p,"supply0"); break;
    default : p += sprintf(p,"illegal0"); break;
    }
    p += sprintf(p,", ");

    switch ((nt & NT_P_1_STRENGTH_MASK)) {
    case NT_P_HIGHZ1 :  p += sprintf(p,"highz1"); break;
    case NT_P_WEAK1 :   p += sprintf(p,"weak1"); break;
    case NT_P_PULL1 :   p += sprintf(p,"pull1"); break;
    case NT_P_STRONG1 : p += sprintf(p,"strong1"); break;
    case NT_P_SUPPLY1 : p += sprintf(p,"supply1"); break;
    default : p += sprintf(p,"illegal1"); break;
    }
    p += sprintf(p,")");
  }
  *p = 0;

  return p - buf;
}

/*****************************************************************************
 *
 * Print the declaration from a keyword value
 *
 * Parameters:
 *     nt		Net type code value
 *     f		File stream to print on.
 *
 *****************************************************************************/
void NT_printDecl(nettype_t nt,FILE *f)
{
  char buf[STRMAX];

  NT_getStr(nt, buf);
  fprintf(f, "%s",buf);
}

/*****************************************************************************
 *
 * Print a net declaration
 *
 * Parameters:
 *     mid		Net declaration module item
 *     f		File stream to print on.
 *
 *****************************************************************************/
static void MINetDecl_print(MINetDecl *mid, FILE *f)
{
  NetDecl *n = mid->mid_decl;

  fprintf(f,"  ");
  NT_printDecl(n->n_type,f);
  fprintf(f," ");

  VRange_print(n->n_range,f);
  fprintf(f," %s",n->n_name);
  if (n->n_addrRange)
    VRange_print(n->n_addrRange,f);
  fprintf(f,";\n");

}

/*****************************************************************************
 *
 * Print an "assign" module item
 *
 * Parameters:
 *     mia		"assign" module item.
 *     f		File stream to print on.
 *
 *****************************************************************************/
static void MIAssign_print(MIAssign *mia, FILE *f)
{
  fprintf(f,"  assign ");

  if (mia->mia_bcond) {
    Expr_print(mia->mia_bcond,f);
    fprintf(f," ");
  }

  Expr_print(mia->mia_lhs,f);
  fprintf(f," = ");
  Expr_print(mia->mia_rhs,f);
  fprintf(f,";\n");
}

/*****************************************************************************
 *
 * Print a block-type module item (i.e., initial or always block).
 *
 * Parameters:
 *     mib		Block item to print.
 *     f		File stream to print on.
 *
 *****************************************************************************/
static void MIBlock_print(MIBlock *mib, FILE *f)
{
  if (mib->mib_type == IC_ALWAYS)
    fprintf(f,"  always");
  else
    fprintf(f,"  initial");

  if (mib->mib_stat->sd_type == ST_TRIGGER) {
    SDTrigger *t = (SDTrigger*) mib->mib_stat;
    fprintf(f," ");
    SDTrigger_printTrigger(t,f);
    fprintf(f,"\n");
    SDTrigger_printBody(t,f,2,0);
  } else {
    fprintf(f,"\n");
    StatDecl_print(mib->mib_stat,f,2,0);
    fprintf(f,"\n");
  }
}

/*****************************************************************************
 *
 * Print a module instance
 *
 * Parameters:
 *     mii		Module instance
 *     f		File stream to print on.
 *
 *****************************************************************************/
static void MIInstance_print(MIInstance *mii, FILE *f)
{
  ListElem *E;

  fprintf(f,"  %s",mii->mii_name);

  if (mii->mii_parms && List_numElems(mii->mii_parms) > 0) {
    int didFirst = 0;

    fprintf(f," #(");

    for (E = List_first(mii->mii_parms);E;E = List_next(mii->mii_parms,E)) {
      NameExpr *ne = (NameExpr*) ListElem_obj(E);

      if (didFirst)
	fprintf(f,", ");
      didFirst = 1;

      if (ne->ne_name) {
	fprintf(f,".%s(",ne->ne_name);
	Expr_print(ne->ne_expr,f);
	fprintf(f,")");
      } else {
	Expr_print(ne->ne_expr,f);
      }
    }
    fprintf(f,")");
  }

  fprintf(f," %s",mii->mii_instName);
  if (mii->mii_slices)
    VRange_print(mii->mii_slices, f);

  fprintf(f," (");
  if (mii->mii_ports && List_numElems(mii->mii_ports) > 0) {
    int didFirst = 0;

    for (E = List_first(mii->mii_ports);E;E = List_next(L,E)) {
      NameExpr *ne = (NameExpr*) ListElem_obj(E);

      if (didFirst)
	fprintf(f,", ");
      didFirst = 1;
    
      if (ne->ne_name) {
	fprintf(f,".%s(",ne->ne_name);
	Expr_print(ne->ne_expr,f);
	fprintf(f,")");
      } else {
	Expr_print(ne->ne_expr,f);
      }
    }
  }
  fprintf(f,");\n");
}

/*****************************************************************************
 *
 * Print a primitive gate instance
 *
 * Parameters:
 *     mig		Module instance
 *     f		File stream to print on.
 *
 *****************************************************************************/
static void MIGate_print(MIGate *mig, FILE *f)
{
  ListElem *E;

  fprintf(f,"  %s",mig->mig_desc ? mig->mig_desc->gd_name : "<unknown>");

  if (mig->mig_delay) {
    Expr_print(mig->mig_delay,f);
  }

  if (mig->mig_instName)
    fprintf(f," %s",mig->mig_instName);
  else
    fprintf(f," ");
  if (mig->mig_slices)
    VRange_print(mig->mig_slices, f);

  fprintf(f," (");
  if (mig->mig_ports && List_numElems(mig->mig_ports) > 0) {
    int didFirst = 0;

    for (E = List_first(mig->mig_ports);E;E = List_next(mig->mig_ports,E)) {
      Expr *e = (Expr*) ListElem_obj(E);

      if (didFirst)
	fprintf(f,", ");
      didFirst = 1;
    
      Expr_print(e, f);
    }
  }
  fprintf(f,");\n");
}

/*****************************************************************************
 *
 * Generate bytecode for a block module item (i.e., initial or always)
 *
 * Parameters:
 *     mib		Module item to be generated
 *     modCtx		Module instance in which we are generating
 *     codeBlock	Code block in which to store generated bytecode
 *
 *****************************************************************************/
static VGThread *MIBlock_generate(MIBlock *mib, ModuleInst *modCtx,CodeBlock *codeBlock)
{
  VGThread *thread = new_VGThread(codeBlock,CodeBlock_size(codeBlock),modCtx,(ModuleItem*)mib);
  unsigned top_bc; 

  if (ModuleItem_getDynamicModule(mib))
    DynamicModule_addThread(ModuleItem_getDynamicModule(mib), thread);

  top_bc = CodeBlock_size(codeBlock);
  StatDecl_generate(mib->mib_stat,ModuleInst_getScope(modCtx),codeBlock);

  /*
   * If this is an initial block, end the thread.  If it is an always block, go to the
   * top of the block and execute it again.
   */
  if (mib->mib_type == IC_INITIAL)
    BCEnd_init(CodeBlock_nextEmpty(codeBlock));
  else
    BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);

  return thread;
}

/*****************************************************************************
 *
 * Generate bytecode for an "assign" module item.
 *
 * Parameters:
 *     mia		Module item to be generated
 *     modCtx		Module instance in which we are generating
 *     codeBlock	Code block in which to store generated bytecode
 *
 *****************************************************************************/
static VGThread *MIAssign_generate(MIAssign *mia, ModuleInst *mi, CodeBlock *codeBlock)
{
  VGThread *thread;		/* Thread used to execute assignment */
  int lsize;			/* Base bit size of left-hand side */
  int rsize;			/* Base bit size of right-hand side */
  int size;			/* Bit size for assignment operation */
  unsigned top_bc;		/* Branch position of top of statement */
  deltatime_t delay = 0;	/* Delay for assignment */
  Trigger *t;			/* Trigger event for re-evaluation of net */
  Value *rhs_ret;		/* Return value of right-hand side */
  List lhs_list;		/* List of left-hand side nets */
  ListElem *le;			/* Iterator for left-hand-side components */
  int base_bit = 0;		/* Base bit from which to get data for left-hand side */


  /*
   * Create a thread and compute bit sizes for operation.
   */
  thread = new_VGThread(codeBlock,CodeBlock_size(codeBlock),mi,(ModuleItem*)mia);

  if (ModuleItem_getDynamicModule(mia))
    DynamicModule_addThread(ModuleItem_getDynamicModule(mia), thread);

  lsize = Expr_getBitSize(mia->mia_lhs, ModuleInst_getScope(mi));
  rsize = Expr_getBitSize(mia->mia_rhs, ModuleInst_getScope(mi));
  size = imax(lsize,rsize);

  /*
   * Test to see if getBitSize generated an error, abort generation if error was found.
   */
  if (lsize == 0 || rsize == 0)
    return 0;

  /*
   * Generate bytecode for right-hand side.
   */
  top_bc = CodeBlock_size(codeBlock);
  rhs_ret = Expr_generate(mia->mia_rhs,size,ModuleInst_getScope(mi), codeBlock);
  if (!rhs_ret) return 0;

  /*
   * Expand any left-hand-side concatenation.
   */
  List_init(&lhs_list);
  Expr_expandConcat(mia->mia_lhs, ModuleInst_getScope(mi), &lhs_list);

  /*
   * Find the delay for the assignment.
   */
  if (mia->mia_bcond) {
    if (Expr_type(mia->mia_bcond) == E_DELAY) { 
      Expr_getDelay(mia->mia_bcond, ModuleInst_getScope(mi), ModuleInst_getTimescale(mi), &delay);
    } else {
      errorFile(ModuleItem_getPlace(mia),ERR_ASGNEVENT);
      return 0;
    }
  }

  for (le = List_first(&lhs_list);le;le = List_next(&lhs_list,le)) {
    Expr *lhs_e = (Expr*) ListElem_obj(le);
    Net *n;
    Value *nLsb;
    unsigned lhs_size;
    int driver_id;

    if (Expr_lhsGenerate(lhs_e,ModuleInst_getScope(mi),codeBlock,&n,&nLsb,&lhs_size,0) < 0) {
      errorFile(ModuleItem_getPlace(mia),ERR_BADASGNLHS);
      return 0;
    }

    driver_id = Net_addDriver(n);
    BCWireAsgnD_init(CodeBlock_nextEmpty(codeBlock),n,driver_id,nLsb,rhs_ret,base_bit,lhs_size,delay);

#if 0
    printf(" assigning %s from [%d +: %d] of size %d\n",n->n_name,base_bit,lhs_size,size);
#endif

    base_bit += Expr_getBitSize(lhs_e, ModuleInst_getScope(mi));
  }

  /*
   * Triggering condition for assign block.
   */
  t = Expr_getDefaultTrigger(mia->mia_rhs, ModuleInst_getScope(mi));
  BCTrigger_init(CodeBlock_nextEmpty(codeBlock),t);
  BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);

#if ASGN_DEBUG
  printf("assign trigger: ");
  Trigger_print(t,stdout);
  printf("\n");
#endif

  return thread;
}

/*****************************************************************************
 *
 * Generator function for "assign" item when part of a path-delay module
 *
 *****************************************************************************/
int MIAssign_pathdGenerate(MIAssign *mia,ModuleInst *mi,CodeBlock *codeBlock,List *asgns)
{
  int lsize;			/* Base bit size of left-hand side */
  int rsize;			/* Base bit size of right-hand side */
  int size;			/* Bit size for assignment operation */
  unsigned top_bc;		/* Branch position of top of statement */
  deltatime_t delay = 0;	/* Delay for assignment */
  Value *rhs_ret;		/* Return value of right-hand side */
  List lhs_list;		/* List of left-hand side nets */
  ListElem *le;			/* Iterator for left-hand-side components */
  int base_bit = 0;		/* Base bit from which to get data for left-hand side */


  lsize = Expr_getBitSize(mia->mia_lhs, ModuleInst_getScope(mi));
  rsize = Expr_getBitSize(mia->mia_rhs, ModuleInst_getScope(mi));
  size = imax(lsize,rsize);

  /*
   * Test to see if getBitSize generated an error, abort generation if error was found.
   */
  if (lsize == 0 || rsize == 0)
    return 0;


#if ASGN_DEBGUG
  printf("assign trigger: ");
  Trigger_print(t,stdout);
  printf("\n");
#endif

  /*
   * Generate bytecode for right-hand side.
   */
  top_bc = CodeBlock_size(codeBlock);
  rhs_ret = Expr_generate(mia->mia_rhs,size, ModuleInst_getScope(mi), codeBlock);
  if (!rhs_ret) return 0;

  /*
   * Expand any left-hand-side concatenation.
   */
  List_init(&lhs_list);
  Expr_expandConcat(mia->mia_lhs, ModuleInst_getScope(mi), &lhs_list);

  /*
   * Find the delay for the assignment.
   */
  if (mia->mia_bcond) {
    if (Expr_type(mia->mia_bcond) == E_DELAY) { 
      Expr_getDelay(mia->mia_bcond, ModuleInst_getScope(mi), ModuleInst_getTimescale(mi), &delay);
    } else {
      errorFile(ModuleItem_getPlace(mia),ERR_ASGNEVENT);
      return 0;
    }
  }

  for (le = List_first(&lhs_list);le;le = List_next(&lhs_list,le)) {
    Expr *lhs_e = (Expr*) ListElem_obj(le);
    Net *n;
    Value *nLsb;
    unsigned lhs_size;
    int driver_id;

    if (Expr_lhsGenerate(lhs_e,ModuleInst_getScope(mi),codeBlock,&n,&nLsb,&lhs_size,0) < 0) {
      errorFile(ModuleItem_getPlace(mia),ERR_BADASGNLHS);
      return 0;
    }

    driver_id = Net_addDriver(n);
    BCAsgn_init(CodeBlock_nextEmpty(codeBlock),n,nLsb,rhs_ret,base_bit,lhs_size);

#if 0
    printf(" assigning %s from [%d +: %d] of size %d\n",n->n_name,base_bit,lhs_size,size);
#endif

    base_bit += Expr_getBitSize(lhs_e, ModuleInst_getScope(mi));
  }

  return 0;
}

/*****************************************************************************
 *
 * Generator function for single-output/multi-input gate instances
 *
 * Parameters:
 *     mig		Module item to be generated
 *     modCtx		Module instance in which we are generating
 *     codeBlock	Code block in which to store generated bytecode
 *     size		Bit size of the gate.
 *
 *****************************************************************************/
static VGThread *MIGate_generate_multin(MIGate *mig, ModuleInst *mi,CodeBlock *codeBlock,unsigned size,int nportsFixed)
{
  VGThread *thread = 0;		/* Thread for handling this gate instance */
  GateDesc *gd = mig->mig_desc;	/* Gate type descriptor */
  unsigned top_bc;		/* Branch position of top of statement */
  deltatime_t delay = 0;	/* Delay for assignment */
  Trigger *t;			/* Trigger event for re-evaluation of net */
  Value *rhs_ret;		/* Return value of right-hand side */
  List lhs_list;		/* List of left-hand side nets */
  ListElem *le;			/* List iterator */
  int base_bit = 0;		/* Base bit for output assignment */
  PHash P;			/* Hash table to get list of variables read by this gate */

  PHash_init(&P);

  /*
   * Make sure we have enough ports
   */
  if (nportsFixed) {
    if (!mig->mig_ports || List_numElems(mig->mig_ports) != gd->gd_minPorts) {
      errorFile(ModuleItem_getPlace(mig),ERR_PRIMPTCOUNT,gd->gd_name);
      return 0;
    }
  } else {
    if (!mig->mig_ports || List_numElems(mig->mig_ports) < gd->gd_minPorts) {
      errorFile(ModuleItem_getPlace(mig),ERR_PRIMPTCOUNT,gd->gd_name);
      return 0;
    }
  }

  /*
   * Save top of handler code address.
   */
  top_bc = CodeBlock_size(codeBlock);

  /*
   * Get the expression for the output port and expand it.  We are assuming the
   * first element in the ports list is the output expression.
   */
  le = List_first(mig->mig_ports);
  List_init(&lhs_list);
  Expr_expandConcat((Expr*)ListElem_obj(le), ModuleInst_getScope(mi), &lhs_list);

  /*
   * Generate first expression
   */
  le = List_next(mig->mig_ports,le);
  rhs_ret = Expr_generate((Expr*)ListElem_obj(le),size,ModuleInst_getScope(mi),codeBlock);
  if (!rhs_ret) goto abortGate;
  Expr_getReaders((Expr*)ListElem_obj(le), ModuleInst_getScope(mi), &P);

  /*
   * Combine arguments
   */
  for (le = List_next(mig->mig_ports,le);le;le = List_next(mig->mig_ports,le)) {
    Value *v1 = rhs_ret;
    Value *v2 = Expr_generate((Expr*)ListElem_obj(le),size,ModuleInst_getScope(mi),codeBlock);
    rhs_ret = new_Value(size);
    if (!v2) goto abortGate;
    Expr_getReaders((Expr*)ListElem_obj(le), ModuleInst_getScope(mi), &P);

    BCOpr_init(CodeBlock_nextEmpty(codeBlock),gd->gd_baseFunc,rhs_ret,v1,v2,0);
  }

  /*
   * Apply output function if necessary.
   */
  if (gd->gd_outFunc) {
    Value *v = rhs_ret;
    rhs_ret = new_Value(size);
    
    BCOpr_init(CodeBlock_nextEmpty(codeBlock),gd->gd_outFunc,rhs_ret,0,v,0);
  }

  /*
   * Find the delay for the assignment.
   */
  if (mig->mig_delay) {
    if (Expr_type(mig->mig_delay) == E_DELAY) { 
      Expr_getDelay(mig->mig_delay, ModuleInst_getScope(mi), ModuleInst_getTimescale(mi), &delay);
    } else {
      errorFile(ModuleItem_getPlace(mig),ERR_ASGNEVENT);
      return 0;
    }
  }

  /*
   * Set up assignments to left-hand side components.  This loop is used to
   * handle the pieces in an output-side concatentation.  If the output is
   * not a concatenation, it will only be executed once.
   */
  for (le = List_first(&lhs_list);le;le = List_next(&lhs_list,le)) {
    Expr *lhs_e = (Expr*) ListElem_obj(le);
    Net *n;
    Value *nLsb;
    unsigned lhs_size;
    int driver_id;

    if (Expr_lhsGenerate(lhs_e,ModuleInst_getScope(mi),codeBlock,&n,&nLsb,&lhs_size,0) < 0) {
      errorFile(ModuleItem_getPlace(mig),ERR_BADASGNLHS);
      goto abortGate;
    }

    driver_id = Net_addDriver(n);
    BCWireAsgnD_init(CodeBlock_nextEmpty(codeBlock),n,driver_id,nLsb,rhs_ret,base_bit,lhs_size,delay);

#if 0
    printf(" assigning %s from [%d +: %d] of size %d\n",n->n_name,base_bit,lhs_size,size);
#endif

    base_bit += Expr_getBitSize(lhs_e, ModuleInst_getScope(mi));
  }

  /*
   * Triggering condition for assign block.
   */
  t = Expr_getDefaultTriggerFromSet(&P, ModuleInst_getCircuit(mi));
  BCTrigger_init(CodeBlock_nextEmpty(codeBlock),t);
  BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);



  /*
   * Create thread starting at the top of the gate instance handler code.
   */
  thread = new_VGThread(codeBlock,top_bc,mi,(ModuleItem*)mig);

  if (ModuleItem_getDynamicModule(mig))
    DynamicModule_addThread(ModuleItem_getDynamicModule(mig), thread);

 abortGate:
  PHash_uninit(&P);

  return thread;
}

/*****************************************************************************
 *
 * Generator function for multi-output/single-input gate instances
 *
 * Parameters:
 *     mig		Module item to be generated
 *     mi		Module instance in which we are generating
 *     codeBlock	Code block in which to store generated bytecode
 *     size		Bit size of the gate.
 *
 *****************************************************************************/
static VGThread *MIGate_generate_multout(MIGate *mig, ModuleInst *mi,CodeBlock *codeBlock,unsigned size)
{
  VGThread *thread = 0;		/* Thread for handling this gate instance */
  GateDesc *gd = mig->mig_desc;	/* Gate type descriptor */
  unsigned top_bc;		/* Branch position of top of statement */
  deltatime_t delay = 0;	/* Delay for assignment */
  Trigger *t;			/* Trigger event for re-evaluation of net */
  Expr *rhs_expr = 0;		/* Return value of right-hand side (expression) */
  Value *rhs_ret;		/* Return value of right-hand side (value) */
  ListElem *le;			/* List iterator */
  int base_bit = 0;		/* Base bit for output assignment */
  PHash P;			/* Hash table to get list of variables read by this gate */

  PHash_init(&P);

  /*
   * Make sure we have enough ports
   */
  if (!mig->mig_ports || List_numElems(mig->mig_ports) < gd->gd_minPorts) {
    errorFile(ModuleItem_getPlace(mig),ERR_PRIMPTCOUNT,gd->gd_name);
    return 0;
  }

  /*
   * Save top of handler code address.
   */
  top_bc = CodeBlock_size(codeBlock);

  /*
   * Evaluate the right-hand side expression
   */
  for (le = List_first(mig->mig_ports);le;le = List_next(mig->mig_ports,le)) {
    rhs_expr = (Expr*)ListElem_obj(le);
  }
  rhs_ret = Expr_generate(rhs_expr,size,ModuleInst_getScope(mi),codeBlock);

  /*
   * Get the read varables in the input
   */
  Expr_getReaders(rhs_expr, ModuleInst_getScope(mi), &P);

  /*
   * Find the delay for the assignment.
   */
  if (mig->mig_delay) {
    if (Expr_type(mig->mig_delay) == E_DELAY) { 
      Expr_getDelay(mig->mig_delay, ModuleInst_getScope(mi), ModuleInst_getTimescale(mi), &delay);
    } else {
      errorFile(ModuleItem_getPlace(mig),ERR_ASGNEVENT);
      return 0;
    }
  }

  /*
   * Apply output function if necessary.
   */
  if (gd->gd_outFunc) {
    Value *v = rhs_ret;
    rhs_ret = new_Value(size);
    
    BCOpr_init(CodeBlock_nextEmpty(codeBlock),gd->gd_outFunc,rhs_ret,0,v,0);
  }

  /*
   * Generate expression for each output
   */
  for (le = List_first(mig->mig_ports);le;le = List_next(mig->mig_ports,le)) {
    Expr *lhs_expr;
    List lhs_list;		/* List of left-hand side nets */
    ListElem *le2;

    if (!List_next(mig->mig_ports,le)) break;

    lhs_expr = (Expr*)ListElem_obj(le);

    /*
     * Get the expression for the output port and expand it.  We are assuming the
     * first element in the ports list is the output expression.
     */
    List_init(&lhs_list);
    Expr_expandConcat(lhs_expr, ModuleInst_getScope(mi), &lhs_list);

    /*
     * Set up assignments to left-hand side components.  This loop is used to
     * handle the pieces in an output-side concatentation.  If the output is
     * not a concatenation, it will only be executed once.
     */
    for (le2 = List_first(&lhs_list);le2;le2 = List_next(&lhs_list,le2)) {
      Expr *lhs_e = (Expr*) ListElem_obj(le2);
      Net *n;
      Value *nLsb;
      unsigned lhs_size;
      int driver_id;

      if (Expr_lhsGenerate(lhs_e,ModuleInst_getScope(mi),codeBlock,&n,&nLsb,&lhs_size,0) < 0) {
	errorFile(ModuleItem_getPlace(mig),ERR_BADASGNLHS);
	goto abortGate;
      }

      driver_id = Net_addDriver(n);
      BCWireAsgnD_init(CodeBlock_nextEmpty(codeBlock),n,driver_id,nLsb,rhs_ret,base_bit,lhs_size,delay);

#if 0
      printf(" assigning %s from [%d +: %d] of size %d\n",n->n_name,base_bit,lhs_size,size);
#endif

      base_bit += Expr_getBitSize(lhs_e, ModuleInst_getScope(mi));
    }

    List_uninit(&lhs_list);
  }

  /*
   * Triggering condition for assign block.
   */
  t = Expr_getDefaultTriggerFromSet(&P, ModuleInst_getCircuit(mi));
  BCTrigger_init(CodeBlock_nextEmpty(codeBlock),t);
  BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);



  /*
   * Create thread starting at the top of the gate instance handler code.
   */
  thread = new_VGThread(codeBlock,top_bc,mi,(ModuleItem*)mig);

  if (ModuleItem_getDynamicModule(mig))
    DynamicModule_addThread(ModuleItem_getDynamicModule(mig), thread);

 abortGate:
  PHash_uninit(&P);

  return thread;
}

/*****************************************************************************
 *
 * Generate bytecode for a primitive gate instance module item.
 *
 * Parameters:
 *     mig		Module item to be generated
 *     mi		Module instance in which we are generating
 *     codeBlock	Code block in which to store generated bytecode
 *
 *****************************************************************************/
static VGThread *MIGate_generate(MIGate *mig, ModuleInst *mi,CodeBlock *codeBlock)
{
  VGThread *thread = 0;		/* Thread used to handle changes in gate inputs */
  unsigned size;		/* Bit size of gate assignment */

  if (VRange_getSize(mig->mig_slices,ModuleInst_getScope(mi),&size) < 0) {
    errorFile(ModuleItem_getPlace(mig),ERR_BADGATERNG);
    return 0;
  }

#if ASGN_DEBGUG
  printf("MIGate_generate %s\n",mig->mig_desc->gd_name);
#endif

  if (mig->mig_desc) {
    switch (mig->mig_desc->gd_gac) {
    case GAC_MULTIN :
      thread = MIGate_generate_multin(mig,mi,codeBlock,size,0);
      break;
    case GAC_MULTOUT :
      thread = MIGate_generate_multout(mig,mi,codeBlock,size);
      break;
    case GAC_FIXED :
      thread = MIGate_generate_multin(mig,mi,codeBlock,size,1);
      break;
    }
  }

  return thread;
}

int MIGate_pathdGenerate(MIGate *mig,ModuleInst *mi,CodeBlock *codeBlock,List *asgns)
{
  return 0;
}


/*****************************************************************************
 *
 * Generate simulator byte code for a module item.
 *
 * Parameters:
 *     mib		Module item to be generated
 *     mi		Module instance in which we are generating
 *     codeBlock	Code block in which to store generated bytecode
 *
 *****************************************************************************/
VGThread *ModuleItem_generate(ModuleItem *mib,ModuleInst *mi,CodeBlock *codeBlock)
{
  if (mib->mi_base.mb_vtable->miv_generate) {
    Place_setCurrent(ModuleItem_getPlace(mib));
    return (*mib->mi_base.mb_vtable->miv_generate)(mib, mi, codeBlock);
  } else
    return 0;
}

/*****************************************************************************
 *
 * Print a module item using the appropriate function
 *
 * Parameters:
 *     mi		Module item
 *     f		File stream to print on.
 *
 *****************************************************************************/
void ModuleItem_print(ModuleItem *mi,FILE *f)
{
  (*mi->mi_base.mb_vtable->miv_print)(mi, f);
}
