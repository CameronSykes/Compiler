// CS 4280
// P2 - Parser and Tree Builder
// Cameron Sykes

#ifndef P2_PRODUCTIONS_H
#define P2_PRODUCTIONS_H

#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "tree.h"
#include "semantics.h"
#include "semanticsIO.h"
#include "node.h"

bool tokenIs(char*);
struct Node* programProd(); // varsProd(), blockProd()
struct Node* varsProd(); // declare, identifier, assignProd(), integer, semicolon, varsProd() || empty
struct Node* exprProd(); // NProd(), subt, exprProd() || NProd()
struct Node* NProd(); // AProd(), div, NProd() || AProd(), mult, NProd() || AProd()
struct Node* AProd(); // MProd(), add, AProd() || MProd()
struct Node* MProd(); // mult, MProd() || RProd()
struct Node* RProd(); // lparens, exprProd(), rparens || identifier || integer
struct Node* blockProd(); // lbracket, varsProd(), statsProd(), rbracket
struct Node* statsProd(); // statProd(), mStatProd()
struct Node* mStatProd(); // empty || statProd(), mStatProd()
struct Node* statProd(); // inProd(), semicolon || outProd(), semicolon || blockProd() || iffyProd(), semicolon || loopProd(), semicolon || assignProd(), semicolon || gotoProd(), semicolon || labelProd(), semicolon
struct Node* inProd(); // in, identifier
struct Node* outProd(); // out, exprProd()
struct Node* iffyProd(); // iffy, lsquare, exprProd(), ROProd(), exprProd(), rsquare, then, statProd()
struct Node* loopProd(); // loop, lsquare, exprProd(), ROProd(), exprProd(), rsquare, statProd()
struct Node* assignProd(); // identifier, assign, exprProd()
struct Node* labelProd(); // label, identifier
struct Node* gotoProd(); // goto, identifier
struct Node* ROProd(); // less || less, less || greater || greater, greater || equal || less, greater

#endif //P2_PRODUCTIONS_H
