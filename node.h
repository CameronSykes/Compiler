#ifndef P0_NODE_H
#define P0_NODE_H

#include "../P1/token.h"

// Node Struct
struct Node
{
    struct Node** children; // Array of children to hold as many products as possible
    Token production; // String value of actual product
    int size; // Number of children
};

// Used for previous version
struct wordNode
{
    struct wordNode* leftChild;
    struct wordNode* rightChild;
    char* key;
};

#endif //P0_NODE_H
