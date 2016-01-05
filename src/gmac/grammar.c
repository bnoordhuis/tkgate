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
     MICROCODE = 258,
     MACROCODE = 259,
     MAP = 260,
     BANK = 261,
     FIELD = 262,
     REGISTERS = 263,
     OPERANDS = 264,
     OP = 265,
     KWBEGIN = 266,
     KWEND = 267,
     NL = 268,
     SEMI = 269,
     COLON = 270,
     COMMA = 271,
     ASGN = 272,
     HASH = 273,
     TILDE = 274,
     PERCENT = 275,
     PLUS = 276,
     AT = 277,
     BOGOCHAR = 278,
     MINUS = 279,
     LBRACE = 280,
     RBRACE = 281,
     LPAREN = 282,
     RPAREN = 283,
     LBRACK = 284,
     RBRACK = 285,
     DSHORT = 286,
     DLONG = 287,
     DBYTE = 288,
     DSYMBOL = 289,
     DBSS = 290,
     DPROC = 291,
     DEND = 292,
     NUMBER = 293,
     LITERAL = 294,
     REGISTER = 295,
     STRING = 296
   };
#endif
/* Tokens.  */
#define MICROCODE 258
#define MACROCODE 259
#define MAP 260
#define BANK 261
#define FIELD 262
#define REGISTERS 263
#define OPERANDS 264
#define OP 265
#define KWBEGIN 266
#define KWEND 267
#define NL 268
#define SEMI 269
#define COLON 270
#define COMMA 271
#define ASGN 272
#define HASH 273
#define TILDE 274
#define PERCENT 275
#define PLUS 276
#define AT 277
#define BOGOCHAR 278
#define MINUS 279
#define LBRACE 280
#define RBRACE 281
#define LPAREN 282
#define RPAREN 283
#define LBRACK 284
#define RBRACK 285
#define DSHORT 286
#define DLONG 287
#define DBYTE 288
#define DSYMBOL 289
#define DBSS 290
#define DPROC 291
#define DEND 292
#define NUMBER 293
#define LITERAL 294
#define REGISTER 295
#define STRING 296




/* Copy the first part of user declarations.  */
#line 39 "grammar.y"

#include "parser.h"
#include "grammar.h"


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
#line 193 "grammar.c"

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
#define YYFINAL  33
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   229

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  124
/* YYNRULES -- Number of states.  */
#define YYNSTATES  232

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    25,    28,    34,    41,    45,    47,    51,    55,
      58,    59,    65,    69,    71,    75,    79,    83,    85,    89,
      93,    94,    95,   104,   105,   108,   109,   117,   118,   126,
     128,   132,   135,   138,   143,   150,   155,   156,   160,   167,
     176,   184,   192,   195,   197,   198,   199,   208,   209,   212,
     218,   224,   227,   231,   232,   233,   241,   242,   250,   251,
     254,   257,   258,   263,   266,   268,   271,   273,   277,   278,
     287,   288,   292,   295,   297,   298,   302,   306,   309,   311,
     312,   316,   318,   319,   323,   326,   327,   329,   331,   333,
     335,   339,   341,   343,   345,   347,   351,   353,   355,   359,
     364,   368,   371,   373,   376,   378,   384,   388,   389,   391,
     393,   395,   397,   400,   402
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    44,    -1,    43,    44,    -1,    45,    -1,
      46,    -1,    53,    -1,    56,    -1,    68,    -1,    75,    -1,
      82,    -1,     1,    14,    -1,     1,    12,    -1,    98,     6,
      96,    39,    14,    -1,    98,     6,    96,    39,    96,    14,
      -1,     7,    47,    14,    -1,    48,    -1,    47,    16,    48,
      -1,    19,    39,    96,    -1,    39,    96,    -1,    -1,    39,
      96,    17,    49,    50,    -1,    25,    51,    26,    -1,    52,
      -1,    51,    16,    52,    -1,    39,    17,    38,    -1,     8,
      54,    14,    -1,    55,    -1,    54,    16,    55,    -1,    39,
      17,    38,    -1,    -1,    -1,     9,    39,    25,    57,    59,
      58,    26,    14,    -1,    -1,    59,    60,    -1,    -1,    61,
      63,    17,    25,    65,    26,    14,    -1,    -1,    62,    24,
      17,    25,    65,    26,    14,    -1,    64,    -1,    63,    16,
      64,    -1,    20,    38,    -1,    18,    38,    -1,    27,    20,
      38,    28,    -1,    18,    38,    27,    20,    38,    28,    -1,
      27,    18,    38,    28,    -1,    -1,    65,    66,    14,    -1,
      21,    38,    97,    17,    38,    97,    -1,    21,    38,    97,
      17,    18,    38,    67,    97,    -1,    21,    38,    97,    17,
      18,    38,    97,    -1,    21,    38,    97,    17,    20,    38,
      97,    -1,    22,    38,    -1,    22,    -1,    -1,    -1,    10,
      39,    25,    69,    71,    70,    26,    14,    -1,    -1,    71,
      72,    -1,     5,    95,    15,    95,    14,    -1,     5,    95,
      15,    66,    14,    -1,    66,    14,    -1,     9,    39,    14,
      -1,    -1,    -1,     9,    25,    73,    59,    74,    26,    14,
      -1,    -1,    11,     3,    22,    38,    76,    77,    12,    -1,
      -1,    77,    78,    -1,    80,    14,    -1,    -1,    99,    79,
      80,    14,    -1,     1,    14,    -1,    81,    -1,    80,    81,
      -1,    95,    -1,    95,    17,    95,    -1,    -1,    11,     4,
      22,    38,    83,    13,    84,    12,    -1,    -1,    84,    85,
      13,    -1,     1,    13,    -1,    87,    -1,    -1,    99,    86,
      87,    -1,    99,    34,    38,    -1,    36,    39,    -1,    37,
      -1,    -1,    39,    88,    93,    -1,    39,    -1,    -1,    90,
      89,    91,    -1,    35,    38,    -1,    -1,    33,    -1,    31,
      -1,    32,    -1,    92,    -1,    91,    16,    92,    -1,    38,
      -1,    39,    -1,    41,    -1,    94,    -1,    93,    16,    94,
      -1,    95,    -1,   100,    -1,    27,   100,    28,    -1,    95,
      27,   100,    28,    -1,    27,    95,    28,    -1,    18,    38,
      -1,    38,    -1,    18,    39,    -1,    39,    -1,    29,    38,
      15,    38,    30,    -1,    29,    38,    30,    -1,    -1,    96,
      -1,     3,    -1,     4,    -1,     5,    -1,    39,    15,    -1,
      40,    -1,    20,    40,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    46,    46,    47,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    64,    65,    71,    74,    75,    78,    79,
      80,    80,    83,    86,    87,    90,    96,    99,   100,   103,
     108,   109,   108,   112,   113,   116,   116,   117,   117,   121,
     122,   125,   126,   127,   128,   129,   132,   133,   136,   137,
     138,   139,   142,   143,   149,   149,   149,   152,   153,   156,
     157,   158,   159,   160,   161,   160,   167,   167,   170,   171,
     174,   175,   175,   176,   179,   180,   183,   184,   192,   192,
     196,   197,   198,   201,   202,   202,   203,   204,   205,   208,
     208,   209,   210,   210,   211,   212,   215,   216,   217,   220,
     221,   224,   225,   226,   229,   230,   233,   234,   235,   236,
     237,   240,   241,   242,   243,   249,   250,   253,   254,   257,
     258,   259,   262,   265,   266
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MICROCODE", "MACROCODE", "MAP", "BANK",
  "FIELD", "REGISTERS", "OPERANDS", "OP", "KWBEGIN", "KWEND", "NL", "SEMI",
  "COLON", "COMMA", "ASGN", "HASH", "TILDE", "PERCENT", "PLUS", "AT",
  "BOGOCHAR", "MINUS", "LBRACE", "RBRACE", "LPAREN", "RPAREN", "LBRACK",
  "RBRACK", "DSHORT", "DLONG", "DBYTE", "DSYMBOL", "DBSS", "DPROC", "DEND",
  "NUMBER", "LITERAL", "REGISTER", "STRING", "$accept", "prog", "top_item",
  "bank_decl", "field_decl", "fitems", "fitem", "@1", "enumset", "eitems",
  "eitem", "reg_decl", "regs", "reg", "operand_decl", "@2", "@3",
  "operdefs", "operdef", "@4", "@5", "oprlhss", "oprlhs", "opasgns",
  "opasgn", "offset", "op_decl", "@6", "@7", "opditems", "opditem", "@8",
  "@9", "ucode", "@10", "uops", "uop", "@11", "uspecs", "uspec", "mcode",
  "@12", "mops", "lmop", "@13", "mop", "@14", "@15", "dtype", "bsl_items",
  "bsl_item", "operands", "operand", "number", "brange", "obrange", "what",
  "label", "register", 0
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
     295,   296
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    45,    45,    46,    47,    47,    48,    48,
      49,    48,    50,    51,    51,    52,    53,    54,    54,    55,
      57,    58,    56,    59,    59,    61,    60,    62,    60,    63,
      63,    64,    64,    64,    64,    64,    65,    65,    66,    66,
      66,    66,    67,    67,    69,    70,    68,    71,    71,    72,
      72,    72,    72,    73,    74,    72,    76,    75,    77,    77,
      78,    79,    78,    78,    80,    80,    81,    81,    83,    82,
      84,    84,    84,    85,    86,    85,    85,    85,    85,    88,
      87,    87,    89,    87,    87,    87,    90,    90,    90,    91,
      91,    92,    92,    92,    93,    93,    94,    94,    94,    94,
      94,    95,    95,    95,    95,    96,    96,    97,    97,    98,
      98,    98,    99,   100,   100
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     5,     6,     3,     1,     3,     3,     2,
       0,     5,     3,     1,     3,     3,     3,     1,     3,     3,
       0,     0,     8,     0,     2,     0,     7,     0,     7,     1,
       3,     2,     2,     4,     6,     4,     0,     3,     6,     8,
       7,     7,     2,     1,     0,     0,     8,     0,     2,     5,
       5,     2,     3,     0,     0,     7,     0,     7,     0,     2,
       2,     0,     4,     2,     1,     2,     1,     3,     0,     8,
       0,     3,     2,     1,     0,     3,     3,     2,     1,     0,
       3,     1,     0,     3,     2,     0,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     3,     1,     1,     3,     4,
       3,     2,     1,     2,     1,     5,     3,     0,     1,     1,
       1,     1,     2,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   119,   120,   121,     0,     0,     0,     0,     0,
       0,     2,     4,     5,     6,     7,     8,     9,    10,     0,
      12,    11,     0,     0,     0,    16,     0,     0,    27,     0,
       0,     0,     0,     1,     3,     0,     0,     0,    19,    15,
       0,     0,    26,     0,    30,    54,     0,     0,     0,    18,
       0,    20,    17,    29,    28,    33,    57,    66,    78,     0,
       0,   116,     0,    35,    55,    68,     0,    13,     0,     0,
       0,    21,     0,    34,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,    14,   115,     0,     0,    23,     0,
       0,     0,     0,     0,    39,     0,     0,   112,   114,     0,
      63,     0,   117,    61,     0,     0,    67,   114,    69,     0,
      74,    76,    71,     0,    95,     0,     0,    22,    32,    42,
      41,     0,     0,     0,     0,     0,   111,   113,     0,    33,
      62,   118,     0,    56,    73,   122,    70,    75,     0,     0,
      82,    79,    97,    98,    96,     0,     0,    88,    89,     0,
      83,    92,    84,    25,    24,     0,     0,     0,    40,    46,
      46,     0,     0,    35,     0,    77,     0,    94,    87,     0,
      81,     0,     0,    95,     0,    45,    43,     0,     0,    60,
      59,     0,     0,     0,   117,    72,     0,     0,   123,    90,
     104,   106,   107,   101,   102,   103,    93,    99,    86,    89,
      85,     0,     0,     0,     0,     0,   117,   117,    48,   124,
       0,     0,     0,     0,     0,    44,    36,    47,    38,    65,
      53,   117,    50,    51,   110,   108,   105,     0,   100,    52,
      49,   109
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    10,    11,    12,    13,    24,    25,    62,    71,    87,
      88,    14,    27,    28,    15,    55,    72,    63,    73,    74,
      75,    93,    94,   177,   203,   221,    16,    56,    80,    64,
      81,   129,   181,    17,    65,    82,   108,   139,   109,   110,
      18,    66,   114,   149,   173,   150,   169,   171,   151,   196,
     197,   189,   190,   111,   131,   132,    19,   112,   192
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -182
static const yytype_int16 yypact[] =
{
     116,    43,  -182,  -182,  -182,    12,   -30,   -13,    42,    14,
     105,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,    83,
    -182,  -182,    79,    93,    74,  -182,   112,   124,  -182,   109,
     123,   120,   128,  -182,  -182,    93,    93,   118,   137,  -182,
      12,   119,  -182,   -30,  -182,  -182,   121,   122,   127,  -182,
     -11,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,    -2,
     129,  -182,   130,   117,    82,  -182,   145,  -182,   147,   133,
     131,  -182,   136,  -182,   110,   140,    40,    24,   134,   151,
     142,  -182,     4,     2,  -182,  -182,   152,    66,  -182,   157,
     135,   138,   126,    84,  -182,   158,   114,  -182,  -182,   159,
    -182,   163,    93,  -182,   164,   165,  -182,   166,  -182,     7,
    -182,   167,  -182,   169,    36,   148,   131,  -182,  -182,   153,
    -182,   149,   150,   110,   160,   168,  -182,  -182,    59,  -182,
    -182,  -182,   172,  -182,  -182,  -182,  -182,  -182,    40,    40,
    -182,  -182,  -182,  -182,  -182,   154,   144,  -182,   132,   177,
    -182,  -182,   161,  -182,  -182,   171,   170,   173,  -182,  -182,
    -182,   180,   182,   125,   -10,  -182,    22,  -182,  -182,    56,
    -182,    15,   162,   100,   174,  -182,  -182,    78,    81,  -182,
    -182,   176,   175,   178,    93,  -182,   179,    46,  -182,   181,
    -182,   183,  -182,  -182,  -182,  -182,   187,  -182,  -182,   186,
    -182,   189,   190,   191,   192,   193,    -9,    93,  -182,  -182,
     194,   195,    56,    30,    15,  -182,  -182,  -182,  -182,  -182,
     188,    93,  -182,  -182,  -182,  -182,  -182,   196,  -182,  -182,
    -182,  -182
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -182,  -182,   198,  -182,  -182,  -182,   185,  -182,  -182,  -182,
      95,  -182,  -182,   184,  -182,  -182,  -182,    80,  -182,  -182,
    -182,  -182,    91,    55,   -63,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,    89,  -107,
    -182,  -182,  -182,  -182,  -182,    45,  -182,  -182,  -182,  -182,
       6,  -182,     9,   -76,   -12,  -177,  -182,   115,  -181
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -92
static const yytype_int16 yytable[] =
{
      99,    79,   137,   113,    60,   105,   211,   208,   182,    26,
     183,    38,    67,   220,   -80,   -80,   106,    31,    32,    61,
      37,   136,    96,    48,    49,    96,    29,    37,   184,   222,
     223,    22,   227,   -80,   -80,   -80,   185,   -80,   -80,   -80,
      96,   -80,    97,   107,   230,    97,    98,    68,   141,   100,
     186,    23,   162,   193,   194,    20,   195,    21,    96,   137,
      97,    98,   165,   101,    96,   161,   186,   142,   143,   144,
     188,   145,   146,   147,    96,   148,   186,    96,    97,    98,
      78,    30,   116,   187,    97,    98,   188,    76,    39,    35,
      40,    77,   117,   191,    97,    98,   188,    97,    98,    78,
     123,   124,    78,    78,   202,    33,     1,   204,     2,     3,
       4,   210,     5,     6,     7,     8,     9,     1,    36,     2,
       3,     4,    37,     5,     6,     7,     8,     9,    90,    41,
      91,   142,   143,   144,    44,   145,   191,    92,    42,   199,
      43,   -37,    46,   -31,   121,   -91,   122,   135,    45,   -37,
      47,   -64,   126,   127,    51,    70,    50,    53,    83,    57,
      58,    84,    89,    85,    95,   103,    59,    69,   104,   115,
      86,   118,   102,   119,   128,   125,   120,   130,   133,   134,
     155,   135,   140,   168,   138,   159,   153,   156,   157,   164,
     170,   174,   167,   160,   179,   172,   180,   212,   175,   -91,
     198,   176,   205,   214,   216,   217,   218,   219,    34,   163,
     213,   154,   201,   206,   158,   178,   207,   215,   200,   209,
     228,   226,   224,   225,   231,    52,   229,    54,   166,   152
};

static const yytype_uint8 yycheck[] =
{
      76,    64,   109,     1,    15,     1,   187,   184,    18,    39,
      20,    23,    14,    22,    12,    13,    12,     3,     4,    30,
      29,    14,    18,    35,    36,    18,    39,    29,    38,   206,
     207,    19,   213,    31,    32,    33,    14,    35,    36,    37,
      18,    39,    38,    39,   221,    38,    39,    59,    12,    25,
      20,    39,   128,    38,    39,    12,    41,    14,    18,   166,
      38,    39,   138,    39,    18,   128,    20,    31,    32,    33,
      40,    35,    36,    37,    18,    39,    20,    18,    38,    39,
      21,    39,    16,    27,    38,    39,    40,     5,    14,     6,
      16,     9,    26,   169,    38,    39,    40,    38,    39,    21,
      16,    17,    21,    21,    26,     0,     1,    26,     3,     4,
       5,   187,     7,     8,     9,    10,    11,     1,    39,     3,
       4,     5,    29,     7,     8,     9,    10,    11,    18,    17,
      20,    31,    32,    33,    25,    35,   212,    27,    14,    39,
      16,    24,    22,    26,    18,    13,    20,    15,    25,    24,
      22,    26,    38,    39,    17,    25,    38,    38,    13,    38,
      38,    14,    26,    30,    24,    14,    39,    38,    26,    17,
      39,    14,    38,    38,    15,    17,    38,    14,    14,    14,
      27,    15,    13,    39,    17,    25,    38,    38,    38,    17,
      13,    20,    38,    25,    14,    34,    14,    16,    28,    13,
      38,    28,    26,    16,    14,    14,    14,    14,    10,   129,
      27,   116,    38,    38,   123,   160,    38,    28,   173,    40,
     214,   212,    28,    28,    28,    40,    38,    43,   139,   114
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     7,     8,     9,    10,    11,
      43,    44,    45,    46,    53,    56,    68,    75,    82,    98,
      12,    14,    19,    39,    47,    48,    39,    54,    55,    39,
      39,     3,     4,     0,    44,     6,    39,    29,    96,    14,
      16,    17,    14,    16,    25,    25,    22,    22,    96,    96,
      38,    17,    48,    38,    55,    57,    69,    38,    38,    39,
      15,    30,    49,    59,    71,    76,    83,    14,    96,    38,
      25,    50,    58,    60,    61,    62,     5,     9,    21,    66,
      70,    72,    77,    13,    14,    30,    39,    51,    52,    26,
      18,    20,    27,    63,    64,    24,    18,    38,    39,    95,
      25,    39,    38,    14,    26,     1,    12,    39,    78,    80,
      81,    95,    99,     1,    84,    17,    16,    26,    14,    38,
      38,    18,    20,    16,    17,    17,    38,    39,    15,    73,
      14,    96,    97,    14,    14,    15,    14,    81,    17,    79,
      13,    12,    31,    32,    33,    35,    36,    37,    39,    85,
      87,    90,    99,    38,    52,    27,    38,    38,    64,    25,
      25,    66,    95,    59,    17,    95,    80,    38,    39,    88,
      13,    89,    34,    86,    20,    28,    28,    65,    65,    14,
      14,    74,    18,    20,    38,    14,    20,    27,    40,    93,
      94,    95,   100,    38,    39,    41,    91,    92,    38,    39,
      87,    38,    26,    66,    26,    26,    38,    38,    97,    40,
      95,   100,    16,    27,    16,    28,    14,    14,    14,    14,
      22,    67,    97,    97,    28,    28,    94,   100,    92,    38,
      97,    28
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
        case 11:
#line 57 "grammar.y"
    { yyerrok; }
    break;

  case 12:
#line 58 "grammar.y"
    { yyerrok; }
    break;

  case 13:
#line 64 "grammar.y"
    { ycBank((yyvsp[(1) - (5)].I),&(yyvsp[(3) - (5)].R),(yyvsp[(4) - (5)].S),0); }
    break;

  case 14:
#line 65 "grammar.y"
    { ycBank((yyvsp[(1) - (6)].I),&(yyvsp[(3) - (6)].R),(yyvsp[(4) - (6)].S),&(yyvsp[(5) - (6)].R)); }
    break;

  case 18:
#line 78 "grammar.y"
    { ycField((yyvsp[(2) - (3)].S),&(yyvsp[(3) - (3)].R),1); }
    break;

  case 19:
#line 79 "grammar.y"
    { ycField((yyvsp[(1) - (2)].S),&(yyvsp[(2) - (2)].R),0); }
    break;

  case 20:
#line 80 "grammar.y"
    { ycField((yyvsp[(1) - (3)].S),&(yyvsp[(2) - (3)].R),0); }
    break;

  case 25:
#line 90 "grammar.y"
    { ycEnum((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].I)); }
    break;

  case 29:
#line 103 "grammar.y"
    { ycRegRecord((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].I)); }
    break;

  case 30:
#line 108 "grammar.y"
    { ycBeginOprGroup((yyvsp[(2) - (3)].S)); }
    break;

  case 31:
#line 109 "grammar.y"
    { ycEndOprGroup(); }
    break;

  case 35:
#line 116 "grammar.y"
    {ycBeginOperand();}
    break;

  case 36:
#line 116 "grammar.y"
    {ycEndOperand();}
    break;

  case 37:
#line 117 "grammar.y"
    {ycBeginOperand();}
    break;

  case 38:
#line 117 "grammar.y"
    {ycEndOperand();}
    break;

  case 41:
#line 125 "grammar.y"
    { ycOLHS(AM_DIRECT,(yyvsp[(2) - (2)].I),0); }
    break;

  case 42:
#line 126 "grammar.y"
    { ycOLHS(AM_IMMEDIATE,0,(yyvsp[(2) - (2)].I)); }
    break;

  case 43:
#line 127 "grammar.y"
    { ycOLHS(AM_INDIRECT,(yyvsp[(3) - (4)].I),0); }
    break;

  case 44:
#line 128 "grammar.y"
    { ycOLHS(AM_INDEXED,(yyvsp[(5) - (6)].I),(yyvsp[(2) - (6)].I)); }
    break;

  case 45:
#line 129 "grammar.y"
    { ycOLHS(AM_IMMIND,0,(yyvsp[(3) - (4)].I)); }
    break;

  case 48:
#line 136 "grammar.y"
    { ycORHS(OI_DATA,(yyvsp[(2) - (6)].I),&(yyvsp[(3) - (6)].R),(yyvsp[(5) - (6)].I),&(yyvsp[(6) - (6)].R),0); (yyval.I) = (yyvsp[(5) - (6)].I); }
    break;

  case 49:
#line 137 "grammar.y"
    { ycORHS(OI_RELNUMOP,(yyvsp[(2) - (8)].I),&(yyvsp[(3) - (8)].R),(yyvsp[(6) - (8)].I),&(yyvsp[(8) - (8)].R),(yyvsp[(7) - (8)].I)); (yyval.I) = 0; }
    break;

  case 50:
#line 138 "grammar.y"
    { ycORHS(OI_NUMOP,(yyvsp[(2) - (7)].I),&(yyvsp[(3) - (7)].R),(yyvsp[(6) - (7)].I),&(yyvsp[(7) - (7)].R),0); (yyval.I) = 0; }
    break;

  case 51:
#line 139 "grammar.y"
    { ycORHS(OI_REGOP,(yyvsp[(2) - (7)].I),&(yyvsp[(3) - (7)].R),(yyvsp[(6) - (7)].I),&(yyvsp[(7) - (7)].R),0); (yyval.I) = 0; }
    break;

  case 52:
#line 142 "grammar.y"
    { (yyval.I) = (yyvsp[(2) - (2)].I); }
    break;

  case 53:
#line 143 "grammar.y"
    { (yyval.I) = 0; }
    break;

  case 54:
#line 149 "grammar.y"
    { ycBeginOp((yyvsp[(2) - (3)].S)); }
    break;

  case 55:
#line 149 "grammar.y"
    { ycEndOp(); }
    break;

  case 59:
#line 156 "grammar.y"
    { ycMap(&(yyvsp[(2) - (5)].N),&(yyvsp[(4) - (5)].N)); }
    break;

  case 60:
#line 157 "grammar.y"
    { Number N; N.v.d = (yyvsp[(4) - (5)].I); N.ntype = NT_VALUE; ycMap(&(yyvsp[(2) - (5)].N),&N); }
    break;

  case 61:
#line 158 "grammar.y"
    { }
    break;

  case 62:
#line 159 "grammar.y"
    { ycAddOprGroup((yyvsp[(2) - (3)].S)); }
    break;

  case 63:
#line 160 "grammar.y"
    { ycBeginOprGroup(0); }
    break;

  case 64:
#line 161 "grammar.y"
    { ycEndOprGroup(); }
    break;

  case 66:
#line 167 "grammar.y"
    { ycBeginUCode((yyvsp[(4) - (4)].I)); }
    break;

  case 67:
#line 167 "grammar.y"
    { ycEndUCode(); }
    break;

  case 69:
#line 171 "grammar.y"
    { ycUNext(); }
    break;

  case 71:
#line 175 "grammar.y"
    { ycULabel((yyvsp[(1) - (1)].S)); }
    break;

  case 73:
#line 176 "grammar.y"
    { yyerrok; }
    break;

  case 76:
#line 183 "grammar.y"
    { ycUSpec(&(yyvsp[(1) - (1)].N),0); }
    break;

  case 77:
#line 184 "grammar.y"
    { ycUSpec(&(yyvsp[(1) - (3)].N),&(yyvsp[(3) - (3)].N)); }
    break;

  case 78:
#line 192 "grammar.y"
    { BeginMA(); ycBeginMCode((yyvsp[(4) - (4)].I)); }
    break;

  case 79:
#line 193 "grammar.y"
    { ycEndMCode(); BeginBA(); }
    break;

  case 81:
#line 197 "grammar.y"
    { ycMNext(); }
    break;

  case 82:
#line 198 "grammar.y"
    { yyerrok; }
    break;

  case 84:
#line 202 "grammar.y"
    { ycMLabel((yyvsp[(1) - (1)].S)); }
    break;

  case 86:
#line 203 "grammar.y"
    { ycMSymbol((yyvsp[(1) - (3)].S),(yyvsp[(3) - (3)].I)); }
    break;

  case 87:
#line 204 "grammar.y"
    { ycBeginProc((yyvsp[(2) - (2)].S)); }
    break;

  case 88:
#line 205 "grammar.y"
    { ycEndProc(); }
    break;

  case 89:
#line 208 "grammar.y"
    { ycMOp((yyvsp[(1) - (1)].S)); }
    break;

  case 91:
#line 209 "grammar.y"
    { ycMOp((yyvsp[(1) - (1)].S)); }
    break;

  case 92:
#line 210 "grammar.y"
    { ycData((yyvsp[(1) - (1)].I)); }
    break;

  case 94:
#line 211 "grammar.y"
    { ycBss((yyvsp[(2) - (2)].I)); }
    break;

  case 96:
#line 215 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 97:
#line 216 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 98:
#line 217 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 101:
#line 224 "grammar.y"
    { ycDataNum((yyvsp[(1) - (1)].I)); }
    break;

  case 102:
#line 225 "grammar.y"
    { ycDataLit((yyvsp[(1) - (1)].S)); }
    break;

  case 103:
#line 226 "grammar.y"
    { ycDataStr((yyvsp[(1) - (1)].S)); }
    break;

  case 106:
#line 233 "grammar.y"
    { ycMOperand(AM_IMMEDIATE,Number_copy(&(yyvsp[(1) - (1)].N)),0); }
    break;

  case 107:
#line 234 "grammar.y"
    { ycMOperand(AM_DIRECT,0,(yyvsp[(1) - (1)].I)); }
    break;

  case 108:
#line 235 "grammar.y"
    { ycMOperand(AM_INDIRECT,0,(yyvsp[(2) - (3)].I)); }
    break;

  case 109:
#line 236 "grammar.y"
    { ycMOperand(AM_INDEXED,Number_copy(&(yyvsp[(1) - (4)].N)),(yyvsp[(3) - (4)].I)); }
    break;

  case 110:
#line 237 "grammar.y"
    { ycMOperand(AM_IMMIND,Number_copy(&(yyvsp[(2) - (3)].N)),0); }
    break;

  case 111:
#line 240 "grammar.y"
    { (yyval.N).ntype = NT_VALUE; (yyval.N).v.d = (yyvsp[(2) - (2)].I); }
    break;

  case 112:
#line 241 "grammar.y"
    { (yyval.N).ntype = NT_VALUE; (yyval.N).v.d = (yyvsp[(1) - (1)].I); }
    break;

  case 113:
#line 242 "grammar.y"
    { (yyval.N).ntype = NT_SYMBOL; (yyval.N).v.s = (yyvsp[(2) - (2)].S); }
    break;

  case 114:
#line 243 "grammar.y"
    { (yyval.N).ntype = NT_SYMBOL; (yyval.N).v.s = (yyvsp[(1) - (1)].S); }
    break;

  case 115:
#line 249 "grammar.y"
    { (yyval.R).msb = (yyvsp[(2) - (5)].I); (yyval.R).lsb = (yyvsp[(4) - (5)].I); }
    break;

  case 116:
#line 250 "grammar.y"
    { (yyval.R).msb = (yyval.R).lsb = (yyvsp[(2) - (3)].I); }
    break;

  case 117:
#line 253 "grammar.y"
    { (yyval.R).msb = (yyval.R).lsb = -1; }
    break;

  case 118:
#line 254 "grammar.y"
    { (yyval.R) = (yyvsp[(1) - (1)].R); }
    break;

  case 119:
#line 257 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 120:
#line 258 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 121:
#line 259 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 122:
#line 262 "grammar.y"
    { (yyval.S) = (yyvsp[(1) - (2)].S); }
    break;

  case 123:
#line 265 "grammar.y"
    { (yyval.I) = (yyvsp[(1) - (1)].I); }
    break;

  case 124:
#line 266 "grammar.y"
    { (yyval.I) = (yyvsp[(2) - (2)].I); }
    break;


/* Line 1267 of yacc.c.  */
#line 1993 "grammar.c"
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



