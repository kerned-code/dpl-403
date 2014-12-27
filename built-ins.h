/* Tucker Meyers: CS403 Programming Language Project
 * Built-Ins Module
 */

#ifndef BUILT_INS_HEADER
#define BUILT_INS_HEADER

extern Lexeme * execute(Lexeme * built_in, Lexeme * env);
/* Type operations */
extern Lexeme *   op_type(void);
extern Lexeme * name_type(void);
extern Lexeme *   ex_type(Lexeme * env);
extern Lexeme *   op_is_null(void);
extern Lexeme * name_is_null(void);
extern Lexeme * name_is_null_pre(void);
extern Lexeme *   ex_is_null(Lexeme * env);
extern Lexeme *   op_is_int(void);
extern Lexeme * name_is_int(void);
extern Lexeme * name_is_int_pre(void);
extern Lexeme *   ex_is_int(Lexeme * env);
extern Lexeme *   op_is_float(void);
extern Lexeme * name_is_float(void);
extern Lexeme * name_is_float_pre(void);
extern Lexeme *   ex_is_float(Lexeme * env);
extern Lexeme *   op_is_string(void);
extern Lexeme * name_is_string(void);
extern Lexeme * name_is_string_pre(void);
extern Lexeme *   ex_is_string(Lexeme * env);
extern Lexeme *   op_is_boolean(void);
extern Lexeme * name_is_boolean(void);
extern Lexeme * name_is_boolean_pre(void);
extern Lexeme *   ex_is_boolean(Lexeme * env);
extern Lexeme *   op_is_array(void);
extern Lexeme * name_is_array(void);
extern Lexeme * name_is_array_pre(void);
extern Lexeme *   ex_is_array(Lexeme * env);
extern Lexeme *   op_is_pair(void);
extern Lexeme * name_is_pair(void);
extern Lexeme * name_is_pair_pre(void);
extern Lexeme * name_is_list(void);
extern Lexeme * name_is_list_pre(void);
extern Lexeme *   ex_is_pair(Lexeme * env);
extern Lexeme *   op_is_identifier(void);
extern Lexeme * name_is_identifier(void);
extern Lexeme * name_is_identifier_pre(void);
extern Lexeme *   ex_is_identifier(Lexeme * env);
extern Lexeme *   op_is_closure(void);
extern Lexeme * name_is_closure(void);
extern Lexeme * name_is_closure_pre(void);
extern Lexeme *   ex_is_closure(Lexeme * env);
extern Lexeme *   op_is_scope(void);
extern Lexeme * name_is_scope(void);
extern Lexeme * name_is_scope_pre(void);
extern Lexeme *   ex_is_scope(Lexeme * env);
extern Lexeme *   op_to_string(void);
extern Lexeme * name_to_string(void);
extern Lexeme *   ex_to_string(Lexeme * env);
/* Scope operations */
extern Lexeme *   op_get(void);
extern Lexeme * name_get(void);
extern Lexeme * name_get_dot(void);
extern Lexeme *   ex_get(Lexeme * env);
extern Lexeme *   op_set(void);
extern Lexeme * name_set(void);
extern Lexeme * name_set_dot(void);
extern Lexeme *   ex_set(Lexeme * env);
/* Arithmetic operations */
extern Lexeme *   op_add(void);
extern Lexeme * name_add(void);
extern Lexeme * name_add_ext(void);
extern Lexeme *   ex_add(Lexeme * env);
extern Lexeme *   op_sub(void);
extern Lexeme * name_sub(void);
extern Lexeme * name_sub_ext(void);
extern Lexeme *   ex_sub(Lexeme * env);
extern Lexeme *   op_mult(void);
extern Lexeme * name_mult(void);
extern Lexeme * name_mult_ext(void);
extern Lexeme *   ex_mult(Lexeme * env);
extern Lexeme *   op_div(void);
extern Lexeme * name_div(void);
extern Lexeme * name_div_ext(void);
extern Lexeme *   ex_div(Lexeme * env);
extern Lexeme *   op_power(void);
extern Lexeme * name_power(void);
extern Lexeme * name_power_ext(void);
extern Lexeme *   ex_power(Lexeme * env);
/* Comparision operations */
extern Lexeme *   op_equal(void);
extern Lexeme * name_equal(void);
extern Lexeme * name_equal_in(void);
extern Lexeme *   ex_equal(Lexeme * env);
extern Lexeme *   op_not_equal(void);
extern Lexeme * name_not_equal(void);
extern Lexeme * name_not_equal_in(void);
extern Lexeme *   ex_not_equal(Lexeme * env);
extern Lexeme *   op_greater(void);
extern Lexeme * name_greater(void);
extern Lexeme * name_greater_in(void);
extern Lexeme *   ex_greater(Lexeme * env);
extern Lexeme *   op_less(void);
extern Lexeme * name_less(void);
extern Lexeme * name_less_in(void);
extern Lexeme *   ex_less(Lexeme * env);
extern Lexeme *   op_not_less(void);
extern Lexeme * name_not_less(void);
extern Lexeme * name_not_less_in(void);
extern Lexeme *   ex_not_less(Lexeme * env);
extern Lexeme *   op_not_greater(void);
extern Lexeme * name_not_greater(void);
extern Lexeme * name_not_greater_in(void);
extern Lexeme *   ex_not_greater(Lexeme * env);
/* Boolean operations */
extern Lexeme *   op_not(void);
extern Lexeme * name_not(void);
extern Lexeme * name_not_pre(void);
extern Lexeme *   ex_not(Lexeme * env);
extern Lexeme *   op_and(void);
extern Lexeme * name_and(void);
extern Lexeme * name_and_ext(void);
extern Lexeme *   ex_and(Lexeme * env);
extern Lexeme *   op_or(void);
extern Lexeme * name_or(void);
extern Lexeme * name_or_ext(void);
extern Lexeme *   ex_or(Lexeme * env);
/* List operations */
extern Lexeme *   op_pair(void);
extern Lexeme * name_pair(void);
extern Lexeme * name_pair_comma(void);
extern Lexeme *   ex_pair(Lexeme * env);
extern Lexeme *   op_left(void);
extern Lexeme * name_left(void);
extern Lexeme *   ex_left(Lexeme * env);
extern Lexeme *   op_right(void);
extern Lexeme * name_right(void);
extern Lexeme *   ex_right(Lexeme * env);
extern Lexeme *   op_set_left(void);
extern Lexeme * name_set_left(void);
extern Lexeme *   ex_set_left(Lexeme * env);
extern Lexeme *   op_set_right(void);
extern Lexeme * name_set_right(void);
extern Lexeme *   ex_set_right(Lexeme * env);
/* Array operations */
extern Lexeme *   op_array(void);
extern Lexeme * name_array(void);
extern Lexeme *   ex_array(Lexeme * env);
extern Lexeme *   op_array_get(void);
extern Lexeme * name_array_get(void);
extern Lexeme *   ex_array_get(Lexeme * env);
extern Lexeme *   op_array_set(void);
extern Lexeme * name_array_set(void);
extern Lexeme *   ex_array_set(Lexeme * env);
/* String operations */
extern Lexeme *   op_char(void);
extern Lexeme * name_char(void);
extern Lexeme *   ex_char(Lexeme * env);
extern Lexeme *   op_concat(void);
extern Lexeme * name_concat(void);
extern Lexeme *   ex_concat(Lexeme * env);
extern Lexeme *   op_compare(void);
extern Lexeme * name_compare(void);
extern Lexeme *   ex_compare(Lexeme * env);
/* Input/output operations */
extern Lexeme *   op_print(void);
extern Lexeme * name_print(void);
extern Lexeme *   ex_print(Lexeme * env);
extern Lexeme *   op_println(void);
extern Lexeme * name_println(void);
extern Lexeme *   ex_println(Lexeme * env);
extern Lexeme *   op_read_terminal(void);
extern Lexeme * name_read_terminal(void);
extern Lexeme *   ex_read_terminal(Lexeme * env);

#endif

