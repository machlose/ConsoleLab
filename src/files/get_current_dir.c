#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

char* getCurrentDir(char* buffer, size_t size) {
    return getcwd(buffer, size);
}