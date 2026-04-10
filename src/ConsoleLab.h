#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#define CL_VERSION_MAJOR 0
#define CL_VERSION_MINOR 0


#include "vec.c"
#include <stdlib.h>
#include <stdio.h>
// #include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    working
} clState;

typedef enum {
    success,
    context_malloc_failure,
    failure
} clResult;

typedef enum {
    windows,
    unix
} clOS;

typedef enum {
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




int clConsoleSpriteInit(clConsoleSprite* sprite);
void clConsoleSpriteFree(clConsoleSprite* sprite);
void clConsoleSpriteCreate(clConsoleSprite* sprite, float x, float y, float width, float height);
void clConsoleSpriteCreateVec(clConsoleSprite* sprite, vec2 position, vec2 dimensions);

#ifdef __cplusplus
}
#endif
#ifdef CL_INCLUDE_IMPLEMENTATION
#include "ConsoleLab.c"
#endif
#endif