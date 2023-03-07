//
// Created by Iluz, Lisaf(AWF) on 07/03/2023.
//

#include <stdlib.h>
#include "shortData.h"

shortData *createShortData(short value) {
    shortData *s;
    s = (shortData *) malloc(sizeof(shortData));
    s->value = value;

    return s;
}