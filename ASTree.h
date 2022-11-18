#ifndef __ASTREE_H__
#define __ASTREE_H__

#include "symbol.h"

typedef struct ASTree {
  struct Symbol node;
  struct LList *children;
  bool has_children;
} AST;

AST *ASTreeInit();
AST *ASTreeCreateNode(AST *tree, symbol node);
AST *ASTreeInsertFirstChild(AST *tree, AST *child);
AST *ASTreeInsertAnotherChild(AST *tree, AST *child);
#endif