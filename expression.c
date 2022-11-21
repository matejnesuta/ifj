#include "expression.h"
#include "parser.h"
#include "parser.c"
#include "stack.c"

#define PREC_TABLE_SIZE 7

typedef enum
{
    S,
    R,
    Eq,
    Er
}Prec_sign;

int Prec_table[PREC_TABLE_SIZE][PREC_TABLE_SIZE] = 
{
//   +- */ R  (  )  i  $
    {R, S, R, S, R, S, R}, // +-
    {R, R, R, S, R, S, R}, // */
    {S, S, Er, S, R, S, R}, // R
    {S, S, S, S, Eq, S, Er}, // (
    {R, R, R, Er, R, Er, R}, // )
    {R, R, R, Er, R, Er, R}, // i
    {S, S, S, S, Er, S, Er} // $
};

void ExpressionParser(Parser *parser)
{
    struct stack *stack = Stack_init(100);
    Push(stack, dollar);


    
}



