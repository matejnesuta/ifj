#include "ASTreeGraphGenerator.h"

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
  char *same_lvl = "{rank = same; ";
  while (child != NULL) {
    if (child->tree->node->is_terminal) {  // if terminal
      if (tree->node->is_terminal)         // if parent is terminal
      {
        fprintf(f, "\"%s\" -> \"%s\";\n",
                strtok(tree->node->terminal->code->data, "\n"),
                strtok(child->tree->node->terminal->code->data, "\n"));
      } else  // else parent is nonterminal
      {
        fprintf(f, "\"%s\" -> \"%s\";\n",
                GetNonterminalName(tree->node->nonterminal),
                strtok(child->tree->node->terminal->code->data, "\n"));
      }
      if (child->next != NULL) {
        fprintf(f, "\"%s\" -> \"%s\";\n",
                strtok(child->tree->node->terminal->code->data, "\n"),
                strtok(child->next->tree->node->terminal->code->data, "\n"));
      }

      same_lvl = add_to_lvl(same_lvl, "\"");
      same_lvl = add_to_lvl(
          same_lvl, strtok(child->tree->node->terminal->code->data, "\n"));
      same_lvl = add_to_lvl(same_lvl, "\"");
      same_lvl = add_to_lvl(same_lvl, "; ");

      // if (child->tree->node->terminal->kind != endOfFileTer) {
      //   printf("term: %s\n", child->tree->node->terminal->code->data);
      // }
    } else {
      fprintf(f, "\"%s\" -> \"%s\";\n",
              GetNonterminalName(tree->node->nonterminal),
              GetNonterminalName(child->tree->node->nonterminal));
      same_lvl = add_to_lvl(same_lvl, "\"");
      same_lvl = add_to_lvl(same_lvl,
                            GetNonterminalName(child->tree->node->nonterminal));
      same_lvl = add_to_lvl(same_lvl, "\"");
      same_lvl = add_to_lvl(same_lvl, "; ");
      ASTreePrintChildrenRec(child->tree, f);
    }
    child = child->next;
  }
  same_lvl = add_to_lvl(same_lvl, "}\n");
  fprintf(f, "%s", same_lvl);
  printf("samelvl: %s\n", same_lvl);
}

char *add_to_lvl(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1 +
                        4);  // +1 for the null-terminator +4 for ""
  if (result == NULL) {
    exit(99);
  }
  strcpy(result, s1);
  strcat(result, s2);

  return result;
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