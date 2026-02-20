#pragma once
#include "lib.c"
#include "vectors.c"

typedef struct {
    vec2 (*vec2)(int, int);
} ConsoleLab;

//Potestuj se wektora

void example(){
    vector vec = vector_create(1024, sizeof(int));
    printf("dupsko\n");
    ((int*)vec.data)[2] = 67;
    printf("dupsko\n");
    int x = ((int*)vec.data)[2];
    ((int*)vec.data)[2] = 42;
    printf("%d\n", x);
    printf("dupsko\n");
    vector_free(&vec);
}

ConsoleLab ConsoleLab_Init(){
    ConsoleLab console;

    // vec2 v = console.vec2(5, 10);
    // printf("v: %d %d\n", v.x, v.y);

    vec4 b;
    b.i[0] = 1;
    1[b.i] = 0;
    b.z = 255;
    b.i[3] = 256;
    printf("r: %d, g: %d, b:%d, a:%d\n", b.x, b.g, 2[b.i], b.a);
    example();
    return console;
}