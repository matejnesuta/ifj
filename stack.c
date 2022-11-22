#include "stack.h"
#include "include.h"
#include "terminal.h"
#include "symbol.h"



stack_val* StackValCreateSymbol(symbol* symbol) {
    stack_val* val = malloc(sizeof(struct stack_val));
    if (val == NULL) {
        exit(99);
    }
    val->sym = symbol;
    val->isSymbol = true;
    return val;
}

stack_val* StackValCreatePrec_sign(Prec_sign shift) {
    stack_val* val = malloc(sizeof(struct stack_val));
    if (val == NULL) {
        exit(99);
    }
    val->shift = shift;
    val->isSymbol = false;
    return val;
}

stack *Stack_init()
{
    stack *s = (stack*)malloc(sizeof(stack));
    if (s == NULL)
        return NULL;
    s->top = -1;
    s->size = 0;
    s->data = (stack_val*)malloc(sizeof(struct stack_val));
    if (s->data == NULL) {
        exit(99);
    }
    return s;
}

bool Is_empty(stack *s)
{
    if (s->top == -1)
        return false;
    else
        return true;
}

void Push(stack *s, stack_val* data)
{
    s->data = (stack_val*)realloc(s->data, sizeof(stack_val) * (s->size++ + 1));
    if (s->data == NULL) {
        exit(99);
    }
    s->data[++s->top] = *data;
}

stack_val* Pop(stack *s)
{
    if (Is_empty(s))
        return -1;
    return &s->data[s->top--];
}

terminal* TopTerminal(stack* stack) {
    if (stack->size == 0) {
        return -1;
    }
    for (int i = stack->size - 1; i >= 0; i--) {
        if (stack->data[i].isSymbol) {
            symbol* sym = stack->data[i].sym;
            if (sym->is_terminal) {
                return sym->terminal;
            }
        }
    }
    return -1;
}

stack_val* Top(stack* stack) {
    if (stack->size == 0) {
        return -1;
    }
    return &stack->data[stack->size - 1];
}