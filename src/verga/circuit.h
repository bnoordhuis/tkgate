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

    Last edit by hansen on Sun Dec  7 20:20:00 2008
****************************************************************************/
#ifndef __circuit_h
#define __circuit_h

/*****************************************************************************
 *
 * Circuit - Class to represent final instantiated circuit
 *
 *****************************************************************************/
struct Circuit_str {
  SHash/*Net*/		c_nets;			/* Global table of nets */
  NHash/*Trigger*/	c_triggers;		/* Triggers in this circuit */
  SHash/*Channel*/	c_channels;		/* Communication channels */
  SHash/*ModuleInst*/	c_moduleInsts;		/* Module instances */
  ModuleInst		*c_root;		/* Root module instance */
  EvQueue		*c_evQueue;		/* Global event queue */
  SHash/*DynamicModule*/ c_dynamicModules;	/* Dynamicly loaded modules */
};

/*****************************************************************************
 * DynamicModule member functions
 *****************************************************************************/
DynamicModule *new_DynamicModule(const char*);
void delete_DynamicModule(DynamicModule*);
void DynamicModule_addMItem(DynamicModule*,ModuleItem*);
void DynamicModule_addThread(DynamicModule*,VGThread*);
#define DynamicModule_setModuleInst(dm,mi) ((dm)->dm_moduleInst = (mi))
void DynamicModule_killNotify(DynamicModule*);

/*****************************************************************************
 * Circuit member functions
 *****************************************************************************/
void Circuit_init(Circuit *c);
void Circuit_build(Circuit *c,ModuleDecl *m);
void Circuit_buildPathDelayMod(Circuit *c,ModuleInst *mi,ModuleInst *parent,char *path);
void Circuit_installScript(Circuit *c,ModuleDecl *m,DynamicModule *dm);
void Circuit_check(Circuit *c);
void Circuit_run(Circuit *c);
Net *Circuit_findNet(Circuit *c,const char *name);
Net *Circuit_findMemoryNet(Circuit *c,const char *name);
ModuleInst *Circuit_findModuleInst(Circuit *c, const char *name);
Trigger *Circuit_getTrigger(Circuit *c,List *posedge,List *negedge);
Trigger *Circuit_getNetTrigger(Circuit *c,Net*,transtype_t);
#define Circuit_getQueue(c) (c)->c_evQueue
#define Circuit_update(c) EvQueue_update((c)->c_evQueue)
void Circuit_exec(Circuit*c,char *cmd);
void Circuit_readMemory(Circuit *c, const char *fileName, Net *net, unsigned start, unsigned stop, unsigned flags);
int Circuit_writeMemory(Circuit *c, const char *fileName, Net *net, unsigned start, unsigned stop, unsigned flags);
Channel *Circuit_getChannel(Circuit *c, const char*name);
void Circuit_sortThreads(Circuit *c);
void Circuit_unloadDynamicModule(Circuit *c,DynamicModule *dm);
void Circuit_enableDynamicModule(Circuit *c,DynamicModule *dm);
void Circuit_disableDynamicModule(Circuit *c,DynamicModule *dm);
void Circuit_declareNet(Circuit *c,Scope *scope,NetDecl *nd,ModuleDecl *md,Place *place); 
void Circuit_execScript(Circuit*c,int argc,char *argv[]);
void Circuit_finishModuleInst(Circuit *c, ModuleInst *mi, CodeBlock *codeBlock);
#define Circuit_getRoot(c) (c)->c_root
Scope *Circuit_getUpScope(Circuit *c,Scope *s);

#endif
