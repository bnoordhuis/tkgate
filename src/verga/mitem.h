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

    Last edit by hansen on Fri Jan  9 22:53:43 2009
****************************************************************************/
#ifndef __mitem_h
#define __mitem_h

/*
 * Module item flags
 */
#define MIF_IMMEDIATE	0x1		/* Schedule item for immediate execution on startup */


/*****************************************************************************
 *
 * itemcode_t - code for module item type
 *
 *****************************************************************************/
typedef enum {
  IC_ASSIGN,		/* assign statement */
  IC_ALWAYS,		/* always statement */
  IC_INITIAL,		/* initial statement */
  IC_INSTANCE,		/* module instantiation */
  IC_PARAMETER,		/* module parameter */
  IC_REGINIT,		/* register initialization */
  IC_NETDECL,		/* net declaration */
  IC_GATE,		/* gate instantiation */
} itemcode_t;

/*****************************************************************************
 *
 * gateargcode_t - code for primitive gate argument handling
 *
 *****************************************************************************/
typedef enum {
  GAC_MULTIN,		/* Multiple inputs */
  GAC_MULTOUT,		/* Multiple outputs */
  GAC_FIXED,		/* Fixed number of ports */
} gateargcode_t;

typedef struct GateDesc_str {
  int		gd_code;	/* Code for this gate type */
  char		*gd_name;	/* Name of this gate type */
  gateargcode_t	gd_gac;		/* Gate argument handling */
  int		gd_minPorts;	/* Minimum number of ports */
  valueop_f	*gd_baseFunc;	/* Base function */
  valueop_f	*gd_outFunc;	/* Final output function */
} GateDesc;

/*****************************************************************************
 *
 * NetDecl - Declaration of a net in a module.
 *
 *
 *****************************************************************************/
typedef struct {
  char		*n_name;		/* Name of net */
  nettype_t	n_type;			/* Type of net */
  VRange	*n_range;		/* Range for declaration (object is shared) */
  VRange	*n_addrRange;		/* Address range for memories */
  Place		n_place;		/* Place where net was declared */
} NetDecl;


typedef VGThread *MIgenerate_f(ModuleItem *mi,ModuleInst *modCtx,CodeBlock *cb);
typedef void MIprint_f(ModuleItem *mi,FILE *f);

/*****************************************************************************
 *
 * Vtable for MItem objects
 *
 *****************************************************************************/
typedef struct {
  MIgenerate_f	*miv_generate;		/* Generate code */
  MIprint_f	*miv_print;		/* Print mitem */
}  MItemVTable;


/*****************************************************************************
 *
 * MICommon - Common field in mitem objects
 *
 *****************************************************************************/
typedef struct {
  int		mb_type;		/* Type code of mitem */
  MItemVTable	*mb_vtable;		/* Virtual function pointer table */
  Place 	mb_place;		/* Location of declaration */
  DynamicModule	*mb_dynMod;		/* Dynamic module if dynamically loaded */
  unsigned	mb_flags;		/* Item flags */
} MICommon;

/*****************************************************************************
 *
 * MIParameter - A parameter declaration
 *
 *****************************************************************************/
typedef struct {
  MICommon	mip_common;		/* Common fields */
  const char	*mip_name;		/* Name of parameter */
  Expr		*mip_expr;		/* Parameter expression */
  int		mip_ppPos;		/* Parameter port position (-1 if not port) */
} MIParameter;

/*****************************************************************************
 *
 * MINetDecl - A net declaration
 *
 *****************************************************************************/
typedef struct {
  MICommon	mid_common;		/* Common fields */
  NetDecl	*mid_decl;		/* Net declaration */
} MINetDecl;

/*****************************************************************************
 *
 * MIAssign - An assignment
 *
 * Examples:
 *   assign w2 = a & b;
 *   assign w3 = #5 a & b;
 *   assign {w4,w5[2]} = #5 a & b;
 *
 *****************************************************************************/
typedef struct {
  MICommon	mia_common;		/* Common fields */
  Expr		*mia_lhs;		/* Left hand side of assignment */
  Expr		*mia_rhs;		/* Right hand side of assignment */
  Expr		*mia_bcond;		/* Block condition (delay/trigger) */
} MIAssign;

/*****************************************************************************
 *
 * MIBlock - An initial or always block
 *
 *****************************************************************************/
typedef struct {
  itemcode_t	mib_type;		/* Type code IC_INITIAL or IC_ALWAYS */
  Place 	mib_place;		/* Location of declaration */
  StatDecl	*mib_stat;		/* Statement to execute */
} MIBlock;

/*****************************************************************************
 *
 * MIInstance - An instance item
 *
 *****************************************************************************/
typedef struct {
  MICommon		mii_common;		/* Common fields */
  const	char		*mii_name;		/* Name of instance type */	
  const	char		*mii_instName;		/* Name of instance */	
  VRange		*mii_slices;		/* Range/number of slices */
  List/*NameExpr*/	*mii_parms;		/* Parameters */
  List/*NameExpr*/	*mii_ports;		/* Ports */
} MIInstance;


/*****************************************************************************
 *
 * MIGate - A gate instance item
 *
 *****************************************************************************/
typedef struct {
  MICommon		mig_common;		/* Common fields */
  GateDesc		*mig_desc;		/* Type description */	
  Expr			*mig_delay;		/* Delay expression */
  const	char		*mig_instName;		/* Name of instance */	
  VRange		*mig_slices;		/* Range/number of slices */
  List/*Expr*/		*mig_ports;		/* Ports */
} MIGate;


/*****************************************************************************
 *
 * ModuleItem - Object that can be at the top level in a module description.
 *
 * There are four types of modules items: assign, initial, always, instance.
 *
 *****************************************************************************/
union ModuleItem_uni {
  itemcode_t	mi_type;		/* Type of module item */
  MICommon	mi_base;		/* Base item members */
  MIAssign	mi_asgn;		/* An "assign" item */
  MIBlock	mi_block;		/* An "initial" or "always" item */
  MIInstance	mi_inst;		/* An instance item */
  MIGate	mi_gate;		/* An gate instance item */
  MINetDecl	mi_netdec;		/* An net declaration item */
  MIParameter	mi_parm;		/* An parameter item */
};

/*****************************************************************************
 * ModuleItem methods
 *****************************************************************************/
ModuleItem *new_ModuleItem(int mitype);
void ModuleItem_print(ModuleItem *mi,FILE *f);
VGThread *ModuleItem_generate(ModuleItem *mi,ModuleInst *modCtx,CodeBlock *cb);
#define ModuleItem_getPlace(mi) (&((ModuleItem*)(mi))->mi_base.mb_place)
#define ModuleItem_getType(mi) (mi)->mi_type
#define ModuleItem_getDynamicModule(mi) ((ModuleItem*)(mi))->mi_base.mb_dynMod
#define ModuleItem_setDynamicModule(mi,dm) (((ModuleItem*)(mi))->mi_base.mb_dynMod = (dm))
void ModuleItem_killNotify(ModuleItem *mi);
#define ModuleItem_getFlags(mi) (mi)->mi_flags.mb_flags
#define ModuleItem_setFlags(mi,flags) ((mi)->mi_base.mb_flags |= (flags))
#define ModuleItem_clearFlags(mi,flags) ((mi)->mi_base.mi_flags &= ~(flags))

/*****************************************************************************
 * MIAssign methods
 *****************************************************************************/
MIAssign *new_MIAssign(Expr*,Expr*,Expr*);
int MIAssign_pathdGenerate(MIAssign *mia,ModuleInst *mi,CodeBlock *codeBlock,List *asgns); 

/*****************************************************************************
 * MIBlock methods
 *****************************************************************************/
MIBlock *new_MIBlock(itemcode_t,StatDecl*);

/*****************************************************************************
 * MIInstance methods
 *****************************************************************************/
MIInstance *new_MIInstance(const char *name, const char *instName, VRange *slices, List *parms, List *ports);
Expr *MIInstance_findParm(MIInstance *mi,const char *name, int ppIdx);

/*****************************************************************************
 * MIGate methods
 *****************************************************************************/
MIGate *new_MIGate(unsigned gateType, Expr *delay, const char *instName, VRange *slices, List *ports);
int MIGate_pathdGenerate(MIGate *mig,ModuleInst *mi,CodeBlock *codeBlock,List *asgns); 

/*****************************************************************************
 * MINetDecl methods
 *****************************************************************************/
MINetDecl *new_MINetDecl(NetDecl *nd);

/*****************************************************************************
 * MIParameter methods
 *****************************************************************************/
MIParameter *new_MIParameter(const char *name,Expr *expr);

/*****************************************************************************
 * NetDecl methods
 *****************************************************************************/
NetDecl *new_NetDecl(const char *name,int wtype,VRange *range,VRange *addrRange,Place *place);
#define NetDecl_getType(nd) (nd)->n_type
#define NetDecl_getRange(nd) (nd)->n_range
#define NetDecl_getName(nd) (nd)->n_name
#define NetDecl_getPlace(nd) (&(nd)->n_place)

int NT_getStr(nettype_t nt,char *buf);
void NT_printDecl(nettype_t nt,FILE *f);

#endif
