// CS 4280
// Cameron Sykes
// P4 - Code Generation

#include "codeGenerator.h"

char* strapnd(char* original, char* addition, int newLine)
{
    int commandLength;
    char* commandOutput;

    commandLength = strlen(addition) + strlen(original) + 2;
    commandOutput = (char*)malloc(commandLength);
    strcpy(commandOutput, original);
    strcat(commandOutput, " ");
    strcat(commandOutput, addition);

    if(newLine)
    {
        strcat(commandOutput, "\n");
    }

    return commandOutput;
}

void assemble(struct Node* root)
{
    int i;

    for(i = 0; i < root->size; i++)
    {
        struct Node* child = root->children[i];
        Token childProduction = child->production;

        if(strcmp(childProduction.tokenValue, "<vars>") == 0)
        {
            assembleVars(child);
        }
        else if(strcmp(childProduction.tokenValue, "<block>") == 0)
        {
            assembleBlock(child);
        }
        else
        {
            assemblerError(root);
        }
    }
    zeroExit();
}

void assembleBlock(struct Node* blockNode)
{
    int i;

    for(i = 0; i < blockNode->size; i++)
    {
        struct Node* child = blockNode->children[i];
        Token childProduction = child->production;

        if(strcmp(childProduction.tokenValue, "<vars>") == 0)
        {
            assembleVars(child);
        }
        else if(strcmp(childProduction.tokenValue, "<stats>") == 0)
        {
            assembleStats(child);
        }
        else
        {
            assemblerError(child);
        }
    }
}

void assembleVars(struct Node* varsNode)
{
    int i;
    char* id;
    char* varsCommand = "";

    for(i = 0; i < varsNode->size; i++)
    {
        struct Node* varsChild  = varsNode->children[i];
        Token varsProduction = varsChild->production;
        if(strcmp(varsProduction.tokenType, "identifier") == 0)
        {
            id = varsProduction.tokenValue;
        }
        else if(strcmp(varsProduction.tokenType, "integer") == 0)
        {
            varsCommand = strapnd("LOAD", varsProduction.tokenValue, 1);
            varsCommand = strapnd(varsCommand, "STORE", 0);
            varsCommand = strapnd(varsCommand, id, 1);
        }
    }

    fprintf(outStream, "%s", varsCommand);
}

void assembleStats(struct Node* statsNode)
{
    int i;

    for(i = 0; i < statsNode->size; i++)
    {
        struct Node* statsChild = statsNode->children[i];
        Token statsProduction = statsChild->production;

        if(strcmp(statsProduction.tokenValue, "<stat>") == 0)
        {
            assembleStat(statsChild);
        }
        else if(strcmp(statsProduction.tokenValue, "<mStat>") == 0)
        {
            assemble_mStat(statsChild);
        }
    }
}

void assembleStat(struct Node* statNode)
{
    int i;

    for(i = 0; i < statNode->size; i++)
    {
        struct Node* statChild = statNode->children[i];
        Token statProduction = statChild->production;

        if(strcmp(statProduction.tokenValue, "<in>") == 0)
        {
            assembleIn(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<out>") == 0)
        {
            assembleOut(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<block>") == 0)
        {
            assembleBlock(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<iffy>") == 0)
        {
            assembleIffy(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<loop>") == 0)
        {
            assembleLoop(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<assign>") == 0)
        {
            assembleAssign(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<goto>") == 0)
        {
            assembleGoto(statChild);
        }
        else if(strcmp(statProduction.tokenValue, "<label>") == 0)
        {
            assembleLabel(statChild);
        }
        else
        {
            assemblerError(statChild);
        }
    }
}

void assemble_mStat(struct Node* mStatNode)
{
    int i;

    for(i = 0; i < mStatNode->size; i++)
    {
        struct Node* mStatChild = mStatNode->children[i];
        Token mStatProduction = mStatChild->production;

        if(strcmp(mStatProduction.tokenValue, "<stat>") == 0)
        {
            assembleStat(mStatChild);
        }
        else if(strcmp(mStatProduction.tokenValue, "<mStat>") == 0)
        {
            assemble_mStat(mStatChild);
        }
    }
}
void assembleIn(struct Node* inNode)
{
    int i;
    char* inCommand;

    for(i = 0; i < inNode->size; i++)
    {
        struct Node* inChild = inNode->children[i];
        Token inProduction = inChild->production;

        if(strcmp(inProduction.tokenType, "identifier") == 0)
        {
            inCommand = strapnd("READ", inProduction.tokenValue, 1);
        }
    }

    fprintf(outStream, "%s", inCommand);
}

void assembleOut(struct Node* outNode)
{
    // Bare minimum:
    // LOAD X
    // WRITE X

    int i;
    char* outCommand = "WRITE";

    for(i = 0; i < outNode->size; i++)
    {
        struct Node* outChild = outNode->children[i];
        Token outProduction = outChild->production;

        if(strcmp(outProduction.tokenValue, "<expr>") == 0)
        {
            assembleExpr(outChild, outCommand);
        }
    }
}

void assembleIffy(struct Node* iffyNode)
{
    int i;

    for(i = 0; i < iffyNode->size; i++)
    {

    }
}

void assembleLoop(struct Node* loopNode)
{

}

void assembleAssign(struct Node* assignNode)
{
    int i;
    char* assignCommand;

    for(i = 0; i < assignNode->size; i++)
    {
        struct Node* assignChild = assignNode->children[i];
        Token assignProduction = assignChild->production;

        if(strcmp(assignProduction.tokenType, "identifier") == 0)
        {
            assignCommand = strapnd("STORE", assignProduction.tokenValue, 1);
        }
        else if(strcmp(assignProduction.tokenValue, "<expr>") == 0)
        {
            assembleExpr(assignChild, assignCommand);
        }
    }
}

void assembleGoto(struct Node* gotoNode)
{

}

void assembleLabel(struct Node* labelNode)
{

}

void assembleExpr(struct Node* exprNode, char* command)
{
    int i;
    char* subtractionCommand;

    for(i = 0; i < exprNode->size; i++)
    {
        struct Node* exprChild = exprNode->children[i];
        Token exprProduction = exprChild->production;

        assembleN(exprChild, command);

        if(strcmp(exprProduction.tokenType, "subt") == 0)
        {
            // ATTN: Variable is returned? Needs to be LOAD then the variable
            // Looks like:
            // LOAD X
            // SUB #
            // STORE X
//            subtractionCommand = strapnd(NCommand, "LOAD ")
            subtractionCommand = "SUB ";
            printf("%s", subtractionCommand);
        }
    }
}

void assembleN(struct Node* NNode, char* command)
{
    int i;

    for(i = 0; i < NNode->size; i++)
    {
        struct Node* NChild = NNode->children[i];
        Token NProduction = NChild->production;

        assembleA(NChild, command);
//        if(strcmp(NProduction.tokenValue, "") == 0)
    }
}

void assembleA(struct Node* ANode, char* command)
{
    int i;

    for(i = 0; i < ANode->size; i++)
    {
        struct Node* AChild = ANode->children[i];
        Token AProduction = AChild->production;

        assembleM(AChild, command);
    }
}

void assembleM(struct Node* MNode, char* command)
{
    int i;
    char* MCommand = "";

    for(i = 0; i < MNode->size; i++)
    {
        struct Node* MChild = MNode->children[i];
        Token MProduction = MChild->production;

        if(strcmp(MProduction.tokenType, "mult") == 0)
        {
            MCommand = strapnd("MULT -1\n", MCommand, 0);
        }
        else if(strcmp(MProduction.tokenValue, "<R>") == 0)
        {
            if(strstr(command, "MULT -1") != NULL)
            {
                MCommand = strapnd(MCommand, "STORE T0", 0);
                command = strapnd(MCommand, command, 1);
            }

            assembleR(MChild, command);
        }
        else if(strcmp(MProduction.tokenValue, "<M>") == 0)
        {
            command = strapnd(MCommand, command, 1);
            assembleM(MChild, command);
        }
    }
}

void assembleR(struct Node* RNode, char* command)
{
    int i;
    char* RCommand;
    Token temp0;
    temp0.tokenValue = "T0";
    temp0.tokenType = "identifier";

    for(i = 0; i < RNode->size; i++)
    {
        struct Node* RChild = RNode->children[i];
        Token RProduction = RChild->production;

        if(strcmp(RProduction.tokenValue, "<expr>") == 0)
        {
            assembleExpr(RChild, command);
        }
        else if(strcmp(RProduction.tokenType, "integer") == 0)
        {
            // insertSymbol(temp0);

            RCommand = strapnd("LOAD", RProduction.tokenValue, 1);
            command = strapnd(RCommand, command, 1);

//            command = strapnd(command, "T0", 1);

//            command = strapnd(command, RProduction.tokenValue, 1);
            fprintf(outStream, "%s", command);
        }
        else if(strcmp(RProduction.tokenType, "identifier") == 0)
        {
//            RCommand = strapnd("STORE", RProduction.tokenValue, 1);
            command = strapnd(command, RProduction.tokenValue, 1);
//            command = strapnd(RCommand, command, 1);
        fprintf(outStream, "%s", command);
        }
    }
}