#include "ConsoleLabMinWin.h"
 
// Indeksy w palecie
#define COL_WHITE  0
#define COL_BLACK  1
#define COL_RED    2
#define COL_GREEN  3
#define COL_BLUE   4
#define COL_YELLOW 5
#define COL_CYAN   6
#define COL_MAGENTA 7
#define COL_GRAY   8
#define COL_TRANSPARENT   255

int main() {
    ConsoleLab_Init();
 
    int playerX = 10, playerY = 5;
    int frame = 0;
    int cellSize = 6;
    bool swapColors = false;
    vec3i cells[8][8];
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            cells[j][i] = (vec3i){2+cellSize*2*j, 1+cellSize*i, 0};
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

    // Sprite pawn;
    // pawn.position = (vec3i){10, 5, 0};
    // pawn.dimensions.width  = 12;
    // pawn.dimensions.height = 6;
    // Sprite_AllocateBuffer(&pawn);

    // Sprite_DrawWStringWrapped(&pawn, 0, 0, U"                 ▄▄         ████         ██         ▄██▄       ██████   ", COL_WHITE, 255);

    while (1) {
        ConsoleLab_Update();
 
        Input* input = &ConsoleLab_GlobalContext.input;
 
        if (InputKeyPressed(input, KEY_ESCAPE)) break;
 
        if (InputKeyPressed(input, KEY_W) || InputKeyPressed(input, KEY_UP))    playerY--;
        if (InputKeyPressed(input, KEY_S) || InputKeyPressed(input, KEY_DOWN))   playerY++;
        if (InputKeyPressed(input, KEY_A) || InputKeyPressed(input, KEY_LEFT))   playerX--;
        if (InputKeyPressed(input, KEY_D) || InputKeyPressed(input, KEY_RIGHT))  playerX++;
 
        ClearBuffer(COL_MAGENTA);

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

        for (size_t i = 0; i < 8; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                swapColors = !swapColors;
                Sprite_DrawWStringWrapped(&field, 0, 0, U"                                                                        ", COL_TRANSPARENT, swapColors ? COL_BLACK : COL_WHITE);
                field.position = cells[i][j];
                DrawToBufferOpaque(&field);
            }
            swapColors = !swapColors;
        }
       

        ConsoleLabFlipBuffer();
        frame++;
 
        Sleep(16);
    }
 
    return 0;
}