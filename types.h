/* Tucker Meyers: CS403 Programming Language Project
 * Lexeme Types Module (types)
 */

#ifndef LEXEME_TYPES_HEADER
#define LEXEME_TYPES_HEADER

/* Types */
extern char * INTEGER;
extern char * FLOAT;
extern char * STRING;
extern char * ARRAY;
extern char * PAIR;
extern char * BOOLEAN;
extern char * IDENTIFIER;
extern char * BUILT_IN;
/* Punctuation */
extern char * LEFT_PARENTHESIS;
extern char * RIGHT_PARENTHESIS;
extern char * LEFT_BRACKET;
extern char * RIGHT_BRACKET;
extern char * LEFT_BRACE;
extern char * RIGHT_BRACE;
extern char * COLON;
/* Keywords */
extern char * KEYWORD_CLOSURE;
extern char * KEYWORD_WITH;
extern char * KEYWORD_FUNCTION;
extern char * KEYWORD_CLASS;
extern char * KEYWORD_BIND;
extern char * KEYWORD_REBIND;
extern char * KEYWORD_IF;
extern char * KEYWORD_ELSE;
extern char * KEYWORD_WHILE;
extern char * KEYWORD_FOR;
/* Special values */
extern int TRUE;
extern int FALSE;
//extern char * SELF;
extern char * NIL;
/* Parser-specific structural types */
extern char * EXPRESSION_LIST;
extern char * IDENTIFIER_LIST;
extern char * CALL;
extern char * CLOSURE;
extern char * BINDING;
extern char * REBINDING;
extern char * CONDITIONAL;
extern char * IF_CLAUSE;
extern char * ELSE_CLAUSE;
extern char * WHILE_LOOP;
//extern char * FOR_LOOP;
//extern char * FOR_LOOP_ITERATOR;
/* Environment-specific structural types */
extern char * ENVIRONMENT;
extern char * TABLE;
/* Other */
extern char * NONDESCRIPT;
extern char * UNRECOGNIZED;
extern char * END_OF_INPUT;

#endif

