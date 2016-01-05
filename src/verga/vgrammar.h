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
     INPUT = 258,
     OUTPUT = 259,
     INOUT = 260,
     WIRE = 261,
     REG = 262,
     SUPPLY0 = 263,
     SUPPLY1 = 264,
     INTEGER = 265,
     MODULE = 266,
     ENDMODULE = 267,
     PRIMITIVE = 268,
     ENDPRIMITIVE = 269,
     TASK = 270,
     ENDTASK = 271,
     DEASSIGN = 272,
     ASSIGN = 273,
     INITIALB = 274,
     ALWAYS = 275,
     PARAMETER = 276,
     AUTOMATIC = 277,
     BBEGIN = 278,
     END = 279,
     IF = 280,
     ELSE = 281,
     CASE = 282,
     CASEX = 283,
     CASEZ = 284,
     ENDCASE = 285,
     DEFAULT = 286,
     REPEAT = 287,
     FOR = 288,
     WHILE = 289,
     FORK = 290,
     JOIN = 291,
     WAIT = 292,
     FOREVER = 293,
     POSEDGE = 294,
     NEGEDGE = 295,
     MPATH = 296,
     DPATH = 297,
     TRIGGER = 298,
     FUNCTION = 299,
     ENDFUNCTION = 300,
     LPAREN = 301,
     RPAREN = 302,
     LBRACE = 303,
     RBRACE = 304,
     LBRACK = 305,
     RBRACK = 306,
     SEMI = 307,
     COLON = 308,
     COMMA = 309,
     DOT = 310,
     ASGN = 311,
     AT = 312,
     HASH = 313,
     REPCAT = 314,
     NOT = 315,
     GT = 316,
     LT = 317,
     EQ = 318,
     EQZ = 319,
     NEZ = 320,
     BAND = 321,
     BNAND = 322,
     BOR = 323,
     BNOR = 324,
     BXOR = 325,
     BNXOR = 326,
     UINV = 327,
     MOD = 328,
     ARSHIFT = 329,
     ALSHIFT = 330,
     LSHIFT = 331,
     RSHIFTEQ = 332,
     QUEST = 333,
     SPECIFY = 334,
     ENDSPECIFY = 335,
     SPECPARAM = 336,
     SMALL = 337,
     MEDIUM = 338,
     LARGE = 339,
     TRI = 340,
     TRI0 = 341,
     TRI1 = 342,
     WAND = 343,
     WOR = 344,
     TRIREG = 345,
     SIGNED = 346,
     SCALAR = 347,
     VECTORED = 348,
     REAL = 349,
     EVENT = 350,
     TIME = 351,
     TRIAND = 352,
     TRIOR = 353,
     STRONG0 = 354,
     STRONG1 = 355,
     PULL0 = 356,
     PULL1 = 357,
     WEAK0 = 358,
     WEAK1 = 359,
     HIGHZ0 = 360,
     HIGHZ1 = 361,
     CMOS = 362,
     RCMOS = 363,
     BUFIF0 = 364,
     BUFIF1 = 365,
     NOTIF0 = 366,
     NOTIF1 = 367,
     NMOS = 368,
     PMOS = 369,
     RNMOS = 370,
     RPMOS = 371,
     PRIMAND = 372,
     PRIMNAND = 373,
     PRIMNOR = 374,
     PRIMXOR = 375,
     PRIMXNOR = 376,
     BUF = 377,
     PRIMNOT = 378,
     TRANIF0 = 379,
     TRANIF1 = 380,
     RTRANIF0 = 381,
     RTRANIF1 = 382,
     RTRAN = 383,
     KWOR = 384,
     TRAN = 385,
     COND = 386,
     BREAKPT = 387,
     SCRIPT = 388,
     BEGINSCRIPT = 389,
     ENDSCRIPT = 390,
     BOGUS = 391,
     BOGOCHAR = 392,
     INSTANCE = 393,
     MEMORY = 394,
     NUMBER = 395,
     HEX = 396,
     SYSLITERAL = 397,
     LITERAL = 398,
     HLITERAL = 399,
     STRING = 400,
     HDLLINE = 401,
     REALNUM = 402,
     OR = 403,
     AND = 404,
     NE = 405,
     LE = 406,
     GE = 407,
     RSHIFT = 408,
     SUB = 409,
     ADD = 410,
     DIV = 411,
     MUL = 412,
     POW = 413,
     UPLUS = 414,
     UNEG = 415
   };
#endif
/* Tokens.  */
#define INPUT 258
#define OUTPUT 259
#define INOUT 260
#define WIRE 261
#define REG 262
#define SUPPLY0 263
#define SUPPLY1 264
#define INTEGER 265
#define MODULE 266
#define ENDMODULE 267
#define PRIMITIVE 268
#define ENDPRIMITIVE 269
#define TASK 270
#define ENDTASK 271
#define DEASSIGN 272
#define ASSIGN 273
#define INITIALB 274
#define ALWAYS 275
#define PARAMETER 276
#define AUTOMATIC 277
#define BBEGIN 278
#define END 279
#define IF 280
#define ELSE 281
#define CASE 282
#define CASEX 283
#define CASEZ 284
#define ENDCASE 285
#define DEFAULT 286
#define REPEAT 287
#define FOR 288
#define WHILE 289
#define FORK 290
#define JOIN 291
#define WAIT 292
#define FOREVER 293
#define POSEDGE 294
#define NEGEDGE 295
#define MPATH 296
#define DPATH 297
#define TRIGGER 298
#define FUNCTION 299
#define ENDFUNCTION 300
#define LPAREN 301
#define RPAREN 302
#define LBRACE 303
#define RBRACE 304
#define LBRACK 305
#define RBRACK 306
#define SEMI 307
#define COLON 308
#define COMMA 309
#define DOT 310
#define ASGN 311
#define AT 312
#define HASH 313
#define REPCAT 314
#define NOT 315
#define GT 316
#define LT 317
#define EQ 318
#define EQZ 319
#define NEZ 320
#define BAND 321
#define BNAND 322
#define BOR 323
#define BNOR 324
#define BXOR 325
#define BNXOR 326
#define UINV 327
#define MOD 328
#define ARSHIFT 329
#define ALSHIFT 330
#define LSHIFT 331
#define RSHIFTEQ 332
#define QUEST 333
#define SPECIFY 334
#define ENDSPECIFY 335
#define SPECPARAM 336
#define SMALL 337
#define MEDIUM 338
#define LARGE 339
#define TRI 340
#define TRI0 341
#define TRI1 342
#define WAND 343
#define WOR 344
#define TRIREG 345
#define SIGNED 346
#define SCALAR 347
#define VECTORED 348
#define REAL 349
#define EVENT 350
#define TIME 351
#define TRIAND 352
#define TRIOR 353
#define STRONG0 354
#define STRONG1 355
#define PULL0 356
#define PULL1 357
#define WEAK0 358
#define WEAK1 359
#define HIGHZ0 360
#define HIGHZ1 361
#define CMOS 362
#define RCMOS 363
#define BUFIF0 364
#define BUFIF1 365
#define NOTIF0 366
#define NOTIF1 367
#define NMOS 368
#define PMOS 369
#define RNMOS 370
#define RPMOS 371
#define PRIMAND 372
#define PRIMNAND 373
#define PRIMNOR 374
#define PRIMXOR 375
#define PRIMXNOR 376
#define BUF 377
#define PRIMNOT 378
#define TRANIF0 379
#define TRANIF1 380
#define RTRANIF0 381
#define RTRANIF1 382
#define RTRAN 383
#define KWOR 384
#define TRAN 385
#define COND 386
#define BREAKPT 387
#define SCRIPT 388
#define BEGINSCRIPT 389
#define ENDSCRIPT 390
#define BOGUS 391
#define BOGOCHAR 392
#define INSTANCE 393
#define MEMORY 394
#define NUMBER 395
#define HEX 396
#define SYSLITERAL 397
#define LITERAL 398
#define HLITERAL 399
#define STRING 400
#define HDLLINE 401
#define REALNUM 402
#define OR 403
#define AND 404
#define NE 405
#define LE 406
#define GE 407
#define RSHIFT 408
#define SUB 409
#define ADD 410
#define DIV 411
#define MUL 412
#define POW 413
#define UPLUS 414
#define UNEG 415




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

