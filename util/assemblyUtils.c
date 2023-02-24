//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <string.h>
#include "assemblyUtils.h"
#include "readerUtils.h"

#define DATA .data
#define STRING .STRING

int isSymbol(char *str) {
    return str[strlen(str) - 1] == ':';
}

int isDataGuidance(char *str) {
    return isEqual(str, ".data") || isEqual(str, ".string");
}

int isExternalGuidance(char *str) {
    return isEqual(str, ".extern");
}

int isExternalOrEntryGuidance(char *str) {
    return isExternalGuidance(str) || isEqual(str, ".entry");
}

