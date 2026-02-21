#pragma once
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t step;
    size_t element_size; // liczba bajtów np sizeof(int) = 4 jesli chcesz inta itp
} vector;

int vector_malloc(vector *vec){
    vec->data = malloc(vec->element_size * vec->capacity);
    return vec->data != NULL; 
}

void vector_free(vector *vec){
    free(vec->data);
}

void vector_copy(vector *vec, const vector *other){
    if(vec->capacity >= other->size){
        vec->size = other->size;
        vec->element_size = other->element_size;
        memcpy(vec->data, other->data, other->size * other->element_size);
    }
}

int vector_init(vector *vec, size_t capacity, size_t element_size){
    vec->capacity = capacity;
    vec->size = 0;
    vec->step = 1024;
    vec->element_size = element_size;
    return vector_malloc(vec);
}

vector vector_create(size_t capacity, size_t element_size){
    vector vec;
    vector_init(&vec, capacity, element_size);
    return vec;
}

void vector_change_step(vector *vec, size_t step){//useles
    vec->step = step;
}

// void vector_resize(vector *vec, size_t size){
//     if(size >= vec->size){
//         vector new = vector_create(size, vec->element_size);
//         vector_copy(&new, vec);
//         vector_free(vec);
//         vec = &new;
//     }
// }

int vector_resize(vector *vec, size_t new_size){
    if(new_size > vec->size){
        void* new_data = realloc(vec->data, new_size * vec->element_size);
        if(new_data){
            vec->data = new_data;
            vec->capacity = new_size;
            return true;
        }
        return false;
    }
}

void vector_push_back(vector *vec, void *value){
    if (vec->size < vec->capacity){
        memcpy((char*)vec->data + vec->size * vec->element_size, value, vec->element_size);
        vec->size++;
    } else{
        vector_resize(vec, vec->capacity + vec->step);
        memcpy((char*)vec->data + vec->size * vec->element_size, value, vec->element_size);
        vec->size++;
    }
}

void vector_pop_back(vector *vec){
    if (vec->size > 0){
        vec->size--;
    }
}

void vector_delete(vector *vec, size_t index){
    if(index < vec->size && vec->size > 0){
        memmove((char*)vec->data + index * vec->element_size, (char*)vec->data + (index + 1) * vec->element_size, (vec->size - index - 1) * vec->element_size);
        vec->size--;
    }
}

void vector_delete_range(vector *vec, size_t index, size_t elements){
    if(index < vec->size && vec->size >= elements && elements > 0){
        memmove((char*)vec->data + index * vec->element_size, (char*)vec->data + (index + elements) * vec->element_size, (vec->size - index - elements) * vec->element_size);
        vec->size--;
    }
}

void vector_erase(vector *vec){
    vector_delete_range(vec, 0, vec->size);
}

//wypierdala program XD
// void* vector_get(vector *vec, size_t index){
//     if(index < vec->size){
//         return (char*)vec->data + index * vec->element_size;
//     } else{
//         return NULL;
//     }
// }

void vector_set(vector *vec, size_t index, void* value){
    if(index < vec->size){
        memcpy((char*)vec->data + index * vec->element_size, value, vec->element_size);
    }
}

void vector_insert(vector *vec, size_t index, void* value){
    if(index < vec->size && vec->size+1 < vec->capacity){
        memmove((char*)vec->data + (index + 1) * vec->element_size, (char*)vec->data + index * vec->element_size, (vec->size - index) * vec->element_size);
        memcpy((char*)vec->data + index * vec->element_size, value, vec->element_size);
        vec->size++;
    }
}

typedef struct {
    vector (*create)(size_t, size_t);
    void (*free)(vector*);
    void (*erase)(vector*);
    int (*malloc)(vector*);
    void (*copy)(vector*, const vector*);
    int (*resize)(vector*, size_t);
    void (*push_back)(vector*, void*);
    void (*pop_back)(vector*);
    void (*delete)(vector*, size_t);
    void (*delete_range)(vector*, size_t, size_t);
    void (*set)(vector*, size_t, void*);
    void (*insert)(vector*, size_t, void*);
    // int (*init)(vector *, size_t, size_t);
} ConsoleLabVectorAPI;