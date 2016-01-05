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
#include "thyme.h"

extern Place curPlace;			/* The current place */

extern struct lex_keywordentry lex_verilog_words[];
extern int lex_verilog_num;

/*****************************************************************************
 *
 * List of all Verilog reserved words.  These are used by the ycIsKW() function
 * to indicate these names should not be used as identifiers in user circuits. 
 * This table may include entries not in the lex_verilog_words to cover verilog
 * reserved words that are not handled by the tkgate verilog simulator.
 *
 *****************************************************************************/
static struct lex_keywordentry lex_reserved_words[] = {
  {"always",1},
  {"and",1},
  {"assign",1},
  {"attribute",1},
  {"begin",1},
  {"buf",1},
  {"bufif0",1},
  {"bufif1",1},
  {"case",1},
  {"casex",1},
  {"casez",1},
  {"cmos",1},
  {"deassign",1},
  {"default",1},
  {"defparam",1},
  {"disable",1},
  {"edge",1},
  {"else",1},
  {"end",1},
  {"endattribute",1},
  {"endcase",1},
  {"endfunction",1},
  {"endmodule",1},
  {"endprimitive",1},
  {"endspecify",1},
  {"endtable",1},
  {"endtask",1},
  {"event",1},
  {"for",1},
  {"force",1},
  {"forever",1},
  {"fork",1},
  {"function",1},
  {"highz0",1},
  {"highz1",1},
  {"if",1},
  {"initial",1},
  {"inout",1},
  {"input",1},
  {"integer",1},
  {"join",1},
  {"large",1},
  {"medium",1},
  {"module",1},
  {"macromodule",1},
  {"nand",1},
  {"negedge",1},
  {"nmos",1},
  {"nor",1},
  {"not",1},
  {"notif0",1},
  {"notif1",1},
  {"or",1},
  {"output",1},
  {"parameter",1},
  {"pmos",1},
  {"posedge",1},
  {"primitive",1},
  {"pull0",1},
  {"pull1",1},
  {"pulldown",1},
  {"pullup",1},
  {"rcmos",1},
  {"real",1},
  {"realtime",1},
  {"reg",1},
  {"release",1},
  {"repeat",1},
  {"rnmos",1},
  {"rpmos",1},
  {"rtran",1},
  {"rtranif0",1},
  {"rtranif1",1},
  {"scalared",1},
  {"signed",1},
  {"small",1},
  {"specify",1},
  {"specparam",1},
  {"strength",1},
  {"strong0",1},
  {"strong1",1},
  {"supply0",1},
  {"supply1",1},
  {"table",1},
  {"task",1},
  {"time",1},
  {"tran",1},
  {"tranif0",1},
  {"tranif1",1},
  {"tri",1},
  {"tri0",1},
  {"tri1",1},
  {"triand",1},
  {"trior",1},
  {"trireg",1},
  {"unsigned",1},
  {"vectored",1},
  {"wait",1},
  {"wand",1},
  {"weak0",1},
  {"weak1",1},
  {"while",1},
  {"wire",1},
  {"wor",1},
  {"xnor",1},
  {"xor",1},
};
static int lex_reserved_num = sizeof(lex_reserved_words)/sizeof(lex_reserved_words[0]);

static struct lex_keywordentry lex_directive_words[] = {
  {"`define",1},
  {"`else",1},
  {"`elseif",1},
  {"`endif",1},
  {"`ifdef",1},
  {"`ifndef",1},
  {"`include",1},
  {"`timescale",1},
};
static int lex_directive_num = sizeof(lex_directive_words)/sizeof(lex_directive_words[0]);

int ycKeyCmp(const char *S1,const char *S2)
{
  for (;*S1;S1++,S2++) {
    int C1 = islower(*S1) ? toupper(*S1) : *S1;
    int C2 = islower(*S2) ? toupper(*S2) : *S2;
    if (C1 != C2) return (C1 < C2) ? -1 : 1;
  }
  return *S2 ? -1 : 0;
}

int ycLookup(char *Tok,struct lex_keywordentry *low,int len)
{
  struct lex_keywordentry *high = low+len-1;
  struct lex_keywordentry *K;

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

/*****************************************************************************
 *
 * Determine if a string is a verilog keyword.
 *
 * Parameters:
 *     token		String to be tested.
 *
 *****************************************************************************/
int ycIsKW(char *token)
{
  if (ycLookup(token,lex_reserved_words,lex_reserved_num) >= 0) return 1;
  return 0;
}

/*****************************************************************************
 *
 * Get the keyword string for a token code
 *
 * Parameters:
 *     tokenCode	Code number for token.
 *
 *****************************************************************************/
const char *ycGetKeyword(int tokenCode)
{
  int i;

  for (i = 0;i < lex_verilog_num;i++)
    if (lex_verilog_words[i].Value == tokenCode)
      return lex_verilog_words[i].Keyword;

  return 0;
}

/*
    Parse a "literal-like" token.  If it is in a keyword table, return the
    token for that keyword, otherwise treat it as a literal.
*/
int ycLiteral(char *Tok,struct lex_keywordentry *low,int len)
{
  int t = 0;

  if (!Tok) Tok = "?";
  yylval.S = "???";

  if (low)
    t = ycLookup(Tok,low,len);

  if (t >= 0) {
    yylval.I = t;
    return t;
  } else {
    yylval.S = yc_strdup(Tok);
    return LITERAL;
  }

  return t >= 0 ? t : LITERAL;
}

int ycHex(char *S)
{
  yylval.S = yc_strdup(S);
  return HEX;
}

int ycNumber(char *S)
{
  if (strlen(S) <= 9) {
    sscanf(S,"%d",&yylval.I);
    return NUMBER;
  } else {
    yylval.S = yc_malloc(strlen(S)+3,ALGN_BYTE);
    sprintf(yylval.S,"'d%s",S);
    return HEX;
  }
}

int ycFloat(char *S)
{
  sscanf(S,"%f",&yylval.F);
  return REALNUM;
}

int ycString(char *S)
{
  S = yylval.S = yc_strdup(S+1);
  S[strlen(S)-1] = 0;

  for (;*S;S++)
    if (*S == '\\' && S[1]) {
      switch (S[1]) {
      case 'n' :
	S[1] = '\n';
	break;
      case 'r' :
	S[1] = '\r';
	break;
      case 't' :
	S[1] = '\t';
	break;
      case 'b' :
	S[1] = '\b';
	break;
      case 'e' :
	S[1] = '\e';
	break;
      default :
	break;
      }
      memmove(S,S+1,strlen(S+1)+1);
      S++;
    }

  return STRING;
}

/*****************************************************************************
 *
 * Handle a verilog directive or macro lookup.
 *
 *****************************************************************************/
int ycDirective(char *token)
{
  if (ycLookup(token,lex_directive_words,lex_directive_num) > 0) {
    yc_unputstring(token);
    SaveLast();
    BeginCD();
    return -1;
  } else
    return Directive_macroReplace(token);
}

/*****************************************************************************
 *
 * Pre-process directive text (count newlines, and remove any backslashes
 * used to continue them).
 *
 *****************************************************************************/
int ycDirectiveText(char *token)
{
  char *p;

  for (p = token;*p;p++)
    if (*p == '\n') {
      Place_incLineno(&curPlace,1); 
      if (p[-1] == '\\') p[-1] = ' ';
      p[0] = ' ';
    }

  return 0;
}

int yywrap()
{
  return 1;
}
