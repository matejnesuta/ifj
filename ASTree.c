#include "ASTree.h"

#include "LList.h"
#include "include.h"
#include "logger.h"
#include "symbol.h"

AST *ASTreeInit() {
  AST *tree = (AST *)malloc(sizeof(struct ASTree));
  if (tree == NULL) {
    exit(99);
  };
  logger("ASTreeInit", "ASTree initialized");
  tree->children = NULL;
  logger("ASTreeInit", "children initialized");
  tree->has_children = false;
  logger("ASTreeInit", "has_children initialized");
  return tree;
}

AST *ASTreeCreateNode(symbol *node) {
  logger("ASTreeCreateNode", "ASTreeCreateNode started");
  AST *tree = ASTreeInit();
  logger("ASTreeCreateNode", "Creating node");
  tree->node = node;
  logger("ASTreeCreateNode", "Node created");
  return tree;
}

AST *ASTreeInsertFirstChild(AST *tree, AST *child) {
  logger("ASTreeInsertFirstChild", "Inserting first child");
  LList *list = LListInit();
  logger("ASTreeInsertFirstChild", "LList initialized");
  list = LListInsertFirstChild(list, child);
  logger("ASTreeInsertFirstChild", "Child inserted");
  tree->children = list;
  logger("ASTreeInsertFirstChild", "Children set");
  tree->has_children = true;
  logger("ASTreeInsertFirstChild", "has_children set");
  return tree;
}

AST *ASTreeInsertAnotherChild(AST *tree, AST *child) {
  tree->children = LListInsertAnotherChild(tree->children, child);
  logger("ASTreeInsertAnotherChild", "Child inserted");
  return tree;
}

const char *GetNonterminalName(nonterminal_kind nonterminal);
void ASTreePrintChildrenRec(AST *tree, FILE *f);
void ASTreePrintChildren(AST *tree) {
  FILE *f = fopen("ASTree.dot", "w");
  fprintf(f, "digraph G {node [shape = circle; width = 2;];\n");
  ASTreePrintChildrenRec(tree, f);
  fprintf(f, "}");
  fflush(f);
  fclose(f);
}
void ASTreePrintChildrenRec(AST *tree, FILE *f) {
  if (!tree->children) {
    return;
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      if (!tree->node->is_terminal)
        fprintf(f, "\"%s\" -> \"%s\";\n",
                GetNonterminalName(tree->node->nonterminal),
                child->tree->node->terminal->code->data);
      else
        fprintf(f, "\"%s\" -> \"%s\";\n", tree->node->terminal->code->data,
                child->tree->node->terminal->code->data);
      if (child->next != NULL) {
        fprintf(f, "\"%s\" -> \"%s\";\n",
                child->tree->node->terminal->code->data,
                child->next->tree->node->terminal->code->data);
      }

      if (child->tree->node->terminal->kind != endOfFileTer) {
        // printf("term: %s\n", child->tree->node->terminal->code->data);
      }
    } else {
      fprintf(f, "\"%s\" -> ", GetNonterminalName(tree->node->nonterminal));
      fprintf(f, "\"%s\";\n",
              GetNonterminalName(child->tree->node->nonterminal));
      ASTreePrintChildrenRec(child->tree, f);
    }
    child = child->next;
  }
}

const char *GetNonterminalName(nonterminal_kind nonterminal) {
  char *name;
  switch (nonterminal) {
    case START:
      name = "START";
      return name;
    case PROG:
      name = "PROG";
      return name;
    case START_PROLOG:
      name = "START_PROLOG";
      return name;
    case END_PROLOG:
      name = "END_PROLOG";
      return name;
    case CODE:
      name = "CODE";
      return name;
    case BODY:
      name = "BODY";
      return name;
    case INNER_SCOPE:
      name = "INNER_SCOPE";
      return name;
    case RETURN_VALUE:
      name = "RETURN_VALUE";
      return name;
    case RETURN_TYPE:
      name = "RETURN_TYPE";
      return name;
    case FUNC_CALL:
      name = "FUNC_CALL";
      return name;
    case FUNC_CALL_ARGS:
      name = "FUNC_CALL_ARGS";
      return name;
    case NEXT_ARG:
      name = "NEXT_ARG";
      return name;
    case ARG:
      name = "ARG";
      return name;
    case LITERAL:
      name = "LITERAL";
      return name;
    case FUNC_DECLARE:
      name = "FUNC_DECLARE";
      return name;
    case FUNC_DECLARE_BODY:
      name = "FUNC_DECLARE_BODY";
      return name;
    case ARG_TYPE:
      name = "ARG_TYPE";
      return name;
    case IF_ELSE:
      name = "IF_ELSE";
      return name;
  }
  name = "UNKNOWN";
  return name;
}