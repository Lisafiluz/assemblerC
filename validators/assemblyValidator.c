/*
 Created by Iluz, Lisaf(AWF) on 21/02/2023.
*/

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assemblyValidator.h"
#include "../enums/enums.h"
#include "../util/readerUtils.h"
#include "../io/messagesHandler.h"
#include "../util/assemblyUtils.h"

#define LINE_MAX_LEN 80
#define SYMBOL_MAX_LEN 30
#define SYMBOL_MIN_LEN 1
#define TRUE 1
#define FALSE 0
#define OPERATIONS_SIZE 16

int getNextTokenIndex(const char *str, int fromIdx, char delim);

int isData(const char *line, int i);

int validateDotDataRow(char *arguments, const char *fileName, int rowCounter);

int validateDotStringRow(char *lineCopy, const char *argument, char *fileName, int rowCounter);

int operationValidator(char *command, int operationNumber, char *fileName, int rowNumber);

int validateNumberOfArgs(int numberOfArgs, int operationNumber);

int validateSourceArgAddressMethod(int addressMethod, int operationNumber);

int validateTargetArgAddressMethod(int addressMethod, int operationNumber);

int validateMacroName(const char *macroName) {
    int isValid;
    isValid = TRUE;
    if (isValidRegister(macroName)) {
        printError(INVALID_MACRO_NAME, macroName, NULL, -1);
        isValid = FALSE;
    }
    return isValid;
}

int validateBasicLine(const char *line, const char *fileName, int rowCounter) {
    int isValid;
    isValid = TRUE;
    if (strlen(line) > LINE_MAX_LEN) {
        printError(LINE_EXCEED_MAX_LEN, line, fileName, rowCounter);
        isValid = FALSE;
    }
    return isValid;
}

int validateSymbolName(char *symbol, linkedList *symbolsTable, char *fileName, int rowCounter) {
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
    } else if (isIdExist(symbol, symbolsTable)) {
        printError(DUPLICATE_SYMBOL_NAME, symbol, fileName, rowCounter);
        isValid = FALSE;
    } else if (isValidRegister(symbol)) {
        printError(SYMBOL_CANT_BE_REGISTER, symbol, fileName, rowCounter);
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
        index = getNextTokenIndex(lineCopy, index, ':');
    }
    if (isData(lineCopy, index)) {
        index = getNextTokenIndex(lineCopy, index, ' ');
        isValid = validateDotDataRow(&lineCopy[index], fileName, rowCounter);

    } else {
        index = getNextTokenIndex(lineCopy, index, ' ');
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
    number = (char *) malloc((size + 1) * sizeof(char));
    for (i = 0; i < size; i++) {
        if (!isspace(arguments[i])) {
            if (arguments[i] == ',') {
                number[numberCounter] = '\0';
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
        } else {
            if (number != NULL && strlen(number) > 0) {
                number[numberCounter] = arguments[i];
                numberCounter++;
            }
        }
    }
    number[numberCounter] = '\0';
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

int validateExternalEntryGuidanceLine(const char *line, int symbolFlag, char *fileName, int rowCounter) {
    int isValid, index, prevIndex;
    char *lineCopy;
    isValid = TRUE;
    index = 0;
    lineCopy = copyStr(line);
    if (symbolFlag) {
        index = getNextTokenIndex(lineCopy, index, ':');
    }
    index = getNextTokenIndex(lineCopy, index, ' ');
    prevIndex = index;
    index = getNextTokenIndex(lineCopy, index, ' ');
    if (index != strlen(lineCopy)) {
        isValid = FALSE;
        printError(TOO_MUCH_ARGUMENTS, lineCopy, fileName, rowCounter);
    } else if (prevIndex == index) {
        isValid = FALSE;
        printError(NO_ARGUMENTS, lineCopy, fileName, rowCounter);
    }
    free(lineCopy);
    return isValid;
}

int validateCodeLine(char *line, int symbolFlag, const char **operationsTable, char *fileName, int rowCounter) {
    int isValid, i, isFound;
    char *lineCopy, *pLineCopy, *operationName;
    isFound = FALSE;
    lineCopy = copyStr(line);
    pLineCopy = lineCopy;
    if (symbolFlag) {
        lineCopy = getLineWithoutSymbol(lineCopy);
    }
    operationName = getToken(lineCopy, ' ', 0);
    for (i = 0; i < OPERATIONS_SIZE && !isFound; ++i) {
        if (isEqual(operationName, operationsTable[i])) {
            isValid = operationValidator(lineCopy, i, fileName, rowCounter);
            isFound = TRUE;
        }
    }
    if (!isFound) {
        printError(OPERATION_NAME_NOT_FOUND, operationName, fileName, rowCounter);
        isValid = FALSE;
    }
    free(operationName);
    free(pLineCopy);
    return isValid;
}

int operationValidator(char *command, int operationNumber, char *fileName, int rowNumber) {
    int numberOfArgs, isValid;
    isValid = TRUE;
    numberOfArgs = getNumberOfArgs(command);
    if (!validateNumberOfArgs(numberOfArgs, operationNumber)) {
        printError(INVALID_COMMAND_NUMBER_ARGUMENTS, command, fileName, rowNumber);
        return FALSE;
    }

    char *sourceArg, *targetArg;
    int sourceAddressMethod, targetAddressMethod;
    if (numberOfArgs == 1) {
        targetArg = getArgument(command, 0);
        targetAddressMethod = getArgumentAddressMethod(targetArg);
        if (targetAddressMethod == -1 || !validateTargetArgAddressMethod(targetAddressMethod, operationNumber)) {
            isValid = FALSE;
            printError(INVALID_TARGET_ARGUMENT_ADDRESS_METHOD, command, fileName, rowNumber);
        }
        free(targetArg);
    } else if (numberOfArgs == 2) {
        sourceArg = getArgument(command, 0);
        sourceAddressMethod = getArgumentAddressMethod(sourceArg);
        if (sourceAddressMethod == -1 || !validateSourceArgAddressMethod(sourceAddressMethod, operationNumber)) {
            isValid = FALSE;
            printError(INVALID_SOURCE_ARGUMENT_ADDRESS_METHOD, command, fileName, rowNumber);
        }
        targetArg = getArgument(command, 1);
        targetAddressMethod = getArgumentAddressMethod(targetArg);
        if (targetAddressMethod == -1 || !validateTargetArgAddressMethod(targetAddressMethod, operationNumber)) {
            isValid = FALSE;
            printError(INVALID_TARGET_ARGUMENT_ADDRESS_METHOD, command, fileName, rowNumber);
        }
        free(sourceArg);
        free(targetArg);
    }
    return isValid;
}

int validateTargetArgAddressMethod(int addressMethod, int operationNumber) {
    switch (operationNumber) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13:
            return addressMethod >= 1 && addressMethod <= 3;
        case 1:
        case 12:
            return addressMethod >= 0 && addressMethod <= 3;
        default:
            return FALSE;
    }
}

int validateSourceArgAddressMethod(int addressMethod, int operationNumber) {
    switch (operationNumber) {
        case 0:
        case 1:
        case 2:
        case 3:
            return addressMethod >= 0 && addressMethod <= 3;
        case 6:
            return addressMethod == 1 || addressMethod == 2;
        default:
            return FALSE;
    }
}

int validateNumberOfArgs(int numberOfArgs, int operationNumber) {
    switch (operationNumber) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            return numberOfArgs == 2;
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return numberOfArgs == 1;
        case 14:
        case 15:
            return numberOfArgs == 0;
        default:
            return FALSE;
    }
}

int isData(const char *line, int i) {
    char *data;
    data = ".data";
    return line[i] == data[0] &&
           line[i + 1] == data[1] &&
           line[i + 2] == data[2] &&
           line[i + 3] == data[3] &&
           line[i + 4] == data[4];
}

int getNextTokenIndex(const char *str, int fromIdx, char delim) {
    int i;
    i = fromIdx;
    if (isspace(delim)) {
        while (!isspace(str[i]) && i < strlen(str)) i++;
    } else {
        while (str[i] != delim && i < strlen(str)) i++;
        i++;
    }
    while (isspace(str[i]) && i < strlen(str)) i++;
    return i;
}

int isValidDirectAddressArgument(const char *argument) {
    if (argument[0] == '#') {
        if (isValidNumber(++argument)) {
            return TRUE;
        }
    }
    return FALSE;
}

int isValidRegister(const char *argument) {
    return strlen(argument) == 2 && argument[0] == 'r' && argument[1] >= '0' && argument[1] <= '7';
}

int isValidSymbol(const char *argument) {
    return isalpha(argument[0]) && !hasSpaces(argument) && isAlphaNumeric(argument) && !isValidRegister(argument);
}

int isValidJumpAddressArgMethod(char *argument) {
    int isValidJumpAddressArgMethod;
    char *symbol;
    isValidJumpAddressArgMethod = TRUE;
    symbol = getToken(argument, '(', 0);
    if (strlen(symbol) == strlen(argument) || !isValidSymbol(symbol)) {
        return FALSE;
    }
    if (argument[strlen(argument) - 1] != ')') {
        return FALSE;
    }
    size_t i;
    int paramIndex, param1Idx, param2Idx;
    char *param1, *param2;
    paramIndex = 1, param1Idx = 0, param2Idx = 0;
    i = strlen(symbol) + 1;
    param1 = (char *) malloc((strlen(argument) + 1) * sizeof(char));
    param2 = (char *) malloc((strlen(argument) + 1) * sizeof(char));
    for (; i < strlen(argument) - 1; i++) {
        if (argument[i] == ',') {
            paramIndex++;
        } else {
            if (paramIndex == 1) {
                param1[param1Idx] = argument[i];
                param1Idx++;
            } else {
                param2[param2Idx] = argument[i];
                param2Idx++;
            }
        }
    }
    param1[param1Idx] = '\0';
    param2[param2Idx] = '\0';

    if (!isValidDirectAddressArgument(param1) &&
        !isValidRegister(param1) &&
        !isValidSymbol(param1)) {
        isValidJumpAddressArgMethod = FALSE;
    }

    if (!isValidDirectAddressArgument(param2) &&
        !isValidRegister(param2) &&
        !isValidSymbol(param2)) {
        isValidJumpAddressArgMethod = FALSE;
    }

    free(symbol);
    free(param1);
    free(param2);
    return isValidJumpAddressArgMethod;
}
