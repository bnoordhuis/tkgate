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
#include "yybasic.h"
#include "ycmalloc.h"
#include "vgrammar.h"

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


/*
 * This flag should be set to zero if we are running as the simulator
 */
int yy_is_editor = 1;

struct lex_keywordentry lex_verilog_words[] = {
  {"always",ALWAYS},
  {"assign",ASSIGN},
  {"begin",BBEGIN},
  {"case",CASE},
  {"casex",CASEX},
  {"casez",CASEZ},
  {"deassign",DEASSIGN},
  {"default",DEFAULT},
  {"else",ELSE},
  {"end",END},
  {"endcase",ENDCASE},
  {"endmodule",ENDMODULE},
  {"endprimitive",ENDPRIMITIVE},
  {"if",IF},
  {"initial",INITIALB},
  {"inout",INOUT},
  {"input",INPUT},
  {"integer",INTEGER},
  {"module",XMODULE},
  {"negedge",NEGEDGE},
  {"output",OUTPUT},
  {"posedge",POSEDGE},
  {"primitive",PRIMITIVE},
  {"reg",REG},
  {"supply0",SUPPLY0},
  {"supply1",SUPPLY1},
  {"tran",TRAN},
  {"tri",KWTRI},
  {"tri0",TRI0},
  {"tri1",TRI1},
  {"triand",TRIAND},
  {"trior",TRIOR},
  {"trireg",TRIREG},
  {"wand",WAND},
  {"wire",WIRE},
  {"wor",WOR},
};
int lex_verilog_num = sizeof(lex_verilog_words)/sizeof(lex_verilog_words[0]);

struct lex_keywordentry lex_anotate_words[] = {
  {"beginscript",BEGINSCRIPT},
  {"comment",COMMENT},
  {"enddecls",ENDDECLS},
  {"endscript",ENDSCRIPT},
  {"frame",FRAME},
  {"interface",BDESC},
  {"joint",JOINT},
  {"property",PROPERTY},
  {"require",REQUIRE},
  {"root_module",ROOTMODULE},
  {"script",SCRIPT},
  {"version",KWVERSION},
};
int lex_anotate_num = sizeof(lex_anotate_words)/sizeof(lex_anotate_words[0]);

struct lex_keywordentry lex_aoption_words[] = {
  {"/bd",BDPORTS},
  {"/builtinBegin",BUILTINBEGIN},
  {"/builtinEnd",END},
  {"/data",DATA},
  {"/dp",DECPOS},
  {"/end",COMEND},
  {"/hdlBegin",HDLBEGIN},
  {"/hdlEnd",END},
  {"/iconBegin",ICONDATA},
  {"/iconEnd",ICONEND},
  {"/netlistBegin",NETLISTBEGIN},
  {"/netlistEnd",END},
  {"/p",PORTS},
  {"/port",PORTDEF},
  {"/r",ROT},
  {"/sn",SHOWNAME},
  {"/symbol",SYMBOLREF},
  {"/symbolBegin",SYMBOLBEGIN},
  {"/symbolEnd",END},
  {"/sz",SIZE},
  {"/textBegin",TEXTBEGIN},
  {"/textEnd",END},
  {"/w",WPLACE},
};
int lex_aoption_num = sizeof(lex_aoption_words)/sizeof(lex_aoption_words[0]);


struct lex_keywordentry lex_delay_words[] = {
  {"area",AREA},
  {"break",BREAK},
  {"case",CASE},
  {"default",DEFAULT},
  {"delay",DELAY},
  {"else",ELSE},
  {"if",IF},
  {"power",POWER},
  {"primitive",PRIMITIVE},
  {"return",RETURN},
  {"switch",SWITCH},
  {"technology",TECHNOLOGY},
  {"version",KWVERSION},
};
int lex_delay_num = sizeof(lex_delay_words)/sizeof(lex_delay_words[0]);

static int yc_last_mode;

int ycLiteral(char *Tok,struct lex_keywordentry *low,int len);
int ycString(char *S);
int ycCheckHdlLine(char *S);

int ycLineNumber;
const char *ycFileName;

/*
  Start states:
	PV	Pure Verilog
	VR	TkGate editor Verilog
	BC	Block comment
	LC	Line comment
	AC	Annotation comment
	DD	Delay definition
	DDP	Delay definition parameter
*/
%}

%start VR BC LC AC DD DDP VN HDL PV

white	[ \t\r]
num	[0-9]
hex	[0-9A-Fa-f]
lit1	[A-Za-z_]
lit2	[A-Za-z0-9_]
litddp	[A-Za-z0-9_/*-]
vnset	[A-Za-z0-9.]
any	[^ \t\r\n]

%%

<PV,VR,DD,DDP>"/*"	{ SaveLast(); BeginBC(); }
<BC>"*/"		{ BeginLast(); }
<BC>.			{ }
<BC>\n			{ ycLineNumber++; }
<PV,VR,DD,DDP>"//"		{ SaveLast(); BeginLC(); }
<LC>\n			{ ycLineNumber++; BeginLast(); }
<LC>.			{ }

<VR,HDL>"//:"		{ SaveLast(); BeginAC(); return ANOTATE; }
<AC>\n			{ ycLineNumber++; BeginLast(); return ENDANOTATE; }

<PV,VR,AC,DD>";"	{ return SEMI; }
<PV,VR,AC,DD>":"	{ return COLON; }
<PV,VR,AC,DD>","	{ return COMMA; }
<PV,VR,AC,DD>"."	{ return DOT; }
<PV,VR,AC,DD>"="	{ return ASGN; }
<PV,VR,AC>"<="		{ return NBASGN; }
<PV,VR,AC>"/"		{ return SLASH; }
<PV,VR,AC,DD>"("	{ return LPAREN; }
<PV,VR,AC,DD>")"	{ return RPAREN; }
<PV,VR,AC,DD>"{"	{ return LBRACE; }
<PV,VR,AC,DD>"}"	{ return RBRACE; }
<PV,VR,AC,DD>"["	{ return LBRACK; }
<PV,VR,AC,DD>"]"	{ return RBRACK; }
<PV,VR,AC,DD>"!"	{ return NOT; }
<PV,VR,AC,DD>"@"	{ return AT; }
<PV,VR,AC,DD>"#"	{ return HASHMARK; }
<PV,VR,AC,DD,DDP>">"	{ return GT; }
<PV,VR,AC,DD,DDP>"<"	{ return LT; }

<DD>"**"		{ return POW; }
<PV,VR,DD>"||"		{ return OR; }
<PV,VR,DD>"&&"		{ return AND; }
<PV,VR,DD>">="		{ return GE; }
<PV,VR,DD>"<="		{ return LE; }
<PV,VR,DD>"=="		{ return EQ; }
<PV,VR,DD>"!="		{ return NE; }
<PV,VR,DD>"+"		{ return ADD; }
<PV,VR,DD>"-"		{ return SUB; }
<PV,VR,DD>"*"		{ return MUL; }
<PV,VR,DD>"/"		{ return DIV; }
<PV,VR,DD>"%"		{ return MOD; }
<PV,VR,DD>"~&"		{ return BNAND; }
<PV,VR,DD>"~|"		{ return BNOR; }
<PV,VR,DD>"&"		{ return BAND; }
<PV,VR,DD>"|"		{ return BOR; }
<PV,VR,DD>"^"		{ return BXOR; }
<PV,VR,DD>"~^"		{ return BNXOR; }
<PV,VR,DD>"~"		{ return UINV; }
<PV,VR,DD>">>"		{ return RSHIFT; }
<PV,VR,DD>"<<"		{ return LSHIFT; }
<PV,VR,DD>"?"		{ return QUEST; }
<PV>">>>"		{ return ARSHIFT; }
<PV>"<<<"		{ return ALSHIFT; }

<PV,VR,AC,DD>\"([^\\\"]*("\\\\"|"\\\"")*)*\"	{ return ycString(yytext); }

<PV,VR,AC,DD>{num}+"'h"{hex}+	{ yylval.S = yc_strdup(yytext); return HEX; } 
<PV,VR,AC,DD>{num}+"'b"{hex}+	{ yylval.S = yc_strdup(yytext); return HEX; } 
<PV,VR,AC,DD>{num}+"'o"{hex}+	{ yylval.S = yc_strdup(yytext); return HEX; } 
<PV,VR,AC,DD>{num}+"'d"{hex}+	{ yylval.S = yc_strdup(yytext); return HEX; } 

<DD>{num}+		{ sscanf(yytext,"%d",&yylval.I); return NUMBER; }
<PV,VR,AC>"-"?{num}+	{ sscanf(yytext,"%d",&yylval.I); return NUMBER; }
<DDP>{litddp}+		{ return ycLiteral(yytext,lex_delay_words,lex_delay_num); }
<DD>{lit1}{lit2}*	{ return ycLiteral(yytext,lex_delay_words,lex_delay_num); }

<PV,VR>\\{any}+		{ yylval.S = yc_strdup(yytext+1); return LITERAL; } 
<PV,VR>{lit1}{lit2}*	{ return ycLiteral(yytext,lex_verilog_words,lex_verilog_num); }
<AC>{lit1}{lit2}*	{ return ycLiteral(yytext,lex_anotate_words,lex_anotate_num); }
<AC>"/"{lit1}{lit2}*	{ return ycLiteral(yytext,lex_aoption_words,lex_aoption_num); }
<PV>${lit1}{lit2}*	{ yylval.S = yc_strdup(yytext); return SYSLITERAL; } 
<PV>.			{ return BOGOCHAR; }

<VN>{vnset}+			{ yylval.S = yc_strdup(yytext); return VERNUM; }
<PV,VR,AC,DD,DDP,VN>{white}+	{ }
<PV,VR,DD,DDP,VN>"\n"		{ ycLineNumber++; }

<VR>"`"{lit1}{lit2}*.*"\n"	{ ycDirective(yytext); }

<HDL>.*"\n"			{ ycLineNumber++; return ycCheckHdlLine(yytext); }

.				{ FLEX_FIX_STAT yyerror("Illegal character (%d) '%c'.\n",*yytext,*yytext); return BOGOCHAR; }


%%

void BeginPV() { BEGIN PV; }
void BeginVR() { BEGIN VR; }
void BeginAC() { BEGIN AC; }
void BeginLC() { BEGIN LC; }
void BeginBC() { BEGIN BC; }
void BeginDD() { BEGIN DD; }
void BeginDDP() { BEGIN DDP; }
void BeginVN() { BEGIN VN; }
void BeginHDL() { if (yy_is_editor) { BEGIN HDL; } }
void BeginLast() { BEGIN yc_last_mode; }
void SaveLast() { yc_last_mode = YY_START; }

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
