#include "ConsoleLab.h"
#define CL_IMPL_VERSION_MAJOR 0
#define CL_IMPL_VERSION_MINOR 0
#if CL_VERSION_MAJOR != CL_IMPL_VERSION_MAJOR || CL_VERSION_MINOR != CL_IMPL_VERSION_MINOR
#error "ConsoleLab version mismatch between header and implementation"
#endif


#ifdef _WIN32
#include "ConsoleLab_win.c"
#else
#include "ConsoleLab_unix.c"
#endif

// void clInit(clContext* context, clResult* result){
    
// }

void clCharInit(clChar* clCharacter,char character, RGBA backgroundColor, RGBA foregroundColor){
    clCharacter->character = character;
    clCharacter->backgroundColor = backgroundColor;
    clCharacter->foregroundColor = foregroundColor;
}

int clConsoleSpriteInit(clConsoleSprite* sprite) {
    if (!sprite) return 1;

    if (sprite->dimensions.x == 0 || sprite->dimensions.y == 0) {
        sprite->buffer = NULL;
        return 2;
    }

    size_t size = sprite->dimensions.x * sprite->dimensions.y;
    if (size / sprite->dimensions.x != sprite->dimensions.y) {
        sprite->buffer = NULL;
        return 3;
    }

    sprite->buffer = malloc(size * sizeof(*sprite->buffer));
    if (!sprite->buffer) {
        return 4;
    }
    return 0;
}

void clConsoleSpriteFree(clConsoleSprite* sprite){
    if (!sprite) return;
    free(sprite->buffer);
    free(sprite);
}

int clConsoleSpriteCreate(clConsoleSprite* sprite, float x, float y, float width, float height){
    sprite->position = Vec2(x, y);
    sprite->dimensions = Vec2(width, height);
    if(!clConsoleSpriteInit(sprite)){
        //error handling
        return 1;
    }
    return 0;
}

int clConsoleSpriteCreateVec(clConsoleSprite* sprite, vec2 position, vec2 dimensions){
    sprite->position = position;
    sprite->dimensions = dimensions;
    if(!clConsoleSpriteInit(sprite)){
        //error handling
        return 1;
    }
    return 0;
}
void GetSpriteFromCharSpriteMap(clConsoleSprite* destSprite,wchar_t* spriteMap,int x, int y, int h, int w, int mapH, int mapW){
    if(x < 0 || x+w > mapW){
        return;
    }
    if(y < 0 || y+h>mapH){
        return;
    }

    const int destWidth = mapW;
    const int startIndex = x+(y*destWidth);

    int index = 0;
    for(int yp = y; yp < y+h;yp++){
        for(int xp = x; xp < x+w;xp++){
            destSprite->buffer[index] = (clChar){.character = spriteMap[(yp*destWidth)+xp],RGBA_NULL,RGBA_NULL};
            index++;
        }
    }
}
void WriteToSprite(clConsoleSprite* destSprite,clConsoleSprite* srcSprite){
    if(srcSprite->position.x < 0 || srcSprite->position.x+srcSprite->dimensions.w>destSprite->dimensions.w){
        return;
    }
    if(srcSprite->position.y < 0 || srcSprite->position.y+srcSprite->dimensions.h>destSprite->dimensions.h){
        return;
    }

    const int destWidth = destSprite->dimensions.w;
    const int startIndex = srcSprite->position.x+(srcSprite->position.y*destWidth);

    int index = 0;
    for(int y = srcSprite->position.y; y < srcSprite->position.y+srcSprite->dimensions.h;y++){
        for(int x = srcSprite->position.x; y < srcSprite->position.x+srcSprite->dimensions.w;x++){
            destSprite->buffer[(y*destWidth)+x] = srcSprite->buffer[index];
            index++;
        }
    }
}

void RenderSpriteString(clConsoleSprite* sprite,char* output,int length){
    int index = 0;
    char* outputEnd = output;
    while(index < length){
        clChar* character = &sprite->buffer[index];
        char buffer[300] = "";
        char* bufferEnd = buffer;

        char background[200] = "";
        if(character->backgroundColor.r == RGBA_NULL.r){
            GetConsoleTextColorDefaultBackgroundString(background);
        }
        else{
            GetConsoleTextBackgroundColorString(background,character->backgroundColor);
        }

        char foreground[200] = "";
        if(character->foregroundColor.r == RGBA_NULL.r){
            GetConsoleTextColorDefaultForegroundString(foreground);
        }
        else{
            GetConsoleTextForegroundColorString(foreground,character->foregroundColor);
        }

        bufferEnd = strcatf(bufferEnd,background);
        bufferEnd = strcatf(bufferEnd,foreground);
        char characterValue[2] = {character->character,'\0'};
        bufferEnd = strcatf(bufferEnd,characterValue);

        outputEnd = strcatf(outputEnd,buffer);
        index += 1;
    }
}
<<<<<<< Updated upstream
//TODO funkcja do printowania sprita na inny sprite, i do printowania sprita do konsoli i najpierw printowac na prite BUFFER i z buffera na kosole
=======
//TODO funkcja do printowania sprita na inny sprite, i do printowania sprita do konsoli i najpierw printowac na prite BUFFER i z buffera na kosole











clWindow* ConsoleLabGetWindow(){
    clWindow* window;
    return window;//undfined behavior
}

clBuffer* ConsoleLabCreateBuffer(){
    clBuffer* buffer;
    return buffer;//undfined behavior
}

clInput* ConsoleLabCreateInputHandle(){
    clInput* input;
    return input;//undfined behavior
}

void clCharSetBuffer(clChar* clchar, char* buffer ,size8_t unicodeLen){
    clchar->unicodeLen = unicodeLen;
    clchar->character = malloc(unicodeLen + 1);
    memcpy(clchar->character, buffer, unicodeLen);
    clchar->character[unicodeLen] = '\0';
}

void clSequenceUnicodeString(clChar* buffer, size_t size, char* unicodeBuffer){
    if (!buffer || !unicodeBuffer || size == 0) {
        return;
    }
    int counter = 0;
    while((*unicodeBuffer) && counter<size){
        size8_t unicodeLen = getCharLength(unicodeBuffer);
        if(unicodeLen == 1){
            unicodeBuffer++;
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        clCharSetBuffer(&buffer[counter], unicodeBuffer, unicodeLen);
        unicodeBuffer+=unicodeLen;
        counter++;
    }
    return;
}

clSprite* ConsoleLabCreateSprite(char* input, vec2 position, vec2 dimensions){
    clSprite* sprite = malloc(sizeof(clSprite));
    if(!sprite){
        return NULL;
    }
    sprite->position = position;
    sprite->dimensions = dimensions;

    if(sprite->dimensions.x == 0 || sprite->dimensions.y == 0) {
        sprite->buffer = NULL;
        return sprite;
    }

    size_t size = sprite->dimensions.x * sprite->dimensions.y;
    sprite->buffer = calloc(size, sizeof(*sprite->buffer));
    if (!sprite->buffer) {
        free(sprite);
        return NULL;
    }
    if (input) {
        clSequenceUnicodeString(sprite->buffer, size, input);
    }
    return sprite;
}

// void ConsoleLabHandleEvents(clWindow* window, clInput* input, clBuffer* buffer){
//     //handle resizing windows console buffer and clBuffer if window resized

// }

void ConsoleLabTick(clWindow* window, clInput* input, clBuffer* buffer){
    ConsoleLabHandleEvents(window, input, buffer);
}

void ConsoleLabFreeSprite(clSprite* sprite){
    if(!sprite) return;

    if(sprite->buffer){
        for(int i = 0; i < sprite->dimensions.x * sprite->dimensions.y; i++){
            free(sprite->buffer[i].character);
        }
        free(sprite->buffer);
    }
    free(sprite);
}


void ConsoleLabInit(){

}

void ConsoleLabClose(){

}

void ConsoleLabFlipBuffer(clWindow* window, clBuffer* buffer){
    if(!buffer || !window) return;
    ConsoleLabDrawBuffer(window, buffer);

    if(buffer->currentBuffer == &buffer->buffers[0])
        buffer->currentBuffer = &buffer->buffers[1];
    else
        buffer->currentBuffer = &buffer->buffers[0];
}

void ConsoleLabFPS(size_t targetFPS){

}

void DrawSpriteToSprite(clSprite* target, clSprite* source){
    if(!target || !source || !target->buffer || !source->buffer){
        return;
    }

    for(int y = 0; y < source->dimensions.y; y++){
        for(int x = 0; x < source->dimensions.x; x++){

            int targetX = source->position.x + x;
            int targetY = source->position.y + y;

            // clipping (czyli żeby nie wyjść poza buffer)
            if(targetX < 0 || targetY < 0 ||
               targetX >= target->dimensions.x ||
               targetY >= target->dimensions.y){
                continue;
            }

            int srcIndex = y * source->dimensions.x + x;
            int dstIndex = targetY * target->dimensions.x + targetX;

            target->buffer[dstIndex] = source->buffer[srcIndex];
        }
    }
}

void ConsoleLabDrawSprite(clBuffer* buffer, clSprite* sprite){
    if(!buffer || !sprite) return;
    DrawSpriteToSprite(buffer->currentBuffer, sprite);
}
>>>>>>> Stashed changes
