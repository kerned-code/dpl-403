/* Tucker Meyers: CS403 Programming Language Project
 * Built-Ins Module
 * This module provides the source code (in C, of course)
 * for the built-in functions.
 *
 * Naming Conventions:
 *    op_   a closure for a built-in operation
 *  name_   a name bound to a closure
 *    ex_   a function to execute a built-in operation
 *  _pre    a name in prefix interleaved form (like 'string_')
 *  _in     a name in infix interleaved form (like '_=_')
 *  _ext    a name in extensible interleaved form (like '_+_&')
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "lexeme.h"
#include "trees.h"
#include "environment.h"
#include "built-in-types.h"
#include "built-ins.h"

static Lexeme * list_one_param(char * p1);
static Lexeme * list_two_params(char * p1, char * p2);
static Lexeme * list_three_params(char * p1, char * p2, char * p3);
static Lexeme * get_first_param(Lexeme * tree);
static Lexeme * get_second_param(Lexeme * tree);
static Lexeme * get_third_param(Lexeme * tree);

static int int_pow(int base, int index);
static double int_pow_reciprocal(int base, int index);

static Lexeme * lexeme_to_string_lexeme(Lexeme * x);
static char * lexeme_to_string(Lexeme * x);
static char * int_to_string(Lexeme * i);
static int count_digits(long n);
static char * float_to_string(Lexeme * x);
static char * Boolean_to_string(Lexeme * Boolean);
static char * array_to_string(Lexeme * array);
static char * list_to_string(Lexeme * list);
static char * closure_to_string(Lexeme * closure);
//static char * id_list_to_string(Lexeme * ids);


/*** Public Interface ***/

Lexeme * execute(Lexeme * built_in, Lexeme * env) {
    char * t = getIdentifierName(built_in);
    if (t == BUILT_IN_TYPE) 
        return ex_type(env);
    else if (t == BUILT_IN_TYPE_NIL)
        return ex_is_null(env);
    else if (t == BUILT_IN_TYPE_INTEGER)
        return ex_is_int(env);
    else if (t == BUILT_IN_TYPE_FLOAT)
        return ex_is_float(env);
    else if (t == BUILT_IN_TYPE_STRING)
        return ex_is_string(env);
    else if (t == BUILT_IN_TYPE_BOOLEAN)
        return ex_is_boolean(env);
    else if (t == BUILT_IN_TYPE_ARRAY)
        return ex_is_array(env);
    else if (t == BUILT_IN_TYPE_PAIR)
        return ex_is_pair(env);
    else if (t == BUILT_IN_TYPE_IDENTIFIER)
        return ex_is_identifier(env);
    else if (t == BUILT_IN_TYPE_CLOSURE)
        return ex_is_closure(env);
    else if (t == BUILT_IN_TYPE_ENVIRONMENT)
        return ex_is_scope(env);
    else if (t == BUILT_IN_STRING_COERCION)
        return ex_to_string(env);
    else if (t == BUILT_IN_GET) 
        return ex_get(env);
    else if (t == BUILT_IN_SET) 
        return ex_set(env);
    else if (t == BUILT_IN_ADDITION) 
        return ex_add(env);
    else if (t == BUILT_IN_SUBTRACTION) 
        return ex_sub(env);
    else if (t == BUILT_IN_MULTIPLICATION) 
        return ex_mult(env);
    else if (t == BUILT_IN_DIVISION) 
        return ex_div(env);
    else if (t == BUILT_IN_EXPONENTIATION) 
        return ex_power(env);
    else if (t == BUILT_IN_EQUAL) 
        return ex_equal(env);
    else if (t == BUILT_IN_NOT_EQUAL) 
        return ex_not_equal(env);
    else if (t == BUILT_IN_GREATER_THAN) 
        return ex_greater(env);
    else if (t == BUILT_IN_LESS_THAN) 
        return ex_less(env);
    else if (t == BUILT_IN_NOT_LESS_THAN) 
        return ex_not_less(env);
    else if (t == BUILT_IN_NOT_GREATER_THAN) 
        return ex_not_greater(env);
    else if (t == BUILT_IN_NEGATION) 
        return ex_not(env);
    else if (t == BUILT_IN_CONJUNCTION) 
        return ex_and(env);
    else if (t == BUILT_IN_DISJUNCTION) 
        return ex_or(env);
    else if (t == BUILT_IN_LIST_PAIR) 
        return ex_pair(env);
    else if (t == BUILT_IN_LIST_LEFT_ACCESSOR) 
        return ex_left(env);
    else if (t == BUILT_IN_LIST_RIGHT_ACCESSOR) 
        return ex_right(env);
    else if (t == BUILT_IN_LIST_LEFT_MUTATOR)
        return ex_set_left(env);
    else if (t == BUILT_IN_LIST_RIGHT_MUTATOR)
        return ex_set_right(env);
    else if (t == BUILT_IN_ARRAY_CONSTRUCTOR) 
        return ex_array(env);
    else if (t == BUILT_IN_ARRAY_ACCESSOR) 
        return ex_array_get(env);
    else if (t == BUILT_IN_ARRAY_MUTATOR) 
        return ex_array_set(env);
    else if (t == BUILT_IN_SINGLE_CHARACTER_ACCESS)
        return ex_char(env);
    else if (t == BUILT_IN_STRING_CONCATENATION)
        return ex_concat(env);
    else if (t == BUILT_IN_STRING_COMPARISON)
        return ex_compare(env);
    else if (t == BUILT_IN_PRINT) 
        return ex_print(env);
    else if (t == BUILT_IN_PRINT_WITH_NEWLINE) 
        return ex_println(env);
    else if (t == BUILT_IN_READ_FROM_TERMINAL) 
        return ex_read_terminal(env);
    else {
        fprintf(stderr, "<%s> is not a recognized built-in operation.\n", t);
        exit(-1);
    }
}

/* Type operations */

Lexeme *   op_type() {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE);}
Lexeme * name_type() {return named_built_in_tree("type", op_type());}
Lexeme *   ex_type(Lexeme * env) {
    printf("look up...\n");
    printf("type %s\n", getType(get_first_param(op_type())));
    Lexeme * x = look_up(env, get_first_param(op_type()));
    printf("not here\n");
    return newStringValueLexeme(getType(x));
}

Lexeme *   op_is_null    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_NIL);}
Lexeme * name_is_null    () {return named_built_in_tree("null?", op_is_null());}
Lexeme * name_is_null_pre() {return named_built_in_tree("null?_", op_is_null());}
Lexeme *   ex_is_null(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_null()));
    if (getType(x) == NIL)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_int    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_INTEGER);}
Lexeme * name_is_int    () {return named_built_in_tree("int?", op_is_int());}
Lexeme * name_is_int_pre() {return named_built_in_tree("int?_", op_is_int());}
Lexeme *   ex_is_int(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_int()));
    if (getType(x) == INTEGER)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_float    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_FLOAT);}
Lexeme * name_is_float    () {return named_built_in_tree("float?", op_is_float());}
Lexeme * name_is_float_pre() {return named_built_in_tree("float?_", op_is_float());}
Lexeme *   ex_is_float(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_float()));
    if (getType(x) == FLOAT)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_string    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_STRING);}
Lexeme * name_is_string    () {return named_built_in_tree("string?", op_is_string());}
Lexeme * name_is_string_pre() {return named_built_in_tree("string?_", op_is_string());}
Lexeme *   ex_is_string(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_string()));
    if (getType(x) == STRING)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_boolean    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_BOOLEAN);}
Lexeme * name_is_boolean    () {return named_built_in_tree("boolean?", op_is_boolean());}
Lexeme * name_is_boolean_pre() {return named_built_in_tree("boolean?_", op_is_boolean());}
Lexeme *   ex_is_boolean(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_boolean()));
    if (getType(x) == BOOLEAN)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_array    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_ARRAY);}
Lexeme * name_is_array    () {return named_built_in_tree("array?", op_is_array());}
Lexeme * name_is_array_pre() {return named_built_in_tree("array?_", op_is_array());}
Lexeme *   ex_is_array(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_array()));
    if (getType(x) == ARRAY)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_pair    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_PAIR);}
Lexeme * name_is_pair    () {return named_built_in_tree("pair?", op_is_pair());}
Lexeme * name_is_pair_pre() {return named_built_in_tree("pair?_", op_is_pair());}
Lexeme * name_is_list    () {return named_built_in_tree("list?", op_is_pair());}
Lexeme * name_is_list_pre() {return named_built_in_tree("list?_", op_is_pair());}
Lexeme *   ex_is_pair(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_pair()));
    if (getType(x) == PAIR)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_identifier    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_IDENTIFIER);}
Lexeme * name_is_identifier    () {return named_built_in_tree("identifier?", op_is_identifier());}
Lexeme * name_is_identifier_pre() {return named_built_in_tree("identifier?_", op_is_identifier());}
Lexeme *   ex_is_identifier(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_identifier()));
    if (getType(x) == IDENTIFIER)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_closure    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_CLOSURE);}
Lexeme * name_is_closure    () {return named_built_in_tree("closure?", op_is_closure());}
Lexeme * name_is_closure_pre() {return named_built_in_tree("closure?_", op_is_closure());}
Lexeme *   ex_is_closure(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_closure()));
    if (getType(x) == CLOSURE)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_is_scope    () {return built_in_tree(list_one_param("x"), BUILT_IN_TYPE_ENVIRONMENT);}
Lexeme * name_is_scope    () {return named_built_in_tree("scope?", op_is_scope());}
Lexeme * name_is_scope_pre() {return named_built_in_tree("scope?_", op_is_scope());}
Lexeme *   ex_is_scope(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_is_scope()));
    if (getType(x) == ENVIRONMENT)
        return newBooleanValueLexeme(TRUE);
    else 
        return newBooleanValueLexeme(FALSE);
}

Lexeme *   op_to_string() {return built_in_tree(list_one_param("x"), BUILT_IN_STRING_COERCION);}
Lexeme * name_to_string() {return named_built_in_tree("string", op_to_string());}
Lexeme *   ex_to_string(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_to_string()));
    return lexeme_to_string_lexeme(x);
}

/* Scope operations */

Lexeme *   op_get    () {return built_in_tree(list_two_params("scope", "name"), BUILT_IN_GET);}
Lexeme * name_get    () {return named_built_in_tree("get",  op_get());}
Lexeme * name_get_dot() {return named_built_in_tree("_._&", op_get());}
Lexeme *   ex_get(Lexeme * env) {
    Lexeme * scope = look_up(env, get_first_param(op_get()));
    Lexeme * name = look_up(env, get_second_param(op_get()));
    Lexeme * id = newIdentifierLexeme(getStringValue(name));
    Lexeme * value = look_up_quietly(scope, id);
    if (value == NULL)
        return newLexeme(NIL);
    else
        return value;
}

Lexeme *   op_set    () {return built_in_tree(list_three_params("scope", "name", "new-value"), BUILT_IN_SET);}
Lexeme * name_set    () {return named_built_in_tree("set", op_set());}
Lexeme * name_set_dot() {return named_built_in_tree("_._set_", op_set());}
Lexeme *   ex_set(Lexeme * env) {
    Lexeme * scope = look_up(env, get_first_param(op_set()));
    Lexeme * name = look_up(env, get_second_param(op_set()));
    Lexeme * id = newIdentifierLexeme(getStringValue(name));
    Lexeme * value = look_up(env, get_third_param(op_set()));
    change_binding(scope, id, value);
    return value;
}

/* Arithmetic operations */

Lexeme *   op_add    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_ADDITION);}
Lexeme * name_add    () {return named_built_in_tree("+",    op_add());}
Lexeme * name_add_ext() {return named_built_in_tree("_+_&", op_add());}
Lexeme *   ex_add(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_add()));
    Lexeme * b = look_up(env, get_second_param(op_add()));
    if (typeInteger(a) && typeInteger(b))
        return newIntegerValueLexeme(getIntegerValue(a) + getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newFloatValueLexeme(getIntegerValue(a) + getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newFloatValueLexeme(getFloatValue(a) + getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newFloatValueLexeme(getFloatValue(a) + getFloatValue(b));
    else {
        fprintf(stderr, "Addition error: Cannot add types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_sub    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_SUBTRACTION);}
Lexeme * name_sub    () {return named_built_in_tree("-",    op_sub());}
Lexeme * name_sub_ext() {return named_built_in_tree("_-_&", op_sub());}
Lexeme *   ex_sub(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_sub()));
    Lexeme * b = look_up(env, get_second_param(op_sub()));
    if (typeInteger(a) && typeInteger(b))
        return newIntegerValueLexeme(getIntegerValue(a) - getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newFloatValueLexeme(getIntegerValue(a) - getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newFloatValueLexeme(getFloatValue(a) - getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newFloatValueLexeme(getFloatValue(a) - getFloatValue(b));
    else {
        fprintf(stderr, "Subtraction error: Cannot subtract types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_mult    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_MULTIPLICATION);}
Lexeme * name_mult    () {return named_built_in_tree("*",    op_mult());}
Lexeme * name_mult_ext() {return named_built_in_tree("_*_&", op_mult());}
Lexeme *   ex_mult(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_mult()));
    Lexeme * b = look_up(env, get_second_param(op_mult()));
    if (typeInteger(a) && typeInteger(b))
        return newIntegerValueLexeme(getIntegerValue(a) * getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newFloatValueLexeme(getIntegerValue(a) * getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newFloatValueLexeme(getFloatValue(a) * getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newFloatValueLexeme(getFloatValue(a) * getFloatValue(b));
    else {
        fprintf(stderr, "Multiplication error: Cannot multiply types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_div    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_DIVISION);}
Lexeme * name_div    () {return named_built_in_tree("/",    op_div());}
Lexeme * name_div_ext() {return named_built_in_tree("_/_&", op_div());}
Lexeme *   ex_div(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_div()));
    Lexeme * b = look_up(env, get_second_param(op_div()));
    if (typeInteger(a) && typeInteger(b))
        return newIntegerValueLexeme(getIntegerValue(a) / getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newFloatValueLexeme(getIntegerValue(a) / getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newFloatValueLexeme(getFloatValue(a) / getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newFloatValueLexeme(getFloatValue(a) / getFloatValue(b));
    else {
        fprintf(stderr, "Division error: Cannot divide types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_power    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_EXPONENTIATION);}
Lexeme * name_power    () {return named_built_in_tree("^",    op_power());}
Lexeme * name_power_ext() {return named_built_in_tree("_^_&", op_power());}
Lexeme *   ex_power(Lexeme * env) {
    Lexeme * base = look_up(env, get_first_param(op_power()));
    Lexeme * exp = look_up(env, get_second_param(op_power()));
    if (typeInteger(base) && typeInteger(exp)) {
        // Integer powers are complicated. If the exponent is negative, use floats
        // to avoid complete loss of precision. Otherwise, just use integers.
        int b = getIntegerValue(base);
        int e = getIntegerValue(exp);
        if (e < 0)
            return newFloatValueLexeme(int_pow_reciprocal(b, e));
        else
            return newIntegerValueLexeme(int_pow(b, e));
    }
    else if (typeInteger(base) && typeFloat(exp))
        return newFloatValueLexeme(pow((double) getIntegerValue(base), getFloatValue(exp)));
    else if (typeFloat(base) && typeInteger(exp))
        return newFloatValueLexeme(pow(getFloatValue(base), (double) getIntegerValue(exp)));
    else if (typeFloat(base) && typeFloat(exp))
        return newFloatValueLexeme(pow(getFloatValue(base), getFloatValue(exp)));
    else {
        fprintf(stderr, "Exponentiation error: Cannot raise types <%s> to power of type <%s>.\n", 
            getType(base), getType(exp));
        exit(-1);
    }
}

/* Comparison operations */

Lexeme *   op_equal   () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_EQUAL); }
Lexeme * name_equal   () {return named_built_in_tree("=", op_equal());}
Lexeme * name_equal_in() {return named_built_in_tree("_=_", op_equal());}
Lexeme *   ex_equal(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_equal()));
    Lexeme * b = look_up(env, get_second_param(op_equal()));
    if (typeInteger(a) && typeInteger(b))
        return newBooleanValueLexeme(getIntegerValue(a) == getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newBooleanValueLexeme((double) getIntegerValue(a) == getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newBooleanValueLexeme(getFloatValue(a) == (double) getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newBooleanValueLexeme(getFloatValue(a) == getFloatValue(b));
    else {
        fprintf(stderr, "Comparison error: Cannot compare == types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_not_equal   () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_NOT_EQUAL);}
Lexeme * name_not_equal   () {return named_built_in_tree("!=", op_not_equal());}
Lexeme * name_not_equal_in() {return named_built_in_tree("_!=_", op_not_equal());}
Lexeme *   ex_not_equal(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_not_equal()));
    Lexeme * b = look_up(env, get_second_param(op_not_equal()));
    if (typeInteger(a) && typeInteger(b))
        return newBooleanValueLexeme(getIntegerValue(a) != getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newBooleanValueLexeme((double) getIntegerValue(a) != getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newBooleanValueLexeme(getFloatValue(a) != (double) getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newBooleanValueLexeme(getFloatValue(a) != getFloatValue(b));
    else {
        fprintf(stderr, "Comparison error: Cannot compare != types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_greater   () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_GREATER_THAN);}
Lexeme * name_greater   () {return named_built_in_tree(">", op_greater());}
Lexeme * name_greater_in() {return named_built_in_tree("_>_", op_greater());}
Lexeme *   ex_greater(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_greater()));
    Lexeme * b = look_up(env, get_second_param(op_greater()));
    if (typeInteger(a) && typeInteger(b))
        return newBooleanValueLexeme(getIntegerValue(a) > getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newBooleanValueLexeme((double) getIntegerValue(a) > getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newBooleanValueLexeme(getFloatValue(a) > (double) getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newBooleanValueLexeme(getFloatValue(a) > getFloatValue(b));
    else {
        fprintf(stderr, "Comparison error: Cannot compare > types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_less   () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_LESS_THAN);}
Lexeme * name_less   () {return named_built_in_tree("<", op_less());}
Lexeme * name_less_in() {return named_built_in_tree("_<_", op_less());}
Lexeme *   ex_less(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_less()));
    Lexeme * b = look_up(env, get_second_param(op_less()));
    if (typeInteger(a) && typeInteger(b))
        return newBooleanValueLexeme(getIntegerValue(a) < getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newBooleanValueLexeme((double) getIntegerValue(a) < getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newBooleanValueLexeme(getFloatValue(a) < (double) getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newBooleanValueLexeme(getFloatValue(a) < getFloatValue(b));
    else {
        fprintf(stderr, "Comparison error: Cannot compare < types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_not_less   () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_NOT_GREATER_THAN);}
Lexeme * name_not_less   () {return named_built_in_tree("<=", op_not_less());}
Lexeme * name_not_less_in() {return named_built_in_tree("_<=_", op_not_less());}
Lexeme *   ex_not_less(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_not_less()));
    Lexeme * b = look_up(env, get_second_param(op_not_less()));
    if (typeInteger(a) && typeInteger(b))
        return newBooleanValueLexeme(getIntegerValue(a) >= getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newBooleanValueLexeme((double) getIntegerValue(a) >= getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newBooleanValueLexeme(getFloatValue(a) >= (double) getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newBooleanValueLexeme(getFloatValue(a) >= getFloatValue(b));
    else {
        fprintf(stderr, "Comparison error: Cannot compare >= types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

Lexeme *   op_not_greater   () {return built_in_tree(list_one_param("a"), BUILT_IN_NOT_LESS_THAN);}
Lexeme * name_not_greater   () {return named_built_in_tree(">=", op_not_greater());}
Lexeme * name_not_greater_in() {return named_built_in_tree("_>=_", op_not_greater());}
Lexeme *   ex_not_greater(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_not_greater()));
    Lexeme * b = look_up(env, get_second_param(op_not_greater()));
    if (typeInteger(a) && typeInteger(b))
        return newBooleanValueLexeme(getIntegerValue(a) <= getIntegerValue(b));
    else if (typeInteger(a) && typeFloat(b))
        return newBooleanValueLexeme((double) getIntegerValue(a) <= getFloatValue(b));
    else if (typeFloat(a) && typeInteger(b))
        return newBooleanValueLexeme(getFloatValue(a) <= (double) getIntegerValue(b));
    else if (typeFloat(a) && typeFloat(b))
        return newBooleanValueLexeme(getFloatValue(a) <= getFloatValue(b));
    else {
        fprintf(stderr, "Comparison error: Cannot compare <= types <%s> and <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
}

/* Boolean operations */

Lexeme *   op_not    () {return built_in_tree(list_one_param("x"), BUILT_IN_NEGATION);}
Lexeme * name_not    () {return named_built_in_tree("not", op_not());}
Lexeme * name_not_pre() {return named_built_in_tree("not_", op_not());}
Lexeme *   ex_not(Lexeme * env) {
    Lexeme * x = look_up(env, get_first_param(op_not()));
    if (! typeBoolean(x)) {
        fprintf(stderr, "Boolean error: Cannot negate item of type <%s>.\n", getType(x));
        exit(-1);
    }
    int b = (getBooleanValue(x) == 0);
    return newBooleanValueLexeme(b);
}

Lexeme *   op_and    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_CONJUNCTION);}
Lexeme * name_and    () {return named_built_in_tree("and", op_and());}
Lexeme * name_and_ext() {return named_built_in_tree("_and_&", op_and());}
Lexeme *   ex_and(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_and()));
    Lexeme * b = look_up(env, get_second_param(op_and()));
    if (! typeBoolean(a) || ! typeBoolean(b)) {
        fprintf(stderr, "Boolean error: Cannot conjoin items of types <%s>, <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
    int value = getBooleanValue(a) && getBooleanValue(b);
    return newBooleanValueLexeme(value);
}

Lexeme *   op_or    () {return built_in_tree(list_two_params("a", "b"), BUILT_IN_DISJUNCTION);}
Lexeme * name_or    () {return named_built_in_tree("or", op_or());}
Lexeme * name_or_ext() {return named_built_in_tree("_or_&", op_or());}
Lexeme *   ex_or(Lexeme * env) {
    Lexeme * a = look_up(env, get_first_param(op_or()));
    Lexeme * b = look_up(env, get_second_param(op_or()));
    if (! typeBoolean(a) || ! typeBoolean(b)) {
        fprintf(stderr, "Boolean error: Cannot disjoin items of types <%s>, <%s>.\n", getType(a), getType(b));
        exit(-1);
    }
    int value = getBooleanValue(a) || getBooleanValue(b);
    return newBooleanValueLexeme(value);
}

/* List operations */

Lexeme *   op_pair      () {return built_in_tree(list_two_params("l", "r"), BUILT_IN_LIST_PAIR);}
Lexeme * name_pair      () {return named_built_in_tree("pair", op_pair());}
Lexeme * name_pair_comma() {return named_built_in_tree("_,_", op_pair());}
Lexeme *   ex_pair(Lexeme * env) {
    Lexeme * l = look_up(env, get_first_param(op_pair()));
    Lexeme * r = look_up(env, get_second_param(op_pair()));
    return consWithTag(PAIR, l, r);
}

Lexeme *   op_left() {return built_in_tree(list_one_param("p"), BUILT_IN_LIST_LEFT_ACCESSOR);}
Lexeme * name_left() {return named_built_in_tree("left", op_left());}
Lexeme *   ex_left(Lexeme * env) {
    Lexeme * p = look_up(env, get_first_param(op_left()));
    if (! typePair(p)) {
        fprintf(stderr, "List error: Cannot get left member of type <%s>.\n", getType(p));
        exit(-1);
    }
    return car(p);
}

Lexeme *   op_right() {return built_in_tree(list_one_param("p"), BUILT_IN_LIST_RIGHT_ACCESSOR);}
Lexeme * name_right() {return named_built_in_tree("right", op_right());}
Lexeme *   ex_right(Lexeme * env) {
    Lexeme * p = look_up(env, get_first_param(op_right()));
    if (! typePair(p)) {
        fprintf(stderr, "List error: Cannot get right member of type <%s>.\n", getType(p));
        exit(-1);
    }
    return cdr(p);
}

Lexeme *   op_set_left() {return built_in_tree(list_two_params("p", "l"), BUILT_IN_LIST_LEFT_MUTATOR);}
Lexeme * name_set_left() {return named_built_in_tree("set-left", op_set_left());}
Lexeme *   ex_set_left(Lexeme * env) {
    Lexeme * p = look_up(env, get_first_param(op_set_left()));
    Lexeme * l = look_up(env, get_second_param(op_set_left()));
    if (! typePair(p)) {
        fprintf(stderr, "List error: Cannot get left member of type <%s>.\n", getType(p));
        exit(-1);
    }
    setCar(p, l);
    return p;
}

Lexeme *   op_set_right() {return built_in_tree(list_two_params("p", "r"), BUILT_IN_LIST_RIGHT_MUTATOR);}
Lexeme * name_set_right() {return named_built_in_tree("set-right", op_set_right());}
Lexeme *   ex_set_right(Lexeme * env) {
    Lexeme * p = look_up(env, get_first_param(op_set_right()));
    Lexeme * r = look_up(env, get_second_param(op_set_right()));
    if (! typePair(p)) {
        fprintf(stderr, "List error: Cannot get right member of type <%s>.\n", getType(p));
        exit(-1);
    }
    setCdr(p, r);
    return p;
}

/* Array operations */

Lexeme *   op_array() {return built_in_tree(list_one_param("length"), BUILT_IN_ARRAY_CONSTRUCTOR);}
Lexeme * name_array() {return named_built_in_tree("array", op_array());}
Lexeme *   ex_array(Lexeme * env) {
    Lexeme * length = look_up(env, get_first_param(op_array()));
    if (! typeInteger(length)) {
        fprintf(stderr, "Array error: The length must have type integer, not <%s>.\n", getType(length));
        exit(-1);
    }
    return newArrayLexeme(getIntegerValue(length));
}

Lexeme *   op_array_get() {return built_in_tree(list_two_params("arr", "index"), BUILT_IN_ARRAY_ACCESSOR);}
Lexeme * name_array_get() {return named_built_in_tree("array-get", op_array_get());}
Lexeme *   ex_array_get(Lexeme * env) {
    Lexeme * arr = look_up(env, get_first_param(op_array_get()));
    Lexeme * index = look_up(env, get_second_param(op_array_get()));
    if (! typeArray(arr)) {
        fprintf(stderr, "Array access error: The first argument must be an array, not <%s>.\n", getType(arr));
        exit(-1);
    }
    if (! typeInteger(index)) {
        fprintf(stderr, "Array access error: The index must have type integer, not <%s>.\n", getType(index));
        exit(-1);
    }
    return getArrayValue(arr, getIntegerValue(index));
}

Lexeme *   op_array_set() {return built_in_tree(list_three_params("arr", "index", "value"), BUILT_IN_ARRAY_MUTATOR);}
Lexeme * name_array_set() {return named_built_in_tree("array-set", op_array_set());}
Lexeme *   ex_array_set(Lexeme * env) {
    Lexeme * arr = look_up(env, get_first_param(op_array_set()));
    Lexeme * index = look_up(env, get_second_param(op_array_set()));
    Lexeme * value = look_up(env, get_third_param(op_array_set()));
    if (! typeArray(arr)) {
        fprintf(stderr, "Array access error: The first argument must be an array, not <%s>.\n", getType(arr));
        exit(-1);
    }
    if (! typeInteger(index)) {
        fprintf(stderr, "Array access error: The index must have type integer, not <%s>.\n", getType(index));
        exit(-1);
    }
    return setArrayValue(arr, getIntegerValue(index), value);
}

/* String operations */

Lexeme *   op_char() {return built_in_tree(list_two_params("source", "index"), BUILT_IN_SINGLE_CHARACTER_ACCESS);}
Lexeme * name_char() {return named_built_in_tree("char", op_char());}
Lexeme *   ex_char(Lexeme * env) {
    Lexeme * source = look_up(env, get_first_param(op_char()));
    Lexeme * index = look_up(env, get_second_param(op_char()));
    int l = getStringLength(source);
    int i = getIntegerValue(index);
    if (i >= l) {
        fprintf(stderr, "Character access error: Index %d not inside string (length %d).\n", i, l);
        exit(-1);
    }
    char * string = getStringValue(source);
    char c[1];
    c[0] = string[i];
    return newStringValueLexeme(c);
}

Lexeme *   op_concat() {return built_in_tree(list_two_params("left", "right"), BUILT_IN_STRING_CONCATENATION);}
Lexeme * name_concat() {return named_built_in_tree("concat", op_concat());}
Lexeme *   ex_concat(Lexeme * env) {
    Lexeme * l = look_up(env, get_first_param(op_char()));
    Lexeme * r = look_up(env, get_second_param(op_char()));
    int left_length = getStringLength(l);
    int right_length = getStringLength(r);
    int length = left_length + right_length;
    char * left = getStringValue(l);
    char * right = getStringValue(r);
    char result[length];
    for (int i = 0; i < left_length; i++) {
        result[i] = left[i];
    }
    for (int j = 0; j < right_length; j++) {
        result[j + left_length] = right[j];
    }
    result[length] = '\0';
    return newStringValueLexeme(result);
}

Lexeme *   op_compare() {return built_in_tree(list_two_params("left", "right"), BUILT_IN_STRING_COMPARISON);}
Lexeme * name_compare() {return named_built_in_tree("compare", op_compare());}
Lexeme *   ex_compare(Lexeme * env) {
    Lexeme * l = look_up(env, get_first_param(op_char()));
    Lexeme * r = look_up(env, get_second_param(op_char()));
    int left_length = getStringLength(l);
    int right_length = getStringLength(r);
    if (left_length != right_length)
        return newBooleanValueLexeme(FALSE);
    char * left = getStringValue(l);
    char * right = getStringValue(r);
    for (int i = 0; i < left_length; i++) {
        if (left[i] != right[i])
            return newBooleanValueLexeme(FALSE);
    }
    return newBooleanValueLexeme(TRUE);
}

/* Input/output operations */

Lexeme *   op_print() {return built_in_tree(list_one_param("message"), BUILT_IN_PRINT);}
Lexeme * name_print() {return named_built_in_tree("print", op_print());}
Lexeme *   ex_print(Lexeme * env) {
    Lexeme * message = look_up(env, get_first_param(op_print()));
    char * m = lexeme_to_string(message);
    fprintf(stdout, "%s", m);
    return message;
}

Lexeme *   op_println() {return built_in_tree(list_one_param("message"), BUILT_IN_PRINT_WITH_NEWLINE);}
Lexeme * name_println() {return named_built_in_tree("println", op_println());}
Lexeme *   ex_println(Lexeme * env) {
    Lexeme * message = look_up(env, get_first_param(op_println()));
    char * m = lexeme_to_string(message);
    fprintf(stdout, "%s\n", m);
    return message;
}

Lexeme *   op_read_terminal() {return built_in_tree(list_one_param("num-chars"), BUILT_IN_READ_FROM_TERMINAL);}
Lexeme * name_read_terminal() {return named_built_in_tree("read-terminal", op_read_terminal());}
Lexeme *   ex_read_terminal(Lexeme * env) {
    Lexeme * num_chars = look_up(env, get_first_param(op_read_terminal()));
    int n;
    if(! typeInteger(num_chars)) {
        fprintf(stderr, "Reading error: The number of characters to read must be an integer.\n");
        exit(-1);
    }
    n = getIntegerValue(num_chars);
    if (n > 512 || n < 1) {
        fprintf(stderr, "Reading error: Can only read strings of 1 to 512 characters.\n");
        exit(-1);
    }
    char buffer[n+1];
    for (int i = 0; i < n; i++) {
        buffer[i] = fgetc(stdin);
    }
    buffer[n] = '\0';
    return newStringValueLexeme(buffer);
}



/*** Private Functions ***/

static Lexeme * list_one_param(char * p1) {
    return identifier_list_tree(newIdentifierLexeme(p1), NULL);
}
static Lexeme * list_two_params(char * p1, char * p2) {
    return identifier_list_tree(newIdentifierLexeme(p1), list_one_param(p2));
}
static Lexeme * list_three_params(char * p1, char * p2, char * p3) {
    return identifier_list_tree(newIdentifierLexeme(p1), list_two_params(p2, p3));
}

static Lexeme * get_first_param(Lexeme * tree) {
    return car(get_built_in_parameters(tree));
}
static Lexeme * get_second_param(Lexeme * tree) {
    return car(cdr(get_built_in_parameters(tree)));
}
static Lexeme * get_third_param(Lexeme * tree) {
    return car(cdr(cdr(get_built_in_parameters(tree))));
}

static int int_pow(int base, int index) {
    if (index < 0) {
        fprintf(stderr, "Warning: int_pow with a negative index gives inaccurate results.");
    }
    int result = 1;
    while (index > 0) {
        // Odd indices are reduced by one more than half.
        // Store the extra factor in the result.
        if (index % 2 == 1) result *= base;
        // Halving the index corresponds to squaring the base.
        base *= base;
        index /= 2;
    }
    return result;
}

static double int_pow_reciprocal(int base, int index) {
    double magnitude = (double) int_pow(base, abs(index));
    if (index < 0) 
        return (1 / magnitude);
    else
        return magnitude;
}

static char * int_to_string(Lexeme * i) {
    long n = getIntegerValue(i);
    int length = sizeof(char) * count_digits(n);
    char * s = malloc(length);
    sprintf(s, "%ld", n);
    return s;
}

static int count_digits(long n) {
    // Since integers are limited to nine digits, a naive approach is safe.
    if (n > -1          && n < 10        ) return 1;
    if (n > -10         && n < 100       ) return 2;
    if (n > -100        && n < 1000      ) return 3;
    if (n > -1000       && n < 10000     ) return 4;
    if (n > -10000      && n < 100000    ) return 5;
    if (n > -100000     && n < 1000000   ) return 6;
    if (n > -1000000    && n < 10000000  ) return 7;
    if (n > -10000000   && n < 100000000 ) return 8;
    if (n > -100000000  && n < 1000000000) return 9;
    else return 10;
}

static Lexeme * lexeme_to_string_lexeme(Lexeme * x) {
    return newStringValueLexeme(lexeme_to_string(x));
}

static char * lexeme_to_string(Lexeme * x) {
    if (typeString(x))
        return getStringValue(x);
    else if (typeInteger(x))
        return int_to_string(x);
    else if (typeFloat(x))
        return float_to_string(x);
    else if (typeBoolean(x))
        return Boolean_to_string(x);
    else if (typeArray(x))
        return array_to_string(x);
    else if (typePair(x))
        return list_to_string(x);
    else if (typeIdentifier(x))
        return getIdentifierName(x);
    else if (getType(x) == CLOSURE)
        return closure_to_string(x);
    else if (getType(x) == NIL)
        return "nil";
    else {
        fprintf(stderr, "Coercion error: Objects of type <%s> cannot be turned into strings.\n", getType(x));
        exit(-1);
    }
}

static char * float_to_string(Lexeme * x) {
    // For simplicity, always print floating-point numbers
    // with sixteen digits.
    char * s = malloc(18);  // Include space for decimal point and \0.
    sprintf(s, "%16.15f", getFloatValue(x));
    return s;
}

static char * Boolean_to_string(Lexeme * Boolean) {
    int b = getBooleanValue(Boolean);
    if (b)
        return "true";
    else
        return "false";
}

static char * array_to_string(Lexeme * array) {
    return "<an array>";
    /*
    Lexeme * a = getArray(array);
    char * front = "[";
    int length_front = 1;
    char * back = "]";
    int length_back = 1;
    
    char * s = malloc(
    */
}

static char * list_to_string(Lexeme * list) {
    char * left = lexeme_to_string(car(list));
    char * right = lexeme_to_string(cdr(list));
    int lenL = strlen(left);
    int lenR = strlen(right);
    char * s = malloc(1 + lenL + 2 + lenR + 2);  // paren, left, comma, space, right, paren, \0
    s[0] = '(';
    for (int i = 0; i < lenL; i++) {
        s[i + 1] = left[i];
    }
    s[lenL + 1] = ',';
    s[lenL + 2] = ' ';
    for (int j = 0; j < lenR; j++) {
        s[j + lenL + 3] = right[j];
    }
    int back = lenL + 3 + lenR;
    s[back] = ')';
    s[back + 1] = '\0';
    return s;
}

static char * closure_to_string(Lexeme * closure) {
    return "<a closure>";
    /*
    char * front = "(closure:";
    int length_front = 9;
    char * back = ")";
    int length_back = 1;
    char * params = id_list_to_string(get_closure_parameters(closure));
    int length_params = strlen(params);
    char * s = malloc(length_front + length_params + length_back + 1);  // front, params, back, \0
    // Copy front.
    for (int i = 0; i < length_front; i++) {
        s[i] = front[i];
    }
    // Copy params.
    for (int i = 0; i < length_params; i++) {
        s[i+length_front] = params[i];
    }
    // Copy back.
    int length_both = length_front + length_params;
    for (int i = 0; i < length_back; i++) {
        s[i + length_both] = back[i];
    }
    s[length_both + length_back] = '\0';
    return s;
    */
}

/*
static char * id_list_to_string(Lexeme * ids) {
    if (ids == NULL) return "";
    char * first = getIdentifierName(car(ids));
    int length_first = strlen(first);
    char * rest = id_list_to_string(cdr(ids));
    int length_rest = strlen(rest);
    char * s = malloc(1 + length_first + length_rest);  // space, first, rest, \0
    // Start with a space.
    s[0] = ' ';
    // Copy the first name.
    for (int i = 0; i < length_first; i++) {
        s[i + 1] = first[i];
    }
    // Copy the rest.
    for (int j = 0; j < length_rest; j++) {
        s[j + 1 + length_first] = rest[j];
    }
    s[1 + length_first + length_rest] = '\0';
    return s;
}
*/

