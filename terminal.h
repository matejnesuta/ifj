#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "mystring.h"
#include "terminals.h"

/** Terminal*/
typedef struct Terminal {
  terminal_kind kind;
  string *code;
} terminal;

#endif