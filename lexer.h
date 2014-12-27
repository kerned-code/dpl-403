/* Tucker Meyers: CS403 Programming Language Project
 * Lexer Module
 */

#ifndef LEXER_HEADER
#define LEXER_HEADER

#include <stdio.h>
#include "lexeme.h"

typedef struct lexer {
    int currentLine;
    Lexeme * currentLexeme;
    FILE * sourceFile;
} Lexer;

extern Lexer * newLexer(FILE * fp);
extern void initializeLexer(Lexer * L);
extern Lexeme * lex(Lexer * L);

extern Lexeme * getCurrentLexeme(Lexer * L);
extern int getCurrentLineNumber(Lexer * L);
extern int check(Lexer * L, char * type);
extern Lexeme * match(Lexer * L, char * type);
extern void require(Lexer * L, char * type);  /* a.k.a "matchNoAdvance" */
extern void typeError(Lexer * L, char * expected);
extern void advance(Lexer * L);

#endif

