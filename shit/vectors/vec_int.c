#pragma once

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

vec2 vec2_init(vec2 vec, int x, int y){
    vec.x = x;
    vec.y = y;
}

vec2 vec2_add(vec2 a, vec2 b){
    vec2 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

vec2 vec2_sub(vec2 a, vec2 b){
    vec2 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

vec2 vec2_mul(vec2 a, vec2 b){
    vec2 c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    return c;
}

vec2 vec2_div(vec2 a, vec2 b){
    vec2 c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    return c;
}

vec3 vec3_add(vec3 a, vec3 b){
    vec3 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

vec3 vec3_sub(vec3 a, vec3 b){
    vec3 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

vec3 vec3_mul(vec3 a, vec3 b){
    vec3 c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    return c;
}

vec3 vec3_div(vec3 a, vec3 b){
    vec3 c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
    return c;
}

vec4 vec4_add(vec4 a, vec4 b){
    vec4 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = a.w + b.w;
    return c;
}

vec4 vec4_sub(vec4 a, vec4 b){
    vec4 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = a.w - b.w;
    return c;
}

vec4 vec4_mul(vec4 a, vec4 b){
    vec4 c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    c.w = a.w * b.w;
    return c;
}

vec4 vec4_div(vec4 a, vec4 b){
    vec4 c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
    c.w = a.w / b.w;
    return c;
}
