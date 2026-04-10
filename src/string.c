#ifndef CLSTRING_H
#define CLSTRING_H
#include "ConsoleLab.h"
#include "vec.c"

struct clChar{
    char character;
    RGBA backgroundColor;
    RGBA foregroundColor;
};

struct clString{
    size_t length;
    size_t capacity;
    clChar* buffer;
};

void clStringInit(clString* str){
    
}










#endif