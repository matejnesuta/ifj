#include "codegen.h"

#include "LList.h"
#include "logger.h"
// #include "symtable.c"
#include "symtable.h"

#define GF "GF@"
#define TF "TF@"
#define LF "LF@"

void generateOperation(AST *tree, tSymtable *global, char *current_frame,
                       char *var) {
  if (tree->children->first->next == NULL) {
    printf("MOVE %s%s ", current_frame, var);
    if (tree->children->first->tree->node->terminal->kind == variableTer) {
      printf("TODO: needs an implementation");
    } else {
      printf("%s\n", tree->children->first->tree->node->terminal->code->data);
    }
    // printf("MOVE %s%s ")
  }
  // else {
  //         generateExp(tree->children->first->next->next->tree, global,
  //         current_frame, var); terminal *operation =
  //         tree->children->first->next; switch(operation->kind){
  //           case (plusTer){
  //             printf("ADD %s%s ")
  //           }
  //         }
  //       }
  //       // terminal *operation =
  //       // t
}

void generateExp(AST *tree, tSymtable *global, char *current_frame, char *var) {
  if (tree->children->first->tree->node->is_terminal == false) {
    generateExp(tree->children->first->tree, global, current_frame, var);
    // } else if (tree->children->first->tree->node->terminal->kind ==
    //                leftBracketTer ||
    //            tree->children->first->tree->node->terminal->kind ==
    //                rightBracketTer) {
    //   generateExp(tree->children->first->tree, global, current_frame, var);
  } else {
    generateOperation(tree, global, current_frame, var);
  }
  printf("%d\n", tree->node->nonterminal);
  printf("%d\n", tree->children->first->tree->node->nonterminal);
  printf("%d\n", tree->children->first->tree->node->terminal->kind);
}

void ASTreeRecGoThru(AST *tree, tSymtable *global, char *current_frame) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;  // if tree has no children (should be error)
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      if (child->tree->node->terminal->kind != 14) {
        terminal *current_terminal = child->tree->node->terminal;

        switch (current_terminal->kind) {
          case 22:
            // logger("codegen lmao",
            //        child->next->tree->node->terminal->code->data);
            if (child->next->tree->node->terminal->kind = assignTer) {
              if (!symtable_search(global, *(current_terminal)->code)) {
                symtable_insert_var(global, *(current_terminal)->code);
                printf("DEFVAR %s%s\n", current_frame,
                       current_terminal->code->data);
              }
              child = child->next->next;
              generateExp(
                  child->tree->children->first->tree->children->first->tree,
                  global, current_frame, current_terminal->code->data);
            }
        }

        // logger("codegen", child->tree->node->terminal->code->data);
        // printf("%d\n", child->tree->node->terminal->kind);
        // do something with terminal
      }
    } else {
      // logger("codegen", "ahoj");
      // do something with nonterminal
      ASTreeRecGoThru(child->tree, global,
                      current_frame);  // get one level deeper thru nonterminal
    }
    child = child->next;
  }
}

void codegen(AST *tree) {
  tSymtable global;
  symtable_init(&global);
  char *current_frame = GF;
  ASTreeRecGoThru(tree, &global, current_frame);
  return;
}