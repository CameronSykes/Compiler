#include "scanner.h"

// Categorizes the character read from the stream
int charType(char tokenChar)
{
    // LOGIC    ~   <  >  ==
    // MATH     ~   +  -  *  /  %
    // DELIM    ~   (  )  ,  {  }  ;
    // MEM      ~   :=  .  [  ]  :

    // All the acceptable character types
    enum characterClass{tkWS, tkLETTER, tkDIGIT, tkLESS, tkGREATER, tkEQUAL, tkPLUS, tkMINUS, tkMULTIPLIER, \
                        tkDIVISOR, tkMODULO, tkLPAREN, tkRPAREN, tkCOMMA, tkLBRACKET, tkRBRACKET, tkSEMICOLON, \
                        tkDOT, tkLSQUARE, tkRSQUARE, tkCOLON, tkEOF};

    if(isspace(tokenChar) || tokenChar == '#')
    {
        return tkWS;
    }
    else if(isalpha(tokenChar))
    {
        return tkLETTER;
    }
    else if(isdigit(tokenChar))
    {
        return tkDIGIT;
    }
    else
    {
        switch(tokenChar)
        {
            case '<':
                return tkLESS;
            case '>':
                return tkGREATER;
            case '=':
                return tkEQUAL;
            case '+':
                return tkPLUS;
            case '-':
                return tkMINUS;
            case '*':
                return tkMULTIPLIER;
            case '/':
                return tkDIVISOR;
            case '%':
                return tkMODULO;
            case '(':
                return tkLPAREN;
            case ')':
                return tkRPAREN;
            case ',':
                return tkCOMMA;
            case '{':
                return tkLBRACKET;
            case '}':
                return tkRBRACKET;
            case ';':
                return tkSEMICOLON;
            case '.':
                return tkDOT;
            case '[':
                return tkLSQUARE;
            case ']':
                return tkRSQUARE;
            case ':':
                return tkCOLON;
            case -1:
                return tkEOF;
            default:
                break;
        }
    }

    printf("SCANNER ERROR: Unrecognized character, %c\n", tokenChar);
    exit(1);
}

// The character is used to create the token's value and update the column / line number
// Handles any unrecognized but non-error characters
void updateToken(char tokenChar, Token* tk)
{
    char* tkTemp;
    static int lineNumber = 1;
    static int columnNumber = 0;

    columnNumber++;

    switch(tokenChar)
    {
        // Carriage return characters don't do anything for the scanner
        case '\r':
            break;
        // A newline character increments the lineColumn member and assigns the column and line
        case '\n':
            tk->lineColumn[LINE] = lineNumber;
            tk->lineColumn[COL] = columnNumber - 1;
            lineNumber++;
            columnNumber = 0;
            break;
        // A tab is 3 columns
        case '\t':
            columnNumber += 3;
            break;
        case ' ':
            break;
        // A valid character updates the token
        default:
            tkTemp = (char*)malloc(strlen(tk->tokenValue) + 1);
            strcpy(tkTemp, tk->tokenValue);
            tkTemp[strlen(tk->tokenValue)] = tokenChar;
            tk->tokenValue = tkTemp;

            tk->lineColumn[LINE] = lineNumber;
            tk->lineColumn[COL] = columnNumber;
            break;
    }
}

Token keywordCheck(Token token)
{
    char* keywords[13] = {"label", "goto", "loop", "void", "declare", "return", "in", "out", "program", "iffy", "then", "assign", "data"};
    int pos;

    for(pos = 0; pos < 13; pos++)
    {
        if(strcmp(keywords[pos], token.tokenValue) == 0)
        {
            //Token is a keyword type token
            token.tokenType = keywords[pos];
            return token;
        }
    }

    // Token is an identifier type token
    token.tokenType = "identifier";
    return token;
}

Token scanner()
{
    // ATTN: When adding to the table edit:
    //  Enum in charType()
    //  if(nextState) in scanner()
    //  Error codes in tokenError()
    int FSA[24][22] = {
            {0,		1, 		2, 		3, 		4, 		5, 		6, 		7, 		8, 		9, 		10, 	11, 	12, 	13, 	14, 	15, 	16, 	17, 	18, 	19, 	20, 	22},
            {1001, 	1, 		1, 		1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001},
            {1002, 	-11, 	2, 		1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002},
            {1003, 	1003, 	1003, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1003, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1003},
            {1004, 	1004, 	1004, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1004, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1004},
            {-12, 	-12, 	-12, 	-12, 	-12, 	23, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1005},
            {1006, 	1006, 	1006, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1006, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1006},
            {1007, 	1007, 	1007, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1007, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1007},
            {1008, 	1008, 	1008, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1008, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1008},
            {1009, 	1009, 	1009, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1009, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1009},
            {1010, 	1010, 	1010, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1010, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	-13, 	1010},
            {1011, 	1011, 	1011, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1011, 	1011, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1011},
            {1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012},
            {1013, 	1013, 	1013, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1013, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1013},
            {1014, 	1014, 	1014, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1014, 	-14, 	-14, 	1014, 	1014, 	-14, 	-14, 	-14, 	-14, 	-14, 	1014},
            {1015, 	1015, 	1015, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	-14, 	-14, 	-14, 	-14, 	1015},
            {1016, 	1016, 	1016, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	-14, 	1016, 	-14, 	-14, 	-14, 	1016, 	-14, 	-14, 	-14, 	-14, 	-14, 	1016},
            {-15, 	1017, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1017, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1017},
            {1018, 	1018, 	1018, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1018, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1018, 	1018, 	-15, 	1018},
            {1019, 	-15, 	-15, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	-15, 	-15, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019},
            {1020, 	1020, 	-15, 	-15, 	-15, 	21, 	-15, 	-15, 	-15, 	-15, 	-15, 	1020, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1020},
            {1021, 	1021, 	1021, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1021, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	-15, 	1021},
            {1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022},
            {1023, 	1023, 	1023, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1023, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	-12, 	1023}
    };

    char streamCharacter = fgetc(inStream);

    int currState = 0;
    // nextState is calculated by the character read in the driver
    int nextState = FSA[currState][charType(streamCharacter)];

    Token token;
    static int poundCounter;

    token.tokenValue = (char*)malloc(1);

    // As long as the current state isn't an error loop through the program
    while(currState >= 0)
    {
        // #'s ASCII value is 35
        if(streamCharacter == 35)
        {
            poundCounter++;
        }

        // A comment starts with a # character and needs a second to end the comment
        // Even number of comment characters means the scanner is out of a comment
        if(poundCounter % 2 == 0)
        {
            // The state is valid so update the token with the character then grab a new one
            updateToken(streamCharacter, &token);
            streamCharacter = fgetc(inStream);

            currState = nextState;

            // nextState can be an error and since it passes its value to currState, currState is too an error
            if(currState >= 0)
            {
                // There is no error so use the lookahead feature
                nextState = FSA[currState][charType(streamCharacter)];
            }

            // Token end states are numbers in the 1000s
            if(nextState > 1000)
            {
                // Check which type of token type based on state
                switch(nextState)
                {
                    case 1001: // Keyword / Identifier
                        ungetc(streamCharacter, inStream);
                        return keywordCheck(token);
                    case 1002: // Integer
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "integer";
                        return token;
                    case 1003: // Less than '<'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "less";
                        return token;
                    case 1004: // Greater than '>'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "greater";
                        return token;
                    case 1005: // Equivalency '=='
                        updateToken(streamCharacter, &token);
                        token.tokenType = "equal";
                        return token;
                    case 1006: // Addition '+'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "add";
                        return token;
                    case 1007: // Subtraction '-'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "subt";
                        return token;
                    case 1008: // Multiplication '*'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "mult";
                        return token;
                    case 1009: // Division '/'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "div";
                        return token;
                    case 1010: // Modulo '%'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "mod";
                        return token;
                    case 1011: // Left parenthesis '('
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "lparens";
                        return token;
                    case 1012: // Right parenthesis ')'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "rparens";
                        return token;
                    case 1013: // Comma ','
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "comma";
                        return token;
                    case 1014: // Left bracket '{'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "lbracket";
                        return token;
                    case 1015: // Right bracket '}'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "rbracket";
                        return token;
                    case 1016: // Semi-colon ';'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "semicolon";
                        return token;
                    case 1017: // Dot '.'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "dot";
                        return token;
                    case 1018: // Left square bracket '['
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "lsquare";
                        return token;
                    case 1019: // Right square bracket ']'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "rsquare";
                        return token;
                    case 1020: // Colon ':'
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "colon";
                        return token;
                    case 1021: // Assignment ':='
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "assign";
                        return token;
                    case 1022: // EOF
                        ungetc(streamCharacter, inStream);
                        token.tokenType = "EOF";
                        return token;
                    default:
                        printf("Unrecognized token: %s\n", token.tokenValue);
                        exit(1);
                }
            }
        }
        else
        {
            // You're in a comment
            streamCharacter = fgetc(inStream);

            // 35 is the ASCII value for the # character
            while(streamCharacter != 35)
            {
                // The line number still needs to be incremented in the case of a multiline comment
                if (streamCharacter == '\n')
                {
                    // Only updates the line number
                    updateToken(streamCharacter, &token);
                }
                // The "character" that equates to EOF is -1
                else if(streamCharacter == -1)
                {
                    // Return the EOF token
                    updateToken(streamCharacter, &token);
                    token.tokenType = "EOF";
                    return token;
                }
                // Read characters from teh string until you reach '#'
                streamCharacter = fgetc(inStream);
            }
            // The last character grabbed was # so grab one more to evaluate the next token
            streamCharacter = fgetc(inStream);
            poundCounter++;
        }
    }

    // currState < 0 which is an error
    tokenError(currState, token);
    exit(1);
}