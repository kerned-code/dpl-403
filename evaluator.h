/* Tucker Meyers: CS403 Programming Language Project
 * Evaluator Module
 */

#ifndef EVALUATOR_HEADER
#define EVALUATOR_HEADER

extern Lexeme * evaluate(Lexeme * tree, Lexeme * env);
extern Lexeme * evaluate_list(Lexeme * list, Lexeme * env);

#endif

