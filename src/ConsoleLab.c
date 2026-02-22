#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "vectors.c"
#include "console/console.h"
#include "flags.c"

typedef struct {
    ConsoleLabVectorAPI Vector;
    ConsoleLabConsoleAPI Console;
    // vec2 (*vec2)(int, int);
} ConsoleLabAPI;

ConsoleLabAPI ConsoleLab;

void ConsoleLabInit(){
    #ifndef ConsoleLabGlobalVector
    ConsoleLabVectorAPI Vector;
    #endif
    ConsoleLab.Vector = Vector;
    ConsoleLabVectorInit(&Vector);
    #ifndef ConsoleLabGlobalConsole
    ConsoleLabConsoleAPI Console;
    #endif
    ConsoleLab.Console = Console;
    ConsoleLabConsoleInit(&Console);

    printf("ConsoleLab init complete\n");
}