#include "codegen.h"

#include <string.h>

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
                   sizeof(char) * sizeof(term->node->terminal->code->data) + 1);
        if (intLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        sprintf(intLit, "int@%s", term->node->terminal->code->data);
        printf("MOVE %s %s\n", temp, intLit);
        return temp;
      case float_litTer:;
        char *floatLit =
            malloc(5 * sizeof(char) +
                   sizeof(char) * sizeof(term->node->terminal->code->data) + 1);
        if (floatLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        sprintf(floatLit, "float@%a",
                strtof(term->node->terminal->code->data, NULL));
        printf("MOVE %s %s\n", temp, floatLit);
        return temp;
      case string_litTer:;
        char *stringLit =
            malloc(7 * sizeof(char) +
                   sizeof(char) * sizeof(term->node->terminal->code->data) + 1);
        if (stringLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        sprintf(stringLit, "string@%s", term->node->terminal->code->data);
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

void ASTreeRecGoThru(AST *tree, tSymtable *symtable, char *current_frame) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      if (child->tree->node->terminal->kind != 14) {
        terminal *current_terminal = child->tree->node->terminal;
        switch (current_terminal->kind) {
          case rightCurlyBracketTer:
            return;
          case 22:  // variableTer
            if (child->next != NULL &&
                child->next->tree->node->is_terminal == true &&
                child->next->tree->node->terminal->kind == assignTer) {
              child = child->next->next;
              char *ret = generateExp(child->tree->children->first->tree,
                                      symtable, current_frame);
              if (!symtable_search(symtable, *(current_terminal)->code)) {
                symtable_insert_var(symtable, *(current_terminal)->code);
                printf("DEFVAR %s%s\n", current_frame,
                       current_terminal->code->data);
              }

              printf("MOVE %s%s %s\n", current_frame,
                     current_terminal->code->data,
                     ret);  // TODO : here needs to be data type check
            }
            break;
          case ifTer:
            // todo def all variables before the if
            child = child->next->next;
            char *ret = generateExp(child->tree, symtable, current_frame);
            printf("DEFVAR %s?%ldexp\n", current_frame, current_terminal->code);
            printf("MOVE %s?%ldexp %s\n", current_frame, current_terminal->code,
                   ret);

            LList_element *backup = child;

            lookForVarsInAScope(child->next->next->next->tree, symtable,
                                current_frame, NULL);
            child = backup;
            lookForVarsInAScope(
                child->next->next->next->next->next->next->next->tree, symtable,
                current_frame, NULL);
            child = backup;
            printf("JUMPIFNEQ else_%ld %s?%ldexp bool@true\n\n",
                   current_terminal->code, current_frame,
                   current_terminal->code);
            child = backup;
            child = child->next->next->next;
            ASTreeRecGoThru(child->tree, symtable, current_frame);
            printf("JUMP end_%ld\n", current_terminal->code);
            printf("\nLABEL else_%ld\n", current_terminal->code);
            child = child->next->next->next->next;
            ASTreeRecGoThru(child->tree, symtable, current_frame);
            printf("\nLABEL end_%ld\n", current_terminal->code);
          default:
            break;
        }
      }
    } else if (child->tree->node->nonterminal == EXP) {
      generateExp(child->tree, symtable, current_frame);
    } else {
      if (child->tree->node->nonterminal != START_PROLOG) {
        ASTreeRecGoThru(
            child->tree, symtable,
            current_frame);  // get one level deeper thru nonterminal
      }
    }
    child = child->next;
  }
}

void InsertAllOpBuiltInFuncs() {
  printf(
      "JUMP ?ADD_jump_over\n"
      "LABEL ?ADD_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?ADD_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?ADD_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?ADD_int LF@typeLeft string@int\n"
      "    JUMPIFEQ ?ADD_float LF@typeLeft string@float\n"
      "    JUMPIFEQ ?ADD_nil LF@typeLeft string@nil\n"
      "    JUMP ?ADD_ops_dont_match\n"
      "\n"
      "    LABEL ?ADD_int\n"
      "        JUMPIFEQ ?ADD_int_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?ADD_int_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?ADD_int_nil LF@typeRight string@nil\n"
      "        JUMP ?ADD_ops_dont_match\n"
      "\n"
      "        LABEL ?ADD_int_int\n"
      "            ADD LF@result LF@left LF@right\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "        LABEL ?ADD_int_float\n"
      "            INT2FLOAT LF@left LF@left\n"
      "            ADD LF@result LF@left LF@right\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "        LABEL ?ADD_int_nil\n"
      "            ADD LF@result LF@left int@0\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "    LABEL ?ADD_float\n"
      "        JUMPIFEQ ?ADD_float_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?ADD_float_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?ADD_float_nil LF@typeRight string@nil\n"
      "        JUMP ?ADD_ops_dont_match\n"
      "\n"
      "        LABEL ?ADD_float_int\n"
      "            INT2FLOAT LF@right LF@right\n"
      "            ADD LF@result LF@left LF@right\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "        LABEL ?ADD_float_float\n"
      "            ADD LF@result LF@left LF@right\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "        LABEL ?ADD_float_nil\n"
      "            ADD LF@result LF@left float@0x0p+0\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "    LABEL ?ADD_nil\n"
      "        JUMPIFEQ ?ADD_nil_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?ADD_nil_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?ADD_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?ADD_ops_dont_match\n"
      "\n"
      "        LABEL ?ADD_nil_int\n"
      "            ADD LF@result int@0 LF@right\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "        LABEL ?ADD_nil_float\n"
      "            ADD LF@result float@0x0p+0 LF@right\n"
      "            JUMP ?ADD_end\n"
      "\n"
      "        LABEL ?ADD_nil_nil\n"
      "            ADD LF@result int@0 int@0\n"
      "            JUMP ?ADD_end\n"
      "    \n"
      "\n"
      "    LABEL ?ADD_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "    \n"
      "    LABEL ?ADD_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?ADD_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?ADD_jump_over\n"
      "\n"
      "\n"
      "JUMP ?SUB_jump_over\n"
      "label ?SUB_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?SUB_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?SUB_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?SUB_int LF@typeLeft string@int\n"
      "    JUMPIFEQ ?SUB_float LF@typeLeft string@float\n"
      "    JUMPIFEQ ?SUB_nil LF@typeLeft string@nil\n"
      "    JUMP ?SUB_ops_dont_match\n"
      "\n"
      "    LABEL ?SUB_int\n"
      "        JUMPIFEQ ?SUB_int_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?SUB_int_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?SUB_int_nil LF@typeRight string@nil\n"
      "        JUMP ?SUB_ops_dont_match\n"
      "\n"
      "        LABEL ?SUB_int_int\n"
      "            SUB LF@result LF@left LF@right\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "        LABEL ?SUB_int_float\n"
      "            INT2FLOAT LF@left LF@left\n"
      "            SUB LF@result LF@left LF@right\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "        LABEL ?SUB_int_nil\n"
      "            SUB LF@result LF@left int@0\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "    LABEL ?SUB_float\n"
      "        JUMPIFEQ ?SUB_float_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?SUB_float_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?SUB_float_nil LF@typeRight string@nil\n"
      "        JUMP ?SUB_ops_dont_match\n"
      "\n"
      "        LABEL ?SUB_float_int\n"
      "            INT2FLOAT LF@right LF@right\n"
      "            SUB LF@result LF@left LF@right\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "        LABEL ?SUB_float_float\n"
      "            SUB LF@result LF@left LF@right\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "        LABEL ?SUB_float_nil\n"
      "            SUB LF@result LF@left float@0x0p+0\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "    LABEL ?SUB_nil\n"
      "        JUMPIFEQ ?SUB_nil_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?SUB_nil_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?SUB_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?SUB_ops_dont_match\n"
      "\n"
      "        LABEL ?SUB_nil_int\n"
      "            SUB LF@result int@0 LF@right\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "        LABEL ?SUB_nil_float\n"
      "            SUB LF@result float@0x0p+0 LF@right\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "        LABEL ?SUB_nil_nil\n"
      "            SUB LF@result int@0 int@0\n"
      "            JUMP ?SUB_end\n"
      "\n"
      "    LABEL ?SUB_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "\n"
      "    LABEL ?SUB_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?SUB_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?SUB_jump_over\n"
      "\n"
      "JUMP ?MUL_jump_over\n"
      "label ?MUL_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?MUL_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?MUL_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?MUL_int LF@typeLeft string@int\n"
      "    JUMPIFEQ ?MUL_float LF@typeLeft string@float\n"
      "    JUMPIFEQ ?MUL_nil LF@typeLeft string@nil\n"
      "    JUMP ?MUL_ops_dont_match\n"
      "\n"
      "    LABEL ?MUL_int\n"
      "        JUMPIFEQ ?MUL_int_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?MUL_int_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?MUL_int_nil LF@typeRight string@nil\n"
      "        JUMP ?MUL_ops_dont_match\n"
      "\n"
      "        LABEL ?MUL_int_int\n"
      "            MUL LF@result LF@left LF@right\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "        LABEL ?MUL_int_float\n"
      "            INT2FLOAT LF@left LF@left\n"
      "            MUL LF@result LF@left LF@right\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "        LABEL ?MUL_int_nil\n"
      "            MUL LF@result LF@left int@0\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "    LABEL ?MUL_float\n"
      "        JUMPIFEQ ?MUL_float_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?MUL_float_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?MUL_float_nil LF@typeRight string@nil\n"
      "        JUMP ?MUL_ops_dont_match\n"
      "\n"
      "        LABEL ?MUL_float_int\n"
      "            INT2FLOAT LF@right LF@right\n"
      "            MUL LF@result LF@left LF@right\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "        LABEL ?MUL_float_float\n"
      "            MUL LF@result LF@left LF@right\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "        LABEL ?MUL_float_nil\n"
      "            MUL LF@result LF@left float@0x0p+0\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "    LABEL ?MUL_nil\n"
      "        JUMPIFEQ ?MUL_nil_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?MUL_nil_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?MUL_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?MUL_ops_dont_match\n"
      "\n"
      "        LABEL ?MUL_nil_int\n"
      "            MUL LF@result int@0 LF@right\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "        LABEL ?MUL_nil_float\n"
      "            MUL LF@result float@0x0p+0 LF@right\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "        LABEL ?MUL_nil_nil\n"
      "            MUL LF@result int@0 int@0\n"
      "            JUMP ?MUL_end\n"
      "\n"
      "    LABEL ?MUL_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "\n"
      "    LABEL ?MUL_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?MUL_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?MUL_jump_over\n"
      "\n"
      "JUMP ?DIV_jump_over\n"
      "LABEL ?DIV_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?DIV_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?DIV_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?DIV_int LF@typeLeft string@int\n"
      "    JUMPIFEQ ?DIV_float LF@typeLeft string@float\n"
      "    JUMPIFEQ ?DIV_nil LF@typeLeft string@nil\n"
      "    JUMP ?DIV_ops_dont_match\n"
      "\n"
      "    LABEL ?DIV_int\n"
      "        JUMPIFEQ ?DIV_int_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?DIV_int_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?DIV_int_nil LF@typeRight string@nil\n"
      "        JUMP ?DIV_ops_dont_match\n"
      "\n"
      "        LABEL ?DIV_int_int\n"
      "            IDIV LF@result LF@left LF@right\n"
      "            JUMP ?DIV_end\n"
      "            \n"
      "        LABEL ?DIV_int_float\n"
      "            INT2FLOAT LF@left LF@left\n"
      "            DIV LF@result LF@left LF@right\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "        LABEL ?DIV_int_nil\n"
      "            IDIV LF@result LF@left int@0\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "    LABEL ?DIV_float\n"
      "        JUMPIFEQ ?DIV_float_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?DIV_float_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?DIV_float_nil LF@typeRight string@nil\n"
      "        JUMP ?DIV_ops_dont_match\n"
      "\n"
      "        LABEL ?DIV_float_int\n"
      "            INT2FLOAT LF@right LF@right\n"
      "            DIV LF@result LF@left LF@right\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "        LABEL ?DIV_float_float\n"
      "            DIV LF@result LF@left LF@right\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "        LABEL ?DIV_float_nil\n"
      "            DIV LF@result LF@left float@0x0p+0\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "    LABEL ?DIV_nil\n"
      "        JUMPIFEQ ?DIV_nil_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?DIV_nil_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?DIV_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?DIV_ops_dont_match\n"
      "\n"
      "        LABEL ?DIV_nil_int\n"
      "            IDIV LF@result int@0 LF@right\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "        LABEL ?DIV_nil_float\n"
      "            DIV LF@result float@0x0p+0 LF@right\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "        LABEL ?DIV_nil_nil\n"
      "            IDIV LF@result int@0 int@0\n"
      "            JUMP ?DIV_end\n"
      "\n"
      "    LABEL ?DIV_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "\n"
      "    LABEL ?DIV_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?DIV_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?DIV_jump_over\n"
      "\n"
      "JUMP ?CONCAT_jump_over\n"
      "LABEL ?CONCAT_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?CONCAT_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?CONCAT_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?CONCAT_string LF@typeLeft string@string\n"
      "    JUMPIFEQ ?CONCAT_nil LF@typeLeft string@nil\n"
      "    JUMP ?CONCAT_ops_dont_match\n"
      "\n"
      "    LABEL ?CONCAT_string\n"
      "        JUMPIFEQ ?CONCAT_string_string LF@typeRight string@string\n"
      "        JUMPIFEQ ?CONCAT_string_nil LF@typeRight string@nil\n"
      "        JUMP ?CONCAT_ops_dont_match\n"
      "\n"
      "        LABEL ?CONCAT_string_string\n"
      "            CONCAT LF@result LF@left LF@right\n"
      "            JUMP ?CONCAT_end\n"
      "\n"
      "        LABEL ?CONCAT_string_nil\n"
      "            CONCAT LF@result LF@left string@\n"
      "            JUMP ?CONCAT_end\n"
      "\n"
      "    LABEL ?CONCAT_nil\n"
      "        JUMPIFEQ ?CONCAT_nil_string LF@typeRight string@string\n"
      "        JUMPIFEQ ?CONCAT_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?CONCAT_ops_dont_match\n"
      "\n"
      "        LABEL ?CONCAT_nil_string\n"
      "            CONCAT LF@result string@ LF@right\n"
      "            JUMP ?CONCAT_end\n"
      "\n"
      "        LABEL ?CONCAT_nil_nil\n"
      "            CONCAT LF@result string@ string@\n"
      "            JUMP ?CONCAT_end\n"
      "\n"
      "    LABEL ?CONCAT_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "\n"
      "    LABEL ?CONCAT_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?CONCAT_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?CONCAT_jump_over\n"
      "\n"
      "JUMP ?EQ_jump_over\n"
      "label ?EQ_op\n"
      "PUSHFRAME\n"
      "DEFVAR LF@typeLeft\n"
      "DEFVAR LF@typeRight\n"
      "\n"
      "TYPE LF@typeLeft LF@left\n"
      "TYPE LF@typeRight LF@right\n"
      "\n"
      "JUMPIFEQ ?EQ_undefined_var LF@typeLeft string@\n"
      "JUMPIFEQ ?EQ_undefined_var LF@typeRight string@\n"
      "\n"
      "DEFVAR LF@result\n"
      "\n"
      "JUMPIFEQ ?EQ_int LF@typeLeft string@int\n"
      "JUMPIFEQ ?EQ_float LF@typeLeft string@float\n"
      "JUMPIFEQ ?EQ_nil LF@typeLeft string@nil\n"
      "JUMP ?EQ_ops_dont_match\n"
      "\n"
      "LABEL ?EQ_int\n"
      "JUMPIFEQ ?EQ_int_int LF@typeRight string@int\n"
      "JUMPIFEQ ?EQ_int_float LF@typeRight string@float\n"
      "JUMPIFEQ ?EQ_int_nil LF@typeRight string@nil\n"
      "JUMP ?EQ_ops_dont_match\n"
      "\n"
      "LABEL ?EQ_int_int\n"
      "EQ LF@result LF@left LF@right\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_int_float\n"
      "INT2FLOAT LF@left LF@left\n"
      "EQ LF@result LF@left LF@right\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_int_nil\n"
      "EQ LF@result LF@left int@0\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_float\n"
      "JUMPIFEQ ?EQ_float_int LF@typeRight string@int\n"
      "JUMPIFEQ ?EQ_float_float LF@typeRight string@float\n"
      "JUMPIFEQ ?EQ_float_nil LF@typeRight string@nil\n"
      "JUMP ?EQ_ops_dont_match\n"
      "\n"
      "LABEL ?EQ_float_int\n"
      "INT2FLOAT LF@right LF@right\n"
      "EQ LF@result LF@left LF@right\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_float_float\n"
      "EQ LF@result LF@left LF@right\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_float_nil\n"
      "EQ LF@result LF@left float@0x0p+0\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_nil\n"
      "JUMPIFEQ ?EQ_nil_int LF@typeRight string@int\n"
      "JUMPIFEQ ?EQ_nil_float LF@typeRight string@float\n"
      "JUMPIFEQ ?EQ_nil_nil LF@typeRight string@nil\n"
      "JUMP ?EQ_ops_dont_match\n"
      "\n"
      "LABEL ?EQ_nil_int\n"
      "EQ LF@result int@0 LF@right\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_nil_float\n"
      "EQ LF@result float@0x0p+0 LF@right\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_nil_nil\n"
      "EQ LF@result int@0 int@0\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_end\n"
      "POPFRAME\n"
      "RETURN\n"
      "\n"
      "LABEL ?EQ_ops_dont_match\n"
      "MOVE LF@result bool@false\n"
      "JUMP ?EQ_end\n"
      "\n"
      "LABEL ?EQ_undefined_var\n"
      "EXIT int@5\n"
      "LABEL ?EQ_jump_over\n"
      "\n"
      "JUMP ?LT_jump_over\n"
      "LABEL ?LT_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?LT_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?LT_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?LT_int LF@typeLeft string@int\n"
      "    JUMPIFEQ ?LT_float LF@typeLeft string@float\n"
      "    JUMPIFEQ ?LT_nil LF@typeLeft string@nil\n"
      "    JUMP ?LT_ops_dont_match\n"
      "\n"
      "    LABEL ?LT_int\n"
      "        JUMPIFEQ ?LT_int_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?LT_int_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?LT_int_nil LF@typeRight string@nil\n"
      "        JUMP ?LT_ops_dont_match\n"
      "\n"
      "        LABEL ?LT_int_int\n"
      "            LT LF@result LF@left LF@right\n"
      "            JUMP ?LT_end\n"
      "\n"
      "        LABEL ?LT_int_float\n"
      "            INT2FLOAT LF@left LF@left\n"
      "            LT LF@result LF@left LF@right\n"
      "            JUMP ?LT_end\n"
      "\n"
      "        LABEL ?LT_int_nil\n"
      "            LT LF@result LF@left int@0\n"
      "            JUMP ?LT_end\n"
      "\n"
      "    LABEL ?LT_float\n"
      "        JUMPIFEQ ?LT_float_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?LT_float_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?LT_float_nil LF@typeRight string@nil\n"
      "        JUMP ?LT_ops_dont_match\n"
      "\n"
      "        LABEL ?LT_float_int\n"
      "            INT2FLOAT LF@right LF@right\n"
      "            LT LF@result LF@left LF@right\n"
      "            JUMP ?LT_end\n"
      "\n"
      "        LABEL ?LT_float_float\n"
      "            LT LF@result LF@left LF@right\n"
      "            JUMP ?LT_end\n"
      "\n"
      "        LABEL ?LT_float_nil\n"
      "            LT LF@result LF@left float@0x0p+0\n"
      "            JUMP ?LT_end\n"
      "\n"
      "    LABEL ?LT_nil\n"
      "        JUMPIFEQ ?LT_nil_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?LT_nil_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?LT_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?LT_ops_dont_match\n"
      "\n"
      "        LABEL ?LT_nil_int\n"
      "            LT LF@result int@0 LF@right\n"
      "            JUMP ?LT_end\n"
      "\n"
      "        LABEL ?LT_nil_float\n"
      "            LT LF@result float@0x0p+0 LF@right\n"
      "            JUMP ?LT_end\n"
      "\n"
      "        LABEL ?LT_nil_nil\n"
      "            LT LF@result int@0 int@0\n"
      "            JUMP ?LT_end\n"
      "    \n"
      "\n"
      "    LABEL ?LT_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "    \n"
      "    LABEL ?LT_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?LT_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?LT_jump_over\n"
      "\n"
      "JUMP ?GT_jump_over\n"
      "LABEL ?GT_op\n"
      "    PUSHFRAME\n"
      "    DEFVAR LF@typeLeft\n"
      "    DEFVAR LF@typeRight\n"
      "\n"
      "    TYPE LF@typeLeft LF@left\n"
      "    TYPE LF@typeRight LF@right\n"
      "\n"
      "    JUMPIFEQ ?GT_undefined_var LF@typeLeft string@\n"
      "    JUMPIFEQ ?GT_undefined_var LF@typeRight string@\n"
      "\n"
      "    DEFVAR LF@result\n"
      "\n"
      "    JUMPIFEQ ?GT_int LF@typeLeft string@int\n"
      "    JUMPIFEQ ?GT_float LF@typeLeft string@float\n"
      "    JUMPIFEQ ?GT_nil LF@typeLeft string@nil\n"
      "    JUMP ?GT_ops_dont_match\n"
      "\n"
      "    LABEL ?GT_int\n"
      "        JUMPIFEQ ?GT_int_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?GT_int_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?GT_int_nil LF@typeRight string@nil\n"
      "        JUMP ?GT_ops_dont_match\n"
      "\n"
      "        LABEL ?GT_int_int\n"
      "            GT LF@result LF@left LF@right\n"
      "            JUMP ?GT_end\n"
      "\n"
      "        LABEL ?GT_int_float\n"
      "            INT2FLOAT LF@left LF@left\n"
      "            GT LF@result LF@left LF@right\n"
      "            JUMP ?GT_end\n"
      "\n"
      "        LABEL ?GT_int_nil\n"
      "            GT LF@result LF@left int@0\n"
      "            JUMP ?GT_end\n"
      "\n"
      "    LABEL ?GT_float\n"
      "        JUMPIFEQ ?GT_float_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?GT_float_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?GT_float_nil LF@typeRight string@nil\n"
      "        JUMP ?GT_ops_dont_match\n"
      "\n"
      "        LABEL ?GT_float_int\n"
      "            INT2FLOAT LF@right LF@right\n"
      "            GT LF@result LF@left LF@right\n"
      "            JUMP ?GT_end\n"
      "\n"
      "        LABEL ?GT_float_float\n"
      "            GT LF@result LF@left LF@right\n"
      "            JUMP ?GT_end\n"
      "\n"
      "        LABEL ?GT_float_nil\n"
      "            GT LF@result LF@left float@0x0p+0\n"
      "            JUMP ?GT_end\n"
      "\n"
      "    LABEL ?GT_nil\n"
      "        JUMPIFEQ ?GT_nil_int LF@typeRight string@int\n"
      "        JUMPIFEQ ?GT_nil_float LF@typeRight string@float\n"
      "        JUMPIFEQ ?GT_nil_nil LF@typeRight string@nil\n"
      "        JUMP ?GT_ops_dont_match\n"
      "\n"
      "        LABEL ?GT_nil_int\n"
      "            GT LF@result int@0 LF@right\n"
      "            JUMP ?GT_end\n"
      "\n"
      "        LABEL ?GT_nil_float\n"
      "            GT LF@result float@0x0p+0 LF@right\n"
      "            JUMP ?GT_end\n"
      "\n"
      "        LABEL ?GT_nil_nil\n"
      "            GT LF@result int@0 int@0\n"
      "            JUMP ?GT_end\n"
      "    \n"
      "\n"
      "    LABEL ?GT_end\n"
      "        POPFRAME\n"
      "        RETURN\n"
      "    \n"
      "    LABEL ?GT_ops_dont_match\n"
      "        EXIT int@7\n"
      "\n"
      "    LABEL ?GT_undefined_var\n"
      "        EXIT int@5\n"
      "LABEL ?GT_jump_over\n"
      "\n");
}

void codegen(AST *tree) {
  tSymtable global;
  symtable_init(&global);

  printf(
      ".IFJcode22\n"
      "\n");  // insert header
  InsertAllOpBuiltInFuncs();

  char *current_frame = GF;
  ASTreeRecGoThru(tree, &global, current_frame);

  // printf(
  //     "WRITE GF@$goofy\n"
  //     "DEFVAR GF@type\n"
  //     "TYPE GF@type GF@$goofy\n"
  //     "WRITE GF@type\n");

  return;
}