#include <stdio.h>
#include "ConsoleLab.h"

enum CL_KEYS{
    KEY_MOUSE_LEFT = index, 
    KEY_A = index,
    KEY_ESC = index
};

typedef struct clInput{
    vec2 mousePos;//vec2 is int
    bool keys[number of keys];//every tick in handle events set to true only if it was clicked in this frame
    bool keysHeld[number of keys];
    //handle mouse scrool too
} clInput;

typedef struct clBuffer{
    clSprite buffers[2];

} clBuffer;

int main(){
    ConsoleLabInit();

    bool run = 1;
    clWindow window = ConsoleLabGetWindow();
    clBuffer buffer = ConsoleLabCreateBuffer();
    clInput input = ConsoleLabCreateInputHandle();
    while(run){
<<<<<<< Updated upstream
        ConsoleLabTick(&window, &input, &buffer);
        if(input.keys[KEY_ESC]){
            run = 0;
        }
        clSprite sprite = ConsoleLabCreateSprite("ascii art(without \ns because I want it to be handled by size when drawing)", input.mousePos, Vec2(5, 5));//if input string is bigger than size trim it  
=======
        printf("1");
        ConsoleLabTick(window, input, buffer);
        if(input->keys[KEY_ESC]){
            run = 0;
        }
        printf("2");

        clSprite* sprite = ConsoleLabCreateSprite("ascii art(without \ns because I want it to be handled by size when drawing)", input->mousePos, Vec2(5, 5));//if input string is bigger than size trim it  
>>>>>>> Stashed changes
        ConsoleLabDrawSprite(buffer, sprite);

        ConsoleLabFlipBuffer(window, buffer);//draws buffer to windows console
        ConsoleLabFPS(60);//target fps
    }
    ConsoleLabClose();
    return 0;
}

void ConsoleLabHandleEvents(clWindow window, clInput input, clBuffer buffer){
    //handle resizing windows console buffer and clBuffer if window resized

}

void ConsoleLabTick(clWindow window, clInput input, clBuffer buffer){
    ConsoleLabHandleEvents(window, input, buffer);
}



// #include "Views/ChessBoard.c"
// #include <time.h>
// #include <uchar.h>

// int main(){
//     clock_t begin = clock();


//     // char buffer[10000];
//     // clConsoleSprite test;
//     // clConsoleSpriteCreate(&test,0,0,6,6);
//     // GetSpriteFromCharSpriteMap(&test,chessPieceSprites,0,0,6,6,6,73);
//     // RenderSpriteString(&test,buffer,6*6);
//     // printf(chessPieceSprites);

//     clConsoleSprite sprite;
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