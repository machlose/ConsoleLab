#pragma once
#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#define CL_VERSION_MAJOR 0
#define CL_VERSION_MINOR 0
#define CL_INCLUDE_IMPLEMENTATION


#include <stdlib.h>
#include <stdio.h>
#include "vec.c"
#include "helpers/stringHelper.c"
#include "helpers/virtualTerminalHelper.c"
// #include <stdbool.h>
#define ESC "\x1b"
#define CSI "\x1b["

#ifdef __cplusplus
extern "C" {
#endif
typedef enum clState{
    working
} clState;

typedef enum clResult{
    success,
    context_malloc_failure,
    failure
} clResult;

typedef enum clOS{
    windows,
    unix
} clOS;

typedef enum clMode{
    console_mode,
} clMode;

typedef struct clWindow clWindow;
typedef struct clContext clContext;
typedef struct clChar clChar;

typedef struct clString clString; 
typedef struct clConsoleSprite clConsoleSprite;
struct clConsoleSprite{
    vec2 position;
    vec2 dimensions;
    clChar* buffer;
};

clContext* clGlobalContext;
clResult clGlobalResult;

// example for every function
// void (clContext* context, clResult* result);
// #define (...) (clGlobalContext, &clGlobalContext->result, __VA_ARGS__)
// #define (result, ...) (clGlobalContext, result, __VA_ARGS__)

void clInit(clContext* context, clResult* result);
#define clInit(...) clInit(clGlobalContext, &clGlobalContext->result, __VA_ARGS__)
#define clInit(result, ...) clInit(clGlobalContext, result, __VA_ARGS__)
void clClose(clContext* context);
#define clClose() clClose(clGlobalContext)




void clCharInit(clChar* clCharacter,char character, RGBA backgroundColor, RGBA foregroundColor);

int clConsoleSpriteInit(clConsoleSprite* sprite);
void clConsoleSpriteFree(clConsoleSprite* sprite);
int clConsoleSpriteCreate(clConsoleSprite* sprite, float x, float y, float width, float height);
int clConsoleSpriteCreateVec(clConsoleSprite* sprite, vec2 position, vec2 dimensions);

#ifdef __cplusplus
}
#endif
#ifdef CL_INCLUDE_IMPLEMENTATION
#include "ConsoleLab.c"
#endif
#endif