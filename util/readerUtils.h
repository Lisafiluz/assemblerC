/*
 Created by Iluz, Lisaf(AWF) on 03/02/2023.
*/


#ifndef ASSEMBLERC_READERUTILS_H
#define ASSEMBLERC_READERUTILS_H

#include <stdio.h>

char *trim(char *str);

char *getToken(const char *line, char delim, int tokenNumber);

int isEqual(const char* str1, const char* str2);

char *getFileName(char **argv, int i, char *suffix);

char *getOutputFileName(const char *sourceFileName, const char *suffix);

int isCommentLine(char *line);

int isEmptyLine(char *line);

int hasSpaces(const char *str);

int isAlphaNumeric(const char *str);

int isValidNumber(const char *number);

char *copyStr(const char *str);

char *getSecondWord(const char *line, int symbolFlag);
#endif
