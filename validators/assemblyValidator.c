//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assemblyValidator.h"
#include "../enums/enums.h"
#include "../util/readerUtils.h"
#include "../io/errorsHandler.h"

#define SYMBOL_MAX_LEN 30
#define SYMBOL_MIN_LEN 1
#define TRUE 1
#define FALSE 0

int getNextTokenIndex(char *str, int fromIdx);

int isData(const char *line);

int validateDotDataRow(char *lineCopy, const char *fileName, int rowCounter);

int validateDotStringRow(char *lineCopy, int index, char *fileName, int rowCounter);

int validateSymbolName(char *symbol, char *fileName, int rowCounter) {
    int isValid;
    isValid = TRUE;
    if (strlen(symbol) > SYMBOL_MAX_LEN) {
        printError(SYMBOL_EXCEED_MAX_LEN, symbol, fileName, rowCounter);
        isValid = FALSE;
    } else if (strlen(symbol) < SYMBOL_MIN_LEN) {
        printError(SYMBOL_IS_EMPTY, symbol, fileName, rowCounter);
        isValid = FALSE;
    } else if (!isalpha(symbol[0]) || hasSpaces(symbol) || !isAlphaNumeric(symbol)) {
        printError(SYMBOL_SYNTAX_NAME, symbol, fileName, rowCounter);
        isValid = FALSE;
    }
    return isValid;
}

int validateDataGuidanceLine(char *line, const int symbolFlag, char *fileName, int rowCounter) {
    // ABC: .data -4, 3,4,1
    // .string      "ssss"
    int isValid, index;
    char *lineCopy;
    isValid = TRUE;
    index = 0;
    lineCopy = copyStr(line);
    if (symbolFlag) {
        index = getNextTokenIndex(lineCopy, index);
    }
    if (isData(lineCopy)) {
        index = getNextTokenIndex(lineCopy, index);
        isValid = validateDotDataRow(lineCopy, fileName, rowCounter);

    } else {
        index = getNextTokenIndex(lineCopy, index);
        isValid = validateDotStringRow(lineCopy, index, fileName, rowCounter);
    }
    free(lineCopy);
    return isValid;
}

int validateDotStringRow(char *lineCopy, int index, char *fileName, int rowCounter) {
    int isValid;
    size_t size;
    size = strlen(lineCopy);
    isValid = TRUE;
    if (lineCopy[0] != '"' || lineCopy[size - index] != '"') {
        isValid = FALSE;
        printError(NOT_A_VALID_STRING, lineCopy, fileName, rowCounter);
    }
    return isValid;
}

int validateDotDataRow(char *lineCopy, const char *fileName, int rowCounter) {
    int isValid, i, numberCounter;
    char *number;
    size_t size;
    size = strlen(lineCopy);
    numberCounter = 0;
    isValid = TRUE;
    number = (char *) malloc(size * sizeof(char));
    for (i = 0; i < size; i++) {
        if (isspace(lineCopy[i])) {
            i++;
        } else {
            if (lineCopy[i] == ',') {
                if (!isValidNumber(number)) {
                    isValid = FALSE;
                    printError(NOT_A_VALID_NUMBER, number, fileName, rowCounter);
                }
                free(number);
                numberCounter = 0;
                number = (char *) malloc((size - i) * sizeof(char));
            } else {
                number[numberCounter] = lineCopy[i];
                numberCounter++;
            }
        }
    }
    if (!isValidNumber(number)) {
        isValid = FALSE;
        printError(NOT_A_VALID_NUMBER, number, fileName, rowCounter);
    }
    free(number);
    return isValid;
}

int validateExternalGuidanceLine(char* line, int symbolFlag, char* fileName, int rowCounter) {

}

int isData(const char *line) {
    char *data = ".data";
    return line[0] == data[0] &&
           line[1] == data[1] &&
           line[2] == data[2] &&
           line[3] == data[3] &&
           line[4] == data[4];
}

int getNextTokenIndex(char *str, int fromIdx) {
    int i;
    i=fromIdx;
    while (!isspace(str[i])) i++;
    while (isspace(str[i])) i++;
    return i;
}
