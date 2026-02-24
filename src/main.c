#include "ConsoleLab.c"
#include "test.c"

int main() {
    ConsoleLabInit();
    while(true){
        ConsoleGetScreenSize(&ConsoleLab.Console);
        ConsoleHandleEvents(&ConsoleLab.Console);
        // printf("w: %d, h: %d\n", ConsoleLab.Console.data.screenSize.w, ConsoleLab.Console.data.screenSize.h);
        Sleep(1000/60);
    }
    return 0;
}