#include <stdio.h>
#include "ConsoleLab.h"

// int main(){
//     clConsoleSprite sprite;
    
//     clConsoleSpriteCreate(&sprite, 4, 4, 4, 5);
//     return 0;
// }





#include "Views/ChessBoard.c"
#include <time.h>
#include <uchar.h>

int main(){
    clock_t begin = clock();


    char buffer[10000];
    clConsoleSprite test;
    clConsoleSpriteCreate(&test,0,0,6,6);
    GetSpriteFromCharSpriteMap(&test,chessPieceSprites,0,0,6,6,6,73);
    RenderSpriteString(&test,buffer,6*6);
    printf(chessPieceSprites);

    clConsoleSprite sprite;
    int len = RenderChessBoard(&sprite);
    char buffer[150000];
    RenderSpriteString(&sprite,buffer,len);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f.5",time_spent);
    return 0;
}