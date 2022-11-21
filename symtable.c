#include "symtable.h"
#include "scanner.h"
#include <stdlib.h>
#include "mystring.h"

void BSTInit (tBSTNodePtr *RootPtr) {
    *RootPtr = NULL;
}

void BSTDispose (tBSTNodePtr *RootPtr) {
    if (*RootPtr != NULL) {
        BSTDispose(&(*RootPtr)->LPtr);
        BSTDispose(&(*RootPtr)->RPtr);
        free((*RootPtr)->Key);
        (*RootPtr)->Key = NULL;

        if ((*RootPtr)->nodeDataType == datatypeFunc) {
            ResetString(&(((tFunction *)((*RootPtr)->Data))->params));
        }

        free((*RootPtr)->Data);
        (*RootPtr)->Data = NULL;

        free(*RootPtr);
        *RootPtr = NULL;
    }
}

void BSTInsert (tBSTNodePtr *RootPtr, char *Key, void *Data, tNodeDataType nodeDataType) {
    if (*RootPtr == NULL) {
        *RootPtr = malloc(sizeof(struct tBSTNode));
        (*RootPtr)->Key = malloc(sizeof(char) * (strlen(Key) + 1));
        strcpy((*RootPtr)->Key, Key);
        (*RootPtr)->Data = Data;
        (*RootPtr)->nodeDataType = nodeDataType;
        (*RootPtr)->LPtr = NULL;
        (*RootPtr)->RPtr = NULL;
    } else {
        if (strcmp(Key, (*RootPtr)->Key) < 0) {
            BSTInsert(&(*RootPtr)->LPtr, Key, Data, nodeDataType);
        } else if (strcmp(Key, (*RootPtr)->Key) > 0) {
            BSTInsert(&(*RootPtr)->RPtr, Key, Data, nodeDataType);
        } else {
            (*RootPtr)->Data = Data;
        }
    }
}

tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char *Key) {
    if (RootPtr == NULL) {
        return NULL;
    } else {
        if (strcmp(Key, RootPtr->Key) < 0) {
            return BSTSearch(RootPtr->LPtr, Key);
        } else if (strcmp(Key, RootPtr->Key) > 0) {
            return BSTSearch(RootPtr->RPtr, Key);
        } else {
            return RootPtr;
        }
    }
}

void BSTReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
    if ((*RootPtr)->RPtr != NULL) {
        BSTReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
        return;
    }

    PtrReplaced->Key = (*RootPtr)->Key;
    PtrReplaced->Data = (*RootPtr)->Data;
    PtrReplaced->nodeDataType = (*RootPtr)->nodeDataType;

    tBSTNodePtr tmp = *RootPtr;
    *RootPtr = (*RootPtr)->LPtr;
    free(tmp);
}

