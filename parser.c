
#include "parser.h"

#include "ASTreeGraphGenerator.h"
#include "codegen.c"
#include "expressionParser.h"
#include "logger.h"
#include "scanner.h"
#include "semanticAnalysis.h"
#include "symbol.h"

void UpdateLLfirst(Parser *parser) {
  if (parser->buffer == NULL) {
    parser->LLfirst = GetTerminal();
    return;
  }
  parser->LLfirst = parser->buffer;
  parser->buffer = NULL;
}

terminal *GetTerminal() {
  logger("GetTerminal", "Getting terminal");
  Lexeme *next = GetLexeme();
  terminal *term = (terminal *)malloc(sizeof(struct Terminal));
  if (term == NULL) {
    exit(99);
  }
  switch (next->kind) {
    case MULTIPLY:
      term->kind = multiplyTer;
      term->code = next->code;
      break;
    case DIVISION:
      term->kind = divideTer;
      term->code = next->code;
      break;
    case MINUS:
      term->kind = minusTer;
      term->code = next->code;
      break;
    case PLUS:
      term->kind = plusTer;
      term->code = next->code;
      break;
    case INTEGER:
      term->kind = int_litTer;
      term->code = next->code;
      break;
    case FLOAT:
      term->kind = float_litTer;
      term->code = next->code;
      break;
    case CONCAT:
      term->kind = dotTer;
      term->code = next->code;
      break;
    case ASSIGN:
      term->kind = assignTer;
      term->code = next->code;
      break;
    case EQUAL:
      term->kind = equalTer;
      term->code = next->code;
      break;
    case GT:
      term->kind = greaterTer;
      term->code = next->code;
      break;
    case GE:
      term->kind = greaterOrEqualTer;
      term->code = next->code;
      break;
    case LT:
      term->kind = lessTer;
      term->code = next->code;
      break;
    case LE:
      term->kind = lessOrEqualTer;
      term->code = next->code;
      break;
    case STARTPROLOG:
      term->kind = startPrologTer;
      term->code = next->code;
      break;
    case SEMICOLON:
      term->kind = semicolonTer;
      term->code = next->code;
      break;
    case LEFTBRACKET:
      term->kind = leftBracketTer;
      term->code = next->code;
      break;
    case RIGHTBRACKET:
      term->kind = rightBracketTer;
      term->code = next->code;
      break;
    case LEFTCURLYBRACKET:
      term->kind = leftCurlyBracketTer;
      term->code = next->code;
      break;
    case RIGHTCURLYBRACKET:
      term->kind = rightCurlyBracketTer;
      term->code = next->code;
      break;
    case ENDPROLOG:
      term->kind = endPrologTer;
      term->code = next->code;
      break;
    case VARIABLE:
      term->kind = variableTer;
      term->code = next->code;
      break;
    case FUNCTION:
      term->kind = function_idTer;
      term->code = next->code;
      break;
    case KEYWORD:
      if (strcmp(next->code->data, "if") == 0) {
        term->kind = ifTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "else") == 0) {
        term->kind = elseTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "while") == 0) {
        term->kind = whileTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "function") == 0) {
        term->kind = functionTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "null") == 0) {
        term->kind = nullTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "return") == 0) {
        term->kind = returnTer;
        term->code = next->code;
        break;
      }
      exit(1);
    case DATATYPE:
      if (strcmp(next->code->data, "int") == 0 ||
          strcmp(next->code->data, "?int") == 0) {
        term->kind = intTypeTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "float") == 0 ||
                 strcmp(next->code->data, "?float") == 0) {
        term->kind = floatTypeTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "string") == 0 ||
                 strcmp(next->code->data, "?string") == 0) {
        term->kind = stringTypeTer;
        term->code = next->code;
        break;
      } else if (strcmp(next->code->data, "void") == 0) {
        term->kind = voidTypeTer;
        term->code = next->code;
        break;
      }
      exit(1);
    case COLON:
      term->kind = colonTer;
      term->code = next->code;
      break;
    case COMMA:
      term->kind = commaTer;
      term->code = next->code;
      break;
    case ENDOFFILE:
      term->kind = endOfFileTer;
      term->code = next->code;
      break;
    case NOTEQUAL:
      term->kind = notEqualTer;
      term->code = next->code;
      break;
    case STRING:
      term->kind = string_litTer;
      term->code = next->code;
      break;
    default:
      exit(1);
  }
  free(next);
  return term;
}

int ChooseRule(nonterminal_kind nonterminal, Parser *parser) {
  logger("ChooseRule", "Choosing rule");

  switch (nonterminal) {
    case START:
      return -1;

    case PROG:
      switch (parser->LLfirst->kind) {
        case startPrologTer:
          return 2;
        default:
          return -1;
      }

    case START_PROLOG:
      switch (parser->LLfirst->kind) {
        case startPrologTer:
          return 3;
        default:
          return -1;
      }

    case END_PROLOG:
      switch (parser->LLfirst->kind) {
        case endPrologTer:
          return 5;
        case endOfFileTer:
          return 4;
        default:
          return -1;
      }

    case CODE:
      switch (parser->LLfirst->kind) {
        case ifTer:
          return 7;
        case whileTer:
          return 7;
        case functionTer:
          return 8;
        case function_idTer:
          return 7;
        case returnTer:
          return 7;
        case leftCurlyBracketTer:
          return 7;

        // not in grammar
        case leftBracketTer:
          return 7;
        case variableTer:
          return 7;
        case string_litTer:
          return 7;
        case float_litTer:
          return 7;
        case int_litTer:
          return 7;
        case nullTer:
          return 7;
          // not in grammar

        default:
          return 6;
      }

    case BODY:
      switch (parser->LLfirst->kind) {
        case ifTer:
          return 9;
        case whileTer:
          return 9;
        case function_idTer:
          return 9;
        case returnTer:
          return 9;
        case leftCurlyBracketTer:
          return 9;

        // not in grammar
        case leftBracketTer:
          return 9;
        case variableTer:
          return 9;
        case string_litTer:
          return 9;
        case float_litTer:
          return 9;
        case int_litTer:
          return 9;
        case nullTer:
          return 9;
          // not in grammar

        default:
          return 10;
      }

    case INNER_SCOPE:
      switch (parser->LLfirst->kind) {
        case ifTer:
          return 11;
        case whileTer:
          return 13;
        case function_idTer:
          return 15;
        case returnTer:
          return 12;
        case leftCurlyBracketTer:
          return 14;

        // not in grammar
        case variableTer:
          logger("ChooseRule", "variableTer");
          parser->buffer = GetTerminal();
          if (parser->buffer->kind == assignTer) {
            return 18;
          }
          return 17;
        case leftBracketTer:
          return 17;
        case string_litTer:
          return 17;
        case float_litTer:
          return 17;
        case int_litTer:
          return 17;
        case nullTer:
          return 17;
          // not in grammar

        default:
          return -1;
      }

    case RETURN_VALUE:
      switch (parser->LLfirst->kind) {
        case variableTer:
          return 22;
        case string_litTer:
          return 22;
        case float_litTer:
          return 22;
        case int_litTer:
          return 22;
        case nullTer:
          return 22;
        default:
          return 21;
      }

    case RETURN_TYPE:
      switch (parser->LLfirst->kind) {
        case intTypeTer:
          return 24;
        case floatTypeTer:
          return 24;
        case stringTypeTer:
          return 24;
        case voidTypeTer:
          return 25;
        default:
          return -1;
      }

    case FUNC_CALL:
      switch (parser->LLfirst->kind) {
        case function_idTer:
          return 26;
        default:
          return -1;
      }

    case FUNC_CALL_ARGS:
      switch (parser->LLfirst->kind) {
        case variableTer:
          return 28;
        case string_litTer:
          return 28;
        case float_litTer:
          return 28;
        case int_litTer:
          return 28;
        case nullTer:
          return 28;
        default:
          return 27;
      }

    case NEXT_ARG:
      switch (parser->LLfirst->kind) {
        case commaTer:
          return 30;
        default:
          return 29;
      }

    case ARG:
      switch (parser->LLfirst->kind) {
        case variableTer:
          return 31;
        case string_litTer:
          return 32;
        case float_litTer:
          return 32;
        case int_litTer:
          return 32;
        case nullTer:
          return 32;
        default:
          return -1;
      }

    case LITERAL:
      switch (parser->LLfirst->kind) {
        case string_litTer:
          return 36;
        case float_litTer:
          return 33;
        case int_litTer:
          return 34;
        case nullTer:
          return 35;
        default:
          return -1;
      }

    case FUNC_DECLARE:
      switch (parser->LLfirst->kind) {
        case functionTer:
          return 42;
        default:
          return -1;
      }

    case FUNC_DECLARE_BODY:
      switch (parser->LLfirst->kind) {
        case intTypeTer:
          return 44;
        case floatTypeTer:
          return 44;
        case stringTypeTer:
          return 44;
        case commaTer:
          return 45;
        default:
          return 43;
      }

    case ARG_TYPE:
      switch (parser->LLfirst->kind) {
        case intTypeTer:
          return 47;
        case floatTypeTer:
          return 48;
        case stringTypeTer:
          return 46;
        default:
          return -1;
      }

    case IF_ELSE:
      switch (parser->LLfirst->kind) {
        case ifTer:
          return 49;
        default:
          return -1;
      }

    case RIGHT_SIDE:
      switch (parser->LLfirst->kind) {
        case function_idTer:
          return 37;
        case leftBracketTer:
          return 38;
        case variableTer:
          return 38;
        case string_litTer:
          return 38;
        case float_litTer:
          return 38;
        case int_litTer:
          return 38;
        case nullTer:
          return 38;
        default:
          return 50;
      }

    default:
      return -1;
  }
}

Parser *ParserCreate() {
  Parser *parser = malloc(sizeof(struct Parser));
  if (parser == NULL) {
    exit(99);
  }
  logger("parser", "created parser");
  parser->root = ASTreeCreateNode(SymbolCreateNonterminal(START));
  logger("parser", "created root node");
  parser->current = parser->root;
  logger("parser", "set current node to root");
  parser->buffer = NULL;
  logger("parser", "created buffer");
  UpdateLLfirst(parser);
  logger("parser", "updated LLfirst");
  return parser;
}

void PrepareCurrentNode(Parser *parser, nonterminal_kind nonterminal) {
  logger("PrepareCurrentNode", "saved current node");
  AST *child = ASTreeCreateNode(SymbolCreateNonterminal(nonterminal));
  logger("PrepareCurrentNode", "created child node");
  parser->current->children =
      LListInsertChild(parser->current->children, child);
  logger("PrepareCurrentNode", "inserted child node");
  parser->current = child;
  logger("PrepareCurrentNode", "set current node to child");
}

void ConsumeTerminal(Parser *parser) {
  parser->current->children =
      LListInsertChild(parser->current->children,
                       ASTreeCreateNode(SymbolCreateTerminal(parser->LLfirst)));
  UpdateLLfirst(parser);
  logger("ConsumeTerminal", "consumed terminal");
}

int main() {
  Parser *parser = ParserCreate();
  rule_START(parser);
  logger("parser", "finished parsing");
  SemanticAnalysis(parser->root);
  // ASTreePrintChildren(parser->root);
  codegen(parser->root);
}

// rules down from here
void rule_START(Parser *parser) {
  logger("rule_START", "started");
  rule_PROG(parser);
  logger("rule_START", "finished");
}

void rule_PROG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, PROG);
  logger("rule_PROG", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 2:
      logger("rule_PROG", "chose rule 2");
      rule_START_PROLOG(parser);
      rule_CODE(parser);
      rule_END_PROLOG(parser);
      parser->current = current;
      break;
    default:
      logger("rule_PROG", "error");
      exit(2);
  }
  logger("rule_PROG", "finished rule PROG");
  parser->current = current;
  logger("rule_PROG", "set current node to saved node");
}

void rule_START_PROLOG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, START_PROLOG);
  logger("rule_START_PROLOG", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 3:
      logger("rule_START_PROLOG", "chose rule 3");

      logger("rule_START_PROLOG", "checking startPrologTer");
      if (parser->LLfirst->kind != startPrologTer) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking function_idTer");
      if (parser->LLfirst->kind != function_idTer ||
          strcmp(parser->LLfirst->code->data, "declare") != 0) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking leftBracketTer");
      if (parser->LLfirst->kind != leftBracketTer) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking function_idTer");
      if (parser->LLfirst->kind != function_idTer ||
          strcmp(parser->LLfirst->code->data, "strict_types") != 0) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking assignTer");
      if (parser->LLfirst->kind != assignTer) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking intTer");
      if (parser->LLfirst->kind != int_litTer ||
          strcmp(parser->LLfirst->code->data, "1") != 0) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking rightBracketTer");
      if (parser->LLfirst->kind != rightBracketTer) {
        exit(2);
      }

      ConsumeTerminal(parser);

      logger("rule_START_PROLOG", "checking semicolonTer");
      if (parser->LLfirst->kind != semicolonTer) {
        exit(2);
      }

      ConsumeTerminal(parser);

      break;

    default:
      logger("rule_START_PROLOG", "error");
      exit(2);
  }

  logger("rule_START_PROLOG", "finished rule START_PROLOG");
  parser->current = current;
  logger("rule_START_PROLOG", "set current node to saved node");
}

void rule_END_PROLOG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, END_PROLOG);
  logger("rule_END_PROLOG", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 4:
      logger("rule_END_PROLOG", "chose rule 4");
      logger("rule_END_PROLOG", "checking endOfFileTer");
      if (parser->LLfirst->kind != endOfFileTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;

    case 5:
      logger("rule_END_PROLOG", "chose rule 5");

      logger("rule_END_PROLOG", "checking endPrologTer");
      if (parser->LLfirst->kind != endPrologTer) {
        exit(2);
      }
      ConsumeTerminal(parser);

      logger("rule_END_PROLOG", "checking endOfFileTer");
      if (parser->LLfirst->kind != endOfFileTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;

    default:
      logger("rule_END_PROLOG", "error");
      exit(2);
  }
  logger("rule_END_PROLOG", "finished rule END_PROLOG");
  parser->current = current;
  logger("rule_START_PROLOG", "set current node to saved node");
}

void rule_CODE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, CODE);
  logger("rule_CODE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 7:
      logger("rule_CODE", "chose rule 7");
      rule_INNER_SCOPE(parser);
      rule_CODE(parser);
      break;
    case 8:
      logger("rule_CODE", "chose rule 8");
      rule_FUNC_DECLARE(parser);
      rule_CODE(parser);
      break;
    case 6:
      logger("rule_CODE", "chose rule 6");
      break;

    default:
      logger("rule_CODE", "error");
      exit(2);
  }

  logger("rule_CODE", "finished rule CODE");
  parser->current = current;
  logger("rule_CODE", "set current node to saved node");
}

void rule_BODY(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, BODY);
  logger("rule_BODY", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 9:
      logger("rule_BODY", "chose rule 9");
      rule_INNER_SCOPE(parser);
      rule_BODY(parser);
      break;
    case 10:
      logger("rule_BODY", "chose rule 10");
      break;

    default:
      exit(2);
  }

  logger("rule_BODY", "finished rule BODY");
  parser->current = current;
  logger("rule_BODY", "set current node to saved node");
}

void rule_INNER_SCOPE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, INNER_SCOPE);
  logger("rule_INNER_SCOPE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 11:
      logger("rule_INNER_SCOPE", "chose rule 11");
      rule_IF_ELSE(parser);
      break;
    case 12:
      logger("rule_INNER_SCOPE", "chose rule 12");
      logger("rule_INNER_SCOPE", "checking returnTer");
      if (parser->LLfirst->kind != returnTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_RETURN_VALUE(parser);
      logger("rule_INNER_SCOPE", "checking semicolonTer");
      if (parser->LLfirst->kind != semicolonTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 13:
      logger("rule_INNER_SCOPE", "chose rule 13");
      logger("rule_INNER_SCOPE", "checking whileTer");
      if (parser->LLfirst->kind != whileTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_INNER_SCOPE", "checking leftBracketTer");
      if (parser->LLfirst->kind != leftBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_EXP(parser);
      logger("rule_INNER_SCOPE", "checking rightBracketTer");
      if (parser->LLfirst->kind != rightBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_INNER_SCOPE", "checking leftCurlyBracketTer");
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      logger("rule_INNER_SCOPE", "checking rightCurlyBracketTer");
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 14:
      logger("rule_INNER_SCOPE", "chose rule 14");
      logger("rule_INNER_SCOPE", "checking leftCurlyBracketTer");
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      logger("rule_INNER_SCOPE", "checking rightCurlyBracketTer");
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 15:
      logger("rule_INNER_SCOPE", "chose rule 15");
      rule_FUNC_CALL(parser);
      break;
    case 17:
      logger("rule_INNER_SCOPE", "chose rule 17");
      rule_EXP(parser);
      break;
    case 18:
      logger("rule_INNER_SCOPE", "chose rule 18");
      logger("rule_INNER_SCOPE", "checking variableTer");
      if (parser->LLfirst->kind != variableTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_INNER_SCOPE", "checking assignTer");
      if (parser->LLfirst->kind != assignTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_RIGHT_SIDE(parser);
      break;
    default:
      logger("rule_INNER_SCOPE", "error");
      exit(2);
  }
  logger("rule_INNER_SCOPE", "finished rule INNER_SCOPE");
  parser->current = current;
  logger("rule_INNER_SCOPE", "set current node to saved node");
}

void rule_RIGHT_SIDE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, RIGHT_SIDE);
  logger("rule_RIGHT_SIDE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 37:
      logger("rule_RIGHT_SIDE", "chose rule 20");
      rule_FUNC_CALL(parser);
      break;
    case 38:
      logger("rule_RIGHT_SIDE", "chose rule 19");
      rule_EXP(parser);
      logger("rule_RIGHT_SIDE", "checking semicolonTer");
      if (parser->LLfirst->kind != semicolonTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_RIGHT_SIDE", "error");
      exit(2);
  }
  logger("rule_RIGHT_SIDE", "finished rule RIGHT_SIDE");
  parser->current = current;
  logger("rule_RIGHT_SIDE", "set current node to saved node");
}

void rule_RETURN_VALUE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, RETURN_VALUE);
  logger("rule_RETURN_VALUE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 21:
      logger("rule_RETURN_VALUE", "chose rule 21");
      break;
    case 22:
      logger("rule_RETURN_VALUE", "chose rule 22");
      rule_EXP(parser);
      break;
    default:
      logger("rule_RETURN_VALUE", "error");
      exit(2);
  }
  logger("rule_RETURN_VALUE", "finished rule RETURN_VALUE");
  parser->current = current;
  logger("rule_RETURN_VALUE", "set current node to saved node");
}

void rule_RETURN_TYPE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, RETURN_TYPE);
  logger("rule_RETURN_TYPE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 24:
      logger("rule_RETURN_TYPE", "chose rule 24");
      rule_ARG_TYPE(parser);
      break;
    case 25:
      logger("rule_RETURN_TYPE", "chose rule 25");
      logger("rule_RETURN_TYPE", "checking voidTypeTer");
      if (parser->LLfirst->kind != voidTypeTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_RETURN_TYPE", "error");
      exit(2);
  }
  logger("rule_RETURN_TYPE", "finished rule RETURN_TYPE");
  parser->current = current;
  logger("rule_RETURN_TYPE", "set current node to saved node");
}

void rule_FUNC_CALL(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_CALL);
  logger("rule_FUNC_CALL", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 26:
      logger("rule_FUNC_CALL", "chose rule 26");
      logger("rule_FUNC_CALL", "checking function_idTer");
      if (parser->LLfirst->kind != function_idTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_FUNC_CALL", "checking leftBracketTer");
      if (parser->LLfirst->kind != leftBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_FUNC_CALL_ARGS(parser);
      logger("rule_FUNC_CALL", "checking rightBracketTer");
      if (parser->LLfirst->kind != rightBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_FUNC_CALL", "checking semicolonTer");
      if (parser->LLfirst->kind != semicolonTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_FUNC_CALL", "error");
      exit(2);
  }
  logger("rule_FUNC_CALL", "finished rule FUNC_CALL");
  parser->current = current;
  logger("rule_FUNC_CALL", "set current node to saved node");
}

void rule_FUNC_CALL_ARGS(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_CALL_ARGS);
  logger("rule_FUNC_CALL_ARGS", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 27:
      logger("rule_FUNC_CALL_ARGS", "chose rule 29");
      break;
    case 28:
      logger("rule_FUNC_CALL_ARGS", "chose rule 30");
      rule_ARG(parser);
      rule_NEXT_ARG(parser);
      break;
    default:
      logger("rule_FUNC_CALL_ARGS", "error");
      exit(2);
  }
  logger("rule_FUNC_CALL_ARGS", "finished rule FUNC_CALL_ARGS");
  parser->current = current;
  logger("rule_FUNC_CALL_ARGS", "set current node to saved node");
}

void rule_NEXT_ARG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, NEXT_ARG);
  logger("rule_NEXT_ARG", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 29:
      logger("rule_NEXT_ARG", "chose rule 29");
      break;
    case 30:
      logger("rule_NEXT_ARG", "chose rule 30");
      logger("rule_NEXT_ARG", "checking commaTer");
      if (parser->LLfirst->kind != commaTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_ARG(parser);
      rule_NEXT_ARG(parser);
      break;
    default:
      logger("rule_NEXT_ARG", "error");
      exit(2);
  }
  logger("rule_NEXT_ARG", "finished rule NEXT_ARG");
  parser->current = current;
  logger("rule_NEXT_ARG", "set current node to saved node");
}

void rule_ARG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, ARG);
  logger("rule_ARG", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 31:
      logger("rule_ARG", "chose rule 31");
      logger("rule_ARG", "checking variableTer");
      if (parser->LLfirst->kind != variableTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 32:
      logger("rule_ARG", "chose rule 32");
      rule_LITERAL(parser);
      break;
    default:
      logger("rule_ARG", "error");
      exit(2);
  }
  logger("rule_ARG", "finished rule ARG");
  parser->current = current;
  logger("rule_ARG", "set current node to saved node");
}

void rule_LITERAL(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, LITERAL);
  logger("rule_LITERAL", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 33:
      logger("rule_LITERAL", "chose rule 33");
      logger("rule_LITERAL", "chose rule float_litTer");
      if (parser->LLfirst->kind != float_litTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 34:
      logger("rule_LITERAL", "chose rule 34");
      logger("rule_LITERAL", "chose rule int_litTer");
      if (parser->LLfirst->kind != int_litTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 35:
      logger("rule_LITERAL", "chose rule 35");
      logger("rule_LITERAL", "chose rule nullTer");
      if (parser->LLfirst->kind != nullTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 36:
      logger("rule_LITERAL", "chose rule 36");
      logger("rule_LITERAL", "chose rule string_litTer");
      if (parser->LLfirst->kind != string_litTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_LITERAL", "error");
      exit(2);
  }
  logger("rule_LITERAL", "finished rule LITERAL");
  parser->current = current;
  logger("rule_LITERAL", "set current node to saved node");
}

void rule_FUNC_DECLARE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_DECLARE);
  logger("rule_FUNC_DECLARE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 42:
      logger("rule_FUNC_DECLARE", "chose rule 42");
      logger("rule_FUNC_DECLARE", "checking functionTer");
      if (parser->LLfirst->kind != functionTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_FUNC_DECLARE", "checking function_idTer");
      if (parser->LLfirst->kind != function_idTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_FUNC_DECLARE", "checking leftBracketTer");
      if (parser->LLfirst->kind != leftBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_FUNC_DECLARE_BODY(parser);
      logger("rule_FUNC_DECLARE", "checking rightBracketTer");
      if (parser->LLfirst->kind != rightBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_FUNC_DECLARE", "checking colonTer");
      if (parser->LLfirst->kind != colonTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_RETURN_TYPE(parser);
      logger("rule_FUNC_DECLARE", "checking leftCurlyBracketTer");
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      logger("rule_FUNC_DECLARE", "checking rightCurlyBracketTer");
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_FUNC_DECLARE", "error");
      exit(2);
  }
  logger("rule_FUNC_DECLARE", "finished rule FUNC_DECLARE");
  parser->current = current;
  logger("rule_FUNC_DECLARE", "set current node to saved node");
}

void rule_FUNC_DECLARE_BODY(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_DECLARE_BODY);
  logger("rule_FUNC_DECLARE_BODY", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 43:
      logger("rule_FUNC_DECLARE_BODY", "chose rule 43");
      break;
    case 44:
      logger("rule_FUNC_DECLARE_BODY", "chose rule 44");
      rule_ARG_TYPE(parser);
      logger("rule_FUNC_DECLARE_BODY", "checking variableTer");
      if (parser->LLfirst->kind != variableTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_FUNC_DECLARE_BODY(parser);
      break;
    case 45:
      logger("rule_FUNC_DECLARE_BODY", "chose rule 45");
      logger("rule_FUNC_DECLARE_BODY", "checking commaTer");
      if (parser->LLfirst->kind != commaTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_ARG_TYPE(parser);
      logger("rule_FUNC_DECLARE_BODY", "checking variableTer");
      if (parser->LLfirst->kind != variableTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_FUNC_DECLARE_BODY(parser);
      break;
    default:
      logger("rule_FUNC_DECLARE_BODY", "error");
      exit(2);
  }
  logger("rule_FUNC_DECLARE_BODY", "finished rule FUNC_DECLARE_BODY");
  parser->current = current;
  logger("rule_FUNC_DECLARE_BODY", "set current node to saved node");
}

void rule_ARG_TYPE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, ARG_TYPE);
  logger("rule_ARG_TYPE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 46:
      logger("rule_ARG_TYPE", "chose rule 46");
      logger("rule_ARG_TYPE", "chose rule stringTypeTer");
      if (parser->LLfirst->kind != stringTypeTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 47:
      logger("rule_ARG_TYPE", "chose rule 47");
      logger("rule_ARG_TYPE", "chose rule intTypeTer");
      if (parser->LLfirst->kind != intTypeTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    case 48:
      logger("rule_ARG_TYPE", "chose rule 48");
      logger("rule_ARG_TYPE", "chose rule floatTypeTer");
      if (parser->LLfirst->kind != floatTypeTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_ARG_TYPE", "error");
      exit(2);
  }
  logger("rule_ARG_TYPE", "finished rule ARG_TYPE");
  parser->current = current;
  logger("rule_ARG_TYPE", "set current node to saved node");
}

void rule_IF_ELSE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, IF_ELSE);
  logger("rule_IF_ELSE", "prepared current node");
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 49:
      logger("rule_IF_ELSE", "chose rule 49");
      logger("rule_IF_ELSE", "checking ifTer");
      if (parser->LLfirst->kind != ifTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_IF_ELSE", "checking leftBracketTer");
      if (parser->LLfirst->kind != leftBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_EXP(parser);
      logger("rule_IF_ELSE", "checking rightBracketTer");
      if (parser->LLfirst->kind != rightBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_IF_ELSE", "checking leftCurlyBracketTer");
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      logger("rule_IF_ELSE", "checking rightCurlyBracketTer");
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_IF_ELSE", "checking elseTer");
      if (parser->LLfirst->kind != elseTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      logger("rule_IF_ELSE", "checking leftCurlyBracketTer");
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      logger("rule_IF_ELSE", "checking rightCurlyBracketTer");
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        exit(2);
      }
      ConsumeTerminal(parser);
      break;
    default:
      logger("rule_IF_ELSE", "error");
      exit(2);
  }
  logger("rule_IF_ELSE", "finished rule IF_ELSE");
  parser->current = current;
  logger("rule_IF_ELSE", "set current node to saved node");
}

// TODO rn accepts only terminal with code "1"
void rule_EXP(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, EXP);
  logger("rule_EXP", "prepared current node");

  ExpressionParser(parser);

  logger("rule_EXP", "finished rule EXP");
  parser->current = current;
  logger("rule_EXP", "set current node to saved node");
}