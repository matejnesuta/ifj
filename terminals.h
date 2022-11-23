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
  dollar, // used in expr parser
  shift, //used in expr parser
  reduce, //used in expr parser
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

  
} terminal_kind;

#endif