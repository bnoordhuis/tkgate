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

    Last edit by hansen on Fri Feb 13 20:25:50 2009
****************************************************************************/
#include "thyme.h"

extern Place curPlace;			/* The current place */

Directive directive = {0};

Timescale currentTimescale = {0,0};

static char *unitCodes[] = {"s", "ms", "us", "ns", "ps", "fs"};
static int numUnitCodes  = sizeof(unitCodes)/sizeof(unitCodes[0]);

int Timescale_getUnitCode(const char *units)
{
  int i;

  for (i = 0;i < numUnitCodes;i++)
    if (strcmp(units,unitCodes[i])) return i;

  /*
   * Unit code was invalid, get unit code for nanoseconds instead.
   */
  return Timescale_getUnitCode("ns");
}

/*
 * Get number of digits in a simtime value (which must be a multiple of 10)
 */
int Timescale_getDigits(simtime_t t)
{
  int d = 0;
  while (t > 1) {
    t /= 10;
    d++;
  }
  return d;
}

void Timescale_decode(simtime_t n, int *num,char *units)
{
  int index = 0;

  while (n >= 1000) {
    n /= 1000;
    index++;
  }

  if (index >= numUnitCodes || (n != 1 && n != 10 && n != 100)) {
    /* Invalid code, so just default to 1ns */
    *num = 1;
    strcpy(units,"ns");
    return;
  }


  strcpy(units,unitCodes[numUnitCodes-1-index]);
  *num = n;
}

simtime_t Directive_parseTimescale(int num,const char *units)
{
  simtime_t U = 1;
  int i;

  for (i = numUnitCodes-1;i >= 0;i--) {
    if (strcmp(units, unitCodes[i]) == 0) break;
    U *= 1000;
  }

  if (i < 0) {
    errorFile(&curPlace,ERR_TIMESCALEU,units);
    return 0;
  }

  if (num != 1 && num != 10 && num != 100) {
    char numS[STRMAX];
    sprintf(numS,"%d",num);
    errorFile(&curPlace,ERR_TIMESCALEN,numS);
    return 0;
  }

  U *= num;

  return U;
}

/*
 * Convert a time with units to a simtime according to the specified timescale.
 */
simtime_t Timescale_toSimtime(Timescale *ts,double n,const char *units)
{
  simtime_t unitTime = Directive_parseTimescale(1,units);
  int unitDigits = Timescale_getDigits(unitTime);
  int tsDigits = Timescale_getDigits(ts->ts_precision);
  int d = unitDigits-tsDigits;
  int i;

  while (d < 0) {
    d++;
    n /= 10.0;
  }
  while (d > 0) {
    d--;
    n *= 10.0;
  }

  return (simtime_t)n;
}


void Directive_setTimescale(int n1,char *b1,int n2,char *b2)
{
  currentTimescale.ts_units = Directive_parseTimescale(n1,b1);
  currentTimescale.ts_precision = Directive_parseTimescale(n2,b2);

  if (currentTimescale.ts_precision > currentTimescale.ts_units)
    errorFile(&curPlace,ERR_TIMESCALEX);
}

char *Directive_findMacro(char *token)
{
  if (!directive.d_macro_table) directive.d_macro_table = new_SHash();
  return (char*)SHash_find(directive.d_macro_table,token);
}

void Directive_defineMacro(char *token,char *replacement)
{
  if (!directive.d_macro_table) directive.d_macro_table = new_SHash();
  SHash_insert(directive.d_macro_table,token,strdup(replacement));
}

int Directive_macroReplace(char *token)
{
  char *replacement;

  replacement = Directive_findMacro(token+1);
  if (replacement)
    yc_unputstring(replacement);

  return -1;
}

int Directive_handle(char *directive,int inSkip)
{
  char b1[STRMAX],b2[STRMAX];
  char name[STRMAX];
  int n1,n2;
  int state = 1;  /* 0 = DS, 1=PV */

  /*
   * ignore non-directive lines
   */
  if (sscanf(directive," `%s",name) != 1) return -1;

  if (sscanf(directive," `define %s %[^\n]",b1,b2) == 2) {
    Directive_defineMacro(b1,b2);
  } else if (sscanf(directive," `timescale %d %[^/ \t] / %d %s",&n1,b1,&n2,b2) == 4) {
    Directive_setTimescale(n1,b1,n2,b2);
  } else if (sscanf(directive," `ifdef %s",b1) == 1) {
    state = Directive_iftoken(CD_IF, Directive_findMacro(b1) != 0);
  } else if (sscanf(directive," `ifndef %s",b1) == 1) {
    state = Directive_iftoken(CD_IF, Directive_findMacro(b1) == 0);
  } else if (sscanf(directive," `elseif %s",b1) == 1) {
    state = Directive_iftoken(CD_ELSEIF, Directive_findMacro(b1) != 0);
  } else if (strcmp(name,"else") == 0) {
    state = Directive_iftoken(CD_ELSE, 0);
  } else if (strcmp(name,"endif") == 0) {
    state = Directive_iftoken(CD_ENDIF, 0);
  } else if (strcmp(name,"timescale") == 0) {
    errorFile(&curPlace,ERR_TIMESCALES);
  }

  if (state) {
    BeginPV();
  } else {
    BeginDS();
  }

  return -1;
}

/*****************************************************************************
 *
 * Process an ifdef token and return 1 if we should be in "normal" mode.
 *
 * Parameters:
 *      code		Code for the type of ifdef token
 *      state		State of the if test (only for ifdef/ifndef and elseif)
 *
 *****************************************************************************/
int Directive_iftoken(int code,int state)
{
  int index = directive.d_ifdefLen - 1;

  if (directive.d_ifdefLen >= directive.d_ifdefAlloced) {
    directive.d_ifdefAlloced += 32;
    if (!directive.d_ifdefStack)
      directive.d_ifdefStack = (int*)malloc(sizeof(int)*directive.d_ifdefAlloced);
    else
      directive.d_ifdefStack = (int*)realloc(directive.d_ifdefStack,sizeof(int)*directive.d_ifdefAlloced);
  }

  switch (code) {
  case CD_ELSEIF :
    if (index < 0 || (directive.d_ifdefStack[index] != CD_IF && directive.d_ifdefStack[index] != CD_IFDEFX)) {
      errorFile(&curPlace,ERR_NOIFDEF,"`elseif");
      break;
    }

    if (directive.d_ifdefInSkip) {
      if (index > directive.d_ifdefSkipLevel)
	break;
      directive.d_ifdefInSkip = 0;
    } else {
      directive.d_ifdefStack[index] = CD_IFDEFX;
      directive.d_ifdefInSkip = 1;
      directive.d_ifdefSkipLevel = directive.d_ifdefLen-1;
      break;
    }

    if (directive.d_ifdefStack[index] == CD_IFDEFX || !state) {
      directive.d_ifdefInSkip = 1;
      directive.d_ifdefSkipLevel = directive.d_ifdefLen-1;
    } else {
      directive.d_ifdefStack[index] = CD_IFDEFX;
    }


    break;
  case CD_IF :
    directive.d_ifdefStack[directive.d_ifdefLen++] = CD_IF;
    if (directive.d_ifdefInSkip)
      break;

    if (!state) {
      directive.d_ifdefInSkip = 1;
      directive.d_ifdefSkipLevel = directive.d_ifdefLen-1;
    }
    break;
  case CD_ELSE :
    if (index < 0 || (directive.d_ifdefStack[index] != CD_IF && directive.d_ifdefStack[index] != CD_IFDEFX)) {
      errorFile(&curPlace,ERR_NOIFDEF,"`else");
      break;
    }

    if (directive.d_ifdefInSkip) {
      if (index > directive.d_ifdefSkipLevel)
	break;

      if (directive.d_ifdefStack[index] != CD_IFDEFX)
	directive.d_ifdefInSkip = 0;
    } else {
      directive.d_ifdefInSkip = 1;
      directive.d_ifdefSkipLevel = directive.d_ifdefLen-1;
    }

    directive.d_ifdefStack[index] = CD_ELSE;

    break;
  case CD_ENDIF :
    if (index < 0) {
      errorFile(&curPlace,ERR_NOIFDEF,"`endif");
      break;
    }
    directive.d_ifdefLen--;

    if (directive.d_ifdefInSkip) {
      if (index > directive.d_ifdefSkipLevel)
	break;

      directive.d_ifdefInSkip = 0;
    }

    break;
  }

  return !directive.d_ifdefInSkip;
}
