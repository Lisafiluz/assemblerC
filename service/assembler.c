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

#define READ "r"
#define WRITE "w"
#define SOURCE_FILE_SUFFIX ".am"
#define TRUE 1
#define FALSE 0

int runFirstTransition(FILE *file, int ic, short *instructionsArray,
                       int dc, short *dataArray, char *fileName);

int runSecondTransition(FILE *file, int ic, short *instructionsArray,
                        int dc, short *dataArray, char *fileName);

void runTransitions(char *fileName, FILE *file);

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
    short *instructionsArray, *dataArray;
    linkedList *symbolsTable;  // id is row number data is the error , id is symbol name data is memory address

    isFileValid = 1;
    instructionsCounter = 0; //need to be initialized here?
    instructionsArray = malloc(sizeof(short));
    dataCounter = 0;  //need to be initialized here?
    dataArray = malloc(sizeof(short));

    isFileValid = runFirstTransition(file, instructionsCounter, instructionsArray, dataCounter, dataArray,
                                     fileName);
    if (isFileValid) {
        isFileValid = runSecondTransition(file, instructionsCounter, instructionsArray, dataCounter, dataArray,
                                          fileName);
        if (isFileValid) {
            //createFiles();
        }
    }
}

//// array of 7 shorts for the registers(only 14 bits needed)
int runFirstTransition(FILE *file, int ic, short *instructionsArray,
                       int dc, short *dataArray, char *fileName) {
    int isValid, rowCounter, symbolFlag;
    char *line, *lineCopy, *firstWord, *secondWord;
    char **operationsTable;
    linkedList *symbolsTable;

    symbolsTable = createNewLinkedList();
    isValid = TRUE, rowCounter = 1, ic = 0, dc = 0, symbolFlag = 0;
    line = NULL;
    size_t len = 0;
    operationsTable = getOperationsTable();

    while (getline(&line, &len, file) != -1) {
        lineCopy = copyStr(line);
        lineCopy = trim(lineCopy);
        if (!isCommentLine(lineCopy) && !isEmptyLine(lineCopy)) {
            firstWord = getToken(line, ' ', 0);
            if (isSymbol(firstWord)) {
                firstWord[strlen(firstWord) - 1] = '\0';  // Without the ':' at the end. maybe not a good practice
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
                    dc += saveGuidanceLine(lineCopy, dataArray);
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
                        dc += saveGuidanceLine(lineCopy, dataArray);
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
                //ic += saveCodeLine(lineCopy, instructionsArray);
            } else {
                isValid = FALSE;
            }

            //free(firstWord); //todo: need to understand why doesn't work with this line. maybe because we use it!!
            //free(secondWord);//todo: need to understand why doesn't work with this line
        }
        rowCounter++;
        symbolFlag = 0;
        ////free
        free(lineCopy);
    }
//    if (postValidation(isValid)) {
//        todo: create linkedList with all the symbols that are defined as arguments (exclude entry) in the file with the row
//        todo: validate here if any of those symbols are not appear in the symbols table
//        //updateDataSymbolsValues(symbolsTable, ic);
//    }
    /*
     * 1.0.0 init IC<-0 DC<-0
     * 2.0.0 while there is lines in the file
     *      2.1.0 if getCommand() == SYMBOL
     *          2.1.1 symbolFlag <- 1
     *      2.2.0 else if this is .data/.string
     *          2.2.1 if symbolFlag == 1
     *              2.2.1.0 if this symbol already exist - ERROR
     *              2.2.1.1 else add to the symbols table (data type) with value=DC
     *              2.2.1.2 return to 2.0.0
     *      2.3.0 else if this is .extern/.entry?
     *          2.3.1 this is .extern?
     *              2.3.1.0 add all operands command as symbols to the symbols tables with value=EXTERNAL_TYPE
     *              2.2.1.1 return to 2.0.0
     *      2.4.0 if symbolFlag == 1
     *          2.4.1 if this symbol already exist - ERROR
     *          2.4.2 else add to the symbols table with value=IC and "code" sign
     *          2.4.3 search the operation in the operation names table
     *              2.4.3.0 if no found -ERROR
     *              2.4.3.1 else
     *                  2.4.3.1.0 calculate L
     *                  2.4.3.1.1 build the binary code of the first command
     *                  2.4.3.1.2 IC <- IC + L
     * 3.0.0 if there is any errors
     *      3.1.0 print the error in nice manner
     * 4.0.0 else
     *      4.1.0 update symbols table
     * 5.0.0 return isValid
     * */
    return isValid;
}


int runSecondTransition(FILE *file, int ic, short *instructionsArray,
                        int dc, short *dataArray, char *fileName) {
    //todo: validate arguments names with the symbolsTable
    int isValid, rowCounter;
    isValid = 1;
    rowCounter = 1;


    return isValid;
}