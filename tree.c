#include "tree.h"

// Put token into BNF tree
void addToken(struct Node* parentNode, struct Node* childNode)
{
    int i = 0;
    int newSize = sizeof(struct Node) * (parentNode->size + 1);
    Token parentToken;
    parentToken.tokenType = "";
    parentToken.tokenValue = "";

    struct Node* tempParent = newNode(parentToken); // Create an empty Node

    // Create enough space for all the children in parentNode plus 1
    tempParent->children = (struct Node**) malloc(newSize);

    // Copy all of parentNode's children into the empty node
    for(i = 0; i < parentNode->size; i++)
    {
        *(tempParent->children + i) = *(parentNode->children + i);
    }

    // Put the newest token in the array of children and set parentNode's child array equal to that updated array
    tempParent->children[parentNode->size] = childNode;
    parentNode->children = tempParent->children;
    parentNode->size++;
}

void printBNFPreorder(struct Node* preNode, int level)
{
    // If the node is a leaf do nothing
    if(preNode->production.tokenValue != NULL)
    {
        int i;

        // Output the production
        printf("%*c%s\n", level * 2, ' ', preNode->production.tokenValue);

        // Cycle through all the children of the production until you reach a leaf
        for(i = 0; i < preNode->size; i++)
        {
            printBNFPreorder(preNode->children[i], level + 1);
        }
    }
    else
    {
        return;
    }
}

// Allocate memory for all of the data structures
struct Node* newNode(Token newToken)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    // Empty string
    node->production = newToken;
    node->size = 0;
    node->children = NULL;

    return node;
}