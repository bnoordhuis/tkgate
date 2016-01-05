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
#include "thyme.h"

#ifndef YY_START
#define YY_START YYSTATE
#endif

/*****************************************************************************
 *
 * Flex has some errors that cause its output to generate several spurious
 * warnings when compiling using gcc and the -Wall option.  One of the errors
 * is a goto label 'find_rule' that is never used.  If SUPPRESS_FLEX_ERRORS
 * is non-zero, then we will try to suppress the error by inserting a goto
 * that label that is conditional on a zero condition so that this in fact
 * acts as a noop.
 *
 *****************************************************************************/
#if SUPPRESS_FLEX_ERRORS
#define FLEX_FIX_STAT if (0) goto find_rule;
#else
#define FLEX_FIX_STAT
#endif

extern Place curPlace;			/* The current place */


/*
 * This flag should be set to zero if we are running as the simulator
 */
int yy_is_editor = 1;

struct lex_keywordentry lex_verilog_words[] = {
  {"always",ALWAYS},
  {"and",PRIMAND},
  {"assign",ASSIGN},
  {"automatic",AUTOMATIC},
  {"begin",BBEGIN},
  {"buf",BUF},
  {"bufif0",BUFIF0},
  {"bufif1",BUFIF1},
  {"case",CASE},
  {"casex",CASEX},
  {"casez",CASEZ},
  {"cmos",CMOS},
  {"deassign",DEASSIGN},
  {"default",DEFAULT},
  {"else",ELSE},
  {"end",END},
  {"endcase",ENDCASE},
  {"endfunction",ENDFUNCTION},
  {"endmodule",ENDMODULE},
  {"endprimitive",ENDPRIMITIVE},
  {"endspecify", ENDSPECIFY},
  {"endtask",ENDTASK},
  {"event",EVENT},
  {"for",FOR},
  {"forever",FOREVER},
  {"fork",FORK},
  {"function",FUNCTION},
  {"highz0",HIGHZ0},
  {"highz1",HIGHZ1},
  {"if",IF},
  {"initial",INITIALB},
  {"inout",INOUT},
  {"input",INPUT},
  {"integer",INTEGER},
  {"join",JOIN},
  {"large", LARGE},
  {"medium", MEDIUM},
  {"module",MODULE},
  {"nand",PRIMNAND},
  {"negedge",NEGEDGE},
  {"nmos",NMOS},
  {"nor",PRIMNOR},
  {"not",PRIMNOT},
  {"notif0",NOTIF0},
  {"notif1",NOTIF1},
  {"or",KWOR},
  {"output",OUTPUT},
  {"parameter",PARAMETER},
  {"pmos",PMOS},
  {"posedge",POSEDGE},
  {"primitive",PRIMITIVE},
  {"pull0",PULL0},
  {"pull1",PULL1},
  {"rcmos",RCMOS},
  {"real",REAL},
  {"reg",REG},
  {"repeat",REPEAT},
  {"rnmos",RNMOS},
  {"rpmos",RPMOS},
  {"rtran",RTRAN},
  {"rtranif0",RTRANIF0},
  {"rtranif1",RTRANIF1},
  {"scalar", SCALAR},
  {"signed", SIGNED},
  {"small", SMALL},
  {"specify", SPECIFY},
  {"specparam", SPECPARAM},
  {"strong0",STRONG0},
  {"strong1",STRONG1},
  {"supply0",SUPPLY0},
  {"supply1",SUPPLY1},
  {"task",TASK},
  {"time",TIME},
  {"tran",TRAN},
  {"tranif0",TRANIF0},
  {"tranif1",TRANIF1},
  {"tri", TRI},
  {"tri0",TRI0},
  {"tri1",TRI1},
  {"triand",TRIAND},
  {"trior",TRIOR},
  {"trireg", TRIREG},
  {"vectored",VECTORED},
  {"wait",WAIT},
  {"wand",WAND},
  {"weak0",WEAK0},
  {"weak1",WEAK1},
  {"while",WHILE},
  {"wire",WIRE},
  {"wor",WOR},
  {"xnor",PRIMXNOR},
  {"xor",PRIMXOR},
};
int lex_verilog_num = sizeof(lex_verilog_words)/sizeof(lex_verilog_words[0]);

static int yc_last_mode;

int ycLiteral(char *Tok,struct lex_keywordentry *low,int len);
int ycString(char *S);
int ycNumber(char *S);
int ycHex(char *S);
int ycCheckHdlLine(char *S);

char *yc_getescriptname(char *token);

/*
  Start states:
	PV	Pure Verilog
	BC	Block comment
	LC	Line comment
	BP	Breakpoint
	SF	Script file
	CD	Compiler Directive
	DS	Directive skip mode
*/
%}

%start BC LC PV BP SF CD DS

white	[ \t\r]
num	[0-9]
hex	[0-9A-Fa-fxzXZlLhH?]
lit1	[A-Za-z_]
lit2	[A-Za-z0-9_]
litsys	[$A-Za-z0-9_]
litddp	[A-Za-z0-9_/*-]
any	[^ \t\r\n]

%%

<PV>"/*"		{ SaveLast(); BeginBC(); }
<BC>"*/"		{ BeginLast(); }
<BC>.			{ }
<BC>\n			{ Place_incLineno(&curPlace,1); }
<PV>"//"		{ SaveLast(); BeginLC(); }
<LC>\n			{ Place_incLineno(&curPlace,1); BeginLast(); }
<LC>.			{ }

<PV>"//:".*		{ VerSpecialTag(yytext); }

<BP>.			{ BeginPV(); unput(*yytext); return BREAKPT; }
<SF>.			{ BeginPV(); unput(*yytext); return SCRIPT; }

<CD>(.|\\"\n")+"\n"	{ ycDirectiveText(yytext); Directive_handle(yytext,0); }
<CD>"\n"		{}

<DS>.*		{ ycDirectiveText(yytext); Directive_handle(yytext,1); }
<DS>"\n"		{ Place_incLineno(&curPlace,1); }

<PV>";"			{ return SEMI; }
<PV>":"			{ return COLON; }
<PV>","			{ return COMMA; }
<PV>"."			{ return DOT; }
<PV>"="			{ return ASGN; }
<PV>"("			{ return LPAREN; }
<PV>")"			{ return RPAREN; }
<PV>"{"			{ return LBRACE; }
<PV>"}"			{ return RBRACE; }
<PV>"["			{ return LBRACK; }
<PV>"]"			{ return RBRACK; }
<PV>"!"			{ return NOT; }
<PV>"@"			{ return AT; }
<PV>"#"			{ return HASH; }
<PV>">"			{ return GT; }
<PV>"<"			{ return LT; }
<PV>"||"		{ return OR; }
<PV>"&&"		{ return AND; }
<PV>"&&&"		{ return COND; }
<PV>"*>"		{ return MPATH; }
<PV>"=>"		{ return DPATH; }
<PV>"->"		{ return TRIGGER; }
<PV>">="		{ return GE; }
<PV>"<="		{ return LE; }
<PV>"=="		{ return EQ; }
<PV>"!="		{ return NE; }
<PV>"==="		{ return EQZ; }
<PV>"!=="		{ return NEZ; }
<PV>"+"			{ return ADD; }
<PV>"-"			{ return SUB; }
<PV>"*"			{ return MUL; }
<PV>"/"			{ return DIV; }
<PV>"%"			{ return MOD; }
<PV>"~&"		{ return BNAND; }
<PV>"~|"		{ return BNOR; }
<PV>"&"			{ return BAND; }
<PV>"|"			{ return BOR; }
<PV>"^"			{ return BXOR; }
<PV>"~^"		{ return BNXOR; }
<PV>"~"			{ return UINV; }
<PV>">>"		{ return RSHIFT; }
<PV>"<<"		{ return LSHIFT; }
<PV>"?"			{ return QUEST; }
<PV>">>>"		{ return ARSHIFT; }
<PV>"<<<"		{ return ALSHIFT; }


<PV>\"([^\\\"]*("\\".)*)*\"	{ return ycString(yytext); }

<PV>\\{any}+		{ yylval.S = yc_strdup(yytext+1); return LITERAL; } 
<PV>{num}*"'h"{hex}+	{ return ycHex(yytext); } 
<PV>{num}*"'b"{hex}+	{ return ycHex(yytext); } 
<PV>{num}*"'o"{hex}+	{ return ycHex(yytext); } 
<PV>{num}*"'d"{hex}+	{ return ycHex(yytext); } 
<PV>{num}+		{ return ycNumber(yytext); }
<PV>{num}+\.{num}+	{ return ycFloat(yytext); }
<PV>{lit1}{lit2}*	{ return ycLiteral(yytext,lex_verilog_words,lex_verilog_num); }
<PV>${litsys}+		{ yylval.S = yc_strdup(yytext); return SYSLITERAL; } 
<PV>{lit1}{lit2}*"."{lit1}{lit2}*("."{lit1}{lit2}*)* { yylval.S = yc_strdup(yytext); return HLITERAL; } 
<PV>"`"{lit2}+		{ int r = ycDirective(yytext); if (r>0) return r; }

<PV>{white}+		{ }
<PV>"\n"		{ Place_incLineno(&curPlace,1); }

<PV>"//:"{white}*"beginscript".*"\n"	{ Place_incLineno(&curPlace,1); yylval.S = yc_getescriptname(yytext); return BEGINSCRIPT; }
<PV>"//:"{white}*"endscript".*"\n"	{ Place_incLineno(&curPlace,1); return ENDSCRIPT; }


.			{ FLEX_FIX_STAT return yc_bogochar(*yytext); }
%%

void BeginPV() { BEGIN PV; }
void BeginSF() { BEGIN SF; }
void BeginLC() { BEGIN LC; }
void BeginBC() { BEGIN BC; }
void BeginCD() { BEGIN CD; }
void BeginDS() { BEGIN DS; }
void BeginLast() { BEGIN yc_last_mode; }
void SaveLast() { yc_last_mode = YY_START; }

void yc_unputstring(char *s)
{
  int l = strlen(s);
  int i;

  s = strdup(s);

  for (i = l-1;i >= 0;i--)
    unput(s[i]);

  free(s);
}

int yc_bogochar()
{
  char buf1[16], buf2[16];

  sprintf(buf1,"%d",*yytext);
  sprintf(buf2,"%c",*yytext);
  errorFile(&curPlace,ERR_BADCHAR,buf1,buf2);
  return BOGOCHAR; 
}

/*****************************************************************************
 *
 * Get the script name part of a
 *
 *****************************************************************************/
char *yc_getescriptname(char *token)
{
  char buf[1024];

  if (sscanf(token," //: beginscript %s",buf) == 1)
    return yc_strdup(buf);
  else {
    errorFile(&curPlace,ERR_SYNTAX);
    return yc_strdup("null");
  }
}

/*****************************************************************************
 *
 * If flex error suppression is on, make some calls to these functions that
 * are defined but not used by flex to suppress spurious warnings.
 *
 *****************************************************************************/
#if SUPPRESS_FLEX_ERRORS
void yc_lexjunk()
{
  yy_flex_realloc(0,0);
  yyunput(0,0);
  yy_flex_strlen(0);
}
#endif
