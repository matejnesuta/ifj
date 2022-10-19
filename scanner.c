#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO fix exits

typedef enum FSMstates {
  Start,
  Multiply,
  Division,
  BlockComment,
  LineComment,
  ExpectEndBlockComment,
  Plus,
  Minus,
  Concat,
  Assign,
  ExpectEqual,
  Equal,
  Gt,
  Ge,
  Lt,
  Le,
  ExpectStartProlog1,
  ExpectStartProlog2,
  ExpectStartProlog3,
  ExpectStartProlog4,
  StartPrologEnd,
  Semicolon,
  LeftBracket,
  RightBracket,
  LeftCurlyBracket,
  RightCurlyBracket,
  Integer,
  FloatIntDotInt,
  Exponent,
  PlusMinus,
  FloatIntAfterExponent,
  EnableNull,
  Epilog1,
  Epilog2,
  Variable,
  Identifier,
  Colon,
  EndOfFile,
  ExclamationMark,
  ExpectNotEqual,
  NotEqual,
  String,
  EscapeSeq,
  StringEnd,
  Error
} state;

typedef struct string {
  char *data;
  size_t size;
} string;

typedef struct Lexeme {
  enum EndStates {
    MULTIPLY,
    DIVISION,
    MINUS,
    PLUS,
    INTEGER,
    FLOAT,
    CONCAT,
    ASSIGN,
    EQUAL,
    GT,
    GE,
    LT,
    LE,
    STARTPROLOG,
    SEMICOLON,
    LEFTBRACKET,
    RIGHTBRACKET,
    LEFTCURLYBRACKET,
    RIGHTCURLYBRACKET,
    ENDPROLOG,
    VARIABLE,
    FUNCTION,
    KEYWORD,
    DATATYPE,
    COLON,
    ENDOFFILE,
    NOTEQUAL,
    STRING
  } kind;
  string code;
} Lexeme;

size_t LengthdataTypes = 6;
char *dataTypes[] = {"?int", "?float", "?string", "int", "float", "string"};

size_t LengthKeywords = 7;
char *keyWords[] = {"else",   "function", "if",   "null",
                    "return", "void",     "while"};

string SetupString() {
  string str;
  str.data = (char *)malloc(sizeof(char));
  if (str.data == NULL) exit(2);
  str.data[0] = '\0';
  str.size = 1;
  return str;
}

string AddToString(string str, char ch) {
  str.size++;
  char *tmp = realloc(str.data, str.size * sizeof(char));
  if (!tmp) {
    free(str.data);
    str.data = NULL;
    exit(2);
  }
  str.data = tmp;
  str.data[str.size - 2] = ch;
  return str;
}

string ReplaceCharInString(string str, size_t index, char ch) {
  if (index > str.size) return str;
  if (ch == '\0') {
    str.size = index + 1;
    char *tmp = realloc(str.data, str.size * sizeof(char));
    if (!tmp) {
      free(str.data);
      str.data = NULL;
      exit(2);
    }
    str.data = tmp;
  }
  str.data[index] = ch;
  return str;
}

string ResetString(string str) {
  free(str.data);
  return SetupString();
}

state transition(state currIn, int edge) {
  switch (currIn) {
    case Start:
      switch (edge) {
        case '*':
          return Multiply;
        case '/':
          return Division;
        case '+':
          return Plus;
        case '-':
          return Minus;
        case '.':
          return Concat;
        case '=':
          return Assign;
        case '>':
          return Gt;
        case '<':
          return Lt;
        case ';':
          return Semicolon;
        case '(':
          return LeftBracket;
        case ')':
          return RightBracket;
        case '{':
          return LeftCurlyBracket;
        case '}':
          return RightCurlyBracket;
        case '?':
          return EnableNull;
        case '$':
          return Variable;
        case ':':
          return Colon;
        case EOF:
          return EndOfFile;
        case '!':
          return ExclamationMark;
        case '"':
          return String;
      }
      if (isspace(edge)) return Start;
      if (isdigit(edge)) return Integer;
      if (isalpha(edge) || edge == '_') return Identifier;
      return Error;

    case Multiply:
      return Error;

    case Division:
      if (edge == '*') return BlockComment;
      if (edge == '/') return LineComment;
      return Error;

    case LineComment:
      if (edge != '\n' && edge != EOF) return LineComment;
      if (edge == '\n') return Start;
      return Error;

    case BlockComment:
      if (edge == '*') return ExpectEndBlockComment;
      return Error;

    case ExpectEndBlockComment:
      if (edge != '/' && edge != EOF) return BlockComment;
      if (edge == '/') return Start;
      return Error;

    case Plus:
      if (isdigit(edge)) return Integer;
      return Error;

    case Minus:
      if (isdigit(edge)) return Integer;
      return Error;

    case Integer:
      if (isdigit(edge)) return Integer;
      if (edge == '.') return FloatIntDotInt;
      if (edge == 'e' || edge == 'E') return Exponent;
      return Error;

    case FloatIntDotInt:
      if (isdigit(edge)) return FloatIntDotInt;
      if (edge == 'e' || edge == 'E') return Exponent;
      return Error;

    case Exponent:
      if (edge == '+' || edge == '-') return PlusMinus;
      if (isdigit(edge)) return FloatIntAfterExponent;
      return Error;

    case PlusMinus:
      if (isdigit(edge)) return FloatIntAfterExponent;
      return Error;

    case FloatIntAfterExponent:
      if (isdigit(edge)) return FloatIntAfterExponent;
      return Error;

    case Concat:
      return Error;

    case Assign:
      if (edge == '=') return ExpectEqual;
      return Error;

    case ExpectEqual:
      if (edge == '=') return Equal;
      return Error;

    case Equal:
      return Error;

    case Gt:
      if (edge == '=') return Ge;
      return Error;

    case Ge:
      return Error;

    case Lt:
      if (edge == '=') return Le;
      if (edge == '?') return ExpectStartProlog1;
      return Error;

    case Le:
      return Error;

    case ExpectStartProlog1:
      if (edge == 'p') return ExpectStartProlog2;
      return Error;

    case ExpectStartProlog2:
      if (edge == 'h') return ExpectStartProlog3;
      return Error;

    case ExpectStartProlog3:
      if (edge == 'p') return ExpectStartProlog4;
      return Error;

    case ExpectStartProlog4:
      if (isspace(edge)) return StartPrologEnd;
      return Error;

    case StartPrologEnd:
      return Error;

    case Semicolon:
      return Error;

    case LeftBracket:
      return Error;

    case RightBracket:
      return Error;

    case LeftCurlyBracket:
      return Error;

    case RightCurlyBracket:
      return Error;

    case EnableNull:
      if (edge == '>') return Epilog1;
      if (edge >= 97 && edge <= 122)
        return Identifier;  // after ? we expect indetifier type, which is
                            // always only lower-case
      return Error;

    case Epilog1:
      if (edge == '\n') return Epilog2;
      return Error;

    case Epilog2:
      return Error;

    case Variable:
      if (isalpha(edge) || edge == '_') return Identifier;
      return Error;

    case Identifier:
      if (isalpha(edge) || isdigit(edge) || edge == '_') return Identifier;
      return Error;

    case Colon:
      return Error;

    case EndOfFile:
      return Error;

    case ExclamationMark:
      if (edge == '=') return ExpectNotEqual;
      return Error;

    case ExpectNotEqual:
      if (edge == '=') return NotEqual;
      return Error;

    case NotEqual:
      return Error;

    case String:
      if (edge > 31 && edge != '\\' && edge != '"' && edge != '$')
        return String;
      if (edge == '\\') return EscapeSeq;
      if (edge == '"') return StringEnd;
      return Error;

    case EscapeSeq:
      if (edge > 31) return String;
      return Error;

    case StringEnd:
      return Error;

    case Error:
      printf("something bad here happened in transition\n");
      exit(1);
  }
}

string TransformEscSeq(string code) {
  string new = SetupString();
  for(size_t i = 0; i < code.size; i++) {
    if(code.data[i] == '\\') {
      if(code.data[i + 1] == '"') {
        new = AddToString(new, '\"');
        i += 2;
        continue;
      }
      if(code.data[i + 1] == 'n') {
        new = AddToString(new, '\n');
        i += 2;
        continue;
      }
      if(code.data[i + 1] == 't') {
        new = AddToString(new, '\t');
        i += 2;
        continue;
      }
      if(code.data[i + 1] == '\\') {
        new = AddToString(new, '\\');
        i += 2;
        continue;
      }
      if(code.data[i + 1] == '$') {
        new = AddToString(new, '$');
        i += 2;
        continue;
      }
      if(code.data[i + 1] == 'x') {
        char hexaStr[] = { code.data[i + 2], code.data[i + 3], '\0' };
        char *junk;
        int hexaVal = (int)strtol(hexaStr, &junk, 16);
        if(junk[0] != '\0') {
          new = AddToString(new, code.data[i]);
          new = AddToString(new, code.data[i + 1]);
          i++;
        }
        else if(hexaVal >= 0x01 && hexaVal <= 0xff) {
          new = AddToString(new, (char)hexaVal);
          i += 2;
        }
        else {
          new = AddToString(new, code.data[i]);
          new = AddToString(new, code.data[i + 1]);
          i++;
        }
        continue;
      }
      if(isnumber(code.data[i + 1]) && isnumber(code.data[i + 2]) && isnumber(code.data[i + 3])) {
          char octaStr[] = { code.data[i + 1], code.data[i + 2], code.data[i + 3], '\0' };
          char *junk;
          int octaVal = (int)strtol(octaStr, &junk, 8);
          if(junk[0] != '\0') {
            new = AddToString(new, code.data[i]);
            new = AddToString(new, code.data[i + 1]);
            i++;
          }
          else if(octaVal >= 1 && octaVal <= 255) { // 1 = 01 , 255 = 0377
            new = AddToString(new, (char)octaVal);
            i += 2;
          }
          else {
          new = AddToString(new, code.data[i]);
          new = AddToString(new, code.data[i + 1]);
          i++;
        }
      }
      else {
          new = AddToString(new, code.data[i]);
          new = AddToString(new, code.data[i + 1]);
          i++;
        }
    }
    else {
      new = AddToString(new, code.data[i]);
    }
  }
  free(code.data);
  return new;
}

Lexeme MakeLexeme(state final, string code) {
  switch (final) {
    case Multiply:
      return (Lexeme){.kind = MULTIPLY, .code = code};
    case Division:
      return (Lexeme){.kind = DIVISION, .code = code};
    case Minus:
      return (Lexeme){.kind = MINUS, .code = code};
    case Plus:
      return (Lexeme){.kind = PLUS, .code = code};
    case Integer:
      return (Lexeme){.kind = INTEGER, .code = code};
    case FloatIntDotInt:
      return (Lexeme){.kind = FLOAT, .code = code};
    case FloatIntAfterExponent:
      return (Lexeme){.kind = FLOAT, .code = code};
    case Assign:
      return (Lexeme){.kind = ASSIGN, .code = code};
    case Equal:
      return (Lexeme){.kind = EQUAL, .code = code};
    case Gt:
      return (Lexeme){.kind = GT, .code = code};
    case Ge:
      return (Lexeme){.kind = GE, .code = code};
    case Lt:
      return (Lexeme){.kind = LT, .code = code};
    case Le:
      return (Lexeme){.kind = LE, .code = code};
    case StartPrologEnd:
      return (Lexeme){.kind = STARTPROLOG, .code = code};
    case Semicolon:
      return (Lexeme){.kind = SEMICOLON, .code = code};
    case LeftBracket:
      return (Lexeme){.kind = LEFTBRACKET, .code = code};
    case RightBracket:
      return (Lexeme){.kind = RIGHTBRACKET, .code = code};
    case LeftCurlyBracket:
      return (Lexeme){.kind = LEFTCURLYBRACKET, .code = code};
    case RightCurlyBracket:
      return (Lexeme){.kind = RIGHTCURLYBRACKET, .code = code};
    case Epilog1:
      return (Lexeme){.kind = ENDPROLOG, .code = code};
    case Epilog2:
      return (Lexeme){.kind = ENDPROLOG, .code = code};
    case Identifier:
      if (code.data[0] == '$') return (Lexeme){.kind = VARIABLE, .code = code};
      for (size_t i = 0; i < LengthdataTypes; i++) {
        if (code.data == dataTypes[i])
          return (Lexeme){.kind = DATATYPE, .code = code};
      }
      for (size_t i = 0; i < LengthKeywords; i++) {
        if (code.data == keyWords[i])
          return (Lexeme){.kind = KEYWORD, .code = code};
      }
      return (Lexeme){.kind = FUNCTION, .code = code};
    case Colon:
      return (Lexeme){.kind = COLON, .code = code};
    case EndOfFile:
      return (Lexeme){.kind = ENDOFFILE, .code = code};
    case NotEqual:
      return (Lexeme){.kind = NOTEQUAL, .code = code};
    case StringEnd:
      code = TransformEscSeq(code);
      return (Lexeme){.kind = STRING, .code = code};

    case Start:
    case BlockComment:
    case LineComment:
    case ExpectEndBlockComment:
    case Concat:
    case ExpectEqual:
    case ExpectStartProlog1:
    case ExpectStartProlog2:
    case ExpectStartProlog3:
    case ExpectStartProlog4:
    case Exponent:
    case PlusMinus:
    case EnableNull:
    case Variable:
    case ExclamationMark:
    case ExpectNotEqual:
    case String:
    case EscapeSeq:
    case Error:
      printf("something bad happened in MakeLexeme\n");
      exit(1);
  }
}

Lexeme GetLexeme() {
  state currIn = Start;
  string code = SetupString();

  while (true) {
    int edge = getchar();
    code = AddToString(code, edge);
    state next = transition(currIn, edge);
    if (next == EndOfFile) {
      if (currIn == Start) {
        free(code.data);
        return (Lexeme){.kind = ENDOFFILE, .code.data = NULL, .code.size = 0};
      }
      next = Error;
    }
    if (next == Error) {
      ungetc(edge, stdin);
      code = ReplaceCharInString(code, code.size - 2, '\0');
      return MakeLexeme(currIn, code);
    }
    if (next == Start) {
      code = ResetString(code);
    }
    currIn = next;
  }
}

void PrintLexeme(Lexeme lexeme) {
  switch (lexeme.kind) {
    case MULTIPLY:
      printf("%-25s", "MULTIPLY");
      break;

    case DIVISION:
      printf("%-25s", "DIVISION");
      break;

    case MINUS:
      printf("%-25s", "MINUS");
      break;

    case PLUS:
      printf("%-25s", "PLUS");
      break;

    case INTEGER:
      printf("%-25s", "INTEGER");
      break;

    case FLOAT:
      printf("%-25s", "FLOAT");
      break;

    case CONCAT:
      printf("%-25s", "CONCAT");
      break;

    case ASSIGN:
      printf("%-25s", "ASSIGN");
      break;

    case EQUAL:
      printf("%-25s", "EQUAL");
      break;

    case GT:
      printf("%-25s", "GT");
      break;

    case GE:
      printf("%-25s", "GE");
      break;

    case LT:
      printf("%-25s", "LT");
      break;

    case LE:
      printf("%-25s", "LE");
      break;

    case STARTPROLOG:
      printf("%-25s", "STARTPROLOG");
      break;

    case SEMICOLON:
      printf("%-25s", "SEMICOLON");
      break;

    case LEFTBRACKET:
      printf("%-25s", "LEFTBRACKET");
      break;

    case RIGHTBRACKET:
      printf("%-25s", "RIGHTBRACKET");
      break;

    case LEFTCURLYBRACKET:
      printf("%-25s", "LEFTCURLYBRACKET");
      break;

    case RIGHTCURLYBRACKET:
      printf("%-25s", "RIGHTCURLYBRACKET");
      break;

    case ENDPROLOG:
      printf("%-25s", "ENDPROLOG");
      break;

    case VARIABLE:
      printf("%-25s", "VARIABLE");
      break;

    case FUNCTION:
      printf("%-25s", "FUNCTION");
      break;

    case KEYWORD:
      printf("%-25s", "KEYWORD");
      break;

    case DATATYPE:
      printf("%-25s", "DATATYPE");
      break;

    case COLON:
      printf("%-25s", "COLON");
      break;

    case ENDOFFILE:
      printf("%-25s", "ENDOFFILE");
      break;

    case NOTEQUAL:
      printf("%-25s", "NOTEQUAL");
      break;

    case STRING:
      printf("%-25s", "STRING");
      break;
  }

  printf("%s", lexeme.code.data);
  printf("\n");
}

int main() {
  Lexeme l;
  do {
    l = GetLexeme();
    PrintLexeme(l);
  } while (l.kind != ENDOFFILE);

  return 0;
}
