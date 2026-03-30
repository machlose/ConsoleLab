#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.c"
#include "string.c"

typedef charType charType;
enum charType{
    space,
    number,
    identifier,
    operator,
    unknown,
};

bool isopertor(unsigned char c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '!' || c == '~' || c == '^' || c == '|' || c == '&' || c == '\\' || c == ';' || c == ':' || c == '\'' || c == '\"' || c == '`' || c == '.' || c == ',' || c == '?' || c == '@' || c == '#' || c == '$'; 
}

void tokenize(char* code_buffer, size_t size){
    String buffer = getString();
    TokenArray tokens;
    size_t i = 0;
    unsigned char c = '/0';
    charType last = space; 
    bool isIdentifier = false;
    do {
        c = i++[code_buffer];
        if(isspace(c)){
            if(last == operator || last == identifier || last == number){
                appendToken(&tokens, createToken(buffer.value, last == operator ? operator : isIdentifier ? identifier : number));
                isIdentifier = false;
                clearString(&buffer);
            }
            last = space; 
        } else if(isalpha(c) || c== '_'){
            if(last == operator){
                appendToken(&tokens, createToken(buffer.value, operator));
                isIdentifier = false;
                clearString(&buffer);
            }
            appendChar(buffer.value, c);
            isIdentifier = true;
            last = identifier; 
        } else if(isdigit(c)){
            if(last == operator){
                appendToken(&tokens, createToken(&buffer, operator));
                isIdentifier = false;
                clearString(&buffer);
            }
            appendChar(&buffer, c);
            last = number; 
        } else if(isopertor(c)){
            if(last == identifier || last == number){
                appendToken(&tokens, createToken(buffer.value, isIdentifier ? identifier : number));
                clearString(&buffer);
                isIdentifier = false;
            }
            appendChar(&buffer, c);
            last = operator; 
        } else{
            printf("unknown\n");
            last = unknown;
        }
    } while(c!='/0');
}