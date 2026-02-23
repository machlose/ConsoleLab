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
    ConsoleLabVectorInit(&Vector);
    ConsoleLab.Vector = Vector;
    #ifndef ConsoleLabGlobalConsole
    ConsoleLabConsoleAPI Console;
    #endif
    ConsoleLabConsoleInit(&Console);
    ConsoleLab.Console = Console;
    printf("ConsoleInit Started\n");
    ConsoleLab.Console.init();
    printf("ConsoleInit Ended\n");

    printf("ConsoleLab init complete\n");
}