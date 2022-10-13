#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define getName(var)  #var


typedef enum FSMstates
{
    Start,
    Multiply,
    Division,
    BlockComment,
    LineComment,
    ExpectEndBlockComment,
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
    ExpectStartProlog4,
    StartPrologEnd,
    Semicolon,
    LeftBracket,
    RightBracket,
    LeftCurlyBracket,
    RightCurlyBracket,
    Integer,
    FloatIntDotInt,
    Exponent,
    PlusMinus,
    FloatIntAfterExponent,
    EnableNull,
    EndPrologEnd1,
    EndPrologEnd2,
    Variable,
    Identifier,
    Colon,
    EndOfFile,
    ExclamationMark,
    ExpectNotEqual,
    NotEqual,
    String,
    EscapeSeq,
    StringEnd,
    Error
} state;

typedef struct 
{
    enum
    {
        MULTIPLY,
        DIVISION,
        MINUS,
        PLUS,
        INTEGER,
        FLOAT,
        CONCAT,
        ASSIGN,
        EQUAL,
        GT,
        GE,
        LT,
        LE,
        STARTPROLOG,
        SEMICOLON,
        LEFTBRACKET,
        RIGHTBRACKET,
        LEFTCURLYBRACKET,
        RIGHTCURLYBRACKET,
        ENDPROLOG,
        IDENTIFIER,
        COLON,
        ENDOFFILE,
        NOTEQUAL,
        STRING
    } kind;
    size_t data;
} Lexeme;

state transition(state currIn, int edge)
{
    switch(currIn)
    {
        case Start:
            switch(edge)
            {
                case '*': return Multiply;
                case '/': return Division;
                case '+': return Plus;
                case '-': return Minus;
                case '.': return Concat;
                case '=': return Assign;
                case '>': return Gt;
                case '<': return Lt;
                case ';': return Semicolon;
                case '(': return LeftBracket;
                case ')': return RightBracket;
                case '{': return LeftCurlyBracket;
                case '}': return RightCurlyBracket;
                case '?': return EnableNull;
                case '$': return Variable;
                case ':': return Colon;
                case EOF: return EndOfFile;
                case '!': return ExclamationMark;
                case '"': return String;
            }
            if(isspace(edge)) return Start;
            if(isdigit(edge)) return Integer;
            if(isalpha(edge) || edge == '_') return Identifier; 
            return Error;

        case Multiply:
            return Error;

        case Division:
            if(edge == '*') return BlockComment;
            if(edge == '/') return LineComment;
            return Error;

        case LineComment:
            if(edge != '\n' && edge != EOF) return LineComment;
            if(edge == '\n') return Start;
            return Error;

        case BlockComment:
            if(edge == '*') return ExpectEndBlockComment;
            return Error;

        case ExpectEndBlockComment:
            if(edge != '/' && edge != EOF) return BlockComment;
            if(edge == '/') return Start;
            return Error;

        case Plus:
            if(isdigit(edge)) return Integer;
            return Error;

        case Minus:
            if(isdigit(edge)) return Integer;
            return Error;

        case Integer:
            if(isdigit(edge)) return Integer;
            if(edge == '.') return FloatIntDotInt;
            if(edge == 'e' || edge == 'E') return Exponent;
            return Error;

        case FloatIntDotInt:
            if(isdigit(edge)) return FloatIntDotInt;
            if(edge == 'e' || edge == 'E') return Exponent;
            return Error;

        case Exponent:
            if(edge == '+' || edge == '-') return PlusMinus;
            if(isdigit(edge)) return FloatIntAfterExponent;
            return Error;

        case PlusMinus:
            if(isdigit(edge)) return FloatIntAfterExponent;
            return Error;

        case FloatIntAfterExponent:
            if(isdigit(edge)) return FloatIntAfterExponent;
            return Error;

        case Concat:
            return Error;

        case Assign:
            if(edge == '=') return ExpectEqual;
            return Error;

        case ExpectEqual:
            if(edge == '=') return Equal;
            return Error;

        case Equal:
            return Error;

        case Gt:
            if(edge == '=') return Ge;
            return Error;

        case Ge:
            return Error;

        case Lt:
            if(edge == '=') return Le;
            if(edge == '?') return ExpectStartProlog1;
            return Error;

        case Le:
            return Error;

        case ExpectStartProlog1:
            if(edge == 'p') return ExpectStartProlog2;
            return Error;

        case ExpectStartProlog2:
            if(edge == 'h') return ExpectStartProlog3;
            return Error;

        case ExpectStartProlog3:
            if(edge == 'p') return ExpectStartProlog4;
            return Error;

        case ExpectStartProlog4:
            if(isspace(edge)) return StartPrologEnd;
            return Error;

        case StartPrologEnd:
            return Error;

        case Semicolon:
            return Error;

        case LeftBracket:
            return Error;

        case RightBracket:
            return Error;

        case LeftCurlyBracket:
            return Error;

        case RightCurlyBracket:
            return Error;

        case EnableNull:
            if(edge == '>') return EndPrologEnd1;
            if(edge >= 97 && edge <= 122) return Identifier; // after ? we expect indetifier type, which is always only lower-case 
            return Error;

        case EndPrologEnd1:
            if(edge == '\n') return EndPrologEnd2;
            return Error;

        case EndPrologEnd2:
            return Error;
        
        case Variable:
            if(isalpha(edge) || edge == '_') return Identifier;
            return Error;
        
        case Identifier:
            if(isalpha(edge) || isdigit(edge) || edge == '_') return Identifier;
            return Error;
        
        case Colon:
            return Error;
        
        case EndOfFile:
            return Error;
        
        case ExclamationMark:
            if(edge == '=') return ExpectNotEqual;
            return Error;
        
        case ExpectNotEqual:
            if(edge == '=') return NotEqual;
            return Error;
        
        case NotEqual:
            return Error;
        
        case String:
            if(edge > 31 && edge != '\\' && edge != '"') return String;
            if(edge == '\\') return EscapeSeq;
            if(edge == '"') return StringEnd;
            return Error;
        
        case EscapeSeq:
            if(edge > 31) return String;
            return Error;
        
        case StringEnd:
            return Error;

        case Error:
            printf("something bad here happened\n");
            exit(1);
    }
    return Error;
}

int main()
{
    printf("Hello World!\n");
    return 0;
}
