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

    Last edit by hansen on Wed Sep 14 23:00:33 2005
****************************************************************************/
#include "thyme.h"
#include <ctype.h>

/*****************************************************************************
 *
 * Helping function for divmnu (taken from Hackers Delight)
 *
 *****************************************************************************/
static int nlz(unsigned x) {
   int n;

   if (x == 0) return(32);
   n = 0;
   if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
   if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
   if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
   if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
   if (x <= 0x7FFFFFFF) {n = n + 1;}
   return n;
}

/******************************************************************************
 *
 * Core function for division and remainder computation (taken from Hackers Delight)
 *
 * Parameters:
 *      q		Quotient return, size m-n+1
 *      r		Remainder return, size n
 *      u		Dividend (top of fraction), size m
 *      v		Divisor (bottom of fraction), size n
 *      m		Number of half-words in dividend u
 *      n		Number of half-words in divisor v
 *
 * q[0], r[0], u[0], and v[0] contain the LEAST significant halfwords.
 * (The sequence is in little-endian order).
 * This first version is a fairly precise implementation of Knuth's
 * Algorithm D, for a binary computer with base b = 2**16.  The caller
 * supplies
 *   1. Space q for the quotient, m - n + 1 halfwords (at least one).
 *   2. Space r for the remainder (optional), n halfwords.
 *   3. The dividend u, m halfwords, m >= 1.
 *   4. The divisor v, n halfwords, n >= 2.
 * The most significant digit of the divisor, v[n-1], must be nonzero.  The
 * dividend u may have leading zeros; this just makes the algorithm take
 * longer and makes the quotient contain more leading zeros.  A value of
 * NULL may be given for the address of the remainder to signify that the
 * caller does not want the remainder.
 * The program does not alter the input parameters u and v.
 * The quotient and remainder returned may have leading zeros.  The
 * function itself returns a value of 0 for success and 1 for invalid
 * parameters (e.g., division by 0).
 * For now, we must have m >= n.  Knuth's Algorithm D also requires
 * that the dividend be at least as long as the divisor.  (In his terms,
 * m >= 0 (unstated).  Therefore m+n >= n.)
 *
 *****************************************************************************/

static int divmnu(unsigned short q[], unsigned short r[],
     const unsigned short u[], const unsigned short v[],
     int m, int n) {

   const unsigned b = 65536; // Number base (16 bits).
   unsigned short *un, *vn;  // Normalized form of u, v.
   unsigned qhat;            // Estimated quotient digit.
   unsigned rhat;            // A remainder.
   unsigned p;               // Product of two digits.
   int s, i, j, t, k;

   if (m < n || n <= 0 || v[n-1] == 0)
      return 1;              // Return if invalid param.

   if (n == 1) {                        // Take care of
      k = 0;                            // the case of a
      for (j = m - 1; j >= 0; j--) {    // single-digit
         q[j] = (k*b + u[j])/v[0];      // divisor here.
         k = (k*b + u[j]) - q[j]*v[0];
      }
      if (r != NULL) r[0] = k;
      return 0;
   }

   // Normalize by shifting v left just enough so that
   // its high-order bit is on, and shift u left the
   // same amount.  We may have to append a high-order
   // digit on the dividend; we do that unconditionally.

   s = nlz(v[n-1]) - 16;        // 0 <= s <= 16.
   vn = (unsigned short *)alloca(2*n);
   for (i = n - 1; i > 0; i--)
      vn[i] = (v[i] << s) | (v[i-1] >> (16-s));
   vn[0] = v[0] << s;

   un = (unsigned short *)alloca(2*(m + 1));
   un[m] = u[m-1] >> (16-s);
   for (i = m - 1; i > 0; i--)
      un[i] = (u[i] << s) | (u[i-1] >> (16-s));
   un[0] = u[0] << s;

   for (j = m - n; j >= 0; j--) {       // Main loop.
      // Compute estimate qhat of q[j].
      qhat = (un[j+n]*b + un[j+n-1])/vn[n-1];
      rhat = (un[j+n]*b + un[j+n-1]) - qhat*vn[n-1];
   again:
      if (qhat >= b || qhat*vn[n-2] > b*rhat + un[j+n-2]) {
	qhat = qhat - 1;
        rhat = rhat + vn[n-1];
        if (rhat < b) goto again;
      }

      // Multiply and subtract.
      k = 0;
      for (i = 0; i < n; i++) {
         p = qhat*vn[i];
         t = un[i+j] - k - (p & 0xFFFF);
         un[i+j] = t;
         k = (p >> 16) - (t >> 16);
      }
      t = un[j+n] - k;
      un[j+n] = t;

      q[j] = qhat;              // Store quotient digit.
      if (t < 0) {              // If we subtracted too
         q[j] = q[j] - 1;       // much, add back.
         k = 0;
         for (i = 0; i < n; i++) {
            t = un[i+j] + vn[i] + k;
            un[i+j] = t;
            k = t >> 16;
         }
         un[j+n] = un[j+n] + k;
      }
   } // End j.
   // If the caller wants the remainder, unnormalize
   // it and pass it back.
   if (r != NULL) {
      for (i = 0; i < n; i++)
         r[i] = (un[i] >> s) | (un[i+1] << (16-s));
   }
   return 0;
}


/*****************************************************************************
 *
 * Test multi-word int for zero.
 *
 * Parameters:
 *      x		Array of int word parts.
 *      n		Number of word parts.
 *
 *****************************************************************************/
int multint_iszero(unsigned *x,int n)
{
  int i;

  for (i = 0;i < n;i++)
    if (x[i] != 0) return 0;
  return 1;
}

void multint_makezero(unsigned *x,int n)
{
  int i;

  for (i = 0;i < n;i++)
    x[i] = 0;
}

void multint_copy(unsigned *x,unsigned *a,int n)
{
  int i;

  for (i = 0;i < n;i++)
    x[i] = a[i];
}

/*****************************************************************************
 *
 * Divide multi-word int by ten and return the remainder.
 *
 * Parameters:
 *      x		Array of int word parts.
 *      n		Number of word parts.
 *
 *****************************************************************************/
unsigned multint_div10(unsigned *x,int n)
{
  const unsigned K1 = 429496729;	/* 2^32 / 10 */
  const unsigned K2 = 6;		/* 2^32 % 10 */
  unsigned r,ry,rx;
  unsigned delta;

  r = x[0] % 10;
  x[0] /= 10;

  if (n == 1)
    return r;

  ry = multint_div10(x+1,n-1);
  rx = K2*ry;
  delta = K1*ry + (rx + r) / 10;

  x[0] += delta;

  r = (rx + r) % 10;

  return r;
}

/*****************************************************************************
 *
 * Multiply multi-word int by ten
 *
 * Parameters:
 *      x		Array of int word parts.
 *      n		Number of word parts.
 *
 *****************************************************************************/
void multint_mul10(unsigned *x,int n)
{
  int i;
  unsigned carry = 0;
  unsigned xcarry = 0;
  unsigned nextcarry;

  /* 1010*/

  //SSWORDSIZE

  for (i = 0;i < n;i++) {
    unsigned Xh = (x[i] >> (SSWORDSIZE/2)) & SSHALFWORDMASK;
    unsigned Xl =  x[i] & SSHALFWORDMASK;
    unsigned P1 = Xh*10;
    unsigned P3 = Xl*10 >> (SSWORDSIZE/2);

    xcarry = 0;
    nextcarry = P1;
    if (nextcarry + P3 < nextcarry) xcarry++;
    nextcarry += P3;
    nextcarry = (nextcarry >> (SSWORDSIZE/2)) + xcarry;
    
    x[i] = x[i] * 10 + carry;
    carry = nextcarry;
  }
}

/*****************************************************************************
 *
 * Convert multi-word int to string
 *
 * Parameters:
 *      x		Array of integers with word parts
 *      n		Number  of elements in array
 *      buf		Buffer to write string to
 *      len		Size of string buffer
 *
 *
 *****************************************************************************/
int multint_getstr(unsigned *x,int n,char *buf,int len)
{
  unsigned *v;
  char *p = buf;
  char *q;
  int i;

  v = (unsigned*)malloc(sizeof(unsigned)*n);
  for (i = 0;i < n;i++)
    v[i] = x[i];

  if (multint_iszero(x,n)) {
    sprintf(buf,"0");
    return 0;
  }

  while (!multint_iszero(v,n)) {
    *p++ = multint_div10(v,n) + '0';
    if (--len <= 0) return -1;		/* Not enough space */
  }
  *p-- = 0;
  q = buf;
  while (q < p) {
    char t = *q;
    *q = *p;
    *p = t;
    q++; p--;
  }

  free(v);

  return 0;
}

/*****************************************************************************
 *
 * Divide a multi-word integer by another multi-word integer.
 *
 * Parameters:
 *      x		Quotient return
 *      a		Operand 1
 *      b		Operand 2
 *      l		Number of array elements
 *
 *****************************************************************************/
int multint_div(unsigned *x,unsigned *a,unsigned *b,int l)
{
  static int allocLen = 0;
  static unsigned short *q = 0,*u = 0,*v = 0;
  int numParts = sizeof(unsigned)/2;
  int m,n,i,j,k;

  /*
   * division code assumes 16-bit shorts.
   */
  if (sizeof(unsigned short) != 2) {
    /* error: unsupported architecture */
    return -2;
  }

  /*
   * Reallocate temp arrays if necessary
   */
  if (allocLen < l*numParts) {
    allocLen = l*numParts;
    if (q) free(q);
    if (u) free(u);
    if (v) free(v);
    q = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
    u = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
    v = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
  }

  m = l*numParts;
  n = l*numParts;

  k = 0;
  for (i = 0;i < l;i++) {
    unsigned aword = a[i];
    unsigned bword = b[i];

    for (j = 0;j < numParts;j++) {
      u[k] = aword & 0xffff;
      v[k] = bword & 0xffff;
      aword >>= 16;
      bword >>= 16;
      k++;
    }
  }

  /*
   * Trim off leading zeros from dividend.  If all words are zero, then we have a divide by zero error.
   */
  while (n > 0 && v[n-1] == 0) n--;
  if (n == 0) {
    /* error: divide by zero */
    multint_makezero(x,l);
    return -1;
  }

  /*
   * Trim off leading zeros from dividend.  If all words are zero, then the result is zero.
   */
  while (m > 0 && u[m-1] == 0) m--;
  if (m == 0) {
    multint_makezero(x,l);
    return 0;
  }

  /*
   * If divisor is bigger than the dividend, then the quotient is zero.
   */
  if (n > m) {
    multint_makezero(x,l);
    return 0;
  }

  /*
   * Do the division operation
   */
  divmnu(q,0,u,v,m,n);

  /*
   * Compute result size
   */
  m = m-n+1;

  k = 0;
  for (i = 0;i < l;i++) {
    x[i] = 0;
    for (j = 0;j < numParts;j++) {
      if (k < m)
	x[i] |= q[k++] << 16*j;
    }
  }

  return 0;
}

/*****************************************************************************
 *
 * Find remainder from dividing one multi-word integer by another multi-word integer.
 *
 * Parameters:
 *      x		Remainder return
 *      a		Operand 1
 *      b		Operand 2
 *      n		Number of array elements
 *
 *****************************************************************************/
int multint_mod(unsigned *x,unsigned *a,unsigned *b,int l)
{
  static int allocLen = 0;
  static unsigned short *q = 0,*r = 0,*u = 0,*v = 0;
  int numParts = sizeof(unsigned)/2;
  int m,n,i,j,k;

  /*
   * division code assumes 16-bit shorts.
   */
  if (sizeof(unsigned short) != 2) {
    /* error: unsupported architecture */
    return -2;
  }

  /*
   * Reallocate temp arrays if necessary
   */
  if (allocLen < l*numParts) {
    allocLen = l*numParts;
    if (q) free(q);
    if (r) free(r);
    if (u) free(u);
    if (v) free(v);
    q = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
    r = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
    u = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
    v = (unsigned short*) malloc(sizeof(unsigned short)*allocLen);
  }

  m = l*numParts;
  n = l*numParts;

  k = 0;
  for (i = 0;i < l;i++) {
    unsigned aword = a[i];
    unsigned bword = b[i];

    for (j = 0;j < numParts;j++) {
      u[k] = aword & 0xffff;
      v[k] = bword & 0xffff;
      aword >>= 16;
      bword >>= 16;
      k++;
    }
  }

  /*
   * Trim off leading zeros from dividend.  If all words are zero, then we have a divide by zero error.
   */
  while (n > 0 && v[n-1] == 0) n--;
  if (n == 0) {
    /* error: divide by zero */
    multint_makezero(x,l);
    return -1;
  }

  /*
   * Trim off leading zeros from dividend.  If all words are zero, then the result is zero.
   */
  while (m > 0 && u[m-1] == 0) m--;
  if (m == 0) {
    multint_makezero(x,l);
    return 0;
  }

  /*
   * If divisor is bigger than the dividend, then the quotient is zero.
   */
  if (n > m) {
    multint_copy(x,a,l);
    return 0;
  }

  /*
   * Do the division operation
   */
  divmnu(q,r,u,v,m,n);

  k = 0;
  for (i = 0;i < l;i++) {
    x[i] = 0;
    for (j = 0;j < numParts;j++) {
      if (k < n)
	x[i] |= r[k++] << 16*j;
    }
  }

  return 0;
}

/*****************************************************************************
 *
 * Multiply two multi-word integers.
 *
 * Parameters:
 *      x		Product return
 *      a		Operand 1
 *      b		Operand 2
 *      n		Number of array elements
 *
 *****************************************************************************/
void multint_mul(unsigned *x,unsigned *a,unsigned *b,int n)
{
  int i,j,k;

  for (i = 0;i < n;i++)
    x[i] = 0;

  for (i = 0;i < n;i++) {
    for (j = 0;j < n;j++) {
      unsigned pp[2];
      unsigned carry;
      unsigned Ah,Al,Bh,Bl,P1,P2,P3;

      if (i+j >= n) continue;

      /*
       * If either of the words is zero, then the partial product will be
       * zero and there is no sense wasting our time on it.
       */
      if (a[i] == 0 || b[j] == 0) continue;

      /*
       * Use regular multiply to get low part of partial product
       */
      pp[0] = a[i]*b[j];


      /*
       * Split words into high and low halves.
       */
      Ah = (a[i] >> (SSWORDSIZE/2)) & SSHALFWORDMASK;
      Al =  a[i] & SSHALFWORDMASK;
      Bh = (b[j] >> (SSWORDSIZE/2)) & SSHALFWORDMASK;
      Bl =  b[j] & SSHALFWORDMASK;

      P1 = Ah*Bl;
      P2 = Al*Bh;
      P3 = Al*Bl >> (SSWORDSIZE/2);

      carry = 0;
      pp[1] = P1;
      if (pp[1] + P2 < pp[1]) carry++;
      pp[1] += P2;
      if (pp[1] + P3 < pp[1]) carry++;
      pp[1] += P3;

      pp[1] = (pp[1] >> (SSWORDSIZE/2)) + carry;
      pp[1] += Ah*Bh;

      k = i+j;
      pp[1] += ((x[k] + pp[0]) < x[k]);
      x[k] += pp[0];

      k++;
      if (k < n) {
	carry = ((x[k] + pp[1]) < x[k]);
	x[k] += pp[1];
      }

      for (k++;k < n;k++) {
	x[k] += carry;
	if (x[k] != 0) break;
      }
    }
  }
}

/*****************************************************************************
 *
 * Convert stream of decimal digits into an integer
 *
 *****************************************************************************/
int multint_cvtstr(unsigned *x,int n,const char *p)
{
  multint_makezero(x,n);

  for (;*p;p++) {
    if (!isdigit(*p)) return -1;
    multint_mul10(x,n);
    x[0] += *p - '0';
  }

  return 0;
}
