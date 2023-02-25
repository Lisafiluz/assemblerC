//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include "enums.h"
char *getFullErrorMessage(enum errorMessages errorType) {
    switch (errorType) {
        case SYMBOL_EXCEED_MAX_LEN: return "aaaa";
        case SYMBOL_IS_EMPTY: return "cccc";
        case NOT_A_VALID_NUMBER: return "xxx";
        case DUPLICATE_SYMBOL_NAME: return "duplicate symbol name";
        case EMPTY_NUMBER: return "empty number";
        case NOT_A_VALID_STRING: return "yyy";
        default: return "Internal Error";
    }
}

char *getFullWarningMessage(enum warningMessages warningType) {
    switch (warningType) {
        case REDUNDANT_SYMBOL: return "aaaa";
        default: return "Internal warning";
    }
}