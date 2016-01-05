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
****************************************************************************/
#include <ctype.h>
#include "thyme.h"

static void SysTask_monitoroff(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_monitoron(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_monitor(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_fmonitor(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_probe(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_unprobe(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_display(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_error(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_fdisplay(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_strobe(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_fstrobe(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_finish(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_stop(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_time(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_stime(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_random(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_readmemb(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_readmemh(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_writememb(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_writememh(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_command(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_wait(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_waituntil(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_systime(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_send(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_recv(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_exec(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_reportBreak(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_post(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_fopen(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_fclose(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_write(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_fwrite(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_setup(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_hold(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_width(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);
static void SysTask_tkg_zoom(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext);

SysTaskDescript sysTaskTable[] = {
  /*Name		Function		min	max	ctx?	nets? */
  /*Name		Function		min	max	flags */
  {"$display",		SysTask_display,	0,	NOLIM,	0},
  {"$error",		SysTask_error,		0,	NOLIM,	0},
  {"$finish",		SysTask_finish,		0,	0,	0},
  {"$fclose",		SysTask_fclose,		1,	1,	0},
  {"$fdisplay",		SysTask_fdisplay,	1,	NOLIM,	0},
  {"$fmonitor",		SysTask_fmonitor,	1,	NOLIM,	STF_NEEDCTX|STF_NEEDNETS},
  {"$fopen",		SysTask_fopen,		1,	1,	0},
  {"$fstrobe",		SysTask_fstrobe,	1,	NOLIM,	STF_NEEDCTX},
  {"$fwrite",		SysTask_fwrite,		0,	NOLIM,	},
  {"$hold",		SysTask_hold,		3,	4,	STF_SPECIFY|STF_NEEDCTX|STF_NEEDNETS, {TAT_TRIGGER,TAT_TRIGGER,TAT_VALUE,TAT_NET}},
  {"$monitor",		SysTask_monitor,	1,	NOLIM,	STF_NEEDCTX|STF_NEEDNETS},
  {"$monitoroff",	SysTask_monitoroff,	0,	0,	0},
  {"$monitoron",	SysTask_monitoron,	0,	0,	0},
  {"$random",		SysTask_random,		0,	1,	0},
  {"$readmemb",		SysTask_readmemb,	1,	3,	0, {TAT_VALUE,TAT_NET,TAT_VALUE}},
  {"$readmemh",		SysTask_readmemh,	1,	3,	0, {TAT_VALUE,TAT_NET,TAT_VALUE}},
  {"$setup",		SysTask_setup,		3,	4,	STF_SPECIFY|STF_NEEDCTX|STF_NEEDNETS, {TAT_TRIGGER,TAT_TRIGGER,TAT_VALUE,TAT_NET}},
  {"$stime",		SysTask_stime,		0,	0,	0},
  {"$stop",		SysTask_stop,		0,	1,	0},
  {"$strobe",		SysTask_strobe,		1,	NOLIM,	STF_NEEDCTX},
  {"$time",		SysTask_time,		0,	0,	0},
  {"$tkg$command",	SysTask_tkg_command,	1,	NOLIM,	0},
  {"$tkg$exec",		SysTask_tkg_exec,	1,	NOLIM,	0},
  {"$tkg$post",		SysTask_tkg_post,	2,	2,	0},
  {"$tkg$probe",	SysTask_tkg_probe,		1,	NOLIM,	STF_NEEDCTX|STF_NEEDNETS},
  {"$tkg$recv",		SysTask_tkg_recv,	1,	1,	0},
  {"$tkg$reportbreak",	SysTask_tkg_reportBreak,0,	NOLIM,	0},
  {"$tkg$send",		SysTask_tkg_send,	2,	2,	0},
  {"$tkg$systime",	SysTask_tkg_systime,	0,	0,	0},
  {"$tkg$unprobe",	SysTask_tkg_unprobe,	1,	NOLIM,	STF_NEEDCTX|STF_NEEDNETS},
  {"$tkg$wait",		SysTask_tkg_wait,	1,	1,	0},
  {"$tkg$waituntil",	SysTask_tkg_waituntil,	1,	1,	0},
  {"$tkg$zoom",		SysTask_tkg_zoom,	1,	1,	0},
  {"$width",		SysTask_width,		2,	3,	STF_SPECIFY|STF_NEEDCTX|STF_NEEDNETS, {TAT_TRIGGERPAIR,TAT_VALUE,TAT_NET}},
  {"$write",		SysTask_write,		0,	NOLIM,	0},
  {"$writememb",	SysTask_writememb,	1,	3,	0, {TAT_VALUE,TAT_NET,TAT_VALUE}},
  {"$writememh",	SysTask_writememh,	1,	3,	0, {TAT_VALUE,TAT_NET,TAT_VALUE}},
};

static FILE *openFiles[STT_MAXFILES] = {0};

/*****************************************************************************
 *
 * Expand any %m substrings in a string to the current module path.
 *
 * Parameters:
 *     s		String to manipulate
 *     mi		Module instance
 *
 *****************************************************************************/
static void string_expand(char *s, ModuleInst *mi)
{
  const char *path = ModuleInst_getPath(mi);
  int path_len = strlen(path);

  while ((s = strstr(s,"%m"))) {
    memmove(s+path_len,s+2,strlen(s+2)+1);
    memmove(s,path,path_len);
    s += path_len;
  }
}

const char *SysTask_findName(systask_f *f)
{
  int i;

  for (i = 0;i < sizeof(sysTaskTable)/sizeof(sysTaskTable[0]);i++) {
    if (sysTaskTable[i].st_func == f)
      return sysTaskTable[i].st_name;
  }
  return 0;
}

systask_f *SysTask_find(const char *name)
{
  int i;

  for (i = 0;i < sizeof(sysTaskTable)/sizeof(sysTaskTable[0]);i++) {
    if (strcmp(sysTaskTable[i].st_name,name) == 0)
      return sysTaskTable[i].st_func;
  }
  return 0;
}

SysTaskDescript *SysTask_findEnt(const char *name)
{
  int i;

  for (i = 0;i < sizeof(sysTaskTable)/sizeof(sysTaskTable[0]);i++) {
    if (strcmp(sysTaskTable[i].st_name,name) == 0)
      return &sysTaskTable[i];
  }
  return 0;
}

TaskContext *new_TaskContext(int nargs,Expr **args,ModuleInst *modCtx)
{
  TaskContext *tc = (TaskContext *) malloc(sizeof(TaskContext));
  PHash H;
  HashElem *he;
  int i;
  
  tc->tc_thread = 0;
  tc->tc_codeBlock = 0;
  tc->tc_action = TA_SETUP;
  tc->tc_numNets = 0;
  tc->tc_nets = 0;
  tc->tc_lastValues = 0;
  tc->tc_data = 0;


  PHash_init(&H);
  for (i = 0;i < nargs;i++) {
    Expr_getReaders(args[i], ModuleInst_getScope(modCtx), &H);
  }
      
  tc->tc_numNets = Hash_numElems(&H);
  if (tc->tc_numNets > 0) {
    tc->tc_nets = (Net**) malloc(sizeof(Net*)*tc->tc_numNets);
    tc->tc_lastValues = (Value**) malloc(sizeof(Value*)*tc->tc_numNets);
  }
  for (i = 0, he = Hash_first(&H);he;i++, he = Hash_next(&H, he)) {
    Net *n = (Net*)HashElem_obj(he);
    tc->tc_nets[i] = n;
    tc->tc_lastValues[i] = new_Value(Net_nbits(n));
  }

  PHash_uninit(&H);

  return tc;
}

void delete_TaskContext(TaskContext *tc)
{
  if (tc->tc_thread) delete_VGThread(tc->tc_thread);
  if (tc->tc_codeBlock) delete_CodeBlock(tc->tc_codeBlock);
  free(tc);
}

void TaskContext_setBlock(TaskContext *tc,CodeBlock *codeBlock,ModuleInst *mi,unsigned start,unsigned stop)
{
  tc->tc_codeBlock = new_CodeBlock(mi);
  tc->tc_thread = new_VGThread(tc->tc_codeBlock,0,mi,0);
  CodeBlock_copy(tc->tc_codeBlock,0,codeBlock,start,stop);
  VGThread_start(tc->tc_thread);
}

int ilog10(simtime_t x)
{
  int n = 0;

  while (x > 1) {
    x /= 10;
    n++;
  }

  return n;
}

static int SysTask_getstr_time(Value *S,char *p, ModuleInst *mi)
{
  Timescale *ts = &mi->mc_mod->m_timescale;
  simtime_t t;

  if (Value_toTime(S, &t) != 0) {
    return sprintf(p,"x");
  }

  /*
   * Convert to local precision value
   */
  t = t/(ts->ts_precision/vgsim.vg_timescale.ts_precision);

  if (ts->ts_precision == ts->ts_units)
    return sprintf(p,"%llu",t);
  else {
    char format[STRMAX];
    simtime_t x = (ts->ts_units/ts->ts_precision);
    simtime_t thigh = t / x;
    simtime_t tlow  = t % x;

    sprintf(format,"%%llu.%%0%dllu",ilog10(x));

    return sprintf(p,format,thigh,tlow);
  }
    
}


/*****************************************************************************
 *
 * Basic printf-like formatting function.
 *
 *****************************************************************************/
static int SysTask_sprintfSSVec(VGThread *t,char *p,int numArgs, void **args)
{
  char fmt[STRMAX];
  char *q;
  int nUsed = 0;

  if (numArgs <= 0) return nUsed;

  Value_toString((Value*)args[0],fmt);

  numArgs--;
  args++;
  nUsed++;

  for (q = fmt;*q;) {
    if (*q != '%') {
      *p++ = *q++;
    } else {
      char *x = q+1;
      char oneFmt[STRMAX];
      const char *path;

      while (isdigit(*x) || (*x == '-') || (*x == '.')) x++;

      /*
       * Just print a percent sign.
       */
      switch (*x) {
      case '%' :
	*p++ = '%';
	q = x + 1;
	break;
      case 'M' :
	{
	  ModuleDecl *m = ModuleInst_getModDecl(VGThread_getModCtx(t));
	  path = ModuleDecl_getName(m);
	  p += sprintf(p,"%s",path);
	  q = x + 1;
	}
	break;
      case 'm' :
	path = ModuleInst_getPath(VGThread_getModCtx(t));
	p += sprintf(p,"%s",path);
	q = x + 1;
	break;
      case 't' :
      case 'T' :
	if (numArgs <= 0) goto bailOut;	/* Not enough arguments */

	p += SysTask_getstr_time((Value*)*args, p, VGThread_getModCtx(t));
	numArgs--;
	args++;
	nUsed++;

	q = x + 1;
	break;
      case 0 :
	q = x;
	break;
      default:
	if (numArgs <= 0) goto bailOut;	/* Not enough arguments */

	memcpy(oneFmt,q,x-q+1);
	oneFmt[(x-q+1)] = 0;

	Value_format((Value*)*args,oneFmt,p);
	p += strlen(p);

	numArgs--;
	args++;
	nUsed++;

	q = x+1;
	break;
      }
    }
  }

  *p = 0;
  return nUsed;

 bailOut:
  p += sprintf(p,"%%bad-format%%");
  *p = 0;
  return nUsed;
}

/*****************************************************************************
 *****************************************************************************/
static void SysTask_printfSSVec(VGThread *t,int numArgs, void **args)
{
  char buf[2*STRMAX];
  int nUsed;
  int lastWasDiscrete = 0;

  while (numArgs > 0) {
    if ((Value_getTypeFlags((Value*)args[0]) & SF_STRING)) {
      nUsed = SysTask_sprintfSSVec(t,buf,numArgs,args);
      vgio_printf("%s",buf);
      numArgs -= nUsed;
      args += nUsed;
      lastWasDiscrete = 0;
    } else {
      if (lastWasDiscrete) vgio_printf(" ");
      Value_getstr(args[0],buf);
      vgio_printf("%s",buf);
      numArgs--;
      args++;
      lastWasDiscrete = 1;
    }
  }
}

/*****************************************************************************
 *
 * $tkg$reportBreak: Stop simulation and report a breakpoint.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage:
 *     $tkg$reportBreak(id, value);
 *
 *****************************************************************************/
static void SysTask_tkg_reportBreak(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);

  /*
   * Stop the simulator
   */
  EvQueue_stopAfter(Q,0);
  VGThread_suspend(t);
  EvQueue_enqueueAfter(Q,new_EvThread(t), 0);

  /*
   * Report the breakpoint. 
   */
  if (numArgs >= 2) {
    int id = -1;
    Value *value;

    char buf[STRMAX];
    char *p;

    Value_toInt((Value*)args[0],(unsigned*)&id);
    value = (Value*)args[1];

    if (Value_nbits(value) == 1)
      sprintf(buf,"break %d %d'b",id,Value_nbits(value));
    else
      sprintf(buf,"break %d %d'h",id,Value_nbits(value));
    p = buf + strlen(buf);
    Value_format(value,"%h",p);
    p += strlen(p);
    sprintf(p," @ %llu",Q->eq_curTime);
    
    vgio_printf("%s\n",buf);
  }
}

/*****************************************************************************
 *
 * $monitor: Set up a monitor on a set of variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_monitor(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  EvQueue *Q;
  Event *e;
  int i;
  int changed = 0;

  switch (taskContext->tc_action) {
  case TA_SETUP :
    e = new_EvStrobe(SysTask_monitor,taskContext);

    for (i = 0;i < taskContext->tc_numNets;i++) {
      Value_copy(taskContext->tc_lastValues[i], Net_getValue(taskContext->tc_nets[i]));
      Net_addMonitor(taskContext->tc_nets[i]);
    }

    Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
    EvQueue_enqueueMonitor(Q,e);
    break;
  case TA_STROBE :
    for (i = 0;i < taskContext->tc_numNets;i++) {
      if (!Value_isEqual(taskContext->tc_lastValues[i], Net_getValue(taskContext->tc_nets[i]))) {
	Value_copy(taskContext->tc_lastValues[i], Net_getValue(taskContext->tc_nets[i]));
	changed = 1;
      }
    }

    if (changed || taskContext->tc_numNets == 0)
      SysTask_display(t,r,numArgs,args,0);

    VGThread_suspend(t);
    break;
  case TA_REMOVE :
    for (i = 0;i < taskContext->tc_numNets;i++) {
      Net_removeMonitor(taskContext->tc_nets[i]);
    }
    break;
  }
  taskContext->tc_action = TA_SETUP;
}

/*****************************************************************************
 *
 * $fmonitor: Set up a monitor on a set of variables (with multi-channel output)
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_fmonitor(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  EvQueue *Q;
  Event *e;
  int i;
  int changed = 0;

  switch (taskContext->tc_action) {
  case TA_SETUP :
    e = new_EvStrobe(SysTask_fmonitor,taskContext);

    for (i = 0;i < taskContext->tc_numNets;i++) {
      Value_copy(taskContext->tc_lastValues[i], Net_getValue(taskContext->tc_nets[i]));
      Net_addMonitor(taskContext->tc_nets[i]);
    }

    Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
    EvQueue_enqueueMonitor(Q,e);
    break;
  case TA_STROBE :
    for (i = 0;i < taskContext->tc_numNets;i++) {
      if (!Value_isEqual(taskContext->tc_lastValues[i], Net_getValue(taskContext->tc_nets[i]))) {
	Value_copy(taskContext->tc_lastValues[i], Net_getValue(taskContext->tc_nets[i]));
	changed = 1;
      }
    }

    if (changed || taskContext->tc_numNets == 0)
      SysTask_fdisplay(t,r,numArgs,args,0);

    VGThread_suspend(t);
    break;
  case TA_REMOVE :
    for (i = 0;i < taskContext->tc_numNets;i++) {
      Net_removeMonitor(taskContext->tc_nets[i]);
    }
    break;
  }
  taskContext->tc_action = TA_SETUP;
}

/*****************************************************************************
 *
 * $tkg$probe: Set probes on a set of variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_tkg_probe(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  Circuit *c = t->t_modCtx->mc_circuit;
  EvQueue *Q = Circuit_getQueue(c);
  char who[STRMAX];
  Event *e;
  int i;

  *who = 0;
  if (numArgs > 0 && (Value_getTypeFlags((Value*)args[0]) & SF_STRING)) {
    Value_toString((Value*)args[0],who);
  }

  for (i = 0;i < taskContext->tc_numNets;i++) {
    char key[STRMAX];

    Net_addMonitor(taskContext->tc_nets[i]);

    if (*who && strcmp(who,"$probe") != 0) {
      sprintf(key, "%s:%s",who,Net_getName(taskContext->tc_nets[i]));
    } else {
      sprintf(key, "$probe:%s",Net_getName(taskContext->tc_nets[i]));
      *who = 0;
    }
    
    if (!*who)
      vgio_printf("showprobe %s %d\n",Net_getName(taskContext->tc_nets[i]),Net_nbits(taskContext->tc_nets[i]));

    Net_reportValue(taskContext->tc_nets[i], 0, Net_getName(taskContext->tc_nets[i]), c);

    /*
     * Add probe only if it does not already exist.
     */
    if (!EvQueue_findFinal(Q,key)) {
      e = new_EvProbe(taskContext->tc_nets[i],(*who ? who : 0),0);
      EvQueue_enqueueFinal(Q,key, e);
    }
  }
}

/*****************************************************************************
 *
 * $tkg$unprobe: Remove probes on a set of variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_tkg_unprobe(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  char who[STRMAX];
  int i;

  *who = 0;
  if (numArgs > 0 && (Value_getTypeFlags((Value*)args[0]) & SF_STRING)) {
    Value_toString((Value*)args[0],who);
  }

  for (i = 0;i < taskContext->tc_numNets;i++) {
    char key[STRMAX];

    Net_removeMonitor(taskContext->tc_nets[i]);

    if (*who && strcmp(who,"$probe") != 0) {
      sprintf(key, "%s:%s",who,Net_getName(taskContext->tc_nets[i]));
    } else {
      sprintf(key, "$probe:%s",Net_getName(taskContext->tc_nets[i]));
    }

    EvQueue_removeFinal(Q,key);

    if (!*who)
      vgio_printf("hideprobe %s\n",Net_getName(taskContext->tc_nets[i]));
  }
}

/*****************************************************************************
 *
 * $fwrite: Display strings/variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_fwrite(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  int handle;
  char buf[2*STRMAX], *p;
  int nUsed;
  int lastWasDiscrete = 0;
  int i;

  if (numArgs < 1) return;
  Value_toInt((Value*) args[0],&handle);

  numArgs--;
  args++;

  p = buf;
  while (numArgs > 0) {
    if ((Value_getTypeFlags((Value*)args[0]) & SF_STRING)) {
      nUsed = SysTask_sprintfSSVec(t,p,numArgs,args);
      p += strlen(p);
      numArgs -= nUsed;
      args += nUsed;
      lastWasDiscrete = 0;
    } else {
      if (lastWasDiscrete) {
	*p++ = ' '; *p=0;
      }
      Value_getstr(args[0],p);
      p += strlen(p);
      numArgs--;
      args++;
      lastWasDiscrete = 1;
    }
  }

  for (i = 0;i < STT_MAXFILES;i++) {
    if ((handle & (1<<i))) {
      if (i == 0)
	vgio_echo("%s",buf);
      else if (openFiles[i])
	fprintf(openFiles[i],"%s",buf);
      else
	errorRun(ERR_CLSDWRITE);
    }
  }
}

/*****************************************************************************
 *
 * $fdisplay: Display strings/variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_fdisplay(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  int i;
  int handle;

  if (numArgs < 1) return;
  Value_toInt((Value*) args[0],&handle);

  SysTask_fwrite(t,r,numArgs,args,tc);
  for (i = 0;i < STT_MAXFILES;i++) {
    if ((handle & (1<<i))) {
      if (i == 0)
	vgio_echo("\n");
      else if (openFiles[i])
	fprintf(openFiles[i],"\n");
      else
	errorRun(ERR_CLSDWRITE);
    }
  }
}

/*****************************************************************************
 *
 * $write: Display strings/variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_write(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  if (numArgs > 0) {
    vgio_echo("");
    SysTask_printfSSVec(t,numArgs,args);
  }
}

/*****************************************************************************
 *
 * $display: Display strings/variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_display(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  SysTask_write(t,r,numArgs,args,tc);
  vgio_printf("\n");
}

/*****************************************************************************
 *
 * $error: Display strings/variables as a run-time error
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_error(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  char buf[STRMAX],*p;
  EvQueue *Q = Circuit_getQueue(&vgsim.vg_circuit);
  simtime_t curTime = EvQueue_getCurTime(Q);

  p = buf;
  if (vgsim.vg_interactive)
    p += sprintf(p,"error run %llu : ",curTime);
  else
    p += sprintf(p,"Runtime error: ");

  SysTask_sprintfSSVec(t,p,numArgs,args);
  vgio_printf("%s\n",buf);
}

/*****************************************************************************
 *
 * $tkg$command: Format and send a command to the GUI
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_tkg_command(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  if (!vgsim.vg_sec.vgs_send) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$tkg$command");
    return;
  }

  if (numArgs > 0) {
    SysTask_printfSSVec(t,numArgs,args);
  }
  vgio_printf("\n");
}

/*****************************************************************************
 *
 * $tkg$exec: Format and execute as a tcl/tk command
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_tkg_exec(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  if (vgsim.vg_sec.vgs_exec < 1) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$tkg$exec");
    return;
  }

  if (numArgs > 0) {
    vgio_printf("exec ");
    SysTask_printfSSVec(t,numArgs,args);
    vgio_printf("\n");
  }
}

/*****************************************************************************
 *
 * $tkg$post: Send command to request that a VPD be posted.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_tkg_post(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  char buf[STRMAX],buf2[STRMAX];
  int i;

  if (vgsim.vg_sec.vgs_exec < 1) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$tkg$post");
    return;
  }

  if (numArgs < 2) return;

  vgio_printf("post");
  for (i = 0;i < numArgs;i++) {
    Value_getstr(args[i],buf);
    string_expand(buf,VGThread_getModCtx(t));
    vgio_printf(" %s",tclizeString(buf2,buf));
  }

  vgio_printf("\n");
}

/*****************************************************************************
 *
 * $strobe: Strobe strings/variables
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_strobe(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  EvQueue *Q;
  Event *e;

  switch (taskContext->tc_action) {
  case TA_SETUP :
    e = new_EvStrobe(SysTask_strobe,taskContext);
    Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
    EvQueue_enqueueStrobe(Q,e);
    break;
  case TA_STROBE :
    SysTask_display(t,r,numArgs,args,0);
    VGThread_suspend(t);
    break;
  case TA_REMOVE :
    break;
  }
  taskContext->tc_action = TA_SETUP;
}

/*****************************************************************************
 *
 * $fstrobe: Strobe strings/variables to multiple outputs
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: $fstrobe(h, p1, p2, ...)
 *
 *****************************************************************************/
static void SysTask_fstrobe(VGThread *t,Value *r,int numArgs,void **args,TaskContext *taskContext)
{
  EvQueue *Q;
  Event *e;

  switch (taskContext->tc_action) {
  case TA_SETUP :
    e = new_EvStrobe(SysTask_fstrobe,taskContext);
    Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
    EvQueue_enqueueStrobe(Q,e);
    break;
  case TA_STROBE :
    SysTask_fdisplay(t,r,numArgs,args,0);
    VGThread_suspend(t);
    break;
  case TA_REMOVE :
    break;
  }
  taskContext->tc_action = TA_SETUP;
}

/*****************************************************************************
 *
 * $finish: Terminate the simulation
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_finish(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  exit(0);
}

/*****************************************************************************
 *
 * $monitoroff: Temporarily disable display of $monitor reports
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_monitoroff(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  Q->eq_monitorOn = 0;
}

/*****************************************************************************
 *
 * $monitoron: Reenable display of $monitor reports
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_monitoron(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  Q->eq_monitorOn = 1;
}

/*****************************************************************************
 *
 * $stop: Stop the simulator and enter paused mode.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_stop(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  int n = 0;

  if (numArgs > 0) {
    Value_toInt((Value*)args[0],(unsigned*)&n);
  }

  VGThread_suspend(t);
  EvQueue_enqueueAfter(Q,new_EvThread(t), 0);

  if (n == 0)
    EvQueue_stop(Q);
  else
    EvQueue_stopAfter(Q,n);
}

/*****************************************************************************
 *
 * $random: Generate random numbers (or set seed)
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage:  $random(seed)		Set the random number seed
 *         $random			Get a random number
 *
 *****************************************************************************/
static void SysTask_random(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  static char state[256];
  unsigned seed;

  if (numArgs > 0) {
    Value_toInt((Value*)args[0],(unsigned*)&seed);
    initstate(seed, state, 256);
  }
  if (r)
    Value_convertI(r,random());
}

/*****************************************************************************
 *
 * $time: Return the current simulation time.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_time(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  if (r) {
    EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
    Value_convertTime(r,EvQueue_getCurTime(Q));
  }
}

/*****************************************************************************
 *
 * $stime: Return the bottom 32-bits of the current simulation time.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 *****************************************************************************/
static void SysTask_stime(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  if (r) {
    EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
    Value_convertI(r,(unsigned long)EvQueue_getCurTime(Q));
  }
}

/*****************************************************************************
 *
 * $tkg$systime: Returns the wall clock time in ms since the epoch.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: t = $tkg$systime
 *
 *****************************************************************************/
static void SysTask_tkg_systime(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  if (r) {
    struct timeval tv;
    simtime_t theTime;

    gettimeofday(&tv,0);

    theTime = tv_to_ms(&tv);
    Value_convertTime(r,theTime);
    Value_toTime(r,&theTime);
  }
}

/*****************************************************************************
 *
 * $tkg$wait: Pause a thread for a specified number of milliseconds.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: $tkg$wait(value)
 *
 *****************************************************************************/
static void SysTask_tkg_wait(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  struct timeval tv;
  simtime_t evTime;
  unsigned ms;

  Value_toInt((Value*)args[0],&ms);
  VGThread_suspend(t);

  gettimeofday(&tv,0);

  tv.tv_sec  += ms/1000;
  tv.tv_usec += (ms % 1000)*1000;
  if (tv.tv_usec > 1000000) {
    tv.tv_sec++;
    tv.tv_usec = tv.tv_usec % 1000000;
  }

  evTime = tv_to_ms(&tv);
  EvQueue_timedEnqueue(Q,new_EvThread(t), evTime);
}

/*****************************************************************************
 *
 * $tkg$zoom: Set the zoom factor in the TkGate scope window
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: $tkg$zoom(value)
 *
 *****************************************************************************/
static void SysTask_tkg_zoom(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  struct timeval tv;
  simtime_t evTime;
  unsigned zoom;

  Value_toInt((Value*)args[0],&zoom);
  vgio_printf("zoom %d\n",zoom);
}

/*****************************************************************************
 *
 * $tkg$waituntil: Pause a thread until a specific time in milliseconds from the epoch.
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: $tkg$waituntil(value)
 *
 *****************************************************************************/
static void SysTask_tkg_waituntil(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  simtime_t evTime;

  Value_toTime((Value*)args[0],&evTime);
  VGThread_suspend(t);
  EvQueue_timedEnqueue(Q,new_EvThread(t), evTime);
}

/*****************************************************************************
 *
 * $tkg$recv: Read a value from a channel, block if necessary
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: $tkg$recv("name")
 *
 *****************************************************************************/
static void SysTask_tkg_recv(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  char name[STRMAX];
  Channel *c;

  if (!vgsim.vg_sec.vgs_queue) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$tkg$recv");
    return;
  }

  if (numArgs != 1) return;
  if (!r) {
    return;
  }

  Value_toString((Value*)args[0],name);
  string_expand(name, VGThread_getModCtx(t));
  c = Circuit_getChannel(t->t_modCtx->mc_circuit, name);

  if (Channel_read(c, r) < 0) {
    /*
     * No data is available on the channel.  Suspend the thread and wait for data.
     * Backup the pc so this system task will be reexecuted when
     * the thread wakes up.
     */
    Channel_wait(c,t);
    t->t_pc--;
  } else {
#if 0
    char buf[1024];
    Value_getstr(r,buf);
    vgio_echo("$tkg$recv %s: data=%s\n",name,buf);
#endif
  }
}



/*****************************************************************************
 *
 * $tkg$send: Write a value to a channel
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage: $tkg$send("name", value)
 *
 *****************************************************************************/
static void SysTask_tkg_send(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  char name[STRMAX];
  Channel *c;

  if (!vgsim.vg_sec.vgs_queue) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$tkg$send");
    return;
  }

  if (numArgs != 2) {
    return;
  }

  Value_toString((Value*)args[0],name);
  string_expand(name, VGThread_getModCtx(t));
  c = Circuit_getChannel(t->t_modCtx->mc_circuit, name);
  Channel_write(c, (Value*) args[1]);
}

/*****************************************************************************
 *
 * $readmemb: Read a memory file in binary format
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage:
 *    $readmemb("filename");
 *    $readmemb("filename", memory);
 *    $readmemb("filename", memory, start);
 *    $readmemb("filename", memory, start, stop);
 *
 *****************************************************************************/
static void SysTask_readmemb(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  Value *vFileName;
  Net *net;
  Value *vStart, *vStop;
  unsigned start = 0, stop = ~0;
  char fileName[STRMAX];

  if (numArgs < 1) return;

  vFileName	= (Value*) args[0];
  net		= (numArgs>=2) ? (Net*) args[1] : 0;
  vStart 	= (numArgs>=3) ? (Value*) args[2] : 0;
  vStop		= (numArgs>=4) ? (Value*) args[3] : 0;

  Value_format(vFileName,"%s",fileName);

  if (vStart) {
    if (Value_toInt(vStart, &start) < 0)
      errorRun(ERR_BADSTART,"$readmemb");
  }

  if (vStop) {
    if (Value_toInt(vStop, &stop) < 0)
      errorRun(ERR_BADSTOP,"$readmemb");
  }

  Circuit_readMemory(t->t_modCtx->mc_circuit, fileName, net, start, stop, SF_BIN);
}

/*****************************************************************************
 *
 * $readmemh: Read a memory file in hex format
 * 
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage:
 *    $readmemh("filename");
 *    $readmemh("filename", memory);
 *    $readmemh("filename", memory, start);
 *    $readmemh("filename", memory, start, stop);
 *
 *****************************************************************************/
static void SysTask_readmemh(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  Value *vFileName;
  Net *net;
  Value *vStart, *vStop;
  unsigned start = 0, stop = ~0;
  char fileName[STRMAX];

  if (numArgs < 1) return;

  vFileName	= (Value*) args[0];
  net		= (numArgs>=2) ? (Net*) args[1] : 0;
  vStart 	= (numArgs>=3) ? (Value*) args[2] : 0;
  vStop		= (numArgs>=4) ? (Value*) args[3] : 0;

  Value_format(vFileName,"%s",fileName);

  if (vStart) {
    if (Value_toInt(vStart, &start) < 0)
      errorRun(ERR_BADSTART,"$readmemh");
  }

  if (vStop) {
    if (Value_toInt(vStop, &stop) < 0)
      errorRun(ERR_BADSTOP,"$readmemh");
  }

  Circuit_readMemory(t->t_modCtx->mc_circuit, fileName, net, start, stop, SF_HEX);
}

static void SysTask_writememb(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  Value *vFileName;
  Net *net;
  Value *vStart, *vStop;
  unsigned start = 0, stop = ~0;
  char fileName[STRMAX];

  if (!vgsim.vg_sec.vgs_writemem) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$writememb");
    return;
  }

  if (numArgs < 1) return;

  vFileName	= (Value*) args[0];
  net		= (numArgs>=2) ? (Net*) args[1] : 0;
  vStart 	= (numArgs>=3) ? (Value*) args[2] : 0;
  vStop		= (numArgs>=4) ? (Value*) args[3] : 0;

  Value_format(vFileName,"%s",fileName);

  if (vStart) {
    if (Value_toInt(vStart, &start) < 0)
      errorRun(ERR_BADSTART,"$writememb");
  }

  if (vStop) {
    if (Value_toInt(vStop, &stop) < 0)
      errorRun(ERR_BADSTOP,"$writememb");
  }

  if (Circuit_writeMemory(t->t_modCtx->mc_circuit, fileName, net, start, stop, SF_BIN) < 0)
    errorRun(ERR_BADOPEN,"$writememb");
}

static void SysTask_writememh(VGThread *t,Value *r,int numArgs,void **args,TaskContext *tc)
{
  Value *vFileName;
  Net *net;
  Value *vStart, *vStop;
  unsigned start = 0, stop = ~0;
  char fileName[STRMAX];

  if (!vgsim.vg_sec.vgs_writemem) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$writememh");
    return;
  }

  if (numArgs < 1) return;

  vFileName	= (Value*) args[0];
  net		= (numArgs>=2) ? (Net*) args[1] : 0;
  vStart 	= (numArgs>=3) ? (Value*) args[2] : 0;
  vStop		= (numArgs>=4) ? (Value*) args[3] : 0;

  Value_format(vFileName,"%s",fileName);

  if (vStart) {
    if (Value_toInt(vStart, &start) < 0)
      errorRun(ERR_BADSTART,"$writememh");
  }

  if (vStop) {
    if (Value_toInt(vStop, &stop) < 0)
      errorRun(ERR_BADSTOP,"$writememh");
  }

  if (Circuit_writeMemory(t->t_modCtx->mc_circuit, fileName, net, start, stop, SF_HEX)< 0)
    errorRun(ERR_BADOPEN,"$writememh");
}

static void SysTask_fclose(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  int i;
  int handle;

  if (!vgsim.vg_sec.vgs_fopen) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$fclose");
    return;
  }

  if (numArgs != 1) return;

  Value_toInt((Value*) args[0],&handle);

  for (i = 1;i < STT_MAXFILES;i++)
    if ((handle & (1<<i))) {
      if (openFiles[i]) {
	fclose(openFiles[i]);
	openFiles[i] = 0;
      } else {
	errorRun(ERR_BADCLOSE,"$fclose");
      }
    }
}

static void SysTask_fopen(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  char fileName[STRMAX];
  FILE *f;
  int i;

  if (!vgsim.vg_sec.vgs_fopen) {
    VGSecurity_handleException(&vgsim.vg_sec,t,"$fopen");
    return;
  }

  if (numArgs != 1) return;

  /* slot 1 is for the "standard output" */
  for (i = 1;i < STT_MAXFILES;i++)
    if (!openFiles[i]) break;

  if (i == STT_MAXFILES) {
    errorRun(ERR_OPENTOOMANY,"$fopen");
    Value_convertI(r,0);
    return;
  }

  /* Convert value into a string */
  Value_format((Value*) args[0],"%s",fileName);

  f = fopen(fileName,"w");
  if (!f) {
    errorRun(ERR_BADOPEN,fileName,"$fopen");
    Value_convertI(r,0);
    return;
  }
  openFiles[i] = f;

  Value_convertI(r,(1<<i));
}

/*****************************************************************************
 *
 * Debugging function to show calls to specify tasks
 *
 *****************************************************************************/
#if 0
static void showSpecTaskCall(const char *name,int *argTypes,VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  simtime_t now = EvQueue_getCurTime(Q);
  ModuleInst *mi = VGThread_getModCtx(t);
  ModuleDecl *m  = ModuleInst_getModDecl(mi);
  char syscall[STRMAX],*p;
  int i;

  /*
   * Create a string with the invoked systask call in it.
   */
  p = syscall;
  p += sprintf(p,"%s(",name);

  for (i = 1;i < numArgs;i++) {
    if (i>1) p += sprintf(p,", ");
    switch (argTypes[i]) {
    case TAT_VALUE :
    case TAT_TRIGGER :
      Value_getstr((Value*)args[i],p);
      p += strlen(p);
      break;
    case TAT_EXPR :
      Expr_getstr((Expr*)args[i],p);
      p += strlen(p);
      break;
    default :
      p += sprintf(p,"*-*");
      break;
    }
  }
  p += sprintf(p,")");

  printf("%lld: %s[%s] %s\n",now, ModuleInst_getPath(mi), ModuleDecl_getName(m), syscall);
}
#endif

static void reportTimingViolation(const char *name,int *argTypes,VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  simtime_t now = EvQueue_getCurTime(Q);
  ModuleInst *mi = VGThread_getModCtx(t);
  ModuleDecl *m  = ModuleInst_getModDecl(mi);
  char syscall[STRMAX],*p;
  char now_s[STRMAX];
  int i;

  /*
   * We are still in circuit initialization period and suppressing violations.
   */
  if (now < vgsim.vg_initTime || vgsim.vg_noTimeViolations)
    return;

  /*
   * Create a string with the invoked systask call in it.
   */
  p = syscall;
  p += sprintf(p,"%s(",name);

  for (i = 1;i < numArgs;i++) {
    if (i>1) p += sprintf(p,", ");
    switch (argTypes[i]) {
    case TAT_VALUE :
    case TAT_TRIGGER :
      Value_getstr((Value*)args[i],p);
      p += strlen(p);
      break;
    case TAT_EXPR :
      Expr_getstr((Expr*)args[i],p);
      p += strlen(p);
      break;
    default :
      p += sprintf(p,"*-*");
      break;
    }
  }
  p += sprintf(p,")");

  errorRun(ERR_TIMING, ModuleInst_getPath(mi), ModuleDecl_getName(m), syscall);
}


/*****************************************************************************
 *
 * Do a timing check for signal setup
 *
 * Parameters:
 *     t		Thread in which system task is executed
 *     r		Optional return value 
 *     numArgs		Number of arguments
 *     args		Array of argument values
 *     taskContext	Optional task context
 *
 * Usage:
 *     $setup(data, posedge clk &&& enable, time);
 *
 *****************************************************************************/
static void SysTask_setup(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  static int argTypes[] = {TAT_VALUE,TAT_EXPR,TAT_EXPR, TAT_VALUE, TAT_NET};
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  simtime_t now = EvQueue_getCurTime(Q);
  simtime_t *lastTrans;
  int which = 0,constraint = 0;

  Value_toInt((Value*)args[0],&which);
  Value_toInt((Value*)args[3],&constraint);

  /*
   * If this is an initialization call,
   */
  if (which < 0) {
    lastTrans = (simtime_t*) malloc(sizeof(simtime_t)*2);
    taskContext->tc_data = lastTrans;
    lastTrans[0] = lastTrans[1] = 0;
    return;
  }

  lastTrans = (simtime_t*) taskContext->tc_data ;

  if (which > 1) {
    errorRun(ERR_IE_BADSTATE,"$setup");
    return;
  }

  /*
   * Don't report errors at time 0.
   */
  if (now == 0) return;


  lastTrans[which] = now;

#if 0
  showSpecTaskCall("$setup",argTypes,t,r,numArgs,args,taskContext);
#endif

  if (which == 1) {
#if 0
    printf("$setup check if (%lld-%lld) < %d\n", lastTrans[1], lastTrans[0], constraint);
#endif
    if (lastTrans[1] - lastTrans[0] < constraint)
      reportTimingViolation("$setup",argTypes,t,r,numArgs,args,taskContext);
  }
}

static void SysTask_hold(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  static int argTypes[] = {TAT_VALUE,TAT_EXPR,TAT_EXPR, TAT_VALUE, TAT_NET};
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  simtime_t now = EvQueue_getCurTime(Q);
  simtime_t *lastTrans;
  int which = 0,constraint = 0;

  Value_toInt((Value*)args[0],&which);
  Value_toInt((Value*)args[3],&constraint);

  /*
   * If this is an initialization call,
   */
  if (which < 0) {
    lastTrans = (simtime_t*) malloc(sizeof(simtime_t)*2);
    taskContext->tc_data = lastTrans;
    lastTrans[0] = lastTrans[1] = 0;
    return;
  }

  lastTrans = (simtime_t*) taskContext->tc_data ;

  if (which > 1) {
    errorRun(ERR_IE_BADSTATE,"$hold");
    return;
  }

  /*
   * Don't report errors at time 0.
   */
  if (now == 0) return;


  lastTrans[which] = now;

#if 0
  showSpecTaskCall("$hold",argTypes,t,r,numArgs,args,taskContext);
#endif

  if (which == 1) {
#if 0
    printf("$hold check if (%lld-%lld) < %d\n", lastTrans[1], lastTrans[0], constraint);
#endif
    if (lastTrans[1] - lastTrans[0] < constraint)
      reportTimingViolation("$hold",argTypes,t,r,numArgs,args,taskContext);
  }
}

static void SysTask_width(VGThread *t, Value *r, int numArgs, void **args, TaskContext *taskContext)
{
  static int argTypes[] = {TAT_VALUE,TAT_EXPR, TAT_VALUE, TAT_NET};
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);
  simtime_t now = EvQueue_getCurTime(Q);
  simtime_t *lastTrans;
  int which = 0,constraint = 0;

  Value_toInt((Value*)args[0],&which);
  Value_toInt((Value*)args[2],&constraint);

  /*
   * If this is an initialization call,
   */
  if (which < 0) {
    lastTrans = (simtime_t*) malloc(sizeof(simtime_t)*2);
    taskContext->tc_data = lastTrans;
    lastTrans[0] = lastTrans[1] = 0;
    return;
  }

  lastTrans = (simtime_t*) taskContext->tc_data ;

  if (which > 1) {
    errorRun(ERR_IE_BADSTATE,"$width");
    return;
  }

  /*
   * Don't report errors at time 0.
   */
  if (now == 0) return;


  lastTrans[which] = now;

#if 0
  showSpecTaskCall("$width",argTypes,t,r,numArgs,args,taskContext);
#endif

  if (which == 1) {
#if 0
    printf("$width check if (%lld-%lld) < %d\n", lastTrans[1], lastTrans[0], constraint);
#endif
    if (lastTrans[1] - lastTrans[0] < constraint)
      reportTimingViolation("$width",argTypes,t,r,numArgs,args,taskContext);
  }
}

