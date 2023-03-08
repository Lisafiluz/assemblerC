//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#ifndef ASSEMBLERC_ASSEMBLYENCODER_H
#define ASSEMBLERC_ASSEMBLYENCODER_H

#include "../structs/list.h"

int saveGuidanceLine(char *line, int symbolFlag, linkedList *dataList, int dc);

int saveCodeLine(char* line, int symbolFlag, const char **operationsTable, linkedList *instructionsList, int ic);

char *getBinaryEncodedWord(short word);
#endif //ASSEMBLERC_ASSEMBLYENCODER_H
