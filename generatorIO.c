// CS 4280
// Cameron Sykes
// P4 - Code Generation

#include "generatorIO.h"

void assemblerError(struct Node* root)
{
    char* errorValue;

    errorValue = root->production.tokenValue;
    printf("Error with %s\n", errorValue);

    zeroExit();
}

void zeroExit()
{
    int i;
    char* variableName;

    fprintf(outStream, "STOP\n");
    for(i = 0; i < projectSymbols.size; i++)
    {
        variableName = (char*)malloc(sizeof(projectSymbols.table[i].tokenValue) + 3);
        strcpy(variableName, projectSymbols.table[i].tokenValue);
        strcat(variableName, " 0\n");
        fprintf(outStream, "%s", variableName);
    }

    fclose(outStream);
    fclose(inStream);
}