/*
 Created by Iluz, Lisaf(AWF) on 21/02/2023.
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fileValidator.h"

int isFileExist(char *fileName) {
    FILE *file;
    int isExist = 1;

    file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "%s %s\n", strerror(errno), fileName);
        isExist = 0;
    }
    fclose(file);
    return isExist;
}
