#ifndef __PARSERV2_H__
#define __PARSERV2_H__

#include "include.h"
#include "mystring.h"
#include "nonterminals.h"
#include "scanner.h"
#include "terminal.h"
#include "terminals.h"

typedef struct Terminal terminal;
typedef struct Symbol symbol;
typedef struct ASTree ASTree;
typedef struct LList LList;
typedef struct LList_element LList_element;
typedef struct Parser Parser;

typedef struct Parser {
  struct ASTree *root;
  struct ASTree *current;
  struct Terminal LLfirst;
} Parser;

void ruleSTART(Parser *parser);
void rulePROG(Parser *parser);
void ruleSTART_PROLOG(Parser *parser);

#endif