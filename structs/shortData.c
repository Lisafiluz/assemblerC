#include <stdlib.h>
#include "shortData.h"

shortData *createShortData(short value) {
    shortData *s;
    s = (shortData *) malloc(sizeof(shortData));
    s->value = value;

    return s;
}
