#include "../ConsoleLab.h"
#include <stdio.h>
#include <stdlib.h>

char* chessPieceBuffer = 
"                                                     »«         ─╡╞─     \n"
"                            ▄_           <>         (██)        (██)     \n"
"    (██)       [████]       ███\\        (██)        ▀██▀        ▀██▀     \n"
"     ▐▌         ▐██▌        ████)       /██\\         ██          ██      \n"
"     ██         ▐██▌        ▐██▌         ██          ██          ██      \n"
"   ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄     ";
clSprite chessPieceSprite;
void initChessPieceSprite(){
    clSpriteCreate(&chessPieceSprite,0,0,74,6);
    clSpriteBufferPopulate(&chessPieceSprite,chessPieceBuffer);
}

char* RenderChessPiece(){

}
int RenderChessBoard(clSprite* sprite){
    const int displayCellProportions = 2;
    const int cellSize = 6;
    const int gridSize = 8;

    int arrayPosition = 0;
    clSpriteCreate(sprite,0,0,displayCellProportions*cellSize*gridSize+1,cellSize*gridSize);
    sprite->buffer[arrayPosition]=(clChar){.character="\n",.foregroundColor=&colorTable[Clear_c],.backgroundColor=&colorTable[Clear_c]};
    arrayPosition++;

    for(int k = 0; k < gridSize;k++){
        for(int j = 0; j < cellSize;j++){
            for(int i = 0; i < gridSize;i++){
                for(int l = 0; l < cellSize*displayCellProportions;l++){
                    clChar character;
                    clCharInit(&character," ",&colorTable[Clear_c],&colorTable[Clear_c]);
                    if((i+k) % 2 == 0){
                        character.backgroundColor = &colorTable[White_c];
                    }
                    else{
                        character.backgroundColor = &colorTable[Black_c];
                    }
                    sprite->buffer[arrayPosition]=character;
                    arrayPosition++;
                }
            }
            sprite->buffer[arrayPosition]=(clChar){.character="\n",.foregroundColor=&colorTable[Clear_c],.backgroundColor=&colorTable[Clear_c]};
            arrayPosition++;
        }
    }
    return arrayPosition;
}