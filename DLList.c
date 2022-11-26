#include "DLList.h"

#include "include.h"

DLList *DLListInit() {
  DLList *list = (DLList *)malloc(sizeof(DLList));
  if (list == NULL) {
    exit(99);
  }
  list->first = NULL;
  list->last = NULL;
  list->active = NULL;
  return list;
}
DLList *DLListInsertFirst(DLList *list, AST *data) {
  if (list == NULL) {
    list = DLListInit();
  }
  DLLElement el = (DLLElement)malloc(sizeof(struct DLLElement));
  if (el == NULL) {
    exit(99);
  }
  el->data = data;
  el->next = list->first;
  el->prev = NULL;
  if (list->first != NULL) {
    list->first->prev = el;
  }
  list->first = el;
  if (list->last == NULL) {
    list->last = el;
  }
  
  return list;
}
DLList *DLListInsertLast(DLList *list, AST *data) {
  if (list == NULL) {
    list = DLListInit();
  }
  DLLElement el = (DLLElement)malloc(sizeof(struct DLLElement));
  if (el == NULL) {
    exit(99);
  }
  el->data = data;
  el->next = NULL;
  el->prev = list->last;
  if (list->last != NULL) {
    list->last->next = el;
  }
  list->last = el;
  if (list->first == NULL) {
    list->first = el;
  }
  
  return list;
}
DLList *DLListDeleteLast(DLList *list) {
  if (list == NULL) {
    return NULL;
  }
  if (list->last == NULL) {
    return list;
  }
  DLLElement el = list->last;
  if (el->prev != NULL) {
    el->prev->next = NULL;
  }
  list->last = el->prev;
  if (list->first == el) {
    list->first = NULL;
  }
  free(el);
  return list;
}
DLList *DLListInsertAfter(DLList *list, AST *data) {
  if (list == NULL) {
    list = DLListInit();
  }
  if (list->active == NULL) {
    return DLListInsertFirst(list, data);
  }
  DLLElement el = (DLLElement)malloc(sizeof(struct DLLElement));
  if (el == NULL) {
    exit(99);
  }
  el->data = data;
  el->next = list->active->next;
  el->prev = list->active;
  if (list->active->next != NULL) {
    list->active->next->prev = el;
  }
  list->active->next = el;
  if (list->last == list->active) {
    list->last = el;
  }
  
  return list;
}
DLList *DLListNext(DLList *list) {
  if (list == NULL) {
    return NULL;
  }
  if (list->active == NULL) {
    return list;
  }
  list->active = list->active->next;
  return list;
}
DLList *DLLFirst(DLList *list) {
  if (list == NULL) {
    return NULL;
  }
  list->active = list->first;
  return list;
}
