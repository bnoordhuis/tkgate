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

    Last edit by hansen on Fri Feb 13 20:26:14 2009
****************************************************************************/
#ifndef __directive_h
#define __directive_h

/*
 * Codes for ifdef processing
 */
#define CD_IF		0
#define CD_ELSE		1
#define CD_ELSEIF	2
#define CD_ENDIF	3
#define CD_IFDEFX	4

struct Timescale_str {
  simtime_t	ts_units;
  simtime_t	ts_precision;
};

/*
 * State information for processing compiler directives
 */
typedef struct {
  SHash		*d_macro_table;		/* Table of defined macros */
  int		*d_ifdefStack;		/* Stack with the current if/else processing state */
  int		d_ifdefLen;		/* Length of the ifdef stack */
  int		d_ifdefAlloced;		/* Size of the ifdef stack array */
  int		d_ifdefSkipLevel;	/* Level of ifdef that is being skipped */
  int		d_ifdefInSkip;		/* Non-zero if we are "skipping" text */
} Directive;

int Directive_handle(char *directive,int inSkip);
int Directive_macroReplace(char *token);
int Directive_iftoken(int code, int state); 
simtime_t Directive_parseTimescale(int num,const char *units);
void Timescale_decode(simtime_t n, int *num,char *units);
simtime_t Timescale_toSimtime(Timescale *ts,double n,const char *units);

extern Timescale currentTimescale;

#endif
