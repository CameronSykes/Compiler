// CS 4280
// P2 - Parser and Tree Builder
// Cameron Sykes

#include "parserIO.h"

#define LINE 0
#define COL 1

void parserError(char* errorName)
{
    printf("Parser Error: Line %i, Column %i\n", token.lineColumn[LINE], token.lineColumn[COL]);
    printf("Expected %s but got %s\n", errorName, token.tokenType);
    exit(1);
}