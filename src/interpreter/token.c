#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "string.c"
#define BASE_TOKEN_ARRAY_SIZE 1028
#define MAX_TOKEN_VALUE 128

typedef enum{
    Token_Identifier,
    Token_Number,
    Token_Operator
} CLTokenType;

typedef struct{
    String value;
    size_t length;
    CLTokenType type;
    int weight;
} CL_Token;

typedef struct TokenArray{
    CL_Token* tokens;
    size_t size;
    size_t capacity;
} TokenArray;

TokenArray getTokenArray(size_t capacity){
    TokenArray arr = {0};
    arr.tokens = malloc(capacity * sizeof(CL_Token));
    if(!arr.tokens) return arr;
    arr.capacity = capacity;
    return arr;
}
#define getTokenArray() getTokenArray(BASE_TOKEN_ARRAY_SIZE)

int resizeTokenArray(TokenArray* arr, size_t new_capacity){
    CL_Token* newPtr = realloc(arr->tokens, new_capacity * sizeof(CL_Token));
    if(!newPtr) return 0;
    arr->tokens = newPtr;
    arr->capacity = new_capacity;
    if(arr->size >= new_capacity){
        arr->size = new_capacity;
    }
    return 1;
}

int appendToken(TokenArray* arr, CL_Token t){
    if(arr->size + 2 > arr->capacity){
        if(!resizeTokenArray(arr, arr->capacity * 2)) return 0;
    }
    arr->tokens[arr->size++] = t;
    return 1;
}

void clearTokenArray(TokenArray* arr){
    arr->size = 0;
}

void freeTokenArray(TokenArray* arr){
    free(arr->tokens);
    arr->tokens = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

CL_Token createToken(char* value, CLTokenType type){
    CL_Token token = {0};
    copyString(&token.value, value);
    printf("Token Created with value %s\n", token.value.value);
    return token;
}