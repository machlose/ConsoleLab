#pragma once
#ifdef _WIN32
#include "console_windows.c"
#else
#include "console_unix.c"
#endif

#include "flags.c"

#ifdef _cplusplus
extern "C" {
#endif

typedef vec2 ScreenSize;

typedef struct {
    HANDLE hOut;
    HANDLE hIn;
} ConsoleLabConsoleAPI;



#ifdef _cplusplus
}
#endif