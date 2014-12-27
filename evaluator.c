/* Tucker Meyers: CS403 Programming Language Project
 * Evaluator Module
 * The evaluator accepts various kinds of parse trees and
 * (of course) evaluates the program contents that they
 * represent.
 */

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexeme.h"
#include "lexer.h"
#include "trees.h"
#include "parser.h"
#include "environment.h"
#include "function-evaluator.h"
#include "built-ins.h"

static int is_a_primary(char * type);
static int is_true(Lexeme * condition, Lexeme * env);
static Lexeme * evaluate_binding(Lexeme * tree, Lexeme * env);
static Lexeme * evaluate_rebinding(Lexeme * tree, Lexeme * env);
static Lexeme * evaluate_if_control(Lexeme * tree, Lexeme * env);
static Lexeme * evaluate_while_control(Lexeme * tree, Lexeme * env);
//static Lexeme * evaluate_for_control(Lexeme * tree, Lexeme * env);
static Lexeme * evaluate_expression_list(Lexeme * tree, Lexeme * env);
/* Notes:
 * Although these functions mostly follow the structure of the parser,
 * some parse trees have no evaluation functions.
 *  - The evaluator for function calls is in a separate module.
 *  - Closures need no evaluation function since they are constructed
 *    as trees by the parser and not actually used until called.
 *  - Functions and classes are reduced to bindings of closures, so
 *    there are no parse trees of type "function" or "class" for the
 *    evaluator to deal with.
 *  - Identifier lists are only used for parameter lists, which are
 *    not evaluated.
 */


/*** Public Interface ***/

Lexeme * evaluate(Lexeme * tree, Lexeme * env) {
    char * type = getType(tree);
    //fprintf(stdout, " >> ");  printOut(tree);
    if (is_a_primary(type)) {
        return tree;
    }
    else if (type == CLOSURE) {
        // If this is a new closure, link it to its defining scope (this one).
        if (get_closure_environment(tree) == NULL) {
            tree = set_closure_environment(tree, env);
        }
        return tree;
    }
    else if (type == IDENTIFIER) {
        return look_up(env, tree);
    }
    else if (type == CALL) {
        // Function calls are evaluated by the function evaluator module.
        return evaluate_function_call(tree, env);
    }
    else if (type == BUILT_IN) {
        // Built-ins are executed directly by the built-ins module.
        return execute(tree, env);
    }
    else if (type == BINDING) {
        return evaluate_binding(tree, env);
    }
    else if (type == REBINDING) {
        return evaluate_rebinding(tree, env);
    }
    else if (type == IF_CLAUSE) {
        return evaluate_if_control(tree, env);
    }
    else if (type == WHILE_LOOP) {
        return evaluate_while_control(tree, env);
    }
    //else if (type == FOR_LOOP) {
    //    return evaluate_for_control(tree, env);
    //}
    else if (type == EXPRESSION_LIST) {
        return evaluate_expression_list(tree, env);
    }
    else {
        fprintf(stderr, "Evaluation error: I don't know what to do with type <%s>.\n", type);
        exit(-1);
    }
}


Lexeme * evaluate_list(Lexeme * list, Lexeme * env) {
    /* Evaluate items in a list and return the values
     * in another list.
     */
    if (list == NULL) {
        return NULL;
    }
    else {
        Lexeme * first = evaluate(car(list), env);
        return expression_list_tree(first, evaluate_list(cdr(list), env));
    }
}


/*** Private Functions ***/

static int is_a_primary(char * type) {
    /* In this context, a "primary" is any program element whose
     * parse tree is already its fully-evaluated representation.
     */
    return (type == INTEGER
        || type == FLOAT
        || type == STRING
        || type == BOOLEAN
        || type == ARRAY
        || type == PAIR
        || type == NIL
    );
}

static int is_true(Lexeme * condition, Lexeme * env) {
    Lexeme * result = evaluate(condition, env);
    if (! typeBoolean(result)) {
        fprintf(stderr, "Conditional error: A condition must evaluate to a Boolean, not <%s>.",
            getType(result));
        exit(-1);
    }
    return getBooleanValue(result);
}

static Lexeme * evaluate_binding(Lexeme * tree, Lexeme * env) {
    Lexeme * n = get_binding_name(tree);
    Lexeme * v = evaluate(get_binding_value(tree), env);
    add_binding(env, n, v);
    return v;
}

static Lexeme * evaluate_rebinding(Lexeme * tree, Lexeme * env) {
    Lexeme * n = get_rebinding_name(tree);
    Lexeme * v = evaluate(get_rebinding_value(tree), env);
    change_binding(env, n, v);
    return v;
}

static Lexeme * evaluate_if_control(Lexeme * tree, Lexeme * env) {
    /* An 'if' control has a condition, a body, and an optional else 
     * clause (just a body). Evaluate the condition first. If it is true,
     * evaluate the regular body; otherwise, if an else clause is present, 
     * evaluate it, and if not, evaluate to 'false'.
     */
    Lexeme * condition = evaluate(get_if_condition(tree), env);
    Lexeme * else_clause = get_if_else(tree);
    if (is_true(condition, env))
        return evaluate(get_if_body(tree), env);
    else if (else_clause != NULL)
        return evaluate(get_else_body(else_clause), env);
    else
        return newBooleanValueLexeme(FALSE);
}

static Lexeme * evaluate_while_control(Lexeme * tree, Lexeme * env) {
    /* A 'while' control has a condition and a body. If the condition 
     * is true, the body is evaluated. This repeats until the condition 
     * becomes false.
     */
    Lexeme * condition = get_while_condition(tree);
    Lexeme * body = get_while_body(tree);
    Lexeme * result = NULL;
    while (is_true(condition, env)) {
        result = evaluate(body, env);
    }
    return result;
}


/*
static Lexeme * evaluate_for_control(Lexeme * tree, Lexeme * env) {
     * A 'for' control has a control variable, an iterator, and a body.
     * The variable is assigned to the value of the iterator and the body
     * evaluated. This repeats until the iterator returns 'nil'.
     *
    Lexeme * control_variable = get_for_iterator_variable(tree);
    Lexeme * iterator = get_for_iterator_call(tree);
    Lexeme * body = get_for_body(tree);
    Lexeme * result = NULL;
    // To start, evaluate the iterator call and bind it to the variable.

    Lexeme * i = evaluate(iterator);
    add_binding(env, control_variable, i);
    // new iterator style returns function with two entries (value, next)

    // As long is the iterator is not 'nil', loop.
    while (getType(i) != NIL) {
        result = evaluate(body, env);

        i = evaluate(iterator);
        change_binding(env, control_variable, i);

    }
    return result;
}
*/

static Lexeme * evaluate_expression_list(Lexeme * tree, Lexeme * env) {
    Lexeme * e = evaluate(car(tree), env);
    if (cdr(tree) == NULL)
        return e;
    else
        return evaluate_expression_list(cdr(tree), env);
}

