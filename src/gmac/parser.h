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
#ifndef __parser_h
#define __parser_h
#include <stdio.h>

#define MAXREGS	1024
#define MAXOPRS 64

#define NT_VALUE	0
#define NT_SYMBOL	1
#define NT_RELSYMBOL	2

#define AM_IMMEDIATE	0
#define AM_DIRECT	1
#define AM_INDIRECT	2
#define AM_INDEXED	3
#define AM_IMMIND	4

#define OI_DATA		0
#define OI_REGOP	1
#define OI_NUMOP	2
#define OI_RELNUMOP	3


struct lex_keywordentry {
    char *Keyword;
    int Value;
};

typedef struct range {
  int msb;
  int lsb;
} Range;

typedef struct number {
  short ntype;
  short offset;
  union {
    char *s;
    int d;
  } v; 
} Number;


typedef union {
  int I;			/* Used for integers */
  char *S;			/* Used for strings */
  void *P;			/* Used for pointers */
  Range R;			/* Used for ranges */
  Number N;			/* Used for numbers which can be symbolic */
} YYVALUE;

#define YYSTYPE YYVALUE

extern YYVALUE yylval;

extern char ycFileName[];
extern int ycLineNumber;

void Number_print(Number *N,FILE *f);
Number *Number_copy(Number*);

void ycRegRecord(char*,int);
void ycBank(int,Range*,char*,Range*);
void ycField(char*,Range*,int);
void ycEnum(char*,int);
void ycBeginOprGroup(char*);
void ycEndOprGroup();
void ycBeginOperand();
void ycEndOperand();
void ycOLHS(int,int,int);
void ycORHS(int,int,Range*,int,Range*,int);
void ycBeginOp(char*);
void ycEndOp();
void ycAddOprGroup(char*);
void ycMap(struct number*,struct number*);
void ycBeginUCode(int);
void ycEndUCode();
void ycUNext();
void ycULabel(char*);
void ycUSpec(struct number *,struct number *);
void ycBeginMCode(int);
void ycEndMCode();
void ycMOp(char*);
void ycMNext();
void ycMLabel(char*);
void ycMOperand(int,struct number*,int);
void ycMSymbol(char*,int);
void ycBss(int);
void ycData(int);
void ycDataNum(int);
void ycDataLit(char*);
void ycDataStr(char*);
void ycBeginProc(char*);
void ycEndProc();

int yyerror(char*,...);
int yyparse();
int yylex();
void yyrestart(FILE*);

int ycKeyCmp(const char *S1,const char *S2);

void BeginBA();
void BeginMA();

#endif
