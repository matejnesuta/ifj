#include "ASTree.h"

#include "LList.h"
#include "include.h"
#include "logger.h"
#include "symbol.h"

AST *ASTreeInit() {
  AST *tree = (AST *)malloc(sizeof(struct ASTree));
  if (tree == NULL) {
    exit(99);
  };
  logger("ASTreeInit", "ASTree initialized");
  tree->children = NULL;
  logger("ASTreeInit", "children initialized");
  tree->has_children = false;
  logger("ASTreeInit", "has_children initialized");
  return tree;
}

AST *ASTreeCreateNode(symbol *node) {
  logger("ASTreeCreateNode", "ASTreeCreateNode started");
  AST *tree = ASTreeInit();
  logger("ASTreeCreateNode", "Creating node");
  tree->node = node;
  logger("ASTreeCreateNode", "Node created");
  return tree;
}

AST *ASTreeInsertFirstChild(AST *tree, AST *child) {
  logger("ASTreeInsertFirstChild", "Inserting first child");
  LList *list = LListInit();
  logger("ASTreeInsertFirstChild", "LList initialized");
  list = LListInsertFirstChild(list, child);
  logger("ASTreeInsertFirstChild", "Child inserted");
  tree->children = list;
  logger("ASTreeInsertFirstChild", "Children set");
  tree->has_children = true;
  logger("ASTreeInsertFirstChild", "has_children set");
  return tree;
}

AST *ASTreeInsertAnotherChild(AST *tree, AST *child) {
  tree->children = LListInsertAnotherChild(tree->children, child);
  logger("ASTreeInsertAnotherChild", "Child inserted");
  return tree;
}
