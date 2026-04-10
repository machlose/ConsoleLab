#pragma once
#ifdef _WIN32

#include "console.h"
#include "../helpers/colors.c"
#include "../ConsoleLab.c"

//virtual terminal starts
#define ESC "\x1b"
#define CSI "\x1b["

rgba color;

void ConsoleInit(ConsoleLabConsoleAPI* console){
    console->hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    console->hIn  = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(console->hIn, &mode);
    mode |=  ENABLE_MOUSE_INPUT;
    mode |=  ENABLE_WINDOW_INPUT;
    mode |=  ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(console->hIn, mode);

    DWORD modeOut;
    GetConsoleMode(console->hOut, &modeOut);
    modeOut |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(console->hOut, modeOut);

    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(console->hOut, &cci);
    console->screenSize = ConsoleGetScreenSize(console);
}

vec2 ConsoleGetScreenSize(ConsoleLabConsoleAPI* console) {
    vec2 output;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console->hOut, &csbi);

    output.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    output.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    return output;
}

void GetMouseInfo(ConsoleLabConsoleAPI* console, MouseInfo* mouseinfo){
    INPUT_RECORD rec;
    DWORD count;

    MouseInfo* mouse = mouseinfo;

    mouse->leftPressed = 0;
    mouse->rightPressed = 0;

    while (PeekConsoleInput(console->hIn, &rec, 1, &count) && count > 0) {
        ReadConsoleInput(console->hIn, &rec, 1, &count);

        if (rec.EventType != MOUSE_EVENT)
            continue;

        MOUSE_EVENT_RECORD* m = &rec.Event.MouseEvent;

        mouse->x = m->dwMousePosition.X;
        mouse->y = m->dwMousePosition.Y;

        int leftNow  = (m->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
        int rightNow = (m->dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;

        if (leftNow && !mouse->leftHeld)
            mouse->leftPressed = 1;

        if (rightNow && !mouse->rightHeld)
            mouse->rightPressed = 1;

        mouse->leftHeld = leftNow;
        mouse->rightHeld = rightNow;
    }
}

void ConsoleTick(){

}

// void ConsoleHandleEvents(ConsoleLabConsoleAPI* console, MouseInfo* mouse) {
//     INPUT_RECORD rec;
//     DWORD count;

//     mouse->leftPressed = 0;
//     mouse->rightPressed = 0;

//     while (PeekConsoleInput(console->hIn, &rec, 1, &count) && count > 0) {
//         ReadConsoleInput(console->hIn, &rec, 1, &count);

//         if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
//             console->screenSize = ConsoleGetScreenSize(console);
//         }

//         if (rec.EventType == MOUSE_EVENT) {
//             MOUSE_EVENT_RECORD* m = &rec.Event.MouseEvent;

//             mouse->x = m->dwMousePosition.X;
//             mouse->y = m->dwMousePosition.Y;

//             int leftNow  = (m->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
//             int rightNow = (m->dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;

//             if (leftNow && !mouse->leftHeld)
//                 mouse->leftPressed = 1;

//             if (rightNow && !mouse->rightHeld)
//                 mouse->rightPressed = 1;

//             mouse->leftHeld = leftNow;
//             mouse->rightHeld = rightNow;
//         }
//     }
// }

void ConsoleHandleEvents(ConsoleLabConsoleAPI* console) {
    INPUT_RECORD rec;
    DWORD count;

    while (PeekConsoleInput(console->hIn, &rec, 1, &count) && count > 0) {
        ReadConsoleInput(console->hIn, &rec, 1, &count);

        if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            console->screenSize = ConsoleGetScreenSize(console);
            COORD newPos;
            newPos.X = 0;
            newPos.Y = 0;
            SetConsoleCursorPosition(console->hOut,newPos);
            printf("w: %d, h: %d     \n", console->screenSize.w, console->screenSize.h);
        }
        if(rec.EventType == MOUSE_EVENT){
            AddHue(&color,1);
            COORD mousePos = rec.Event.MouseEvent.dwMousePosition;
            COORD newPos;
            newPos.X = 0;
            newPos.Y = 1;
            SetConsoleCursorPosition(console->hOut,mousePos);
            printf(ESC "(0"); // Enter Line drawing mode
            printf(CSI "38;2;%d;%d;%dm",color.red,color.green,color.blue); // bright yellow on bright blue
            printf_s("O"); // in line drawing mode, \x78 -> \u2502 "Vertical Bar"
            printf(CSI "0m"); // restore color
            printf(ESC "(B"); // exit line drawing mode
            SetConsoleCursorPosition(console->hOut,newPos);
            printf("%d, %d, %d          ",color.red,color.green,color.blue);
            newPos.Y = 2;
            SetConsoleCursorPosition(console->hOut,newPos);
            printf("x: %d, y: %d     ", mousePos.X, mousePos.Y);
        }
    }
}


#endif