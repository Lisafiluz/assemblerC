#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "readerUtils.h"

#define TRUE 1
#define FALSE 0

char *ltrim(char *str);

char *rtrim(char *str);

char *trim(char *str) {
    str = ltrim(str);
    str = rtrim(str);
    return str;
}

char *rtrim(char *str) {
    char *lastChar;
    lastChar = str + strlen(str) - 1;
    while (isspace(*lastChar)) {
        lastChar--;
    }
    *(++lastChar) = '\0';
    return str;
}

char *ltrim(char *str) {
    while (isspace(*str)) str++;
    return str;
}

char *getToken(const char *line, const char delim, int tokenNumber) {
    char *token;
    int i, tokenIdx;

    token = (char *) malloc((strlen(line) + 1) * sizeof(char));
    i = 0;
    tokenIdx = 0;

    for (; tokenNumber > 0; tokenNumber--) {
        while (line[i] != delim && i < strlen(line)) {
            i++;
        }
        while (line[i] == delim && i < strlen(line)) {
            i++;
        }
    }
    while (line[i] != delim && i < strlen(line)) {
        token[tokenIdx] = line[i];
        i++;
        tokenIdx++;
    }
    token[tokenIdx] = '\0';
    return token;
}

int isEqual(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0 ? TRUE : FALSE;
}

char *getFileName(char **const argv, int i, char *suffix) {
    char *fileName;
    fileName = (char *) calloc(strlen(argv[i]) + 4, sizeof(char));
    strcpy(fileName, argv[i]);
    strcat(fileName, suffix);
    fileName[strlen(fileName)] = '\0';
    return fileName;
}

char *getOutputFileName(const char *sourceFileName, const char *suffix) {
    char *fileName;
    fileName = (char *) calloc(strlen(sourceFileName) + 4, sizeof(char));
    strcpy(fileName, sourceFileName);
    strcat(fileName, suffix);
    fileName[strlen(fileName)] = '\0';
    return fileName;
}

int isCommentLine(char *line) {
    return strlen(line) > 0 && line[0] == ';';
}

int isEmptyLine(char *line) {
    size_t length;
    length = strlen(line);
    while (isspace(*line)) {
        line++;
        length--;
    }
    return length == 0;
}

int hasSpaces(const char *str) {
    int i;
    size_t size = strlen(str);
    for (i = 0; i < size; i++) {
        if (isspace(str[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

int isAlphaNumeric(const char *str) {
    int i;
    size_t size = strlen(str);
    for (i = 0; i < size; i++) {
        if (!isalpha(str[i]) && !isdigit(str[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

int isValidNumber(const char *number) {
    int i;
    size_t size;

    i = 0;
    size = strlen(number);
    if (size == 0) {
        return FALSE;
    }
    if (number[0] == '-' || number[0] == '+') {
        i = 1;
    }

    for (; i < size; i++) {
        if (!isdigit(number[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

char *copyStr(const char *str) {
    char *copy;
    copy = (char *) malloc((strlen(str) + 1) * sizeof(char));
    strcpy(copy, str);
    return copy;
}

char *getSecondWord(const char *line, int symbolFlag) {
    if (symbolFlag) {
        char *secondWord;
        int i, wordIdx;
        i = 0, wordIdx = 0;
        secondWord = (char *) malloc(strlen(line) * sizeof(char));
        while (line[i] != ':' && i < strlen(line)) i++;
        i++;
        while (isspace(line[i]) && i < strlen(line)) i++;
        while (!isspace(line[i]) && i < strlen(line)) {
            secondWord[wordIdx] = line[i];
            wordIdx++;
            i++;
        }
        secondWord[wordIdx] = '\0';
        return secondWord;

    } else {
        return getToken(line, ' ', 1);
    }
}

