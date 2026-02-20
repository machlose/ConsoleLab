#pragma once
#include "lib.c"
#include "types.c"

typedef struct {
    vec2 (*vec2)(int, int);
} ConsoleLab;

ConsoleLab ConsoleLab_Init(){
    ConsoleLab console;

    // vec2 v = console.vec2(5, 10);
    // printf("v: %d %d\n", v.x, v.y);

    vec4 b;
    b.i[0] = 1;
    b.i[1] = 0;
    b.z = 255;
    b.i[3] = 256;
    printf("r: %d, g: %d, b:%d, a:%d\n", b.x, b.g, b.z, b.a);
    return console;
}