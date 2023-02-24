//
// Created by Iluz, Lisaf(AWF) on 03/02/2023.
//

#ifndef ASSEMBLERC_READERUTILS_H
#define ASSEMBLERC_READERUTILS_H

#include <stdio.h>

char *readData(FILE *fp);

char *trim(char *str);

char *getToken(const char *line, char delim, int tokenNumber);

int isEqual(const char* str1, const char* str2);

char *getFileName(char **argv, int i, char *suffix);

int isCommentLine(char *line);

int isEmptyLine(char *line);

int hasSpaces(char *str);

int isAlphaNumeric(char *str);

int isValidNumber(char *number);

char *copyStr(char *str);
#endif //ASSEMBLERC_READERUTILS_H
