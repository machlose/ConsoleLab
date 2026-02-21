#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "vectors.c"
#include "console/console.h"

typedef struct {
    ConsoleLabVectorAPI Vector;
    vec2 (*vec2)(int, int);
} ConsoleLabAPI;

ConsoleLabAPI ConsoleLab;

void ConsoleLab_Init(){
    ConsoleLabVectorAPI Vector;
    Vector.create = vector_create;
    Vector.free = vector_free;
    Vector.erase = vector_erase;
    Vector.malloc = vector_malloc;
    Vector.copy = vector_copy;
    Vector.resize = vector_resize;
    Vector.push_back = vector_push_back;
    Vector.pop_back = vector_pop_back;
    Vector.delete = vector_delete;
    Vector.delete_range = vector_delete_range;
    Vector.set = vector_set;
    Vector.insert = vector_insert;
    ConsoleLab.Vector = Vector;

    // vec2 v = console.vec2(5, 10);
    // printf("v: %d %d\n", v.x, v.y);

    // vec4 b;
    // b.i[0] = 1;
    // 1[b.i] = 0;
    // b.z = 255;
    // b.i[3] = 256;
    // printf("r: %d, g: %d, b:%d, a:%d\n", b.x, b.g, 2[b.i], b.a);
    // example();

    // vector_test();


    printf("ConsoleLab init complete\n");
}