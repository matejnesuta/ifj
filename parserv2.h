#ifndef __PARSERV2_H__
#define __PARSERV2_H__

#include "include.h"
#include "mystring.h"
#include "terminal.h"

typedef struct Parser {
  struct ASTree *root;
  struct ASTree *current;
  struct Terminal LLfirst;
} Parser;

void rule_START();
void rule_PROG(Parser *parser);
void rule_START_PROLOG(Parser *parser);
void rule_END_PROLOG(Parser *parser);
void rule_CODE(Parser *parser);
#endif