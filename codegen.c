#include "codegen.h"

#include "LList.h"
#include "logger.h"
// #include "symtable.c"
#include "symtable.h"

void ASTreeRecGoThru(AST *tree, tSymtable *global) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;  // if tree has no children (should be error)
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      logger("codegen", child->tree->node->terminal->code->data);
      printf("%d\n", child->tree->node->terminal->kind);
      // do something with terminal

    } else {
      // logger("codegen", "ahoj");
      // do something with nonterminal
      ASTreeRecGoThru(child->tree,
                      global);  // get one level deeper thru nonterminal
    }
    child = child->next;
  }
}

void codegen(AST *tree) {
  tSymtable global;
  symtable_init(&global);
  ASTreeRecGoThru(tree, &global);
  return;
}