//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include "enums.h"
char *getFullErrorMessage(enum errorMessages errorType) {
    switch (errorType) {
        case INVALID_MACRO_NAME: return "Invalid Macro Name";
        case SYMBOL_EXCEED_MAX_LEN: return "Symbol name exceed max len of 30 characters";
        case SYMBOL_IS_EMPTY: return "Symbol name cannot be empty";
        case NOT_A_VALID_NUMBER: return "Not a valid number";
        case DUPLICATE_SYMBOL_NAME: return "duplicate symbol name";
        case EMPTY_NUMBER: return "empty number";
        case NOT_A_VALID_STRING: return "Not a valid String";
        case TOO_MUCH_ARGUMENTS: return "Too much arguments in the command";
        case INVALID_COMMAND_NUMBER_ARGUMENTS: return "Invalid number of arguments for command";
        case OPERATION_NAME_NOT_FOUND: return "Operation name was not found";
        case SYMBOL_NOT_FOUND: return "Symbol used but not found";
        default: return "Internal Error";
    }
}

char *getFullWarningMessage(enum warningMessages warningType) {
    switch (warningType) {
        case REDUNDANT_SYMBOL: return "Redundant symbol declaration found";
        default: return "Internal warning";
    }
}