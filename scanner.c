#include <stdio.h>

typedef enum 
{
    Start,
    Multiply,
    Division,
    LineComment,
    BlockComment,
    ExpectEnd,
    LineCommentEnd,
    BlockCommentEnd,
    Whitespace,
    Plus,
    Minus,
    Concat,
    Assign,
    ExpectEqual,
    Equal,
    Gt,
    Ge,
    Lt,
    Le,
    ExpectStartProlog1,
    ExpectStartProlog2,
    ExpectStartProlog3,
    StartPrologEnd,
    Semicolon,
    LeftBracket,
    RightBracket,
    LeftCurlyBracket,
    RightCurlyBracket,
    Integer,
    ExpFloat,
    IntDotInt,
    Exponent,
    PlusMinus,
    IntAfterExponent,
    Float,
    EnableNull,
    EndPrologEnd,
    Identifier,
    Colon,
    ExclamationMark,
    ExpectNotEqual,
    NotEqual,
    StartQuotes,
    escapeSeq,
    Text,
    EndQuotes,
    Error
} FSMstate;

int main() 
{
    printf("Hello World!\n");
    return 0;
}