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

    Last edit by hansen on Mon Jun 20 15:16:07 2005
****************************************************************************/
#ifndef __multint_h
#define __multint_h

int multint_cvtstr(unsigned *x,int n,const char *buf);
int multint_getstr(unsigned *x,int n,char *buf,int len);
unsigned multint_div10(unsigned *x,int n);
int multint_iszero(unsigned *x,int n);
void multint_mul(unsigned *x,unsigned *a,unsigned *b,int n);
int multint_div(unsigned *x,unsigned *a,unsigned *b,int n);
int multint_mod(unsigned *x,unsigned *a,unsigned *b,int n);

#endif
