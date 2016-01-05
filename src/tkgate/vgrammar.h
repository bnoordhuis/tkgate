/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NETLISTBEGIN = 258,
     HDLBEGIN = 259,
     BUILTINBEGIN = 260,
     SYMBOLBEGIN = 261,
     XMODULE = 262,
     NETMODULE = 263,
     HDLMODULE = 264,
     BUILTINMODULE = 265,
     ASSIGN = 266,
     ENDMODULE = 267,
     PRIMITIVE = 268,
     TECHNOLOGY = 269,
     DELAY = 270,
     AREA = 271,
     POWER = 272,
     IF = 273,
     ELSE = 274,
     SWITCH = 275,
     CASE = 276,
     CASEX = 277,
     CASEZ = 278,
     RETURN = 279,
     BREAK = 280,
     NEXT = 281,
     FUNC = 282,
     DEFAULT = 283,
     INITIALB = 284,
     INPUT = 285,
     REG = 286,
     OUTPUT = 287,
     INOUT = 288,
     WIRE = 289,
     KWVERSION = 290,
     SUPPLY0 = 291,
     SUPPLY1 = 292,
     KWTRI = 293,
     TRI0 = 294,
     TRI1 = 295,
     TRIREG = 296,
     TRIAND = 297,
     TRIOR = 298,
     WAND = 299,
     WOR = 300,
     DEASSIGN = 301,
     BBEGIN = 302,
     END = 303,
     ENDCASE = 304,
     ALWAYS = 305,
     POSEDGE = 306,
     NEGEDGE = 307,
     INTEGER = 308,
     ENDPRIMITIVE = 309,
     SEMI = 310,
     COLON = 311,
     COMMA = 312,
     DOT = 313,
     ASGN = 314,
     NBASGN = 315,
     LPAREN = 316,
     RPAREN = 317,
     LBRACE = 318,
     RBRACE = 319,
     LBRACK = 320,
     RBRACK = 321,
     AT = 322,
     ANOTATE = 323,
     ENDANOTATE = 324,
     ROT = 325,
     WPLACE = 326,
     SHOWNAME = 327,
     ENDDECLS = 328,
     NOT = 329,
     PORTS = 330,
     PORTDEF = 331,
     BDPORTS = 332,
     SIZE = 333,
     JOINT = 334,
     COMMENT = 335,
     FRAME = 336,
     SLASH = 337,
     BDESC = 338,
     ROOTMODULE = 339,
     TRAN = 340,
     COMLINE = 341,
     COMEND = 342,
     SCRIPT = 343,
     BEGINSCRIPT = 344,
     ENDSCRIPT = 345,
     REQUIRE = 346,
     PROPERTY = 347,
     DECPOS = 348,
     ICONDATA = 349,
     DATA = 350,
     ICONEND = 351,
     SYMBOLREF = 352,
     TEXTBEGIN = 353,
     HASHMARK = 354,
     GT = 355,
     LT = 356,
     EQ = 357,
     BAND = 358,
     BNAND = 359,
     BOR = 360,
     BNOR = 361,
     BXOR = 362,
     BNXOR = 363,
     UINV = 364,
     MOD = 365,
     LSHIFT = 366,
     RSHIFTEQ = 367,
     QUEST = 368,
     ARSHIFT = 369,
     ALSHIFT = 370,
     BOGOCHAR = 371,
     NUMBER = 372,
     HEX = 373,
     VERNUM = 374,
     LITERAL = 375,
     STRING = 376,
     HDLLINE = 377,
     SYSLITERAL = 378,
     OR = 379,
     AND = 380,
     NE = 381,
     LE = 382,
     GE = 383,
     RSHIFT = 384,
     SUB = 385,
     ADD = 386,
     DIV = 387,
     MUL = 388,
     POW = 389,
     UPLUS = 390,
     UNEG = 391
   };
#endif
/* Tokens.  */
#define NETLISTBEGIN 258
#define HDLBEGIN 259
#define BUILTINBEGIN 260
#define SYMBOLBEGIN 261
#define XMODULE 262
#define NETMODULE 263
#define HDLMODULE 264
#define BUILTINMODULE 265
#define ASSIGN 266
#define ENDMODULE 267
#define PRIMITIVE 268
#define TECHNOLOGY 269
#define DELAY 270
#define AREA 271
#define POWER 272
#define IF 273
#define ELSE 274
#define SWITCH 275
#define CASE 276
#define CASEX 277
#define CASEZ 278
#define RETURN 279
#define BREAK 280
#define NEXT 281
#define FUNC 282
#define DEFAULT 283
#define INITIALB 284
#define INPUT 285
#define REG 286
#define OUTPUT 287
#define INOUT 288
#define WIRE 289
#define KWVERSION 290
#define SUPPLY0 291
#define SUPPLY1 292
#define KWTRI 293
#define TRI0 294
#define TRI1 295
#define TRIREG 296
#define TRIAND 297
#define TRIOR 298
#define WAND 299
#define WOR 300
#define DEASSIGN 301
#define BBEGIN 302
#define END 303
#define ENDCASE 304
#define ALWAYS 305
#define POSEDGE 306
#define NEGEDGE 307
#define INTEGER 308
#define ENDPRIMITIVE 309
#define SEMI 310
#define COLON 311
#define COMMA 312
#define DOT 313
#define ASGN 314
#define NBASGN 315
#define LPAREN 316
#define RPAREN 317
#define LBRACE 318
#define RBRACE 319
#define LBRACK 320
#define RBRACK 321
#define AT 322
#define ANOTATE 323
#define ENDANOTATE 324
#define ROT 325
#define WPLACE 326
#define SHOWNAME 327
#define ENDDECLS 328
#define NOT 329
#define PORTS 330
#define PORTDEF 331
#define BDPORTS 332
#define SIZE 333
#define JOINT 334
#define COMMENT 335
#define FRAME 336
#define SLASH 337
#define BDESC 338
#define ROOTMODULE 339
#define TRAN 340
#define COMLINE 341
#define COMEND 342
#define SCRIPT 343
#define BEGINSCRIPT 344
#define ENDSCRIPT 345
#define REQUIRE 346
#define PROPERTY 347
#define DECPOS 348
#define ICONDATA 349
#define DATA 350
#define ICONEND 351
#define SYMBOLREF 352
#define TEXTBEGIN 353
#define HASHMARK 354
#define GT 355
#define LT 356
#define EQ 357
#define BAND 358
#define BNAND 359
#define BOR 360
#define BNOR 361
#define BXOR 362
#define BNXOR 363
#define UINV 364
#define MOD 365
#define LSHIFT 366
#define RSHIFTEQ 367
#define QUEST 368
#define ARSHIFT 369
#define ALSHIFT 370
#define BOGOCHAR 371
#define NUMBER 372
#define HEX 373
#define VERNUM 374
#define LITERAL 375
#define STRING 376
#define HDLLINE 377
#define SYSLITERAL 378
#define OR 379
#define AND 380
#define NE 381
#define LE 382
#define GE 383
#define RSHIFT 384
#define SUB 385
#define ADD 386
#define DIV 387
#define MUL 388
#define POW 389
#define UPLUS 390
#define UNEG 391




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

