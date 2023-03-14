#ifndef ASSEMBLERC_SYMBOL_H
#define ASSEMBLERC_SYMBOL_H
#include "../enums/enums.h"

typedef struct symbol *symbol;

symbol createSymbol(char *symbolName, enum symbolTypes type, int value);

char *getSymbolName(symbol symbol);

enum symbolTypes getSymbolType(symbol symbol);

int getSymbolValue(symbol symbol);

void increaseSymbolValue(symbol symbol, int valueToAdd);
#endif
