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

int yyparse();
void BeginVR();
#ifndef yyrestart
void yyrestart(FILE*);
#endif

#define DEBUG 0

Place		curPlace;			/* The current place */


/*****************************************************************************
 *
 * "current" object structure used by parser
 *
 *****************************************************************************/
struct {
  ModuleDecl	*mod;			/* Module currently being defined */
  int		dtype;			/* Current type for declarations */
  VRange	*range;			/* Current range for declarations */
  char		*instType;		/* Type name of current instances */
  List		modParms;		/* List of module parameters */
  unsigned	gtype;			/* Current primitive gate type */
  Expr		*gdelay;		/* Current delay specification */
  DynamicModule	*dynmod;		/* Dynamic module */
  UserTaskDecl	*task;			/* User task declaration */
  ScopeDecl	*scope;			/* Scope in which to make variable declarations */
  int		isRedef;		/* This module is a redefinition (i.e., in error). */
  const char	*esName;		/* Name of embedded script  */
} cur = {0};


/*****************************************************************************
 *
 * Initialize the parser "current" state to begin parsing the specified file.
 *
 * Parameters:
 *      fileName		Name of file we are parsing.
 *
 * This function is called before parsing an input file.  The 'fileName' specified
 * is used for error reporting.
 *
 *****************************************************************************/
static void cur_init(const char *fileName)
{
  static int is_first_init = 1;

  flushErrors();

  if (is_first_init) {
    List_init(&cur.modParms);
    is_first_init = 0;
  } else {
    if (cur.instType) free(cur.instType);
    List_flush(&cur.modParms);
  }

  cur.mod = 0;
  cur.isRedef = 0;
  cur.scope = 0;
  cur.task = 0;
  cur.range = 0;
  cur.instType = 0;
  Place_init(&curPlace, fileName);
}

static int cur_getDeclContext()
{
  if (cur.task)
    return TASK;
  else
    return MODULE;
}


/*****************************************************************************
 *
 * Read in a verilog script file.
 *
 * Parameters:
 *     fileName			File name to read
 *     dm			Dynamic module object to use
 *
 * Dynamic module are modules that are read and parsed at run-time. 
 *
 *****************************************************************************/
int VerilogLoadScript(const char *fileName,DynamicModule *dm)
{
  FILE *f;
  extern char *current_script;
  extern int errCount;

  if (!(f = openInPath(fileName))) {
    return -1;
  }

  cur.dynmod = dm;
  cur.isRedef = 0;
  current_script = dm->dm_name;

  flushErrors();
  cur_init(fileName);
  yc_setup();
  BeginSF();
  yyrestart(f);
  yc_pushpool();
  VerBeginScript();
  yyparse();
  VerEndScript();
  yc_poppool();
  fclose(f);

  cur.dynmod = 0;
  cur.isRedef = 0;
  current_script = 0;

  return (errCount == 0) ? 0 : -1; 
}

/*****************************************************************************
 *
 * Read in a verilog file.
 *
 * Parameters:
 *      name			Name of file to read.
 *
 *****************************************************************************/
int VerilogLoad(const char *name)
{
  FILE *f;

  if (!(f = openInPath(name))) {
    return -1;
  }

  cur_init(name);

  yc_setup();
  BeginPV();
  yyrestart(f);
  yc_pushpool();
  yyparse();
  yc_poppool();
  fclose(f);

  return 0;
}

/*****************************************************************************
 *
 * Start a new module declaration.
 *
 * Parameters:
 *      name		Name of module to declare.
 *
 *
 *****************************************************************************/
void VerNewModule(const char *name)
{
  Place *p = Place_getCurrent();

  cur.isRedef = 0;

  if (VGSim_findModule(&vgsim, name)) {
    errorFile(&curPlace,ERR_MODREDEF,name);
    cur.isRedef = 1;
  }

#if DEBUG
  printf("VerNewModule(%s)\n",name);
#endif
  cur.mod = new_ModuleDecl(name);
  cur.scope = ModuleDecl_getScope(cur.mod);
  Place_startModule(p, name);
  Place_copy(&cur.mod->m_place,p);

  if (strcmp(p->p_moduleName,name) != 0) {
    errorModule(cur.mod,p,ERR_WRONGMOD,name,p->p_moduleName);
  }
}

/*****************************************************************************
 *
 * End a module declaration
 *
 *****************************************************************************/
void VerEndModule()
{
#if DEBUG
  printf("VerEndModule()\n");
#endif

  if (!cur.isRedef)
    VGSim_addModule(&vgsim, cur.mod);
  cur.mod = 0;
  cur.isRedef = 0;
  cur.scope = 0;

  Place_endModule(Place_getCurrent());
}

/*****************************************************************************
 *
 * Begin a group of net/wire declarations.
 *
 * Parameters:
 *     dtype		Type for subsequent declarations.
 *     range		Range for subsequent declarations.
 *
 *****************************************************************************/
void VerBeginDecls(int dtype,VRange *range)
{
#if DEBUG
  printf("VerBeginDecls(%d, ",dtype);
  VRange_print(range, stdout);
  printf(")\n");
#endif

  /*
   * If this is a task or function, make sure we are using allowed types.
   */
  if (cur.task) {
    if (!(dtype & NT_P_REG)) {
      errorModule(cur.mod,Place_getCurrent(),ERR_TASKBADTYPE);
    }

    if (UserTaskDecl_getType(cur.task) == UTT_FUNCTION) {
      if ((dtype & NT_P_IO_MASK) == NT_P_OUTPUT || (dtype & NT_P_IO_MASK) == NT_P_INOUT) {
	errorModule(cur.mod,Place_getCurrent(),ERR_TASKBADPORT);
      }
    }
  }

  switch ((dtype & NT_P_REGTYPE_MASK)) {
  case NT_P_INTEGER :
    dtype |= NT_P_SIGNED;
    range = new_VRange(RS_MAXMIN,new_Expr_num(SSWORDSIZE-1),new_Expr_num(0));
    break;
  case NT_P_TIME :
    range = new_VRange(RS_MAXMIN,new_Expr_num(63),new_Expr_num(0));
    break;
  case NT_P_EVENT :
    range = new_VRange(RS_MAXMIN,new_Expr_num(0),new_Expr_num(0));
    break;
  default :
    break;
  }

  if (!range) range = new_VRange(RS_MAXMIN,new_Expr_num(SSWORDSIZE-1),new_Expr_num(0));

  cur.dtype = dtype;
  cur.range = range;
}

/******************************************************************************
 *
 * Declare a variable in the current context with the current type 
 *
 * Parameters:
 *     name		Name of the variable
 *     addrRange	Address range of variable if this is a memory.
 *
 * This function is called for each variable name in a variable declaration.  It
 * is assumed that VerBeginDecls() has already been called to set the type and
 * bit range to be used for each subsequent call to VerDecl().  The variables
 * declared can be in a module, a named block, a task or in a function depending
 * on context.
 *
 *****************************************************************************/
void VerDecl(const char *name,VRange *addrRange)
{
  NetDecl *n;
  nettype_t ntype = cur.dtype;

#if DEBUG
  printf("VerDecl(%s, %p)\n",name,addrRange);
#endif

  /*
   *
   */
  n = ScopeDecl_findNet(cur.scope,name,SDF_LOCAL_ONLY);
  if (n) {
    /*
     * A net can be declared as both "output" and "reg", check for
     * that case here.  For all other cases this is a redeclaration
     * error.
     */
    if ( (NT_GETIO(n->n_type) == NT_P_OUTPUT)
    	 && (NT_GETBASE(n->n_type) == NT_WIRE) 
         && (NT_GETBASE(ntype) == NT_REG)) {

      if(addrRange)
	errorModule(cur.mod,Place_getCurrent(),ERR_BADADDR,name);

      n->n_type &= ~NT_P_WIRE;
      n->n_type |= NT_P_REG;
      return;
    }

    errorModule(cur.mod,Place_getCurrent(),ERR_NETREDEF,name);
    return;
  }

  if(addrRange){
    if ( ((NT_GETIO(ntype) != 0) || (ntype & NT_WIRE)) && addrRange) {
      errorModule(cur.mod,Place_getCurrent(),ERR_BADADDRSPEC,name);
    } else
      ntype |= NT_P_MEMORY;
  }

  n = new_NetDecl(name,ntype,cur.range,addrRange,Place_getCurrent());

  switch (cur_getDeclContext()) {
  case MODULE :
    ModuleDecl_defNet(cur.mod, n);
    break;
  case TASK :
  default :
    ScopeDecl_defNet(cur.scope, n);
    if ((ntype & NT_P_IO_MASK))
      UserTaskDecl_addParm(cur.task,n);
    break;
  }
}


/*****************************************************************************
 *
 * Declare a verilog 'assign' module item.
 *
 * Parameter:
 *      lval		Left-hand side (target) of assignment
 *      rval		Right-hand side of assignment
 *      econd		Condition on assignment (usually a delay expression)
 *
 *****************************************************************************/
void VerAssign(Expr *lval,Expr *rval,Expr *econd)
{
  MIAssign *a = new_MIAssign(lval,rval,econd);

  if (cur.dynmod)
    DynamicModule_addMItem(cur.dynmod, (ModuleItem*)a);

  List_addToTail(&cur.mod->m_items,a);

#if DEBUG
  printf("VerAssign: ");
  Expr_print(lval,stdout);
  printf(" = ");
  Expr_print(rval,stdout);
  printf(";");
  if (econd) {
    printf(" ec=");
    Expr_print(econd,stdout);
  }
  printf("\n");
#endif
}

/*****************************************************************************
 *
 * An assign used as part of a declaration
 *
 *****************************************************************************/
void VerDeclAssign(Expr *lval,Expr *rval,Expr *econd)
{
  if ((cur.dtype & NT_P_REG) && !econd) {
    MIAssign *a = new_MIAssign(lval,rval,econd);
    a->mia_common.mb_type = IC_REGINIT;		/* Change type code to register init */

    if (cur.dynmod)
      DynamicModule_addMItem(cur.dynmod, (ModuleItem*)a);

    List_addToTail(&cur.mod->m_items,a);
  } else {
    VerAssign(lval, rval, econd);
  }
}


/*****************************************************************************
 *
 * Declare a verilog 'assign' module item when it is part of a declaration.
 *
 * Parameter:
 *      dtype		Type of declaration
 *      lval		Left-hand side (target) of assign/declaration
 *      rval		Right-hand side of assign
 *
 *****************************************************************************/
void VerAutoAssign(int dtype,const char *lval,Expr *rval)
{
  MIAssign *a;

  if (dtype != NT_WIRE) {
    errorModule(cur.mod,Place_getCurrent(),ERR_BADAUTORNG);
  }

  VerBeginDecls(dtype,VerRange(RS_AUTO,rval,0));
  VerDecl(lval,0);

  a = new_MIAssign(new_Expr_lit(lval),rval,0);
  List_addToTail(&cur.mod->m_items,a);

  if (cur.dynmod)
    DynamicModule_addMItem(cur.dynmod, (ModuleItem*)a);
}

/*****************************************************************************
 *
 * Create a range object from the left and right expression
 *
 * Parameters:
 *      rs			Type of range expression
 *      left			Left-hand expression in range
 *      right			Right-hand expression in range
 *
 *****************************************************************************/
VRange *VerRange(rangestyle_t rs, Expr *left,Expr *right)
{
  return new_VRange(rs, left, right);
}

/*****************************************************************************
 *
 * Add a port to the current module
 *
 * Parameters:
 *      name			Name of the port to add
 *
 *****************************************************************************/
void VerPort(const char *name)
{
#if DEBUG
  printf("VerPort(%s)\n",name);
#endif
  ModuleDecl_addPort(cur.mod,name);
}

/*****************************************************************************
 *
 * Add a parameter to the current module
 *
 * Parameters:
 *      name		Name of the parameter to add
 *      e		Expression for value of the parameter
 *      isPort		Non-zero if this is a parameter port
 *
 *****************************************************************************/
void VerParmDef(const char *name,Expr *e, int isPort)
{
#if DEBUG
  printf("VerParmDef(%s, %p)\n",name, e);
#endif
  ModuleDecl_defineParm(cur.mod, name,e, isPort);
}

/*****************************************************************************
 *
 * Declare "always" or "initial" block in the current module
 *
 * Parameters:
 *      btype		Type (ALWAYS or INITIAL) of the block.
 *      stat		Statement to be executed in the block.
 *
 *****************************************************************************/
void VerIABlock(int btype,StatDecl *stat)
{
  MIBlock *b;

  if (btype == ALWAYS)
    b = new_MIBlock(IC_ALWAYS,stat);
  else
    b = new_MIBlock(IC_INITIAL,stat);

  List_addToTail(&cur.mod->m_items,b);

  if (cur.dynmod)
    DynamicModule_addMItem(cur.dynmod, (ModuleItem*)b);
}

/*****************************************************************************
 *
 * Create an empty list object
 *
 *****************************************************************************/
List *VerEmptyList()
{
  List *L = (List*) malloc(sizeof(List));

  List_init(L);

  return L;
}

/*****************************************************************************
 *
 * Append an item to the end of a list
 *
 * Parameters:
 *      L		List to which to append an item
 *      vi		Item to be appended to the list
 *
 * Returns:		List object passed as parameter L.
 *
 *****************************************************************************/
List *VerListAppend(List *L,void *vi)
{
  if (!L) L = VerEmptyList();
  List_addToTail(L,vi);
  return L;
}

/*****************************************************************************
 *
 * Prepend an item to the begining of a list
 *
 * Parameters:
 *      L		List to which to prepend an item
 *      vi		Item to be prepended to the list
 *
 * Returns:		List object passed as parameter L.
 *
 *****************************************************************************/
List *VerListPrepend(void *vi,List *L)
{
  if (!L) L = VerEmptyList();
  List_addToHead(L,vi);
  return L;
}

/*****************************************************************************
 *
 * Create a conditioned (delay or event trigger) statement 
 *
 * Parameters:
 *      e		Condition expression
 *      s		Statement to be conditioned
 *
 * Returns:		New statement with condition added.
 *
 *****************************************************************************/
StatDecl *VerCondStat(Expr *e,StatDecl *s)
{
  if (e->e_type == E_AT) {
    return new_SDTrigger(e->e.opr[1],s);
  } else if (e->e_type == E_DELAY) {
    return new_SDDelay(e,s);
  } else {
    return s;
  }
}

/*****************************************************************************
 *
 * Set the module type for instance creation.
 *
 * Parameters:
 *      name		Name of module 
 *
 *****************************************************************************/
void VerModDecl(const char *name)
{
  if (cur.instType) free(cur.instType);
  cur.instType = strdup(name);
  List_flush(&cur.modParms);
}

/*****************************************************************************
 *
 * Set the parameters to use for instances.
 *
 * Parameters:
 *      name		Name of parameter
 *      e		Value of parameter
 *
 *****************************************************************************/
void VerModDeclParm(const char *name, Expr *e)
{
  List_addToTail(&cur.modParms,new_NameExpr(name,e));
}

/*****************************************************************************
 *
 * Create an instance of a module
 *
 * Parameters:
 *      name		Name of instance
 *      range		Range to create multiple instance
 *	ports		Ports on the instance.
 *
 *****************************************************************************/
void VerModInst(const char *name, VRange *range, List *ports)
{
  MIInstance *mii;

  mii = new_MIInstance(cur.instType, name, range, copy_List(&cur.modParms), ports);

  List_addToTail(&cur.mod->m_items,mii);

  if (cur.dynmod)
    DynamicModule_addMItem(cur.dynmod, (ModuleItem*)mii);

}

/*****************************************************************************
 *
 * Create an instance of a module
 *
 * Parameters:
 *      n		ID number of breakpoint
 *	e		Expression of breakpoint
 *
 *****************************************************************************/
void VerBreakpoint(int n,Expr *e)
{
}

/*****************************************************************************
 *
 * Begin declaration of a script
 *
 *****************************************************************************/
void VerBeginScript()
{
  cur.mod = new_ModuleDecl(0);
  cur.isRedef = 0;
  cur.mod->m_timescale = *ModuleInst_getTimescale(Circuit_getRoot(&vgsim.vg_circuit));
  cur.scope = ModuleDecl_getScope(cur.mod);
}

/*****************************************************************************
 *
 * End declaration of a script
 *
 *****************************************************************************/
void VerEndScript()
{
  extern int errCount;

  if (errCount == 0)
    Circuit_installScript(&vgsim.vg_circuit,cur.mod,cur.dynmod);
  cur.mod = 0;
  cur.isRedef = 0;
  cur.scope = 0;
}

/*****************************************************************************
 *
 * Declare a timing task (e.g., $setup) used in a specify block.
 *
 * Parameters:
 *      name		Name of the task
 *      args		Arguments to pass to the task.
 *
 *****************************************************************************/
void VerSpecTask(const char *name,List *args)
{
  Specify *s;

  ModuleDecl_makeSpecify(cur.mod);
  s = ModuleDecl_getSpecify(cur.mod);

  Specify_addTask(s, (StatDecl*) new_SDTask(name,args));
}

/*****************************************************************************
 *
 * Path delay specification 
 *
 * Parameters:
 *      cond		Condition under which path delay applies
 *      ptype		Type of path delay ("<*" vs. "<=")
 *      lhs		Left-hand-side port declaration
 *      rhs		Right-hand-side port declaration
 *      value		Delay value for this path
 *
 *****************************************************************************/
void VerSpecify(Expr *cond,int ptype,Expr *lhs,Expr *rhs,Expr *value)
{
  Specify *s;
  SpecifyStat *specstat = new_SpecifyStat(cond,ptype, lhs, rhs, value);

  ModuleDecl_makeSpecify(cur.mod);
  s = ModuleDecl_getSpecify(cur.mod);

  Specify_addStat(ModuleDecl_getSpecify(cur.mod), specstat);
}

/*****************************************************************************
 *
 * Declare a specparam
 *
 * Parameters:
 *      name		Name of the specparam
 *      value		Value of the specparam
 *
 *****************************************************************************/
void VerSpecParam(const char *name,Expr *value)
{
  Specify *s;

  ModuleDecl_makeSpecify(cur.mod);
  s = ModuleDecl_getSpecify(cur.mod);
  Specify_addSpecParam(s, name, value);
}


/*****************************************************************************
 *
 * Set the type for declaring built-in gates ("and", "or", etc.)
 *
 * Parameters:
 *      gtype		Type code 
 *      delay		Delay expression
 *
 *****************************************************************************/
void VerGateDecl(int gtype, Expr *delay)
{
  cur.gtype = gtype;
  cur.gdelay = delay;
}

/*****************************************************************************
 *
 * Create an instance of a built-in gate
 *
 * Parameters:
 *      iname		Instance name
 *      range		Range to use
 *      ports		Port list for gate
 *
 *****************************************************************************/
void VerGateInst(const char *iname, VRange *range, List *ports)
{
  MIGate *mig;

  mig = new_MIGate(cur.gtype, cur.gdelay, iname, range, ports);

  List_addToTail(&cur.mod->m_items,mig);

  if (cur.dynmod)
    DynamicModule_addMItem(cur.dynmod, (ModuleItem*)mig);
}

/*****************************************************************************
 *
 * Process a special comment tag
 *
 * Parameters:
 *      tag		Text of tag ro be processed
 *
 * Most tkgate special commentsw are ignored.  The exception are special comments
 * used to indicate what type of module is being declared.  The primary effect
 * on verga is to do additional consistency checks if the tags exist and to
 * slightly modify the way errors are reported.
 * 
 *****************************************************************************/
void VerSpecialTag(const char *tag)
{
  char name[STRMAX];

  if (sscanf(tag,"//: /netlistBegin %s",name) == 1) {
  } else if (sscanf(tag,"//: /hdlBegin %s",name) == 1) {
  } else if (sscanf(tag,"//: /builtinBegin %s",name) == 1) {
  } else if (strcmp(tag,"//: /netlistEnd") == 0
	     || strcmp(tag,"//: /hdlEnd") == 0
	     || strcmp(tag,"//: /buitinEnd") == 0) {
    *name = 0;
  } else if (strcmp(tag,"//: enddecls") == 0) {
    Place_resetModLine(Place_getCurrent());
    return;
  } else {
    return;	/* ignore */
  }

  if (*name)
    Place_startMTag(Place_getCurrent(),name);
  else
    Place_endMTag(Place_getCurrent());
}

/*****************************************************************************
 *
 * Convert a task to a function.
 *
 *****************************************************************************/
void VerTaskToFunc(VRange *range)
{
  if (cur.task) {
    cur.task->utd_type = UTT_FUNCTION;
    UserTaskDecl_setRange(cur.task, range);
  }
}

/*****************************************************************************
 *
 * Begin declaration of a user-defined task
 *
 * Parameters:
 *      name		Name of the task
 *      isauto		Non-zero if this is an "automatic" function/task
 *
 *****************************************************************************/
void VerBeginTask(const char *name,int isauto)
{
  ModuleDecl *m = cur.mod;

  if (!m) {
    errorModule(m,Place_getCurrent(),ERR_IE_TASK,name);
    return;
  }

  if (ModuleDecl_findTask(m,name)) {
    errorModule(m,Place_getCurrent(),ERR_TASKREDEF,name,ModuleDecl_getName(cur.mod));
    return;
  }

  cur.task = new_UserTaskDecl(name,m,UTT_TASK,isauto);
  cur.scope = UserTaskDecl_getScope(cur.task);

  ModuleDecl_defineTask(m,name,cur.task);
}

/*****************************************************************************
 *
 * End a user-defined task declaration
 *
 * Parameters:
 *     body		Body of the task
 *
 *****************************************************************************/
void VerEndTask(StatDecl *body)
{
  if (!cur.mod || !cur.task) {
    return;
  }

  UserTaskDecl_setStat(cur.task,body);

  cur.task = 0;
  cur.scope = 0;
}

void VerBeginEScript(const char *name)
{
  cur.esName = name;
  vgio_printf("embeddedscript begin %s %d\n",cur.esName,curPlace.p_lineNo);
}

void VerEndEScript()
{
  vgio_printf("embeddedscript end %s %d\n",cur.esName,curPlace.p_lineNo-2);
  cur.esName = 0;
}

