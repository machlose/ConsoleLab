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

typedef  unsigned char size8_t;
typedef struct stringU stringU;
typedef struct unicodeChar unicodeChar;

struct stringU{
    size_t length; // unicode length
    size_t capacity;
    char* buffer;
    char* end;
};

size8_t getCharLength(char* start){
    size8_t i = 7;
    while(((*start)&(1<<i)) > 0){ i--; }
    return 7-i;
}

void stringUInit(stringU* str,char* buffer){
    int desiredLen;
    size_t len = strlen(buffer);

    for(int i = 0; i < 32; i++){
        if((len&(1<<i)) > 0){
            desiredLen = ((1<<i)<<1);
        }
    };
    str->buffer = calloc(desiredLen * sizeof(char),sizeof(char));
    str->capacity = desiredLen;
    str->length = len;

    return;
}

void sequenceUnicodeString(char* buffer){
    int index = 0;
    size_t len = strlen(buffer);
    while(index < len){
        size8_t unicodeLen = getCharLength(buffer);
        if(unicodeLen == 1){
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }

        for(int i = 0; i < unicodeLen;i++){
            //process
        }
    };
}
void clStringInit(clString* str){
    
}










#endif