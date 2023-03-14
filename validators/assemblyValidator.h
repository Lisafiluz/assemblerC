/*
 Created by Iluz, Lisaf(AWF) on 03/02/2023.
*/


#ifndef ASSEMBLERC_ASSEMBLYVALIDATOR_H
#define ASSEMBLERC_ASSEMBLYVALIDATOR_H

#include "../structs/list.h"

int validateMacroName(const char *macroName);

int validateBasicLine(const char *line, const char *fileName, int rowCounter);

int validateSymbolName(char *symbol, linkedList  symbolsTable, char *fileName, int rowCounter);

int validateDataGuidanceLine(char *line, int symbolFlag, char *fileName, int rowCounter);

int validateExternalEntryGuidanceLine(const char *line, int symbolFlag, linkedList  symbolsTable, char *fileName,
                                      int rowCounter);

int validateCodeLine(char *line, int symbolFlag, const char **operationsTable, char *fileName, int rowCounter);

int isValidDirectAddressArgument(const char *argument);

int isValidRegister(const char *argument);

int isValidSymbol(const char *argument);

int isValidJumpAddressArgMethod(char *argument);

#endif
