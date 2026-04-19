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
 
int main() {
    ConsoleLab_Init();
 
    int playerX = 10, playerY = 5;
    int frame = 0;
 
    while (1) {
        ConsoleLab_Update();
 
        Input* input = &ConsoleLab_GlobalContext.input;
 
        // Wyjscie
        if (InputKeyPressed(input, KEY_ESCAPE)) break;
 
        // Ruch gracza
        if (InputKeyPressed(input, KEY_W) || InputKeyPressed(input, KEY_UP))    playerY--;
        if (InputKeyPressed(input, KEY_S) || InputKeyPressed(input, KEY_DOWN))   playerY++;
        if (InputKeyPressed(input, KEY_A) || InputKeyPressed(input, KEY_LEFT))   playerX--;
        if (InputKeyPressed(input, KEY_D) || InputKeyPressed(input, KEY_RIGHT))  playerX++;
 
        // Rysuj
        ClearBuffer(COL_BLACK);
 
        // Ramka
        vec2i size = ConsoleLab_GlobalContext.buffer.current->dimensions;
        for (int x = 0; x < size.width; x++) {
            DrawChar(x, 0,          U'─', COL_CYAN, COL_BLACK);
            DrawChar(x, size.height-1, U'─', COL_CYAN, COL_BLACK);
        }
        for (int y = 1; y < size.height-1; y++) {
            DrawChar(0,           y, U'│', COL_CYAN, COL_BLACK);
            DrawChar(size.width-1, y, U'│', COL_CYAN, COL_BLACK);
        }
        DrawChar(0,            0,           U'┌', COL_CYAN, COL_BLACK);
        DrawChar(size.width-1, 0,           U'┐', COL_CYAN, COL_BLACK);
        DrawChar(0,            size.height-1, U'└', COL_CYAN, COL_BLACK);
        DrawChar(size.width-1, size.height-1, U'┘', COL_CYAN, COL_BLACK);
 
        // Gracz
        DrawChar(playerX, playerY, U'@', COL_YELLOW, COL_BLACK);
 
        // UI - pozycja myszy
        char info[64];
        sprintf(info, "Mysz: %d,%d  Kolko: %d  Klatka: %d",
            input->mousePosition.x, input->mousePosition.y,
            input->mouseWheel, frame);
        CL_DrawText(2, 1, info, COL_WHITE, COL_BLACK);
 
        CL_DrawText(2, 2, "WASD/strzalki = ruch  ESC = wyjscie", COL_GRAY, COL_BLACK);
 
        // Kursor myszy
        DrawChar(input->mousePosition.x, input->mousePosition.y,
                 U'▪', COL_RED, COL_BLACK);
 
        // Animacja w rogu
        const char* anim[] = {"|", "/", "-", "\\"};
        CL_DrawText(size.width - 3, 1, anim[frame % 4], COL_GREEN, COL_BLACK);
 
        // LPM = rysuj znak w miejscu kursora
        if (InputKeyDown(input, MOUSE_LEFT)) {
            DrawChar(input->mousePosition.x, input->mousePosition.y,
                     U'█', COL_MAGENTA, COL_BLACK);
        }
        
        Sprite pawn;
        pawn.position = (vec3i){10, 5, 0};  // albo ręcznie .x=10 .y=5 .z=0
        pawn.dimensions.width  = 12;
        pawn.dimensions.height = 6;
        Sprite_AllocateBuffer(&pawn);

        Sprite_DrawWStringWrapped(&pawn, 0, 0, U"                 ▄▄         ████         ██         ▄██▄       ██████   ", COL_WHITE, 255);
        
        Sprite bg;
        bg.position = (vec3i){10, 5, 0};  // albo ręcznie .x=10 .y=5 .z=0
        bg.dimensions.width  = 12;
        bg.dimensions.height = 6;
        Sprite_AllocateBuffer(&bg);
        
        Sprite_DrawWStringWrapped(&bg, 0, 0, U"                                                                        ", COL_WHITE, COL_MAGENTA);

        DrawToBufferOpaque(&bg);
        bg.position = (vec3i){22, 5, 0};
        Sprite_DrawWStringWrapped(&bg, 0, 0, U"                                                                        ", COL_BLACK, COL_WHITE);
        DrawToBufferOpaque(&bg);
        DrawToBuffer(&pawn);
        

        ConsoleLabFlipBuffer();
        frame++;
 
        Sleep(16); // ~60fps
    }
 
    return 0;
}