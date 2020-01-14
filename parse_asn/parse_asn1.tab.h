/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSE_ASN1_TAB_H_INCLUDED
# define YY_YY_PARSE_ASN1_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NEED_TOKEN = 258,
     COND_TOKEN = 259,
     DEFAULT_TOKNE = 260,
     SIZE_TOKEN = 261,
     OP_TOKEN = 262,
     OR_TOKEN = 263,
     ON_TOKEN = 264,
     OPTION_TOKEN = 265,
     SEQUENCE_TOKEN = 266,
     ENUMERATED_TOKEN = 267,
     CHOICE_TOKEN = 268,
     INTEGER_TOKEN = 269,
     OCTET_STRING_TOKEN = 270,
     BIT_STRING_TOKEN = 271,
     CONTAINING_TOKEN = 272,
     EXTENSION_TAG_TOKEN = 273,
     BOOLEAN_TOKEN = 274,
     NULL_TOKEN = 275,
     SETUP_RELEASE_TOKEN = 276,
     LFIRSTCASETERM = 277,
     UFIRSTCASETERM = 278,
     DIGIT = 279,
     BITVALUE_TOKEN = 280,
     ASSIGN_TOKEN = 281,
     COMMA_TOKEN = 282,
     DASH_TOKEN = 283,
     OF_TOKEN = 284,
     LPARENTHESIS_TOKEN = 285,
     RPARENTHESIS_TOKEN = 286,
     LBRACKET_TOKEN = 287,
     RBRACKET_TOKEN = 288,
     LDBRACKET_TOKEN = 289,
     RDBRACKET_TOKEN = 290,
     ELLIPSIS_TOKEN = 291,
     LEFTBRACE_TOKEN = 292,
     RIGHTBRACE_TOKEN = 293
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 30 "parse_asn1.y"

	char *chPtr;			/* 保存各种元素的名称 */

	IEItem *structPtr;	/* 保存结构体指针 */
	BASE_TYPE_E type;		/* 用于baseType */
	IE_ATTRI_E  attri;		/* 用于ie的属性 */
	IERange *ieRangPtr;
	NEED_TYPE_E needType;


/* Line 2058 of yacc.c  */
#line 106 "parse_asn1.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
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

#endif /* !YY_YY_PARSE_ASN1_TAB_H_INCLUDED  */
