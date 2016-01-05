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
%expect 1

%token NETLISTBEGIN HDLBEGIN BUILTINBEGIN SYMBOLBEGIN

%token XMODULE NETMODULE HDLMODULE BUILTINMODULE

%token ASSIGN ENDMODULE 
%token PRIMITIVE TECHNOLOGY DELAY AREA POWER IF ELSE SWITCH CASE CASEX CASEZ RETURN
%token BREAK NEXT FUNC DEFAULT INITIALB
%token INPUT REG OUTPUT INOUT WIRE KWVERSION SUPPLY0 SUPPLY1 KWTRI TRI0 TRI1 TRIREG TRIAND TRIOR WAND WOR

%token DEASSIGN BBEGIN END ENDCASE ALWAYS POSEDGE NEGEDGE
%token INTEGER ENDPRIMITIVE

%token SEMI COLON COMMA DOT ASGN NBASGN LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK AT
%token ANOTATE ENDANOTATE ROT WPLACE SHOWNAME ENDDECLS NOT PORTS PORTDEF BDPORTS
%token SIZE JOINT COMMENT FRAME SLASH BDESC ROOTMODULE TRAN COMLINE COMEND SCRIPT BEGINSCRIPT ENDSCRIPT REQUIRE
%token PROPERTY DECPOS ICONDATA DATA ICONEND SYMBOLREF
%token TEXTBEGIN HASHMARK

%token GT LT EQ BAND BNAND BOR BNOR BXOR BNXOR UINV MOD LSHIFT RSHIFTEQ QUEST

%token ARSHIFT ALSHIFT

%token BOGOCHAR

%token <I> NUMBER
%token <S> HEX VERNUM
%token <S> LITERAL STRING HDLLINE SYSLITERAL

%type <I> bwidth obang bdir dtype wsizep
%type <S> dparm clit modparmval
%type <E> pexpr pcode pc_stats pc_stat


%left QUEST COLON
%left OR
%left AND
%left BOR BNOR
%left BXOR BNXOR
%left BAND BNAND
%left EQ NE
%left GT LT GE LE
%left LSHIFT RSHIFT
%left ADD SUB
%left MUL DIV MOD
%left POW
%left UNEG UPLUS NOT UINV

%{
#include <unistd.h>
#include <stdio.h>
#include "config.h"
#include "yybasic.h"
#include "vgrammar.h"
#include "vparser.h"
#include "ycmalloc.h"
#include "misc.h"

extern int yy_is_editor;

static char *tech_name = "default";

void BeginPV();		/* Pure verilog */
void BeginVR();		/* tkgate verilog */
void BeginAC();
void BeginLC();
void BeginBC();
void BeginDD();
void BeginDDP();
void BeginVN();
void BeginHDL();

%}

%%

/*****************************************************************************
 * Top-level file.
 *****************************************************************************/
prog	: ANOTATE KWVERSION STRING ENDANOTATE { VerCheckVersion($3); } vunits
	| KWVERSION { BeginVN(); } VERNUM { BeginDD(); }  tunits
	| vunit vunits
	| /* Empty file */
	;


/*****************************************************************************
 * Tech file units
 *****************************************************************************/
tunits	:
	| tunits tunit
	;

tunit	: primdef
	| tech_def
	;

/*****************************************************************************
 * Verilog file units
 *****************************************************************************/
vunits	:
	| vunits vunit
	;

vunit	: net_module
	| builtin_module
	| hdl_module
	| modsymbol
	| ANOTATE SCRIPT STRING ENDANOTATE			{ VerAddScript($3); }
	| ANOTATE REQUIRE STRING ENDANOTATE			{ VerAddVLib($3); }
	| ANOTATE PROPERTY LITERAL ASGN NUMBER ENDANOTATE	{ VerCircuitProp($3,&$5,TYPE_INT); }
	| ANOTATE PROPERTY LITERAL ASGN LITERAL ENDANOTATE	{ VerCircuitProp($3,$5,TYPE_STR); }
	| ANOTATE PROPERTY LITERAL ASGN STRING ENDANOTATE 	{ VerCircuitProp($3,$5,TYPE_STR); }
	| error ENDANOTATE
	| SEMI
	;

/*****************************************************************************
 * HDL module
 *
 * There are two types of Verilog HDL modules we can scan.  'native' and 'annotated'.
 * An 'annotated' module is one with tkgate annotations. A native module is one that
 * is in standard verilog without any tkgate annotations.  These cases correspond
 * to the first two rules below.
 *
 *****************************************************************************/
hdl_module	: ANOTATE HDLBEGIN LITERAL ENDANOTATE { nativeVerilog = 0; VerNewModule(HDLMODULE,$3,0); }
		  hprops ANOTATE ENDDECLS ENDANOTATE
		  { BeginHDL(); } hstats END { BeginVR(); VerEndModule(); }
		| hdl_modhead { VerModParm(0); BeginHDL(); } hstats ENDMODULE { BeginVR(); VerEndModule(); }
		| error END { BeginVR(); }
		| error ENDMODULE { BeginVR(); }
		;

hdl_modhead	: HDLMODULE LITERAL LPAREN { nativeVerilog = 1; VerNewModule(HDLMODULE,$2,0); } omargs RPAREN SEMI
		| HDLMODULE LITERAL SEMI { nativeVerilog = 1; VerNewModule(HDLMODULE,$2,1); }
		| HDLMODULE LITERAL HASHMARK { nativeVerilog = 1; VerNewModule(HDLMODULE,$2,0); } LPAREN mparms RPAREN LPAREN omargs RPAREN SEMI
		;

mparms	: mparm
	| mparms COMMA mparm
	;

mparm	: LITERAL ASGN pexpr			{ VerModHashParm($1,$3); }
	| DOT LITERAL LPAREN pexpr RPAREN	{ VerModHashParm($2,$4); }
	;


hstats		:
		| hstats HDLLINE { VerAddHdlLine($2); }
		;

hprops	:
	| hprops hprop
	;

hprop	: ANOTATE BDESC { VerBeginBD(); } cnote ENDANOTATE { VerEndBD(); }
	| ANOTATE SYMBOLREF COLON NUMBER ENDANOTATE { VerModuleProp("symbol",&$4,TYPE_INT); }
	| ANOTATE ROOTMODULE ENDANOTATE { VerSetRootMod(); }
	| ANOTATE PROPERTY LITERAL ASGN NUMBER ENDANOTATE { VerModuleProp($3,&$5,TYPE_INT); }
	| ANOTATE PROPERTY LITERAL ASGN LITERAL ENDANOTATE { VerModuleProp($3,$5,TYPE_STR); }
	| ANOTATE PROPERTY LITERAL ASGN STRING ENDANOTATE { VerModuleProp($3,$5,TYPE_STR); }
	;

/*****************************************************************************
 * Built-in module - These should simply be ignored
 *****************************************************************************/
builtin_module : ANOTATE BUILTINBEGIN ENDANOTATE
		 { nativeVerilog = 0; BeginHDL(); } skiplines END { BeginVR(); }
		;

skiplines	:
		| skiplines HDLLINE
		;

/*****************************************************************************
 * Netlist module
 *****************************************************************************/
net_module	: ANOTATE NETLISTBEGIN LITERAL ENDANOTATE { VerNewModule(NETMODULE,$3,0); }
		  net_e_modhead decls enddecl stats ENDMODULE { VerEndModule(); }
		  ANOTATE END ENDANOTATE
		| net_modhead decls enddecl stats ENDMODULE { VerEndModule(); }
		;

net_modhead	: NETMODULE LITERAL LPAREN { VerNewModule(NETMODULE,$2,0); } omargs RPAREN SEMI
		| NETMODULE LITERAL SEMI  { VerNewModule(NETMODULE,$2,1); }
		;
net_e_modhead	: NETMODULE LITERAL LPAREN omargs RPAREN SEMI
		| NETMODULE LITERAL SEMI
		;

omargs	: 
	| margs
	;

margs	: LITERAL			{ VerModParm($1); }
	| margs COMMA LITERAL		{ VerModParm($3); }
	;


decls	:
	| decls decl
	;

decl	: dtype bwidth LITERAL SEMI 	{ VerNewNet($3,$2,$1); }
	| ANOTATE wnotes ENDANOTATE
	| ANOTATE BDESC { VerBeginBD(); } cnote ENDANOTATE { VerEndBD(); }
	| ANOTATE SYMBOLREF COLON NUMBER ENDANOTATE { VerModuleProp("symbol",&$4,TYPE_INT); }
	| ANOTATE ROOTMODULE ENDANOTATE { VerSetRootMod(); }
	| ANOTATE PROPERTY LITERAL ASGN NUMBER ENDANOTATE { VerModuleProp($3,&$5,TYPE_INT); }
	| ANOTATE PROPERTY LITERAL ASGN LITERAL ENDANOTATE { VerModuleProp($3,$5,TYPE_STR); }
	| ANOTATE PROPERTY LITERAL ASGN STRING ENDANOTATE { VerModuleProp($3,$5,TYPE_STR); }
	;

dtype	: INPUT				{ $$ = INPUT; } 
	| OUTPUT			{ $$ = OUTPUT; } 
	| INOUT				{ $$ = INOUT; } 
	| SUPPLY0			{ $$ = SUPPLY0; } 
	| SUPPLY1			{ $$ = SUPPLY1; } 
	| REG 				{ $$ = REG; } 
	| WIRE 				{ $$ = WIRE; } 
	| WAND 				{ $$ = WAND; } 
	| WOR 				{ $$ = WOR; } 
	| KWTRI 			{ $$ = KWTRI; } 
	| TRI0 				{ $$ = TRI0; } 
	| TRI1 				{ $$ = TRI1; } 
	| TRIAND 			{ $$ = TRIAND; } 
	| TRIOR 			{ $$ = TRIOR; } 
	| TRIREG 			{ $$ = TRIREG; } 
	;

wnotes	: 
	| wnotes wnote
	;

wnote	: wend wcoords wend 
	| SHOWNAME COLON NUMBER				{ VerSetShowName($3); }
	| DECPOS COLON NUMBER				{ VerSetWireDecorationPos($3,-1,-1); }
	| DECPOS COLON LPAREN NUMBER COMMA NUMBER COMMA NUMBER RPAREN	{ VerSetWireDecorationPos($4,$6,$8); }
	;

wcoords	: wcoord
	| wcoords wcoord
	;

/*
wcoord	: LPAREN NUMBER COMMA NUMBER RPAREN					{ VerMakeNode($2,$4,0,0,0); }
	| LBRACK NUMBER RBRACK LPAREN NUMBER COMMA NUMBER RPAREN		{ VerMakeNode($5,$7,0,1,$2); }
	| HASHMARK LPAREN NUMBER COMMA NUMBER RPAREN				{ VerMakeNode($3,$5,1,0,0); }
	| HASHMARK LBRACK NUMBER RBRACK LPAREN NUMBER COMMA NUMBER RPAREN	{ VerMakeNode($6,$8,1,1,$3); }
	;
*/
wcoord	: LPAREN wsizep NUMBER COMMA NUMBER RPAREN				{ VerMakeNode($3,$5,$2,0,0); }
	| LPAREN wsizep NUMBER COLON NUMBER COMMA NUMBER RPAREN			{ VerMakeNode($5,$7,$2,1,$3); }
	;

wsizep	: HASHMARK COLON	{ $$ = 1; }
	|			{ $$ = 0; }
	;

wend	: LBRACE NUMBER RBRACE			{ VerMakeWire($2); }
	;

enddecl	: ANOTATE ENDDECLS ENDANOTATE		{ VerEndDecls(); }
	;

bwidth	:					{ $$ = 1; }
	| LBRACK NUMBER COLON NUMBER RBRACK	{ $$ = ($2-$4)+1; }
	;

stats	:
	| stats stat
	;

stat	: call SEMI ANOTATE cnote ENDANOTATE { VerEndGate(); }
	| ANOTATE joint ENDANOTATE { VerEndGate(); }
	| ANOTATE iogate ENDANOTATE { VerEndGate(); }
	| ANOTATE comment ENDANOTATE { VerEndGate(); }
	| ANOTATE frame ENDANOTATE { VerEndGate(); }
	| ANOTATE script { VerEndGate(); }
	| concat { VerEndGate(); }
	| tran { VerEndGate(); }
	| tapassign { VerEndGate(); }
	| concat_assign { VerEndGate(); }
	| error ENDANOTATE { yyerrok; }
	;

comment	: COMMENT LITERAL { VerGate("COMMENT",$2); } cnote ENDANOTATE comlines ANOTATE COMEND
	;

script	: BEGINSCRIPT LITERAL { VerGate("SCRIPT",$2); } cnote ENDANOTATE {BeginHDL(); } script_lines END { BeginVR(); }
	;

script_lines	:
		| script_lines HDLLINE { VerAddScriptLine($2); }
		;


frame	: FRAME LITERAL { VerGate("FRAME",$2); } cnote
	;

comlines :
	| comlines ANOTATE cnote ENDANOTATE
	;

joint	: JOINT LITERAL { VerGate("JOINT",$2); } LPAREN LITERAL RPAREN { VerJointNet($5); } cnote
	; 

iogate	: LITERAL LPAREN { VerGate($1,0); } ocargs RPAREN cnote
	| LITERAL LITERAL LPAREN { VerGate($1,$2); } ocargs RPAREN cnote
	;

tapassign	: ASSIGN LITERAL ASGN LITERAL LBRACK NUMBER RBRACK SEMI ANOTATE LITERAL LITERAL
		{ VerGate("TAP",$11); VerTranRange($6,$6); VerAttach("Z",$2,0); VerAttach("I",$4,0); VerTranDup(); }
		cnote ENDANOTATE
		| ASSIGN LITERAL ASGN LITERAL LBRACK NUMBER COLON NUMBER RBRACK SEMI ANOTATE LITERAL LITERAL
		{ VerGate("TAP",$13); VerTranRange($6,$8); VerAttach("Z",$2,0); VerAttach("I",$4,0); VerTranDup(); }
		cnote ENDANOTATE
		;

concat_assign	: ASSIGN LITERAL ASGN LBRACE { VerGate("CONCAT",0); VerAttach("Z",$2,0); } concat_args RBRACE SEMI
			ANOTATE LITERAL LITERAL { VerRenameCurrent($11); VerRevPad(0); } cnote ENDANOTATE
		| ASSIGN LBRACE  { VerGate("CONCAT",0); } concat_args RBRACE ASGN LITERAL { VerAttach("Z",$7,0); } SEMI
			ANOTATE LITERAL LITERAL { VerRenameCurrent($12); VerRevPad(0); } cnote ENDANOTATE
		;


concat	: TRAN LITERAL LBRACK  NUMBER COLON NUMBER RBRACK LPAREN { VerGate("CONCAT",$2); }
	  LBRACE concat_args RBRACE
	  COMMA
	  LITERAL { VerAttach("Z",$14,0); }
	  RPAREN SEMI { VerRevPad(0); }
	  ANOTATE cnote ENDANOTATE 
	;

concat_args : LITERAL { VerAttach("I",$1,0); }
	    | concat_args COMMA LITERAL { VerAttach("I",$3,0); }
		;

/*
 * Old style syntac for TAP gates.
 */
tran	: TRAN LITERAL LPAREN { VerGate("TAP",$2); } carg COMMA 
		tran_arg { VerTranDup(); } RPAREN SEMI ANOTATE cnote ENDANOTATE
	;

tran_arg	: DOT LITERAL LPAREN obang LITERAL tran_range RPAREN { VerAttach($2,$5,$4); }
		| obang LITERAL tran_range			{ VerAttach(0,$2,$1); }
		;

tran_range	: LBRACK NUMBER RBRACK			{ VerTranRange($2,$2); }
		|  LBRACK NUMBER COLON NUMBER RBRACK	{ VerTranRange($2,$4); }
		;

call		: LITERAL { VerCallMParmFlush(); } omodparms LITERAL LPAREN { VerCell($1,$4); } ocargs RPAREN
		;

omodparms	:
		| HASHMARK LPAREN modparms RPAREN  
		;

modparms	: modparm	
		| modparms COMMA modparm
		;

modparm		: modparmval				{ VerCallMParmAdd(0,$1); }
		| DOT LITERAL LPAREN modparmval RPAREN	{ VerCallMParmAdd($2,$4); }
		| LITERAL ASGN modparmval		{ VerCallMParmAdd($1,$3); }
		;

modparmval	: NUMBER 				{ $$ = yc_sprintf("%u",$1); }
		| HEX					{ $$ = $1; }
		| STRING				{ char buf[1024]; $$ = yc_sprintf("\"%s\"",quoteChars(buf,$1,"\"\\")); }
		; 

ocargs	: 
	| cargs
	;

cargs	: carg
	| cargs COMMA carg
	;

carg	: DOT LITERAL LPAREN obang clit RPAREN		{ VerAttach($2,$5,$4); }
	| obang LITERAL					{ VerAttach(0,$2,$1); }
	;

/*
 * We ignore any bit repetitions in netlist modules and assume tkgate will figure
 * out what to do on its own.
 */
clit	: LITERAL					{ $$ = $1; }
	| LBRACE NUMBER LBRACE LITERAL RBRACE RBRACE	{ $$ = $4; }
	; 

obang	:			{ $$ = 0; }
	| NOT			{ $$ = 1; }
	| UINV			{ $$ = 1; }
	;



cnote	: citem
	| cnote citem
	;

citem	: AT LPAREN NUMBER COMMA NUMBER RPAREN	{ VerSetPos($3,$5); }
	| SIZE COLON LPAREN NUMBER COMMA NUMBER RPAREN { VerSetSize($4,$6); }
	| ROT COLON NUMBER			{ VerSetRot($3); }
	| WPLACE COLON LBRACK wplist RBRACK
	| PORTS COLON LBRACK pplist RBRACK
	| BDPORTS COLON LBRACK bdplist RBRACK
	| SHOWNAME COLON NUMBER			{ VerSetShowName($3); }
	| SYMBOLREF COLON NUMBER		{ VerSetBlockSymbol($3); }
	| LITERAL COLON NUMBER			{ VerSetProperty($1,$3); }
	| LITERAL COLON STRING			{ VerSetStrProperty($1,$3); }
	| LITERAL COLON LITERAL			{ VerSetStrProperty($1,$3); }
	;

wplist	: 
	| wplist NUMBER				{ VerPlaceWire($2); }
	;

pplist	: 
	| pplist bport
	;

bport	: LITERAL bdir NUMBER			{ VerBlockPort($1,$2,$3); }
	;

bdir	: GT					{ $$ = 2; }
	| LT					{ $$ = 3; }
	| ASGN					{ $$ = 1; }
	;
	
bdplist	: 
	| bdplist bdport
	;

bdport	: LITERAL bdir LITERAL bwidth LPAREN NUMBER SLASH NUMBER RPAREN
			{ VerBDPort($1,$2,$3,$4,$6,$8); }
	;

/*****************************************************************************
 * Symbol definition
 *****************************************************************************/
modsymbol : ANOTATE SYMBOLBEGIN COLON NUMBER ENDANOTATE { VerSymbol($4); }
	  mslines
	  ANOTATE END ENDANOTATE			{ VerSymbol(0); }
	;

mslines	: msline
	| mslines msline
	;

msline	: ANOTATE ICONDATA LITERAL NUMBER NUMBER NUMBER ENDANOTATE	{ VerSetIcon($3,$4,$5,$6); }
	| ANOTATE DATA STRING ENDANOTATE 				{ VerIconData($3); }
	| ANOTATE ICONEND ENDANOTATE					{ VerSetIcon(0,0,0,0); }
	| ANOTATE PORTDEF portdef ENDANOTATE
	;

portdef	: LITERAL LITERAL bwidth
		AT LPAREN NUMBER COMMA NUMBER RPAREN
		ROT COLON NUMBER					{ VerSymPort($1,$2,$3,$6,$8,$12); }
	;


/******************************************************************************/
/* Delay file specifications */

tech_def : TECHNOLOGY LITERAL { tech_name = $2; } LBRACE primdefs RBRACE { tech_name = "default"; }
	| TECHNOLOGY DEFAULT LBRACE primdefs RBRACE 
	;

primdefs :
	 | primdefs primdef
	 | primdefs SEMI
	 ;

primdef	: PRIMITIVE LITERAL { VerBeginDelayDef(tech_name,$2); } LBRACE pstats RBRACE { VerEndDelayDef(); }
	;

pstats	: 
	| pstats pstat
	;

pstat	: AREA ASGN pexpr SEMI { PrimSet("area",0,$3); }
	| AREA ASGN pcode semi { PrimSet("area",0,$3); }
	| POWER ASGN pexpr SEMI { PrimSet("power",0,$3); }
	| POWER ASGN pcode semi { PrimSet("power",0,$3); }
	| DELAY dparm ASGN pexpr SEMI { PrimSet("delay",$2,$4); }
	| DELAY dparm ASGN pcode semi { PrimSet("delay",$2,$4); }
	;

/*
 * Delay parameter specifier 
 */
dparm	: LT { BeginDDP(); } LITERAL GT {  BeginDD(); $$ = $3; }
	;

pexpr	: pexpr ADD pexpr		{ $$ = Expr_op(ADD,$1,$3); }
	| pexpr SUB pexpr		{ $$ = Expr_op(SUB,$1,$3); }
	| pexpr MUL pexpr		{ $$ = Expr_op(MUL,$1,$3); }
	| pexpr DIV pexpr		{ $$ = Expr_op(DIV,$1,$3); }
	| pexpr AND pexpr		{ $$ = Expr_op(AND,$1,$3); }
	| pexpr POW pexpr		{ $$ = Expr_op(POW,$1,$3); }
	| pexpr OR pexpr		{ $$ = Expr_op(OR,$1,$3); }
	| pexpr GT pexpr		{ $$ = Expr_op(GT,$1,$3); }
	| pexpr LT pexpr		{ $$ = Expr_op(LT,$1,$3); }
	| pexpr GE pexpr		{ $$ = Expr_op(GE,$1,$3); }
	| pexpr LE pexpr		{ $$ = Expr_op(LE,$1,$3); }
	| pexpr EQ pexpr		{ $$ = Expr_op(EQ,$1,$3); }
	| pexpr NE pexpr		{ $$ = Expr_op(NE,$1,$3); }
	| NOT pexpr			{ $$ = Expr_op(NOT,0,$2); }
	| SUB pexpr	%prec UNEG	{ $$ = Expr_op(UNEG,0,$2); }
	| LPAREN pexpr RPAREN		{ $$ = $2; }
	| LITERAL LPAREN pexpr RPAREN	{ $$ = Expr_func($1,$3,0); }
	| LITERAL			{ $$ = Expr_lit($1); }
	| NUMBER			{ $$ = Expr_num($1); }
	;

pcode	: LBRACE pc_stats RBRACE	{ $$ = $2; }
	;

pc_stats :				{ $$ = 0; }
	| pc_stats pc_stat		{ $$ = Expr_op(NEXT,$1,$2); }
	;

pc_stat	: LITERAL ASGN pexpr SEMI			{ $$ = Expr_op(ASGN,Expr_lit($1),$3); }
	| IF LPAREN pexpr RPAREN pc_stat		{ $$ = Expr_op3(IF,$3,$5,0); }
	| IF LPAREN pexpr RPAREN pc_stat ELSE pc_stat	{ $$ = Expr_op3(IF,$3,$5,$7); }
	| LBRACE pc_stats RBRACE			{ $$ = $2; }
	| BREAK	SEMI					{ $$ = Expr_op(BREAK,0,0); }
	| CASE NUMBER COLON				{ $$ = Expr_case(CASE,$2); }
	| CASE SUB NUMBER COLON				{ $$ = Expr_case(CASE,-$3); }
	| DEFAULT COLON					{ $$ = Expr_case(DEFAULT,0); }
	| RETURN pexpr SEMI				{ $$ = Expr_op(RETURN,$2,0); }
	| SWITCH LPAREN pexpr RPAREN pc_stat		{ $$ = Expr_op(SWITCH,$3,$5); }
	;

semi	:
	| SEMI
	;

%%
