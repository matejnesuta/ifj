#include "symbol.h"
/**
 * @brief Function to create new symbol from nonterminal
 * 
 * @param nonterminal nonterminal
 * @return symbol* 
 */
symbol *SymbolCreateNonterminal(nonterminal_kind nonterminal) {
  symbol *new_symbol = malloc(sizeof(struct Symbol));
  if (new_symbol == NULL) {
    exit(99);
  }
  new_symbol->nonterminal = nonterminal;
  new_symbol->is_terminal = false;
  return new_symbol;
}
/**
 * @brief Function to create new symbol from terminal
 * 
 * @param terminal terminal
 * @return symbol* 
 */
symbol *SymbolCreateTerminal(terminal *terminal) {
  symbol *new_symbol = malloc(sizeof(struct Symbol));
  if (new_symbol == NULL) {
    exit(99);
  }
  new_symbol->terminal = terminal;
  new_symbol->is_terminal = true;
  return new_symbol;
}