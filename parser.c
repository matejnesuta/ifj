#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LLTable.c"
#include "scanner.c"

typedef struct Terminal {
  terminal_kind kind;
  string code;
} terminal;

terminal current;
terminal next;
int countToNext = 0;
bool wasEps = false;

terminal GetTerminal();
bool STARTgram();
bool PROGgram();
bool START_PROLOGgram();
bool END_PROLOGgram();
bool CODEgram();
bool BODYgram();
bool INNER_SCOPEgram();
bool RIGHT_SIDEgram();
bool RETURN_VALUEgram();
bool RETURN_TYPEgram();
bool FUNC_CALLgram();
bool FUNC_CALL_ARGSgram();
bool NEXT_ARGgram();
bool ARGgram();
bool LITERALgram();
bool FUNC_DECLAREgram();
bool FUNC_DECLARE_BODYgram();
bool ARG_TYPEgram();
bool IF_ELSEgram();
bool EXPgram();

terminal GetTerminal() {
  // takes care of eps transitions
  if (wasEps) {
    wasEps = false;
    return current;
  }
  // takes care of non-deterministic transition in rule 16.
  if (countToNext > 0) {
    countToNext--;
    current = next;
    return current;
  }
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

int main() {
  current = GetTerminal();
  bool passed = STARTgram();
  printf("%d\n", passed);
  if (passed == true) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  return 0;
}

bool STARTgram() {
  printf("START\n");
  return PROGgram();
}

bool PROGgram() {
  printf("PROG\n");
  switch (ChooseRule(PROG, current.kind)) {
    case 2:
      if (!START_PROLOGgram()) return false;
      current = GetTerminal();
      if (!CODEgram()) return false;
      current = GetTerminal();
      if (!END_PROLOGgram()) return false;
      return true;
    default:
      return false;
  }
}

bool START_PROLOGgram() {
  printf("START_PROLOG\n");
  if (current.kind != startPrologTer) return false;
  current = GetTerminal();
  if (current.kind != function_idTer ||
      strcmp(current.code.data, "declare") != 0)
    return false;
  current = GetTerminal();
  if (current.kind != leftBracketTer) return false;
  current = GetTerminal();
  if (current.kind != function_idTer ||
      strcmp(current.code.data, "strict_types") != 0)
    return false;
  current = GetTerminal();
  if (current.kind != assignTer) return false;
  current = GetTerminal();
  if (current.kind != int_litTer || strcmp(current.code.data, "1") != 0)
    return false;
  current = GetTerminal();
  if (current.kind != rightBracketTer) return false;
  current = GetTerminal();
  if (current.kind != semicolonTer) return false;
  return true;
}

bool END_PROLOGgram() {
  printf("END_PROLOG\n");
  switch (ChooseRule(END_PROLOG, current.kind)) {
    case 4:
      if (current.kind != endOfFileTer) return false;
      return true;
    case 5:
      if (current.kind != endPrologTer) return false;
      current = GetTerminal();
      if (current.kind != endOfFileTer) return false;
      return true;
    default:
      return false;
  }
}

bool CODEgram() {
  printf("CODE\n");
  switch (ChooseRule(CODE, current.kind)) {
    case 6:
      wasEps = true;
      return true;
    case 7:
      if (!INNER_SCOPEgram()) return false;
      current = GetTerminal();
      if (!CODEgram()) return false;
      return true;
    case 8:
      if (!FUNC_DECLAREgram()) return false;
      current = GetTerminal();
      if (!CODEgram()) return false;
      return true;
    default:
      return false;
  }
}

bool BODYgram() {
  printf("BODY\n");
  switch (ChooseRule(BODY, current.kind)) {
    case 9:
      if (!INNER_SCOPEgram()) return false;
      current = GetTerminal();
      if (!BODYgram()) return false;
      return true;
    case 10:
      wasEps = true;
      return true;
    default:
      return false;
  }
}

bool INNER_SCOPEgram() {
  printf("INNER_SCOPE\n");
  switch (ChooseRule(INNER_SCOPE, current.kind)) {
    case 11:
      if (!IF_ELSEgram()) return false;
      return true;
    case 12:
      if (current.kind != returnTer) return false;
      current = GetTerminal();
      if (!RETURN_VALUEgram()) return false;
      return true;
    case 13:
      if (current.kind != whileTer) return false;
      current = GetTerminal();
      if (current.kind != leftBracketTer) return false;
      current = GetTerminal();
      if (!EXPgram()) return false;
      current = GetTerminal();
      if (current.kind != rightBracketTer) return false;
      current = GetTerminal();
      if (current.kind != leftCurlyBracketTer) return false;
      current = GetTerminal();
      if (!BODYgram()) return false;
      if (current.kind != rightCurlyBracketTer) return false;
      return true;
    case 14:
      if (current.kind != leftCurlyBracketTer) return false;
      current = GetTerminal();
      if (!BODYgram()) return false;
      if (current.kind != rightCurlyBracketTer) return false;
      return true;
    case 15:
      if (!FUNC_CALLgram()) return false;
      return true;

    // not in grammar
    case 16:
      if (current.kind != variableTer) {
        if (!EXPgram()) return false;
        current = GetTerminal();
        if (current.kind != semicolonTer) return false;
        return true;
      }
      next = GetTerminal();
      if (next.kind != assignTer) {
        if (!EXPgram()) return false;
        current = GetTerminal();
        if (current.kind != semicolonTer) return false;
        return true;
      }
      if (current.kind != variableTer) return false;
      current = GetTerminal();
      if (current.kind != assignTer) return false;
      current = GetTerminal();
      if (!EXPgram()) return false;
      current = GetTerminal();
      if (current.kind != semicolonTer) return false;
      return true;
      // not in grammar

    default:
      return false;
  }
}

bool RIGHT_SIDEgram() {
  printf("RIGHT_SIDE\n");
  switch (ChooseRule(RIGHT_SIDE, current.kind)) {
    case 20:
      if (!FUNC_CALLgram()) return false;
      return true;
    default:
      return false;
  }
}

bool RETURN_VALUEgram() {
  printf("RETURN_VALUE\n");
  switch (ChooseRule(RETURN_VALUE, current.kind)) {
    case 21:
      wasEps = true;
      return true;
    case 22:
      if (!EXPgram()) return false;
      return true;
    default:
      return false;
  }
}

bool RETURN_TYPEgram() {
  printf("RETURN_TYPE\n");
  switch (ChooseRule(RETURN_TYPE, current.kind)) {
    case 24:
      if (!ARG_TYPEgram()) return false;
      return true;
    case 25:
      if (current.kind != voidTypeTer) return false;
      return true;
    default:
      return false;
  }
}

bool FUNC_CALLgram() {
  printf("FUNC_CALL\n");
  switch (ChooseRule(FUNC_CALL, current.kind)) {
    case 26:
      if (current.kind != function_idTer) return false;
      current = GetTerminal();
      if (current.kind != leftBracketTer) return false;
      current = GetTerminal();
      if (!FUNC_CALL_ARGSgram()) return false;
      current = GetTerminal();
      if (current.kind != rightBracketTer) return false;
      current = GetTerminal();
      if (current.kind != semicolonTer) return false;
      return true;
    default:
      return false;
  }
}

bool FUNC_CALL_ARGSgram() {
  printf("FUNC_CALL_ARGS\n");
  switch (ChooseRule(FUNC_CALL_ARGS, current.kind)) {
    case 27:
      wasEps = true;
      return true;
    case 28:
      if (!ARGgram()) return false;
      current = GetTerminal();
      if (!NEXT_ARGgram()) return false;
      return true;
    default:
      return false;
  }
}

bool NEXT_ARGgram() {
  printf("NEXT_ARG\n");
  switch (ChooseRule(NEXT_ARG, current.kind)) {
    case 29:
      wasEps = true;
      return true;
    case 30:
      if (current.kind != commaTer) return false;
      current = GetTerminal();
      if (!ARGgram()) return false;
      current = GetTerminal();
      if (!NEXT_ARGgram()) return false;
      return true;
    default:
      return false;
  }
}

bool ARGgram() {
  printf("ARG\n");
  switch (ChooseRule(ARG, current.kind)) {
    case 31:
      if (current.kind != variableTer) return false;
      return true;
    case 32:
      if (!LITERALgram()) return false;
      return true;
    default:
      return false;
  }
}

bool LITERALgram() {
  printf("LITERAL\n");
  switch (ChooseRule(LITERAL, current.kind)) {
    case 33:
      if (current.kind != float_litTer) return false;
      return true;
    case 34:
      if (current.kind != int_litTer) return false;
      return true;
    case 35:
      if (current.kind != nullTer) return false;
      return true;
    case 36:
      if (current.kind != string_litTer) return false;
      return true;
    default:
      return false;
  }
}

bool FUNC_DECLAREgram() {
  printf("FUNC_DECLARE\n");
  switch (ChooseRule(FUNC_DECLARE, current.kind)) {
    case 42:
      if (current.kind != functionTer) return false;
      current = GetTerminal();
      if (current.kind != function_idTer) return false;
      current = GetTerminal();
      if (current.kind != leftBracketTer) return false;
      current = GetTerminal();
      if (!FUNC_DECLARE_BODYgram()) return false;
      current = GetTerminal();
      if (current.kind != rightBracketTer) return false;
      current = GetTerminal();
      if (current.kind != colonTer) return false;
      current = GetTerminal();
      if (!RETURN_TYPEgram()) return false;
      current = GetTerminal();
      if (current.kind != leftCurlyBracketTer) return false;
      current = GetTerminal();
      if (!BODYgram()) return false;
      current = GetTerminal();
      if (current.kind != rightCurlyBracketTer) return false;
      return true;
    default:
      return false;
  }
}

bool FUNC_DECLARE_BODYgram() {
  printf("FUNC_DECLARE_BODY\n");
  switch (ChooseRule(FUNC_DECLARE_BODY, current.kind)) {
    case 43:
      wasEps = true;
      return true;
    case 44:
      if (!ARG_TYPEgram()) return false;
      current = GetTerminal();
      if (current.kind != variableTer) return false;
      current = GetTerminal();
      if (!FUNC_DECLARE_BODYgram()) return false;
      return true;
    case 45:
      if (current.kind != commaTer) return false;
      current = GetTerminal();
      if (!ARG_TYPEgram()) return false;
      current = GetTerminal();
      if (current.kind != variableTer) return false;
      current = GetTerminal();
      if (!FUNC_DECLARE_BODYgram()) return false;
      return true;
    default:
      return false;
  }
}

bool ARG_TYPEgram() {
  printf("ARG_TYPE\n");
  switch (ChooseRule(ARG_TYPE, current.kind)) {
    case 46:
      if (current.kind != stringTypeTer) return false;
      return true;
    case 47:
      if (current.kind != intTypeTer) return false;
      return true;
    case 48:
      if (current.kind != floatTypeTer) return false;
      return true;
    default:
      return false;
  }
}

bool IF_ELSEgram() {
  printf("IF_ELSE\n");
  switch (ChooseRule(IF_ELSE, current.kind)) {
    case 49:
      if (current.kind != ifTer) return false;
      current = GetTerminal();
      if (current.kind != leftBracketTer) return false;
      current = GetTerminal();
      if (!EXPgram()) return false;
      current = GetTerminal();
      if (current.kind != rightBracketTer) return false;
      current = GetTerminal();
      if (current.kind != leftCurlyBracketTer) return false;
      current = GetTerminal();
      if (!BODYgram()) return false;
      current = GetTerminal();
      if (current.kind != rightCurlyBracketTer) return false;
      current = GetTerminal();
      if (current.kind != elseTer) return false;
      current = GetTerminal();
      if (current.kind != leftCurlyBracketTer) return false;
      current = GetTerminal();
      if (!BODYgram()) return false;
      current = GetTerminal();
      if (current.kind != rightCurlyBracketTer) return false;
      return true;
    default:
      return false;
  }
}

// TODO
bool EXPgram() {
  printf("EXP\n");
  return true;
}