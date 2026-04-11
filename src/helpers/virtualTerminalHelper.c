#include "../ConsoleLab.h" 
#define ESC "\x1b"
#define CSI "\x1b["

void GetConsoleTextColorDefaultString(char* buffer){
    strcpy(buffer,CSI "0m");
}
void GetConsoleTextColorDefaultForegroundString(char* buffer){
    strcpy(buffer,CSI "39m");
}
void GetConsoleTextColorDefaultBackgroundString(char* buffer){
    strcpy(buffer,CSI "49m");
}
void GetConsoleTextForegroundColorString(char* buffer,RGBA color){
    sprintf(buffer,CSI "38;2;%g;%g;%gm",color.r,color.g,color.b);
}
void GetConsoleTextBackgroundColorString(char* buffer,RGBA color){
    sprintf(buffer,CSI "48;2;%g;%g;%gm",color.r,color.g,color.b);
}