#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "tree.h"

int main(int argc, char** argv)
{
    char* fileName = (char*)malloc(100);
    const char* outputFileName;
    char* outNameCopy;
    char* pos;
    FILE* orderFile;
    
    errno = 0;
    struct Node* root = NULL;
    
    // With more than one argument there must be a file name passed from the command line
    if(argc > 1)
    {
        outputFileName = parseFileName(argv[1]); // outputFileName stores the base name
        fileName = argv[1]; // fileName holds the entire file name, which includes the extension
    }
    else
    {
        printf("Source: ");
        fgets(fileName, 100, stdin); // Gather keyboard input
    
        // Input from the keyboard MUST have a .sp2020 extension
        if(strstr(fileName, ".sp2020") != NULL)
        {
            outputFileName = parseFileName(fileName); // The appropriate extension exists so get the base name of the file
        }
        else
        {
            // The extension rule was violated so inform the user and exit the program
            printf("Improper format. Expected '.sp2020'\n");
            exit(0);
        }
    }
    
    // Remove the trailing newline character from the file name
    if ((pos = strchr(fileName, '\n')) != NULL)
        *pos = '\0';
    
    root = buildTree(fileName);
    
    // If for some reason the buildTree function finds a valid non-empty file and can't read anything this section is skipped
    // The reason is, if something is configured a little wonky the program doesn't output files, which requires traversing NULLs.
    if(root != NULL)
    {
        // outNameCopy holds outputFileName so the original variable isn't completely changed. outputFileName is also
        // constant so it can't be changed, thus the need for outNameCopy. outNameCopy is changed based on the traversal
        // type and each traversal type doesn't need its own variable to hold a name for its output file.
        
        outNameCopy = (char*)malloc(sizeof(outputFileName));
        strcpy(outNameCopy, outputFileName);
        strcat(outNameCopy, ".preorder"); // Preorder comes first so throw the expected extension on the name of the output file
        
        if (!(orderFile = fopen(outNameCopy, "a")))
        {
            // The file doesn't want to work. This is mostly to cover those crazy edge cases. I like to work on a better
            // safe than sorry methodology. This outputs the exact error so you can see and remedy those wonky edge cases
            // This policy is exercised in traversal case
            printf("File error #%i: %s\n", errno, strerror(errno));
            exit(0);
        }
        else
        {
            preOrder(root, 0, orderFile);
        }
        
        outNameCopy = (char*)malloc(sizeof(outputFileName));
        strcpy(outNameCopy, outputFileName);
        strcat(outNameCopy, ".inorder");
    
        if (!(orderFile = fopen(outNameCopy, "a")))
        {
            printf("File error #%i: %s\n", errno, strerror(errno));
            exit(0);
        }
        else
        {
            inOrder(root, 0, orderFile);
        }
    
    
        outNameCopy = (char*)malloc(sizeof(outputFileName));
        strcpy(outNameCopy, outputFileName);
        strcat(outNameCopy, ".postorder");
        if (!(orderFile = fopen(outNameCopy, "a")))
        {
            printf("File error #%i: %s\n", errno, strerror(errno));
            exit(0);
        }
        else
        {
            postOrder(root, 0, orderFile);
        }
    }
    
    printf("Done\n"); // Communication with the user is one of the most important parts of programming.
    return 0;
}