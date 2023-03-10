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

int runFirstTransition(FILE *file, linkedList *instructionsList, linkedList *dataList, char *fileName,
                       linkedList *symbolsTable, linkedList *entries);

int runSecondTransition(linkedList *symbolsTable, linkedList *instructionsList, char *fileName);

void runTransitions(char *fileName, FILE *file, const char *fileNameTemplate);

void addIcToDataSymbols(int ic, linkedList *symbolsTable);

short getSymbolAddress(char *id, linkedList *symbolsTable);

void
createOutputFiles(linkedList *instructionsList, linkedList *dataList, linkedList *entries, linkedList *symbolsTable,
                  const char *fileName);

void addObjectRows(linkedList *pList, FILE *pFile, int memoryRow);

void addStartMemoryAddress(linkedList *symbolsTable);

void createObjectFile(linkedList *instructionsList, linkedList *dataList, const char *fileName);

void createEntriesFile(linkedList *entries, linkedList *symbolsTable, const char *fileName);

void createExternalsFile(linkedList *instructionsList, linkedList *symbolsTable, const char *fileName);

void assembler(int argc, char **argv) {
    int i;
    char *fileName;
    FILE *sourceFile;

    for (i = 1; i < argc; i++) {
        fileName = getFileName(argv, i, SOURCE_FILE_SUFFIX);
        if (isFileExist(fileName)) {
            sourceFile = fopen(fileName, READ);
            runTransitions(fileName, sourceFile, argv[i]);
            free(fileName);
            fclose(sourceFile);
        }
    }
}

void runTransitions(char *fileName, FILE *file, const char *fileNameTemplate) {
    int isFileValid;
    linkedList *instructionsList, *dataList, *symbolsTable, *entries;  // id is row number data is the error , id is symbol name data is memory address

    isFileValid = TRUE;
    symbolsTable = createNewLinkedList();
    instructionsList = createNewLinkedList();
    dataList = createNewLinkedList();
    entries = createNewLinkedList();

    isFileValid = runFirstTransition(file, instructionsList, dataList, fileName, symbolsTable, entries);
    if (isFileValid) {
        isFileValid = runSecondTransition(symbolsTable, instructionsList, fileName);
        if (isFileValid) {
            createOutputFiles(instructionsList, dataList, entries, symbolsTable, fileNameTemplate);
            freeLinkedList(instructionsList);
            freeLinkedList(dataList);
            freeLinkedList(symbolsTable);
            freeLinkedList(entries);
        }
    }
}

void
createOutputFiles(linkedList *instructionsList, linkedList *dataList, linkedList *entries, linkedList *symbolsTable,
                  const char *fileName) {
    createObjectFile(instructionsList, dataList, fileName);
    createEntriesFile(entries, symbolsTable, fileName);
    createExternalsFile(instructionsList, symbolsTable, fileName);
}

void createExternalsFile(linkedList *instructionsList, linkedList *symbolsTable, const char *fileName) {
    char *externalsFileName;
    FILE *externalsOutputFile;
    int isExist, i;
    isExist = FALSE;
    node *currNode;
    i = 100;
    currNode = instructionsList->head;
    while (currNode != NULL) {
        if (currNode->id != NULL) {
            // this is line with symbol
            if (((symbol *) getDataById(currNode->id, symbolsTable))->symbolType == EXTERNAL_TYPE) {
                if (!isExist) {
                    externalsFileName = getOutputFileName(fileName, ".ext");
                    externalsOutputFile = fopen(externalsFileName, WRITE);
                    isExist = TRUE;
                }
                fprintf(externalsOutputFile, "%s         %d\n", (char *) currNode->id, i);
            }
        }
        i++;
        currNode = currNode->next;
    }
    if (isExist) {
        fclose(externalsOutputFile);
        free(externalsFileName);
    }
}

void createEntriesFile(linkedList *entries, linkedList *symbolsTable, const char *fileName) {
    char *entriesFileName;
    FILE *entriesOutputFile;
    if (isListNotEmpty(entries)) {
        entriesFileName = getOutputFileName(fileName, ".ent");
        entriesOutputFile = fopen(entriesFileName, WRITE);
        node *currNode;
        int address;
        currNode = entries->head;
        while (currNode != NULL) {
            address = ((symbol *) getDataById(currNode->id, symbolsTable))->value;
            fprintf(entriesOutputFile, "%s         %d\n", (char *) currNode->id, address);
            currNode = currNode->next;
        }
        free(entriesFileName);
        fclose(entriesOutputFile);
    }
}

void createObjectFile(linkedList *instructionsList, linkedList *dataList, const char *fileName) {
    char *objectFileName;
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
        word = ((shortData *) currNode->data)->value;
        binaryEncodedWord = getBinaryEncodedWord(word);
        fprintf(pFile, "%04d   %s\n", memoryRow, binaryEncodedWord);
        free(binaryEncodedWord);
        currNode = currNode->next;
        memoryRow++;
    }
}

int runFirstTransition(FILE *file, linkedList *instructionsList, linkedList *dataList, char *fileName,
                       linkedList *symbolsTable, linkedList *entries) {
    int dc, ic;
    int isValid, rowCounter, symbolFlag;
    char *line, *lineCopy, *pLineCopy, *firstWord, *secondWord;
    const char **operationsTable;

    isValid = TRUE, rowCounter = 1, ic = 0, dc = 0, symbolFlag = 0;
    line = NULL;
    size_t len = 0;
    operationsTable = getOperationsTable();

    while (getline(&line, &len, file) != -1) {
        lineCopy = copyStr(line);
        pLineCopy = lineCopy;
        lineCopy = trim(lineCopy);
        if (!isCommentLine(lineCopy) && !isEmptyLine(lineCopy)) {
            if (validateBasicLine(lineCopy, fileName, rowCounter)) {
                firstWord = getToken(lineCopy, ':', 0);
                if (strlen(firstWord) < strlen(lineCopy)) {
                    symbolFlag = 1;
                    if (!validateSymbolName(firstWord, symbolsTable, fileName, rowCounter)) {
                        isValid = FALSE;
                    }
                }
                if (!symbolFlag) {
                    free(firstWord);
                    firstWord = getToken(lineCopy, ' ', 0);
                }
                secondWord = getSecondWord(lineCopy, symbolFlag);
                if (isDataGuidance(firstWord) || isDataGuidance(secondWord)) {
                    if (validateDataGuidanceLine(lineCopy, symbolFlag, fileName, rowCounter)) {
                        if (symbolFlag) {
                            node *n;
                            symbol *s;
                            char *symbolName;
                            symbolName = copyStr(firstWord);
                            s = createSymbol(symbolName, DATA_TYPE, dc);
                            n = createNewNode(symbolName, s);
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
                        if (validateExternalEntryGuidanceLine(lineCopy, symbolFlag, fileName, rowCounter)) {
                            char *externalSymbolName;
                            node *n;
                            symbol *s;
                            externalSymbolName = getToken(lineCopy, ' ', symbolFlag + 1);
                            s = createSymbol(externalSymbolName, EXTERNAL_TYPE, 0);
                            n = createNewNode(externalSymbolName, s);
                            add(n, symbolsTable);
                        } else {
                            isValid = FALSE;
                        }
                    } else {
                        // entry
                        if (validateExternalEntryGuidanceLine(lineCopy, symbolFlag, fileName, rowCounter)) {
                            add(createNewNode(getToken(lineCopy, ' ', symbolFlag + 1), NULL), entries);
                        } else {
                            isValid = FALSE;
                        }
                    }
                } else if (validateCodeLine(lineCopy, symbolFlag, operationsTable, fileName, rowCounter)) {
                    if (symbolFlag) {
                        node *n;
                        symbol *s;
                        char *symbolName;
                        symbolName = copyStr(firstWord);
                        s = createSymbol(symbolName, CODE_TYPE, ic);
                        n = createNewNode(symbolName, s);
                        add(n, symbolsTable);
                    }
                    ic += saveCodeLine(lineCopy, symbolFlag, operationsTable, instructionsList);
                } else {
                    isValid = FALSE;
                }

                free(firstWord); //todo: need to understand why doesn't work with this line. maybe because we use it!!
                free(secondWord);//todo: need to understand why doesn't work with this line
            } else {
                isValid = FALSE;
            }
        }
        rowCounter++;
        symbolFlag = 0;
        ////free
        free(pLineCopy);
    }
    addIcToDataSymbols(ic, symbolsTable);
    addStartMemoryAddress(symbolsTable);
    return isValid;
}

void addStartMemoryAddress(linkedList *symbolsTable) {
    node *currNode;
    currNode = symbolsTable->head;

    while (currNode != NULL) {
        if (((symbol *) currNode->data)->symbolType != EXTERNAL_TYPE) {
            ((symbol *) currNode->data)->value += 100;
        }
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
    short address;
    address = -1;
    while (currNode != NULL) {
        if (isEqual((char *) currNode->id, id)) {
            address = (short) (((symbol *) currNode->data)->value << 2);
            switch (((symbol *) currNode->data)->symbolType) {
                case DATA_TYPE:
                case CODE_TYPE:
                    address += 2;
                    break;
                case EXTERNAL_TYPE:
                    address += 1;
                    break;
                default:
                    break;
            }
        }
        currNode = currNode->next;
    }
    return address;
}
