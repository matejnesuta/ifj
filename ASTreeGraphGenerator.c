#include "ASTreeGraphGenerator.h"

#include "error.h"

void ASTreePrintChildren(AST *tree) {
  FILE *f = fopen("ASTree.dot", "w");
  fprintf(f, "digraph G {node [shape = circle; width = 2;];\n");
  ASTreePrintChildrenRec(tree, f);
  fprintf(f, "}");
  fflush(f);
  fclose(f);
}
void ASTreePrintChildrenRec(AST *tree, FILE *f) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;
  }
  char *same_lvl = "{rank = same; ";

  LList_element *child = tree->children->first;
  while (child != NULL) {
    char *otec = tree->node->is_terminal ? formatTerminal(tree)
                                         : formatNonterminal(tree);
    if (child->tree->node->is_terminal) {
      // format child1 terminal
      char *dite1 = formatTerminal(child->tree);
      if (child == tree->children->first) {
        fprintf(f, "%s -> %s ;\n", otec, dite1);
      }

      // format child2
      char *dite2;
      bool skip2 = false;
      if (child->next == NULL) {
        dite2 = formatNull();
        skip2 = true;
      } else {
        dite2 = child->next->tree->node->is_terminal
                    ? formatTerminal(child->next->tree)
                    : formatNonterminal(child->next->tree);
      }
      same_lvl = add_to_lvl(same_lvl, dite1);
      // fprintf(f, "%s -> %s ;\n", otec, dite1);
      if (!skip2) {
        fprintf(f, "%s -> %s ;\n", dite1, dite2);
      }
    } else {
      // format child1 terminal
      char *dite1 = formatNonterminal(child->tree);
      if (child == tree->children->first) {
        fprintf(f, "%s -> %s ;\n", otec, dite1);
      }
      // format child2
      char *dite2;
      bool skip2 = false;
      if (child->next == NULL) {
        dite2 = formatNull();
        skip2 = true;
      } else {
        dite2 = child->next->tree->node->is_terminal
                    ? formatTerminal(child->next->tree)
                    : formatNonterminal(child->next->tree);
      }

      same_lvl = add_to_lvl(same_lvl, dite1);
      // fprintf(f, "%s -> %s ;\n", otec, dite1);
      if (!skip2) {
        fprintf(f, "%s -> %s ;\n", dite1, dite2);
      }
    }
    ASTreePrintChildrenRec(child->tree, f);
    child = child->next;
  }
  same_lvl = add_to_lvl(same_lvl, "}\n");
  fprintf(f, "%s", same_lvl);
}

char *formatNull() {
  char *new_str = (char *)malloc(sizeof(char) * (strlen("NULL") + 100));
  if (new_str == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  strcpy(new_str, "\"");
  strcat(new_str, "NULL");
  char *c = (char *)malloc(sizeof(char) * 100);
  if (c == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  sprintf(c, "  %d", rand() * 10);
  strcat(new_str, c);
  strcat(new_str, "\"");
  return new_str;
}

char *formatTerminal(AST *tree) {
  char *new_str = (char *)malloc(
      sizeof(char) * (strlen(tree->node->terminal->code->data) + 100));
  if (new_str == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  strcpy(new_str, "\"");
  strcat(new_str, GetTerminalName(tree->node->terminal));
  char *c = (char *)malloc(sizeof(char) * 100);
  if (c == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  sprintf(c, "  %ld", (long)tree->node);
  strcat(new_str, c);
  strcat(new_str, "\"");
  return new_str;
}

char *formatNonterminal(AST *tree) {
  char *new_str = (char *)malloc(
      sizeof(char) *
      (strlen(GetNonterminalName(tree->node->nonterminal)) + 100));
  if (new_str == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  strcpy(new_str, "\"");
  strcat(new_str, strtok(GetNonterminalName(tree->node->nonterminal), "\n"));
  char *c = (char *)malloc(sizeof(char) * 100);
  if (c == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  sprintf(c, "  %ld", (long)tree->node);
  strcat(new_str, c);
  strcat(new_str, "\"");
  return new_str;
}

char *add_to_lvl(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1 + 4 +
                        100);  // +1 for the null-terminator +4 for ""
  if (result == NULL) {
    ErrorExit(99, "Malloc failed!");
  }
  strcpy(result, s1);
  strcat(result, s2);

  return result;
}

char *GetTerminalName(terminal *ter) {
  char *type;
  switch (ter->kind) {
    case ifTer:
      type = "ifTer";
      break;
    case elseTer:
      type = "elseTer";
      break;
    case whileTer:
      type = "whileTer";
      break;
    case functionTer:
      type = "functionTer";
      break;
    case function_idTer:
      type = "function_idTer";
      break;
    case intTypeTer:
      type = "intTypeTer";
      break;
    case floatTypeTer:
      type = "floatTypeTer";
      break;
    case stringTypeTer:
      type = "stringTypeTer";
      break;
    case voidTypeTer:
      type = "voidTypeTer";
      break;
    case nullTer:
      type = "nullTer";
      break;
    case returnTer:
      type = "returnTer";
      break;
    case assignTer:
      type = "assignTer";
      break;
    case startPrologTer:
      type = "startPrologTer";
      break;
    case endPrologTer:
      type = "endPrologTer";
      break;
    case endOfFileTer:
      type = "endOfFileTer";
      break;
    case semicolonTer:
      type = "semicolonTer";
      break;
    case leftBracketTer:
      type = "leftBracketTer";
      break;
    case rightBracketTer:
      type = "rightBracketTer";
      break;
    case leftCurlyBracketTer:
      type = "leftCurlyBracketTer";
      break;
    case rightCurlyBracketTer:
      type = "rightCurlyBracketTer";
      break;
    case colonTer:
      type = "colonTer";
      break;
    case commaTer:
      type = "commaTer";
      break;
    case variableTer:
      type = "variableTer";
      break;
    case string_litTer:
      type = "string_litTer";
      break;
    case int_litTer:
      type = "int_litTer";
      break;
    case float_litTer:
      type = "float_litTer";
      break;
    case epsTer:
      type = "epsTer";
      break;
    case dotTer:
      type = "dotTer";
      break;
    case plusTer:
      type = "plusTer";
      break;
    case minusTer:
      type = "minusTer";
      break;
    case multiplyTer:
      type = "multiplyTer";
      break;
    case divideTer:
      type = "divideTer";
      break;
    case lessTer:
      type = "lessTer";
      break;
    case lessOrEqualTer:
      type = "lessOrEqualTer";
      break;
    case greaterTer:
      type = "greaterTer";
      break;
    case greaterOrEqualTer:
      type = "greaterOrEqualTer";
      break;
    case equalTer:
      type = "equalTer";
      break;
    case notEqualTer:
      type = "notEqualTer";
      break;
  }

  char *ret = (char *)malloc(sizeof(char) *
                             (strlen(type) + strlen(ter->code->data) + 4 + 1));

  ret = strcpy(ret, type);
  ret = strcat(ret, "( ");
  ret = strcat(ret, strtok(ter->code->data, "\n"));
  ret = strcat(ret, " )");
  return ret;
}

char *GetNonterminalName(nonterminal_kind nonterminal) {
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
    case RIGHT_SIDE:
      name = "RIGHT_SIDE";
      return name;
    case EXP:
      name = "EXP";
      return name;
    case E:
      name = "E";
      return name;
  }
  name = "UNKNOWN";
  return name;
}