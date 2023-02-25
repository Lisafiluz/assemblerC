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
#include "../io/messagesHandler.h"

#define SYMBOL_MAX_LEN 30
#define SYMBOL_MIN_LEN 1
#define TRUE 1
#define FALSE 0

int getNextTokenIndex(char *str, int fromIdx);

int isData(const char *line, int i);

int validateDotDataRow(char *arguments, const char *fileName, int rowCounter);

int validateDotStringRow(char *lineCopy, const char *argument, char *fileName, int rowCounter);

int validateSymbolName(char *symbol, linkedList* symbolsTable, char *fileName, int rowCounter) {
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
    else if (isIdExist(symbol, symbolsTable)) {
        printError(DUPLICATE_SYMBOL_NAME, symbol, fileName, rowCounter);
        isValid = FALSE;
    }
    return isValid;
}

int validateDataGuidanceLine(char *line, const int symbolFlag, char *fileName, int rowCounter) {
    int isValid, index;
    char *lineCopy, *startPtr;
    isValid = TRUE;
    index = 0;
    lineCopy = copyStr(line);
    startPtr = lineCopy;
    if (symbolFlag) {
        index = getNextTokenIndex(lineCopy, index);
    }
    if (isData(lineCopy, index)) {
        index = getNextTokenIndex(lineCopy, index);
        isValid = validateDotDataRow(&lineCopy[index], fileName, rowCounter);

    } else {
        index = getNextTokenIndex(lineCopy, index);
        isValid = validateDotStringRow(startPtr, &lineCopy[index], fileName, rowCounter);
    }
    free(lineCopy);
    return isValid;
}

int validateDotStringRow(char *lineCopy, const char *argument, char *fileName, int rowCounter) {
    int isValid;
    size_t size;
    size = strlen(lineCopy);
    isValid = TRUE;
    if (argument[0] != '"' || lineCopy[size - 1] != '"') {
        isValid = FALSE;
        printError(NOT_A_VALID_STRING, argument, fileName, rowCounter);
    }
    return isValid;
}

int validateDotDataRow(char *arguments, const char *fileName, int rowCounter) {
    int isValid, i, numberCounter;
    char *number;
    size_t size;
    size = strlen(arguments);
    numberCounter = 0;
    isValid = TRUE;
    number = (char *) malloc(size * sizeof(char));
    for (i = 0; i < size; i++) {
        if (isspace(arguments[i])) {
            i++;
        } else {
            if (arguments[i] == ',') {
                if (strlen(number) == 0) {
                    isValid = FALSE;
                    printError(EMPTY_NUMBER, number, fileName, rowCounter);
                } else if (!isValidNumber(number)) {
                    isValid = FALSE;
                    printError(NOT_A_VALID_NUMBER, number, fileName, rowCounter);
                }
                free(number);
                numberCounter = 0;
                number = (char *) malloc((size - i + 1) * sizeof(char));
            } else {
                number[numberCounter] = arguments[i];
                numberCounter++;
            }
        }
    }
    if (strlen(number) == 0) {
        isValid = FALSE;
        printError(EMPTY_NUMBER, number, fileName, rowCounter);
    } else if (!isValidNumber(number)) {
        isValid = FALSE;
        printError(NOT_A_VALID_NUMBER, number, fileName, rowCounter);
    }
    free(number);
    return isValid;
}

int validateExternalGuidanceLine(char *line, int symbolFlag, char *fileName, int rowCounter) {

}

int isData(const char *line, int i) {
    char *data = ".data";
    return line[i] == data[0] &&
           line[i + 1] == data[1] &&
           line[i + 2] == data[2] &&
           line[i + 3] == data[3] &&
           line[i + 4] == data[4];
}

int getNextTokenIndex(char *str, int fromIdx) {
    int i;
    i = fromIdx;
    while (!isspace(str[i])) i++;
    while (isspace(str[i])) i++;
    return i;
}
