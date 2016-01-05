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
#ifndef __module_h
#define __module_h

#define SDF_LOCAL_ONLY		0x1	/* Lookup variable only in the local scope */

/*****************************************************************************
 *
 * ScopeDecl - Scope in which variables can be declared
 *
 *****************************************************************************/
struct ScopeDecl_str {
  ScopeDecl		*sd_parent;	/* Parent scope */
  SHash/*NetDecl*/	sd_nets;	/* Net declarations */  
};


/*****************************************************************************
 *
 * Scope - Scope in which instantiated variables are declared
 *
 *****************************************************************************/
struct Scope_str {
  char			*s_path;	/* Path name of scope */
  ModuleInst		*s_instance;	/* Module instance this scope is in */
  Scope			*s_parent;	/* Parent scope */
  Scope			*s_peer;	/* "Peer" scope */
  SHash/*Net*/		s_nets;		/* Local scope table of nets/parameters */
  SHash/*UserTask*/	s_tasks;	/* User tasks */
};

/*****************************************************************************
 *
 * FaninNode - Used to build a fanin tree for a path-delay module.  Fanin nodes
 * are only used if a module contains a "specify" block with a path delay.
 *
 *****************************************************************************/
typedef struct FaninNode_str {
  ModuleItem	*fn_item;	/* Module item represented */
  char		**fn_out;	/* Names of fanouts */
  char		**fn_in;	/* Names of fanins */
  Net		**fn_outNets;	/* Nets associated with outputs */
  Net		**fn_inNets;	/* Nets associated with inputs */
  int		fn_flag;	/* Flag used in module generation */
} FaninNode;


/*****************************************************************************
 *
 * ModuleDecl - Declaration of a module
 *
 *****************************************************************************/
struct ModuleDecl_str {
  char			*m_name;	/* Module name */
  Place			m_place;	/* Place of declarations */
  List/*char**/		m_ports;	/* Port names */
  List/*char**/		m_parmPorts;	/* Parameter ports */
  ScopeDecl		m_scope;	/* Variable declaration scope */
  SHash/*UserTaskDecl*/	m_tasks;	/* User tasks */
  List			m_items;	/* Module "items" */
  unsigned		m_errorsDone;	/* We are done doing error reporting */
  Specify		*m_specify;	/* Specify block of module */
  SHash/*FaninNode*/	*m_faninnodes;	/* Fanin nodes (only used for path-delay modules) */
  Timescale		m_timescale;	/* Timescale of this module */
};

/*****************************************************************************
 *
 * ModuleInst - Context information for current module being expanded
 *
 *****************************************************************************/
struct ModuleInst_str {
  char			*mc_path;	/* Path for this context */
  ModuleInst		*mc_peer;	/* Peer module (used for simulation scripts) */
  ModuleDecl		*mc_mod;	/* Module definition */
  Circuit		*mc_circuit;	/* Circuit we are building */
  ModuleInst		*mc_parent;	/* Parent instance */
  List/*VGThread*/	mc_threads;	/* Threads in the module instance */
  Scope			mc_scope;	/* Scope in which varaibles/tasks are defined */
  CodeBlock		*mc_codeBlock;	/* Code block for this module instance */
};

/*****************************************************************************
 *
 * Dynamic modules are used to keep track of mitems that are loaded as part
 * of a script or breakpoint so that it can be dynamically unloaded on request.
 *
 *****************************************************************************/
struct DynamicModule_str {
  char			*dm_name;		/* tag name of dynamic module */
  List			dm_mitems;		/* MItems in the dynamc module */

  ModuleInst		*dm_moduleInst;		/* Module instance */
  List			dm_threads;		/* Threads corresponding to module */
  int			dm_aliveThreads;	/* Number of threads currently alive */
};

/*****************************************************************************
 * ScopeDecl methods
 *****************************************************************************/
void ScopeDecl_init(ScopeDecl *s,ScopeDecl *parent);
void ScopeDecl_uninit(ScopeDecl *s);
NetDecl *ScopeDecl_findNet(ScopeDecl *s,const char *name,unsigned flags);
void ScopeDecl_defNet(ScopeDecl *s,NetDecl*n);


/*****************************************************************************
 * Scope methods
 *****************************************************************************/
Scope *new_Scope(const char *path,Scope *parent,ModuleInst *mi);
void delete_Scope(Scope *s);
void Scope_init(Scope *s,const char *path,Scope *parent,ModuleInst *mi);
void Scope_uninit(Scope *s);
void Scope_setPeer(Scope *s,Scope *peer);
Net *Scope_findNet(Scope *s,const char *name,unsigned flags);
void Scope_defNet(Scope *s,const char *name,Net *n);
void Scope_replaceLocalNet(Scope *s,const char *name,Net *n);
UserTask *Scope_findTask(Scope *mi,const char *name);
int Scope_defineTask(Scope *mi,const char *name,UserTask *ut);
Value *Scope_findParm(Scope *scope,const char *name);
#define Scope_getModuleInst(s) (s)->s_instance
#define Scope_getPath(s) (s)->s_path
#define Scope_getParent(s) (s)->s_parent

/*****************************************************************************
 * ModuleDecl methods
 *****************************************************************************/
ModuleDecl *new_ModuleDecl(const char *);
void delete_ModuleDecl(ModuleDecl*);
void ModuleDecl_addPort(ModuleDecl*,const char*);
void ModuleDecl_defineParm(ModuleDecl *m,const char *name,Expr *e,int isPort);
int ModuleDecl_isParm(ModuleDecl *m,const char *name);
void ModuleDecl_print(ModuleDecl*,FILE*);
NetDecl *ModuleDecl_findNet(ModuleDecl *m,const char *name);
void ModuleDecl_defNet(ModuleDecl *m,NetDecl*n);
#define ModuleDecl_getName(m) (m)->m_name
void ModuleDecl_printData(ModuleDecl*);
#define ModuleDecl_getSpecify(m) (m)->m_specify
#define ModuleDecl_getPlace(m) (&(m)->m_place)
void ModuleDecl_startSpecify(ModuleDecl*m);
int ModuleDecl_numPorts(ModuleDecl *m,nettype_t ptype);
UserTaskDecl *ModuleDecl_findTask(ModuleDecl *m,const char *name);
int ModuleDecl_defineTask(ModuleDecl *m,const char *name,UserTaskDecl *utd);
#define ModuleDecl_getNets(m) (&(m)->m_scope.sd_nets)
#define ModuleDecl_getScope(m) (&(m)->m_scope)
int ModuleDecl_makeFaninTree(ModuleDecl *m);
void ModuleDecl_clearFaninFlags(ModuleDecl *m);
#define ModuleDecl_findFanin(m,name) ((FaninNode*)SHash_find((m)->m_faninnodes, name))
#define ModuleDecl_isPathDelayMod(m) ((m)->m_specify && Specify_numStats((m)->m_specify) > 0)
void ModuleDecl_makeSpecify(ModuleDecl *m);
#define ModuleDecl_getTimescale(m) (&(m)->m_timescale)

/*****************************************************************************
 * ModuleInst member functions
 *****************************************************************************/
ModuleInst *new_ModuleInst(ModuleDecl *md,Circuit *c,ModuleInst *parent,const char *path);
void delete_ModuleInst(ModuleInst*);
void ModuleInst_init(ModuleInst *mc,ModuleDecl *md,Circuit *c,ModuleInst *parent,const char *path);
void ModuleInst_uninit(ModuleInst *mc);
Value *ModuleInst_findParm(ModuleInst *mc,const char *name);
Net *ModuleInst_findNet(ModuleInst *mc,const char *name);
void ModuleInst_defParm(ModuleInst *mc,const char *name,Value *value);
void ModuleInst_defNet(ModuleInst *mc,const char *name,Net *n);
const char *ModuleInst_findLocalNetName(ModuleInst *mc,Net *n);
#define ModuleInst_getPath(mc) (mc)->mc_path
#define ModuleInst_getCircuit(mc) (mc)->mc_circuit
#define ModuleInst_setCodeBlock(mc,cb) ((mc)->mc_codeBlock = (cb))
#define ModuleInst_getModDecl(mc) (mc)->mc_mod
UserTask *ModuleInst_findTask(ModuleInst *m,const char *name);
int ModuleInst_defineTask(ModuleInst *m,const char *name,UserTask *ut);
#define ModuleInst_getScope(mc) (&(mc)->mc_scope)
#define ModuleInst_isPathDelayMod(mi) ModuleDecl_isPathDelayMod((mi)->mc_mod)
#define ModuleInst_addThread(mi, t) List_addToTail(&(mi)->mc_threads,(t))
#define ModuleInst_getTimescale(mi) ModuleDecl_getTimescale((mi)->mc_mod)

FaninNode *new_FaninNode(ModuleItem *item);

#endif
