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
    clChar* end;
};

#define size8_t unsigned char;
typedef struct string string;
typedef struct unicodeChar unicodeChar;

struct unicodeChar{
    char* character;
    size8_t length;
};

struct string{
    size_t length;
    size_t capacity;
    size_t unicodeLength;
    unicodeChar* unicodeMaping;
    char* buffer;
    char* end;
};

void clStringInit(clString* str){
    
}










#endif