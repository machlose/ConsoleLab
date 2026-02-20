#pragma once
#include "vectors/vec_int.c"
#include "vectors/vec_float.c"
#include "vectors/vector_int.c"

// union robi że dane współdzielą miejsce w pamięci dzięki czemu np. vec2.x == vec2.i[0] == vec2.w == vec2.width
// typedef union {
//     struct {int x; int y;};
//     struct {int w; int h;};
//     struct {int width; int height;};
//     int i[2];
// } vec2;


// #include <stdint.h>

// typedef union {
//     struct {uint8_t r; uint8_t g; uint8_t b;};
//     uint8_t i[3];
// } rgb;

// typedef union {
//     struct {uint8_t r; uint8_t g; uint8_t b; uint8_t a;};
//     uint8_t i[4];
// } rgba;