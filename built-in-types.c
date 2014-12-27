/* Tucker Meyers: CS403 Programming Language Project
 * Built-In Types Module
 */

#include "built-in-types.h"

/* Type operations */
char * BUILT_IN_TYPE = "FUNCTION type";
char * BUILT_IN_TYPE_NIL = "FUNCTION nil type predicate";
char * BUILT_IN_TYPE_INTEGER = "FUNCTION integer type predicate";
char * BUILT_IN_TYPE_FLOAT = "FUNCTION floating-point type predicate";
char * BUILT_IN_TYPE_STRING = "FUNCTION string type predicate";
char * BUILT_IN_TYPE_BOOLEAN = "FUNCTION Boolean type predicate";
char * BUILT_IN_TYPE_ARRAY = "FUNCTION array type predicate";
char * BUILT_IN_TYPE_PAIR = "FUNCTION pair type predicate";
char * BUILT_IN_TYPE_IDENTIFIER = "FUNCTION identifier type predicate";
char * BUILT_IN_TYPE_CLOSURE = "FUNCTION closure type predicate";
char * BUILT_IN_TYPE_ENVIRONMENT = "FUNCTION environment type predicate";
char * BUILT_IN_STRING_COERCION = "FUNCTION string";

/* Scope operations */
char * BUILT_IN_GET = "FUNCTION environment accessor";
char * BUILT_IN_SET = "FUNCTION environment mutator";

/* Arithmetic operations */
char * BUILT_IN_ADDITION = "FUNCTION addition";
char * BUILT_IN_SUBTRACTION = "FUNCTION subtraction";
char * BUILT_IN_MULTIPLICATION = "FUNCTION multiplication";
char * BUILT_IN_DIVISION = "FUNCTION division";
char * BUILT_IN_REMAINDER = "FUNCTION remainder";
char * BUILT_IN_EXPONENTIATION = "FUNCTION exponentiation";

/* Comparison operations */
char * BUILT_IN_EQUAL = "FUNCTION equal";
char * BUILT_IN_NOT_EQUAL = "FUNCTION not equal";
char * BUILT_IN_GREATER_THAN = "FUNCTION greater";
char * BUILT_IN_LESS_THAN = "FUNCTION less than";
char * BUILT_IN_NOT_GREATER_THAN = "FUNCTION not greater than";
char * BUILT_IN_NOT_LESS_THAN = "FUNCTION not less than";

/* Boolean operations */
char * BUILT_IN_NEGATION = "FUNCTION negation";
char * BUILT_IN_CONJUNCTION = "FUNCTION conjunction";
char * BUILT_IN_DISJUNCTION = "FUNCTION disjunction";

/* List operations */
char * BUILT_IN_LIST_PAIR = "FUNCTION pair constructor";
char * BUILT_IN_LIST_LEFT_ACCESSOR = "FUNCTION pair left accessor";
char * BUILT_IN_LIST_RIGHT_ACCESSOR = "FUNCTION pair right accessor";
char * BUILT_IN_LIST_LEFT_MUTATOR = "FUNCTION pair left mutator";
char * BUILT_IN_LIST_RIGHT_MUTATOR = "FUNCTION pair right mutator";

/* Array operations */
char * BUILT_IN_ARRAY_CONSTRUCTOR = "FUNCTION array constructor";
char * BUILT_IN_ARRAY_ACCESSOR = "FUNCTION array accessor";
char * BUILT_IN_ARRAY_MUTATOR = "FUNCTION array mutator";

/* String operations */
char * BUILT_IN_SINGLE_CHARACTER_ACCESS = "FUNCTION single-character accessor";
char * BUILT_IN_STRING_CONCATENATION = "FUNCTION string concatenator";
char * BUILT_IN_STRING_COMPARISON = "FUNCTION string comparison";

/* Input/output operations */
char * BUILT_IN_PRINT = "FUNCTION print";
char * BUILT_IN_PRINT_BY_FORMAT = "FUNCTION printf";
char * BUILT_IN_PRINT_WITH_NEWLINE = "FUNCTION println";
char * BUILT_IN_READ_FROM_TERMINAL = "FUNCTION read terminal input";

