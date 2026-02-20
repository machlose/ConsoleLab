#pragma once
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} vec2;

vec2 vec2_constructor(int x, int y){
    vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

typedef struct {
    vec2 (*vec2)(int, int);
} ConsoleLab;

ConsoleLab ConsoleLabInit(){
    ConsoleLab console;
    console.vec2 = vec2_constructor;

    vec2 v = console.vec2(5, 10);
    printf("v: %d %d\n", v.x, v.y);
    return console;
}