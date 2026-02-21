#pragma once
#include "lib.c"
#include "vectors.c"

typedef struct {
    vec2 (*vec2)(int, int);
} ConsoleLab;

//Potestuj se wektora

void example(){
    vector vec = vector_create(1024, sizeof(int));
    ((int*)vec.data)[2] = 67;
    int x = ((int*)vec.data)[2];
    ((int*)vec.data)[2] = 42;
    printf("%d\n", x);
    x = ((int*)vec.data)[2];
    printf("%d\n", x);
    vector_free(&vec);
}

void vector_test() {
    printf("=== TEST VECTOR ===\n");

    vector vec = vector_create(4, sizeof(int));

    // push_back
    for (int i = 0; i < 10; i++) {
        vector_push_back(&vec, &i);
    }

    printf("Po push_back (size=%zu):\n", vec.size);
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");

    // set
    int value = 999;
    vector_set(&vec, 2, &value);

    printf("Po vector_set(index 2 = 999):\n");
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");

    // delete
    vector_delete(&vec, 3);

    printf("Po vector_delete(index 3):\n");
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");

    // pop_back
    vector_pop_back(&vec);

    printf("Po vector_pop_back:\n");
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");

    vector_free(&vec);
    printf("=== KONIEC TESTU ===\n");
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

    vector_test();
    printf("aaa");
    return console;
}