#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "vectors.c"
#include "console/console.h"
#include "flags.c"
#include "time.c"

typedef enum {
    Windows,
    UNIX,
    MacOS,
    Linux,
    Other    
} OperatingSystem;

typedef struct {
    Time time; 
    OperatingSystem os;
    void* osContext;
} ConsoleLabContext;

ConsoleLabContext ConsoleLabCreateContext(){
    ConsoleLabContext context;
    TimeInit(&context.time);
    #ifdef _WIN32
    context.os = Windows;
    #else
    context.os = UNIX;
    #endif
    return context;
}

typedef struct {
    ConsoleLabContext context;
    ConsoleLabVectorAPI Vector;
    ConsoleLabConsoleAPI Console;
} ConsoleLabAPI;

ConsoleLabAPI ConsoleLab;

void ConsoleLabInit(){
    ConsoleLabVectorInit(&ConsoleLab.Vector);
    ConsoleLabConsoleInit(&ConsoleLab.Console);
    ConsoleLab.context = ConsoleLabCreateContext();

    ConsoleInit(&ConsoleLab.Console);
}