/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file ASTree.c
 * @authors Stefan Peknik xpekni01
 * @brief ASTree
 * @date 2022-11-16
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "ASTree.h"

#include "LList.h"
#include "error.h"
#include "include.h"
#include "symbol.h"

/**
 * @brief Creates new ASTree
 *
 * @return AST*
 */
AST *ASTreeInit() {
  AST *tree = (AST *)malloc(sizeof(struct ASTree));
  if (tree == NULL) {
    ErrorExit(99, "Malloc failed!");
  };
  tree->children = LListInit();
  tree->has_children = false;
  return tree;
}
/**
 * @brief Creates new node to ASTree
 *
 * @param node Symbol *
 * @return AST*
 */
AST *ASTreeCreateNode(symbol *node) {
  AST *tree = ASTreeInit();
  tree->node = node;
  return tree;
}
/**
 * @brief Inserts first child to ASTree
 *
 * @param tree Tree to insert to
 * @param child AST element
 * @return AST*
 */
AST *ASTreeInsertFirstChild(AST *tree, AST *child) {
  LList *list = LListInit();
  list = LListInsertFirstChild(list, child);
  tree->children = list;
  tree->has_children = true;
  return tree;
}
/**
 * @brief Inserts another child to ASTree
 *
 * @param tree Tree to insert to
 * @param child AST element
 * @return AST*
 */
AST *ASTreeInsertAnotherChild(AST *tree, AST *child) {
  tree->children = LListInsertAnotherChild(tree->children, child);
  return tree;
}
