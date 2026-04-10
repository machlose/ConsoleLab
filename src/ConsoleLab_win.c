#include <windows.h>

struct clWindow{
    //windows
    HANDLE hOut;
    HANDLE hIn;

};

struct clContext{
    clOS os;
    clMode mode;
    clState state;
    clResult result;
    clWindow window;
    void* osContext;
};

//functions implemetations here
