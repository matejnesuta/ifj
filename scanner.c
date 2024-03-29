/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file scanner.c
 * @authors Stefan Peknik xpekni01
 * @brief scanner
 * @date 2022-10-05
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "scanner.h"

#include "error.h"
#include "mystring.h"

/**
 * @brief Creates new scanner
 *
 * @param currIn state
 * @param edge
 * @return state
 */
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
        case ',':
          return Comma;
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
      if (edge == '\n' || edge == EOF) return Start;
      return Error;

    case BlockComment:
      if (edge != '*' && edge != EOF) return BlockComment;
      if (edge == '*') return ExpectEndBlockComment;
      return Error;

    case ExpectEndBlockComment:
      if (edge != '/' && edge != EOF) return BlockComment;
      if (edge == '/') return Start;
      return Error;

    case Plus:
      return Error;

    case Minus:
      return Error;

    case Integer:
      if (isdigit(edge)) return Integer;
      if (edge == '.') return decDot;
      if (edge == 'e' || edge == 'E') return Exponent;
      return Error;

    case decDot:
      if (isdigit(edge)) return FloatIntDotInt;
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
      if (edge == '/') return expectCommAfterProlog;
      if (isspace(edge)) return StartPrologEnd;
      return Error;

    case expectCommAfterProlog:
      if (edge == '/') return lineCommentAfterProlog;
      if (edge == '*') return blockCommentAfterProlog;
      return Error;

    case lineCommentAfterProlog:
      if (edge != '\n' && edge != EOF) return lineCommentAfterProlog;
      if (edge == '\n' || edge == EOF) return StartPrologEnd;
      return Error;

    case blockCommentAfterProlog:
      if (edge != '*' && edge != EOF) return blockCommentAfterProlog;
      if (edge == '*') return expectEndBlockCommentAfterProlog;
      return Error;

    case expectEndBlockCommentAfterProlog:
      if (edge != '/' && edge != EOF) return blockCommentAfterProlog;
      if (edge == '/') return StartPrologEnd;
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

    case Comma:
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
    default:
      printf("something bad here happened in transition\n");
      ErrorExit(1, "Error in structure of current lexeme");
  }
  ErrorExit(69420, "No way you got here!");
  return 69420;
}
/**
 * @brief Transform escaped sequence
 *
 * @param code
 * @return string*
 */
string *TransformEscSeq(string *code) {
  string *new = SetupString();
  char *escSeq = malloc(3 * sizeof(char));
  if (escSeq == NULL) {
    ErrorExit(99, "Error in structure of current lexeme");
  }
  for (size_t i = 1; i < code->size - 2; i++) {
    if (code->data[i] == '\\') {
      if (code->data[i + 1] == '"') {
        sprintf(escSeq, "\\%03d", (int)'\"');
        new = ConcatString(new, escSeq);
        i += 1;
        continue;
      }
      if (code->data[i + 1] == 'n') {
        sprintf(escSeq, "\\%03d", (int)'\n');
        new = ConcatString(new, escSeq);
        i += 1;
        continue;
      }
      if (code->data[i + 1] == 't') {
        sprintf(escSeq, "\\%03d", (int)'\t');
        new = ConcatString(new, escSeq);
        i += 1;
        continue;
      }
      if (code->data[i + 1] == '\\') {
        sprintf(escSeq, "\\%03d", (int)'\\');
        new = ConcatString(new, escSeq);
        i += 1;
        continue;
      }
      if (code->data[i + 1] == '$') {
        sprintf(escSeq, "\\%03d", (int)'$');
        new = ConcatString(new, escSeq);
        i += 1;
        continue;
      }
      if (code->data[i + 1] == 'x') {
        char hexaStr[] = {code->data[i + 2], code->data[i + 3], '\0'};
        char *junk;
        int hexaVal = (int)strtol(hexaStr, &junk, 16);
        if (junk[0] != '\0') {
          new = AddToString(new, code->data[i]);
          new = AddToString(new, code->data[i + 1]);
          i += 1;
        } else if (hexaVal >= 0x01 && hexaVal <= 0xff) {
          sprintf(escSeq, "\\%03d", hexaVal);
          new = ConcatString(new, escSeq);
          i += 3;
        } else {
          new = AddToString(new, code->data[i]);
          new = AddToString(new, code->data[i + 1]);
          i += 1;
        }
        continue;
      }
      if (isdigit(code->data[i + 1]) && isdigit(code->data[i + 2]) &&
          isdigit(code->data[i + 3])) {
        char octaStr[] = {code->data[i + 1], code->data[i + 2],
                          code->data[i + 3], '\0'};
        char *junk;
        int octaVal = (int)strtol(octaStr, &junk, 8);
        if (junk[0] != '\0') {
          new = AddToString(new, code->data[i]);
          new = AddToString(new, code->data[i + 1]);
          i++;
        } else if (octaVal >= 1 && octaVal <= 255) {  // 1 = 01 , 255 = 0377
          sprintf(escSeq, "\\%03d", octaVal);
          new = ConcatString(new, escSeq);
          i += 3;
        } else {
          new = AddToString(new, code->data[i]);
          new = AddToString(new, code->data[i + 1]);
          i++;
        }
      } else {
        new = AddToString(new, code->data[i]);
        new = AddToString(new, code->data[i + 1]);
        i++;
      }
    } else {
      new = AddToString(new, code->data[i]);
    }
  }
  new = AddToString(new, '\0');
  free(code->data);
  free(code);

  string *no_whitespace = SetupString();
  for (size_t i = 0; i < new->size; i++) {
    if (isspace(new->data[i])) {
      char *whitespace = malloc(5 * sizeof(char));
      if (whitespace == NULL) {
        ErrorExit(99, "malloc failed");
      }
      sprintf(whitespace, "\\%03d", (int)new->data[i]);
      no_whitespace = ConcatString(no_whitespace, whitespace);
    } else {
      no_whitespace = AddToString(no_whitespace, new->data[i]);
    }
  }
  no_whitespace = AddToString(no_whitespace, '\0');
  free(new->data);
  free(new);

  return no_whitespace;
}
/**
 * @brief Make Lexeme
 *
 * @param final state
 * @param code
 * @return Lexeme*
 */
Lexeme *MakeLexeme(state final, string *code) {
  Lexeme *lexeme = (Lexeme *)malloc(sizeof(struct Lexeme));
  if (lexeme == NULL) {
    ErrorExit(1, "Error in structure of current lexeme");
  }
  switch (final) {
    case Multiply:
      lexeme->kind = MULTIPLY;
      lexeme->code = code;
      return lexeme;
    case Division:
      lexeme->kind = DIVISION;
      lexeme->code = code;
      return lexeme;
    case Minus:
      lexeme->kind = MINUS;
      lexeme->code = code;
      return lexeme;
    case Plus:
      lexeme->kind = PLUS;
      lexeme->code = code;
      return lexeme;
    case Integer:
      lexeme->kind = INTEGER;
      lexeme->code = code;
      return lexeme;
    case FloatIntDotInt:
      lexeme->kind = FLOAT;
      lexeme->code = code;
      return lexeme;
    case FloatIntAfterExponent:
      lexeme->kind = FLOAT;
      lexeme->code = code;
      return lexeme;
    case Concat:
      lexeme->kind = CONCAT;
      lexeme->code = code;
      return lexeme;
    case Assign:
      lexeme->kind = ASSIGN;
      lexeme->code = code;
      return lexeme;
    case Equal:
      lexeme->kind = EQUAL;
      lexeme->code = code;
      return lexeme;
    case Gt:
      lexeme->kind = GT;
      lexeme->code = code;
      return lexeme;
    case Ge:
      lexeme->kind = GE;
      lexeme->code = code;
      return lexeme;
    case Lt:
      lexeme->kind = LT;
      lexeme->code = code;
      return lexeme;
    case Le:
      lexeme->kind = LE;
      lexeme->code = code;
      return lexeme;
    case StartPrologEnd:
      lexeme->kind = STARTPROLOG;
      lexeme->code = code;
      return lexeme;
    case Semicolon:
      lexeme->kind = SEMICOLON;
      lexeme->code = code;
      return lexeme;
    case LeftBracket:
      lexeme->kind = LEFTBRACKET;
      lexeme->code = code;
      return lexeme;
    case RightBracket:
      lexeme->kind = RIGHTBRACKET;
      lexeme->code = code;
      return lexeme;
    case LeftCurlyBracket:
      lexeme->kind = LEFTCURLYBRACKET;
      lexeme->code = code;
      return lexeme;
    case RightCurlyBracket:
      lexeme->kind = RIGHTCURLYBRACKET;
      lexeme->code = code;
      return lexeme;
    case Epilog1:
      lexeme->kind = ENDPROLOG;
      lexeme->code = code;
      return lexeme;
    case Epilog2:
      lexeme->kind = ENDPROLOG;
      lexeme->code = code;
      return lexeme;
    case Identifier:
      if (code->data[0] == '$') {
        lexeme->kind = VARIABLE;
        lexeme->code = code;
        return lexeme;
      }
      size_t LengthdataTypes = 7;
      char *dataTypes[] = {"?int",  "?float", "?string", "int",
                           "float", "string", "void"};
      for (size_t i = 0; i < LengthdataTypes; i++) {
        if (strcmp(code->data, dataTypes[i]) == 0) {
          lexeme->kind = DATATYPE;
          lexeme->code = code;
          return lexeme;
        }
      }
      size_t LengthKeywords = 6;
      char *keyWords[] = {"else", "function", "if", "null", "return", "while"};
      for (size_t i = 0; i < LengthKeywords; i++) {
        if (strcmp(code->data, keyWords[i]) == 0) {
          lexeme->kind = KEYWORD;
          lexeme->code = code;
          return lexeme;
        }
      }
      lexeme->kind = FUNCTION;
      lexeme->code = code;
      return lexeme;
    case Colon:
      lexeme->kind = COLON;
      lexeme->code = code;
      return lexeme;
    case Comma:
      lexeme->kind = COMMA;
      lexeme->code = code;
      return lexeme;
    case EndOfFile:
      lexeme->kind = ENDOFFILE;
      lexeme->code = code;
      return lexeme;
    case NotEqual:
      lexeme->kind = NOTEQUAL;
      lexeme->code = code;
      return lexeme;
    case StringEnd:
      code = TransformEscSeq(code);
      lexeme->kind = STRING;
      lexeme->code = code;
      return lexeme;

    default:
      // case Start:
      // case BlockComment:
      // case LineComment:
      // case ExpectEndBlockComment:
      // case ExpectEqual:
      // case ExpectStartProlog1:
      // case ExpectStartProlog2:
      // case ExpectStartProlog3:
      // case ExpectStartProlog4:
      // case Exponent:
      // case PlusMinus:
      // case EnableNull:
      // case Variable:
      // case ExclamationMark:
      // case ExpectNotEqual:
      // case String:
      // case EscapeSeq:
      // case Error:
      ErrorExit(1, "Error in structure of current lexeme");
  }
  ErrorExit(69420, "how did you get here");
  return NULL;
}
/**
 * @brief Get the Lexeme object
 *
 * @return Lexeme*
 */
Lexeme *GetLexeme() {
  state currIn = Start;
  string *code = SetupString();
  bool ignoredWhitespace = false;
  static bool epilogUsed = false;

  while (true) {
    int edge = getchar();
    code = AddToString(code, edge);
    state next = transition(currIn, edge);

    // stuff to assure that the prolog is at the start of the file
    if (ignoredWhitespace && next == ExpectStartProlog1) {
      ErrorExit(2, "Syntax error");  // "header is missing" should be error in
                                     // syntax analysis
    }
    // stuff to assure that EOF is exactly after epilog
    if (currIn == Epilog1 || currIn == Epilog2) {
      epilogUsed = true;
    }
    if (epilogUsed && ignoredWhitespace && next == EndOfFile) {
      ErrorExit(2, "Syntax error");  // "no EOF" after epilog should be error in
                                     // syntax analysis
    }
    if (next != Lt) {
      ignoredWhitespace = false;
    }
    //

    if (next == EndOfFile) {
      if (currIn == Start) {
        Lexeme *lexeme = (Lexeme *)malloc(sizeof(struct Lexeme));
        if (lexeme == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        lexeme->kind = ENDOFFILE;
        lexeme->code = code;
        return lexeme;
      }
      next = Error;
    }
    if (next == Error) {
      ungetc(edge, stdin);
      code = ReplaceCharInString(code, code->size - 2, '\0');
      return MakeLexeme(currIn, code);
    }
    if (next == Start) {
      ignoredWhitespace = true;
      code = ResetString(code);
    }
    currIn = next;
  }
}
/**
 * @brief Print Lexeme
 *
 * @param lexeme
 */
void PrintLexeme(Lexeme *lexeme) {
  switch (lexeme->kind) {
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

    case COMMA:
      printf("%-25s", "COMMA");
      break;
  }

  printf("%s", lexeme->code->data);
  printf("\n");
}
