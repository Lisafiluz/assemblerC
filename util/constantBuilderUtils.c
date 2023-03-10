//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <stdlib.h>
#include "constantBuilderUtils.h"

#define NUMBER_OF_OPERATIONS 16

const char ** getOperationsTable() {
    char **opTable;
    opTable = malloc(NUMBER_OF_OPERATIONS * sizeof(char *));
    opTable[0] = "mov";
    opTable[1] = "cmp";
    opTable[2] = "add";
    opTable[3] = "sub";
    opTable[4] = "not";
    opTable[5] = "clr";
    opTable[6] = "lea";
    opTable[7] = "inc";
    opTable[8] = "dec";
    opTable[9] = "jmp";
    opTable[10] = "bne";
    opTable[11] = "red";
    opTable[12] = "prn";
    opTable[13] = "jsr";
    opTable[14] = "rts";
    opTable[15] = "stop";

    return (const char **) opTable;
}