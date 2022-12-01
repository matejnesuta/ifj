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
generatedVar Operation(terminal_kind op, char *temp, generatedVar left,
                       generatedVar right) {
  switch (op) {
    case plusTer:
      if (left.type == intDatatype && right.type == intDatatype) {
        printf("ADD %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == floatDatatype) {
        printf("ADD %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == floatDatatype) {
        printf("INT2FLOAT %s, %s\n", left.name, left.name);
        printf("ADD %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == floatDatatype && right.type == intDatatype) {
        printf("INT2FLOAT %s, %s\n", right.name, right.name);
        printf("ADD %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == intDatatype) {
        printf("ADD %s, %s, %s\n", temp, "int@0", right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == nilDatatype && right.type == floatDatatype) {
        printf("ADD %s, %s, %s\n", temp, "float@0x0p+0", right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == nilDatatype) {
        printf("ADD %s, %s, %s\n", temp, left.name, "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == nilDatatype) {
        printf("ADD %s, %s, %s\n", temp, left.name, "float@0x0p+0");
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == nilDatatype) {
        printf("ADD %s, %s, %s\n", temp, "int@0", "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      ErrorExit(4, "Wrong type of operands!");
      break;

    case minusTer:
      if (left.type == intDatatype && right.type == intDatatype) {
        printf("SUB %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == floatDatatype) {
        printf("SUB %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == floatDatatype) {
        printf("INT2FLOAT %s, %s\n", left.name, left.name);
        printf("SUB %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == floatDatatype && right.type == intDatatype) {
        printf("INT2FLOAT %s, %s\n", right.name, right.name);
        printf("SUB %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == intDatatype) {
        printf("SUB %s, %s, %s\n", temp, "int@0", right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == nilDatatype && right.type == floatDatatype) {
        printf("SUB %s, %s, %s\n", temp, "float@0x0p+0", right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == nilDatatype) {
        printf("SUB %s, %s, %s\n", temp, left.name, "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == nilDatatype) {
        printf("SUB %s, %s, %s\n", temp, left.name, "float@0x0p+0");
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == nilDatatype) {
        printf("SUB %s, %s, %s\n", temp, "int@0", "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      ErrorExit(4, "Wrong type of operands!");
      break;

    case multiplyTer:
      if (left.type == intDatatype && right.type == intDatatype) {
        printf("MUL %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == floatDatatype) {
        printf("MUL %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == floatDatatype) {
        printf("INT2FLOAT %s, %s\n", left.name, left.name);
        printf("MUL %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == floatDatatype && right.type == intDatatype) {
        printf("INT2FLOAT %s, %s\n", right.name, right.name);
        printf("MUL %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == intDatatype) {
        printf("MUL %s, %s, %s\n", temp, "int@0", right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == nilDatatype && right.type == floatDatatype) {
        printf("MUL %s, %s, %s\n", temp, "float@0x0p+0", right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == nilDatatype) {
        printf("MUL %s, %s, %s\n", temp, left.name, "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == nilDatatype) {
        printf("MUL %s, %s, %s\n", temp, left.name, "float@0x0p+0");
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == nilDatatype) {
        printf("MUL %s, %s, %s\n", temp, "int@0", "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      ErrorExit(4, "Wrong type of operands!");
      break;
    case dotTer:
      if (left.type == stringDatatype && right.type == stringDatatype) {
        printf("CONCAT %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = stringDatatype};
      }
      ErrorExit(4, "Wrong type of operands!");
      break;
    case divideTer:
      if (left.type == intDatatype && right.type == intDatatype) {
        printf("IDIV %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if (left.type == floatDatatype && right.type == floatDatatype) {
        printf("DIV %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == intDatatype && right.type == floatDatatype) {
        printf("INT2FLOAT %s, %s\n", left.name, left.name);
        printf("DIV %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if(left.type == floatDatatype && right.type == intDatatype) {
        printf("INT2FLOAT %s, %s\n", right.name, right.name);
        printf("DIV %s, %s, %s\n", temp, left.name, right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if(left.type == nilDatatype && right.type == intDatatype) {
        printf("IDIV %s, %s, %s\n", temp, "int@0", right.name);
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if(left.type == nilDatatype && right.type == floatDatatype) {
        printf("DIV %s, %s, %s\n", temp, "float@0x0p+0", right.name);
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if(left.type == intDatatype && right.type == nilDatatype) {
        printf("IDIV %s, %s, %s\n", temp, left.name, "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      if(left.type == floatDatatype && right.type == nilDatatype) {
        printf("DIV %s, %s, %s\n", temp, left.name, "float@0x0p+0");
        return (generatedVar){.name = temp, .type = floatDatatype};
      }
      if (left.type == nilDatatype && right.type == nilDatatype) {
        printf("IDIV %s, %s, %s\n", temp, "int@0", "int@0");
        return (generatedVar){.name = temp, .type = intDatatype};
      }
      
      ErrorExit(4, "Wrong type of operands!");
      break;
    default:  // TODO finish other operators
      return (generatedVar){.name = "nil", .type = nilDatatype};
  }
}

generatedVar generateExp(AST *tree, tSymtable *symtable, char *current_frame) {
  AST *term = tree;
  while (term->node->is_terminal == false) {  // finds first terminal
    term = term->children->first->tree;
  }
  if (term->node->terminal->kind == plusTer ||  // the terminal is an operator
      term->node->terminal->kind == minusTer ||
      term->node->terminal->kind == multiplyTer  ||
      term->node->terminal->kind == divideTer  || 
      term->node->terminal->kind == dotTer /*|| // TODO finish other operators
      term->node->terminal->kind == lessTer ||
      term->node->terminal->kind == lessOrEqualTer ||
      term->node->terminal->kind == greaterTer ||
      term->node->terminal->kind == greaterOrEqualTer ||
      term->node->terminal->kind == equalTer ||
      term->node->terminal->kind == notEqualTer */) {
    generatedVar left_var =
        generateExp(term->children->first->tree, symtable,
                    current_frame);  // gets left side of the operator
    generatedVar right_var =
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
    variableDefined(symtable, term->node->terminal);
    char *temp =
        malloc(sizeof(current_frame) + sizeof(char) * sizeof(long) + 1);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    sprintf(temp, "%s_%ld", current_frame, (long)term->node);
    printf("DEFVAR %s\n", temp);
    printf("MOVE %s %s%s\n", temp, current_frame,
           term->node->terminal->code->data);

    return (generatedVar){
        .name = temp, .type = 1 /*TODO : should be retrieved from symtable*/};
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
        return (generatedVar){.name = temp, .type = intDatatype};
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
        return (generatedVar){.name = temp, .type = floatDatatype};
      case string_litTer:;
        char *stringLit =
            malloc(7 * sizeof(char) +
                   sizeof(char) * sizeof(term->node->terminal->code->data) + 1);
        if (stringLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        sprintf(stringLit, "string@%s", term->node->terminal->code->data);
        printf("MOVE %s %s\n", temp, stringLit);
        return (generatedVar){.name = temp, .type = stringDatatype};
      case nullTer:
        printf("MOVE %s nil@nil\n", temp);
        return (generatedVar){.name = temp, .type = nilDatatype};
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
          case 22:  // variableTer
            if (child->next->tree->node->terminal->kind == assignTer) {
              child = child->next->next;
              generatedVar ret = generateExp(child->tree->children->first->tree,
                                             global, current_frame);
              if (!symtable_search(global, *(current_terminal)->code)) {
                symtable_insert_var(global, *(current_terminal)->code);
                printf("DEFVAR %s%s\n", current_frame,
                       current_terminal->code->data);
              }
              printf("MOVE %s%s %s\n", current_frame,
                     current_terminal->code->data,
                     ret.name);  // TODO : here needs to be data type check
            }
          default:
            break;
        }
      }
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

void codegen(AST *tree) {
  tSymtable global;
  symtable_init(&global);
  char *current_frame = GF;
  ASTreeRecGoThru(tree, &global, current_frame);
  return;
}