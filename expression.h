#ifndef __EXPRESSION_H__;
#define __EXPRESSION_H__;

#include "include.h"

typedef enum
{
    shift, // <
    reduce,  // >
    equal, // =
    error // error
} Express_sign;

typedef enum
{
    plus_minus_concat, // +-
    multiply_divide, // */
    left_bracket, // (
    right_bracket, // )
    reltion, // < > <= >= === !==
    id, // i
    dollar, // $
} Express_token;

#endif;