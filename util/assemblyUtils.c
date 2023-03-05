//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "assemblyUtils.h"
#include "readerUtils.h"
#include "../validators/assemblyValidator.h"

#define DATA ".data"
#define STRING ".string"
#define EXTERN ".extern"
#define ENTRY ".entry"
#define FALSE 0
#define TRUE 1
//#define INVALID_ARGUMENT '-1'


int isSymbol(char *str) {
    return str[strlen(str) - 1] == ':';
}

int isDataGuidance(char *str) {
    return isEqual(str, DATA) || isEqual(str, STRING);
}

int isExternalGuidance(char *str) {
    return isEqual(str, EXTERN);
}

int isExternalOrEntryGuidance(char *str) {
    return isExternalGuidance(str) || isEqual(str, ENTRY);
}

int getNumberOfArgs(const char *command) {
    // "xxxx    3,l1(r0,#5)"
    // "sss r3, r4"
    // "xxx r0"
    // "xxx"
    int i, numberOfArgs, jumpAddressMethodFlag;
    i = 0, numberOfArgs = 0, jumpAddressMethodFlag = 0;
    while (isspace(command[i])) {
        i++;
    }
    while (!isspace(command[i])) {
        i++;
    }
    if (i + 1 == strlen(command)) {
        return 0;
    }
    numberOfArgs++;

    for (; i < strlen(command); i++) {
        if (command[i] == '(') {
            jumpAddressMethodFlag = 1;
        } else if (command[i] == ')') {
            jumpAddressMethodFlag = 0;
        } else if (!jumpAddressMethodFlag && command[i] == ',') {
            numberOfArgs++;
        }
    }
    return numberOfArgs;
}

char *getArgument(const char *command, int argumentNumber) {
    // mov r0,r1
    char *argument;
    int i, argIdx, jumpAddressMethodFlag;
    i = 0, argIdx = 0, jumpAddressMethodFlag = 0;
    while (isspace(command[i])) {
        i++;
    }
    while (!isspace(command[i])) {
        i++;
    }
    argument = (char *) malloc((strlen(command) - i + 1) * sizeof(char));
    for (; i < strlen(command); i++) {
        if (command[i] == '(') {
            jumpAddressMethodFlag = 1;
        } else if (command[i] == ')') {
            jumpAddressMethodFlag = 0;
        }

        if (command[i] == ',' && !jumpAddressMethodFlag) {
            if (argumentNumber == 0) {
                argument[argIdx] = '\0';
                return argument;
            } else argumentNumber--;
        } else if (argumentNumber == 0 && !isspace(command[i])) {
            argument[argIdx] = command[i];
            argIdx++;
        }
    }
    argument[argIdx] = '\0';
    return argument;
}

int getArgumentAddressMethod(char *arg) {
    if (arg == NULL || strlen(arg) == 0) {
        return -1;
    }
    if (isValidDirectAddressArgument(arg)) {
        return 0;
    }
    if (isValidRegister(arg)) {
        return 3;
    }
    if (isValidJumpAddressArgMethod(arg)) {
        return 2;
    }
    if (isValidSymbol(arg)) {
        return 1;
    }
    return -1;
}

char *getLineWithoutSymbol(char *line) {
    while (!isspace(*line)) line++;
    while (isspace(*line)) line++;
    return line;
}



