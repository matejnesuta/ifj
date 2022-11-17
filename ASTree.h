#ifndef __ASTREE_H__
#define __ASTREE_H__

#include "symbol.h"

typedef struct ASTree {
  struct Symbol *node;
  struct LList *children;
  bool has_children;
} AST;

#endif