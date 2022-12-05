#ifndef __ASTREE_H__
#define __ASTREE_H__

#include "symbol.h"

/** Abstract syntax tree*/
typedef struct ASTree {
  struct Symbol *node;
  struct LList *children;
  bool has_children;
} AST;

AST *ASTreeInit();
AST *ASTreeCreateNode(symbol *);
AST *ASTreeInsertFirstChild(AST *, AST *);
AST *ASTreeInsertAnotherChild(AST *, AST *);
#endif
