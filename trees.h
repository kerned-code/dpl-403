/* Tucker Meyers: CS403 Programming Language Project
 * Trees Modules
 */

#ifndef TREES_HEADER
#define TREES_HEADER

/* Calls */
extern Lexeme * function_call_tree(Lexeme * name, Lexeme * args);
extern Lexeme * get_call_name(Lexeme * call);
extern Lexeme * get_call_arguments(Lexeme * call);
/* Bindings */
extern Lexeme * variable_definition_tree(Lexeme * name, Lexeme * value);
extern Lexeme * variable_redefinition_tree(Lexeme * name, Lexeme * value);
extern Lexeme * get_binding_name(Lexeme * binding);
extern Lexeme * get_binding_value(Lexeme * binding);
extern Lexeme * get_rebinding_name(Lexeme * rebinding);
extern Lexeme * get_rebinding_value(Lexeme * rebinding);
/* Closures */
extern Lexeme * closure_tree(Lexeme * params, Lexeme * body);
extern Lexeme * function_tree(Lexeme * name, Lexeme * params, Lexeme * body);
extern Lexeme * class_tree(Lexeme * name, Lexeme * params, Lexeme * body);
extern Lexeme * get_closure_parameters(Lexeme * closure);
extern Lexeme * get_closure_body(Lexeme * closure);
extern Lexeme * set_closure_environment(Lexeme * closure, Lexeme * env);
extern Lexeme * get_closure_environment(Lexeme * closure);
extern Lexeme * get_function_name(Lexeme * function);
extern Lexeme * get_function_parameters(Lexeme * function);
extern Lexeme * get_function_body(Lexeme * function);
extern Lexeme * get_class_name(Lexeme * class);
extern Lexeme * get_class_parameters(Lexeme * class);
extern Lexeme * get_class_body(Lexeme * class);
/* Control structures */
extern Lexeme * if_control_tree(Lexeme * condition, Lexeme * body);
extern Lexeme * else_control_tree(Lexeme * body);
extern Lexeme * add_else_control(Lexeme * if_tree, Lexeme * else_tree);
extern Lexeme * while_control_tree(Lexeme * condition, Lexeme * body);
//extern Lexeme * for_control_tree(Lexeme * control_variable, Lexeme * iterator, Lexeme * body);
extern Lexeme * get_if_condition(Lexeme * if_tree);
extern Lexeme * get_if_body(Lexeme * if_tree);
extern Lexeme * get_if_else(Lexeme * if_tree);
extern Lexeme * get_else_body(Lexeme * else_tree);
extern Lexeme * get_while_condition(Lexeme * while_tree);
extern Lexeme * get_while_body(Lexeme * while_tree);
//extern Lexeme * get_for_iterator_variable(Lexeme * for_tree);
//extern Lexeme * get_for_iterator_call(Lexeme * for_tree);
//extern Lexeme * get_for_body(Lexeme * for_tree);
/* Lists (of expressions) */
extern Lexeme * expression_list_tree(Lexeme * expr, Lexeme * rest);
extern Lexeme * identifier_list_tree(Lexeme * id, Lexeme * rest);
/* Built-ins */
extern Lexeme * built_in_tree(Lexeme * params, char * key);
extern Lexeme * named_built_in_tree(char * name, Lexeme * built_in);
extern Lexeme * get_built_in_name(Lexeme * named_built_in);
extern Lexeme * get_built_in_closure(Lexeme * named_built_in);
extern Lexeme * get_built_in_parameters(Lexeme * built_in);
extern Lexeme * get_built_in_key(Lexeme * built_in);

#endif

