#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "ASTree.h"
#include "LList.h"
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

void codegen(AST *);

#endif
