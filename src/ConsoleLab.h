#pragma once
#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#define CL_VERSION_MAJOR 0
#define CL_VERSION_MINOR 0
#define CL_INCLUDE_IMPLEMENTATION


#include <stdlib.h>
#include <stdio.h>
#include <uchar.h>
#include "vec.c"
#include "helpers/stringHelper.c"
#include "helpers/virtualTerminalHelper.c"
// #include <stdbool.h>
#define ESC "\x1b"
#define CSI "\x1b["

#ifdef __cplusplus
extern "C" {
#endif


typedef struct clWindow clWindow;
typedef struct clContext clContext;
typedef struct clChar clChar;

typedef struct clString clString; 
#define NUMBER_OF_KEYS 128

enum CL_KEYS {
    // Mouse
    KEY_MOUSE_LEFT,
    KEY_MOUSE_RIGHT,
    KEY_MOUSE_MIDDLE,
    KEY_MOUSE_X1,
    KEY_MOUSE_X2,

    // Letters
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,
    KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
    KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
    KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

    // Numbers (top row)
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
    KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

    // Numpad
    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,
    KEY_NUMPAD_MULTIPLY,
    KEY_NUMPAD_ADD,
    KEY_NUMPAD_SUBTRACT,
    KEY_NUMPAD_DECIMAL,
    KEY_NUMPAD_DIVIDE,
    KEY_NUMPAD_ENTER,

    // Function keys
    KEY_F1,  KEY_F2,  KEY_F3,  KEY_F4,
    KEY_F5,  KEY_F6,  KEY_F7,  KEY_F8,
    KEY_F9,  KEY_F10, KEY_F11, KEY_F12,

    // Control keys
    KEY_ESC,
    KEY_TAB,
    KEY_CAPS_LOCK,
    KEY_SHIFT,
    KEY_CTRL,
    KEY_ALT,
    KEY_SPACE,
    KEY_ENTER,
    KEY_BACKSPACE,

    // Arrows
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,

    // Navigation
    KEY_INSERT,
    KEY_DELETE,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,

    // Symbols
    KEY_MINUS,
    KEY_EQUAL,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_GRAVE, // `

    // Locks
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,

    // System
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_LCTRL,
    KEY_RCTRL,
    KEY_LALT,
    KEY_RALT,
    KEY_LSUPER, // Windows / Cmd
    KEY_RSUPER,

    // Other
    KEY_MENU,

    KEY_MAX // sentinel
};

typedef struct clInput{
    vec2 mousePos;//vec2 is int
    bool keys[NUMBER_OF_KEYS];//every tick in handle events set to true only if it was clicked in this frame
    bool keysHeld[NUMBER_OF_KEYS];
    //handle mouse scrool too
} clInput;

typedef struct clSprite clSprite;
struct clSprite{
    vec2 position;
    vec2 dimensions;
    RGBA* backgroundColor;
    RGBA* foregroundColor;
    clChar* buffer;
};

typedef struct clBuffer{
    clSprite buffers[2];
    clSprite* currentBuffer;
} clBuffer;


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


<<<<<<< Updated upstream
typedef struct clString clString; 
typedef struct clConsoleSprite clConsoleSprite;
struct clConsoleSprite{
    vec2 position;
    vec2 dimensions;
    clChar* buffer;
};
=======
>>>>>>> Stashed changes

clContext* clGlobalContext;
clResult clGlobalResult;

// example for every function
// void (clContext* context, clResult* result);
// #define (...) (clGlobalContext, &clGlobalContext->result, __VA_ARGS__)
// #define (result, ...) (clGlobalContext, result, __VA_ARGS__)

void clInit(clContext* context, clResult* result);
#define clInit(...) clInit(clGlobalContext, &clGlobalContext->result, __VA_ARGS__)
// #define clInit(result, ...) clInit(clGlobalContext, result, __VA_ARGS__)
void clClose(clContext* context);
#define clClose() clClose(clGlobalContext)




void clCharInit(clChar* clCharacter,char character, RGBA backgroundColor, RGBA foregroundColor);

int clConsoleSpriteInit(clConsoleSprite* sprite);
void clConsoleSpriteFree(clConsoleSprite* sprite);
int clConsoleSpriteCreate(clConsoleSprite* sprite, float x, float y, float width, float height);
int clConsoleSpriteCreateVec(clConsoleSprite* sprite, vec2 position, vec2 dimensions);

#include "string.c"

#ifdef __cplusplus
}
#endif
#ifdef CL_INCLUDE_IMPLEMENTATION
#include "ConsoleLab.c"
#endif
#endif