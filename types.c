/* Tucker Meyers: CS403 Programming Language Project
 * Lexeme Types Modules (types)
 * These "types" are just constants that point to
 * unique string representations of the various 
 * lexeme types.
 */

#include "types.h"

/* Types */
char * INTEGER = "INTEGER";
char * FLOAT = "FLOATING-POINT NUMBER";
char * STRING = "STRING";
char * ARRAY = "LEXEME ARRAY";
char * PAIR = "PAIR";
char * BOOLEAN = "BOOLEAN";
char * IDENTIFIER = "IDENTIFIER";
char * BUILT_IN = "BUILT-IN OPERATION";
/* Punctuation */
char * LEFT_PARENTHESIS = "LEFT PARENTHESIS";
char * RIGHT_PARENTHESIS = "RIGHT PARENTHESIS";
char * LEFT_BRACKET = "LEFT BRACKET";
char * RIGHT_BRACKET = "RIGHT BRACKET";
char * LEFT_BRACE = "LEFT BRACE";
char * RIGHT_BRACE = "RIGHT BRACE";
char * COLON = "COLON";
/* Keywords */
char * KEYWORD_CLOSURE = "KEYWORD closure";
char * KEYWORD_WITH = "KEYWORD with";
char * KEYWORD_FUNCTION = "KEYWORD function";
char * KEYWORD_CLASS = "KEYWORD class";
char * KEYWORD_BIND = "KEYWORD bind";
char * KEYWORD_REBIND = "KEYWORD rebind";
char * KEYWORD_IF = "KEYWORD if";
char * KEYWORD_ELSE = "KEYWORD else";
char * KEYWORD_WHILE = "KEYWORD while";
char * KEYWORD_FOR = "KEYWORD for";
/* Special values */
int TRUE = 1;
int FALSE = 0;
//char * SELF = "LOCAL SCOPE";
char * NIL = "NULL VALUE";
/* Parser-specific structural types */
char * EXPRESSION_LIST = "TREE NODE expression list";
char * IDENTIFIER_LIST = "TREE NODE identifier list";
char * CALL = "TREE NODE function call";
char * CLOSURE = "CLOSURE";
char * BINDING = "TREE NODE variable binding";
char * REBINDING = "TREE NODE variable rebinding";
char * CONDITIONAL = "TREE NODE conditional statement";
char * IF_CLAUSE = "TREE NODE if clause";
char * ELSE_CLAUSE = "TREE NODE else clause";
char * WHILE_LOOP = "TREE NODE while loop";
//char * FOR_LOOP = "TREE NODE for loop";
//char * FOR_LOOP_ITERATOR = "TREE NODE for loop iterator";
/* Environment-specific structural types */
char * ENVIRONMENT = "ENVIRONMENT";
char * TABLE = "TREE NODE environment table";
/* Other */
char * NONDESCRIPT = "TYPELESS TREE NODE";
char * UNRECOGNIZED = "UNRECOGNIZED LEXEME TYPE";
char * END_OF_INPUT = "END OF INPUT";

