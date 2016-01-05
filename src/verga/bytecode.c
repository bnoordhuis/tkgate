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

    Last edit by hansen on Mon Feb  2 13:45:47 2009
****************************************************************************/
#include "thyme.h"

#define DEBUG 0
/*****************************************************************************
 *
 * Create a new CodeBlock
 *
 * Returns:		Newly created and initialized CodeBlock.
 *
 *****************************************************************************/
CodeBlock *new_CodeBlock(ModuleInst *mi)
{
  CodeBlock *cb = (CodeBlock *) malloc(sizeof(CodeBlock));
  CodeBlock_init(cb,mi);
  return cb;
}

/*****************************************************************************
 *
 * Delete a CodeBlock
 *
 * Parameters:
 *     cb		CodeBlock to be deleted.
 *
 *****************************************************************************/
void delete_CodeBlock(CodeBlock *cb)
{
  CodeBlock_uninit(cb);
  free(cb);
}

/*****************************************************************************
 *
 * Initialize a CodeBlock
 *
 * Parameters:
 *     cb		CodeBlock to be initialized.
 *
 *****************************************************************************/
void CodeBlock_init(CodeBlock *cb,ModuleInst *mi)
{
  cb->cb_length =  0;
  cb->cb_nalloced = BCODE_BLOCKSIZE;
  cb->cb_module = mi;
  cb->cb_instructions = (ByteCode*) malloc(sizeof(ByteCode)*BCODE_BLOCKSIZE);
}

/*****************************************************************************
 *
 * Resize a codeblock to use the minimum number of bytes.
 *
 *****************************************************************************/
void CodeBlock_close(CodeBlock *cb)
{
  ByteCode *oldBC = cb->cb_instructions;

  cb->cb_nalloced = cb->cb_length;
  cb->cb_instructions = (ByteCode*) malloc(sizeof(ByteCode)*cb->cb_nalloced);
  memcpy(cb->cb_instructions, oldBC, sizeof(ByteCode)*cb->cb_nalloced);
  free(oldBC);
}

/*****************************************************************************
 *
 * Uninitialize a CodeBlock
 *
 * Parameters:
 *     cb		CodeBlock to be uninitialized.
 *
 *****************************************************************************/
void CodeBlock_uninit(CodeBlock *cb)
{
  free(cb->cb_instructions);
}

/*****************************************************************************
 *
 * Allocate and return the next empty ByteCode in a CodeBlock
 *
 * Paramaters:
 *     cb		CodeBlock to use.
 *
 * Returns:		Next unused ByteCode in cb.
 *
 *****************************************************************************/
ByteCode *CodeBlock_nextEmpty(CodeBlock *cb)
{
  if (cb->cb_length >= cb->cb_nalloced) {
    cb->cb_nalloced += BCODE_BLOCKSIZE;
    cb->cb_instructions = (ByteCode*) realloc(cb->cb_instructions,sizeof(ByteCode)*cb->cb_nalloced);
  }

  return cb->cb_instructions + cb->cb_length++;
}

void CodeBlock_copy(CodeBlock *dst,unsigned dpos,CodeBlock *src,unsigned start,unsigned stop)
{
  unsigned reqLen = dpos + (stop-start+1);		/* Required length */

  if (reqLen >= dst->cb_nalloced) {
    dst->cb_nalloced = reqLen;
    dst->cb_instructions = (ByteCode*) realloc(dst->cb_instructions,sizeof(ByteCode)*dst->cb_nalloced);
  }

  memcpy(CodeBlock_get(dst,dpos),CodeBlock_get(src,start),sizeof(ByteCode)*(stop-start+1));
#if 0
  {
    int i;
    unsigned *ptr = (unsigned*) CodeBlock_get(dst,dpos);
    int n = sizeof(ByteCode)*(stop-start+1)/sizeof(unsigned);

    printf("copy %d <- %d..%d  n=%d\n",dpos,start,stop,n);
    printf("  dst:");
    for (i = 0;i <= n;i++) {
      printf("%08x",ptr[i]);
    }
    printf("\n");

    ptr = (unsigned*) CodeBlock_get(src,start);
    printf("  src:");
    for (i = 0;i <= n;i++) {
      printf("%08x",ptr[i]);
    }
    printf("\n");
  }
#endif
}



/*****************************************************************************
 *
 * BCEnd initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *
 *****************************************************************************/
void BCEnd_init(ByteCode *bc)
{
  bc->bc_func = (BCfunc*) BCEnd_exec;
}


/*****************************************************************************
 *
 * Execute BCEnd instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCEnd_exec(BCEnd *bc,VGThread *t)
{
#if DEBUG
  vgio_echo("%p: BCEnd\n",t);
#endif
  VGThread_kill(t);
  if (t->t_parent)
    VGThread_childEndNotify(t->t_parent);
}

/*****************************************************************************
 *
 * BCNoop initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize as an BCNoop
 *
 *****************************************************************************/
void BCNoop_init(ByteCode *bc)
{
  bc->bc_func = (BCfunc*) BCNoop_exec;
}

/*****************************************************************************
 *
 * Execute BCNoop instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCNoop_exec(BCNoop *bc,VGThread *t)
{
#if DEBUG
  vgio_echo("%p: BCNoop\n",t);
#endif
  t->t_pc++;
}

/*****************************************************************************
 *
 * BCOpr initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     func		Value function to execute
 *     r		Destination Value
 *     a,b,c		Source Value objects
 *
 *****************************************************************************/
void BCOpr_init(ByteCode *bc,valueop_f *func,Value*r,Value*a,Value*b,Value*c)
{
  if (!func)
    abort();

  bc->bc_func = (BCfunc*) BCOpr_exec;
  bc->bc_opr.o_op =  func;
  bc->bc_opr.o_dest = r;
  bc->bc_opr.o_opr[0] = a;
  bc->bc_opr.o_opr[1] = b;
  bc->bc_opr.o_opr[2] = c;
}

/*****************************************************************************
 *
 * Execute BCOpr instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCOpr_exec(BCOpr *bc,VGThread *t)
{
  (*bc->o_op)(bc->o_dest,bc->o_opr[0],bc->o_opr[1],bc->o_opr[2]);
  t->t_pc++;
#if DEBUG
  {
    OpDesc *od = OpDesc_findFunc(bc->o_op);
    const char *op = "[?]\0[?]";

    if (od) op = od->od_text;

    vgio_echo("%p: BCOpr: ",t);
    Value_print(bc->o_dest,stdout);
    vgio_echo(" = ");
    if (bc->o_opr[0]) Value_print(bc->o_opr[0],stdout);
    if (bc->o_opr[1]) {
      vgio_echo(" %s ",op);
      Value_print(bc->o_opr[1],stdout);
    }
    if (bc->o_opr[2]) {
      op = strend((char*)op) + 1;
      vgio_echo(" %s ",op);
      Value_print(bc->o_opr[2],stdout);
    }


    vgio_echo("\n");
  }
#endif
}

/*****************************************************************************
 *
 * BCGoto initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     cond		Condition value (or null for unconditional branch)
 *     neg		If non-zero, branch condition is revered
 *     cb		CodeBlock in which destination is contained
 *     offset		Offset into CodeBlock to branch to.
 *
 *****************************************************************************/
void BCGoto_init(ByteCode *bc, Value *cond,int neg,CodeBlock *cb,unsigned offset)
{
  BCGoto *g = (BCGoto*)bc;

  g->g_func = (BCfunc*) BCGoto_exec;
  g->g_cond = cond;
  g->g_block = cb;
  g->g_offset = offset;
  g->g_neg = neg;
}

/*****************************************************************************
 *
 * Execute BCGoto instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCGoto_exec(BCGoto *g,VGThread *t)
{
  int doskip = (g->g_cond && (Value_isZero(g->g_cond)||!Value_isValue(g->g_cond)));

  if (g->g_neg) doskip = !doskip;
  if (doskip) {
#if DEBUG
    vgio_echo("%p: BCGoto: nojump\n",t);
#endif
    t->t_pc++;
  } else {
#if DEBUG
    vgio_echo("%p: BCGoto: jump %p:%x\n",t,g->g_block,g->g_offset);
#endif
    t->t_pc = CodeBlock_first(g->g_block) + g->g_offset;
  }
}

/*****************************************************************************
 *
 * BCTask initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     func		System task function pointer
 *     context		Optional task context used by delayed execution tasks.
 *     numArgs		Number of arguments
 *     args		Array of values
 *
 *****************************************************************************/
void BCTask_init(ByteCode *bc,systask_f *func,TaskContext *context,Value *rval,int numArgs,void **args)
{
  BCTask *t = (BCTask*)bc;

  t->t_func = (BCfunc*) BCTask_exec;
  t->t_context = context;
  t->t_task = func;
  t->t_rvalue = rval;
  t->t_numArgs = numArgs;
  t->t_args = args;

}

/*****************************************************************************
 *
 * Execute BCTask instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCTask_exec(BCTask *t,VGThread *th)
{
#if DEBUG
  vgio_echo("%p: BCTask(%s) in %s\n",t,SysTask_findName(t->t_task),th->t_modCtx->mc_path);
#endif
  (*t->t_task)(th,t->t_rvalue,t->t_numArgs,t->t_args,t->t_context);
  th->t_pc++;
}

/*****************************************************************************
 *
 * BCDelay initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     t		Amount of delay
 *
 *****************************************************************************/
void BCDelay_init(ByteCode *bc,deltatime_t t)
{
  BCDelay *d = (BCDelay*)bc;

  d->d_func = (BCfunc*) BCDelay_exec;
  d->d_delay = t;
}

/*****************************************************************************
 *
 * Execute BCDelay instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCDelay_exec(BCDelay *d,VGThread *t)
{
  if (d->d_delay == 0)
    VGThread_delayToEnd(t);
  else
    VGThread_delay(t,d->d_delay);
#if DEBUG
  vgio_echo("%p: BCDelay %d\n",t,d->d_delay);
#endif
  t->t_pc++;
}

/*****************************************************************************
 *
 * BCTrigger initialization function
 *
 * Parameters:
 *     bct		ByteCode object to initialize
 *     t		Triggering event
 *
 *****************************************************************************/
void BCTrigger_init(ByteCode *bc,Trigger *t)
{
  BCTrigger *bct = (BCTrigger*)bc;

  bct->t_func = (BCfunc*) BCTrigger_exec;
  bct->t_trigger = t;
}

/*****************************************************************************
 *
 * Execute BCTrigger instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCTrigger_exec(BCTrigger *bct,VGThread *t)
{
#if DEBUG
  char buf[STRMAX],*p;
  ListElem *le;

  p = buf;
  for (le = List_first(bct->t_trigger->t_posedges);le;le = List_next(bct->t_trigger->t_posedges,le)) {
    Net *n = ListElem_obj(le);
    p += sprintf(p," %s",Net_getName(n));
  }
  vgio_echo("%p: BCTrigger on %s\n",t,buf);
#endif
  VGThread_eventWait(t,bct->t_trigger);
  t->t_pc++;
}

/*****************************************************************************
 *
 * BCLock initialization function
 *
 * Parameters:
 *     bct		ByteCode object to initialize
 *     t		Triggering event
 *     value		Semephore variable
 *
 *****************************************************************************/
void BCLock_init(ByteCode *bc,Trigger *t, Value *value)
{
  BCLock *bcl = (BCLock*)bc;

  bcl->l_func = (BCfunc*) BCLock_exec;
  bcl->l_trigger = t;
  bcl->l_value = value;
}

/*****************************************************************************
 *
 * Execute BCLock instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 * If the semephore variable is zero, we set it to one and advance the PC.  Otherwise
 * we wait for a change in the semephore value and reexecute ourself.
 *
 *****************************************************************************/
void BCLock_exec(BCLock *bcl,VGThread *t)
{
#if DEBUG
  char buf[STRMAX],*p;
  ListElem *le;

  p = buf;
  for (le = List_first(bcl->l_trigger->t_posedges);le;le = List_next(bct->t_trigger->t_posedges,le)) {
    Net *n = ListElem_obj(le);
    p += sprintf(p," %s",Net_getName(n));
  }
  vgio_echo("%p: BCLock on %s\n",t,buf);
#endif

  if (Value_isZero(bcl->l_value)) {
    Value_one(bcl->l_value);
    t->t_pc++;
  } else
    VGThread_eventWait(t,bcl->l_trigger);
}

/*****************************************************************************
 *
 * BCCopy initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     dst		Destination Value for copy
 *     src		Source Value for copy
 *
 *****************************************************************************/
void BCCopy_init(ByteCode *bc, Value*dst, Value*src)
{
  BCCopy *c = (BCCopy*)bc;

  c->c_func = (BCfunc*) BCCopy_exec;
  c->c_dst = dst;
  c->c_src = src;
}

/*****************************************************************************
 *
 * Execute BCCopy instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCCopy_exec(BCCopy *c, VGThread *t)
{
  int nd = Value_nbits(c->c_dst);
  int ns = Value_nbits(c->c_src);

  if (nd == ns)
    Value_copy(c->c_dst,c->c_src);
  else if (nd < ns)
    Value_copyRange(c->c_dst,0,c->c_src,ns-1,0);
  else {
    Value_zero(c->c_dst);
    Value_copyRange(c->c_dst,0,c->c_src,ns-1,0);
  }

#if DEBUG
  vgio_echo("%p: BCCopy: ",t);
  Value_print(c->c_dst,stdout);
  vgio_echo(" = ");
  Value_print(c->c_src,stdout);
  vgio_echo("\n");
#endif

  t->t_pc++;
}

/*****************************************************************************
 *
 * BCCopyRange initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     dst		Destination Value for copy
 *     dLsb		LSB in destination
 *     src		Source Value for copy
 *     sLSB		LSB in source
 *     width		Bits to copy
 *
 *****************************************************************************/
void BCCopyRange_init(ByteCode *bc, Value *dst, unsigned dLsb,Value *src,Value *sLsb, unsigned width)
{
  BCCopyRange *r = (BCCopyRange*)bc;

  r->r_func = (BCfunc*) BCCopyRange_exec;
  r->r_dst = dst;
  r->r_dLsb = dLsb;
  r->r_src = src;
  r->r_sLsb = sLsb;
  r->r_width = width;
}

void BCCopyRange_exec(BCCopyRange *r, VGThread *t)
{
  unsigned sLsb;

  if (!r->r_sLsb)
    sLsb = 0;
  else if (Value_toInt(r->r_sLsb,&sLsb) < 0) {
    /*
     * Source bit is unknown.
     */
    Value *x = new_Value(r->r_width);
    Value_unknown(x);
    Value_copyRange(r->r_dst, r->r_dLsb, x,r->r_width-1,0);
    delete_Value(x);
    t->t_pc++;

#if DEBUG
  vgio_echo("%p: BCCopyRange(",t);
  Value_print(r->r_dst, stdout);
  vgio_echo(", %d, ",r->r_dLsb);
  Value_print(r->r_src, stdout);
  vgio_echo(", [? +: %d])\n",r->r_width-1);
#endif

    return;
  }

  /*
   * Normal case
   */
  Value_copyRange(r->r_dst, r->r_dLsb, r->r_src,sLsb+r->r_width-1,sLsb);
  t->t_pc++;

#if DEBUG
  vgio_echo("%p: BCCopyRange(",t);
  Value_print(r->r_dst, stdout);
  vgio_echo(", %d, ",r->r_dLsb);
  Value_print(r->r_src, stdout);
  vgio_echo(", [%d:%d])\n",sLsb+r->r_width-1,sLsb);
#endif
}

/*****************************************************************************
 *
 * Initialize a BCMemFetch instruction
 *
 *****************************************************************************/
void BCMemFetch_init(ByteCode *bc,Net *n, Value *addr, Value *data)
{
  BCMemFetch *m = (BCMemFetch*)bc;

  m->m_func = (BCfunc*) BCMemFetch_exec;
  m->m_net = n;
  m->m_addr = addr;
  m->m_data = data;
}

/*****************************************************************************
 *
 * Executete a BCMemFetch instruction
 *
 *****************************************************************************/
void BCMemFetch_exec(BCMemFetch *bmo,VGThread *t)
{
  Memory *m  = &bmo->m_net->n_data.memory;
  unsigned addr;

  if (Value_toInt(bmo->m_addr,&addr) < 0) {
    Value_unknown(bmo->m_data);
#if DEBUG
  vgio_echo("%p: BCMemFetch: %s[?]=?",t,Net_getName(bmo->m_net));
#endif
  } else {
    Memory_setFlags(m, MF_INITIALIZED);
    Memory_get(m, addr, bmo->m_data);
    Memory_accessNotify(m, addr, 0);
#if DEBUG
    vgio_echo("%p: BCMemFetch: %s[%x]=",t,Net_getName(bmo->m_net), addr);
    Value_print(bmo->m_data, stdout);
    vgio_printf("\n");
#endif
  }

  t->t_pc++;
}

/*****************************************************************************
 *
 * Initialize a BCNbMemPut instruction
 *
 *****************************************************************************/
void BCNbMemPutD_init(ByteCode *bc,Net *n, Value *addr, Value *netLsb, Value *data,
		     unsigned valLsb,unsigned width,deltatime_t delay)
{
  BCNbMemPutD *m = (BCNbMemPutD*)bc;

  m->m_func = (BCfunc*) BCNbMemPutD_exec;
  m->m_net = n;
  m->m_addr = addr;
  m->m_netLsb = netLsb;
  m->m_data = data;
  m->m_valLsb = valLsb;
  m->m_width = width;
  m->m_delay = delay;
}

/*****************************************************************************
 *
 * Executete a BCNbMemPut instruction
 *
 *****************************************************************************/
void BCNbMemPutD_exec(BCNbMemPutD *mpd,VGThread *thread)
{
  unsigned netLsb = 0;
  EvQueue *Q = VGThread_getQueue(thread);
  Event *e;

#if DEBUG
  vgio_echo("%p: BCNbMemPutD(%p) #%d\n",thread,mpd,mpd->m_delay);
#endif

  if (mpd->m_netLsb) {
    if (Value_toInt(mpd->m_netLsb,&netLsb) < 0) {
      /*
       * An LSB was specified but it contains unknown bits
       */
      Value *xs = new_Value(Value_nbits(mpd->m_data));
      Value_unknown(xs);
      e = new_EvMem(mpd->m_net, mpd->m_addr, netLsb, xs, mpd->m_valLsb+mpd->m_width-1,mpd->m_valLsb);
      EvQueue_enqueueAfter(Q,e,mpd->m_delay);
      delete_Value(xs);
      thread->t_pc++;
      return;
    }
  } else
    netLsb = 0;

  e = new_EvMem(mpd->m_net, mpd->m_addr, netLsb, mpd->m_data, mpd->m_valLsb+mpd->m_width-1,mpd->m_valLsb);
  EvQueue_enqueueAfter(Q,e,mpd->m_delay);
  thread->t_pc++;
}

/*****************************************************************************
 *
 * Initialize a BCNbMemPut instruction
 *
 *****************************************************************************/
void BCNbMemPutE_init(ByteCode *bc,Net *n, Value *addr, Value *netLsb, Value *data,
		     unsigned valLsb,unsigned width,Trigger *trigger)
{
  BCNbMemPutE *m = (BCNbMemPutE*)bc;

  m->m_func = (BCfunc*) BCNbMemPutE_exec;
  m->m_net = n;
  m->m_addr = addr;
  m->m_netLsb = netLsb;
  m->m_data = data;
  m->m_valLsb = valLsb;
  m->m_width = width;
  m->m_trigger = trigger;
}

/*****************************************************************************
 *
 * Executete a BCNbMemPut instruction
 *
 *****************************************************************************/
void BCNbMemPutE_exec(BCNbMemPutE *mpe,VGThread *thread)
{
  unsigned netLsb = 0;
  EvQueue *Q = VGThread_getQueue(thread);
  Event *e;

#if DEBUG
  vgio_echo("%p: BCNbMemPutD(%p) #%d\n",thread,mpe,mpe->m_delay);
#endif

  if (mpe->m_netLsb) {
    if (Value_toInt(mpe->m_netLsb,&netLsb) < 0) {
      /*
       * An LSB was specified but it contains unknown bits
       */
      Value *xs = new_Value(Value_nbits(mpe->m_data));
      Value_unknown(xs);
      e = new_EvMem(mpe->m_net, mpe->m_addr, netLsb, xs, mpe->m_valLsb+mpe->m_width-1,mpe->m_valLsb);
      Trigger_enqueue(mpe->m_trigger, e);
      delete_Value(xs);
      thread->t_pc++;
      return;
    }
  } else
    netLsb = 0;

  e = new_EvMem(mpe->m_net, mpe->m_addr, netLsb, mpe->m_data, mpe->m_valLsb+mpe->m_width-1,mpe->m_valLsb);
  Trigger_enqueue(mpe->m_trigger, e);
  thread->t_pc++;
}

/*****************************************************************************
 *
 * Initialize a BCMemPut instruction
 *
 *****************************************************************************/
void BCMemPut_init(ByteCode *bc,Net *n, Value *addr, Value *netLsb, Value *data,unsigned valLsb,unsigned width)
{
  BCMemPut *m = (BCMemPut*)bc;

  m->m_func = (BCfunc*) BCMemPut_exec;
  m->m_net = n;
  m->m_addr = addr;
  m->m_netLsb = netLsb;
  m->m_data = data;
  m->m_valLsb = valLsb;
  m->m_width = width;
}

/*****************************************************************************
 *
 * Executete a BCMemPut instruction
 *
 *****************************************************************************/
void BCMemPut_exec(BCMemPut *bmo,VGThread *t)
{
  Memory *m  = &bmo->m_net->n_data.memory;
  unsigned addr;
  unsigned netLsb = 0;

  if (Value_toInt(bmo->m_addr,&addr) < 0) {
#if DEBUG
    vgio_echo("%p: BCMemPut: %s[?]=?",t,Net_getName(bmo->m_net));
#endif
    if ((Memory_getFlags(m) & MF_INITIALIZED))
      errorRun(ERR_MEMADDR,Net_getName(bmo->m_net));
    goto done;
  } else {
    Memory_setFlags(m, MF_INITIALIZED);
    if (bmo->m_netLsb && Value_toInt(bmo->m_netLsb,&netLsb) < 0) {
      errorRun(ERR_MEMBITS,Net_getName(bmo->m_net));
      goto done;
    } else if (Net_nbits(bmo->m_net) == bmo->m_width) {
      Net_memSet(bmo->m_net, addr, bmo->m_data);
    } else
      Net_memSetRange(bmo->m_net, addr, netLsb, bmo->m_data, bmo->m_valLsb+bmo->m_width-1, bmo->m_valLsb);
  }

#if DEBUG
  vgio_echo("%p: BCMemPut: %s[%x][%d:%d]=",t,Net_getName(bmo->m_net), addr,bmo->m_width+netLsb-1,netLsb);
  Value_print(bmo->m_data, stdout);
  vgio_printf("[%d:%d]\n",bmo->m_width+bmo->m_valLsb-1,bmo->m_valLsb);
#endif
 done:
  t->t_pc++;
}

/*****************************************************************************
 *
 * BCRaise initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     net		Destination Net for assignment
 *
 *****************************************************************************/
void BCRaise_init(ByteCode *bc, Net *net)
{
  BCRaise *r = (BCRaise*)bc;

  r->r_func = (BCfunc*) BCRaise_exec;
  r->r_net = net;
}

/*****************************************************************************
 *
 * Execute BCRaise instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCRaise_exec(BCRaise *r, VGThread *t)
{
  Net_set(r->r_net, 0);

#if DEBUG
  vgio_echo("%p: BCRaise: %s\n",t,r->r_net->n_name);
#endif

  t->t_pc++;
}

/*****************************************************************************
 *
 * BCAsgn initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     net		Destination Net for assignment
 *     netLsb		Least significant bit in assignment
 *     value		Value to assign
 *     valLsb		Least significant bit in value
 *     width		Width of value to copy
 *
 *****************************************************************************/
void BCAsgn_init(ByteCode *bc, Net *net, Value *netLsb, Value *value, unsigned valLsb, unsigned width)
{
  BCAsgn *a = (BCAsgn*)bc;

  a->a_func = (BCfunc*) BCAsgn_exec;
  a->a_net = net;
  a->a_netLsb = netLsb;
  a->a_value = value;
  a->a_valLsb = valLsb;
  a->a_width = width;
}

/*****************************************************************************
 *
 * Execute BCAsgn instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCAsgn_exec(BCAsgn *a, VGThread *t)
{
  int nd = Net_nbits(a->a_net);
  int ns = Value_nbits(a->a_value);
  unsigned netLsb = 0;

  if (nd == a->a_width && ns == a->a_width) {
    Net_set(a->a_net, a->a_value);
  } else {

    /*
     * If an lsb is specified but it is unknown, set the target net to unknown
     * since we do not know where we are writing the value.
     */
    if (a->a_netLsb) {
      if (Value_toInt(a->a_netLsb,&netLsb) < 0) {
	Net_makeUnknown(a->a_net);
	return;
      }
    } else
      netLsb = 0;

    Net_setRange(a->a_net, netLsb,a->a_value,a->a_valLsb+a->a_width-1,a->a_valLsb);
  }

#if DEBUG
  vgio_echo("%p: BCAsgn: %s[%d:%d] = ",t,a->a_net->n_name,netLsb+a->a_width-1,netLsb);
  Value_print(a->a_value,stdout);
  vgio_echo("[%d:%d]\n",a->a_valLsb+a->a_width-1,a->a_valLsb);
#endif

  t->t_pc++;
}

/*****************************************************************************
 *
 * BCNbAsgnD initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     net		Destination Net for assignment
 *     netLsb		Least significant bit in assignment
 *     value		Value to assign
 *     valLsb		Least significant bit in value
 *     width		Width of value to copy
 *     delay		Delay after which to queue assignment.
 *
 *****************************************************************************/
void BCNbAsgnD_init(ByteCode *bc, Net *net, Value *netLsb, Value *value, 
		    unsigned valLsb, unsigned width,deltatime_t delay)
{
  BCNbAsgnD *a = (BCNbAsgnD*)bc;

  a->a_func = (BCfunc*) BCNbAsgnD_exec;
  a->a_net = net;
  a->a_netLsb = netLsb;
  a->a_value = value;
  a->a_valLsb = valLsb;
  a->a_width = width;
  a->a_delay = delay;
}

/*****************************************************************************
 *
 * Execute BCNbAsgnD instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCNbAsgnD_exec(BCNbAsgnD *a, VGThread *thread)
{
  unsigned netLsb = 0;
  EvQueue *Q = VGThread_getQueue(thread);
  Event *e;

#if DEBUG
  vgio_echo("%p: BCNbAsgnD(%p) #%d\n",thread,a,a->a_delay);
#endif

  if (a->a_netLsb) {
    if (Value_toInt(a->a_netLsb,&netLsb) < 0) {
      /*
       * An LSB was specified but it contains unknown bits
       */
      Value *xs = new_Value(Value_nbits(a->a_value));
      Value_unknown(xs);
      e = new_EvNet(a->a_net, netLsb, xs, a->a_valLsb+a->a_width-1,a->a_valLsb);
      EvQueue_enqueueAfter(Q,e,a->a_delay);
      delete_Value(xs);
      thread->t_pc++;
      return;
    }
  } else
    netLsb = 0;

  e = new_EvNet(a->a_net, netLsb, a->a_value, a->a_valLsb+a->a_width-1,a->a_valLsb);
  EvQueue_enqueueAfter(Q,e,a->a_delay);
  thread->t_pc++;
}


/*****************************************************************************
 *
 * BCNbAsgnE initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     net		Destination Net for assignment
 *     netLsb		Least significant bit in assignment
 *     value		Value to assign
 *     valLsb		Least significant bit in value
 *     width		Width of value to copy
 *     trigger		Triggering event for assigment.
 *
 *****************************************************************************/
void BCNbAsgnE_init(ByteCode *bc, Net *net, Value *netLsb, Value *value, unsigned valLsb, unsigned width,Trigger *trigger)
{
  BCNbAsgnE *a = (BCNbAsgnE*)bc;

  a->a_func = (BCfunc*) BCNbAsgnE_exec;
  a->a_net = net;
  a->a_netLsb = netLsb;
  a->a_value = value;
  a->a_valLsb = valLsb;
  a->a_width = width;
  a->a_trigger = trigger;
}

/*****************************************************************************
 *
 * Execute BCNbAsgnE instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCNbAsgnE_exec(BCNbAsgnE *a, VGThread *thread)
{
  unsigned netLsb = 0;
  Event *e;

#if DEBUG
  vgio_echo("%p: BCNbAsgnE\n",thread);
#endif

  if (a->a_netLsb) {
    if (Value_toInt(a->a_netLsb,&netLsb) < 0) {
      /*
       * An LSB was specified but it contains unknown bits
       */
      Value *xs = new_Value(Value_nbits(a->a_value));
      Value_unknown(xs);
      e = new_EvNet(a->a_net, netLsb, xs, a->a_valLsb+a->a_width-1,a->a_valLsb);
      Trigger_enqueue(a->a_trigger ,e);
      delete_Value(xs);
      thread->t_pc++;
      return;
    }
  } else
    netLsb = 0;

  e = new_EvNet(a->a_net, netLsb, a->a_value, a->a_valLsb+a->a_width-1,a->a_valLsb);
  Trigger_enqueue(a->a_trigger ,e);
  thread->t_pc++;
}

/*****************************************************************************
 *
 * BCWireAsgnD initialization function
 *
 * Parameters:
 *     bc		ByteCode object to initialize
 *     net		Destination Net for assignment
 *     netLsb		Least significant bit in assignment
 *     value		Value to assign
 *     valLsb		Least significant bit in value
 *     width		Width of value to copy
 *     delay		Delay after which to queue assignment.
 *
 *****************************************************************************/
void BCWireAsgnD_init(ByteCode *bc, Net *net, int id, Value *netLsb, Value *value, 
		    unsigned valLsb, unsigned width,deltatime_t delay)
{
  BCWireAsgnD *a = (BCWireAsgnD*)bc;

  a->a_func = (BCfunc*) BCWireAsgnD_exec;
  a->a_net = net;
  a->a_id = id;
  a->a_netLsb = netLsb;
  a->a_value = value;
  a->a_valLsb = valLsb;
  a->a_width = width;
  a->a_delay = delay;
}

/*****************************************************************************
 *
 * Execute BCWireAsgnD instruction
 *
 * Parameters:
 *     bc		ByteCode object to execute
 *     t		Thread that is executing instruction
 *
 *****************************************************************************/
void BCWireAsgnD_exec(BCWireAsgnD *a, VGThread *thread)
{
  unsigned netLsb = 0;
  EvQueue *Q = VGThread_getQueue(thread);
  Event *e;

  if (a->a_netLsb) {
    if (Value_toInt(a->a_netLsb,&netLsb) < 0) {
      Value *xs = new_Value(Value_nbits(a->a_value));
      Value_unknown(xs);
      e = new_EvNet(a->a_net, netLsb, xs, a->a_valLsb+a->a_width-1,a->a_valLsb);
      EvQueue_enqueueAfter(Q,e,a->a_delay);

#if DEBUG
      vgio_echo("%p: BCWireAsgnD: #%d %s[%d:%d] = ",thread,a->a_delay,Net_getName(a->a_net),a->a_valLsb+a->a_width-1);
      Value_print(xs,stdout);
      vgio_echo("\n");
#endif

      delete_Value(xs);
      thread->t_pc++;
      return;
    }
  } else {
    netLsb = 0;
  }

  e = new_EvDriver(a->a_net, a->a_id, netLsb, a->a_value, a->a_valLsb+a->a_width-1,a->a_valLsb);
  EvQueue_enqueueAfter(Q,e,a->a_delay);

#if DEBUG
  vgio_echo("%p: BCWireAsgnD: #%d %s = ",thread,a->a_delay,Net_getName(a->a_net));
  Value_print(a->a_value,stdout);
  vgio_echo("\n");
#endif

  thread->t_pc++;
}

/*****************************************************************************
 *
 * Initialize a BCSpawn instruction.
 *
 *****************************************************************************/
void BCSpawn_init(ByteCode *bc,CodeBlock *cb,unsigned offset)
{
  BCSpawn *s = (BCSpawn*)bc;

  s->s_func = (BCfunc*) BCSpawn_exec;
  s->s_block = cb;
  s->s_offset = offset;
}

/*****************************************************************************
 *
 * Execute a BCSpawn instruction.
 *
 *****************************************************************************/
void BCSpawn_exec(BCSpawn *s,VGThread *thread)
{
  VGThread *child = VGThread_spawn(thread,s->s_block,s->s_offset);
  VGThread_start(child);
  VGThread_delay(child,0);
  thread->t_pc++;

#if DEBUG
  vgio_echo("%p: BCSpawn(%u) -> %p\n",thread,s->s_offset,child);
#endif
}

/*****************************************************************************
 *
 * Initialize a BCWait instruction.
 *
 *****************************************************************************/
void BCWait_init(ByteCode *bc)
{
  BCWait *w = (BCWait*)bc;

  w->w_func = (BCfunc*) BCWait_exec;
}

/*****************************************************************************
 *
 * Execute a BCWait instruction.
 *
 *****************************************************************************/
void BCWait_exec(BCWait *w,VGThread *thread)
{
  VGThread_suspend(thread);
  thread->t_pc++;
#if DEBUG
  vgio_echo("%p: BCWait()\n",thread);
#endif
}

/*****************************************************************************
 *
 * Initialize a BCSubr instruction
 *
 *****************************************************************************/
void BCSubr_init(ByteCode *bc,CodeBlock *block,unsigned offset)
{
  BCSubr *s = (BCSubr*)bc;

  s->s_func = (BCfunc*) BCSubr_exec;
  s->s_block = block;
  s->s_offset = offset;
}

/*****************************************************************************
 *
 * Execute a BCSubr instruction
 *
 *****************************************************************************/
void BCSubr_exec(BCSubr *s,VGThread *t)
{
#if DEBUG
  vgio_echo("%p: BCSubr: jump %p:%x\n",t,s->s_block,s->s_offset);
#endif
  t->t_callStack = new_VGFrame(t->t_pc+1, t->t_callStack);
  t->t_pc = CodeBlock_first(s->s_block) + s->s_offset;
}

/*****************************************************************************
 *
 * Initialize a BCReturn instruction
 *
 *****************************************************************************/
void BCReturn_init(ByteCode *bc)
{
  BCReturn *r = (BCReturn*)bc;

  r->r_func = (BCfunc*) BCReturn_exec;
}

/*****************************************************************************
 *
 * Execute a BCReturn instruction
 *
 *****************************************************************************/
void BCReturn_exec(BCReturn *r,VGThread *t)
{
  VGFrame *vgf = t->t_callStack;

#if DEBUG
  vgio_echo("%p: BCReturn\n",t);
#endif

  if (!vgf) {
    errorRun(ERR_IE_RETURN);

    VGThread_kill(t);
    if (t->t_parent)
      VGThread_childEndNotify(t->t_parent);
    return;
  }

  t->t_callStack = vgf->f_next;
  t->t_pc = vgf->f_pc;
  delete_VGFrame(vgf);
}

/*****************************************************************************
 *
 * Initialize a BCDebugPrint instruction
 *
 *****************************************************************************/
void BCDebugPrint_init(ByteCode *bc,char *msg,...)
{
  BCDebugPrint *dp = (BCDebugPrint*)bc;
  char buf[STRMAX];
  va_list ap;

  va_start(ap, msg);
  vsprintf(buf,msg,ap);
  va_end(ap);

  dp->dp_func = (BCfunc*) BCDebugPrint_exec;
  dp->dp_message = strdup(buf);
  
}

/*****************************************************************************
 *
 * Execute a BCDebugPrint instruction
 *
 *****************************************************************************/
void BCDebugPrint_exec(BCDebugPrint *dp,VGThread *t)
{
#if DEBUG
  vgio_echo("%p: BCDebugPrint()\n",t);
#endif
  printf("%s",dp->dp_message);
  t->t_pc++;
}



/*****************************************************************************
 *
 * Create a new VGThread object.
 *
 * Parameters:
 *     cb		Codeblock in which to begin execution
 *     pc		Thread program counter
 *     modCtx		Module context in which thread was declared.
 *     mitem		Module item this thread manages if applicable
 *
 *****************************************************************************/
VGThread *new_VGThread(CodeBlock *cb,unsigned pc,ModuleInst *modCtx,ModuleItem *mitem)
{
  VGThread *thread = (VGThread *) malloc(sizeof(VGThread));

  VGThread_init(thread,cb,pc,modCtx,mitem);

  return thread;
}

/*****************************************************************************
 *
 * Destroy a VGThread object.
 *
 * Parameters:
 *     thread		VGThread object to be destroyed.
 *
 *****************************************************************************/
void delete_VGThread(VGThread *thread)
{
  VGThread_uninit(thread);
  free(thread);
}

/*****************************************************************************
 *
 * Constructor for VGThread objects.
 *
 * Parameters:
 *     thread		Thread to be constructed.
 *     cb		Codeblock in which to begin execution
 *     pc		Thread program counter
 *     modCtx		Module context in which thread was declared.
 *     mitem		Module item this thread manages if applicable
 *
 *****************************************************************************/
void VGThread_init(VGThread *thread,CodeBlock *cb,unsigned pc, ModuleInst *modCtx,ModuleItem *mitem)
{
  thread->t_pending = 0;
  thread->t_state = TS_ACTIVE;
  thread->t_isLive = 1;
  thread->t_wait = 0;
  thread->t_start_block = cb;
  thread->t_start_pc = pc;
  thread->t_pc = 0;
  thread->t_modCtx = modCtx;
  thread->t_mitem = mitem;
  thread->t_next = 0;
  thread->t_numChild = 0;
  thread->t_parent = 0;
  thread->t_callStack = 0;
}

void VGThread_start(VGThread *thread)
{
  thread->t_pc = CodeBlock_first(thread->t_start_block) + thread->t_start_pc;
}

void VGThread_kill(VGThread *t)
{
  /*  vgio_echo("killing thread %p.\n",t);*/
  t->t_pending = 0;
  t->t_state = TS_BLOCKED;
  t->t_isLive = 0;

  if (t->t_mitem)
    ModuleItem_killNotify(t->t_mitem);
}


/*****************************************************************************
 *
 * Spawn a child thread.
 *
 * Parameters:
 *     parent		Parent thread initiating the spawn
 *     cb		Codeblock to execute in
 *     offset		Offset of starting address.
 *
 *****************************************************************************/
VGThread *VGThread_spawn(VGThread *parent,CodeBlock *cb,unsigned offset)
{
  VGThread *child = new_VGThread(cb, offset, parent->t_modCtx, parent->t_mitem);

  child->t_parent = parent;
  parent->t_numChild++;

  return child;
}

/*****************************************************************************
 *
 * Receive notification that a child thread has terminated.
 *
 *****************************************************************************/
void VGThread_childEndNotify(VGThread *thread)
{
  if (--thread->t_numChild == 0 && !VGThread_isActive(thread)) {
    VGThread_delay(thread,0);
  }
}

/*****************************************************************************
 *
 * Uninitialize a VGThread object.
 *
 * Parameters:
 *     thread		VGThread object to be destroyed.
 *
 *****************************************************************************/
void VGThread_uninit(VGThread *thread)
{
  /* nothing to do now - placeholder for future use. */
}

void VGThread_goto(VGThread *thread,CodeBlock *codeBlock,unsigned offset)
{
  thread->t_pc = CodeBlock_get(codeBlock,offset);
}


/*****************************************************************************
 *
 * Execute a thread until it is suspended.
 *
 * Parameters:
 *     thread		Thread to execute.
 *
 *****************************************************************************/
void VGThread_exec(VGThread *thread)
{
  VGThread_resume(thread);
  while (VGThread_isActive(thread)) {
    if (do_input_check) {
      EvQueue *Q = VGThread_getQueue(thread);
      VGThread_suspend(thread);
      EvQueue_enqueueAtHead(Q,new_EvThread(thread));
    } else
      VGThread_doNextInsruction(thread);
  }
}

/*****************************************************************************
 *
 * Suspend execution of a thread for t time units.
 *
 * Parameters:
 *     thread		Thread to be suspended.
 *     delay		Number of epochs to wait before resuming thread.
 *
 *****************************************************************************/
void VGThread_delay(VGThread *thread,deltatime_t delay)
{
  EvQueue *Q = VGThread_getQueue(thread);

  VGThread_suspend(thread);
  EvQueue_enqueueAfter(Q,new_EvThread(thread), delay);
}

/*****************************************************************************
 *
 * Suspend execution of a thread until end of epoch .
 *
 * Parameters:
 *     thread		Thread to be suspended.
 *
 *****************************************************************************/
void VGThread_delayToEnd(VGThread *thread)
{
  EvQueue *Q = VGThread_getQueue(thread);

  VGThread_suspend(thread);
  EvQueue_enqueueInactive(Q,new_EvThread(thread));
}

/*****************************************************************************
 *
 * Suspend execution of a thread until a triggering event occurs.
 *
 * Parameters:
 *     thread		Thread to be suspended
 *     trigger		Trigger on which to wait.
 *
 *****************************************************************************/
void VGThread_eventWait(VGThread *thread,Trigger *trigger)
{
  VGThread_suspend(thread);
  Trigger_enqueue(trigger,new_EvThread(thread));
}

/*****************************************************************************
 *
 * Create a thread stack frame object
 *
 * Parameters:
 *     bc		Stack frame address
 *     next		Next element on stack frame
 *
 *****************************************************************************/
VGFrame *new_VGFrame(ByteCode *bc,VGFrame *next)
{
  VGFrame *vgf = (VGFrame *) malloc(sizeof(VGFrame));

  vgf->f_pc = bc;
  vgf->f_next = next;

  return vgf;
}

/*****************************************************************************
 *
 * Destroy a thread stack frame object
 *
 *****************************************************************************/
void delete_VGFrame(VGFrame *vgf)
{
  free(vgf);
}
