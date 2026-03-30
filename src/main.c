// #include "ConsoleLab.h"
#include "files/file.h"

int main(){
    char buffer[1028];
    getCurrentDir(buffer, 1028);
    fileArray files = scan(buffer);
    printf("%s\n", (files.files+1)->path);
    fileString file = CL_ReadFile((files.files+1)->path);
    printf("%s", file.string);
    
    freeFileString(&file);
    printf("\ndone\n");
    return 0;
}