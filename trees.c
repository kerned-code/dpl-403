/* Tucker Meyers: CS403 Programming Language Project
 * Trees Module
 *   The trees module provides functions for building elements 
 * of parse trees, such as function calls, expression lists, 
 * and control structures. This just makes things  easier in 
 * parser.c (avoiding lots of calls to consWithTag and so on).
 *   There are also functions for retrieving parts from 
 * parse trees of various kinds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "types.h"
#include "lexeme.h"
#include "lexer.h"
#include "trees.h"


static Lexeme * conditional_tree(Lexeme * condition, Lexeme * body);
static Lexeme * get_conditional_condition(Lexeme * conditional);
static Lexeme * get_conditional_body(Lexeme * conditional);
//static Lexeme * for_iterator_tree(Lexeme * control_variable, Lexeme * iterator_call);
//static Lexeme * get_iterator_variable(Lexeme * for_iterator);
//static Lexeme * get_iterator_call(Lexeme * for_iterator);
static Lexeme * make_class_body(Lexeme * body);


/*** Public Interface ***/

/* CALLS */

Lexeme * function_call_tree(Lexeme * name, Lexeme * args) {
    /* LEFT:  name (closure expression)
     * RIGHT: args (expression list)
     * If the name is omitted, the function call is interleaved.
     * The evaluator will be able to identify the function-name
     * parts and the arguments based on the length of the
     * list of "arguments". See grammar for details.
     */
    return consWithTag(CALL, name, args);
}

Lexeme * get_call_name(Lexeme * call) {
    return car(call);
}
Lexeme * get_call_arguments(Lexeme * call) {
    return cdr(call);
}


/* BINDINGS */

Lexeme * variable_definition_tree(Lexeme * name, Lexeme * value) {
    /* LEFT:  name (identifier)
     * RIGHT: value (expression)
     */
    return consWithTag(BINDING, name, value);
}

Lexeme * variable_redefinition_tree(Lexeme * name, Lexeme * value) {
    /* LEFT:  name (identifier)
     * RIGHT: value (expression)
     */
    return consWithTag(REBINDING, name, value);
}

// It's entirely acceptable to use the 'binding' versions
// for both bindings and rebindings.
Lexeme * get_binding_name(Lexeme * binding) {return car(binding);}
Lexeme * get_binding_value(Lexeme * binding) {return cdr(binding);}
Lexeme * get_rebinding_name(Lexeme * rebinding) {return car(rebinding);}
Lexeme * get_rebinding_value(Lexeme * rebinding) {return cdr(rebinding);}


/* CLOSURES */

Lexeme * closure_tree(Lexeme * params, Lexeme * body) {
    /* LEFT:  params (identifier list)
     * RIGHT: value -> LEFT:  body (expression list)
     *                 RIGHT: environment (table)
     */
    // Closures are initially built by the parser, which has no
    // knowledge of environment tables, so the initial closure
    // has no environment.
    Lexeme * default_env = NULL;
    return consWithTag(CLOSURE, params, cons(body, default_env));
}

Lexeme * function_tree(Lexeme * name, Lexeme * params, Lexeme * body) {
    /* A definition of the form [function name with params: body]
     * is treated as [bind name: [closure with params: body]].
     * LEFT:  name (identifier)
     * RIGHT: value -> LEFT:  params (identifier list)
     *                 RIGHT: body (expression list)
     */
    return variable_definition_tree(name, closure_tree(params, body));
}

Lexeme * class_tree(Lexeme * name, Lexeme * params, Lexeme * body) {
    /* A class is like a function, but it also has an extra
     * body statement added to return its local scope.
     * LEFT:  name (identifier)
     * RIGHT: value -> LEFT:  params (identifier list)
     *                 RIGHT: body (expression list)
     */
    body = make_class_body(body);
    return function_tree(name, params, body);
}

Lexeme * get_closure_parameters(Lexeme * closure) {
    return car(closure);
}
Lexeme * get_closure_body(Lexeme * closure) {
    return car(cdr(closure));
}
Lexeme * set_closure_environment(Lexeme * closure, Lexeme * env) {
    setCdr(cdr(closure), env);
    return closure;
}
Lexeme * get_closure_environment(Lexeme * closure) {
    return cdr(cdr(closure));
}
Lexeme * get_function_name(Lexeme * function) {
    return get_binding_name(function);
}
Lexeme * get_function_parameters(Lexeme * function) {
    return get_closure_parameters(get_binding_value(function));
}
Lexeme * get_function_body(Lexeme * function) {
    return get_closure_body(get_binding_value(function));
}
Lexeme * get_function_environment(Lexeme * function) {
    return get_closure_environment(get_binding_value(function));
}
Lexeme * get_class_name(Lexeme * class) {
    return get_binding_name(class);
}
Lexeme * get_class_parameters(Lexeme * class) {
    return get_closure_parameters(get_binding_value(class));
}
Lexeme * get_class_body(Lexeme * class) {
    return get_closure_body(get_binding_value(class));
}
Lexeme * get_class_environment(Lexeme * class) {
    return get_closure_environment(get_binding_value(class));
}


/* CONTROL STRUCTURES */

Lexeme * if_control_tree(Lexeme * condition, Lexeme * body) {
    /* LEFT:  conditional -> LEFT:  condition (expression)
     *                       RIGHT: body (expression list)
     * RIGHT: else clause (optional)
     */
    return consWithTag(IF_CLAUSE, conditional_tree(condition, body), NULL);
}

/*
Lexeme * else_if_control_tree(Lexeme * condition, Lexeme * body) {
    return consWithTag(ELSE_IF_CLAUSE, conditional_tree(condition, body), NULL);
}

Lexeme * add_else_if_control(Lexeme * if_tree, Lexeme * condition, Lexeme * body) {
    // Add another conditional clause to a chain of conditionals.
    setTail(if_tree, else_if_control_tree(condition, body));
    return if_tree;
}
*/

Lexeme * else_control_tree(Lexeme * body) {
    return consWithTag(ELSE_CLAUSE, conditional_tree(NULL, body), NULL);
}

Lexeme * add_else_control(Lexeme * if_tree, Lexeme * else_tree) {
    setTail(if_tree, else_tree);
    return if_tree;
}

Lexeme * while_control_tree(Lexeme * condition, Lexeme * body) {
    /* LEFT:  condition (expression)
     * RIGHT: body (expression list)
     */
    return consWithTag(WHILE_LOOP, condition, body);
}


//Lexeme * for_control_tree(Lexeme * control_variable, Lexeme * iterator, Lexeme * body) {
    /* LEFT:  for iterator -> LEFT:  control variable (identifier)
     *                        RIGHT: iterator (function call)
     * RIGHT: body (expression list)
     */
//    return consWithTag(FOR_LOOP, for_iterator_tree(control_variable, iterator), body);
//}

Lexeme * get_if_condition(Lexeme * if_tree) {
    return get_conditional_condition(car(if_tree));
}
Lexeme * get_if_body(Lexeme * if_tree) {
    return get_conditional_body(car(if_tree));
}
Lexeme * get_if_else(Lexeme * if_tree) {
    return cdr(if_tree);
}
Lexeme * get_else_body(Lexeme * else_tree) {
    return get_conditional_body(car(else_tree));
}
Lexeme * get_while_condition(Lexeme * while_tree) {
    return car(while_tree);
}
Lexeme * get_while_body(Lexeme * while_tree) {
    return cdr(while_tree);
}
/*
Lexeme * get_for_iterator_variable(Lexeme * for_tree) {
    return get_iterator_variable(car(for_tree));
}
Lexeme * get_for_iterator_call(Lexeme * for_tree) {
    return get_iterator_call(car(for_tree));
}
Lexeme * get_for_body(Lexeme * for_tree) {
    return cdr(for_tree);
}
*/


/* LISTS */

Lexeme * expression_list_tree(Lexeme * expr, Lexeme * rest) {
    /* LEFT:  expression
     * RIGHT: (next expression-list node)
     */
    return consWithTag(EXPRESSION_LIST, expr, rest);
}

Lexeme * identifier_list_tree(Lexeme * id, Lexeme * rest) {
    /* LEFT:  identifier
     * RIGHT: (next identifier-list node)
     */
    return consWithTag(IDENTIFIER_LIST, id, rest);
}

// These have no need of accessors since they're lists; 
// just use 'car' and 'cdr'.


/* BUILT-INS */

Lexeme * built_in_tree(Lexeme * params, char * key) {
    /* A built-in function is a closure with a special
     * Built-In-type lexeme as its body. This triggers
     * the evaluator to execute the operation directly
     * using provided source code in the program. The key
     * is a special string constant indicating the 
     * built-in operation to be performed.
     */
    return closure_tree(
        params,
        expression_list_tree(
            newBuiltInLexeme(key), 
            NULL
        )
    );
}

Lexeme * named_built_in_tree(char * name, Lexeme * built_in) {
    return cons(newIdentifierLexeme(name), built_in);
}

Lexeme * get_built_in_name(Lexeme * named_built_in) {
    return car(named_built_in);
}
Lexeme * get_built_in_closure(Lexeme * named_built_in) {
    return cdr(named_built_in);
}
Lexeme * get_built_in_parameters(Lexeme * built_in) {
    return car(built_in);
}
Lexeme * get_built_in_key(Lexeme * built_in) {
    return cdr(built_in);
}


/*** Private Functions ***/

static Lexeme * conditional_tree(Lexeme * condition, Lexeme * body) {
    /* LEFT:  condition (expression)
     * RIGHT: body (expression list)
     */
    return consWithTag(CONDITIONAL, condition, body);
}

static Lexeme * get_conditional_condition(Lexeme * conditional) {return car(conditional);}
static Lexeme * get_conditional_body(Lexeme * conditional) {return cdr(conditional);}

//static Lexeme * for_iterator_tree(Lexeme * control_variable, Lexeme * iterator_call) {
    /* LEFT:  control variable (identifier)
     * RIGHT: iterator (function call)
     */
//    return consWithTag(FOR_LOOP_ITERATOR, control_variable, iterator_call);
//}

//static Lexeme * get_iterator_variable(Lexeme * for_iterator) {return car(for_iterator);}
//static Lexeme * get_iterator_call(Lexeme * for_iterator) {return cdr(for_iterator);}

static Lexeme * make_class_body(Lexeme * body) {
    /* Add a secret return statement to the class's body
     * so that its scope is accessible to clients.
     */
    if (getType(body) == EXPRESSION_LIST) {
        Lexeme * return_self = expression_list_tree(newIdentifierLexeme("@"), NULL);
        setTail(body, return_self);
        return body;
    } else {
        fprintf(stderr, "Tree error: This class body should be a tree of type <EXPRESSION_LIST>,\n");
        fprintf(stderr, "but its type is <%s>.\n", getType(body));
        exit(-1);
    }
}

