#include "LList.h"

#include "include.h"

LList *LListInit() {
  LList *list = (LList *)malloc(sizeof(LList));
  if (list == NULL) {
    exit(99);
  }
  logger("LListInit", "LList initialized");
  list->first = NULL;
  logger("LListInit", "first initialized");
  list->active = NULL;
  logger("LListInit", "active initialized");
  return list;
}

LList *LListInsertChild(LList *list, AST *child) {
  if (list == NULL) {
    list = LListInit();
  }
  if (list->first == NULL) {
    return LListInsertFirstChild(list, child);
  }
  return LListInsertAnotherChild(list, child);
}

LList *LListInsertFirstChild(LList *list, AST *child) {
  if (list == NULL) {
    list = LListInit();
  }
  if (list->first == NULL) {
    LList_element *el = (LList_element *)malloc(sizeof(struct LList_element));
    if (el == NULL) {
      exit(99);
    }
    logger("LListInsertFirstChild", "LList_element initialized");
    el->tree = child;
    logger("LListInsertFirstChild", "tree initialized");
    el->next = NULL;
    logger("LListInsertFirstChild", "next initialized");
    list->first = el;
    logger("LListInsertFirstChild", "first initialized");
    list->active = el;
    logger("LListInsertFirstChild", "active initialized");
    return list;
  } else {
    LList_element *old_first = list->first;
    LList_element *new_first =
        (LList_element *)malloc(sizeof(struct LList_element));
    if (new_first == NULL) {
      exit(99);
    }
    logger("LListInsertFirstChild", "LList_element initialized");
    new_first->tree = child;
    logger("LListInsertFirstChild", "tree initialized");
    new_first->next = old_first;
    logger("LListInsertFirstChild", "next initialized");
    list->first = new_first;
    logger("LListInsertFirstChild", "first initialized");
    list->active = new_first;
    logger("LListInsertFirstChild", "active initialized");
    return list;
  }
  return list;
}

LList *LListInsertAnotherChild(LList *list, AST *child) {
  LList_element *el = (LList_element *)malloc(sizeof(struct LList_element));
  if (el == NULL) {
    exit(99);
  }
  logger("LListInsertAnotherChild", "LList_element initialized");
  el->tree = child;
  logger("LListInsertAnotherChild", "tree initialized");
  el->next = NULL;
  logger("LListInsertAnotherChild", "next initialized");
  list->active->next = el;
  logger("LListInsertAnotherChild", "next initialized");
  list->active = el;
  logger("LListInsertAnotherChild", "active initialized");
  return list;
}