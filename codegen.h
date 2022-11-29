#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "ASTree.h"
#include "include.h"
#include "symtable.h"

void codegen(AST *);
void ASTreeRecGoThru(AST *, tSymtable *, char *);
void generateExp(AST *, tSymtable *, char *, char *);
void generateOperation(AST *, tSymtable *, char *, char *);

#endif
