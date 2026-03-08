#ifndef CONSOLELAB

typedef struct {
    
} ConsoleLabContext;

ConsoleLabContext* ConsoleLabGlobalContext = nullptr;

void ConsoleLabInit(ConsoleLabContext* context){
    if(!context){
         
    }
}

#else 
#define CONSOLELAB
#endif