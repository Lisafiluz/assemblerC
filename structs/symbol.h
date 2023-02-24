//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#ifndef ASSEMBLERC_SYMBOL_H
#define ASSEMBLERC_SYMBOL_H
#include "../enums/enums.h"

typedef struct symbol {
    char *symbolName;
    enum symbolTypes symbolType;
    int value;
} symbol;

symbol *createSymbol(char *symbolName, enum symbolTypes type, int value);


#endif //ASSEMBLERC_SYMBOL_H
