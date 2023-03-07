//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assemblyEncoder.h"
#include "../util/assemblyUtils.h"
#include "../util/readerUtils.h"

#define TRUE 1
#define FALSE 0
#define OPERATIONS_SIZE 16
#define ADD_ABSOLUTE 0
#define ADD_EXTERNAL 1
#define ADD_RELOCATABLE 0 // ???2
#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define JUMP_ADDRESSING 2
#define REGISTER_ADDRESSING 3

size_t saveString(char *line, short *dataArray, int dc);

size_t saveData(char *line, short *dataArray, int dc);

int saveCode(char *command, int operationNumber, short *instructionsArray, int ic);

short addOperationCode(short operationNumber);

short addSourceAddressMethod(short sourceAddressMethod);

short addTargetAddressMethod(int targetAddressMethod);

int calculateWordsToAdd(int sourceAddressMethod, int targetAddressMethod);

int calculateWordsToAddInJumpArg(const char *jumpCommand);

short addSourceAndTargetParams(const char *jumpCommand);

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
    number = (char *) malloc((size + 1) * sizeof(char));
    for (i = 0; i < size; i++) {
        if (!isspace(line[i])) {
            if (line[i] == ',') {
                dataArray = realloc(dataArray, (dc + 1) * sizeof(short));
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
    dataArray = realloc(dataArray, (dc + 1) * sizeof(short));
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
    string = (char *) malloc((strlen(line) + 1) * sizeof(char));
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
    dataArray = realloc(dataArray, dc + strlen(string) + 1);

    for (i = 0; i < strlen(string); i++) {
        dataArray[dc] = (short) string[i];
        dc++;
    }
    dataArray[dc] = 0;
    return strlen(string) + 1;
}

int saveCodeLine(char *line, int symbolFlag, const char **operationsTable, short *instructionsArray, int ic) {
    char *lineCopy, *pCopy, *operationName;
    int i, isFound, L;
    isFound = FALSE;
    lineCopy = copyStr(line);
    pCopy = lineCopy;
    if (symbolFlag) {
        lineCopy = getLineWithoutSymbol(lineCopy);
    }
    operationName = getToken(lineCopy, ' ', 0);
    for (i = 0; i < OPERATIONS_SIZE && !isFound; ++i) {
        if (isEqual(operationName, operationsTable[i])) {
            L = saveCode(lineCopy, i, instructionsArray, ic);
            isFound = TRUE;
        }
    }

    free(operationName);
    free(pCopy);
    return L;
}

int saveCode(char *command, int operationNumber, short *instructionsArray, int ic) {
    int numberOfArgs, L;
    short binaryCommand;
    char *sourceArg, *targetArg;
    int sourceAddressMethod, targetAddressMethod;
    binaryCommand = 0, L = 1;
    numberOfArgs = getNumberOfArgs(command);
    if (numberOfArgs == 1) {
        targetArg = getArgument(command, 0);
        targetAddressMethod = getArgumentAddressMethod(targetArg);
        L += calculateWordsToAdd(-1, targetAddressMethod);
        if (targetAddressMethod == 1 || targetAddressMethod == 2) {
            binaryCommand += ADD_RELOCATABLE; //or external??
        } else {
            binaryCommand += ADD_ABSOLUTE;
        }
        binaryCommand |= addTargetAddressMethod(targetAddressMethod);

        if (targetAddressMethod == JUMP_ADDRESSING) {
            L += calculateWordsToAddInJumpArg(targetArg);
            binaryCommand |= addSourceAndTargetParams(targetArg);
        }

        free(targetArg);
    } else if (numberOfArgs == 2) {
        sourceArg = getArgument(command, 0);
        sourceAddressMethod = getArgumentAddressMethod(sourceArg);
        targetArg = getArgument(command, 1);
        targetAddressMethod = getArgumentAddressMethod(targetArg);
        L += calculateWordsToAdd(sourceAddressMethod, targetAddressMethod);
        if (sourceAddressMethod == 1 || sourceAddressMethod == 2 ||
            targetAddressMethod == 1 || targetAddressMethod == 2) {
            binaryCommand += ADD_RELOCATABLE; //or external??
        } else {
            binaryCommand += ADD_ABSOLUTE;
        }
        binaryCommand |= addSourceAddressMethod((short) sourceAddressMethod);
        binaryCommand |= addTargetAddressMethod((short) targetAddressMethod);
        if (targetAddressMethod == JUMP_ADDRESSING) {
            L += calculateWordsToAddInJumpArg(targetArg);
            binaryCommand |= addSourceAndTargetParams(targetArg);
        }

        free(sourceArg);
        free(targetArg);
    }
    binaryCommand |= addOperationCode((short) operationNumber);
    //instructionsArray = realloc(instructionsArray, sizeof(short));
    //instructionsArray[ic] = binaryCommand;

    return L;
}

short addSourceAndTargetParams(const char *jumpCommand) {
    char *param1, *param2;
    int param1AddressMethod, param2AddressMethod, binaryCommandToAdd;
    binaryCommandToAdd = 0;
    param1 = getParam(jumpCommand, 0);
    param2 = getParam(jumpCommand, 1);
    param1AddressMethod = getArgumentAddressMethod(param1);
    param2AddressMethod = getArgumentAddressMethod(param2);
    binaryCommandToAdd = param1AddressMethod << 12;
    binaryCommandToAdd += param2AddressMethod << 10;
    free(param1);
    free(param2);
    return (short) binaryCommandToAdd;
}

int calculateWordsToAddInJumpArg(const char *jumpCommand) {
    char *param1, *param2;
    int param1AddressMethod, param2AddressMethod, wordsToAdd;
    param1 = getParam(jumpCommand, 0);
    param2 = getParam(jumpCommand, 1);
    param1AddressMethod = getArgumentAddressMethod(param1);
    param2AddressMethod = getArgumentAddressMethod(param2);
    if (param1AddressMethod == REGISTER_ADDRESSING && param2AddressMethod == REGISTER_ADDRESSING) {
        wordsToAdd = 1;
    } else {
        wordsToAdd = 2;
    }
    free(param1);
    free(param2);
    return wordsToAdd;
}

int calculateWordsToAdd(int sourceAddressMethod, int targetAddressMethod) {
    if ((sourceAddressMethod == REGISTER_ADDRESSING && targetAddressMethod == REGISTER_ADDRESSING) ||
        sourceAddressMethod == -1) {
        return 1;
    }
    return 2;
}

short addTargetAddressMethod(int targetAddressMethod) {
    targetAddressMethod <<= 2;
    return (short)targetAddressMethod;
}

short addSourceAddressMethod(short sourceAddressMethod) {
    sourceAddressMethod <<= 4;
    return sourceAddressMethod;
}

short addOperationCode(short operationNumber) {
    operationNumber <<= 6;
    return operationNumber;
}
