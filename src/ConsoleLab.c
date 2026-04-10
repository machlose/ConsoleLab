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

#include "string.c"


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

//TODO funkcja do printowania sprita na inny sprite, i do printowania sprita do konsoli i najpierw printowac na prite BUFFER i z buffera na kosole