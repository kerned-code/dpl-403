/* Tucker Meyers: CS403 Programming Language Project
 * Built-In Types Module
 */

#ifndef BUILT_IN_TYPES_HEADER
#define BUILT_IN_TYPES_HEADER

/* Type operations */
extern char * BUILT_IN_TYPE;
extern char * BUILT_IN_TYPE_NIL;
extern char * BUILT_IN_TYPE_INTEGER;
extern char * BUILT_IN_TYPE_FLOAT;
extern char * BUILT_IN_TYPE_STRING;
extern char * BUILT_IN_TYPE_BOOLEAN;
extern char * BUILT_IN_TYPE_ARRAY;
extern char * BUILT_IN_TYPE_PAIR;
extern char * BUILT_IN_TYPE_IDENTIFIER;
extern char * BUILT_IN_TYPE_CLOSURE;
extern char * BUILT_IN_TYPE_ENVIRONMENT;
extern char * BUILT_IN_STRING_COERCION;

/* Scope operations */
extern char * BUILT_IN_GET;
extern char * BUILT_IN_SET;

/* Arithmetic operations */
extern char * BUILT_IN_ADDITION;
extern char * BUILT_IN_SUBTRACTION;
extern char * BUILT_IN_MULTIPLICATION;
extern char * BUILT_IN_DIVISION;
extern char * BUILT_IN_REMAINDER;
extern char * BUILT_IN_EXPONENTIATION;

/* Comparison operations */
extern char * BUILT_IN_EQUAL;
extern char * BUILT_IN_NOT_EQUAL;
extern char * BUILT_IN_GREATER_THAN;
extern char * BUILT_IN_LESS_THAN;
extern char * BUILT_IN_NOT_GREATER_THAN;
extern char * BUILT_IN_NOT_LESS_THAN;

/* Boolean operations */
extern char * BUILT_IN_NEGATION;
extern char * BUILT_IN_CONJUNCTION;
extern char * BUILT_IN_DISJUNCTION;

/* List operations */
extern char * BUILT_IN_LIST_PAIR;
extern char * BUILT_IN_LIST_LEFT_ACCESSOR;
extern char * BUILT_IN_LIST_RIGHT_ACCESSOR;
extern char * BUILT_IN_LIST_LEFT_MUTATOR;
extern char * BUILT_IN_LIST_RIGHT_MUTATOR;

/* String operations */
extern char * BUILT_IN_SINGLE_CHARACTER_ACCESS;
extern char * BUILT_IN_STRING_CONCATENATION;
extern char * BUILT_IN_STRING_COMPARISON;

/* Array operations */
extern char * BUILT_IN_ARRAY_CONSTRUCTOR;
extern char * BUILT_IN_ARRAY_ACCESSOR;
extern char * BUILT_IN_ARRAY_MUTATOR;

/* Input/output operations */
extern char * BUILT_IN_PRINT;
extern char * BUILT_IN_PRINT_BY_FORMAT;
extern char * BUILT_IN_PRINT_WITH_NEWLINE;
extern char * BUILT_IN_READ_FROM_TERMINAL;

#endif

