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

    Last edit by hansen on Sat Sep  9 23:51:04 2006
****************************************************************************/
#ifndef __specify_h
#define __specify_h

#define SPF_ACTIVE 0x1		/* Specify block is being actively generated */

typedef enum {
  SPT_MPATH = 0,
  SPT_DPATH = 1
} spectype_t ;

/*****************************************************************************
 *
 * SpecifyStat Path delay specification
 *
 *****************************************************************************/
struct SpecifyStat_str {
  Expr			*ss_cond;	/* Condition of specify */
  spectype_t		ss_ptype;	/* Type of connection */
  List/*Expr*/		ss_from;	/* From net literals */
  List/*Expr*/		ss_to;		/* To net literals */
  Expr			*ss_delay;	/* Delay expression */ 
  unsigned		ss_idelay;	/* Computed delay value (must computed before use) */ 
};

/*****************************************************************************
 *
 * All specify block data
 *
 *****************************************************************************/
struct Specify_str {
  unsigned		s_flags;	/* Flags for specify block */
  ModuleDecl		*s_module;	/* Module declaration */
  List/*StatDecl*/	s_tasks;	/* Specify tasks such as $setup() */
  List/*SpecifyStat*/	s_stats;	/* Path delay specifications */

  SHash/*Expr*/		s_specParms;	/* Specify parameters */
};

/*****************************************************************************
 * SpecifyStat member functions
 *****************************************************************************/
SpecifyStat *new_SpecifyStat(Expr *cond,spectype_t ptype,Expr *lhs,Expr *rhs,Expr *value);

/*****************************************************************************
 * Specify member functions
 *****************************************************************************/
Specify *new_Specify(ModuleDecl *);
void delete_Specify(Specify *);
void Specify_init(Specify *,ModuleDecl *);
void Specify_uninit(Specify *);
void Specify_addStat(Specify *,SpecifyStat *);
void Specify_addTask(Specify *,StatDecl *);
#define Specify_numStats(s) List_numElems(&(s)->s_stats)
#define Specify_numTasks(s) List_numElems(&(s)->s_tasks)
#define Specify_getFlags(s) (s)->s_flags
#define Specify_getStats(s) (&(s)->s_stats)
void Specify_generateTasks(Specify *s,ModuleInst *mi,CodeBlock *cb);
int Specify_addSpecParam(Specify *,const char *name,Expr *expr);
int Specify_getSpecParm(Specify *,const char *name, ModuleInst *mi, int *value);
int Specify_getDelays(Specify*,const char *src,const char *dst,List/*SpecifyStat**/ *match);

#endif
