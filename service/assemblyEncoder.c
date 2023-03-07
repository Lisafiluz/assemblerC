//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assemblyEncoder.h"
#include "../util/assemblyUtils.h"
#include "../util/readerUtils.h"
#include "../structs/list.h"
#include "../structs/shortData.h"

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

size_t saveString(char *line, linkedList *dataList, int dc);

size_t saveData(char *line, linkedList *dataList, int dc);

int saveCode(char *command, int operationNumber, linkedList *instructionsList, int ic);

short addOperationCode(short operationNumber);

short addSourceAddressMethod(short sourceAddressMethod);

short addTargetAddressMethod(int targetAddressMethod);

int calculateWordsToAdd(int sourceAddressMethod, int targetAddressMethod);

int calculateWordsToAddInJumpArg(const char *jumpCommand);

short addSourceAndTargetParams(const char *jumpCommand);

void addAdditionalWords(char *sourceArg, char *targetArg, linkedList *instructionsList);

int getRegisterNumber(const char *registerName);

short addSourceRegisterToWord(int num);

short addTargetRegisterToWord(int num);

short getImmediateNumber(const char *arg);

void addAdditionalWordsForJumpAddressing(const char *arg, linkedList *instructionsList);

void addParamWord(const char *param, int paramAddressMethod, linkedList *instructionsList);

void addRegistersWord(const char *reg1, const char *reg2, linkedList *instructionsList);

void addArgumentWord(const char *arg, int argMethodAddress, linkedList *instructionsList);

void *getSymbolNameFromJumpArgument(const char *arg);

int saveGuidanceLine(char *line, int symbolFlag, linkedList *dataList, int dc) {
    size_t L;
    char *lineCopy, *pLine, *guidanceCommand;
    lineCopy = copyStr(line);
    pLine = lineCopy;
    if (symbolFlag) {
        lineCopy = getLineWithoutSymbol(lineCopy);
    }

    guidanceCommand = getToken(lineCopy, ' ', 0);
    if (isEqual(guidanceCommand, ".data")) {
        L = saveData(lineCopy, dataList, dc);
    } else {
        L = saveString(lineCopy, dataList, dc);
    }
    free(pLine);
    free(guidanceCommand);
    return (int) L;
}

size_t saveData(char *line, linkedList *dataList, int dc) {
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
                //(*dataList) = realloc((*dataList), (dc + 1) * sizeof(short));
                add(createNewNode(NULL, createShortData((short) atoi(number))), dataList);
                //(*dataList)[dc] = atoi(number);  // todo: verify 14 bits saving including negative numbers
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
    //(*dataList) = realloc((*dataList), (dc + 1) * sizeof(short));
    //(*dataList)[dc] = atoi(number);
    add(createNewNode(NULL, createShortData((short) atoi(number))), dataList);
    dc++;
    free(number);
    return dc - startDc;
}

size_t saveString(char *line, linkedList *dataList, int dc) {
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
    //*dataList = realloc(*dataList, dc + strlen(string) + 1);

    for (i = 0; i < strlen(string); i++) {
        add(createNewNode(NULL, createShortData(string[i])), dataList);
        //(*dataList)[dc] = (short) string[i];
        dc++;
    }
    //(*dataList)[dc] = 0;
    add(createNewNode(NULL, createShortData(0)), dataList);
    return strlen(string) + 1;
}

int saveCodeLine(char *line, int symbolFlag, const char **operationsTable, linkedList *instructionsList, int ic) {
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
            L = saveCode(lineCopy, i, instructionsList, ic);
            isFound = TRUE;
        }
    }

    free(operationName);
    free(pCopy);
    return L;
}

int saveCode(char *command, int operationNumber, linkedList *instructionsList, int ic) {
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
        binaryCommand |= addOperationCode((short) operationNumber);
        add(createNewNode(NULL, createShortData(binaryCommand)), instructionsList);
        addAdditionalWords(NULL, targetArg, instructionsList);
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
        binaryCommand |= addOperationCode((short) operationNumber);
        add(createNewNode(NULL, createShortData(binaryCommand)), instructionsList);
        addAdditionalWords(sourceArg, targetArg, instructionsList);
        free(sourceArg);
        free(targetArg);
    } else {
        binaryCommand |= addOperationCode((short) operationNumber);
        add(createNewNode(NULL, createShortData(binaryCommand)), instructionsList);
    }
    return L;
}

void addAdditionalWords(char *sourceArg, char *targetArg, linkedList *instructionsList) {
    int sourceArgAddressMethod, targetArgAddressMethod;
    sourceArgAddressMethod = -1;
    if (sourceArg != NULL) {
        sourceArgAddressMethod = getArgumentAddressMethod(sourceArg);
    }
    targetArgAddressMethod = getArgumentAddressMethod(targetArg);

    if (sourceArgAddressMethod == REGISTER_ADDRESSING && sourceArgAddressMethod == targetArgAddressMethod) {
        // In this case both of the arguments are registers => only one word needed
        addRegistersWord(sourceArg, targetArg, instructionsList);
    } else {
        addArgumentWord(sourceArg, sourceArgAddressMethod, instructionsList);
        addArgumentWord(targetArg, targetArgAddressMethod, instructionsList);
    }
}

void addArgumentWord(const char *arg, int argMethodAddress, linkedList *instructionsList) {
    switch (argMethodAddress) {
        case JUMP_ADDRESSING:
            addAdditionalWordsForJumpAddressing(arg, instructionsList);
            break;
        case IMMEDIATE_ADDRESSING:
            add(createNewNode(NULL, createShortData(getImmediateNumber(arg))), instructionsList);
            break;
        case DIRECT_ADDRESSING:
            add(createNewNode(copyStr(arg), NULL), instructionsList);
            break;
        case REGISTER_ADDRESSING:
            //potential bug with the A,R,E
            add(createNewNode(NULL, createShortData(addSourceRegisterToWord(getRegisterNumber(arg)))),
                instructionsList);
            break;
        default:
            break;
    }
}

void addRegistersWord(const char *reg1, const char *reg2, linkedList *instructionsList) {
    int num1, num2;
    short word;
    word = 0;
    num1 = getRegisterNumber(reg1);
    num2 = getRegisterNumber(reg2);
    word |= addSourceRegisterToWord(num1);
    word |= addTargetRegisterToWord(num2);
    add(createNewNode(NULL, createShortData(word)), instructionsList);
}

void addAdditionalWordsForJumpAddressing(const char *arg, linkedList *instructionsList) {
    char *param1, *param2;
    int param1AddressMethod, param2AddressMethod;
    add(createNewNode(getSymbolNameFromJumpArgument(arg), NULL), instructionsList);

    param1 = getParam(arg, 0);
    param2 = getParam(arg, 1);
    param1AddressMethod = getArgumentAddressMethod(param1);
    param2AddressMethod = getArgumentAddressMethod(param2);

    if (param1AddressMethod == REGISTER_ADDRESSING && param2AddressMethod == REGISTER_ADDRESSING) {
        addRegistersWord(param1, param2, instructionsList);
    } else {
        addParamWord(param1, param1AddressMethod, instructionsList);
        addParamWord(param2, param2AddressMethod, instructionsList);
    }
    free(param1);
    free(param2);
}

void *getSymbolNameFromJumpArgument(const char *arg) {
    int i, symbolIdx;
    char *symbolName;
    symbolIdx = 0, i = 0;
    symbolName = (char *) malloc(strlen(arg) * sizeof(char));

    while (arg[i] != '(' && i < strlen(arg)) {
        symbolName[symbolIdx] = arg[i];
        symbolIdx++;
        i++;
    }
    symbolName[symbolIdx] = '\0';
    return symbolName;
}

void addParamWord(const char *param, int paramAddressMethod, linkedList *instructionsList) {
    switch (paramAddressMethod) {
        case IMMEDIATE_ADDRESSING:
            add(createNewNode(NULL, createShortData(getImmediateNumber(param))), instructionsList);
            break;
        case DIRECT_ADDRESSING:
            add(createNewNode(copyStr(param), NULL), instructionsList);
            break;
        case REGISTER_ADDRESSING:
            add(createNewNode(NULL, createShortData(addTargetRegisterToWord(getRegisterNumber(param)))),
                instructionsList);
            break;
        default:
            break;
    }
}

short getImmediateNumber(const char *arg) {
    int i;
    char *numberStr;
    numberStr = (char *) malloc(strlen(arg) * sizeof(char));
    for (i = 1; i < strlen(arg); ++i) {
        numberStr[i - 1] = arg[i];
    }
    numberStr[strlen(arg) - 1] = '\0';
    return (short) atoi(numberStr);
}

short addTargetRegisterToWord(int num) {
    num <<= 2;
    return (short) num;
}

short addSourceRegisterToWord(int num) {
    num <<= 8;
    return (short) num;
}

int getRegisterNumber(const char *registerName) {
    // 48 is the ascii code of '0'
    return registerName[1] - 48;
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
    return (short) targetAddressMethod;
}

short addSourceAddressMethod(short sourceAddressMethod) {
    sourceAddressMethod <<= 4;
    return sourceAddressMethod;
}

short addOperationCode(short operationNumber) {
    operationNumber <<= 6;
    return operationNumber;
}
