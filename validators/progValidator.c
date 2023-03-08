//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "progValidator.h"
#include "fileValidator.h"

#define FILE_NOT_FOUND 0
#define SOURCE_FILES_NOT_PROVIDED 0

int validateProgramArgs(int argc, char **argv) {
    int i, result;
    char *fileName;

    result = 1;
    if (argc < 2) {
        fprintf(stderr, "No source files found\n");
        return SOURCE_FILES_NOT_PROVIDED;
    }

    for (i = 1; i < argc; i++) {
        fileName = (char *) calloc(strlen(argv[i]) + 3, sizeof(char));
        strcpy(fileName, argv[i]);
        strcat(fileName, ".as");
        if (!isFileExist(fileName)) {
            result = FILE_NOT_FOUND;
        }
        free(fileName);
    }
    return result;
}

