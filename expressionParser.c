/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file expressionParser.c
 * @authors Jiri Stipek xstipe02, Stefan Peknik xpekni01
 * @brief expressionParser
 * @date 2022-11-16
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "expressionParser.h"

#include "ASTree.h"
#include "LList.h"
#include "error.h"
#include "include.h"
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
    {Reduce, Shift, Reduce, Shift, Reduce, Shift, Reduce},   // +-.
    {Reduce, Reduce, Reduce, Shift, Reduce, Shift, Reduce},  // */
    {Shift, Shift, Err, Shift, Reduce, Shift, Reduce},       // <>!=
    {Shift, Shift, Shift, Shift, Same, Shift, Err},          // (
    {Reduce, Reduce, Reduce, Err, Reduce, Err, Reduce},      // )
    {Reduce, Reduce, Reduce, Err, Reduce, Err, Reduce},      // i
    {Shift, Shift, Shift, Shift, Err, Shift, Finish}         // $
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
    case nullTer:
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
      term->kind == nullTer || term->kind == leftBracketTer ||
      term->kind == rightBracketTer || term->kind == plusTer ||
      term->kind == minusTer || term->kind == dotTer ||
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
    ErrorExit(99, "Malloc failed!");
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
    expr_list_el *el = (expr_list_el *)malloc(sizeof(expr_list_el));
    if (el == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    el->value = value;
    el->next = NULL;
    list->first = el;
    list->active = el;
    return list;
  } else {
    expr_list_el *old_first = list->first;
    expr_list_el *new_first = (expr_list_el *)malloc(sizeof(expr_list_el));
    if (new_first == NULL) {
      ErrorExit(99, "Malloc failed!");
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
    expr_list_el *el = (expr_list_el *)malloc(sizeof(expr_list_el));
    if (el == NULL) {
      ErrorExit(99, "Malloc failed!");
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
    ErrorExit(2, "Syntax error");
  }
  AST *reduced = ASTreeCreateNode(SymbolCreateNonterminal(E));

  el = last_shift->next;
  while (el != NULL) {
    if (!el->value->is_dollar) {
      reduced->children = LListInsertChild(reduced->children, el->value->tree);
    }
    el = el->next;
  }

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
    ErrorExit(2, "Syntax error");
  }

  // workaround to remove () and reorder E op E
  if (  // (
      reduced->children->first != NULL &&
      reduced->children->first->tree->node->is_terminal == true &&
      reduced->children->first->tree->node->terminal->kind == leftBracketTer) {
    LList_element *el = reduced->children->first->next;
    el->next = NULL;
    reduced->children->first = el;
    reduced->children->active = el;
  }
  if (  // E
      reduced->children->first != NULL &&
      reduced->children->first->tree->node->is_terminal == false &&
      reduced->children->first->tree->node->nonterminal == E &&
      // op
      reduced->children->first->next != NULL &&
      reduced->children->first->next->tree->node->is_terminal == true &&
      (reduced->children->first->next->tree->node->terminal->kind == plusTer ||
       reduced->children->first->next->tree->node->terminal->kind == minusTer ||
       reduced->children->first->next->tree->node->terminal->kind == dotTer ||
       reduced->children->first->next->tree->node->terminal->kind ==
           multiplyTer ||
       reduced->children->first->next->tree->node->terminal->kind ==
           divideTer ||
       reduced->children->first->next->tree->node->terminal->kind == lessTer ||
       reduced->children->first->next->tree->node->terminal->kind ==
           lessOrEqualTer ||
       reduced->children->first->next->tree->node->terminal->kind ==
           greaterTer ||
       reduced->children->first->next->tree->node->terminal->kind ==
           greaterOrEqualTer ||
       reduced->children->first->next->tree->node->terminal->kind == equalTer ||
       reduced->children->first->next->tree->node->terminal->kind ==
           notEqualTer) &&
      // E
      reduced->children->first->next->next != NULL &&
      reduced->children->first->next->next->tree->node->is_terminal == false &&
      reduced->children->first->next->next->tree->node->nonterminal == E &&
      // nothing else after
      reduced->children->first->next->next->next == NULL) {
    AST *new = ASTreeCreateNode(SymbolCreateNonterminal(E));
    new->children =
        LListInsertChild(new->children, reduced->children->first->next->tree);
    new->children->first->tree->children = LListInsertChild(
        new->children->first->tree->children, reduced->children->first->tree);
    new->children->first->tree->children =
        LListInsertChild(new->children->first->tree->children,
                         reduced->children->first->next->next->tree);
    reduced = new;
  }
  expr_val *reduced_val = (expr_val *)malloc(sizeof(expr_val));
  if (reduced_val == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  reduced_val->action = None;
  reduced_val->is_dollar = false;
  reduced_val->tree = reduced;

  expr_list_el *reduced_E = (expr_list_el *)malloc(sizeof(expr_list_el));
  if (reduced_E == NULL) {
    ErrorExit(99, "Malloc failed!");
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
  expr_list *list = expr_list_init();
  expr_val *value = (expr_val *)malloc(sizeof(expr_val));
  if (value == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  value->is_dollar = true;
  list = expr_list_insert(list, value);

  while (true) {
    expr_val *a = expr_list_top_terminal(list);
    expr_val *b = (expr_val *)malloc(sizeof(expr_val));
    if (b == NULL) {
      ErrorExit(99, "Malloc failed!");
    }

    b->action = None;
    if (ValidateTerminalInExpr(parser->LLfirst)) {
      b->is_dollar = false;
      b->tree = ASTreeCreateNode(SymbolCreateTerminal(parser->LLfirst));
    } else {
      b->is_dollar = true;
      b->tree = NULL;
    }
    switch (Prec_table[GetPrecIndex(a)][GetPrecIndex(b)]) {
      case Same:
        list = expr_list_insert(list, b);
        UpdateLLfirst(parser);
        break;

      case Shift:
        a->action = Shift;
        list = expr_list_insert(list, b);
        UpdateLLfirst(parser);
        break;

      case Reduce:
        list = ReduceExpression(list);
        break;

      case Err:
        parser->buffer = GetTerminal();
        if (parser->buffer->kind == leftCurlyBracketTer) {
          parser->current->children = LListInsertChild(
              parser->current->children, list->first->next->value->tree);

          return;
        }
        ErrorExit(2, "Syntax error");
        return;

      case Finish:
        parser->current->children = LListInsertChild(
            parser->current->children, list->first->next->value->tree);
        return;
    }
  }
}
