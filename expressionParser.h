#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "ASTree.h"
#include "include.h"
#include "parser.h"
/**
 * @enum Precedence table actions
 */
typedef enum { None, Shift, Reduce, Same, Err, Finish } action;

/**
 * @enum Precedence table index
 */
typedef enum {
  plus_minus_dot,
  multiply_divide,
  relation,
  left_bracket,
  right_bracket,
  identifier,
  dollar_E
} Prec_index;

/** Values of the element */
typedef struct expr_value {
  action action;
  AST *tree;
  bool is_dollar;
} expr_val;

/** Element of expr list */
typedef struct expr_list_el {
  expr_val *value;
  struct expr_list_el *next;
} expr_list_el;

/** List of expressions */
typedef struct expr_list {
  expr_list_el *first;
  expr_list_el *active;
} expr_list;

expr_list *expr_list_init();

expr_list *expr_list_insert(expr_list *list, expr_val *value);

expr_list *expr_list_insert_first(expr_list *list, expr_val *value);

expr_list *expr_list_insert_another(expr_list *list, expr_val *value);

void ExpressionParser(Parser *parser);

#endif