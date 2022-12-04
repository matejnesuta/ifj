#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "ASTree.h"
#include "include.h"
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

void codegen(AST *);
void lookForVarsInAScope(AST *, tSymtable *, char *, string *);
void GoThruMain(AST *, tSymtable *, char *);
char *generateExp(AST *, tSymtable *, char *);
void generateOperation(AST *, tSymtable *, char *, char *);

#endif
