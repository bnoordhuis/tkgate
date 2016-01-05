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




/* Copy the first part of user declarations.  */
#line 67 "vgrammar.y"

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
#line 404 "vgrammar.c"

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
#define YYFINAL  29
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   965

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  120
/* YYNRULES -- Number of rules.  */
#define YYNRULES  275
/* YYNRULES -- Number of states.  */
#define YYNSTATES  666

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   391

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
     135,   136
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    11,    12,    13,    19,    22,    23,
      24,    27,    29,    31,    32,    35,    37,    39,    41,    43,
      48,    53,    60,    67,    74,    77,    79,    80,    81,    94,
      95,   100,   103,   106,   107,   115,   119,   120,   132,   134,
     138,   142,   148,   149,   152,   153,   156,   157,   163,   169,
     173,   180,   187,   194,   195,   202,   203,   206,   207,   208,
     223,   229,   230,   238,   242,   249,   253,   254,   256,   258,
     262,   263,   266,   271,   275,   276,   282,   288,   292,   299,
     306,   313,   315,   317,   319,   321,   323,   325,   327,   329,
     331,   333,   335,   337,   339,   341,   343,   344,   347,   351,
     355,   359,   369,   371,   374,   381,   390,   393,   394,   398,
     402,   403,   409,   410,   413,   419,   423,   427,   431,   435,
     438,   440,   442,   444,   446,   449,   450,   459,   460,   461,
     470,   471,   474,   475,   480,   481,   486,   487,   488,   497,
     498,   505,   506,   514,   515,   530,   531,   548,   549,   550,
     565,   566,   567,   568,   584,   585,   586,   587,   609,   611,
     615,   616,   617,   631,   639,   643,   647,   653,   654,   655,
     664,   665,   670,   672,   676,   678,   684,   688,   690,   692,
     694,   695,   697,   699,   703,   710,   713,   715,   722,   723,
     725,   727,   729,   732,   739,   747,   751,   757,   763,   769,
     773,   777,   781,   785,   789,   790,   793,   794,   797,   801,
     803,   805,   807,   808,   811,   821,   822,   833,   835,   838,
     846,   851,   855,   860,   873,   874,   881,   887,   888,   891,
     894,   895,   902,   903,   906,   911,   916,   921,   926,   932,
     938,   939,   944,   948,   952,   956,   960,   964,   968,   972,
     976,   980,   984,   988,   992,   996,   999,  1002,  1006,  1011,
    1013,  1015,  1019,  1020,  1023,  1028,  1034,  1042,  1046,  1049,
    1053,  1058,  1061,  1065,  1071,  1072
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     138,     0,    -1,    -1,    68,    35,   121,    69,   139,   144,
      -1,    -1,    -1,    35,   140,   119,   141,   142,    -1,   145,
     144,    -1,    -1,    -1,   142,   143,    -1,   246,    -1,   243,
      -1,    -1,   144,   145,    -1,   162,    -1,   159,    -1,   146,
      -1,   238,    -1,    68,    88,   121,    69,    -1,    68,    91,
     121,    69,    -1,    68,    92,   120,    59,   117,    69,    -1,
      68,    92,   120,    59,   120,    69,    -1,    68,    92,   120,
      59,   121,    69,    -1,     1,    69,    -1,    55,    -1,    -1,
      -1,    68,     4,   120,    69,   147,   156,    68,    73,    69,
     148,   155,    48,    -1,    -1,   150,   149,   155,    12,    -1,
       1,    48,    -1,     1,    12,    -1,    -1,     9,   120,    61,
     151,   168,    62,    55,    -1,     9,   120,    55,    -1,    -1,
       9,   120,    99,   152,    61,   153,    62,    61,   168,    62,
      55,    -1,   154,    -1,   153,    57,   154,    -1,   120,    59,
     252,    -1,    58,   120,    61,   252,    62,    -1,    -1,   155,
     122,    -1,    -1,   156,   157,    -1,    -1,    68,    83,   158,
     230,    69,    -1,    68,    97,    56,   117,    69,    -1,    68,
      84,    69,    -1,    68,    92,   120,    59,   117,    69,    -1,
      68,    92,   120,    59,   120,    69,    -1,    68,    92,   120,
      59,   121,    69,    -1,    -1,    68,     5,    69,   160,   161,
      48,    -1,    -1,   161,   122,    -1,    -1,    -1,    68,     3,
     120,    69,   163,   167,   170,   180,   182,    12,   164,    68,
      48,    69,    -1,   165,   170,   180,   182,    12,    -1,    -1,
       8,   120,    61,   166,   168,    62,    55,    -1,     8,   120,
      55,    -1,     8,   120,    61,   168,    62,    55,    -1,     8,
     120,    55,    -1,    -1,   169,    -1,   120,    -1,   169,    57,
     120,    -1,    -1,   170,   171,    -1,   173,   181,   120,    55,
      -1,    68,   174,    69,    -1,    -1,    68,    83,   172,   230,
      69,    -1,    68,    97,    56,   117,    69,    -1,    68,    84,
      69,    -1,    68,    92,   120,    59,   117,    69,    -1,    68,
      92,   120,    59,   120,    69,    -1,    68,    92,   120,    59,
     121,    69,    -1,    30,    -1,    32,    -1,    33,    -1,    36,
      -1,    37,    -1,    31,    -1,    34,    -1,    44,    -1,    45,
      -1,    38,    -1,    39,    -1,    40,    -1,    42,    -1,    43,
      -1,    41,    -1,    -1,   174,   175,    -1,   179,   176,   179,
      -1,    72,    56,   117,    -1,    93,    56,   117,    -1,    93,
      56,    61,   117,    57,   117,    57,   117,    62,    -1,   177,
      -1,   176,   177,    -1,    61,   178,   117,    57,   117,    62,
      -1,    61,   178,   117,    56,   117,    57,   117,    62,    -1,
      99,    56,    -1,    -1,    63,   117,    64,    -1,    68,    73,
      69,    -1,    -1,    65,   117,    56,   117,    66,    -1,    -1,
     182,   183,    -1,   218,    55,    68,   230,    69,    -1,    68,
     193,    69,    -1,    68,   196,    69,    -1,    68,   184,    69,
      -1,    68,   190,    69,    -1,    68,   186,    -1,   208,    -1,
     213,    -1,   199,    -1,   202,    -1,     1,    69,    -1,    -1,
      80,   120,   185,   230,    69,   192,    68,    87,    -1,    -1,
      -1,    89,   120,   187,   230,    69,   188,   189,    48,    -1,
      -1,   189,   122,    -1,    -1,    81,   120,   191,   230,    -1,
      -1,   192,    68,   230,    69,    -1,    -1,    -1,    79,   120,
     194,    61,   120,    62,   195,   230,    -1,    -1,   120,    61,
     197,   225,    62,   230,    -1,    -1,   120,   120,    61,   198,
     225,    62,   230,    -1,    -1,    11,   120,    59,   120,    65,
     117,    66,    55,    68,   120,   120,   200,   230,    69,    -1,
      -1,    11,   120,    59,   120,    65,   117,    56,   117,    66,
      55,    68,   120,   120,   201,   230,    69,    -1,    -1,    -1,
      11,   120,    59,    63,   203,   212,    64,    55,    68,   120,
     120,   204,   230,    69,    -1,    -1,    -1,    -1,    11,    63,
     205,   212,    64,    59,   120,   206,    55,    68,   120,   120,
     207,   230,    69,    -1,    -1,    -1,    -1,    85,   120,    65,
     117,    56,   117,    66,    61,   209,    63,   212,    64,    57,
     120,   210,    62,    55,   211,    68,   230,    69,    -1,   120,
      -1,   212,    57,   120,    -1,    -1,    -1,    85,   120,    61,
     214,   227,    57,   216,   215,    62,    55,    68,   230,    69,
      -1,    58,   120,    61,   229,   120,   217,    62,    -1,   229,
     120,   217,    -1,    65,   117,    66,    -1,    65,   117,    56,
     117,    66,    -1,    -1,    -1,   120,   219,   221,   120,    61,
     220,   225,    62,    -1,    -1,    99,    61,   222,    62,    -1,
     223,    -1,   222,    57,   223,    -1,   224,    -1,    58,   120,
      61,   224,    62,    -1,   120,    59,   224,    -1,   117,    -1,
     118,    -1,   121,    -1,    -1,   226,    -1,   227,    -1,   226,
      57,   227,    -1,    58,   120,    61,   229,   228,    62,    -1,
     229,   120,    -1,   120,    -1,    63,   117,    63,   120,    64,
      64,    -1,    -1,    74,    -1,   109,    -1,   231,    -1,   230,
     231,    -1,    67,    61,   117,    57,   117,    62,    -1,    78,
      56,    61,   117,    57,   117,    62,    -1,    70,    56,   117,
      -1,    71,    56,    65,   232,    66,    -1,    75,    56,    65,
     233,    66,    -1,    77,    56,    65,   236,    66,    -1,    72,
      56,   117,    -1,    97,    56,   117,    -1,   120,    56,   117,
      -1,   120,    56,   121,    -1,   120,    56,   120,    -1,    -1,
     232,   117,    -1,    -1,   233,   234,    -1,   120,   235,   117,
      -1,   100,    -1,   101,    -1,    59,    -1,    -1,   236,   237,
      -1,   120,   235,   120,   181,    61,   117,    82,   117,    62,
      -1,    -1,    68,     6,    56,   117,    69,   239,   240,    68,
      48,    69,    -1,   241,    -1,   240,   241,    -1,    68,    94,
     120,   117,   117,   117,    69,    -1,    68,    95,   121,    69,
      -1,    68,    96,    69,    -1,    68,    76,   242,    69,    -1,
     120,   120,   181,    67,    61,   117,    57,   117,    62,    70,
      56,   117,    -1,    -1,    14,   120,   244,    63,   245,    64,
      -1,    14,    28,    63,   245,    64,    -1,    -1,   245,   246,
      -1,   245,    55,    -1,    -1,    13,   120,   247,    63,   248,
      64,    -1,    -1,   248,   249,    -1,    16,    59,   252,    55,
      -1,    16,    59,   253,   256,    -1,    17,    59,   252,    55,
      -1,    17,    59,   253,   256,    -1,    15,   250,    59,   252,
      55,    -1,    15,   250,    59,   253,   256,    -1,    -1,   101,
     251,   120,   100,    -1,   252,   131,   252,    -1,   252,   130,
     252,    -1,   252,   133,   252,    -1,   252,   132,   252,    -1,
     252,   125,   252,    -1,   252,   134,   252,    -1,   252,   124,
     252,    -1,   252,   100,   252,    -1,   252,   101,   252,    -1,
     252,   128,   252,    -1,   252,   127,   252,    -1,   252,   102,
     252,    -1,   252,   126,   252,    -1,    74,   252,    -1,   130,
     252,    -1,    61,   252,    62,    -1,   120,    61,   252,    62,
      -1,   120,    -1,   117,    -1,    63,   254,    64,    -1,    -1,
     254,   255,    -1,   120,    59,   252,    55,    -1,    18,    61,
     252,    62,   255,    -1,    18,    61,   252,    62,   255,    19,
     255,    -1,    63,   254,    64,    -1,    25,    55,    -1,    21,
     117,    56,    -1,    21,   130,   117,    56,    -1,    28,    56,
      -1,    24,   252,    55,    -1,    20,    61,   252,    62,   255,
      -1,    -1,    55,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    98,    98,    98,    99,    99,    99,   100,   101,   108,
     109,   112,   113,   119,   120,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   145,   147,   145,   148,
     148,   149,   150,   153,   153,   154,   155,   155,   158,   159,
     162,   163,   167,   168,   171,   172,   175,   175,   176,   177,
     178,   179,   180,   187,   186,   190,   191,   197,   198,   197,
     200,   203,   203,   204,   206,   207,   210,   211,   214,   215,
     219,   220,   223,   224,   225,   225,   226,   227,   228,   229,
     230,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   250,   251,   254,   255,
     256,   257,   260,   261,   271,   272,   275,   276,   279,   282,
     285,   286,   289,   290,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   306,   306,   309,   309,   309,
     312,   313,   317,   317,   320,   321,   324,   324,   324,   327,
     327,   328,   328,   332,   331,   335,   334,   339,   340,   339,
     341,   341,   342,   341,   346,   349,   350,   346,   354,   355,
     361,   362,   361,   365,   366,   369,   370,   373,   373,   373,
     376,   377,   380,   381,   384,   385,   386,   389,   390,   391,
     394,   395,   398,   399,   402,   403,   410,   411,   414,   415,
     416,   421,   422,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   438,   439,   442,   443,   446,   449,
     450,   451,   454,   455,   458,   465,   465,   470,   471,   474,
     475,   476,   477,   480,   489,   489,   490,   493,   494,   495,
     498,   498,   501,   502,   505,   506,   507,   508,   509,   510,
     516,   516,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   540,   543,   544,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   559,   560
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NETLISTBEGIN", "HDLBEGIN",
  "BUILTINBEGIN", "SYMBOLBEGIN", "XMODULE", "NETMODULE", "HDLMODULE",
  "BUILTINMODULE", "ASSIGN", "ENDMODULE", "PRIMITIVE", "TECHNOLOGY",
  "DELAY", "AREA", "POWER", "IF", "ELSE", "SWITCH", "CASE", "CASEX",
  "CASEZ", "RETURN", "BREAK", "NEXT", "FUNC", "DEFAULT", "INITIALB",
  "INPUT", "REG", "OUTPUT", "INOUT", "WIRE", "KWVERSION", "SUPPLY0",
  "SUPPLY1", "KWTRI", "TRI0", "TRI1", "TRIREG", "TRIAND", "TRIOR", "WAND",
  "WOR", "DEASSIGN", "BBEGIN", "END", "ENDCASE", "ALWAYS", "POSEDGE",
  "NEGEDGE", "INTEGER", "ENDPRIMITIVE", "SEMI", "COLON", "COMMA", "DOT",
  "ASGN", "NBASGN", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACK",
  "RBRACK", "AT", "ANOTATE", "ENDANOTATE", "ROT", "WPLACE", "SHOWNAME",
  "ENDDECLS", "NOT", "PORTS", "PORTDEF", "BDPORTS", "SIZE", "JOINT",
  "COMMENT", "FRAME", "SLASH", "BDESC", "ROOTMODULE", "TRAN", "COMLINE",
  "COMEND", "SCRIPT", "BEGINSCRIPT", "ENDSCRIPT", "REQUIRE", "PROPERTY",
  "DECPOS", "ICONDATA", "DATA", "ICONEND", "SYMBOLREF", "TEXTBEGIN",
  "HASHMARK", "GT", "LT", "EQ", "BAND", "BNAND", "BOR", "BNOR", "BXOR",
  "BNXOR", "UINV", "MOD", "LSHIFT", "RSHIFTEQ", "QUEST", "ARSHIFT",
  "ALSHIFT", "BOGOCHAR", "NUMBER", "HEX", "VERNUM", "LITERAL", "STRING",
  "HDLLINE", "SYSLITERAL", "OR", "AND", "NE", "LE", "GE", "RSHIFT", "SUB",
  "ADD", "DIV", "MUL", "POW", "UPLUS", "UNEG", "$accept", "prog", "@1",
  "@2", "@3", "tunits", "tunit", "vunits", "vunit", "hdl_module", "@4",
  "@5", "@6", "hdl_modhead", "@7", "@8", "mparms", "mparm", "hstats",
  "hprops", "hprop", "@9", "builtin_module", "@10", "skiplines",
  "net_module", "@11", "@12", "net_modhead", "@13", "net_e_modhead",
  "omargs", "margs", "decls", "decl", "@14", "dtype", "wnotes", "wnote",
  "wcoords", "wcoord", "wsizep", "wend", "enddecl", "bwidth", "stats",
  "stat", "comment", "@15", "script", "@16", "@17", "script_lines",
  "frame", "@18", "comlines", "joint", "@19", "@20", "iogate", "@21",
  "@22", "tapassign", "@23", "@24", "concat_assign", "@25", "@26", "@27",
  "@28", "@29", "concat", "@30", "@31", "@32", "concat_args", "tran",
  "@33", "@34", "tran_arg", "tran_range", "call", "@35", "@36",
  "omodparms", "modparms", "modparm", "modparmval", "ocargs", "cargs",
  "carg", "clit", "obang", "cnote", "citem", "wplist", "pplist", "bport",
  "bdir", "bdplist", "bdport", "modsymbol", "@37", "mslines", "msline",
  "portdef", "tech_def", "@38", "primdefs", "primdef", "@39", "pstats",
  "pstat", "dparm", "@40", "pexpr", "pcode", "pc_stats", "pc_stat", "semi", 0
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
     385,   386,   387,   388,   389,   390,   391
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   137,   139,   138,   140,   141,   138,   138,   138,   142,
     142,   143,   143,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   147,   148,   146,   149,
     146,   146,   146,   151,   150,   150,   152,   150,   153,   153,
     154,   154,   155,   155,   156,   156,   158,   157,   157,   157,
     157,   157,   157,   160,   159,   161,   161,   163,   164,   162,
     162,   166,   165,   165,   167,   167,   168,   168,   169,   169,
     170,   170,   171,   171,   172,   171,   171,   171,   171,   171,
     171,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   174,   175,   175,
     175,   175,   176,   176,   177,   177,   178,   178,   179,   180,
     181,   181,   182,   182,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   185,   184,   187,   188,   186,
     189,   189,   191,   190,   192,   192,   194,   195,   193,   197,
     196,   198,   196,   200,   199,   201,   199,   203,   204,   202,
     205,   206,   207,   202,   209,   210,   211,   208,   212,   212,
     214,   215,   213,   216,   216,   217,   217,   219,   220,   218,
     221,   221,   222,   222,   223,   223,   223,   224,   224,   224,
     225,   225,   226,   226,   227,   227,   228,   228,   229,   229,
     229,   230,   230,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   232,   232,   233,   233,   234,   235,
     235,   235,   236,   236,   237,   239,   238,   240,   240,   241,
     241,   241,   241,   242,   244,   243,   243,   245,   245,   245,
     247,   246,   248,   248,   249,   249,   249,   249,   249,   249,
     251,   250,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   253,   254,   254,   255,   255,   255,   255,   255,   255,
     255,   255,   255,   255,   256,   256
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     6,     0,     0,     5,     2,     0,     0,
       2,     1,     1,     0,     2,     1,     1,     1,     1,     4,
       4,     6,     6,     6,     2,     1,     0,     0,    12,     0,
       4,     2,     2,     0,     7,     3,     0,    11,     1,     3,
       3,     5,     0,     2,     0,     2,     0,     5,     5,     3,
       6,     6,     6,     0,     6,     0,     2,     0,     0,    14,
       5,     0,     7,     3,     6,     3,     0,     1,     1,     3,
       0,     2,     4,     3,     0,     5,     5,     3,     6,     6,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     2,     3,     3,
       3,     9,     1,     2,     6,     8,     2,     0,     3,     3,
       0,     5,     0,     2,     5,     3,     3,     3,     3,     2,
       1,     1,     1,     1,     2,     0,     8,     0,     0,     8,
       0,     2,     0,     4,     0,     4,     0,     0,     8,     0,
       6,     0,     7,     0,    14,     0,    16,     0,     0,    14,
       0,     0,     0,    15,     0,     0,     0,    21,     1,     3,
       0,     0,    13,     7,     3,     3,     5,     0,     0,     8,
       0,     4,     1,     3,     1,     5,     3,     1,     1,     1,
       0,     1,     1,     3,     6,     2,     1,     6,     0,     1,
       1,     1,     2,     6,     7,     3,     5,     5,     5,     3,
       3,     3,     3,     3,     0,     2,     0,     2,     3,     1,
       1,     1,     0,     2,     9,     0,    10,     1,     2,     7,
       4,     3,     4,    12,     0,     6,     5,     0,     2,     2,
       0,     6,     0,     2,     4,     4,     4,     4,     5,     5,
       0,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     3,     4,     1,
       1,     3,     0,     2,     4,     5,     7,     3,     2,     3,
       4,     2,     3,     5,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,     0,     0,     4,    25,     0,     0,    13,    17,
      29,    16,    15,    70,    18,    32,    31,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,    42,     0,    63,    61,    35,    33,    36,     5,     0,
       0,    53,     0,     0,     0,     0,     0,     0,    14,     0,
      81,    86,    82,    83,    87,    84,    85,    90,    91,    92,
      95,    93,    94,    88,    89,    96,    71,   110,   112,    66,
      66,     0,     9,    57,    26,    55,     0,     2,    19,    20,
       0,    30,    43,     0,    74,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    67,     0,     0,     6,     0,    44,
       0,   215,    13,     0,     0,     0,   109,     0,    77,     0,
       0,     0,    73,     0,     0,    97,     0,     0,     0,     0,
       0,    60,     0,     0,   167,   113,   122,   123,   120,   121,
       0,     0,     0,     0,     0,     0,     0,    38,     0,     0,
      10,    12,    11,     0,    70,     0,    54,    56,     0,     0,
      21,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,     0,     0,     0,     0,     0,   107,
       0,   102,     0,    72,   124,   150,     0,     0,     0,     0,
       0,     0,     0,   119,     0,     0,     0,     0,   170,     0,
      62,    69,    34,     0,     0,     0,     0,   230,     0,   224,
       0,     0,     0,    45,     0,     0,   217,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   192,     0,     0,
       0,    76,   108,    99,     0,   100,     0,     0,   103,    98,
       0,     0,     0,   136,   125,   132,   127,   139,     0,   117,
     118,   115,   116,   160,     0,     0,     0,     0,     0,     0,
       0,   260,   259,     0,    40,    39,    66,     0,   227,     0,
      65,    66,   112,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,   218,     0,   195,   204,   199,   206,   212,
       0,   200,   201,   203,   202,    78,    79,    80,     0,   106,
       0,   111,   158,     0,   147,     0,     0,     0,     0,     0,
     180,   141,   188,     0,     0,     0,     0,     0,     0,   255,
       0,   256,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,   227,     0,
       0,    27,     0,    49,     0,     0,     0,     0,     0,     0,
     221,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   133,     0,     0,   189,
     190,     0,   181,   182,     0,   180,     0,     0,     0,   177,
     178,     0,   179,     0,   172,   174,   168,   114,    41,   257,
       0,   249,   250,   253,   248,   246,   254,   252,   251,   243,
     242,   245,   244,   247,     0,     0,   229,   226,   228,     0,
       0,    58,    42,     0,     0,     0,   110,   222,     0,   220,
     216,     0,   196,   205,   197,     0,   207,   198,     0,   213,
       0,     0,     0,     0,   159,     0,     0,     0,     0,   134,
     128,     0,     0,   188,   185,     0,   188,     0,     0,     0,
       0,   171,   180,   258,    37,     0,     0,     0,   231,   233,
     225,    64,     0,     0,    47,     0,     0,     0,    48,     0,
       0,   193,   211,   209,   210,     0,     0,     0,     0,     0,
     104,   151,     0,     0,     0,   137,     0,   130,   188,   140,
     183,     0,     0,   161,     0,     0,     0,   176,   173,     0,
     240,     0,     0,     0,     0,    28,    50,    51,    52,     0,
       0,   208,   110,   194,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   142,     0,     0,     0,   154,     0,
     169,     0,     0,   262,     0,   274,     0,   274,     0,     0,
     219,     0,   101,   105,     0,     0,     0,     0,   138,   126,
       0,   129,   131,     0,   186,     0,   188,     0,     0,   164,
       0,   175,     0,     0,   274,     0,   234,   275,   235,   236,
     237,    59,     0,     0,     0,     0,     0,     0,   135,     0,
     184,     0,     0,     0,     0,   241,   238,   239,     0,     0,
       0,     0,     0,     0,   262,   261,     0,   263,     0,     0,
       0,   148,     0,   143,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,     0,   268,   271,     0,     0,     0,
       0,   152,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,   272,   267,     0,     0,     0,     0,
       0,   145,     0,     0,   163,   162,   166,     0,     0,     0,
     270,   264,     0,   214,     0,   149,     0,   144,   187,   155,
     265,   273,     0,   153,     0,     0,     0,   223,   146,     0,
     266,   156,     0,     0,     0,   157
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,   102,    20,    72,    97,   140,    30,    48,     9,
      99,   402,    31,    10,    70,    71,   136,   137,    49,   145,
     203,   332,    11,    75,   100,    12,    98,   452,    13,    69,
     144,    93,    94,    32,    66,   107,    67,    88,   115,   170,
     171,   227,   116,    68,    90,    91,   125,   182,   297,   183,
     299,   477,   512,   184,   298,   476,   185,   296,   510,   186,
     300,   365,   126,   614,   646,   127,   352,   612,   231,   506,
     629,   128,   550,   655,   662,   293,   129,   302,   516,   483,
     549,   130,   188,   442,   246,   373,   374,   375,   361,   362,
     363,   545,   364,   162,   163,   343,   344,   416,   465,   345,
     419,    14,   148,   205,   206,   337,   141,   259,   327,   398,
     257,   395,   449,   491,   521,   254,   525,   555,   587,   558
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -477
static const yytype_int16 yypact[] =
{
     236,   159,  -102,   -73,  -477,  -477,    25,   115,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,    72,   141,
     -69,    27,    36,    62,   123,   109,   117,   182,    83,  -477,
     242,  -477,   649,  -477,  -477,  -477,  -477,  -477,  -477,   258,
     265,  -477,   225,   295,   296,   298,   290,    16,  -477,    -3,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,   454,  -477,   305,  -477,   257,
     257,   314,  -477,  -477,  -477,  -477,   310,  -477,  -477,  -477,
     237,  -477,  -477,   312,  -477,   313,   269,   359,   190,   299,
     303,    12,  -477,   365,   371,   374,   -31,   276,   439,  -477,
     -13,  -477,  -477,   384,   391,   392,  -477,   576,  -477,   419,
     358,   364,  -477,   442,   444,  -477,   429,   452,   455,   447,
     -20,  -477,   239,   394,  -477,  -477,  -477,  -477,  -477,  -477,
     462,   466,   404,   473,   410,   475,   273,  -477,   420,   -17,
    -477,  -477,  -477,   428,  -477,   484,  -477,  -477,   491,   421,
    -477,  -477,  -477,   499,   511,   515,   520,   521,   523,   526,
     528,   530,   254,  -477,   346,   522,   533,   477,    68,   506,
     211,  -477,   481,  -477,  -477,  -477,   549,   492,   493,   500,
     501,   -22,   542,  -477,   557,   558,   561,   244,   538,   572,
    -477,  -477,  -477,   571,    81,   -31,   584,  -477,   586,  -477,
     191,   649,   674,  -477,   330,   582,  -477,   540,   541,   590,
     543,   594,   598,   601,   548,   412,  -477,  -477,   595,   599,
     600,  -477,  -477,  -477,   553,  -477,   610,   554,  -477,  -477,
     606,   575,    -8,  -477,  -477,  -477,  -477,  -477,   616,  -477,
    -477,  -477,  -477,  -477,   583,   617,   581,   576,    81,    81,
      81,  -477,   641,    81,   597,  -477,   257,   640,  -477,   643,
    -477,   257,  -477,   635,  -477,   636,   591,   654,   592,   593,
     605,   645,   580,  -477,   658,  -477,  -477,  -477,  -477,  -477,
     619,  -477,  -477,  -477,  -477,  -477,  -477,  -477,   659,  -477,
     255,  -477,  -477,    82,  -477,   688,   693,   576,   576,   576,
      90,  -477,    96,   699,     3,   695,   387,   213,   260,  -477,
      81,  -477,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,   697,  -477,   136,  -477,   698,
      14,  -477,   576,  -477,   702,   646,   644,   696,   650,   708,
    -477,   709,   651,    28,   -24,    -2,   705,   662,   663,   664,
     665,   723,   575,   666,   667,   402,   576,   416,   668,  -477,
    -477,   722,   729,  -477,   669,    90,   733,   675,   671,  -477,
    -477,   734,  -477,   291,  -477,  -477,  -477,  -477,  -477,  -477,
     271,   280,   280,   642,   607,   618,   642,   280,   280,   363,
     363,   660,   660,  -477,   740,    17,  -477,  -477,  -477,   212,
     741,  -477,  -477,   434,   445,   728,   305,  -477,   681,  -477,
    -477,   737,  -477,  -477,  -477,   207,  -477,  -477,   207,  -477,
     683,   744,   745,   742,  -477,   685,   169,   173,   746,  -477,
    -477,   748,   576,    96,  -477,   749,    99,   747,   751,   140,
       3,  -477,    90,  -477,  -477,   706,   755,   756,  -477,  -477,
    -477,  -477,   735,   -12,  -477,   750,   752,   753,  -477,   739,
     700,  -477,  -477,  -477,  -477,   701,   690,   754,   703,   707,
    -477,  -477,   768,   710,   770,  -477,   758,  -477,   -60,   576,
    -477,   576,   711,  -477,   712,   767,   140,  -477,  -477,   771,
    -477,   775,   143,   143,   781,  -477,  -477,  -477,  -477,   769,
     766,  -477,   305,  -477,   774,   776,   782,   772,   773,   777,
     576,   448,   -11,    -6,   576,   780,   784,   778,  -477,   785,
    -477,   724,   143,  -477,    35,   787,    50,   787,   779,   732,
    -477,   789,  -477,  -477,   783,   736,   797,   738,   576,  -477,
     472,  -477,  -477,   743,  -477,   791,   -60,   799,   757,  -477,
     792,  -477,   759,    88,   787,    20,  -477,  -477,  -477,  -477,
    -477,  -477,   800,   760,   761,   762,   793,   763,  -477,   801,
    -477,   764,   794,   250,   575,  -477,  -477,  -477,   802,   804,
      11,    81,   811,   812,  -477,  -477,   808,  -477,   786,   788,
     765,  -477,   790,  -477,   795,   778,   576,   796,  -477,   170,
      81,    81,   813,   803,   154,  -477,  -477,    38,    81,   809,
     805,  -477,   576,   798,   576,   814,   810,   486,   807,   818,
     307,   318,  -477,   820,  -477,  -477,   168,   816,   817,   576,
     503,  -477,   518,   823,  -477,  -477,  -477,   806,    67,    67,
    -477,  -477,   824,  -477,   532,  -477,   576,  -477,  -477,  -477,
     869,  -477,   815,  -477,   547,   827,    67,  -477,  -477,   835,
    -477,  -477,   825,   576,   564,  -477
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -477,  -477,  -477,  -477,  -477,  -477,  -477,   819,   891,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,   704,   490,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,
    -477,   -64,  -477,   821,  -477,  -477,  -477,  -477,  -477,  -477,
     725,  -477,   726,   713,  -396,   632,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,  -350,  -477,  -477,  -477,  -477,
     302,  -477,  -477,  -477,  -477,  -477,   458,  -314,  -349,  -477,
    -295,  -477,  -424,  -246,  -162,  -477,  -477,  -477,   482,  -477,
    -477,  -477,  -477,  -477,   714,  -477,  -477,  -477,   573,   826,
    -477,  -477,  -477,  -477,  -477,  -245,  -476,   320,  -407,  -401
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -189
static const yytype_int16 yytable[] =
{
     217,   306,   426,   307,   308,   309,    95,   366,   311,    81,
     459,   198,   484,   119,   359,   119,   435,   527,    18,    21,
      22,    23,    24,   120,   121,   120,   401,   134,    21,    22,
      23,    24,   445,   446,   447,   146,   495,   541,   578,   237,
     579,   580,   414,   175,   581,   582,   554,    19,   583,   360,
      38,   355,   356,   357,   513,   294,   578,   543,   579,   580,
      25,   368,   581,   582,   417,   380,   583,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     122,   448,   122,   584,   585,   578,   403,   579,   580,   135,
     556,   581,   582,   489,   412,   583,   415,   123,   238,   123,
     176,   584,   625,   199,    26,   559,   531,    27,    28,   147,
      82,   542,   295,    26,   544,    29,    27,    28,   418,    82,
     369,   370,   571,   371,   372,   487,   560,    33,   602,   224,
     584,    41,   124,    34,   124,   312,   313,   314,   480,   350,
     586,   603,   249,   576,   217,   413,   351,    39,   358,   138,
     312,   313,   314,   577,   358,   250,    40,   482,   586,   315,
     316,   317,   318,   319,   359,   320,   321,   322,   323,   324,
     359,    15,   519,   359,   315,   316,   317,   318,   319,    42,
     320,   321,   322,   323,   324,   225,   479,   586,   312,   313,
     314,   396,   325,   217,   217,   217,    35,   329,   251,   360,
     397,   252,    36,    46,   249,   360,   523,    16,   360,   624,
    -188,   253,   315,   316,   317,   318,   319,   250,   320,   321,
     322,   323,   324,   641,   599,   138,   350,   350,    17,   473,
      43,   650,   651,   472,   619,   514,    -8,     1,    44,   474,
      37,   217,    -7,     1,     2,     3,   260,   524,   526,   660,
       2,     3,   261,   111,   312,   313,   314,   369,   370,   112,
     251,   372,   113,   252,   538,   540,   462,   396,   312,   313,
     314,     4,   169,   253,   111,   378,   450,   553,   315,   316,
     317,   318,   319,   114,   320,   321,   322,   323,   324,   138,
     139,     5,   315,   316,   317,   318,   319,     5,   320,   321,
     322,   323,   324,    45,     6,   243,   597,   463,   464,   244,
      47,   348,   349,   312,   313,   314,   598,   217,   177,   178,
     179,   153,   379,   216,   154,   155,   156,    73,   180,   157,
     195,   158,   159,   443,    74,   196,   604,   315,   316,   317,
     318,   319,    76,   320,   321,   322,   323,   324,   440,    80,
     617,   160,   217,   441,   103,   620,   621,   104,   105,   181,
     312,   313,   314,   626,    77,    78,   630,    79,   632,   638,
      89,   312,   313,   314,   161,    96,   217,    92,   217,   101,
     639,   106,   108,   644,   315,   316,   317,   318,   319,   109,
     320,   321,   322,   323,   324,   315,   316,   317,   318,   319,
     654,   320,   321,   322,   323,   324,   268,   312,   313,   314,
     320,   321,   322,   323,   324,   110,   117,   664,   312,   313,
     314,    -3,     1,   118,   269,   270,   271,   131,   132,     2,
       3,   315,   316,   317,   318,   319,   133,   320,   321,   322,
     323,   324,   315,   316,   317,   318,   319,   143,   320,   321,
     322,   323,   324,   150,   153,   217,   377,   154,   155,   156,
     151,   152,   157,   218,   158,   159,   219,   220,   217,   153,
     217,   429,   154,   155,   156,   165,     5,   157,   164,   158,
     159,   166,   217,   153,   160,   430,   154,   155,   156,    47,
     169,   157,   217,   158,   159,   322,   323,   324,   167,   160,
     168,   153,   217,   454,   154,   155,   156,   161,   172,   157,
     173,   158,   159,   160,   187,   153,   174,   189,   154,   155,
     156,   190,   161,   157,   191,   158,   159,    83,   192,   282,
     193,   160,   283,   284,   194,   539,   161,    84,    85,   153,
     197,   568,   154,   155,   156,   160,    86,   157,   200,   158,
     159,    87,   202,   153,   161,   635,   154,   155,   156,   204,
     207,   157,   455,   158,   159,   456,   457,   208,   161,   160,
     153,   209,   645,   154,   155,   156,   210,   211,   157,   212,
     158,   159,   213,   160,   214,   153,   215,   647,   154,   155,
     156,   221,   161,   157,   223,   158,   159,   222,   230,   153,
     160,   653,   154,   155,   156,   226,   161,   157,   232,   158,
     159,   239,   233,   234,   153,   160,   658,   154,   155,   156,
     235,   236,   157,   161,   158,   159,   240,   241,   341,   160,
     242,   153,   248,   665,   154,   155,   156,   245,   161,   157,
     247,   158,   159,   153,   160,   256,   154,   155,   156,   258,
     272,   157,   161,   158,   159,   276,   268,   274,   275,   278,
     277,   160,   280,   279,   285,   281,   289,   161,   286,   287,
     288,   290,   291,   160,   269,   270,   271,   301,   304,    50,
      51,    52,    53,    54,   161,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,   292,   161,   312,   313,   314,
     303,   305,   310,   326,   331,   333,   328,   312,   313,   314,
     335,   334,   336,   338,   340,   342,   347,    65,   312,   313,
     314,   315,   316,   317,   318,   319,   339,   320,   321,   322,
     323,   324,   316,   317,   318,   319,   346,   320,   321,   322,
     323,   324,   312,   313,   317,   318,   319,   263,   320,   321,
     322,   323,   324,   353,   354,   367,   376,   264,   265,   394,
     400,   404,   420,   405,   406,   407,   266,   408,   411,   318,
     319,   267,   320,   321,   322,   323,   324,   409,   410,   421,
     422,   423,   425,   427,   432,   424,   433,   428,   431,   434,
     436,   438,   437,   439,   324,   444,   451,   458,   460,   461,
     467,   468,   469,   494,   470,   471,   499,   490,   475,   478,
     502,   481,   486,   485,   492,   493,   503,   500,   501,   496,
     504,   497,   498,   507,   505,   509,   511,   508,   518,   528,
     529,   515,   517,   520,   522,   530,   532,   534,   533,   536,
     535,   546,   557,   548,   552,   537,   547,   551,   561,   562,
     563,   564,   566,   570,   572,   574,   565,   588,   567,   575,
     569,   592,   596,   600,   594,   601,   605,   608,   606,   622,
     610,   627,   634,   636,   573,   637,   640,   589,   633,   643,
     652,   590,   591,   593,   595,   611,   642,   648,   656,   659,
     661,     8,   453,   663,   330,   228,   229,   616,   488,   255,
     466,   399,     0,   609,   607,     0,     0,     0,     0,     0,
     613,     0,     0,   618,   262,   615,     0,     0,   631,   273,
     623,   149,   628,   142,     0,     0,   649,     0,     0,     0,
       0,     0,   657,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   201
};

static const yytype_int16 yycheck[] =
{
     162,   247,   352,   248,   249,   250,    70,   302,   253,    12,
     406,    28,   436,     1,    74,     1,   365,   493,   120,     3,
       4,     5,     6,    11,    12,    11,    12,    58,     3,     4,
       5,     6,    15,    16,    17,    48,    48,    48,    18,    61,
      20,    21,    66,    63,    24,    25,   522,   120,    28,   109,
     119,   297,   298,   299,   478,    63,    18,    63,    20,    21,
      35,    58,    24,    25,    66,   310,    28,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
      68,    64,    68,    63,    64,    18,   332,    20,    21,   120,
      55,    24,    25,   442,    66,    28,   120,    85,   120,    85,
     120,    63,    64,   120,    88,    55,   502,    91,    92,   122,
     122,   122,   120,    88,   120,     0,    91,    92,   120,   122,
     117,   118,   546,   120,   121,   439,   527,    55,   117,    61,
      63,    69,   120,    61,   120,   100,   101,   102,   433,    57,
     120,   130,    61,    55,   306,   117,    64,   120,    58,    13,
     100,   101,   102,   554,    58,    74,   120,    58,   120,   124,
     125,   126,   127,   128,    74,   130,   131,   132,   133,   134,
      74,    12,   486,    74,   124,   125,   126,   127,   128,    56,
     130,   131,   132,   133,   134,   117,   432,   120,   100,   101,
     102,    55,   256,   355,   356,   357,    55,   261,   117,   109,
      64,   120,    61,   120,    61,   109,    63,    48,   109,    55,
     120,   130,   124,   125,   126,   127,   128,    74,   130,   131,
     132,   133,   134,    55,   574,    13,    57,    57,    69,    56,
     121,   638,   639,    64,    64,   481,     0,     1,   121,    66,
      99,   403,     0,     1,     8,     9,    55,   492,   493,   656,
       8,     9,    61,    63,   100,   101,   102,   117,   118,    69,
     117,   121,    72,   120,   510,   511,    59,    55,   100,   101,
     102,    35,    61,   130,    63,    62,    64,   522,   124,   125,
     126,   127,   128,    93,   130,   131,   132,   133,   134,    13,
      14,    55,   124,   125,   126,   127,   128,    55,   130,   131,
     132,   133,   134,   121,    68,    61,    56,   100,   101,    65,
      68,    56,    57,   100,   101,   102,    66,   479,    79,    80,
      81,    67,    62,    69,    70,    71,    72,    69,    89,    75,
      57,    77,    78,    62,    69,    62,   581,   124,   125,   126,
     127,   128,   117,   130,   131,   132,   133,   134,    57,    59,
     596,    97,   514,    62,   117,   600,   601,   120,   121,   120,
     100,   101,   102,   608,    69,    69,   612,    69,   614,    62,
      65,   100,   101,   102,   120,    61,   538,   120,   540,    69,
      62,    69,    69,   629,   124,   125,   126,   127,   128,   120,
     130,   131,   132,   133,   134,   124,   125,   126,   127,   128,
     646,   130,   131,   132,   133,   134,    76,   100,   101,   102,
     130,   131,   132,   133,   134,    56,   117,   663,   100,   101,
     102,     0,     1,   120,    94,    95,    96,    62,    57,     8,
       9,   124,   125,   126,   127,   128,    62,   130,   131,   132,
     133,   134,   124,   125,   126,   127,   128,     8,   130,   131,
     132,   133,   134,    69,    67,   617,    69,    70,    71,    72,
      69,    69,    75,   117,    77,    78,   120,   121,   630,    67,
     632,    69,    70,    71,    72,   117,    55,    75,    59,    77,
      78,   117,   644,    67,    97,    69,    70,    71,    72,    68,
      61,    75,   654,    77,    78,   132,   133,   134,    56,    97,
      56,    67,   664,    69,    70,    71,    72,   120,    56,    75,
      55,    77,    78,    97,   120,    67,    69,    55,    70,    71,
      72,    55,   120,    75,   120,    77,    78,    73,    55,   117,
     120,    97,   120,   121,    59,    87,   120,    83,    84,    67,
     120,    69,    70,    71,    72,    97,    92,    75,   120,    77,
      78,    97,    68,    67,   120,    69,    70,    71,    72,    68,
      61,    75,   117,    77,    78,   120,   121,    56,   120,    97,
      67,    56,    69,    70,    71,    72,    56,    56,    75,    56,
      77,    78,    56,    97,    56,    67,    56,    69,    70,    71,
      72,    69,   120,    75,   117,    77,    78,    64,   117,    67,
      97,    69,    70,    71,    72,    99,   120,    75,    59,    77,
      78,    69,   120,   120,    67,    97,    69,    70,    71,    72,
     120,   120,    75,   120,    77,    78,    69,    69,    48,    97,
      69,    67,    61,    69,    70,    71,    72,    99,   120,    75,
      68,    77,    78,    67,    97,    61,    70,    71,    72,    63,
      68,    75,   120,    77,    78,    65,    76,   117,   117,    65,
     117,    97,    61,    65,    69,   117,    56,   120,    69,    69,
     117,   117,    66,    97,    94,    95,    96,    61,    61,    30,
      31,    32,    33,    34,   120,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,   120,   120,   100,   101,   102,
     117,   120,    61,    63,    69,    69,    63,   100,   101,   102,
      56,   120,   120,   120,    69,    57,    57,    68,   100,   101,
     102,   124,   125,   126,   127,   128,   121,   130,   131,   132,
     133,   134,   125,   126,   127,   128,   117,   130,   131,   132,
     133,   134,   100,   101,   126,   127,   128,    73,   130,   131,
     132,   133,   134,    65,    61,    56,    61,    83,    84,    62,
      62,    59,    57,   117,   120,    69,    92,   117,   117,   127,
     128,    97,   130,   131,   132,   133,   134,    69,    69,   117,
     117,   117,    59,   117,    62,   120,    57,   120,   120,   120,
      57,   120,   117,    59,   134,    55,    55,    69,   117,    62,
     117,    57,    57,    68,    62,   120,    67,   101,    62,    61,
     120,    62,    61,    66,    59,    59,    62,   117,   117,    69,
     117,    69,    69,    55,   117,    55,    68,   117,    61,    48,
      61,   120,   120,    62,    59,    69,    62,    55,    62,    66,
      68,    61,    55,    65,   120,    68,    62,    62,    69,   117,
      61,    68,    55,    62,    55,    63,   120,    57,   120,   100,
     117,    68,    68,    61,    63,    61,    55,    59,    56,    56,
      82,    62,    62,    66,   117,    57,    56,   117,    64,    62,
      56,   120,   120,   120,   120,   120,    70,    64,    19,    62,
      55,     0,   402,    68,   262,   170,   170,   595,   440,   195,
     418,   328,    -1,   117,   584,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,   117,   201,   120,    -1,    -1,   120,   205,
     117,   102,   117,    97,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     8,     9,    35,    55,    68,   138,   145,   146,
     150,   159,   162,   165,   238,    12,    48,    69,   120,   120,
     140,     3,     4,     5,     6,    35,    88,    91,    92,     0,
     144,   149,   170,    55,    61,    55,    61,    99,   119,   120,
     120,    69,    56,   121,   121,   121,   120,    68,   145,   155,
      30,    31,    32,    33,    34,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    68,   171,   173,   180,   166,
     151,   152,   141,    69,    69,   160,   117,    69,    69,    69,
      59,    12,   122,    73,    83,    84,    92,    97,   174,    65,
     181,   182,   120,   168,   169,   168,    61,   142,   163,   147,
     161,    69,   139,   117,   120,   121,    69,   172,    69,   120,
      56,    63,    69,    72,    93,   175,   179,   117,   120,     1,
      11,    12,    68,    85,   120,   183,   199,   202,   208,   213,
     218,    62,    57,    62,    58,   120,   153,   154,    13,    14,
     143,   243,   246,     8,   167,   156,    48,   122,   239,   144,
      69,    69,    69,    67,    70,    71,    72,    75,    77,    78,
      97,   120,   230,   231,    59,   117,   117,    56,    56,    61,
     176,   177,    56,    55,    69,    63,   120,    79,    80,    81,
      89,   120,   184,   186,   190,   193,   196,   120,   219,    55,
      55,   120,    55,   120,    59,    57,    62,   120,    28,   120,
     120,   170,    68,   157,    68,   240,   241,    61,    56,    56,
      56,    56,    56,    56,    56,    56,    69,   231,   117,   120,
     121,    69,    64,   117,    61,   117,    99,   178,   177,   179,
     117,   205,    59,   120,   120,   120,   120,    61,   120,    69,
      69,    69,    69,    61,    65,    99,   221,    68,    61,    61,
      74,   117,   120,   130,   252,   154,    61,   247,    63,   244,
      55,    61,   180,    73,    83,    84,    92,    97,    76,    94,
      95,    96,    68,   241,   117,   117,    65,   117,    65,    65,
      61,   117,   117,   120,   121,    69,    69,    69,   117,    56,
     117,    66,   120,   212,    63,   120,   194,   185,   191,   187,
     197,    61,   214,   117,    61,   120,   230,   252,   252,   252,
      61,   252,   100,   101,   102,   124,   125,   126,   127,   128,
     130,   131,   132,   133,   134,   168,    63,   245,    63,   168,
     182,    69,   158,    69,   120,    56,   120,   242,   120,   121,
      69,    48,    57,   232,   233,   236,   117,    57,    56,    57,
      57,    64,   203,    65,    61,   230,   230,   230,    58,    74,
     109,   225,   226,   227,   229,   198,   227,    56,    58,   117,
     118,   120,   121,   222,   223,   224,    61,    69,    62,    62,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,    62,   248,    55,    64,   246,   245,
      62,    12,   148,   230,    59,   117,   120,    69,   117,    69,
      69,   117,    66,   117,    66,   120,   234,    66,   120,   237,
      57,   117,   117,   117,   120,    59,   212,   117,   120,    69,
      69,   120,    62,    57,   120,   225,    57,   117,   120,    59,
      57,    62,   220,    62,    55,    15,    16,    17,    64,   249,
      64,    55,   164,   155,    69,   117,   120,   121,    69,   181,
     117,    62,    59,   100,   101,   235,   235,   117,    57,    57,
      62,   120,    64,    56,    66,    62,   192,   188,    61,   230,
     227,    62,    58,   216,   229,    66,    61,   224,   223,   225,
     101,   250,    59,    59,    68,    48,    69,    69,    69,    67,
     117,   117,   120,    62,   117,   117,   206,    55,   117,    55,
     195,    68,   189,   229,   230,   120,   215,   120,    61,   224,
      62,   251,    59,    63,   252,   253,   252,   253,    48,    61,
      69,   181,    62,    62,    55,    68,    66,    68,   230,    87,
     230,    48,   122,    63,   120,   228,    61,    62,    65,   217,
     209,    62,   120,   252,   253,   254,    55,    55,   256,    55,
     256,    69,   117,    61,    68,   120,    55,   120,    69,   117,
      62,   229,    55,   117,    63,   100,    55,   256,    18,    20,
      21,    24,    25,    28,    63,    64,   120,   255,    57,   117,
     120,   120,    68,   120,    63,   120,    68,    56,    66,   212,
      61,    61,   117,   130,   252,    55,    56,   254,    59,   117,
      82,   120,   204,   120,   200,   120,   217,   230,   117,    64,
     252,   252,    56,   117,    55,    64,   252,    62,   117,   207,
     230,   120,   230,    64,    62,    69,    66,    57,    62,    62,
      56,    55,    70,    62,   230,    69,   201,    69,    64,   120,
     255,   255,    56,    69,   230,   210,    19,   117,    69,    62,
     255,    55,   211,    68,   230,    69
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
        case 2:
#line 98 "vgrammar.y"
    { VerCheckVersion((yyvsp[(3) - (4)].S)); }
    break;

  case 4:
#line 99 "vgrammar.y"
    { BeginVN(); }
    break;

  case 5:
#line 99 "vgrammar.y"
    { BeginDD(); }
    break;

  case 19:
#line 127 "vgrammar.y"
    { VerAddScript((yyvsp[(3) - (4)].S)); }
    break;

  case 20:
#line 128 "vgrammar.y"
    { VerAddVLib((yyvsp[(3) - (4)].S)); }
    break;

  case 21:
#line 129 "vgrammar.y"
    { VerCircuitProp((yyvsp[(3) - (6)].S),&(yyvsp[(5) - (6)].I),TYPE_INT); }
    break;

  case 22:
#line 130 "vgrammar.y"
    { VerCircuitProp((yyvsp[(3) - (6)].S),(yyvsp[(5) - (6)].S),TYPE_STR); }
    break;

  case 23:
#line 131 "vgrammar.y"
    { VerCircuitProp((yyvsp[(3) - (6)].S),(yyvsp[(5) - (6)].S),TYPE_STR); }
    break;

  case 26:
#line 145 "vgrammar.y"
    { nativeVerilog = 0; VerNewModule(HDLMODULE,(yyvsp[(3) - (4)].S),0); }
    break;

  case 27:
#line 147 "vgrammar.y"
    { BeginHDL(); }
    break;

  case 28:
#line 147 "vgrammar.y"
    { BeginVR(); VerEndModule(); }
    break;

  case 29:
#line 148 "vgrammar.y"
    { VerModParm(0); BeginHDL(); }
    break;

  case 30:
#line 148 "vgrammar.y"
    { BeginVR(); VerEndModule(); }
    break;

  case 31:
#line 149 "vgrammar.y"
    { BeginVR(); }
    break;

  case 32:
#line 150 "vgrammar.y"
    { BeginVR(); }
    break;

  case 33:
#line 153 "vgrammar.y"
    { nativeVerilog = 1; VerNewModule(HDLMODULE,(yyvsp[(2) - (3)].S),0); }
    break;

  case 35:
#line 154 "vgrammar.y"
    { nativeVerilog = 1; VerNewModule(HDLMODULE,(yyvsp[(2) - (3)].S),1); }
    break;

  case 36:
#line 155 "vgrammar.y"
    { nativeVerilog = 1; VerNewModule(HDLMODULE,(yyvsp[(2) - (3)].S),0); }
    break;

  case 40:
#line 162 "vgrammar.y"
    { VerModHashParm((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].E)); }
    break;

  case 41:
#line 163 "vgrammar.y"
    { VerModHashParm((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E)); }
    break;

  case 43:
#line 168 "vgrammar.y"
    { VerAddHdlLine((yyvsp[(2) - (2)].S)); }
    break;

  case 46:
#line 175 "vgrammar.y"
    { VerBeginBD(); }
    break;

  case 47:
#line 175 "vgrammar.y"
    { VerEndBD(); }
    break;

  case 48:
#line 176 "vgrammar.y"
    { VerModuleProp("symbol",&(yyvsp[(4) - (5)].I),TYPE_INT); }
    break;

  case 49:
#line 177 "vgrammar.y"
    { VerSetRootMod(); }
    break;

  case 50:
#line 178 "vgrammar.y"
    { VerModuleProp((yyvsp[(3) - (6)].S),&(yyvsp[(5) - (6)].I),TYPE_INT); }
    break;

  case 51:
#line 179 "vgrammar.y"
    { VerModuleProp((yyvsp[(3) - (6)].S),(yyvsp[(5) - (6)].S),TYPE_STR); }
    break;

  case 52:
#line 180 "vgrammar.y"
    { VerModuleProp((yyvsp[(3) - (6)].S),(yyvsp[(5) - (6)].S),TYPE_STR); }
    break;

  case 53:
#line 187 "vgrammar.y"
    { nativeVerilog = 0; BeginHDL(); }
    break;

  case 54:
#line 187 "vgrammar.y"
    { BeginVR(); }
    break;

  case 57:
#line 197 "vgrammar.y"
    { VerNewModule(NETMODULE,(yyvsp[(3) - (4)].S),0); }
    break;

  case 58:
#line 198 "vgrammar.y"
    { VerEndModule(); }
    break;

  case 60:
#line 200 "vgrammar.y"
    { VerEndModule(); }
    break;

  case 61:
#line 203 "vgrammar.y"
    { VerNewModule(NETMODULE,(yyvsp[(2) - (3)].S),0); }
    break;

  case 63:
#line 204 "vgrammar.y"
    { VerNewModule(NETMODULE,(yyvsp[(2) - (3)].S),1); }
    break;

  case 68:
#line 214 "vgrammar.y"
    { VerModParm((yyvsp[(1) - (1)].S)); }
    break;

  case 69:
#line 215 "vgrammar.y"
    { VerModParm((yyvsp[(3) - (3)].S)); }
    break;

  case 72:
#line 223 "vgrammar.y"
    { VerNewNet((yyvsp[(3) - (4)].S),(yyvsp[(2) - (4)].I),(yyvsp[(1) - (4)].I)); }
    break;

  case 74:
#line 225 "vgrammar.y"
    { VerBeginBD(); }
    break;

  case 75:
#line 225 "vgrammar.y"
    { VerEndBD(); }
    break;

  case 76:
#line 226 "vgrammar.y"
    { VerModuleProp("symbol",&(yyvsp[(4) - (5)].I),TYPE_INT); }
    break;

  case 77:
#line 227 "vgrammar.y"
    { VerSetRootMod(); }
    break;

  case 78:
#line 228 "vgrammar.y"
    { VerModuleProp((yyvsp[(3) - (6)].S),&(yyvsp[(5) - (6)].I),TYPE_INT); }
    break;

  case 79:
#line 229 "vgrammar.y"
    { VerModuleProp((yyvsp[(3) - (6)].S),(yyvsp[(5) - (6)].S),TYPE_STR); }
    break;

  case 80:
#line 230 "vgrammar.y"
    { VerModuleProp((yyvsp[(3) - (6)].S),(yyvsp[(5) - (6)].S),TYPE_STR); }
    break;

  case 81:
#line 233 "vgrammar.y"
    { (yyval.I) = INPUT; }
    break;

  case 82:
#line 234 "vgrammar.y"
    { (yyval.I) = OUTPUT; }
    break;

  case 83:
#line 235 "vgrammar.y"
    { (yyval.I) = INOUT; }
    break;

  case 84:
#line 236 "vgrammar.y"
    { (yyval.I) = SUPPLY0; }
    break;

  case 85:
#line 237 "vgrammar.y"
    { (yyval.I) = SUPPLY1; }
    break;

  case 86:
#line 238 "vgrammar.y"
    { (yyval.I) = REG; }
    break;

  case 87:
#line 239 "vgrammar.y"
    { (yyval.I) = WIRE; }
    break;

  case 88:
#line 240 "vgrammar.y"
    { (yyval.I) = WAND; }
    break;

  case 89:
#line 241 "vgrammar.y"
    { (yyval.I) = WOR; }
    break;

  case 90:
#line 242 "vgrammar.y"
    { (yyval.I) = KWTRI; }
    break;

  case 91:
#line 243 "vgrammar.y"
    { (yyval.I) = TRI0; }
    break;

  case 92:
#line 244 "vgrammar.y"
    { (yyval.I) = TRI1; }
    break;

  case 93:
#line 245 "vgrammar.y"
    { (yyval.I) = TRIAND; }
    break;

  case 94:
#line 246 "vgrammar.y"
    { (yyval.I) = TRIOR; }
    break;

  case 95:
#line 247 "vgrammar.y"
    { (yyval.I) = TRIREG; }
    break;

  case 99:
#line 255 "vgrammar.y"
    { VerSetShowName((yyvsp[(3) - (3)].I)); }
    break;

  case 100:
#line 256 "vgrammar.y"
    { VerSetWireDecorationPos((yyvsp[(3) - (3)].I),-1,-1); }
    break;

  case 101:
#line 257 "vgrammar.y"
    { VerSetWireDecorationPos((yyvsp[(4) - (9)].I),(yyvsp[(6) - (9)].I),(yyvsp[(8) - (9)].I)); }
    break;

  case 104:
#line 271 "vgrammar.y"
    { VerMakeNode((yyvsp[(3) - (6)].I),(yyvsp[(5) - (6)].I),(yyvsp[(2) - (6)].I),0,0); }
    break;

  case 105:
#line 272 "vgrammar.y"
    { VerMakeNode((yyvsp[(5) - (8)].I),(yyvsp[(7) - (8)].I),(yyvsp[(2) - (8)].I),1,(yyvsp[(3) - (8)].I)); }
    break;

  case 106:
#line 275 "vgrammar.y"
    { (yyval.I) = 1; }
    break;

  case 107:
#line 276 "vgrammar.y"
    { (yyval.I) = 0; }
    break;

  case 108:
#line 279 "vgrammar.y"
    { VerMakeWire((yyvsp[(2) - (3)].I)); }
    break;

  case 109:
#line 282 "vgrammar.y"
    { VerEndDecls(); }
    break;

  case 110:
#line 285 "vgrammar.y"
    { (yyval.I) = 1; }
    break;

  case 111:
#line 286 "vgrammar.y"
    { (yyval.I) = ((yyvsp[(2) - (5)].I)-(yyvsp[(4) - (5)].I))+1; }
    break;

  case 114:
#line 293 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 115:
#line 294 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 116:
#line 295 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 117:
#line 296 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 118:
#line 297 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 119:
#line 298 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 120:
#line 299 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 121:
#line 300 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 122:
#line 301 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 123:
#line 302 "vgrammar.y"
    { VerEndGate(); }
    break;

  case 124:
#line 303 "vgrammar.y"
    { yyerrok; }
    break;

  case 125:
#line 306 "vgrammar.y"
    { VerGate("COMMENT",(yyvsp[(2) - (2)].S)); }
    break;

  case 127:
#line 309 "vgrammar.y"
    { VerGate("SCRIPT",(yyvsp[(2) - (2)].S)); }
    break;

  case 128:
#line 309 "vgrammar.y"
    {BeginHDL(); }
    break;

  case 129:
#line 309 "vgrammar.y"
    { BeginVR(); }
    break;

  case 131:
#line 313 "vgrammar.y"
    { VerAddScriptLine((yyvsp[(2) - (2)].S)); }
    break;

  case 132:
#line 317 "vgrammar.y"
    { VerGate("FRAME",(yyvsp[(2) - (2)].S)); }
    break;

  case 136:
#line 324 "vgrammar.y"
    { VerGate("JOINT",(yyvsp[(2) - (2)].S)); }
    break;

  case 137:
#line 324 "vgrammar.y"
    { VerJointNet((yyvsp[(5) - (6)].S)); }
    break;

  case 139:
#line 327 "vgrammar.y"
    { VerGate((yyvsp[(1) - (2)].S),0); }
    break;

  case 141:
#line 328 "vgrammar.y"
    { VerGate((yyvsp[(1) - (3)].S),(yyvsp[(2) - (3)].S)); }
    break;

  case 143:
#line 332 "vgrammar.y"
    { VerGate("TAP",(yyvsp[(11) - (11)].S)); VerTranRange((yyvsp[(6) - (11)].I),(yyvsp[(6) - (11)].I)); VerAttach("Z",(yyvsp[(2) - (11)].S),0); VerAttach("I",(yyvsp[(4) - (11)].S),0); VerTranDup(); }
    break;

  case 145:
#line 335 "vgrammar.y"
    { VerGate("TAP",(yyvsp[(13) - (13)].S)); VerTranRange((yyvsp[(6) - (13)].I),(yyvsp[(8) - (13)].I)); VerAttach("Z",(yyvsp[(2) - (13)].S),0); VerAttach("I",(yyvsp[(4) - (13)].S),0); VerTranDup(); }
    break;

  case 147:
#line 339 "vgrammar.y"
    { VerGate("CONCAT",0); VerAttach("Z",(yyvsp[(2) - (4)].S),0); }
    break;

  case 148:
#line 340 "vgrammar.y"
    { VerRenameCurrent((yyvsp[(11) - (11)].S)); VerRevPad(0); }
    break;

  case 150:
#line 341 "vgrammar.y"
    { VerGate("CONCAT",0); }
    break;

  case 151:
#line 341 "vgrammar.y"
    { VerAttach("Z",(yyvsp[(7) - (7)].S),0); }
    break;

  case 152:
#line 342 "vgrammar.y"
    { VerRenameCurrent((yyvsp[(12) - (12)].S)); VerRevPad(0); }
    break;

  case 154:
#line 346 "vgrammar.y"
    { VerGate("CONCAT",(yyvsp[(2) - (8)].S)); }
    break;

  case 155:
#line 349 "vgrammar.y"
    { VerAttach("Z",(yyvsp[(14) - (14)].S),0); }
    break;

  case 156:
#line 350 "vgrammar.y"
    { VerRevPad(0); }
    break;

  case 158:
#line 354 "vgrammar.y"
    { VerAttach("I",(yyvsp[(1) - (1)].S),0); }
    break;

  case 159:
#line 355 "vgrammar.y"
    { VerAttach("I",(yyvsp[(3) - (3)].S),0); }
    break;

  case 160:
#line 361 "vgrammar.y"
    { VerGate("TAP",(yyvsp[(2) - (3)].S)); }
    break;

  case 161:
#line 362 "vgrammar.y"
    { VerTranDup(); }
    break;

  case 163:
#line 365 "vgrammar.y"
    { VerAttach((yyvsp[(2) - (7)].S),(yyvsp[(5) - (7)].S),(yyvsp[(4) - (7)].I)); }
    break;

  case 164:
#line 366 "vgrammar.y"
    { VerAttach(0,(yyvsp[(2) - (3)].S),(yyvsp[(1) - (3)].I)); }
    break;

  case 165:
#line 369 "vgrammar.y"
    { VerTranRange((yyvsp[(2) - (3)].I),(yyvsp[(2) - (3)].I)); }
    break;

  case 166:
#line 370 "vgrammar.y"
    { VerTranRange((yyvsp[(2) - (5)].I),(yyvsp[(4) - (5)].I)); }
    break;

  case 167:
#line 373 "vgrammar.y"
    { VerCallMParmFlush(); }
    break;

  case 168:
#line 373 "vgrammar.y"
    { VerCell((yyvsp[(1) - (5)].S),(yyvsp[(4) - (5)].S)); }
    break;

  case 174:
#line 384 "vgrammar.y"
    { VerCallMParmAdd(0,(yyvsp[(1) - (1)].S)); }
    break;

  case 175:
#line 385 "vgrammar.y"
    { VerCallMParmAdd((yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].S)); }
    break;

  case 176:
#line 386 "vgrammar.y"
    { VerCallMParmAdd((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].S)); }
    break;

  case 177:
#line 389 "vgrammar.y"
    { (yyval.S) = yc_sprintf("%u",(yyvsp[(1) - (1)].I)); }
    break;

  case 178:
#line 390 "vgrammar.y"
    { (yyval.S) = (yyvsp[(1) - (1)].S); }
    break;

  case 179:
#line 391 "vgrammar.y"
    { char buf[1024]; (yyval.S) = yc_sprintf("\"%s\"",quoteChars(buf,(yyvsp[(1) - (1)].S),"\"\\")); }
    break;

  case 184:
#line 402 "vgrammar.y"
    { VerAttach((yyvsp[(2) - (6)].S),(yyvsp[(5) - (6)].S),(yyvsp[(4) - (6)].I)); }
    break;

  case 185:
#line 403 "vgrammar.y"
    { VerAttach(0,(yyvsp[(2) - (2)].S),(yyvsp[(1) - (2)].I)); }
    break;

  case 186:
#line 410 "vgrammar.y"
    { (yyval.S) = (yyvsp[(1) - (1)].S); }
    break;

  case 187:
#line 411 "vgrammar.y"
    { (yyval.S) = (yyvsp[(4) - (6)].S); }
    break;

  case 188:
#line 414 "vgrammar.y"
    { (yyval.I) = 0; }
    break;

  case 189:
#line 415 "vgrammar.y"
    { (yyval.I) = 1; }
    break;

  case 190:
#line 416 "vgrammar.y"
    { (yyval.I) = 1; }
    break;

  case 193:
#line 425 "vgrammar.y"
    { VerSetPos((yyvsp[(3) - (6)].I),(yyvsp[(5) - (6)].I)); }
    break;

  case 194:
#line 426 "vgrammar.y"
    { VerSetSize((yyvsp[(4) - (7)].I),(yyvsp[(6) - (7)].I)); }
    break;

  case 195:
#line 427 "vgrammar.y"
    { VerSetRot((yyvsp[(3) - (3)].I)); }
    break;

  case 199:
#line 431 "vgrammar.y"
    { VerSetShowName((yyvsp[(3) - (3)].I)); }
    break;

  case 200:
#line 432 "vgrammar.y"
    { VerSetBlockSymbol((yyvsp[(3) - (3)].I)); }
    break;

  case 201:
#line 433 "vgrammar.y"
    { VerSetProperty((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].I)); }
    break;

  case 202:
#line 434 "vgrammar.y"
    { VerSetStrProperty((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].S)); }
    break;

  case 203:
#line 435 "vgrammar.y"
    { VerSetStrProperty((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].S)); }
    break;

  case 205:
#line 439 "vgrammar.y"
    { VerPlaceWire((yyvsp[(2) - (2)].I)); }
    break;

  case 208:
#line 446 "vgrammar.y"
    { VerBlockPort((yyvsp[(1) - (3)].S),(yyvsp[(2) - (3)].I),(yyvsp[(3) - (3)].I)); }
    break;

  case 209:
#line 449 "vgrammar.y"
    { (yyval.I) = 2; }
    break;

  case 210:
#line 450 "vgrammar.y"
    { (yyval.I) = 3; }
    break;

  case 211:
#line 451 "vgrammar.y"
    { (yyval.I) = 1; }
    break;

  case 214:
#line 459 "vgrammar.y"
    { VerBDPort((yyvsp[(1) - (9)].S),(yyvsp[(2) - (9)].I),(yyvsp[(3) - (9)].S),(yyvsp[(4) - (9)].I),(yyvsp[(6) - (9)].I),(yyvsp[(8) - (9)].I)); }
    break;

  case 215:
#line 465 "vgrammar.y"
    { VerSymbol((yyvsp[(4) - (5)].I)); }
    break;

  case 216:
#line 467 "vgrammar.y"
    { VerSymbol(0); }
    break;

  case 219:
#line 474 "vgrammar.y"
    { VerSetIcon((yyvsp[(3) - (7)].S),(yyvsp[(4) - (7)].I),(yyvsp[(5) - (7)].I),(yyvsp[(6) - (7)].I)); }
    break;

  case 220:
#line 475 "vgrammar.y"
    { VerIconData((yyvsp[(3) - (4)].S)); }
    break;

  case 221:
#line 476 "vgrammar.y"
    { VerSetIcon(0,0,0,0); }
    break;

  case 223:
#line 482 "vgrammar.y"
    { VerSymPort((yyvsp[(1) - (12)].S),(yyvsp[(2) - (12)].S),(yyvsp[(3) - (12)].I),(yyvsp[(6) - (12)].I),(yyvsp[(8) - (12)].I),(yyvsp[(12) - (12)].I)); }
    break;

  case 224:
#line 489 "vgrammar.y"
    { tech_name = (yyvsp[(2) - (2)].S); }
    break;

  case 225:
#line 489 "vgrammar.y"
    { tech_name = "default"; }
    break;

  case 230:
#line 498 "vgrammar.y"
    { VerBeginDelayDef(tech_name,(yyvsp[(2) - (2)].S)); }
    break;

  case 231:
#line 498 "vgrammar.y"
    { VerEndDelayDef(); }
    break;

  case 234:
#line 505 "vgrammar.y"
    { PrimSet("area",0,(yyvsp[(3) - (4)].E)); }
    break;

  case 235:
#line 506 "vgrammar.y"
    { PrimSet("area",0,(yyvsp[(3) - (4)].E)); }
    break;

  case 236:
#line 507 "vgrammar.y"
    { PrimSet("power",0,(yyvsp[(3) - (4)].E)); }
    break;

  case 237:
#line 508 "vgrammar.y"
    { PrimSet("power",0,(yyvsp[(3) - (4)].E)); }
    break;

  case 238:
#line 509 "vgrammar.y"
    { PrimSet("delay",(yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E)); }
    break;

  case 239:
#line 510 "vgrammar.y"
    { PrimSet("delay",(yyvsp[(2) - (5)].S),(yyvsp[(4) - (5)].E)); }
    break;

  case 240:
#line 516 "vgrammar.y"
    { BeginDDP(); }
    break;

  case 241:
#line 516 "vgrammar.y"
    {  BeginDD(); (yyval.S) = (yyvsp[(3) - (4)].S); }
    break;

  case 242:
#line 519 "vgrammar.y"
    { (yyval.E) = Expr_op(ADD,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 243:
#line 520 "vgrammar.y"
    { (yyval.E) = Expr_op(SUB,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 244:
#line 521 "vgrammar.y"
    { (yyval.E) = Expr_op(MUL,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 245:
#line 522 "vgrammar.y"
    { (yyval.E) = Expr_op(DIV,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 246:
#line 523 "vgrammar.y"
    { (yyval.E) = Expr_op(AND,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 247:
#line 524 "vgrammar.y"
    { (yyval.E) = Expr_op(POW,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 248:
#line 525 "vgrammar.y"
    { (yyval.E) = Expr_op(OR,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 249:
#line 526 "vgrammar.y"
    { (yyval.E) = Expr_op(GT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 250:
#line 527 "vgrammar.y"
    { (yyval.E) = Expr_op(LT,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 251:
#line 528 "vgrammar.y"
    { (yyval.E) = Expr_op(GE,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 252:
#line 529 "vgrammar.y"
    { (yyval.E) = Expr_op(LE,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 253:
#line 530 "vgrammar.y"
    { (yyval.E) = Expr_op(EQ,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 254:
#line 531 "vgrammar.y"
    { (yyval.E) = Expr_op(NE,(yyvsp[(1) - (3)].E),(yyvsp[(3) - (3)].E)); }
    break;

  case 255:
#line 532 "vgrammar.y"
    { (yyval.E) = Expr_op(NOT,0,(yyvsp[(2) - (2)].E)); }
    break;

  case 256:
#line 533 "vgrammar.y"
    { (yyval.E) = Expr_op(UNEG,0,(yyvsp[(2) - (2)].E)); }
    break;

  case 257:
#line 534 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 258:
#line 535 "vgrammar.y"
    { (yyval.E) = Expr_func((yyvsp[(1) - (4)].S),(yyvsp[(3) - (4)].E),0); }
    break;

  case 259:
#line 536 "vgrammar.y"
    { (yyval.E) = Expr_lit((yyvsp[(1) - (1)].S)); }
    break;

  case 260:
#line 537 "vgrammar.y"
    { (yyval.E) = Expr_num((yyvsp[(1) - (1)].I)); }
    break;

  case 261:
#line 540 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 262:
#line 543 "vgrammar.y"
    { (yyval.E) = 0; }
    break;

  case 263:
#line 544 "vgrammar.y"
    { (yyval.E) = Expr_op(NEXT,(yyvsp[(1) - (2)].E),(yyvsp[(2) - (2)].E)); }
    break;

  case 264:
#line 547 "vgrammar.y"
    { (yyval.E) = Expr_op(ASGN,Expr_lit((yyvsp[(1) - (4)].S)),(yyvsp[(3) - (4)].E)); }
    break;

  case 265:
#line 548 "vgrammar.y"
    { (yyval.E) = Expr_op3(IF,(yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].E),0); }
    break;

  case 266:
#line 549 "vgrammar.y"
    { (yyval.E) = Expr_op3(IF,(yyvsp[(3) - (7)].E),(yyvsp[(5) - (7)].E),(yyvsp[(7) - (7)].E)); }
    break;

  case 267:
#line 550 "vgrammar.y"
    { (yyval.E) = (yyvsp[(2) - (3)].E); }
    break;

  case 268:
#line 551 "vgrammar.y"
    { (yyval.E) = Expr_op(BREAK,0,0); }
    break;

  case 269:
#line 552 "vgrammar.y"
    { (yyval.E) = Expr_case(CASE,(yyvsp[(2) - (3)].I)); }
    break;

  case 270:
#line 553 "vgrammar.y"
    { (yyval.E) = Expr_case(CASE,-(yyvsp[(3) - (4)].I)); }
    break;

  case 271:
#line 554 "vgrammar.y"
    { (yyval.E) = Expr_case(DEFAULT,0); }
    break;

  case 272:
#line 555 "vgrammar.y"
    { (yyval.E) = Expr_op(RETURN,(yyvsp[(2) - (3)].E),0); }
    break;

  case 273:
#line 556 "vgrammar.y"
    { (yyval.E) = Expr_op(SWITCH,(yyvsp[(3) - (5)].E),(yyvsp[(5) - (5)].E)); }
    break;


/* Line 1267 of yacc.c.  */
#line 3216 "vgrammar.c"
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


#line 563 "vgrammar.y"


