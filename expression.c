#include "expression.h"

#include "ASTree.c"
#include "parser.h"
#include "symbol.h"


#define PREC_TABLE_SIZE 7

int Prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = {
    //  +-. */ R  (  )  i  $
    {R, S, R, S, R, S, R},    // +-.
    {R, R, R, S, R, S, R},    // */
    {S, S, Er, S, R, S, R},   // R
    {S, S, S, S, Eq, S, Er},  // (
    {R, R, R, Er, R, Er, R},  // )
    {R, R, R, Er, R, Er, R},  // i
    {S, S, S, S, Er, S, Er}   // $
};
static Prec_index GetPrecIndex(terminal_kind kind) {
  switch (kind) {
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

    case dollar:
    case e:
      return dollar_E;
    
    default:
      return -1;
  }
}
void CheckExpression(DLList *list, AST *tree, Parser *parser) {
  // E -> i
  if (list->active->data->node->terminal->kind == variableTer ||
      list->active->data->node->terminal->kind == string_litTer ||
      list->active->data->node->terminal->kind == int_litTer ||
      list->active->data->node->terminal->kind == float_litTer) {
    ASTreeCreateNode(E);
    ASTreeInsertFirstChild(tree, list->active->data->node->terminal);
    logger("CheckExpression", "E -> i");
    while (list->last->data->node->terminal->kind != shift) {
      list = DLListDeleteLast(list);
    }
    list = DLListDeleteLast(list); // delete shift
    DLLFirst(list);
  }
  
  
}

void ExpressionParser(Parser *parser) {
  DLList *list = DLListInit();
  logger("ExpressionParser", "Init list");
  AST *ExpTree = ASTreeInit();
  logger("ExpressionParser", "Init ExpTree");
  terminal *Dollar = (terminal *)malloc(sizeof(struct Terminal));
  if (Dollar == NULL) {
    exit(99);
  }
  Dollar->kind = dollar;
  Dollar->code = NULL;

  terminal *Shift = (terminal *)malloc(sizeof(struct Terminal));
  if (Shift == NULL) {
    exit(99);
  }
  Shift->kind = shift;
  Shift->code = NULL;

  terminal *Reduce = (terminal *)malloc(sizeof(struct Terminal));
  if (Reduce == NULL) {
    exit(99);
  }
  Reduce->kind = reduce;
  Reduce->code = NULL;

  bool isEnd = false;
  DLListInsertFirst(list,
                    ASTreeCreateNode(SymbolCreateTerminal(Dollar)));  // dollar
  DLLFirst(list);
  logger("ExpressionParser", "Dollar inserted");

  while (!isEnd) {
    logger("ExpressionParser", "While start");
    if (parser->LLfirst->kind == semicolonTer) 
    {
      
      parser->LLfirst = Reduce;
      logger("ExpressionParser", "Reduce inserted");
    }
    else if (parser->LLfirst->kind == reduce && list->last->data->node->terminal->kind == dollar)
    {
      isEnd = true;
    }
    switch (Prec_table[GetPrecIndex(list->last->data->node->terminal->kind)]
                      [GetPrecIndex(parser->LLfirst->kind)]) {
      case S:
        logger("ExpressionParser", "S");
        DLListInsertAfter(list, ASTreeCreateNode(SymbolCreateTerminal(Shift)));
        DLListNext(list);
        DLListInsertLast(
            list, ASTreeCreateNode(SymbolCreateTerminal(parser->LLfirst)));
        DLListLast(list);
        logger("ExpressionParser", "Pushed");
        
        UpdateLLfirst(parser);
        logger("ExpressionParser", "Updated top stack");
        break;

      case R:
        logger("ExpressionParser", "R");
        DLListInsertAfter(list, ASTreeCreateNode(SymbolCreateTerminal(Reduce)));
        CheckExpression(list, ExpTree, parser);
        logger("ExpressionParser", "Reduced");
        
        break;

      case Eq:
        logger("ExpressionParser", "Eq");
        break;
      case Er:
        logger("ExpressionParser", "Er");
        exit(99);
      default:
        break;
    }
  }
  }
 
