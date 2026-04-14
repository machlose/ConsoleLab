#pragma once
#include <windows.h>
#include "ConsoleLab.h"

typedef struct clWindow{
    //windows
    HANDLE hOut;
    HANDLE hIn;
} clWindow;

typedef struct clContext{
    clOS os;
    clMode mode;
    clState state;
    clResult result;
    clWindow window;
    void* osContext;
} clContext;
clWindow window;

void changeEncoding(){
    system("chcp 65001");//freaky shit zmienia encoding znakow konsoli i czasem psuje wyswietlanie w tej vscodowej czemu coz ni chuj nie mam pojecia
};

void SetConsoleTextColorDefault(){
    printf(CSI "0m");
}
void SetConsoleTextForegroundColor(RGBA* color){
    printf(CSI "38;2;%g;%g;%gm",color->r,color->g,color->b);
}
void SetConsoleTextBackgroundColor(RGBA* color){
    printf(CSI "48;2;%g;%g;%gm",color->r,color->g,color->b);
}
COORD GetConsoleCursorPosition(clWindow window){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(window.hOut, &csbi)){
        printf("%d,%d",csbi.dwCursorPosition.X,csbi.dwCursorPosition.Y);
        return csbi.dwCursorPosition;
    }
    return (COORD){};
}

void ConsoleInit(){
    window.hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    window.hIn = GetStdHandle(STD_INPUT_HANDLE);
}