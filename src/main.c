#include "ConsoleLab.h"
#include <stdio.h>

int main(){
    clConsoleSprite sprite;
    
    clConsoleSpriteCreate(&sprite, 4, 4, 4, 5);
    return 0;
}





// #include "ConsoleLab.h"
// #include "Views/ChessBoard.c"
// #include <stdio.h>
// int main(){
//     clConsoleSprite sprite;
//     RenderChessBoard(&sprite);
//     return 0;
// }