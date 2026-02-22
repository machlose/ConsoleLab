#pragma once
#include "console/console.h"
#include "ConsoleLab.c"

void test(){
    for(;;){
        printf("w: %d, h: %d", ConsoleLab.Console.data.screenSize.w, ConsoleLab.Console.data.screenSize.h);
        printf("w: %d, h: %d", Console.data.screenSize.w, Console.data.screenSize.h);
    }
}
