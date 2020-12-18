#ifndef P0_TREE_H
#define P0_TREE_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "node.h"
#include "token.h"

// Parser Adaptation
void addToken(struct Node*, struct Node*);
void printBNFPreorder(struct Node*, int);
struct Node* newNode(Token);

#endif //P0_TREE_H