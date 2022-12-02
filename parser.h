#ifndef __PARSER_H__
#define __PARSER_H__

#include "include.h"
#include "mystring.h"
#include "nonterminals.h"
#include "terminal.h"

typedef struct Parser {
  struct ASTree *root;
  struct ASTree *current;
  struct Terminal *buffer;
  struct Terminal *LLfirst;
} Parser;

void UpdateLLfirst(Parser *);
terminal *GetTerminal();
int ChooseRule(nonterminal_kind, Parser *);

void rule_START();
void rule_PROG(Parser *);
void rule_START_PROLOG(Parser *);
void rule_END_PROLOG(Parser *);
void rule_CODE(Parser *);
void rule_BODY(Parser *);
void rule_INNER_SCOPE(Parser *);
void rule_RETURN_VALUE(Parser *);
void rule_FUNC_CALL(Parser *);
void rule_FUNC_CALL_ARGS(Parser *);
void rule_NEXT_ARG(Parser *);
void rule_ARG(Parser *);
void rule_LITERAL(Parser *);
void rule_FUNC_DECLARE(Parser *);
void rule_FUNC_DECLARE_BODY(Parser *);
void rule_ARG_TYPE(Parser *);
void rule_IF_ELSE(Parser *);
void rule_OPERATOR(Parser *);
void rule_RIGHT_SIDE(Parser *);
void rule_EXP(Parser *);

#endif