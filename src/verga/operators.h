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
#ifndef __operators_h
#define __operators_h

/*****************************************************************************
 * Value operator methods 
 *****************************************************************************/
int Value_lnot(Value *r,Value *a,Value *b,Value *c);
int Value_not(Value *r,Value *a,Value *b,Value *c);
int Value_buf(Value *r,Value *a,Value *b,Value *c);
int Value_neg(Value *r,Value *a,Value *b,Value *c);
int Value_mul(Value *r,Value *a,Value *b,Value *c);
int Value_div(Value *r,Value *a,Value *b,Value *c);
int Value_mod(Value *r,Value *a,Value *b,Value *c);
int Value_add(Value *r,Value *a,Value *b,Value *c);
int Value_sub(Value *r,Value *a,Value *b,Value *c);
int Value_rshift(Value *r,Value *a,Value *b,Value *c);
int Value_lshift(Value *r,Value *a,Value *b,Value *c);
int Value_arshift(Value *r,Value *a,Value *b,Value *c);
int Value_alshift(Value *r,Value *a,Value *b,Value *c);
int Value_gt(Value *r,Value *a,Value *b,Value *c);
int Value_lt(Value *r,Value *a,Value *b,Value *c);
int Value_ge(Value *r,Value *a,Value *b,Value *c);
int Value_le(Value *r,Value *a,Value *b,Value *c);
int Value_eq(Value *r,Value *a,Value *b,Value *c);
int Value_neq(Value *r,Value *a,Value *b,Value *c);
int Value_caseEq(Value *r,Value *a,Value *b,Value *c);
int Value_caseNeq(Value *r,Value *a,Value *b,Value *c);
int Value_and(Value *r,Value *a,Value *b,Value *c);
int Value_nand(Value *r,Value *a,Value *b,Value *c);
int Value_xor(Value *r,Value *a,Value *b,Value *c);
int Value_nxor(Value *r,Value *a,Value *b,Value *c);
int Value_or(Value *r,Value *a,Value *b,Value *c);
int Value_nor(Value *r,Value *a,Value *b,Value *c);
int Value_land(Value *r,Value *a,Value *b,Value *c);
int Value_lor(Value *r,Value *a,Value *b,Value *c);
int Value_ifelse(Value *r,Value *a,Value *b,Value *c);
int Value_concat(Value *r,Value *a,Value *b,Value *c);
int Value_repcat(Value *r,Value *a,Value *b,Value *c);
int Value_uand(Value *r,Value *a,Value *b,Value *c);
int Value_uor(Value *r,Value *a,Value *b,Value *c);
int Value_uxor(Value *r,Value *a,Value *b,Value *c);
int Value_unand(Value *r,Value *a,Value *b,Value *c);
int Value_unor(Value *r,Value *a,Value *b,Value *c);
int Value_unxor(Value *r,Value *a,Value *b,Value *c);
int Value_nmos(Value *r,Value *i,Value *g,Value *dummy);
int Value_pmos(Value *r,Value *i,Value *g,Value *dummy);
int Value_bufif1(Value *r,Value *i,Value *e,Value *dummy);
int Value_bufif0(Value *r,Value *i,Value *e,Value *dummy);

/*****************************************************************************
 * Single-word operator methods.
 *
 * Some operators may have more efficient implementations when we know the
 * operands fit into a single word (32-bit or 64-bit depending on the machine).
 * These functions with a _w_ peform the same action as the above normal
 * operator function but are optimized under the single word assumption.
 * The expression byte code generator will automatically select the correct
 * implementation.
 *
 *****************************************************************************/
int Value_w_not(Value *r,Value *a,Value *b,Value *c);
int Value_w_neg(Value *r,Value *a,Value *b,Value *c);
int Value_w_uand(Value *r,Value *a,Value *b,Value *c);
int Value_w_uor(Value *r,Value *a,Value *b,Value *c);
int Value_w_uxor(Value *r,Value *a,Value *b,Value *c);
int Value_w_unand(Value *r,Value *a,Value *b,Value *c);
int Value_w_unor(Value *r,Value *a,Value *b,Value *c);
int Value_w_unxor(Value *r,Value *a,Value *b,Value *c);
int Value_w_add(Value *r,Value *a,Value *b,Value *c);
int Value_w_sub(Value *r,Value *a,Value *b,Value *c);
int Value_w_mul(Value *r,Value *a,Value *b,Value *c);
int Value_w_div(Value *r,Value *a,Value *b,Value *c);
int Value_w_mod(Value *r,Value *a,Value *b,Value *c);
int Value_w_rshift(Value *r,Value *a,Value *b,Value *c);
int Value_w_lshift(Value *r,Value *a,Value *b,Value *c);
int Value_w_arshift(Value *r,Value *a,Value *b,Value *c);
int Value_w_alshift(Value *r,Value *a,Value *b,Value *c);
int Value_w_gt(Value *r,Value *a,Value *b,Value *c);
int Value_w_lt(Value *r,Value *a,Value *b,Value *c);
int Value_w_ge(Value *r,Value *a,Value *b,Value *c);
int Value_w_le(Value *r,Value *a,Value *b,Value *c);
int Value_w_land(Value *r,Value *a,Value *b,Value *c);
int Value_w_lor(Value *r,Value *a,Value *b,Value *c);
int Value_w_mul(Value *r,Value *a,Value *b,Value *c);
int Value_w_div(Value *r,Value *a,Value *b,Value *c);

/*****************************************************************************
 * Floating-point operator methods
 *
 *****************************************************************************/
int Value_f_gt(Value *r,Value *a,Value *b,Value *c);
int Value_f_lt(Value *r,Value *a,Value *b,Value *c);
int Value_f_ge(Value *r,Value *a,Value *b,Value *c);
int Value_f_le(Value *r,Value *a,Value *b,Value *c);
int Value_f_eq(Value *r,Value *a,Value *b,Value *c);
int Value_f_neq(Value *r,Value *a,Value *b,Value *c);
int Value_f_add(Value *r,Value *a,Value *b,Value *c);
int Value_f_sub(Value *r,Value *a,Value *b,Value *c);
int Value_f_mul(Value *r,Value *a,Value *b,Value *c);
int Value_f_div(Value *r,Value *a,Value *b,Value *c);
int Value_f_neg(Value *r,Value *a,Value *b,Value *c);
int Value_f_caseEq(Value *r,Value *a,Value *b,Value *c);
int Value_f_caseNeq(Value *r,Value *a,Value *b,Value *c);

#endif
