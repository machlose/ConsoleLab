#pragma once
#include <stdlib.h>

typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t step;
    size_t element_size; // liczba bajtów np sizeof(int) = 4 jesli chcesz inta itp
} vector;

// void example(){
//     vector vec = vector_create(1024, sizeof(int));
//     ((int*)vec.data)[2] = 67;

//     int *p = (int*)vector_get(&vec, 2);
//     printf("%d\n", *p);
// }
// nie kompiluj z tym bo ci nie znajdzie funkcji

void vector_malloc(vector *vec){
    vec->data = malloc(vec->element_size * vec->capacity);
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

vector vector_create(size_t capacity, size_t element_size){
    vector vec;
    vec.capacity = capacity;
    vec.size = 0;
    vec.step = 1024;
    vec.element_size = element_size;
    vector_malloc(&vec);
    return vec;
}

void vector_init(vector *vec, size_t capacity, size_t element_size){
    vec->capacity = capacity;
    vec->size = 0;
    vec->step = 1024;
    vec->element_size = element_size;
    vector_malloc(vec);
}

void vector_change_step(vector *vec, size_t step){
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

void vector_resize(vector *vec, size_t size){
    if(size > vec->size){
        vec->data = realloc(vec->data, size * vec->element_size);
    }
}

void vector_push_back(vector *vec, void *value){
    if (vec->size < vec->capacity){
        // vec->data[vec->size] = x;
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

//wypierdala program XD
// void* vector_get(vector *vec, size_t index){
//     if(index < vec->size){
//         return (char*)vec->data + index * vec->element_size;
//     } else{
//         return NULL;
//     }
// }

void vector_set(vector *vec, size_t index, void* value){
    memcpy((char*)vec->data + index * vec->element_size, value, vec->element_size);
}

void vector_insert(vector *vec, size_t index, void* value){
    if(index < vec->size && vec->size+1 < vec->capacity){
        memmove((char*)vec->data + (index + 1) * vec->element_size, (char*)vec->data + index * vec->element_size, (vec->size - index) * vec->element_size);
        memcpy((char*)vec->data + index * vec->element_size, value, vec->element_size);
    }
}