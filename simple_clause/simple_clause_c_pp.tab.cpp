/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for GLR parsing with Bison,
   Copyright (C) 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

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

/* This is the parser code for GLR (Generalized LR) parser. */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.cc"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Tokens.  */
   enum yytokentype {
     INDEFINITE_ARTICLE = 258,
     SINGULAR_NOUN = 259,
     PLURAL_NOUN = 260,
     VERB = 261,
     SEMICOLON = 262,
     CONJUNCTION = 263
   };
/* Tokens.  */
#define INDEFINITE_ARTICLE 258
#define SINGULAR_NOUN 259
#define PLURAL_NOUN 260
#define VERB 261
#define SEMICOLON 262
#define CONJUNCTION 263




/* Copy the first part of user declarations.  */


       #include "../stdafx.h"
       #define YYSTYPE double
       //#include <stdio.h>
       //int yylex (void);
       #include "simple_clause_tokenizer.hpp"
       void yyerror (char const *);
       #include "simple_clause_c_pp.tab.hpp"
     

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
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{

  int first_line;
  int first_column;
  int last_line;
  int last_column;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template,
   here we set the default value of $$ to a zeroed-out value.
   Since the default value is undefined, this behavior is
   technically correct. */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */
/* Line 215 of glr.cc.  */


static void yyerror (b4_namespace::BisonEnglishParser::location_type *yylocationp, b4_namespace::b4_parser_class_name& yyparser, [yyparser], const char* msg);

/* Line 217 of glr.c.  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(env) setjmp (env)
# define YYLONGJMP(env, val) longjmp (env, val)
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (!defined (__GNUC__) || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__)
#  define __attribute__(Spec) /* empty */
# endif
#endif

#define YYOPTIONAL_LOC(Name) Name

#ifndef YYASSERT
# define YYASSERT(condition) ((void) ((condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  9
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  3
/* YYNRULES -- Number of rules. */
#define YYNRULES  3
/* YYNRULES -- Number of states. */
#define YYNSTATES  5
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule. */
#define YYMAXRHS 2
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule. */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   263

#define YYTRANSLATE(YYX)						\
  ((YYX <= 0) ? YYEOF :							\
   (unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
       5,     6,     7,     8
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const signed char yyrhs[] =
{
      10,     0,    -1,    11,    -1,     5,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    69,    69,   112
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INDEFINITE_ARTICLE", "SINGULAR_NOUN",
  "PLURAL_NOUN", "VERB", "SEMICOLON", "CONJUNCTION", "$accept", "sentence",
  "plural_noun", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,     9,    10,    11
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none). */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM. */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error.  */
static const unsigned char yydefact[] =
{
       0,     3,     0,     2,     1
};

/* YYPDEFGOTO[NTERM-NUM]. */
static const signed char yydefgoto[] =
{
      -1,     2,     3
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -6
static const signed char yypact[] =
{
      -5,    -6,     1,    -6,    -6
};

/* YYPGOTO[NTERM-NUM].  */
static const signed char yypgoto[] =
{
      -6,    -6,    -6
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
       1,     4
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0
};

static const unsigned char yycheck[] =
{
       5,     0
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     5,    10,    11,     0
};


/* Prevent warning if -Wmissing-prototypes.  */
int yyparse (b4_namespace::b4_parser_class_name& yyparser, [yyparser]);

/* Error token number */
#define YYTERROR 1

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */


#define YYRHSLOC(Rhs, K) ((Rhs)[K].yystate.yyloc)
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

/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# define YY_LOCATION_PRINT(File, Loc)			\
    fprintf (File, "%d.%d-%d.%d",			\
             (Loc).first_line, (Loc).first_column,	\
             (Loc).last_line,  (Loc).last_column)
#endif


#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex ()

YYSTYPE yylval;

YYLTYPE yylloc;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)							     \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }	     \
   while (0)

#if YYDEBUG

#if ! defined (YYFPRINTF)
#  define YYFPRINTF fprintf
#endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");

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


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value, Location);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data. */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined (__cplusplus) \
      || (defined (YYLTYPE_IS_TRIVIAL) && YYLTYPE_IS_TRIVIAL \
	  && defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static size_t
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
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;
typedef struct yyGLRStateSet yyGLRStateSet;

struct yyGLRState {
  /** Type tag: always true. */
  yybool yyisState;
  /** Type tag for yysemantics. If true, yysval applies, otherwise
   *  yyfirstVal applies. */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state. */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext. */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state. */
    YYSTYPE yysval;
  } yysemantics;
  /** Source location for this state. */
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false. */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced. */
  yyGLRState* yystate;
  /** Next sibling in chain of options. To facilitate merging,
   *  options are chained in decreasing order by address. */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack. The yyisState field
 *  indicates which item of the union is valid. */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;
  /* To compute the location of the error token.  */
  yyGLRStackItem yyerror_range[3];

  yySymbol* yytokenp;
  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

static void yyexpandGLRStack (yyGLRStack* yystack);

static void yyFail (yyGLRStack* yystack, b4_namespace::b4_parser_class_name& yyparser, [yyparser], const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystack, b4_namespace::b4_parser_class_name& yyparser, [yyparser], const char* yymsg)
{
  if (yymsg != NULL)
    yyerror (, yymsg);
  YYLONGJMP (yystack->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystack)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystack)
{
  YYLONGJMP (yystack->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain. Assumes
 *  YYLOW1 < YYLOW0.  */
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  yyGLRState* s;
  int i;
  s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      yyvsp[i].yystate.yyloc = s->yyloc;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
   YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
   For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     __attribute__ ((__unused__));
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$). Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT. */
static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
	      YYSTYPE* yyvalp,
	      YYLTYPE* YYOPTIONAL_LOC (yylocp),
	      yyGLRStack* yystack
              , b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystack->yysplitPoint == NULL);
  int yylow;

# undef yyerrok
# define yyerrok (yystack->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING (yystack->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = *(yystack->yytokenp) = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)						     \
  return yyerror (, YY_("syntax error: cannot back up")),     \
	 yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  YYLLOC_DEFAULT (*yylocp, yyvsp - yyrhslen, yyrhslen);
  yystack->yyerror_range[1].yystate.yyloc = *yylocp;

  switch (yyn)
    {
        case 3:

    { g_enumeleCurrent.push_back(englishnounrecordCurrent) ;  ;}
    break;


      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
/* Line 872 of glr.c.  */

}


static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  /* `Use' the arguments.  */
  (void) yy0;
  (void) yy1;

  switch (yyn)
    {
      
      default: break;
    }
}

			      /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
		&yys->yysemantics.yysval, &yys->yyloc);
  else
    {
#if YYDEBUG
      if (yydebug)
	{
	  YYFPRINTF (stderr, "%s unresolved ", yymsg);
	  yysymprint (stderr, yystos[yys->yylrState],
		      &yys->yysemantics.yysval, &yys->yyloc);
	  YYFPRINTF (stderr, "\n");
	}
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh);
        }
    }
}

/** Left-hand-side symbol for rule #RULE. */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yyis_pact_ninf(yystate) \
  ((yystate) == YYPACT_NINF)

/** True iff LR state STATE has only a default reduction (regardless
 *  of token). */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yyis_pact_ninf (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one. */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yyis_table_ninf(yytable_value) \
  0

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
	        int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yyis_table_ninf (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

				/* GLRStates */

static void
yyaddDeferredAction (yyGLRStack* yystack, yyGLRState* yystate,
		     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewItem;
  yynewItem = &yystack->yynextFree->yyoption;
  yystack->yyspaceLeft -= 1;
  yystack->yynextFree += 1;
  yynewItem->yyisState = yyfalse;
  yynewItem->yystate = rhs;
  yynewItem->yyrule = yyrule;
  yynewItem->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewItem;
  if (yystack->yyspaceLeft < YYHEADROOM)
    yyexpandGLRStack (yystack);
}

				/* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack. */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = NULL;
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE. */
static yybool
yyinitGLRStack (yyGLRStack* yystack, size_t yysize)
{
  yystack->yyerrState = 0;
  yynerrs = 0;
  yystack->yyspaceLeft = yysize;
  yystack->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystack->yynextFree[0]);
  if (!yystack->yyitems)
    return yyfalse;
  yystack->yynextFree = yystack->yyitems;
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;
  return yyinitStateSet (&yystack->yytops);
}

#define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation. */
static void
yyexpandGLRStack (yyGLRStack* yystack)
{
#if YYSTACKEXPANDABLE
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yysize, yynewSize;
  size_t yyn;
  yysize = yystack->yynextFree - yystack->yyitems;
  if (YYMAXDEPTH <= yysize)
    yyMemoryExhausted (yystack);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystack);
  for (yyp0 = yystack->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
	{
	  yyGLRState* yys0 = &yyp0->yystate;
	  yyGLRState* yys1 = &yyp1->yystate;
	  if (yys0->yypred != NULL)
	    yys1->yypred =
	      YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
	  if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != NULL)
	    yys1->yysemantics.yyfirstVal =
	      YYRELOC(yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
	}
      else
	{
	  yySemanticOption* yyv0 = &yyp0->yyoption;
	  yySemanticOption* yyv1 = &yyp1->yyoption;
	  if (yyv0->yystate != NULL)
	    yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
	  if (yyv0->yynext != NULL)
	    yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
	}
    }
  if (yystack->yysplitPoint != NULL)
    yystack->yysplitPoint = YYRELOC (yystack->yyitems, yynewItems,
				 yystack->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystack->yytops.yysize; yyn += 1)
    if (yystack->yytops.yystates[yyn] != NULL)
      yystack->yytops.yystates[yyn] =
	YYRELOC (yystack->yyitems, yynewItems,
		 yystack->yytops.yystates[yyn], yystate);
  YYFREE (yystack->yyitems);
  yystack->yyitems = yynewItems;
  yystack->yynextFree = yynewItems + yysize;
  yystack->yyspaceLeft = yynewSize - yysize;

#else
  yyMemoryExhausted (yystack);
#endif
}

static void
yyfreeGLRStack (yyGLRStack* yystack)
{
  YYFREE (yystack->yyitems);
  yyfreeStateSet (&yystack->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S. */
static inline void
yyupdateSplit (yyGLRStack* yystack, yyGLRState* yys)
{
  if (yystack->yysplitPoint != NULL && yystack->yysplitPoint > yys)
    yystack->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK. */
static inline void
yymarkStackDeleted (yyGLRStack* yystack, size_t yyk)
{
  if (yystack->yytops.yystates[yyk] != NULL)
    yystack->yylastDeleted = yystack->yytops.yystates[yyk];
  yystack->yytops.yystates[yyk] = NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted. */
static void
yyundeleteLastStack (yyGLRStack* yystack)
{
  if (yystack->yylastDeleted == NULL || yystack->yytops.yysize != 0)
    return;
  yystack->yytops.yystates[0] = yystack->yylastDeleted;
  yystack->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystack->yylastDeleted = NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystack)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystack->yytops.yysize)
    {
      if (yystack->yytops.yystates[yyi] == NULL)
	{
	  if (yyi == yyj)
	    {
	      YYDPRINTF ((stderr, "Removing dead stacks.\n"));
	    }
	  yystack->yytops.yysize -= 1;
	}
      else
	{
	  yystack->yytops.yystates[yyj] = yystack->yytops.yystates[yyi];
	  if (yyj != yyi)
	    {
	      YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
			  (unsigned long int) yyi, (unsigned long int) yyj));
	    }
	  yyj += 1;
	}
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL. */
static inline void
yyglrShift (yyGLRStack* yystack, size_t yyk, yyStateNum yylrState,
	    size_t yyposn,
	    YYSTYPE yysval, YYLTYPE* yylocp)
{
  yyGLRStackItem* yynewItem;

  yynewItem = yystack->yynextFree;
  yystack->yynextFree += 1;
  yystack->yyspaceLeft -= 1;
  yynewItem->yystate.yyisState = yytrue;
  yynewItem->yystate.yylrState = yylrState;
  yynewItem->yystate.yyposn = yyposn;
  yynewItem->yystate.yyresolved = yytrue;
  yynewItem->yystate.yypred = yystack->yytops.yystates[yyk];
  yystack->yytops.yystates[yyk] = &yynewItem->yystate;
  yynewItem->yystate.yysemantics.yysval = yysval;
  yynewItem->yystate.yyloc = *yylocp;
  if (yystack->yyspaceLeft < YYHEADROOM)
    yyexpandGLRStack (yystack);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE. */
static inline void
yyglrShiftDefer (yyGLRStack* yystack, size_t yyk, yyStateNum yylrState,
		 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRStackItem* yynewItem;

  yynewItem = yystack->yynextFree;
  yynewItem->yystate.yyisState = yytrue;
  yynewItem->yystate.yylrState = yylrState;
  yynewItem->yystate.yyposn = yyposn;
  yynewItem->yystate.yyresolved = yyfalse;
  yynewItem->yystate.yypred = yystack->yytops.yystates[yyk];
  yynewItem->yystate.yysemantics.yyfirstVal = NULL;
  yystack->yytops.yystates[yyk] = &yynewItem->yystate;
  yystack->yynextFree += 1;
  yystack->yyspaceLeft -= 1;
  yyaddDeferredAction (yystack, &yynewItem->yystate, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved. Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction. */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystack, size_t yyk, yyRuleNum yyrule,
	    YYSTYPE* yyvalp, YYLTYPE* yylocp, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystack->yysplitPoint == NULL)
    {
      /* Standard special case: single stack. */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystack->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystack->yynextFree -= yynrhs;
      yystack->yyspaceLeft += yynrhs;
      yystack->yytops.yystates[0] = & yystack->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs,
			   yyvalp, yylocp, yystack, );
    }
  else
    {
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
	= yystack->yytops.yystates[yyk];
      for (yyi = 0; yyi < yynrhs; yyi += 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystack, yys);
      yystack->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
			   yyvalp, yylocp, yystack, );
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(K, Rule)
#else
# define YY_REDUCE_PRINT(K, Rule)	\
do {					\
  if (yydebug)				\
    yy_reduce_print (K, Rule);		\
} while (0)

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

static inline void
yy_reduce_print (size_t yyk, yyRuleNum yyrule)
{
  int yyi;
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu), ",
	     (unsigned long int) yyk, yyrule - 1,
	     (unsigned long int) yyrline[yyrule]);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytokenName (yyrhs[yyi]));
  YYFPRINTF (stderr, "-> %s\n", yytokenName (yyr1[yyrule]));
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK. In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystack, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  size_t yyposn = yystack->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystack->yysplitPoint == NULL)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YY_REDUCE_PRINT (yyk, yyrule);
      YYCHK (yydoAction (yystack, yyk, yyrule, &yysval, &yyloc, ));
      yyglrShift (yystack, yyk,
		  yyLRgotoState (yystack->yytops.yystates[yyk]->yylrState,
				 yylhsNonterm (yyrule)),
		  yyposn, yysval, &yyloc);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystack->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystack->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
	   0 < yyn; yyn -= 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystack, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
		  "Reduced stack %lu by rule #%d; action deferred. Now in state %d.\n",
		  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystack->yytops.yysize; yyi += 1)
	if (yyi != yyk && yystack->yytops.yystates[yyi] != NULL)
	  {
	    yyGLRState* yyp, *yysplit = yystack->yysplitPoint;
	    yyp = yystack->yytops.yystates[yyi];
	    while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
	      {
		if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
		  {
		    yyaddDeferredAction (yystack, yyp, yys0, yyrule);
		    yymarkStackDeleted (yystack, yyk);
		    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
				(unsigned long int) yyk,
				(unsigned long int) yyi));
		    return yyok;
		  }
		yyp = yyp->yypred;
	      }
	  }
      yystack->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystack, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystack, size_t yyk)
{
  if (yystack->yysplitPoint == NULL)
    {
      YYASSERT (yyk == 0);
      yystack->yysplitPoint = yystack->yytops.yystates[yyk];
    }
  if (yystack->yytops.yysize >= yystack->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      if (! ((yystack->yytops.yycapacity
	      <= (YYSIZEMAX / (2 * sizeof yynewStates[0])))
	     && (yynewStates =
		 (yyGLRState**) YYREALLOC (yystack->yytops.yystates,
					   ((yystack->yytops.yycapacity *= 2)
					    * sizeof yynewStates[0])))))
	yyMemoryExhausted (yystack);
      yystack->yytops.yystates = yynewStates;
    }
  yystack->yytops.yystates[yystack->yytops.yysize]
    = yystack->yytops.yystates[yyk];
  yystack->yytops.yysize += 1;
  return yystack->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols. */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
	   yyn = yyrhsLength (yyy0->yyrule);
	   yyn > 0;
	   yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
	if (yys0->yyposn != yys1->yyposn)
	  return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0. */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
	break;
      else if (yys0->yyresolved)
	{
	  yys1->yyresolved = yytrue;
	  yys1->yysemantics.yysval = yys0->yysemantics.yysval;
	}
      else if (yys1->yyresolved)
	{
	  yys0->yyresolved = yytrue;
	  yys0->yysemantics.yysval = yys1->yysemantics.yysval;
	}
      else
	{
	  yySemanticOption** yyz0p;
	  yySemanticOption* yyz1;
	  yyz0p = &yys0->yysemantics.yyfirstVal;
	  yyz1 = yys1->yysemantics.yyfirstVal;
	  while (yytrue)
	    {
	      if (yyz1 == *yyz0p || yyz1 == NULL)
		break;
	      else if (*yyz0p == NULL)
		{
		  *yyz0p = yyz1;
		  break;
		}
	      else if (*yyz0p < yyz1)
		{
		  yySemanticOption* yyz = *yyz0p;
		  *yyz0p = yyz1;
		  yyz1 = yyz1->yynext;
		  (*yyz0p)->yynext = yyz;
		}
	      yyz0p = &(*yyz0p)->yynext;
	    }
	  yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
	}
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred. */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
	return 0;
      else
	return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yySemanticOption* yyoptionList,
				   yyGLRStack* yystack, YYSTYPE* yyvalp,
				   YYLTYPE* yylocp, b4_namespace::b4_parser_class_name& yyparser, [yyparser]);

static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn, yyGLRStack* yystack, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  YYRESULTTAG yyflag;
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      yyflag = yyresolveStates (yys->yypred, yyn-1, yystack, );
      if (yyflag != yyok)
	return yyflag;
      if (! yys->yyresolved)
	{
	  yyflag = yyresolveValue (yys->yysemantics.yyfirstVal, yystack,
				   &yys->yysemantics.yysval, &yys->yyloc
				  , );
	  if (yyflag != yyok)
	    return yyflag;
	  yys->yyresolved = yytrue;
	}
    }
  return yyok;
}

static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystack,
	         YYSTYPE* yyvalp, YYLTYPE* yylocp, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs;

  yynrhs = yyrhsLength (yyopt->yyrule);
  YYCHK (yyresolveStates (yyopt->yystate, yynrhs, yystack, ));
  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  return yyuserAction (yyopt->yyrule, yynrhs,
		       yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
		       yyvalp, yylocp, yystack, );
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule, (unsigned long int) (yys->yyposn + 1),
	       (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
	{
	  if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
	    YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
	  else
	    YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
		       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
		       (unsigned long int) yystates[yyi]->yyposn);
	}
      else
	yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

static void yyreportAmbiguity (yySemanticOption* yyx0, yySemanticOption* yyx1,
			       yyGLRStack* yystack, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
  __attribute__ ((__noreturn__));
static void
yyreportAmbiguity (yySemanticOption* yyx0, yySemanticOption* yyx1,
		   yyGLRStack* yystack, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  /* `Unused' warnings.  */
  (void) yyx0;
  (void) yyx1;

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif
  yyFail (yystack, , YY_("syntax is ambiguous"));
}


/** Resolve the ambiguity represented by OPTIONLIST, perform the indicated
 *  actions, and return the result. */
static YYRESULTTAG
yyresolveValue (yySemanticOption* yyoptionList, yyGLRStack* yystack,
		YYSTYPE* yyvalp, YYLTYPE* yylocp, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  yySemanticOption* yybest;
  yySemanticOption** yypp;
  yybool yymerge;

  yybest = yyoptionList;
  yymerge = yyfalse;
  for (yypp = &yyoptionList->yynext; *yypp != NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
	{
	  yymergeOptionSets (yybest, yyp);
	  *yypp = yyp->yynext;
	}
      else
	{
	  switch (yypreference (yybest, yyp))
	    {
	    case 0:
	      yyreportAmbiguity (yybest, yyp, yystack, );
	      break;
	    case 1:
	      yymerge = yytrue;
	      break;
	    case 2:
	      break;
	    case 3:
	      yybest = yyp;
	      yymerge = yyfalse;
	      break;
	    default:
	      /* This cannot happen so it is not worth a YYASSERT (yyfalse),
	         but some compilers complain if the default case is
		 omitted.  */
	      break;
	    }
	  yypp = &yyp->yynext;
	}
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      YYCHK (yyresolveAction (yybest, yystack, yyvalp, yylocp, ));
      for (yyp = yybest->yynext; yyp != NULL; yyp = yyp->yynext)
	{
	  if (yyprec == yydprec[yyp->yyrule])
	    {
	      YYSTYPE yyval1;
	      YYLTYPE yydummy;
	      YYCHK (yyresolveAction (yyp, yystack, &yyval1, &yydummy, ));
	      yyuserMerge (yymerger[yyp->yyrule], yyvalp, &yyval1);
	    }
	}
      return yyok;
    }
  else
    return yyresolveAction (yybest, yystack, yyvalp, yylocp, );
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystack, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  if (yystack->yysplitPoint != NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystack->yytops.yystates[0];
	   yys != yystack->yysplitPoint;
	   yys = yys->yypred, yyn += 1)
	continue;
      YYCHK (yyresolveStates (yystack->yytops.yystates[0], yyn, yystack
			     , ));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystack)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystack->yytops.yysize != 1 || yystack->yysplitPoint == NULL)
    return;

  for (yyp = yystack->yytops.yystates[0], yyq = yyp->yypred, yyr = NULL;
       yyp != yystack->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystack->yyspaceLeft += yystack->yynextFree - yystack->yyitems;
  yystack->yynextFree = ((yyGLRStackItem*) yystack->yysplitPoint) + 1;
  yystack->yyspaceLeft -= yystack->yynextFree - yystack->yyitems;
  yystack->yysplitPoint = NULL;
  yystack->yylastDeleted = NULL;

  while (yyr != NULL)
    {
      yystack->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystack->yynextFree->yystate.yypred = & yystack->yynextFree[-1].yystate;
      yystack->yytops.yystates[0] = &yystack->yynextFree->yystate;
      yystack->yynextFree += 1;
      yystack->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystack, size_t yyk,
	           size_t yyposn, YYSTYPE* yylvalp, YYLTYPE* yyllocp
		  , b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;
  yySymbol* const yytokenp = yystack->yytokenp;

  while (yystack->yytops.yystates[yyk] != NULL)
    {
      yyStateNum yystate = yystack->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
		  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
	{
	  yyrule = yydefaultAction (yystate);
	  if (yyrule == 0)
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
	      yymarkStackDeleted (yystack, yyk);
	      return yyok;
	    }
	  YYCHK (yyglrReduce (yystack, yyk, yyrule, yyfalse, ));
	}
      else
	{
	  if (*yytokenp == YYEMPTY)
	    {
	      YYDPRINTF ((stderr, "Reading a token: "));
	      yychar = YYLEX;
	      *yytokenp = YYTRANSLATE (yychar);
	      YY_SYMBOL_PRINT ("Next token is", *yytokenp, yylvalp, yyllocp);
	    }
	  yygetLRActions (yystate, *yytokenp, &yyaction, &yyconflicts);

	  while (*yyconflicts != 0)
	    {
	      size_t yynewStack = yysplitStack (yystack, yyk);
	      YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
			  (unsigned long int) yynewStack,
			  (unsigned long int) yyk));
	      YYCHK (yyglrReduce (yystack, yynewStack,
				  *yyconflicts, yyfalse, ));
	      YYCHK (yyprocessOneStack (yystack, yynewStack, yyposn,
					yylvalp, yyllocp, ));
	      yyconflicts += 1;
	    }

	  if (yyisShiftAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yyk));
	      YY_SYMBOL_PRINT ("shifting", *yytokenp, yylvalp, yyllocp);
	      yyglrShift (yystack, yyk, yyaction, yyposn+1,
			  *yylvalp, yyllocp);
	      YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
			  (unsigned long int) yyk,
			  yystack->yytops.yystates[yyk]->yylrState));
	      break;
	    }
	  else if (yyisErrorAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
	      yymarkStackDeleted (yystack, yyk);
	      break;
	    }
	  else
	    YYCHK (yyglrReduce (yystack, yyk, -yyaction, yyfalse, ));
	}
    }
  return yyok;
}

static void
yyreportSyntaxError (yyGLRStack* yystack,
		     YYSTYPE* yylvalp, YYLTYPE* yyllocp, b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  /* `Unused' warnings. */
  (void) yylvalp;
  (void) yyllocp;

  if (yystack->yyerrState == 0)
    {
#if YYERROR_VERBOSE
      yySymbol* const yytokenp = yystack->yytokenp;
      int yyn;
      yyn = yypact[yystack->yytops.yystates[0]->yylrState];
      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  size_t yysize0 = yytnamerr (NULL, yytokenName (*yytokenp));
	  size_t yysize = yysize0;
	  size_t yysize1;
	  yybool yysize_overflow = yyfalse;
	  char* yymsg = NULL;
	  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;
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

	  yyarg[0] = yytokenName (*yytokenp);
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
		yyarg[yycount++] = yytokenName (yyx);
		yysize1 = yysize + yytnamerr (NULL, yytokenName (yyx));
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + strlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow)
	    yymsg = (char *) YYMALLOC (yysize);

	  if (yymsg)
	    {
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
	      yyerror (, yymsg);
	      YYFREE (yymsg);
	    }
	  else
	    {
	      yyerror (, YY_("syntax error"));
	      yyMemoryExhausted (yystack);
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (, YY_("syntax error"));
      yynerrs += 1;
    }
}

/* Recover from a syntax error on YYSTACK, assuming that YYTOKENP,
   YYLVALP, and YYLLOCP point to the syntactic category, semantic
   value, and location of the look-ahead.  */
static void
yyrecoverSyntaxError (yyGLRStack* yystack,
		      YYSTYPE* yylvalp,
		      YYLTYPE* YYOPTIONAL_LOC (yyllocp)
		      , b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  yySymbol* const yytokenp = yystack->yytokenp;
  size_t yyk;
  int yyj;

  if (yystack->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (yytrue)
      {
	if (*yytokenp == YYEOF)
	  yyFail (yystack, , NULL);
	if (*yytokenp != YYEMPTY)
	  {
	    /* We throw away the lookahead, but the error range
	       of the shifted error token must take it into account. */
	    yyGLRState *yys = yystack->yytops.yystates[0];
	    yyGLRStackItem yyerror_range[3];
	    yyerror_range[1].yystate.yyloc = yys->yyloc;
	    yyerror_range[2].yystate.yyloc = *yyllocp;
	    YYLLOC_DEFAULT (yys->yyloc, yyerror_range, 2);
	    yydestruct ("Error: discarding",
			*yytokenp, yylvalp, yyllocp);
	  }
	YYDPRINTF ((stderr, "Reading a token: "));
	yychar = YYLEX;
	*yytokenp = YYTRANSLATE (yychar);
	YY_SYMBOL_PRINT ("Next token is", *yytokenp, yylvalp, yyllocp);
	yyj = yypact[yystack->yytops.yystates[0]->yylrState];
	if (yyis_pact_ninf (yyj))
	  return;
	yyj += *yytokenp;
	if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != *yytokenp)
	  {
	    if (yydefact[yystack->yytops.yystates[0]->yylrState] != 0)
	      return;
	  }
	else if (yytable[yyj] != 0 && ! yyis_table_ninf (yytable[yyj]))
	  return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystack->yytops.yysize; yyk += 1)
    if (yystack->yytops.yystates[yyk] != NULL)
      break;
  if (yyk >= yystack->yytops.yysize)
    yyFail (yystack, , NULL);
  for (yyk += 1; yyk < yystack->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystack, yyk);
  yyremoveDeletes (yystack);
  yycompressStack (yystack);

  /* Now pop stack until we find a state that shifts the error token. */
  yystack->yyerrState = 3;
  while (yystack->yytops.yystates[0] != NULL)
    {
      yyGLRState *yys = yystack->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yyis_pact_ninf (yyj))
	{
	  yyj += YYTERROR;
	  if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
	      && yyisShiftAction (yytable[yyj]))
	    {
	      /* Shift the error token having adjusted its location.  */
	      YYLTYPE yyerrloc;
	      yystack->yyerror_range[2].yystate.yyloc = *yyllocp;
	      YYLLOC_DEFAULT (yyerrloc, yystack->yyerror_range, 2);
	      YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
			       yylvalp, &yyerrloc);
	      yyglrShift (yystack, 0, yytable[yyj],
			  yys->yyposn, *yylvalp, &yyerrloc);
	      yys = yystack->yytops.yystates[0];
	      break;
	    }
	}
      yystack->yyerror_range[1].yystate.yyloc = yys->yyloc;
      yydestroyGLRState ("Error: popping", yys);
      yystack->yytops.yystates[0] = yys->yypred;
      yystack->yynextFree -= 1;
      yystack->yyspaceLeft += 1;
    }
  if (yystack->yytops.yystates[0] == NULL)
    yyFail (yystack, , NULL);
}

#define YYCHK1(YYE)							     \
  do {									     \
    switch (YYE) {							     \
    case yyok:								     \
      break;								     \
    case yyabort:							     \
      goto yyabortlab;							     \
    case yyaccept:							     \
      goto yyacceptlab;							     \
    case yyerr:								     \
      goto yyuser_error;						     \
    default:								     \
      goto yybuglab;							     \
    }									     \
  } while (0)


/*----------.
| yyparse.  |
`----------*/

int
yyparse (b4_namespace::b4_parser_class_name& yyparser, [yyparser])
{
  int yyresult;
  yySymbol yytoken;
  yyGLRStack yystack;
  size_t yyposn;


  YYSTYPE* const yylvalp = &yylval;
  YYLTYPE* const yyllocp = &yylloc;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yytoken = YYEMPTY;
  yylval = yyval_default;

#if YYLTYPE_IS_TRIVIAL
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif


  if (! yyinitGLRStack (&yystack, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yystack.yytokenp = &yytoken;
  yyglrShift (&yystack, 0, 0, 0, yylval, &yylloc);
  yyposn = 0;

  while (yytrue)
    {
      /* For efficiency, we have two loops, the first of which is
	 specialized to deterministic operation (single stack, no
	 potential ambiguity).  */
      /* Standard mode */
      while (yytrue)
	{
	  yyRuleNum yyrule;
	  int yyaction;
	  const short int* yyconflicts;

	  yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
	  if (yystate == YYFINAL)
	    goto yyacceptlab;
	  if (yyisDefaultedState (yystate))
	    {
	      yyrule = yydefaultAction (yystate);
	      if (yyrule == 0)
		{
		  yystack.yyerror_range[1].yystate.yyloc = *yyllocp;
		  yyreportSyntaxError (&yystack, yylvalp, yyllocp, );
		  goto yyuser_error;
		}
	      YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue, ));
	    }
	  else
	    {
	      if (yytoken == YYEMPTY)
		{
		  YYDPRINTF ((stderr, "Reading a token: "));
		  yychar = YYLEX;
		  yytoken = YYTRANSLATE (yychar);
                  YY_SYMBOL_PRINT ("Next token is", yytoken, yylvalp, yyllocp);
		}
	      yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
	      if (*yyconflicts != 0)
		break;
	      if (yyisShiftAction (yyaction))
		{
		  YY_SYMBOL_PRINT ("Shifting", yytoken, yylvalp, yyllocp);
		  if (yytoken != YYEOF)
		    yytoken = YYEMPTY;
		  yyposn += 1;
		  yyglrShift (&yystack, 0, yyaction, yyposn, yylval, yyllocp);
		  if (0 < yystack.yyerrState)
		    yystack.yyerrState -= 1;
		}
	      else if (yyisErrorAction (yyaction))
		{
		  yystack.yyerror_range[1].yystate.yyloc = *yyllocp;
		  yyreportSyntaxError (&yystack, yylvalp, yyllocp, );
		  goto yyuser_error;
		}
	      else
		YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue, ));
	    }
	}

      while (yytrue)
	{
	  size_t yys;
	  size_t yyn = yystack.yytops.yysize;
	  for (yys = 0; yys < yyn; yys += 1)
	    YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn,
				       yylvalp, yyllocp, ));
	  yytoken = YYEMPTY;
	  yyposn += 1;
	  yyremoveDeletes (&yystack);
	  if (yystack.yytops.yysize == 0)
	    {
	      yyundeleteLastStack (&yystack);
	      if (yystack.yytops.yysize == 0)
		yyFail (&yystack, , YY_("syntax error"));
	      YYCHK1 (yyresolveStack (&yystack, ));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yystack.yyerror_range[1].yystate.yyloc = *yyllocp;
	      yyreportSyntaxError (&yystack, yylvalp, yyllocp, );
	      goto yyuser_error;
	    }
	  else if (yystack.yytops.yysize == 1)
	    {
	      YYCHK1 (yyresolveStack (&yystack, ));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yycompressStack (&yystack);
	      break;
	    }
	}
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack, yylvalp, yyllocp, );
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  /* Fall through.  */

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */

 yyreturn:
  if (yytoken != YYEOF && yytoken != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                yytoken, yylvalp, yyllocp);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
	while (yystates[0])
	  {
	    yyGLRState *yys = yystates[0];
	  yystack.yyerror_range[1].yystate.yyloc = yys->yyloc;
	    yydestroyGLRState ("Cleanup: popping", yys);
	    yystates[0] = yys->yypred;
	    yystack.yynextFree -= 1;
	    yystack.yyspaceLeft += 1;
	  }
      yyfreeGLRStack (&yystack);
    }

  return yyresult;
}

/* DEBUGGING ONLY */
#ifdef YYDEBUG
static void yypstack (yyGLRStack* yystack, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystack) __attribute__ ((__unused__));

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      fprintf (stderr, " -> ");
    }
  fprintf (stderr, "%d@%lu", yys->yylrState, (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == NULL)
    fprintf (stderr, "<null>");
  else
    yy_yypstack (yyst);
  fprintf (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystack, size_t yyk)
{
  yypstates (yystack->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)							     \
    ((YYX) == NULL ? -1 : (yyGLRStackItem*) (YYX) - yystack->yyitems)


static void
yypdumpstack (yyGLRStack* yystack)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystack->yyitems; yyp < yystack->yynextFree; yyp += 1)
    {
      fprintf (stderr, "%3lu. ", (unsigned long int) (yyp - yystack->yyitems));
      if (*(yybool *) yyp)
	{
	  fprintf (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
		   yyp->yystate.yyresolved, yyp->yystate.yylrState,
		   (unsigned long int) yyp->yystate.yyposn,
		   (long int) YYINDEX (yyp->yystate.yypred));
	  if (! yyp->yystate.yyresolved)
	    fprintf (stderr, ", firstVal: %ld",
		     (long int) YYINDEX (yyp->yystate.yysemantics.yyfirstVal));
	}
      else
	{
	  fprintf (stderr, "Option. rule: %d, state: %ld, next: %ld",
		   yyp->yyoption.yyrule,
		   (long int) YYINDEX (yyp->yyoption.yystate),
		   (long int) YYINDEX (yyp->yyoption.yynext));
	}
      fprintf (stderr, "\n");
    }
  fprintf (stderr, "Tops:");
  for (yyi = 0; yyi < yystack->yytops.yysize; yyi += 1)
    fprintf (stderr, "%lu: %ld; ", (unsigned long int) yyi,
	     (long int) YYINDEX (yystack->yytops.yystates[yyi]));
  fprintf (stderr, "\n");
}
#endif





     %{
       #define YYSTYPE double
       //#include <stdio.h>
       //int yylex (void);
       #include "simple_clause_tokenizer.hpp"
       void yyerror (char const *);
     %}
     
/* Line 2318 of glr.cc.  */

/*------------------.
| Report an error.  |
`------------------*/

static void
yyerror (b4_namespace::BisonEnglishParser::location_type *yylocationp, b4_namespace::b4_parser_class_name& yyparser, [yyparser], const char* msg)
{
b4_parse_param_use  yyparser.error (*yylocationp, msg);
}


namespace b4_namespace
{
  /// Build a parser object.
  BisonEnglishParser::BisonEnglishParser ()
    : yycdebug_ (&std::cerr)
  {
  }

  BisonEnglishParser::~BisonEnglishParser ()
  {
  }

  int
  BisonEnglishParser::parse ()
  {
    return ::yyparse (*this);
  }

#if YYDEBUG
  /*--------------------.
  | Print this symbol.  |
  `--------------------*/

  inline void
  BisonEnglishParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    /* Pacify ``unused variable'' warnings.  */
    YYUSE (yyvaluep);
    YYUSE (yylocationp);
    switch (yytype)
      {
          default:
	  break;
      }
  }


  void
  BisonEnglishParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < YYNTOKENS ? "token" : "nterm")
	       << ' ' << yytname[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }

  std::ostream&
  BisonEnglishParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  BisonEnglishParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  BisonEnglishParser::debug_level_type
  BisonEnglishParser::debug_level () const
  {
    return ::yydebug;
  }

  void
  BisonEnglishParser::set_debug_level (debug_level_type l)
  {
    ::yydebug = l;
  }

#endif /* ! YYDEBUG */
} // namespace b4_namespace


