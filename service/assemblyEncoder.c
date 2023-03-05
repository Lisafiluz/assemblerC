//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assemblyEncoder.h"
#include "../util/assemblyUtils.h"
#include "../util/readerUtils.h"

size_t saveString(char *line, short *dataArray, int dc);

size_t saveData(char *line, short *dataArray, int dc);

int saveGuidanceLine(char *line, int symbolFlag, short *dataArray, int dc) {
    size_t L;
    char *lineCopy, *pLine, *guidanceCommand;
    lineCopy = copyStr(line);
    pLine = lineCopy;
    if (symbolFlag) {
        lineCopy = getLineWithoutSymbol(lineCopy);
    }

    guidanceCommand = getToken(lineCopy, ' ', 0);
    if (isEqual(guidanceCommand, ".data")) {
        L = saveData(lineCopy, dataArray, dc);
    } else {
        L = saveString(lineCopy, dataArray, dc);
    }
    free(pLine);
    free(guidanceCommand);
    return (int) L;
}

size_t saveData(char *line, short *dataArray, int dc) {
    // .data -52, 53, +49
    int startDc, i, numberCounter;
    char *number;
    size_t size;

    startDc = dc;
    while (isspace(*line)) line++;
    while (!isspace(*line)) line++;
    size = strlen(line);
    numberCounter = 0;
    number = (char *) malloc(size * sizeof(char));
    for (i = 0; i < size; i++) {
        if (!isspace(line[i])) {
            if (line[i] == ',') {
                dataArray = realloc(dataArray, sizeof(short));
                dataArray[dc] = atoi(number);  // todo: verify 14 bits saving including negative numbers
                dc++;
                free(number);
                numberCounter = 0;
                number = (char *) malloc((size - i + 1) * sizeof(char));
            } else {
                number[numberCounter] = line[i];
                numberCounter++;
            }
        }
    }
    dataArray = realloc(dataArray, sizeof(short));
    dataArray[dc] = atoi(number);
    dc++;
    free(number);
    return dc - startDc;
}

size_t saveString(char *line, short *dataArray, int dc) {
    //.string "abcdef"
    char *string;
    int i, startCopy, stringIdx;
    startCopy = 0, stringIdx = 0;
    string = (char *) malloc(strlen(line) * sizeof(char));
    for (i = 0; i < strlen(line) - 1; ++i) {
        if (startCopy) {
            string[stringIdx] = line[i];
            stringIdx++;
        }
        if (line[i] == '"') {
            startCopy = 1;
        }
    }
    string[stringIdx] = '\0';
    dataArray = realloc(dataArray, strlen(string) + 1);
    i = 0;
    for (; i < strlen(string); i++) {
        dataArray[dc] = (short) string[i];
        dc++;
    }
    dataArray[dc] = 0;
    return strlen(string) + 1;
}
