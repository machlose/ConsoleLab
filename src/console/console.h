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

typedef struct {
    int x;
    int y;
    int leftPressed;   // true tylko przez jedną klatkę po wciśnięciu
    int rightPressed;
    int leftHeld;      // true dopóki trzymasz
    int rightHeld;
} MouseInfo;

typedef struct {
    HANDLE hOut;
    HANDLE hIn;
    vec2 screenSize;
    MouseInfo mouse;
} ConsoleLabConsoleData;

ConsoleLabConsoleData ConsoleData;

void console_init();
vec2 console_getScreenSize();

typedef struct {
    ConsoleLabConsoleData data;
    void (*init)();
    vec2 (*getScreenSize)();
} ConsoleLabConsoleAPI;

void ConsoleLabConsoleInit(ConsoleLabConsoleAPI* Console){
    Console->data = ConsoleData;
    Console->init = console_init;
    Console->getScreenSize = console_getScreenSize;
}

#ifdef ConsoleLabGlobalConsole
ConsoleLabConsoleAPI Console;
#endif

#ifdef _cplusplus
}
#endif

#ifdef _WIN32
#include "console_windows.c"
#else
#include "console_unix.c"
#endif