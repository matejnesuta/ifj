#include "stack.h"
#include "include.h"
#include "terminal.h"
#include "symbol.h"

stack *Stack_init()
{
    stack *s = (stack*)malloc(sizeof(stack));
    if (s == NULL)
        return NULL;
    s->top = -1;
    s->size = malloc(sizeof(int));
    s->data = (symbol*)malloc(sizeof(symbol));
    if (s->data == NULL) {
        free(s);
        return NULL;
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

void Push(stack *s, symbol* data)
{
    s->data[++s->top] = *data;
    s->size = realloc(s->size, (s->top + 1) * sizeof(int));
}

symbol* Pop(stack *s)
{
    if (Is_empty(s))
        return -1;
    return &s->data[s->top--];
}

symbol* Top(stack* stack) {
    if (stack->size == 0) {
        //todo error
    }
    symbol* symbol = NULL;
    for (int i = 1; i <= stack->size; i++) {
        symbol = &stack->data[stack->size - i];
        if (symbol->is_terminal) {
            return symbol;
        }
    }
    //todo error
    return symbol;
}