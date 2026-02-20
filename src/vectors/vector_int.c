#pragma once
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
    int step;
} vector_int;

void vector_int_malloc(vector_int *vec){
    vec->data = malloc(sizeof(int) * vec->capacity);
}

void vector_int_free(vector_int *vec){
    free(vec->data);
}

void vector_int_copy(vector_int *vec, const vector_int *other){
    if(vec->capacity >= other->size){
        vec->size = other->size;
        memcpy(vec->data, other->data, other->size * sizeof(int));
    }
}

vector_int vector_int_init(int capacity){
    vector_int vec;
    vec.capacity = capacity;
    vec.size = 0;
    vec.step = 1024;
    vector_int_malloc(&vec);
    return vec;
}

void vector_int_change_step(vector_int *vec, int step){
    vec->step = step;
}

void vector_int_resize(vector_int *vec, int size){
    if(size >= vec->size){
        vector_int new = vector_int_init(size);
        vector_int_copy(&new, vec);
        vector_int_free(vec);
        vec = &new;
    }
}

void vector_int_push_back(vector_int *vec, int x){
    if (vec->size < vec->capacity){
        vec->data[vec->size] = x;
        vec->size++;
    } else{
        vector_int_resize(vec, vec->capacity + vec->step);
        vec->data[vec->size] = x;
        vec->size++;
    }
}

void vector_int_pop_back(vector_int *vec){
    if (vec->size > 0){
        vec->size--;
    }
}

// vector get_vector(){
//     vector vec;
//     return vec;
// }


// vector Vector;
// Vector.get = get_vector; // must be in func

