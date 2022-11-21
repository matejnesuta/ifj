#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__
#include "mystring.h"

// BINARY SEARCH TREE //

typedef enum {
    datatypeVar,
    datatypeFunc,
} tNodeDataType;

typedef struct tBSTNode{
    struct tBSTNode *LPtr;
    struct tBSTNode *RPtr;
    tNodeDataType nodeDataType;
    char *Key;
    void *Data;
} *tBSTNodePtr;

void BSTInit (tBSTNodePtr *);
void BSTDispose (tBSTNodePtr *);
void BSTInsert (tBSTNodePtr *, char *, void *, tNodeDataType);
tBSTNodePtr BSTSearch (tBSTNodePtr, char *);
void BSTDelete (tBSTNodePtr *, char *);

// SYMTABLE //

typedef struct variable {
    int dataType;
} tVariable;

typedef struct function {
    int returnType;
    bool defined;
    bool declared;
    string params;
    string paramnames[12];

} tFunction;

typedef struct symtable {
    tBSTNodePtr root;
} tSymtable;

void symtableInit (tSymtable *);

void symtableDispose (tSymtable *);

void symtableInsertFunc (tSymtable *, string);

void symtableInsertBuiltInFunc (tSymtable *);

void symtableInsertVar (tSymtable *, string);

tBSTNodePtr symtableSearch (tSymtable *, string);

void symtableDelete (tSymtable *, string);

#endif


