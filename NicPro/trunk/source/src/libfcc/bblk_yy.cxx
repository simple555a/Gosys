/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse bblk_parse
#define yylex   bblk_lex
#define yyerror bblk_error
#define yylval  bblk_lval
#define yychar  bblk_char
#define yydebug bblk_debug
#define yynerrs bblk_nerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_NUMBER = 258,
     TOK_INTEGER = 259,
     TOK_FLOAT = 260,
     TOK_IN = 261,
     TOK_OUT = 262,
     TOK_BOOL = 263,
     TOK_BEGIN = 264,
     TOK_END = 265,
     TOK_VAR = 266,
     TOK_INTERFACE = 267,
     TOK_IMPLEMENTATION = 268,
     TOK_FBLOCK = 269,
     TOK_INSTRUCTION = 270,
     TOK_ID = 271,
     TOK_TIMER = 272,
     TOK_INVOKE = 273,
     TOK_CONST = 274,
     TOK_TRUE = 275,
     TOK_FALSE = 276,
     TOK_EVENT = 277,
     TOK_ERROR = 278,
     TOK_ARRAY = 279,
     TOK_OF = 280,
     TOK_DATE = 281,
     TOK_BYTE = 282,
     TOK_WORD = 283,
     TOK_DOUBLE = 284,
     TOK_CHAR = 285,
     TOK_SHORT = 286,
     TOK_DWORD = 287,
     TOK_HEXNUMBER = 288,
     TOK_UUID = 289,
     TOK_STRING = 290,
     TOK_COMMENTS = 291,
     TOK_ATTRIBUTE = 292,
     TOK_HIDDEN = 293
   };
#endif
/* Tokens.  */
#define TOK_NUMBER 258
#define TOK_INTEGER 259
#define TOK_FLOAT 260
#define TOK_IN 261
#define TOK_OUT 262
#define TOK_BOOL 263
#define TOK_BEGIN 264
#define TOK_END 265
#define TOK_VAR 266
#define TOK_INTERFACE 267
#define TOK_IMPLEMENTATION 268
#define TOK_FBLOCK 269
#define TOK_INSTRUCTION 270
#define TOK_ID 271
#define TOK_TIMER 272
#define TOK_INVOKE 273
#define TOK_CONST 274
#define TOK_TRUE 275
#define TOK_FALSE 276
#define TOK_EVENT 277
#define TOK_ERROR 278
#define TOK_ARRAY 279
#define TOK_OF 280
#define TOK_DATE 281
#define TOK_BYTE 282
#define TOK_WORD 283
#define TOK_DOUBLE 284
#define TOK_CHAR 285
#define TOK_SHORT 286
#define TOK_DWORD 287
#define TOK_HEXNUMBER 288
#define TOK_UUID 289
#define TOK_STRING 290
#define TOK_COMMENTS 291
#define TOK_ATTRIBUTE 292
#define TOK_HIDDEN 293




/* Copy the first part of user declarations.  */
#line 1 "bblk.yy"

#define yytext bblk_text
#define yylex bblk_lex
#define yyerror bblk_error


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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 186 "bblk_yy.cxx"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

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

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
      while (0)
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
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   73

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  23
/* YYNRULES -- Number of rules. */
#define YYNRULES  49
/* YYNRULES -- Number of states. */
#define YYNSTATES  79

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,     2,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    39,
       2,    43,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,    12,    13,    19,    21,    25,    27,    28,
      31,    33,    34,    37,    38,    41,    46,    48,    51,    52,
      55,    56,    59,    65,    67,    69,    71,    73,    75,    77,
      79,    81,    83,    85,    87,    89,    91,    98,    99,   102,
     104,   108,   110,   112,   117,   122,   124,   126,   128,   130
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      48,     0,    -1,    14,    68,    39,    49,    52,    54,    58,
      69,    -1,    -1,    37,    40,    50,    41,    39,    -1,    51,
      -1,    50,    42,    51,    -1,    68,    -1,    -1,    53,    39,
      -1,    34,    -1,    -1,    36,    55,    -1,    -1,    55,    56,
      -1,    68,    43,    57,    39,    -1,    35,    -1,    57,    35,
      -1,    -1,    11,    59,    -1,    -1,    59,    60,    -1,    68,
      44,    61,    39,    64,    -1,    62,    -1,    63,    -1,    22,
      -1,     4,    -1,     5,    -1,     8,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,
      -1,    24,    45,    67,    46,    25,    62,    -1,    -1,    65,
      39,    -1,    66,    -1,    65,    42,    66,    -1,     6,    -1,
       7,    -1,    17,    40,    67,    41,    -1,    18,    40,    68,
      41,    -1,    38,    -1,     3,    -1,    33,    -1,    16,    -1,
      13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    44,    44,    59,    62,    71,    75,    86,    93,   100,
     106,   114,   117,   127,   130,   141,   148,   153,   163,   165,
     175,   178,   189,   202,   206,   212,   216,   220,   224,   228,
     232,   236,   240,   244,   248,   252,   258,   267,   270,   276,
     280,   287,   291,   295,   300,   305,   311,   316,   323,   330
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_NUMBER", "TOK_INTEGER", "TOK_FLOAT",
  "TOK_IN", "TOK_OUT", "TOK_BOOL", "TOK_BEGIN", "TOK_END", "TOK_VAR",
  "TOK_INTERFACE", "TOK_IMPLEMENTATION", "TOK_FBLOCK", "TOK_INSTRUCTION",
  "TOK_ID", "TOK_TIMER", "TOK_INVOKE", "TOK_CONST", "TOK_TRUE",
  "TOK_FALSE", "TOK_EVENT", "TOK_ERROR", "TOK_ARRAY", "TOK_OF", "TOK_DATE",
  "TOK_BYTE", "TOK_WORD", "TOK_DOUBLE", "TOK_CHAR", "TOK_SHORT",
  "TOK_DWORD", "TOK_HEXNUMBER", "TOK_UUID", "TOK_STRING", "TOK_COMMENTS",
  "TOK_ATTRIBUTE", "TOK_HIDDEN", "';'", "'('", "')'", "','", "'='", "':'",
  "'['", "']'", "$accept", "program", "blk_attributes",
  "blk_attribute_list", "blk_attribute", "uuid", "uuid_const", "comments",
  "comment_list", "comment", "string", "interface", "var_list", "var_def",
  "type_def", "basic_type", "array_type", "attribute_part",
  "attribute_list", "attribute", "integer_number", "id", "implementation", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,    59,
      40,    41,    44,    61,    58,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    47,    48,    49,    49,    50,    50,    51,    52,    52,
      53,    54,    54,    55,    55,    56,    57,    57,    58,    58,
      59,    59,    60,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    63,    64,    64,    65,
      65,    66,    66,    66,    66,    66,    67,    67,    68,    69
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     8,     0,     5,     1,     3,     1,     0,     2,
       1,     0,     2,     0,     2,     4,     1,     2,     0,     2,
       0,     2,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     0,     2,     1,
       3,     1,     1,     4,     4,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,    48,     0,     1,     3,     0,     8,     0,
      10,    11,     0,     0,     5,     7,    13,    18,     9,     0,
       0,    12,    20,     0,     4,     6,    14,     0,    19,    49,
       2,     0,    21,     0,    16,     0,     0,    17,    15,    26,
      27,    28,    25,     0,    29,    30,    31,    32,    33,    34,
      35,     0,    23,    24,     0,    37,    46,    47,     0,    41,
      42,     0,     0,    45,    22,     0,    39,     0,     0,     0,
      38,     0,     0,     0,     0,    40,    36,    43,    44
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     8,    13,    14,    11,    12,    17,    21,    26,
      35,    23,    28,    32,    51,    52,    53,    64,    65,    66,
      58,    15,    30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -35
static const yysigned_char yypact[] =
{
      -8,    -7,    14,   -35,   -24,   -35,   -21,   -18,   -15,    -7,
     -35,   -13,   -14,   -31,   -35,   -35,   -35,    15,   -35,    -9,
      -7,    -7,   -35,    11,   -35,   -35,   -35,   -12,    -7,   -35,
     -35,     3,   -35,     4,   -35,   -32,    13,   -35,   -35,   -35,
     -35,   -35,   -35,     2,   -35,   -35,   -35,   -35,   -35,   -35,
     -35,    -3,   -35,   -35,     1,    -5,   -35,   -35,    12,   -35,
     -35,     9,    17,   -35,   -35,   -34,   -35,    34,     1,    -7,
     -35,    -5,    24,    19,    20,   -35,   -35,   -35,   -35
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -35,   -35,   -35,   -35,    42,   -35,   -35,   -35,   -35,   -35,
     -35,   -35,   -35,   -35,   -35,    -6,   -35,   -35,   -35,    -4,
       5,    -1,   -35
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
       4,    59,    60,    37,    56,    70,     1,    38,    71,     3,
      19,    20,    61,    62,     5,     6,     7,    39,    40,    10,
      27,    41,     9,    16,    29,    18,    22,    33,    39,    40,
      24,    31,    41,    63,    57,    42,    55,    43,    34,    44,
      45,    46,    47,    48,    49,    50,    42,    54,    36,    68,
      44,    45,    46,    47,    48,    49,    50,    69,    67,    72,
      77,    78,    25,     0,     0,     0,    76,    75,    74,     0,
       0,     0,     0,    73
};

static const yysigned_char yycheck[] =
{
       1,     6,     7,    35,     3,    39,    14,    39,    42,    16,
      41,    42,    17,    18,     0,    39,    37,     4,     5,    34,
      21,     8,    40,    36,    13,    39,    11,    28,     4,     5,
      39,    43,     8,    38,    33,    22,    39,    24,    35,    26,
      27,    28,    29,    30,    31,    32,    22,    45,    44,    40,
      26,    27,    28,    29,    30,    31,    32,    40,    46,    25,
      41,    41,    20,    -1,    -1,    -1,    72,    71,    69,    -1,
      -1,    -1,    -1,    68
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    14,    48,    16,    68,     0,    39,    37,    49,    40,
      34,    52,    53,    50,    51,    68,    36,    54,    39,    41,
      42,    55,    11,    58,    39,    51,    56,    68,    59,    13,
      69,    43,    60,    68,    35,    57,    44,    35,    39,     4,
       5,     8,    22,    24,    26,    27,    28,    29,    30,    31,
      32,    61,    62,    63,    45,    39,     3,    33,    67,     6,
       7,    17,    18,    38,    64,    65,    66,    46,    40,    40,
      39,    42,    25,    67,    68,    66,    62,    41,    41
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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
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
    while (0)
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
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

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
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
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
      size_t yyn = 0;
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

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
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
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

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
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
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
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
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

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


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
	short int *yyss1 = yyss;
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

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

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

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 45 "bblk.yy"
    {
		(yyval) = (yyvsp[-6]); 
		(yyval)->type = sym_program;
		(yyval)->AddChild((yyvsp[-4]));
		(yyval)->AddChild((yyvsp[-3]));
		(yyval)->AddChild((yyvsp[-2]));
		(yyval)->AddChild((yyvsp[-1]));
		(yyval)->AddChild((yyvsp[0]));
		g_program = (yyval);
		// printf("g_program = 0x%08x\n", g_program);
	;}
    break;

  case 3:
#line 59 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_ATTRIBUTE);
	;}
    break;

  case 4:
#line 63 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_ATTRIBUTE);
		if((yyvsp[-2])){
			(yyval)->AddChild((yyvsp[-2]));
		}
	;}
    break;

  case 5:
#line 72 "bblk.yy"
    {
		(yyval) = (yyvsp[0]);
	;}
    break;

  case 6:
#line 76 "bblk.yy"
    {
		if((yyvsp[-2])){
			(yyval) = (yyvsp[-2]);
			(yyval)->AddSibling((yyvsp[0]));
		}else{
			(yyval) = (yyvsp[0]);
		}
	;}
    break;

  case 7:
#line 87 "bblk.yy"
    {
		(yyval) = new CProperty((yyvsp[0])->val.text, 1);
	;}
    break;

  case 8:
#line 93 "bblk.yy"
    {
		/*
			generate a 8-byte uuid by folding a 16-byte uuid
		*/
		(yyval) = new CSymbol(TOK_UUID);
		create_f8_uuid(&(yyval)->val.uuid);
	;}
    break;

  case 9:
#line 101 "bblk.yy"
    {
		(yyval) = (yyvsp[-1]);
	;}
    break;

  case 10:
#line 107 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_UUID);
		f8_uuid_from_string(&(yyval)->val.uuid, yytext);
	;}
    break;

  case 11:
#line 114 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_COMMENTS);
	;}
    break;

  case 12:
#line 118 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_COMMENTS);
		if((yyvsp[0])){
			(yyval)->AddChild((yyvsp[0]));
		}
	;}
    break;

  case 13:
#line 127 "bblk.yy"
    {
		(yyval) = NULL;
	;}
    break;

  case 14:
#line 131 "bblk.yy"
    {
		if((yyvsp[-1])){
			(yyval) = (yyvsp[-1]);
			(yyval)->AddSibling((yyvsp[0]));
		}else{
			(yyval) = (yyvsp[0]);
		}
	;}
    break;

  case 15:
#line 142 "bblk.yy"
    {
		(yyval) = (yyvsp[-3]);
		(yyval)->AddChild((yyvsp[-1]));
	;}
    break;

  case 16:
#line 149 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_STRING);
		(yyval)->val.text = strdup(yytext);
	;}
    break;

  case 17:
#line 154 "bblk.yy"
    {
		string s;
		s = string((yyval)->val.text) + yytext;
		free((yyval)->val.text);
		(yyval)->val.text = strdup(s.c_str());
	;}
    break;

  case 18:
#line 163 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_INTERFACE);
	;}
    break;

  case 19:
#line 166 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_INTERFACE);
		if( (yyvsp[0]) ){
			(yyval)->AddChild((yyvsp[0]));
		}
	;}
    break;

  case 20:
#line 175 "bblk.yy"
    {
		(yyval) = NULL;
	;}
    break;

  case 21:
#line 179 "bblk.yy"
    {
		(yyval) = (yyvsp[-1]);
		if( !(yyval) ){
 			(yyval) = (yyvsp[0]);
		}else{
			(yyval)->AddSibling((yyvsp[0]));
		}
	;}
    break;

  case 22:
#line 190 "bblk.yy"
    {
		(yyval) = new CSymbol(sym_vardecl);
		(yyval)->val.text = (yyvsp[-4])->val.text;
		(yyvsp[-4])->val.text = NULL;
		delete (yyvsp[-4]);
		(yyval)->AddChild((yyvsp[-2]));
		if((yyvsp[0])){
			(yyval)->AddChild((yyvsp[0]));
		}
	;}
    break;

  case 23:
#line 203 "bblk.yy"
    {
		(yyval) = (yyvsp[0]);
	;}
    break;

  case 24:
#line 207 "bblk.yy"
    {
		(yyval) = (yyvsp[0]);
	;}
    break;

  case 25:
#line 213 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_EVENT);
	;}
    break;

  case 26:
#line 217 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_INTEGER);
	;}
    break;

  case 27:
#line 221 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_FLOAT);
	;}
    break;

  case 28:
#line 225 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_BOOL);
	;}
    break;

  case 29:
#line 229 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_DATE);
	;}
    break;

  case 30:
#line 233 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_BYTE);
	;}
    break;

  case 31:
#line 237 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_WORD);
	;}
    break;

  case 32:
#line 241 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_DOUBLE);
	;}
    break;

  case 33:
#line 245 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_CHAR);
	;}
    break;

  case 34:
#line 249 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_SHORT);
	;}
    break;

  case 35:
#line 253 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_DWORD);
	;}
    break;

  case 36:
#line 259 "bblk.yy"
    {
		(yyval) = new CProperty("type", PIN_T_ARRAY);
		(yyval)->AddChild((yyvsp[-3]));
		(yyval)->AddChild((yyvsp[0]));
	;}
    break;

  case 37:
#line 267 "bblk.yy"
    {
		(yyval) = NULL;
	;}
    break;

  case 38:
#line 271 "bblk.yy"
    {
		(yyval) = (yyvsp[-1]);
	;}
    break;

  case 39:
#line 277 "bblk.yy"
    {
		(yyval) = (yyvsp[0]);
	;}
    break;

  case 40:
#line 281 "bblk.yy"
    {
		(yyval) = (yyvsp[-2]);
		(yyval)->AddSibling((yyvsp[0]));
	;}
    break;

  case 41:
#line 288 "bblk.yy"
    {
		(yyval) = new CProperty("direction", TOK_IN);
	;}
    break;

  case 42:
#line 292 "bblk.yy"
    {
		(yyval) = new CProperty("direction", TOK_OUT);
	;}
    break;

  case 43:
#line 296 "bblk.yy"
    {
		(yyval) = new CProperty("timer", (yyvsp[-1])->val.ival);
		delete (yyvsp[-1]);
	;}
    break;

  case 44:
#line 301 "bblk.yy"
    {
		(yyval) = new CProperty("invoke", (yyvsp[-1])->val.text);
		delete (yyvsp[-1]);
	;}
    break;

  case 45:
#line 306 "bblk.yy"
    {
		(yyval) = new CProperty("hidden", 0);
	;}
    break;

  case 46:
#line 312 "bblk.yy"
    {
		(yyval) = new CSymbol(sym_integer_number);
		(yyval)->val.ival = atoi(yytext);
	;}
    break;

  case 47:
#line 317 "bblk.yy"
    {
		(yyval) = new CSymbol(sym_integer_number);
		sscanf(yytext, "%x", &(yyval)->val.ival);
	;}
    break;

  case 48:
#line 324 "bblk.yy"
    {
		(yyval) = new CSymbol(sym_id);
		(yyval)->val.text = strdup(yytext);
	;}
    break;

  case 49:
#line 331 "bblk.yy"
    {
		(yyval) = new CSymbol(TOK_IMPLEMENTATION);
		(yyval)->val.text = strdup(yytext);
	;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1640 "bblk_yy.cxx"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
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
	  int yychecklim = YYLAST - yyn;
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
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
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
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
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
	  yydestruct ("Error: discarding", yytoken, &yylval);
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
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
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
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 336 "bblk.yy"


void dumpBblkGrammar()
{
	int i;	
	int lastN=-1;	
	for(i=1; i<sizeof(yyrline)/sizeof(yyrline[0]); i++){	  
		int yyi;
		unsigned int yylno = yyrline[i];
	  	// YYFPRINTF (stdout, "Rule %03d (line %03u), ", i - 1, yylno);	  
	  	if(lastN != yyr1[i]){
	  		YYFPRINTF (stdout, "\t;\n\n%s -> ", yytname [yyr1[i]]);
	  		lastN=yyr1[i];
	  	}else{
	  		YYFPRINTF (stdout, "\t| ");
	  	}
		/* Print the symbols being reduced, and their result.  */
		for (yyi = yyprhs[i]; 0 <= yyrhs[yyi]; yyi++){
			YYFPRINTF (stdout, "%s ", yytname [yyrhs[yyi]]);
		}
		YYFPRINTF(stdout, "\n");
	}
}

