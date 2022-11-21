#ifndef __STACK_H__
#define __STACK_H__
#include "symbol.h"

typedef struct stack {
    int top;
    int size;
    symbol *data;
} stack;


struct stack *Stack_init();
void Push(struct stack *s, symbol* data);
symbol* Pop(struct stack *s);
symbol* Top(struct stack *s);
bool Is_empty(struct stack *s);

#endif