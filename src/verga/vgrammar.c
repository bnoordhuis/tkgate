/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 88 "vgrammar.y"

#include "thyme.h"

void BeginPV();		/* Pure verilog */
void BeginLC();
void BeginBC();



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 435 "vgrammar.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3600

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  161
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  126
/* YYNRULES -- Number of rules.  */
#define YYNRULES  363
/* YYNRULES -- Number of states.  */
#define YYNSTATES  715

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   415

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,    12,    15,    17,    20,    22,
      26,    29,    31,    32,    35,    37,    39,    41,    43,    45,
      47,    49,    52,    53,    60,    61,    66,    68,    72,    78,
      82,    83,    86,    90,    92,    96,    97,   100,   102,   104,
     106,   108,   110,   112,   114,   116,   118,   120,   122,   125,
     131,   138,   144,   147,   150,   151,   156,   159,   160,   163,
     165,   167,   169,   171,   173,   175,   177,   179,   181,   183,
     184,   190,   191,   197,   198,   204,   205,   211,   212,   218,
     219,   225,   226,   232,   234,   236,   238,   240,   242,   244,
     246,   248,   250,   252,   254,   256,   258,   260,   262,   264,
     266,   268,   270,   272,   274,   276,   278,   280,   282,   286,
     292,   296,   297,   303,   308,   309,   311,   313,   319,   323,
     331,   333,   337,   343,   348,   351,   353,   355,   357,   361,
     367,   375,   376,   381,   382,   391,   392,   397,   398,   402,
     403,   408,   417,   420,   425,   430,   436,   438,   442,   444,
     447,   449,   451,   453,   456,   459,   462,   465,   468,   471,
     474,   477,   480,   483,   486,   489,   492,   495,   497,   499,
     501,   503,   504,   507,   509,   511,   513,   517,   523,   529,
     531,   533,   535,   537,   539,   541,   543,   545,   547,   549,
     551,   553,   555,   556,   558,   564,   571,   578,   582,   583,
     586,   592,   601,   614,   620,   622,   624,   626,   630,   632,
     636,   637,   639,   641,   643,   648,   651,   654,   659,   662,
     669,   673,   675,   677,   679,   681,   682,   684,   688,   690,
     693,   696,   697,   707,   708,   721,   722,   731,   732,   744,
     745,   748,   749,   755,   756,   762,   763,   769,   770,   775,
     777,   781,   783,   784,   789,   791,   792,   796,   802,   805,
     811,   816,   822,   824,   826,   828,   830,   832,   834,   836,
     838,   840,   842,   844,   847,   850,   851,   854,   858,   864,
     872,   878,   884,   887,   890,   897,   899,   901,   903,   904,
     907,   911,   915,   925,   931,   934,   940,   944,   948,   954,
     958,   960,   964,   966,   970,   972,   974,   979,   987,   995,
    1002,  1006,  1010,  1014,  1018,  1022,  1026,  1030,  1034,  1038,
    1042,  1046,  1050,  1054,  1058,  1062,  1066,  1070,  1074,  1078,
    1082,  1086,  1090,  1094,  1098,  1102,  1108,  1111,  1114,  1117,
    1120,  1123,  1126,  1129,  1132,  1135,  1138,  1142,  1144,  1146,
    1148,  1150,  1152,  1156,  1158,  1163,  1168,  1170,  1174,  1176,
    1181,  1183,  1187,  1188
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     162,     0,    -1,    -1,   163,    -1,   132,   140,    53,   282,
      52,    -1,   133,   166,    -1,   164,    -1,   163,   164,    -1,
     165,    -1,   169,   176,    12,    -1,     1,    12,    -1,   167,
      -1,    -1,   167,   168,    -1,   213,    -1,   180,    -1,   181,
      -1,   178,    -1,   179,    -1,   250,    -1,   247,    -1,     1,
      52,    -1,    -1,    11,   143,   170,   171,   174,    52,    -1,
      -1,    58,    46,   172,    47,    -1,   173,    -1,   172,    54,
     173,    -1,    55,   143,    46,   282,    47,    -1,   143,    56,
     282,    -1,    -1,    46,    47,    -1,    46,   175,    47,    -1,
     143,    -1,   175,    54,   143,    -1,    -1,   176,   177,    -1,
     213,    -1,   180,    -1,   181,    -1,   178,    -1,   203,    -1,
     186,    -1,   179,    -1,   233,    -1,   250,    -1,   247,    -1,
     182,    -1,     1,    52,    -1,    18,   279,    56,   282,    52,
      -1,    18,   242,   279,    56,   282,    52,    -1,    21,   143,
      56,   282,    52,    -1,    19,   263,    -1,    20,   263,    -1,
      -1,   134,   183,   184,   135,    -1,     1,   135,    -1,    -1,
     184,   185,    -1,   213,    -1,   180,    -1,   181,    -1,   178,
      -1,   203,    -1,   186,    -1,   179,    -1,   233,    -1,   250,
      -1,   247,    -1,    -1,   194,   244,   187,   201,    52,    -1,
      -1,   195,   244,   188,   201,    52,    -1,    -1,   196,   244,
     189,   201,    52,    -1,    -1,   197,   244,   190,   201,    52,
      -1,    -1,   198,   244,   191,   201,    52,    -1,    -1,   199,
     244,   192,   201,    52,    -1,    -1,   200,   244,   193,   201,
      52,    -1,   117,    -1,   118,    -1,   119,    -1,   120,    -1,
     121,    -1,   129,    -1,   122,    -1,   123,    -1,   107,    -1,
     108,    -1,   113,    -1,   114,    -1,   115,    -1,   116,    -1,
     109,    -1,   110,    -1,   111,    -1,   112,    -1,   130,    -1,
     128,    -1,   124,    -1,   125,    -1,   126,    -1,   127,    -1,
     202,    -1,   201,    54,   202,    -1,   143,   231,    46,   285,
      47,    -1,    46,   285,    47,    -1,    -1,   143,   204,   205,
     208,    52,    -1,    58,    46,   206,    47,    -1,    -1,   207,
      -1,   282,    -1,    55,   143,    46,   282,    47,    -1,   207,
      54,   282,    -1,   207,    54,    55,   143,    46,   282,    47,
      -1,   209,    -1,   208,    54,   209,    -1,   143,   231,    46,
     210,    47,    -1,   143,   231,    46,    47,    -1,   143,   231,
      -1,   211,    -1,   212,    -1,   282,    -1,   211,    54,   282,
      -1,    55,   143,    46,   282,    47,    -1,   212,    54,    55,
     143,    46,   282,    47,    -1,    -1,   223,   231,   214,   219,
      -1,    -1,   223,   231,   242,   215,   221,    56,   282,    52,
      -1,    -1,   224,   231,   216,   219,    -1,    -1,   225,   217,
     219,    -1,    -1,   222,   231,   218,   219,    -1,   223,    50,
     157,    51,   143,    56,   282,    52,    -1,   221,    52,    -1,
     221,    54,   220,    52,    -1,   221,    56,   282,    52,    -1,
     221,    56,   242,   282,    52,    -1,   221,    -1,   220,    54,
     221,    -1,   143,    -1,   143,   232,    -1,     3,    -1,     4,
      -1,     5,    -1,     4,     7,    -1,     7,     4,    -1,     6,
     226,    -1,    88,   226,    -1,    89,   226,    -1,    85,   226,
      -1,    87,   226,    -1,    86,   226,    -1,    97,   226,    -1,
      98,   226,    -1,    90,   226,    -1,     8,   226,    -1,     9,
     226,    -1,     7,   226,    -1,    10,    -1,    94,    -1,    96,
      -1,    95,    -1,    -1,   226,   227,    -1,    93,    -1,    92,
      -1,    91,    -1,    46,   228,    47,    -1,    46,   229,    54,
     230,    47,    -1,    46,   230,    54,   229,    47,    -1,    82,
      -1,    83,    -1,    84,    -1,     8,    -1,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,     9,    -1,   100,    -1,   102,
      -1,   104,    -1,   106,    -1,    -1,   232,    -1,    50,   282,
      53,   282,    51,    -1,    50,   282,   155,    53,   282,    51,
      -1,    50,   282,   154,    53,   282,    51,    -1,    79,   234,
      80,    -1,    -1,   234,   235,    -1,   142,    46,   239,    47,
      52,    -1,    46,   280,   236,   280,    47,    56,   282,    52,
      -1,    25,    46,   282,    47,    46,   280,   236,   280,    47,
      56,   282,    52,    -1,    81,   143,    56,   282,    52,    -1,
      41,    -1,    42,    -1,   245,    -1,   245,   131,   282,    -1,
     237,    -1,   238,    54,   237,    -1,    -1,   238,    -1,   241,
      -1,   242,    -1,    57,    46,   245,    47,    -1,    57,   143,
      -1,    57,   157,    -1,    57,    46,   157,    47,    -1,    58,
     243,    -1,    58,   243,    53,   243,    53,   243,    -1,    46,
     282,    47,    -1,   143,    -1,   140,    -1,   147,    -1,   242,
      -1,    -1,   246,    -1,   245,   129,   246,    -1,   282,    -1,
      39,   282,    -1,    40,   282,    -1,    -1,   262,    44,   231,
     143,   248,    52,   253,   263,    45,    -1,    -1,   262,    44,
     231,   143,   249,    46,   259,    47,    52,   253,   263,    45,
      -1,    -1,   262,    15,   143,   251,    52,   253,   263,    16,
      -1,    -1,   262,    15,   143,   252,    46,   259,    47,    52,
     253,   263,    16,    -1,    -1,   253,   254,    -1,    -1,   222,
     231,   255,   220,    52,    -1,    -1,   223,   231,   256,   220,
      52,    -1,    -1,   224,   231,   257,   220,    52,    -1,    -1,
     225,   258,   220,    52,    -1,   260,    -1,   259,    54,   260,
      -1,   221,    -1,    -1,   222,   231,   261,   221,    -1,    22,
      -1,    -1,    23,   264,    24,    -1,    23,    53,   143,   264,
      24,    -1,   265,    52,    -1,   279,    56,   240,   282,    52,
      -1,   279,   151,   282,    52,    -1,   279,   151,   240,   282,
      52,    -1,   267,    -1,   266,    -1,   268,    -1,   272,    -1,
     273,    -1,   274,    -1,   277,    -1,   278,    -1,   275,    -1,
     276,    -1,    52,    -1,   240,   263,    -1,     1,    52,    -1,
      -1,   264,   263,    -1,   279,    56,   282,    -1,    25,    46,
     282,    47,   263,    -1,    25,    46,   282,    47,   263,    26,
     263,    -1,   143,    46,   286,    47,    52,    -1,   142,    46,
     286,    47,    52,    -1,   143,    52,    -1,   142,    52,    -1,
     269,    46,   282,    47,   270,    30,    -1,    27,    -1,    28,
      -1,    29,    -1,    -1,   270,   271,    -1,   282,    53,   263,
      -1,    31,    53,   263,    -1,    33,    46,   265,    52,   282,
      52,   265,    47,   263,    -1,    34,    46,   282,    47,   263,
      -1,    38,   263,    -1,    37,    46,   282,    47,   263,    -1,
      43,   143,    52,    -1,    43,   144,    52,    -1,    32,    46,
     282,    47,   263,    -1,    35,   264,    36,    -1,   281,    -1,
      48,   280,    49,    -1,   279,    -1,   280,    54,   279,    -1,
     143,    -1,   144,    -1,   281,    50,   282,    51,    -1,   281,
      50,   282,   155,    53,   282,    51,    -1,   281,    50,   282,
     154,    53,   282,    51,    -1,   281,    50,   282,    53,   282,
      51,    -1,   282,   155,   282,    -1,   282,   154,   282,    -1,
     282,   157,   282,    -1,   282,   156,   282,    -1,   282,   149,
     282,    -1,   282,   148,   282,    -1,   282,    61,   282,    -1,
     282,    62,   282,    -1,   282,   152,   282,    -1,   282,   151,
     282,    -1,   282,    63,   282,    -1,   282,   150,   282,    -1,
     282,    64,   282,    -1,   282,    65,   282,    -1,   282,   153,
     282,    -1,   282,    76,   282,    -1,   282,    74,   282,    -1,
     282,    75,   282,    -1,   282,    73,   282,    -1,   282,    66,
     282,    -1,   282,    68,   282,    -1,   282,    70,   282,    -1,
     282,    67,   282,    -1,   282,    69,   282,    -1,   282,    71,
     282,    -1,   282,    78,   282,    53,   282,    -1,    60,   282,
      -1,    72,   282,    -1,    66,   282,    -1,    68,   282,    -1,
      70,   282,    -1,    67,   282,    -1,    69,   282,    -1,    71,
     282,    -1,   154,   282,    -1,   155,   282,    -1,    46,   282,
      47,    -1,   281,    -1,   147,    -1,   140,    -1,   141,    -1,
     145,    -1,    48,   283,    49,    -1,   142,    -1,   142,    46,
     286,    47,    -1,   143,    46,   286,    47,    -1,   284,    -1,
     283,    54,   284,    -1,   282,    -1,   282,    48,   282,    49,
      -1,   282,    -1,   285,    54,   282,    -1,    -1,   285,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   108,   108,   109,   110,   111,   114,   115,   118,   121,
     122,   130,   133,   134,   137,   138,   139,   140,   141,   142,
     143,   144,   160,   160,   163,   164,   167,   168,   172,   173,
     176,   177,   178,   182,   183,   200,   201,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   218,
     219,   222,   225,   228,   238,   238,   239,   242,   243,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   274,
     274,   275,   275,   276,   276,   277,   277,   278,   278,   279,
     279,   280,   280,   283,   283,   283,   283,   283,   283,   284,
     284,   285,   285,   286,   286,   286,   286,   287,   287,   287,
     287,   288,   288,   289,   289,   289,   289,   291,   292,   295,
     296,   313,   313,   316,   317,   321,   325,   326,   327,   328,
     337,   338,   341,   342,   343,   346,   347,   351,   352,   355,
     356,   374,   374,   375,   375,   376,   376,   377,   377,   378,
     378,   379,   382,   383,   384,   385,   390,   391,   394,   395,
     398,   399,   400,   401,   402,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   418,   421,   422,   423,
     424,   427,   428,   431,   432,   433,   434,   435,   436,   439,
     440,   441,   445,   446,   447,   448,   449,   451,   452,   453,
     454,   455,   458,   459,   462,   463,   464,   484,   487,   488,
     491,   492,   493,   495,   498,   499,   503,   504,   507,   508,
     511,   512,   527,   528,   531,   532,   533,   534,   537,   538,
     541,   542,   543,   544,   547,   548,   551,   552,   555,   556,
     557,   572,   571,   575,   574,   590,   590,   591,   591,   595,
     596,   599,   599,   600,   600,   601,   601,   602,   602,   605,
     606,   609,   610,   610,   613,   614,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   650,   651,   654,   669,   670,
     682,   683,   684,   685,   702,   705,   706,   707,   710,   711,
     714,   715,   728,   745,   759,   772,   785,   786,   798,   815,
     832,   833,   836,   837,   840,   841,   842,   843,   844,   845,
     848,   849,   850,   851,   852,   853,   854,   855,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,   866,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   877,   878,
     879,   880,   881,   882,   884,   885,   886,   887,   888,   889,
     890,   891,   892,   893,   894,   895,   898,   899,   902,   903,
     906,   907,   910,   911
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INPUT", "OUTPUT", "INOUT", "WIRE",
  "REG", "SUPPLY0", "SUPPLY1", "INTEGER", "MODULE", "ENDMODULE",
  "PRIMITIVE", "ENDPRIMITIVE", "TASK", "ENDTASK", "DEASSIGN", "ASSIGN",
  "INITIALB", "ALWAYS", "PARAMETER", "AUTOMATIC", "BBEGIN", "END", "IF",
  "ELSE", "CASE", "CASEX", "CASEZ", "ENDCASE", "DEFAULT", "REPEAT", "FOR",
  "WHILE", "FORK", "JOIN", "WAIT", "FOREVER", "POSEDGE", "NEGEDGE",
  "MPATH", "DPATH", "TRIGGER", "FUNCTION", "ENDFUNCTION", "LPAREN",
  "RPAREN", "LBRACE", "RBRACE", "LBRACK", "RBRACK", "SEMI", "COLON",
  "COMMA", "DOT", "ASGN", "AT", "HASH", "REPCAT", "NOT", "GT", "LT", "EQ",
  "EQZ", "NEZ", "BAND", "BNAND", "BOR", "BNOR", "BXOR", "BNXOR", "UINV",
  "MOD", "ARSHIFT", "ALSHIFT", "LSHIFT", "RSHIFTEQ", "QUEST", "SPECIFY",
  "ENDSPECIFY", "SPECPARAM", "SMALL", "MEDIUM", "LARGE", "TRI", "TRI0",
  "TRI1", "WAND", "WOR", "TRIREG", "SIGNED", "SCALAR", "VECTORED", "REAL",
  "EVENT", "TIME", "TRIAND", "TRIOR", "STRONG0", "STRONG1", "PULL0",
  "PULL1", "WEAK0", "WEAK1", "HIGHZ0", "HIGHZ1", "CMOS", "RCMOS", "BUFIF0",
  "BUFIF1", "NOTIF0", "NOTIF1", "NMOS", "PMOS", "RNMOS", "RPMOS",
  "PRIMAND", "PRIMNAND", "PRIMNOR", "PRIMXOR", "PRIMXNOR", "BUF",
  "PRIMNOT", "TRANIF0", "TRANIF1", "RTRANIF0", "RTRANIF1", "RTRAN", "KWOR",
  "TRAN", "COND", "BREAKPT", "SCRIPT", "BEGINSCRIPT", "ENDSCRIPT", "BOGUS",
  "BOGOCHAR", "INSTANCE", "MEMORY", "NUMBER", "HEX", "SYSLITERAL",
  "LITERAL", "HLITERAL", "STRING", "HDLLINE", "REALNUM", "OR", "AND", "NE",
  "LE", "GE", "RSHIFT", "SUB", "ADD", "DIV", "MUL", "POW", "UPLUS", "UNEG",
  "$accept", "prog", "punits", "punit", "module", "script", "sitems",
  "sitem", "modhead", "@1", "omparmdecls", "mparmdecls", "mparmdecl",
  "omargs", "margs", "mitems", "mitem", "assign", "parameter", "initial",
  "always", "escript", "@2", "esitems", "esitem", "gate", "@3", "@4", "@5",
  "@6", "@7", "@8", "@9", "inx_gtype", "outx_gtype", "cmos_gtype",
  "mos_gtype", "bif_gtype", "tran_gtype", "trif_gtype", "ginsts", "ginst",
  "instance", "@10", "omparmsets", "mparmsets", "mpexprs", "minsts",
  "minst", "miports", "miexprs", "miasgns", "decl", "@11", "@12", "@13",
  "@14", "@15", "decltail", "dlits", "dlit", "port_type", "net_type",
  "reg_type", "xreg_type", "netattrs", "netattr", "capsize", "size0",
  "size1", "orange", "range", "specblock", "specitems", "specitem",
  "ptype", "starg", "tgparms", "otgparms", "econd", "event", "delay",
  "dexpr", "odelay", "triggers", "trigger", "userfunc", "@16", "@17",
  "usertask", "@18", "@19", "taskdecls", "taskdecl", "@20", "@21", "@22",
  "@23", "taskprotos", "taskproto", "@24", "oautomatic", "stat", "stats",
  "asgn_stat", "if_stat", "task_stat", "case_stat", "casekw", "caseents",
  "caseent", "for_stat", "while_stat", "forever_stat", "wait_stat",
  "raise_stat", "repeat_stat", "fork_stat", "lval", "lvals", "bval",
  "expr", "catexprs", "catexpr", "exprs", "oexprs", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   161,   162,   162,   162,   162,   163,   163,   164,   165,
     165,   166,   167,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   170,   169,   171,   171,   172,   172,   173,   173,
     174,   174,   174,   175,   175,   176,   176,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   178,
     178,   179,   180,   181,   183,   182,   182,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   187,
     186,   188,   186,   189,   186,   190,   186,   191,   186,   192,
     186,   193,   186,   194,   194,   194,   194,   194,   194,   195,
     195,   196,   196,   197,   197,   197,   197,   198,   198,   198,
     198,   199,   199,   200,   200,   200,   200,   201,   201,   202,
     202,   204,   203,   205,   205,   206,   207,   207,   207,   207,
     208,   208,   209,   209,   209,   210,   210,   211,   211,   212,
     212,   214,   213,   215,   213,   216,   213,   217,   213,   218,
     213,   213,   219,   219,   219,   219,   220,   220,   221,   221,
     222,   222,   222,   222,   222,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   224,   225,   225,   225,
     225,   226,   226,   227,   227,   227,   227,   227,   227,   228,
     228,   228,   229,   229,   229,   229,   229,   230,   230,   230,
     230,   230,   231,   231,   232,   232,   232,   233,   234,   234,
     235,   235,   235,   235,   236,   236,   237,   237,   238,   238,
     239,   239,   240,   240,   241,   241,   241,   241,   242,   242,
     243,   243,   243,   243,   244,   244,   245,   245,   246,   246,
     246,   248,   247,   249,   247,   251,   250,   252,   250,   253,
     253,   255,   254,   256,   254,   257,   254,   258,   254,   259,
     259,   260,   261,   260,   262,   262,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   264,   264,   265,   266,   266,
     267,   267,   267,   267,   268,   269,   269,   269,   270,   270,
     271,   271,   272,   273,   274,   275,   276,   276,   277,   278,
     279,   279,   280,   280,   281,   281,   281,   281,   281,   281,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   283,   283,   284,   284,
     285,   285,   286,   286
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     5,     2,     1,     2,     1,     3,
       2,     1,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     0,     6,     0,     4,     1,     3,     5,     3,
       0,     2,     3,     1,     3,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     5,
       6,     5,     2,     2,     0,     4,     2,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     5,     0,
       5,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       3,     0,     5,     4,     0,     1,     1,     5,     3,     7,
       1,     3,     5,     4,     2,     1,     1,     1,     3,     5,
       7,     0,     4,     0,     8,     0,     4,     0,     3,     0,
       4,     8,     2,     4,     4,     5,     1,     3,     1,     2,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     1,
       1,     0,     2,     1,     1,     1,     3,     5,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     5,     6,     6,     3,     0,     2,
       5,     8,    12,     5,     1,     1,     1,     3,     1,     3,
       0,     1,     1,     1,     4,     2,     2,     4,     2,     6,
       3,     1,     1,     1,     1,     0,     1,     3,     1,     2,
       2,     0,     9,     0,    12,     0,     8,     0,    11,     0,
       2,     0,     5,     0,     5,     0,     5,     0,     4,     1,
       3,     1,     0,     4,     1,     0,     3,     5,     2,     5,
       4,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     2,     3,     5,     7,
       5,     5,     2,     2,     6,     1,     1,     1,     0,     2,
       3,     3,     9,     5,     2,     5,     3,     3,     5,     3,
       1,     3,     1,     3,     1,     1,     4,     7,     7,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     1,     1,     1,
       1,     1,     3,     1,     4,     4,     1,     3,     1,     4,
       1,     3,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,     0,     0,    12,     0,     0,     6,     8,    35,
      10,    22,     0,     5,     0,     1,     7,     0,    24,     0,
       0,   150,   151,   152,   171,   171,   171,   171,   167,     0,
       0,     0,     0,   254,   171,   171,   171,   171,   171,   171,
     168,   170,   169,   171,   171,    13,    17,    18,    15,    16,
      14,   192,   192,   192,   137,    20,    19,     0,     0,     9,
     198,    91,    92,    97,    98,    99,   100,    93,    94,    95,
      96,    83,    84,    85,    86,    87,    89,    90,   103,   104,
     105,   106,   102,    88,   101,    54,   111,    36,    40,    43,
      38,    39,    47,    42,   225,   225,   225,   225,   225,   225,
     225,    41,    37,    44,    46,    45,     0,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
     353,   304,   305,   351,   348,     0,     0,   347,     0,    21,
     153,   155,   154,   166,   164,   165,     0,     0,   304,     0,
       0,   300,     0,   275,     0,   285,   286,   287,     0,     0,
       0,   275,     0,     0,     0,   272,     0,     0,   304,     0,
     212,   213,    52,     0,   263,   262,   264,     0,   265,   266,
     267,   270,   271,   268,   269,     0,    53,     0,   158,   160,
     159,   156,   157,   163,   161,   162,     0,   139,   193,     0,
     131,   135,     0,     0,   192,    48,    56,     0,    57,   114,
     224,    69,    71,    73,    75,    77,    79,    81,     0,     0,
       0,     0,   358,     0,   356,   336,   338,   341,   339,   342,
     340,   343,   337,   362,   362,   344,   345,     0,     4,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,   174,   173,   172,
     302,     0,     0,   222,   221,   223,   218,     0,     0,   274,
       0,     0,     0,     0,     0,     0,     0,     0,   294,     0,
       0,     0,   215,   216,   362,   283,   362,   282,   273,   258,
       0,     0,     0,     0,     0,     0,     0,     0,   133,     0,
     148,   138,     0,   235,     0,     0,     0,   197,     0,     0,
     199,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    31,    33,     0,    23,   346,
       0,   352,     0,   360,   363,     0,     0,     0,   316,   317,
     320,   322,   323,   329,   332,   330,   333,   331,   334,   328,
     326,   327,   325,     0,   315,   314,   321,   319,   318,   324,
     311,   310,   313,   312,   182,   187,   179,   180,   181,   183,
     188,   184,   189,   185,   190,   186,   191,     0,     0,     0,
     301,     0,     0,     0,     0,     0,   275,   256,   276,     0,
       0,     0,     0,     0,   299,     0,   296,   297,     0,     0,
       0,     0,   226,   228,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,   140,     0,   132,     0,   136,
     149,   142,     0,     0,     0,     0,   231,     0,     0,     0,
     210,    55,    62,    65,    60,    61,    58,    64,    63,    59,
      66,    68,    67,     0,   192,     0,   120,     0,   192,     0,
     107,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,    32,     0,     0,   357,     0,   354,   355,   306,     0,
       0,     0,     0,   176,     0,     0,   303,   220,     0,     0,
      49,     0,     0,     0,     0,     0,     0,     0,   229,   230,
     217,   214,     0,     0,     0,   288,     0,     0,   260,    51,
       0,     0,     0,     0,     0,     0,   146,     0,     0,   239,
       0,     0,     0,     0,   204,   205,     0,     0,   208,   211,
       0,   206,     0,     0,   115,   116,   124,   112,     0,     0,
       0,    70,     0,    72,    74,    76,    78,    80,    82,     0,
      29,    27,    34,   359,   361,     0,     0,     0,   335,     0,
       0,     0,    50,   257,   278,   298,     0,   293,   295,   227,
     281,   280,     0,   259,   261,   194,     0,     0,     0,     0,
     143,     0,     0,   144,     0,     0,   251,   192,     0,   249,
     239,     0,     0,     0,     0,     0,     0,     0,     0,   113,
       0,     0,   121,   110,     0,   108,     0,   309,     0,     0,
     177,   178,   219,     0,     0,   284,     0,   289,     0,   196,
     195,     0,     0,   147,   145,   192,   192,   192,   247,   240,
       0,   252,     0,     0,     0,     0,     0,     0,   203,   209,
     200,   207,     0,     0,   118,   123,     0,     0,   125,   126,
     127,     0,    28,   308,   307,   279,     0,     0,     0,   141,
     134,   241,   243,   245,     0,   236,     0,   239,   250,     0,
       0,     0,     0,     0,     0,     0,   122,     0,     0,   109,
       0,   291,   290,     0,     0,     0,     0,   253,     0,   232,
     239,     0,     0,   117,     0,     0,   128,     0,   292,     0,
       0,     0,   248,     0,     0,     0,   201,     0,     0,     0,
     242,   244,   246,   238,     0,     0,   119,   129,     0,   234,
       0,     0,     0,   130,   202
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    13,    14,    45,     9,    18,
     107,   323,   324,   210,   327,    17,    87,    46,    47,    48,
      49,    92,   198,   311,   436,    93,   314,   315,   316,   317,
     318,   319,   320,    94,    95,    96,    97,    98,    99,   100,
     449,   450,   101,   199,   313,   523,   524,   445,   446,   637,
     638,   639,    50,   297,   418,   299,   192,   295,   301,   505,
     506,   615,   616,   617,   618,   131,   259,   377,   378,   379,
     187,   188,   103,   197,   310,   516,   518,   519,   520,   159,
     160,   161,   266,   201,   521,   402,    55,   511,   512,    56,
     424,   425,   574,   619,   673,   674,   675,   654,   578,   579,
     656,    57,   388,   271,   163,   164,   165,   166,   167,   562,
     607,   168,   169,   170,   171,   172,   173,   174,   175,   261,
     127,   333,   213,   214,   334,   335
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -594
static const yytype_int16 yypact[] =
{
      36,    34,   -93,   -35,  -594,    84,    51,  -594,  -594,  -594,
    -594,  -594,    68,  -594,  3320,  -594,  -594,   938,    89,  2119,
      96,  -594,   168,  -594,  -594,   158,  -594,  -594,  -594,     0,
     872,   872,    33,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,   129,   145,   129,  -594,  -594,  -594,    30,    -9,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,   183,   183,   183,   183,   183,   183,
     183,  -594,  -594,  -594,  -594,  -594,   200,   201,  2119,  2119,
    2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,  -594,  -594,
     205,   217,  -594,  -594,  -594,  2119,  2119,   220,  2560,  -594,
    -594,    37,  -594,    37,    37,    37,     6,    -6,  -594,     6,
     226,   220,   257,   264,   272,  -594,  -594,  -594,   274,   281,
     282,  -594,   283,   872,   -56,  -594,   -12,   105,   113,   872,
    -594,  -594,  -594,   295,  -594,  -594,  -594,   284,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,   -15,  -594,   294,    37,    37,
      37,    37,    37,    37,    37,    37,  2119,  -594,  -594,  1075,
     183,  -594,   213,   214,   129,  -594,  -594,    19,  -594,   300,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,    21,    12,
     308,  1087,  1699,    70,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  2119,  2119,  -594,  -594,  2119,  -594,  2119,
    2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,
    2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,  2119,
    2119,  2119,  2119,  2119,  2119,   100,  -594,  -594,  -594,  -594,
    -594,    86,  2119,  -594,  -594,  -594,   309,   305,  2119,  -594,
     221,   745,  2119,  2119,     6,  2119,   791,  2119,  -594,   311,
     314,   492,  -594,  -594,  2119,  -594,  2119,  -594,  -594,  -594,
    2119,  1811,  1811,  2119,  3117,   213,   316,   213,  -594,   213,
     129,  -594,   203,   322,   233,   323,     6,  -594,   238,   332,
    -594,   990,   336,   243,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,   244,   327,     2,  -594,  -594,  -594,    31,  -594,  -594,
    2119,  -594,  2119,  3282,   335,   344,   350,  2242,   112,   112,
    1137,  1137,  1137,  3443,  3443,   278,   278,  3427,  3427,  -594,
     169,   169,   169,  3222,  3395,  3411,  1137,   112,   112,   169,
     -34,   -34,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,   351,   345,   346,
    -594,     6,  1112,    -6,  2119,  2580,  -594,  -594,  -594,  1209,
    1234,   353,   354,  1259,  -594,  1356,  -594,  -594,  2119,  2119,
     359,    13,  -594,  3282,   362,   371,  1381,  2119,  3282,  2119,
    2600,  2620,  2119,  1865,  1965,  -594,   276,  -594,   213,  -594,
    -594,  -594,   213,  1901,   368,   375,   376,  2119,   139,   367,
     447,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  1929,   129,    23,  -594,  2119,   129,   137,
    -594,   155,   186,   223,   227,   231,   234,   379,  2119,  -594,
      21,  -594,   293,  2216,  -594,  2119,  -594,  -594,  -594,  2119,
    2029,  2065,  2119,  -594,    54,    93,  -594,  -594,   373,  2726,
    -594,   834,   872,   872,  2119,  2119,   872,   872,  3282,  3282,
    -594,  -594,   447,   385,   386,  -594,  2746,  2766,  -594,  -594,
    2263,  2119,  2119,   384,   387,   235,  -594,  2119,  2786,  -594,
      28,   389,   381,  1406,  -594,  -594,     6,  2119,  -594,   388,
     398,   -65,   304,   401,   395,  3282,   408,  -594,   243,    32,
     412,  -594,    -4,  -594,  -594,  -594,  -594,  -594,  -594,  2119,
    3282,  -594,  -594,  -594,  3282,  2284,  2119,  2119,  3379,   415,
     416,    -6,  -594,  -594,   433,  -594,  2892,  -594,  -594,  -594,
    -594,  -594,   230,  -594,  -594,  -594,  2391,  2412,  2119,  2119,
    -594,   213,  2912,  -594,   669,   158,  -594,   129,    57,  -594,
    -594,    28,   418,    78,  2932,   447,   417,  2119,   421,  -594,
    2092,  1838,  -594,  -594,  2119,  -594,  1503,  -594,  2433,  2454,
    -594,  -594,  -594,   872,     6,  -594,   419,  -594,  3241,  -594,
    -594,  2952,  3058,  -594,  -594,   129,   129,   129,  -594,  -594,
     452,  -594,   422,    28,   669,   119,     6,   414,  -594,  -594,
    -594,  3282,  2119,   328,  3282,  -594,   338,   431,   430,   435,
    3282,   143,  -594,  -594,  -594,  -594,   438,   872,   872,  -594,
    -594,  -594,  -594,  -594,   213,  -594,   213,  -594,  -594,   446,
     440,   139,  2119,  1528,   450,   451,  -594,  2119,   444,  -594,
     872,  -594,  -594,   213,   213,   213,   239,  -594,   669,  -594,
    -594,     6,  3078,  -594,  2119,  2119,  3282,   357,  -594,   240,
     252,   253,  -594,   485,   669,   190,  -594,  1553,  1650,   457,
    -594,  -594,  -594,  -594,   459,   449,  -594,  -594,  2119,  -594,
    2119,  1675,  3098,  -594,  -594
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -594,  -594,  -594,   500,  -594,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,    48,  -594,  -594,  -594,  -594,   -16,    -3,    -1,
       4,  -594,  -594,  -594,  -594,   198,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,
      17,   -21,   199,  -594,  -594,  -594,  -594,  -594,    -5,  -594,
    -594,  -594,     8,  -594,  -594,  -594,  -594,  -594,  -242,  -593,
    -179,    -8,     1,     3,     5,   697,  -594,  -594,    46,    38,
     -41,   222,   215,  -594,  -594,  -137,   -60,  -594,  -594,  -189,
    -594,   -27,  -373,   317,   248,    41,    10,  -594,  -594,    11,
    -594,  -594,  -542,  -594,  -594,  -594,  -594,  -594,   -51,   -89,
    -594,  -594,    -7,  -122,  -266,  -594,  -594,  -594,  -594,  -594,
    -594,  -594,  -594,  -594,  -594,  -594,  -594,  -594,   -22,  -280,
     -26,   -19,  -594,   207,  -417,  -112
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -256
static const yytype_int16 yytable[] =
{
     128,    88,   139,   141,   141,   141,    51,   140,   391,    51,
     478,   190,   191,   302,    89,    52,    90,    53,    52,    54,
      53,    91,    54,   162,   176,   102,   428,   104,   105,   276,
     529,    21,    22,    23,   281,   575,    -2,     1,   624,   240,
     262,   291,   447,   195,   305,   193,    10,     2,   136,   459,
      11,    -3,     1,   415,   136,   417,   460,   419,   137,   325,
     491,   676,     2,   365,   492,   306,   587,   200,   200,   200,
     200,   200,   200,   200,   194,   527,   321,   528,   461,   593,
     689,   690,   691,   255,    15,   462,   465,   279,   280,   211,
     212,   215,   216,   217,   218,   219,   220,   221,   222,   307,
     308,   364,   407,   409,   622,    12,   225,   226,   364,   365,
     141,   623,   336,   141,   260,   678,   302,   267,   302,   331,
     302,    19,   253,   254,   332,   627,   196,   141,   256,   257,
     258,   282,   381,   141,   263,   380,   292,   264,   694,   448,
     381,   265,   492,   138,   122,   283,   278,   106,   129,   138,
     122,   284,   288,   304,   370,   326,   372,   285,   374,   286,
     376,   309,   132,   298,   322,   287,   660,   294,     3,     4,
     294,   300,   404,   623,   405,   130,   177,   641,   602,   186,
     514,   515,   366,   367,   368,   240,   241,   242,   243,   531,
     669,   532,   369,   381,   371,   189,   373,   465,   375,   369,
     370,   371,   372,   373,   374,   375,   376,   533,   337,   532,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   583,   705,   534,   504,
     532,   137,   240,   382,   381,   141,   208,   209,   141,   385,
     141,   223,   392,   389,   390,   421,   393,   422,   395,   423,
     605,   606,   403,   224,   481,   250,   251,   252,   253,   254,
     227,   406,   408,   410,   411,   535,   108,   532,   109,   536,
     141,   532,   268,   537,   260,   532,   538,   570,   532,   571,
     110,   692,   700,   571,   571,   432,   111,   112,   113,   114,
     115,   116,   117,    51,   701,   702,   571,   571,   433,   269,
     434,   463,    52,   212,    53,   435,    54,   270,   272,   439,
     273,   441,   442,   251,   252,   253,   254,   274,   275,   277,
     290,   576,   451,   452,   453,   454,   455,   456,   646,   229,
     230,   231,   232,   233,   234,   235,   661,   289,   238,   239,
     293,   240,   241,   242,   243,   141,   300,   303,   312,   476,
     328,   384,   383,   396,   386,   479,   397,   416,  -237,   427,
     118,   119,   120,   121,   122,   123,   426,   124,   430,   488,
     489,   429,   443,   458,   125,   126,   444,   457,   496,   465,
     497,   466,   613,   500,   360,   361,   507,   467,   473,   474,
     475,   695,   576,   526,   508,   484,   490,   530,   513,   493,
     485,   403,   202,   203,   204,   205,   206,   207,   494,   503,
     509,   510,  -233,   517,   525,   539,   551,   581,   247,   248,
     249,   250,   251,   252,   253,   254,   542,   560,   561,   540,
     568,   580,   585,   569,   576,   586,   544,   588,   589,   590,
     545,   360,   361,   548,   591,   141,   141,   141,   594,   603,
     141,   141,   600,   601,   626,   556,   408,   632,   655,   630,
     662,   664,   647,   403,   657,   554,   555,   677,   666,   557,
     558,   665,   566,   567,   667,   670,   398,   399,   572,   668,
     141,   679,   680,   108,   260,   109,   684,   685,   584,   687,
     699,   703,   577,   708,   709,   710,    16,   110,   541,   437,
     438,   595,   549,   111,   112,   113,   114,   115,   116,   117,
     596,   550,   420,   592,   681,   629,   440,   598,   599,   401,
     625,   398,   399,   559,   658,     0,   621,     0,   108,   464,
     109,     0,     0,   608,     0,     0,     0,     0,   141,   611,
     612,     0,   110,     0,     0,     0,     0,     0,   111,   112,
     113,   114,   115,   116,   117,     0,   403,   620,   631,     0,
       0,   634,   640,   577,   651,   652,   653,   141,   141,     0,
       0,     0,   392,     0,     0,     0,     0,   118,   119,   120,
     121,   122,   123,     0,   124,     0,   645,     0,   141,     0,
     141,   125,   126,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,   663,     0,   577,     0,   659,     0,     0,
       0,   141,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   118,   119,   120,   121,   122,   123,     0,   124,
     671,   672,     0,   682,   141,     0,   125,   126,   686,   400,
       0,     0,   141,     0,     0,   141,     0,     0,     0,   260,
       0,     0,     0,   688,     0,   697,   698,     0,   141,     0,
     142,   693,    21,    22,    23,    24,    25,    26,    27,    28,
       0,     0,     0,     0,     0,     0,     0,   704,     0,   711,
       0,   712,   143,     0,   144,     0,   145,   146,   147,     0,
       0,   148,   149,   150,   151,     0,   152,   153,     0,     0,
       0,     0,   154,     0,     0,     0,     0,   136,     0,     0,
       0,   155,   133,   134,   135,     0,   156,   137,     0,     0,
       0,   178,   179,   180,   181,   182,   183,     0,     0,     0,
     184,   185,     0,     0,     0,     0,   142,     0,     0,     0,
       0,     0,     0,     0,    34,    35,    36,    37,    38,    39,
       0,     0,     0,    40,    41,    42,    43,    44,   143,   387,
     144,     0,   145,   146,   147,     0,     0,   148,   149,   150,
     151,     0,   152,   153,     0,     0,     0,     0,   154,     0,
       0,     0,   142,   136,     0,     0,     0,   155,     0,     0,
       0,     0,   156,   137,     0,     0,     0,     0,     0,     0,
       0,   157,   158,   122,   143,     0,   144,     0,   145,   146,
     147,     0,     0,   148,   149,   150,   151,   394,   152,   153,
       0,     0,     0,     0,   154,   142,     0,     0,     0,   136,
       0,     0,     0,   155,     0,     0,     0,     0,   156,   137,
       0,     0,     0,     0,     0,     0,     0,   143,   553,   144,
       0,   145,   146,   147,     0,     0,   148,   149,   150,   151,
       0,   152,   153,   142,     0,     0,     0,   154,     0,     0,
       0,     0,   136,     0,     0,     0,   155,   157,   158,   122,
       0,   156,   137,     0,     0,   143,     0,   144,     0,   145,
     146,   147,     0,     0,   148,   149,   150,   151,     0,   152,
     153,     0,     0,     0,     0,   154,     0,     0,     0,     0,
     136,     0,     0,     0,   155,     0,     0,     0,     0,   156,
     137,     0,     0,   157,   158,   122,     0,     0,     0,    58,
       0,    21,    22,    23,    24,    25,    26,    27,    28,     0,
      59,     0,     0,  -255,     0,     0,    29,    30,    31,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   157,   158,   122,     0,
       0,     0,  -255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,     0,     0,     0,     0,     0,    29,    30,
      31,    32,    33,     0,   157,   158,   122,    60,     0,     0,
       0,     0,     0,    34,    35,    36,    37,    38,    39,     0,
       0,     0,    40,    41,    42,    43,    44,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    60,
       0,     0,    85,     0,     0,    34,    35,    36,    37,    38,
      39,    86,     0,     0,    40,    41,    42,    43,    44,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,   108,     0,   109,     0,   431,     0,     0,     0,     0,
       0,     0,     0,    86,   329,   110,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,   116,   117,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   477,
     240,   241,   242,   243,     0,   244,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   240,   241,   242,   243,     0,
     244,     0,     0,     0,     0,     0,     0,     0,   229,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     240,   241,   242,   243,     0,   118,   119,   120,   121,   122,
     123,     0,   124,     0,     0,     0,     0,     0,     0,   125,
     126,     0,   296,     0,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   483,   240,   241,   242,   243,     0,   244,   248,   249,
     250,   251,   252,   253,   254,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   486,   240,   241,   242,
     243,     0,   244,     0,     0,     0,     0,     0,     0,     0,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,   240,   241,   242,   243,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   495,   240,
     241,   242,   243,     0,   244,     0,     0,     0,     0,     0,
       0,     0,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   582,   240,   241,   242,   243,     0,   244,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,   240,
     241,   242,   243,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     642,     0,     0,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   683,   240,   241,   242,   243,
       0,   244,     0,     0,     0,     0,     0,     0,     0,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     706,   240,   241,   242,   243,     0,   244,     0,     0,     0,
       0,     0,     0,     0,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,     0,   240,   241,   242,   243,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   707,     0,     0,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   713,   240,   241,   242,   243,     0,   244,     0,
       0,     0,     0,     0,     0,     0,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   330,   240,   241,
     242,   243,     0,   244,     0,     0,     0,     0,     0,     0,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,   240,   241,   242,   243,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   108,     0,   109,
       0,     0,     0,     0,     0,     0,     0,     0,   156,   137,
       0,   110,     0,     0,     0,     0,     0,   111,   112,   113,
     114,   115,   116,   117,   108,   635,   109,     0,     0,     0,
       0,     0,     0,   636,     0,     0,     0,     0,   110,     0,
       0,     0,     0,     0,   111,   112,   113,   114,   115,   116,
     117,   108,     0,   109,     0,     0,     0,     0,   501,     0,
       0,     0,     0,     0,     0,   110,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,   116,   117,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,   109,
       0,   118,   119,   120,   121,   122,   123,     0,   124,   137,
       0,   110,     0,     0,     0,   125,   126,   111,   112,   113,
     114,   115,   116,   117,     0,   108,     0,   109,   118,   119,
     120,   121,   122,   123,   522,   124,     0,     0,     0,   110,
       0,     0,   125,   126,     0,   111,   112,   113,   114,   115,
     116,   117,     0,     0,     0,   118,   119,   120,   121,   122,
     123,   108,   124,   109,     0,     0,     0,     0,   502,   125,
     126,     0,     0,     0,     0,   110,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,   116,   117,     0,     0,
       0,   118,   119,   120,   121,   122,   123,     0,   124,     0,
       0,     0,     0,     0,     0,   125,   126,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   118,
     119,   120,   121,   122,   123,   108,   124,   109,     0,     0,
       0,     0,   546,   125,   126,     0,     0,     0,     0,   110,
       0,     0,     0,     0,     0,   111,   112,   113,   114,   115,
     116,   117,     0,     0,     0,   118,   119,   120,   121,   122,
     123,   108,   124,   109,     0,     0,     0,     0,   547,   125,
     126,     0,     0,     0,     0,   110,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,   116,   117,   108,     0,
     109,     0,     0,     0,     0,     0,     0,   633,     0,     0,
       0,     0,   110,     0,     0,     0,     0,     0,   111,   112,
     113,   114,   115,   116,   117,   108,     0,   109,     0,   118,
     119,   120,   121,   122,   123,     0,   124,     0,     0,   110,
       0,     0,     0,   125,   126,   111,   112,   113,   114,   115,
     116,   117,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   118,   119,   120,   121,   122,
     123,     0,   124,     0,     0,     0,     0,     0,     0,   125,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   118,   119,   120,   121,   122,   123,     0,   124,
       0,     0,     0,     0,     0,     0,   125,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   118,
     119,   120,   121,   122,   123,   543,   124,     0,     0,     0,
       0,     0,     0,   125,   126,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,   240,
     241,   242,   243,   468,   244,   469,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   565,   240,   241,   242,   243,     0,
     244,     0,     0,     0,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   597,   240,   241,   242,   243,
       0,   244,     0,     0,     0,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,   240,   241,   242,
     243,     0,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   470,   471,   253,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   609,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   610,   240,   241,   242,   243,     0,   244,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   643,   240,   241,   242,   243,     0,
     244,     0,     0,     0,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   644,   240,   241,   242,   243,
       0,   244,     0,     0,     0,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,     0,   240,   241,   242,
     243,     0,   244,     0,     0,     0,     0,     0,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   228,     0,     0,     0,     0,     0,     0,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   480,   240,   241,   242,   243,     0,   244,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   498,   240,   241,   242,   243,     0,   244,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   499,   240,   241,   242,   243,     0,   244,     0,
       0,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,     0,   240,   241,   242,   243,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   552,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   563,   240,
     241,   242,   243,     0,   244,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   564,   240,
     241,   242,   243,     0,   244,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   573,   240,
     241,   242,   243,     0,   244,     0,     0,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,     0,   240,
     241,   242,   243,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   604,     0,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   614,   240,   241,   242,   243,     0,
     244,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   628,   240,   241,   242,   243,     0,
     244,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   649,   240,   241,   242,   243,     0,
     244,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   240,   241,   242,   243,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     650,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     696,   240,   241,   242,   243,     0,   244,     0,     0,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     714,   240,   241,   242,   243,     0,   244,     0,     0,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     412,   240,   241,   242,   243,     0,   244,     0,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,     0,
     240,   241,   242,   243,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,   246,   247,   248,   249,
     250,   413,   414,   253,   254,   472,     0,     0,     0,     0,
       0,     0,     0,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   648,   240,   241,   242,   243,     0,
     244,     0,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   240,   241,   242,   243,     0,   244,
     -11,    20,     0,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,     0,     0,  -255,     0,     0,    29,    30,
      31,    32,    33,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,   240,   241,   242,   243,     0,
     244,     0,     0,     0,  -255,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,    34,    35,    36,    37,    38,
      39,     0,     0,     0,    40,    41,    42,    43,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,     0,   240,   241,   242,   243,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,     0,   240,   241,
     242,   243,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,     0,   240,   241,   242,   243,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
     240,   241,   242,   243,   229,   230,   231,   232,   233,     0,
       0,     0,     0,     0,     0,     0,   240,   241,   242,   243,
       0,     0,     0,     0,     0,     0,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,     0,     0,     0,     0,     0,     0,
       0,   247,   248,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,     0,     0,     0,     0,   247,   248,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,     0,     0,   247,   248,   249,   250,   251,   252,   253,
     254
};

static const yytype_int16 yycheck[] =
{
      19,    17,    29,    29,    30,    31,    14,    29,   274,    17,
     383,    52,    53,   192,    17,    14,    17,    14,    17,    14,
      17,    17,    17,    30,    31,    17,   306,    17,    17,   151,
     447,     3,     4,     5,    46,     7,     0,     1,   580,    73,
      46,    56,    46,    52,    25,    15,    12,    11,    48,    47,
     143,     0,     1,   295,    48,   297,    54,   299,    58,    47,
      47,   654,    11,     9,   129,    46,   131,    94,    95,    96,
      97,    98,    99,   100,    44,    52,    55,    54,    47,    47,
     673,   674,   675,    46,     0,    54,    54,   143,   144,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    80,
      81,     8,   291,   292,    47,   140,   125,   126,     8,     9,
     136,    54,   224,   139,   136,   657,   295,   139,   297,    49,
     299,    53,   156,   157,    54,    47,   135,   153,    91,    92,
      93,   143,    54,   159,   140,    49,   151,   143,   680,   143,
      54,   147,   129,   143,   144,   157,   153,    58,    52,   143,
     144,    46,   159,   194,   100,   143,   102,    52,   104,    46,
     106,   142,     4,   190,   143,    52,    47,   186,   132,   133,
     189,   143,   284,    54,   286,     7,   143,   594,   551,    50,
      41,    42,    82,    83,    84,    73,    74,    75,    76,    52,
      47,    54,    99,    54,   101,    50,   103,    54,   105,    99,
     100,   101,   102,   103,   104,   105,   106,    52,   227,    54,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   516,    47,    52,   418,
      54,    58,    73,   262,    54,   271,    46,    46,   274,   268,
     276,    46,   274,   272,   273,    52,   275,    54,   277,    56,
      30,    31,   281,    46,   386,   153,   154,   155,   156,   157,
      50,   290,   291,   292,   293,    52,    46,    54,    48,    52,
     306,    54,    56,    52,   306,    54,    52,    52,    54,    54,
      60,    52,    52,    54,    54,   311,    66,    67,    68,    69,
      70,    71,    72,   311,    52,    52,    54,    54,   311,    52,
     311,   330,   311,   332,   311,   311,   311,    53,    46,   311,
      46,   311,   311,   154,   155,   156,   157,    46,    46,    46,
      46,   510,   315,   316,   317,   318,   319,   320,   604,    61,
      62,    63,    64,    65,    66,    67,   626,    52,    70,    71,
      56,    73,    74,    75,    76,   381,   143,   143,    58,   381,
      52,    56,    53,    52,   143,   384,    52,    51,    46,    46,
     140,   141,   142,   143,   144,   145,   143,   147,    46,   398,
     399,   143,    46,    56,   154,   155,   143,   143,   407,    54,
     409,    47,   571,   412,   413,   414,   423,    47,    47,    54,
      54,   681,   581,   444,   423,    52,    47,   448,   427,    47,
      56,   430,    95,    96,    97,    98,    99,   100,    47,   143,
      52,    46,    46,    56,   443,    46,    53,    46,   150,   151,
     152,   153,   154,   155,   156,   157,   143,    52,    52,   458,
      56,    52,    54,    56,   623,    47,   465,   143,    47,    54,
     469,   470,   471,   472,    46,   481,   482,   483,    46,    26,
     486,   487,    47,    47,    46,   484,   485,    46,    16,    52,
      56,   143,    53,   492,    52,   482,   483,   656,    47,   486,
     487,   143,   501,   502,    54,    47,    39,    40,   507,    54,
     516,    45,    52,    46,   516,    48,    46,    46,   517,    55,
     143,    16,   510,    46,    45,    56,     6,    60,   460,   311,
     311,   532,   474,    66,    67,    68,    69,    70,    71,    72,
     539,   475,   300,   528,   661,   585,   311,   546,   547,   281,
     581,    39,    40,   492,   623,    -1,   577,    -1,    46,   332,
      48,    -1,    -1,   562,    -1,    -1,    -1,    -1,   574,   568,
     569,    -1,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    -1,   585,   574,   587,    -1,
      -1,   590,   591,   581,   615,   616,   617,   603,   604,    -1,
      -1,    -1,   604,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,   147,    -1,   603,    -1,   624,    -1,
     626,   154,   155,    -1,   626,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   632,    -1,   623,    -1,   624,    -1,    -1,
      -1,   647,   648,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,   147,
     647,   648,    -1,   662,   670,    -1,   154,   155,   667,   157,
      -1,    -1,   678,    -1,    -1,   681,    -1,    -1,    -1,   681,
      -1,    -1,    -1,   670,    -1,   684,   685,    -1,   694,    -1,
       1,   678,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   694,    -1,   708,
      -1,   710,    23,    -1,    25,    -1,    27,    28,    29,    -1,
      -1,    32,    33,    34,    35,    -1,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    25,    26,    27,    -1,    57,    58,    -1,    -1,
      -1,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      43,    44,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    23,    24,
      25,    -1,    27,    28,    29,    -1,    -1,    32,    33,    34,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,     1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,    23,    -1,    25,    -1,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    -1,    43,     1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      -1,    27,    28,    29,    -1,    -1,    32,    33,    34,    35,
      -1,    37,    38,     1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,   142,   143,   144,
      -1,    57,    58,    -1,    -1,    23,    -1,    25,    -1,    27,
      28,    29,    -1,    -1,    32,    33,    34,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      58,    -1,    -1,   142,   143,   144,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    -1,   142,   143,   144,    79,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    79,
      -1,    -1,   134,    -1,    -1,    85,    86,    87,    88,    89,
      90,   143,    -1,    -1,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    46,    -1,    48,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    47,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    47,
      73,    74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,   157,    -1,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    47,    73,    74,    75,    76,    -1,    78,   151,   152,
     153,   154,   155,   156,   157,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    47,    73,    74,    75,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    47,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    47,    73,    74,    75,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    47,    73,    74,    75,    76,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      47,    73,    74,    75,    76,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    74,    75,    76,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    47,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    48,    73,    74,
      75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,    68,
      69,    70,    71,    72,    46,    47,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,    71,
      72,    46,    -1,    48,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,
      -1,   140,   141,   142,   143,   144,   145,    -1,   147,    58,
      -1,    60,    -1,    -1,    -1,   154,   155,    66,    67,    68,
      69,    70,    71,    72,    -1,    46,    -1,    48,   140,   141,
     142,   143,   144,   145,    55,   147,    -1,    -1,    -1,    60,
      -1,    -1,   154,   155,    -1,    66,    67,    68,    69,    70,
      71,    72,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    46,   147,    48,    -1,    -1,    -1,    -1,    53,   154,
     155,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    46,   147,    48,    -1,    -1,
      -1,    -1,    53,   154,   155,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      71,    72,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    46,   147,    48,    -1,    -1,    -1,    -1,    53,   154,
     155,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    71,    72,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    71,    72,    46,    -1,    48,    -1,   140,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,    60,
      -1,    -1,    -1,   154,   155,    66,    67,    68,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    49,   147,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   155,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    51,    78,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    51,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    51,    73,    74,    75,    76,
      -1,    78,    -1,    -1,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    -1,    78,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    51,    73,    74,    75,    76,    -1,    78,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    51,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    51,    73,    74,    75,    76,
      -1,    78,    -1,    -1,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    52,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    52,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    52,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    52,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    52,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    52,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    52,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    52,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    52,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      52,    73,    74,    75,    76,    -1,    78,    -1,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      52,    73,    74,    75,    76,    -1,    78,    -1,    -1,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      53,    73,    74,    75,    76,    -1,    78,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    53,    73,    74,    75,    76,    -1,
      78,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    -1,    78,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      20,    21,    22,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    76,    -1,
      78,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,
      90,    -1,    -1,    -1,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    74,    75,    76,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      75,    76,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    74,    75,    76,    61,    62,
      63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   150,   151,   152,   153,   154,   155,   156,
     157
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,    11,   132,   133,   162,   163,   164,   165,   169,
      12,   143,   140,   166,   167,     0,   164,   176,   170,    53,
       1,     3,     4,     5,     6,     7,     8,     9,    10,    18,
      19,    20,    21,    22,    85,    86,    87,    88,    89,    90,
      94,    95,    96,    97,    98,   168,   178,   179,   180,   181,
     213,   222,   223,   224,   225,   247,   250,   262,     1,    12,
      79,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   134,   143,   177,   178,   179,
     180,   181,   182,   186,   194,   195,   196,   197,   198,   199,
     200,   203,   213,   233,   247,   250,    58,   171,    46,    48,
      60,    66,    67,    68,    69,    70,    71,    72,   140,   141,
     142,   143,   144,   145,   147,   154,   155,   281,   282,    52,
       7,   226,     4,   226,   226,   226,    48,    58,   143,   242,
     279,   281,     1,    23,    25,    27,    28,    29,    32,    33,
      34,    35,    37,    38,    43,    52,    57,   142,   143,   240,
     241,   242,   263,   265,   266,   267,   268,   269,   272,   273,
     274,   275,   276,   277,   278,   279,   263,   143,   226,   226,
     226,   226,   226,   226,   226,   226,    50,   231,   232,    50,
     231,   231,   217,    15,    44,    52,   135,   234,   183,   204,
     242,   244,   244,   244,   244,   244,   244,   244,    46,    46,
     174,   282,   282,   283,   284,   282,   282,   282,   282,   282,
     282,   282,   282,    46,    46,   282,   282,    50,    52,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      73,    74,    75,    76,    78,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    46,    91,    92,    93,   227,
     279,   280,    46,   140,   143,   147,   243,   279,    56,    52,
      53,   264,    46,    46,    46,    46,   264,    46,   263,   143,
     144,    46,   143,   157,    46,    52,    46,    52,   263,    52,
      46,    56,   151,    56,   282,   218,   157,   214,   242,   216,
     143,   219,   221,   143,   231,    25,    46,    80,    81,   142,
     235,   184,    58,   205,   187,   188,   189,   190,   191,   192,
     193,    55,   143,   172,   173,    47,   143,   175,    52,    47,
      48,    49,    54,   282,   285,   286,   286,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,     8,     9,    82,    83,    84,    99,
     100,   101,   102,   103,   104,   105,   106,   228,   229,   230,
      49,    54,   282,    53,    56,   282,   143,    24,   263,   282,
     282,   265,   279,   282,    36,   282,    52,    52,    39,    40,
     157,   245,   246,   282,   286,   286,   282,   240,   282,   240,
     282,   282,    53,   154,   155,   219,    51,   219,   215,   219,
     232,    52,    54,    56,   251,   252,   143,    46,   280,   143,
      46,   135,   178,   179,   180,   181,   185,   186,   203,   213,
     233,   247,   250,    46,   143,   208,   209,    46,   143,   201,
     202,   201,   201,   201,   201,   201,   201,   143,    56,    47,
      54,    47,    54,   282,   284,    54,    47,    47,    51,    53,
     154,   155,    53,    47,    54,    54,   279,    47,   243,   282,
      52,   264,    47,    47,    52,    56,    47,    47,   282,   282,
      47,    47,   129,    47,    47,    47,   282,   282,    52,    52,
     282,    53,    53,   143,   221,   220,   221,   242,   282,    52,
      46,   248,   249,   282,    41,    42,   236,    56,   237,   238,
     239,   245,    55,   206,   207,   282,   231,    52,    54,   285,
     231,    52,    54,    52,    52,    52,    52,    52,    52,    46,
     282,   173,   143,    49,   282,   282,    53,    53,   282,   230,
     229,    53,    52,    24,   263,   263,   282,   263,   263,   246,
      52,    52,   270,    52,    52,    51,   282,   282,    56,    56,
      52,    54,   282,    52,   253,     7,   221,   222,   259,   260,
      52,    46,    47,   280,   282,    54,    47,   131,   143,    47,
      54,    46,   209,    47,    46,   202,   282,    51,   282,   282,
      47,    47,   243,    26,    52,    30,    31,   271,   282,    51,
      51,   282,   282,   221,    52,   222,   223,   224,   225,   254,
     263,   231,    47,    54,   253,   259,    46,    47,    52,   237,
      52,   282,    46,    55,   282,    47,    55,   210,   211,   212,
     282,   285,    47,    51,    51,   263,   265,    53,    53,    52,
      52,   231,   231,   231,   258,    16,   261,    52,   260,   263,
      47,   280,    56,   282,   143,   143,    47,    54,    54,    47,
      47,   263,   263,   255,   256,   257,   220,   221,   253,    45,
      52,   236,   282,    47,    46,    46,   282,    55,   263,   220,
     220,   220,    52,   263,   253,   280,    52,   282,   282,   143,
      52,    52,    52,    16,   263,    47,    47,    47,    46,    45,
      56,   282,   282,    47,    52
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 110 "vgrammar.y"
    { VerBreakpoint((yyvsp[(2) - (5)].I),(yyvsp[(4) - (5)].E)); }
    break;

  case 9:
#line 121 "vgrammar.y"
    { VerEndModule(); }
    break;

  case 22:
#line 160 "vgrammar.y"
    { VerNewModule((yyvsp[(2) - (2)].S)); }
    break;

  case 28:
#line 172 "vgrammar.y"
    { VerParmDef((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E),1); }
    break;

  case 29:
#line 173 "vgrammar.y"
    { VerParmDef((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].E),1); }
    break;

  case 33:
#line 182 "vgrammar.y"
    { VerPort((yyvsp[(1) - (1)].S)); }
    break;

  case 34:
#line 183 "vgrammar.y"
    { VerPort((yyvsp[(3) - (3)].S)); }
    break;

  case 49:
#line 218 "vgrammar.y"
    { VerAssign((yyvsp[(2) - (5)].E),(yyvsp[(4) - (5)].E),0); }
    break;

  case 50:
#line 219 "vgrammar.y"
    { VerAssign((yyvsp[(3) - (6)].E),(yyvsp[(5) - (6)].E),(yyvsp[(2) - (6)].E)); }
    break;

  case 51:
#line 222 "vgrammar.y"
    { VerParmDef((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E),0); }
    break;

  case 52:
#line 225 "vgrammar.y"
    { VerIABlock(INITIALB,(yyvsp[(2) - (2)].SD)); }
    break;

  case 53:
#line 228 "vgrammar.y"
    { VerIABlock(ALWAYS,(yyvsp[(2) - (2)].SD)); }
    break;

  case 54:
#line 238 "vgrammar.y"
    { VerBeginEScript((yyvsp[(1) - (1)].S)); }
    break;

  case 55:
#line 238 "vgrammar.y"
    { VerEndEScript(); }
    break;

  case 69:
#line 274 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 71:
#line 275 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 73:
#line 276 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 75:
#line 277 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 77:
#line 278 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 79:
#line 279 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 81:
#line 280 "vgrammar.y"
    { VerGateDecl((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].E)); }
    break;

  case 109:
#line 295 "vgrammar.y"
    { VerGateInst((yyvsp[(1) - (5)].S),(yyvsp[(2) - (5)].R),(yyvsp[(4) - (5)].L)); }
    break;

  case 110:
#line 296 "vgrammar.y"
    { VerGateInst(0,0,(yyvsp[(2) - (3)].L)); }
    break;

  case 111:
#line 313 "vgrammar.y"
    { VerModDecl((yyvsp[(1) - (1)].S)); }
    break;

  case 116:
#line 325 "vgrammar.y"
    { VerModDeclParm(0,(yyvsp[(1) - (1)].E)); }
    break;

  case 117:
#line 326 "vgrammar.y"
    { VerModDeclParm((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E)); }
    break;

  case 118:
#line 327 "vgrammar.y"
    { VerModDeclParm(0,(yyvsp[(3) - (3)].E)); }
    break;

  case 119:
#line 328 "vgrammar.y"
    { VerModDeclParm((yyvsp[(4) - (7)].S),(yyvsp[(6) - (7)].E)); }
    break;

  case 122:
#line 341 "vgrammar.y"
    { VerModInst((yyvsp[(1) - (5)].S),(yyvsp[(2) - (5)].R),(yyvsp[(4) - (5)].L)); }
    break;

  case 123:
#line 342 "vgrammar.y"
    { VerModInst((yyvsp[(1) - (4)].S),(yyvsp[(2) - (4)].R),VerEmptyList()); }
    break;

  case 124:
#line 343 "vgrammar.y"
    { VerModInst((yyvsp[(1) - (2)].S),(yyvsp[(2) - (2)].R),VerEmptyList()); }
    break;

  case 127:
#line 351 "vgrammar.y"
    { (yyval.L) = VerListAppend(VerEmptyList(),new_NameExpr(0,(yyvsp[(1) - (1)].E))); }
    break;

  case 128:
#line 352 "vgrammar.y"
    { (yyval.L) = VerListAppend((yyvsp[(1) - (3)].L),new_NameExpr(0,(yyvsp[(3) - (3)].E))); }
    break;

  case 129:
#line 355 "vgrammar.y"
    { (yyval.L) = VerListAppend(VerEmptyList(),new_NameExpr((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E))); }
    break;

  case 130:
#line 356 "vgrammar.y"
    { (yyval.L) = VerListAppend((yyvsp[(1) - (7)].L),new_NameExpr((yyvsp[(4) - (7)].S),(yyvsp[(6) - (7)].E))); }
    break;

  case 131:
#line 374 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].R)); }
    break;

  case 133:
#line 375 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (3)].I),(yyvsp[(2) - (3)].R)); }
    break;

  case 134:
#line 375 "vgrammar.y"
    { VerAssign(new_Expr_lit((yyvsp[(5) - (8)].S)),(yyvsp[(7) - (8)].E),(yyvsp[(3) - (8)].E)); }
    break;

  case 135:
#line 376 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].R)); }
    break;

  case 137:
#line 377 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (1)].I),0); }
    break;

  case 139:
#line 378 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].R)); }
    break;

  case 141:
#line 379 "vgrammar.y"
    { VerAutoAssign((yyvsp[(1) - (8)].I),(yyvsp[(5) - (8)].S),(yyvsp[(7) - (8)].E)); }
    break;

  case 144:
#line 384 "vgrammar.y"
    { VerDeclAssign(new_Expr_lit((yyvsp[(1) - (4)].S)),(yyvsp[(3) - (4)].E),0); }
    break;

  case 145:
#line 385 "vgrammar.y"
    { VerDeclAssign(new_Expr_lit((yyvsp[(1) - (5)].S)),(yyvsp[(4) - (5)].E),(yyvsp[(3) - (5)].E)); }
    break;

  case 148:
#line 394 "vgrammar.y"
    { VerDecl((yyvsp[(1) - (1)].S),0); (yyval.S) = (yyvsp[(1) - (1)].S); }
    break;

  case 149:
#line 395 "vgrammar.y"
    { VerDecl((yyvsp[(1) - (2)].S),(yyvsp[(2) - (2)].R)); (yyval.S) = (yyvsp[(1) - (2)].S); }
    break;

  case 150:
#line 398 "vgrammar.y"
    { (yyval.I) = NT_INPUT; }
    break;

  case 151:
#line 399 "vgrammar.y"
    { (yyval.I) = NT_OUTPUT; }
    break;

  case 152:
#line 400 "vgrammar.y"
    { (yyval.I) = NT_INOUT; }
    break;

  case 153:
#line 401 "vgrammar.y"
    { (yyval.I) = NT_REG_OUTPUT; }
    break;

  case 154:
#line 402 "vgrammar.y"
    { (yyval.I) = NT_REG_OUTPUT; }
    break;

  case 155:
#line 405 "vgrammar.y"
    { (yyval.I) = NT_WIRE|(yyvsp[(2) - (2)].I); }
    break;

  case 156:
#line 406 "vgrammar.y"
    { (yyval.I) = NT_WAND|(yyvsp[(2) - (2)].I); }
    break;

  case 157:
#line 407 "vgrammar.y"
    { (yyval.I) = NT_WOR|(yyvsp[(2) - (2)].I); }
    break;

  case 158:
#line 408 "vgrammar.y"
    { (yyval.I) = NT_TRI|(yyvsp[(2) - (2)].I); }
    break;

  case 159:
#line 409 "vgrammar.y"
    { (yyval.I) = NT_TRI1|(yyvsp[(2) - (2)].I); }
    break;

  case 160:
#line 410 "vgrammar.y"
    { (yyval.I) = NT_TRI0|(yyvsp[(2) - (2)].I); }
    break;

  case 161:
#line 411 "vgrammar.y"
    { (yyval.I) = NT_TRIAND|(yyvsp[(2) - (2)].I); }
    break;

  case 162:
#line 412 "vgrammar.y"
    { (yyval.I) = NT_TRIOR|(yyvsp[(2) - (2)].I); }
    break;

  case 163:
#line 413 "vgrammar.y"
    { (yyval.I) = NT_TRIREG|(yyvsp[(2) - (2)].I); }
    break;

  case 164:
#line 414 "vgrammar.y"
    { (yyval.I) = NT_SUPPLY0|(yyvsp[(2) - (2)].I); }
    break;

  case 165:
#line 415 "vgrammar.y"
    { (yyval.I) = NT_SUPPLY1|(yyvsp[(2) - (2)].I); }
    break;

  case 166:
#line 418 "vgrammar.y"
    { (yyval.I) = NT_REG|(yyvsp[(2) - (2)].I); }
    break;

  case 167:
#line 421 "vgrammar.y"
    { (yyval.I) = NT_INTEGER; }
    break;

  case 168:
#line 422 "vgrammar.y"
    { (yyval.I) = NT_REAL; }
    break;

  case 169:
#line 423 "vgrammar.y"
    { (yyval.I) = NT_TIME; }
    break;

  case 170:
#line 424 "vgrammar.y"
    { (yyval.I) = NT_EVENT; }
    break;

  case 171:
#line 427 "vgrammar.y"
    {(yyval.I) = 0; }
    break;

  case 172:
#line 428 "vgrammar.y"
    {(yyval.I) = (yyvsp[(1) - (2)].I)|(yyvsp[(2) - (2)].I); }
    break;

  case 173:
#line 431 "vgrammar.y"
    { (yyval.I) = NT_P_VECTORED; }
    break;

  case 174:
#line 432 "vgrammar.y"
    { (yyval.I) = NT_P_SCALAR; }
    break;

  case 175:
#line 433 "vgrammar.y"
    { (yyval.I) = NT_P_SIGNED; }
    break;

  case 176:
#line 434 "vgrammar.y"
    { (yyval.I) = (yyvsp[(2) - (3)].I); }
    break;

  case 177:
#line 435 "vgrammar.y"
    { (yyval.I) = (yyvsp[(2) - (5)].I)|(yyvsp[(4) - (5)].I); }
    break;

  case 178:
#line 436 "vgrammar.y"
    { (yyval.I) = (yyvsp[(2) - (5)].I)|(yyvsp[(4) - (5)].I); }
    break;

  case 179:
#line 439 "vgrammar.y"
    { (yyval.I) = NT_P_SMALL; }
    break;

  case 180:
#line 440 "vgrammar.y"
    { (yyval.I) = NT_P_MEDIUM; }
    break;

  case 181:
#line 441 "vgrammar.y"
    { (yyval.I) = NT_P_LARGE; }
    break;

  case 182:
#line 445 "vgrammar.y"
    { (yyval.I) = NT_P_SUPPLY0; }
    break;

  case 183:
#line 446 "vgrammar.y"
    { (yyval.I) = NT_P_STRONG0; }
    break;

  case 184:
#line 447 "vgrammar.y"
    { (yyval.I) = NT_P_PULL0; }
    break;

  case 185:
#line 448 "vgrammar.y"
    { (yyval.I) = NT_P_WEAK0; }
    break;

  case 186:
#line 449 "vgrammar.y"
    { (yyval.I) = NT_P_HIGHZ0; }
    break;

  case 187:
#line 451 "vgrammar.y"
    { (yyval.I) = NT_P_SUPPLY1; }
    break;

  case 188:
#line 452 "vgrammar.y"
    { (yyval.I) = NT_P_STRONG1; }
    break;

  case 189:
#line 453 "vgrammar.y"
    { (yyval.I) = NT_P_PULL1; }
    break;

  case 190:
#line 454 "vgrammar.y"
    { (yyval.I) = NT_P_WEAK1; }
    break;

  case 191:
#line 455 "vgrammar.y"
    { (yyval.I) = NT_P_HIGHZ1; }
    break;

  case 192:
#line 458 "vgrammar.y"
    { (yyval.R) = VerRange(RS_MAXMIN,new_Expr_num(0),new_Expr_num(0)); }
    break;

  case 193:
#line 459 "vgrammar.y"
    { (yyval.R) = (yyvsp[(1) - (1)].R); }
    break;

  case 194:
#line 462 "vgrammar.y"
    { (yyval.R) = VerRange(RS_MAXMIN,(yyvsp[(2) - (5)].E),(yyvsp[(4) - (5)].E)); }
    break;

  case 195:
#line 463 "vgrammar.y"
    { (yyval.R) = VerRange(RS_BASEUP,(yyvsp[(2) - (6)].E),(yyvsp[(5) - (6)].E)); }
    break;

  case 196:
#line 464 "vgrammar.y"
    { (yyval.R) = VerRange(RS_BASEDN,(yyvsp[(2) - (6)].E),(yyvsp[(5) - (6)].E)); }
    break;

  case 200:
#line 491 "vgrammar.y"
    { VerSpecTask((yyvsp[(1) - (5)].S),(yyvsp[(3) - (5)].L)); }
    break;

  case 201:
#line 492 "vgrammar.y"
    { VerSpecify(0,(yyvsp[(3) - (8)].I),(yyvsp[(2) - (8)].E),(yyvsp[(4) - (8)].E),(yyvsp[(7) - (8)].E)); }
    break;

  case 202:
#line 494 "vgrammar.y"
    { VerSpecify((yyvsp[(3) - (12)].E),(yyvsp[(7) - (12)].I),(yyvsp[(6) - (12)].E),(yyvsp[(8) - (12)].E),(yyvsp[(11) - (12)].E)); }
    break;

  case 203:
#line 495 "vgrammar.y"
    { VerSpecParam((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E)); }
    break;

  case 204:
#line 498 "vgrammar.y"
    { (yyval.I) = SPT_MPATH; }
    break;

  case 205:
#line 499 "vgrammar.y"
    { (yyval.I) = SPT_DPATH; }
    break;

  case 206:
#line 503 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 207:
#line 504 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_COND,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 208:
#line 507 "vgrammar.y"
    { (yyval.L) = VerListAppend(0,(yyvsp[(1) - (1)].E)); }
    break;

  case 209:
#line 508 "vgrammar.y"
    { (yyval.L) = VerListAppend((yyvsp[(1) - (3)].L),(yyvsp[(3) - (3)].E)); }
    break;

  case 210:
#line 511 "vgrammar.y"
    { (yyval.L) = 0; }
    break;

  case 211:
#line 512 "vgrammar.y"
    { (yyval.L) = (yyvsp[(1) - (1)].L); }
    break;

  case 212:
#line 527 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 213:
#line 528 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 214:
#line 531 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_AT,(yyvsp[(3) - (4)].E)); }
    break;

  case 215:
#line 532 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_AT,new_Expr_lit((yyvsp[(2) - (2)].S))); }
    break;

  case 216:
#line 533 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_AT,0); }
    break;

  case 217:
#line 534 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_AT,0); }
    break;

  case 218:
#line 537 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_DELAY,(yyvsp[(2) - (2)].E)); }
    break;

  case 219:
#line 538 "vgrammar.y"
    { (yyval.E) = new_Expr_op3(E_DELAY,(yyvsp[(2) - (6)].E),(yyvsp[(4) - (6)].E),(yyvsp[(6) - (6)].E)); }
    break;

  case 220:
#line 541 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 221:
#line 542 "vgrammar.y"
    { (yyval.E) = new_Expr_lit((yyvsp[(1) - (1)].S)); }
    break;

  case 222:
#line 543 "vgrammar.y"
    { (yyval.E) = new_Expr_num((yyvsp[(1) - (1)].I)); }
    break;

  case 223:
#line 544 "vgrammar.y"
    { (yyval.E) = new_Expr_realnum((yyvsp[(1) - (1)].F)); }
    break;

  case 224:
#line 547 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 225:
#line 548 "vgrammar.y"
    { (yyval.E) = 0; }
    break;

  case 226:
#line 551 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 227:
#line 552 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_EVENTOR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 228:
#line 555 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 229:
#line 556 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_POSEDGE,(yyvsp[(2) - (2)].E)); }
    break;

  case 230:
#line 557 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_NEGEDGE,(yyvsp[(2) - (2)].E)); }
    break;

  case 231:
#line 572 "vgrammar.y"
    { VerBeginTask((yyvsp[(4) - (4)].S),(yyvsp[(1) - (4)].I)); VerBeginDecls(NT_OUTPUT|NT_P_REG,(yyvsp[(3) - (4)].R)); VerDecl((yyvsp[(4) - (4)].S),0); VerTaskToFunc((yyvsp[(3) - (4)].R)); }
    break;

  case 232:
#line 573 "vgrammar.y"
    { VerEndTask((yyvsp[(8) - (9)].SD)); }
    break;

  case 233:
#line 575 "vgrammar.y"
    { VerBeginTask((yyvsp[(4) - (4)].S),(yyvsp[(1) - (4)].I)); VerBeginDecls(NT_OUTPUT|NT_P_REG,(yyvsp[(3) - (4)].R)); VerDecl((yyvsp[(4) - (4)].S),0); VerTaskToFunc((yyvsp[(3) - (4)].R)); }
    break;

  case 234:
#line 577 "vgrammar.y"
    { VerEndTask((yyvsp[(11) - (12)].SD)); }
    break;

  case 235:
#line 590 "vgrammar.y"
    { VerBeginTask((yyvsp[(3) - (3)].S),(yyvsp[(1) - (3)].I)); }
    break;

  case 236:
#line 590 "vgrammar.y"
    { VerEndTask((yyvsp[(7) - (8)].SD)); }
    break;

  case 237:
#line 591 "vgrammar.y"
    { VerBeginTask((yyvsp[(3) - (3)].S),(yyvsp[(1) - (3)].I)); }
    break;

  case 238:
#line 592 "vgrammar.y"
    { VerEndTask((yyvsp[(10) - (11)].SD)); }
    break;

  case 241:
#line 599 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I)|NT_P_REG,(yyvsp[(2) - (2)].R)); }
    break;

  case 243:
#line 600 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].R)); }
    break;

  case 245:
#line 601 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I),(yyvsp[(2) - (2)].R)); }
    break;

  case 247:
#line 602 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (1)].I),0); }
    break;

  case 252:
#line 610 "vgrammar.y"
    { VerBeginDecls((yyvsp[(1) - (2)].I)|NT_P_REG,(yyvsp[(2) - (2)].R)); }
    break;

  case 254:
#line 613 "vgrammar.y"
    { (yyval.I) = 1; }
    break;

  case 255:
#line 614 "vgrammar.y"
    { (yyval.I) = 0; }
    break;

  case 256:
#line 629 "vgrammar.y"
    { (yyval.SD) = new_SDBlock(0,(yyvsp[(2) - (3)].L)); }
    break;

  case 257:
#line 630 "vgrammar.y"
    { (yyval.SD) = new_SDBlock((yyvsp[(3) - (5)].S),(yyvsp[(4) - (5)].L)); }
    break;

  case 259:
#line 632 "vgrammar.y"
    { (yyval.SD) = new_SDAsgn((yyvsp[(1) - (5)].E),(yyvsp[(4) - (5)].E),0,1); }
    break;

  case 260:
#line 633 "vgrammar.y"
    { (yyval.SD) = new_SDAsgn((yyvsp[(1) - (4)].E),(yyvsp[(3) - (4)].E),0,0); }
    break;

  case 261:
#line 634 "vgrammar.y"
    { (yyval.SD) = new_SDAsgn((yyvsp[(1) - (5)].E),(yyvsp[(4) - (5)].E),(yyvsp[(3) - (5)].E),0); }
    break;

  case 272:
#line 645 "vgrammar.y"
    { (yyval.SD) = 0; }
    break;

  case 273:
#line 646 "vgrammar.y"
    { (yyval.SD) = VerCondStat((yyvsp[(1) - (2)].E),(yyvsp[(2) - (2)].SD)); }
    break;

  case 274:
#line 647 "vgrammar.y"
    { (yyval.SD) = new_SDNull(); }
    break;

  case 275:
#line 650 "vgrammar.y"
    { (yyval.L) = VerEmptyList(); }
    break;

  case 276:
#line 651 "vgrammar.y"
    { (yyval.L) = VerListAppend((yyvsp[(1) - (2)].L),(yyvsp[(2) - (2)].SD)); }
    break;

  case 277:
#line 654 "vgrammar.y"
    { (yyval.SD) = new_SDAsgn((yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E),0,1); }
    break;

  case 278:
#line 669 "vgrammar.y"
    { (yyval.SD) = new_SDIf((yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].SD),0); }
    break;

  case 279:
#line 670 "vgrammar.y"
    { (yyval.SD) = new_SDIf((yyvsp[(3) - (7)].E),(yyvsp[(5) - (7)].SD),(yyvsp[(7) - (7)].SD)); }
    break;

  case 280:
#line 682 "vgrammar.y"
    { (yyval.SD) = new_SDTask((yyvsp[(1) - (5)].S),(yyvsp[(3) - (5)].L)); }
    break;

  case 281:
#line 683 "vgrammar.y"
    { (yyval.SD) = new_SDTask((yyvsp[(1) - (5)].S),(yyvsp[(3) - (5)].L)); }
    break;

  case 282:
#line 684 "vgrammar.y"
    { (yyval.SD) = new_SDTask((yyvsp[(1) - (2)].S),0); }
    break;

  case 283:
#line 685 "vgrammar.y"
    { (yyval.SD) = new_SDTask((yyvsp[(1) - (2)].S),0); }
    break;

  case 284:
#line 702 "vgrammar.y"
    { (yyval.SD) = new_SDCase((yyvsp[(1) - (6)].I),(yyvsp[(3) - (6)].E),(yyvsp[(5) - (6)].L)); }
    break;

  case 285:
#line 705 "vgrammar.y"
    { (yyval.I) = CASE; }
    break;

  case 286:
#line 706 "vgrammar.y"
    { (yyval.I) = CASEX; }
    break;

  case 287:
#line 707 "vgrammar.y"
    { (yyval.I) = CASEZ; }
    break;

  case 288:
#line 710 "vgrammar.y"
    { (yyval.L) = VerEmptyList(); }
    break;

  case 289:
#line 711 "vgrammar.y"
    { (yyval.L) = VerListAppend((yyvsp[(1) - (2)].L),(yyvsp[(2) - (2)].CE)); }
    break;

  case 290:
#line 714 "vgrammar.y"
    { (yyval.CE) = new_SDCaseEnt((yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].SD)); }
    break;

  case 291:
#line 715 "vgrammar.y"
    { (yyval.CE) = new_SDCaseEnt(0,(yyvsp[(3) - (3)].SD)); }
    break;

  case 292:
#line 729 "vgrammar.y"
    { (yyval.SD) = new_SDFor((yyvsp[(3) - (9)].SD),(yyvsp[(5) - (9)].E),(yyvsp[(7) - (9)].SD),(yyvsp[(9) - (9)].SD)); }
    break;

  case 293:
#line 746 "vgrammar.y"
    { (yyval.SD) = new_SDWhile((yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].SD)); }
    break;

  case 294:
#line 759 "vgrammar.y"
    { (yyval.SD) = new_SDForever((yyvsp[(2) - (2)].SD)); }
    break;

  case 295:
#line 773 "vgrammar.y"
    { (yyval.SD) = new_SDWait((yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].SD)); }
    break;

  case 296:
#line 785 "vgrammar.y"
    { (yyval.SD) = new_SDRaise(new_Expr_lit((yyvsp[(2) - (3)].S))); }
    break;

  case 297:
#line 786 "vgrammar.y"
    { (yyval.SD) = new_SDRaise(new_Expr_lit((yyvsp[(2) - (3)].S))); }
    break;

  case 298:
#line 799 "vgrammar.y"
    { (yyval.SD) = new_SDRepeat((yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].SD)); }
    break;

  case 299:
#line 815 "vgrammar.y"
    { (yyval.SD) = new_SDFork((yyvsp[(2) - (3)].L)); }
    break;

  case 300:
#line 832 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 301:
#line 833 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 302:
#line 836 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 303:
#line 837 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_CONCAT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 304:
#line 840 "vgrammar.y"
    { (yyval.E) = new_Expr_lit((yyvsp[(1) - (1)].S)); }
    break;

  case 305:
#line 841 "vgrammar.y"
    { (yyval.E) = new_Expr_lit((yyvsp[(1) - (1)].S)); }
    break;

  case 306:
#line 842 "vgrammar.y"
    { (yyval.E) = new_Expr_op3(E_VECTORP,(yyvsp[(1) - (4)].E),(yyvsp[(3) - (4)].E),0); }
    break;

  case 307:
#line 843 "vgrammar.y"
    { (yyval.E) = new_Expr_op3(E_VECTORP,(yyvsp[(1) - (7)].E),(yyvsp[(3) - (7)].E),(yyvsp[(6) - (7)].E)); }
    break;

  case 308:
#line 844 "vgrammar.y"
    { (yyval.E) = new_Expr_op3(E_VECTORN,(yyvsp[(1) - (7)].E),(yyvsp[(3) - (7)].E),(yyvsp[(6) - (7)].E)); }
    break;

  case 309:
#line 845 "vgrammar.y"
    { (yyval.E) = new_Expr_op3(E_RANGE,(yyvsp[(1) - (6)].E),(yyvsp[(3) - (6)].E),(yyvsp[(5) - (6)].E)); }
    break;

  case 310:
#line 848 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_ADD,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 311:
#line 849 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_SUB,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 312:
#line 850 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_MUL,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 313:
#line 851 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_DIV,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 314:
#line 852 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_AND,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 315:
#line 853 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_OR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 316:
#line 854 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_GT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 317:
#line 855 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_LT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 318:
#line 856 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_GE,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 319:
#line 857 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_LE,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 320:
#line 858 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_EQ,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 321:
#line 859 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_NE,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 322:
#line 860 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_EQZ,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 323:
#line 861 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_NEZ,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 324:
#line 862 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_RSHIFT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 325:
#line 863 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_LSHIFT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 326:
#line 864 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_ARSHIFT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 327:
#line 865 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_ALSHIFT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 328:
#line 866 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_MOD,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 329:
#line 867 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_BAND,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 330:
#line 868 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_BOR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 331:
#line 869 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_BXOR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 332:
#line 870 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_BNAND,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 333:
#line 871 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_BNOR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 334:
#line 872 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_BNXOR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 335:
#line 873 "vgrammar.y"
    { (yyval.E) = new_Expr_op3(E_QUEST,(yyvsp[(1) - (5)].E),(yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].E)); }
    break;

  case 336:
#line 874 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_NOT,(yyvsp[(2) - (2)].E)); }
    break;

  case 337:
#line 875 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UINV,(yyvsp[(2) - (2)].E)); }
    break;

  case 338:
#line 877 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UAND,(yyvsp[(2) - (2)].E)); }
    break;

  case 339:
#line 878 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UOR,(yyvsp[(2) - (2)].E)); }
    break;

  case 340:
#line 879 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UXOR,(yyvsp[(2) - (2)].E)); }
    break;

  case 341:
#line 880 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UNAND,(yyvsp[(2) - (2)].E)); }
    break;

  case 342:
#line 881 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UNOR,(yyvsp[(2) - (2)].E)); }
    break;

  case 343:
#line 882 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UNXOR,(yyvsp[(2) - (2)].E)); }
    break;

  case 344:
#line 884 "vgrammar.y"
    { (yyval.E) = new_Expr_op1(E_UNEG,(yyvsp[(2) - (2)].E)); }
    break;

  case 345:
#line 885 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (2)].E); }
    break;

  case 346:
#line 886 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 347:
#line 887 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 348:
#line 888 "vgrammar.y"
    { (yyval.E) = new_Expr_realnum((yyvsp[(1) - (1)].F)); }
    break;

  case 349:
#line 889 "vgrammar.y"
    { (yyval.E) = new_Expr_num((yyvsp[(1) - (1)].I)); }
    break;

  case 350:
#line 890 "vgrammar.y"
    { (yyval.E) = new_Expr_hex((yyvsp[(1) - (1)].S)); }
    break;

  case 351:
#line 891 "vgrammar.y"
    { (yyval.E) = new_Expr_str((yyvsp[(1) - (1)].S)); }
    break;

  case 352:
#line 892 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 353:
#line 893 "vgrammar.y"
    { (yyval.E) = new_Expr_task((yyvsp[(1) - (1)].S),0); }
    break;

  case 354:
#line 894 "vgrammar.y"
    { (yyval.E) = new_Expr_task((yyvsp[(1) - (4)].S),(yyvsp[(3) - (4)].L)); }
    break;

  case 355:
#line 895 "vgrammar.y"
    { (yyval.E) = new_Expr_task((yyvsp[(1) - (4)].S),(yyvsp[(3) - (4)].L)); }
    break;

  case 356:
#line 898 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 357:
#line 899 "vgrammar.y"
    { (yyval.E) = new_Expr_op(E_CONCAT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 358:
#line 902 "vgrammar.y"
    { (yyval.E) = (yyvsp[(1) - (1)].E); }
    break;

  case 359:
#line 903 "vgrammar.y"
    { (yyval.E) = new_Expr_repcat((yyvsp[(1) - (4)].E),(yyvsp[(3) - (4)].E)); }
    break;

  case 360:
#line 906 "vgrammar.y"
    { (yyval.L) = VerListAppend(0,(yyvsp[(1) - (1)].E)); }
    break;

  case 361:
#line 907 "vgrammar.y"
    { (yyval.L) = VerListAppend((yyvsp[(1) - (3)].L),(yyvsp[(3) - (3)].E)); }
    break;

  case 362:
#line 910 "vgrammar.y"
    { (yyval.L) = 0; }
    break;

  case 363:
#line 911 "vgrammar.y"
    { (yyval.L) = (yyvsp[(1) - (1)].L); }
    break;


/* Line 1267 of yacc.c.  */
#line 4084 "vgrammar.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 914 "vgrammar.y"


