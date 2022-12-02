#ifndef __ASTREEGRAPHGENERATOR_H__
#define __ASTREEGRAPHGENERATOR_H__

#include "ASTree.h"
#include "LList.h"

char *GetNonterminalName(nonterminal_kind);
void ASTreePrintChildrenRec(AST *, FILE *);
void ASTreePrintChildren(AST *);
char *add_to_lvl(const char *, const char *);
char *formatNull();
char *formatTerminal(AST *);
char *formatNonterminal(AST *);
char *GetTerminalName(terminal *);

#endif