#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "token.c"
#include "string.c"

typedef charType charType;
enum charType{
    space,
    digit,
    letter,
    operator,
    unknown,
};

bool isopertor(unsigned char c){
    return '+' || '-' || '*' || '/' || '%' || '=' || '<' || '>' || '(' || ')' || '[' || ']' || '{' || '}' || '!' || '~' || '^' || '|' || '&' || '\\' || ';' || ':' || '\'' || '\"' || '`' || '.' || ',' || '?' || '@' || '#' || '$'; 
}

void tokenize(char* buffer, size_t size){
    String buffer = getString();
    TokenArray tokens;
    size_t i = 0;
    unsigned char c = '/0';
    charType last = space; 
    do {
        c = i++[buffer];
        if(isspace(c)){

            last = space; 
        } else if(isalpha(c)){

            last = letter; 
        } else if(isdigit(c)){

            last = digit; 
        } else if(isopertor(c)){

            last = operator; 
        } else{
            printf("unknown\n");
            last = unknown;
        }
    } while(c!='/0');
}