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
#ifndef __systask_h
#define __systask_h

/*
 * Indicate there is no limit to the number of arguments
 */
#define NOLIM		0x7ffffff

/*
 * Maximum number of files open through Verilog $fopen()
 */
#define STT_MAXFILES	31

/*****************************************************************************
 *
 * Task argument type code
 *
 *****************************************************************************/
typedef enum {
  TAT_VALUE=0,
  TAT_NET=1,
  TAT_TRIGGER=2,
  TAT_TRIGGERPAIR=3,
  TAT_EXPR=4
} stargtype_t;

/*****************************************************************************
 *
 * Task action code
 *
 *****************************************************************************/
typedef enum {
  TA_SETUP,					/* This is the setup call of the task */
  TA_STROBE,					/* This is a strobed call of the task */
  TA_REMOVE,					/* This is a remove call of the task */
} taskact_t;


/*****************************************************************************
 *
 * System task flags
 *
 *****************************************************************************/
typedef enum {
  STF_NEEDCTX = 0x1,
  STF_NEEDNETS = 0x2,
  STF_SPECIFY = 0x4,
} taskflag_t; 

/*****************************************************************************
 *
 * Context for persistent or late executing tasks.
 *
 *****************************************************************************/
typedef struct {
  taskact_t	tc_action;			/* Type of action we are doing */

  VGThread	*tc_thread;			/* Thread to use in block */
  CodeBlock	*tc_codeBlock;			/* Copy of bytecode to execute systask */
  int		tc_numNets;			/* Number of dependent nets */
  Net		**tc_nets;			/* Array of dependent nets */
  Value		**tc_lastValues;		/* Array of last values for nets */
  void		*tc_data;			/* Task specific data */
} TaskContext;

/*****************************************************************************
 *
 * Function pointer class for function implementing Verilog system tasks such
 * as $display, $monitor, $strobe, $stop, etc.
 *
 *****************************************************************************/
typedef void systask_f(VGThread *t,		/* Thread system task is executing in */
		       Value *r,		/* Return value if used in expression */
		       int numArgs,		/* Number of arguments */
		       void **args,		/* Argument values */
		       TaskContext *tc);	/* Optional task context (used by persistent tasks) */

/*****************************************************************************
 *
 * Description of system tasks
 *
 *****************************************************************************/
typedef struct {
  const char	*st_name;		/* Name of task */
  systask_f	*st_func;		/* Function implementing task */
  int		st_minArgs;		/* Minimum number of arguments */
  int		st_maxArgs;		/* Maximum number of arguments */
#if 0
  int		st_needContext;		/* Non-zero if we need to pass context */
  int		st_needNets;		/* Do we need net dependency info? */
#endif
  taskflag_t	st_flags;		/* Task flags */
  stargtype_t	st_argTypes[STASK_MAXSPECARGS];	/* Argument types */
} SysTaskDescript;


systask_f *SysTask_find(const char*);		/* Find named system task */
SysTaskDescript *SysTask_findEnt(const char*);	/* Find named system task */
const char *SysTask_findName(systask_f *);	/* Find task name from task function */

/*****************************************************************************
 * TaskContext member functions
 *****************************************************************************/
TaskContext *new_TaskContext(int nargs,Expr **args,ModuleInst *modCtx);
void delete_TaskContext(TaskContext *);
void TaskContext_setBlock(TaskContext *tc,CodeBlock *cb,ModuleInst *modCtx,unsigned start,unsigned stop);

#endif

