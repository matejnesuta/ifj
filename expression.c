#include "expression.h"
#include "parser.h"
#include "stack.c"
#include "symbol.h"

#define PREC_TABLE_SIZE 7


int Prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = 
{
//  +-. */ R  (  )  i  $
    {R, S, R, S, R, S, R}, // +-.
    {R, R, R, S, R, S, R}, // */
    {S, S, Er, S, R, S, R}, // R
    {S, S, S, S, Eq, S, Er}, // (
    {R, R, R, Er, R, Er, R}, // )
    {R, R, R, Er, R, Er, R}, // i
    {S, S, S, S, Er, S, Er} // $
};

void ExpressionParser(Parser *parser)
{
    logger("ExpressionParser", "Start");
    struct stack *stack = Stack_init();
    logger("ExpressionParser", "Stack initialized");
    terminal *dollar = (terminal*)malloc(sizeof(struct Terminal));
    if (dollar == NULL) {
        exit(99);
    }
    dollar->kind = dollar;
    dollar->code = NULL;
    Push(stack, StackValCreateSymbol(SymbolCreateTerminal(dollar)));
    bool isEnd = false;
    while(isEnd) {
        stack_val *top = Top(stack);
        symbol* next = SymbolCreateTerminal(parser->LLfirst);
        
        // TODO 

    }
}



