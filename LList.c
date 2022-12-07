/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file LList.c
 * @authors Stefan Peknik xpekni01
 * @brief LList
 * @date 2022-11-17
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "LList.h"

#include "error.h"
#include "include.h"
#include "logger.h"

/**
 * @brief Initializes list
 * @return LList*
 */
LList *LListInit() {
  LList *list = (LList *)malloc(sizeof(LList));
  if (list == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  list->first = NULL;
  list->active = NULL;
  return list;
}
/**
 * @brief Disposes list
 * @param list List to dispose
 */
void LListDispose(LList *list) {
  if (list == NULL) {
    return;
  }
  LList_element *el = list->first;
  while (el != NULL) {
    LList_element *next = el->next;
    free(el);
    el = next;
  }
  free(list);
}
/**
 * @brief Inserts child to list
 *
 * @param list List to insert to
 * @param child AST element
 * @return LList*
 */
LList *LListInsertChild(LList *list, AST *child) {
  if (list == NULL) {
    list = LListInit();
  }
  if (list->first == NULL) {
    return LListInsertFirstChild(list, child);
  }
  return LListInsertAnotherChild(list, child);
}
/**
 * @brief Inserts first child to list
 *
 * @param list List to insert to
 * @param child AST element
 * @return LList*
 */
LList *LListInsertFirstChild(LList *list, AST *child) {
  if (list == NULL) {
    list = LListInit();
  }
  if (list->first == NULL) {
    LList_element *el = (LList_element *)malloc(sizeof(struct LList_element));
    if (el == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    el->tree = child;
    el->next = NULL;
    list->first = el;
    list->active = el;
    return list;
  } else {
    LList_element *old_first = list->first;
    LList_element *new_first =
        (LList_element *)malloc(sizeof(struct LList_element));
    if (new_first == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    new_first->tree = child;
    new_first->next = old_first;
    list->first = new_first;
    list->active = new_first;
    return list;
  }
  return list;
}
/**
 * @brief Inserts another child to list
 *
 * @param list List to insert to
 * @param child AST element
 * @return LList*
 */
LList *LListInsertAnotherChild(LList *list, AST *child) {
  LList_element *el = (LList_element *)malloc(sizeof(struct LList_element));
  if (el == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  el->tree = child;
  el->next = NULL;
  list->active->next = el;
  list->active = el;
  return list;
}
