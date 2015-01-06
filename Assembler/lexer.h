#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <string>
#include <cstdio>

enum {MOV=1, PUSH, LOAD, STORE, ADD, SUB, MUL, JMP, JZ, PRINT, AX, BX, CX, DX, EX, EOL, LABEL, COLON, INTEGER};

class Lexer
{
    FILE * f;
    
    int last;
    int readChar();
    
    public:
    Lexer(const std::string & fname);
    ~Lexer();
    
    int getToken();
    
    std::string lastIdentifier;
    int lastInteger;
    int lastToken;
};

#endif // LEXER_H_INCLUDED
