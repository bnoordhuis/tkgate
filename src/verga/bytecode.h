/*****************************************************************************
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

    Last edit by hansen on Thu Jan 29 10:04:52 2009
****************************************************************************/
#ifndef __bytecode_h
#define __bytecode_h
/*****************************************************************************
 *
 * This file implements the simulation byte code classes.  The basic byte code
 * oepration is handled by the ByteCode object. A sequence of instructions is
 * stored in an array in which instructions are executed ni sequence.  Each
 * thread is handled by a VGThread object which contains the current thread
 * state.  At any given time there may be a number of active threads and one
 * current thread.  Execution on the current thread continues until it is 
 * suspended for some reason either by executing an event wait or a delay.
 * At that time the next ative thread is invoked until it is suspened and so
 * on.  When there are no longer any active threads, simulation time is
 * advanced until more threads can become active.
 *
 * List of ByteCode instructions:
 *
 *   BCOpr		Execute an operation on Values.
 *   BCCopy		Copy a Value to another Value
 *   BCCopyRange	Copy a range of a Value to another Value
 *   BCMemFetch		Fetch a value from a memory
 *   BCMemPut		Put a value into a memory
 *   BCNbMemPut		Non-blocking store of value into a memory
 *   BCRaise		Raise an event
 *   BCAsgn		Assign a Value to a Net
 *   BCNbAsgnD		Non-blocking assign a Value to a reg Net after delay
 *   BCNbAsgnE		Non-blocking assign a Value to a reg Net after event
 *   BCWireAsgnD	Non-blocking assign a Value to a wire Net after delay
 *   BCEnd		End the current thread
 *   BCNoop		Do a No-op
 *   BCGoto		Conditional or unconditional jump to a new address
 *   BCTask		Execute a system task
 *   BCDelay		Suspend task and schedule wake up time
 *   BCTrigger		Suspend task and wait for a triggering event
 *   BCSpawn		Spawn a new bytecode task
 *   BCLock		Begin a critical section (wait for reg=0, then set reg=1)
 *   BCWait		Wait for child tasks to complete
 *   BCSubr		Jump to a subroutine
 *   BCReturn		Return from a subroutine
 *   BCDebugPrint	Print a string for debugging purposes
 *
 *****************************************************************************/

#define BCODE_BLOCKSIZE		64


/*****************************************************************************
 *
 * Thread states
 *
 *****************************************************************************/
#define TS_ACTIVE	0x0		/* Thread is actively executing */ 
#define TS_BLOCKED	0x1		/* Thread is blocked waiting for event */ 
#define TS_DISABLED	0x2		/* Thread is disabled */

/*****************************************************************************
 *
 * VGFrame - Represents a stack frame for a thread.
 *
 *****************************************************************************/
typedef struct VGFrame_str VGFrame;
struct VGFrame_str {
  ByteCode	*f_pc;		/* Instruction to which to return */
  VGFrame	*f_next;	/* Next address on stack */
};


/*****************************************************************************
 *
 * VGThread - Thread state
 *
 *****************************************************************************/
struct VGThread_str {
  Event		*t_pending;	/* Pointer to event if pending, null otherwise */
  int		t_state;	/* State of the thread (active, blocked, paused, etc.)  */
  int		t_isLive;	/* Non-zero if this thread is live */
  int		t_wait;		/* Suspend until this many threads end */
  CodeBlock	*t_start_block;	/* CodeBlock to use for starting thread */
  unsigned	t_start_pc;	/* Offset into starting thread for PC */
  ByteCode	*t_pc;		/* Thread program counter */
  ModuleInst	*t_modCtx;	/* Module context in which thread was initiated */
  ModuleItem	*t_mitem;	/* Module item of thead (if applicable) */
  int		t_numChild;	/* Number of running child tasks */
  VGThread	*t_parent;	/* Parent task */
  VGThread	*t_next;	/* Next pointer when we are in the active queue */
  VGFrame	*t_callStack;	/* Call stack for any calls to user tasks/functions */
};

/*****************************************************************************
 *
 * BCOpr - Do a basic 1 to 3 operand operation
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*o_func;	/* Function for */
  valueop_f	*o_op;		/* Operation function */
  Value		*o_dest;	/* Destination state */
  Value		*o_opr[3];	/* Operand states */
} BCOpr;

/*****************************************************************************
 *
 * BCMemFetch - Fetch a value from a memory
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*m_func;	/* Handler function */
  Net		*m_net;		/* Net for memory */
  Value		*m_addr;	/* Address we are operating on */
  Value		*m_data;	/* Value to store/retrieve from memory */ 
} BCMemFetch;

/*****************************************************************************
 *
 * BCMemPut - Put a value into memory
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*m_func;	/* Handler function */
  Net		*m_net;		/* Net for memory */
  Value		*m_addr;	/* Address we are operating on */
  Value		*m_netLsb;	/* LSB on net (memory) */ 
  Value		*m_data;	/* Value to store/retrieve from memory */ 
  unsigned	m_valLsb;	/* Lsb in value */
  unsigned	m_width;	/* Width of assignment */
} BCMemPut;

/*****************************************************************************
 *
 * BCNbMemPutD - Non-blocking store of a value into memory
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*m_func;	/* Handler function */
  Net		*m_net;		/* Net for memory */
  Value		*m_addr;	/* Address we are operating on */
  Value		*m_netLsb;	/* LSB on net (memory) */ 
  Value		*m_data;	/* Value to store/retrieve from memory */ 
  unsigned	m_valLsb;	/* Lsb in value */
  unsigned	m_width;	/* Width of assignment */
  deltatime_t	m_delay;	/* Delay for assignment */
} BCNbMemPutD;

/*****************************************************************************
 *
 * BCNbMemPutE - Non-blocking store of a value into memory
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*m_func;	/* Handler function */
  Net		*m_net;		/* Net for memory */
  Value		*m_addr;	/* Address we are operating on */
  Value		*m_netLsb;	/* LSB on net (memory) */ 
  Value		*m_data;	/* Value to store/retrieve from memory */ 
  unsigned	m_valLsb;	/* Lsb in value */
  unsigned	m_width;	/* Width of assignment */
  Trigger	*m_trigger;	/* Trigger for assignment */
} BCNbMemPutE;

/*****************************************************************************
 *
 * BCCopy - Copy a value with possible mismatch in bit size.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*c_func;	/* Function for */
  Value		*c_dst;		/* Destination state */
  Value		*c_src;		/* Source states */
} BCCopy;

/*****************************************************************************
 *
 * BCCopyRange - Copy a range of bits from a value.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*r_func;	/* Function for */
  Value		*r_dst;		/* Destination value */
  unsigned	r_dLsb;		/* LSB in destination */
  Value		*r_src;		/* Source value */
  Value		*r_sLsb;	/* Least significant bit in Net (null for full assignment) */
  unsigned	r_width;	/* Number of bits to assign */
} BCCopyRange;

/*****************************************************************************
 *
 * BCAsgn - Assign a value to a net and invoke any resulting actions
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*a_func;	/* Function for */
  Net		*a_net;		/* Destination net */
  Value		*a_value;	/* Value to assign */
  unsigned	a_width;	/* Number of bits to assign */
  unsigned	a_valLsb;	/* Least significant bit in Value */
  Value		*a_netLsb;	/* Least significant bit in Net (null for full assignment) */
} BCAsgn;

/*****************************************************************************
 *
 * BCRaise - Raise an event
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*r_func;	/* Function for */
  Net		*r_net;		/* Destination net */
} BCRaise;

/*****************************************************************************
 *
 * BCNbAsgnD - Non-blocking assign a value to a net and invoke any resulting
 * actions after a specified (possibly 0) delay.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*a_func;	/* Function for */
  Net		*a_net;		/* Destination net */
  Value		*a_value;	/* Value to assign */
  unsigned	a_width;	/* Number of bits to assign */
  unsigned	a_valLsb;	/* Least significant bit in Value */
  Value		*a_netLsb;	/* Least significant bit in Net (null for full assignment) */
  deltatime_t	a_delay;	/* Delay for assignment */
} BCNbAsgnD;

/*****************************************************************************
 *
 * BCNbAsgnE - Non-blocking assign a value to a net and invoke any resulting
 * actions after a triggering event.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*a_func;	/* Function for */
  Net		*a_net;		/* Destination net */
  Value		*a_value;	/* Value to assign */
  unsigned	a_width;	/* Number of bits to assign */
  unsigned	a_valLsb;	/* Least significant bit in Value */
  Value		*a_netLsb;	/* Least significant bit in Net (null for full assignment) */
  Trigger	*a_trigger;	/* Event trigger */
} BCNbAsgnE;

/*****************************************************************************
 *
 * BCWireAsgnD - Non-blocking assign a value to a wire net and invoke any
 * resulting actions after a specified (possibly 0) delay.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*a_func;	/* Function for */
  Net		*a_net;		/* Destination net */
  int		a_id;		/* Driver ID for net driver that is changing */
  Value		*a_value;	/* Value to assign */
  unsigned	a_width;	/* Number of bits to assign */
  unsigned	a_valLsb;	/* Least significant bit in Value */
  Value		*a_netLsb;	/* Least significant bit in Net (null for full assignment) */
  deltatime_t	a_delay;	/* Delay for assignment */
} BCWireAsgnD;

/*****************************************************************************
 *
 * BCWireAsgnDF - Same as BCWireAsgnD but assumes value and net are the
 * same bit size.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*a_func;	/* Function for */
  Net		*a_net;		/* Destination net */
  int		a_id;		/* Driver ID for net driver that is changing */
  Value		*a_value;	/* Value to assign */
  deltatime_t	a_delay;	/* Delay for assignment */
} BCWireAsgnDF;

/*****************************************************************************
 *
 * BCEnd - End execution of a thread
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*e_func;
} BCEnd;

/*****************************************************************************
 *
 * BCNoop - Null opperation
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*n_func;
} BCNoop;

/*****************************************************************************
 *
 * BCGoto - Conditional/unconditional goto
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*g_func;	/* Handler function */
  Value		*g_cond;	/* Condition or null for unconditional jump */
  CodeBlock	*g_block;	/* Destination block of jump */
  unsigned	g_offset;	/* Offset into block of jump */
  int		g_neg;		/* Jump on negative of condition */
} BCGoto;

/*****************************************************************************
 *
 * BCSpawn - Spawn a child thread
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*s_func;	/* Handler function */
  CodeBlock	*s_block;	/* Codeblock for child thread */
  unsigned	s_offset;	/* Offset for child thread */
} BCSpawn;

/*****************************************************************************
 *
 * BCWait - Suspend current thread until something interesting happends.
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*w_func;	/* Handler function */
} BCWait;

/*****************************************************************************
 *
 * BCTask - Invoke a task
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*t_func;	/* Handler function */
  systask_f	*t_task;	/* Task function */
  TaskContext	*t_context;	/* Task context if used */
  Value		*t_rvalue;	/* Return value for task */
  int		t_numArgs;	/* Number of arguments */
  void		**t_args;	/* Array of arguments */
} BCTask;

/*****************************************************************************
 *
 * BCDelay - Delay current task (corresponds to #n )
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*d_func;	/* Handler function */
  deltatime_t	d_delay;	/* Relative delay time */
} BCDelay;

/*****************************************************************************
 *
 * BCTrigger - Wait for a triggering event (corresponds to @(a) )
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*t_func;	/* Handler function */
  Trigger	*t_trigger;	/* Trigger event to wait for */
} BCTrigger;

/*****************************************************************************
 *
 * BCLock - Wait for semephore to be zero, set semaphore to 1 and advance
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*l_func;	/* Handler function */
  Trigger	*l_trigger;	/* Trigger indicating change in semephore value */
  Value		*l_value;	/* Value of semephore variable */
} BCLock;

/*****************************************************************************
 *
 * BCSubr - Jump to a subroutine
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*s_func;	/* Handler function */
  CodeBlock	*s_block;	/* Codeblock in which to jump to */
  unsigned	s_offset;	/* Offsets of instructions to jump to */
} BCSubr;

/*****************************************************************************
 *
 * BCReturn - Jump to a subroutine
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*r_func;
} BCReturn;

/*****************************************************************************
 *
 * BCSubr - Jump to a subroutine
 *
 *****************************************************************************/
typedef struct {
  BCfunc	*dp_func;
  char		*dp_message;	/* Message to print */
} BCDebugPrint;


/*****************************************************************************
 *
 * ByteCode - A single byte code instruction.
 *
 *****************************************************************************/
union ByteCode_union {
  BCfunc	*bc_func;	/* Handler function for operation */
  BCOpr		bc_opr;		/* Basic operation */
  BCEnd		bc_end;		/* End thread */
  BCNoop	bc_noop;	/* No-op */
  BCGoto	bc_goto;	/* Goto (conditional or unconditional) */
  BCSpawn	bc_spawn;	/* Spawn child thread */
  BCCopy	bc_copy;	/* Copy */
  BCCopyRange	bc_copyrange;	/* Copy of range */
  BCTask	bc_task;	/* Invoke a task */
  BCAsgn	bc_asgn;	/* Net assignment */
  BCRaise	bc_raise;	/* Raise an event */
  BCNbAsgnD	bc_nbasgnd;	/* Non-blocking reg net assignment (on delay) */
  BCNbAsgnE	bc_nbasgne;	/* Non-blocking reg net assignment (on trigger) */
  BCWireAsgnD	bc_wireasgnd;	/* Non-blocking wire net assignment (on trigger) */
  BCWait	bc_wait;	/* Suspend current thread */
  BCDelay	bc_delay;	/* Delay current task */
  BCTrigger	bc_trigger;	/* Suspend and wait for trigger */
  BCLock	bc_lock;	/* Lock/semephore */
  BCMemFetch	bc_memfetch;	/* Memory fetch */
  BCMemPut	bc_memput;	/* Memory store */
  BCNbMemPutD	bc_nbmemputd;	/* Non-blocking memory store (on delay) */
  BCNbMemPutE	bc_nbmempute;	/* Non-blocking memory store (on trigger) */
  BCSubr	bc_subr;	/* Jump to a subroutine */
  BCReturn	bc_ret;		/* Return from a subroutine */
  BCDebugPrint	bc_dbgprint;	/* Print a debugging message */
};

/*****************************************************************************
 *
 * CodeBlock - a block of bytecode instructions.
 *
 *****************************************************************************/
struct CodeBlock_str {
  int		cb_length;		/* Number of generated instructions */
  int		cb_nalloced;		/* Number of allocated entries */
  ModuleInst	*cb_module;		/* Module instance we are in */
  ByteCode	*cb_instructions;	/* Vector of instructions */
};

/*****************************************************************************
 * CodeBlock member functions
 *****************************************************************************/
CodeBlock *new_CodeBlock(ModuleInst *mi);
void delete_CodeBlock(CodeBlock *);
void CodeBlock_init(CodeBlock *cb,ModuleInst *mi);
void CodeBlock_uninit(CodeBlock *cb);
ByteCode *CodeBlock_nextEmpty(CodeBlock *cb);
#define CodeBlock_first(cb) (cb)->cb_instructions
#define CodeBlock_size(cb) (cb)->cb_length
#define CodeBlock_get(cb,offset) (&(cb)->cb_instructions[offset])
#define CodeBlock_getModuleInst(cb) ((cb)->cb_module)
void CodeBlock_close(CodeBlock*);
void CodeBlock_copy(CodeBlock *dst,unsigned dpos,CodeBlock *src,unsigned start,unsigned stop);

/*****************************************************************************
 * ByteCode member functions
 *****************************************************************************/
#define ByteCode_exec(bc,t) (*bc->bc_func)(bc,t)

/*****************************************************************************
 * BCEnd - member functions
 *****************************************************************************/
void BCEnd_init(ByteCode*);
void BCEnd_exec(BCEnd *bc,VGThread *t);

/*****************************************************************************
 * BCNoop - member functions
 *****************************************************************************/
void BCNoop_init(ByteCode*);
void BCNoop_exec(BCNoop *bc,VGThread *t);

/*****************************************************************************
 * BCOpr - member functions
 *****************************************************************************/
void BCOpr_init(ByteCode*,valueop_f*S,Value*r,Value*a,Value*b,Value*c);
void BCOpr_exec(BCOpr *bc,VGThread *t);

/*****************************************************************************
 * BCCopy - member functions
 *****************************************************************************/
void BCCopy_init(ByteCode *bc, Value*dst, Value*src);
void BCCopy_exec(BCCopy *bc, VGThread *t);

/*****************************************************************************
 * BCCopyRange - member functions
 *****************************************************************************/
void BCCopyRange_init(ByteCode *bc, Value *dst, unsigned dLsb,Value *src,Value *srcLsb, unsigned width);
void BCCopyRange_exec(BCCopyRange *bc, VGThread *t);

/*****************************************************************************
 * BCAsgn - member functions
 *****************************************************************************/
void BCAsgn_init(ByteCode *bc, Net *net, Value *netLsb, Value *value, unsigned valLsb, unsigned width);
void BCAsgn_exec(BCAsgn *bc, VGThread *t);

/*****************************************************************************
 * BCRaise - member functions
 *****************************************************************************/
void BCRaise_init(ByteCode *bc, Net *net);
void BCRaise_exec(BCRaise *br, VGThread *t);

/*****************************************************************************
 * BCNbAsgnD - member functions
 *****************************************************************************/
void BCNbAsgnD_init(ByteCode *bc, Net *net, Value *netLsb, Value *value, unsigned valLsb, unsigned width,deltatime_t delay);
void BCNbAsgnD_exec(BCNbAsgnD *bc, VGThread *t);

/*****************************************************************************
 * BCNbAsgnE - member functions
 *****************************************************************************/
void BCNbAsgnE_init(ByteCode *bc, Net *net, Value *netLsb, Value *value, unsigned valLsb, unsigned width,Trigger *trigger);
void BCNbAsgnE_exec(BCNbAsgnE *bc, VGThread *t);

/*****************************************************************************
 * BCWireAsgnD - member functions
 *****************************************************************************/
void BCWireAsgnD_init(ByteCode *bc, Net *net, int id, Value *netLsb, Value *value, unsigned valLsb, unsigned width,deltatime_t delay);
void BCWireAsgnD_exec(BCWireAsgnD *bc, VGThread *t);

/*****************************************************************************
 * BCGoto - member functions
 *****************************************************************************/
void BCGoto_init(ByteCode *g,Value*cond,int neg,CodeBlock *cb,unsigned offset);
void BCGoto_exec(BCGoto *g,VGThread *t);
#define BCGoto_setOffset(g,offset) ((g)->g_offset = (offset))

/*****************************************************************************
 * BCSpawn - member functions
 *****************************************************************************/
void BCSpawn_init(ByteCode *s,CodeBlock *cb,unsigned offset);
void BCSpawn_exec(BCSpawn *s,VGThread *t);
#define BCSpawn_setOffset(s,offset) ((s)->s_offset = (offset))

/*****************************************************************************
 * BCWait - member functions
 *****************************************************************************/
void BCWait_init(ByteCode *w);
void BCWait_exec(BCWait *w,VGThread *t);

/*****************************************************************************
 * BCTask - member functions
 *****************************************************************************/
void BCTask_init(ByteCode *g,systask_f*, TaskContext *tctx, Value *rval,int numArgs,void **args);
void BCTask_exec(BCTask *g,VGThread *t);

/*****************************************************************************
 * BCDelay - member functions
 *****************************************************************************/
void BCDelay_init(ByteCode *d,deltatime_t t);
void BCDelay_exec(BCDelay *d,VGThread *t);

/*****************************************************************************
 * BCTrigger - member functions
 *****************************************************************************/
void BCTrigger_init(ByteCode *bct,Trigger *t);
void BCTrigger_exec(BCTrigger *bct,VGThread *t);

/*****************************************************************************
 * BCLock - member functions
 *****************************************************************************/
void BCLock_init(ByteCode *bc,Trigger *t, Value *value);
void BCLock_exec(BCLock *lt,VGThread *t);

/*****************************************************************************
 * BCMemFetch - member functions
 *****************************************************************************/
void BCMemFetch_init(ByteCode *bct,Net *n, Value *addr, Value *data);
void BCMemFetch_exec(BCMemFetch *,VGThread *t);

/*****************************************************************************
 * BCMemPut - member functions
 *****************************************************************************/
void BCMemPut_init(ByteCode *bc,Net *n, Value *addr, Value *netLsb, Value *data,unsigned valLsb,unsigned width);
void BCMemPut_exec(BCMemPut *bct,VGThread *t);

/*****************************************************************************
 * BCNbMemPutD - member functions
 *****************************************************************************/
void BCNbMemPutD_init(ByteCode *bc,Net *n, Value *addr, Value *netLsb, Value *data,unsigned valLsb,unsigned width,deltatime_t t);
void BCNbMemPutD_exec(BCNbMemPutD *bct,VGThread *t);

/*****************************************************************************
 * BCNbMemPutE - member functions
 *****************************************************************************/
void BCNbMemPutE_init(ByteCode *bc,Net *n, Value *addr, Value *netLsb, Value *data,unsigned valLsb,unsigned width,Trigger *t);
void BCNbMemPutE_exec(BCNbMemPutE *bct,VGThread *t);

/*****************************************************************************
 * BCSubr - member functions
 *****************************************************************************/
void BCSubr_init(ByteCode *bc,CodeBlock *block,unsigned offset); 
void BCSubr_exec(BCSubr *s,VGThread *t);

/*****************************************************************************
 * BCReturn - member functions
 *****************************************************************************/
void BCReturn_init(ByteCode *bc); 
void BCReturn_exec(BCReturn *r,VGThread *t);

/*****************************************************************************
 * BCDebugPrint - member functions
 *****************************************************************************/
void BCDebugPrint_init(ByteCode *bc,char *msg,...); 
void BCDebugPrint_exec(BCDebugPrint *dp,VGThread *t);

/*****************************************************************************
 * VGFrame - member functions
 *****************************************************************************/
VGFrame *new_VGFrame(ByteCode *bc,VGFrame *next);
void delete_VGFrame(VGFrame *);

/*****************************************************************************
 * VGThread - member functions
 *****************************************************************************/
VGThread *new_VGThread(CodeBlock *cb,unsigned pc,ModuleInst *modCtx,ModuleItem *mitem);
void delete_VGThread(VGThread *);
void VGThread_init(VGThread *thread,CodeBlock *cb,unsigned pc,ModuleInst *modCtx,ModuleItem *mitem);
void VGThread_uninit(VGThread *thread);
void VGThread_exec(VGThread *thread);
void VGThread_delay(VGThread *thread,deltatime_t);
void VGThread_delayToEnd(VGThread *thread);
void VGThread_eventWait(VGThread *thread,Trigger *t);
void VGThread_start(VGThread *thread);
void VGThread_childEndNotify(VGThread *thread);
VGThread *VGThread_spawn(VGThread *parent,CodeBlock *cb,unsigned offset);
void VGThread_goto(VGThread *thread,CodeBlock *codeBlock,unsigned offset);
void VGThread_kill(VGThread *thread);
#define VGThread_getModCtx(thread) (thread)->t_modCtx
#define VGThread_getCircuit(thread) (thread)->t_modCtx->mc_circuit
#define VGThread_getQueue(thread) Circuit_getQueue((thread)->t_modCtx->mc_circuit)
#define VGThread_suspend(t) ((t)->t_state |= TS_BLOCKED)
#define VGThread_resume(t) ((t)->t_state &= ~TS_BLOCKED)
#define VGThread_enable(t) ((t)->t_state &= ~TS_DISABLED)
#define VGThread_disable(t) ((t)->t_state |= TS_DISABLED)
#define VGThread_isActive(t) ((t)->t_state == TS_ACTIVE)
#define VGThread_doNextInsruction(t) (*t->t_pc->bc_func)(t->t_pc,t)
#define VGThread_getMItem(t) (t)->t_mitem

#endif
