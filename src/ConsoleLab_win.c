#include <windows.h>

typedef struct clWindow{
    //windows
    HANDLE hOut;
    HANDLE hIn;

};

typedef struct clContext{
    clOS os;
    clMode mode;
    clState state;
    clResult result;
    clWindow window;
    void* osContext;
};

//functions implemetations here
