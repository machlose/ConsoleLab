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
typedef struct string string;
typedef struct unicodeChar unicodeChar;

struct string{
    size_t unicodeLength;
    size_t length;
    size_t capacity;
    char* buffer;
    char* end;
};
size8_t getCharLength(char* start);
void setStringUnicodeLenAndLen(string* String);
void setStringUnicodeLen(string* String);
int charStringUnicodeLenAndLen(char* buffer,size_t* len);
int charStringUnicodeLen(char* buffer);
void stringUInit(string* str,char* buffer);
void sequenceUnicodeString(char* buffer);
void stringCat(string* dest,char* src);
void stringCatString(string* dest,string* src);
void stringExtendBuffer(string* dest);

size8_t getCharLength(char* start){
    size8_t i = 7;
    while(((*start)&(1<<i)) > 0){ i--; }
    return 7-i;
}

void setStringUnicodeLenAndLen(string* String){
    int counter = 0;
    int index = 0;
    char* buffer = String->buffer;
    while((*buffer)){
        size8_t unicodeLen = getCharLength(buffer);
        if(unicodeLen == 1){
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        buffer+=unicodeLen;
        index+=unicodeLen;
        counter++;
    }
    String->unicodeLength = counter;
    String->length = index;
}
void setStringUnicodeLen(string* String){
    int counter = 0;
    char* buffer = String->buffer;
    while((*buffer)){
        size8_t unicodeLen = getCharLength(buffer);
        if(unicodeLen == 1){
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        buffer+=unicodeLen;
        counter++;
    }
    String->unicodeLength = counter;
}
int charStringUnicodeLenAndLen(char* buffer,size_t* len){
    int counter = 0;
    (*len) = 0;
    while((*buffer)){
        size8_t unicodeLen = getCharLength(buffer);
        if(unicodeLen == 1){
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        buffer+=unicodeLen;
        (*len)+=unicodeLen;
        counter++;
    }
    return counter;
}
int charStringUnicodeLen(char* buffer){
    int counter = 0;
    while((*buffer)){
        size8_t unicodeLen = getCharLength(buffer);
        if(unicodeLen == 1){
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        buffer+=unicodeLen;
        counter++;
    }
    return counter;
}

void stringUInit(string* str,char* buffer){
    int desiredLen;
    size_t len = strlen(buffer);

    for(int i = 0; i < 32; i++){
        if((len&(1<<i)) > 0){
            desiredLen = ((1<<i)<<1);
        }
    };
    str->buffer = calloc(desiredLen,sizeof(char));
    str->capacity = desiredLen;
    str->end = str->buffer;
    stringCat(str,buffer);
    setStringUnicodeLenAndLen(str);
    return;
}

void sequenceUnicodeString(char* buffer){
    int counter = 0;
    while((*buffer)){
        size8_t unicodeLen = getCharLength(buffer);
        if(unicodeLen == 1){
            continue;
        }
        if(unicodeLen == 0){
            unicodeLen = 1;
        }
        buffer+=unicodeLen;
        counter++;
    }
    return;
}
void stringCat(string* dest,char* src){
    size_t len;
    dest->unicodeLength+=charStringUnicodeLenAndLen(src,&len);
    dest->length+=len;
    if(dest->length >= dest->capacity){
        stringExtendBuffer(dest);
    }
    while (*dest->end) dest->end++;
    while (*dest->end++ = *src++);
    --dest->end;
}
void stringCatString(string* dest,string* src){
    stringCat(dest,src->buffer);
}
void stringExtendBuffer(string* dest){
    dest->capacity*=2;
    char* prevBuffer = dest->buffer;
    char* newBuffer = calloc(dest->capacity,sizeof(char));
    strcpy(newBuffer,prevBuffer);
    dest->buffer = newBuffer;
    dest->end=strend(dest->buffer);
    free(prevBuffer);
}
void clStringInit(clString* str){
    
}










#endif