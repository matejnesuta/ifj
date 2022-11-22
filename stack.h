#ifndef __STACK_H__
#define __STACK_H__
#include "symbol.h"

typedef enum
{
    S,
    R,
    Eq,
    Er
}Prec_sign;

typedef struct stack_val {
    struct symbol *sym;
    Prec_sign shift;
    bool isSymbol;
} stack_val;

typedef struct stack {
    int top;
    int size;
    struct stack_val *data;
} stack;


struct stack *Stack_init();
void Push(struct stack *s, stack_val* data);
stack_val* Pop(struct stack *s);
stack_val* Top(struct stack *s);
terminal* TopTerminal(struct stack *s);
bool Is_empty(struct stack *s);

#endif