#ifndef __LLIST_H__
#define __LLIST_H__

#include "ASTree.h"
/** Element of list*/
typedef struct LList_element {
  struct ASTree *tree;
  struct LList_element *next;
} LList_element;

/** Linked list*/
typedef struct LList {
  struct LList_element *first;
  struct LList_element *active;
} LList;

LList *LListInit();
LList *LListInsertChild(LList *, AST *);
LList *LListInsertFirstChild(LList *, AST *);
LList *LListInsertAnotherChild(LList *, AST *);
#endif
