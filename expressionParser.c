#include "expressionParser.h"

#include "ASTree.h"
#include "LList.h"
#include "include.h"
#include "logger.h"
#include "mystring.h"
#include "parser.h"
/**
 * @brief Prints token to stdout
 * 
 * @param val 
 */
void expr_val_printer(expr_val *val) {
  if (val == NULL) {
    return;
  }
  if (val->is_dollar) {
    printf("$ ");
  } else {
    if (val->tree->node->is_terminal) {
      printf("%s ", val->tree->node->terminal->code->data);
    } else {
      printf("E ");
    }
  }
}
/**
 * @brief Print every token in list
 * 
 * @param list List of tokens
 */
void expr_list_printer(expr_list *list) {
  if (list == NULL) {
    return;
  }
  expr_list_el *el = list->first;
  while (el != NULL) {
    expr_val_printer(el->value);
    el = el->next;
  }
  printf("\n");
}
/**
 * @brief Precendence table
 */
int Prec_table[7][7] = {
    //  +-.    */   <>!=    (       )       i       $
    {Reduce, Shift, Err, Shift, Reduce, Shift, Reduce},   // +-.
    {Reduce, Reduce, Err, Shift, Reduce, Shift, Reduce},  // */
    {Err, Err, Err, Shift, Reduce, Shift, Reduce},        // <>!=
    {Shift, Shift, Err, Shift, Same, Shift, Err},         // (
    {Reduce, Reduce, Reduce, Err, Reduce, Err, Reduce},   // )
    {Reduce, Reduce, Reduce, Err, Reduce, Err, Reduce},   // i
    {Shift, Shift, Shift, Shift, Err, Shift, Finish}      // $
};
/**
 * @brief Get the Prec Index object
 * 
 * @param value Value to get index of
 * @return Prec_index 
 */
Prec_index GetPrecIndex(expr_val *value) {
  if (value->is_dollar) {
    return dollar_E;
  }
  switch (value->tree->node->terminal->kind) {
    case plusTer:
    case minusTer:
    case dotTer:
      return plus_minus_dot;

    case multiplyTer:
    case divideTer:
      return multiply_divide;

    case lessTer:
    case lessOrEqualTer:
    case greaterTer:
    case greaterOrEqualTer:
    case equalTer:
    case notEqualTer:
      return relation;

    case leftBracketTer:
      return left_bracket;

    case rightBracketTer:
      return right_bracket;

    case variableTer:
    case string_litTer:
    case int_litTer:
    case float_litTer:
      return identifier;

    default:
      return -1;
  }
}
/**
 * @brief Checks if the token is terminal
 * 
 * @param term Terminal
 * @return true 
 * @return false 
 */
bool ValidateTerminalInExpr(terminal *term) {
  if (term->kind == variableTer || term->kind == string_litTer ||
      term->kind == int_litTer || term->kind == float_litTer ||
      term->kind == leftBracketTer || term->kind == rightBracketTer ||
      term->kind == plusTer || term->kind == minusTer || term->kind == dotTer ||
      term->kind == multiplyTer || term->kind == divideTer ||
      term->kind == lessTer || term->kind == lessOrEqualTer ||
      term->kind == greaterTer || term->kind == greaterOrEqualTer ||
      term->kind == equalTer || term->kind == notEqualTer) {
    return true;
  }
  return false;
}
/**
 * @brief List initialization
 * @return expr_list* 
 */
expr_list *expr_list_init() {
  expr_list *list = (expr_list *)malloc(sizeof(expr_list));
  if (list == NULL) {
    exit(99);
  }
  list->first = NULL;
  list->active = NULL;
  return list;
}
/**
 * @brief Insert value
 * 
 * @param list List to insert to
 * @param value Value to insert
 * @return expr_list* 
 */
expr_list *expr_list_insert(expr_list *list, expr_val *value) {
  if (list == NULL) {
    list = expr_list_init();
  }
  if (list->first == NULL) {
    return expr_list_insert_first(list, value);
  }
  return expr_list_insert_another(list, value);
}
/**
 * @brief Insert value to the first position in the list
 * 
 * @param list List to insert to
 * @param value Value to insert
 * @return expr_list* 
 */
expr_list *expr_list_insert_first(expr_list *list, expr_val *value) {
  if (list == NULL) {
    list = expr_list_init();
  }
  if (list->first == NULL) {
    logger("expr_list_insert_first", "Inserting completely first element");

    expr_list_el *el = (expr_list_el *)malloc(sizeof(expr_list_el));
    if (el == NULL) {
      exit(99);
    }
    el->value = value;
    el->next = NULL;
    list->first = el;
    list->active = el;
    return list;
  } else {
    logger("expr_list_insert_first", "Inserting new first element");
    expr_list_el *old_first = list->first;
    expr_list_el *new_first = (expr_list_el *)malloc(sizeof(expr_list_el));
    if (new_first == NULL) {
      exit(99);
    }
    new_first->value = value;
    new_first->next = old_first;
    list->first = new_first;
    return list;
  }
}
/**
 * @brief Inserts new element after active element
 * 
 * @param list List to insert to
 * @param value Value to be insert
 * @return expr_list* 
 */
expr_list *expr_list_insert_another(expr_list *list, expr_val *value) {
  if (list == NULL) {
    list = expr_list_init();
  }
  if (list->first == NULL) {
    return expr_list_insert_first(list, value);
  } else {
    logger("expr_list_insert_another", "Inserting another element");
    expr_list_el *el = (expr_list_el *)malloc(sizeof(expr_list_el));
    if (el == NULL) {
      exit(99);
    }
    el->value = value;
    el->next = NULL;

    list->active->next = el;
    list->active = el;

    return list;
  }
}
/**
 * @brief Function for find out top terminal in expression list
 * 
 * @param list 
 * @return expr_val* 
 */
expr_val *expr_list_top_terminal(expr_list *list) {
  logger("expr_list_top_terminal", "Getting top terminal");
  if (list == NULL) {
    return NULL;
  }
  expr_val *last_found = NULL;
  expr_list_el *el = list->first;
  while (el != NULL) {
    if (el->value->is_dollar || el->value->tree->node->is_terminal) {
      last_found = el->value;
    }
    el = el->next;
  }

  return last_found;
}
/**
 * @brief Redeuces expression by rules
 * @param list 
 * @return expr_list* 
 */
expr_list *ReduceExpression(expr_list *list) {
  expr_list_el *last_shift = NULL;
  expr_list_el *el = list->first;
  while (el != NULL) {
    if (el->value->action == Shift) {
      last_shift = el;
    }
    el = el->next;
  }
  if (last_shift == NULL) {
    logger("ReduceExpression", "No shift found");
    exit(2);
  }
  AST *reduced = ASTreeCreateNode(SymbolCreateNonterminal(E));

  el = last_shift->next;
  while (el != NULL) {
    logger("ReduceExpression", "Adding child to reduced expression");
    if (!el->value->is_dollar) {
      reduced->children = LListInsertChild(reduced->children, el->value->tree);
    }
    el = el->next;
  }
  logger("ReduceExpression", "Reduced expression");

  // check if reduction was under rules
  if (
      // E -> i
      (reduced->children->first != NULL &&
       reduced->children->first->tree->node->is_terminal == true &&  // i
       reduced->children->first->next == NULL) ||
      //
      // E -> E op E
      (  // E
          reduced->children->first != NULL &&
          reduced->children->first->tree->node->is_terminal == false &&
          reduced->children->first->tree->node->nonterminal == E &&
          // op
          reduced->children->first->next != NULL &&
          reduced->children->first->next->tree->node->is_terminal == true &&
          (reduced->children->first->next->tree->node->terminal->kind ==
               plusTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               minusTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               dotTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               multiplyTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               divideTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               lessTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               lessOrEqualTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               greaterTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               greaterOrEqualTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               equalTer ||
           reduced->children->first->next->tree->node->terminal->kind ==
               notEqualTer) &&
          // E
          reduced->children->first->next->next != NULL &&
          reduced->children->first->next->next->tree->node->is_terminal ==
              false &&
          reduced->children->first->next->next->tree->node->nonterminal == E &&
          // nothing else after
          reduced->children->first->next->next->next == NULL) ||
      //
      // E -> (E)
      (  // (
          reduced->children->first != NULL &&
          reduced->children->first->tree->node->is_terminal == true &&
          reduced->children->first->tree->node->terminal->kind ==
              leftBracketTer &&
          // E
          reduced->children->first->next != NULL &&
          reduced->children->first->next->tree->node->is_terminal == false &&
          reduced->children->first->next->tree->node->nonterminal == E &&
          // )
          reduced->children->first->next->next != NULL &&
          reduced->children->first->next->next->tree->node->is_terminal ==
              true &&
          reduced->children->first->next->next->tree->node->terminal->kind ==
              rightBracketTer &&
          // nothing else after
          reduced->children->first->next->next->next == NULL)) {
  } else {
    logger("ReduceExpression", "Reduction was not under rules");
    exit(2);
  }

  logger("ReduceExpression", "Reduction passed rules");
  expr_val *reduced_val = (expr_val *)malloc(sizeof(expr_val));
  if (reduced_val == NULL) {
    exit(99);
  }
  reduced_val->action = None;
  reduced_val->is_dollar = false;
  reduced_val->tree = reduced;

  expr_list_el *reduced_E = (expr_list_el *)malloc(sizeof(expr_list_el));
  if (reduced_E == NULL) {
    exit(99);
  }
  reduced_E->value = reduced_val;
  reduced_E->next = NULL;

  last_shift->value->action = None;
  last_shift->next = reduced_E;

  list->active = reduced_E;

  return list;
}
/**
 * @brief Function for parsing expression
 * @param parser List of tokens
 * @return ASTree of expression
 */
void ExpressionParser(Parser *parser) {
  logger("ExpressionParser", "Start");
  expr_list *list = expr_list_init();
  expr_val *value = (expr_val *)malloc(sizeof(expr_val));
  if (value == NULL) {
    exit(99);
  }
  value->is_dollar = true;
  list = expr_list_insert(list, value);

  while (true) {
    expr_list_printer(list);
    expr_val *a = expr_list_top_terminal(list);
    expr_val *b = (expr_val *)malloc(sizeof(expr_val));
    if (b == NULL) {
      exit(99);
    }

    b->action = None;
    if (ValidateTerminalInExpr(parser->LLfirst)) {
      logger("ExpressionParser", "b is terminal");
      b->is_dollar = false;
      b->tree = ASTreeCreateNode(SymbolCreateTerminal(parser->LLfirst));
    } else {
      logger("ExpressionParser", "a is dollar");

      b->is_dollar = true;
      b->tree = NULL;
    }
    printf("%d\n", parser->LLfirst->kind);
    switch (Prec_table[GetPrecIndex(a)][GetPrecIndex(b)]) {
      case Same:
        logger("ExpressionParser", "Same");
        list = expr_list_insert(list, b);
        UpdateLLfirst(parser);
        break;

      case Shift:
        logger("ExpressionParser", "Shift");
        a->action = Shift;
        list = expr_list_insert(list, b);
        UpdateLLfirst(parser);
        break;

      case Reduce:
        logger("ExpressionParser", "Reduce");
        list = ReduceExpression(list);
        break;

      case Err:
        parser->buffer = GetTerminal();
        if (parser->buffer->kind == leftCurlyBracketTer) {
          logger("ExpressionParser",
                 "Wrongly included right bracket into EXP -> not considered as "
                 "error in EXP");
          parser->current->children = LListInsertChild(
              parser->current->children, list->first->next->value->tree);
          return;
        } else {
          logger("ExpressionParser", "Error");
          exit(2);
        }

      case Finish:
        logger("ExpressionParser", "Finish");
        parser->current->children = LListInsertChild(
            parser->current->children, list->first->next->value->tree);
        return;
    }
  }
}