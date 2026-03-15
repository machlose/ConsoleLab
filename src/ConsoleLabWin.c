#ifndef CONSOLELAB_IMPLEMENTATION
#define CONSOLELAB_IMPLEMENTATION
// #include "ConsoleLab.h"
#ifdef _WIN32
typedef enum clState {
    working
} clState;

typedef enum clResult {
    success,
    failure
} clResult;

typedef struct clContext{
    clState state;
    clResult result;
} clContext;

void cl_context_create(clContext* context, clResult* result){
    // allocate everything
    result = success;
}

void cl_context_destroy(clContext* context){
    // free everything
}

#endif
#endif