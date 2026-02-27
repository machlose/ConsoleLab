#pragma once
#include "file.h"

void scan_r(const char* basePath, filePath** files, int *count, int *capacity) {
    char searchPath[MAX_PATH_LEN];
    snprintf(searchPath, MAX_PATH_LEN, "%s\\*", basePath);

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do {
        // pomiń . i ..
        if (strcmp(findData.cFileName, ".") == 0 ||
            strcmp(findData.cFileName, "..") == 0)
            continue;

        char fullPath[MAX_PATH_LEN];
        snprintf(fullPath, MAX_PATH_LEN, "%s\\%s", basePath, findData.cFileName);

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // rekurencja dla podfolderu
            scan_r(fullPath, files, count, capacity);
        } else {
            // powiększ tablicę jeśli trzeba
            if (*count >= *capacity) {
                *capacity *= 2;
                *files = realloc(*files, (*capacity) * sizeof(filePath));
                if (*files == NULL) {
                    printf("Błąd alokacji pamięci\n");
                    exit(1);
                }
            }

            strncpy((*files)[*count].name, findData.cFileName, MAX_NAME);
            strncpy((*files)[*count].path, fullPath, MAX_PATH_LEN);

            (*count)++;
        }

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}

filePath* scan(const char* path){
    filePath *files = malloc(MAX_FILES * sizeof(filePath));
    int count = 0;
    int capacity = MAX_FILES;

    scan_r(path, &files, &count, &capacity);

    for (int i = 0; i < count; i++) {
        printf("Nazwa: %s\n", files[i].name);
        printf("Sciezka: %s\n\n", files[i].path);
    }

    free(files);
    return 0;
}