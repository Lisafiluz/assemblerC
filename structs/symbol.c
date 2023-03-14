#include <stdlib.h>
#include "symbol.h"

struct symbol {
    char *symbolName;
    enum symbolTypes symbolType;
    int value;
};

symbol createSymbol(char *symbolName, enum symbolTypes type, int value) {
    symbol s;
    s = malloc(sizeof(*s));
    s->symbolName = symbolName;
    s->symbolType = type;
    s->value = value;

    return s;
}

char *getSymbolName(symbol symbol) {
    return symbol->symbolName;
}

enum symbolTypes getSymbolType(symbol symbol) {
    return symbol->symbolType;
}

int getSymbolValue(symbol symbol) {
    return symbol->value;
}

void increaseSymbolValue(symbol symbol, int valueToAdd) {
    symbol->value += valueToAdd;
}
