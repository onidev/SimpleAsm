#include "lexer.h"
#include <cstdlib>

Lexer::Lexer(const std::string & fname)
{
    f = fopen(fname.c_str(), "r");
    if(!f)
    {
        fprintf(stderr, "An error occurred while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    
    last = readChar();
}

Lexer::~Lexer()
{
    fclose(f);
}

int Lexer::readChar()
{
    return fgetc(f);
}

int Lexer::getToken()
{
    // skip spaces
	while(last == ' ' || last == '\t')
	{
		last = readChar();
	}
	
	// skip comments
	if(last == ';')
	{
		do
		{
			last = readChar();
		}
		while(last != '\n' && last != EOF);
	}
	
	if(last == '\n')
	{
		last = readChar();
		return EOL;
	}
	
	if(last == EOF)
	{
		return EOF;
	}
	
	if(last == ':')
	{
        last = readChar();
        return COLON;
    }
	
	// read instructions
	if(isalpha(last))
	{
		lastIdentifier = (char)last;
        while( isalnum(last = readChar()) || last == '_' )
            lastIdentifier += last;
		
		if(lastIdentifier == "ax") return AX;
		if(lastIdentifier == "bx") return BX;
		if(lastIdentifier == "cx") return CX;
		if(lastIdentifier == "dx") return DX;
		if(lastIdentifier == "ex") return EX;
		
		if(lastIdentifier == "mov") return MOV;
		if(lastIdentifier == "push") return PUSH;
		if(lastIdentifier == "load") return LOAD;
		if(lastIdentifier == "store") return STORE;
		if(lastIdentifier == "add") return ADD;
		if(lastIdentifier == "sub") return SUB;
		if(lastIdentifier == "mul") return MUL;
		if(lastIdentifier == "jmp") return JMP;
		if(lastIdentifier == "jz") return JZ;
		if(lastIdentifier == "print") return PRINT;
		
		return LABEL;
	}
	
	if(isdigit(last))
	{
		lastInteger = last - '0';
            
        while(true)
        {
            if( (last = readChar()) == '_' )
                continue;
            
            if( isdigit(last) )
            {
                lastInteger *= 10;
                lastInteger += last - '0';
            }
            else
                break;
        }
        return INTEGER;
	}
	
	return EOF;
}
