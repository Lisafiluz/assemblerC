#include <stdlib.h>
#include "shortData.h"

struct shortDataPtr {
    short value;
};

shortDataPtr createShortData(short value) {
    shortDataPtr s;
    s = malloc(sizeof(*s));
    s->value = value;

    return s;
}

short getShortDataValue(shortDataPtr shortDataPtr) {
    return shortDataPtr->value;
}
