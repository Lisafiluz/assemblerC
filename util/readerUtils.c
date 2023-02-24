//
// Created by Iluz, Lisaf on 14/01/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "readerUtils.h"

#define START_SIZE 80  // Each Assembly sentence has 80 chars
#define ADD_SIZE 64  // In case we need to get Staring that bigger than 80 chars
#define TRUE 1
#define FALSE 0

char *ltrim(char *str);

char *rtrim(char *str);

//todo: 1.understand 2.use other variable names 3. read in the book for alternate solution
char *readData(FILE *fp) {
    int size, len;
    char *str;
    int ch;

    size = START_SIZE;
    len = 0;  // todo: understand this type size_t
    str = realloc(NULL, sizeof(*str) * START_SIZE);  /*size is start size*/
    // if (!str)return str;  //todo probably for not enough memory, remove it or find it in the book
    while (EOF != (ch = fgetc(fp)) && ch != '\n') {
        str[len++] = ch;
        if (len == size) {
            str = realloc(str, sizeof(*str) * (size += ADD_SIZE));
            //if (!str)return str;
        }
    }
    str[len++] = '\0';

    return realloc(str, sizeof(*str) * len);
}

char *trim(char *str) {
    str = ltrim(str);
    str = rtrim(str);
    return str;
}

char *rtrim(char *str) {
    char *lastChar = str + strlen(str);
    while (isspace(*--lastChar));
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

    token = (char *) malloc(strlen(line) * sizeof(char));
    i = 0;
    tokenIdx = 0;

    for (; tokenNumber > 0; tokenNumber--) {
        while (line[i] != delim) {
            i++;
        }
        while (line[i] == delim) {
            i++;
        }
    }
    while (line[i] != delim && i < strlen(line)) {
        token[tokenIdx] = line[i];
        i++;
        tokenIdx++;
    }
    token[tokenIdx + 1] = '\0';
    return token;
}

char *getSecondToken(const char *line) {

}

int isEqual(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0 ? 1 : 0;
}

char *getFileName(char **const argv, int i, char *suffix) {
    char *fileName;
    fileName = (char *) calloc(strlen(argv[i]) + 4, sizeof(char));
    strcpy(fileName, argv[i]);
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

int hasSpaces(char *str) {
    int i;
    size_t size = strlen(str);
    for (i = 0; i < size; i++) {
        if (isspace(str[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

int isAlphaNumeric(char *str) {
    int i;
    size_t size = strlen(str);
    for (i = 0; i < size; i++) {
        if (!isalpha(str[i]) && !isdigit(str[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

int isValidNumber(char *number) {
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

char *copyStr(char *str) {
    char *copy;
    copy = (char *) malloc(strlen(str) * sizeof(char));
    strcpy(copy, str);
    return copy;
}

