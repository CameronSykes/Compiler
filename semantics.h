// CS 4280
// P3 - Static Semantics
// Cameron Sykes

#ifndef P3_SEMANTICS_H
#define P3_SEMANTICS_H

#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "semanticsIO.h"

// Working with pointers and trying to get the size of an array is difficult
// Making a struct that has its own size variable is much easier and straightforward
struct symbolTable
{
    Token* table;
    int size;
}typedef symbolTable;

symbolTable projectSymbols; // Global symbol table

void insertSymbol(Token);
int symbolInTable(char*);

#endif //P3_SEMANTICS_H
