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
#include <assert.h>
#include <ctype.h>
#include "thyme.h"

#define DEBUG_VALUE 0

#define isBDig(x)	((x) == SYM_ONE || (x) == SYM_ZERO)

#define ABS(n)		((n) < 0 ? -(n) : (n))

static char *sym_table_lc = "*01@zlhx";
static char *sym_table = "*01@zLHx";
static int sym_table_len = 8;

static int value_print_showbits = 1;

static struct value_fl *value_freePool = 0;

/*
  Allocate a temporary Value object
 */
Value *new_Value(int nbits)
{
  struct value_fl *S;
  static int fl_count = 0;
  static int ma_count = 0;

  if (!value_freePool) {
    S = (struct value_fl*) malloc(sizeof(struct value_fl));
    if (nbits > 0)
      Value_init((Value*)S,nbits);
    else
      Value_init((Value*)S,1);
#if DEBUG_VALUE_MEMMGR
    S->state.status = 1;
#endif
    ma_count++;
  } else {
    S = value_freePool;
    value_freePool = value_freePool->next;
    fl_count++;
#if DEBUG_VALUE_MEMMGR
    if (S->state.status != -1) {
      vgio_echo("bad state object in alloc_State() (status=%d).\n",S->state.status);
      assert(0);
    }
    S->state.status = 2;
#endif
    if (nbits > 0)
      Value_reinit((Value*)S,nbits);
  }

  S->state.flags = 0;
  S->state.permFlags = 0;

#if DEBUG_VALUE
  if (((fl_count+ma_count) % 10000) == 0) {
    vgio_echo("value: fl:%d ma:%d   (%f)\n",fl_count,ma_count,(double)ma_count/(double)fl_count);
    fflush(stdout);
  }
#endif

  return (Value*)S;
}

void delete_Value(Value *S)
{
  struct value_fl *flS = (struct value_fl *)S;
  flS->next = value_freePool;
  value_freePool = flS;

#if DEBUG_VALUE_MEMMGR
  if (S->status != 1 && S->status != 2) {
    vgio_echo("bad state object in free_State() (status=%d).\n",S->status);
    assert(0);
  }
  S->status = -1;
#endif
}

/*
 * Does S contain only 1s and 0s?
 */
int Value_isLogic(Value *S)
{
  int wc = SSNUMWORDS(S->nbits);
  unsigned mask = SSWORDMASK;
  int i;

  for (i = 0;i < wc;i++) {
    if ((S->nbits & SSBITMASK))
      mask = LMASK((S->nbits & SSBITMASK));

    if ((S->flt[i] & mask)) return 0;
  }

  return 1;
}

/*****************************************************************************
 *
 * Make a value stored in the one[] array consistant in the other arrays.
 *
 * Parameter:
 *     S		Value to be "normalized"
 *
 *****************************************************************************/
void Value_normalize(Value *r)
{
  int wc = SSNUMWORDS(r->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    r->zero[i] = ~r->one[i];
    r->flt[i]  = 0;
  }
}


/*
         Logic values
         0  1  x  z  L  H
-------------------------
one      0  1  1  0  0  1
zero     1  0  1  0  1  0
flt      0  0  1  1  1  1
*/

static int charToSym(char c)
{
  char *x;
  int p;

  if (c == '?') c = 'z';
  if (isupper((int)c))
    c = tolower(c);
  x = strchr(sym_table_lc,c);
  p = x ? (x-sym_table_lc) : SYM_INVALID;

  return p;
}

void Value_init(Value *S,int nbits)
{
  int wc = SSNUMWORDS(nbits);

#if DEBUG_VALUE_MEMMGR
  S->status = 0;
#endif
  S->nbits = nbits;
  S->nalloc = wc;
  S->one = (unsigned*)malloc(wc*sizeof(unsigned));
  S->zero = (unsigned*)malloc(wc*sizeof(unsigned));
  S->flt = (unsigned*)malloc(wc*sizeof(unsigned));
  S->flags = 0;
  S->permFlags = 0;
}

void Value_uninit(Value *S)
{
  free(S->one);
  free(S->zero);
  free(S->flt);
}

void Value_reinit(Value *S,int nbits)
{
  int nwc = SSNUMWORDS(nbits);

  if (nwc <= S->nalloc) {
    S->nbits = nbits;
    S->nalloc = nwc;
  } else {
    Value_uninit(S);
    Value_init(S,nbits);
  }
}

/*****************************************************************************
 *
 * Test to see if the bits in a from lowz up are all zero.
 *
 *****************************************************************************/
int Value_isPartZero(Value *A,int lowz)
{
  int wc = SSNUMWORDS(lowz);
  unsigned mask = (lowz&SSBITMASK) ? LMASK((lowz & SSBITMASK)) : SSWORDMASK;
  int wc2 = SSNUMWORDS(A->nbits);
  unsigned mask2 = (A->nbits&SSBITMASK) ? LMASK((A->nbits & SSBITMASK)) : SSWORDMASK;
  int i;

  if (wc2 == wc) 
    mask = (~mask) & mask2;
  else
    mask = (~mask);

  if ((A->one[wc-1]&mask) != 0) return 0;
  if ((A->zero[wc-1]&mask) != mask) return 0;
  if ((A->flt[wc-1]&mask) != 0) return 0;

  for (i = wc;i < wc2;i++) {
    if (i == wc2-1) {
      if ((A->one[i]&mask2) != 0) return 0;
      if ((A->zero[i]&mask2) != mask2) return 0;
      if ((A->flt[i]&mask2) != 0) return 0;
    } else {
      if ((A->one[i]) != 0) return 0;
      if ((A->zero[i]) != SSWORDMASK) return 0;
      if ((A->flt[i]) != 0) return 0;
    }
  }

  return 1;
}


/*****************************************************************************
 *
 * Check for exact equivalence (including tests z and x bits)
 *
 * Paramaters:
 *     A,B		Values to compare
 *
 * Returns:		Non-zero if A and B are identical including x/z bits.
 *
 *
 *****************************************************************************/
int Value_isEqual(Value *A,Value *B)
{
  unsigned nbits = imin(A->nbits, B->nbits);
  int wc = SSNUMWORDS(nbits);
  unsigned mask = (nbits&SSBITMASK) ? LMASK((nbits & SSBITMASK)) : SSWORDMASK;
  int i;

  /*
   * Check for equality up to smaller of a and b bit size.
   */
  for (i = 0;i < wc;i++) {
    if (i == wc-1) {
      if ((A->one[i]&mask) != (B->one[i]&mask)) return 0;
      if ((A->zero[i]&mask) != (B->zero[i]&mask)) return 0;
      if ((A->flt[i]&mask) != (B->flt[i]&mask)) return 0;
    } else {
      if (A->one[i] != B->one[i]) return 0;
      if (A->zero[i] != B->zero[i]) return 0;
      if (A->flt[i] != B->flt[i]) return 0;
    }
  }

  /*
   * If bit sizes are different, extend with zeros.
   */
  if (A->nbits > B->nbits) {
    return Value_isPartZero(A,B->nbits);
  } else if (A->nbits < B->nbits) {
    return Value_isPartZero(B,A->nbits);
  } else
    return 1;  
}

/*****************************************************************************
 *
 * Returns the transition type
 *
 *
 *****************************************************************************/
transtype_t Value_transitionType(Value *A,Value *B)
{
  if (Value_nbits(A) == 1) {
    int fromSym = Value_getBitSym(A,0);
    int toSym   = Value_getBitSym(B,0);

    if (fromSym == toSym) return TT_NONE;
    if (toSym == SYM_ZERO) return TT_NEGEDGE;
    if (fromSym == SYM_ONE && toSym != SYM_FLOAT) return TT_NEGEDGE;
    return TT_POSEDGE;						    
  } else {
    if (Value_isEqual(A,B))
      return TT_NONE;
    else
      return TT_EDGE;
  }
}

void Value_unknown(Value *S)
{
  int wc = SSNUMWORDS(S->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    S->zero[i] = ~0;
    S->one[i] = ~0;
    S->flt[i] = ~0;
  }
}

void Value_float(Value *S)
{
  int wc = SSNUMWORDS(S->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    S->zero[i] = 0;
    S->one[i] = 0;
    S->flt[i] = ~0;
  }
}

void Value_zero(Value *S)
{
  int wc = SSNUMWORDS(S->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    S->zero[i] = ~0;
    S->one[i] = 0;
    S->flt[i] = 0;
  }
}

void Value_one(Value *S)
{
  int wc = SSNUMWORDS(S->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    S->one[i] = ~0;
    S->flt[i] = 0;
    S->zero[i] = 0;
  }
}

/*****************************************************************************
 *
 * Make S a logic 1 (low bit is 1, all others 0)
 *
 *****************************************************************************/
void Value_lone(Value *S)
{
  Value_zero(S);
  S->one[0] |= 1;
  S->zero[0] &= ~1;
}

/*
  return non-zero if S has only 0 and 1 bits
 */
int Value_isValue(Value *S)
{
  register int i;
  register unsigned mask = SSWORDMASK;
  register int wc = SSNUMWORDS(S->nbits);

  for (i = 0;i < wc;i++) {
    if (i == wc-1 && (S->nbits & SSBITMASK))
      mask = LMASK(S->nbits);
      

    if ((S->flt[i] & mask)) return 0;
  }
  return 1;
}

/*
  return non-zero if S is zero
 */
int Value_isZero(Value *S)
{
  register int i;
  register unsigned mask = SSWORDMASK;
  register int wc = SSNUMWORDS(S->nbits);

  for (i = 0;i < wc;i++) {
    if (i == wc-1 && (S->nbits & SSBITMASK))
      mask = LMASK(S->nbits);
      
    if (((S->flt[i] & mask) != 0)
	|| ((S->one[i] & mask) != 0)
	|| ((S->zero[i] & mask) != mask))
      return 0;
			   
  }
  return 1;
}


void Value_xclear(Value *S)
{
  int wc = SSNUMWORDS(S->nbits);
  int i;

  for (i = 0;i < wc;i++) {
    S->one[i] = 0;
    S->flt[i] = 0;
    S->zero[i] = 0;
  }
}

int Value_getBitSym(Value *S,int i)
{
  int w;
  unsigned b;
  int x = 0;

  if (i >= S->nbits)
    return SYM_ZERO;

  w = i >> SSWORDSHIFT;
  b = 1 << (i & SSBITMASK);
  if ((S->zero[w] & b)) x |= 0x1;
  if ((S->one[w] & b))  x |= 0x2;
  if ((S->flt[w] & b))  x |= 0x4;

  return x;
}

void Value_putBitSym(Value *S,int bit,int p)
{
  int w;
  unsigned b;

  if (bit >= S->nbits) return;

  w = bit >> SSWORDSHIFT;
  b = 1 << (bit & SSBITMASK);
  if ((p & 0x1)) S->zero[w] |= b; else S->zero[w] &= ~b;
  if ((p & 0x2)) S->one[w] |= b; else S->one[w] &= ~b;
  if ((p & 0x4)) S->flt[w] |= b; else S->flt[w] &= ~b;
}


/*****************************************************************************
 *
 * Extend symbols in a value from bit d to the top bit.
 *
 * Parameters:
 *     S		Word to be extended
 *     d		Position to begin extension
 *
 * If the bit S[d-1] is a 1 or 0, then fill all higher bit position with a 0,
 * if it is other than a 1 or 0, then fill all higher position with that symbol.
 * This is used so that specifications like "16'bz" will result in a 16-bit
 * value of all floats.
 *
 *****************************************************************************/
void Value_extend(Value *S,int d)
{
  int i;
  int p = (d > 0) ? Value_getBitSym(S,d-1) : SYM_ZERO;
  if (p == SYM_ONE) p = SYM_ZERO;

  for (i = d;i < S->nbits;i++)
    Value_putBitSym(S,i,p);
}

static void Value_extendSym(Value *S,int d,int sym)
{
  int i;

  for (i = d;i < S->nbits;i++)
    Value_putBitSym(S,i,sym);
}

/*****************************************************************************
 *
 * Resize a Value value object
 *
 * Paramaters:
 *     R			Value value object
 *     nbits			Number of bits
 *     dosign			Do sign extension.
 *
 *****************************************************************************/
void Value_resize(Value *R,int nbits)
{
  if (nbits < R->nbits) { 		/* Downsizing */
    R->nbits = nbits;
  } else if (nbits > R->nbits) {	/* Upsizing */
    int ob = R->nbits;
    int nwc = SSNUMWORDS(nbits);

    if (nwc > R->nalloc) {
      R->nalloc = nwc;
      R->zero = (unsigned*) realloc(R->zero,nwc*sizeof(unsigned));
      R->one  = (unsigned*) realloc(R->one,nwc*sizeof(unsigned));
      R->flt  = (unsigned*) realloc(R->flt,nwc*sizeof(unsigned));
    }
    R->nbits = nbits;

    Value_extendSym(R,ob,SYM_ZERO);
  }
}


int Value_toInt(Value *S,unsigned *I)
{
  if (!Value_isValue(S)) return -1;

  if (Value_isReal(S))
    *I = (unsigned) *(real_t*)&S->one[0];
  else
    *I = S->one[0] & LMASK(S->nbits);

  return 0;
}

int Value_toReal(Value *S,real_t *n)
{
  if (!Value_isValue(S)) return -1;

  if (Value_isReal(S)) {
    *n = *(real_t*)&S->one[0];
  } else {
    *n = (real_t)S->one[0];
  }

  return 0;
}

int Value_toTime(Value *S,simtime_t *I)
{
  if (!Value_isValue(S)) return -1;

#if SSWORDSIZE == 64
  *I = S->one[0] & LMASK(S->nbits);
#else
  if (S->nbits < SSWORDSIZE)
    *I = S->one[0] & LMASK(S->nbits);
  else if (S->nbits == SSWORDSIZE)
    *I = S->one[0];
  else if (S->nbits < 2*SSWORDSIZE)
    *I = (((simtime_t)S->one[1]&LMASK(S->nbits-SSWORDSIZE)) << 32) | S->one[0];
  else
    *I = (((simtime_t)S->one[1]) << 32) | S->one[0];
#endif
  return 0;
}

int Value_convertBits(Value *S,const char *A,int nbits)
{
  int d = strlen(A);
  int i;
  int return_value = 0;

  if (!nbits) nbits = d;

  Value_reinit(S,nbits);
  Value_xclear(S);

  for (i = 0;i < d;i++) {
    int p = charToSym(A[d-i-1]);
    if (p == SYM_INVALID) {
      return_value = -1;
      p = SYM_ZERO;
    }
    
    Value_putBitSym(S,i,p);
  }
  Value_extend(S,d);

  return return_value;
}

int Value_convertHex(Value *S,const char *A,int nbits)
{
  int d = strlen(A);
  int return_value = 0;
  int i;

  if (nbits == 0) nbits = d*4;

  Value_reinit(S,nbits);
  Value_xclear(S);

  for (i = 0;i < d;i++) {
    char c = A[d-i-1];
    if (isxdigit((int)c)) {
      int b = 4*i;
      int v;
      if (isdigit((int)c))
	v = c - '0';
      else {
	if (isupper((int)c)) c = tolower(c);
	v = c - 'a' + 10;
      }
      Value_putBitSym(S,b,  (v&0x1)?SYM_ONE:SYM_ZERO);
      Value_putBitSym(S,b+1,(v&0x2)?SYM_ONE:SYM_ZERO);
      Value_putBitSym(S,b+2,(v&0x4)?SYM_ONE:SYM_ZERO);
      Value_putBitSym(S,b+3,(v&0x8)?SYM_ONE:SYM_ZERO);
    } else {
      int p = charToSym(c);
      int b = 4*i;

      if (p == SYM_INVALID) {
	return_value = -1;
	p = SYM_ZERO;
      }

      Value_putBitSym(S,b,p);
      Value_putBitSym(S,b+1,p);
      Value_putBitSym(S,b+2,p);
      Value_putBitSym(S,b+3,p);
    }
  }
  Value_extend(S,4*d);

  return return_value;
}

int Value_convertOct(Value *S,const char *A,int nbits)
{
  int d = strlen(A);
  int i;
  int return_value = 0;

  if (nbits == 0) nbits = d*3;

  Value_reinit(S,nbits);
  Value_xclear(S);

  for (i = 0;i < d;i++) {
    char c = A[d-i-1];
    if (isodigit((int)c)) {
      int b = 3*i;
      int v;

      v = c - '0';
      Value_putBitSym(S,b,  (v&0x1)?SYM_ONE:SYM_ZERO);
      Value_putBitSym(S,b+1,(v&0x2)?SYM_ONE:SYM_ZERO);
      Value_putBitSym(S,b+2,(v&0x4)?SYM_ONE:SYM_ZERO);
    } else {
      int p = charToSym(c);
      int b = 3*i;

      if (p == SYM_INVALID) {
	return_value = -1;
	p = SYM_ZERO;
      }

      Value_putBitSym(S,b,p);
      Value_putBitSym(S,b+1,p);
      Value_putBitSym(S,b+2,p);
    }
  }
  Value_extend(S,3*d);

  return return_value;
}

/*****************************************************************************
 *
 * Convert a string in decimal format to a value.
 *
 * Parameters:
 *      S 		Place to store value
 *      p 		String to convert
 *      nbits 		Number of bits for value (or 0 for auto sizing)
 *
 *****************************************************************************/
int Value_convertDec(Value *S,const char *p,int nbits)
{
  int l = strlen(p);
  unsigned n;
  int return_value = 0;

  S->flags = SF_DEC;

  if (l <= 9) {
    /*
     * Value is short enough to fit into 32 bit value
     */
    if (S->nbits == 0)
      Value_reinit(S,SSWORDSIZE);
    else
      Value_reinit(S,nbits);

    if (sscanf(p,"%u",&n) != 1) {
      *S->one = *S->zero = *S->flt = 0;
      return -1;
    }
    S->one[0] = n;
    if (nbits == 0)
      S->nbits = SSWORDSIZE;
  } else {
    const double K = 3.32192809488736234791;	/* 1/log10(2) */
    int reqbits = (int)(l*K+1);			/* Estimated bits required */
    int wc;

    if (nbits == 0)
      Value_reinit(S,reqbits);
    else
      Value_reinit(S,nbits);

    wc = SSNUMWORDS(S->nbits);
    multint_cvtstr(S->one,wc,p);

    /*
     * Readjust size if automatic sizing is used.
     */
    if (nbits == 0) {
      while (S->one[wc-1] == 0 && wc > 1) wc--;
      S->nbits = wc*SSWORDSIZE;
    }
  }

  Value_normalize(S);

  return return_value;
}

/*****************************************************************************
 *
 * Convert a text string to an Value object
 *
 * Parameters:
 *     S		Target state value in which to store scanned value
 *     A		Ascii string with verilog format constant.
 *
 *****************************************************************************/
int Value_convert(Value *S,const char *A)
{
  int nbits = 0;
  const char *p;
  char c;
  int return_value = 0;

  S->flags = 0;

  if (sscanf(A,"%d'%c",&nbits,&c) == 2) {
    p = strchr(A,'\'')+2;
  } else if (sscanf(A,"'%c",&c) == 1) {
    nbits = 0;
    p = strchr(A,'\'')+2;
  } else {
    nbits = 0;
    p = A;
    c = 'i';
  }

  switch (c) {
  case 'b' :
    return_value = Value_convertBits(S,p,nbits);
    S->flags = SF_BIN;
    break;
  case 'h' :
    return_value = Value_convertHex(S,p,nbits);
    S->flags = SF_HEX;
    break;
  case 'o' :
    return_value = Value_convertOct(S,p,nbits);
    S->flags = SF_OCT;
    break;
  case 'd' :
    return_value = Value_convertDec(S,p,nbits);
    S->flags = SF_DEC;
    break;
  case 'i' :
    return_value = Value_convertDec(S,p,nbits);
    S->flags = SF_INT;
    break;
  default:
    S->nbits = 1;
    *S->one = *S->zero = *S->flt = 0;
    return -1;
  }

  return return_value;
}


int Value_convertFromInt(Value *S,unsigned I)
{
  if (S->nbits > SSWORDSIZE) return -1;
  S->one[0] = I;
  S->zero[0] = ~I;
  S->flt[0] = 0;
  return 0;
}

/*****************************************************************************
 *
 * Convert an integer to a 32-bit/64-bit Value
 *
 * Parameters:
 *     S		Target state value in which to store scanned value
 *     n		Integer to convert
 *
 *****************************************************************************/
int Value_convertI(Value *S,int n)
{
  Value_resize(S,SSWORDSIZE);
  S->flags = SF_INT;
  S->one[0] = n;
  S->zero[0] = ~n;
  S->flt[0] = 0;

  return 0;
}

/*****************************************************************************
 *
 * Convert a real
 *
 * Parameters:
 *     S		Target state value in which to store scanned value
 *     n		Integer to convert
 *
 *****************************************************************************/
int Value_convertR(Value *S,real_t n)
{
  Value_resize(S,SSREALSIZE);

  S->flags = SF_REAL;
  memmove(S->one,&n,SSREALBYTES);

  Value_normalize(S);

  return 0;
}

/*****************************************************************************
 *
 * Convert a real NaN
 *
 * Parameters:
 *     S		Target state value in which to store scanned value
 *     n		Integer to convert
 *
 *****************************************************************************/
int Value_convertNaN(Value *S)
{
  Value_resize(S,SSREALSIZE);

  S->flags = SF_REAL;
  memset(S->one,0,SSREALBYTES);
  memset(S->zero,0,SSREALBYTES);
  memset(S->flt,0xff,SSREALBYTES);

  return 0;
}

/*****************************************************************************
 *
 * Convert a 64-bit time to a 64-bit Value
 *
 * Parameters:
 *     S		Target state value in which to store scanned value
 *     n		Integer to convert
 *
 *****************************************************************************/
int Value_convertTime(Value *S,simtime_t n)
{
  Value_resize(S,sizeof(simtime_t)*8);
  S->flags = SF_INT;

#if TKGATE_WORDSIZE == 64
  S->one[0] = n;
  S->zero[0] = ~n;
  S->flt[0] = 0;
#else
  S->one[0] = n & 0xffffffff;
  S->zero[0] = ~n & 0xffffffff;
  S->flt[0] = 0;

  S->one[1] = (n>>32) & 0xffffffff;
  S->zero[1] = ~(n>>32) & 0xffffffff;
  S->flt[1] = 0;
#endif

  return 0;
}

/*****************************************************************************
 *
 * Convert a string into an Value encoding that string.
 *
 * Parameters:
 *     S		Target value
 *     str		String representation of value to be converted
 *
 *****************************************************************************/
int Value_convertStr(Value *S,const char *str)
{
  int l = strlen(str);
  int nbits = imax(l*8,8);
  int wc = SSNUMWORDS(nbits);
  int i,j;
  const char *s_end = str+l-1;

  Value_reinit(S,nbits);
  Value_zero(S);

  for (i = 0;i < wc;i++) {
    for (j = 0;j < SSWORDBYTES;j++) {
      const char *cstart = s_end-SSWORDBYTES*i-j;
      if (cstart >= str)
	S->one[i] |= (*cstart & 0xff) << 8*j;
    }
  }

  Value_normalize(S);
  S->flags = SF_STRING;

  return 0;
}

void Value_printSymbol(int x,FILE *f)
{
  if (x >= 0 && x < sym_table_len)
    fputc(sym_table[x],f);
  else
    fprintf(f,":%d:",x);
}

int Value_symbolStr(int x,char *p)
{
  if (x >= 0 && x < sym_table_len) {
    *p = sym_table[x];
    return 1;
  } else
    return sprintf(p,":%d:",x);
}

static int Value_getstr_str(Value *S,char *p,int prefix)
{
  int wc = SSNUMWORDS(S->nbits);
  int i,j;

  if (prefix)
    *p++ = '"';
  for (i = wc-1;i >= 0;i--) {
    for (j = SSWORDBYTES-1;j >= 0;j--) {
      unsigned c = (S->one[i] >> (j*8)) & 0xff;
      if (c) {
	if (prefix && (c == '"' || c == '\\')) *p++ = '\\';
	*p++ = c;
      }
    }
  }
  if (prefix)
    *p++ = '"';

  *p = 0;
  return 0;
}

/*****************************************************************************
 *
 * Convert an integer in value form into its string representation.
 *
 * Parameters:
 *     S		Value to be converted
 *     str		Buffer to which to write string.
 *
 *****************************************************************************/
static int Value_getstr_int(Value *S,char *p)
{
  if (Value_isValue(S)) {
    if (Value_nbits(S) <= SSWORDSIZE) {
      sprintf(p,"%u",S->one[0]&LMASK(Value_nbits(S)));
    } else {
      int wc = SSNUMWORDS(S->nbits);

      S->one[wc-1] &= LMASK(Value_nbits(S));
      multint_getstr(S->one,wc,p,1024);
    }
  } else
    sprintf(p,"x");

  return 0;
}

#if 0
static int Value_getstr_time(Value *S,char *p)
{
  if (Value_isValue(S)) {
#if SSWORDSIZE == 64
    sprintf(p,"%u",S->one[0]&LMASK(Value_nbits(S)));
#else
    if (Value_nbits(S) <= SSWORDSIZE) {
      sprintf(p,"%u",S->one[0]&LMASK(Value_nbits(S)));
    } else {
      sprintf(p,"%llu",((((simtime_t)S->one[1]&LMASK(S->nbits-SSWORDSIZE)) << 32) | S->one[0]));
    }
#endif
  } else
    sprintf(p,"x");

  return 0;
}
#endif

static int Value_getstr_dec(Value *S,char *p,int prefix)
{
  if (prefix)
    p += sprintf(p,"%d'd",Value_nbits(S));

  if (Value_isValue(S)) {
    if (Value_nbits(S) <= SSWORDSIZE)
      sprintf(p,"%u",S->one[0]&LMASK(Value_nbits(S)));
    else
      Value_getstr_int(S,p);
  } else
    sprintf(p,"x");

  return 0;
}

/*****************************************************************************
 *
 * Generate the string encoded by value S
 *
 *****************************************************************************/
int Value_toString(Value *S,char *p)
{
  Value_getstr_str(S,p,0);
  return 0;
}

/*****************************************************************************
 *
 * Trim leading 0/x/z digits from the string representation of a value
 *
 * Parameters:
 *     p		String to be trimmed
 *
 * Returns:		Pointer to end of trimmed string.
 *
 *****************************************************************************/
static char *Value_trim(char *p)
{
  char *q = p;
  char top = *q;

  if (isxdigit(top) && top != '0')
    return p + strlen(p);

  while (top == *q) q++;

  if ((!*q || !isxdigit(*q) || !isxdigit(top)) && q != p) q--;

  if (p != q)
    memmove(p,q,strlen(q)+1);
  return p + strlen(p);
}

static int Value_getstr_bin(Value *S,char *p,int prefix)
{
  char *trimPart;
  char *q = p;
  int i;

  if (prefix)
    p += sprintf(p,"%d'b",Value_nbits(S));
  trimPart = p;

  for (i = Value_nbits(S)-1;i >= 0;i--) {

    Value_symbolStr(Value_getBitSym(S,i),p);
    p++;
  }

  *p = 0;
  return Value_trim(trimPart) - q;
}

static int Value_getstr_oct(Value *S,char *p,int prefix)
{
  int N = S->nbits;
  int M = ((N+2)/3)*3;		/* Round to multiple of 3 */
  int i;
  char *q = p;
  char *trimPart;

  if (prefix)
    p += sprintf(p,"%d'o",N);
  trimPart = p;


  for (i = M;i > 0;i -= 3) {
    int x0 = Value_getBitSym(S,i-3);
    int x1 = Value_getBitSym(S,i-2);
    int x2 = Value_getBitSym(S,i-1);
    int bits_same,logic_digs;

    bits_same = (x2 == x0 || i-1 >= N)
      && (x1 == x0 || i-2 >= N);
    logic_digs = isBDig(x0) && isBDig(x1) && isBDig(x2);

    if (bits_same && !isBDig(x0))
      p += Value_symbolStr(x0,p);
    else if (logic_digs) {
      int d = 0;
      if (x0 == SYM_ONE) d |= 0x1;
      if (x1 == SYM_ONE) d |= 0x2;
      if (x2 == SYM_ONE) d |= 0x4;

      p += sprintf(p,"%d",d);
    } else {
      if (value_print_showbits) {
	p += sprintf(p,"(");
	if (i-1 < N) p += Value_symbolStr(x2,p);
	if (i-2 < N) p += Value_symbolStr(x1,p);
	if (i-3 < N) p += Value_symbolStr(x0,p);
	p += sprintf(p,")");
      } else
	p += sprintf(p,"?");
    }
  }
  *p = 0;

  return Value_trim(trimPart) - q;
}


static int Value_getstr_real(Value *S,char *p,int useG)
{
  if (!Value_isValue(S))
    return sprintf(p,"NaN");
  else {
    real_t n = *(real_t*)&S->one[0];
    if (useG)
      return sprintf(p,"%g",n);
    else
      return sprintf(p,"%f",n);
  }
}

static int Value_getstr_hex(Value *S,char *p,int prefix)
{
  int N = S->nbits;
  int M = (N & 0x3) ? (N | 0x3)+1 : N;	/* Round to multiple of 4 */
  int i;
  char *q = p;
  char *trimPart;

  if (prefix)
    p += sprintf(p,"%d'h",N);
  trimPart = p;

  for (i = M;i > 0;i -= 4) {
    int x0 = Value_getBitSym(S,i-4);
    int x1 = Value_getBitSym(S,i-3);
    int x2 = Value_getBitSym(S,i-2);
    int x3 = Value_getBitSym(S,i-1);
    int bits_same,logic_digs;

    bits_same = (x3 == x0 || i-1 >= N)
      && (x2 == x0 || i-2 >= N)
	&& (x1 == x0 || i-3 >= N);
    logic_digs = isBDig(x0) && isBDig(x1) && isBDig(x2) && isBDig(x3);

    if (bits_same && !isBDig(x0))
      p += Value_symbolStr(x0,p);
    else if (logic_digs) {
      int d = 0;
      if (x0 == SYM_ONE) d |= 0x1;
      if (x1 == SYM_ONE) d |= 0x2;
      if (x2 == SYM_ONE) d |= 0x4;
      if (x3 == SYM_ONE) d |= 0x8;

      if (d <= 9)
	p += sprintf(p,"%d",d);
      else
	p += sprintf(p,"%c",d-10+'a');
    } else {
      if (value_print_showbits) {
	p += sprintf(p,"(");
	if (i-1 < N) p += Value_symbolStr(x3,p);
	if (i-2 < N) p += Value_symbolStr(x2,p);
	if (i-3 < N) p += Value_symbolStr(x1,p);
	if (i-4 < N) p += Value_symbolStr(x0,p);
	p += sprintf(p,")");
      } else
	p += sprintf(p,"?");
    }
  }
  *p = 0;

  return Value_trim(trimPart) - q;
}


/*****************************************************************************
 *
 * Get a string representing the state value.
 *
 * Parameters:
 *     s		Value to convert to string
 *     p		Buffer in which to write string representation
 *
 *****************************************************************************/
int Value_getstr(Value *S,char *p)
{
  if ((S->flags & SF_REAL)) {
    Value_getstr_real(S,p,1);
    return 0;
  }

  /*
   * Display as a decimal value 
   */
  if ((S->flags & SF_INT)) {
    Value_getstr_int(S,p);
#if 0
    if (Value_nbits(S) == SSWORDSIZE)
      Value_getstr_int(S,p);
    else
      Value_getstr_hex(S,p,1);
#endif
    return 0;
  }

  if ((S->flags & SF_DEC)) {
    Value_getstr_dec(S,p,1);
#if 0
    if (Value_nbits(S) <= SSWORDSIZE)
      Value_getstr_dec(S,p,1);
    else
      Value_getstr_hex(S,p,1);
#endif
    return 0;
  }

  if ((S->flags & SF_HEX))
    return Value_getstr_hex(S,p,1);

  if ((S->flags & SF_OCT))
    return Value_getstr_oct(S,p,1);

  if ((S->flags & SF_BIN))
    return Value_getstr_bin(S,p,1);

  if ((S->flags & SF_STRING)) {
    return Value_getstr_str(S,p,1);
  }

  if (Value_nbits(S) == 1)
    return Value_getstr_bin(S,p,1);
  else
    return Value_getstr_hex(S,p,1);
}

/*****************************************************************************
 *
 * Get a string representing the state value, but exclude the "integer" types.
 *
 * Parameters:
 *     s		Value to convert to string
 *     p		Buffer in which to write string representation
 *
 *****************************************************************************/
int Value_getvstr(Value *S,char *p)
{
  if (Value_nbits(S) == 1)
    return Value_getstr_bin(S,p,1);
  else
    return Value_getstr_hex(S,p,1);
}


/*****************************************************************************
 *
 * Convert state value to a string with formatting.
 *
 * Parameters:
 *     S	State value to convert 
 *     fmt	A format string (see below)
 *     p	Buffer in which to write formatted string
 *
 * 'fmt' must be a single Verilog format string.  The first character must be
 * a '%' and the last character must be the type control character.
 * 
 *****************************************************************************/
int Value_format(Value *S,const char *fmt,char *p)
{
  const char *x = fmt + strlen(fmt) - 1;
  int n;
  int has_length = 0;
  int right_just = 1;
  int zero_fill = 0;
  int p_len;

  if (sscanf(fmt+1,"0%d",&n) == 1) {
    zero_fill = 1;
    has_length = 1;
  } else if (sscanf(fmt+1,"-%d",&n) == 1) {
    right_just = 0;
    has_length = 1;
  } else if (sscanf(fmt+1,"%d",&n) == 1) {
    has_length = 1;
  }

  switch (*x) {
  case 'd' :
  case 'D' :
    Value_getstr_int(S,p);
    break;
  case 'b' :
  case 'B' :
    Value_getstr_bin(S,p,0);
    break;
  case 'h' :
  case 'H' :
    Value_getstr_hex(S,p,0);
    break;
  case 'o' :
  case 'O' :
    Value_getstr_oct(S,p,0);
    break;
  case 'c' :
  case 'C' :
    break;
  case 's' :
  case 'S' :
    Value_getstr_str(S,p,0);
    break;
  case 'm' :
  case 'M' :
    break;
  case 'v' :
  case 'V' :
    break;
#if 0
  case 't' :
  case 'T' :
    Value_getstr_time(S,p);
    break;
#endif
  case 'e' :
  case 'E' :
    break;
  case 'f' :
  case 'F' :
    Value_getstr_real(S,p,0);
    break;
  case 'g' :
  case 'G' :
    Value_getstr_real(S,p,1);
    break;
  }

  p_len = strlen(p);

  if (has_length && p_len < n) {
    if (right_just) {
      int fc = ' ';
      int i;
      if (zero_fill) {
	if (isxdigit(*p))
	  fc = '0';
	else
	  fc = *p;
      }

      memmove(p+n-p_len,p,p_len+1);
      for (i = 0;i < n-p_len;i++)
	p[i] = fc;
    } else {
      int i;
      for (i = p_len;i < n;i++)
	p[i] = ' ';
      p[n] = 0;
    }
  }

  return 0;
}



void Value_print(Value *S,FILE *f)
{
  char buf[STRMAX];

  Value_getstr(S,buf);
  fprintf(f,"%s",buf);
}

/*****************************************************************************
 *
 * Make A and B the same bit size by increasing the size of the smaller one.
 *
 * Parameters:
 *     A,B		Value values to be resized
 *
 *****************************************************************************/
void Value_makeSameSize(Value *A,Value *B)
{
  if (A->nbits < B->nbits) {
    Value_resize(A,B->nbits);
  } else if (B->nbits < A->nbits) {
    Value_resize(B,A->nbits);
  }
}

/*****************************************************************************
 *
 * Make A, B and C the same bit size by increasing the size of the smaller ones.
 *
 * Parameters:
 *     A,B,C		Value values to be resized
 *
 *****************************************************************************/
void Value_makeSameSize3(Value *A,Value *B,Value *C)
{
  int maxSize = A->nbits;
  if (maxSize < B->nbits) maxSize = B->nbits;
  if (maxSize < C->nbits) maxSize = C->nbits;

  Value_resize(A,maxSize);
  Value_resize(B,maxSize);
  Value_resize(C,maxSize);
}

/*****************************************************************************
 *
 * Copy a Value value
 *
 * Parameters:
 *     r		Target of assignment
 *     a		Source of assignment
 *
 *****************************************************************************/
void Value_copy(Value *r,Value *a)
{
  int wc = SSNUMWORDS(r->nbits);
  int i;

  r->flags = a->flags;

  for (i = 0;i < wc;i++) {
    r->one[i] = a->one[i];
    r->zero[i] = a->zero[i];
    r->flt[i] = a->flt[i];
  }
}

/*****************************************************************************
 *
 * Copy range of bits
 *
 * Parameters:
 *     R		Target Value value
 *     rl		Low bit in R at which to start copy
 *     A		Source Value value
 *     ah		High bit in source to copy
 *     al		Low bit in source to copy
 *
 * Returns:		Type of transition that occured.
 *
 * Copy a range of bits.  Copies bits in the range [ah:al] of A into R
 * starting at bit rl.
 *
 *****************************************************************************/
transtype_t Value_copyRange(Value *R,int rl,Value *A,int ah,int al)
{
  transtype_t tt = TT_NONE;
  int rh = rl+(ah-al);						/* High bit of target in R */

  if (ah == al) {
    /*****************************************************************************
     * Special case for one-bit copy.
     *****************************************************************************/
    int fromSym = Value_getBitSym(R,rl);
    int toSym   = Value_getBitSym(A,ah);

    R->flags |= A->flags;

    if (fromSym == toSym) 
      tt = TT_NONE;
    else if (toSym == SYM_ZERO) 
      tt = TT_NEGEDGE;
    else if (fromSym == SYM_ONE && toSym != SYM_FLOAT) 
      tt = TT_NEGEDGE;
    else
      tt = TT_POSEDGE;

    if (Value_nbits(R) != 1 && tt != TT_NONE)
      tt = TT_EDGE;

    Value_putBitSym(R,rl,toSym);
  } else {
    int rl_w = rl >> SSWORDSHIFT;
    int rl_b = rl & SSBITMASK;
    int al_w = al >> SSWORDSHIFT;
    int al_b = al & SSBITMASK;
    int rh_w = rh >> SSWORDSHIFT;
    int rh_b = rh & SSBITMASK;
    int dst_w;
    int src_w;

    if (rl_b == al_b) {
      /*****************************************************************************
       * Copy subregion with shift that is a multiple of word size
       *****************************************************************************/
      src_w = al_w;
      for (dst_w = rl_w; dst_w <= rh_w; dst_w++, src_w++) {
	unsigned rone, rzero, rflt, mask;

	/*
	 * "mask" is a mask of bits in R that should change in this word.
	 */
	mask = SSWORDMASK;
	if (dst_w == rl_w) mask &= HMASKZ(rl_b);
	if (dst_w == rh_w) mask &= LMASK(rh_b+1);

	rone  = (A->one[src_w]&mask)  | (R->one[dst_w]&~mask);
	rzero = (A->zero[src_w]&mask) | (R->zero[dst_w]&~mask);
	rflt  = (A->flt[src_w]&mask)  | (R->flt[dst_w]&~mask);

	/*
	 * If any bits of R changed, update the value and record this as an edge.
	 */
	if ((rone ^ R->one[dst_w]) || (rzero ^ R->zero[dst_w]) || (rflt ^ R->flt[dst_w]) ) {
	  tt = TT_EDGE;
	  R->one[dst_w]  = rone;
	  R->zero[dst_w] = rzero;
	  R->flt[dst_w]  = rflt;
	}
      }
    } else if (rl_b > al_b) {
      /*****************************************************************************
       * Up shifting
       *****************************************************************************/
      int rl_w = rl >> SSWORDSHIFT;
      int rl_b = rl & SSBITMASK;
      int al_w = al >> SSWORDSHIFT;
      int al_b = al & SSBITMASK;
      int rh_w = rh >> SSWORDSHIFT;
      int rh_b = rh & SSBITMASK;
      int dst_w;
      int src_w;
      int b_up = (rl_b - al_b) & SSBITMASK;			/* Shift up bits */
      int b_dn = SSWORDSIZE - b_up;			/* Shift down for next word */

      src_w = al_w;
      for (dst_w = rl_w; dst_w <= rh_w; dst_w++, src_w++) {
	unsigned aone, azero, aflt, rone, rzero, rflt, mask;

	aone  = A->one[src_w] << b_up;
	azero = A->zero[src_w] << b_up;
	aflt  = A->flt[src_w] << b_up;
	if (src_w > 0) {
	  aone  |= A->one[src_w-1] >> b_dn;
	  azero |= A->zero[src_w-1] >> b_dn;
	  aflt  |= A->flt[src_w-1] >> b_dn;
	}

	/*
	 * "mask" is a mask of bits in R that should change in this word.
	 */
	mask = SSWORDMASK;
	if (dst_w == rl_w) mask &= HMASKZ(rl_b);
	if (dst_w == rh_w) mask &= LMASK(rh_b+1);

	rone  = (aone&mask)  | (R->one[dst_w]&~mask);
	rzero = (azero&mask) | (R->zero[dst_w]&~mask);
	rflt  = (aflt&mask)  | (R->flt[dst_w]&~mask);

	/*
	 * If any bits an R changed, update the value and record this as an edge.
	 */
	if ((rone ^ R->one[dst_w]) || (rzero ^ R->zero[dst_w]) || (rflt ^ R->flt[dst_w]) ) {
	  tt = TT_EDGE;
	  R->one[dst_w]  = rone;
	  R->zero[dst_w] = rzero;
	  R->flt[dst_w]  = rflt;
	}
      }
    } else {
      /*****************************************************************************
       * Down shifting
       *****************************************************************************/
      int rl_w = rl >> SSWORDSHIFT;
      int rl_b = rl & SSBITMASK;
      int al_w = al >> SSWORDSHIFT;
      int al_b = al & SSBITMASK;
      int rh_w = rh >> SSWORDSHIFT;
      int rh_b = rh & SSBITMASK;
      int ah_w = ah >> SSWORDSHIFT;
      int dst_w;
      int src_w;
      int b_dn = (al_b - rl_b) & SSBITMASK;			/* Shift down bits */
      int b_up = SSWORDSIZE - b_dn;			/* Shift up for next word */

      src_w = al_w;
      for (dst_w = rl_w; dst_w <= rh_w; dst_w++, src_w++) {
	unsigned aone, azero, aflt, rone, rzero, rflt, mask;

	aone  = A->one[src_w] >> b_dn;
	azero = A->zero[src_w] >> b_dn;
	aflt  = A->flt[src_w] >> b_dn;
	if (src_w <= ah_w) {
	  aone  |= A->one[src_w+1] << b_up;
	  azero |= A->zero[src_w+1] << b_up;
	  aflt  |= A->flt[src_w+1] << b_up;
	}

	/*
	 * "mask" is a mask of bits in R that should change in this word.
	 */
	mask = SSWORDMASK;
	if (dst_w == rl_w) mask &= HMASKZ(rl_b);
	if (dst_w == rh_w) mask &= LMASK(rh_b+1);

	rone  = (aone&mask)  | (R->one[dst_w]&~mask);
	rzero = (azero&mask) | (R->zero[dst_w]&~mask);
	rflt  = (aflt&mask)  | (R->flt[dst_w]&~mask);

	/*
	 * If any bits an R changed, update the value and record this as an edge.
	 */
	if ((rone ^ R->one[dst_w]) || (rzero ^ R->zero[dst_w]) || (rflt ^ R->flt[dst_w]) ) {
	  tt = TT_EDGE;
	  R->one[dst_w]  = rone;
	  R->zero[dst_w] = rzero;
	  R->flt[dst_w]  = rflt;
	}
      }
    }
  }
  return tt;
}

/*****************************************************************************
 *
 *  Wire merge function for "wire" and "tri"
 *
 * Parameters:
 *      R		Return value
 *      A		Wire A
 *      B		Wire B
 *
 *
 *WIRE                one                zero                flt              
 *   0 1 x z L H        0 1 x z L H        0 1 x z L H        0 1 x z L H  01z
 *  +-----------       +-----------       +-----------       +-----------  ---
 * 0|0 x x 0 0 x      0|0 1 1 0 0 1      0|1 1 1 1 1 1      0|0 1 1 0 0 1  100
 * 1|x 1 x 1 x 1      1|1 1 1 1 1 1      1|1 0 1 0 1 0      1|1 0 1 0 1 0  010
 * x|x x x x x x      x|1 1 1 1 1 1      x|1 1 1 1 1 1      x|1 1 1 1 1 1  111
 * z|0 1 x z L H      z|0 1 1 0 0 1      z|1 0 1 0 1 0      z|0 0 1 1 1 1  001
 * L|0 x x L L x      L|0 1 1 0 0 1      L|1 1 1 1 1 1      L|0 1 1 1 1 1  101
 * H|x 1 x H x H      H|1 1 1 1 1 1      H|1 0 1 0 1 0      H|1 0 1 1 1 1  011
 *
 *****************************************************************************/
void Value_wire(Value *R,Value *A,Value *B)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]|B->one[i];
    register unsigned RisZero = A->zero[i]|B->zero[i];
    register unsigned RhasFloat = (A->flt[i]&B->flt[i]);

    R->one[i]  = RisOne;
    R->zero[i] = RisZero;
    R->flt[i]  = ~(RisOne ^ RisZero) | RhasFloat;
  }
}

/*****************************************************************************
 *
 *  Wire merge function for "wand" and "triand"
 *
 * Parameters:
 *      R		Return value
 *      A		Wire A
 *      B		Wire B
 *
 *WAND/TRIAND        one                zero                flt              
 *   0 1 x z L H       0 1 x z L H        0 1 x z L H        0 1 x z L H  01z
 *  +-----------      +-----------       +-----------       +-----------  ---
 * 0|0 0 0 0 0 0     0|0 0 0 0 0 0      0|1 1 1 1 1 1      0|0 0 0 0 0 0  100
 * 1|0 1 x 1 x 1     1|0 1 1 1 1 1      1|1 0 1 0 1 0      1|0 0 1 0 1 0  010
 * x|0 x x x x x     x|0 1 1 1 1 1      x|1 1 1 1 1 1      x|0 1 1 1 1 1  111
 * z|0 1 x z L H     z|0 1 1 0 0 1      z|1 0 1 0 1 0      z|0 0 1 1 1 1  001
 * L|0 x x L L x     L|0 1 1 0 0 1      L|1 1 1 1 1 1      L|0 1 1 1 1 1  101
 * H|0 1 x H x H     H|0 1 1 1 1 1      H|1 0 1 0 1 0      H|0 0 1 1 1 1  011
 *
 */
void Value_wand(Value *R,Value *A,Value *B)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]|B->one[i];
    register unsigned RisZero = A->zero[i]|B->zero[i];
    register unsigned RhasFloat = (A->flt[i]&B->flt[i]);
    register unsigned Rzero = (~A->one[i]&A->zero[i]&~A->flt[i]) | (~B->one[i]&B->zero[i]&~B->flt[i]);

    R->one[i]  = RisOne & ~Rzero;
    R->zero[i] = RisZero;
    R->flt[i]  = (~(RisOne ^ RisZero) | RhasFloat) & ~Rzero;
  }
}

/*****************************************************************************
 *
 *  Wire merge function for "wor" and "trior"
 *
 * Parameters:
 *      R		Return value
 *      A		Wire A
 *      B		Wire B
 *
 *WOR/TRIOR          one                zero                flt              
 *   0 1 x z L H       0 1 x z L H        0 1 x z L H        0 1 x z L H  01z
 *  +-----------      +-----------       +-----------       +-----------  ---
 * 0|0 1 x 0 0 x     0|0 1 1 0 0 1      0|1 0 1 1 1 1      0|0 0 1 0 0 1  100
 * 1|1 1 1 1 1 1     1|1 1 1 1 1 1      1|0 0 0 0 0 0      1|0 0 0 0 0 0  010
 * x|x 1 x x x x     x|1 1 1 1 1 1      x|1 0 1 1 1 1      x|1 0 1 1 1 1  111
 * z|0 1 x z L H     z|0 1 1 0 0 1      z|1 0 1 0 1 0      z|0 0 1 1 1 1  001
 * L|0 1 x L L x     L|0 1 1 0 0 1      L|1 0 1 1 1 1      L|0 0 1 1 1 1  101
 * H|x 1 x H x H     H|1 1 1 1 1 1      H|1 0 1 0 1 0      H|1 0 1 1 1 1  011
 */
void Value_wor(Value *R,Value *A,Value *B)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]|B->one[i];
    register unsigned RisZero = A->zero[i]|B->zero[i];
    register unsigned RhasFloat = (A->flt[i]&B->flt[i]);
    register unsigned Rone = (A->one[i]&~A->zero[i]&~A->flt[i]) | (B->one[i]&~B->zero[i]&~B->flt[i]);

    R->one[i]  = RisOne;
    R->zero[i] = RisZero & ~Rone;
    R->flt[i]  = (~(RisOne ^ RisZero) | RhasFloat) & ~Rone;
  }
}


/*****************************************************************************
 *
 *  Wire merge function for "tri0"
 *
 * Parameters:
 *      R		Return value
 *      A		Wire A
 *      B		Wire B
 *
 *TRI0          
 *   0 1 x z L H
 *  +-----------
 * 0|0 x x 0 0 x
 * 1|x 1 x 1 x 1
 * x|x x x x x x
 * z|0 1 x 0 0 x
 * L|0 x x 0 0 x
 * H|x 1 x x x x
 */
void Value_tri0(Value *R,Value *A,Value *B)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]|B->one[i];
    register unsigned RisZero = A->zero[i]|B->zero[i];
    register unsigned RhasFloat = (A->flt[i]&B->flt[i]);
    register unsigned Rconflict = RisOne & (RisZero | RhasFloat);

    R->one[i]  = Rconflict | RisOne;
    R->zero[i] = Rconflict | ~RisOne;
    R->flt[i]  = Rconflict;
  }
}

/*****************************************************************************
 *
 *  Wire merge function for "tri1"
 *
 * Parameters:
 *      R		Return value
 *      A		Wire A
 *      B		Wire B
 *
 *TRI1          
 *   0 1 x z L H
 *  +-----------
 * 0|0 x x 0 0 x
 * 1|x 1 x 1 x 1
 * x|x x x x x x
 * z|0 1 x 1 x 1
 * L|0 x x x x x
 * H|x 1 x 1 x 1
 */
void Value_tri1(Value *R,Value *A,Value *B)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned RisOne = A->one[i]|B->one[i];
    register unsigned RisZero = A->zero[i]|B->zero[i];
    register unsigned RhasFloat = (A->flt[i]&B->flt[i]);
    register unsigned Rconflict = RisZero & (RisOne | RhasFloat);

    R->one[i]  = Rconflict | ~RisZero;
    R->zero[i] = Rconflict | RisZero;
    R->flt[i]  = Rconflict;
  }
}

/*****************************************************************************
 *
 *  Wire assignment function for trireg wires
 *
 * Parameters:
 *      R		Return value
 *      A		Driven value
 *      B		Current net value
 *
 *TRIREG
 *          (A)          one                zero                flt              
 *       0 1 x z L H       0 1 x z L H        0 1 x z L H        0 1 x z L H  01z 
 *      +-----------      +-----------       +-----------       +-----------  ---
 *     0|0 1 x 0 0 x     0|0 1 1 0 0 1      0|1 0 1 1 1 1      0|0 0 1 0 0 1  100
 *     1|0 1 x 1 x 1     1|0 1 1 1 1 1      1|1 0 1 0 1 0      1|0 0 1 0 1 0  010
 * (B) x|0 1 x x x x     x|0 1 1 1 1 1      x|1 0 1 1 1 1      x|0 0 1 1 1 1  111
 *     z|0 1 x z L H     z|0 1 1 0 0 1      z|1 0 1 0 1 0      z|0 0 1 1 1 1  001
 *     L|0 1 x L L x     L|0 1 1 0 0 1      L|1 0 1 1 1 1      L|0 0 1 1 1 1  101
 *     H|0 1 x H x H     H|0 1 1 1 1 1      H|1 0 1 0 1 0      H|0 0 1 1 1 1  011
 */
void Value_trireg(Value *R,Value *A,Value *B)
{
  int wc = SSNUMWORDS(R->nbits);
  register int i;

  for (i = 0;i < wc;i++) {
    register unsigned Aone = A->one[i];
    register unsigned Azero = A->zero[i];
    register unsigned Afloat = A->flt[i];
    register unsigned Bzero = B->zero[i];
    register unsigned Bone = B->one[i];
    register unsigned Bfloat = B->flt[i];

    R->one[i]  = (~Afloat & Aone)  | (Afloat & ~(~Bone & ~Aone));
    R->zero[i] = (~Afloat & Azero) | (Afloat & ~(~Bzero & ~Azero));
    R->flt[i]  = (Afloat & Bfloat) |  (R->one[i]&R->zero[i]);
  }
}


/*****************************************************************************
 *
 * Shift I and place the result in R.
 *
 * Parameters:
 *
 *    R		Return value from operation
 *    I		Value to be shifted
 *    n		Bits to shift (>0 for left shift, <0 for right shift)
 *    in1	Shift-in value to use in 'one' field
 *    in0	Shift-in value to use in 'zero' field
 *    inZ	Shift-in value to use in 'flt' field
 *
 *****************************************************************************/
void Value_roll(Value *R,Value *I,int n)
{
  int bits = R->nbits;
  unsigned mask = (bits<SSWORDSIZE) ? ((1<<bits)-1) : ~0;

  if (n < 0)
    n += bits;

  assert(I->nbits <= SSWORDSIZE);
  assert(n >= 0);

  if (n > 0) {
    unsigned emask = (n<SSWORDSIZE) ? ((1<<n)-1) : ~0;

    R->one[0] = (I->one[0] << n) | ((I->one[0] >> (bits-n)) & emask);
    R->zero[0] = (I->zero[0] << n) | ((I->zero[0] >> (bits-n)) & emask);
    R->flt[0] = (I->flt[0] << n) | ((I->flt[0] >> (bits-n)) & emask);
  } else {
    R->one[0] = I->one[0];
    R->zero[0] = I->zero[0];
    R->flt[0] = I->flt[0];
  }

  R->one[0] &= mask;
  R->zero[0] &= mask;
  R->flt[0] &= mask;
}


/*****************************************************************************
 *
 * Shift I and place the result in R.
 *  for nbits <= SSWORDSIZE
 *
 * Parameters:
 *
 *    R		Return value from operation
 *    I		Value to be shifted
 *    n		Bits to shift (>0 for left shift, <0 for right shift)
 *    in1	Shift-in value to use in 'one' field
 *    in0	Shift-in value to use in 'zero' field
 *    inZ	Shift-in value to use in 'flt' field
 *
 *****************************************************************************/
void Value_w_shift(Value *R,Value *I,int n,int in1,int in0,int inZ)
{
  unsigned mask = LMASK(R->nbits);

  /* special case */
  if (abs(n) >= R->nbits) {
    Value_zero(R);
    return;
  }

  if (n > 0) {
    unsigned emask = (n<SSWORDSIZE) ? ((1<<n)-1) : ~0;

    R->one[0] = (I->one[0] << n) | (in1*emask);
    R->zero[0] = (I->zero[0] << n) | (in0*emask);
    R->flt[0] = (I->flt[0] << n) | (inZ*emask);
  } else if (n < 0) {
    unsigned emask;

    n = -n;
    emask = ((n<SSWORDSIZE) ? ((1<<n)-1) : ~0) << (R->nbits-n);

    I->one[0] &= mask;
    I->zero[0] &= mask;
    I->flt[0] &= mask;

    R->one[0] = (I->one[0] >> n) | (in1*emask);
    R->zero[0] = (I->zero[0] >> n) | (in0*emask);
    R->flt[0] = (I->flt[0] >> n) | (inZ*emask);
  } else {
    R->one[0] = I->one[0];
    R->zero[0] = I->zero[0];
    R->flt[0] = I->flt[0];
  }

  R->one[0] &= mask;
  R->zero[0] &= mask;
  R->flt[0] &= mask;
}

/*****************************************************************************
 *
 * same, for nbits > SSWORDSIZE 
 *
 *****************************************************************************/
void Value_shift(Value *R,Value *I,int n,int in1,int in0,int inZ) {
  int na = abs(n);
  int shift = na & SSBITMASK;
  int ishift = SSWORDSIZE - shift;
  int wordshift = na >> SSWORDSHIFT;

  int wc = SSNUMWORDS(R->nbits);
  int i;

  /* 
     src represents the word we're copying in from
     prv is the one before that 
     if there aren't enough words there, they point to in

     for a left shift:
     destword srcword prvword

     Both start at in1/in0/inZ until 'in range' to read from I[i-wordshift], etc..
  */
  unsigned src1 = in1 ? ~0 : 0;
  unsigned src0 = in0 ? ~0 : 0;
  unsigned srcZ = inZ ? ~0 : 0;
  unsigned prv1 = src1;
  unsigned prv0 = src0;
  unsigned prvZ = srcZ;

  if (n > 0) {
    /* left shift */
    for (i = 0; i < wc; i++) {
      if (i - wordshift >= 0) {
	src1 = I->one[i - wordshift];
	src0 = I->zero[i - wordshift];
	srcZ = I->flt[i - wordshift];
      }
      if (!shift)
	prv1 = prv0 = prvZ = 0;
      else if (i - wordshift - 1 >= 0) {
	prv1 = I->one[i - wordshift - 1];
	prv0 = I->zero[i - wordshift - 1];
	prvZ = I->flt[i - wordshift - 1];
      }

      R->one[i] = (src1 << shift) | (prv1 >> ishift);
      R->zero[i] = (src0 << shift) | (prv0 >> ishift);
      R->flt[i] = (srcZ << shift) | (prvZ >> ishift);
    }

  } else if (n < 0) {
    /* right shift */
    for (i = wc-1; i >= 0; i--) {
      if (i + wordshift < wc) {
	src1 = I->one[i + wordshift];
	src0 = I->zero[i + wordshift];
	srcZ = I->flt[i + wordshift];
      }
      if (!shift)
	prv1 = prv0 = prvZ = 0;
      else if (i + wordshift + 1 < wc) {
	prv1 = I->one[i + wordshift + 1];
	prv0 = I->zero[i + wordshift + 1];
	prvZ = I->flt[i + wordshift + 1];
      }

      R->one[i] = (prv1 << ishift) | (src1 >> shift);
      R->zero[i] = (prv0 << ishift) | (src0 >> shift);
      R->flt[i] = (prvZ << ishift) | (srcZ >> shift);
    }

  } else {
    for (i=0; i<wc; i++) {
      R->one[i] = I->one[i];
      R->zero[i] = I->zero[i];
      R->flt[i] = I->flt[i];
    }
  }
}
