//
// Created by Iluz, Lisaf(AWF) on 21/02/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "../util/readerUtils.h"
#include "../validators/fileValidator.h"
#include "../structs/list.h"
#include "../util/constantBuilderUtils.h"
#include "../util/assemblyUtils.h"
#include "../validators/assemblyValidator.h"
#include "assemblyEncoder.h"
#include "../structs/symbol.h"
#include "../io/messagesHandler.h"
#include "../structs/shortData.h"

#define READ "r"
#define WRITE "w"
#define SOURCE_FILE_SUFFIX ".am"
#define TRUE 1
#define FALSE 0

int runFirstTransition(FILE *file, int ic, linkedList *instructionsList, int dc, linkedList *dataList, char *fileName,
                       linkedList *symbolsTable);

int runSecondTransition(linkedList *symbolsTable, linkedList *instructionsList, char *fileName);

void runTransitions(char *fileName, FILE *file);

void addIcToDataSymbols(int ic, linkedList *symbolsTable);

short getSymbolAddress(char *id, linkedList *symbolsTable);

void createOutputFiles(linkedList *instructionsList, linkedList *dataList, linkedList *symbolsTable, char *fileName);

void addObjectRows(linkedList *pList, FILE *pFile, int memoryRow);

void addStartMemoryAddress(linkedList *symbolsTable);

void assembler(int argc, char **argv) {
    int i;
    char *fileName;
    FILE *sourceFile;

    for (i = 1; i < argc; i++) {
        fileName = getFileName(argv, i, SOURCE_FILE_SUFFIX);
        if (isFileExist(fileName)) {
            sourceFile = fopen(fileName, READ);
            runTransitions(fileName, sourceFile);
            free(fileName);
            fclose(sourceFile);
        }
    }
}

void runTransitions(char *fileName, FILE *file) {
    int isFileValid, i;
    int instructionsCounter, dataCounter;
    linkedList *instructionsList, *dataList;
    linkedList *symbolsTable;  // id is row number data is the error , id is symbol name data is memory address

    isFileValid = 1;
    instructionsCounter = 0; //need to be initialized here?
    dataCounter = 0;  //need to be initialized here?
    symbolsTable = createNewLinkedList();
    instructionsList = createNewLinkedList();
    dataList = createNewLinkedList();

    isFileValid = runFirstTransition(file, instructionsCounter, instructionsList, dataCounter, dataList,
                                     fileName, symbolsTable);
    if (isFileValid) {
        isFileValid = runSecondTransition(symbolsTable, instructionsList,
                                          fileName);
        if (isFileValid) {
            createOutputFiles(instructionsList, dataList, symbolsTable, fileName);
            //freeAll
        }
    }
}

void createOutputFiles(linkedList *instructionsList, linkedList *dataList, linkedList *symbolsTable, char *fileName) {
    char *objectFileName, *firstLine;
    FILE *objectOutputFile;
    int memoryRow;
    memoryRow = 100;

    objectFileName = getOutputFileName(fileName, ".ob");
    objectOutputFile = fopen(objectFileName, WRITE);
    fprintf(objectOutputFile, "    %d  %d     \n", getLength(instructionsList), getLength(dataList));
    addObjectRows(instructionsList, objectOutputFile, memoryRow);
    addObjectRows(dataList, objectOutputFile, memoryRow + getLength(instructionsList));

    free(objectFileName);
    fclose(objectOutputFile);

}

void addObjectRows(linkedList *pList, FILE *pFile, int memoryRow) {
    node *currNode;
    char *binaryEncodedWord;
    currNode = pList->head;
    while (currNode != NULL) {
        short word;
        word = ((shortData *)currNode->data)->value;
        binaryEncodedWord = getBinaryEncodedWord(word);
        fprintf(pFile, "%4d   %s\n", memoryRow, binaryEncodedWord);
        free(binaryEncodedWord);
        currNode = currNode->next;
        memoryRow++;
    }
}

int runFirstTransition(FILE *file, int ic, linkedList *instructionsList, int dc, linkedList *dataList, char *fileName,
                       linkedList *symbolsTable) {
    int isValid, rowCounter, symbolFlag;
    char *line, *lineCopy, *firstWord, *secondWord;
    const char **operationsTable;

    isValid = TRUE, rowCounter = 1, ic = 0, dc = 0, symbolFlag = 0;
    line = NULL;
    size_t len = 0;
    operationsTable = getOperationsTable();

    while (getline(&line, &len, file) != -1) {
        lineCopy = copyStr(line);
        lineCopy = trim(lineCopy);
        if (!isCommentLine(lineCopy) && !isEmptyLine(lineCopy)) {
            firstWord = getToken(lineCopy, ' ', 0);
            if (isSymbol(firstWord)) {
                firstWord[strlen(firstWord) - 1] = '\0';  // Without the ':' at the end. maybe not a good practice
                //firstWord = getToken(firstWord, ':', 0); // cant free the previous firstWord
                if (validateSymbolName(firstWord, symbolsTable, fileName, rowCounter)) {
                    symbolFlag = 1;
                } else {
                    isValid = FALSE;
                }
            }
            secondWord = getToken(lineCopy, ' ', 1);
            if (isDataGuidance(firstWord) || isDataGuidance(secondWord)) {
                if (validateDataGuidanceLine(lineCopy, symbolFlag, fileName, rowCounter)) {
                    if (symbolFlag) {
                        node *n;
                        symbol *s;
                        s = createSymbol(firstWord, DATA_TYPE, dc);
                        n = createNewNode(firstWord, s);
                        add(n, symbolsTable);
                    }
                    dc += saveGuidanceLine(lineCopy, symbolFlag, dataList, dc);
                } else {
                    isValid = FALSE;
                }
            } else if (isExternalOrEntryGuidance(firstWord) || isExternalOrEntryGuidance(secondWord)) {
                if (isExternalOrEntryGuidance(secondWord)) {
                    printWarning(REDUNDANT_SYMBOL, lineCopy, fileName, rowCounter);
                }
                if (isExternalGuidance(firstWord) || isExternalGuidance(secondWord)) {
                    if (validateExternalGuidanceLine(lineCopy, symbolFlag, fileName, rowCounter)) {
                        char *externalSymbolName;
                        node *n;
                        symbol *s;
                        externalSymbolName = getToken(lineCopy, ' ', symbolFlag);
                        s = createSymbol(externalSymbolName, EXTERNAL_TYPE, dc);
                        n = createNewNode(externalSymbolName, s);
                        add(n, symbolsTable);
                        //dc += saveGuidanceLine(lineCopy, dataArray);
                        //check for potential duplicates external vs entry?
                    } else {
                        isValid = FALSE;
                    }
                }
            } else if (validateCodeLine(lineCopy, symbolFlag, operationsTable, fileName, rowCounter)) {
                if (symbolFlag) {
                    node *n;
                    symbol *s;
                    s = createSymbol(firstWord, CODE_TYPE, ic);
                    n = createNewNode(firstWord, s);
                    add(n, symbolsTable);
                }
                ic += saveCodeLine(lineCopy, symbolFlag, operationsTable, instructionsList, ic);  // use &iArray
            } else {
                isValid = FALSE;
            }

            //free(firstWord); //todo: need to understand why doesn't work with this line. maybe because we use it!!
            free(secondWord);//todo: need to understand why doesn't work with this line
        }
        rowCounter++;
        symbolFlag = 0;
        ////free
        free(lineCopy);
    }
    addIcToDataSymbols(ic, symbolsTable);
    addStartMemoryAddress(symbolsTable);
//    if (postValidation(isValid)) {
//        todo: create linkedList with all the symbols that are defined as arguments (exclude entry) in the file with the row
//        todo: validate here if any of those symbols are not appear in the symbols table
//        //updateDataSymbolsValues(symbolsTable, ic);
//    }
    return isValid;
}

void addStartMemoryAddress(linkedList *symbolsTable) {
    node *currNode;
    currNode = symbolsTable->head;

    while (currNode != NULL) {
        ((symbol *) currNode->data)->value += 100;
        currNode = currNode->next;
    }
}

void addIcToDataSymbols(int ic, linkedList *symbolsTable) {
    node *currNode;
    currNode = symbolsTable->head;

    while (currNode != NULL) {
        if (((symbol *) currNode->data)->symbolType == DATA_TYPE) {
            ((symbol *) currNode->data)->value += ic;
        }
        currNode = currNode->next;
    }
}


int runSecondTransition(linkedList *symbolsTable, linkedList *instructionsList, char *fileName) {
    int isValid;
    node *currNode;

    isValid = TRUE;
    currNode = instructionsList->head;

    while (currNode != NULL) {
        char *id;
        id = (char *) currNode->id;
        if (id != NULL) {
            if (isIdExist(id, symbolsTable)) {
                currNode->data = createShortData(getSymbolAddress(id, symbolsTable));
            } else {
                printError(SYMBOL_NOT_FOUND, id, fileName, -1);
                isValid = FALSE;
            }
        }
        currNode = currNode->next;
    }
    return isValid;
}

short getSymbolAddress(char *id, linkedList *symbolsTable) {
    node *currNode;
    currNode = symbolsTable->head;

    while (currNode != NULL) {
        if (isEqual((char *) currNode->id, id)) {
            return (short) ((symbol *) currNode->data)->value;
        }
        currNode = currNode->next;
    }
    return -1;
}
