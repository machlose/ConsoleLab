#pragma once

// union robi że dane współdzielą miejsce w pamięci dzięki czemu np. vec2.x == vec2.i[0] == vec2.w == vec2.width
typedef union {
    struct {int x; int y;};
    struct {int w; int h;};
    struct {int width; int height;};
    int i[2];
} vec2;

typedef union {
    struct {int x; int y; int z;};
    struct {int w; int h; int d;};
    struct {int r; int g; int b;};
    struct {int width; int height; int depth;};
    int i[3];
} vec3;

typedef union {
    struct {int x; int y; int z; int w;};
    struct {int r; int g; int b; int a;};
    int i[4];
} vec4;

typedef union {
    struct {float x; float y;};
    struct {float w; float h;};
    struct {float width; float height;};
    float i[2];
} vec2f;

typedef union {
    struct {float x; float y; float z;};
    struct {float w; float h; float d;};
    struct {float r; float g; float b;};
    struct {float width; float height; float depth;};
    float i[3];
} vec3f;

typedef union {
    struct {float x; float y; float z; float w;};
    struct {float r; float g; float b; float a;};
    float i[4];
} vec4f;

// #include <stdint.h>

// typedef union {
//     struct {uint8_t r; uint8_t g; uint8_t b;};
//     uint8_t i[3];
// } rgb;

// typedef union {
//     struct {uint8_t r; uint8_t g; uint8_t b; uint8_t a;};
//     uint8_t i[4];
// } rgba;