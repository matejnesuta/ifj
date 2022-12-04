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

      // case equalTer:
      //   printf("EQ %s %s %s\n", temp, left.name, right.name);
      //   printf("%s\n", temp);
      //   return (generatedVar){.name = temp, .type = boolDatatype};

      // case notEqualTer:
      //   printf("EQ %s %s %s\n", temp, left.name, right.name);
      //   printf("NOT %s %s\n", temp, temp);
      //   return (generatedVar){.name = temp, .type = boolDatatype};

      // case lessOrEqualTer:;
      // case greaterOrEqualTer:;
      //   char x = '\0';
      //   char *temp2 = temp;
      //   temp2[3] = '*';
      //   x = (op == lessOrEqualTer) ? 'L' : 'G';
      //   printf("DEFVAR %s\n", temp2);
      //   if (left.type == intDatatype && right.type == intDatatype) {
      //     printf("%cT %s %s %s\n", x, temp, left.name, right.name);
      //     printf("EQ %s %s %s\n", temp2, left.name, right.name);
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == floatDatatype && right.type == floatDatatype) {
      //     printf("%cT %s %s %s\n", x, temp, left.name, right.name);
      //     printf("EQ %s %s %s\n", temp2, left.name, right.name);
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == intDatatype && right.type == floatDatatype) {
      //     printf("INT2FLOAT %s %s\n", left.name, left.name);
      //     printf("%cT %s %s %s\n", x, temp, left.name, right.name);
      //     printf("EQ %s %s %s\n", temp2, left.name, right.name);
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == floatDatatype && right.type == intDatatype) {
      //     printf("INT2FLOAT %s %s\n", right.name, right.name);
      //     printf("%cT %s %s %s\n", x, temp, left.name, right.name);
      //     printf("EQ %s %s %s\n", temp2, left.name, right.name);
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == stringDatatype && right.type == stringDatatype) {
      //     printf("%cT %s %s %s\n", x, temp, left.name, right.name);
      //     printf("EQ %s %s %s\n", temp2, left.name, right.name);
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == nilDatatype &&
      //       (right.type == stringDatatype || right.type == floatDatatype ||
      //        right.type == intDatatype)) {
      //     printf("%cT %s %s %s\n", x, temp, "string@", right.name);
      //     printf("EQ %s %s %s\n", temp2, "string@", right.name);
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if ((left.type == intDatatype || floatDatatype || stringDatatype) &&
      //       right.type == nilDatatype) {
      //     printf("%cT %s %s %s\n", x, temp, left.name, "string@");
      //     printf("EQ %s %s %s\n", temp2, left.name, "string@");
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == nilDatatype && right.type == nilDatatype) {
      //     printf("%cT %s %s %s\n", x, temp, "string@", "string@");
      //     printf("EQ %s %s %s\n", temp2, "string@", "string@");
      //     printf("OR %s %s %s\n", temp, temp, temp2);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   ErrorExit(7, "Wrong type of operands!");
      //   break;

      // case greaterTer:;
      // case lessTer:;
      //   char c = '\0';
      //   c = (op == lessTer) ? 'L' : 'G';
      //   if (left.type == intDatatype && right.type == intDatatype) {
      //     printf("%cT %s %s %s\n", c, temp, left.name, right.name);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == floatDatatype && right.type == floatDatatype) {
      //     printf("%cT %s %s %s\n", c, temp, left.name, right.name);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == intDatatype && right.type == floatDatatype) {
      //     printf("INT2FLOAT %s %s\n", left.name, left.name);
      //     printf("%cT %s %s %s\n", c, temp, left.name, right.name);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == floatDatatype && right.type == intDatatype) {
      //     printf("INT2FLOAT %s %s\n", right.name, right.name);
      //     printf("%cT %s %s %s\n", c, temp, left.name, right.name);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == stringDatatype && right.type == stringDatatype) {
      //     printf("%cT %s %s %s\n", c, temp, left.name, right.name);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == nilDatatype &&
      //       (right.type == stringDatatype || right.type == floatDatatype ||
      //        right.type == intDatatype)) {
      //     printf("%cT %s %s %s\n", c, temp, "string@", right.name);
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if ((left.type == intDatatype || floatDatatype || stringDatatype) &&
      //       right.type == nilDatatype) {
      //     printf("%cT %s %s %s\n", c, temp, left.name, "string@");
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   if (left.type == nilDatatype && right.type == nilDatatype) {
      //     printf("%cT %s %s %s\n", c, temp, "string@", "string@");
      //     return (generatedVar){.name = temp, .type = boolDatatype};
      //   }
      //   ErrorExit(7, "Wrong type of operands!");
      //   break;

    default:  // TODO finish other operators
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

void ASTreeRecGoThru(AST *tree, tSymtable *global, char *current_frame) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      if (child->tree->node->terminal->kind != 14) {
        terminal *current_terminal = child->tree->node->terminal;
        switch (current_terminal->kind) {
          case variableTer:
            if (child->next != NULL &&
                child->next->tree->node->is_terminal == true &&
                child->next->tree->node->terminal->kind == assignTer) {
              child = child->next->next;
              char *ret = generateExp(child->tree->children->first->tree,
                                      global, current_frame);
              if (!symtable_search(global, *(current_terminal)->code)) {
                symtable_insert_var(global, *(current_terminal)->code);
                printf("DEFVAR %s%s\n", current_frame,
                       current_terminal->code->data);
              }

              printf("MOVE %s%s %s\n", current_frame,
                     current_terminal->code->data,
                     ret);  // TODO : here needs to be data type check
            }
            break;

          case functionTer:;
            terminal *next_terminal = child->next->tree->node->terminal;

            bst_node_ptr_t NewFunc =
                symtable_search(global, *(next_terminal->code));
            if (NewFunc != NULL) {
              ErrorExit(3, "Function already defined!");
            }
            symtable_insert_func(global, *(next_terminal->code));

          default:
            break;
        }
      }
    } else if (child->tree->node->nonterminal == EXP) {
      generateExp(child->tree, global, current_frame);
    } else {
      if (child->tree->node->nonterminal != START_PROLOG) {
        ASTreeRecGoThru(
            child->tree, global,
            current_frame);  // get one level deeper thru nonterminal
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

  printf("WRITE GF@$y\n");

  return;
}