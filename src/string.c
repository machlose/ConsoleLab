#ifndef CLSTRING_H
#define CLSTRING_H
#include "ConsoleLab.h"
#include "vec.c"

struct clChar{
    unsigned char asciCharacter;
    vec3 color;
};

struct clString{
    size_t length;
    size_t capacity;
    clChar* buffer;
};

void clStringInit(clString* str){
    
}










#endif