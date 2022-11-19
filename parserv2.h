#ifndef __PARSERV2_H__
#define __PARSERV2_H__

#include "include.h"
#include "mystring.h"
#include "terminal.h"
#include "nonterminals.h"

typedef struct Parser {
  struct ASTree *root;
  struct ASTree *current;
  struct Terminal *buffer;
  struct Terminal *LLfirst;
} Parser;

void UpdateLLfirst(Parser *parser);
terminal *GetTerminal();
int ChooseRule(nonterminal_kind nonterminal, terminal_kind nextTerminal);

void rule_START();
void rule_PROG(Parser *parser);
void rule_START_PROLOG(Parser *parser);
void rule_END_PROLOG(Parser *parser);
void rule_CODE(Parser *parser);
void rule_BODY(Parser *parser);
void rule_INNER_SCOPE(Parser *parser);
void rule_RETURN_VALUE(Parser *parser);
void rule_FUNC_CALL(Parser *parser);
void rule_FUNC_CALL_ARGS(Parser *parser);
void rule_NEXT_ARG(Parser *parser);
void rule_ARG(Parser *parser);
void rule_LITERAL(Parser *parser);
void rule_FUNC_DECLARE(Parser *parser);
void rule_FUNC_DECLARE_BODY(Parser *parser);
void rule_ARG_TYPE(Parser *parser);
void rule_IF_ELSE(Parser *parser);
void rule_DECIDE(Parser *parser);
void rule_OPERATOR(Parser *parser);
void rule_BACK_TO_GRAMMAR(Parser *parser);
void rule_EXPRESSION(Parser *parser);

#endif