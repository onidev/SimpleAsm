#include "parser.h"
#include <cstdio>

int main(int argc, char ** argv)
{
    Parser parser;
    
    if(argc == 3)
    {
        parser.parseFile(argv[1], argv[2]);
    }
    else
    {
        printf("Usage: bca filein fileout\n");
    }
    return 0;
}
