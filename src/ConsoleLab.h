#ifndef CONSOLELAB_H
#define CONSOLELAB_H

typedef enum clState clState;
typedef enum clState {
    working
} clState;

typedef enum clResult clResult;
typedef enum clResult {
    success,
    failure
} clResult;

typedef struct clContext clContext;
typedef struct clContext{
    clState state;    
} clContext;

clContext* cl_global_context;

void cl_context_create(clContext* context, clResult* result);
#define cl_context_create(...) cl_context_create(cl_global_context, &cl_global_context->result, __VA_ARGS__)
#define cl_context_create(result, ...) cl_context_create(cl_global_context, result, __VA_ARGS__)
void cl_context_destroy(clContext* context, clResult* result);

#else 
#define CONSOLELAB
#endif