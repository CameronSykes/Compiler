#ifndef P1_SCANNERIO_H
#define P1_SCANNERIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "token.h"
#include "scanner.h"

void getFile(int, char**);
void printToken(Token);
void tokenError(int, Token);

// File stream
FILE* inStream;

// Assembly file stream
FILE* outStream;

#endif //P1_SCANNERIO_H
