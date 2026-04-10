#include "../ConsoleLab.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct strPair{
    int key;
    char value;
} strPair;
strPair encodingArray[] = {
    (strPair){.key = ' ', .value = ' '},
    (strPair){.key = '█', .value = (char)219},
    (strPair){.key = '▄', .value = (char)220},
    (strPair){.key = '▌', .value = (char)221},
    (strPair){.key = '▐', .value = (char)222},
    (strPair){.key = '▀', .value = (char)223},
};

#define "█" (char)219:w

char* RenderChessBoard(){
    const int displayCellProportions = 2;
    const int cellSize = 6;
    const int gridSize = 8;

    unsigned char* sprites = 
    "                                                     »«         ─╡╞─     \n"
    "                            ▄_           <>         (██)        (██)     \n"
    "    (██)       [████]       ███\        (██)        ▀██▀        ▀██▀     \n"
    "     ▐▌         ▐██▌        ████)       /██\         ██          ██      \n"
    "     ██         ▐██▌        ▐██▌         ██          ██          ██      \n"
    "   ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄      ▄████▄    ";

    char* board = malloc(10000);
    board[0] = '\0';
    board[9999] = '\0';

    board = strcat(board,"\n");
    for(int k = 0; k < gridSize;k++){
        for(int j = 0; j < cellSize;j++){
            for(int i = 0; i < gridSize;i++){
                char* test = malloc(2);
                if((i+k) % 2 == 0){
                    test[0] = (char)219; 
                    test[1] = '\0';
                }
                else{
                    test[0] = ' '; 
                    test[1] = '\0';
                }
                for(int i = 0; i < cellSize*displayCellProportions;i++){
                    board = strcat(board,test);
                }
            }
            board = strcat(board,"\n");
        }
    }
    return board;
}