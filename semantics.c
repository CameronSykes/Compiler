// CS 4280
// P3 - Static Semantics
// Cameron Sykes

#include "semantics.h"

// Put the identifier in the global table of identifiers
void insertSymbol(Token variable)
{
    // Create an array of Tokens one bigger than the current table's size so that the new Token can be put in
    Token* symbolTableCopy = (Token*)malloc(sizeof(Token) * (projectSymbols.size + 1));
    int i;

    // Check if the symbol is already in the table so repeats don't exist
    if(!symbolInTable(variable.tokenValue))
    {
        // Create a copy of the table, add the new member, and assign that temp table to the master table.
        // This eliminates any memory issues cutting down on SEGFAULTS
        for(i = 0; i < projectSymbols.size; i++)
        {
            symbolTableCopy[i] = projectSymbols.table[i];
        }

        symbolTableCopy[projectSymbols.size] = variable;
        projectSymbols.table = symbolTableCopy;
        projectSymbols.size++;
    }
    else
    {
        semanticsError("Variable already declared in this scope", variable);
    }
}

// Iterate through the table looking for the passed value and return the result of the search
int symbolInTable(char* variable)
{
    int i;

    for(i = 0; i < projectSymbols.size; i++)
    {
        if(strcmp(projectSymbols.table[i].tokenValue, variable) == 0)
        {
            return 1;
        }
    }

    return 0;
}