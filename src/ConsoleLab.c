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

typedef struct {
    ConsoleLabVectorAPI Vector;
    ConsoleLabConsoleAPI Console;
    ConsoleLabFileAPI File;
    Time time;
    void (*Tick)();
    void (*Close)();
} ConsoleLabAPI;

ConsoleLabAPI ConsoleLab;

void ConsoleLabConsoleTick(){
    ConsoleGetScreenSize(&ConsoleLab.Console);
    printf("w %d, h %d\n", ConsoleLab.Console.data.screenSize.x, ConsoleLab.Console.data.screenSize.y);
}

void ConsoleLabTick(){
    // ConsoleLabConsoleTick();
    ConsoleLab.time.update(&ConsoleLab.time);
    FrameLimiter(&ConsoleLab.time);
}

void ConsoleLabColse(){
    // freeFileArray();
}

void ConsoleLabInit(){
    ConsoleLab.Tick = ConsoleLabTick;
    ConsoleLabVectorInit(&ConsoleLab.Vector);
    ConsoleLabConsoleInit(&ConsoleLab.Console);
    TimeInit(&ConsoleLab.time);

    ConsoleInit(&ConsoleLab.Console);
}