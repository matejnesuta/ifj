#ifndef __LLIST_H__
#define __LLIST_H__

#include "ASTree.h"

typedef struct LList_element {
  struct ASTree *tree;
  struct LList_element *next;
} LList_element;

typedef struct LList {
  struct LList_element *first;
  struct LList_element *active;
} LList;

LList *LListInit();
LList *LListInsertFirstChild(LList *list, AST *child);
LList *LListInsertAnotherChild(LList *list, AST *child);
LList *PopOutStuffToBeReduced(LList **popped, LList *list);
LList *LListRemoveFirst(LList *list);
#endif