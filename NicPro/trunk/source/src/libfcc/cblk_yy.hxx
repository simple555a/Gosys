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
     cblk_TOK_NAT = 258,
     cblk_TOK_ID = 259,
     cblk_TOK_CBLOCK = 260,
     cblk_TOK_USES = 261,
     cblk_TOK_LINKS = 262,
     cblk_TOK_EXPORTS = 263,
     cblk_TOK_POINTER = 264,
     cblk_TOK_END = 265,
     cblk_TOK_UUID = 266,
     cblk_TOK_STRING = 267,
     cblk_TOK_COMMENTS = 268,
     SYM_BLOCK_DECLARATION = 269,
     SYM_LINK_DECLARATION = 270,
     SYM_PIN = 271,
     SYM_EXPORT_DECLARATION = 272
   };
#endif
/* Tokens.  */
#define cblk_TOK_NAT 258
#define cblk_TOK_ID 259
#define cblk_TOK_CBLOCK 260
#define cblk_TOK_USES 261
#define cblk_TOK_LINKS 262
#define cblk_TOK_EXPORTS 263
#define cblk_TOK_POINTER 264
#define cblk_TOK_END 265
#define cblk_TOK_UUID 266
#define cblk_TOK_STRING 267
#define cblk_TOK_COMMENTS 268
#define SYM_BLOCK_DECLARATION 269
#define SYM_LINK_DECLARATION 270
#define SYM_PIN 271
#define SYM_EXPORT_DECLARATION 272




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE cblk_lval;



