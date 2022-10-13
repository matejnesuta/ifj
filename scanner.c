#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

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
            printf("something bad here happened in transition\n");
            exit(1);
    }
    return Error;
}

Lexeme MakeLexeme(state final)
{
    switch(final)
    {
        case Multiply:
            return (Lexeme){.kind=MULTIPLY};
        case Division:
            return (Lexeme){.kind=DIVISION};
        case Minus:
            return (Lexeme){.kind=MINUS};
        case Plus:
            return (Lexeme){.kind=PLUS};
        case Integer:
            return (Lexeme){.kind=INTEGER};
        case FloatIntDotInt:
            return (Lexeme){.kind=FLOAT};
        case FloatIntAfterExponent:
            return (Lexeme){.kind=FLOAT};
        case Assign:
            return (Lexeme){.kind=ASSIGN};
        case Equal:
            return (Lexeme){.kind=EQUAL};
        case Gt:
            return (Lexeme){.kind=GT};
        case Ge:
            return (Lexeme){.kind=GE};
        case Lt:
            return (Lexeme){.kind=LT};
        case Le:
            return (Lexeme){.kind=LE};
        case StartPrologEnd:
            return (Lexeme){.kind=STARTPROLOG};
        case Semicolon:
            return (Lexeme){.kind=SEMICOLON};
        case LeftBracket:
            return (Lexeme){.kind=LEFTBRACKET};
        case RightBracket:
            return (Lexeme){.kind=RIGHTBRACKET};
        case LeftCurlyBracket:
            return (Lexeme){.kind=LEFTCURLYBRACKET};
        case RightCurlyBracket:
            return (Lexeme){.kind=RIGHTCURLYBRACKET};
        case EndPrologEnd1:
            return (Lexeme){.kind=ENDPROLOG};
        case EndPrologEnd2:
            return (Lexeme){.kind=ENDPROLOG};
        case Identifier:
            return (Lexeme){.kind=IDENTIFIER};
        case Colon:
            return (Lexeme){.kind=COLON};
        case EndOfFile:
            return (Lexeme){.kind=ENDOFFILE};
        case NotEqual:
            return (Lexeme){.kind=NOTEQUAL};
        case StringEnd:
            return (Lexeme){.kind=STRING};
        
        case Start:
        case BlockComment:
        case LineComment:
        case ExpectEndBlockComment:
        case Concat:
        case ExpectEqual:
        case ExpectStartProlog1:
        case ExpectStartProlog2:
        case ExpectStartProlog3:
        case ExpectStartProlog4:
        case Exponent:
        case PlusMinus:
        case EnableNull:
        case Variable:
        case ExclamationMark:
        case ExpectNotEqual:
        case String:
        case EscapeSeq:
        case Error:
            printf("something bad happened in MakeLexeme\n");
            exit(1);
    }
}

Lexeme GetLexeme()
{
    state currIn = Start;
    while(true)
    {
        int edge = getchar();
        if(edge == EOF)
        {
            if(currIn == Start)
                return (Lexeme){.kind=ENDOFFILE};
            return MakeLexeme(currIn);
        }
        state next = transition(currIn, edge);
        if(next == Error)
        {
            ungetc(edge, stdin);
            return MakeLexeme(currIn);
        }
        currIn = next;
    }
}

void PrintLexeme(Lexeme lexeme)
{
    switch(lexeme.kind)
    {
        case MULTIPLY:
             printf("%s", "MULTIPLY\n");
             break;
            
        case DIVISION:
             printf("%s", "DIVISION\n");
             break;
            
        case MINUS:
             printf("%s", "MINUS\n");
             break;
            
        case PLUS:
             printf("%s", "PLUS\n");
             break;
            
        case INTEGER:
             printf("%s", "INTEGER\n");
             break;
            
        case FLOAT:
             printf("%s", "FLOAT\n");
             break;
            
        case CONCAT:
             printf("%s", "CONCAT\n");
             break;
            
        case ASSIGN:
             printf("%s", "ASSIGN\n");
             break;
            
        case EQUAL:
             printf("%s", "EQUAL\n");
             break;
            
        case GT:
             printf("%s", "GT\n");
             break;
            
        case GE:
             printf("%s", "GE\n");
             break;
            
        case LT:
             printf("%s", "LT\n");
             break;
            
        case LE:
             printf("%s", "LE\n");
             break;
            
        case STARTPROLOG:
             printf("%s", "STARTPROLOG\n");
             break;
            
        case SEMICOLON:
             printf("%s", "SEMICOLON\n");
             break;
            
        case LEFTBRACKET:
             printf("%s", "LEFTBRACKET\n");
             break;
            
        case RIGHTBRACKET:
             printf("%s", "RIGHTBRACKET\n");
             break;
            
        case LEFTCURLYBRACKET:
             printf("%s", "LEFTCURLYBRACKET\n");
             break;
            
        case RIGHTCURLYBRACKET:
             printf("%s", "RIGHTCURLYBRACKET\n");
             break;
            
        case ENDPROLOG:
             printf("%s", "ENDPROLOG\n");
             break;
            
        case IDENTIFIER:
             printf("%s", "IDENTIFIER\n");
             break;
            
        case COLON:
             printf("%s", "COLON\n");
             break;
            
        case ENDOFFILE:
             printf("%s", "ENDOFFILE\n");
             break;
            
        case NOTEQUAL:
             printf("%s", "NOTEQUAL\n");
             break;
            
        case STRING:
             printf("%s", "STRING\n");
             break;
    }
}

int main()
{
    Lexeme l = {0};
    while(l.kind != ENDOFFILE)
    {
        l = GetLexeme();
        PrintLexeme(l);
    }

    return 0;
}
