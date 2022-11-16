#ifndef __PARSER_H__
#define __PARSER_H__

#include "include.h"
#include "mystring.h"

// parser stuff
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
typedef struct Terminal {
  terminal_kind kind;
  string code;
} terminal;

terminal GetTerminal();
int ChooseRule(nonterminal_kind nonterminal, terminal_kind nextTerminal);
bool STARTgram();
bool PROGgram();
bool START_PROLOGgram();
bool END_PROLOGgram();
bool CODEgram();
bool BODYgram();
bool INNER_SCOPEgram();
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
// parser stuff

#endif