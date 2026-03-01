#include "ConsoleLab.c"
#include "test.c"

void mainLoop(int* run){
    if(ConsoleLab.Time.time > 5){
        *run = 0;
    }
}

int main() {
    ConsoleLab.Run(mainLoop);
    return 0;
}