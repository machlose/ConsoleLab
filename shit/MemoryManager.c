#include <stdlib.h>

typedef struct {
    void* pointers;
    void* (*malloc)(size_t bytes);
    void (*free)();
} MemoryManagerAPI;


void* clmalloc(size_t bytes){
    void* ptr = malloc(bytes);
    return ptr;
}

void clfree(void* ptr){
    free(ptr);
}

MemoryManagerAPI MemoryManager = {
    .malloc = clmalloc,
    .free = clfree
};