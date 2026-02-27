#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 255
#define MAX_PATH_LEN 1024
#define MAX_FILES 128

typedef struct {
    char name[MAX_NAME];
    char path[MAX_PATH_LEN];
} filePath;

typedef struct {
    filePath* files;
    size_t length;
} fileArray;

void freeFileArray(fileArray* arr){
    free(arr->files);
}

filePath* scan(const char* path);

#ifdef _WIN32
#include "file_windows.c"
#else
#include "file_unix.c"
#endif