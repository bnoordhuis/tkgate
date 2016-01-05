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
   yybasic.h: Basic definitions used only by grammar and lexicon.
   This is a C header file.
*/
#ifndef __yybasic_h
#define __yybasic_h

extern int ycLineNumber;
extern char *ycFileName;

struct lex_keywordentry {
    char *Keyword;
    int Value;
};


typedef union {
  int I;			/* Used for integers */
  float F;			/* Used for floats */
  char *S;			/* Used for strings */
  void *P;			/* Generic pointer */ 
  VRange *R;			/* Bit range (or address range) */
  Expr *E;			/* Expression */
  StatDecl *SD;			/* Statement declaration */
  List *L;			/* Generic list */
  SDCaseEnt *CE;		/* Case entry */
} YYVALUE;

#define YYSTYPE YYVALUE
extern YYVALUE yylval;


extern int ycLineNumber;

void BeginPV();
void BeginSF();
void BeginBC();
void BeginLC();
void BeginCD();
void BeginDS();
void BeginLast();
void SaveLast();

int ycHex(char *S);
int ycNumber(char *S);
int ycFloat(char *S);
int ycString(char *S);
int ycLiteral(char *Tok,struct lex_keywordentry *low,int len);
int ycDirective(char *token);
int ycDirectiveText(char *token);

int yyparse();
int yylex();
int yyerror(char*);

int ycIsKW(char *Tok);
const char *ycGetKeyword(int tokenCode);

void yc_unputstring(char *s);

int yc_bogochar();
void yc_lexjunk();

#endif


