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
void SetConsoleTextForegroundColor(RGBA color){
    printf(CSI "38;2;%g;%g;%gm",color.r,color.g,color.b);
}
void SetConsoleTextBackgroundColor(RGBA color){
    printf(CSI "48;2;%g;%g;%gm",color.r,color.g,color.b);
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

<<<<<<< Updated upstream
void ConsoleLabGetWindow(){
    
}

void ConsoleLabCreateWindow(){

}
=======

void ConsoleLabDrawBuffer(clWindow* window, clBuffer* buffer){
    if(!window || !buffer || !buffer->currentBuffer) return;

    clSprite* sprite = buffer->currentBuffer;

    int width  = sprite->dimensions.x;
    int height = sprite->dimensions.y;

    CHAR_INFO* consoleBuf = malloc(sizeof(CHAR_INFO) * width * height);
    if(!consoleBuf) return;

    for(int i = 0; i < width * height; i++){
        clChar* ch = &sprite->buffer[i];

        // znak
        if(ch->character && ch->unicodeLen > 0){
            consoleBuf[i].Char.AsciiChar = ch->character[0]; // na razie tylko ASCII
        } else {
            consoleBuf[i].Char.AsciiChar = ' ';
        }

        // kolor (minimalnie, bo nie pokazałeś RGBA → WinAPI)
        consoleBuf[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    COORD bufferSize = { width, height };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT region = { 0, 0, width - 1, height - 1 };

    WriteConsoleOutputA(
        window->hOut,
        consoleBuf,
        bufferSize,
        bufferCoord,
        &region
    );

    free(consoleBuf);
}

void ResizeBuffer(clBuffer* buffer, int width, int height){
    for(int b = 0; b < 2; b++){
        clSprite* s = &buffer->buffers[b];

        // free stare
        if(s->buffer){
            for(int i = 0; i < s->dimensions.x * s->dimensions.y; i++){
                free(s->buffer[i].character);
            }
            free(s->buffer);
        }

        s->dimensions.x = width;
        s->dimensions.y = height;

        s->buffer = calloc(width * height, sizeof(clChar));
    }
}

void ConsoleLabHandleEvents(clWindow* window, clInput* input, clBuffer* buffer){
    if(!window || !input || !buffer) return;
    // === RESIZE ===
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(window->hOut, &csbi);

    int newWidth  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    int newHeight = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;


    printf("c");
    if(newWidth != buffer->currentBuffer->dimensions.w || newHeight != buffer->currentBuffer->dimensions.h){
        printf("d");

        buffer->currentBuffer->dimensions.x= newWidth;
        buffer->currentBuffer->dimensions.y = newHeight;

        free(buffer->currentBuffer);
        buffer->currentBuffer = calloc(newWidth * newHeight, sizeof(CHAR_INFO));

        // ustaw rozmiar bufora konsoli
        COORD size = { newWidth, newHeight };
        SetConsoleScreenBufferSize(window->hOut, size);

        SMALL_RECT rect = {0, 0, newWidth - 1, newHeight - 1};
        SetConsoleWindowInfo(window->hOut, TRUE, &rect);
    }
    printf("e");

    // === INPUT ===
    DWORD events = 0;
    GetNumberOfConsoleInputEvents(window->hIn, &events);
    printf("f");

    if(events == 0) return;

    INPUT_RECORD record;
    DWORD read;
    printf("g");

    for(DWORD i = 0; i < events; i++){
        ReadConsoleInput(window->hIn, &record, 1, &read);

        switch(record.EventType){

            case KEY_EVENT: {
                KEY_EVENT_RECORD key = record.Event.KeyEvent;

                int vk = key.wVirtualKeyCode;
                input->keys[vk] = key.bKeyDown;
            } break;

            case MOUSE_EVENT: {
                MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;

                input->mousePos.x = mouse.dwMousePosition.X;
                input->mousePos.y = mouse.dwMousePosition.Y;

                // opcjonalnie przyciski myszy
                input->keys[KEY_MOUSE_LEFT] = (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
                input->keys[KEY_MOUSE_RIGHT] = (mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;
            } break;
        }
    }
}
>>>>>>> Stashed changes
