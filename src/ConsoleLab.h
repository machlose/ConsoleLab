#ifndef CONSOLELAB
#define 



typedef enum {
    working
} clState;

typedef struct {
    clState state;    
} ConsoleLabContext;

ConsoleLabContext ConsoleLabCreateContext;

#else 
#define CONSOLELAB
#endif