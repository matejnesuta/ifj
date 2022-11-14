#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Terminals {
  ifTer,
  elseTer,
  whileTer,
  functionTer,
  function_idTer,
  intTypeTer,
  floatTypeTer,
  stringTypeTer,
  voidTypeTer,
  nullTer,
  returnTer,
  assignTer,
  startPrologTer,
  endPrologTer,
  endOfFileTer,
  semicolonTer,
  leftBracketTer,
  rightBracketTer,
  leftCurlyBracketTer,
  rightCurlyBracketTer,
  colonTer,
  commaTer,
  variableTer,
  string_litTer,
  int_litTer,
  float_litTer,
  epsTer,
  dotTer,
  plusTer,
  minusTer,
  multiplyTer,
  divideTer,
  lessTer,
  lessOrEqualTer,
  greaterTer,
  greaterOrEqualTer,
  equalTer,
  notEqualTer
} terminal_kind;

typedef enum Nonterminals {
  START,
  PROG,
  START_PROLOG,
  END_PROLOG,
  CODE,
  BODY,
  INNER_SCOPE,
  RIGHT_SIDE,
  RETURN_VALUE,
  RETURN_TYPE,
  FUNC_CALL,
  FUNC_CALL_ARGS,
  NEXT_ARG,
  ARG,
  LITERAL,
  FUNC_DECLARE,
  FUNC_DECLARE_BODY,
  ARG_TYPE,
  IF_ELSE
} nonterminal_kind;

int ChooseRule(nonterminal_kind nonterminal, terminal_kind nextTerminal) {
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

    case RIGHT_SIDE:
      switch (nextTerminal) {
        case function_idTer:
          return 20;
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
