#pragma once
#include <stdlib.h>

void arrayDelete(void* array,size_t arraySize,int sizeOfType, size_t index){
    if(index < arraySize && arraySize > 0){
        memmove(array + (index * sizeOfType), array + ((index+1) * sizeOfType), (arraySize - index - 1) * sizeOfType);
    }
}