#ifndef FILE_C
#define FILE_C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define READ_FILE_BUFFER_SIZE 4096
#define FILE_STRING_STEP_SIZE READ_FILE_BUFFER_SIZE
#define BASE_FILE_STRING_SIZE FILE_STRING_STEP_SIZE*8
const size_t readFileBufferSize = READ_FILE_BUFFER_SIZE;
const size_t fileStringStepSize = FILE_STRING_STEP_SIZE;

typedef struct fileString fileString;
struct fileString{
    char* string;
    size_t size;
    size_t capacity;
    char success;
};

fileString getFileString(size_t capacity) {
    fileString str;
    str.size = 0;
    str.capacity = 0;
    str.success = 'f';

    str.string = malloc(capacity * sizeof(char));
    if (!str.string) {
        return str;
    }

    str.capacity = capacity;
    str.string[0] = '\0';
    str.success = 's';

    return str;
}
#define getFileString() getFileString(BASE_FILE_STRING_SIZE)

void writeToFileString(fileString* str, char* value){
    size_t len = strlen(value);

    if (str->size + len + 1 > str->capacity) {
        size_t newCapacity = str->capacity;

        while (str->size + len + 1 > newCapacity) {
            newCapacity += FILE_STRING_STEP_SIZE;
        }

        char* newString = realloc(str->string, newCapacity);
        if (!newString) return;

        str->string = newString;
        str->capacity = newCapacity;
    }

    memcpy(str->string + str->size, value, len);
    str->size += len;
    str->string[str->size] = '\0';
}

void freeFileString(fileString* str){
    free(str->string);
}

fileString CL_ReadFile(char* path){
    FILE *file;
    fileString fileString = getFileString();
    if(fileString.success == 'f') return fileString;
    fileString.success = 'f';

    file = fopen(path, "r");
    if(!file) return fileString;

    char readFileBuffer[READ_FILE_BUFFER_SIZE];

    while(fgets(readFileBuffer, READ_FILE_BUFFER_SIZE, file)) {
        writeToFileString(&fileString, readFileBuffer);
    }

    fclose(file);
    writeToFileString(&fileString, "\n\0");
    fileString.success = 's';
    return fileString;
}
#endif