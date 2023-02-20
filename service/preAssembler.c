//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "preAssembler.h"
#include "../structs/macro.h"
#include "../structs/list.h"
#include "../util/readerUtils.h"

#define READ "r"
#define WRITE "w"
#define SOURCE_FILE_SUFFIX ".as"
#define TARGET_FILE_SUFFIX ".am"

char *getFileName(char **argv, int i, char *suffix);

char *getOutputFileName(char **argv, int i);

void openMacros(FILE *file, FILE *outputFile);

void writeMacroDataToFile(linkedList *macro, FILE *outputFile);

char *getMacroName(char *line);

void preAssembler(int argc, char **argv) {
    int i;
    char *fileName, *outputFileName;
    FILE *sourceFile, *outputFile;

    for (i = 1; i < argc; i++) {
        fileName = getFileName(argv, i, SOURCE_FILE_SUFFIX);
        outputFileName = getFileName(argv, i, TARGET_FILE_SUFFIX);

        sourceFile = fopen(fileName, READ);
        outputFile = fopen(outputFileName, WRITE);

        free(fileName);
        free(outputFileName);

        openMacros(sourceFile, outputFile);

        fclose(sourceFile);
        fclose(outputFile);
    }
}

void openMacros(FILE *file, FILE *outputFile) {
    linkedList *macrosList, *macroData;
    node *macro, *row;
    char *labelName, *macroName, *line, *lineCopy;
    void *macroDataRowCounter;
    int macroFlag;

    macrosList = (linkedList *) malloc(sizeof(linkedList));
    newLinkedList(macrosList);
    size_t len = 0;
    line = NULL;
    macroFlag = 0;
    while (getline(&line, &len, file) != -1) {
        lineCopy = (char *) malloc(strlen(line) * sizeof(char));
        strcpy(lineCopy, line);
        lineCopy = trim(lineCopy);
        labelName = getCommandName(lineCopy);
        if (strcmp(labelName, "endmcr") == 0) {
            macroFlag = 0;
            macro = (node *) malloc(sizeof(node));
            newNode(macro);
            macro->id = macroName;
            macro->data = macroData;
            add(macro, macrosList);
        } else if (macroFlag) {
            row = (node *) malloc(sizeof(node));
            newNode(row);
            row->id = macroDataRowCounter;
            row->data = lineCopy;
            add(row, macroData);
            macroDataRowCounter++;
        } else if (strcmp(labelName, "mcr") == 0) {
            macroData = (linkedList *) malloc(sizeof(linkedList));
            newLinkedList(macroData);
            macroFlag = 1;
            macroDataRowCounter = (void *) 1;
            macroName = getMacroName(lineCopy);
        } else if (isIdExist(labelName, macrosList)) {
            linkedList *macroDataToAdd = (linkedList *) getDataById(labelName, macrosList);
            writeMacroDataToFile(macroDataToAdd, outputFile);
        } else {
            fputs(lineCopy, outputFile);
            fputs("\n", outputFile);
        }
        free(labelName);
    }
    if (line) {
        free(line);
    }
    //TODO: free all the allocations (linkedList should be freed by list function)
}

char *getMacroName(char *line) {
    char *macroName;
    int i;
    while (!isspace(*line)) {
        line++;
    }
    while (isspace(*line)) {
        line++;
    }

    macroName = (char *) malloc(strlen(line) * sizeof(char));
    i = 0;
    while (!isspace(line[i]) && i < strlen(line)) {
        macroName[i] = line[i];
        i++;
    }
    macroName[i] = '\0';
    return macroName;
}

void writeMacroDataToFile(linkedList *macro, FILE *outputFile) {
    node *currNode = macro->head;
    if (isNotEmpty(macro)) {
        while (currNode != NULL) {
            fputs((char *) currNode->data, outputFile);
            fputs("\n", outputFile);
            currNode = currNode->next;
        }
    }
}

char *getFileName(char **const argv, int i, char *suffix) {
    char *fileName;
    fileName = (char *) calloc(strlen(argv[i]) + 4, sizeof(char));
    strcpy(fileName, argv[i]);
    strcat(fileName, suffix);
    fileName[strlen(fileName)] = '\0';
    return fileName;
}

char *getOutputFileName(char **const argv, int i) {
    char *fileName;
    fileName = (char *) calloc(strlen(argv[i]) + 3, sizeof(char));
    strcpy(fileName, argv[i]);
    strcat(fileName, ".am");
    return fileName;
}