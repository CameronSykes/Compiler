#ifndef P0_SCANNER_H
#define P0_SCANNER_H

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "scannerIO.h"
#include "scanner.h"

// Token struct's member lineColumn is an array with 2 members, line and column in that order
#define LINE 0
#define COL 1

// Creates a token
Token scanner();

// Updates the tokens value, line number, column number, and type
void updateToken(char, Token*);

// Classifies the character from the stream
int charType(char);

// Checks the identifier against keywords
Token keywordCheck(Token);

#endif //P0_SCANNER_H