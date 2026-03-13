#ifndef CONSOLELAB


typedef enum {
    working
} clState;

typedef struct {
    clState state;    
} ConsoleLabContext;

ConsoleLabContext* ConsoleLabGlobalContext;

#else 
#define CONSOLELAB
#endif