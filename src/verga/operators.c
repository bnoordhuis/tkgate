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

    Last edit by hansen on Mon Dec 22 20:06:49 2008
****************************************************************************/
#include <assert.h>
#include <ctype.h>
#include "thyme.h"

/*****************************************************************************
 *
 * Table of operators, precidence levels and symbols.  This table is primarily
 * used for printing expressions.  The entries in the table are:
 *
 *  opcode	Code symbol for this operation
 *  prec.	Precedence of the operator
 *  #opr.	Number of operands
 *  sym		Symbol for this operator (used for printing)
 *  bitsize	Result bit-size computation method
 *  func.	General handler function
 *  w.func.	Handler function optimized for operands up to native word size.
 *
 *****************************************************************************/
OpDesc opTable[] = {
  /*opcode	prec.	#opr.	sym	bitsize		func.		w.func.         f.func.*/
  {E_NOT,	12,	1,	"!",	OS_ONE,		Value_lnot,	0,		0},

  {E_UINV,	12,	1,	"~",	OS_MAX,		Value_not,	Value_w_not,	0},	
  {E_UNEG,	12,	1,	"-",	OS_MAX,		Value_neg,	Value_w_neg,	Value_f_neg},	
  {E_UAND,	12,	1,	"&",	OS_MAX,		Value_uand,	Value_w_uand,	0},	
  {E_UOR,	12,	1,	"|",	OS_MAX,		Value_uor,	Value_w_uor,	0},	
  {E_UXOR,	12,	1,	"^",	OS_MAX,		Value_uxor,	Value_w_uxor,	0},	
  {E_UNAND,	12,	1,	"~&",	OS_MAX,		Value_unand,	Value_w_unand,	0},	
  {E_UNOR,	12,	1,	"~|",	OS_MAX,		Value_unor,	Value_w_unor,	0},	
  {E_UNXOR,	12,	1,	"~^",	OS_MAX,		Value_unxor,	Value_w_unxor,	0},	

  {E_POSEDGE,	12,	1,	"posedge ", OS_NONE,	0,		0,		0},
  {E_NEGEDGE,	12,	1,	"negedge ", OS_NONE,	0,		0,		0},

  {E_MUL,	11,	2,	"*",	OS_MAX,		Value_mul,	Value_w_mul,	Value_f_mul},
  {E_DIV,	11,	2,	"/",	OS_MAX,		Value_div,	Value_w_div,	Value_f_div},
  {E_MOD,	11,	2,	"%",	OS_MAX,		Value_mod,	Value_w_mod,	0},

  {E_ADD,	10,	2,	"+",	OS_MAX,		Value_add,	Value_w_add,	Value_f_add},
  {E_SUB,	10,	2,	"-",	OS_MAX,		Value_sub,	Value_w_sub,	Value_f_sub},

  {E_RSHIFT,	9,	2,	">>",	OS_MAX,		Value_rshift,	Value_w_rshift,	0},
  {E_LSHIFT,	9,	2,	"<<",	OS_MAX,		Value_lshift,	Value_w_lshift,	0},
  {E_ARSHIFT,	9,	2,	">>>",	OS_MAX,		Value_arshift,	Value_w_arshift,	0},
  {E_ALSHIFT,	9,	2,	"<<<",	OS_MAX,		Value_alshift,	Value_w_alshift,	0},

  {E_GT,	8,	2,	">",	OS_ONE,		Value_gt,	Value_w_gt,	Value_f_gt},
  {E_LT,	8,	2,	"<",	OS_ONE,		Value_lt,	Value_w_lt,	Value_f_lt},
  {E_GE,	8,	2,	">=",	OS_ONE,		Value_ge,	Value_w_ge,	Value_f_ge},
  {E_LE,	8,	2,	"<=",	OS_ONE,		Value_le,	Value_w_le,	Value_f_le},

  {E_EQ,	7,	2,	"==",	OS_ONE,		Value_eq,	0,		Value_f_eq},
  {E_NE,	7,	2,	"!=",	OS_ONE,		Value_neq,	0,		Value_f_neq},
  {E_EQZ,	7,	2,	"===",	OS_ONE,		Value_caseEq,	0,		Value_f_caseEq},
  {E_NEZ,	7,	2,	"!==",	OS_ONE,		Value_caseNeq,	0,		Value_f_caseNeq},

  {E_BAND,	6,	2,	"&",	OS_MAX,		Value_and,	0,		0},
  {E_BNAND,	6,	2,	"~&",	OS_MAX,		Value_nand,	0,		0},

  {E_BXOR,	5,	2,	"^",	OS_MAX,		Value_xor,	0,		0},
  {E_BNXOR,	5,	2,	"~^",	OS_MAX,		Value_nxor,	0,		0},

  {E_BOR,	4,	2,	"|",	OS_MAX,		Value_or,	0,		0},
  {E_BNOR,	4,	2,	"~|",	OS_MAX,		Value_nor,	0,		0},

  {E_AND,	3,	2,	"&&",	OS_ONE,		Value_land,	Value_w_land,	0},

  {E_OR,	2,	2,	"||",	OS_ONE,		Value_lor,	Value_w_lor,	0},

  {E_QUEST,	1,	2,	"?\0:",	OS_MAX,		Value_ifelse,	0,		0},

  {E_COND,	0,	2,	"&&&",	OS_NONE,	0,		0,		0},
  {E_AT,	0,	1,	"@",	OS_NONE,	0,		0,		0},		
  {E_DELAY,	0,	1,	"#",	OS_NONE,	0,		0,		0},		

  {E_EVENTOR,	0,	1,	"or",	OS_NONE,	0,		0,		0},		

  {E_CONCAT,	0,	1,	"{}",	OS_SUM,		Value_concat,	0,		0},
  {E_REPCAT,	0,	1,	"{n{}}",OS_SPECIAL,	Value_repcat,	0,		0},
};
int opTable_size = sizeof(opTable)/sizeof(opTable[0]);

/*****************************************************************************
 *
 * Functions below here implement Verilog operators.  They must all have 
 * declarations of the form:
 *
 *  void Value_name(Value *r, Value *a, Value *b, Value *c);
 *
 * Some parameters may be unusued by some functions.  All of these functions
 * assume that r has already been sized correctly and does not need resizing.
 *
 * Many operators have a normal form and a _w_ form.  The _w_ form is optimized
 * for values that are no more than the native word size on the machine.  The
 * normal form functions do not make that assumption.
 *
 *****************************************************************************/


/*****************************************************************************
 *
 * Do a bitwise-NOT operation: r = ~b
 *
 *****************************************************************************/
int Value_w_not(Value *R,Value *A,Value *B,Value *C)
{
  R->flags = B->flags;

  R->one[0] = B->zero[0] | B->flt[0];
  R->zero[0] = B->one[0] | B->flt[0];
  R->flt[0] = B->flt[0];

  return 0;
}


/*****************************************************************************
 *
 * Do a bitwise-NOT operation: r = ~b
 *
 *****************************************************************************/
int Value_not(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  int i;

  R->flags = B->flags;

  for (i = 0;i < wc;i++) {
    register unsigned one = B->zero[i];
    register unsigned zero = B->one[i];
    R->one[i] = one|B->flt[i];
    R->zero[i] = zero|B->flt[i];
    R->flt[i] = B->flt[i];
  }
  return 0;
}

/*****************************************************************************
 *
 * Buffer a value.
 *
 * Parameters:
 *      R		Return value
 *      A		Input value.
 *
 * Buffers the input and returns the result in R.  A buffered value will filter
 * and floating values and convert them to unknown.
 *
 *****************************************************************************/
int Value_buf(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    R->one[i] = B->one[i]|B->flt[i];
    R->zero[i] = B->zero[i]|B->flt[i];
    R->flt[i] = B->flt[i];
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a bitwise-OR function: r = a | b
 *
 * Parameters:
 *     R		Return value
 *     A,B		Operands
 *     C		Unused
 *
 *OR                 one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H
 *  +-----------       +-----------       +-----------       +-----------
 * 0|0 1 x x x x      0|0 1 1 1 1 1      0|1 0 1 1 1 1      0|0 0 1 1 1 1
 * 1|1 1 1 1 1 1      1|1 1 1 1 1 1      1|0 0 0 0 0 0      1|0 0 0 0 0 0
 * x|x 1 x x x x      x|1 1 1 1 1 1      x|1 0 1 1 1 1      x|1 0 1 1 1 1
 * z|x 1 x x x x      z|1 1 1 1 1 1      z|1 0 1 1 1 1      z|1 0 1 1 1 1
 * L|x 1 x x x x      L|1 1 1 1 1 1      L|1 0 1 1 1 1      L|1 0 1 1 1 1
 * H|x 1 x x x x      H|1 1 1 1 1 1      H|1 0 1 1 1 1      H|1 0 1 1 1 1
 *
 *****************************************************************************/
int Value_or(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  R->flags = A->flags | B->flags;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = (A->one[i]&~A->flt[i])|(B->one[i]&~B->flt[i]);
    register unsigned inHasFloat = A->flt[i]|B->flt[i];

    R->one[i] = RisOne|inHasFloat;
    R->zero[i] = ~RisOne;
    R->flt[i] = (inHasFloat&~RisOne);
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a bitwise-NOR function: r = a ~| b
 *
 * Parameters:
 *     R		Return value
 *     A,B		Operands
 *     C		Unused
 *
 *****************************************************************************/
int Value_nor(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = (A->one[i]&~A->flt[i])|(B->one[i]&~B->flt[i]);
    register unsigned inHasFloat = A->flt[i]|B->flt[i];

    R->one[i] = ~RisOne;
    R->zero[i] = RisOne|inHasFloat;
    R->flt[i] = (inHasFloat&~RisOne);
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a bitwise-AND function: r = a & b
 *
 * Parameters:
 *     R		Return value
 *     A,B		Operands
 *     C		Unused
 *
 *AND                 one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H
 *  +-----------       +-----------       +-----------       +-----------
 * 0|0 0 0 0 0 0      0|0 0 0 0 0 0      0|1 1 1 1 1 1      0|0 0 0 0 0 0
 * 1|0 1 x x x x      1|0 1 1 1 1 1      1|1 0 1 1 1 1      1|0 0 1 1 1 1
 * x|0 x x x x x      x|0 1 1 1 1 1      x|1 1 1 1 1 1      x|0 1 1 1 1 1
 * z|0 x x x x x      z|0 1 1 1 1 1      z|1 1 1 1 1 1      z|0 1 1 1 1 1
 * L|0 x x x x x      L|0 1 1 1 1 1      L|1 1 1 1 1 1      L|0 1 1 1 1 1
 * H|0 x x x x x      H|0 1 1 1 1 1      H|1 1 1 1 1 1      H|0 1 1 1 1 1
 *
 *****************************************************************************/
int Value_and(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  R->flags = A->flags | B->flags;

  for (i = 0;i < wc;i++) {
    register unsigned RisZero = (A->zero[i]&~A->flt[i])|(B->zero[i]&~B->flt[i]);
    register unsigned inHasFloat = A->flt[i]|B->flt[i];

    R->one[i] = ~RisZero;
    R->zero[i] = RisZero|inHasFloat;
    R->flt[i] = (inHasFloat&~RisZero);
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a bitwise-NAND function: r = a ~& b
 *
 * Parameters:
 *     R		Return value
 *     A,B		Operands
 *     C		Unused
 *
 *****************************************************************************/
int Value_nand(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  R->flags = A->flags | B->flags;

  for (i = 0;i < wc;i++) {
    register unsigned RisZero = (A->zero[i]&~A->flt[i])|(B->zero[i]&~B->flt[i]);
    register unsigned inHasFloat = A->flt[i]|B->flt[i];

    R->one[i] = RisZero|inHasFloat;
    R->zero[i] = ~RisZero;
    R->flt[i] = (inHasFloat&~RisZero);
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a bitwise-XOR function: r = a ^ b
 *
 * Parameters:
 *     R		Return value
 *     A,B		Operands
 *     C		Unused
 *
 *XOR                 one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H
 *  +-----------       +-----------       +-----------       +-----------
 * 0|0 1 x x x x      0|0 1 1 1 1 1      0|1 0 1 1 1 1      0|0 0 1 1 1 1
 * 1|1 0 x x x x      1|1 0 1 1 1 1      1|0 1 1 1 1 1      1|0 0 1 1 1 1
 * x|x x x x x x      x|1 1 1 1 1 1      x|1 1 1 1 1 1      x|1 1 1 1 1 1
 * z|x x x x x x      z|1 1 1 1 1 1      z|1 1 1 1 1 1      z|1 1 1 1 1 1
 * L|x x x x x x      L|1 1 1 1 1 1      L|1 1 1 1 1 1      L|1 1 1 1 1 1
 * H|x x x x x x      H|1 1 1 1 1 1      H|1 1 1 1 1 1      H|1 1 1 1 1 1
 *
 *****************************************************************************/
int Value_xor(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  R->flags = A->flags | B->flags;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]^B->one[i];
    register unsigned inHasFloat = A->flt[i]|B->flt[i];

    R->one[i]  = RisOne|inHasFloat;
    R->zero[i] = ~(RisOne)|inHasFloat;
    R->flt[i]  = inHasFloat;
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a bitwise-NXOR function: r = a ~^ b
 *
 * Parameters:
 *     R		Return value
 *     A,B		Operands
 *     C		Unused
 *
 *****************************************************************************/
int Value_nxor(Value *R,Value *A,Value *B,Value *C)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  R->flags = A->flags | B->flags;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]^B->one[i];
    register unsigned inHasFloat = A->flt[i]|B->flt[i];

    R->one[i]  = ~(RisOne)|inHasFloat;
    R->zero[i] = RisOne|inHasFloat;
    R->flt[i]  = inHasFloat;
  }
  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction AND function: r = &b
 *
 *****************************************************************************/
int Value_uand(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(b->nbits);
  register int i;
  int has_unknown = 0;

  r->flags = b->flags;

  for (i = 0;i < wc;i++) {
    unsigned mask = (i==(wc-1)) ? LMASK(b->nbits&SSBITMASK) : SSWORDMASK;

    /*
     * If there are any zero bits, return zero.
     */
    if ((b->zero[i] & ~b->flt[i] & mask)) {
      Value_zero(r);
      return 0;
    }

    if ((b->flt[i] & mask))
      has_unknown = 1;
  }

  /*
   * There where no zero bits, if there where any unknown bits return
   * unknown otherwise return 1.
   */
  if (has_unknown)
    Value_unknown(r);
  else
    Value_lone(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction AND function: r = &b
 *
 *****************************************************************************/
int Value_w_uand(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(b->nbits&SSBITMASK);

  r->flags = b->flags;

  /*
   * If there are any zero bits, return zero.
   */
  if ((b->zero[0] & ~b->flt[0] & mask)) {
    Value_zero(r);
    return 0;
  }

  /*
   * There where no zero bits, if there where any unknown bits return
   * unknown otherwise return 1.
   */
  if ((b->flt[0] & mask))
    Value_unknown(r);
  else
    Value_lone(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction OR function: r = |b
 *
 *****************************************************************************/
int Value_uor(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(b->nbits);
  register int i;
  int has_unknown = 0;

  r->flags = b->flags;

  for (i = 0;i < wc;i++) {
    unsigned mask = (i==(wc-1)) ? LMASK(b->nbits&SSBITMASK) : SSWORDMASK;

    /*
     * If there are any one bits, return 1.
     */
    if ((b->one[i] & ~b->flt[i] & mask)) {
      Value_lone(r);
      return 0;
    }

    if ((b->flt[i] & mask))
      has_unknown = 1;
  }

  /*
   * There where no one bits, if there where any unknown bits return
   * unknown otherwise return 0.
   */
  if (has_unknown)
    Value_unknown(r);
  else
    Value_zero(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction OR function: r = |b
 *
 *****************************************************************************/
int Value_w_uor(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(b->nbits&SSBITMASK);

  r->flags = b->flags;

  /*
   * If there are any one bits, return one.
   */
  if ((b->one[0] & ~b->flt[0] & mask)) {
    Value_lone(r);
    return 0;
  }

  /*
   * There where no one bits, if there where any unknown bits return
   * unknown otherwise return 0.
   */
  if ((b->flt[0] & mask))
    Value_unknown(r);
  else
    Value_zero(r);

  return 0;
}


/*****************************************************************************
 *
 * Perform a reduction XOR function: r = ^b
 *
 *****************************************************************************/
int Value_uxor(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(b->nbits);
  register int i,j;
  int n = 0;

  r->flags = b->flags;

  for (i = 0;i < wc;i++) {
    unsigned mask = (i==(wc-1)) ? LMASK(b->nbits&SSBITMASK) : SSWORDMASK;
    int wsize = (i==(wc-1)) ? (b->nbits&SSBITMASK) : SSWORDSIZE;

    /*
     * If there are any unknown bits, return unknown
     */
    if ((b->flt[i] & mask)) {
      Value_unknown(r);
      return 0;
    }

    /*
     * count one bits
     */
    for (j = 0;j < wsize;j++)
      if ((b->one[i] & (1 << j))) n++;
  }

  /*
   * If there were an odd number of one bits, return 1
   * otherwise return 0.
   */
  if ((n & 1))
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction XOR function: r = ^b
 *
 *****************************************************************************/
int Value_w_uxor(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(b->nbits&SSBITMASK);
  unsigned x,count;

  r->flags = b->flags;

  /*
   * If there are any unknown bits, return unknown
   */
  if ((b->flt[0] & mask)) {
    Value_unknown(r);
    return 0;
  }

  /*
   * Count number of bits that are set
   */
  count = 0;
  for (x = b->one[0] & mask;x;x = (x ^ (-x)) & x) count++;

  /*
   * Return 1 if there where an odd number of bits set, zero otherwise.
   */
  if ((count & 1))
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}


/*****************************************************************************
 *
 * Perform a reduction NAND function: r = ~&b
 *
 *****************************************************************************/
int Value_unand(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(b->nbits);
  register int i;
  int has_unknown = 0;

  r->flags = b->flags;

  for (i = 0;i < wc;i++) {
    unsigned mask = (i==(wc-1)) ? LMASK(b->nbits&SSBITMASK) : SSWORDMASK;

    /*
     * If there are any zero bits, return one.
     */
    if ((b->zero[i] & ~b->flt[i] & mask)) {
      Value_lone(r);
      return 0;
    }

    if ((b->flt[i] & mask))
      has_unknown = 1;
  }

  /*
   * There where no zero bits, if there where any unknown bits return
   * unknown otherwise return 1.
   */
  if (has_unknown)
    Value_unknown(r);
  else
    Value_zero(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction NAND function: r = ~&b
 *
 *****************************************************************************/
int Value_w_unand(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(b->nbits&SSBITMASK);

  r->flags = b->flags;

  /*
   * If there are any zero bits, return one.
   */
  if ((b->zero[0] & ~b->flt[0] & mask)) {
    Value_lone(r);
    return 0;
  }

  /*
   * There where no zero bits, if there where any unknown bits return
   * unknown otherwise return 1.
   */
  if ((b->flt[0] & mask))
    Value_unknown(r);
  else
    Value_zero(r);

  return 0;
}


/*****************************************************************************
 *
 * Perform a reduction NOR function: r = ~|b
 *
 *****************************************************************************/
int Value_unor(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(b->nbits);
  register int i;
  int has_unknown = 0;

  r->flags = b->flags;

  for (i = 0;i < wc;i++) {
    unsigned mask = (i==(wc-1)) ? LMASK(b->nbits&SSBITMASK) : SSWORDMASK;

    /*
     * If there are any one bits, return 0.
     */
    if ((b->one[i] & ~b->flt[i] & mask)) {
      Value_zero(r);
      return 0;
    }

    if ((b->flt[i] & mask))
      has_unknown = 1;
  }

  /*
   * There where no one bits, if there where any unknown bits return
   * unknown otherwise return 1.
   */
  if (has_unknown)
    Value_unknown(r);
  else
    Value_lone(r);

  return 0;
}

int Value_w_unor(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(b->nbits&SSBITMASK);

  r->flags = b->flags;

  /*
   * If there are any one bits, return one.
   */
  if ((b->one[0] & ~b->flt[0] & mask)) {
    Value_zero(r);
    return 0;
  }

  /*
   * There where no one bits, if there where any unknown bits return
   * unknown otherwise return 0.
   */
  if ((b->flt[0] & mask))
    Value_unknown(r);
  else
    Value_lone(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a reduction NOR function: r = ~^b
 *
 *****************************************************************************/
int Value_unxor(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(b->nbits);
  register int i,j;
  int n = 0;

  r->flags = b->flags;

  for (i = 0;i < wc;i++) {
    unsigned mask = (i==(wc-1)) ? LMASK(b->nbits&SSBITMASK) : SSWORDMASK;
    int wsize = (i==(wc-1)) ? (b->nbits&SSBITMASK) : SSWORDSIZE;

    /*
     * If there are any unknown bits, return unknown
     */
    if ((b->flt[i] & mask)) {
      Value_unknown(r);
      return 0;
    }

    /*
     * count one bits
     */
    for (j = 0;j < wsize;j++)
      if ((b->one[i] & (1 << j))) n++;
  }

  /*
   * If there were an odd number of one bits, return 0
   * otherwise return 1
   */
  if ((n & 1))
    Value_zero(r);
  else
    Value_lone(r);

  return 0;
}

int Value_w_unxor(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(b->nbits&SSBITMASK);
  unsigned x,count;

  r->flags = b->flags;

  /*
   * If there are any unknown bits, return unknown
   */
  if ((b->flt[0] & mask)) {
    Value_unknown(r);
    return 0;
  }

  /*
   * Count number of bits that are set.
   */
  count = 0;
  for (x = b->one[0] & mask;x;x = (x ^ (-x)) & x) count++;

  /*
   * Return 0 if there where an odd number of bits set, zero otherwise.
   */
  if ((count & 1))
    Value_zero(r);
  else
    Value_lone(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a logical-NOT function: r = !b
 *
 *****************************************************************************/
int Value_lnot(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = b->flags;

  if (!Value_isValue(b))
    Value_unknown(r);
  else if (Value_isZero(b))
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a negation function: r = -b
 *
 *****************************************************************************/
int Value_neg(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(r->nbits);
  int carry = 1;
  int i;

  r->flags = b->flags;

  if (!Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  for (i = 0;i < wc;i++) {
    r->one[i] = ~b->one[i] + carry;
    carry = (b->one[i] == ~0);
  }

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a negation function: r = -b
 *
 *****************************************************************************/
int Value_w_neg(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = b->flags;

  if (!Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  r->one[0] = -b->one[0];

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a multiply function: r = a*b
 *
 *****************************************************************************/
int Value_mul(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = SSWORDMASK;
  int wc = SSNUMWORDS(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  a->one[wc-1] &= mask;
  b->one[wc-1] &= mask;
  multint_mul(r->one,a->one,b->one,wc);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a multiply function: r = a*b
 *
 *****************************************************************************/
int Value_w_mul(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  r->one[0] = (a->one[0]&mask)*(b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a divide function: r = a*b
 *
 *****************************************************************************/
int Value_div(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = SSWORDMASK;
  int wc = SSNUMWORDS(r->nbits);
  int ecode;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  a->one[wc-1] &= mask;
  b->one[wc-1] &= mask;

  ecode = multint_div(r->one,a->one,b->one,wc);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a divide function: r = a*b
 *
 *****************************************************************************/
int Value_w_div(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }


  if ((b->one[0]&mask) != 0)
    r->one[0] = (a->one[0]&mask)/(b->one[0]&mask);
  else {
    r->one[0] = 0;
  }

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a modulo function: r = a % b
 *
 *****************************************************************************/
int Value_mod(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = SSWORDMASK;
  int wc = SSNUMWORDS(r->nbits);
  int ecode;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  a->one[wc-1] &= mask;
  b->one[wc-1] &= mask;

  ecode = multint_mod(r->one,a->one,b->one,wc);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a modulo function: r = a % b
 *
 *****************************************************************************/
int Value_w_mod(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  if ((b->one[0]&mask) != 0)
    r->one[0] = (a->one[0]&mask) % (b->one[0]&mask);
  else {
    r->one[0] = 0;
  }

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform an add function: r = a + b
 *
 *****************************************************************************/
int Value_w_add(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  r->one[0] = (a->one[0]&mask) + (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform an add function: r = a + b
 *
 *****************************************************************************/
int Value_add(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(r->nbits);
  register int i;
  int carry = 0;
  unsigned mask = (r->nbits&SSBITMASK) ? LMASK(r->nbits&SSBITMASK) : SSWORDMASK;
  
  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  wc--;
  for (i = 0;i < wc;i++) {
    r->one[i] = a->one[i] + b->one[i] + carry;
    
    /*
     * Generate carry for next word.
     */
    carry = ((a->one[i] & 0x1) + (b->one[i] & 0x1) + carry) >> 1;
    carry = ((a->one[i]>>1) + (b->one[i]>>1) + carry) >> (SSWORDSIZE-1);
  }
  r->one[wc] = (a->one[wc]&mask) + (b->one[wc]&mask) + carry;
  wc++;

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform an subtract function: r = a - b
 *
 * NOTE: okay if r == a, as when called by SDRepeat_generate
 *
 *****************************************************************************/
int Value_w_sub(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  r->one[0] = (a->one[0]&mask) - (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform an subtract function: r = a - b
 *
 * NOTE: okay if r == a, as when called by SDRepeat_generate
 *
 *****************************************************************************/
int Value_sub(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(r->nbits);
  register int i;
  int carry = 1;
  unsigned mask = (r->nbits&SSBITMASK) ? LMASK(r->nbits&SSBITMASK) : SSWORDMASK;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  wc--;
  for (i = 0;i < wc;i++) {
    int ncarry;

    /*
     * Generate carry for next word.
     */
    ncarry = ((a->one[i] & 0x1) + (~b->one[i] & 0x1) + carry) >> 1;
    ncarry = ((a->one[i]>>1) + (~b->one[i]>>1) + carry) >> (SSWORDSIZE-1);

    r->one[i] = a->one[i] + ~b->one[i] + carry;
    
    carry = ncarry;
  }
  r->one[wc] = (a->one[wc]&mask) + (~b->one[wc]&mask) + carry;
  wc++;

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a right shift function: r = a >> b
 *
 *****************************************************************************/
int Value_rshift(Value *r,Value *a,Value *b,Value *c)
{
  unsigned smask = (b->nbits < SSWORDSIZE) ? LMASK(b->nbits) : ~0;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(b))
    Value_unknown(r);
  else
    Value_shift(r, a, -(b->one[0]&smask), 0, 1, 0);

  return 0;
}

/*****************************************************************************
 *
 * Perform an arithmetic right shift function: r = a >>> b
 *
 *****************************************************************************/
int Value_arshift(Value *r,Value *a,Value *b,Value *c)
{
  unsigned smask = (b->nbits < SSWORDSIZE) ? LMASK(b->nbits) : ~0;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(b)) {
    Value_unknown(r);
  } else {
    int wc = SSNUMWORDS(a->nbits);
    int hb = 1 << SSHIGHBIT(a->nbits);
    int in1 = (a->one[wc-1] & hb) != 0;
    int in0 = (a->zero[wc-1] & hb) != 0;
    int inZ = (a->flt[wc-1] & hb) != 0;

    Value_shift(r, a, -(b->one[0]&smask), in1, in0, inZ);
  }

  return 0;
}

/*****************************************************************************
 *
 * Perform a left shift function: r = a << b
 *
 *****************************************************************************/
int Value_lshift(Value *r,Value *a,Value *b,Value *c)
{
  unsigned smask = (b->nbits < SSWORDSIZE) ? LMASK(b->nbits) : ~0;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(b))
    Value_unknown(r);
  else
    Value_shift(r, a, b->one[0]&smask, 0, 1, 0);

  return 0;
}

/*****************************************************************************
 *
 * Perform an arithmetic left shift function: r = a <<< b
 *
 *****************************************************************************/
int Value_alshift(Value *r,Value *a,Value *b,Value *c)
{
  return Value_lshift(r, a, b, c);
}

/*****************************************************************************
 *
 * Perform a right shift function: r = a >> b
 *
 *****************************************************************************/
int Value_w_rshift(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = a->flags | b->flags;

  if (!Value_isValue(b))
    Value_unknown(r);
  else
    Value_w_shift(r, a, -(b->one[0]&LMASK(b->nbits)), 0, 1, 0);

  return 0;
}

/*****************************************************************************
 *
 * Perform an arithmetic right shift function: r = a >>> b
 *
 *****************************************************************************/
int Value_w_arshift(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = a->flags | b->flags;

  if (!Value_isValue(b)) {
    Value_unknown(r);
  } else {
    int hb = 1 << SSHIGHBIT(a->nbits);
    int in1 = (a->one[0] & hb) != 0;
    int in0 = (a->zero[0] & hb) != 0;
    int inZ = (a->flt[0] & hb) != 0;

    Value_w_shift(r, a, -(b->one[0]&LMASK(b->nbits)), in1, in0, inZ);
  }

  return 0;
}

/*****************************************************************************
 *
 * Perform a left shift function: r = a << b
 *
 *****************************************************************************/
int Value_w_lshift(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = a->flags | b->flags;

  if (!Value_isValue(b))
    Value_unknown(r);
  else
    Value_w_shift(r, a, b->one[0]&LMASK(b->nbits), 0, 1, 0);

  return 0;
}

/*****************************************************************************
 *
 * Perform an arithmetic left shift function: r = a <<< b
 *
 *****************************************************************************/
int Value_w_alshift(Value *r,Value *a,Value *b,Value *c)
{
  return Value_w_lshift(r, a, b, c);
}

/*****************************************************************************
 *
 * Perform a greater-than function: r = a > b
 *
 *****************************************************************************/
int Value_w_gt(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }
  
  r->one[0] = (a->one[0]&mask) > (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a greater-than function: r = a > b
 *
 *****************************************************************************/
int Value_gt(Value *r,Value *a,Value *b,Value *c)
{
  int i;
  int wc = SSNUMWORDS(r->nbits);
  unsigned mask = (r->nbits & SSBITMASK) ? LMASK(r->nbits & SSBITMASK) : SSWORDMASK;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  /* default if a==b */
  Value_zero(r);

  for (i=wc-1; i>=0; i--) {
    if ((a->one[i]&mask) > (b->one[i]&mask)) {
      Value_lone(r);
      break;
    } else if ((a->one[i]&mask) != (b->one[i]&mask)) {
      Value_zero(r);
      break;
    }
    mask = SSWORDMASK; /* after first (wc-1th) word, mask is cleared */
  }

  return 0;
}

/*****************************************************************************
 *
 * Perform a less-than function: r = a < b
 *
 *****************************************************************************/
int Value_w_lt(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }
  
  r->one[0] = (a->one[0]&mask) < (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a less-than function: r = a < b
 *
 *****************************************************************************/
int Value_lt(Value *r,Value *a,Value *b,Value *c)
{
  int i;
  int wc = SSNUMWORDS(r->nbits);
  unsigned mask = (r->nbits & SSBITMASK) ? LMASK(r->nbits & SSBITMASK) : SSWORDMASK;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  /* default if a==b */
  Value_zero(r);

  for (i=wc-1; i>=0; i--) {
    if ((a->one[i]&mask) < (b->one[i]&mask)) {
      Value_lone(r);
      break;
    } else if ((a->one[i]&mask) != (b->one[i]&mask)) {
      Value_zero(r);
      break;
    }
    mask = SSWORDMASK; /* after first (wc-1th) word, mask is cleared */
  }

  return 0;
}

/*****************************************************************************
 *
 * Perform a greater-than-or-equal function: r = a >= b
 *
 *****************************************************************************/
int Value_w_ge(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }
  
  r->one[0] = (a->one[0]&mask) >= (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a greater-than-or-equal function: r = a >= b
 *
 *****************************************************************************/
int Value_ge(Value *r,Value *a,Value *b,Value *c)
{
  int i;
  int wc = SSNUMWORDS(r->nbits);
  unsigned mask = (r->nbits & SSBITMASK) ? LMASK(r->nbits & SSBITMASK) : SSWORDMASK;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  /* default if a==b */
  Value_lone(r);

  for (i=wc-1; i>=0; i--) {
    if ((a->one[i]&mask) > (b->one[i]&mask)) {
      Value_lone(r);
      break;
    } else if ((a->one[i]&mask) != (b->one[i]&mask)) {
      Value_zero(r);
      break;
    }
    mask = SSWORDMASK; /* after first (wc-1th) word, mask is cleared */
  }

  return 0;
}

/*****************************************************************************
 *
 * Perform a less-than-or-equal function: r = a <= b
 *
 *****************************************************************************/
int Value_w_le(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }
  
  r->one[0] = (a->one[0]&mask) <= (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a less-than-or-equal function: r = a <= b
 *
 *****************************************************************************/
int Value_le(Value *r,Value *a,Value *b,Value *c)
{
  int i;
  int wc = SSNUMWORDS(r->nbits);
  unsigned mask = (r->nbits & SSBITMASK) ? LMASK(r->nbits & SSBITMASK) : SSWORDMASK;

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  /* default if a==b */
  Value_lone(r);

  for (i=wc-1; i>=0; i--) {
    if ((a->one[i]&mask) < (b->one[i]&mask)) {
      Value_lone(r);
      break;
    } else if ((a->one[i]&mask) != (b->one[i]&mask)) {
      Value_zero(r);
      break;
    }
    mask = SSWORDMASK; /* after first (wc-1th) word, mask is cleared */
  }

  return 0;
}

/*****************************************************************************
 *
 * Perform a case equal function: r = a === b
 *
 *****************************************************************************/
int Value_caseEq(Value *r,Value *a,Value *b,Value *c)
{
  if (Value_isEqual(a,b))
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a case not equal function: r = a !== b
 *
 *****************************************************************************/
int Value_caseNeq(Value *r,Value *a,Value *b,Value *c)
{
  if (Value_isEqual(a,b))
    Value_zero(r);
  else
    Value_lone(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a case equalx function
 *
 *****************************************************************************/
int Value_casexEq(Value *r,Value *a,Value *b,Value *c)
{
  return 0;
}

/*****************************************************************************
 *
 * Perform a case equalz function
 *
 *****************************************************************************/
int Value_casezEq(Value *r,Value *a,Value *b,Value *c)
{
  return 0;
}

/*****************************************************************************
 *
 * Perform an equal function: r = a == b
 *
 *****************************************************************************/
int Value_eq(Value *r,Value *a,Value *b,Value *c)
{
  int wc = SSNUMWORDS(a->nbits);
  int i;

  r->flags = a->flags | b->flags;

  if (a->nbits != b->nbits) {
    Value_lone(r);
    return -1;
  }

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  for (i = 0;i < wc;i++) {
    if (i == wc-1 && (r->nbits & SSBITMASK)) {
      unsigned mask = LMASK((r->nbits & SSBITMASK));

      if ((a->one[i]&mask) != (b->one[i]&mask)) {
	Value_zero(r);
	return 0;
      }
    } else {
      if (a->one[i] != b->one[i]) {
	Value_zero(r);
	return 0;
      }
    }
  }

  Value_lone(r);
  return 0;
}

/*****************************************************************************
 *
 * Perform a not equal function: r = a != b
 *
 *****************************************************************************/
int Value_neq(Value *r,Value *a,Value *b,Value *c)
{
  Value_eq(r,a,b,c);
  Value_lnot(r,0,r,0);
  return 0;
}

/*****************************************************************************
 *
 * Perform a logical and function: r = a && b
 *
 *****************************************************************************/
int Value_w_land(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }
  
  r->one[0] = (a->one[0]&mask) && (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a logical and function: r = a && b
 *
 *****************************************************************************/
int Value_land(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  Value_zero(r);
  r->one[0] = (!Value_isZero(a) && !Value_isZero(b));
  r->zero[0] = ~r->one[0];

  return 0;
}

/*****************************************************************************
 *
 * Perform a logical or function: r = a || b
 *
 *****************************************************************************/
int Value_w_lor(Value *r,Value *a,Value *b,Value *c)
{
  unsigned mask = LMASK(r->nbits);

  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  r->one[0] = (a->one[0]&mask) || (b->one[0]&mask);

  Value_normalize(r);

  return 0;
}

/*****************************************************************************
 *
 * Perform a logical or function: r = a || b
 *
 *****************************************************************************/
int Value_lor(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = a->flags | b->flags;

  if (!Value_isValue(a) || !Value_isValue(b)) {
    Value_unknown(r);
    return 0;
  }

  Value_zero(r);
  r->one[0] = (!Value_isZero(a) || !Value_isZero(b));
  r->zero[0] = ~r->one[0];

  return 0;
}

/*****************************************************************************
 *
 * Do a if/else operation: r = a ? b : c
 *
 *****************************************************************************/
int Value_ifelse(Value *r,Value *a,Value *b,Value *c)
{
  r->flags = a->flags | b->flags;

  if (Value_isValue(a)) {
    if (Value_isZero(a))
      Value_copy(r,c);
    else
      Value_copy(r,b);
  } else {
    int wc = SSNUMWORDS(r->nbits);
    int i;
    /*
     * The select line is unknown.  Look at both the then and else cases and
     * bit by bit and copy the bits when they match or generate an unknown
     * when they do not match.
     */

    for (i = 0;i < wc;i++) {
      unsigned unknown = b->flt[i] | c->flt[i] | (b->one[i] ^ c->one[i]);

      r->one[i]  = (b->one[i] &c->one[i]) | unknown;
      r->zero[i] = (b->zero[i]&c->zero[i]) | unknown;
      r->flt[i]  = unknown;

      if (unknown)
	r->flags = SF_HEX;
    }
  }

  return 0;
}

/*****************************************************************************
 *
 * Do a concat operation: r = {a,b}
 *
 *****************************************************************************/
int Value_concat(Value *r,Value *a,Value *b,Value *c)
{
  int rhs_nbits = a->nbits + b->nbits;

  r->flags = a->flags | b->flags;

  if (rhs_nbits <= SSWORDSIZE) {
    unsigned mask = LMASK(b->nbits);

    r->one[0] = (b->one[0] & mask) | (a->one[0] << b->nbits);
    r->zero[0] = (b->zero[0] & mask) | (a->zero[0] << b->nbits);
    r->flt[0] = (b->flt[0] & mask) | (a->flt[0] << b->nbits);

    /*
     * Zero any bits left over in r 
     */
    if (r->nbits > rhs_nbits) {
      int rwc = SSNUMWORDS(r->nbits);
      int i;

      r->zero[0] |= ~LMASK(rhs_nbits);
      for (i = 1;i < rwc;i++) {
	r->one[i] = 0;
	r->zero[i] = ~0;
	r->flt[i] = 0;
      }
    }
  } else {
    Value_zero(r);
    Value_copyRange(r,0,b,b->nbits-1,0);
    Value_copyRange(r,b->nbits,a,a->nbits-1,0);

#if 0
    //
    // This code could (not sure) be faster, but it is broken in some cases
    // so I've disabled it for now....
    //
    int rwc = SSNUMWORDS(r->nbits);
    int bwc = SSNUMWORDS(b->nbits);
    int awc = SSNUMWORDS(a->nbits);
    int shift = b->nbits & SSBITMASK;
    int i,j;

    /*
     * Copy bits for b
     */
    for (j = i = 0;i < bwc;i++, j++) {
      r->one[j] = b->one[i];
      r->zero[j] = b->zero[i];
      r->flt[j] = b->flt[i];
    }

    if (shift == 0) {
      printf("case A\n");

      /*
       * Perfectly aligned concat.
       */
      for (i = 0;i < awc;i++, j++) {
	if (j >= rwc) break;
	r->one[j] = a->one[i];
	r->zero[j] = a->zero[i];
	r->flt[j] = a->flt[i];
      }
    } else if (j < rwc) {
      printf("case B\n");

      /*
       * Word overlapped concat
       */
      r->one[j-1]  |= a->one[0] << shift;
      r->zero[j-1] |= a->zero[0] << shift;
      r->flt[j-1]  |= a->flt[0] << shift;
      for (i = 0;i < awc;i++, j++) {
	if (j >= rwc) break;
	r->one[j]  = a->one[i] >> (SSWORDSIZE-shift);
	r->zero[j] = a->zero[i] >> (SSWORDSIZE-shift);
	r->flt[j]  = a->flt[i] >> (SSWORDSIZE-shift);
	if (i+1 < awc) {
	  r->one[j]  |= a->one[i+1] << shift;
	  r->zero[j] |= a->zero[i+1] << shift;
	  r->flt[j]  |= a->flt[i+1] << shift;
	}
      }
    }
#endif
  }
  return 0;
}

/*****************************************************************************
 *
 * Do a repcat operation such as r = {8{b}}
 *
 *****************************************************************************/
int Value_repcat(Value *r,Value *a,Value *b,Value *c)
{
  int rwc = SSNUMWORDS(r->nbits);
  int n,i;
  int rhs_nbits;

  r->flags = b->flags;

  if (Value_toInt(a,&n)) return -1;	/* Must be a number */

  //  Value_resize(r,n*b->nbits);

  if (n == 1) {
    Value_copy(r,b);
    return 0;
  }

  rhs_nbits = n*b->nbits;
  if (rhs_nbits <= SSWORDSIZE) {
    unsigned mask = LMASK(b->nbits);

    r->one[0]  = b->one[0]&mask;
    r->zero[0] = b->zero[0]&mask;
    r->flt[0]  = b->flt[0]&mask;
    for (i = 1;i < n;i++) {
      r->one[0]  = (r->one[0] << b->nbits)  | (b->one[0]&mask);
      r->zero[0] = (r->zero[0] << b->nbits) | (b->zero[0]&mask);
      r->flt[0]  = (r->flt[0] << b->nbits)  | (b->flt[0]&mask);
    }
    //
    // If r is bigger than right-hand-side, extend with zero bits
    //
    if (r->nbits > rhs_nbits) {
      if (rhs_nbits < SSWORDSIZE) {
	unsigned mask = LMASK(rhs_nbits);
	r->zero[0] |= ~mask;
      }
      for (i = 1;i < rwc;i++) {
	r->one[i] = 0;
	r->zero[i] = ~0;
	r->flt[i] = 0;
      }
    }
  } else {
    Value_zero(r);
    for (i = 0;i < n;i++)
      Value_copyRange(r,i*b->nbits,b,b->nbits-1,0);
  }

  return 0;
}

/*****************************************************************************
 *
 * bufif1 - Buffer if control is set
 *
 * Parameters:
 *      R		Return value
 *      I		Input value
 *      E		Enable value
 *
 * Truth tables for operator:
 *
 *      /E\           one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H
 *  +-----------       +-----------       +-----------       +-----------
 * 0|z 0 L L L L      0|0 0 0 0 0 0      0|0 1 1 1 1 1      0|1 0 1 1 1 1
 * 1|z 1 H H H H      1|0 1 1 1 1 1      1|0 0 0 0 0 0      1|1 0 1 1 1 1
 *Ix|z x x x x x      x|0 1 1 1 1 1      x|0 1 1 1 1 1      x|1 1 1 1 1 1
 * z|z x x x x x      z|0 1 1 1 1 1      z|0 1 1 1 1 1      z|1 1 1 1 1 1
 * L|z x x x x x      L|0 1 1 1 1 1      L|0 1 1 1 1 1      L|1 1 1 1 1 1
 * H|z x x x x x      H|0 1 1 1 1 1      H|0 1 1 1 1 1      H|1 1 1 1 1 1
 *
 *****************************************************************************/
int Value_bufif1(Value *R,Value *I,Value *E,Value *dummy)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned IisZero = I->zero[i] & ~I->one[i] & ~I->flt[i];
    register unsigned EisZero = E->zero[i] & ~E->one[i] & ~E->flt[i];
    register unsigned IisOne  = ~I->zero[i] & I->one[i] & ~I->flt[i];
    register unsigned EisOne  = ~E->zero[i] & E->one[i] & ~E->flt[i];
    R->one[i] = ~(IisZero|EisZero);
    R->zero[i] = ~(EisZero|IisOne);
    R->flt[i] = ~((IisZero|IisOne)&EisOne);
  }

  return 0;
}

/*****************************************************************************
 *
 * bufif1 - Buffer if control is not set
 *
 * Parameters:
 *      R		Return value
 *      I		Input value
 *      E		Enable value
 *
 * Truth tables for operator:
 *
 *      /E\           one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H
 *  +-----------       +-----------       +-----------       +-----------
 * 0|0 z L L L L      0|0 0 0 0 0 0      0|1 0 1 1 1 1      0|0 1 1 1 1 1
 * 1|1 z H H H H      1|1 0 1 1 1 1      1|0 0 0 0 0 0      1|0 1 1 1 1 1
 *Ix|x z x x x x      x|1 0 1 1 1 1      x|1 0 1 1 1 1      x|1 1 1 1 1 1
 * z|x z x x x x      z|1 0 1 1 1 1      z|1 0 1 1 1 1      z|1 1 1 1 1 1
 * L|x z x x x x      L|1 0 1 1 1 1      L|1 0 1 1 1 1      L|1 1 1 1 1 1
 * H|x z x x x x      H|1 0 1 1 1 1      H|1 0 1 1 1 1      H|1 1 1 1 1 1
 *
 *****************************************************************************/
int Value_bufif0(Value *R,Value *I,Value *E,Value *dummy)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned IisZero = I->zero[i] & ~I->one[i] & ~I->flt[i];
    register unsigned EisZero = E->zero[i] & ~E->one[i] & ~E->flt[i];
    register unsigned IisOne  = ~I->zero[i] & I->one[i] & ~I->flt[i];
    register unsigned EisOne  = ~E->zero[i] & E->one[i] & ~E->flt[i];
    R->one[i] = ~(IisZero|EisOne);
    R->zero[i] = ~(EisOne|IisOne);
    R->flt[i] = ~((IisZero|IisOne)&EisZero);
  }

  return 0;
}

/*****************************************************************************
 *
 * nmos - do nmos transistor function
 *
 * Parameters:
 *      R		Return value
 *      I		Input value
 *      E		Control value
 *      dummy		Unused parameter (to allow use as "operator" function).
 *
 *      /G\           one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H  01z
 *  +-----------       +-----------       +-----------       +-----------  ---
 * 0|z 0 L L L L      0|0 0 0 0 0 0      0|0 1 1 1 1 1      0|1 0 1 1 1 1  100
 * 1|z 1 H H H H      1|0 1 1 1 1 1      1|0 0 0 0 0 0      1|1 0 1 1 1 1  010
 *Ix|z x x x x x      x|0 1 1 1 1 1      x|0 1 1 1 1 1      x|1 1 1 1 1 1  111
 * z|z z z z z z      z|0 0 0 0 0 0      z|0 0 0 0 0 0      z|1 1 1 1 1 1  001
 * L|z L L L L L      L|0 0 0 0 0 0      L|0 1 1 1 1 1      L|1 1 1 1 1 1  101
 * H|z H H H H H      H|0 1 1 1 1 1      H|0 0 0 0 0 0      H|1 1 1 1 1 1  011
 *
 *****************************************************************************/
int Value_nmos(Value *R,Value *I,Value *G,Value *dummy)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned GisZero = G->zero[i] & ~G->one[i] & ~G->flt[i];
    register unsigned GisOne = ~G->zero[i] & G->one[i] & ~G->flt[i];
    register unsigned IisLogic = (I->zero[i] ^ I->one[i]) & ~I->flt[i];

    R->one[i]  = I->one[i] & ~GisZero;
    R->zero[i] = I->zero[i] & ~GisZero;
    R->flt[i] = ~(IisLogic & GisOne);
  }

  return 0;
}

/*****************************************************************************
 *
 * pmos - do pmos transistor function
 *
 * Parameters:
 *      R		Return value
 *      I		Input value
 *      E		Control value
 *      dummy		Unused parameter (to allow use as "operator" function).
 *
 *      /G\           one                zero                flt          
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H  01z
 *  +-----------       +-----------       +-----------       +-----------  ---
 * 0|0 z L L L L      0|0 0 0 0 0 0      0|1 0 1 1 1 1      0|0 1 1 1 1 1  100
 * 1|1 z H H H H      1|1 0 1 1 1 1      1|0 0 0 0 0 0      1|0 1 1 1 1 1  010
 *Ix|z z x x x x      x|1 0 1 1 1 1      x|1 0 1 1 1 1      x|1 1 1 1 1 1  111
 * z|z z z z z z      z|0 0 0 0 0 0      z|0 0 0 0 0 0      z|1 1 1 1 1 1  001
 * L|L z L L L L      L|0 0 0 0 0 0      L|1 0 1 1 1 1      L|1 1 1 1 1 1  101
 * H|H z H H H H      H|1 0 1 1 1 1      H|0 0 0 0 0 0      H|1 1 1 1 1 1  011
 *
 *****************************************************************************/
int Value_pmos(Value *R,Value *I,Value *G,Value *dummy)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned GisZero = G->zero[i] & ~G->one[i] & ~G->flt[i];
    register unsigned GisOne = ~G->zero[i] & G->one[i] & ~G->flt[i];
    register unsigned IisLogic = (I->zero[i] ^ I->one[i]) & ~I->flt[i];

    R->one[i]  = I->one[i] & ~GisOne;
    R->zero[i] = I->zero[i] & ~GisOne;
    R->flt[i] = ~(IisLogic & GisZero);
  }

  return 0;
}

int Value_f_gt(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fa > fb)
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_lt(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fa < fb)
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_ge(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fa >= fb)
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_le(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fa <= fb)
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_eq(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fa == fb)
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_neq(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fa != fb)
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_add(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  Value_convertR(r,fa+fb);

  return 0;
}

int Value_f_sub(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  Value_convertR(r,fa-fb);

  return 0;
}

int Value_f_mul(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  Value_convertR(r,fa*fb);

  return 0;
}

int Value_f_div(Value *r,Value *a,Value *b,Value *c)
{
  real_t fa,fb;

  if (Value_toReal(a,&fa) != 0 || Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  if (fb == 0.0) {
    errorRun(ERR_DIVZERO);
    Value_convertR(r,0.0);
  } else
    Value_convertR(r,fa/fb);

  return 0;
}

int Value_f_neg(Value *r,Value *a,Value *b,Value *c)
{
  real_t fb;

  if (Value_toReal(b,&fb)) {
    Value_convertNaN(r);
    return 0;
  }

  Value_convertR(r,-fb);

  return 0;
}

int Value_f_caseEq(Value *r,Value *a,Value *b,Value *c)
{
  if (Value_isEqual(a,b))
    Value_lone(r);
  else
    Value_zero(r);

  return 0;
}

int Value_f_caseNeq(Value *r,Value *a,Value *b,Value *c)
{
  if (Value_isEqual(a,b))
    Value_zero(r);
  else
    Value_lone(r);

  return 0;
}

