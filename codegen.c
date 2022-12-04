#include "codegen.h"

#include "LList.h"
#include "error.h"
#include "logger.h"

// #include "symtable.c"
#include "symtable.h"

#define GF "GF@"
#define TF "TF@"
#define LF "LF@"

void variableDefined(tSymtable *symtable, terminal *term) {
  if (!symtable_search(symtable, *(term)->code)) {
    ErrorExit(5, "Variable was not defined!");
  }
}

void framePush() {
  printf("CREATEFRAME\n");
  printf("DEFVAR TF@returnval\n");
  printf("PUSHFRAME\n");
}

void framePop(terminal *current_terminal, char *current_frame, char *ret) {
  printf("MOVE LF@returnval %s\n", ret);
  printf("POPFRAME\n");
  if (current_terminal != NULL) {
    printf("MOVE %s%s TF@returnval\n\n", current_frame,
           current_terminal->code->data);
  }
}

// does the operation // TODO finish other operators && check for errors (Nil)
char *Operation(terminal_kind op, char *temp, char *left, char *right) {
  switch (op) {
    case plusTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?ADD_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case minusTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?SUB_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case multiplyTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?MUL_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case divideTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?DIV_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case dotTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?CONCAT_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case equalTer:
    case notEqualTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?EQ_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      if (op == notEqualTer) {
        printf("NOT %s %s\n", temp, temp);
      }
      return temp;

    case lessTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?LT_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case greaterTer:
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?GT_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case lessOrEqualTer:;
    case greaterOrEqualTer:;
      char x = '\0';
      char *temp2 = malloc(sizeof(char) * sizeof(strlen(temp) + 1));
      strcpy(temp2, temp);
      if (temp2 == NULL) {
        ErrorExit(99, "Malloc failed!");
      }
      temp2[3] = '*';
      x = (op == lessOrEqualTer) ? 'L' : 'G';
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?%cT_op\n"
          "MOVE %s TF@result\n",
          left, right, x, temp);
      printf("DEFVAR %s\n", temp2);
      printf(
          "CREATEFRAME\n"
          "DEFVAR TF@left\n"
          "DEFVAR TF@right\n"
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?EQ_op\n"
          "MOVE %s TF@result\n",
          left, right, temp2);
      printf("OR %s %s %s\n", temp, temp, temp2);
      free(temp2);
      return temp;

    default:
      ErrorExit(7, "Wrong type of operands!");
  }
}

char *generateExp(AST *tree, tSymtable *symtable, char *current_frame) {
  AST *term = tree;
  while (term->node->is_terminal == false) {  // finds first terminal
    term = term->children->first->tree;
  }
  if (term->node->terminal->kind == plusTer ||  // the terminal is an operator
      term->node->terminal->kind == minusTer ||
      term->node->terminal->kind == multiplyTer ||
      term->node->terminal->kind == divideTer ||
      term->node->terminal->kind == dotTer ||
      term->node->terminal->kind == lessTer ||
      term->node->terminal->kind == lessOrEqualTer ||
      term->node->terminal->kind == greaterTer ||
      term->node->terminal->kind == greaterOrEqualTer ||
      term->node->terminal->kind == equalTer ||
      term->node->terminal->kind == notEqualTer) {
    char *left_var =
        generateExp(term->children->first->tree, symtable,
                    current_frame);  // gets left side of the operator
    char *right_var =
        generateExp(term->children->first->next->tree, symtable,
                    current_frame);  // gets right side of the operator

    // prepares temp var used in the expression
    char *temp =
        malloc(sizeof(current_frame) + sizeof(char) * sizeof(long) + 1);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    sprintf(temp, "%s_%ld", current_frame, (long)term->node);
    printf("DEFVAR %s\n", temp);

    // does the operation // TODO finish other operators && check for errors
    return Operation(term->node->terminal->kind, temp, left_var, right_var);

  } else if (term->node->terminal->kind == variableTer) {  // handle variable
    if (!(symtable_search(symtable, *(term->node->terminal->code)))) {
      ErrorExit(5, "Variable not defined!");
    }
    char *temp =
        malloc(sizeof(current_frame) + sizeof(char) * sizeof(long) + 1);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    sprintf(temp, "%s_%ld", current_frame, (long)term->node);
    printf("DEFVAR %s\n", temp);
    printf("MOVE %s %s%s\n", temp, current_frame,
           term->node->terminal->code->data);
    return temp;
  } else {  // handle constant
    char *temp =
        malloc(sizeof(current_frame) + sizeof(char) * sizeof(long) + 1);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    sprintf(temp, "%s_%ld", current_frame, (long)term->node);
    printf("DEFVAR %s\n", temp);
    switch (term->node->terminal->kind) {
      case int_litTer:;
        char *intLit =
            malloc(4 * sizeof(char) +
                   sizeof(char) * sizeof(term->node->terminal->code->size) + 1);
        if (intLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        sprintf(intLit, "int@%s", term->node->terminal->code->data);
        printf("MOVE %s %s\n", temp, intLit);
        return temp;
      case float_litTer:;
        char *floatLit =
            malloc(5 * sizeof(char) +
                   sizeof(char) * sizeof(term->node->terminal->code->size) + 1);
        if (floatLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        sprintf(floatLit, "float@%a",
                strtof(term->node->terminal->code->data, NULL));
        printf("MOVE %s %s\n", temp, floatLit);
        return temp;
      case string_litTer:;
        size_t size =
            snprintf(NULL, 0, "string@%s", term->node->terminal->code->data) +
            1;
        char *stringLit = malloc(size);
        snprintf(stringLit, size, "string@%s",
                 term->node->terminal->code->data);
        printf("MOVE %s %s\n", temp, stringLit);
        return temp;
      case nullTer:
        printf("MOVE %s nil@nil\n", temp);
        return temp;
      default:
        break;
    }
  }
}

void lookForVarsInAScope(AST *tree, tSymtable *symtable, char *current_frame,
                         string *var) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;  // if tree has no children (should be error)
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      if (child->tree->node->terminal->kind == variableTer) {
        var = child->tree->node->terminal->code;
      } else if (child->tree->node->terminal->kind == assignTer) {
        if (!symtable_search(symtable, *var)) {
          symtable_insert_var(symtable, *var);
          printf("DEFVAR %s%s\n", current_frame, var->data);
        }
      }
    } else {
      lookForVarsInAScope(child->tree, symtable, current_frame, var);
    }
    child = child->next;
  }
}

void SolveVariableAssignment(LList_element *child, tSymtable *symtable,
                             char *current_frame) {
  framePush();
  terminal *current_terminal = child->tree->node->terminal;

  child = child->next->next;
  char *ret =
      generateExp(child->tree->children->first->tree, symtable, current_frame);
  if (!symtable_search(symtable, *(current_terminal)->code)) {
    symtable_insert_var(symtable, *(current_terminal)->code);
    printf("DEFVAR %s%s\n", current_frame, current_terminal->code->data);
  }
  framePop(current_terminal, current_frame, ret);
}

void GenerateWhileInMain(LList_element *termWhile, tSymtable *global,
                         char *current_frame) {
  terminal *current_terminal = termWhile->tree->node->terminal;
  // get vars from inside
  printf("DEFVAR %s?%ldexp\n", current_frame, current_terminal->code);
  LList_element *inner_child = termWhile->next->next;
  LList_element *backup = inner_child;
  lookForVarsInAScope(inner_child->next->next->next->tree, global,
                      current_frame, NULL);
  inner_child = backup;
  // jump loop condition
  printf("\nJUMP ?%ldstart\n", current_terminal->code);
  // label loop
  printf("LABEL ?%ldloop\n", current_terminal->code);
  // body
  GoThruMain(inner_child->next->next->next->tree, global, current_frame);
  printf("JUMP ?%ldcondition\n", current_terminal->code);
  // label loop comp
  printf("\nLABEL ?%ldstart\n\n", current_terminal->code);
  printf("LABEL ?%ldcondition\n", current_terminal->code);
  // comp expr
  framePush();
  char *ret = generateExp(inner_child->tree, global, current_frame);
  framePop(NULL, current_frame, ret);
  // printf("MOVE %s?%ldexp %s\n", current_frame, current_terminal->code, ret);
  // jumpifeq loop
  printf("JUMPIFEQ ?%ldloop TF@returnval bool@true\n\n",
         current_terminal->code);
}

void GenerateIfElseInMain(LList_element *IF, tSymtable *global,
                          char *current_frame) {
  terminal *current_terminal = IF->tree->node->terminal;
  LList_element *inner_child = IF->next->next;
  framePush();
  char *ret = generateExp(inner_child->tree, global, current_frame);
  printf("DEFVAR %s?%ldexp\n", current_frame, current_terminal->code);
  framePop(NULL, current_frame, ret);
  LList_element *backup = inner_child;

  lookForVarsInAScope(inner_child->next->next->next->tree, global,
                      current_frame, NULL);
  inner_child = backup;
  lookForVarsInAScope(
      inner_child->next->next->next->next->next->next->next->tree, global,
      current_frame, NULL);
  inner_child = backup;
  printf("JUMPIFNEQ else_%ld TF@returnval bool@true\n\n",
         current_terminal->code);
  inner_child = backup;
  inner_child = inner_child->next->next->next;
  GoThruMain(inner_child->tree, global, current_frame);
  printf("JUMP end_%ld\n", current_terminal->code);
  printf("\nLABEL else_%ld\n", current_terminal->code);
  inner_child = inner_child->next->next->next->next;
  GoThruMain(inner_child->tree, global, current_frame);
  printf("\nLABEL end_%ld\n", current_terminal->code);
}

void SolveEmptyExpression(LList_element *child, tSymtable *symtable,
                          char *current_frame) {
  framePush();
  char *ret = generateExp(child->tree, symtable, current_frame);
  framePop(NULL, current_frame, ret);
}

void GoThruMain(AST *tree, tSymtable *global, char *current_frame) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      if (child->tree->node->terminal->kind != endOfFileTer) {
        terminal *current_terminal = child->tree->node->terminal;
        switch (current_terminal->kind) {
          case functionTer:;
            terminal *func_id = child->next->tree->node->terminal;
            bst_node_ptr_t NewFunc = symtable_search(global, *(func_id->code));
            if (NewFunc != NULL) {
              ErrorExit(3, "Function already defined!");
            }
            symtable_insert_func(global, *(func_id->code));
            break;

          default:
            break;
        }
      }
    } else {
      if (child->tree->node->nonterminal == INNER_SCOPE) {
        LList_element *inner_child = child->tree->children->first;
        if (inner_child->tree->node->is_terminal) {
          terminal *LL_ter = inner_child->tree->node->terminal;
          switch (LL_ter->kind) {
            case variableTer:
              // solves variable assignment by expression
              if (inner_child->next->next->tree->node->is_terminal == false &&
                  inner_child->next->next->tree->children->first->tree->node
                          ->nonterminal == EXP) {
                SolveVariableAssignment(inner_child, global, current_frame);
              }
              break;

            case returnTer:
              // TODO
              break;

            case whileTer:
              GenerateWhileInMain(inner_child, global, LF);
              break;

            case leftCurlyBracketTer:
              // TODO
              break;

            default:
              break;
          }
        } else if (inner_child->tree->node->nonterminal == EXP) {
          SolveEmptyExpression(inner_child, global, current_frame);
        } else if (inner_child->tree->node->nonterminal == IF_ELSE) {
          GenerateIfElseInMain(inner_child->tree->children->first, global,
                               current_frame);

        } else if (inner_child->tree->node->nonterminal == FUNC_CALL) {
          // TODO
        }
      } else if (child->tree->node->nonterminal == FUNC_DECLARE) {
        {
          LList_element *func_id = child->tree->children->first->next;
          bst_node_ptr_t NewFunc =
              symtable_search(global, *(func_id->tree->node->terminal->code));
          if (NewFunc != NULL) {
            ErrorExit(3, "Function already defined!");
          }
          symtable_insert_func(global, *(func_id->tree->node->terminal->code));

          // TODO create function
        }
      }
      // here insert other nonterminals
      else {
        if (child->tree->node->nonterminal != START_PROLOG) {
          GoThruMain(child->tree, global,
                     current_frame);  // get one level deeper thru nonterminal
        }
      }
    }
    child = child->next;
  }
}

void InsertAllOpBuiltInFuncs() {
  printf("\n");
  printf("JUMP ?ADD_jump_over\n");
  printf("LABEL ?ADD_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?ADD_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?ADD_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?ADD_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?ADD_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?ADD_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?ADD_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?ADD_int\n");
  printf("        JUMPIFEQ ?ADD_int_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?ADD_int_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?ADD_int_nil LF@typeRight string@nil\n");
  printf("        JUMP ?ADD_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?ADD_int_int\n");
  printf("            ADD LF@result LF@left LF@right\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("        LABEL ?ADD_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            ADD LF@result LF@left LF@right\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("        LABEL ?ADD_int_nil\n");
  printf("            ADD LF@result LF@left int@0\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("    LABEL ?ADD_float\n");
  printf("        JUMPIFEQ ?ADD_float_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?ADD_float_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?ADD_float_nil LF@typeRight string@nil\n");
  printf("        JUMP ?ADD_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?ADD_float_int\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            ADD LF@result LF@left LF@right\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("        LABEL ?ADD_float_float\n");
  printf("            ADD LF@result LF@left LF@right\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("        LABEL ?ADD_float_nil\n");
  printf("            ADD LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("    LABEL ?ADD_nil\n");
  printf("        JUMPIFEQ ?ADD_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?ADD_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?ADD_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?ADD_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?ADD_nil_int\n");
  printf("            ADD LF@result int@0 LF@right\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("        LABEL ?ADD_nil_float\n");
  printf("            ADD LF@result float@0x0p+0 LF@right\n");
  printf("            JUMP ?ADD_end\n");
  printf("\n");
  printf("        LABEL ?ADD_nil_nil\n");
  printf("            ADD LF@result int@0 int@0\n");
  printf("            JUMP ?ADD_end\n");
  printf("    \n");
  printf("\n");
  printf("    LABEL ?ADD_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("    \n");
  printf("    LABEL ?ADD_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?ADD_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?ADD_jump_over\n");
  printf("\n");
  printf("\n");
  printf("JUMP ?SUB_jump_over\n");
  printf("label ?SUB_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?SUB_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?SUB_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?SUB_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?SUB_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?SUB_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?SUB_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?SUB_int\n");
  printf("        JUMPIFEQ ?SUB_int_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?SUB_int_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?SUB_int_nil LF@typeRight string@nil\n");
  printf("        JUMP ?SUB_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?SUB_int_int\n");
  printf("            SUB LF@result LF@left LF@right\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("        LABEL ?SUB_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            SUB LF@result LF@left LF@right\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("        LABEL ?SUB_int_nil\n");
  printf("            SUB LF@result LF@left int@0\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("    LABEL ?SUB_float\n");
  printf("        JUMPIFEQ ?SUB_float_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?SUB_float_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?SUB_float_nil LF@typeRight string@nil\n");
  printf("        JUMP ?SUB_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?SUB_float_int\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            SUB LF@result LF@left LF@right\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("        LABEL ?SUB_float_float\n");
  printf("            SUB LF@result LF@left LF@right\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("        LABEL ?SUB_float_nil\n");
  printf("            SUB LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("    LABEL ?SUB_nil\n");
  printf("        JUMPIFEQ ?SUB_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?SUB_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?SUB_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?SUB_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?SUB_nil_int\n");
  printf("            SUB LF@result int@0 LF@right\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("        LABEL ?SUB_nil_float\n");
  printf("            SUB LF@result float@0x0p+0 LF@right\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("        LABEL ?SUB_nil_nil\n");
  printf("            SUB LF@result int@0 int@0\n");
  printf("            JUMP ?SUB_end\n");
  printf("\n");
  printf("    LABEL ?SUB_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL ?SUB_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?SUB_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?SUB_jump_over\n");
  printf("\n");
  printf("JUMP ?MUL_jump_over\n");
  printf("label ?MUL_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?MUL_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?MUL_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?MUL_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?MUL_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?MUL_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?MUL_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?MUL_int\n");
  printf("        JUMPIFEQ ?MUL_int_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?MUL_int_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?MUL_int_nil LF@typeRight string@nil\n");
  printf("        JUMP ?MUL_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?MUL_int_int\n");
  printf("            MUL LF@result LF@left LF@right\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("        LABEL ?MUL_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            MUL LF@result LF@left LF@right\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("        LABEL ?MUL_int_nil\n");
  printf("            MUL LF@result LF@left int@0\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("    LABEL ?MUL_float\n");
  printf("        JUMPIFEQ ?MUL_float_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?MUL_float_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?MUL_float_nil LF@typeRight string@nil\n");
  printf("        JUMP ?MUL_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?MUL_float_int\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            MUL LF@result LF@left LF@right\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("        LABEL ?MUL_float_float\n");
  printf("            MUL LF@result LF@left LF@right\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("        LABEL ?MUL_float_nil\n");
  printf("            MUL LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("    LABEL ?MUL_nil\n");
  printf("        JUMPIFEQ ?MUL_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?MUL_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?MUL_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?MUL_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?MUL_nil_int\n");
  printf("            MUL LF@result int@0 LF@right\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("        LABEL ?MUL_nil_float\n");
  printf("            MUL LF@result float@0x0p+0 LF@right\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("        LABEL ?MUL_nil_nil\n");
  printf("            MUL LF@result int@0 int@0\n");
  printf("            JUMP ?MUL_end\n");
  printf("\n");
  printf("    LABEL ?MUL_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL ?MUL_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?MUL_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?MUL_jump_over\n");
  printf("\n");
  printf("JUMP ?DIV_jump_over\n");
  printf("LABEL ?DIV_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?DIV_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?DIV_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?DIV_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?DIV_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?DIV_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?DIV_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?DIV_int\n");
  printf("        JUMPIFEQ ?DIV_int_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?DIV_int_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?DIV_int_nil LF@typeRight string@nil\n");
  printf("        JUMP ?DIV_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?DIV_int_int\n");
  printf("            IDIV LF@result LF@left LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("            \n");
  printf("        LABEL ?DIV_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            DIV LF@result LF@left LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("        LABEL ?DIV_int_nil\n");
  printf("            IDIV LF@result LF@left int@0\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("    LABEL ?DIV_float\n");
  printf("        JUMPIFEQ ?DIV_float_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?DIV_float_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?DIV_float_nil LF@typeRight string@nil\n");
  printf("        JUMP ?DIV_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?DIV_float_int\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            DIV LF@result LF@left LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("        LABEL ?DIV_float_float\n");
  printf("            DIV LF@result LF@left LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("        LABEL ?DIV_float_nil\n");
  printf("            DIV LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("    LABEL ?DIV_nil\n");
  printf("        JUMPIFEQ ?DIV_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?DIV_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?DIV_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?DIV_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?DIV_nil_int\n");
  printf("            IDIV LF@result int@0 LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("        LABEL ?DIV_nil_float\n");
  printf("            DIV LF@result float@0x0p+0 LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("        LABEL ?DIV_nil_nil\n");
  printf("            IDIV LF@result int@0 int@0\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("    LABEL ?DIV_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL ?DIV_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?DIV_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?DIV_jump_over\n");
  printf("\n");
  printf("JUMP ?CONCAT_jump_over\n");
  printf("LABEL ?CONCAT_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?CONCAT_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?CONCAT_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?CONCAT_string LF@typeLeft string@string\n");
  printf("    JUMPIFEQ ?CONCAT_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?CONCAT_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?CONCAT_string\n");
  printf("        JUMPIFEQ ?CONCAT_string_string LF@typeRight string@string\n");
  printf("        JUMPIFEQ ?CONCAT_string_nil LF@typeRight string@nil\n");
  printf("        JUMP ?CONCAT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?CONCAT_string_string\n");
  printf("            CONCAT LF@result LF@left LF@right\n");
  printf("            JUMP ?CONCAT_end\n");
  printf("\n");
  printf("        LABEL ?CONCAT_string_nil\n");
  printf("            CONCAT LF@result LF@left string@\n");
  printf("            JUMP ?CONCAT_end\n");
  printf("\n");
  printf("    LABEL ?CONCAT_nil\n");
  printf("        JUMPIFEQ ?CONCAT_nil_string LF@typeRight string@string\n");
  printf("        JUMPIFEQ ?CONCAT_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?CONCAT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?CONCAT_nil_string\n");
  printf("            CONCAT LF@result string@ LF@right\n");
  printf("            JUMP ?CONCAT_end\n");
  printf("\n");
  printf("        LABEL ?CONCAT_nil_nil\n");
  printf("            CONCAT LF@result string@ string@\n");
  printf("            JUMP ?CONCAT_end\n");
  printf("\n");
  printf("    LABEL ?CONCAT_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL ?CONCAT_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?CONCAT_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?CONCAT_jump_over\n");
  printf("\n");
  printf("JUMP ?EQ_jump_over\n");
  printf("label ?EQ_op\n");
  printf("PUSHFRAME\n");
  printf("DEFVAR LF@typeLeft\n");
  printf("DEFVAR LF@typeRight\n");
  printf("\n");
  printf("TYPE LF@typeLeft LF@left\n");
  printf("TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("JUMPIFEQ ?EQ_undefined_var LF@typeLeft string@\n");
  printf("JUMPIFEQ ?EQ_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("DEFVAR LF@result\n");
  printf("\n");
  printf("JUMPIFEQ ?EQ_int LF@typeLeft string@int\n");
  printf("JUMPIFEQ ?EQ_float LF@typeLeft string@float\n");
  printf("JUMPIFEQ ?EQ_nil LF@typeLeft string@nil\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_int\n");
  printf("JUMPIFEQ ?EQ_int_int LF@typeRight string@int\n");
  printf("JUMPIFEQ ?EQ_int_float LF@typeRight string@float\n");
  printf("JUMPIFEQ ?EQ_int_nil LF@typeRight string@nil\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_int_int\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_int_float\n");
  printf("INT2FLOAT LF@left LF@left\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_int_nil\n");
  printf("EQ LF@result LF@left int@0\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_float\n");
  printf("JUMPIFEQ ?EQ_float_int LF@typeRight string@int\n");
  printf("JUMPIFEQ ?EQ_float_float LF@typeRight string@float\n");
  printf("JUMPIFEQ ?EQ_float_nil LF@typeRight string@nil\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_float_int\n");
  printf("INT2FLOAT LF@right LF@right\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_float_float\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_float_nil\n");
  printf("EQ LF@result LF@left float@0x0p+0\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_nil\n");
  printf("JUMPIFEQ ?EQ_nil_int LF@typeRight string@int\n");
  printf("JUMPIFEQ ?EQ_nil_float LF@typeRight string@float\n");
  printf("JUMPIFEQ ?EQ_nil_nil LF@typeRight string@nil\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_nil_int\n");
  printf("EQ LF@result int@0 LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_nil_float\n");
  printf("EQ LF@result float@0x0p+0 LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_nil_nil\n");
  printf("EQ LF@result int@0 int@0\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_end\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("\n");
  printf("LABEL ?EQ_ops_dont_match\n");
  printf("MOVE LF@result bool@false\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_undefined_var\n");
  printf("EXIT int@5\n");
  printf("LABEL ?EQ_jump_over\n");
  printf("\n");
  printf("JUMP ?LT_jump_over\n");
  printf("LABEL ?LT_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?LT_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?LT_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?LT_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?LT_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?LT_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?LT_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?LT_int\n");
  printf("        JUMPIFEQ ?LT_int_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?LT_int_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?LT_int_nil LF@typeRight string@nil\n");
  printf("        JUMP ?LT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?LT_int_int\n");
  printf("            LT LF@result LF@left LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            LT LF@result LF@left LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_int_nil\n");
  printf("            LT LF@result LF@left int@0\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("    LABEL ?LT_float\n");
  printf("        JUMPIFEQ ?LT_float_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?LT_float_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?LT_float_nil LF@typeRight string@nil\n");
  printf("        JUMP ?LT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?LT_float_int\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            LT LF@result LF@left LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_float_float\n");
  printf("            LT LF@result LF@left LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_float_nil\n");
  printf("            LT LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("    LABEL ?LT_nil\n");
  printf("        JUMPIFEQ ?LT_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?LT_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?LT_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?LT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?LT_nil_int\n");
  printf("            LT LF@result int@0 LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_nil_float\n");
  printf("            LT LF@result float@0x0p+0 LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_nil_nil\n");
  printf("            LT LF@result int@0 int@0\n");
  printf("            JUMP ?LT_end\n");
  printf("    \n");
  printf("\n");
  printf("    LABEL ?LT_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("    \n");
  printf("    LABEL ?LT_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?LT_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?LT_jump_over\n");
  printf("\n");
  printf("JUMP ?GT_jump_over\n");
  printf("LABEL ?GT_op\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@typeLeft\n");
  printf("    DEFVAR LF@typeRight\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?GT_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?GT_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ ?GT_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?GT_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?GT_nil LF@typeLeft string@nil\n");
  printf("    JUMP ?GT_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?GT_int\n");
  printf("        JUMPIFEQ ?GT_int_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?GT_int_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?GT_int_nil LF@typeRight string@nil\n");
  printf("        JUMP ?GT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?GT_int_int\n");
  printf("            GT LF@result LF@left LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            GT LF@result LF@left LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_int_nil\n");
  printf("            GT LF@result LF@left int@0\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("    LABEL ?GT_float\n");
  printf("        JUMPIFEQ ?GT_float_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?GT_float_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?GT_float_nil LF@typeRight string@nil\n");
  printf("        JUMP ?GT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?GT_float_int\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            GT LF@result LF@left LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_float_float\n");
  printf("            GT LF@result LF@left LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_float_nil\n");
  printf("            GT LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("    LABEL ?GT_nil\n");
  printf("        JUMPIFEQ ?GT_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?GT_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?GT_nil_nil LF@typeRight string@nil\n");
  printf("        JUMP ?GT_ops_dont_match\n");
  printf("\n");
  printf("        LABEL ?GT_nil_int\n");
  printf("            GT LF@result int@0 LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_nil_float\n");
  printf("            GT LF@result float@0x0p+0 LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_nil_nil\n");
  printf("            GT LF@result int@0 int@0\n");
  printf("            JUMP ?GT_end\n");
  printf("    \n");
  printf("\n");
  printf("    LABEL ?GT_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("    \n");
  printf("    LABEL ?GT_ops_dont_match\n");
  printf("        EXIT int@7\n");
  printf("\n");
  printf("    LABEL ?GT_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?GT_jump_over\n");
  printf("\n");
}

void codegen(AST *tree) {
  tSymtable global;
  symtable_init(&global);

  printf(
      ".IFJcode22\n"
      "\n");  // insert header
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
  InsertAllOpBuiltInFuncs();

  char *current_frame = GF;

  GoThruMain(tree, &global, current_frame);

  return;
}