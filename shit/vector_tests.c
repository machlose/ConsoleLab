
#include "vectors.c"

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

    for (int i = 0; i < 10; i++) {
        vector_push_back(&vec, &i);
    }

    printf("Po push_back (size=%zu):\n", vec.size);
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");

    int value = 999;
    vector_set(&vec, 2, &value);

    printf("Po vector_set(index 2 = 999):\n");
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");

    vector_delete(&vec, 3);

    printf("Po vector_delete(index 3):\n");
    for (size_t i = 0; i < vec.size; i++) {
        int *v = (int*)((char*)vec.data + i * vec.element_size);
        printf("%d ", *v);
    }
    printf("\n");


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
