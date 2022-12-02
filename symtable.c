#include "symtable.h"

void bst_init(bst_node_ptr_t *TreeRootPtr) { *TreeRootPtr = NULL; }

void bst_dispose(bst_node_ptr_t *TreeRootPtr) {
  if (*TreeRootPtr != NULL) {
    bst_dispose(&(*TreeRootPtr)->LPtr);
    bst_dispose(&(*TreeRootPtr)->RPtr);
    free((*TreeRootPtr)->key);
    (*TreeRootPtr)->key = NULL;

    if ((*TreeRootPtr)->nodeDataType == datatypeFunc) {
      ResetString(&(*TreeRootPtr)->data->func->params);
    }

    free(&(*TreeRootPtr)->data->func);
    (*TreeRootPtr)->data->func = NULL;

    free(*TreeRootPtr);
    *TreeRootPtr = NULL;
  }
}

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

void symtable_init(tSymtable *TableRoot) { bst_init(&TableRoot->root); }

void symtable_dispose(tSymtable *TableRoot) { bst_dispose(&TableRoot->root); }

void symtable_insert_func(tSymtable *TableRoot, string key) {
  tData *dataPtr;
  if ((dataPtr = malloc(sizeof(tData))) == NULL) {
    return;
  }
  string params;
  SetupString(&params);
  dataPtr->func->params = params;
  dataPtr->func->defined = false;
  dataPtr->func->declared = false;
  dataPtr->func->returnType = -1;

  bst_insert(&TableRoot->root, key.data, dataPtr, datatypeFunc);
}

// void symtable_insert_builtin_func (tSymtable *);

void symtable_insert_var(tSymtable *TableRoot, string key) {
  tData *var;
  if ((var = malloc(sizeof(tData))) == NULL) {
    return;
  }
  var->var->dataType = -1;
  bst_insert(&(TableRoot->root), key.data, var, datatypeVar);
}

bst_node_ptr_t symtable_search(tSymtable *TableRoot, string key) {
  return bst_search(TableRoot->root, key.data);
}

void symtable_delete(tSymtable *TableRoot, string key) {
  bst_delete(&TableRoot->root, key.data);
}
