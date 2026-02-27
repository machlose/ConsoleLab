#include "ConsoleLab.c"
#include "test.c"
#include "helpers/array.c"
#include "UI/container.c"

int main() {

    ConsoleLabInit();
    color.red = 225;
    color.green = 134;
    color.blue = 227;

    element a;
    element b;
    for(int i = 0; i < MAX_CHILDREN; i++){
        printf("%p ",a.children[i]);
    }
    printf("\n");
    addChild(&a,&b);
    for(int i = 0; i < MAX_CHILDREN; i++){
        printf("%p ",a.children[i]);
    }
    printf("\n");
    removeChild(&a,&b);
    for(int i = 0; i < MAX_CHILDREN; i++){
        printf("%p ",a.children[i]);
    }
    printf("\n");

    while(true){
        // ConsoleGetScreenSize(&ConsoleLab.Console);
        // ConsoleHandleEvents(&ConsoleLab.Console);
        // Sleep(1000/501);
    }
    return 0;
}