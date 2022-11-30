#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "ASTree.h"
#include "include.h"
#include "symtable.h"

void codegen(AST *);
void ASTreeRecGoThru(AST *, tSymtable *, char *);
char *generateExp(AST *, tSymtable *, char *);
void generateOperation(AST *, tSymtable *, char *, char *);

#endif
