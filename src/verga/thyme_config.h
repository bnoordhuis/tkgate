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
#ifndef __vgsim_config_h
#define __vgsim_config_h

#define VGSIM_NAME		"Verga"		/* Name of program */
#define VGSIM_VERSION		"1.0"		/* Version number */
#define VGSIM_COPYRIGHT		"Copyright (C) 2005-2009 by Jeffery P. Hansen"
#define STRMAX			1024		/* Length of longest string */

#define THYMEWHEEL_SIZE		0x1000		/* Size of timewheel (must be power of two) */
#define THYMEWHEEL_MASK		0x0fff		/* Mask for thymewheel (= size-1) */

#define POLL_RATE		50		/* Time between input checks (milliseconds) */

/* 
 * Maximum number of arguments that can have a type specification
 */
#define STASK_MAXSPECARGS 8

/*****************************************************************************
 * advance declarations from specify.h
 *****************************************************************************/
typedef struct Specify_str Specify;
typedef struct SpecifyStat_str SpecifyStat;

/*****************************************************************************
 * advance declarations from value.h
 *****************************************************************************/
typedef struct Value_str Value;
typedef int valueop_f(Value *r,Value *a,Value *b,Value *c);

/*****************************************************************************
 * advance declarations from mitem.h
 *****************************************************************************/
typedef union ModuleItem_uni ModuleItem;

/*****************************************************************************
 * advance declarations from module.h
 *****************************************************************************/
typedef struct ModuleInst_str ModuleInst;
typedef struct ModuleDecl_str ModuleDecl;
typedef struct ScopeDecl_str ScopeDecl;
typedef struct Scope_str Scope;

/*****************************************************************************
 * advance declarations from bytecode.h
 *****************************************************************************/
typedef union ByteCode_union ByteCode;
typedef struct VGThread_str VGThread;
typedef void BCfunc(ByteCode *bc,VGThread *t);
typedef struct CodeBlock_str CodeBlock;

/*****************************************************************************
 * advance declarations from expr.h 
 *****************************************************************************/
typedef struct Expr_str Expr;

/*****************************************************************************
 * advance declarations from trigger.h 
 *****************************************************************************/
typedef struct Trigger_str Trigger;

/*****************************************************************************
 * advance declarations from thyme.h
 *****************************************************************************/
typedef union Event_union Event;
typedef struct EvQueue_str EvQueue;
typedef unsigned long long simtime_t;		/* simulation time */
typedef unsigned long long deltatime_t;		/* time delay */

/*****************************************************************************
 * advance declarations from statment.h
 *****************************************************************************/
typedef union StatDecl_union StatDecl;

/*****************************************************************************
 * advance declarations from circuit.h
 *****************************************************************************/
typedef struct Circuit_str Circuit;
typedef struct DynamicModule_str DynamicModule;

/*****************************************************************************
 * advance declarations from task.h
 *****************************************************************************/
typedef struct UserTaskDecl_str UserTaskDecl;
typedef struct UserTask_str UserTask;

/*****************************************************************************
 * advance declarations from net.h
 *****************************************************************************/
typedef unsigned nettype_t;
typedef struct Net_str Net;

/*****************************************************************************
 * advance declarations from channel.h
 *****************************************************************************/
typedef struct Channel_str Channel;

/*****************************************************************************
 * advance declarations from directive.h
 *****************************************************************************/
typedef struct Timescale_str Timescale;

#endif
