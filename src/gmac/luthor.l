%{
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
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "parser.h"
#include "grammar.h"

#if SUPPRESS_FLEX_ERRORS
#define FLEX_FIX_STAT if (0) goto find_rule;
#else
#define FLEX_FIX_STAT
#endif


struct lex_keywordentry lex_words[] = {
  {".bss",	DBSS }, 
  {".byte",	DBYTE }, 
  {".end",	DEND }, 
  {".long",	DLONG }, 
  {".proc",	DPROC }, 
  {".short",	DSHORT }, 
  {".symbol",	DSYMBOL }, 
  {"bank",	BANK }, 
  {"begin",	KWBEGIN }, 
  {"end",	KWEND }, 
  {"field",	FIELD }, 
  {"macrocode",	MACROCODE }, 
  {"map",	MAP }, 
  {"microcode",	MICROCODE }, 
  {"op",	OP }, 
  {"operands",	OPERANDS }, 
  {"registers",	REGISTERS } 
};
int num_lex_words = sizeof(lex_words)/sizeof(lex_words[0]);

struct lex_keywordentry lex_regs[MAXREGS];
int num_lex_regs = 0;

int ycString(char *S);
int ycLiteral(char *Tok);

%}

white	[ \t]
num	[0-9]
hex	[0-9A-Fa-f]
lit1	[$A-Za-z_.]
lit2	[$A-Za-z0-9_.]

%start BA MA

%%

<BA,MA>"//".*			{/* comment */ }
<BA,MA>";"			{ return SEMI; }
<BA,MA>":"			{ return COLON; }
<BA,MA>","			{ return COMMA; }
<BA,MA>"="			{ return ASGN; }
<BA,MA>"#"			{ return HASH; }
<BA,MA>"~"			{ return TILDE; }
<BA,MA>"%"			{ return PERCENT; }
<BA,MA>"+"			{ return PLUS; }
<BA,MA>"@"			{ return AT; }
<BA,MA>"-"			{ return MINUS; }

<BA,MA>"("			{ return LPAREN; }
<BA,MA>")"			{ return RPAREN; }
<BA,MA>"["			{ return LBRACK; }
<BA,MA>"]"			{ return RBRACK; }
<BA,MA>"{"			{ return LBRACE; }
<BA,MA>"}"			{ return RBRACE; }

<BA,MA>\"([^\\\"]*("\\".)*)*\"	{ return ycString(yytext); }
<BA,MA>"-"?{num}+		{ sscanf(yytext,"%d",&yylval.I); return NUMBER; }
<BA,MA>"'"."'"			{ unsigned char c; sscanf(yytext,"'%c'",&c); yylval.I = c; return NUMBER; }
<BA,MA>"'\\n'"			{ yylval.I = '\n'; return NUMBER; }
<BA,MA>"'\\r'"			{ yylval.I = '\r'; return NUMBER; }
<BA,MA>"'\\b'"			{ yylval.I = '\b'; return NUMBER; }
<BA,MA>"0x"{hex}+		{ sscanf(yytext,"0x%x",&yylval.I); return NUMBER; }
<BA,MA>{lit1}{lit2}*		{ return ycLiteral(yytext); }
<BA,MA>{white}+			{ }
<BA>"\n"			{ ycLineNumber++; }
<MA>"\n"			{ ycLineNumber++; return NL; }
<BA,MA>.			{ FLEX_FIX_STAT yyerror("Illegal character (ascii 0x%x).",*yytext); return BOGOCHAR; }


%%

int ycKeyCmp(const char *S1,const char *S2)
{
  for (;*S1;S1++,S2++) {
    int C1 = *S1;
    int C2 = *S2;
    if (C1 != C2) return (C1 < C2) ? -1 : 1;
  }
  return *S2 ? -1 : 0;
}

int ycFindKW(char *Tok,struct lex_keywordentry *low,int len)
{
  struct lex_keywordentry *high = low+len-1;
  struct lex_keywordentry *K;

  if (!Tok) Tok = yytext;
  while (low <= high) {
    K = low + (high-low)/2;
    switch (ycKeyCmp(K->Keyword,Tok)) {
    case  0 :
      return K->Value;
    case -1 :
      low = K + 1;
      break;
    case  1 :
      high = K - 1;
      break;
    }
  }

  return -1;
}

int ycLiteral(char *Tok)
{
  int r;

  yylval.S = "???";

  if ((r = ycFindKW(Tok,lex_words,num_lex_words)) >= 0) {
    yylval.I = r;
    return r;
  }

  if ((r = ycFindKW(Tok,lex_regs,num_lex_regs)) >= 0) {
    yylval.I = r;
    return REGISTER;
  }

  yylval.S = strdup(Tok);
  return LITERAL;
}

int ycString(char *S)
{
  S = yylval.S = strdup(S+1);
  S[strlen(S)-1] = 0;

  for (;*S;S++)
    if (*S == '\\' && S[1]) {
      memmove(S,S+1,strlen(S+1)+1);
      switch (S[0]) {
      case 'n' :
	S[0] = '\n';
	break;
      case 't' :
	S[0] = '\t';
	break;
      case 'r' :
	S[0] = '\r';
	break;
      }
    }

  return STRING;
}


void BeginBA() { BEGIN BA; }
void BeginMA() { BEGIN MA; }


/*****************************************************************************
 *
 * If flex error suppression is on, make some calls to these functions that
 * are defined but not used by flex to suppress spurious warnings.
 *
 *****************************************************************************/
#if SUPPRESS_FLEX_ERRORS
void yylexjunk()
{
  yy_flex_realloc(0,0);
  yyunput(0,0);
  yy_flex_strlen(0);
}
#endif
