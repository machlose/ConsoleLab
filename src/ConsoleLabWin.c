#ifndef CONSOLELAB_IMPLEMENTATION
#define CONSOLELAB_IMPLEMENTATION
#define CL_IMPL_VERSION_MAJOR 0
#define CL_IMPL_VERSION_MINOR 0
#include "types.c"
// #include "ConsoleLab.h"
#ifdef _WIN32
typedef enum clState {
    working
} clState;

typedef enum clResult {
    success,
    failure
} clResult;

typedef enum clOS {
    windows,
    unix
} clOS;

typedef struct clContext{
    clOS os;
    clState state;
    clResult result;
} clContext;

void cl_context_create(clContext* context, clResult* result){
    #ifdef _WIN32
        context->os = windows;
    #else
        context->os = unix
    #endif
    // allocate everything
    result = success;
}

void cl_context_destroy(clContext* context){
    // free everything
}

#endif
#endif