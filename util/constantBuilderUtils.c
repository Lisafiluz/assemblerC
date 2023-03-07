//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <stdlib.h>
#include "constantBuilderUtils.h"

#define NUMBER_OF_OPERATIONS 16

const char ** getOperationsTable() {
//    linkedList *opTable;
//    opTable = createNewLinkedList();
//    add(createNewNode("mov", (void *) 0), opTable);
//    add(createNewNode("cmp", (void *) 1), opTable);
//    add(createNewNode("add", (void *) 2), opTable);
//    add(createNewNode("sub", (void *) 3), opTable);
//    add(createNewNode("not", (void *) 4), opTable);
//    add(createNewNode("clr", (void *) 5), opTable);
//    add(createNewNode("lea", (void *) 6), opTable);
//    add(createNewNode("inc", (void *) 7), opTable);
//    add(createNewNode("dec", (void *) 8), opTable);
//    add(createNewNode("jmp", (void *) 9), opTable);
//    add(createNewNode("bne", (void *) 10), opTable);
//    add(createNewNode("red", (void *) 11), opTable);
//    add(createNewNode("prn", (void *) 12), opTable);
//    add(createNewNode("jsr", (void *) 13), opTable);
//    add(createNewNode("rts", (void *) 14), opTable);
//    add(createNewNode("stop", (void *) 15), opTable);

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