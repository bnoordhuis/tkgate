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
/*****************************************************************************
 *
 * This file containes methods for handling statements represented by
 * the primary class StatDecl.  Statements are anything that can go
 * in a "always" or "initial" item in a module.
 *
 *****************************************************************************/
#include "thyme.h"

static StatDeclVTable vtable_SDNull = { 
  (StatPrint_f*) SDNull_print,
  (StatGenerate_f*) SDNull_generate,
  (StatGetReaders_f*) SDNull_getReaders,
};
static StatDeclVTable vtable_SDDelay = {
  (StatPrint_f*) SDDelay_print,
  (StatGenerate_f*) SDDelay_generate,
  (StatGetReaders_f*) SDDelay_getReaders,
};
static StatDeclVTable vtable_SDTrigger = {
  (StatPrint_f*) SDTrigger_print,
  (StatGenerate_f*) SDTrigger_generate,
  (StatGetReaders_f*) SDTrigger_getReaders,
};
static StatDeclVTable vtable_SDWait = {
  (StatPrint_f*) SDWait_print,
  (StatGenerate_f*) SDWait_generate,
  (StatGetReaders_f*) SDWait_getReaders,
};
static StatDeclVTable vtable_SDRaise = {
  (StatPrint_f*) SDRaise_print,
  (StatGenerate_f*) SDRaise_generate,
  (StatGetReaders_f*) SDRaise_getReaders,
};
static StatDeclVTable vtable_SDAsgn = {
  (StatPrint_f*) SDAsgn_print,
  (StatGenerate_f*) SDAsgn_generate,
  (StatGetReaders_f*) SDAsgn_getReaders,
};
static StatDeclVTable vtable_SDBlock = { 
  (StatPrint_f*) SDBlock_print,
  (StatGenerate_f*) SDBlock_generate,
  (StatGetReaders_f*) SDBlock_getReaders,
};
static StatDeclVTable vtable_SDIf = { 
  (StatPrint_f*) SDIf_print,
  (StatGenerate_f*) SDIf_generate,
  (StatGetReaders_f*) SDIf_getReaders,
};
static StatDeclVTable vtable_SDWhile = { 
  (StatPrint_f*) SDWhile_print,
  (StatGenerate_f*) SDWhile_generate,
  (StatGetReaders_f*) SDWhile_getReaders,
};
static StatDeclVTable vtable_SDForever = { 
  (StatPrint_f*) SDForever_print,
  (StatGenerate_f*) SDForever_generate,
  (StatGetReaders_f*) SDForever_getReaders,
};
static StatDeclVTable vtable_SDFor = { 
  (StatPrint_f*) SDFor_print,
  (StatGenerate_f*) SDFor_generate,
  (StatGetReaders_f*) SDFor_getReaders,
};
static StatDeclVTable vtable_SDCase = { 
  (StatPrint_f*) SDCase_print,
  (StatGenerate_f*) SDCase_generate,
  (StatGetReaders_f*) SDCase_getReaders,
};
static StatDeclVTable vtable_SDFork = {
  (StatPrint_f*) SDFork_print,
  (StatGenerate_f*) SDFork_generate,
  (StatGetReaders_f*) SDFork_getReaders,
};
static StatDeclVTable vtable_SDRepeat = { 
  (StatPrint_f*) SDRepeat_print,
  (StatGenerate_f*) SDRepeat_generate,
  (StatGetReaders_f*) SDRepeat_getReaders,
};
static StatDeclVTable vtable_SDTask = { 
  (StatPrint_f*) SDTask_print,
  (StatGenerate_f*) SDTask_generate,
  (StatGetReaders_f*) SDTask_getReaders,
};

/*****************************************************************************
 *
 * Array of vtable for statement declarations.  The order must match the value
 * of the ST_ type codes for statements.
 *
 *****************************************************************************/
static StatDeclVTable *SDvtables[] = {
  &vtable_SDNull,
  &vtable_SDBlock,
  &vtable_SDAsgn,
  &vtable_SDIf,
  &vtable_SDWhile,
  &vtable_SDCase,
  &vtable_SDFork,
  &vtable_SDDelay,
  &vtable_SDTrigger,
  &vtable_SDRepeat,
  &vtable_SDFor,
  &vtable_SDTask,
  &vtable_SDWait,
  &vtable_SDForever,
  &vtable_SDRaise,
};

static StatDecl *new_StatDecl(int stype)
{
  StatDecl *sd = (StatDecl*) malloc(sizeof(StatDecl));

  sd->sd_common.sdc_type = stype;
  sd->sd_common.sdc_vtable = SDvtables[stype];
  Place_copy(&sd->sd_common.sdc_place, Place_getCurrent());

  return sd;
}


StatDecl *new_SDDelay(Expr *delay,StatDecl *stat)
{
  SDDelay *sdd = (SDDelay*) new_StatDecl(ST_DELAY);

  sdd->d_delay = delay;
  sdd->d_stat = stat;

  return (StatDecl*) sdd;
}

StatDecl *new_SDTrigger(Expr *trigger,StatDecl *stat)
{
  SDTrigger *sdt = (SDTrigger*) new_StatDecl(ST_TRIGGER);

  sdt->t_stat = stat;
  sdt->t_trigger = trigger;

  return (StatDecl*) sdt;
}

StatDecl *new_SDWait(Expr *cond,StatDecl *stat)
{
  SDWait *sdw = (SDWait*) new_StatDecl(ST_WAIT);

  sdw->w_stat = stat;
  sdw->w_cond = cond;

  return (StatDecl*) sdw;
}

StatDecl *new_SDRaise(Expr *event)
{
  SDRaise *srw = (SDRaise*) new_StatDecl(ST_RAISE);

  srw->r_event = event;

  return (StatDecl*) srw;
}

StatDecl *new_SDAsgn(Expr *lhs, Expr *rhs,Expr *bcond,int block)
{
  SDAsgn *sda = (SDAsgn*) new_StatDecl(ST_ASGN);
  
  sda->a_lhs = lhs;
  sda->a_rhs = rhs;
  sda->a_bcond = bcond;
  sda->a_block = block;

  return (StatDecl*) sda;
}

StatDecl *new_SDBlock(const char *name,List *stats)
{
  SDBlock *sdb = (SDBlock*) new_StatDecl(ST_BLOCK);

  sdb->b_name = name ? strdup(name) : 0;
  sdb->b_stats = stats;

  return (StatDecl*) sdb;
}

StatDecl *new_SDIf(Expr *cond,StatDecl *then_body,StatDecl *else_body)
{
  SDIf *sdi = (SDIf*) new_StatDecl(ST_IF);

  sdi->i_cond = cond;
  sdi->i_then = then_body;
  sdi->i_else = else_body;

  return (StatDecl*) sdi;
}

StatDecl *new_SDWhile(Expr *cond,StatDecl *body)
{
  SDWhile *sdw = (SDWhile*) new_StatDecl(ST_WHILE);

  sdw->w_cond = cond;
  sdw->w_body = body;

  return (StatDecl*) sdw;
}

StatDecl *new_SDForever(StatDecl *body)
{
  SDForever *sdf = (SDForever*) new_StatDecl(ST_FOREVER);

  sdf->f_body = body;

  return (StatDecl*) sdf;
}

StatDecl *new_SDFor(StatDecl *init,Expr *cond,StatDecl *next,StatDecl *body)
{
  SDFor *sdf = (SDFor*) new_StatDecl(ST_FOR);

  sdf->f_init = init;
  sdf->f_cond = cond;
  sdf->f_next = next;
  sdf->f_body = body;

  return (StatDecl*) sdf;
}

StatDecl *new_SDCase(int caseType,Expr *sel,List *cents)
{
  SDCase *sdc = (SDCase*) new_StatDecl(ST_CASE);

  sdc->c_caseType = caseType;
  sdc->c_select = sel;
  sdc->c_caseEnts = cents;

  return (StatDecl*) sdc;
}


StatDecl *new_SDFork(List *stats)
{
  SDFork *sdf = (SDFork*) new_StatDecl(ST_FORK);

  sdf->f_stats = stats;

  return (StatDecl*) sdf;
}

StatDecl *new_SDRepeat(Expr *count,StatDecl *body)
{
  SDRepeat *sdr = (SDRepeat*) new_StatDecl(ST_REPEAT);

  sdr->r_count = count;
  sdr->r_body = body;

  return (StatDecl*) sdr;
}

StatDecl *new_SDTask(const char *name, List *args)
{
  SDTask *sdt = (SDTask*) new_StatDecl(ST_TASK);

  sdt->t_name = strdup(name);
  sdt->t_nargs = args ? List_numElems(args) : 0;
  if (sdt->t_nargs > 0) {
    int i = 0;
    ListElem *le;

    sdt->t_args = (Expr**) malloc(sdt->t_nargs*sizeof(Expr*));
    for (le = List_first(args);le;le = List_next(args,le)) {
      sdt->t_args[i++] = (Expr*) ListElem_obj(le);
    }
  } else
    sdt->t_args = 0;

  return (StatDecl*) sdt;
}

StatDecl *new_SDNull()
{
  return new_StatDecl(ST_NULL);
}

SDCaseEnt *new_SDCaseEnt(Expr *cond,StatDecl *stat)
{
  SDCaseEnt *ce = (SDCaseEnt *) malloc(sizeof(SDCaseEnt));

  ce->ce_cond = cond;
  ce->ce_stat = stat;

  return ce;
}


void SDDelay_print(SDDelay *sdd, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  Expr_print(sdd->d_delay,f);
  fprintf(f," ");
  if (sdd->d_stat)
    StatDecl_print(sdd->d_stat,f,indent,PF_NOINDENT);
}

void SDTrigger_printTrigger(SDTrigger *sdt, FILE *f)
{
  if (sdt->t_trigger) {
    ListElem *E;
    int notfirst = 0;
    List triggerList;

    List_init(&triggerList);
    Expr_makeTriggerList(sdt->t_trigger,&triggerList);

    fprintf(f,"@(");
    for (E = List_first(&triggerList);E;E = List_next(&triggerList,E)) {
      Expr *trigger = (Expr *) ListElem_obj(E);

      if (notfirst)
	fprintf(f," or ");
      notfirst = 1;

      Expr_print(trigger,f);
    }
    fprintf(f,") ");

    List_uninit(&triggerList);
  } else
    fprintf(f,"@(*) ");
}

void SDTrigger_printBody(SDTrigger *sdt, FILE *f, int indent, unsigned flags)
{
  StatDecl_print(sdt->t_stat,f,indent,flags);
}

void SDTrigger_print(SDTrigger *sdt, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  SDTrigger_printTrigger(sdt,f);
  SDTrigger_printBody(sdt,f,indent,PF_NOINDENT);
}

void SDWait_print(SDWait *sdw, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  fprintf(f,"wait(");
  Expr_print(sdw->w_cond,f);
  fprintf(f,") ");
  StatDecl_print(sdw->w_stat,f,indent,PF_NOINDENT);
}

void SDRaise_print(SDRaise *srw, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  fprintf(f,"-> ");
  Expr_print(srw->r_event,f);
}

void SDAsgn_print(SDAsgn *sda, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  Expr_print(sda->a_lhs,f);
  if (sda->a_block)
    fprintf(f," = ");
  else
    fprintf(f," <= ");

  if (sda->a_bcond) {
    Expr_print(sda->a_bcond,f);
    fprintf(f," ");
  }

  Expr_print(sda->a_rhs,f);
  if (!(flags & PF_NOSEMI))
    fprintf(f,";");
}

void SDBlock_print(SDBlock *sdb, FILE *f, int indent, unsigned flags)
{
  ListElem *E;

  if ((flags & PF_BLOCKNL)) {
    fprintf(f,"\n");
    printIndent(indent,f);
  } else if (!(flags & PF_NOINDENT))
    printIndent(indent,f);


  fprintf(f,"begin\n");

  for (E = List_first(sdb->b_stats);E;E = List_next(sdb->b_stats,E)) {
    StatDecl *subStat = (StatDecl*) ListElem_obj(E);
    StatDecl_print(subStat,f,indent+1,0);
    fprintf(f,"\n");
  }

  printIndent(indent,f);
  fprintf(f,"end");
}

void SDIf_print(SDIf *sdi, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);
  fprintf(f,"if (");
  Expr_print(sdi->i_cond,f);
  fprintf(f,")\n");
  StatDecl_print(sdi->i_then,f,indent+1,0);
  fprintf(f,"\n");

  if (sdi->i_else) {
    printIndent(indent,f);
    fprintf(f,"else\n");
    StatDecl_print(sdi->i_else,f,indent+1,0);
  }
}

void SDWhile_print(SDWhile *sdw, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);
  fprintf(f,"while (");
  Expr_print(sdw->w_cond,f);
  fprintf(f,")\n");
  StatDecl_print(sdw->w_body,f,indent+1,0);
}

void SDForever_print(SDForever *sdf, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);
  fprintf(f,"forever");
  StatDecl_print(sdf->f_body,f,indent+1,0);
}

void SDFor_print(SDFor *sdf, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  fprintf(f,"for (");
  StatDecl_print(sdf->f_init,f,0,PF_NOSEMI);
  fprintf(f,";");
  Expr_print(sdf->f_cond,f);
  fprintf(f,";");
  StatDecl_print(sdf->f_next,f,0,PF_NOSEMI);
  fprintf(f,")\n");
  StatDecl_print(sdf->f_body,f,indent+1,0);
}

void SDCase_print(SDCase *sdc, FILE *f, int indent, unsigned flags)
{
  ListElem *E;

  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  fprintf(f,"%s (",ycGetKeyword(sdc->c_caseType));
  Expr_print(sdc->c_select,f);
  fprintf(f,")\n");

  for (E = List_first(sdc->c_caseEnts);E;E = List_next(sdc->c_caseEnts,E)) {
    SDCaseEnt *ce = (SDCaseEnt*) ListElem_obj(E);

    printIndent(indent+1,f);
    if (ce->ce_cond)
      Expr_print(ce->ce_cond,f);
    else
      fprintf(f,"default");
    fprintf(f,": ");
    StatDecl_print(ce->ce_stat,f,indent+2,PF_NOINDENT|PF_BLOCKNL);
    fprintf(f,"\n");
  }

}

void SDFork_print(SDFork *sdf, FILE *f, int indent, unsigned flags)
{
  ListElem *le;

  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);
  fprintf(f,"fork\n");

  for (le = List_first(sdf->f_stats);le;le = List_next(sdf->f_stats,le)) {
    StatDecl *subStat = (StatDecl*) ListElem_obj(le);
    StatDecl_print(subStat,f,indent+1,0);
    fprintf(f,"\n");
  }

  printIndent(indent,f);
  fprintf(f,"join");

}

void SDRepeat_print(SDRepeat *sdr, FILE *f, int indent, unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);
  fprintf(f,"repeat (");
  Expr_print(sdr->r_count,f);
  fprintf(f,")\n");
  StatDecl_print(sdr->r_body,f,indent+1,0);
}

void SDTask_print(SDTask *t, FILE *f, int indent,unsigned flags)
{
  int i;

  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);

  fprintf(f,"%s(",t->t_name);
  for (i = 0;i < t->t_nargs;i++) {
    if (i > 0) fprintf(f,", ");
    Expr_print(t->t_args[i],f);
  }
  fprintf(f,");");
}


void SDNull_print(StatDecl *sd, FILE *f, int indent,unsigned flags)
{
  if (!(flags & PF_NOINDENT))
    printIndent(indent,f);
  fprintf(f,";");
}


void SDNull_generate(StatDecl *sd, Scope *scope, CodeBlock *cb)
{
  BCNoop_init(CodeBlock_nextEmpty(cb));
}

static void SDTask_generateUserTask(SDTask *t,Scope *scope,UserTask *ut,CodeBlock *cb)
{
  ListElem *le;
  unsigned top_bc, end_bc;
  void **sargs;
  int i;

  if (UserTask_getType(ut) != UTT_TASK) {
    errorFile(StatDecl_getPlace(t),ERR_FUNCASTASK,t->t_name);
    return;
  }

  /*
   * Make sure number of arguments passed matches number expected.
   */
  if (t->t_nargs != List_numElems(&ut->ut_decl->utd_parms)) {
    errorFile(StatDecl_getPlace(t),ERR_TASKARGS,t->t_name);
    return;
  }

  /*
   * Remember the top of the code we are generating 
   */
  top_bc = CodeBlock_size(cb);
  if (t->t_nargs)
    sargs = (void**) malloc(sizeof(void*)*t->t_nargs);

  /*
   * Pass arguments to task
   */
  for (i = 0, le = List_first(&ut->ut_decl->utd_parms);i < t->t_nargs;
       i++, le = List_next(&ut->ut_decl->utd_parms,le)) {
    NetDecl *netdecl = (NetDecl*) ListElem_obj(le);

    switch ((NetDecl_getType(netdecl) & NT_P_IO_MASK)) {
    case NT_P_INPUT :
      sargs[i] = Expr_generateS(t->t_args[i], scope, cb);
      break;
    case NT_P_OUTPUT :
    case NT_P_INOUT :
      if (Expr_type(t->t_args[i]) == E_LITERAL) {
	const char *name = Expr_getLitName(t->t_args[i]);
	Net *n = Scope_findNet(scope, name, 0);

	if (!n) {
	  errorFile(StatDecl_getPlace(t),ERR_NOTDEF,name);
	  return;
	}
	if (!(Net_getType(n) & NT_P_REG)) {
	  errorFile(StatDecl_getPlace(t),ERR_LHSNOTREG,Net_getLocalName(n));
	}

	sargs[i] = n;
      } else {
	errorFile(StatDecl_getPlace(t),ERR_NEEDIDENT,i+1,t->t_name);
	sargs[i] = 0;
	return;
      }
      break;
    }
  }
  end_bc = CodeBlock_size(cb);

  if (UserTask_isAuto(ut))
    UserTask_generateInlineCall(ut,sargs,cb);
  else
    UserTask_generateCall(ut,sargs,cb);

  if (t->t_nargs)
    free(sargs);
}

/*****************************************************************************
 *
 * Generate bytecode for a system task call.
 *
 * Parameters:
 *      t		Task call statement
 *      scope		Scope for variable lookup
 *      cb		Codeblock to use
 *      taskEnt		System task descriptor
 *
 *****************************************************************************/
void SDTask_generateSysTask(SDTask *t, Scope *scope, CodeBlock *cb,SysTaskDescript *taskEnt)
{
  int i;
  void **sargs = 0;
  unsigned top_bc, end_bc;
  TaskContext *taskCtx = 0;
  ModuleInst *mi = Scope_getModuleInst(scope);
  
  /*
   * Make sure only 'specify' tasks are used in specify blocks and only non-specify
   * tasks are used outside specify blocks.
   */
  if (ModuleDecl_getSpecify(mi->mc_mod) &&
      (Specify_getFlags(ModuleDecl_getSpecify(mi->mc_mod)) & SPF_ACTIVE)) {
    if (!(taskEnt->st_flags & STF_SPECIFY)) {
      /* Non-specify task used in specify block */
      errorFile(StatDecl_getPlace(t),ERR_BADSPECTASK,t->t_name);
      return;
    }
  } else {
      /* Specify task used outside specify block */
    if ((taskEnt->st_flags & STF_SPECIFY)) {
      errorFile(StatDecl_getPlace(t),ERR_SPECTASKUSG,t->t_name);
      return;
    }
  }

  /*
   * Make sure the argument count is within limits
   */
  if (t->t_nargs < taskEnt->st_minArgs || t->t_nargs > taskEnt->st_maxArgs) {
    errorFile(StatDecl_getPlace(t),ERR_TASKARGS,t->t_name);
    return;
  }

  /*
   * Generate a task context if needed
   */
  if ((taskEnt->st_flags & STF_NEEDCTX)) {
    if ((taskEnt->st_flags & STF_NEEDNETS))
      taskCtx = new_TaskContext(t->t_nargs,t->t_args, mi);
    else
      taskCtx = new_TaskContext(0,0,0);
  }

  /*
   * Remember the top of the code we are generating 
   */
  top_bc = CodeBlock_size(cb);
  if (t->t_nargs)
    sargs = (void**) malloc(sizeof(void*)*t->t_nargs);

  /*
   * Pass arguments to task
   */
  for (i = 0;i < t->t_nargs;i++) {
    stargtype_t argtype;

    if (i < STASK_MAXSPECARGS)
      argtype = taskEnt->st_argTypes[i];
    else
      argtype = TAT_VALUE;

    switch (argtype) {
    case TAT_NET :
      if (Expr_type(t->t_args[i]) == E_LITERAL) {
	const char *name = Expr_getLitName(t->t_args[i]);
	Net *n = Scope_findNet(scope, name, 0);

	if (!n) {
	  errorFile(StatDecl_getPlace(t),ERR_NOTDEF,name);
	  return;
	}

	sargs[i] = n;
      } else {
	errorFile(StatDecl_getPlace(t),ERR_NEEDIDENT,i+1,t->t_name);
	return;
      }
      break;
    case TAT_TRIGGER :
      break;
    case TAT_VALUE :
    default :
      sargs[i] = Expr_generateS(t->t_args[i], scope, cb);
      break;
    }
  }
  end_bc = CodeBlock_size(cb);

  /*
   * Generate the actual call to the task.
   */
  BCTask_init(CodeBlock_nextEmpty(cb),taskEnt->st_func,taskCtx,0,t->t_nargs,sargs);

  /*
   * If we needed a task context, set the block for the context.
   */
  if (taskCtx) {
    TaskContext_setBlock(taskCtx,cb, mi,top_bc,end_bc);
  }
}

void SDTask_generate(SDTask *t, Scope *scope, CodeBlock *cb)
{
  SysTaskDescript *taskEnt;
  UserTask *ut;

  if ((taskEnt = SysTask_findEnt(t->t_name)))
    SDTask_generateSysTask(t,scope,cb,taskEnt);
  else if ((ut = ModuleInst_findTask(Scope_getModuleInst(scope),t->t_name)))
    SDTask_generateUserTask(t,scope,ut,cb);
  else {
    errorFile(StatDecl_getPlace(t),ERR_NOTASK,t->t_name);
    return;
  }
}


void SDDelay_generate(SDDelay *sd, Scope *scope, CodeBlock *cb)
{
  if (Expr_generateDelay(sd->d_delay, scope, cb) < 0)
    return;

  if (sd->d_stat)
    StatDecl_generate(sd->d_stat, scope,cb);
}

/*****************************************************************************
 *
 * Generate byte code for a trigger statement
 *
 * Parameters:
 *     sd		Trigger statement declaration
 *     scope		Scope to use for variable lookup
 *     cb		Codeblock in which to generate byte code. 
 *
 *****************************************************************************/
void SDTrigger_generate(SDTrigger *sdt, Scope *scope, CodeBlock *cb)
{
  if (Expr_generateTrigger(sdt->t_trigger, scope, cb, sdt->t_stat) < 0)
    return;

  if (sdt->t_stat)
    StatDecl_generate(sdt->t_stat, scope, cb);

}

/*****************************************************************************
 *
 * Generate byte code for a wait statement
 *
 * Parameters:
 *     sd		Wait statement declaration
 *     scope		Scope to use for variable lookup
 *     cb		Codeblock in which to generate byte code. 
 *
 * Generates code equivalent to:
 *
 *    while (!cond) @(*cond*);
 *    stat; 
 *
 * where @(*cond*) is the trigger list for variables used in the condition.
 *
 *****************************************************************************/
void SDWait_generate(SDWait *sdw, Scope *scope, CodeBlock *cb)
{
  unsigned top_bc,branch_bc,end_bc;
  Value *ret_val;
  Trigger *trigger;
  PHash H;

  PHash_init(&H);

  top_bc = CodeBlock_size(cb);
  ret_val = Expr_generateS(sdw->w_cond,scope, cb);
  branch_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),ret_val,0,cb,0);

  trigger = Expr_getDefaultTrigger(sdw->w_cond, scope);
  BCTrigger_init(CodeBlock_nextEmpty(cb),trigger);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,top_bc);
  end_bc = CodeBlock_size(cb);

  if (sdw->w_stat)
    StatDecl_generate(sdw->w_stat, scope, cb);

  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,branch_bc),end_bc);

  PHash_uninit(&H);
}

/*****************************************************************************
 *
 * Generate byte code for a raise statement
 *
 * Parameters:
 *     sd		Wait statement declaration
 *     scope		Scope to use for variable lookup
 *     cb		Codeblock in which to generate byte code. 
 *
 * Generates code for:
 *
 *   -> named_event;
 *
 * where "named_event" is a named event that can be used in a trigger list
 * elsewhere in the verilog description.
 *
 *****************************************************************************/
void SDRaise_generate(SDRaise *srw, Scope *scope, CodeBlock *cb)
{
  const char *name = Expr_getLitName(srw->r_event);
  Net *net =  Scope_findNet(scope, name, 0);

  BCRaise_init(CodeBlock_nextEmpty(cb),net);
}


/*****************************************************************************
 *
 * Generate single part of a left-hand-side assignment for a reg
 *
 * Parameters:
 *     sd		Assignment statement
 *     scope		Scope to use for variable lookup
 *     cb		Code block to use
 *     n		Net we are assigning
 *     nLsb		Value of lsb in n to assign to.
 *     r		Value to be assigned.
 *     base_bit		Base bit in r from which to extract assignment.
 *     lhs_size		Number of bits we are assigning.
 *
 *****************************************************************************/
static void SDAsgn_generateNetAsgnPiece(SDAsgn *sd, Scope *scope, CodeBlock *cb, 
					Net *n,Value *nLsb,Value *r,int base_bit, int lhs_size)
{
    if (sd->a_block) {
      /*
       * This is a blocking assignment.  If an assignment condition or delay is
       * given generate it now followed by the net assignemnt.
       */
      if (sd->a_bcond)
	Expr_generateBCond(sd->a_bcond, scope, cb, (StatDecl*)sd);
      BCAsgn_init(CodeBlock_nextEmpty(cb),n,nLsb,r,base_bit,lhs_size);
    } else {
      if (!sd->a_bcond) {
	/*
	 * This is a non-blocking assignment with no blocking condition on the
	 * assignment.
	 */
	BCNbAsgnD_init(CodeBlock_nextEmpty(cb),n,nLsb,r,base_bit,lhs_size,0);
      } else {
	/*
	 * This is a non-blocking assignment with a blocking condition on the
	 * assignment.  Figure out what the blocking condition for the assigment
	 * is and generate the appropriate enqueue instruction.
	 */
	if (Expr_type(sd->a_bcond) == E_AT) {
	  Trigger *t = Expr_getTrigger(sd->a_bcond->e.opr[1], scope, (StatDecl*)sd);
	  if (t)
	    BCNbAsgnE_init(CodeBlock_nextEmpty(cb),n,nLsb,r,base_bit,lhs_size,t);
	} else if (Expr_type(sd->a_bcond) == E_DELAY) { 
	  Timescale *ts = ModuleInst_getTimescale(CodeBlock_getModuleInst(cb));
	  deltatime_t delay;

	  if (Expr_getDelay(sd->a_bcond, scope, ts, &delay) == 0)
	    BCNbAsgnD_init(CodeBlock_nextEmpty(cb),n,nLsb,r,base_bit,lhs_size,delay);
	}
      }
    }
}

/*****************************************************************************
 *
 * Generate single part of a left-hand-side assignment for a memory
 *
 * Parameters:
 *     sd		Assignment statement
 *     scope		Scope to use for variable lookup
 *     cb		Code block to use
 *     n		Memory we are assigning
 *     nAddr		Address we are assigning to.
 *     nLsb		Value of lsb in n to assign to.
 *     r		Value to be assigned.
 *     base_bit		Base bit in r from which to extract assignment.
 *     lhs_size		Number of bits we are assigning.
 *
 *****************************************************************************/
static void SDAsgn_generateMemAsgnPiece(SDAsgn *sd, Scope *scope, CodeBlock *cb, 
					Net *n,Value *nAddr,Value *nLsb,Value *r,int base_bit, int lhs_size)
{
    if (sd->a_block) {
      /*
       * This is a blocking assignment.  If an assignment condition or delay is
       * given generate it now followed by the net assignemnt.
       */
      if (sd->a_bcond)
	Expr_generateBCond(sd->a_bcond, scope, cb, (StatDecl*)sd);
      BCMemPut_init(CodeBlock_nextEmpty(cb),n,nAddr, nLsb, r, base_bit, lhs_size);
    } else {
      if (!sd->a_bcond) {
	BCNbMemPutD_init(CodeBlock_nextEmpty(cb),n,nAddr, nLsb, r, base_bit, lhs_size,0);
      } else {
	switch (Expr_type(sd->a_bcond)) {
	case E_AT :
	  {
	    Trigger *trigger = Expr_getTrigger(sd->a_bcond->e.opr[1], scope, (StatDecl*)sd);
	    if (trigger)
	      BCNbMemPutE_init(CodeBlock_nextEmpty(cb),n,nAddr, nLsb, r, base_bit, lhs_size, trigger);
	  }
	  break;
	case E_DELAY :
	  {
	    Timescale *ts = ModuleInst_getTimescale(CodeBlock_getModuleInst(cb));
	    deltatime_t delay;

	    if (Expr_getDelay(sd->a_bcond, scope, ts, &delay) == 0)
	      BCNbMemPutD_init(CodeBlock_nextEmpty(cb),n,nAddr, nLsb, r, base_bit, lhs_size, delay);
	  }
	  break;
	}
      }
    }
}


/*****************************************************************************
 *
 * Generate byte code for an assignment
 *
 * Parameters:
 *     sd		Trigger statement declaration
 *     scope		Scope to use for variable lookup
 *     cb		Codeblock in which to generate byte code. 
 *
 *****************************************************************************/
void SDAsgn_generate(SDAsgn *sd, Scope *scope, CodeBlock *cb)
{
  int lsize = Expr_getBitSize(sd->a_lhs, scope);
  int rsize = Expr_getBitSize(sd->a_rhs, scope);
  int size = imax(lsize,rsize);
  Value *r;
  List lhs_list;
  ListElem *le;
  int base_bit = 0;

  if (lsize <= 0 || rsize <= 0) return;			/* Expression error, don't generate anything */
  /*
   * Generate expression, abort generation if we found an error. 
   */
  r = Expr_generate(sd->a_rhs, size, scope, cb);
  if (!r) return;

  /*
   * Expand any left-hand-side concatenation.
   */
  List_init(&lhs_list);
  Expr_expandConcat(sd->a_lhs, scope, &lhs_list);

  /*
   * If the output value is directly associated with a net, we need to copy it instead
   * of using it directly.  This will allow statments such as:
   *
   *  wire [7;0] a = 8'h42;
   *  {a[3:0],a[7:4]} = a;
   *
   * to work correctly.
   */
  if ((Value_getAllFlags(r) & SF_NETVAL)) {
    Value *r_copy = new_Value(Value_nbits(r));
    BCCopy_init(CodeBlock_nextEmpty(cb),r_copy,r);
    r = r_copy;
  }
  

#if 0
  printf("Stat(%d): ",size);
  Expr_print(sd->a_lhs,stdout);
  printf(" = ");
  Expr_print(sd->a_rhs,stdout);
  printf("\n");
#endif

  /*
   * Assign parts of generated right-hand side expression to lhs expressions.
   */
  for (le = List_first(&lhs_list);le;le = List_next(&lhs_list,le)) {
    Expr *lhs_e = (Expr*) ListElem_obj(le);
    int lhs_size;
    Net *n;
    Value *nLsb,*nAddr;

    if (Expr_lhsGenerate(lhs_e, scope, cb, &n, &nLsb, &lhs_size, &nAddr) < 0) {
      errorFile(StatDecl_getPlace(sd),ERR_BADLHS);
      return;
    }

    //
    // LHS pieces must be reg.
    //
    if (!(Net_getType(n) & NT_P_REG)) {
      errorFile(StatDecl_getPlace(sd),ERR_LHSNOTREG,Net_getLocalName(n));
    }


    if (nAddr) {
      SDAsgn_generateMemAsgnPiece(sd, scope, cb, n, nAddr, nLsb, r, base_bit, lhs_size);
    } else {
      SDAsgn_generateNetAsgnPiece(sd, scope, cb, n, nLsb, r, base_bit, lhs_size);
    }

    base_bit += Expr_getBitSize(lhs_e, scope);
  }

  List_uninit(&lhs_list);
}


void SDBlock_generate(SDBlock *sd, Scope *scope, CodeBlock *cb)
{
  ListElem *e;

  for (e = List_first(sd->b_stats);e;e = List_next(sd->b_stats,e)) {
    StatDecl_generate((StatDecl*)ListElem_obj(e), scope, cb);
  }
}

void SDIf_generate(SDIf *sd, Scope *scope, CodeBlock *cb)
{
  Value *cond;
  unsigned branch_bc,after_bc,then_bc;

  /*
   * Generate condition and save branch instruction so we can adjust branch
   * position. The branch will skip the else part if the condition is true,
   * otherwise we fall into the else part.
   */
  cond = Expr_generateS(sd->i_cond, scope, cb);
  branch_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),cond,0,cb,0);

  /*
   * Generate the else block if there is one followed by an uncondition jump
   * past the then block.
   */
  if (sd->i_else)
    StatDecl_generate(sd->i_else, scope, cb);
  then_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,0);

  /*
   * Generate the then block.
   */
  if (sd->i_then)
    StatDecl_generate(sd->i_then, scope, cb);
  after_bc = CodeBlock_size(cb);

  /*
   * Go back and fix all of the branch offsets.
   */
  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,branch_bc),then_bc+1);
  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,then_bc),after_bc);
}

void SDWhile_generate(SDWhile *sd, Scope *scope, CodeBlock *cb)
{
  Value *cond;
  unsigned top_bc, branch_bc, after_bc;

  /*
   * Generate condition and save branch instruction and top of loop
   */
  top_bc = CodeBlock_size(cb);
  cond = Expr_generateS(sd->w_cond, scope, cb);
  branch_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),cond,1,cb,0);
  StatDecl_generate(sd->w_body,scope,cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,top_bc);
  after_bc = CodeBlock_size(cb);

  /*
   * Go back and fix all of the branch offsets.
   */
  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,branch_bc),after_bc);
}

void SDForever_generate(SDForever *sd, Scope *scope, CodeBlock *cb)
{
  unsigned top_bc;

  /*
   * Generate condition and save branch instruction and top of loop
   */
  top_bc = CodeBlock_size(cb);
  StatDecl_generate(sd->f_body,scope,cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,top_bc);
}

void SDFor_generate(SDFor *sd, Scope *scope, CodeBlock *cb)
{
  Value *cond;
  unsigned top_bc, branch_bc, after_bc;

  /*
   * Generate intialization
   */
  StatDecl_generate(sd->f_init,scope,cb);

  /*
   * Generate condition and save branch instruction and top of loop
   */
  top_bc = CodeBlock_size(cb);
  cond = Expr_generateS(sd->f_cond, scope, cb);
  branch_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),cond,1,cb,0);
  StatDecl_generate(sd->f_body,scope,cb);
  StatDecl_generate(sd->f_next,scope,cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,top_bc);
  after_bc = CodeBlock_size(cb);

  /*
   * Go back and fix all of the branch offsets.
   */
  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,branch_bc),after_bc);
}

void SDCase_generate(SDCase *sdc, Scope *scope, CodeBlock *cb)
{
  Value *select;
  ListElem *le;
  unsigned *branch_bc,after_bc;
  int n = List_numElems(sdc->c_caseEnts);
  int i;
  StatDecl *defaultStat = 0;

  if (n <= 0) return;

  branch_bc = (unsigned*) malloc(sizeof(unsigned)*n);

  /*
   * Evaluate selection condition.
   */
  select = Expr_generateS(sdc->c_select, scope, cb);
  for (le = List_first(sdc->c_caseEnts), i = 0;le;le = List_next(sdc->c_caseEnts,le), i++) {
    SDCaseEnt *ce = (SDCaseEnt*)ListElem_obj(le);
    Value *result;
    Value *cond;
    unsigned skip_bc;

    if (!ce->ce_cond) {
      if (defaultStat) {
	errorFile(StatDecl_getPlace(sdc),ERR_REPCASE);
	return;
      }
      defaultStat = ce->ce_stat;
      continue;
    }

    cond = Expr_generateS(ce->ce_cond, scope, cb);
    result = new_Value(imax(Value_nbits(cond), Value_nbits(select)));
    BCOpr_init(CodeBlock_nextEmpty(cb),Value_caseEq,result,select,cond,0);
    skip_bc = CodeBlock_size(cb);
    BCGoto_init(CodeBlock_nextEmpty(cb),result,1,cb,0);
    StatDecl_generate(ce->ce_stat, scope, cb);
    branch_bc[i] = CodeBlock_size(cb);
    BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,0);
    BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,skip_bc),branch_bc[i]+1);
  }
  if (defaultStat) {
    StatDecl_generate(defaultStat, scope, cb);
    n--;
  }
  after_bc = CodeBlock_size(cb);

  for (i = 0;i < n;i++)
    BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,branch_bc[i]),after_bc);

  free(branch_bc);
}

void SDFork_generate(SDFork *s, Scope *scope, CodeBlock *cb)
{
  ListElem *le;
  unsigned *branch_bc,start_bc,after_bc;
  int n = List_numElems(s->f_stats);
  int i;

  if (n <= 0) return;

  /*
   * Goto to jump over code for fork branches
   */
  start_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,0);
  branch_bc = (unsigned*) malloc(sizeof(unsigned)*n);

  for (le = List_first(s->f_stats), i = 0;le;le = List_next(s->f_stats, le), i++) {
    StatDecl *branch = (StatDecl*) ListElem_obj(le);

    branch_bc[i] = CodeBlock_size(cb);
    StatDecl_generate(branch, scope, cb);
    BCEnd_init(CodeBlock_nextEmpty(cb));
  }

  /*
   * Fix initial goto to jump here.
   */
  after_bc = CodeBlock_size(cb);
  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,start_bc),after_bc);

  /*
   * Spawn branch threads
   */
  for (i = 0;i < n;i++) {
    BCSpawn_init(CodeBlock_nextEmpty(cb),cb,branch_bc[i]);
  }

  /*
   * Wait for branches to finish.
   */
  BCWait_init(CodeBlock_nextEmpty(cb));

  free(branch_bc);
}

void SDRepeat_generate(SDRepeat *sdr, Scope *scope, CodeBlock *cb)
{
  Value *count,*zero,*one,*cond;
  unsigned top_bc,test_bc,after_bc;

  cond = new_Value(SSWORDSIZE);
  zero = new_Value(SSWORDSIZE);
  one = new_Value(SSWORDSIZE);
  Value_zero(zero);
  Value_lone(one);

  /*
   * If this is a direct access value, make a copy.
   */
  count = Expr_generate(sdr->r_count, SSWORDSIZE, scope, cb);
  if ((Value_getAllFlags(count) & SF_NETVAL)) {
    Value *r_copy = new_Value(Value_nbits(count));
    BCCopy_init(CodeBlock_nextEmpty(cb),r_copy,count);
    count = r_copy;
  }


  top_bc = CodeBlock_size(cb);
  BCOpr_init(CodeBlock_nextEmpty(cb),Value_eq,cond,zero,count,0);
  test_bc = CodeBlock_size(cb);
  BCGoto_init(CodeBlock_nextEmpty(cb),cond,0,cb,0);
  StatDecl_generate(sdr->r_body, scope, cb);
  if (count->nbits <= SSWORDSIZE)
    BCOpr_init(CodeBlock_nextEmpty(cb),Value_w_sub,count,count,one,0);
  else
    BCOpr_init(CodeBlock_nextEmpty(cb),Value_sub,count,count,one,0);
  BCGoto_init(CodeBlock_nextEmpty(cb),0,0,cb,top_bc);
  after_bc = CodeBlock_size(cb);

  /*
   * Go back and fix all of the branch offsets.
   */
  BCGoto_setOffset((BCGoto*)CodeBlock_get(cb,test_bc),after_bc);
}



/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDNull_getReaders(StatDecl *s,Scope *scope,PHash *H)
{
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDDelay_getReaders(SDDelay *s,Scope *scope, PHash *H)
{
  StatDecl_getReaders(s->d_stat, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope fo use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDTrigger_getReaders(SDTrigger *s,Scope *scope, PHash *H)
{
  StatDecl_getReaders(s->t_stat, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope fo use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDWait_getReaders(SDWait *s,Scope *scope, PHash *H)
{
  Expr_getReaders(s->w_cond, scope, H);
  StatDecl_getReaders(s->w_stat, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDRaise_getReaders(SDRaise *s,Scope *scope, PHash *H)
{
  /*no readers*/
}


/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDAsgn_getReaders(SDAsgn *s,Scope *scope, PHash *H)
{
  Expr_getReaders(s->a_rhs, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDBlock_getReaders(SDBlock *s, Scope *scope, PHash *H)
{
  ListElem *e;

  for (e = List_first(s->b_stats);e;e = List_next(s->b_stats,e)) {
    StatDecl_getReaders((StatDecl*)ListElem_obj(e), scope, H);
  }
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDIf_getReaders(SDIf *s, Scope *scope, PHash *H)
{
  Expr_getReaders(s->i_cond, scope, H);
  if (s->i_then)
    StatDecl_getReaders((StatDecl*)s->i_then, scope, H);
  if (s->i_else)
    StatDecl_getReaders((StatDecl*)s->i_else, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDWhile_getReaders(SDWhile *s, Scope *scope, PHash *H)
{
  Expr_getReaders(s->w_cond, scope, H);
  StatDecl_getReaders(s->w_body, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDForever_getReaders(SDForever *s, Scope *scope, PHash *H)
{
  StatDecl_getReaders(s->f_body, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDFor_getReaders(SDFor *s, Scope *scope, PHash *H)
{
  if (s->f_init)   StatDecl_getReaders(s->f_init, scope, H);
  if (s->f_cond)   Expr_getReaders(s->f_cond, scope, H);
  if (s->f_body)   StatDecl_getReaders(s->f_body, scope, H);
  if (s->f_next)   StatDecl_getReaders(s->f_next, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDCase_getReaders(SDCase *s, Scope *scope, PHash *H)
{
  ListElem *le;

  if (s->c_select)
    Expr_getReaders(s->c_select, scope, H);

  for (le = List_first(s->c_caseEnts);le;le = List_next(s->c_caseEnts, le)) {
    SDCaseEnt *ce = (SDCaseEnt*) ListElem_obj(le);
    if (ce->ce_cond)  Expr_getReaders(ce->ce_cond, scope, H);
    if (ce->ce_stat)  StatDecl_getReaders(ce->ce_stat, scope, H);
  }
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDFork_getReaders(SDFork *s, Scope *scope, PHash *H)
{
  ListElem *le;

  for (le = List_first(s->f_stats);le;le = List_next(s->f_stats, le)) {
    StatDecl *fs = (StatDecl*) ListElem_obj(le);
    StatDecl_getReaders(fs, scope, H);
  }
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDRepeat_getReaders(SDRepeat *s, Scope *scope, PHash *H)
{
  if (s->r_count) Expr_getReaders(s->r_count, scope, H);
  if (s->r_body)  StatDecl_getReaders(s->r_body, scope, H);
}

/*****************************************************************************
 *
 * Get nets that are read by a statement.
 *
 * Parameters:
 *     s		Statment to check
 *     scope		Scope to use for variable lookup
 *     H		Hash table used to return set of used Net*s.
 *
 *****************************************************************************/
void SDTask_getReaders(SDTask *s, Scope *scope, PHash *H)
{
  int i;

  for (i = 0;i < s->t_nargs;i++)
    Expr_getReaders(s->t_args[i], scope, H);
}

void StatDecl_generate(StatDecl *S,Scope *scope,CodeBlock *cb)
{
  if (!S) return;
  Place_setCurrent(&S->sd_common.sdc_place);
  (*S->sd_common.sdc_vtable->sdv_generate)(S,scope,cb);
}



