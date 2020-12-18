// CS 4280
// Cameron Sykes
// P4 - Code Generation

#include "codeGenerator.h"
#include "parser.h"

int main(int argc, char** argv)
{
    // Open the passed file
    getFile(argc, argv);

    // Initialize the size of the global symbol table
    projectSymbols.size = 0;

    // No tree is output so the return value doesn't need to be caught
    struct Node* root = parser();

    assemble(root);

    return 0;
}