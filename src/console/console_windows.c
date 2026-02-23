#pragma once
#ifdef _WIN32

#include "console.h"

void console_init(ConsoleLabConsoleAPI* console){
    console->data.hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    console->data.hIn  = GetStdHandle(STD_INPUT_HANDLE);
    printf("2");

    DWORD mode;
    GetConsoleMode(console->data.hIn, &mode);
    mode |=  ENABLE_MOUSE_INPUT;
    mode |=  ENABLE_WINDOW_INPUT;
    SetConsoleMode(console->data.hIn, mode);
    printf("3");

    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(console->data.hOut, &cci);
    printf("4");
    console->data.screenSize = console_getScreenSize(console);
    printf("5");
}

vec2 console_getScreenSize(ConsoleLabConsoleAPI* console) {
    vec2 output;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console->data.hOut, &csbi);

    output.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    output.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    return output;
}


#endif