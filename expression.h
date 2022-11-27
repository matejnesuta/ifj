#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "include.h"
#include "DLList.c"

typedef enum
{
    S,
    R,
    Eq,
    Er
}Prec_sign;

typedef enum
{
    plus_minus_dot,
    multiply_divide,
    relation,
    left_bracket,
    right_bracket,
    identifier,
    dollar_E
}Prec_index;

typedef enum
{
    identifier_E,       //E -> i
    lb_rb_E,            //E -> (E)

    E_plus_E,           //E -> E + E
    E_minus_E,          //E -> E - E
    E_dot_E,            //E -> E . E

    E_multiply_E,       //E -> E * E
    E_divide_E,         //E -> E / E

    E_less_E,           //E -> E < E
    E_lessOrEqual_E,    //E -> E <= E
    E_greater_E,        //E -> E > E
    E_greaterOrEqual_E, //E -> E >= E
    E_equal_E,          //E -> E === E
    E_notEqual_E        //E -> E !== E

}Expression_rules;


void ExpressionParser();
void CheckExpression(DLList *list, AST *tree, Parser *parser);
// void Rule_i_to_E(DLList *list);
// void Rule_lb_rb_E(LList *list, LList *list_ptr);
// void Rule_E_pmcmd_E_to_E(LList *list, LList *list_ptr);
// void Rule_E_r_E_to_E(LList *list, LList *list_ptr);
#endif