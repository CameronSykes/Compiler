// CS 4280
// P2 - Parser and Tree Builder
// Cameron Sykes

#include "parser.h"

struct Node* parser()
{
    struct Node* parseTree; // Holds completed tree

    token = scanner(); // Lookahead
    parseTree = programProd(); // programProd() returns the completed tree, its children and their children are attached
    printf("\n");

    if(strcmp(token.tokenType, "EOF") != 0)
    {
        parserError("EOF"); // Check for EOF and raise error if none
    }

    return parseTree; // Return tree
}