#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "vectors.c"
#include "console/console.h"
#include "flags.c"
#include "time.c"

typedef enum {
    Windows10,
    Windows11,
    MacOS,
    Linux,
    Other    
} OperatingSystem;

typedef struct {
    Time time; 
    OperatingSystem os;
    void* osContext;
} ConsoleLabContext;

ConsoleLabContext* clglobalContext;

ConsoleLabContext ConsoleLabCreateContext(){
    ConsoleLabContext context;
    TimeInit(&context.time);
    return context;
}

typedef struct {
    ConsoleLabVectorAPI Vector;
    ConsoleLabConsoleAPI Console;
} ConsoleLabAPI;

ConsoleLabAPI ConsoleLab;

void ConsoleLabInit(){
    ConsoleLabVectorInit(&ConsoleLab.Vector);
    ConsoleLabConsoleInit(&ConsoleLab.Console);


    ConsoleInit(&ConsoleLab.Console);
}