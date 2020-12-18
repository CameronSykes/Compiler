// CS 4280
// P2 - Parser and Tree Builder
// Cameron Sykes

#ifndef P2_PARSER_H
#define P2_PARSER_H

#include "parser.h"
#include <string.h>
#include "scanner.h"
#include "productions.h"
#include "parserIO.h"
#include "token.h"

Token token;
struct Node* parser(); // Returns a Tree

#endif //P2_PARSER_H
