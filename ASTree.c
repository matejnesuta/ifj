#include "ASTree.h"

#include "LList.h"
#include "include.h"
#include "symbol.h"

AST *ASTreeInit() {
  AST *tree = (AST *)malloc(sizeof(AST));
  if (tree == NULL) {
    exit(99);
  }
  tree->node = NULL;
  tree->children = NULL;
  tree->has_children = false;
  return tree;
}

AST *ASTreeCreateNode(AST *tree, symbol *node) {
  tree->node = node;
  return tree;
}

AST *ASTreeInsertFirstChild(AST *tree, AST child) {
  LList *list = LListInit();
  list = LListInsertFirstChild(list, child);
  tree->children = list;
  tree->has_children = true;
  return tree;
}

AST *ASTreeInsertAnotherChild(AST *tree, AST child) {
  tree->children = LListInsertAnotherChild(tree->children, child);
  return tree;
}
