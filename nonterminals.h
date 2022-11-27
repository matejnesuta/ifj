#ifndef __NONTERMINALS_H__
#define __NONTERMINALS_H__

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
  IF_ELSE,
  OPERATOR,
  RIGHT_SIDE,
  EXP,
  E //used for expression parsing
} nonterminal_kind;

#endif