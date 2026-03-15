#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#include "ConsoleLabFlags.c" // delete later
#ifdef _WIN32
#include <windows.h>
#else
//unix includes
#endif
#ifdef __cplusplus
extern "C" {
#endif
typedef enum clState clState;
typedef enum clResult clResult;
typedef struct clContext clContext;

clContext* cl_global_context;

// example for every function
// void cl_context_create(clContext* context, clResult* result);
// #define cl_context_create(...) cl_context_create(cl_global_context, &cl_global_context->result, __VA_ARGS__)
// #define cl_context_create(result, ...) cl_context_create(cl_global_context, result, __VA_ARGS__)

void cl_context_create(clContext* context, clResult* result);
#define cl_context_create() cl_context_create(cl_global_context, &cl_global_context->result)
#define cl_context_create(result) cl_context_create(cl_global_context, result)
void cl_context_destroy(clContext* context);
#define cl_context_create() cl_context_create(cl_global_context)


#ifdef __cplusplus
}
#endif
#ifdef CL_AUTO_INCLUDE
#ifdef _WIN32
#include "ConsoleLabWin.c"
#else
#include "ConsoleLabUNIX.c"
#endif
#endif
#endif