/* Tucker Meyers: CS403 Programming Language Project
 * Runner Module
 * The runner accepts a file of source code and executes it.
 */

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "environment.h"
#include "loader.h"

void runner(char * file_name);
static void space(void);
//static void parse_reader(Lexeme * tree, int depth);

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stdout, "To run a program, give the file name as an argument.\n");
        return -1;
    }
    runner(argv[1]);
    return 0;
}

void runner(char * file_name) {
    FILE * fp = fopen(file_name, "r");
    if (fp == 0) {
        fprintf(stderr, "File <%s> could not be opened for reading.\n", file_name);
        exit(-1);
    }
    // Prepare a base layer with the built-in bindings.
    Lexeme * base = new_environment();
    load_built_in_names(base);
    // The program itself runs on an extension of the base.
    Lexeme * env = extend_environment(base, NULL, NULL);
    // Start up a lexer for the source code.
    Lexer * L = newLexer(fp);
    initializeLexer(L);
    // Parse the source code using the lexer.
    Lexeme * parse_tree = program(L);
    //parse_reader(parse_tree, 0);
    // Evaluate the parse tree.
    space();
    evaluate(parse_tree, env);
    space();
    //printf("[[Evaluation complete.]]\nFinal return value: ");
    //printout(result);
    // Done.
    fclose(fp);
}

static void space() {printf("\n");}

/*
static void parse_reader(Lexeme * tree, int depth) {
    if (tree != NULL) {
        for (int i = 0; i < depth; i++) fprintf(stdout, "  ");
        printout(tree);
        parse_reader(car(tree), depth + 1);
        parse_reader(cdr(tree), depth + 1);
    }
}
*/

