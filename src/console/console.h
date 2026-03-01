#pragma once
#ifdef _WIN32
#include <windows.h>
#else
//shit dla unixa
#endif
#include "../flags.c"
#include "../vectors/vec_int.c"

#ifdef __cplusplus
extern "C" {
#endif

typedef vec2 Pos;
typedef vec2 size;
//typedef struct {
//     int x;
//     int y;
// } Pos;

typedef struct {
    int x;
    int y;
    int leftPressed;   // true tylko przez jedną klatkę po wciśnięciu
    int rightPressed;
    int leftHeld;      // true dopóki trzymasz
    int rightHeld;
} MouseInfo;

typedef struct {
    int key;
} KeyboardInfo;

// typedef struct {
    
// } ConsoleLabConsoleData;


typedef struct {
    #ifdef _WIN32
    HANDLE hOut;
    HANDLE hIn;
    #else

    #endif
    vec2 screenSize;
    MouseInfo mouse;
    // ConsoleLabConsoleData data;
    void (*Tick)();
} ConsoleLabConsoleAPI;

void ConsoleInit(ConsoleLabConsoleAPI* console);
vec2 ConsoleGetScreenSize(ConsoleLabConsoleAPI* console);
void ConsoleHandleEvents(ConsoleLabConsoleAPI* console);
void ConsoleLabConsoleTick();

void ConsoleLabConsoleInit(ConsoleLabConsoleAPI* Console){
    Console->Tick = ConsoleLabConsoleTick;
}

#ifdef _cplusplus
}
#endif

#ifdef _WIN32
#include "console_windows.c"
#else
#include "console_unix.c"
#endif