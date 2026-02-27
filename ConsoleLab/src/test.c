#pragma once
#include "console/console.h"
#include "ConsoleLab.c"

void test(){
    printf("w: %d, h: %d", ConsoleLab.Console.data.screenSize.w, ConsoleLab.Console.data.screenSize.h);
}
