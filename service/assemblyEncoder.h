//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#ifndef ASSEMBLERC_ASSEMBLYENCODER_H
#define ASSEMBLERC_ASSEMBLYENCODER_H

int saveGuidanceLine(char *line, int symbolFlag, short *dataArray, int dc);

int saveCodeLine(char* line, int symbolFlag, const char **operationsTable, short* instructionsArray, int ic);
#endif //ASSEMBLERC_ASSEMBLYENCODER_H
