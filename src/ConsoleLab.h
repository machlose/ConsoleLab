#ifndef CONSOLELAB_H
#define CONSOLELAB_H
#define CL_VERSION_MAJOR 0
#define CL_VERSION_MINOR 0
#include "ConsoleLabFlags.c" // delete later
#include "types.c"
#ifdef _WIN32
#include <windows.h>
#else
//unix includes
#endif
#ifdef __cplusplus
extern "C" {
#endif
clContext* cl_global_context;

//https://www.lookuptables.com/text/extended-ascii-table

// example for every function
// void cl_(clContext* context, clResult* result);
// #define cl_(...) cl_(cl_global_context, &cl_global_context->result, __VA_ARGS__)
// #define cl_(result, ...) cl_(cl_global_context, result, __VA_ARGS__)

void cl_create_context(clContext* context, clResult* result);
#define cl_create_context() cl_create_context(cl_global_context, &cl_global_context->result)
#define cl_create_context(result) cl_create_context(cl_global_context, result)
void cl_destroy_context(clContext* context);
#define cl_destroy_context() cl_destroy_context(cl_global_context)
void cl_create_window(clContext* context, clResult* result);
void cl_destroy_window(clContext* context);
void cl_get_window_handle(clContext* context, clResult* result);
void cl_draw_sprite(clContext* context, clResult* result);


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