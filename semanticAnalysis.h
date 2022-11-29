#ifndef __SEMANTIC_ANALYSIS_H__
#define __SEMANTIC_ANALYSIS_H__

#include "ASTree.h"
#include "LList.h"

void SemanticAnalysis(AST *root);

void CheckFunctionCallsAndDeclarations(AST *tree);
void LoadFunctionCallsAndDeclarationsRecursively(AST *tree, LList *funcCalls,
                                                 LList *funcDecls);
bool FindFuncDeclForFuncCall(LList_element *funcCall, LList *funcDecls);

#endif