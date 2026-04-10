typedef struct clMemoryArena {
    void* base;
    size_t size;
    size_t offset;
} clMemoryArena;

void cl_arena_init(clMemoryArena* arena, size_t size) {
    arena->base = malloc(size);
    arena->size = size;
    arena->offset = 0;
}

void* cl_arena_alloc(clMemoryArena* arena, size_t sz) {
    if(arena->offset + sz > arena->size) return NULL; // brak miejsca
    void* ptr = (char*)arena->base + arena->offset;
    arena->offset += sz;
    return ptr;
}

void cl_arena_free_all(clMemoryArena* arena) {
    arena->offset = 0; // po prostu resetujemy wskaźnik
}

void cl_arena_destroy(clMemoryArena* arena) {
    free(arena->base);
    arena->base = NULL;
    arena->size = arena->offset = 0;
}