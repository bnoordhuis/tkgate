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
#ifndef __ycmalloc_h
#define __ycmalloc_h

/*
   Allignment type for memory allocation
*/
#define ALGN_BYTE	sizeof(char)		/* Byte alligned allocation */
#define ALGN_SHORT	sizeof(short)		/* Short word alligned allocation */
#define ALGN_WORD	sizeof(int)		/* Standard word alligned allocation */
#define ALGN_LONG	sizeof(long)		/* Long word alligned allocation */
#define ALGN_FLOAT	sizeof(float)		/* Double word alligned allocation */
#define ALGN_DOUBLE	sizeof(double)		/* Double word alligned allocation */

#ifndef YC_POOL_SIZE
#define YC_POOL_SIZE 16384		/* Block size for malloc pool */
#endif

/*
  Management functions
*/
void yc_setup();			/* Guarantee at least one memory pool on stack */
void yc_freeall();			/* Flush the top memory pool on stack */
void yc_pushpool();			/* Create a new memory temporary memory area */
void yc_poppool();			/* Delete the top temporary memory area */

/*
  Allocation functions
*/
char *yc_strdup(const char *S);		/* Duplicate string on temporary memory pool */
void *yc_malloc(long L,int A);		/* Allocate memory on temporary memory pool */
char *yc_sprintf(char *s,...);		/* Concatenate a bunch of strings */
void *yc_calloc(long N,long S,int Algn);
void *yc_realloc(void *P,long oldL,long L,int Algn);

/*
   Allocate memory for an object of type T from the yc temporary pool
*/
#define ycnew(T) (T*) yc_malloc(sizeof(T),ALGN_WORD)
#define ycvnew(T,n) (T*) yc_malloc((n)*sizeof(T),ALGN_WORD)

#endif
