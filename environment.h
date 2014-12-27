/* Tucker Meyers: CS403 Programming Language Project
 * Environment Module
 */

#ifndef ENVIRONMENT_HEADER
#define ENVIRONMENT_HEADER

extern Lexeme * new_environment(void);
extern Lexeme * extend_environment(Lexeme * env, Lexeme * ids, Lexeme * values);
extern void add_binding(Lexeme * env, Lexeme * id, Lexeme * value);
extern void add_bindings(Lexeme * env, Lexeme * ids, Lexeme * values);
extern void change_binding(Lexeme * env, Lexeme * target_id, Lexeme * new_value);
extern Lexeme * look_up(Lexeme * env, Lexeme * target_id);
extern Lexeme * look_up_quietly(Lexeme * env, Lexeme * target_id);
extern void inspect_local_table(Lexeme * env);
extern void inspect_all_tables(Lexeme * env);

#endif

