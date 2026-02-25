#pragma once
#ifdef _WIN32

#include "console.h"
#include "../helpers/colors.c"


//virtual terminal starts
#define ESC "\x1b"
#define CSI "\x1b["

rgb color;

void ConsoleInit(ConsoleLabConsoleAPI* console){
    console->data.hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    console->data.hIn  = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(console->data.hIn, &mode);
    mode |=  ENABLE_MOUSE_INPUT;
    mode |=  ENABLE_WINDOW_INPUT;
    mode |=  ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(console->data.hIn, mode);

    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(console->data.hOut, &cci);
    console->data.screenSize = ConsoleGetScreenSize(console);
}

vec2 ConsoleGetScreenSize(ConsoleLabConsoleAPI* console) {
    vec2 output;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console->data.hOut, &csbi);

    output.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    output.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    return output;
}

void ConsoleHandleEvents(ConsoleLabConsoleAPI* console) {
    INPUT_RECORD rec;
    DWORD count;

    while (PeekConsoleInput(console->data.hIn, &rec, 1, &count) && count > 0) {
        ReadConsoleInput(console->data.hIn, &rec, 1, &count);

        if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            console->data.screenSize = ConsoleGetScreenSize(console);
            COORD newPos;
            newPos.X = 0;
            newPos.Y = 0;
            SetConsoleCursorPosition(console->data.hOut,newPos);
            printf("w: %d, h: %d     ", console->data.screenSize.w, console->data.screenSize.h);
        }
        if(rec.EventType == MOUSE_EVENT){
            AddHue(&color,1);
            COORD mousePos = rec.Event.MouseEvent.dwMousePosition;
            COORD newPos;
            newPos.X = 0;
            newPos.Y = 1;
            SetConsoleCursorPosition(console->data.hOut,mousePos);
            printf(ESC "(0"); // Enter Line drawing mode
            printf(CSI "38;2;%d;%d;%dm",color.red,color.green,color.blue); // bright yellow on bright blue
            printf_s("O"); // in line drawing mode, \x78 -> \u2502 "Vertical Bar"
            printf(CSI "0m"); // restore color
            printf(ESC "(B"); // exit line drawing mode
            SetConsoleCursorPosition(console->data.hOut,newPos);
            printf("%d, %d, %d          ",color.red,color.green,color.blue);
            newPos.Y = 2;
            SetConsoleCursorPosition(console->data.hOut,newPos);
            printf("x: %d, y: %d     ", mousePos.X, mousePos.Y);
        }
    }
}


#endif