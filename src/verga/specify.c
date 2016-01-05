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

    Last edit by hansen on Thu Jan 29 14:53:50 2009
****************************************************************************/
#include "thyme.h"

#define SPECIFY_DEBUG 0

/*****************************************************************************
 *
 * Create a specify block for a module.
 *
 * Parameters:
 *      m		Module declaration in which specify block is used
 *
 * Returns:		Returns the specify block object.
 *
 *****************************************************************************/
Specify *new_Specify(ModuleDecl *m)
{
  Specify *s = (Specify*) malloc(sizeof(Specify));

  Specify_init(s,m);

  return s;
}

/*****************************************************************************
 *
 * Initialize a specify block for a module.
 *
 * Parameters:
 *      s		Specify block to be initialized
 *      m		Module declaration in which specify block is used
 *
 *****************************************************************************/
void Specify_init(Specify *s,ModuleDecl *m)
{
  s->s_flags = 0;
  s->s_module = m;
  List_init(&s->s_tasks);
  List_init(&s->s_stats);
  SHash_init(&s->s_specParms);
}

/*****************************************************************************
 *
 * Delete a specify block
 *
 * Parameters:
 *      s		Specify block to be deleted.
 *
 *****************************************************************************/
void delete_Specify(Specify *s)
{
  Specify_uninit(s);
  free(s);
}

/*****************************************************************************
 *
 * Uninitialize a specify block
 *
 * Parameters:
 *      s		Specify block to be uninitialize.
 *
 *****************************************************************************/
void Specify_uninit(Specify *s)
{
  List_uninit(&s->s_tasks);
  List_uninit(&s->s_stats);
}

/*****************************************************************************
 *
 * Add a path delay specification to a specify block.
 *
 * Parameters:
 *      s		Specify block to add spec to.
 *      k		Path delay object
 *
 *****************************************************************************/
void Specify_addStat(Specify *s,SpecifyStat *k)
{
  List_addToTail(&s->s_stats,k);
}

/*****************************************************************************
 *
 * Add a specify task to a specify block.
 *
 * Parameters:
 *      s		Specify block to add spec to.
 *      t		Specify task to be added
 *
 *****************************************************************************/
void Specify_addTask(Specify *s,StatDecl *t)
{
  List_addToTail(&s->s_tasks,t);
}

/*****************************************************************************
 *
 * Generate a call to a specific task for a specific trigger
 *
 * Parameters:
 *      s		Specify block for this module
 *      mi		Module instance
 *      codeBlock	Code block in which to write bytecode
 *      index		Index of tigger parameter to use
 *      which		Code for which trigger to use
 *      task		Specify task that we are generating
 *      taskCtx		Task context to use for task calls.
 *      negateEdge	Non-zero if the edge event should be negated
 *
 * This function is called to generate bytecode for timing check system tasks
 * that appear in specify blocks.  Such tasks include $setup, $hold and $width.
 * This generate function is called once with "which=-1" to generate initialization
 * code for the timing check, and once with "which" equal to the parameter index
 * for each index that references a triggering event.
 *
 *****************************************************************************/
static void Specify_generateTaskCall(Specify *s,ModuleInst *mi,CodeBlock *codeBlock,int index,int which,
				     SDTask *task,TaskContext *taskCtx,int negateEdge,Scope *scope)
{
  VGThread *thread = new_VGThread(codeBlock,CodeBlock_size(codeBlock),mi,0);
  const char *task_name = task->t_name;
  SysTaskDescript *taskEnt = SysTask_findEnt(task_name);
  unsigned top_bc = CodeBlock_size(codeBlock);
  int nargs = 0;
  void **sargs = 0;
  int i,j;

  /*
   * Register this thread
   */  
  List_addToTail(&mi->mc_threads,thread);


  if (which < 0) {
#if SPECIFY_DEBUG
    printf("initial %s(",task_name);
#endif
  } else {
    Expr *trigger = task->t_args[index];
    Expr *cond = 0;

    if (Expr_type(trigger) == E_COND) {
      cond = trigger->e.opr[1];
      trigger = trigger->e.opr[0];
    }

    /*
     * exchange posedge and negedge on a trigger expression.  Generate error if
     * no edge specifier was given.
     */
    if (negateEdge) {
      if (Expr_type(trigger) == E_POSEDGE) {
	trigger->e_type = E_NEGEDGE;
	Expr_generateTrigger(trigger, ModuleInst_getScope(mi), codeBlock, 0);
	trigger->e_type = E_POSEDGE;
      } else if (Expr_type(trigger) == E_NEGEDGE) {
	trigger->e_type = E_POSEDGE;
	Expr_generateTrigger(trigger, ModuleInst_getScope(mi), codeBlock, 0);
	trigger->e_type = E_NEGEDGE;
      } else {
	errorFile(StatDecl_getPlace(task),ERR_NEEDEDGE,task->t_name);
      }
    } else
      Expr_generateTrigger(trigger, ModuleInst_getScope(mi), codeBlock, 0);

    if (cond) {
      Value *ret_val = Expr_generate(cond,1,ModuleInst_getScope(mi),codeBlock);
      BCGoto_init(CodeBlock_nextEmpty(codeBlock),ret_val,1,codeBlock,top_bc);
    }

#if SPECIFY_DEBUG
    printf("always @(");
    Expr_print(trigger,stdout);
    printf(")");
    if (cond) {
      printf(" if (");
      Expr_print(cond,stdout);
      printf(")");
    }
    printf(" %s(",task_name);
#endif
  }

  nargs = task->t_nargs + 1;
  sargs = (void**) malloc(sizeof(void*)*nargs);

  sargs[0] = new_Value(SSWORDSIZE);
  Value_convertI(sargs[0],which);

  for (i = 0, j = 1;i < task->t_nargs;i++, j++) {
    stargtype_t argtype = taskEnt->st_argTypes[i];

    switch (argtype) {
    case TAT_NET :
      if (Expr_type(task->t_args[i]) == E_LITERAL) {
	const char *name = Expr_getLitName(task->t_args[i]);
	Net *n = ModuleInst_findNet(mi, name);

	if (!n) {
	  errorFile(StatDecl_getPlace(task),ERR_NOTDEF,name);
	  return;
	}

	sargs[j] = n;
      } else {
	errorFile(StatDecl_getPlace(task),ERR_NEEDIDENT,i+1,task->t_name);
	return;
      }
      break;
    case TAT_TRIGGERPAIR :
    case TAT_TRIGGER :
      sargs[j] = task->t_args[i];
      break;
    case TAT_VALUE :
    default :
      /*      sargs[j] = Expr_generateS(task->t_args[i], ModuleInst_getScope(mi), codeBlock);*/
      sargs[j] = Expr_generateS(task->t_args[i], scope, codeBlock);
      break;
    }
  }

  BCTask_init(CodeBlock_nextEmpty(codeBlock), taskEnt->st_func, taskCtx, 0, nargs, sargs);

  if (which < 0) {
    BCEnd_init(CodeBlock_nextEmpty(codeBlock));
  } else {
    BCGoto_init(CodeBlock_nextEmpty(codeBlock),0,0,codeBlock,top_bc);
  }

}

/*****************************************************************************
 *
 * Generate code to implement specify tasks.
 *
 * Parameters:
 *      s		Specify block to use 
 *      mi		Module instance to use
 *      codeBlock	Codeblock in which to write byte code
 *
 *****************************************************************************/
void Specify_generateTasks(Specify *s,ModuleInst *mi,CodeBlock *codeBlock)
{
  int which;
  ListElem *le;
  Scope *scope;
  HashElem *he;

  /*
   * Create a scope for the specify block and define any specparams in it.
   */
  scope = new_Scope(ModuleInst_getPath(mi),ModuleInst_getScope(mi),mi);
  for (he = Hash_first(&s->s_specParms);he;he = Hash_next(&s->s_specParms,he)) {
    const char *name = SHashElem_key(he);
    Expr *e = (Expr*)HashElem_obj(he);
    Value *value = Expr_parmEval(e, scope, 0);
    Net *n = new_Net(name, NT_PARAMETER, Value_nbits(value)-1,0);
    Scope_defNet(scope, name, n);
  }


  s->s_flags |= SPF_ACTIVE;

  for (le = List_first(&s->s_tasks);le; le = List_next(&s->s_tasks,le)) {
    SDTask *task = (SDTask*) ListElem_obj(le);
    const char *task_name = task->t_name;
    SysTaskDescript *taskEnt = SysTask_findEnt(task_name);
    TaskContext *taskCtx = 0;
    unsigned top_bc,end_bc;
    int i;

    if (!taskEnt) {
      errorFile(StatDecl_getPlace(task),ERR_NOTASK,task_name);
      delete_Scope(scope);
      return;
    }

    /*
     * Make sure the argument count is within limits
     */
    if (task->t_nargs < taskEnt->st_minArgs || task->t_nargs > taskEnt->st_maxArgs) {
      errorFile(StatDecl_getPlace(task),ERR_TASKARGS,task->t_name);
      return;
    }


    taskCtx = new_TaskContext(task->t_nargs,task->t_args, mi);

    top_bc = CodeBlock_size(codeBlock);
    Specify_generateTaskCall(s,mi,codeBlock,-1,-1,task,taskCtx,0,scope);

    which = 0; 
    for (i = 0;i < task->t_nargs;i++) {
      if (taskEnt->st_argTypes[i] == TAT_TRIGGER) {
	Specify_generateTaskCall(s,mi,codeBlock,i,which++,task,taskCtx,0,scope);
      } else if (taskEnt->st_argTypes[i] == TAT_TRIGGERPAIR) {
	Specify_generateTaskCall(s,mi,codeBlock,i,which++,task,taskCtx,0,scope);
	Specify_generateTaskCall(s,mi,codeBlock,i,which++,task,taskCtx,1,scope);
      }
    }
    end_bc = CodeBlock_size(codeBlock);

    TaskContext_setBlock(taskCtx,codeBlock,mi,top_bc,end_bc);
  }

  s->s_flags &= ~SPF_ACTIVE;
  delete_Scope(scope);
}

/*****************************************************************************
 *
 * Add a specparam declaration to a specify block
 *
 * Parameters:
 *      s		Specify block in which to define parameter
 *      name		Name of parameter to define
 *      value		Expression for value of parameter
 *
 * Returns:		Non-zero on failure (redefinition).
 *
 *****************************************************************************/
int Specify_addSpecParam(Specify *s,const char *name,Expr *value)
{
  Expr *x = (Expr*) SHash_find(&s->s_specParms,name);
  if (x) return -1;

  SHash_insert(&s->s_specParms,name,value);

  return 0;
}

/*****************************************************************************
 *
 * Get a specify parameter value
 *
 * Parameters:
 *      s		Specify block in which to define parameter
 *      name		Name of parameter to lookup
 *      mi		Module instance to use for variable lookup
 *      value		Value of evaluated expression
 *
 * Returns:		Non-zero on failure (undefined).
 *
 *****************************************************************************/
int Specify_getSpecParm(Specify *s,const char *name,ModuleInst *mi, int *value)
{
  Expr *x = (Expr*)SHash_find(&s->s_specParms,name);
  if (!x) return -1;

  return Expr_parmEvalI(x, ModuleInst_getScope(mi), value, PEF_SPECPARM);
}

/*****************************************************************************
 *
 * Create a path delay object
 *
 * Parameters:
 *      cond		Condition for path delay
 *      ptype		Type of path (=> vs *>)
 *      lhs		Input port list
 *      rhs		Output port list
 *      delay		Delay expression
 *
 * Returns:		New path delay object
 *
 *****************************************************************************/
SpecifyStat *new_SpecifyStat(Expr *cond,spectype_t ptype,Expr *lhs,Expr *rhs,Expr *delay)
{
  SpecifyStat *ss = (SpecifyStat*) malloc(sizeof(SpecifyStat));
  ListElem *le;
  int lit_only = 1;
  static int direct_conn = 0;

  if (ptype == SPT_DPATH) {
    if (!direct_conn) {
      errorFile(Place_getCurrent(),WRN_DIRECTCONN);
      direct_conn = 1;
    }
  }

  ss->ss_cond = cond;
  ss->ss_ptype = ptype;
  List_init(&ss->ss_from);
  List_init(&ss->ss_to);
  ss->ss_delay = delay;

  Expr_expandConcat(lhs, 0, &ss->ss_from);
  Expr_expandConcat(rhs, 0, &ss->ss_to);

  for (le = List_first(&ss->ss_from);le;le = List_next(&ss->ss_from, le)) {
    Expr *e = (Expr*)ListElem_obj(le);
    if (Expr_type(e) != E_LITERAL)
      lit_only = 0;
  }
  for (le = List_first(&ss->ss_to);le;le = List_next(&ss->ss_to, le)) {
    Expr *e = (Expr*)ListElem_obj(le);
    if (Expr_type(e) != E_LITERAL)
      lit_only = 0;
  }

  if (!lit_only)
    errorFile(Place_getCurrent(),ERR_BADSPECLVAL);

#if SPECIFY_DEBUG
  printf("specify");
  if (cond){
    printf("if (");
    Expr_print(cond,stdout);
    printf(")");
  }
  printf(" (");
  for (le = List_first(&ss->ss_from);le;le = List_next(&ss->ss_from, le)) {
    if (le != List_first(&ss->ss_from))
      printf(", ");
    Expr_print((Expr*)ListElem_obj(le), stdout);
  }
  printf(") *> (");
  for (le = List_first(&ss->ss_to);le;le = List_next(&ss->ss_to, le)) {
    if (le != List_first(&ss->ss_to))
      printf(", ");
    Expr_print((Expr*)ListElem_obj(le), stdout);
  }
  printf(") = ");
  Expr_print(delay,stdout);
  printf("\n");
#endif

  return ss;
}

/*****************************************************************************
 *
 * Get the delays that match a given source/destination pair
 *
 * 
 *
 *****************************************************************************/
int Specify_getDelays(Specify *s,const char *src,const char *dst,List/*SpecifyStat**/ *match)
{
  ListElem *le,*le2;

  for (le = List_first(&s->s_stats);le;le = List_next(&s->s_stats,le)) {
    SpecifyStat *ss = (SpecifyStat*) ListElem_obj(le);
    int in_match = 0, out_match = 0;

    for (le2 = List_first(&ss->ss_from);le2;le2 = List_next(&ss->ss_from,le2)) {
      Expr *e = (Expr*) ListElem_obj(le2);
      if (strcmp(Expr_getLitName(e),src) == 0) {
	in_match = 1;
	break;
      }
    }
    if (!in_match) continue;

    for (le2 = List_first(&ss->ss_to);le2;le2 = List_next(&ss->ss_to,le2)) {
      Expr *e = (Expr*) ListElem_obj(le2);
      if (strcmp(Expr_getLitName(e),dst) == 0) {
	out_match = 1;
	break;
      }
    }
    if (!out_match) continue;

    List_addToTail(match, ss);
  }

  return 0;
}

