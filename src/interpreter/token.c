#include <stdlib.h>
#include <stdbool.h>
#include "string.c"
#define BASE_TOKEN_ARRAY_SIZE 1028
#define MAX_TOKEN_VALUE 128

typedef TokenType TokenType;
enum TokenType{
    identifier,
    number,
    operator
};

typedef Token Token;
struct Token{
    String value;
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
    if(!arr) return arr;
    arr.capacity = capacity;
    return arr;
}
#define getTokenArray() getTokenArray(BASE_TOKEN_ARRAY_SIZE)

int resizeTokenArray(TokenArray* arr, size_t new_capacity){
    char* newPtr = realloc(arr->value, new_capacity);
    if(!newPtr) return 0;
    arr->value = newPtr;
    arr->capacity = new_capacity;
    if(arr->size >= new_capacity){
        arr->size = new_capacity;
    }
    return 1;
}

int appendToken(TokenArray* arr, char c){
    if(arr->size + 2 > arr->capacity){
        if(!resizeTokenArray(arr, arr->capacity * 2)) return 0;
    }
    arr->value[arr->size++] = c;
    return 1;
}

void clearTokenArray(TokenArray* arr){
    arr->size = 0;
}

void freeTokenArray(TokenArray* arr){
    free(arr->value);
    arr->value = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void createToken(char* value, TokenType type){
    Token token = {0};
    copyString(&token.value, value);
    printf("Token Created with value %s", token.value.value);
    
}