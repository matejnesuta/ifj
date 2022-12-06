#include "codegen.h"

#include "LList.h"
#include "error.h"
#include "logger.h"
#include "symtable.h"

#define GF "GF@"
#define TF "TF@"
#define LF "LF@"

void variableDefined(tSymtable *symtable, terminal *term) {
  if (!symtable_search(symtable, *(term)->code)) {
    ErrorExit(5, "Variable was not defined!");
  }
}

void CreateTempFrameBeforeExp() {
  printf(
      "CREATEFRAME\n"
      "DEFVAR TF@result\n"
      "DEFVAR TF@left\n"
      "DEFVAR TF@right\n"
      "DEFVAR TF@typeLeft\n"
      "DEFVAR TF@typeRight\n"
      "DEFVAR TF@conditional\n");
}

char *Operation(terminal_kind op, char *temp, char *left, char *right) {
  switch (op) {
    case plusTer:
      printf("MOVE %s %s\n", "TF@left", left);
      printf("MOVE %s %s\n", "TF@right", right);
      printf("CALL ?ADD_op\n");
      printf("MOVE %s TF@result\n", temp);
      return temp;
    case minusTer:
      printf("MOVE %s %s\n", "TF@left", left);
      printf("MOVE %s %s\n", "TF@right", right);
      printf("CALL ?SUB_op\n");
      printf("MOVE %s TF@result\n", temp);
      return temp;
    case multiplyTer:
      printf("MOVE %s %s\n", "TF@left", left);
      printf("MOVE %s %s\n", "TF@right", right);
      printf("CALL ?MUL_op\n");
      printf("MOVE %s TF@result\n", temp);
      return temp;
    case divideTer:
      printf("MOVE %s %s\n", "TF@left", left);
      printf("MOVE %s %s\n", "TF@right", right);
      printf("CALL ?DIV_op\n");
      printf("MOVE %s TF@result\n", temp);
      return temp;
    case dotTer:
      printf("MOVE %s %s\n", "TF@left", left);
      printf("MOVE %s %s\n", "TF@right", right);
      printf("CALL ?CONCAT_op\n");
      printf("MOVE %s TF@result\n", temp);
      return temp;
    case equalTer:
    case notEqualTer:
      printf(
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
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?LT_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case greaterTer:
      printf(
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?GT_op\n"
          "MOVE %s TF@result\n",
          left, right, temp);
      return temp;

    case lessOrEqualTer:;
    case greaterOrEqualTer:;
      char x = '\0';
      x = (op == lessOrEqualTer) ? 'G' : 'L';
      printf(
          "MOVE TF@left %s\n"
          "MOVE TF@right %s\n"
          "call ?%cT_op\n"
          "MOVE %s TF@result\n",
          left, right, x, temp);
      printf("NOT %s %s\n", temp, temp);
      return temp;
    default:
      exit(100);  // TODO
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
    int size =
        snprintf(NULL, 0, "%s_left_op_right_%ld", TF, (long)term->node) + 1;
    char *temp = malloc(size);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    sprintf(temp, "%s_left_op_right_%ld", TF, (long)term->node);
    printf("DEFVAR %s\n", temp);
    return Operation(term->node->terminal->kind, temp, left_var, right_var);

  } else if (term->node->terminal->kind == variableTer) {  // handle variable
    if (!(symtable_search(symtable, *(term->node->terminal->code)))) {
      ErrorExit(5, "Variable not defined!");
    }
    int size = snprintf(NULL, 0, "%s_%s_%ld", TF,
                        term->node->terminal->code->data, (long)term->node) +
               1;
    char *temp = malloc(size);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    snprintf(temp, size, "%s_%s_%ld", TF, term->node->terminal->code->data,
             (long)term->node);
    printf("DEFVAR %s\n", temp);
    printf("MOVE %s %s%s\n", temp, current_frame,
           term->node->terminal->code->data);
    return temp;
  } else {  // handle constant
    int size = snprintf(NULL, 0, "%s_konst_%ld", TF, (long)term->node) + 1;
    char *temp = malloc(size);
    if (temp == NULL) {
      ErrorExit(99, "Malloc failed!");
    }
    snprintf(temp, size, "%s_konst_%ld", TF, (long)term->node);
    printf("DEFVAR %s\n", temp);
    switch (term->node->terminal->kind) {
      case int_litTer:
        size =
            snprintf(NULL, 0, "int@%s", term->node->terminal->code->data) + 1;
        char *intLit = malloc(size);
        if (intLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(intLit, size, "int@%s", term->node->terminal->code->data);
        printf("MOVE %s %s\n", temp, intLit);
        return temp;
      case float_litTer:
        size = snprintf(NULL, 0, "float@%a",
                        strtod(term->node->terminal->code->data, NULL)) +
               1;
        char *floatLit = malloc(size);
        if (floatLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(floatLit, size, "float@%a",
                 strtod(term->node->terminal->code->data, NULL));
        printf("MOVE %s %s\n", temp, floatLit);
        return temp;
      case string_litTer:
        size =
            snprintf(NULL, 0, "string@%s", term->node->terminal->code->data) +
            1;
        char *stringLit = malloc(size);
        if (stringLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(stringLit, size, "string@%s",
                 term->node->terminal->code->data);
        printf("MOVE %s %s\n", temp, stringLit);
        return temp;
      case nullTer:
        printf("MOVE %s nil@nil\n", temp);
        return temp;
      default:
        ErrorExit(69420, "No way you got here!");
    }
  }
  ErrorExit(69420, "No way you got here!");
  return NULL;
}

/**
 * @brief Add params to symtable
 *
 * @param current currnet terminal
 * @param param parameter
 * @param NewFunc function in symtable
 * @param position position of param in function
 */
void CheckParam(LList_element *current, string param, bst_node_ptr_t NewFunc) {
  LList_element *tmp = current;
  while (tmp->tree->node->nonterminal != FUNC_DECLARE_BODY) {
    tmp = tmp->next;
  }
  if (tmp->tree->children->first == NULL) {
    return;  // no other arguments
  }
  tmp = tmp->tree->children->first;
  while (tmp->tree->node->nonterminal != ARG_TYPE) {
    tmp = tmp->next;
  }
  LList_element *argType = tmp->tree->children->first;
  tmp = tmp->next;
  param = *(tmp->tree->node->terminal->code);

  NewFunc->data->func->paramNames[NewFunc->data->func->paramCount] = param;

  NewFunc->data->func->paramDataTypes[NewFunc->data->func->paramCount] =
      *(argType->tree->node->terminal->code);

  NewFunc->data->func->paramCount++;
  CheckParam(tmp, param, NewFunc);
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

void SolveVariableAssignmentByExp(LList_element *child, tSymtable *symtable,
                                  char *current_frame) {
  terminal *current_terminal = child->tree->node->terminal;

  child = child->next->next;
  CreateTempFrameBeforeExp();
  char *ret =
      generateExp(child->tree->children->first->tree, symtable, current_frame);
  if (!symtable_search(symtable, *(current_terminal)->code)) {
    symtable_insert_var(symtable, *(current_terminal)->code);
    printf("DEFVAR %s%s\n", current_frame, current_terminal->code->data);
  }
  printf("MOVE %s%s %s\n", current_frame, current_terminal->code->data, ret);
}

void GenerateWhileInMain(LList_element *termWhile, tSymtable *global,
                         char *current_frame) {
  terminal *current_terminal = termWhile->tree->node->terminal;
  // get vars from inside
  LList_element *inner_child = termWhile->next->next;
  LList_element *backup = inner_child;
  lookForVarsInAScope(inner_child->next->next->next->tree, global,
                      current_frame, NULL);
  inner_child = backup;
  // jump loop condition
  printf("\nJUMP ?%ldstart\n", (long)current_terminal->code);
  // label loop
  printf("LABEL ?%ldloop\n", (long)current_terminal->code);
  // body
  GoThruMain(inner_child->next->next->next->tree, global, current_frame);
  printf("JUMP ?%ldcondition\n", (long)current_terminal->code);
  // label loop comp
  printf("\nLABEL ?%ldstart\n\n", (long)current_terminal->code);
  printf("LABEL ?%ldcondition\n", (long)current_terminal->code);
  // comp expr
  // framePush();

  CreateTempFrameBeforeExp();
  char *ret = generateExp(inner_child->tree, global, current_frame);
  // framePop(NULL, current_frame, ret);
  // printf("MOVE %s?%ldexp %s\n", current_frame, current_terminal->code, ret);
  // jumpifeq loop
  printf("MOVE %s %s\n", "TF@left", ret);
  printf("CALL ?condition_op\n");
  printf("JUMPIFEQ ?%ldloop TF@conditional bool@true\n\n",
         (long)current_terminal->code);
}

void GenerateIfElseInMain(LList_element *IF, tSymtable *global,
                          char *current_frame) {
  terminal *current_terminal = IF->tree->node->terminal;
  LList_element *inner_child = IF->next->next;
  CreateTempFrameBeforeExp();
  char *ret = generateExp(inner_child->tree, global, current_frame);
  LList_element *backup = inner_child;

  lookForVarsInAScope(inner_child->next->next->next->tree, global,
                      current_frame, NULL);
  inner_child = backup;
  lookForVarsInAScope(
      inner_child->next->next->next->next->next->next->next->tree, global,
      current_frame, NULL);
  inner_child = backup;
  printf("MOVE %s %s\n", "TF@left", ret);
  printf("CALL ?condition_op\n");
  printf("JUMPIFNEQ else_%ld TF@conditional bool@true\n\n",
         (long)current_terminal->code);
  inner_child = backup;
  inner_child = inner_child->next->next->next;
  GoThruMain(inner_child->tree, global, current_frame);
  printf("JUMP end_%ld\n", (long)current_terminal->code);
  printf("\nLABEL else_%ld\n", (long)current_terminal->code);
  inner_child = inner_child->next->next->next->next;
  GoThruMain(inner_child->tree, global, current_frame);
  printf("\nLABEL end_%ld\n", (long)current_terminal->code);
}

void SolveEmptyExpression(LList_element *child, tSymtable *symtable,
                          char *current_frame) {
  CreateTempFrameBeforeExp();
  generateExp(child->tree, symtable, current_frame);
}

void FindNextArgs(AST *nontermNextArg, LList *func_call_args) {
  if (nontermNextArg->children->first == NULL) {
    return;
  }
  AST *nontermArg = nontermNextArg->children->first->next->tree;
  if (nontermArg->children->first->tree->node->is_terminal) {
    func_call_args =
        LListInsertChild(func_call_args, nontermArg->children->first->tree);
  } else {
    func_call_args = LListInsertChild(
        func_call_args,
        nontermArg->children->first->tree->children->first->tree);
  }
  FindNextArgs(nontermNextArg->children->first->next->next->tree,
               func_call_args);
}

void FindAllFuncCallArgs(AST *nontermFuncCallArgs, LList *func_call_args) {
  if (nontermFuncCallArgs->children->first == NULL) {
    return;
  }
  if (nontermFuncCallArgs->children->first->tree->children->first->tree->node
          ->is_terminal) {
    func_call_args = LListInsertChild(
        func_call_args,
        nontermFuncCallArgs->children->first->tree->children->first->tree);
  } else {
    func_call_args = LListInsertChild(
        func_call_args, nontermFuncCallArgs->children->first->tree->children
                            ->first->tree->children->first->tree);
  }
  FindNextArgs(nontermFuncCallArgs->children->first->next->tree,
               func_call_args);
}

void GenerateWriteFuncCall(LList *func_call_args) {
  LList_element *arg = func_call_args->first;
  while (arg != NULL) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@_arg1\n");
    int size;
    switch (arg->tree->node->terminal->kind) {
      case variableTer:
        printf("MOVE TF@_arg1 LF@%s\n", arg->tree->node->terminal->code->data);
        printf("CALL write\n");
        break;
      case int_litTer:
        size =
            snprintf(NULL, 0, "int@%s", arg->tree->node->terminal->code->data) +
            1;
        char *intLit = malloc(size);
        if (intLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(intLit, size, "int@%s", arg->tree->node->terminal->code->data);
        printf("MOVE TF@_arg1 %s\n", intLit);
        printf("CALL write\n");
        break;
      case float_litTer:
        size = snprintf(NULL, 0, "float@%a",
                        strtod(arg->tree->node->terminal->code->data, NULL)) +
               1;
        char *floatLit = malloc(size);
        if (floatLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(floatLit, size, "float@%a",
                 strtod(arg->tree->node->terminal->code->data, NULL));
        printf("MOVE TF@_arg1 %s\n", floatLit);
        printf("CALL write\n");
        break;
      case string_litTer:
        printf("MOVE TF@_arg1 string@%s\n",
               arg->tree->node->terminal->code->data);
        printf("CALL write\n");
        break;
      case nullTer:
        printf("MOVE TF@_arg1 nil@nil\n");
        printf("CALL write\n");
        break;
      default:
        ErrorExit(69420, "no goddamn way u r here");
        break;
    }
    arg = arg->next;
  }
}

void GenerateFuncCall(LList_element *func_name, LList *func_call_args) {
  LList_element *arg = func_call_args->first;
  printf("CREATEFRAME\n");
  int arg_num = 1;
  while (arg != NULL) {
    printf("DEFVAR TF@_arg%d\n", arg_num);
    int size;
    switch (arg->tree->node->terminal->kind) {
      case variableTer:
        printf("MOVE TF@_arg%d LF@%s\n", arg_num,
               arg->tree->node->terminal->code->data);
        break;
      case int_litTer:
        size =
            snprintf(NULL, 0, "int@%s", arg->tree->node->terminal->code->data) +
            1;
        char *intLit = malloc(size);
        if (intLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(intLit, size, "int@%s", arg->tree->node->terminal->code->data);
        printf("MOVE TF@_arg%d %s\n", arg_num, intLit);
        break;
      case float_litTer:
        size = snprintf(NULL, 0, "float@%a",
                        strtod(arg->tree->node->terminal->code->data, NULL)) +
               1;
        char *floatLit = malloc(size);
        if (floatLit == NULL) {
          ErrorExit(99, "Malloc failed!");
        }
        snprintf(floatLit, size, "float@%a",
                 strtod(arg->tree->node->terminal->code->data, NULL));
        printf("MOVE TF@_arg%d %s\n", arg_num, floatLit);
        break;
      case string_litTer:
        printf("MOVE TF@_arg%d string@%s\n", arg_num,
               arg->tree->node->terminal->code->data);
        break;
      case nullTer:
        printf("MOVE TF@_arg%d nil@nil\n", arg_num);
        break;
      default:
        ErrorExit(69420, "no goddamn way u r here");
        break;
    }
    arg = arg->next;
  }
  printf("CALL %s\n", func_name->tree->node->terminal->code->data);
}

void SolveVariableAssignmentByFuncCall(LList_element *child,
                                       tSymtable *symtable,
                                       char *current_frame) {
  LList_element *var = child;
  LList_element *func_name =
      child->next->next->tree->children->first->tree->children->first;
  LList *func_call_args = LListInit();
  FindAllFuncCallArgs(func_name->next->next->tree, func_call_args);
  GenerateFuncCall(func_name, func_call_args);
  if (!symtable_search(symtable, *(var->tree->node->terminal->code))) {
    symtable_insert_var(symtable, *(var->tree->node->terminal->code));
    printf("DEFVAR %s%s\n", current_frame,
           var->tree->node->terminal->code->data);
  }
  printf("MOVE %s%s TF@result\n", current_frame,
         var->tree->node->terminal->code->data);
}

void GenerateFuncDeclare(LList_element *nontermFuncDecl, tSymtable *symtable,
                         char *current_frame) {}

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
                SolveVariableAssignmentByExp(inner_child, global,
                                             current_frame);
              } else if (inner_child->next->next->tree->node->is_terminal ==
                             false &&
                         inner_child->next->next->tree->children->first->tree
                                 ->node->nonterminal == FUNC_CALL) {
                // solves variable assignment by function call
                SolveVariableAssignmentByFuncCall(inner_child, global,
                                                  current_frame);
              }
              break;

            case returnTer:
              if (inner_child->next->tree->children->first == NULL) {
                printf("EXIT int@0\n");
              } else {
                SolveEmptyExpression(inner_child->next, global, current_frame);
                printf("EXIT int@0\n");
              }
              break;

            case whileTer:
              GenerateWhileInMain(inner_child, global, current_frame);
              break;

            case leftCurlyBracketTer:
              GoThruMain(inner_child->next->tree, global, current_frame);
              break;

            default:
              ErrorExit(69420, "how did you get here?");
              break;
          }
        } else if (inner_child->tree->node->nonterminal == EXP) {
          SolveEmptyExpression(inner_child, global, current_frame);
        } else if (inner_child->tree->node->nonterminal == IF_ELSE) {
          GenerateIfElseInMain(inner_child->tree->children->first, global,
                               current_frame);

        } else if (inner_child->tree->node->nonterminal == FUNC_CALL) {
          inner_child = inner_child->tree->children->first;
          LList *func_call_args = LListInit();
          FindAllFuncCallArgs(inner_child->next->next->tree, func_call_args);
          if (strcmp(inner_child->tree->node->terminal->code->data, "write") ==
              0) {
            GenerateWriteFuncCall(func_call_args);
          } else {
            GenerateFuncCall(inner_child, func_call_args);
          }
        }
      } else if (child->tree->node->nonterminal == FUNC_DECLARE) {
        {
          LList_element *inner_child = child->tree->children->first;
          terminal *next_terminal = inner_child->next->tree->node->terminal;
          string param = (string){.data = NULL, .size = 0};
          bst_node_ptr_t NewFunc =
              symtable_search(global, *(next_terminal->code));
          if (NewFunc != NULL) {
            ErrorExit(3, "Function already defined!");
          }
          symtable_insert_func(global, *(next_terminal->code));
          bst_node_ptr_t func = symtable_search(global, *(next_terminal->code));
          CheckParam(inner_child, param, func);

          GenerateFuncDeclare(child, global, current_frame);
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

void codegen(AST *tree) {
  tSymtable global;
  symtable_init(&global);

  printf(
      ".IFJcode22\n"
      "\n");  // insert header
  printf("CREATEFRAME\n");
  printf("PUSHFRAME\n");
  char *current_frame = LF;

  GoThruMain(tree, &global, current_frame);
  SecondGo(tree, &global);

  GenerateAllFuncs();

  return;
}

void CompFuncCallsAndDecls(AST *tree, tSymtable *global) {
  if (tree->children == NULL || tree->children->first == NULL) {
    return;  // if tree has no children (should be error)
  }

  LList_element *child = tree->children->first;
  while (child != NULL) {
    if (child->tree->node->is_terminal) {
      // do something with terminal
    } else {
      // do something with nonterminal
      if (child->tree->node->nonterminal == FUNC_CALL) {
        LList_element *func_call = child->tree->children->first;
        bool is_write =
            strcmp(func_call->tree->node->terminal->code->data, "write") == 0;
        if (!is_write) {
          bst_node_ptr_t func =
              symtable_search(global, *(func_call->tree->node->terminal->code));
          if (func == NULL) {
            ErrorExit(3, "Function not defined!");
          }
          LList *func_call_args = LListInit();
          FindAllFuncCallArgs(func_call->next->next->tree, func_call_args);
          int count_args = 0;
          LList_element *arg = func_call_args->first;
          while (arg != NULL) {
            count_args++;
            arg = arg->next;
          }
          if (count_args != func->data->func->paramCount) {
            ErrorExit(4, "Wrong number of arguments!");
          }
        }
      }
      CompFuncCallsAndDecls(child->tree,
                            global);  // get one level deeper thru nonterminal
    }
    child = child->next;
  }
}

void SecondGo(AST *tree, tSymtable *global) {
  if (tree == NULL) {
    return;
  }

  CompFuncCallsAndDecls(tree, global);
}
void GenerateAllFuncs() {
  printf("\n");
  printf("\n");
  printf("JUMP ?ADD_jump_over\n");
  printf("LABEL ?ADD_op\n");
  printf("    PUSHFRAME\n");
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?ADD_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?ADD_undefined_var LF@typeRight string@\n");
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
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?SUB_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?SUB_undefined_var LF@typeRight string@\n");
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
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?MUL_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?MUL_undefined_var LF@typeRight string@\n");
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
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?DIV_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?DIV_undefined_var LF@typeRight string@\n");
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
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            INT2FLOAT LF@right LF@right\n");
  printf("            DIV LF@result LF@left LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("            \n");
  printf("        LABEL ?DIV_int_float\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            DIV LF@result LF@left LF@right\n");
  printf("            JUMP ?DIV_end\n");
  printf("\n");
  printf("        LABEL ?DIV_int_nil\n");
  printf("            INT2FLOAT LF@left LF@left\n");
  printf("            DIV LF@result LF@left float@0x0p+0\n");
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
  printf("            DIV LF@result float@0x0p+0 float@0x0p+0\n");
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
  printf("    \n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?CONCAT_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?CONCAT_undefined_var LF@typeRight string@\n");
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
  printf("\n");
  printf("JUMP ?EQ_jump_over\n");
  printf("label ?EQ_op\n");
  printf("PUSHFRAME\n");
  printf("\n");
  printf("TYPE LF@typeLeft LF@left\n");
  printf("TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("JUMPIFEQ ?EQ_undefined_var LF@typeLeft string@\n");
  printf("JUMPIFEQ ?EQ_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("\n");
  printf("JUMPIFEQ ?EQ_int LF@typeLeft string@int\n");
  printf("JUMPIFEQ ?EQ_float LF@typeLeft string@float\n");
  printf("JUMPIFEQ ?EQ_nil LF@typeLeft string@nil\n");
  printf("JUMPIFEQ ?EQ_string LF@typeLeft string@string\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_string\n");
  printf("JUMPIFEQ ?EQ_string_string LF@typeRight string@string\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_int\n");
  printf("JUMPIFEQ ?EQ_int_int LF@typeRight string@int\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_int_int\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_float\n");
  printf("JUMPIFEQ ?EQ_float_float LF@typeRight string@float\n");
  printf("JUMP ?EQ_ops_dont_match\n");
  printf("\n");
  printf("LABEL ?EQ_float_float\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("LABEL ?EQ_string_string\n");
  printf("EQ LF@result LF@left LF@right\n");
  printf("JUMP ?EQ_end\n");
  printf("\n");
  printf("LABEL ?EQ_nil\n");
  printf("JUMPIFEQ ?EQ_nil_nil LF@typeRight string@nil\n");
  printf("JUMP ?EQ_ops_dont_match\n");
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
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?LT_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?LT_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("\n");
  printf("    JUMPIFEQ ?LT_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?LT_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?LT_nil LF@typeLeft string@nil\n");
  printf("    JUMPIFEQ ?LT_string LF@typeLeft string@string\n");
  printf("    JUMP ?LT_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?LT_string\n");
  printf("        JUMPIFEQ ?LT_string_string LF@typeRight string@string\n");
  printf("        JUMPIFEQ ?LT_string_nil LF@typeRight string@nil\n");
  printf("        JUMP ?LT_ops_dont_match\n");
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
  printf("        LABEL ?LT_string_string\n");
  printf("            LT LF@result LF@left LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_string_nil\n");
  printf("        LT LF@result LF@left string@\n");
  printf("        JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_float_nil\n");
  printf("            LT LF@result LF@left float@0x0p+0\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("    LABEL ?LT_nil\n");
  printf("        JUMPIFEQ ?LT_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?LT_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?LT_nil_nil LF@typeRight string@nil\n");
  printf("        JUMPIFEQ ?LT_nil_string LF@typeRight string@string\n");
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
  printf("        LABEL ?LT_nil_string\n");
  printf("            LT LF@result string@ LF@right\n");
  printf("            JUMP ?LT_end\n");
  printf("\n");
  printf("        LABEL ?LT_nil_nil\n");
  printf("            LT LF@result int@0 int@0\n");
  printf("            JUMP ?LT_end\n");
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
  printf("\n");
  printf("    TYPE LF@typeLeft LF@left\n");
  printf("    TYPE LF@typeRight LF@right\n");
  printf("\n");
  printf("    JUMPIFEQ ?GT_undefined_var LF@typeLeft string@\n");
  printf("    JUMPIFEQ ?GT_undefined_var LF@typeRight string@\n");
  printf("\n");
  printf("\n");
  printf("    JUMPIFEQ ?GT_int LF@typeLeft string@int\n");
  printf("    JUMPIFEQ ?GT_float LF@typeLeft string@float\n");
  printf("    JUMPIFEQ ?GT_nil LF@typeLeft string@nil\n");
  printf("    JUMPIFEQ ?GT_string LF@typeLeft string@string\n");
  printf("    JUMP ?GT_ops_dont_match\n");
  printf("\n");
  printf("    LABEL ?GT_string\n");
  printf("        JUMPIFEQ ?GT_string_string LF@typeRight string@string\n");
  printf("        JUMPIFEQ ?GT_string_nil LF@typeRight string@nil\n");
  printf("        JUMP ?GT_ops_dont_match\n");
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
  printf("        LABEL ?GT_string_string\n");
  printf("            GT LF@result LF@left LF@right\n");
  printf("            JUMP ?GT_end\n");
  printf("\n");
  printf("        LABEL ?GT_string_nil\n");
  printf("        GT LF@result LF@left string@\n");
  printf("        JUMP ?GT_end\n");
  printf("\n");
  printf("    LABEL ?GT_nil\n");
  printf("        JUMPIFEQ ?GT_nil_int LF@typeRight string@int\n");
  printf("        JUMPIFEQ ?GT_nil_float LF@typeRight string@float\n");
  printf("        JUMPIFEQ ?GT_nil_nil LF@typeRight string@nil\n");
  printf("        JUMPIFEQ ?GT_nil_string LF@typeRight string@string\n");
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
  printf("        LABEL ?GT_nil_string\n");
  printf("            GT LF@result string@ LF@right\n");
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
  printf("\n");
  printf("JUMP ?reads_jump_over\n");
  printf("LABEL reads\n");
  printf("    CREATEFRAME\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@result\n");
  printf("    READ LF@result string\n");
  printf("    POPFRAME\n");
  printf("    RETURN\n");
  printf("LABEL ?reads_jump_over\n");
  printf("\n");
  printf("JUMP ?readi_jump_over\n");
  printf("LABEL readi\n");
  printf("    CREATEFRAME\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@result\n");
  printf("    READ LF@result int\n");
  printf("    POPFRAME\n");
  printf("    RETURN\n");
  printf("LABEL ?readi_jump_over\n");
  printf("\n");
  printf("JUMP ?readf_jump_over\n");
  printf("LABEL readf\n");
  printf("    CREATEFRAME\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@result\n");
  printf("    READ LF@result float\n");
  printf("    POPFRAME\n");
  printf("    RETURN\n");
  printf("LABEL ?readf_jump_over\n");
  printf("\n");
  printf("JUMP ?write_jump_over\n");
  printf("LABEL write\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    JUMPIFEQ write_undefined_var LF@type string@\n");
  printf("    WRITE LF@_arg1\n");
  printf("    POPFRAME\n");
  printf("    RETURN\n");
  printf("\n");
  printf("    LABEL write_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?write_jump_over\n");
  printf("\n");
  printf("JUMP ?floatval_jump_over\n");
  printf("LABEL floatval\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    JUMPIFEQ floatval_undefined_var LF@type string@\n");
  printf("    DEFVAR LF@result\n");
  printf("    JUMPIFEQ floatval_int LF@type string@int\n");
  printf("    JUMPIFEQ floatval_float LF@type string@float\n");
  printf("    JUMPIFEQ floatval_nil LF@type string@nil\n");
  printf("    EXIT int@4\n");
  printf("\n");
  printf("    LABEL floatval_int\n");
  printf("        INT2FLOAT LF@result LF@_arg1\n");
  printf("        JUMP floatval_end\n");
  printf("\n");
  printf("    LABEL floatval_float\n");
  printf("        MOVE LF@result LF@_arg1\n");
  printf("        JUMP floatval_end\n");
  printf("\n");
  printf("    LABEL floatval_nil\n");
  printf("        MOVE LF@result float@0x0p+0\n");
  printf("        JUMP floatval_end\n");
  printf("\n");
  printf("    LABEL floatval_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL floatval_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?floatval_jump_over\n");
  printf("\n");
  printf("JUMP ?intval_jump_over\n");
  printf("LABEL intval\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    JUMPIFEQ intval_undefined_var LF@type string@\n");
  printf("    DEFVAR LF@result\n");
  printf("\n");
  printf("    JUMPIFEQ intval_int LF@type string@int\n");
  printf("    JUMPIFEQ intval_float LF@type string@float\n");
  printf("    JUMPIFEQ intval_nil LF@type string@nil\n");
  printf("    EXIT int@4\n");
  printf("\n");
  printf("    LABEL intval_int\n");
  printf("        MOVE LF@result LF@_arg1\n");
  printf("        JUMP intval_end\n");
  printf("\n");
  printf("    LABEL intval_float\n");
  printf("        FLOAT2INT LF@result LF@_arg1\n");
  printf("        JUMP intval_end\n");
  printf("\n");
  printf("    LABEL intval_nil\n");
  printf("        MOVE LF@result int@0\n");
  printf("        JUMP intval_end\n");
  printf("\n");
  printf("    LABEL intval_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL intval_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?intval_jump_over\n");
  printf("\n");
  printf("JUMP ?strval_jump_over\n");
  printf("LABEL strval\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    JUMPIFEQ strval_undefined_var LF@type string@\n");
  printf("    DEFVAR LF@result\n");
  printf("    JUMPIFEQ strval_string LF@type string@string\n");
  printf("    JUMPIFEQ strval_nil LF@type string@nil\n");
  printf("    EXIT int@4\n");
  printf("\n");
  printf("    LABEL strval_string\n");
  printf("        MOVE LF@result LF@_arg1\n");
  printf("        JUMP strval_end\n");
  printf("\n");
  printf("    LABEL strval_nil\n");
  printf("        MOVE LF@result string@\n");
  printf("        JUMP strval_end\n");
  printf("\n");
  printf("    LABEL strval_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL strval_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?strval_jump_over\n");
  printf("\n");
  printf("JUMP ?strlen_jump_over\n");
  printf("LABEL strlen\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    JUMPIFEQ strlen_undefined_var LF@type string@\n");
  printf("    DEFVAR LF@result\n");
  printf("    JUMPIFEQ strlen_string LF@type string@string\n");
  printf("    EXIT int@4\n");
  printf("\n");
  printf("    LABEL strlen_string\n");
  printf("        STRLEN LF@result LF@_arg1\n");
  printf("        JUMP strlen_end\n");
  printf("    \n");
  printf("    LABEL strlen_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL strlen_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?strlen_jump_over\n");
  printf("\n");
  printf("JUMP ?substring_jump_over\n");
  printf("LABEL substring\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type1\n");
  printf("    DEFVAR LF@type2\n");
  printf("    DEFVAR LF@type3\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    TYPE LF@type2 LF@_arg2\n");
  printf("    TYPE LF@type3 LF@_arg3\n");
  printf("    JUMPIFEQ substring_undefined_var LF@type1 string@\n");
  printf("    JUMPIFEQ substring_undefined_var LF@type2 string@\n");
  printf("    JUMPIFEQ substring_undefined_var LF@type3 string@\n");
  printf("    JUMPIFNEQ substring_bad_arg_type LF@type1 string@string\n");
  printf("    JUMPIFNEQ substring_bad_arg_type LF@type2 string@int\n");
  printf("    JUMPIFNEQ substring_bad_arg_type LF@type3 string@int\n");
  printf("    DEFVAR LF@len\n");
  printf("    STRLEN LF@len LF@_arg1\n");
  printf("    DEFVAR LF@bool\n");
  printf("    LT LF@bool LF@_arg2 int@0\n");
  printf("    JUMPIFNEQ substring_bad_arg_value LF@bool bool@true\n");
  printf("    LT LF@bool LF@_arg3 int@0\n");
  printf("    JUMPIFNEQ substring_bad_arg_value LF@bool bool@true\n");
  printf("    GT LF@bool LF@_arg2 LF@_arg3\n");
  printf("    JUMPIFNEQ substring_bad_arg_value LF@bool bool@true \n");
  printf("    DEFVAR LF@bool2\n");
  printf("    GT LF@bool LF@_arg2 LF@len\n");
  printf("    EQ LF@bool2 LF@_arg2 LF@len\n");
  printf("    OR LF@bool LF@bool LF@bool2\n");
  printf("    JUMPIFNEQ substring_bad_arg_value LF@bool bool@true\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("    MOVE LF@result string@\n");
  printf("    DEFVAR LF@i\n");
  printf("    MOVE LF@i LF@_arg2\n");
  printf("    DEFVAR LF@char\n");
  printf("    LABEL substring_loop\n");
  printf("        JUMPIFEQ substring_end LF@i LF@_arg3\n");
  printf("        GETCHAR LF@char LF@_arg1 LF@i\n");
  printf("        CONCAT LF@result LF@result LF@char\n");
  printf("        ADD LF@i LF@i int@1\n");
  printf("        JUMP substring_loop\n");
  printf("    \n");
  printf("    LABEL substring_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL substring_ret_null\n");
  printf("        MOVE LF@result string@\n");
  printf("        JUMP substring_end\n");
  printf("\n");
  printf("    LABEL substring_bad_arg_type\n");
  printf("        EXIT int@4\n");
  printf("    LABEL substring_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?substring_jump_over\n");
  printf("\n");
  printf("JUMP ?ord_jump_over\n");
  printf("LABEL ord\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@$c\n");
  printf("    JUMPIFEQ ord_undefined_var LF@type string@\n");
  printf("    JUMPIFNEQ ord_bad_arg_type LF@type string@string\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("    JUMPIFEQ ord_empty_string LF@$c string@\n");
  printf("    STRI2INT LF@result LF@$c int@0\n");
  printf("    JUMP ord_end\n");
  printf("\n");
  printf("    LABEL ord_empty_string\n");
  printf("        MOVE LF@result int@0\n");
  printf("        JUMP ord_end\n");
  printf("\n");
  printf("    LABEL ord_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL ord_bad_arg_type\n");
  printf("        EXIT int@4\n");
  printf("\n");
  printf("    LABEL ord_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?ord_jump_over\n");
  printf("\n");
  printf("JUMP ?chr_jump_over\n");
  printf("LABEL chr\n");
  printf("    PUSHFRAME\n");
  printf("    DEFVAR LF@type\n");
  printf("    TYPE LF@type LF@_arg1\n");
  printf("    JUMPIFEQ chr_undefined_var LF@type string@\n");
  printf("    JUMPIFNEQ chr_bad_arg_type LF@type string@int\n");
  printf("\n");
  printf("    DEFVAR LF@result\n");
  printf("    INT2CHAR LF@result LF@_arg1\n");
  printf("    JUMP chr_end\n");
  printf("\n");
  printf("    LABEL chr_end\n");
  printf("        POPFRAME\n");
  printf("        RETURN\n");
  printf("\n");
  printf("    LABEL chr_bad_arg_type\n");
  printf("        EXIT int@4\n");
  printf("\n");
  printf("    LABEL chr_undefined_var\n");
  printf("        EXIT int@5\n");
  printf("LABEL ?chr_jump_over\n");
  printf("\n");
  printf("\n");
  printf("JUMP ?condition_jump_over\n");
  printf("label ?condition_op\n");
  printf("PUSHFRAME\n");
  printf("\n");
  printf("TYPE LF@typeLeft LF@left\n");
  printf("\n");
  printf("JUMPIFEQ ?condition_bool LF@typeLeft string@bool\n");
  printf("JUMPIFEQ ?condition_false LF@typeLeft string@\n");
  printf("JUMPIFEQ ?condition_int LF@typeLeft string@int\n");
  printf("JUMPIFEQ ?condition_float LF@typeLeft string@float\n");
  printf("JUMPIFEQ ?condition_false LF@typeLeft string@nil\n");
  printf("JUMP ?condition_string\n");
  printf("\n");
  printf("LABEL ?condition_string\n");
  printf("JUMPIFEQ ?condition_false LF@left string@0\n");
  printf("JUMPIFEQ ?condition_false LF@left string@\n");
  printf("JUMP ?condition_true\n");
  printf("\n");
  printf("LABEL ?condition_int\n");
  printf("JUMPIFEQ ?condition_false LF@left int@0\n");
  printf("JUMP ?condition_true\n");
  printf("\n");
  printf("LABEL ?condition_float\n");
  printf("JUMPIFEQ ?condition_false LF@left float@0x0p+0\n");
  printf("JUMP ?condition_true\n");
  printf("\n");
  printf("LABEL ?condition_bool\n");
  printf("MOVE LF@conditional LF@left\n");
  printf("JUMP ?condition_end\n");
  printf("\n");
  printf("LABEL ?condition_false\n");
  printf("MOVE LF@conditional bool@false\n");
  printf("JUMP ?condition_end\n");
  printf("\n");
  printf("LABEL ?condition_true\n");
  printf("MOVE LF@conditional bool@true\n");
  printf("JUMP ?condition_end\n");
  printf("\n");
  printf("LABEL ?condition_end\n");
  printf("POPFRAME\n");
  printf("RETURN\n");
  printf("\n");
  printf("LABEL ?condition_jump_over\n");
}
