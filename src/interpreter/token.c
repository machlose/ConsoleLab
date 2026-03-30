#include <stdlib.h>
#include <stdbool.h>
#define BASE_TOKEN_ARRAY_SIZE 1028
#define MAX_TOKEN_VALUE 128
typedef TokenType TokenType;
enum TokenType{
    identifier,
    operator
};

typedef Token Token;
struct Token{
    char* value;
    size_t length;
    TokenType type;
    int weight;
};

typedef TokenArray TokenArray;
struct TokenArray{
    Token* tokens;
    size_t size;
    size_t capacity;
};

TokenArray getTokenArray(size_t capacity){
    TokenArray arr = {0};
    arr = malloc(capacity*sizeof(Token));
    if(!arr) return;
    arr.capacity = capacity;
    return arr;
}
#define getTokenArray() getTokenArray(BASE_TOKEN_ARRAY_SIZE)