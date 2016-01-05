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
#ifndef __task_h
#define __task_h

#define UTT_TASK		0
#define UTT_FUNCTION		1

/*****************************************************************************
 *
 * Object representing a task declaration.
 *
 *****************************************************************************/
struct UserTaskDecl_str {
  char			*utd_name;		/* Name of user task */
  unsigned		utd_type;		/* task or function */
  int			utd_isauto;		/* Is this an automatic task/function */
  VRange		*utd_range;		/* Output range if this is a function */
  ModuleDecl		*utd_module;		/* Module task is defined within */
  StatDecl		*utd_stat;		/* Task body */
  ScopeDecl		utd_scope;		/* Scope of this task declaration */
  List/*NetDecl*/	utd_parms;		/* Task parameters */
};

/*****************************************************************************
 *
 * Object representing an instance of a task.
 *
 *****************************************************************************/
struct UserTask_str {
  UserTaskDecl		*ut_decl;		/* Task declaration */
  Scope			ut_scope;		/* Scope */
  CodeBlock		*ut_block;		/* Block in which to write code */
  unsigned		ut_offset;		/* Start of task bytecode */
  unsigned		ut_nbits;		/* Number of output bits if this is a function */
};

UserTaskDecl *new_UserTaskDecl(const char *name,ModuleDecl *m,unsigned ttype,int isauto);
void UserTaskDecl_setStat(UserTaskDecl *utd, StatDecl *sd);
void UserTaskDecl_addParm(UserTaskDecl *utd,NetDecl *netd);
#define UserTaskDecl_getName(utd) (utd)->utd_name
#define UserTaskDecl_getScope(utd) (&(utd)->utd_scope)
#define UserTaskDecl_getType(utd) ((utd)->utd_type)
#define UserTaskDecl_setRange(utd,range) ((utd)->utd_range = (range))
#define UserTaskDecl_isAuto(utd) (utd)->utd_isauto

UserTask *new_UserTask(UserTaskDecl*,Scope *scope);
void UserTask_generate(UserTask*,CodeBlock*);
void UserTask_generateCall(UserTask*,void **,CodeBlock*);
void UserTask_generateInlineCall(UserTask*,void **,CodeBlock*);
#define UserTask_getName(ut) ((ut)->ut_decl->utd_name)
#define UserTask_getScope(ut) (ut)->ut_scope
#define UserTask_getType(ut) ((ut)->ut_decl->utd_type)
#define UserTask_nbits(ut) (ut)->ut_nbits
#define UserTask_isAuto(ut) (ut)->ut_decl->utd_isauto

#endif
