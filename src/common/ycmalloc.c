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
/*
   ycmalloc.cc: Temporary Memory Pool
   Last edit by hansen on Sun Jun  5 17:18:16 2005
*/
/*
   The temporary memory pool is used to store return values
   form yacc grammar rules.  Memory is allocated using the
   ycmalloc call, and all allocated memory is freed by
   a call to ycfreeall.  Generally, freeall is called after
   processing a siginificant parse unit such as a module.
   By using the memory pool we do not need to worry about
   freeing temporary values used by the parser.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "ycmalloc.h"

struct yc_pool_elem {
  struct yc_pool_elem *pe_next;		/* Next pool */
  int		      pe_istop;		/* Is this the top of a context */
  int   	      pe_size;		/* Size of this pool */
  int                 pe_last;		/* One byte past last used byte */
  char		      *pe_data;		/* Data buffer of pool */
};

static struct yc_pool_elem *yc_pool = 0;	/* Current pool */

void yc_growpool()
{
  struct yc_pool_elem *P = (struct yc_pool_elem*) malloc(sizeof(struct yc_pool_elem));

  P->pe_next = yc_pool;
  P->pe_size = YC_POOL_SIZE;
  P->pe_last = 0;
  P->pe_istop = 0;
  P->pe_data = (char*) malloc(YC_POOL_SIZE);

  yc_pool = P;
}

void yc_pushpool()
{
  yc_growpool();
  yc_pool->pe_istop = 1;
}

/*
   Put a distinctive pattern into the pool so that any objects
   we forgot to copy will be obvious.
*/
void yc_poppool()
{
  long i;
  struct yc_pool_elem *D = 0;

  while (yc_pool) {
    D = yc_pool;
    yc_pool = yc_pool->pe_next;

    for (i = 0;i < YC_POOL_SIZE-3;i += 4)
      *((long*)&D->pe_data[i]) = 0xdeadf001;
    free(D->pe_data);
    free(D);

    if (yc_pool && yc_pool->pe_istop) break;
  }
}

void yc_setup()
{
  if (!yc_pool) yc_pushpool();
}

void yc_freeall()
{
  if (yc_pool) {
    yc_poppool();
    yc_pushpool();
  }
}

char *yc_strdup(const char *S)
{
  long L;
  char *N;

  if (!S) return 0;
  L = strlen(S);
  N = (char*)yc_malloc(L+1,ALGN_BYTE);
  strcpy(N,S);
  return N;
}

void *yc_malloc(long L,int Algn)
{
  char *P;
  int Remaining = yc_pool->pe_size - yc_pool->pe_last;	/* Bytes remaining in pool */
  assert(L < YC_POOL_SIZE);

  if (Algn + L > Remaining)
    yc_growpool();

  /* Align offset to next Algn boundary */
  yc_pool->pe_last = ((yc_pool->pe_last-1)|(Algn-1))+1;

  P = &yc_pool->pe_data[yc_pool->pe_last];
  yc_pool->pe_last += L;

  return P;
}

void *yc_calloc(long N,long S,int Algn)
{
  long L = N*S;
  char *P;

  P = yc_malloc(L,Algn);
  memset(P,0,L);

  return P;
}

void *yc_realloc(void *P,long oldL,long L,int Algn)
{
  void *Q = yc_malloc(L,Algn);

  memmove(Q,P,oldL);

  return Q;
}

char *yc_sprintf(char *s,...)
{
  va_list ap;
  char buf[1024];

  va_start(ap,s);
  vsprintf(buf,s,ap);
  va_end(ap);

  return yc_strdup(buf);
}
