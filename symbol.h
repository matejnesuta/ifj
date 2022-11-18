#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "nonterminals.h"
#include "terminal.h"

typedef struct Symbol {
  nonterminal_kind nonterminal;
  struct Terminal *terminal;
  bool is_terminal;
} symbol;

symbol *SymbolCreateNonterminal(nonterminal_kind nonterminal);
symbol *SymbolCreateTerminal(terminal *terminal);
#endif