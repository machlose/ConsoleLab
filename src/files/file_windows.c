#pragma once
#include "file.h"

static const char* clscanignore[] = {
    ".git",
    ".svn",
    ".hg",
    "node_modules",
    "__pycache__",
    ".vs",
    ".idea",
    "build",
    "bin",
    "obj",
    NULL
};

static int is_ignored(const char* name) {
    for (int i = 0; clscanignore[i] != NULL; i++) {
        if (strcmp(name, clscanignore[i]) == 0)
            return 1;
    }
    return 0;
}

void scan_r(const char* basePath, filePath** files, size_t* count, int* capacity) {
    char searchPath[MAX_PATH_LEN];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", basePath);

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    while (FindNextFile(hFind, &findData)) {
        if (strcmp(findData.cFileName, ".") == 0 ||
            strcmp(findData.cFileName, "..") == 0)
            continue;

        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            is_ignored(findData.cFileName))
            continue;

        char fullPath[MAX_PATH_LEN];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", basePath, findData.cFileName);

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            scan_r(fullPath, files, count, capacity);
            continue;
        }

        if (*count >= *capacity) {
            size_t newCapacity = (*capacity) * 2;
            filePath* newFiles = realloc(*files, newCapacity * sizeof(filePath));
            if (newFiles == NULL) {
                printf("Błąd alokacji pamięci\n");
                FindClose(hFind);
                exit(1);
            }
            *files = newFiles;
            *capacity = newCapacity;
        }
        memset(&(*files)[*count], 0, sizeof(filePath));

        strncpy((*files)[*count].name, findData.cFileName, MAX_NAME - 1);
        strncpy((*files)[*count].path, fullPath, MAX_PATH_LEN - 1);
        (*files)[*count].size = ((size_t)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
        // (*files)[*count].createdAt  = findData.ftCreationTime;   // czas utworzenia pliku
        // (*files)[*count].modifiedAt = findData.ftLastWriteTime;  // czas ostatniej modyfikacji zawartości
        // (*files)[*count].accessedAt = findData.ftLastAccessTime; // czas ostatniego odczytu (często niewiarygodny)
        // (*files)[*count].isReadOnly = (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)  != 0; // czy plik jest tylko do odczytu
        // (*files)[*count].isHidden   = (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    != 0; // czy plik jest ukryty (np. .gitignore)
        // (*files)[*count].isSystem   = (findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)    != 0; // czy plik systemowy (nie ruszać)
        // (*files)[*count].isArchive  = (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)   != 0; // czy plik był zmodyfikowany od ostatniego backupu
        // (*files)[*count].isTemp     = (findData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0; // czy plik tymczasowy (może być usunięty)
        // (*files)[*count].isCompressed  = (findData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)  != 0; // czy plik jest skompresowany przez NTFS
        // (*files)[*count].isEncrypted   = (findData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)   != 0; // czy plik jest zaszyfrowany przez EFS
        // (*files)[*count].isReparsePoint= (findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)!= 0; // czy to symlink lub junction point

        (*count)++;
    }

    FindClose(hFind);
}

void printFiles(fileArray* files){
    for (int i = 0; i < files->length; i++) {
        printf("index: %d, size: %d\n", i, files->files[i].size);
        printf("Nazwa: %s\n", files->files[i].name);
        printf("Sciezka: %s\n\n", files->files[i].path);
    }
}

fileArray scan(const char* path){
    fileArray files;
    files.files = calloc(MAX_FILES, sizeof(filePath));
    files.length = 0;
    int capacity = MAX_FILES;

    scan_r(path, &files.files, &files.length, &capacity);

    // printFiles(&files);

    return files;
}