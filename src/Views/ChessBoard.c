#include "../ConsoleLab.h"
#include <stdio.h>
#include <stdlib.h>

const char* chessPieceSprites = 
"                                                     »«         ─╡╞─     \n"
"                            ▄_           <>         (██)        (██)     \n"
"    (██)       [████]       ███\\        (██)        ▀██▀        ▀██▀     \n"
"     ▐▌         ▐██▌        ████)       /██\\         ██          ██      \n"
"     ██         ▐██▌        ▐██▌         ██          ██          ██      \n"
"   ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄    ";

char* RenderChessPiece(){

}
char* RenderChessBoard(){
    const int displayCellProportions = 2;
    const int cellSize = 6;
    const int gridSize = 8;

    printf(chessPieceSprites);
    char* board = malloc(10000);
    board[0] = '\0';
    board[9999] = '\0';

    printf("\n");
    for(int k = 0; k < gridSize;k++){
        for(int j = 0; j < cellSize;j++){
            for(int i = 0; i < gridSize;i++){
                if((i+k) % 2 == 0){
                    SetConsoleTextBackgroundColor((RGBA){.r=255,.g=255,.b=255});
                }
                else{
                    SetConsoleTextBackgroundColor((RGBA){.r=1,.g=1,.b=1});
                }
                for(int i = 0; i < cellSize*displayCellProportions;i++){
                    printf(" ");
                }
            }
            SetConsoleTextColorDefault();
            printf("\n");
        }
    }
    SetConsoleTextColorDefault();
    return board;
}