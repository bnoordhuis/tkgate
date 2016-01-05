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

%token INPUT OUTPUT INOUT WIRE REG SUPPLY0 SUPPLY1 INTEGER
%token MODULE ENDMODULE PRIMITIVE ENDPRIMITIVE TASK ENDTASK
%token DEASSIGN ASSIGN INITIALB ALWAYS PARAMETER AUTOMATIC
%token BBEGIN END IF ELSE CASE CASEX CASEZ ENDCASE DEFAULT
%token REPEAT FOR WHILE FORK JOIN WAIT FOREVER
%token POSEDGE NEGEDGE MPATH DPATH TRIGGER FUNCTION ENDFUNCTION
%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK
%token SEMI COLON COMMA DOT ASGN AT HASH REPCAT
%token NOT GT LT EQ EQZ NEZ BAND BNAND BOR BNOR BXOR BNXOR UINV MOD
%token ARSHIFT ALSHIFT  LSHIFT RSHIFTEQ QUEST SPECIFY ENDSPECIFY SPECPARAM
%token SMALL MEDIUM LARGE TRI TRI0 TRI1 WAND WOR TRIREG SIGNED SCALAR VECTORED
%token REAL EVENT TIME TRIAND TRIOR
%token STRONG0 STRONG1 PULL0 PULL1 WEAK0 WEAK1 HIGHZ0 HIGHZ1

%token <I> CMOS RCMOS BUFIF0 BUFIF1 NOTIF0 NOTIF1 NMOS PMOS RNMOS RPMOS 
%token <I> PRIMAND PRIMNAND PRIMNOR PRIMXOR PRIMXNOR BUF PRIMNOT
%token <I> TRANIF0 TRANIF1 RTRANIF0 RTRANIF1 RTRAN KWOR TRAN COND

%token BREAKPT SCRIPT

 /*
  * Markers for embedded scripts
  */
%token <S> BEGINSCRIPT
%token ENDSCRIPT

/*
 * Tokens that are not directly used in the parser, but are generated to create
 * unique numeric codes.
 */
%token BOGUS BOGOCHAR INSTANCE MEMORY

%token <I> NUMBER
%token <S> HEX 
%token <S> SYSLITERAL LITERAL HLITERAL STRING HDLLINE
%token <F> REALNUM

%type <S> dlit decltail dlits
%type <I> casekw cmos_gtype mos_gtype inx_gtype outx_gtype bif_gtype tran_gtype trif_gtype	


%type <I> net_type reg_type xreg_type port_type capsize netattrs netattr size0 size1 ptype oautomatic
%type <E> expr bval lval lvals catexprs delay dexpr odelay econd trigger triggers event catexpr starg
%type <R> orange range 
%type <L> caseents stats miports miexprs miasgns exprs oexprs otgparms tgparms 
%type <CE> caseent
%type <SD> stat if_stat case_stat for_stat while_stat wait_stat raise_stat repeat_stat fork_stat asgn_stat task_stat
%type <SD> forever_stat

//%type <I> bwidth obang bdir nexpr
//%type <S> dparm clit
//%type <E> pexpr pcode pc_stats pc_stat hexpr


%left QUEST COLON
%left OR
%left AND
%left BOR BNOR
%left BXOR BNXOR
%left BAND BNAND
%left EQ NE EQZ NEZ
%left GT LT GE LE
%left LSHIFT RSHIFT ALSHIFT ARSHIFT
%left ADD SUB
%left MUL DIV MOD
%left POW
%left UNEG UPLUS NOT UINV

%{
#include "thyme.h"

void BeginPV();		/* Pure verilog */
void BeginLC();
void BeginBC();

%}

%%

/*****************************************************************************
 *
 * Top-level declarations
 *
 * Examples:
 *
 *    module foo(...); .... endmodule
 *
 *****************************************************************************/
prog	: 
	| punits
	| BREAKPT NUMBER COLON expr SEMI { VerBreakpoint($2,$4); }
	| SCRIPT script
	;

punits	: punit
	| punits punit
	;

punit	: module
	;

module	: modhead mitems ENDMODULE { VerEndModule(); }
	| error ENDMODULE
	;

/*****************************************************************************
 *
 * Parsing for a script file.
 *
 *****************************************************************************/
script	: sitems
	;

sitems	:
	| sitems sitem
	;

sitem	: decl
	| initial
	| always
	| assign
	| parameter
	| usertask
	| userfunc
	| error SEMI
	;


/*****************************************************************************
 *
 * Module header declaration
 *
 * Examples:
 *
 *    module foo;
 *    module bar(a,b,c,z);
 *    module beep();
 *    module #(delay1, delay2) bam(z,a,b,c);
 *
 *****************************************************************************/
modhead	: MODULE LITERAL { VerNewModule($2); } omparmdecls omargs SEMI
	;

omparmdecls : 
	    | HASH LPAREN mparmdecls RPAREN
	    ;

mparmdecls  : mparmdecl
	    | mparmdecls COMMA mparmdecl
	    ;


mparmdecl   : DOT LITERAL LPAREN expr RPAREN	{ VerParmDef($2,$4,1); }
	    | LITERAL ASGN expr			{ VerParmDef($1,$3,1); }
	    ;

omargs	: 
	| LPAREN RPAREN
	| LPAREN margs RPAREN
	;


margs	: LITERAL				{ VerPort($1); }
	| margs COMMA LITERAL			{ VerPort($3); }
	;

/*****************************************************************************
 *
 * Module items are things that can be in a module at the top level
 *
 * Examples:
 *
 *    wire w1,w2;
 *    initial ck = 0;
 *    always @(posedge CLK) A = A + 1'b1;
 *    assign w = a & b;
 *    AND2 a1(a,b,z);
 *
 *
 *****************************************************************************/
mitems	:
	| mitems mitem
	;

mitem	: decl
	| initial
	| always
	| assign
	| instance
	| gate
	| parameter
	| specblock
	| usertask
	| userfunc
	| escript
	| error SEMI
	;

assign	: ASSIGN lval ASGN expr SEMI		{ VerAssign($2,$4,0); }
	| ASSIGN delay lval ASGN expr SEMI	{ VerAssign($3,$5,$2); }
	;

parameter : PARAMETER LITERAL ASGN expr SEMI	{ VerParmDef($2,$4,0); }
	;

initial	: INITIALB stat				{ VerIABlock(INITIALB,$2); }
	;

always	: ALWAYS stat				{ VerIABlock(ALWAYS,$2); }
	;



/*****************************************************************************
 *
 * Embedded script - Used for "script" objects.
 *
 *****************************************************************************/
escript	: BEGINSCRIPT  { VerBeginEScript($1); } esitems ENDSCRIPT  { VerEndEScript(); } 
	| error ENDSCRIPT
	;

esitems	:
	| esitems esitem
	;

/*
 * Should be the same as mitem but w/o error or escript branches
 */
esitem	: decl
	| initial
	| always
	| assign
	| instance
	| gate
	| parameter
	| specblock
	| usertask
	| userfunc
	;


/*****************************************************************************
 *
 * Primitive gate instantiations
 *
 * Exmples:
 *
 *   and g1(a,b,c), g2(q,s,d);
 *   xor g3[7:0] (a,b,c);
 *   nor  (a,b,c);
 *   tran  (a,b);
 *
 *****************************************************************************/
gate	: inx_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	| outx_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	| cmos_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	| mos_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	| bif_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	| tran_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	| trif_gtype odelay { VerGateDecl($1,$2); } ginsts SEMI
	;

inx_gtype	: PRIMAND | PRIMNAND | PRIMNOR | PRIMXOR | PRIMXNOR | KWOR ;
outx_gtype	: BUF | PRIMNOT ;
cmos_gtype	: CMOS | RCMOS ;
mos_gtype	: NMOS | PMOS | RNMOS | RPMOS ;
bif_gtype	: BUFIF0 | BUFIF1 | NOTIF0 | NOTIF1 ;
tran_gtype	: TRAN | RTRAN ;
trif_gtype	: TRANIF0 | TRANIF1 | RTRANIF0 | RTRANIF1 ;

ginsts		: ginst
		| ginsts COMMA ginst
		;

ginst		: LITERAL orange LPAREN exprs RPAREN		{ VerGateInst($1,$2,$4); }
		| LPAREN exprs RPAREN				{ VerGateInst(0,0,$2); }
		;


/*****************************************************************************
 *
 * Module instantiations
 *
 * Exmples:
 *
 *   foo f1(a,b,c), f2(d,e,f);
 *   foo f1(.p1(a),.p2(b),.p3(c));
 *   foo f1[2:0](.p1(a),.p2(b),.p3(c));
 *   foo #(4, 5) f1[2:0](.p1(a),.p2(b),.p3(c));
 *   foo #(.delay1(4), .delay2(5)) f1[2:0](.p1(a),.p2(b),.p3(c));
 *
 *****************************************************************************/
instance : LITERAL { VerModDecl($1); } omparmsets minsts SEMI
	;

omparmsets : HASH LPAREN mparmsets RPAREN
	   | 
	   ;


mparmsets : mpexprs
/*	  | mpasgns*/
	  ;

mpexprs : expr						{ VerModDeclParm(0,$1); }
	| DOT LITERAL LPAREN expr RPAREN		{ VerModDeclParm($2,$4); }
	| mpexprs COMMA expr				{ VerModDeclParm(0,$3); }
	| mpexprs COMMA DOT LITERAL LPAREN expr RPAREN	{ VerModDeclParm($4,$6); }
	;

/*
mpasgns	: DOT LITERAL LPAREN expr RPAREN		{ VerModDeclParm($2,$4); }
	| mpasgns COMMA DOT LITERAL LPAREN expr RPAREN	{ VerModDeclParm($4,$6); }
	;
*/

minsts	: minst
	| minsts COMMA minst
	;

minst	: LITERAL orange LPAREN miports RPAREN		{ VerModInst($1,$2,$4); }
	| LITERAL orange LPAREN RPAREN			{ VerModInst($1,$2,VerEmptyList()); }
	| LITERAL orange				{ VerModInst($1,$2,VerEmptyList()); }
	;

miports	: miexprs 
	| miasgns
	;


miexprs : expr						{ $$ = VerListAppend(VerEmptyList(),new_NameExpr(0,$1)); }
	| miexprs COMMA expr				{ $$ = VerListAppend($1,new_NameExpr(0,$3)); }
	;

miasgns	: DOT LITERAL LPAREN expr RPAREN		{ $$ = VerListAppend(VerEmptyList(),new_NameExpr($2,$4)); }
	| miasgns COMMA DOT LITERAL LPAREN expr RPAREN	{ $$ = VerListAppend($1,new_NameExpr($4,$6)); }
	;



/*****************************************************************************
 *
 * Declarations are used to define nets, registers or ports.
 *
 * Examples:
 *
 *    wire [7:0] w1, w2;
 *    reg [7:0] r1;
 *    reg [7:0] mem[0:65535];
 *    input a,b,c;
 *    wire x = #10 b | c;
 *
 *****************************************************************************/
decl	: net_type orange { VerBeginDecls($1,$2); } decltail
	| net_type orange delay { VerBeginDecls($1,$2); } dlit ASGN expr SEMI { VerAssign(new_Expr_lit($5),$7,$3); }
	| reg_type orange { VerBeginDecls($1,$2); } decltail
	| xreg_type { VerBeginDecls($1,0); } decltail
	| port_type orange { VerBeginDecls($1,$2); } decltail
	| net_type LBRACK MUL RBRACK LITERAL ASGN expr SEMI { VerAutoAssign($1,$5,$7); }
	;

decltail : dlit SEMI
	| dlit COMMA dlits SEMI
	| dlit ASGN expr SEMI { VerDeclAssign(new_Expr_lit($1),$3,0); }
	| dlit ASGN delay expr SEMI { VerDeclAssign(new_Expr_lit($1),$4,$3); }
	;



dlits		: dlit
		| dlits COMMA dlit
		;

dlit		: LITERAL { VerDecl($1,0); $$ = $1; }
		| LITERAL range { VerDecl($1,$2); $$ = $1; }
		;

port_type	: INPUT				{ $$ = NT_INPUT; }
		| OUTPUT			{ $$ = NT_OUTPUT; }
		| INOUT				{ $$ = NT_INOUT; }
		| OUTPUT REG			{ $$ = NT_REG_OUTPUT; }
		| REG OUTPUT			{ $$ = NT_REG_OUTPUT; }
		;

net_type	: WIRE netattrs			{ $$ = NT_WIRE|$2; }
		| WAND netattrs			{ $$ = NT_WAND|$2; }
		| WOR netattrs			{ $$ = NT_WOR|$2; }
		| TRI netattrs			{ $$ = NT_TRI|$2; }
		| TRI1 netattrs			{ $$ = NT_TRI1|$2; }
		| TRI0 netattrs			{ $$ = NT_TRI0|$2; }
		| TRIAND netattrs		{ $$ = NT_TRIAND|$2; }
		| TRIOR	netattrs		{ $$ = NT_TRIOR|$2; }
		| TRIREG netattrs		{ $$ = NT_TRIREG|$2; }
		| SUPPLY0 netattrs		{ $$ = NT_SUPPLY0|$2; }
		| SUPPLY1 netattrs		{ $$ = NT_SUPPLY1|$2; }
		;

reg_type	: REG netattrs			{ $$ = NT_REG|$2; }
		;

xreg_type	: INTEGER			{ $$ = NT_INTEGER; }
		| REAL				{ $$ = NT_REAL; }
		| TIME				{ $$ = NT_TIME; }
		| EVENT				{ $$ = NT_EVENT; }
		;

netattrs	:				{$$ = 0; }
		| netattrs netattr		{$$ = $1|$2; }
		;

netattr		: VECTORED			{ $$ = NT_P_VECTORED; }
		| SCALAR			{ $$ = NT_P_SCALAR; }
		| SIGNED			{ $$ = NT_P_SIGNED; }
		| LPAREN capsize RPAREN		{ $$ = $2; }
		| LPAREN size0 COMMA size1 RPAREN { $$ = $2|$4; }
		| LPAREN size1 COMMA size0 RPAREN { $$ = $2|$4; }
		;

capsize		: SMALL				{ $$ = NT_P_SMALL; }
		| MEDIUM			{ $$ = NT_P_MEDIUM; }
		| LARGE				{ $$ = NT_P_LARGE; }
		;


size0		: SUPPLY0			{ $$ = NT_P_SUPPLY0; }
		| STRONG0			{ $$ = NT_P_STRONG0; }
		| PULL0				{ $$ = NT_P_PULL0; }
		| WEAK0				{ $$ = NT_P_WEAK0; }
		| HIGHZ0			{ $$ = NT_P_HIGHZ0; }
		;
size1		: SUPPLY1			{ $$ = NT_P_SUPPLY1; }
		| STRONG1			{ $$ = NT_P_STRONG1; }
		| PULL1				{ $$ = NT_P_PULL1; }
		| WEAK1				{ $$ = NT_P_WEAK1; }
		| HIGHZ1			{ $$ = NT_P_HIGHZ1; }
		;

orange	: 					{ $$ = VerRange(RS_MAXMIN,new_Expr_num(0),new_Expr_num(0)); }
	| range					{ $$ = $1; }
	;

range	: LBRACK expr COLON expr RBRACK		{ $$ = VerRange(RS_MAXMIN,$2,$4); }
	| LBRACK expr ADD COLON expr RBRACK	{ $$ = VerRange(RS_BASEUP,$2,$5); }
	| LBRACK expr SUB COLON expr RBRACK	{ $$ = VerRange(RS_BASEDN,$2,$5); }
	;

/*****************************************************************************
 *
 * Specify blocks
 *
 * Examples:
 *    specify
 *      specparam ABtXY = 23;
 *      (a,b) *> (x,y) = ABtXY;
 *      if (c == 1)
 *        (a,b) *> (z) = 43;
 *      if (c != 1)
 *        (a,b) *> (z) = 88;
 *      $hold(posedge ck, d, 12);
 *      $setup(d, posedge ck &&& (clr == 1'b0), 12);
 *    endspecify
 *
 *****************************************************************************/
specblock	: SPECIFY specitems ENDSPECIFY
		;

specitems	:
		| specitems specitem
		;

specitem	: SYSLITERAL LPAREN otgparms RPAREN SEMI			{ VerSpecTask($1,$3); }
		| LPAREN lvals ptype lvals RPAREN ASGN expr SEMI 		{ VerSpecify(0,$3,$2,$4,$7); }
		| IF LPAREN expr RPAREN LPAREN lvals ptype lvals RPAREN
			 ASGN expr SEMI						{ VerSpecify($3,$7,$6,$8,$11); }
		| SPECPARAM LITERAL ASGN expr SEMI				{ VerSpecParam($2,$4); }
		;

ptype		: MPATH								{ $$ = SPT_MPATH; }
		| DPATH								{ $$ = SPT_DPATH; }
		;


starg		: triggers							{ $$ = $1; }
		| triggers COND expr						{ $$ = new_Expr_op(E_COND,$1,$3); }
		;

tgparms		: starg								{ $$ = VerListAppend(0,$1); }
		| tgparms COMMA starg						{ $$ = VerListAppend($1,$3); }
		;

otgparms	:								{ $$ = 0; } 
		| tgparms							{ $$ = $1; }
		;
	
/*****************************************************************************
 *
 * Enabling conditions
 *
 * Examples:
 *
 *    # 10
 *    # (10 + 5)
 *    @ (A)
 *    @ (A or posedge CLK)
 *
 *****************************************************************************/
econd	: event			{ $$ = $1; }
	| delay 		{ $$ = $1; }
	;

event	: AT LPAREN triggers RPAREN	{ $$ = new_Expr_op1(E_AT,$3); }
	| AT LITERAL			{ $$ = new_Expr_op1(E_AT,new_Expr_lit($2)); }
	| AT MUL			{ $$ = new_Expr_op1(E_AT,0); }
	| AT LPAREN MUL RPAREN		{ $$ = new_Expr_op1(E_AT,0); }
	;

delay	: HASH dexpr			{ $$ = new_Expr_op1(E_DELAY,$2); }
	| HASH dexpr COLON dexpr COLON dexpr { $$ = new_Expr_op3(E_DELAY,$2,$4,$6); }
	;

dexpr	: LPAREN expr RPAREN		{ $$ = $2; }
	| LITERAL			{ $$ = new_Expr_lit($1); }
	| NUMBER			{ $$ = new_Expr_num($1); }
	| REALNUM			{ $$ = new_Expr_realnum($1); }
	;

odelay	: delay				{ $$ = $1; }
	|				{ $$ = 0; }
	;

triggers : trigger 			{ $$ = $1; }
	| triggers KWOR trigger		{ $$ = new_Expr_op(E_EVENTOR,$1,$3); }
	;

trigger	: expr				{ $$ = $1; }
	| POSEDGE expr			{ $$ = new_Expr_op1(E_POSEDGE,$2); }
	| NEGEDGE expr			{ $$ = new_Expr_op1(E_NEGEDGE,$2); }
	;

/*****************************************************************************
 *
 * User function declarations
 *
 * Examples:
 *     function name(a,b, ..., z); ... endfunction 
 *     function [7:0] name(a,b, ..., z); ... endfunction 
 *     function name(input a,input [3:0] b, ..., input z); ... endfunction 
 *     automatic function name(a,b, ..., z); ... endfunction 
 *
 *****************************************************************************/
userfunc	: oautomatic FUNCTION orange LITERAL
			{ VerBeginTask($4,$1); VerBeginDecls(NT_OUTPUT|NT_P_REG,$3); VerDecl($4,0); VerTaskToFunc($3); }
			SEMI taskdecls stat ENDFUNCTION { VerEndTask($8); }
		| oautomatic FUNCTION orange LITERAL
			{ VerBeginTask($4,$1); VerBeginDecls(NT_OUTPUT|NT_P_REG,$3); VerDecl($4,0); VerTaskToFunc($3); }
			LPAREN taskprotos RPAREN SEMI
		   taskdecls stat ENDFUNCTION { VerEndTask($11); } 
		;

/*****************************************************************************
 *
 * User task declarations
 *
 * Examples:
 *     task name(a,b, ..., z); ... endfunction 
 *     task name(input a,input [3:0] b, ..., output z); ... endfunction 
 *     automatic task name(a,b, ..., z); ... endfunction 
 *
 *****************************************************************************/
usertask	: oautomatic TASK LITERAL { VerBeginTask($3,$1); } SEMI taskdecls stat ENDTASK { VerEndTask($7); } 
		| oautomatic TASK LITERAL { VerBeginTask($3,$1); } LPAREN taskprotos RPAREN SEMI
		   taskdecls stat ENDTASK { VerEndTask($10); } 
		;

taskdecls	:
		| taskdecls taskdecl
		;

taskdecl	: port_type orange { VerBeginDecls($1|NT_P_REG,$2); } dlits SEMI
		| net_type orange { VerBeginDecls($1,$2); } dlits SEMI
		| reg_type orange { VerBeginDecls($1,$2); } dlits SEMI
		| xreg_type { VerBeginDecls($1,0); } dlits SEMI
		;

taskprotos	: taskproto
		| taskprotos COMMA taskproto
		;

taskproto	: dlit
		| port_type orange { VerBeginDecls($1|NT_P_REG,$2); } dlit
		; 

oautomatic	: AUTOMATIC	{ $$ = 1; }
		|		{ $$ = 0; }
		;

/*****************************************************************************
 *
 * Statements including begin end blocks.  Note that the LE token "<=" doubles
 * as a non-blocking assignment.
 *
 * Examples:
 *    a = 4 + c;
 *    if (a > c) u = u + 1;
 *    while (n < 10) begin a = a + n; n = n + 1; end
 *
 *****************************************************************************/

stat	: BBEGIN stats END { $$ = new_SDBlock(0,$2); }
	| BBEGIN COLON LITERAL stats END { $$ = new_SDBlock($3,$4); }
	| asgn_stat SEMI
	| lval ASGN econd expr SEMI { $$ = new_SDAsgn($1,$4,0,1); }
	| lval LE expr SEMI { $$ = new_SDAsgn($1,$3,0,0); }
	| lval LE econd expr SEMI { $$ = new_SDAsgn($1,$4,$3,0); }
	| task_stat
	| if_stat
	| case_stat
	| for_stat
	| while_stat
	| forever_stat
	| repeat_stat
	| fork_stat
	| wait_stat
	| raise_stat
	| SEMI	     { $$ = 0; }
	| econd stat { $$ = VerCondStat($1,$2); }
	| error SEMI { $$ = new_SDNull(); }
	; 

stats	:		{ $$ = VerEmptyList(); }
	| stats stat	{ $$ = VerListAppend($1,$2); }
	;

asgn_stat : lval ASGN expr { $$ = new_SDAsgn($1,$3,0,1); }
	;

/*****************************************************************************
 *
 * if statements
 *
 * Example:
 *
 *    if (A > B)
 *       C = A - B;
 *    else
 *       C = B - A;
 *
 *****************************************************************************/
if_stat	: IF LPAREN expr RPAREN stat		{ $$ = new_SDIf($3,$5,0); }
	| IF LPAREN expr RPAREN stat ELSE stat	{ $$ = new_SDIf($3,$5,$7); }
	;

/*****************************************************************************
 *
 * task invocation statements
 *
 * Example:
 *
 * $display("value of a is: %x",a);
 *
 *****************************************************************************/
task_stat : LITERAL LPAREN oexprs RPAREN SEMI	{ $$ = new_SDTask($1,$3); }
	  | SYSLITERAL LPAREN oexprs RPAREN SEMI { $$ = new_SDTask($1,$3); }
	  | LITERAL SEMI { $$ = new_SDTask($1,0); }
	  | SYSLITERAL SEMI { $$ = new_SDTask($1,0); }
	  ;

/*****************************************************************************
 *
 * case statements
 *
 * Example:
 *
 *    case (S)
 *      3'b000:   S = 3'b110;
 *      3'b001:   S = 3'b010;
 *      3'b1??:   S = 3'b100;
 *      default:  S = 3'b000;
 *    endcase
 *
 *****************************************************************************/
case_stat : casekw LPAREN expr RPAREN caseents ENDCASE { $$ = new_SDCase($1,$3,$5); }
	;

casekw	: CASE  { $$ = CASE; }
	| CASEX { $$ = CASEX; }
	| CASEZ { $$ = CASEZ; }
	;

caseents :			{ $$ = VerEmptyList(); }
	| caseents caseent	{ $$ = VerListAppend($1,$2); }
	;

caseent	: expr COLON stat	{ $$ = new_SDCaseEnt($1,$3); }
	| DEFAULT COLON stat	{ $$ = new_SDCaseEnt(0,$3); }
	;

/*****************************************************************************
 *
 * for statements
 *
 * Example:
 *
 *    for (i = 0; i < 10;i = i + 1)
 *      B = B + M[i];
 *
 *****************************************************************************/
for_stat : FOR LPAREN asgn_stat SEMI expr SEMI asgn_stat RPAREN stat
		{ $$ = new_SDFor($3,$5,$7,$9); }
	;

/*****************************************************************************
 *
 * while statements
 *
 * Example:
 *
 *    while ( A < B)
 *      begin
 *         C = C + A;
 *         A = A + 1;
 *      end
 *
 *****************************************************************************/
while_stat : WHILE LPAREN expr RPAREN stat
		{ $$ = new_SDWhile($3,$5); }
	;

/*****************************************************************************
 *
 * forever statements
 *
 * Example:
 *
 *    forever
 *      #50 CK = !CK;
 *
 *****************************************************************************/
forever_stat : FOREVER stat { $$ = new_SDForever($2); }
	;


/*****************************************************************************
 *
 * wait statements
 *
 * Example:
 *
 *    wait (A < B) B = B - A;
 *
 *****************************************************************************/
wait_stat : WAIT LPAREN expr RPAREN stat
		{ $$ = new_SDWait($3,$5); }
	;

/*****************************************************************************
 *
 * raise (event) statements
 *
 * Example:
 *
 *    -> named_event;
 *
 *****************************************************************************/
raise_stat	: TRIGGER LITERAL SEMI { $$ = new_SDRaise(new_Expr_lit($2)); }
		| TRIGGER HLITERAL SEMI { $$ = new_SDRaise(new_Expr_lit($2)); }
	;

/*****************************************************************************
 *
 * repeat statements
 *
 * Example:
 *
 *    repeat (10) @ (posedge CK);
 *
 *****************************************************************************/
repeat_stat : REPEAT LPAREN expr RPAREN stat
		{ $$ = new_SDRepeat($3,$5); }
	;

/*****************************************************************************
 *
 * fork/join statements
 *
 * Example:
 *
 *    fork
 *      @ (A) X = 2;
 *      @ (B) Y = 4;
 *      @ (C) Z = 1;
 *    join
 *
 *****************************************************************************/
fork_stat : FORK stats JOIN { $$ = new_SDFork($2); }
	;

/*****************************************************************************
 *
 * Expressions and other basic syntax elements
 *
 * Examples:
 *
 *    12
 *    4'h8
 *    x
 *    a | b & c
 *    {a,b}
 *    {4{a}}
 *
 *****************************************************************************/
lval	: bval			 			{ $$ = $1; }
	| LBRACE lvals RBRACE				{ $$ = $2; }
	;

lvals	: lval						{ $$ = $1; }
	| lvals COMMA lval				{ $$ = new_Expr_op(E_CONCAT,$1,$3); }
	;

bval	: LITERAL					{ $$ = new_Expr_lit($1); }
	| HLITERAL					{ $$ = new_Expr_lit($1); }
	| bval LBRACK expr RBRACK			{ $$ = new_Expr_op3(E_VECTORP,$1,$3,0); }
	| bval LBRACK expr ADD COLON expr  RBRACK	{ $$ = new_Expr_op3(E_VECTORP,$1,$3,$6); }
	| bval LBRACK expr SUB COLON expr  RBRACK	{ $$ = new_Expr_op3(E_VECTORN,$1,$3,$6); }
	| bval LBRACK expr COLON expr  RBRACK		{ $$ = new_Expr_op3(E_RANGE,$1,$3,$5); }
	;

expr	: expr ADD expr			{ $$ = new_Expr_op(E_ADD,$1,$3); }
	| expr SUB expr			{ $$ = new_Expr_op(E_SUB,$1,$3); }
	| expr MUL expr			{ $$ = new_Expr_op(E_MUL,$1,$3); }
	| expr DIV expr			{ $$ = new_Expr_op(E_DIV,$1,$3); }
	| expr AND expr			{ $$ = new_Expr_op(E_AND,$1,$3); }
	| expr OR expr			{ $$ = new_Expr_op(E_OR,$1,$3); }
	| expr GT expr			{ $$ = new_Expr_op(E_GT,$1,$3); }
	| expr LT expr			{ $$ = new_Expr_op(E_LT,$1,$3); }
	| expr GE expr			{ $$ = new_Expr_op(E_GE,$1,$3); }
	| expr LE expr			{ $$ = new_Expr_op(E_LE,$1,$3); }
	| expr EQ expr			{ $$ = new_Expr_op(E_EQ,$1,$3); }
	| expr NE expr			{ $$ = new_Expr_op(E_NE,$1,$3); }
	| expr EQZ expr			{ $$ = new_Expr_op(E_EQZ,$1,$3); }
	| expr NEZ expr			{ $$ = new_Expr_op(E_NEZ,$1,$3); }
	| expr RSHIFT expr		{ $$ = new_Expr_op(E_RSHIFT,$1,$3); }
	| expr LSHIFT expr		{ $$ = new_Expr_op(E_LSHIFT,$1,$3); }
	| expr ARSHIFT expr		{ $$ = new_Expr_op(E_ARSHIFT,$1,$3); }
	| expr ALSHIFT expr		{ $$ = new_Expr_op(E_ALSHIFT,$1,$3); }
	| expr MOD expr			{ $$ = new_Expr_op(E_MOD,$1,$3); }
	| expr BAND expr		{ $$ = new_Expr_op(E_BAND,$1,$3); }
	| expr BOR expr			{ $$ = new_Expr_op(E_BOR,$1,$3); }
	| expr BXOR expr		{ $$ = new_Expr_op(E_BXOR,$1,$3); }
	| expr BNAND expr		{ $$ = new_Expr_op(E_BNAND,$1,$3); }
	| expr BNOR expr		{ $$ = new_Expr_op(E_BNOR,$1,$3); }
	| expr BNXOR expr		{ $$ = new_Expr_op(E_BNXOR,$1,$3); }
	| expr QUEST expr COLON expr	{ $$ = new_Expr_op3(E_QUEST,$1,$3,$5); }
	| NOT expr			{ $$ = new_Expr_op1(E_NOT,$2); }
	| UINV expr			{ $$ = new_Expr_op1(E_UINV,$2); }

	| BAND expr	%prec NOT	{ $$ = new_Expr_op1(E_UAND,$2); }
	| BOR expr	%prec NOT	{ $$ = new_Expr_op1(E_UOR,$2); }
	| BXOR expr	%prec NOT	{ $$ = new_Expr_op1(E_UXOR,$2); }
	| BNAND expr	%prec NOT	{ $$ = new_Expr_op1(E_UNAND,$2); }
	| BNOR expr	%prec NOT	{ $$ = new_Expr_op1(E_UNOR,$2); }
	| BNXOR expr	%prec NOT	{ $$ = new_Expr_op1(E_UNXOR,$2); }

	| SUB expr	%prec NOT	{ $$ = new_Expr_op1(E_UNEG,$2); }
	| ADD expr	%prec NOT	{ $$ = $2; }
	| LPAREN expr RPAREN		{ $$ = $2; }
	| bval				{ $$ = $1; }
	| REALNUM			{ $$ = new_Expr_realnum($1); }
	| NUMBER			{ $$ = new_Expr_num($1); }
	| HEX				{ $$ = new_Expr_hex($1); }
	| STRING			{ $$ = new_Expr_str($1); }
	| LBRACE catexprs RBRACE 	{ $$ = $2; }
	| SYSLITERAL 				{ $$ = new_Expr_task($1,0); }
	| SYSLITERAL LPAREN oexprs RPAREN	{ $$ = new_Expr_task($1,$3); }
	| LITERAL LPAREN oexprs RPAREN	{ $$ = new_Expr_task($1,$3); }
	;

catexprs : catexpr			{ $$ = $1; }
	| catexprs COMMA catexpr	{ $$ = new_Expr_op(E_CONCAT,$1,$3); }
	;

catexpr	: expr				{ $$ = $1; }
	| expr LBRACE expr RBRACE	{ $$ = new_Expr_repcat($1,$3); }
	;

exprs	: expr				{ $$ = VerListAppend(0,$1); }
	| exprs COMMA expr		{ $$ = VerListAppend($1,$3); }
	;

oexprs	: 		{ $$ = 0; }
	| exprs		{ $$ = $1; }
	;

%%
