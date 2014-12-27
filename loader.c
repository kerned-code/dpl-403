/* Tucker Meyers: CS403 Programming Language Project
 * Loader Module
 * This module provides functions for loading built-in
 * functions and other such things into environments.
 */

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexeme.h"
#include "trees.h"
#include "environment.h"
#include "built-in-types.h"
#include "built-ins.h"
#include "loader.h"

static void load_type_operations(Lexeme * env);
static void load_scope_operations(Lexeme * env);
static void load_arithmetic_operations(Lexeme * env);
static void load_comparison_operations(Lexeme * env);
static void load_Boolean_operations(Lexeme * env);
static void load_list_operations(Lexeme * env);
static void load_array_operations(Lexeme * env);
static void load_string_operations(Lexeme * env);
static void load_input_output_operations(Lexeme * env);
static void load(Lexeme * env, Lexeme * built_in);


/*** Public Interface ***/

void load_built_in_names(Lexeme * env) {
    /* Add the built-in function bindings to the given environment table. */
    load_type_operations(env);
    load_scope_operations(env);
    load_arithmetic_operations(env);
    load_comparison_operations(env);
    load_Boolean_operations(env);
    load_list_operations(env);
    load_array_operations(env);
    load_string_operations(env);
    load_input_output_operations(env);
}

void load_local_names(Lexeme * env) {
    /* Add scope-related variables to the given environment table. */
    add_binding(env,
        newIdentifierLexeme("@"),
        env
    );
}


/*** Private Functions ***/

static void load_type_operations(Lexeme * env) {
    load(env, name_type());
    load(env, name_is_null());
    load(env, name_is_null_pre());
    load(env, name_is_int());
    load(env, name_is_int_pre());
    load(env, name_is_float());
    load(env, name_is_float_pre());
    load(env, name_is_string());
    load(env, name_is_string_pre());
    load(env, name_is_boolean());
    load(env, name_is_boolean_pre());
    load(env, name_is_array());
    load(env, name_is_array_pre());
    load(env, name_is_pair());
    load(env, name_is_pair_pre());
    load(env, name_is_list());
    load(env, name_is_list_pre());
    load(env, name_is_identifier());
    load(env, name_is_identifier_pre());
    load(env, name_is_closure());
    load(env, name_is_closure_pre());
    load(env, name_is_scope());
    load(env, name_is_scope_pre());
    load(env, name_to_string());
}

static void load_scope_operations(Lexeme * env) {
    load(env, name_get());
    load(env, name_get_dot());
    load(env, name_set());
    load(env, name_set_dot());
}

static void load_arithmetic_operations(Lexeme * env) {
    load(env, name_add());
    load(env, name_add_ext());
    load(env, name_sub());
    load(env, name_sub_ext());
    load(env, name_mult());
    load(env, name_mult_ext());
    load(env, name_div());
    load(env, name_div_ext());
    //load(env, name_mod());
    //load(env, name_mod_ext());
    load(env, name_power());
    load(env, name_power_ext());
}

static void load_comparison_operations(Lexeme * env) {
    load(env, name_equal());
    load(env, name_equal_in());
    load(env, name_not_equal());
    load(env, name_not_equal_in());
    load(env, name_greater());
    load(env, name_greater_in());
    load(env, name_less());
    load(env, name_less_in());
    load(env, name_not_less());
    load(env, name_not_less_in());
    load(env, name_not_greater());
    load(env, name_not_greater_in());
}

static void load_Boolean_operations(Lexeme * env) {
    load(env, name_not());
    load(env, name_not_pre());
    load(env, name_and());
    load(env, name_and_ext());
    load(env, name_or());
    load(env, name_or_ext());
}

static void load_list_operations(Lexeme * env) {
    load(env, name_pair());
    load(env, name_pair_comma());
    load(env, name_left());
    load(env, name_right());
    load(env, name_set_left());
    load(env, name_set_right());
}

static void load_array_operations(Lexeme * env) {
    load(env, name_array());
    load(env, name_array_get());
    load(env, name_array_set());
}

static void load_string_operations(Lexeme * env) {
    load(env, name_char());
    load(env, name_concat());
}

static void load_input_output_operations(Lexeme * env) {
    load(env, name_print());
    load(env, name_println());
    load(env, name_read_terminal());
}


static void load(Lexeme * env, Lexeme * built_in) {
    Lexeme * name = get_built_in_name(built_in);
    Lexeme * closure = get_built_in_closure(built_in);
    // Link closure to its defining environment (the base layer).
    closure = set_closure_environment(closure, env);
    add_binding(env, name, closure);
}


/* functions needed:

I need to add syntax checking for function names, perhaps, 
at least for extensible calls (must be of form _op_& with exactly two parameters).

predicates?

*/
