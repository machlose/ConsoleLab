#pragma once
#include "lib.c"
#include "vectors.c"
#include "console/console.h"

typedef struct {
    vec2 (*vec2)(int, int);
} ConsoleLabAPI;

ConsoleLabAPI ConsoleLab;

void ConsoleLab_Init(){
    // ConsoleLab;

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