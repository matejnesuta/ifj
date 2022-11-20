#ifndef __STACK_H__
#define __STACK_H__

struct stack {
    int top;
    int size;
    int *data;
};
struct stack *Stack_init(int size);
int Size(struct stack *s);
int Is_empty(struct stack *s);
int Is_full(struct stack *s);
void Push(struct stack *s, int data);
int Peek(struct stack *s);
int Pop(struct stack *s);

#endif