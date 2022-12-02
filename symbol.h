#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "nonterminals.h"
#include "terminal.h"

/** Symbol */
typedef struct Symbol {
  nonterminal_kind nonterminal;
  struct Terminal *terminal;
  bool is_terminal;
} symbol;

symbol *SymbolCreateNonterminal(nonterminal_kind);
symbol *SymbolCreateTerminal(terminal *);
#endif