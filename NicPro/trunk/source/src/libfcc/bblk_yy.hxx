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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE bblk_lval;



