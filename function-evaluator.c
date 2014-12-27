/* Tucker Meyers: CS403 Programming Language Project
 * Function Evaluator Module
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "lexeme.h"
#include "lexer.h"
#include "trees.h"
#include "parser.h"
#include "environment.h"
#include "evaluator.h"
#include "function-evaluator.h"

static Lexeme * execute_function_call(char * name, Lexeme * closure, Lexeme * args_expr, Lexeme * env_call);
static Lexeme * evaluate_interleaved_function_call(Lexeme * parts, Lexeme * env);
static Lexeme * evaluate_interleaved_prefix_call(Lexeme * names, Lexeme * args, Lexeme * env);
static Lexeme * evaluate_interleaved_infix_call(Lexeme * names, Lexeme * args, Lexeme * env);
static char * get_name(Lexeme * tree);
static char * make_interleaved_name_prefix(Lexeme * names);
static char * make_interleaved_name_infix(Lexeme * names);
static char * make_extensible_name(Lexeme * name);
//static void printclosure(Lexeme * closure);


/*** Public Interface ***/

Lexeme * evaluate_function_call(Lexeme * tree, Lexeme * env) {
    Lexeme * name = get_call_name(tree);
    Lexeme * args = get_call_arguments(tree);
    if (name == NULL) {
        // If the name is null, the function's name is interleaved with its arguments.
        return evaluate_interleaved_function_call(args, env);
    }
    else {
        // For a regular call, simply evaluate the name expression
        // (and hope that it resolves to a closure).
        Lexeme * closure = evaluate(name, env);
        //printf("%s ", get_name(name));
        //printclosure(closure);
        if (getType(closure) != CLOSURE) {
            fprintf(stderr, "Function call error: The expression does not resolve to a closure.");
            fprintf(stderr, "    Its type is <%s>.", getType(closure));
            exit(-1);
        }
        return execute_function_call(get_name(name), closure, args, env);
    }
}


/*** Private Functions ***/

static Lexeme * execute_function_call(char * name, Lexeme * closure, Lexeme * args_expr, Lexeme * env_call) {
    /* Once the closure for the function has been acquired, evaluate its arguments,
     * populate them to their parameters in a new environment table, and evaluate
     * the closure body in the new environment.
     */
    Lexeme * params = get_closure_parameters(closure);
    int param_count = length(params);
    int arg_count = length(args_expr);
    if (param_count != arg_count) {
        fprintf(stderr, "Function call error: The function %s requires %d parameter(s),\n", name, param_count);
        fprintf(stderr, "    but %d argument(s) were supplied.\n", arg_count);
        exit(-1);
    }
    // Evaluate the arguments in the scope of the call.
    Lexeme * args_eval = evaluate_list(args_expr, env_call);
    // Acquire the closure's defining scope and extend it for the body evaluation.
    Lexeme * env_def = get_closure_environment(closure);
    Lexeme * env_ext = extend_environment(env_def, params, args_eval);
    // Compute and return.
    return evaluate(get_closure_body(closure), env_ext);
}

static Lexeme * evaluate_interleaved_function_call(Lexeme * parts, Lexeme * env) {
    // Use a proper copy of the parts list to extract name(s) and arguments.
    Lexeme * names = copyLexemeTree(parts);
    Lexeme * args = names;
    // Use the length of the parts list to decide how to separate the elements.
    if (length(parts) % 2 == 0) {
        // Call has prefix form (name arg name arg ... name arg).
        args = cdr(args);
        uninterleave(names, args);
        return evaluate_interleaved_prefix_call(names, args, env);
    }
    else {
        // Call has infix form (arg name arg ... name arg).
        names = cdr(names);
        uninterleave(args, names);
        return evaluate_interleaved_infix_call(names, args, env);
    }
}

static Lexeme * evaluate_interleaved_prefix_call(Lexeme * names, Lexeme * args, Lexeme * env) {
    /* A name in prefix form must resolve to a whole function with
     * name f1_f2_..fn_. (Extensible calls are infix by definition.)
     */
    char * long_name = make_interleaved_name_prefix(names);
    Lexeme * closure = look_up_quietly(env, newIdentifierLexeme(long_name));
    if (closure != NULL) {
        if (getType(closure) != CLOSURE) {
            fprintf(stderr, "Function call error: <%s> does not resolve to a closure.\n", long_name);
            exit(-1);
        }
        return execute_function_call(long_name, closure, args, env);
    }
    else {
        fprintf(stderr, "Interleaved call error: The function name segments given do not resolve to a known function.\n");
        fprintf(stderr, "    Tried:\n");
        fprintf(stderr, "      <%s> (interleaved)\n", long_name);
        // This call to getIdentifierName is safe because we checked that all of the
        // name parts are identifiers when we made long_name.
        fprintf(stderr, "    Did you mean (%s: ...)?\n", getIdentifierName(car(names)));
        exit(-1);
    }
}

static Lexeme * evaluate_interleaved_infix_call(Lexeme * names, Lexeme * args, Lexeme * env) {
    /* An infix-form call may resolve to a single function wih name
     * _f1_f2_..fn_ or to a series of extensible functions, which
     * have names of the form _f_&.
     */
    char * long_name = make_interleaved_name_infix(names);
    //inspect_all_tables(env);
    Lexeme * closure = look_up_quietly(env, newIdentifierLexeme(long_name));
    if (closure != NULL && getType(closure) == CLOSURE) {
        // Full infix name exists (_f1_f2_..fn_).
        return execute_function_call(long_name, closure, args, env);
    }
    else {
        // Check for extensible function _f1_&.
        char * ext_name = make_extensible_name(car(names));
        closure = look_up_quietly(env, newIdentifierLexeme(ext_name));
        if (closure != NULL) {
            // Extensible function exists. Get two arguments and evaluate.
            // Save rest of arguments for next evaluation.
            Lexeme * remaining_args = cdr(cdr(args));
            // Separate first two arguments from rest.
            setCdr(cdr(args), NULL);
            if (getType(closure) != CLOSURE) {
                fprintf(stderr, "Function call error: Neither <%s> nor <%s> resolves to a closure.\n", long_name, ext_name);
                exit(-1);
            }
            Lexeme * new_arg = execute_function_call(ext_name, closure, args, env);
            if (remaining_args != NULL) {
                // Evaluate the rest of the call with this call as the first argument.
                return evaluate_interleaved_infix_call(cdr(names), expression_list_tree(new_arg, remaining_args), env);
            }
            else {
                // The function evaluation is complete.
                return new_arg;
            }
        }
        else {
            fprintf(stderr, "Interleaved call error: The function name segments given do not resolve to a known function.\n");
            fprintf(stderr, "    Tried:\n");
            fprintf(stderr, "      <%s> (interleaved)\n", long_name);
            fprintf(stderr, "      <%s> (extensible)\n", ext_name);
            char * backup = (typeIdentifier(car(args)) ? getIdentifierName(car(args)) : "<closure>");
            fprintf(stderr, "    Did you mean (%s: ...)?\n", backup);
            exit(-1);
        }
    }
}

/* Strings and name manipulation */

static char * get_name(Lexeme * tree) {
    if (getType(tree) == IDENTIFIER)
        return getIdentifierName(tree);
    else
        return "(anonymous)";
}

static char * make_interleaved_name_prefix(Lexeme * names) {
    /* Build a string of form "f1_f2_...fn_", an interleaved 
     * function name in prefix form.
     */
    char * first;
    char * rest;
    char * whole;
    int length_first;
    int length_rest;
    if (names == NULL)
        return "";
    // Get the name of the first identifier in the list.
    if (! typeIdentifier(car(names))) {
        fprintf(stderr, 
            "Interleaved call error: A function name part must be an identifier, but I received a token of type <%s>.\n", 
            getType(car(names))
        );
        exit(-1);
    }
    first = getIdentifierName(car(names));
    length_first = strlen(first);
    // Get the rest of the name by recursion.
    rest = make_interleaved_name_prefix(cdr(names));
    length_rest = strlen(rest);
    whole = malloc(length_first + 1 + length_rest + 1);  // first, underscore, rest, \0
    // Copy first into front of whole.
    int i;
    for (i = 0; i < length_first; i++) {
        whole[i] = first[i];
    }
    whole[i] = '_';
    // Copy the rest (including \0 at the end).
    for (int j = 0; j <= length_rest; j++) {
        whole[length_first + 1 + j] = rest[j];
    }
    return whole;
}

static char * make_interleaved_name_infix(Lexeme * names) {
    /* Build a string of form "_f1_f2_...fn_" by calling 
     * make_interleaved_name_prefix and prepending an underscore.
     * The leading underscore turns a prefix-form function name
     * into an infix-form name.
     */
    char * base = make_interleaved_name_prefix(names);
    int length_base = strlen(base);
    char * whole = malloc(length_base + 1 + 1);  // underscore, string, \0
    whole[0] = '_';
    // Copy base into whole.
    for (int i = 0; i < length_base; i++) {
        whole[i + 1] = base[i];
    }
    whole[length_base + 2] = '\0';
    return whole;
}

static char * make_extensible_name(Lexeme * name) {
    char * base = getIdentifierName(name);
    int length_base = strlen(base);
    char * whole = malloc(1 + length_base + 1 + 1 + 1);  // underscore, base, underscore, ampersand, \0
    whole[0] = '_';
    for (int i = 0; i < length_base; i++) {
        whole[i + 1] = base[i];
    }
    whole[length_base + 1] = '_';
    whole[length_base + 2] = '&';
    whole[length_base + 3] = '\0';
    return whole;
}

/*
void printclosure(Lexeme * closure) {
    printf("CLOSURE -- params: ");
    for (Lexeme * p = get_closure_parameters(closure); p != NULL; p = cdr(p))
        printf("%s, ", getIdentifierName(car(p)));
    printf("\n");
}
*/

