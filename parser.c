/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file parser.c
 * @authors Stefan Peknik xpekni01
 * @brief parser
 * @date 2022-10-30
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */

#include "parser.h"

#include "codegen.h"
#include "error.h"
#include "expressionParser.h"
#include "scanner.h"
#include "symbol.h"
/**
 * @brief Move to next token
 *
 * @param parser
 */
void UpdateLLfirst(Parser *parser) {
  if (parser->buffer == NULL) {
    parser->LLfirst = GetTerminal();
    return;
  }
  parser->LLfirst = parser->buffer;
  parser->buffer = NULL;
}
/**
 * @brief Get the Terminal object
 *
 * @return terminal*
 */
terminal *GetTerminal() {
  Lexeme *next = GetLexeme();
  terminal *term = (terminal *)malloc(sizeof(struct Terminal));
  if (term == NULL) {
    ErrorExit(99, "Malloc failed!");
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
      ErrorExit(1, "Error in structure of current lexeme");
      break;
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
      ErrorExit(1, "Error in structure of current lexeme");
      break;
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
      ErrorExit(1, "Error in structure of current lexeme");
  }
  free(next);
  return term;
}
/**
 * @brief Chooses the rule form LLgrammar
 *
 * @param nonterminal
 * @param parser
 * @return int
 */
int ChooseRule(nonterminal_kind nonterminal, Parser *parser) {

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
        case leftBracketTer:
          return 22;
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
/**
 * @brief Create a parser
 *
 * @return Parser*
 */
Parser *ParserCreate() {
  Parser *parser = malloc(sizeof(struct Parser));
  if (parser == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  parser->root = ASTreeCreateNode(SymbolCreateNonterminal(START));
  parser->current = parser->root;
  parser->buffer = NULL;
  UpdateLLfirst(parser);
  return parser;
}
/**
 * @brief Prepare node in ASTree
 *
 * @param parser
 * @param nonterminal
 */
void PrepareCurrentNode(Parser *parser, nonterminal_kind nonterminal) {
  AST *child = ASTreeCreateNode(SymbolCreateNonterminal(nonterminal));
  parser->current->children =
      LListInsertChild(parser->current->children, child);
  parser->current = child;
}
/**
 * @brief Update LLfirst with buffer
 *
 * @param parser
 */
void ConsumeTerminal(Parser *parser) {
  parser->current->children =
      LListInsertChild(parser->current->children,
                       ASTreeCreateNode(SymbolCreateTerminal(parser->LLfirst)));
  UpdateLLfirst(parser);
}

Parser *StartParsing() {
  Parser *parser = ParserCreate();
  rule_START(parser);
  // ASTreePrintChildren(parser->root);
  return parser;
}

// rules down from here
void rule_START(Parser *parser) {
  rule_PROG(parser);
}

void rule_PROG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, PROG);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 2:
      rule_START_PROLOG(parser);
      rule_CODE(parser);
      rule_END_PROLOG(parser);
      parser->current = current;
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_START_PROLOG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, START_PROLOG);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 3:

      if (parser->LLfirst->kind != startPrologTer) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != function_idTer ||
          strcmp(parser->LLfirst->code->data, "declare") != 0) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != leftBracketTer) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != function_idTer ||
          strcmp(parser->LLfirst->code->data, "strict_types") != 0) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != assignTer) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != int_litTer ||
          strcmp(parser->LLfirst->code->data, "1") != 0) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != rightBracketTer) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != semicolonTer) {
        ErrorExit(2, "Syntax error");
      }

      ConsumeTerminal(parser);

      break;

    default:
      ErrorExit(2, "Syntax error");
  }

  parser->current = current;
}

void rule_END_PROLOG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, END_PROLOG);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 4:
      if (parser->LLfirst->kind != endOfFileTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;

    case 5:

      if (parser->LLfirst->kind != endPrologTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);

      if (parser->LLfirst->kind != endOfFileTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;

    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_CODE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, CODE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 7:
      rule_INNER_SCOPE(parser);
      rule_CODE(parser);
      break;
    case 8:
      rule_FUNC_DECLARE(parser);
      rule_CODE(parser);
      break;
    case 6:
      break;

    default:
      ErrorExit(2, "Syntax error");
  }

  parser->current = current;
}

void rule_BODY(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, BODY);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 9:
      rule_INNER_SCOPE(parser);
      rule_BODY(parser);
      break;
    case 10:
      break;

    default:
      ErrorExit(2, "Syntax error");
  }

  parser->current = current;
}

void rule_INNER_SCOPE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, INNER_SCOPE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 11:
      rule_IF_ELSE(parser);
      break;
    case 12:
      if (parser->LLfirst->kind != returnTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_RETURN_VALUE(parser);
      if (parser->LLfirst->kind != semicolonTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 13:
      if (parser->LLfirst->kind != whileTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_EXP(parser);
      if (parser->LLfirst->kind != rightBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 14:
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 15:
      rule_FUNC_CALL(parser);
      break;
    case 17:
      rule_EXP(parser);
      if (parser->LLfirst->kind != semicolonTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 18:
      if (parser->LLfirst->kind != variableTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != assignTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_RIGHT_SIDE(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_RIGHT_SIDE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, RIGHT_SIDE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 37:
      rule_FUNC_CALL(parser);
      break;
    case 38:
      rule_EXP(parser);
      if (parser->LLfirst->kind != semicolonTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_RETURN_VALUE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, RETURN_VALUE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 21:
      break;
    case 22:
      rule_EXP(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_RETURN_TYPE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, RETURN_TYPE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 24:
      rule_ARG_TYPE(parser);
      break;
    case 25:
      if (parser->LLfirst->kind != voidTypeTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_FUNC_CALL(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_CALL);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 26:
      if (parser->LLfirst->kind != function_idTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_FUNC_CALL_ARGS(parser);
      if (parser->LLfirst->kind != rightBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != semicolonTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_FUNC_CALL_ARGS(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_CALL_ARGS);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 27:
      break;
    case 28:
      rule_ARG(parser);
      rule_NEXT_ARG(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_NEXT_ARG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, NEXT_ARG);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 29:
      break;
    case 30:
      if (parser->LLfirst->kind != commaTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_ARG(parser);
      rule_NEXT_ARG(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_ARG(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, ARG);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 31:
      if (parser->LLfirst->kind != variableTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 32:
      rule_LITERAL(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_LITERAL(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, LITERAL);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 33:
      if (parser->LLfirst->kind != float_litTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 34:
      if (parser->LLfirst->kind != int_litTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 35:
      if (parser->LLfirst->kind != nullTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 36:
      if (parser->LLfirst->kind != string_litTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_FUNC_DECLARE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_DECLARE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 42:
      if (parser->LLfirst->kind != functionTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != function_idTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_FUNC_DECLARE_BODY(parser);
      if (parser->LLfirst->kind != rightBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != colonTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_RETURN_TYPE(parser);
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_FUNC_DECLARE_BODY(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, FUNC_DECLARE_BODY);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 43:
      break;
    case 44:
      rule_ARG_TYPE(parser);
      if (parser->LLfirst->kind != variableTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_FUNC_DECLARE_BODY(parser);
      break;
    case 45:
      if (parser->LLfirst->kind != commaTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_ARG_TYPE(parser);
      if (parser->LLfirst->kind != variableTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_FUNC_DECLARE_BODY(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_ARG_TYPE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, ARG_TYPE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 46:
      if (parser->LLfirst->kind != stringTypeTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 47:
      if (parser->LLfirst->kind != intTypeTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    case 48:
      if (parser->LLfirst->kind != floatTypeTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_IF_ELSE(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, IF_ELSE);
  switch (ChooseRule(parser->current->node->nonterminal, parser)) {
    case 49:
      if (parser->LLfirst->kind != ifTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_EXP(parser);
      if (parser->LLfirst->kind != rightBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != elseTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      if (parser->LLfirst->kind != leftCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      rule_BODY(parser);
      if (parser->LLfirst->kind != rightCurlyBracketTer) {
        ErrorExit(2, "Syntax error");
      }
      ConsumeTerminal(parser);
      break;
    default:
      ErrorExit(2, "Syntax error");
  }
  parser->current = current;
}

void rule_EXP(Parser *parser) {
  AST *current = parser->current;
  PrepareCurrentNode(parser, EXP);

  ExpressionParser(parser);

  parser->current = current;
}