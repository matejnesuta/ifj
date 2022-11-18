
#include "parserv2.h"

#include "ASTree.c"
#include "LList.c"
#include "logger.c"
#include "nonterminals.h"
#include "scanner.h"
#include "terminals.h"

terminal GetTerminal() {
  logger("GetTerminal", "Getting terminal");
  Lexeme next = GetLexeme();
  switch (next.kind) {
    case MULTIPLY:
      return (terminal){.kind = multiplyTer, .code = next.code};
    case DIVISION:
      return (terminal){.kind = divideTer, .code = next.code};
    case MINUS:
      return (terminal){.kind = minusTer, .code = next.code};
    case PLUS:
      return (terminal){.kind = plusTer, .code = next.code};
    case INTEGER:
      return (terminal){.kind = int_litTer, .code = next.code};
    case FLOAT:
      return (terminal){.kind = float_litTer, .code = next.code};
    case CONCAT:
      return (terminal){.kind = dotTer, .code = next.code};
    case ASSIGN:
      return (terminal){.kind = assignTer, .code = next.code};
    case EQUAL:
      return (terminal){.kind = equalTer, .code = next.code};
    case GT:
      return (terminal){.kind = greaterTer, .code = next.code};
    case GE:
      return (terminal){.kind = greaterOrEqualTer, .code = next.code};
    case LT:
      return (terminal){.kind = lessTer, .code = next.code};
    case LE:
      return (terminal){.kind = lessOrEqualTer, .code = next.code};
    case STARTPROLOG:
      return (terminal){.kind = startPrologTer, .code = next.code};
    case SEMICOLON:
      return (terminal){.kind = semicolonTer, .code = next.code};
    case LEFTBRACKET:
      return (terminal){.kind = leftBracketTer, .code = next.code};
    case RIGHTBRACKET:
      return (terminal){.kind = rightBracketTer, .code = next.code};
    case LEFTCURLYBRACKET:
      return (terminal){.kind = leftCurlyBracketTer, .code = next.code};
    case RIGHTCURLYBRACKET:
      return (terminal){.kind = rightCurlyBracketTer, .code = next.code};
    case ENDPROLOG:
      return (terminal){.kind = endPrologTer, .code = next.code};
    case VARIABLE:
      return (terminal){.kind = variableTer, .code = next.code};
    case FUNCTION:
      return (terminal){.kind = function_idTer, .code = next.code};
    case KEYWORD:
      if (strcmp(next.code.data, "if") == 0) {
        return (terminal){.kind = ifTer, .code = next.code};
      } else if (strcmp(next.code.data, "else") == 0) {
        return (terminal){.kind = elseTer, .code = next.code};
      } else if (strcmp(next.code.data, "while") == 0) {
        return (terminal){.kind = whileTer, .code = next.code};
      } else if (strcmp(next.code.data, "function") == 0) {
        return (terminal){.kind = functionTer, .code = next.code};
      } else if (strcmp(next.code.data, "null") == 0) {
        return (terminal){.kind = nullTer, .code = next.code};
      } else if (strcmp(next.code.data, "return") == 0) {
        return (terminal){.kind = returnTer, .code = next.code};
      }
      break;
    case DATATYPE:
      if (strcmp(next.code.data, "int") == 0 ||
          strcmp(next.code.data, "?int") == 0) {
        return (terminal){.kind = intTypeTer, .code = next.code};
      } else if (strcmp(next.code.data, "float") == 0 ||
                 strcmp(next.code.data, "?float") == 0) {
        return (terminal){.kind = floatTypeTer, .code = next.code};
      } else if (strcmp(next.code.data, "string") == 0 ||
                 strcmp(next.code.data, "?string") == 0) {
        return (terminal){.kind = stringTypeTer, .code = next.code};
      } else if (strcmp(next.code.data, "void") == 0) {
        return (terminal){.kind = voidTypeTer, .code = next.code};
      }
      break;
    case COLON:
      return (terminal){.kind = colonTer, .code = next.code};
    case COMMA:
      return (terminal){.kind = commaTer, .code = next.code};
    case ENDOFFILE:
      return (terminal){.kind = endOfFileTer, .code = next.code};
    case NOTEQUAL:
      return (terminal){.kind = notEqualTer, .code = next.code};
    case STRING:
      return (terminal){.kind = string_litTer, .code = next.code};
  }
}

// TODO update rules based on updated LL table
int ChooseRule(nonterminal_kind nonterminal, terminal_kind nextTerminal) {
  logger("ChooseRule", "Choosing rule");
  switch (nonterminal) {
    case START:
      return -1;

    case PROG:
      switch (nextTerminal) {
        case startPrologTer:
          return 2;
        default:
          return -1;
      }

    case START_PROLOG:
      switch (nextTerminal) {
        case startPrologTer:
          return 3;
        default:
          return -1;
      }

    case END_PROLOG:
      switch (nextTerminal) {
        case endPrologTer:
          return 5;
        case endOfFileTer:
          return 4;
        default:
          return -1;
      }

    case CODE:
      switch (nextTerminal) {
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
        case endPrologTer:
          return 6;
        case leftCurlyBracketTer:
          return 7;
        case endOfFileTer:
          return 6;

        // not in grammar
        case int_litTer:
          return 7;
        case float_litTer:
          return 7;
        case string_litTer:
          return 7;
        case variableTer:
          return 7;
          // not in grammar

        default:
          return 6;
      }

    case BODY:
      switch (nextTerminal) {
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
        case rightCurlyBracketTer:
          return 10;

        // not in grammar
        case int_litTer:
          return 9;
        case float_litTer:
          return 9;
        case string_litTer:
          return 9;
        case variableTer:
          return 9;
          // not in grammar

        default:
          return 10;
      }

    case INNER_SCOPE:
      switch (nextTerminal) {
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
        case int_litTer:
          return 16;
        case float_litTer:
          return 16;
        case string_litTer:
          return 16;
        case variableTer:
          return 16;
          // not in grammar

        default:
          return -1;
      }

    case RETURN_VALUE:
      switch (nextTerminal) {
        case semicolonTer:
          return 21;
        default:
          return 21;
      }

    case RETURN_TYPE:
      switch (nextTerminal) {
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
      switch (nextTerminal) {
        case function_idTer:
          return 26;
        default:
          return -1;
      }

    case FUNC_CALL_ARGS:
      switch (nextTerminal) {
        case rightBracketTer:
          return 27;
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
      switch (nextTerminal) {
        case rightBracketTer:
          return 29;
        case commaTer:
          return 30;
        default:
          return 29;
      }

    case ARG:
      switch (nextTerminal) {
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
      switch (nextTerminal) {
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
      switch (nextTerminal) {
        case functionTer:
          return 42;
        default:
          return -1;
      }

    case FUNC_DECLARE_BODY:
      switch (nextTerminal) {
        case intTypeTer:
          return 44;
        case floatTypeTer:
          return 44;
        case stringTypeTer:
          return 44;
        case rightBracketTer:
          return 43;
        case commaTer:
          return 45;
        default:
          return 43;
      }

    case ARG_TYPE:
      switch (nextTerminal) {
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
      switch (nextTerminal) {
        case ifTer:
          return 49;
        default:
          return -1;
      }
  }
  return -1;
}

Parser *ParserCreate() {
  Parser *parser = malloc(sizeof(struct Parser));
  if (parser == NULL) {
    exit(99);
  }
  logger("parser", "created parser");
  parser->root = ASTreeInit();
  parser->root = ASTreeCreateNode(
      parser->root, (symbol){.nonterminal = START, .is_terminal = false});
  logger("parser", "created root node");
  parser->current = parser->root;
  logger("parser", "set current node to root");
  parser->LLfirst = GetTerminal();
  logger("parser", "got first terminal");
  return parser;
}

int main() { rule_START(); }

void rule_START() {
  Parser *parser = ParserCreate();
  rule_PROG(parser);
  logger("parser", "finished parsing");
  ASTreePrintChildren(parser->root);
}

void rule_PROG(Parser *parser) {
  AST *current = parser->current;
  logger("parser", "saved current node");
  AST *child = ASTreeInit();
  logger("parser", "inited child node");
  child = ASTreeCreateNode(child,
                           (symbol){.nonterminal = PROG, .is_terminal = false});
  logger("parser", "created child node");
  parser->current->children = LListInit();
  parser->current->children =
      LListInsertFirstChild(parser->current->children, child);
  logger("parser", "inserted child node");
  parser->current = child;
  logger("parser", "set current node to child");
  switch (ChooseRule(parser->current->node.nonterminal, parser->LLfirst.kind)) {
    case 2:
      logger("parser", "chose rule 2");
      rule_START_PROLOG(parser);
      rule_CODE(parser);
      rule_END_PROLOG(parser);
      break;
    default:
      exit(2);
  }
  logger("parser", "finished rule PROG");
  parser->current = current;
  logger("parser", "set current node to saved node");
}

void rule_START_PROLOG(Parser *parser) { return; }
void rule_END_PROLOG(Parser *parser) { return; }
void rule_CODE(Parser *parser) { return; }
