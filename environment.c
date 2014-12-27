/* Tucker Meyers: CS403 Programming Language Project
 * Environment Module
 * The environment keeps track of the program's values
 * and the names to which they are bound. It stores these
 * in a stack of tables or "scopes".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "trees.h"
#include "loader.h"
#include "environment.h"

static Lexeme * new_environment_with(Lexeme * parent);
static Lexeme * new_table(void);
static Lexeme * get_table(Lexeme * env);
static Lexeme * get_identifiers(Lexeme * table);
static Lexeme * get_values(Lexeme * table);
static void set_identifiers(Lexeme * table, Lexeme * identifier_list);
static void set_values(Lexeme * table, Lexeme * value_list);
static int same_names(Lexeme * id1, Lexeme * id2);
static void inspect_table(Lexeme * table);


/*** Public Interface ***/

Lexeme * new_environment() {
    /* An environment is a list of ENVIRONMENT nodes that point to tables. */
    return new_environment_with(NULL);
}

Lexeme * extend_environment(Lexeme * env, Lexeme * ids, Lexeme * values) {
    /* To extend an environment is to create a new innermost scope enclosed in
     * the existing local scope. This new table is populated with the given
     * names and values.
     */
    Lexeme * env_ext = new_environment_with(env);
    load_local_names(env_ext);
    if (ids != NULL && values != NULL)
        add_bindings(env_ext, ids, values);
    return env_ext;
}

void add_binding(Lexeme * env, Lexeme * id, Lexeme * value) {
    /* Add a new name-value binding to the table. */
    Lexeme * t = get_table(env);
    set_identifiers(t, identifier_list_tree(id, get_identifiers(t)));
    set_values(t, expression_list_tree(value, get_values(t)));
}

void add_bindings(Lexeme * env, Lexeme * ids, Lexeme * values) {
    /* Add a list of name-value bindings to the table.
     * Note that the items are inserted to the front of the list
     * en masse, so this operation is not equivalent to inserting
     * the items individually by add_binding; they end up in the 
     * opposite order. This is not a problem unless the user
     * decided to, say, give a function two parameters with the
     * same name. The *first* one assigned will be the one that is
     * found during evaluation.
     */
    int count_ids = length(ids);
    int count_values = length(values);
    if (count_ids != count_values) {
        fprintf(stderr, "Binding (scope) error: Cannot bind %d names to %d values.\n", count_ids, count_values);
        exit(-1);
    }
    Lexeme * t = get_table(env);
    Lexeme * copy_ids = copyLexemeTree(ids); 
    Lexeme * copy_values = copyLexemeTree(values);
    setTail(copy_ids, get_identifiers(t));
    setTail(copy_values, get_values(t));
    set_identifiers(t, copy_ids);
    set_values(t, copy_values);
}

void change_binding(Lexeme * env, Lexeme * target_id, Lexeme * new_value) {
    while (env != NULL) {
        Lexeme * ids = get_identifiers(get_table(env));
        Lexeme * values = get_values(get_table(env));
        while (ids != NULL) {
            if (same_names(target_id, car(ids))) {
                setCar(values, new_value);
                return;
            }
            else {
                ids = cdr(ids);
                values = cdr(values);
            }
        }
        env = cdr(env);
    }
    fprintf(stderr,
        "Name reference error: \"%s\" cannot be rebound because it is not defined.\n",
        getIdentifierName(target_id));
    exit(-1);
}

Lexeme * look_up(Lexeme * env, Lexeme * target_id) {
    /* Check for the given identifier in all accessible tables
     * in the given environment stack. If the identifier is not
     * defined, complain loudly.
     */
    Lexeme * value = look_up_quietly(env, target_id);
    if (value == NULL) {
        fprintf(stderr, 
            "Name reference error: <%s> is not defined in any accessible scope.\n", 
            getIdentifierName(target_id));
        exit(-1);
    }
    return value;
}

Lexeme * look_up_quietly(Lexeme * env, Lexeme * target_id) {
    /* Check for the given identifier in all accessible tables
     * in the given environment stack. If the identifier is not
     * defined, simply return NULL.
     */
    // Loop through tables in stack.
    while (env != NULL) {
        Lexeme * ids = get_identifiers(get_table(env));
        Lexeme * values = get_values(get_table(env));
        // Traverse the list of IDs.
        while (ids != NULL) {
            if (same_names(target_id, car(ids))) {
                return car(values);
            }
            else {
                ids = cdr(ids);
                values = cdr(values);
            }
        }
        env = cdr(env);
    }
    return NULL;
}

void inspect_local_table(Lexeme * env) {
    if (env == NULL) {
        fprintf(stderr, "Curiously, this environment has no tables at all.\n");
        exit(-1);
    }
    printf("Local environment:\n");
    inspect_table(env);
    //printf("done with local inspection\n");
}

void inspect_all_tables(Lexeme * env) {
    inspect_local_table(env);
    env = cdr(env);
    while (env != NULL) {
        printf("Parent environment:\n");
        inspect_table(env);
        env = cdr(env);
    }
    //printf("done with inspection\n");
}


/*** Private Functions ***/

static Lexeme * new_environment_with(Lexeme * parent) {
    Lexeme * env = consWithTag(ENVIRONMENT, new_table(), parent);
    if (parent == NULL) {
        //printf("Made new root table\n");
        setEnvironmentDepth(env, 0);
    }
    else
        setEnvironmentDepth(env, 1 + getEnvironmentDepth(parent));
    return env;
}

static Lexeme * new_table() {
    return consWithTag(TABLE, NULL, NULL);
}
static Lexeme * get_table(Lexeme * env) {
    return car(env);
}

static Lexeme * get_identifiers(Lexeme * table) {
    return car(table);
}
static void set_identifiers(Lexeme * table, Lexeme * identifier_list) {
    setCar(table, identifier_list);
}
static Lexeme * get_values(Lexeme * table) {
    return cdr(table);
}
static void set_values(Lexeme * table, Lexeme * value_list) {
    setCdr(table, value_list);
}

static int same_names(Lexeme * id1, Lexeme * id2) {
    return (0 == strcmp(
            getIdentifierName(id1),
            getIdentifierName(id2))
    );
}

static void inspect_table(Lexeme * env) {
    Lexeme * table = get_table(env);
    char * pad = "   ";
    char * bound = "  ->  ";
    Lexeme * ids = get_identifiers(table);
    Lexeme * values = get_values(table);
    printf("Table, level %d:\n", getEnvironmentDepth(env));
    while (ids != NULL) {
        printf("%s%s%s", pad, getIdentifierName(car(ids)), bound);
        printOut(car(values));
        if (cdr(values) == NULL && cdr(ids) != NULL) {
            fprintf(stderr, "Environment error: Malformed table has more identifiers than values.\n");
            exit(-1);
        }
        ids = cdr(ids);
        values = cdr(values);
    }
}

