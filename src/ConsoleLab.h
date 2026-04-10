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

clContext* clGlobalContext;

// example for every function
// void cl(clContext* context, clResult* result);
// #define cl(...) cl(clGlobalContext, &clGlobalContext->result, __VA_ARGS__)
// #define cl(result, ...) cl(clGlobalContext, result, __VA_ARGS__)

void clInit(clContext* context, clResult* result);
void clClose(clContext* context, clResult* result);




void clConsoleSpriteInit(clConsoleSprite* sprite);
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