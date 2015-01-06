#include "parser.h"
#include "lexer.h"
#include <cstdlib>
#include <vector>

static bool isRegister(int v)
{
    return v >= AX && v <= EX;
}

static int registerValue(int v)
{
	return v - AX + 1;
}

struct Label
{
    std::string name;
    int pos;
    Label() : pos(-1) {}
    Label(std::string name, int pos) : name(name), pos(pos) {}
};

struct Opcode
{
    int instr;
    int value;
    Opcode() : instr(0), value(0) {}
    Opcode(int i, int v) : instr(i), value(v) {}
};

struct Labels
{
    std::vector<Label> labels;
    std::vector<Label> unknown;
    
    int findLabel(const std::string & str)
    {
        for(unsigned int i=0; i<labels.size(); ++i)
        {
            if(labels[i].name == str)
                return i;
        }
        return -1;
    }
};

void Parser::parseFile(const std::string & fname, const std::string & out)
{
    Lexer lex(fname);
    
    Labels labels;
    std::vector<Opcode> instructions;
    
	int instr=0, value=0;
	int cur;
	
	while((cur = lex.getToken()) != EOF)
	{
		if(cur == EOL)
			continue;
		
		if(cur == MOV)
		{
			instr = MOV << 24;
			cur = lex.getToken();
			if(isRegister(cur))
			{
				instr |= registerValue(cur) << 16;
			}
			else
			{
				exit(EXIT_FAILURE);
			}
			cur = lex.getToken();
			if(isRegister(cur))
			{
				instr |= registerValue(cur) << 8;
			}
			else if(cur == INTEGER)
			{
				value = lex.lastInteger;
			}
			else
			{
                fprintf(stderr, "Integer expected.\n");
                exit(EXIT_FAILURE);
			}
		}
		else if(cur == PUSH)
		{
            instr = PUSH << 24;
            if(lex.getToken() == INTEGER)
            {
                value = lex.lastInteger;
            }
            else
			{
                fprintf(stderr, "Integer expected.\n");
                exit(EXIT_FAILURE);
			}
		}
		else if(cur == LOAD || cur == STORE)
		{
            instr = cur << 24;
            value = 0;
            cur = lex.getToken();
            if(isRegister(cur))
			{
				instr |= registerValue(cur) << 16;
			}
			else
			{
                fprintf(stderr, "Register expected.\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(cur == ADD || cur == SUB || cur == MUL || cur == PRINT)
		{
            instr = cur << 24;
            value = 0;
		}
		else if(cur == JZ || cur == JMP)
		{
            instr = cur << 24;
            if(lex.getToken() == LABEL)
            {
                int p = labels.findLabel(lex.lastIdentifier);
                if(p != -1)
                {
                    value = labels.labels[p].pos;
                }
                else
                {
                    value = 0xFFFFFFFF;
                    labels.unknown.push_back(Label(lex.lastIdentifier, instructions.size()));
                }
            }
            else
            {
                fprintf(stderr, "Label expected.\n");
                exit(EXIT_FAILURE);
            }
		}
		else if(cur == LABEL)
		{
            labels.labels.push_back(Label(lex.lastIdentifier, instructions.size()));
            if(lex.getToken() != COLON)
            {
                fprintf(stderr, "':' expected.\n");
                exit(EXIT_FAILURE);
            }
		}
		
		if(cur != LABEL)
            instructions.push_back(Opcode(instr, value));
	}
	
	instructions.push_back(Opcode(0, 0)); // the end
	
	// Pour chaque instruction jump ou le label n'étais pas encore connu, on lui assigne le bon label
	for(unsigned int i=0; i<labels.unknown.size(); ++i)
	{
        const Label & label = labels.unknown[i];
        int p = labels.findLabel(label.name);
        
        if(p != -1)
        {
            instructions[label.pos].value = labels.labels[p].pos;
        }
        else
        {
            fprintf(stderr, "Label %s not found\n", label.name.c_str());
            exit(EXIT_FAILURE);
        }
	}
	
	
	FILE * f = fopen(out.c_str(), "wb");
	for(unsigned int i=0; i<instructions.size(); ++i)
	{
        fwrite(&instructions[i].instr, sizeof(int), 1, f);
        fwrite(&instructions[i].value, sizeof(int), 1, f);
        //printf("%d 0x%8.8X 0x%8.8X\n", i, instructions[i].instr, instructions[i].value); 
	}
	fclose(f);
}
