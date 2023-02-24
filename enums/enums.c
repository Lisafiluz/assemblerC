//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include "enums.h"
char *getFullErrorMessage(enum errorMessages errorType) {
    switch (errorType) {
        case SYMBOL_EXCEED_MAX_LEN: return "aaaa";
        case SYMBOL_IS_EMPTY: return "cccc";
        default: return "Internal Error";
    }
}