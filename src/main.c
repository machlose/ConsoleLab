#include <stdio.h>
#include "ConsoleLab.h"

// int main(){
//     clConsoleSprite sprite;
    
//     clConsoleSpriteCreate(&sprite, 4, 4, 4, 5);
//     return 0;
// }





#include "Views/ChessBoard.c"

int main(){
    clConsoleSprite sprite;
    int len = RenderChessBoard(&sprite);
    char buffer[150000];
    RenderSpriteString(&sprite,buffer,len);
    printf(buffer);
    return 0;
}