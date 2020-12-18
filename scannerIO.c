#include "scannerIO.h"

// Token struct's member lineColumn is an array with 2 members, line and column in that order
#define LINE 0
#define COL 1

void getFile(int numArgs, char** args)
{
    char* fileName = (char *) malloc(100);
    char* assemblyFile = (char *)malloc(100);
    errno = 0;

    // With more than one argument there must be a file name passed from the command line
    if (numArgs > 1)
    {
        fileName = args[1]; // fileName holds the entire file name, which includes the extension
        strcat(assemblyFile, "kb");
    }
    else
    {
        printf("Source: ");
        fgets(fileName, 100, stdin); // Gather keyboard input
        // Check extension
        if(strstr(fileName, ".sp2020") == NULL)
        {
            printf("Unsupported file extension\n");
            exit(1);
        }

        char* baseFileName = malloc(sizeof(fileName) + 1);
        char* extension = strchr(fileName, '.');
        int extensionStart = extension - fileName;
        strncpy(baseFileName, fileName, extensionStart);
        assemblyFile = (char*)malloc(sizeof(baseFileName) + sizeof(fileName) + 1);
        strcpy(assemblyFile, baseFileName);
    }

    // Remove new line character
    char* pos;
    if((pos = strchr(fileName, '\n')) != NULL)
    {
        *pos = '\0';
    }

    // If the file doesn't exist or can't be opened for some reason then the user is notified
    if(!(inStream = fopen(fileName, "r")))
    {
        printf("File error #%i: %s\n", errno, strerror(errno));
        exit(0);
    }

    strcat(assemblyFile, ".asm");
    outStream = fopen(assemblyFile, "w");
}

void printToken(Token tk)
{
    printf("<%s, \'%s\', [%i:%i]>\n", tk.tokenType, tk.tokenValue, tk.lineColumn[LINE], tk.lineColumn[COL]);
}

void tokenError(int errNum, Token errorToken)
{
    char* errorName;

    switch(errNum)
    {
        case -11:
            errorName = "Invalid integer or identifier syntax";
            break;
        case -12:
            errorName = "Incorrect logical operator syntax";
            break;
        case -13:
            errorName = "Incorrect mathematical operator syntax";
            break;
        case -14:
            errorName = "Incorrect delimiter operator syntax";
            break;
        case -15:
            errorName = "Incorrect memory operator syntax";
            break;
        default:
            printf("SCANNER ERROR: Unknown error code\n");
            exit(1);
    }

        printf("SCANNER ERROR: %s near %s at Column: %i and Line: %i\n", \
    errorName, errorToken.tokenValue, errorToken.lineColumn[LINE], errorToken.lineColumn[COL]);
    
    exit(1);
}