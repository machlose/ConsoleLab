#ifndef STRING
#define STRING
#include <stdlib.h>
#include <string.h>

#define BASE_STRING_SIZE 4

typedef struct {
    char* value;
    size_t size;
    size_t capacity;
} String;

String getString(size_t capacity){
    String str = {0};

    str.value = malloc(capacity);
    if(!str.value) return str;

    str.capacity = capacity;
    str.value[0] = '\0';

    return str;
}
#define getString() getString(BASE_STRING_SIZE)

int resizeString(String* str, size_t new_capacity){
    char* newPtr = realloc(str->value, new_capacity);
    if(!newPtr) return 0;

    str->value = newPtr;
    str->capacity = new_capacity;

    if(str->size >= new_capacity){
        str->size = new_capacity - 1;
        str->value[str->size] = '\0';
    }

    return 1;
}

int appendString(String* str, const char* text){
    size_t len = strlen(text);

    if(str->size + len + 1 > str->capacity){
        size_t newCap = str->capacity;

        while(str->size + len + 1 > newCap){
            newCap *= 2;
        }

        if(!resizeString(str, newCap)) return 0;
    }

    memcpy(str->value + str->size, text, len);
    str->size += len;
    str->value[str->size] = '\0';

    return 1;
}

int appendChar(String* str, char c){
    if(str->size + 2 > str->capacity){
        if(!resizeString(str, str->capacity * 2)) return 0;
    }

    str->value[str->size++] = c;
    str->value[str->size] = '\0';

    return 1;
}

void clearString(String* str){
    str->size = 0;
    if(str->value) str->value[0] = '\0';
}

int copyString(String* dest, const char* src){
    size_t len = strlen(src);

    if(len + 1 > dest->capacity){
        if(!resizeString(dest, len + 1)) return 0;
    }

    memcpy(dest->value, src, len + 1);
    dest->size = len;

    return 1;
}

void freeString(String* str){
    free(str->value);
    str->value = NULL;
    str->size = 0;
    str->capacity = 0;
}
#endif