#include <stdio.h>
#include <string.h>

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
    Variable,
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

FSMstate transition(FSMstate currIn, char edge)
{
    switch(currIn)
    {
        case Start:
            switch(edge)
            {
                case '*':
                    return Multiply;
                case '/':
                    return Division;
                case '+':
                    return Plus;
                case '-':
                    return Minus;
                case '.':
                    return Concat;
                case '=':
                    return Assign;
                case '>':
                    return Gt;
                case '<':
                    return Lt;
                case ';':
                    return Semicolon;
                case '(':
                    return LeftBracket;
                case ')':
                    return RightBracket;
                case '{':
                    return LeftCurlyBracket;
                case '}':
                    return RightCurlyBracket;
                case '?':
                    return EnableNull;
                case '$':
                    return Variable;
                case ':':
                    return Colon;
                case '!':
                    return ExclamationMark;
                case '"':
                    return StartQuotes;
            }
            if(isspace(edge))
                return Whitespace;
            if(isdigit(edge))
                return Integer;
            if(isletter(edge) || edge == '_')
                return Identifier;
    }
}

int main()
{
    printf("Hello World!\n");
    return 0;
}
