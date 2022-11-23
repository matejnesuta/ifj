#ifndef __EXPRESSION_H__;
#define __EXPRESSION_H__;

#include "include.h"

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
}Prec_rules;

void ExpressionParser();
#endif;