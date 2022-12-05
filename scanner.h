#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "include.h"
#include "mystring.h"


/**
 * @enum FSMstates
 * 
 */
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
  Comma,
  EndOfFile,
  ExclamationMark,
  ExpectNotEqual,
  NotEqual,
  String,
  EscapeSeq,
  StringEnd,
  Error
} state;
/**
 * @enum Lexemes
 * 
 */
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
    COMMA,
    ENDOFFILE,
    NOTEQUAL,
    STRING
  } kind;
  string *code;
} Lexeme;

state transition(state, int);
string *TransformEscSeq(string *);
Lexeme *MakeLexeme(state, string *);
Lexeme *GetLexeme();
void PrintLexeme(Lexeme *);

#endif