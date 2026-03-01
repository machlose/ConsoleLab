#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "vectors.c"
#include "console/console.h"
#include "files/file.h"
#include "flags.c"
#include "time.c"

// typedef enum {
//     Windows,
//     UNIX,
//     MacOS,
//     Linux,
//     Other    
// } OperatingSystem;

// typedef struct {
//     Time time; 
//     OperatingSystem os;
//     void* osContext;
// } ConsoleLabContext;

// ConsoleLabContext ConsoleLabCreateContext(){
//     ConsoleLabContext context;
//     TimeInit(&context.time);
//     #ifdef _WIN32
//     context.os = Windows;
//     #else
//     context.os = UNIX;
//     #endif
//     return context;
// }
typedef struct ConsoleLabAPI ConsoleLabAPI;
typedef struct ConsoleLabAPI{
    ConsoleLabVectorAPI Vector;
    ConsoleLabConsoleAPI Console;
    ConsoleLabFileAPI File;
    fileArray files;
    clTime Time;
    MouseInfo Mouse;
    KeyboardInfo Keyboard;
    void (*Tick)();
    void (*Close)();
    void (*Run)(void (*mainLoop)(int* run));
} ConsoleLabAPI;

void ConsoleLabRun(void (*update)(int* run));
ConsoleLabAPI ConsoleLab = {
    .Run = ConsoleLabRun
};

void ConsoleLabConsoleTick(){
    ConsoleGetScreenSize(&ConsoleLab.Console);
    printf("x %d, y %d\n", ConsoleLab.Mouse.x, ConsoleLab.Mouse.y);
    printf("l %d/%d, r %d/%d\n", ConsoleLab.Mouse.leftPressed, ConsoleLab.Mouse.leftHeld, ConsoleLab.Mouse.rightPressed, ConsoleLab.Mouse.rightHeld);
}

void ConsoleLabTick(){
    ConsoleLabConsoleTick();
    GetMouseInfo(&ConsoleLab.Console, &ConsoleLab.Mouse);
    ConsoleLab.Time.update(&ConsoleLab.Time);
    FrameLimiter(&ConsoleLab.Time);
}

void ConsoleLabClose(){
    freeFileArray(&ConsoleLab.files);
}

void ConsoleLabInit();
void ConsoleLabRun(void (*update)(int* run)){
    ConsoleLabInit();
    int run = 1;
    while(run){
        update(&run);
        ConsoleLabTick();
    }
    ConsoleLabClose();
}

void ConsoleLabInit(){
    ConsoleLab.Tick = ConsoleLabTick;
    ConsoleLab.Close = ConsoleLabClose;
    ConsoleLab.Run = ConsoleLabRun;
    ConsoleLabVectorInit(&ConsoleLab.Vector);
    ConsoleLabConsoleInit(&ConsoleLab.Console);
    ConsoleLabFileInit(&ConsoleLab.File);
    TimeInit(&ConsoleLab.Time);

    ConsoleInit(&ConsoleLab.Console);
    // ConsoleLab.files = ConsoleLab.File.scan("./");
    ConsoleLab.files = ConsoleLab.File.scan("./textures");
    // ConsoleLab.File.printFiles(&ConsoleLab.files);
}