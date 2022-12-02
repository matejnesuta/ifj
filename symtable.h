#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__
#include "include.h"
#include "mystring.h"
#include "scanner.h"

// BINARY SEARCH TREE //

typedef enum {
  datatypeVar,
  datatypeFunc,
} tNodeDataType;

typedef struct function {
  int returnType;
  bool defined;
  bool declared;
  string params;
  string paramnames[12];
} tFunction;

typedef struct variable {
  int dataType;
} tVariable;

typedef union data {
  tFunction *func;
  tVariable *var;
} tData;

typedef struct bst_node_t {
  struct bst_node_t *LPtr;
  struct bst_node_t *RPtr;
  tNodeDataType nodeDataType;
  char *key;
  tData *data;
} * bst_node_ptr_t;

void bst_init(bst_node_ptr_t *);
void bst_dispose(bst_node_ptr_t *);
void bst_insert(bst_node_ptr_t *, char *, tData *, tNodeDataType);
bst_node_ptr_t bst_search(bst_node_ptr_t, char *);
void bst_delete(bst_node_ptr_t *, char *);
void bst_replace_by_rightmost(bst_node_ptr_t, bst_node_ptr_t *);

// SYMTABLE //
typedef struct symtable {
  bst_node_ptr_t root;
} tSymtable;

void symtable_init(tSymtable *);

void symtable_dispose(tSymtable *);

void symtable_insert_func(tSymtable *, string);

void symtable_insert_builtin_func(tSymtable *);

void symtable_insert_var(tSymtable *, string);

bst_node_ptr_t symtable_search(tSymtable *, string);

void symtable_delete(tSymtable *, string);

#endif
