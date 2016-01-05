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

    Last edit by hansen on Wed Jan  7 14:51:23 2004
****************************************************************************/
%token <I> MICROCODE MACROCODE MAP

%token BANK FIELD REGISTERS OPERANDS OP KWBEGIN KWEND NL

%token SEMI COLON COMMA ASGN HASH TILDE PERCENT PLUS AT BOGOCHAR MINUS
%token LBRACE RBRACE LPAREN RPAREN LBRACK RBRACK

%token <I> DSHORT DLONG DBYTE DSYMBOL DBSS DPROC DEND

%token <I> NUMBER
%token <S> LITERAL
%token <I> REGISTER
%token <S> STRING

%type <S>  label
%type <I> what dtype register offset opasgn
%type <R> brange obrange
%type <N> number

%{
#include "parser.h"
#include "grammar.h"
%}

%%

prog		: top_item
		| prog top_item
		;

top_item	: bank_decl
		| field_decl
		| reg_decl
		| operand_decl
		| op_decl
		| ucode
		| mcode
		| error SEMI { yyerrok; }
		| error KWEND { yyerrok; }
		;

/***********************************************************************
 * Bank and map declarations
 ***********************************************************************/
bank_decl	: what BANK brange LITERAL SEMI 	{ ycBank($1,&$3,$4,0); }
		| what BANK brange LITERAL brange SEMI	{ ycBank($1,&$3,$4,&$5); }
		;

/***********************************************************************
 * Field declarations
 ***********************************************************************/
field_decl	: FIELD fitems SEMI
		;

fitems		: fitem
		| fitems COMMA fitem
		;

fitem		: TILDE LITERAL brange			{ ycField($2,&$3,1); }
		| LITERAL brange			{ ycField($1,&$2,0); }
		| LITERAL brange ASGN { ycField($1,&$2,0); } enumset		
		;

enumset		: LBRACE eitems RBRACE
		;

eitems		: eitem
		| eitems COMMA eitem
		;

eitem		: LITERAL ASGN NUMBER			{ ycEnum($1,$3); }
		;

/***********************************************************************
 * Register declarations
 ***********************************************************************/
reg_decl	: REGISTERS regs SEMI
		;

regs		: reg
		| regs COMMA reg
		;

reg		: LITERAL ASGN NUMBER			{ ycRegRecord($1,$3); }
		;
/***********************************************************************
 * Operand declarations
 ***********************************************************************/
operand_decl	: OPERANDS LITERAL LBRACE { ycBeginOprGroup($2); } operdefs
			{ ycEndOprGroup(); } RBRACE SEMI
		;

operdefs	: 
		| operdefs operdef
		;

operdef		: {ycBeginOperand();} oprlhss ASGN LBRACE opasgns RBRACE SEMI {ycEndOperand();}
		| {ycBeginOperand();} MINUS ASGN LBRACE opasgns RBRACE SEMI {ycEndOperand();}
			
		;

oprlhss		: oprlhs
		| oprlhss COMMA oprlhs
		;

oprlhs		: PERCENT NUMBER				 { ycOLHS(AM_DIRECT,$2,0); }
		| HASH NUMBER					 { ycOLHS(AM_IMMEDIATE,0,$2); }
		| LPAREN PERCENT NUMBER RPAREN			 { ycOLHS(AM_INDIRECT,$3,0); }
		| HASH NUMBER LPAREN PERCENT NUMBER RPAREN	 { ycOLHS(AM_INDEXED,$5,$2); }
		| LPAREN HASH NUMBER RPAREN			 { ycOLHS(AM_IMMIND,0,$3); }
		;

opasgns		: 
		| opasgns opasgn SEMI
		;

opasgn		: PLUS NUMBER obrange ASGN NUMBER obrange		{ ycORHS(OI_DATA,$2,&$3,$5,&$6,0); $$ = $5; }
		| PLUS NUMBER obrange ASGN HASH NUMBER offset obrange	{ ycORHS(OI_RELNUMOP,$2,&$3,$6,&$8,$7); $$ = 0; }
		| PLUS NUMBER obrange ASGN HASH NUMBER obrange		{ ycORHS(OI_NUMOP,$2,&$3,$6,&$7,0); $$ = 0; }
		| PLUS NUMBER obrange ASGN PERCENT NUMBER obrange	{ ycORHS(OI_REGOP,$2,&$3,$6,&$7,0); $$ = 0; }
		;

offset 		: AT NUMBER	{ $$ = $2; }
		| AT		{ $$ = 0; }
		;

/***********************************************************************
 * Opcode declarations
 ***********************************************************************/
op_decl		: OP LITERAL LBRACE { ycBeginOp($2); } opditems { ycEndOp(); } RBRACE SEMI
		;

opditems	: 
		| opditems opditem
		;

opditem		: MAP number COLON number SEMI		{ ycMap(&$2,&$4); }
		| MAP number COLON opasgn SEMI		{ Number N; N.v.d = $4; N.ntype = NT_VALUE; ycMap(&$2,&N); }
		| opasgn SEMI { }
		| OPERANDS LITERAL SEMI { ycAddOprGroup($2); }
		| OPERANDS LBRACE  { ycBeginOprGroup(0); } operdefs
			{ ycEndOprGroup(); } RBRACE SEMI
		;

/***********************************************************************
 * Microcode declarations
 ***********************************************************************/
ucode		: KWBEGIN MICROCODE AT NUMBER { ycBeginUCode($4); } uops KWEND { ycEndUCode(); }
		;

uops		:
		| uops uop { ycUNext(); }
		;

uop		: uspecs SEMI
		| label { ycULabel($1); } uspecs SEMI
		| error SEMI { yyerrok; }
		;

uspecs		: uspec
		| uspecs uspec
		;

uspec		: number			{ ycUSpec(&$1,0); }
		| number ASGN number		{ ycUSpec(&$1,&$3); }
		;


/***********************************************************************
 * Macrocode declarations
 ***********************************************************************/

mcode		: KWBEGIN MACROCODE AT NUMBER { BeginMA(); ycBeginMCode($4); } NL
			mops KWEND { ycEndMCode(); BeginBA(); }
		;

mops		: 
		| mops lmop NL { ycMNext(); }
		| error NL { yyerrok; }
		;

lmop		: mop
		| label { ycMLabel($1); } mop
		| label DSYMBOL NUMBER { ycMSymbol($1,$3); }
		| DPROC LITERAL { ycBeginProc($2); }
		| DEND { ycEndProc(); }
		;

mop		: LITERAL { ycMOp($1); } operands		
		| LITERAL { ycMOp($1); }
		| dtype { ycData($1); } bsl_items
		| DBSS NUMBER { ycBss($2); }
		|
		;

dtype		: DBYTE		 { $$ = $1; }
		| DSHORT	 { $$ = $1; }
		| DLONG		 { $$ = $1; }
		;

bsl_items	: bsl_item
		| bsl_items COMMA bsl_item
		;

bsl_item	: NUMBER	{ ycDataNum($1); }
		| LITERAL	{ ycDataLit($1); }
		| STRING	{ ycDataStr($1); }
		;

operands	: operand
		| operands COMMA operand
		;

operand		: number				{ ycMOperand(AM_IMMEDIATE,Number_copy(&$1),0); }
		| register				{ ycMOperand(AM_DIRECT,0,$1); }
		| LPAREN register RPAREN		{ ycMOperand(AM_INDIRECT,0,$2); }
		| number LPAREN register RPAREN		{ ycMOperand(AM_INDEXED,Number_copy(&$1),$3); }
		| LPAREN number RPAREN			{ ycMOperand(AM_IMMIND,Number_copy(&$2),0); }
		;

number		: HASH NUMBER	{ $$.ntype = NT_VALUE; $$.v.d = $2; }
		| NUMBER	{ $$.ntype = NT_VALUE; $$.v.d = $1; }
		| HASH LITERAL	{ $$.ntype = NT_SYMBOL; $$.v.s = $2; }
		| LITERAL	{ $$.ntype = NT_SYMBOL; $$.v.s = $1; }
		;		

/***********************************************************************
 * Generic declarations
 ***********************************************************************/
brange		: LBRACK NUMBER COLON NUMBER RBRACK	{ $$.msb = $2; $$.lsb = $4; }
		| LBRACK NUMBER RBRACK			{ $$.msb = $$.lsb = $2; }
		;

obrange		:					{ $$.msb = $$.lsb = -1; }
		| brange				{ $$ = $1; }
		;

what		: MICROCODE	{ $$ = $1; }
		| MACROCODE	{ $$ = $1; }
		| MAP		{ $$ = $1; }
		;

label		: LITERAL COLON				{ $$ = $1; }
		;

register	: REGISTER				{ $$ = $1; }
		| PERCENT REGISTER			{ $$ = $2; }
		;
