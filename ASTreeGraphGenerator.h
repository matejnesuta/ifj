#ifndef __ASTREEGRAPHGENERATOR_H__
#define __ASTREEGRAPHGENERATOR_H__

#include "ASTree.h"
#include "LList.h"

const char *GetNonterminalName(nonterminal_kind nonterminal);
void ASTreePrintChildrenRec(AST *tree, FILE *f);
void ASTreePrintChildren(AST *tree);
char *add_to_lvl(const char *s1, const char *s2);

#endif