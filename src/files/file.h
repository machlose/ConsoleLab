#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_NAME 255
#define MAX_PATH_LEN 1024
#define MAX_FILES 128

//TEPM
#include "read_file_c.c"
#include "get_current_dir.c"

typedef struct {
    char name[MAX_NAME];
    char path[MAX_PATH_LEN];
    size_t size;
    // bool isReadOnly;
    // bool isHidden;   
    // bool isSystem;
} filePath;

typedef struct {
    filePath* files;
    size_t length;
} fileArray;

void freeFileArray(fileArray* arr){
    free(arr->files);
}

fileArray scan(const char* path);

void printFiles(fileArray* files);
// fileString readFile(char* path);

/* Shit and garbage
typedef struct {
    fileArray (*scan)(const char* path);
    void (*printFiles)(fileArray* files);
} ConsoleLabFileAPI;

void ConsoleLabFileInit(ConsoleLabFileAPI* File){
    File->scan = scan;
    File->printFiles = printFiles;
}*/

#ifdef _WIN32
#include "file_windows.c"
#else
#include "file_unix.c"
#endif