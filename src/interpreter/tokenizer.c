#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.c"
#include "string.c"

typedef enum {
    char_space,
    char_number,
    char_identifier,
    char_operator,
    char_unknown,
} cl_charType;

bool isopertor(unsigned char c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '!' || c == '~' || c == '^' || c == '|' || c == '&' || c == '\\' || c == ';' || c == ':' || c == '\'' || c == '\"' || c == '`' || c == '.' || c == ',' || c == '?' || c == '@' || c == '#' || c == '$'; 
}

void tokenize(char* code_buffer, size_t size){
    String buffer = getString();
    TokenArray tokens = getTokenArray();;
    CL_Token newToken;
    size_t i = 0;
    unsigned char c = i++[code_buffer];;
    cl_charType last = char_space; 
    bool isIdentifier = false;
    while(c!='\0') {
        if(isspace(c)){
            if(last == char_operator || last == char_identifier || last == char_number){
                newToken = createToken(buffer.value, last == char_operator ? Token_Operator : isIdentifier ? Token_Identifier : Token_Number);
                appendToken(&tokens, newToken);
                isIdentifier = false;
                clearString(&buffer);
            }
            last = char_space; 
        } else if(isalpha(c) || c== '_'){
            if(last == char_operator){
                newToken = createToken(buffer.value, Token_Operator);
                appendToken(&tokens, newToken);
                isIdentifier = false;
                clearString(&buffer);
            }
            appendChar(&buffer, c);
            isIdentifier = true;
            last = char_identifier; 
        } else if(isdigit(c)){
            if(last == char_operator){
                newToken = createToken(buffer.value, Token_Operator);
                appendToken(&tokens, newToken);
                isIdentifier = false;
                clearString(&buffer);
            }
            appendChar(&buffer, c);
            last = char_number; 
        } else if(isopertor(c)){
            if(last == char_identifier || last == char_number){
                newToken = createToken(buffer.value, isIdentifier ? Token_Identifier : Token_Number);
                appendToken(&tokens, newToken);
                clearString(&buffer);
                isIdentifier = false;
            }
            appendChar(&buffer, c);
            last = char_operator; 
        } else{
            printf("unknown\n");
            last = char_unknown;
        }
        c = i++[code_buffer];
    }
}