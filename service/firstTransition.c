//
// Created by Iluz, Lisaf(AWF) on 21/02/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include "firstTransition.h"
#include "../util/readerUtils.h"
#include "../validators/fileValidator.h"

#define READ "r"
#define WRITE "w"
#define SOURCE_FILE_SUFFIX ".am"

int runFirstTransition(FILE *file);

int firstTransition(int argc, char **argv) {
    int isValid, i;
    char *fileName;
    FILE *sourceFile;

    isValid = 1;

    for (i = 1; i < argc; i++) {
        int isFileValid = 1;
        fileName = getFileName(argv, i, SOURCE_FILE_SUFFIX);

        if (isFileExist(fileName)) {
            sourceFile = fopen(fileName, READ);

            free(fileName);

            isFileValid = runFirstTransition(sourceFile);
            if(!isFileValid) {
                isValid = 0;
            }
            fclose(sourceFile);
        }
    }

    return isValid;
}

int runFirstTransition(FILE *file) {
    int isValid;
    isValid = 1;

    return isValid;
}
