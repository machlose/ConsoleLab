#ifdef _WIN32
#ifndef CONSOLELAB_IMPLEMENTATION
#define CONSOLELAB_IMPLEMENTATION
#define CL_IMPL_VERSION_MAJOR 0
#define CL_IMPL_VERSION_MINOR 0
#include <windows.h>
#include "ConsoleLab.h"

typedef enum clState {
    working
} clState;

typedef enum clResult {
    success,
    context_malloc_failure,
    failure
} clResult;

typedef enum clOS {
    windows,
    unix
} clOS;

typedef enum clMode {
    console_mode,
} clMode;

typedef struct clWindow{
    //windows
    HANDLE hOut;
    HANDLE hIn;

} clWindow;

typedef struct clContext{
    clOS os;
    clMode mode;
    clState state;
    clResult result;
    clWindow window;
} clContext;

void cl_context_create(clContext* context, clResult* result){
    context = malloc(sizeof(clContext));
    if(!context){
        result = context_malloc_failure;
        return;
    }
    context->mode = console_mode;
    #ifdef _WIN32
        context->os = windows;
    #else
        context->os = unix
    #endif
    // allocate everything

    result = success;
}

void cl_context_destroy(clContext* context){
    free(context);
    // free everything
}

void cl_get_console_size(clContext* context, clResult* result){
    
}

void cl_get_console_handle(clContext* context, clResult* result){
    context->window.hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    context->window.hIn  = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(context->window.hIn, &mode);
    mode |=  ENABLE_MOUSE_INPUT;
    mode |=  ENABLE_WINDOW_INPUT;
    mode |=  ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(context->window.hIn, mode);

    DWORD modeOut;
    GetConsoleMode(context->window.hOut, &modeOut);
    modeOut |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(context->window.hOut, modeOut);

    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(context->window.hOut, &cci);
    cl_get_console_size(context, result);
}



#endif
#endif