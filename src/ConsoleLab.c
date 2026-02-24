#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "vectors.c"
#include "console/console.h"
#include "flags.c"

typedef struct {
    ConsoleLabVectorAPI Vector;
    ConsoleLabConsoleAPI Console;
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

    ConsoleLab.Console.init(&ConsoleLab.Console);
}