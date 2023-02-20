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

char* trim(char *str) {
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

char *getCommandName(const char *line) {
    char *command;
    int i;

    command = (char *)malloc(sizeof(char));
    i = 0;

    while (!isspace(line[i]) && i < strlen(line)) {
        command[i] = line[i];
        i++;
    }
    command[i + 1] = '\0';
    return command;
}

