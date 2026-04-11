#include "../ConsoleLab.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char* chessPieceSprites = 
"                                                     »«         ─╡╞─     \n"
"                            ▄_           <>         (██)        (██)     \n"
"    (██)       [████]       ███\\        (██)        ▀██▀        ▀██▀     \n"
"     ▐▌         ▐██▌        ████)       /██\\         ██          ██      \n"
"     ██         ▐██▌        ▐██▌         ██          ██          ██      \n"
"   ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄    ";

char* RenderChessPiece(){

}
int RenderChessBoard(clConsoleSprite* sprite){
    const int displayCellProportions = 2;
    const int cellSize = 6;
    const int gridSize = 8;

    int arrayPosition = 0;
    clConsoleSpriteCreate(sprite,0,0,displayCellProportions*cellSize*gridSize+1,cellSize*gridSize);
    const RGBA colors[] = {
        (RGBA){.r=255,.g=255,.b=255},
        (RGBA){.r=0,.g=0,.b=0 }
    };
    sprite->buffer[arrayPosition]=(clChar){.character='\n',.foregroundColor=RGBA_NULL,.backgroundColor=RGBA_NULL};
    arrayPosition++;

    for(int k = 0; k < gridSize;k++){
        for(int j = 0; j < cellSize;j++){
            for(int i = 0; i < gridSize;i++){
                for(int l = 0; l < cellSize*displayCellProportions;l++){
                    clChar character;
                    clCharInit(&character,' ',RGBA_NULL,RGBA_NULL);
                    if((i+k) % 2 == 0){
                        character.backgroundColor = colors[0];
                    }
                    else{
                        character.backgroundColor = colors[1];
                    }
                    sprite->buffer[arrayPosition]=character;
                    arrayPosition++;
                }
            }
            sprite->buffer[arrayPosition]=(clChar){.character='\n',.foregroundColor=RGBA_NULL,.backgroundColor=RGBA_NULL};
            arrayPosition++;
        }
    }
    return arrayPosition;
}