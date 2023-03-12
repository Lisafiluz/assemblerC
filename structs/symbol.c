#include <stdlib.h>
#include "symbol.h"

symbol *createSymbol(char *symbolName, enum symbolTypes type, int value) {
    symbol *s;
    s = (symbol *) malloc(sizeof(symbol));
    s->symbolName = symbolName;
    s->symbolType = type;
    s->value = value;

    return s;
}
