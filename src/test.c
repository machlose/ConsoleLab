#include "ConsoleLabMinWin.h"
#include <stdbool.h>
 
#define COL_WHITE  11
#define COL_BLACK  12
#define COL_RED    2
#define COL_GREEN  3
#define COL_BLUE   4
#define COL_YELLOW 5
#define COL_CYAN   6
#define COL_MAGENTA 7
#define COL_GRAY   8
#define COL_TRANSPARENT   255

typedef enum{
    Zastaw, //Pawn
    Gawron, //Rook
    Rycerz, //Knight
    Biskup, //Bishop
    Krulowa,//Queen
    Krul    //King
} PieceType;

typedef struct {
    PieceType type;
    bool white;
    bool captured;
    Sprite sprite;
} Piece; 

int main() {
    ConsoleLab_Init();
    
    int playerX = 0, playerY = 0;
    int frame = 0;
    int cellSize = 6;
    bool swapColors = false;
    vec3i cells[8][8];
    Piece* piecesOnCells[8][8];
    vec2i hoverCell;
    Piece* holdedPiece = NULL;
    Piece holdedPieceBefore;
    vec2i hoverCellBefore;

    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            cells[j][i] = (vec3i){2+cellSize*2*j, 1+cellSize*i, 0};
        }
    }
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            piecesOnCells[j][i] = NULL;
        }
    }


    Sprite cell;
    cell.position = (vec3i){0, 0, 0};
    cell.dimensions.width  = 2;
    cell.dimensions.height = 1;
    Sprite_AllocateBuffer(&cell);

    Sprite field;
    field.position = (vec3i){0, 0, 0};
    field.dimensions.width  = cellSize*2;
    field.dimensions.height = cellSize;
    Sprite_AllocateBuffer(&field);

    Piece pieces[32]; 
    Piece* white = &0[pieces];
    Piece* black = &16[pieces];

    for (size_t i = 0; i < 8; i++)
    {
        pieces[16+i].sprite.position = cells[i][6];
        pieces[i].sprite.position = cells[i][1];
        piecesOnCells[i][6] = &pieces[16+i];
        piecesOnCells[i][1] = &pieces[i];
        pieces[16+i].sprite.dimensions.width  = cellSize*2;
        pieces[16+i].sprite.dimensions.height = cellSize;
        pieces[i].sprite.dimensions.width  = cellSize*2;
        pieces[i].sprite.dimensions.height = cellSize;
        pieces[16+i].type = Zastaw;
        pieces[i].type = Zastaw;
        Sprite_AllocateBuffer(&pieces[16+i].sprite);
        Sprite_AllocateBuffer(&pieces[i].sprite);
        Sprite_DrawWStringWrapped(&pieces[16+i].sprite, 0, 0, U"                 ▄▄         ████         ██         ▄██▄       ██████   ", 9, COL_TRANSPARENT);
        Sprite_DrawWStringWrapped(&pieces[i].sprite, 0, 0, U"                 ▄▄         ████         ██         ▄██▄       ██████   ", 10, COL_TRANSPARENT);
        
        pieces[24+i].sprite.position = cells[i][7];
        pieces[8+i].sprite.position = cells[i][0];
        piecesOnCells[i][7] = &pieces[24+i];
        piecesOnCells[i][0] = &pieces[8+i];
        pieces[24+i].sprite.dimensions.width  = cellSize*2;
        pieces[24+i].sprite.dimensions.height = cellSize;
        pieces[8+i].sprite.dimensions.width  = cellSize*2;
        pieces[8+i].sprite.dimensions.height = cellSize;
        Sprite_AllocateBuffer(&pieces[24+i].sprite);
        Sprite_AllocateBuffer(&pieces[8+i].sprite);

        pieces[i].captured = false;
        pieces[8+i].captured = false;
        pieces[16+i].captured = false;
        pieces[24+i].captured = false;
    }
    
    white[8].type = Gawron;
    white[15].type = Gawron;
    Sprite_DrawWStringWrapped(&white[8].sprite, 0, 0, U"   ▄ ▄▄ ▄      ██████       ████        ████       ▐████▌      ██████   ", 10, COL_TRANSPARENT);
    Sprite_DrawWStringWrapped(&white[15].sprite, 0, 0, U"   ▄ ▄▄ ▄      ██████       ████        ████       ▐████▌      ██████   ", 10, COL_TRANSPARENT);
    white[9].type = Rycerz;
    white[14].type = Rycerz;
    Sprite_DrawWStringWrapped(&white[9].sprite, 0, 0, U"    ▄▄▄        ▄████       ▀▀███         ██         ▄██▄       ██████   ", 10, COL_TRANSPARENT);
    Sprite_DrawWStringWrapped(&white[14].sprite, 0, 0, U"    ▄▄▄        ▄████       ▀▀███         ██         ▄██▄       ██████   ", 10, COL_TRANSPARENT);
    white[10].type = Biskup;
    white[13].type = Biskup;
    Sprite_DrawWStringWrapped(&white[10].sprite, 0, 0, U"     ▐▌         ▄██▄        ████         ██         ▄██▄       ██████   ", 10, COL_TRANSPARENT);
    Sprite_DrawWStringWrapped(&white[13].sprite, 0, 0, U"     ▐▌         ▄██▄        ████         ██         ▄██▄       ██████   ", 10, COL_TRANSPARENT);
    white[11].type = Krul;
    Sprite_DrawWStringWrapped(&white[11].sprite, 0, 0, U"   ▄ ██ ▄      ██████       ▐██▌        ▐██▌        ████       ██████   ", 10, COL_TRANSPARENT);
    white[12].type = Krulowa;
    Sprite_DrawWStringWrapped(&white[12].sprite, 0, 0, U"    ▄██▄        ▄██▄        ▐██▌         ██         ▄██▄       ██████   ", 10, COL_TRANSPARENT);

    black[8].type = Gawron;
    black[15].type = Gawron;
    Sprite_DrawWStringWrapped(&black[8].sprite, 0, 0, U"   ▄ ▄▄ ▄      ██████       ████        ████       ▐████▌      ██████   ", 9, COL_TRANSPARENT);
    Sprite_DrawWStringWrapped(&black[15].sprite, 0, 0, U"   ▄ ▄▄ ▄      ██████       ████        ████       ▐████▌      ██████   ", 9, COL_TRANSPARENT);
    black[9].type = Rycerz;
    black[14].type = Rycerz;
    Sprite_DrawWStringWrapped(&black[9].sprite, 0, 0, U"    ▄▄▄        ▄████       ▀▀███         ██         ▄██▄       ██████   ", 9, COL_TRANSPARENT);
    Sprite_DrawWStringWrapped(&black[14].sprite, 0, 0, U"    ▄▄▄        ▄████       ▀▀███         ██         ▄██▄       ██████   ", 9, COL_TRANSPARENT);
    black[10].type = Biskup;
    black[13].type = Biskup;
    Sprite_DrawWStringWrapped(&black[10].sprite, 0, 0, U"     ▐▌         ▄██▄        ████         ██         ▄██▄       ██████   ", 9, COL_TRANSPARENT);
    Sprite_DrawWStringWrapped(&black[13].sprite, 0, 0, U"     ▐▌         ▄██▄        ████         ██         ▄██▄       ██████   ", 9, COL_TRANSPARENT);
    black[11].type = Krul;
    Sprite_DrawWStringWrapped(&black[11].sprite, 0, 0, U"   ▄ ██ ▄      ██████       ▐██▌        ▐██▌        ████       ██████   ", 9, COL_TRANSPARENT);
    black[12].type = Krulowa;
    Sprite_DrawWStringWrapped(&black[12].sprite, 0, 0, U"    ▄██▄        ▄██▄        ▐██▌         ██         ▄██▄       ██████   ", 9, COL_TRANSPARENT);

    for (size_t i = 0; i < 16; i++)
    {
        white[i].white = true;
        black[i].white = false;
    }

    while (1) {
        ConsoleLab_Update();
 
        Input* input = &ConsoleLab_GlobalContext.input;
 
        if (InputKeyPressed(input, KEY_ESCAPE)) break;
 
        if (InputKeyPressed(input, KEY_W) || InputKeyPressed(input, KEY_UP))    playerY--;
        if (InputKeyPressed(input, KEY_S) || InputKeyPressed(input, KEY_DOWN))   playerY++;
        if (InputKeyPressed(input, KEY_A) || InputKeyPressed(input, KEY_LEFT))   playerX--;
        if (InputKeyPressed(input, KEY_D) || InputKeyPressed(input, KEY_RIGHT))  playerX++;
 
        ClearBuffer(1);

        swapColors = true;
        Sprite_DrawWStringWrapped(&cell, 0, 0, U"██", swapColors ? COL_BLACK : COL_WHITE, 255);

        cell.position = (vec3i){0, 0, 0};
        DrawToBuffer(&cell);

        cell.position = (vec3i){cellSize*2*8+2, cellSize*8+1, 0};
        DrawToBuffer(&cell);

        swapColors = !swapColors;
        Sprite_DrawWStringWrapped(&cell, 0, 0, U"██", swapColors ? COL_BLACK : COL_WHITE, 255);

        cell.position = (vec3i){0, cellSize*8+1, 0};
        DrawToBuffer(&cell);

        cell.position = (vec3i){cellSize*2*8+2, 0, 0};
        DrawToBuffer(&cell);

        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < cellSize; j++)
            {
                cell.position = (vec3i){j*2+2+i*cellSize*2, 0, 0};
                DrawToBuffer(&cell);
                cell.position = (vec3i){0, j+1+i*cellSize, 0};
                DrawToBuffer(&cell);
            }
            swapColors = !swapColors;
            Sprite_DrawWStringWrapped(&cell, 0, 0, U"██", swapColors ? COL_BLACK : COL_WHITE, 255);
            for (size_t j = 0; j < cellSize; j++)
            {
                cell.position = (vec3i){j*2+2+i*cellSize*2, cellSize*8+1, 0};
                DrawToBuffer(&cell);
                cell.position = (vec3i){cellSize*2*8+2, j+1+i*cellSize, 0};
                DrawToBuffer(&cell);
            }
        }

        hoverCell = (vec2i){-1, -1};
        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                swapColors = !swapColors;
                if(input->mousePosition.x>=cells[i][j].x && input->mousePosition.x<cells[i][j].x+field.dimensions.width && input->mousePosition.y>=cells[i][j].y && input->mousePosition.y<cells[i][j].y+field.dimensions.height){
                    hoverCell = (vec2i){i, j};//&cells[i][j];
                    Sprite_DrawWStringWrapped(&field, 0, 0, U"                                                                        ", COL_TRANSPARENT, 14);//swapColors ? 13 : 13
                } else{
                    Sprite_DrawWStringWrapped(&field, 0, 0, U"                                                                        ", COL_TRANSPARENT, swapColors ? COL_BLACK : COL_WHITE);
                }
                field.position = (vec3i){cells[i][j].x+playerX, cells[i][j].y+playerY, 0};
                DrawToBufferOpaque(&field);
            }
            swapColors = !swapColors;
        }

        if(InputKeyPressed(input, MOUSE_LEFT)){ 
            if(hoverCell.x!=-1){
                holdedPiece = piecesOnCells[hoverCell.x][hoverCell.y];
                piecesOnCells[hoverCell.x][hoverCell.y] = NULL;
                if(holdedPiece){
                    holdedPieceBefore = *holdedPiece;
                    hoverCellBefore = hoverCell;
                }
            }
        } else if(InputKeyReleased(input, MOUSE_LEFT)){
            if(holdedPiece){
                if(hoverCell.x==-1){
                    *holdedPiece = holdedPieceBefore;
                    piecesOnCells[hoverCellBefore.x][hoverCellBefore.y] = holdedPiece;
                } else{ 
                    if(piecesOnCells[hoverCell.x][hoverCell.y]){
                        if(holdedPiece->white == piecesOnCells[hoverCell.x][hoverCell.y]->white){
                            *holdedPiece = holdedPieceBefore;
                            hoverCell = hoverCellBefore;
                            // piecesOnCells[hoverCellBefore.x][hoverCellBefore.y] = holdedPiece;
                        } else{
                            piecesOnCells[hoverCell.x][hoverCell.y]->captured = true;
                        }
                    }
                    holdedPiece->sprite.position = cells[hoverCell.x][hoverCell.y];
                    piecesOnCells[hoverCell.x][hoverCell.y] = holdedPiece;
                    holdedPiece = NULL;
                }
            }
        } else if(input->keys[MOUSE_LEFT]){
            if(holdedPiece){
                holdedPiece->sprite.position.x+=input->mouseDelta.x;
                holdedPiece->sprite.position.y+=input->mouseDelta.y;
            }
        }

       
        for (size_t i = 0; i < 32; i++)
        {
            if(!pieces[i].captured){
                DrawToBuffer(&pieces[i].sprite);
            }
        }
        if(holdedPiece){
            DrawToBuffer(&holdedPiece->sprite);
        }

        ConsoleLabFlipBuffer();
        frame++;
 
        Sleep(16);
    }
 
    return 0;
}