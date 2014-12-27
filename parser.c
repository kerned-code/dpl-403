/* Tucker Meyers: CS403 Programming Language Project
 * Parser Module
 * The parser determines by recursive descent whether the given
 * program is syntactically acceptable.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"
#include "trees.h"

static int show_debugging_messages = 0;

/* Expressions: */
    static Lexeme * expression_list(Lexer * L);
    static int  expression_list_pending(Lexer * L);
    static Lexeme * optional_expression_list(Lexer * L);
    static Lexeme * block(Lexer * L);
//  static int  block_pending(Lexer * L);
    static Lexeme * identifier_list(Lexer * L);
    static int  identifier_list_pending(Lexer * L);
    static Lexeme * optional_identifier_list(Lexer * L);
    static Lexeme * expression(Lexer * L);
    static int  expression_pending(Lexer * L);
    static Lexeme * control_expression(Lexer * L);
    static int  control_expression_pending(Lexer * L);
    static Lexeme * definition_expression(Lexer * L);
    static int  definition_expression_pending(Lexer * L);
/* Control structures: */
    static Lexeme * control(Lexer * L);
//  static int  control_pending(Lexer * L);
    static Lexeme * if_control(Lexer * L);
    static int  if_control_pending(Lexer * L);
    static Lexeme * else_control(Lexer * L);
    static int  else_control_pending(Lexer * L);
    static Lexeme * optional_else_control(Lexer * L);
    static Lexeme * while_control(Lexer * L);
    static int  while_control_pending(Lexer * L);
//    static Lexeme * for_control(Lexer * L);
//    static int  for_control_pending(Lexer * L);
/* Definitions: */
    static Lexeme * definition(Lexer * L);
//  static int  definition_pending(Lexer * L);
    static Lexeme * closure_definition(Lexer * L);
    static int  closure_definition_pending(Lexer * L);
    static Lexeme * function_definition(Lexer * L);
    static int  function_definition_pending(Lexer * L);
    static Lexeme * class_definition(Lexer * L);
    static int  class_definition_pending(Lexer * L);
    static Lexeme * optional_with(Lexer * L);
    static Lexeme * variable_definition(Lexer * L);
    static int  variable_definition_pending(Lexer * L);
    static Lexeme * variable_redefinition(Lexer * L);
    static int  variable_redefinition_pending(Lexer * L);
/* Primaries: */
    static Lexeme * primary(Lexer * L);
    static int  primary_pending(Lexer * L);
    static Lexeme * function_call_expression(Lexer * L);
    static int  function_call_expression_pending(Lexer * L);
    static Lexeme * function_call(Lexer * L);
//  static int  function_call_pending(Lexer * L);

/* Helpers: */
static void debug_log(char * message);


/*** Public Interface ***/

Lexeme * program(Lexer * L) {
    return expression_list(L);
}


/*** Private Functions ***/

/* Expressions */

Lexeme * expression_list(Lexer * L) {
    Lexeme * first, * rest;
    debug_log("expression list\n");
    first = expression(L);
    if (expression_list_pending(L))
        rest = expression_list(L);
    else
        rest = NULL;
    debug_log("expression list end\n");
    return expression_list_tree(first, rest);
}

int expression_list_pending(Lexer * L) {
    return expression_pending(L);
}

Lexeme * optional_expression_list(Lexer * L) {
    Lexeme * expr_list;
    debug_log("optional expression list\n");
    if (expression_list_pending(L))
        expr_list = expression_list(L);
    else
        expr_list = NULL;
    debug_log("optional expression list end\n");
    return expr_list;
}

Lexeme * block(Lexer * L) {
    debug_log("block\n");
    return expression_list(L);
    debug_log("block end\n");
}

/*
int block_pending(Lexer * L) {
    return expression_list_pending(L);
}
*/

Lexeme * identifier_list(Lexer * L) {
    Lexeme * first, * rest;
    debug_log("variable list\n");
    first = match(L, IDENTIFIER);
    if (identifier_list_pending(L))
        rest = identifier_list(L);
    else
        rest = NULL;
    debug_log("variable list end\n");
    return identifier_list_tree(first, rest);
}

int identifier_list_pending(Lexer * L) {
    return check(L, IDENTIFIER);
}

Lexeme *  optional_identifier_list(Lexer * L) {
    Lexeme * ids = NULL;
    debug_log("optional variable list\n");
    if (identifier_list_pending(L))
        ids = identifier_list(L);
    debug_log("optional variable list end\n");
    return ids;
}

Lexeme * expression(Lexer * L) {
    Lexeme * expr = NULL;
    debug_log("expression\n");
    if (primary_pending(L))
        expr = primary(L);
    else if (control_expression_pending(L))
        expr = control_expression(L);
    else if (definition_expression_pending(L))
        expr = definition_expression(L);
    else
        typeError(L, "primary, block expression, or definition");
    debug_log("expression end\n");
    return expr;
}

int expression_pending(Lexer * L) {
    return (primary_pending(L) 
        || control_expression_pending(L)
        || definition_expression_pending(L));
}

Lexeme * control_expression(Lexer * L) {
    Lexeme * ctrl;
    debug_log("control expression\n");
    match(L, LEFT_BRACE);
    ctrl = control(L);
    match(L, RIGHT_BRACE);
    debug_log("control expression end\n");
    return ctrl;
}

int control_expression_pending(Lexer * L) {
    return check(L, LEFT_BRACE);
}

Lexeme * definition_expression(Lexer * L) {
    Lexeme * def;
    debug_log("definition expression\n");
    match(L, LEFT_BRACKET);
    def = definition(L);
    match(L, RIGHT_BRACKET);
    debug_log("definition expression end\n");
    return def;
}

int definition_expression_pending(Lexer * L) {
    return check(L, LEFT_BRACKET);
}

/* Control structures */

Lexeme * control(Lexer * L) {
    Lexeme * ctrl = NULL;
    debug_log("control\n");
    if (if_control_pending(L))
        ctrl = if_control(L);
    else if (while_control_pending(L))
        ctrl = while_control(L);
    //else if (for_control_pending(L))
    //    ctrl = for_control(L);
    else
        typeError(L, "control structure (if, while, for)");
    debug_log("control end\n");
    return ctrl;
}

Lexeme * if_control(Lexer * L) {
    Lexeme * condition, * body, * else_tree;
    debug_log("if control\n");
    match(L, KEYWORD_IF);
    condition = expression(L);
    body = block(L);
    else_tree = optional_else_control(L);
    debug_log("if control end\n");
    return add_else_control(if_control_tree(condition, body), else_tree);
}

int if_control_pending(Lexer * L) {
    return check(L, KEYWORD_IF);
}

Lexeme * else_control(Lexer * L) {
    Lexeme * body;
    debug_log("else control\n");
    match(L, KEYWORD_ELSE);
    body = block(L);
    debug_log("else control end\n");
    return else_control_tree(body);
}

int else_control_pending(Lexer * L) {
    return check(L, KEYWORD_ELSE);
}

Lexeme * optional_else_control(Lexer * L) {
    Lexeme * e = NULL;
    debug_log("optional else control\n");
    if (else_control_pending(L))
        e = else_control(L);
    debug_log("optional else control end\n");
    return e;
}

Lexeme * while_control(Lexer * L) {
    Lexeme * condition, * body;
    debug_log("while control\n");
    match(L, KEYWORD_WHILE);
    condition = expression(L);
    body = block(L);
    debug_log("while control end\n");
    return while_control_tree(condition, body);
}

int while_control_pending(Lexer * L) {
    return check(L, KEYWORD_WHILE);
}

/*
Lexeme * for_control(Lexer * L) {
    Lexeme * variable, * iterator, * body;
    debug_log("for control\n");
    match(L, KEYWORD_FOR);
    variable = match(L, IDENTIFIER);
    iterator = function_call(L);
    body = block(L);
    debug_log("for control end\n");
    return for_control_tree(variable, iterator, body);
}

int for_control_pending(Lexer * L) {
    return check(L, KEYWORD_FOR);
}
*/

/* Definitions */

Lexeme * definition(Lexer * L) {
    Lexeme * def = NULL;
    debug_log("definition\n");
    if (closure_definition_pending(L))
        def = closure_definition(L);
    else if (function_definition_pending(L))
        def = function_definition(L);
    else if (class_definition_pending(L))
        def = class_definition(L);
    else if (variable_definition_pending(L))
        def = variable_definition(L);
    else if (variable_redefinition_pending(L))
        def = variable_redefinition(L);
    else
        typeError(L, "definition (closure, function, class, or variable (re)binding)");
    debug_log("definition end\n");
    return def;
}

/*
int definition_pending(Lexer * L) {
    return (closure_definition_pending(L)
        || function_definition_pending(L) 
        || class_definition_pending(L)
        || variable_definition_pending(L)
        || variable_redefinition_pending(L));
}
*/

Lexeme * closure_definition(Lexer * L) {
    Lexeme * params, * body;
    debug_log("closure defintion\n");
    match(L, KEYWORD_CLOSURE);
    optional_with(L);
    params = optional_identifier_list(L);
    match(L, COLON);
    body = block(L);
    debug_log("closure definition end\n");
    return closure_tree(params, body);
}

int closure_definition_pending(Lexer * L) {
    return check(L, KEYWORD_CLOSURE);
}

Lexeme * function_definition(Lexer * L) {
    Lexeme * name, * params, * body;
    debug_log("function definition\n");
    match(L, KEYWORD_FUNCTION);
    name = match(L, IDENTIFIER);
    optional_with(L);
    params = optional_identifier_list(L);
    match(L, COLON);
    body = block(L);
    debug_log("function definition end\n");
    return function_tree(name, params, body);
}

int function_definition_pending(Lexer * L) {
    return check(L, KEYWORD_FUNCTION);
}

Lexeme * class_definition(Lexer * L) {
    Lexeme * name, * params, * body;
    debug_log("class definition\n");
    match(L, KEYWORD_CLASS);
    name = match(L, IDENTIFIER);
    optional_with(L);
    params = optional_identifier_list(L);
    match(L, COLON);
    body = block(L);
    debug_log("class definition end\n");
    return class_tree(name, params, body);
}

int class_definition_pending(Lexer * L) {
    return check(L, KEYWORD_CLASS);
}

Lexeme * optional_with(Lexer * L) {
    if (check(L, KEYWORD_WITH)) 
        return match(L, KEYWORD_WITH);
    return NULL;
}

Lexeme * variable_definition(Lexer * L) {
    Lexeme * name, * value;
    debug_log("variable definition\n");
    match(L, KEYWORD_BIND);
    name = match(L, IDENTIFIER);
    match(L, COLON);
    value = expression(L);
    debug_log("variable definition end\n");
    return variable_definition_tree(name, value);
}

int variable_definition_pending(Lexer * L) {
    return check(L, KEYWORD_BIND);
}

Lexeme * variable_redefinition(Lexer * L) {
    Lexeme * name, * value;
    debug_log("variable redefinition\n");
    match(L, KEYWORD_REBIND);
    name = match(L, IDENTIFIER);
    match(L, COLON);
    value = expression(L);
    debug_log("variable redefinition end\n");
    return variable_redefinition_tree(name, value);
}

int variable_redefinition_pending(Lexer * L) {
    return check(L, KEYWORD_REBIND);
}

/* Primaries */

Lexeme * primary(Lexer * L) {
    if (function_call_expression_pending(L))
        return function_call_expression(L);
    else if (check(L, INTEGER))
        return match(L, INTEGER);
    else if (check(L, FLOAT))
        return match(L, FLOAT);
    else if (check(L, STRING))
        return match(L, STRING);
    else if (check(L, BOOLEAN))
        return match(L, BOOLEAN);
    else if (check(L, IDENTIFIER))
        return match(L, IDENTIFIER);
    else if (check(L, NIL))
        return match(L, NIL);
    else
        typeError(L, "primary (call, number, string, Boolean, or variable)");
    return NULL;
}

int primary_pending(Lexer * L) {
    return (function_call_expression_pending(L)
        || check(L, INTEGER)
        || check(L, FLOAT)
        || check(L, STRING)
        || check(L, BOOLEAN)
        || check(L, IDENTIFIER)
        || check(L, NIL)
    );
}

Lexeme * function_call_expression(Lexer * L) {
    Lexeme * call;
    debug_log("function call expression\n");
    match(L, LEFT_PARENTHESIS);
    call = function_call(L);
    match(L, RIGHT_PARENTHESIS);
    debug_log("function call expression end\n");
    return call;
}

int function_call_expression_pending(Lexer * L) {
    return check(L, LEFT_PARENTHESIS);
}

Lexeme * function_call(Lexer * L) {
    /* If this is a regular call, the name will be followed
     * by a colon and the arguments will follow that. The 
     * name will be lexeme 'first' and the arguments will
     * be the 'rest'. For an interleaved call, all name parts
     * and arguments will be placed in 'rest'.
     */
    Lexeme * first, * rest;
    debug_log("function call\n");
    first = expression(L);
    if (check(L, COLON)) {
        /* Regular call. */
        match(L, COLON);
        rest = optional_expression_list(L);
    } else if (expression_list_pending(L)) {
        /* Interleaved call: put all parts, including first,
         * in argument list. 
         */
        rest = expression_list_tree(first, expression_list(L));
        first = NULL;
    } else {
        /* Call with no arguments. */
        rest = NULL;
    }
    debug_log("function call end\n");
    return function_call_tree(first, rest);
}

/* Helper */

static void debug_log(char * message) {
    if (show_debugging_messages)
        printf("%s", message);
}

