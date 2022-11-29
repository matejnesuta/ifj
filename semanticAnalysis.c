#include "semanticAnalysis.h"

#include "logger.h"

void SemanticAnalysis(AST *root) { CheckFunctionCallsAndDeclarations(root); }

void CheckFunctionCallsAndDeclarations(AST *tree) {
  LList *funcCalls = LListInit();
  LList *funcDecls = LListInit();

  LoadFunctionCallsAndDeclarationsRecursively(tree, funcCalls, funcDecls);

  LList_element *funcCall = funcCalls->first;
  while (funcCall != NULL) {
    bool found = FindFuncDeclForFuncCall(funcCall, funcDecls);
    if (!found) {
      logger("CheckFunctionCallsAndDeclarations",
             "Function call not found in declarations");
      exit(3);
    }
    funcCall = funcCall->next;
  }
}

void LoadFunctionCallsAndDeclarationsRecursively(AST *tree, LList *funcCalls,
                                                 LList *funcDecls) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      // do something with terminal
      if (child->tree->node->terminal->kind == functionTer &&
          child->next != NULL && child->next->tree->node->is_terminal &&
          child->next->tree->node->terminal->kind == function_idTer) {
        funcDecls =
            LListInsertChild(funcDecls, child->next->tree);  // add func decl
      }
      if (tree->node->is_terminal == false &&
          tree->node->nonterminal == FUNC_CALL &&
          child->tree->node->is_terminal &&
          child->tree->node->terminal->kind == function_idTer) {
        funcCalls = LListInsertChild(funcCalls, child->tree);  // add func call
      }

    } else {
      // do something with nonterminal
      LoadFunctionCallsAndDeclarationsRecursively(
          child->tree, funcCalls,
          funcDecls);  // get one level deeper thru nonterminal
    }
    child = child->next;
  }
}

bool FindFuncDeclForFuncCall(LList_element *funcCall, LList *funcDecls) {
  LList_element *funcDecl = funcDecls->first;
  while (funcDecl != NULL) {
    if (strcmp(funcCall->tree->node->terminal->code->data,
               funcDecl->tree->node->terminal->code->data) == 0) {
      return true;
    }
    funcDecl = funcDecl->next;
  }
  return false;
}