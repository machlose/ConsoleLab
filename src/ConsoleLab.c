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

struct clConsoleSprite{
    vec2 position;
    vec2 dimensions;
    clChar* buffer;
};

void clConsoleSpriteInit(clConsoleSprite* sprite){
    sprite->buffer = malloc(vec2_sum(sprite->dimensions));
    if(!sprite->buffer){
        return;
    }
    //succes
}

void clConsoleSpriteFree(clConsoleSprite* sprite){
    free(sprite);
}

void clConsoleSpriteCreate(clConsoleSprite* sprite, float x, float y, float width, float height){
    sprite->position = Vec2(x, y);
    sprite->dimensions = Vec2(width, height);
    clConsoleSpriteInit(sprite);
}

void clConsoleSpriteCreateVec(clConsoleSprite* sprite, vec2 position, vec2 dimensions){
    sprite->position = position;
    sprite->dimensions = dimensions;
    clConsoleSpriteInit(sprite);
}

//TODO funkcja do printowania sprita na inny sprite, i do printowania sprita do konsoli i najpierw printowac na prite BUFFER i z buffera na kosole