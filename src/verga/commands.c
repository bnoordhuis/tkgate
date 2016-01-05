/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Genreal Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Last edit by hansen on Mon Feb  2 22:17:24 2009
****************************************************************************/
/*****************************************************************************
 *
 * This file handles commands from tkgate.
 *
 *****************************************************************************/
#include <ctype.h>
#include "thyme.h"

typedef void circexec_f(Circuit*c,int argc,char *argv[]);

typedef struct {
  char		*cet_command;
  circexec_f	*cet_func;
} CircExecFunc;

void Circuit_execScript(Circuit*c,int argc,char *argv[]);
void Circuit_execShow(Circuit*c,int argc,char *argv[]);
void Circuit_execQueue(Circuit*c,int argc,char *argv[]);
void Circuit_execCPath(Circuit*c,int argc,char *argv[]);
void Circuit_execSet(Circuit*c,int argc,char *argv[]);
/*void Circuit_execBreak(Circuit*c,int argc,char *argv[]);*/
void Circuit_execDelScript(Circuit*c,int argc,char *argv[]);
void Circuit_execProbe(Circuit*c,int argc,char *argv[]);
void Circuit_execUnprobe(Circuit*c,int argc,char *argv[]);
void Circuit_execMemLoad(Circuit*c,int argc,char *argv[]);
void Circuit_execMemDump(Circuit*c,int argc,char *argv[]);
void Circuit_execTime(Circuit*c,int argc,char *argv[]);
void Circuit_execStep(Circuit*c,int argc,char *argv[]);
void Circuit_execRegClock(Circuit*c,int argc,char *argv[]);
void Circuit_execStepClock(Circuit*c,int argc,char *argv[]);
void Circuit_execWait(Circuit*c,int argc,char *argv[]);
void Circuit_execStop(Circuit*c,int argc,char *argv[]);
void Circuit_execGo(Circuit*c,int argc,char *argv[]);
void Circuit_execExit(Circuit*c,int argc,char *argv[]);
void Circuit_execWrite(Circuit*c,int argc,char *argv[]);
void Circuit_execOption(Circuit*c,int argc,char *argv[]);
void Circuit_execDebug(Circuit*c,int argc,char *argv[]);
void Circuit_execQWatch(Circuit*c,int argc,char *argv[]);
void Circuit_execEnableScript(Circuit*c,int argc,char *argv[]);
void Circuit_execDisableScript(Circuit*c,int argc,char *argv[]);
void Circuit_execNetInfo(Circuit*c,int argc,char *argv[]);
void Circuit_execMemPut(Circuit*c,int argc,char *argv[]);
void Circuit_execMemGet(Circuit*c,int argc,char *argv[]);
void Circuit_execMemWatch(Circuit*c,int argc,char *argv[]);

CircExecFunc circExecTable[] = {
  {"$script", Circuit_execScript},
  {"$show", Circuit_execShow},
  {"$sendto", Circuit_execQueue},
  {"$cpath", Circuit_execCPath},
  {"$enablescript", Circuit_execEnableScript},
  {"$disablescript", Circuit_execDisableScript},
  {"$set", Circuit_execSet},
  /*  {"$break", Circuit_execBreak},*/
  {"$delscript", Circuit_execDelScript},
  {"$probe", Circuit_execProbe},
  {"$unprobe", Circuit_execUnprobe},
  {"$memload", Circuit_execMemLoad},
  {"$memdump", Circuit_execMemDump},
  {"$netinfo", Circuit_execNetInfo},
  {"$memput", Circuit_execMemPut},
  {"$memget", Circuit_execMemGet},
  {"$memwatch", Circuit_execMemWatch},
  {"$time", Circuit_execTime},
  {"$step", Circuit_execStep},
  {"$regclock", Circuit_execRegClock},
  {"$clock", Circuit_execStepClock},
  {"$wait", Circuit_execWait},
  {"$stop", Circuit_execStop},
  {"$go", Circuit_execGo},
  {"$exit", Circuit_execExit},
  {"$write", Circuit_execWrite},
  {"$qwatch", Circuit_execQWatch},
  {"$option", Circuit_execOption},
  {"$debug", Circuit_execDebug},
};
int circExecTable_len = sizeof(circExecTable)/sizeof(circExecTable[0]);

static void noNetError(const char *cmd,const char *net)
{
  errorCmd(ERR_CMDNOTNET,net,cmd);
}

static void argError(const char *cmd)
{
  errorCmd(ERR_CMDARGS,cmd);
}

/*****************************************************************************
 *
 * Loads the specified script
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $script <id> <file> [-delete]
 *
 *****************************************************************************/
void Circuit_execScript(Circuit *c,int argc,char *argv[])
{
  DynamicModule *dm;
  int i;
  int do_delete = 0;
  int is_silent = 0;
  int is_breakpt = 0;

  if (argc < 3) {
    argError("$script");
    return;
  }

  if (SHash_find(&c->c_dynamicModules,argv[1])) {
    errorCmd(ERR_CMDMODREDEF,argv[1]);
    return;
  }

  dm = new_DynamicModule(argv[1]);

  for (i = 3;i < argc;i++)
    if (strcmp(argv[i],"-delete") == 0)
      do_delete = 1;
    else if (strcmp(argv[i],"-silent") == 0)
      is_silent = 1;
    else if (strcmp(argv[i],"-breakpt") == 0)
      is_breakpt = 1;

  if (VerilogLoadScript(argv[2], dm) < 0) {
    vgio_printf("badscript %s\n",argv[1]);
    delete_DynamicModule(dm);
  } else {
    if (!is_silent)
      vgio_echo("loaded script file '%s'.\n",argv[2]);
    SHash_insert(&c->c_dynamicModules, argv[1], dm);
  }

  if (do_delete)
    unlink(argv[2]);
}


/*****************************************************************************
 *
 * Implements $show command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $show <net>
 *
 *****************************************************************************/
void Circuit_execShow(Circuit*c,int argc,char *argv[])
{
  Net *n;
  const char *netName;

  if (argc != 2) {
    argError("$show");
    return;
  }

  netName = argv[1];

  n = Circuit_findNet(c,netName);
  if (!n) {
    noNetError("$show",netName);
    return;
  }

  Net_reportValue(n,"$show", netName, c);
}


void Circuit_execQueue(Circuit*c,int argc,char *argv[])
{
}


void Circuit_execCPath(Circuit*c,int argc,char *argv[])
{
}


/*****************************************************************************
 *
 * Implements $set command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $set <net> <value>
 *
 *****************************************************************************/
void Circuit_execSet(Circuit*c,int argc,char *argv[])
{
  Net *n;
  Value *v;

  if (argc != 3) {
    argError("$set");
    return;
  }

  n = Circuit_findNet(c,argv[1]);
  if (!n) {
    noNetError("$set",argv[1]);
    return;
  }

  v = new_Value(Net_nbits(n));
  Value_convert(v,argv[2]);
  Net_set(n,v);
  delete_Value(v);
}

#if 0
/*****************************************************************************
 *
 * Implements $break command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $break <id> <file>		Set a breakpoint in the circuit
 *
 *****************************************************************************/
void Circuit_execBreak(Circuit*c,int argc,char *argv[])
{
  int id = -1;

  if (argc < 3) return;
  if (sscanf(argv[1],"%d",&id) != 1) return;

  if (*argv[2] == 'X') {
    vgio_printf("badbreak %d\n",id);
  }
  if (*argv[2] == 'S') {
    vgio_printf("break %d\n",id);
  }
}
#endif

/*****************************************************************************
 *
 * Implements $enablescript command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $enablescript <id> 		Enables a script
 *
 *****************************************************************************/
void Circuit_execEnableScript(Circuit*c,int argc,char *argv[])
{
  DynamicModule *dm;

  if (argc < 2) {
    argError("$delscript");
    return;
  }

  dm = (DynamicModule*) SHash_find(&c->c_dynamicModules, argv[1]);

  if (!dm) {
    /* We probably deleted a script with an error before tkgate received the error message */
    /*    errorCmd(ERR_CMDNOMOD,argv[1],"$delscript");*/
    return;
  }

  Circuit_enableDynamicModule(c,dm);
}

/*****************************************************************************
 *
 * Implements $delscript command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $disablescript <id> 		Disables a script
 *
 *****************************************************************************/
void Circuit_execDisableScript(Circuit*c,int argc,char *argv[])
{
  DynamicModule *dm;

  if (argc < 2) {
    argError("$delscript");
    return;
  }

  dm = (DynamicModule*) SHash_find(&c->c_dynamicModules, argv[1]);

  if (!dm) {
    /* We probably deleted a script with an error before tkgate received the error message */
    /*    errorCmd(ERR_CMDNOMOD,argv[1],"$delscript");*/
    return;
  }

  Circuit_disableDynamicModule(c,dm);
}

/*****************************************************************************
 *
 * Implements $delscript command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $delscript <id> 		Deletes a breakpoint in the circuit
 *
 *****************************************************************************/
void Circuit_execDelScript(Circuit*c,int argc,char *argv[])
{
  DynamicModule *dm;

  if (argc < 2) {
    argError("$delscript");
    return;
  }

  dm = (DynamicModule*) SHash_find(&c->c_dynamicModules, argv[1]);

  if (!dm) {
    /* We probably deleted a script with an error before tkgate received the error message */
    /*    errorCmd(ERR_CMDNOMOD,argv[1],"$delscript");*/
    return;
  }

  Circuit_unloadDynamicModule(c,dm);
  SHash_remove(&c->c_dynamicModules, argv[1]);
  delete_DynamicModule(dm);
}

/*****************************************************************************
 *
 * Implements $probe command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $probe <net>		Place a generic probe on a net 
 *   $probe <net> <who>		Place a probe on behalf of <who> 
 *
 *****************************************************************************/
void Circuit_execProbe(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;
  char key[STRMAX];
  char *who = 0;
  Net *n;
  Event *e;
  const char *netName;

  netName = argv[1];

  if (argc == 2) {
    n = Circuit_findNet(c, netName);
    sprintf(key, "$probe:%s", netName);
  } else if (argc == 3) {
    n = Circuit_findNet(c,netName);
    who = argv[2];
    sprintf(key, "%s:%s", who, netName);
    if (strcmp(who,"$probe") == 0) who = 0;
  } else {
    argError("$probe");
    return;
  }

  if (!n) {
    noNetError("$probe",netName);
    return;
  }

  Net_addMonitor(n);
  e = new_EvProbe(n,who,netName);
  EvQueue_enqueueFinal(Q,key, e);
  Net_reportValue(n, who, netName, c);
}

/*****************************************************************************
 *
 * Implements $unprobe command
 *
 * Parameters:
 *     c		Current circuit
 *     arc		Number of arguments
 *     argv		Argument strings
 *
 * Command summary:
 *
 *   $unprobe <net>		Remove a generic probe on a net 
 *   $unprobe <net> <who>	Remove a probe on behalf of <who> 
 *
 *****************************************************************************/
void Circuit_execUnprobe(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;
  char key[STRMAX];
  char *who = 0;
  Net *n;

  if (argc == 2) {
    n = Circuit_findNet(c,argv[1]);
    sprintf(key, "$probe:%s",argv[1]);
  } else if (argc == 3) {
    n = Circuit_findNet(c,argv[1]);
    who = argv[2];
    sprintf(key, "%s:%s",who,argv[1]);
    if (strcmp(who,"$probe") == 0) who = 0;
  } else {
    argError("$unprobe");
    return;
  }

  if (!n) {
    noNetError("$unprobe",argv[1]);
    return;
  }

  Net_removeMonitor(n);
  EvQueue_removeFinal(Q,key);
}


/*****************************************************************************
 *
 * Load memory data from a file.
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $memload file [gate]
 *
 *****************************************************************************/
void Circuit_execMemLoad(Circuit*c,int argc,char *argv[])
{
  Net *net = 0;

  if (argc == 3)
    net = Circuit_findMemoryNet(c, argv[2]);

  Circuit_readMemory(c, argv[1], net, 0, ~0, SF_HEX);
}

/*****************************************************************************
 *
 * Dump memory data to a file.
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $memload file gate					Dump one memory
 *   $memload file					Dump all memories
 *
 *****************************************************************************/
void Circuit_execMemDump(Circuit*c,int argc,char *argv[])
{
  Net *net = 0;
  const char *fileName = 0;

  if (argc != 2 && argc != 3) {
    argError("$memdump");
    return;
  }

  fileName = argv[1];

  if (argc == 3) {
    net = Circuit_findMemoryNet(c, argv[2]);
    if (!net) {
      noNetError(argv[0],argv[2]);
      return;
    }

    if (!(Net_getType(net) & NT_P_MEMORY)) {
      errorCmd(ERR_CMDNOTMEM,argv[2],argv[0]);
      return;
    }
  }


  Circuit_writeMemory(c,fileName,net,0,~0,SF_HEX);
}


void Circuit_execTime(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;

  vgio_printf("time @ %llu\n",EvQueue_getCurTime(Q));
}


/*****************************************************************************
 *
 * Step simulator a number of epochs
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $step [<count>]	Step simulator 'count' steps (or 1 step if count is
 *			omitted).
 *
 *****************************************************************************/
void Circuit_execStep(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;
  int n = 1;

  if (argc > 1)
    sscanf(argv[1],"%d",&n);

  EvQueue_stopAfter(Q,n);
  EvQueue_go(Q);
}

/*****************************************************************************
 *
 * Register a circuit clock.
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $regclock <net>
 *
 *****************************************************************************/
void Circuit_execRegClock(Circuit*c,int argc,char *argv[])
{
  Net *n;

  if (argc != 2) {
    argError("$regclock");
    return;
  }

  n = Circuit_findNet(c,argv[1]);
  if (!n) {
    noNetError("$regclock",argv[1]);
    return;
  }

  n->n_flags |= NA_CLOCK;
}

/*****************************************************************************
 *
 * Step a number of clock steps.
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $clock +/- <nclk> <overstep> [<active>]
 *
 *****************************************************************************/
void Circuit_execStepClock(Circuit *c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;
  int overstep = 0;
  transtype_t tt;
  Net *n = 0;
  int numEdge = 1;

  if (argc != 4 && argc != 5) {
    argError("$clock");
    return;
  } 

  if (argc == 5) {
    n = Circuit_findNet(c,argv[4]);
    if (!n) {
      noNetError("$regclock",argv[4]);
      return;
    }
  }

  tt = (*argv[1] == '+') ? TT_POSEDGE : TT_NEGEDGE;
  sscanf(argv[2],"%u",&numEdge);
  sscanf(argv[3],"%u",&overstep);

  EvQueue_clockStop(Q,n,numEdge,tt,(deltatime_t)overstep);
  EvQueue_go(Q);
}

/*****************************************************************************
 *
 * Set simulator options
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $option [[<name> <value>]...]
 *
 *****************************************************************************/
void Circuit_execOption(Circuit*c,int argc,char *argv[])
{
  int i;

  /* Must be an odd number of arguments */
  if ((argc & 1) != 1) {
    argError("$option");
    return;
  } 

  for (i = 1;i < argc;i += 2) {
    int value = 0;

    if (sscanf(argv[i+1],"%d",&value) != 1) {
      argError("$option");
      return;
    }

    if (strcmp(argv[i],"sec.send") == 0) {
      vgsim.vg_sec.vgs_send = value;
    } else if (strcmp(argv[i],"sec.fopen") == 0) {
      vgsim.vg_sec.vgs_fopen = value;
    } else if (strcmp(argv[i],"sec.writemem") == 0) {
      vgsim.vg_sec.vgs_writemem = value;
    } else if (strcmp(argv[i],"sec.queue") == 0) {
      vgsim.vg_sec.vgs_queue = value;
    } else if (strcmp(argv[i],"sec.exec") == 0) {
      vgsim.vg_sec.vgs_exec = value;
    } else if (strcmp(argv[i],"sec.handling") == 0) {
      vgsim.vg_sec.vgs_handling = value;
    } else {
      argError("$option");
      return;
    }
  }
}

/*****************************************************************************
 *
 * Write a value to a channel.
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $write <channel-name> <value>
 *
 *****************************************************************************/
void Circuit_execWrite(Circuit*c,int argc,char *argv[])
{
  Channel *channel;
  Value *v;

  if (argc != 3) {
    argError("$write");
    return;
  } 

  channel = Circuit_getChannel(c,argv[1]);

  v = new_Value(1);
  Value_convert(v, argv[2]);

#if 0
  {
    char buf[STRMAX];

    Value_format(v,"%d",buf);
    vgio_echo("Circuit_execWrite(%s, %s) -> %s (%x,%x,%x)\n",argv[1],argv[2],buf,v->zero[0],v->one[0],v->flt[0]);
  }
#endif

  Channel_write(channel, v);
  delete_Value(v);
}

/*****************************************************************************
 *
 * Puts a watch on a named channel.
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $qwatch <channel-name> [-format <format>]
 *
 *****************************************************************************/
void Circuit_execQWatch(Circuit*c,int argc,char *argv[])
{
  Channel *channel;
  char *format = "%h";

  if (argc == 2) {
  } else if (argc == 4) {
    if (strcmp(argv[2],"-format") != 0) {
      argError("$qwatch");
      return;
    }
    format = argv[3];
  } else {
    argError("$qwatch");
    return;
  }

  channel = Circuit_getChannel(c,argv[1]);
  if (channel)
    Channel_setWatch(channel,1,format);
}


void Circuit_execWait(Circuit*c,int argc,char *argv[])
{
}


void Circuit_execStop(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;

  EvQueue_stopAfter(Q,0);
}


/*****************************************************************************
 *
 * Begin simulation of the loaded script
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $go
 *
 *****************************************************************************/
void Circuit_execGo(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;

  EvQueue_go(Q);
}


void Circuit_execExit(Circuit*c,int argc,char *argv[])
{
}


/*****************************************************************************
 *
 * Execute an interactive command from tkgate
 *
 * Parameters:
 *     c		Current circuit
 *     cmd		Command to execute (assumed in-place modifiable)
 *
 * See README file for a summary of command recognized by the simulator.
 *
 *****************************************************************************/
void Circuit_exec(Circuit*c,char *cmd)
{
  char *argv[100];
  int argc = 0;
  char *p = cmd;
  int i;

  /*
   * Partition command string into arguments.
   */
  while (*p) {
    while (isspace(*p)) p++;
    if (!*p) break;
    argv[argc++] = p;
    while (*p && !isspace(*p)) p++;
    if (!*p) break;
    *p++ = 0;
  }

  if (argc == 0) return;

  for (i = 0;i < circExecTable_len;i++) {
    if (strcmp(argv[0],circExecTable[i].cet_command) == 0) {
      (*circExecTable[i].cet_func)(c,argc,argv);
      return;
    }
  }
  errorCmd(ERR_BADCMD,argv[0]);
}

void Circuit_execDebug(Circuit*c,int argc,char *argv[])
{
  EvQueue *Q = c->c_evQueue;

  if (argc < 2) return;

  if (strcmp(argv[1],"queue") == 0) {
    EvQueue_print(Q);
  }
}

/*****************************************************************************
 *
 * Request information about a net or memory
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $netinfo name
 *
 * Return command:
 *   netinfo name type [msb:lsb]
 *   netinfo name type [msb:lsb] [addr_min:addr_max]
 *
 *****************************************************************************/
void Circuit_execNetInfo(Circuit*c,int argc,char *argv[])
{
  Net *n;
  const char *netName;
  char buf[STRMAX];

  if (argc != 2) {
    argError("$netinfo");
    return;
  }

  netName = argv[1];

  n = Circuit_findNet(c,netName);
  if (!n) {
    noNetError("$netinfo",netName);
    return;
  }

  NT_getStr(Net_getType(n),buf);

  if ((Net_getType(n) & NT_P_MEMORY)) {
    Memory *m = Net_getMemory(n);
    vgio_printf("netinfo %s %s [%d:%d] [%u:%u]\n",
		netName,buf,Net_getMsb(n),Net_getLsb(n),
		Memory_beginAddr(m),Memory_endAddr(m));
  } else {
    vgio_printf("netinfo %s %s [%d:%d]\n",netName,buf,Net_getMsb(n),Net_getLsb(n));
  }
}

/*****************************************************************************
 *
 * Store a value in memory
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $memput name addr value
 *
 *****************************************************************************/
void Circuit_execMemPut(Circuit*c,int argc,char *argv[])
{
  Net *n;
  Memory *m;
  const char *netName;
  char buf[STRMAX];
  unsigned addr;
  unsigned beginAddr, endAddr;
  Value *data;
  int numOnLine = 0;

  if (argc != 4) {
    argError("$memput");
    return;
  }

  netName = argv[1];
  n = Circuit_findMemoryNet(c, netName);
  if (!n) {
    noNetError(argv[0],netName);
    return;
  }
  m = Net_getMemory(n);

  beginAddr = Memory_beginAddr(m);
  endAddr = Memory_endAddr(m);

  if (sscanf(argv[2],"%u",&addr) != 1) {
    errorCmd(ERR_SYNTAX);
    return;
  }
  if (addr < beginAddr || addr > endAddr) return;

  data = new_Value(Net_nbits(n));
  Value_convert(data,argv[3]);
  Memory_put(m, addr, data);
  delete_Value(data);
}

/*****************************************************************************
 *
 * Get a range of values from memory
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $memget name addr1 addr2 
 *
 * Return command:
 *   memory name addr : value1 value2 ...
 *
 *****************************************************************************/
void Circuit_execMemGet(Circuit*c,int argc,char *argv[])
{
  Net *n;
  Memory *m;
  const char *netName;
  char buf[STRMAX];
  unsigned addr1,addr2,a;
  unsigned beginAddr, endAddr;
  Value *data;
  int numOnLine = 0;

  if (argc != 4) {
    argError("$memget");
    return;
  }

  netName = argv[1];
  n = Circuit_findMemoryNet(c, netName);
  if (!n) {
    noNetError(argv[0],netName);
    return;
  }
  m = Net_getMemory(n);

  beginAddr = Memory_beginAddr(m);
  endAddr = Memory_endAddr(m);

  if (sscanf(argv[2],"%u",&addr1) != 1 || sscanf(argv[3],"%u",&addr2) != 1 || addr2 < addr1) {
    errorCmd(ERR_SYNTAX);
    return;
  }
  if (addr2  > endAddr) addr2 = endAddr;
  if (addr1  > endAddr) addr1 = endAddr;

  data = new_Value(Net_getMsb(n)-Net_getLsb(n)+1);

  numOnLine = 0;
  for (a = addr1;a <= addr2;a++) {
    char dstr[STRMAX];

    Memory_get(m, a, data);
    Value_getstr(data, dstr);
    if (numOnLine == 0) {
      vgio_printf("memory %s %u : %s",netName,a,dstr);
      numOnLine++;
    } else {
      vgio_printf(" %s",dstr);
      if (++numOnLine == 8) {
	numOnLine = 0;
	vgio_printf("\n");
      }
    }
  }
  if (numOnLine > 0)
    vgio_printf("\n");

  delete_Value(data);
}

/*****************************************************************************
 *
 * Set the watch for a memory (only one in effect at a time)
 *
 * Parameters:
 *     c		Current Circuit 
 *     argc		Number of arguments
 *     argv		Argument array
 *
 * Usage:
 *   $memwatch name addr1 addr2 
 *   $memwatch name delete 
 *
 * Return command (on events):
 *   memory name addr : value1 value2 ...
 *   memory-addr name addr
 *
 * The first return command shows the value at a memory or range of memory
 * locations.  The second return command shows the address that was most
 * recently accessed. 
 *
 *****************************************************************************/
void Circuit_execMemWatch(Circuit*c,int argc,char *argv[])
{
  Net *n;
  Memory *m;
  const char *netName;
  char buf[STRMAX];
  unsigned addr1,addr2,a;
  Value *data;
  int numOnLine = 0;
  unsigned beginAddr, endAddr;

  if (argc < 3 || argc > 4) {
    argError("$memwatch");
    return;
  }

  netName = argv[1];
  n = Circuit_findMemoryNet(c, netName);
  if (!n) {
    noNetError(argv[0],netName);
    return;
  }
  m = Net_getMemory(n);

  if (argc == 3) {
    if (strcmp(argv[2],"delete") != 0) {
      errorCmd(ERR_SYNTAX);
      return;
    }
    Memory_unsetMonitor(m);
    return;
  }

  beginAddr = Memory_beginAddr(m);
  endAddr = Memory_endAddr(m);

  if (sscanf(argv[2],"%u",&addr1) != 1 || sscanf(argv[3],"%u",&addr2) != 1 || addr2 < addr1) {
    errorCmd(ERR_SYNTAX);
    return;
  }
  if (addr2  > endAddr) addr2 = endAddr;
  if (addr1  > endAddr) addr1 = endAddr;

  Memory_setMonitor(m,addr1,addr2);
}
