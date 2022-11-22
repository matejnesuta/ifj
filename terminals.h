#ifndef __TERMINALS_H__
#define __TERMINALS_H__

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
  notEqualTer,

  dollar // used in expr parser
} terminal_kind;

#endif