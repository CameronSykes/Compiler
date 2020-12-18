#ifndef P0_TOKEN_H
#define P0_TOKEN_H

// Holds the type of token, the actual word that constitutes the token, and the location
struct Token
{
    char* tokenType;
    char* tokenValue;
    int lineColumn[2];
}typedef Token;

#endif //P0_TOKEN_H
