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
void ASTreeRecGoThru(AST *, tSymtable *, char *);
generatedVar generateExp(AST *, tSymtable *, char *);
void generateOperation(AST *, tSymtable *, char *, char *);

#endif
