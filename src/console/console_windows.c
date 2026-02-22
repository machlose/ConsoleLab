#pragma once
#ifdef _WIN32

#include "console.h"

void console_init(){
    ConsoleData.hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleData.hIn  = GetStdHandle(STD_INPUT_HANDLE);
    
    DWORD mode;
    GetConsoleMode(ConsoleData.hIn, &mode);
    mode |=  ENABLE_MOUSE_INPUT;
    mode |=  ENABLE_WINDOW_INPUT;
    SetConsoleMode(ConsoleData.hIn, mode);

    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(ConsoleData.hOut, &cci);

    ConsoleData.screenSize = console_getScreenSize();
}

vec2 console_getScreenSize() {
    vec2 output;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(ConsoleData.hOut, &csbi);

    output.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    output.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    return output;
}


#endif