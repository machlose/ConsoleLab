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
        
        COORD newPos;
        newPos.X = 0;
        newPos.Y = 0;
        SetConsoleCursorPosition(ConsoleLab.Console.data.hOut,newPos);
        printf("fps: %d, %d", ConsoleLab.time.fps, ConsoleLab.time.delta);
        double sleepTime = 1000.0/60.0-((double)(ConsoleLab.time.delta)*1000.0);
        sleepTime = sleepTime < 0 ? 0 : sleepTime;
        Sleep(sleepTime);
    }
    return 0;
}