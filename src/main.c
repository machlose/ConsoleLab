#include "ConsoleLab.c"
#include "test.c"

int main() {
    ConsoleLabInit();
    color.red = 225;
    color.green = 134;
    color.blue = 227;
    // ConsoleLab.time.setTargetFPS(&ConsoleLab.time, 60);

    while(true){
        ConsoleGetScreenSize(&ConsoleLab.Console);
        ConsoleHandleEvents(&ConsoleLab.Console);
        ConsoleLab.Tick();
        
        // printf(ConsoleLab.time.delta);
        COORD newPos;
        newPos.X = 0;
        newPos.Y = 0;
        SetConsoleCursorPosition(ConsoleLab.Console.data.hOut,newPos);
        printf("fps: %d, %d", ConsoleLab.time.fps, ConsoleLab.time.delta);
        //Sleep(1000/400);
    }
    return 0;
}