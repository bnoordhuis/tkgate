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
#ifndef __value_h
#define __value_h

/*****************************************************************************
 *
 * Define the real type we are going to use.
 *
 *****************************************************************************/
typedef float real_t;

/*
  State symbol codes
 */
#define SYM_INVALID	-1
#define SYM_NUL1	0
#define SYM_ZERO	1
#define SYM_ONE		2
#define SYM_NUL2	3
#define SYM_FLOAT	4
#define SYM_LOW		5
#define SYM_HIGH	6
#define SYM_UNKNOWN	7


/*
 * Special property flags of Value
 */
#define SF_INT		0x1				/* Declared as an integer */
#define SF_DEC		0x2				/* Declared as sized decimal */
#define SF_HEX		0x4				/* Declared as sized hex */
#define SF_OCT		0x8				/* Declaeed as sized octal */
#define SF_BIN		0x10				/* Declared as sized binary */
#define SF_STRING	0x20				/* Declared as string */
#define SF_REAL		0x40				/* Declared as real */
#define SF_STICKY_MASK	0xffff				/* These flags are sticky and are propegated */
#define SF_NETVAL	0x10000				/* Value is directly associated with a net */

/*
 * Basic word size/byte size declarations.
 */
#define SSWORDSIZE		TKGATE_WORDSIZE		/* # bits in an unsigned */
#define SSWORDMASK		((unsigned)~0)		/* Word with all bits set */
#define SSREALSIZE		(8*sizeof(real_t))	/* Size of real in bits */
#define SSREALBYTES		(sizeof(real_t))	/* Size of real in bytes */
#if (SSWORDSIZE == 32)
#define SSBITMASK		0x1f			/* Mask to get bit in word */
#define SSWORDSHIFT		5			/* Shift to get word index */
#define SSWORDBYTES		4			/* Number of bytes per word */
#define SSHALFWORDMASK		0xffff			/* Mask for a half word */
#elif (SSWORDSIZE == 64)
#define SSBITMASK		0x3f			/* Mask to get bit in word */
#define SSWORDSHIFT		6			/* Shift to get word index */
#define SSWORDBYTES		8			/* Number of bytes per word */
#define SSHALFWORDMASK		0xffffffff		/* Mask for a half word */
#else
#error Unsupported word size.
#endif


/*
  LMASK returns a mask with the low n bits set
  HMASK returns a mask with the high n bits set

  HMASKZ returns a mask with all but the low n bits set
  LMASKZ returns a mask with all but the high n bits set
 */
#define LMASK(n)	(((n) == 0) ? 0 : (SSWORDMASK >> (SSWORDSIZE-(n))))
#define HMASK(n)	(((n) == 0) ? 0 : (SSWORDMASK << (SSWORDSIZE-(n))))
#define LMASKZ(n)	(((n) == SSWORDSIZE) ? 0 : (SSWORDMASK >> (n)))
#define HMASKZ(n)	(((n) == SSWORDSIZE) ? 0 : (SSWORDMASK << (n)))

/*
 * Number of words needed for b bits.
 */
#define SSNUMWORDS(b)	(((b)>>SSWORDSHIFT) + (((b)&SSBITMASK)!=0))

/*
 * Bit position (in the high words) of the high bit in a b-bit word
 */
#define SSHIGHBIT(b)	(((b)-1)&SSBITMASK)

/*****************************************************************************
 *
 * Wire handler function
 *
 *****************************************************************************/
typedef void wirefunc_f(Value *R,Value *A,Value *B);


/*****************************************************************************
 *
 * transtype_t - transition type
 *
 *****************************************************************************/
typedef enum {
  TT_NONE,		/* No transition */
  TT_POSEDGE,		/* Rising edge transition */
  TT_NEGEDGE,		/* Falling edge transition */
  TT_EDGE,		/* Generic transition */
} transtype_t;


/*
         Logic values
         0  1  x  z  L  H
-------------------------
zero     1  0  1  0  1  0
one      0  1  1  0  0  1
flt      0  0  1  1  1  1
*/

#define DEBUG_VALUE_MEMMGR 0

struct Value_str {
#if DEBUG_VALUE_MEMMGR
  int		status;		/* Status code for memory management */
#endif
  unsigned	flags;		/* Property flags */
  unsigned	permFlags;	/* Perminant property flags */
  short		nbits;		/* Number of bits in state */
  short		nalloc;		/* Number of words allocated */
  unsigned	*zero;		/* Bit indicating zero */
  unsigned	*one;		/* Bit indicating one */
  unsigned	*flt;		/* Bit indicating float */
};


/*****************************************************************************
 *
 * Freelist for values
 *
 *****************************************************************************/
struct value_fl {
  Value			state;	/* Actual state data */
  struct value_fl	*next;	/* Next pointer for free list */
};

Value *new_Value(int nbits);
void delete_Value(Value*);
void Value_init(Value *S,int nbits);
void Value_uninit(Value *S);
void Value_reinit(Value *S,int nbits);
void Value_zero(Value *S);
void Value_one(Value *S);
void Value_lone(Value *S);
void Value_unknown(Value *S);
void Value_float(Value *S);
void Value_print(Value *S,FILE *f);
void Value_copy(Value *R,Value *A);
transtype_t Value_copyRange(Value *R,int rl,Value *A,int ah,int al);
void Value_resize(Value *R,int nbits);
void Value_makeSameSize(Value *A,Value *B);
void Value_makeSameSize3(Value *A,Value *B,Value *C);
int Value_getBitSym(Value*,int);
int Value_isLogic(Value*);
void Value_wire(Value *R,Value *A,Value *B);
void Value_wand(Value *R,Value *A,Value *B);
void Value_wor(Value *R,Value *A,Value *B);
void Value_tri0(Value *R,Value *A,Value *B);
void Value_tri1(Value *R,Value *A,Value *B);
void Value_trireg(Value *R,Value *A,Value *B);
int Value_isValue(Value *S);
#define Value_nbits(S) (S)->nbits
void Value_w_roll(Value *R,Value *I,int shift);
void Value_w_shift(Value *R,Value *I,int n,int in1,int in0,int inZ);
void Value_shift(Value *R,Value *I,int n,int in1,int in0,int inZ);
int Value_isEqual(Value *A,Value *B);
transtype_t Value_transitionType(Value *A,Value *B);
int Value_isZero(Value *S);
#define Value_getAllFlags(S) ((S)->flags|(S)->permFlags)
#define Value_getTypeFlags(S) (S)->flags
#define Value_isReal(S) ((S)->flags & SF_REAL)
void Value_normalize(Value *r);


/*****************************************************************************
 * Value conversion methods - other to Value
 *****************************************************************************/
int Value_convert(Value *S,const char *A);
int Value_convertStr(Value *S,const char *s);
int Value_convertI(Value *S,int A);
int Value_convertR(Value *S,real_t A);
int Value_convertNaN(Value *S);
int Value_convertTime(Value *S,simtime_t A);
int Value_convertFromInt(Value*,unsigned);
int Value_convertHex(Value *S, const char *p,int nbits);
int Value_convertBits(Value *S, const char *p,int nbits);
int Value_convertOct(Value *S,const char *A,int nbits);
int Value_convertDec(Value *S,const char *A,int nbits);

int Value_format(Value *S,const char *fmt,char *p);
int Value_toReal(Value*,real_t*);
int Value_toInt(Value*,unsigned*);
int Value_toTime(Value*,simtime_t*);
int Value_toString(Value *S,char *p);
int Value_getstr(Value *S,char *p);
int Value_getvstr(Value *S,char *p);

#endif
