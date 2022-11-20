#include "stack.h"
#include "include.h"

struct stack *Stack_init(int size)
{
    struct stack *s = (struct stack*)malloc(sizeof(struct stack));
    if (s == NULL)
        return NULL;
    s->top = -1;
    s->size = size;
    s->data = (int*)malloc(sizeof(int) * size);
    if (s->data == NULL) {
        free(s);
        return NULL;
    }
    return s;
}

int Size(struct stack *s)
{
    return s->top + 1;
}

int Is_empty(struct stack *s)
{
    return s->top == -1;
}

int Is_full(struct stack *s)
{
    return s->top == s->size - 1;
}

void Push(struct stack *s, int data)
{
    if (is_full(s))
        return;
    s->data[++s->top] = data;
}

int Peek(struct stack *s)
{
    if (is_empty(s))
        return -1;
    return s->data[s->top];
}

int Pop(struct stack *s)
{
    if (is_empty(s))
        return -1;
    return s->data[s->top--];
}