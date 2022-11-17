#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "terminal.h"
#include "nonterminals.h"

typedef struct Symbol {
  nonterminal_kind nonterminal;
  struct Terminal terminal;
  bool is_terminal;
} symbol;

#endif