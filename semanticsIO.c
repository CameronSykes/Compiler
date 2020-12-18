// CS 4280
// P3 - Static Semantics
// Cameron Sykes

#include "semanticsIO.h"

void semanticsError(char* error, Token variable)
{
    printf("Semantics Error: %s\n", error);
    printf("%s at Line %i, Column %i\n", variable.tokenValue, variable.lineColumn[0], variable.lineColumn[1]);
    exit(0);
}