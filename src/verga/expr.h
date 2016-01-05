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

    Last edit by hansen on Sat Sep  9 19:28:06 2006
****************************************************************************/
#ifndef __expr_h
#define __expr_h

/*****************************************************************************
 *
 * Table of expression codes.
 *
 *****************************************************************************/
typedef enum {
  E_NOT		= 0,
  E_UINV	= 1,
  E_UNEG	= 2,
  E_POSEDGE	= 3,
  E_NEGEDGE	= 4,
  E_MUL		= 5,
  E_DIV		= 6,
  E_MOD		= 7,
  E_ADD		= 8,
  E_SUB		= 9,
  E_RSHIFT	= 10,
  E_LSHIFT	= 11,
  E_ARSHIFT	= 12,
  E_ALSHIFT	= 13,
  E_GT		= 14,
  E_LT		= 15,
  E_GE		= 16,
  E_LE		= 17,
  E_EQ		= 18,
  E_NE		= 19,
  E_BAND	= 20,
  E_BNAND	= 21,
  E_BXOR	= 22,
  E_BNXOR	= 23,
  E_BOR		= 24,
  E_BNOR	= 25,
  E_AND		= 26,
  E_OR		= 27,
  E_QUEST	= 28,
  E_AT		= 29,
  E_DELAY	= 30,
  E_CONCAT	= 31,
  E_REPCAT	= 32,
  E_EQZ		= 33,
  E_NEZ		= 34,
  E_VECTORP	= 35,
  E_VECTORN	= 36,
  E_RANGE	= 37,
  E_UAND	= 38,
  E_UOR		= 39,
  E_UXOR	= 40,
  E_UNAND	= 41,
  E_UNOR	= 42,
  E_UNXOR	= 43,
  E_COND	= 44,

  E_LITERAL	= 1000,
  E_HEX		= 1001,
  E_NUMBER	= 1002,
  E_EVENTOR	= 1005,
  E_TASK	= 1006,
  E_REAL	= 1007,
} exprcode_t;

/*****************************************************************************
 *
 * Operator output bit size.
 *
 *****************************************************************************/
typedef enum {
  OS_NONE,		/* Output size is not applicable */
  OS_ONE,		/* Output is always 1-bit */
  OS_MAX,		/* Output is size of largest operand */
  OS_SUM,		/* Output size is sum of operands */
  OS_SPECIAL,		/* Special handling for output size */
} outsize_t;

/*****************************************************************************
 *
 * Range styles
 *
 *****************************************************************************/
typedef enum {
  RS_SINGLE,		/* This is the [expr] form */
  RS_MAXMIN,		/* This is the [max:min] form */
  RS_BASEUP,		/* This is the [base+:width] form */
  RS_BASEDN,		/* This is the [base-:width] form */
  RS_AUTO,		/* This is the [*] form */
} rangestyle_t;

typedef enum {
  PEF_NONE = 0x0,	/* Normal processing */
  PEF_SPECPARM = 0x1,	/* Lookup specify parameters */
} parmevflags_t;

/*****************************************************************************
 *
 * Expr - Verilog expression object
 *
 * This object is used to represent a Verilog expression.
 *
 *****************************************************************************/
struct Expr_str {
  exprcode_t	e_type;		/* Expression type */
  int		e_nbits;	/* Number of bits in expression (or -1 for unsized) */
  union {
    Expr	*opr[3];	/* Operator arguments */
    Value	*snum;		/* Sized number */
    struct {
      char	*name;		/* Literal name */
      int	ishlit;		/* Is this a hierarchical literal */
    } literal;
    struct {
      char *name;		/* Name of task */
      int argc;			/* Number of arguments */
      Expr **argv;		/* Arguments passed */
    } task;
  } e;
};

/*****************************************************************************
 *
 * NameExpr - Name/Expr pair
 *
 *****************************************************************************/
typedef struct {
  char		*ne_name;	/* Name */
  Expr		*ne_expr;	/* Expression */
} NameExpr;

/*****************************************************************************
 *
 * OpDesc - Operator description
 *
 *****************************************************************************/
typedef struct {
  exprcode_t	od_type;	/* Type code for operand */
  int		od_plev;	/* Precidence level */
  int 		od_nopr;	/* Number of operands */
  char		*od_text;	/* Text for operand */
  outsize_t	od_outSize;	/* Code representing output bit sizing */
  valueop_f	*od_opfunc;	/* Function implementing operator */
  valueop_f	*od_w_opfunc;	/* Function implementing operator when operands <32 bits */
  valueop_f	*od_f_opfunc;	/* Function implementing floating point operands */
} OpDesc;


/*****************************************************************************
 *
 * VRange - Range declaration (may contain uninstantiated parameters)
 *
 *****************************************************************************/
typedef struct {
  rangestyle_t	vr_style;	/* Range style */
  Expr	*vr_left,*vr_right;	/* Left and right portions of range */
} VRange;

/*****************************************************************************
 * Expr methods
 *****************************************************************************/
Expr *new_Expr_lit(const char *name);
Expr *new_Expr_op1(exprcode_t op,Expr *e);
Expr *new_Expr_op(exprcode_t op,Expr *l,Expr *r);
Expr *new_Expr_op3(exprcode_t op,Expr *l,Expr *m,Expr *r);
Expr *new_Expr_repcat(Expr *n,Expr *e);
Expr *new_Expr_realnum(real_t);
Expr *new_Expr_num(int);
Expr *new_Expr_hex(const char *spec);
Expr *new_Expr_str(const char *s);
Expr *new_Expr_task(const char *name,List *);
void delete_Expr(Expr*e);
void Expr_print(Expr*e,FILE *f);
char *Expr_getstr(Expr*e,char *p);
Value *Expr_parmEval(Expr *e,Scope *scope,parmevflags_t);
int Expr_parmEvalI(Expr *e,Scope *scope,unsigned *n,parmevflags_t);
void Expr_getLastError(char*);
int Expr_getBitSize(Expr *e,Scope *scope);
int Expr_getCollapsedBitSize(Expr *e,Scope *scope);
Value *Expr_generate(Expr *e,int nbits,Scope *scope,CodeBlock *cb);
Value *Expr_generateS(Expr *e,Scope *scope,CodeBlock *cb);
#define Expr_type(ex) (ex)->e_type
#define Expr_getLitName(ex) (ex)->e.literal.name
#define Expr_isHLiteral(ex) (ex)->e.literal.ishlit

void Expr_expandConcat(Expr *e,Scope *scope, List *l);
void Expr_makeTriggerList(Expr *trigger, List *triggerList);
Trigger *Expr_getTrigger(Expr *trigger,Scope *scope, StatDecl *stat);
int Expr_generateTrigger(Expr *triggerExpr, Scope *scope, CodeBlock *cb, StatDecl *stat);
int Expr_generateBCond(Expr *bcond, Scope *scope, CodeBlock *cb, StatDecl *stat);
int Expr_getDelay(Expr *delayExpr,Scope *scope,Timescale *ts,deltatime_t *delay);
int Expr_generateDelay(Expr *delay, Scope *scope, CodeBlock *cb);
void Expr_getReaders(Expr*e, Scope *scope, PHash *H);
void Expr_getStaticReaders(Expr*e, SHash *H);
Trigger *Expr_getDefaultTrigger(Expr *e,Scope *scope);
Trigger *Expr_getDefaultTriggerFromSet(PHash *P,Circuit *c);
int Expr_lhsGenerate(Expr *e,Scope *scope, CodeBlock *cb,Net **n,Value **nLsb,unsigned *size,Value **nAddr);
int Expr_decodeVector(Expr *e,Scope *scope,Net **n,VRange **addr,VRange **bits);

/*****************************************************************************
 * VRange methods
 *****************************************************************************/
VRange *new_VRange(rangestyle_t ,Expr *msb,Expr *lsb);
void delete_VRange(VRange *r, int recursive);
void VRange_print(VRange *r,FILE *f);
int VRange_parmEvalMsb(VRange *r,Scope *scope,unsigned *msb);
int VRange_parmEvalLsb(VRange *r,Scope *scope,unsigned *lsb);
Expr *VRange_getLsb(VRange*);
int VRange_getSize(VRange*,Scope *scope,unsigned *width);
int VRange_getDirect(VRange *r,char *buf);
#define VRange_getStyle(r) (r)->vr_style

/*****************************************************************************
 * NameExpr methods
 *****************************************************************************/
NameExpr *new_NameExpr(const char *name,Expr *e);

/*****************************************************************************
 * OpDesc methods
 *****************************************************************************/
OpDesc *OpDesc_findFunc(valueop_f *opfunc);
#endif
