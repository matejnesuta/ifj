/**
 * -----------------------------------------------------------------
 * IFJ22 compiler implementation
 * @file symtable.c
 * @authors Ilia Markelov xmarke00
 * @brief symtable
 * @date 2022-12-02
 * @copyright Copyright (c) 2022
 * -----------------------------------------------------------------
 */
#include "symtable.h"

/**
 * @brief Creates new BSTree
 *
 * @param TreeRootPtr
 */
void bst_init(bst_node_ptr_t *TreeRootPtr) { *TreeRootPtr = NULL; }

void bst_dispose(bst_node_ptr_t *TreeRootPtr) {
  if (*TreeRootPtr != NULL) {
    bst_dispose(&(*TreeRootPtr)->LPtr);
    bst_dispose(&(*TreeRootPtr)->RPtr);
    free((*TreeRootPtr)->key);
    (*TreeRootPtr)->key = NULL;

    if ((*TreeRootPtr)->nodeDataType == datatypeFunc) {
      for (int i = 0; i < (*TreeRootPtr)->data->func->paramCount; i++) {
        free((*TreeRootPtr)->data->func->paramNames[i].data);
      }
    }
    if ((*TreeRootPtr)->nodeDataType == datatypeVar) {
      free((*TreeRootPtr)->data->var);
    }
    else {
      free((*TreeRootPtr)->data->func);
    }
    free((*TreeRootPtr)->data);
    (*TreeRootPtr)->data = NULL;

    free(*TreeRootPtr);
    *TreeRootPtr = NULL;
  }
}
/**
 * @brief Inserts new node into tree
 *
 * @param TreeRootPtr
 * @param key
 * @param data
 * @param nodeDataType
 */
void bst_insert(bst_node_ptr_t *TreeRootPtr, char *key, tData *data,
                tNodeDataType nodeDataType) {
  if (*TreeRootPtr == NULL) {
    *TreeRootPtr = malloc(sizeof(struct bst_node_t));
    (*TreeRootPtr)->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy((*TreeRootPtr)->key, key);
    (*TreeRootPtr)->data = data;
    (*TreeRootPtr)->nodeDataType = nodeDataType;
    (*TreeRootPtr)->LPtr = NULL;
    (*TreeRootPtr)->RPtr = NULL;
  } else {
    if (strcmp(key, (*TreeRootPtr)->key) < 0) {
      bst_insert(&(*TreeRootPtr)->LPtr, key, data, nodeDataType);
    } else if (strcmp(key, (*TreeRootPtr)->key) > 0) {
      bst_insert(&(*TreeRootPtr)->RPtr, key, data, nodeDataType);
    } else {
      (*TreeRootPtr)->data = data;
    }
  }
}
/**
 * @brief Deletes node from tree
 *
 * @param TreeRootPtr
 * @param key
 */
void bst_delete(bst_node_ptr_t *TreeRootPtr, char *key) {
  if ((TreeRootPtr != NULL) && ((*TreeRootPtr) != NULL)) {
    if (strcmp(key, (*TreeRootPtr)->key) < 0) {
      bst_delete(&((*TreeRootPtr)->LPtr), key);
    } else if (strcmp(key, (*TreeRootPtr)->key) > 0) {
      bst_delete(&((*TreeRootPtr)->RPtr), key);
    } else {
      bst_node_ptr_t tmp = *TreeRootPtr;
      if (((*TreeRootPtr)->LPtr == NULL) && ((*TreeRootPtr)->RPtr == NULL)) {
        free((*TreeRootPtr)->data);
        free(*TreeRootPtr);
        *TreeRootPtr = NULL;
      } else if ((*TreeRootPtr)->LPtr == NULL) {
        *TreeRootPtr = (*TreeRootPtr)->RPtr;
      } else if ((*TreeRootPtr)->RPtr == NULL) {
        *TreeRootPtr = (*TreeRootPtr)->LPtr;
      } else {
        bst_replace_by_rightmost((*TreeRootPtr)->LPtr, &tmp);
      }
    }
  }
}
/**
 * @brief Searches for node in tree
 *
 * @param TreeRootPtr
 * @param key
 * @return bst_node_ptr_t
 */
bst_node_ptr_t bst_search(bst_node_ptr_t TreeRootPtr, char *key) {
  if (TreeRootPtr == NULL) {
    return NULL;
  } else {
    if (strcmp(key, TreeRootPtr->key) < 0) {
      return bst_search(TreeRootPtr->LPtr, key);
    } else if (strcmp(key, TreeRootPtr->key) > 0) {
      return bst_search(TreeRootPtr->RPtr, key);
    } else {
      return TreeRootPtr;
    }
  }
}
/**
 * @brief Replaces node with rightmost node
 *
 * @param PtrReplaced
 * @param TreeRootPtr
 */
void bst_replace_by_rightmost(bst_node_ptr_t PtrReplaced,
                              bst_node_ptr_t *TreeRootPtr) {
  if ((*TreeRootPtr)->RPtr != NULL) {
    bst_replace_by_rightmost(PtrReplaced, &(*TreeRootPtr)->RPtr);
    return;
  }

  PtrReplaced->key = (*TreeRootPtr)->key;
  PtrReplaced->nodeDataType = (*TreeRootPtr)->nodeDataType;
  PtrReplaced->data = (*TreeRootPtr)->data;

  bst_node_ptr_t tmp = *TreeRootPtr;
  *TreeRootPtr = (*TreeRootPtr)->LPtr;
  free(tmp);
}
/**
 * @brief Creates symtable
 *
 * @param TableRoot
 */
void symtable_init(tSymtable *TableRoot) {
  bst_init(&TableRoot->root);
  symtable_insert_builtin_func(TableRoot);
}
/**
 * @brief Disposes symtable
 *
 * @param TableRoot
 */
void symtable_dispose(tSymtable *TableRoot) { bst_dispose(&TableRoot->root); }

void symtable_insert_func(tSymtable *TableRoot, string key) {
  tData *dataPtr;
  if ((dataPtr = malloc(sizeof(tData))) == NULL) {
    return;
  }
  if ((dataPtr->func = malloc(sizeof(tFunction))) == NULL) {
    return;
  }
  dataPtr->func->defined = false;
  dataPtr->func->paramCount = 0;
  dataPtr->func->returnType = voidType;

  bst_insert(&TableRoot->root, key.data, dataPtr, datatypeFunc);
}
/**
 * @brief Inserts builtin functions into symtable
 *
 * @param TableRoot
 */
void symtable_insert_builtin_func(tSymtable *TableRoot) {
  bst_node_ptr_t node;
  tFunction *func;

  /// reads///
  string *reads = SetupString();
  ConcatString(reads, "reads");
  symtable_insert_func(TableRoot, *reads);

  node = symtable_search(TableRoot, *reads);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 0;
  func->returnType = nullStringType;

  /// readi///
  string *readi = SetupString();
  ConcatString(readi, "readi");
  symtable_insert_func(TableRoot, *readi);

  node = symtable_search(TableRoot, *readi);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 0;
  func->returnType = nullIntType;

  /// readf///
  string *readf = SetupString();
  ConcatString(readf, "readf");
  symtable_insert_func(TableRoot, *readf);

  node = symtable_search(TableRoot, *readf);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 0;
  func->returnType = nullFloatType;

  /// write///
  string *write = SetupString();
  ConcatString(write, "write");
  symtable_insert_func(TableRoot, *write);

  node = symtable_search(TableRoot, *write);
  func = node->data->func;
  func->defined = true;
  func->paramCount = -1;
  func->returnType = voidType;

  /// floatval///
  string *floatval = SetupString();
  ConcatString(floatval, "floatval");
  symtable_insert_func(TableRoot, *floatval);

  node = symtable_search(TableRoot, *floatval);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 1;
  func->returnType = floatType;
  string *floatvalparam0 = &(func->paramNames[0]);
  floatvalparam0 = SetupString();
  ConcatString(floatvalparam0, "term");

  /// intval///
  string *intval = SetupString();
  ConcatString(intval, "intval");
  symtable_insert_func(TableRoot, *intval);

  node = symtable_search(TableRoot, *intval);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 1;
  func->returnType = intType;
  string *intvalparam0 = &(func->paramNames[0]);
  intvalparam0 = SetupString();
  ConcatString(intvalparam0, "term");

  /// strval///
  string *strval = SetupString();
  ConcatString(strval, "strval");
  symtable_insert_func(TableRoot, *strval);

  node = symtable_search(TableRoot, *strval);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 1;
  func->returnType = stringType;
  string *strvalparam0 = &(func->paramNames[0]);
  strvalparam0 = SetupString();
  ConcatString(strvalparam0, "term");

  /// strlen///
  string *strlen = SetupString();
  ConcatString(strlen, "strlen");
  symtable_insert_func(TableRoot, *strlen);

  node = symtable_search(TableRoot, *strlen);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 1;
  func->returnType = intType;
  string *strlenparam0 = &(func->paramNames[0]);
  strlenparam0 = SetupString();
  ConcatString(strlenparam0, "$s");

  /// substring///
  string *substring = SetupString();

  ConcatString(substring, "substring");
  symtable_insert_func(TableRoot, *substring);

  node = symtable_search(TableRoot, *substring);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 3;
  func->returnType = nullStringType;
  string *substrparam0 = &(func->paramNames[0]);
  string *substrparam1 = &(func->paramNames[1]);
  string *substrparam2 = &(func->paramNames[2]);
  substrparam0 = SetupString();
  substrparam1 = SetupString();
  substrparam2 = SetupString();
  ConcatString(substrparam0, "$s");
  ConcatString(substrparam1, "$i");
  ConcatString(substrparam2, "$j");

  /// ord///
  string *ord = SetupString();

  ConcatString(ord, "ord");
  symtable_insert_func(TableRoot, *ord);

  node = symtable_search(TableRoot, *ord);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 1;
  func->returnType = intType;
  string *ordparam0 = &(func->paramNames[0]);
  ordparam0 = SetupString();
  ConcatString(ordparam0, "$c");

  /// chr///
  string *chr = SetupString();

  ConcatString(chr, "chr");
  symtable_insert_func(TableRoot, *chr);

  node = symtable_search(TableRoot, *chr);
  func = node->data->func;
  func->defined = true;
  func->paramCount = 1;
  func->returnType = stringType;
  string *chrparam0 = &(func->paramNames[0]);
  chrparam0 = SetupString();
  ConcatString(chrparam0, "$i");
}
/**
 * @brief Inserts variable into symtable
 *
 * @param TableRoot
 * @param key
 */
void symtable_insert_var(tSymtable *TableRoot, string key) {
  tData *data;
  if ((data = (tData *)malloc(sizeof(tData))) == NULL) {
    return;
  }
  if ((data->var = (tVariable *)malloc(sizeof(tVariable))) == NULL) {
    return;
  }
  data->var->dataType = voidType;
  bst_insert(&(TableRoot->root), key.data, data, datatypeVar);
}
/**
 * @brief Searches in symtable
 *
 * @param TableRoot
 * @param key
 * @return bst_node_ptr_t
 */
bst_node_ptr_t symtable_search(tSymtable *TableRoot, string key) {
  return bst_search(TableRoot->root, key.data);
}
/**
 * @brief Deletes from symtable
 *
 * @param TableRoot
 * @param key
 */
void symtable_delete(tSymtable *TableRoot, string key) {
  bst_delete(&TableRoot->root, key.data);
}
