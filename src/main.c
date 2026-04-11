#include <stdio.h>
#include "ConsoleLab.h"

// int main(){
//     clConsoleSprite sprite;
    
//     clConsoleSpriteCreate(&sprite, 4, 4, 4, 5);
//     return 0;
// }





#include "Views/ChessBoard.c"
#include <time.h>

int main(){
    clock_t begin = clock();

    clConsoleSprite sprite;
    int len = RenderChessBoard(&sprite);
    char buffer[150000];
    RenderSpriteString(&sprite,buffer,len);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f.5",time_spent);
    return 0;
}