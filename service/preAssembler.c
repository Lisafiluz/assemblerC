//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "preAssembler.h"
#include "../structs/list.h"
#include "../util/readerUtils.h"
#include "../validators/fileValidator.h"
#include "../validators/assemblyValidator.h"

#define READ "r"
#define WRITE "w"
#define SOURCE_FILE_SUFFIX ".as"
#define TARGET_FILE_SUFFIX ".am"
#define MACRO "mcr"
#define END_MACRO "endmcr"
#define TRUE 1
#define FALSE 0

int openMacros(FILE *file, FILE *outputFile);

void writeMacroDataToFile(linkedList *macro, FILE *outputFile);

char *getMacroName(char *line);

void freeMacros(linkedList *pList);

int preAssembler(int argc, char **argv) {
    int i, isValid;
    char *fileName, *outputFileName;
    FILE *sourceFile, *outputFile;
    isValid = TRUE;
    for (i = 1; i < argc; i++) {
        int fileValid;
        fileName = getFileName(argv, i, SOURCE_FILE_SUFFIX);
        outputFileName = getFileName(argv, i, TARGET_FILE_SUFFIX);
        if (isFileExist(fileName)) {
            sourceFile = fopen(fileName, READ);
            outputFile = fopen(outputFileName, WRITE);

            free(fileName);
            free(outputFileName);

            fileValid = openMacros(sourceFile, outputFile);
            if (!fileValid) {
                isValid = FALSE;
            }
            fclose(sourceFile);
            fclose(outputFile);
        }
    }
    return isValid;
}

int openMacros(FILE *file, FILE *outputFile) {
    linkedList *macrosList, *macroData;
    node *macro, *row;
    char *firstToken, *macroName, *line, *lineCopy, *pLineCopy;
    void *macroDataRowCounter;
    int macroFlag, isValid;

    isValid = TRUE;
    macrosList = createNewLinkedList();
    line = NULL;
    size_t len = 0;
    macroFlag = 0;
    while (getline(&line, &len, file) != -1) {
        lineCopy = copyStr(line);
        pLineCopy = lineCopy;
        lineCopy = trim(lineCopy);
        firstToken = getToken(lineCopy, ' ', 0);
        if (isEqual(firstToken, END_MACRO)) {
            macroFlag = 0;
            macro = createNewNode(macroName, macroData);
            add(macro, macrosList);
        } else if (macroFlag) {
            row = createNewNode(macroDataRowCounter, lineCopy);
            add(row, macroData);
            macroDataRowCounter++;
        } else if (isEqual(firstToken, MACRO)) {
            macroData = createNewLinkedList();
            macroFlag = 1;
            macroDataRowCounter = (void *) 1;
            macroName = getMacroName(lineCopy);
            if (!validateMacroName(macroName)) {
                isValid = FALSE;
            }
        } else if (isIdExist(firstToken, macrosList)) {
            linkedList *macroDataToAdd = getDataById(firstToken, macrosList);
            writeMacroDataToFile(macroDataToAdd, outputFile);
        } else {
            fprintf(outputFile, "%s\n", lineCopy);
            free(pLineCopy);
        }
        free(firstToken);
    }
    if (line) {
        free(line);
    }
    freeMacros(macrosList);
    return isValid;
}

void freeMacros(linkedList *pList) {
    if (isListNotEmpty(pList)) {
        node *currNode = pList->head;
        while (currNode != NULL) {
            free(currNode->id);
            freeLinkedList((linkedList *) currNode->data);
            currNode = currNode->next;
        }
    }
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
    macroName = (char *) malloc((strlen(line) + 1) * sizeof(char));
    i = 0;
    while (!isspace(line[i]) && i < strlen(line)) {
        macroName[i] = line[i];
        i++;
    }
    macroName[i] = '\0';
    return macroName;
}

void writeMacroDataToFile(linkedList *macro, FILE *outputFile) {
    if (isListNotEmpty(macro)) {
        node *currNode = macro->head;
        while (currNode != NULL) {
            fprintf(outputFile, "%s\n", (char *) currNode->data);
            currNode = currNode->next;
        }
    }
}
