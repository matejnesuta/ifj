#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "ASTree.h"
#include "include.h"
#include "mystring.h"
#include "symtable.h"

/**
 * @enum datatype
 */
typedef enum Datatypes {
  intDatatype,
  floatDatatype,
  stringDatatype,
  boolDatatype,
  nilDatatype
} datatype;

typedef struct Variable {
  char *name;
  datatype type;
} generatedVar;

void GenerateIfElseInFunc(char *, LList_element *, tSymtable *, char *);
void codegen(AST *);
void lookForVarsInAScope(AST *, tSymtable *, char *, string *);
void GoThruMain(AST *, tSymtable *, char *);
char *generateExp(AST *, tSymtable *, char *);
void generateOperation(AST *, tSymtable *, char *, char *);
void SecondGo(AST *, tSymtable *);
void CheckParam(LList_element *, string, bst_node_ptr_t);

void CreateTempFrameBeforeExp();
void GenerateAllFuncs();
#endif
