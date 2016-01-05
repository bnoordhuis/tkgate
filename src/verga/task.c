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

    Last edit by hansen on Sat May 20 19:09:35 2006
****************************************************************************/
#include "thyme.h"

UserTaskDecl *new_UserTaskDecl(const char *name,ModuleDecl *m,unsigned ttype,int isauto)
{
  UserTaskDecl *utd = (UserTaskDecl*) malloc(sizeof(UserTaskDecl));

  utd->utd_type = ttype;
  utd->utd_name = strdup(name);
  utd->utd_range = 0;
  utd->utd_module = m;
  utd->utd_stat = 0;
  utd->utd_isauto = isauto;
  ScopeDecl_init(&utd->utd_scope,ModuleDecl_getScope(m));
  //  List_init(&utd->utd_inputs);
  //  List_init(&utd->utd_outputs);
  //  List_init(&utd->utd_inouts);
  List_init(&utd->utd_parms);

  return utd;
}


void UserTaskDecl_addParm(UserTaskDecl *utd,NetDecl *netd)
{
  List_addToTail(&utd->utd_parms, netd);
}

void UserTaskDecl_setStat(UserTaskDecl *utd, StatDecl *sd)
{
  utd->utd_stat = sd; 
}

UserTask *new_UserTask(UserTaskDecl *utd,Scope *scope)
{
  UserTask *ut = (UserTask*) malloc(sizeof(UserTask));
  char path[STRMAX];

  ut->ut_decl = utd;
  ut->ut_block = 0;
  ut->ut_offset = 0;
  ut->ut_nbits = 0;

  sprintf(path,"%s.%s", Scope_getPath(scope), UserTask_getName(ut));
  Scope_init(&ut->ut_scope, path, scope, Scope_getModuleInst(scope));

  if (utd->utd_range) {
    VRange_getSize(utd->utd_range, scope, &ut->ut_nbits);
  }

  return ut;
}

/*****************************************************************************
 *
 * Generate code for a user task.
 *
 * Parameters:
 *       ut		User task descriptor
 *       cb		Codeblock to write in
 *
 *****************************************************************************/
void UserTask_generate(UserTask *ut,CodeBlock *cb)
{
  UserTaskDecl *utd = ut->ut_decl;
  Circuit *c = &vgsim.vg_circuit;
  SHash *decl_table = &ut->ut_decl->utd_scope.sd_nets;
  ModuleDecl *m = ut->ut_decl->utd_module;
  HashElem *he;

  /*
   * Declare nets used in the user task
   */
  for (he = Hash_first(decl_table);he;he = Hash_next(decl_table,he)) {
    NetDecl *nd = (NetDecl*) HashElem_obj(he);
    Circuit_declareNet(c, &ut->ut_scope, nd, m, &nd->n_place);
  }

  ut->ut_block = cb;
  ut->ut_offset = CodeBlock_size(cb);
  StatDecl_generate(utd->utd_stat,&ut->ut_scope,cb);

  BCReturn_init(CodeBlock_nextEmpty(cb));
}


/*****************************************************************************
 *
 * Generate bytecode to pass values to a user task or function.
 *
 * Parameters:
 *       ut		User task descriptor
 *       sargs		Arguments to task
 *       cb		Codeblock to write in
 *       scope		Scope of task/function
 *
 *****************************************************************************/
void UserTask_generateCallInputs(UserTask *ut,void **sargs,CodeBlock *cb,Scope *scope)
{
  List *parms = &ut->ut_decl->utd_parms;
  ListElem *le;
  int i;

  for (i = 0, le = List_first(parms);le;le = List_next(parms,le),i++) {
    NetDecl *netdecl = (NetDecl*) ListElem_obj(le);
    Net *net = Scope_findNet(scope, NetDecl_getName(netdecl),0);
    Value *value = 0;

    switch ((NetDecl_getType(netdecl) & NT_P_IO_MASK)) {
    case NT_P_INOUT :
      value = Net_getValue((Net*)sargs[i]);
      break;
    case NT_P_INPUT :
      value = (Value*)sargs[i];
      break;
    case NT_P_OUTPUT :
      break;
    }

    if (value) {
      if (Value_nbits(value) < Net_nbits(net)) {
	Value *new_value = new_Value(Net_nbits(net));
	BCCopy_init(CodeBlock_nextEmpty(cb),new_value,value);
	value = new_value;
      }
      BCAsgn_init(CodeBlock_nextEmpty(cb),net,0,value,0,Net_nbits(net));
    }
  }
}

/*****************************************************************************
 *
 * Generate bytecode to pass values from a user task or function.
 *
 * Parameters:
 *       ut		User task descriptor
 *       sargs		Arguments to task
 *       cb		Codeblock to write in
 *       scope		Scope of task/function
 *
 *****************************************************************************/
void UserTask_generateCallOutputs(UserTask *ut,void **sargs,CodeBlock *cb,Scope *scope)
{
  List *parms = &ut->ut_decl->utd_parms;
  ListElem *le;
  int i;

  for (i = 0, le = List_first(parms);le;le = List_next(parms,le),i++) {
    NetDecl *netdecl = (NetDecl*) ListElem_obj(le);
    Net *net = Scope_findNet(scope, NetDecl_getName(netdecl),0);
    Value *value = 0;
    Value *out_value = 0;
    Net *out_net = 0;

    switch ((NetDecl_getType(netdecl) & NT_P_IO_MASK)) {
    case NT_P_INOUT :
    case NT_P_OUTPUT :
      if (UserTask_getType(ut) == UTT_FUNCTION) 
	out_value = (Value*)sargs[i];
      else
	out_net = (Net*)sargs[i];
      value = Net_getValue(net);
      break;
    }

    if (out_net) {
      if (Net_nbits(out_net) > Value_nbits(value)) {
	Value *new_value = new_Value(Net_nbits(out_net));
	BCCopy_init(CodeBlock_nextEmpty(cb),new_value,value);
	value = new_value;
      }
      BCAsgn_init(CodeBlock_nextEmpty(cb),out_net,0,value,0,Net_nbits(out_net));
    } else if (out_value) {
      BCCopy_init(CodeBlock_nextEmpty(cb),out_value,value);
    }
  }

}

/*****************************************************************************
 *
 * Generate code to call a user task.
 *
 * Parameters:
 *       ut		User task descriptor
 *       sargs		Arguments to task
 *       cb		Codeblock to write in
 *
 * Note: The elements of the sargs array can be Value* for input arguments or
 * Net* for inout or output arguments.
 *
 *****************************************************************************/
void UserTask_generateCall(UserTask *ut,void **sargs,CodeBlock *cb)
{
  /*
   * Generate code to copy input values and inout values into the arguments
   * of the task call.
   */
  UserTask_generateCallInputs(ut,sargs,cb,&ut->ut_scope);

  /*
   * Generate a call to the task
   */
  BCSubr_init(CodeBlock_nextEmpty(cb),ut->ut_block,ut->ut_offset);

  /*
   * Generate code to copy output values and inout values from the arguments
   * of the task call.
   */
  UserTask_generateCallOutputs(ut,sargs,cb,&ut->ut_scope);
}

/*****************************************************************************
 *
 * Generate bytecode for an inlined task/function call
 *
 * Parameters:
 *       ut		User task descriptor
 *       sargs		Arguments to task
 *       cb		Codeblock to write in
 *
 * The semantics of this function is similar to UserTask_generateCall except 
 * that bytecode is generated for each invocation of the task.  This results
 * in each invocation getting its own set of nets/variables instead of a 
 * shared set.
 *
 *****************************************************************************/
void UserTask_generateInlineCall(UserTask *ut,void **sargs,CodeBlock *cb)
{
  UserTaskDecl *utd = ut->ut_decl;
  Circuit *c = &vgsim.vg_circuit;
  SHash *decl_table = &ut->ut_decl->utd_scope.sd_nets;
  ModuleDecl *m = ut->ut_decl->utd_module;
  HashElem *he;
  Scope *parent_scope = ut->ut_scope.s_parent;
  Scope *scope;
  char path[STRMAX];
  static int index = 0;

  /*
   * Generate code for the user task.
   */
  sprintf(path,"%s.%s$%d", Scope_getPath(parent_scope), UserTask_getName(ut),index++);
  scope = new_Scope(path, parent_scope, Scope_getModuleInst(parent_scope));

  /*
   * Declare nets used in the user task
   */
  for (he = Hash_first(decl_table);he;he = Hash_next(decl_table,he)) {
    NetDecl *nd = (NetDecl*) HashElem_obj(he);
    Circuit_declareNet(c, scope, nd, m, &nd->n_place);
  }


  /*
   * Generate code to copy input values and inout values into the arguments
   * of the task call.
   */
  UserTask_generateCallInputs(ut,sargs,cb,scope);

  ut->ut_block = cb;
  ut->ut_offset = CodeBlock_size(cb);
  StatDecl_generate(utd->utd_stat,scope,cb);

  /*
   * Generate code to copy output values and inout values from the arguments
   * of the task call.
   */
  UserTask_generateCallOutputs(ut,sargs,cb,scope);
}

