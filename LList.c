#include "LList.h"

#include "include.h"

LList *LListInit() {
  LList *list = (LList *)malloc(sizeof(LList));
  if (list == NULL) {
    exit(99);
  }
  list->first = NULL;
  list->active = NULL;
  return list;
}

LList *LListInsertFirstChild(LList *list, AST child) {
  LList_element *el = (AST *)malloc(sizeof(AST));
  if (el == NULL) {
    exit(99);
  }
  el->tree = child;
  el->next = NULL;
  list->first = el;
  list->active = el;
  return list;
}

LList *LListInsertAnotherChild(LList *list, AST child) {
  LList_element *el = (AST *)malloc(sizeof(AST));
  if (el == NULL) {
    exit(99);
  }
  el->tree = child;
  el->next = NULL;
  list->active->next = el;
  list->active = el;
  return list;
}