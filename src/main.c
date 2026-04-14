#include <stdio.h>
#include "ConsoleLab.h"

void ConsoleLabInit(){

}
void ConsoleLabClose(){

}

void ConsoleLabFlipBuffer(clBuffer* buffer){

}

void ConsoleLabFPS(size_t targetFPS){

}

void DrawSpriteToSprite(clSprite* sprite, clSprite* sprite2){
    
}

void ConsoleLabDrawSprite(clBuffer* buffer, clSprite* sprite){
    DrawSpriteToSprite(buffer->currentBuffer, sprite);
}

int main(){
    ConsoleLabInit();

    bool run = 1;
    clWindow* window = ConsoleLabGetWindow();
    clBuffer* buffer = ConsoleLabCreateBuffer();
    clInput* input = ConsoleLabCreateInputHandle();
    while(run){
        ConsoleLabTick(window, input, buffer);
        if(input->keys[KEY_ESC]){
            run = 0;
        }
        clSprite* sprite = ConsoleLabCreateSprite("ascii art(without \ns because I want it to be handled by size when drawing)", input->mousePos, Vec2(5, 5));//if input string is bigger than size trim it  
        ConsoleLabDrawSprite(buffer, sprite);
        ConsoleLabFlipBuffer(buffer);//draws buffer to windows console
        ConsoleLabFPS(60);//target fps
        run = 0;
    }
    ConsoleLabClose();
    printf("ended");
    return 0;
}






// #include "Views/ChessBoard.c"
// #include <time.h>
// #include <uchar.h>

// int main(){
//     clock_t begin = clock();


//     // char buffer[10000];
//     // clSprite test;
//     // clSpriteCreate(&test,0,0,6,6);
//     // GetSpriteFromCharSpriteMap(&test,chessPieceSprites,0,0,6,6,6,73);
//     // RenderSpriteString(&test,buffer,6*6);
//     // printf(chessPieceSprites);

//     clSprite sprite;
//     int len = RenderChessBoard(&sprite);
//     char buffer[250000];
//     RenderSpriteString(&sprite,buffer,len);
//     printf(buffer);
//     string test;
//     stringUInit(&test,"███");
//     stringCat(&test,"███");
//     printf(test.buffer);

//     clock_t end = clock();
//     double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//     printf("%f.5",time_spent);
//     return 0;
// }