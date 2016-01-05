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
#ifndef __statement_h
#define __statement_h

/*
 * Flags for print functions
 */
#define PF_NOINDENT		0x1		/* Do not indent next line */
#define PF_NOSEMI		0x2		/* Do not add semicolon to end */
#define PF_BLOCKNL		0x4		/* Add preceeding newline if block */



/*****************************************************************************
 *
 * Type codes for statement declarations
 *
 *****************************************************************************/
typedef enum { 
  ST_NULL = 0,
  ST_BLOCK = 1,
  ST_ASGN = 2,
  ST_IF = 3,
  ST_WHILE = 4,
  ST_CASE = 5,
  ST_FORK = 6,
  ST_DELAY = 7,
  ST_TRIGGER = 8,
  ST_REPEAT = 9,
  ST_FOR = 10,
  ST_TASK = 11,
  ST_WAIT = 12,
  ST_FOREVER = 13,
  ST_RAISE = 14
} statcode_t;


typedef void StatPrint_f(StatDecl*,FILE *f,int indent,unsigned flags);
typedef void StatGenerate_f(StatDecl*,Scope *scope,CodeBlock *cb);
typedef void StatGetReaders_f(StatDecl*,Scope *scope,PHash *H);

/*****************************************************************************
 *
 * Vtable for StatDecl objects
 *
 *****************************************************************************/
typedef struct {
  StatPrint_f		*sdv_print;		/* Print function pointer */
  StatGenerate_f	*sdv_generate;		/* Code generation function pointer */
  StatGetReaders_f	*sdv_getreaders;	/* Get all Net*s read by a statement */
} StatDeclVTable;

/*****************************************************************************
 *
 * SDCommon - common portion of statement
 *
 *****************************************************************************/
typedef struct {
  statcode_t		sdc_type;		/* Type of statement */	
  StatDeclVTable	*sdc_vtable;		/* Table of function pointers */
  Place			sdc_place;		/* Place of statement */
} SDCommon;

/*****************************************************************************
 *
 * SDDelay - Delayed statement
 *
 * #n ...
 *
 *****************************************************************************/
typedef struct {
  SDCommon	d_common;	/* Type of statement ST_DELAY */	
  Expr		*d_delay;	/* Delay expression */
  StatDecl	*d_stat;	/* Statement subjected to delay */
} SDDelay;

/*****************************************************************************
 *
 * SDRaise - Raise event statement
 *
 * -> named_event;
 *
 *****************************************************************************/
typedef struct {
  SDCommon	r_common;	/* Type of statement ST_RAISE */	
  Expr		*r_event;	/* Event name expression */
} SDRaise;

/*****************************************************************************
 *
 * SDTrigger - Event triggered statement
 *
 * @ (ev) ...
 *
 *****************************************************************************/
typedef struct {
  SDCommon	t_common;	/* Type of statement ST_TRIGGER */	
  Expr		*t_trigger;	/* Trigger expression */
  StatDecl	*t_stat;	/* Statement subjected to trigger(s) */
} SDTrigger;

/*****************************************************************************
 *
 * SDWait - Wait for condition
 *
 * wait (cond) ...
 *
 *****************************************************************************/
typedef struct {
  SDCommon	w_common;	/* Type of statement ST_WAIT */	
  Expr		*w_cond;	/* Condition to wait for */
  StatDecl	*w_stat;	/* Statement to execute when condition is true */
} SDWait;

/*****************************************************************************
 *
 * SDIf
 *
 *****************************************************************************/
typedef struct {
  SDCommon	i_common;	/* Type of statement ST_IF */
  Expr		*i_cond;	/* Condition */
  StatDecl	*i_then;	/* Then statement */
  StatDecl	*i_else;	/* Else statement */
} SDIf;

/*****************************************************************************
 *
 * SDAsgn
 *
 *****************************************************************************/
typedef struct {
  SDCommon	a_common;	/* Type of statement ST_ASGN */	
  Expr		*a_lhs;		/* Left-hand-side expression */
  Expr		*a_rhs;		/* Right-hand-side expression */
  Expr		*a_bcond;	/* Conditional blocking */
  int		a_block;	/* Do blocking assignment */
} SDAsgn;

/*****************************************************************************
 *
 * SDBlock
 *
 *****************************************************************************/
typedef struct {
  SDCommon	b_common;	/* Type of statement ST_BLOCK */	
  const char	*b_name;	/* Name if this is a named block */
  List		*b_stats;	/* List of statements in block */
} SDBlock;


/*****************************************************************************
 *
 * SDCaseEnt - Entry in an SDCase
 *
 *****************************************************************************/
typedef struct {
  Expr		*ce_cond;	/* Condition (or null for default) */
  StatDecl	*ce_stat;	/* Statement to execute on condition */
} SDCaseEnt;

/*****************************************************************************
 *
 * SDCase
 *
 *****************************************************************************/
typedef struct {
  SDCommon	c_common;	/* Type of statement ST_CASE */	
  int		c_caseType;	/* CASE, CASEX or CASEZ */
  Expr		*c_select;	/* Selection expression */
  List		*c_caseEnts;	/* List of case entries */
} SDCase;

/*****************************************************************************
 *
 * SDFor
 *
 *****************************************************************************/
typedef struct {
  SDCommon	f_common;	/* Type of statement ST_FOR */	
  StatDecl	*f_init;	/* Initialization statement */
  Expr		*f_cond;	/* Loop condition */
  StatDecl	*f_next;	/* Next statement */
  StatDecl	*f_body;	/* Loop body */
} SDFor;

/*****************************************************************************
 *
 * SDWhile
 *
 *****************************************************************************/
typedef struct {
  SDCommon	w_common;	/* Type of statement ST_WHILE */	
  Expr		*w_cond;	/* Loop condition */
  StatDecl	*w_body;	/* Loop body */
} SDWhile;

/*****************************************************************************
 *
 * SDForever
 *
 *****************************************************************************/
typedef struct {
  SDCommon	f_common;	/* Type of statement ST_FOREVER */	
  StatDecl	*f_body;	/* Loop body */
} SDForever;

/*****************************************************************************
 *
 * SDFork
 *
 *****************************************************************************/
typedef struct {
  SDCommon	f_common;	/* Type of statement ST_FORK */	
  List		*f_stats;	/* Expressions to fork */
} SDFork;

/*****************************************************************************
 *
 * SDRepeat
 *
 *****************************************************************************/
typedef struct {
  SDCommon	r_common;	/* Type of statement ST_REPEAT */	
  Expr		*r_count;	/* Number of repitions */
  StatDecl	*r_body;	/* Expressions to fork */
} SDRepeat;

/*****************************************************************************
 *
 * SDTask
 *
 *****************************************************************************/
typedef struct {
  SDCommon	t_common;	/* Type of statement ST_TASK */
  char		*t_name;	/* Name of task */
  int		t_nargs;	/* Number of arguments */
  Expr		**t_args;	/* Expressions for arguments */
} SDTask;

/*****************************************************************************
 *
 * StatDecl - Statement declaration
 *
 * This is a "simulated" virtual base class.
 *
 *****************************************************************************/
union StatDecl_union {
  statcode_t	sd_type;		/* Type of statement */
  SDCommon	sd_common;		/* Common statement fields */
  SDDelay	sd_delay;		/* Delayed statement */
  SDTrigger	sd_trigger;		/* Event triggered statement */
  SDWait	sd_wait;		/* wait statement */
  SDAsgn	sd_asgn;		/* Assignment statement */
  SDBlock	sd_block;		/* Block statement */
  SDIf		sd_if;			/* If statement */
  SDWhile	sd_while;		/* While statement */
  SDForever	sd_forever;		/* Forever statement */
  SDFor		sd_for;			/* For statement */
  SDCase	sd_case;		/* Case statement */
  SDFork	sd_fork;		/* Fork statement */
  SDRepeat	sd_repeat;		/* Repeat statement */
  SDTask	sd_task;		/* Task invocation */
};

#define StatDecl_getPlace(sd) (&((StatDecl*)(sd))->sd_common.sdc_place)

StatDecl *new_SDNull();
StatDecl *new_SDDelay(Expr *delay,StatDecl *stat);
StatDecl *new_SDTrigger(Expr *trigger,StatDecl *stat);
StatDecl *new_SDRaise(Expr *event);
StatDecl *new_SDWait(Expr *cond,StatDecl *stat);
StatDecl *new_SDAsgn(Expr *lhs, Expr *rhs,Expr *bcond,int block);
StatDecl *new_SDBlock(const char *name,List *L);
StatDecl *new_SDIf(Expr *cond,StatDecl *then_body,StatDecl *else_body);
StatDecl *new_SDWhile(Expr *cond,StatDecl *body);
StatDecl *new_SDForever(StatDecl *body);
StatDecl *new_SDFor(StatDecl *init,Expr *cond,StatDecl *next,StatDecl *body);
StatDecl *new_SDCase(int,Expr *sel,List *ents);
StatDecl *new_SDFork(List *stats);
StatDecl *new_SDRepeat(Expr *count,StatDecl *body);
StatDecl *new_SDTask(const char *name,List *expr_list);
SDCaseEnt *new_SDCaseEnt(Expr *cond,StatDecl *stat);

void SDTrigger_printTrigger(SDTrigger*, FILE*);
void SDTrigger_printBody(SDTrigger*, FILE*,int indent,unsigned flags);

void SDNull_print(StatDecl*, FILE*, int,unsigned flags);
void SDDelay_print(SDDelay*, FILE*, int,unsigned flags);
void SDTrigger_print(SDTrigger*, FILE*, int,unsigned flags);
void SDWait_print(SDWait*, FILE*, int,unsigned flags);
void SDRaise_print(SDRaise*, FILE*, int,unsigned flags);
void SDAsgn_print(SDAsgn*, FILE*, int,unsigned flags);
void SDBlock_print(SDBlock*, FILE*, int,unsigned flags);
void SDIf_print(SDIf*, FILE*, int,unsigned flags);
void SDWhile_print(SDWhile*, FILE*, int,unsigned flags);
void SDForever_print(SDForever*, FILE*, int,unsigned flags);
void SDFor_print(SDFor*, FILE*, int,unsigned flags);
void SDCase_print(SDCase*, FILE*, int,unsigned flags);
void SDFork_print(SDFork*, FILE*, int,unsigned flags);
void SDRepeat_print(SDRepeat*, FILE*, int,unsigned flags);
void SDTask_print(SDTask*, FILE*, int,unsigned flags);

void SDNull_generate(StatDecl*,Scope *scope,CodeBlock*);
void SDDelay_generate(SDDelay*,Scope *scope,CodeBlock*);
void SDTrigger_generate(SDTrigger*,Scope *scope,CodeBlock*);
void SDWait_generate(SDWait*,Scope *scope,CodeBlock*);
void SDRaise_generate(SDRaise*,Scope *scope,CodeBlock*);
void SDAsgn_generate(SDAsgn*,Scope *scope,CodeBlock*);
void SDBlock_generate(SDBlock*,Scope *scope,CodeBlock*);
void SDIf_generate(SDIf*,Scope *scope,CodeBlock*);
void SDWhile_generate(SDWhile*,Scope *scope,CodeBlock*);
void SDForever_generate(SDForever*,Scope *scope,CodeBlock*);
void SDFor_generate(SDFor*,Scope *scope,CodeBlock*);
void SDCase_generate(SDCase*,Scope *scope,CodeBlock*);
void SDFork_generate(SDFork*,Scope *scope,CodeBlock*);
void SDRepeat_generate(SDRepeat*,Scope *scope,CodeBlock*);
void SDTask_generate(SDTask*,Scope *scope,CodeBlock*);
void StatDecl_generate(StatDecl*,Scope *scope,CodeBlock*);

void SDNull_getReaders(StatDecl*,Scope *scope,PHash *H);
void SDDelay_getReaders(SDDelay*,Scope *scope, PHash *H);
void SDTrigger_getReaders(SDTrigger*,Scope *scope, PHash *H);
void SDWait_getReaders(SDWait*,Scope *scope, PHash *H);
void SDRaise_getReaders(SDRaise*,Scope *scope, PHash *H);
void SDAsgn_getReaders(SDAsgn*,Scope *scope, PHash *H);
void SDBlock_getReaders(SDBlock*,Scope *scope, PHash *H);
void SDIf_getReaders(SDIf*,Scope *scope, PHash *H);
void SDWhile_getReaders(SDWhile*,Scope *scope, PHash *H);
void SDForever_getReaders(SDForever*,Scope *scope, PHash *H);
void SDFor_getReaders(SDFor*,Scope *scope, PHash *H);
void SDCase_getReaders(SDCase*,Scope *scope, PHash *H);
void SDFork_getReaders(SDFork*,Scope *scope, PHash *H);
void SDRepeat_getReaders(SDRepeat*,Scope *scope, PHash *H);
void SDTask_getReaders(SDTask*,Scope *scope, PHash *H);


#define StatDecl_print(S,F,I,flags)	(*(S)->sd_common.sdc_vtable->sdv_print)(S,F,I,flags)
#define StatDecl_getReaders(S,mc,cb)		(*(S)->sd_common.sdc_vtable->sdv_getreaders)(S,mc,cb)

#endif

