#include "ConsoleLab.c"
#include "test.c"

int main() {
    ConsoleLabInit();
    color.red = 225;
    color.green = 134;
    color.blue = 227;

    while(true){
        ConsoleGetScreenSize(&ConsoleLab.Console);
        ConsoleHandleEvents(&ConsoleLab.Console);
        Sleep(1000/400);
    }
    return 0;
}