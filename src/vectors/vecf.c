#pragma once

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

vec2f vec2f_add(vec2f a, vec2f b){
    vec2f c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

vec2f vec2f_sub(vec2f a, vec2f b){
    vec2f c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

vec2f vec2f_mul(vec2f a, vec2f b){
    vec2f c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    return c;
}

vec2f vec2f_div(vec2f a, vec2f b){
    vec2f c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    return c;
}

vec3f vec3f_add(vec3f a, vec3f b){
    vec3f c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

vec3f vec3f_sub(vec3f a, vec3f b){
    vec3f c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

vec3f vec3f_mul(vec3f a, vec3f b){
    vec3f c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    return c;
}

vec3f vec3f_div(vec3f a, vec3f b){
    vec3f c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
    return c;
}

vec4f vec4f_add(vec4f a, vec4f b){
    vec4f c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    c.w = a.w + b.w;
    return c;
}

vec4f vec4f_sub(vec4f a, vec4f b){
    vec4f c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    c.w = a.w - b.w;
    return c;
}

vec4f vec4f_mul(vec4f a, vec4f b){
    vec4f c;
    c.x = a.x * b.x;
    c.y = a.y * b.y;
    c.z = a.z * b.z;
    c.w = a.w * b.w;
    return c;
}

vec4f vec4f_div(vec4f a, vec4f b){
    vec4f c;
    c.x = a.x / b.x;
    c.y = a.y / b.y;
    c.z = a.z / b.z;
    c.w = a.w / b.w;
    return c;
}
