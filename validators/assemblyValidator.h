//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#ifndef ASSEMBLERC_ASSEMBLYVALIDATOR_H
#define ASSEMBLERC_ASSEMBLYVALIDATOR_H
int validateSymbolName(char* symbol, char* fileName, int rowCounter);
int validateDataGuidanceLine(char* line, int symbolFlag, char* fileName, int rowCounter);
int validateExternalGuidanceLine(char* line, int symbolFlag, char* fileName, int rowCounter);
#endif //ASSEMBLERC_ASSEMBLYVALIDATOR_H
