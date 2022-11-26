#ifndef __DLList_h__
#define __DLList_h__
#include "symbol.h"
#include "ASTree.h"

typedef struct DLLElement {
    struct DLLElement *next;
    struct DLLElement *prev;
    struct ASTree *data;
} *DLLElement;

typedef struct {
	DLLElement first;
	DLLElement active;
	DLLElement last;
} DLList;

DLList *DLListInit();
DLList *DLListInsertFirst(DLList *list, AST *data);
DLList *DLListInsertLast(DLList *list, AST *data);
DLList *DLListDeleteLast(DLList *list);
DLList *DLListInsertAfter(DLList *list, AST *data);
DLList *DLListNext(DLList *list);
DLList *DLLFirst(DLList *list);

#endif