#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1
//#define SHOW_ASM 1

#define NUM_REG 5
#define STACK_SIZE 1024
// ax, bx, cx, dx, ex
unsigned int regs[NUM_REG + 1];

unsigned int stack[STACK_SIZE] = {0};
int pstack = 0;

void push(int v)
{
	pstack++;
	stack[pstack] = v;
	
	#ifdef DEBUG
	if(pstack == STACK_SIZE)
	{
		printf("Stack overflow.\n");
		exit(EXIT_FAILURE);
	}
	#endif
}

int popv()
{
	#ifdef DEBUG
	if(pstack == 0)
	{
		printf("Stack is empty.\n");
		exit(EXIT_FAILURE);
	}
	#endif
	return stack[pstack--];
}

unsigned int * program;

int instrNum = 0;
int reg1     = 0;
int reg2     = 0;
int reg3     = 0;
int value    = 0;

void decode(int instr)
{
	instrNum = (instr & 0xFF000000) >> 24;
	reg1     = (instr & 0x00FF0000) >> 16;
	reg2     = (instr & 0x0000FF00) >> 8;
	reg3     = (instr & 0x000000FF);
}


int pc = 0;

void fetch(int * instr)
{
	*instr = program[pc  ];
	value = program[pc+1];
	pc += 2;
}


int running = 1;

void eval()
{
	enum {END=0, MOV, PUSH, LOAD, STORE, ADD, SUB, MUL, JMP, JZ, PRINT};
	
	// Ajouter vérifications en mode DEBUG
	#ifdef SHOW_ASM
	printf("%d: ", pc/2);
	#endif
	switch(instrNum)
	{
		case END:
			running = 0;
			break;
		
		case MOV:
			if(reg2 == 0)
			{
				regs[reg1] = value;
			}
			else
			{
				regs[reg1] = regs[reg2];
			}
			#ifdef SHOW_ASM
			printf("mov \n");
			#endif
			break;
		
		case PUSH:
			push(value);
			#ifdef SHOW_ASM
			printf("push %d\n", value);
			#endif
			break;
		
		case LOAD:
			push(regs[reg1]);
			#ifdef SHOW_ASM
			printf("load %d(%d)\n", reg1 ,regs[reg1]);
			#endif
			break;
		
		case STORE:
			regs[reg1] = popv();
			#ifdef SHOW_ASM
			printf("store %d(%d)\n", reg1, regs[reg1]);
			#endif
			break;
		
		case ADD:
		{
			int b = popv();
			int a = popv();
			push(a + b);
			#ifdef SHOW_ASM
			printf("add \n");
			#endif
			break;
		}
		
		case SUB:
		{
			int b = popv();
			int a = popv();
			push(a - b);
			#ifdef SHOW_ASM
			printf("sub \n");
			#endif
			break;
		}
		
		case MUL:
		{
			int b = popv();
			int a = popv();
			push(a * b);
			#ifdef SHOW_ASM
			printf("mul\n");
			#endif
			break;
		}
		
		case JMP:
			pc = value * 2;
			#ifdef SHOW_ASM
			printf("jmp %d\n", value);
			#endif
			break;
		
		case JZ:
		{
			int v = popv();
			if(v == 0)
			{
				pc = value * 2;
			}
			#ifdef SHOW_ASM
			printf("jz %d\n", v);
			#endif
			break;
		}
		
		case PRINT:
		{
			int v = popv();
			printf("%d\n", v);
			#ifdef SHOW_ASM
			printf("print \n");
			#endif
			break;
		}
		
		default:
			printf("Bad operation (%d).\n", instrNum);
			running = 0;
	}
}

void run()
{
	int instr;
	while(running)
	{
		fetch(&instr);
		decode(instr);
		eval();
	}
}

int main(int argc, char ** argv)
{
	if(argc == 2)
	{
		FILE * f = fopen(argv[1], "rb");
		if(!f)
		{
			fprintf(stderr, "An error occurred while opening the file.\n");
			exit(EXIT_FAILURE);
		}
		
		fseek(f, 0, SEEK_END);
		int len = ftell(f);
		fseek(f, 0, SEEK_SET);
		program = malloc(len);
		fread(program, 1, len, f);
		fclose(f);
		
		run();
	}
	else
	{
		printf("Usage: vm file\n");
	}
	return EXIT_SUCCESS;
}
