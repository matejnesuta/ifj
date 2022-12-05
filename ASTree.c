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
#include "logger.h"
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
  logger("ASTreeInit", "ASTree initialized");
  tree->children = LListInit();
  logger("ASTreeInit", "children initialized");
  tree->has_children = false;
  logger("ASTreeInit", "has_children initialized");
  return tree;
}
/**
 * @brief Creates new node to ASTree
 *
 * @param node Symbol *
 * @return AST*
 */
AST *ASTreeCreateNode(symbol *node) {
  logger("ASTreeCreateNode", "ASTreeCreateNode started");
  AST *tree = ASTreeInit();
  logger("ASTreeCreateNode", "Creating node");
  tree->node = node;
  logger("ASTreeCreateNode", "Node created");
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
/**
 * @brief Inserts another child to ASTree
 *
 * @param tree Tree to insert to
 * @param child AST element
 * @return AST*
 */
AST *ASTreeInsertAnotherChild(AST *tree, AST *child) {
  tree->children = LListInsertAnotherChild(tree->children, child);
  logger("ASTreeInsertAnotherChild", "Child inserted");
  return tree;
}
