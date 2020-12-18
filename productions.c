// CS 4280
// P2 - Parser and Tree Builder
// Cameron Sykes

//ATTN: Check if the token matches a condition. If it does consume the token
// No match is an error or a return with no consumption

#include "productions.h"

// TODO: Comment

bool tokenIs(char* tokenType) { return strcmp(token.tokenType, tokenType) == 0; }

struct Node* programProd()
{
    Token programToken;
    programToken.tokenValue = "<program>";
    programToken.tokenType = "production";

    struct Node* programNode = newNode(programToken);

    // Call child production and save the return Node
    struct Node* varsNode = varsProd();

    // Add the node from the child production to the children array of the parentNode. This makes the tree
    addToken(programNode, varsNode);

    struct Node* blockNode = blockProd();
    addToken(programNode, blockNode);

    return programNode;
}

struct Node* blockProd()
{
    Token blockToken;
    blockToken.tokenValue = "<block>";
    blockToken.tokenType = "production";
    struct Node* blockNode = newNode(blockToken);

    if(tokenIs("lbracket"))
    {
        token = scanner();

        struct Node* varsNode = varsProd();
        addToken(blockNode, varsNode);

        struct Node* statsNode = statsProd();
        addToken(blockNode, statsNode);

        if(tokenIs("rbracket"))
        {
            token = scanner();
        }
        else
        {
            parserError("right bracket '}'");
        }
    }
    else
    {
        parserError("left bracket '{'");
    }
    return blockNode;
}

struct Node* varsProd()
{
    Token varsToken;
    varsToken.tokenValue = "<vars>";
    varsToken.tokenType = "production";
    struct Node* varsNode = newNode(varsToken);

    if(tokenIs("declare"))
    {
        struct Node* declareNode = newNode(token);
        addToken(varsNode, declareNode);
        token = scanner();

        if(tokenIs("identifier"))
        {
            struct Node* identifierNode = newNode(token);
            addToken(varsNode, identifierNode);

            // An identifier was declared so add the identifier token to the global array of identifiers
            insertSymbol(token);
            token = scanner();

            if(tokenIs("assign"))
            {
                struct Node* assignNode = newNode(token);
                addToken(varsNode, assignNode);
                token = scanner();

                if(tokenIs("integer"))
                {
                    struct Node* integerNode = newNode(token);
                    addToken(varsNode, integerNode);
                    token = scanner();

                    if(tokenIs("semicolon"))
                    {
                        token = scanner();

                        struct Node* varsNode2 = varsProd();

                        // ATTN: Necessary?
                        addToken(varsNode, varsNode2);
                    }
                    else
                    {
                        parserError("semicolon ';'");
                    }
                }
                else
                {
                    parserError("integer");
                }
            }
            else
            {
                parserError("assignment operator ':='");
            }
        }
        else
        {
            parserError("identifier");
        }
    }
    // No else statement because vars can produce an empty product

    return varsNode;
}

struct Node* exprProd()
{
    Token exprToken;
    exprToken.tokenValue = "<expr>";
    exprToken.tokenType = "production";

    struct Node* exprNode = newNode(exprToken);

    struct Node* NNode = NProd();
    addToken(exprNode, NNode);

    if(tokenIs("subt"))
    {
        struct Node* subtNode = newNode(token);
        addToken(exprNode, subtNode);
        token = scanner();

        struct Node* exprNode2 = exprProd();
        addToken(exprNode, exprNode2);
    }

    return exprNode;
}

struct Node* NProd()
{
    Token NToken;
    NToken.tokenValue = "<N>";
    NToken.tokenType = "production";

    struct Node* NNode = newNode(NToken);

    struct Node* ANode = AProd();
    addToken(NNode, ANode);

    if(tokenIs("div"))
    {
        struct Node* divNode = newNode(token);
        addToken(NNode, divNode);
        token = scanner();

        struct Node* NNode2 = NProd();
        addToken(NNode, NNode2);
    }
    else if(tokenIs("mult"))
    {
        struct Node* multNode = newNode(token);
        addToken(NNode, multNode);
        token = scanner();

        struct Node* NNode2 = NProd();
        addToken(NNode, NNode2);
    }

    return NNode;
}

struct Node* AProd()
{
    Token AToken;
    AToken.tokenValue = "<A>";
    AToken.tokenType = "production";

    struct Node* ANode = newNode(AToken);

    struct Node* MNode = MProd();
    addToken(ANode, MNode);

    if(tokenIs("add"))
    {
        struct Node* addNode = newNode(token);
        addToken(ANode, addNode);
        token = scanner();

        struct Node* ANode2 = AProd();
        addToken(ANode, ANode2);
    }

    return ANode;
}

struct Node* MProd()
{
    Token MToken;
    MToken.tokenValue = "<M>";
    MToken.tokenType = "production";

    struct Node* MNode = newNode(MToken);

    if(tokenIs("mult"))
    {
        struct Node* multNode = newNode(token);
        addToken(MNode, multNode);
        token = scanner();

        struct Node* MNode2 = MProd();
        addToken(MNode, MNode2);
    }
    else
    {
        struct Node* RNode = RProd();
        addToken(MNode, RNode);
    }

    return MNode;
}

struct Node* RProd()
{
    Token RToken;
    RToken.tokenValue = "<R>";
    RToken.tokenType = "production";

    struct Node* RNode = newNode(RToken);

    if(tokenIs("lparens"))
    {
        token = scanner();

        struct Node* exprNode = exprProd();
        addToken(RNode, exprNode);

        if(!tokenIs("rparens"))
        {
            parserError("right parenthesis ')'");
        }
    }
    else if(tokenIs("identifier"))
    {
        // An identifier is being used, make sure it exists first
        if(!symbolInTable(token.tokenValue))
        {
            // If the symbol is not in the table output an error
            semanticsError("Undeclared variable", token);
        }

        // semanticsError outputs an error and exits the program so if the program makes it to this line then the identifier exists in the table
        struct Node* identifierNode = newNode(token);
        addToken(RNode, identifierNode);
        token = scanner();
    }
    else if(tokenIs("integer"))
    {
        struct Node* integerNode = newNode(token);
        addToken(RNode, integerNode);
        token = scanner();
    }
    else
    {
        parserError("left parenthesis '(', identifier, or integer");
    }

    return RNode;
}

struct Node* statsProd()
{
    Token statsToken;
    statsToken.tokenValue = "<stats>";
    statsToken.tokenType = "production";

    struct Node* statsNode = newNode(statsToken);

    struct Node* statNode = statProd();
    addToken(statsNode, statNode);

    struct Node* mStatNode = mStatProd();
    addToken(statsNode, mStatNode);

    return statsNode;
}

struct Node* mStatProd()
{
    Token mStatToken;
    mStatToken.tokenValue = "<mStat>";
    mStatToken.tokenType = "production";

    struct Node* mStatNode = newNode(mStatToken);

    // ATTN: Special case of token consumption?
    if(!tokenIs("rbracket"))
    {
        struct Node* statNode = statProd();
        addToken(mStatNode, statNode);

        struct Node* mStatNode2 = mStatProd();
        addToken(mStatNode, mStatNode2);
    }

    return mStatNode;
}

struct Node* statProd()
{
    //ATTN: block() does not require a semicolon after it
    bool needsSemicolon = true;

    Token statToken;
    statToken.tokenValue = "<stat>";
    statToken.tokenType = "production";

    struct Node* statNode = newNode(statToken);

    if(tokenIs("in"))
    {
        token = scanner();

        struct Node* inNode = inProd();
        addToken(statNode, inNode);
    }
    else if(tokenIs("out"))
    {
        token = scanner();

        struct Node* outNode = outProd();
        addToken(statNode, outNode);
    }
    else if(tokenIs("iffy"))
    {
        token = scanner();

        struct Node* iffyNode = iffyProd();
        addToken(statNode, iffyNode);
    }
    else if(tokenIs("loop"))
    {
        token = scanner();

        struct Node* loopNode = loopProd();
        addToken(statNode, loopNode);
    }
    else if(tokenIs("identifier"))
    {
        // An identifier needs to be declared before use so reference the global table of identifiers
        if(!symbolInTable(token.tokenValue))
        {
            // Output an error and exit the program if the identifier is not in the table
            semanticsError("Undeclared variable", token);
        }

        struct Node* assignNode = assignProd();
        addToken(statNode, assignNode);
    }
    else if(tokenIs("goto"))
    {
        token = scanner();

        struct Node* gotoNode = gotoProd();
        addToken(statNode, gotoNode);
    }
    else if(tokenIs("label"))
    {
        token = scanner();

        struct Node* labelNode = labelProd();
        addToken(statNode, labelNode);
    }
    else if(tokenIs("lbracket"))
    {
        token = scanner();

        struct Node* blockNode = blockProd();
        addToken(statNode, blockNode);
        needsSemicolon = false;
    }
    else
    {
        parserError("in, out, iffy, loop, identifier, goto, label, or '{'");
    }

    if(needsSemicolon)
    {
        if(tokenIs("semicolon"))
        {
            token = scanner();
        }
        else
        {
            parserError("semicolon ';'");
        }
    }

    return statNode;
}

struct Node* inProd()
{
    Token inProdToken;
    inProdToken.tokenValue = "<in>";
    inProdToken.tokenType = "production";

    struct Node* inProdNode = newNode(inProdToken);

    if(tokenIs("identifier"))
    {
        // Check for the identifier in the table before use
        if(!symbolInTable(token.tokenValue))
        {
            semanticsError("Undeclared variable", token);
        }

        struct Node* identifierNode = newNode(token);
        addToken(inProdNode, identifierNode);
        token = scanner();

        return inProdNode;
    }
    else
    {
        parserError("identifier");
    }
}

struct Node* outProd()
{
    Token outToken;
    outToken.tokenValue = "<out>";
    outToken.tokenType = "production";

    struct Node* outProdNode = newNode(outToken);

    struct Node* exprNode = exprProd();
    addToken(outProdNode, exprNode);

    return outProdNode;
}

struct Node* iffyProd()
{
    Token iffyToken;
    iffyToken.tokenValue = "<iffy>";
    iffyToken.tokenType = "production";

    struct Node* iffyProdNode = newNode(iffyToken);

    struct Node* iffyNode = newNode(token);
    addToken(iffyProdNode, iffyNode);

    if(tokenIs("lsquare"))
    {
        // TODO: Print when the tree traversal comes by but don't put into the tree
        token = scanner();

        struct Node* exprNode = exprProd();
        addToken(iffyProdNode, exprNode);

        struct Node* RONode = ROProd();
        addToken(iffyProdNode, RONode);

        struct Node* exprNode2 = exprProd();
        addToken(iffyProdNode, exprNode2);

        if(tokenIs("rsquare"))
        {
            token = scanner();

            if(tokenIs("then"))
            {
                struct Node* thenNode = newNode(token);
                addToken(iffyProdNode, thenNode);
                token = scanner();

                struct Node* statNode = statProd();
                addToken(iffyProdNode, statNode);
            }
            else
            {
                parserError("then");
            }
        }
        else
        {
            parserError("right square bracket ']'");
        }
    }
    else
    {
        parserError("left square bracket '['");
    }
    return iffyProdNode;
}

struct Node* loopProd()
{
    Token loopToken;
    loopToken.tokenValue = "<loop>";
    loopToken.tokenValue = "production";

    struct Node* loopProdNode = newNode(loopToken);

    if(tokenIs("lsquare"))
    {
        token = scanner();
        struct Node* exprNode = exprProd();
        addToken(loopProdNode, exprNode);

        struct Node* RONode = ROProd();
        addToken(loopProdNode, RONode);

        struct Node* exprNode2 = exprProd();
        addToken(loopProdNode, exprNode2);

        if(tokenIs("rsquare"))
        {
            token = scanner();

            struct Node* statNode = statProd();
            addToken(loopProdNode, statNode);
        }
        else
        {
            parserError("right square bracket ']'");
        }
    }
    else
    {
        parserError("left square bracket '['");
    }

    return loopProdNode;
}

struct Node* assignProd()
{
    Token assignToken;
    assignToken.tokenValue = "<assign>";
    assignToken.tokenType = "production";

    struct Node* assignNode = newNode(assignToken);

    if(tokenIs("identifier"))
    {
        // Check for existence of identifier
        if(!symbolInTable(token.tokenValue))
        {
            semanticsError("Undeclared variable", token);
        }

        struct Node* identifierNode = newNode(token);
        addToken(assignNode, identifierNode);
        token = scanner();

        if(tokenIs("assign"))
        {
            struct Node* assignmentNode = newNode(token);
            addToken(assignNode, assignmentNode);
            token = scanner();

            struct Node* exprNode = exprProd();
            addToken(assignNode, exprNode);
        }
        else
        {
            parserError("assignment operator ':='");
        }
    }
    else
    {
        parserError("identifier");
    }

    return assignNode;
}

struct Node* labelProd()
{
    Token labelToken;
    labelToken.tokenValue = "<label>";
    labelToken.tokenType = "production";

    struct Node* labelProdNode = newNode(labelToken);

    if(tokenIs("identifier"))
    {
        // Identifier must be in the table of symbols before it can be used which means it needs to be declared first
        if(!symbolInTable(token.tokenValue))
        {
            semanticsError("Undeclared variable", token);
        }

        struct Node* identifierNode = newNode(token);
        addToken(labelProdNode, identifierNode);
        token = scanner();

        return labelProdNode;
    }
    else
    {
        parserError("identifier");
    }
}

struct Node* gotoProd()
{
    Token gotoProdToken;
    gotoProdToken.tokenValue = "<goto>";
    gotoProdToken.tokenType = "production";

    struct Node* gotoProdNode = newNode(gotoProdToken);

    struct Node* gotoNode = newNode(token);
    addToken(gotoProdNode, gotoNode);

    if(tokenIs("identifier"))
    {
        // Check for identifier in table
        if(!symbolInTable(token.tokenValue))
        {
            semanticsError("Undeclared variable", token);
        }

        struct Node* identifierNode = newNode(token);
        addToken(gotoProdNode, identifierNode);
        token = scanner();

        return gotoProdNode;
    }
    else
    {
        parserError("identifier");
    }
}

struct Node* ROProd()
{
    Token ROToken;
    ROToken.tokenValue = "<RO>";
    ROToken.tokenType = "production";

    struct Node* RONode = newNode(ROToken);

    if(tokenIs("less"))
    {
        struct Node* lessNode = newNode(token);
        addToken(RONode, lessNode);
        token = scanner();

        if(tokenIs("less"))
        {
            struct Node* lesslessNode = newNode(token);
            addToken(RONode, lesslessNode);
            token = scanner();
        }
        else if(tokenIs("greater"))
        {
            struct Node* lessGreaterNode = newNode(token);
            addToken(RONode, lessGreaterNode);
            token = scanner();
        }
        return RONode;
    }
    else if(tokenIs("greater"))
    {
        struct Node* greaterNode = newNode(token);
        addToken(RONode, greaterNode);
        token = scanner();

        if(tokenIs("greater"))
        {
            struct Node* greatergreaterNode = newNode(token);
            addToken(RONode, greatergreaterNode);
            token = scanner();
        }
        return RONode;
    }
    else if(tokenIs("equal"))
    {
        struct Node* equalNode = newNode(token);
        addToken(RONode, equalNode);
        token = scanner();

        return RONode;
    }
    else
    {
        parserError("less than '<', greater than '>', or equal to '=='");
    }
}