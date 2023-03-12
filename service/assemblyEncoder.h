#ifndef ASSEMBLERC_ASSEMBLYENCODER_H
#define ASSEMBLERC_ASSEMBLYENCODER_H

#include "../structs/list.h"

int saveGuidanceLine(char *line, int symbolFlag, linkedList *dataList, int dc);

int saveCodeLine(char *line, int symbolFlag, const char **operationsTable, linkedList *instructionsList);

char *getBinaryEncodedWord(short word);
#endif
