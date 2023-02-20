//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#ifndef ASSEMBLERC_READERUTILS_H
#define ASSEMBLERC_READERUTILS_H

#include <stdio.h>

char *readData(FILE *fp);

char *trim(char *str);

char *getCommandName(const char *line);

#endif //ASSEMBLERC_READERUTILS_H
