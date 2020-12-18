//
// Created by camjs on 5/12/2020.
//

#ifndef _CODEGENERATOR_H
#define _CODEGENERATOR_H

#include "node.h"
#include "scanner.h"
#include "scannerIO.h"
#include "generatorIO.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* strapnd(char*, char*, int);
void assemble(struct Node*);
void assembleBlock(struct Node*);
void assembleVars(struct Node*);
void assembleExpr(struct Node*, char*);
void assembleN(struct Node*, char*);
void assembleA(struct Node*, char*);
void assembleM(struct Node*, char*);
void assembleR(struct Node*, char*);
void assembleStats(struct Node*);
void assembleStat(struct Node*);
void assemble_mStat(struct Node*);
void assembleIn(struct Node*);
void assembleOut(struct Node*);
void assembleIffy(struct Node*);
void assembleLoop(struct Node*);
void assembleAssign(struct Node*);
void assembleGoto(struct Node*);
void assembleLabel(struct Node*);

#endif //_CODEGENERATOR_H
