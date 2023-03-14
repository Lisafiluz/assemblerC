#include "enums.h"

char *getFullErrorMessage(enum errorMessages errorType) {
    switch (errorType) {
        case INVALID_MACRO_NAME:
            return "Invalid Macro Name";
        case LINE_EXCEED_MAX_LEN:
            return "Line exceed max len of 80 characters";
        case SYMBOL_EXCEED_MAX_LEN:
            return "Symbol name exceed max len of 30 characters";
        case SYMBOL_IS_EMPTY:
            return "Symbol name cannot be empty";
        case SYMBOL_SYNTAX_NAME:
            return "Symbol invalid syntax";
        case NOT_A_VALID_NUMBER:
            return "Not a valid number";
        case DUPLICATE_SYMBOL_NAME:
            return "Duplicate symbol name";
        case SYMBOL_CANT_BE_REGISTER:
            return "Symbol name cannot be the same as register name";
        case EMPTY_NUMBER:
            return "Empty number";
        case NOT_A_VALID_STRING:
            return "Not a valid String";
        case TOO_MUCH_ARGUMENTS:
            return "Too much arguments in the command";
        case NO_ARGUMENTS:
            return "No arguments found for command";
        case INVALID_COMMAND_NUMBER_ARGUMENTS:
            return "Invalid number of arguments for command";
        case INVALID_SOURCE_ARGUMENT_ADDRESS_METHOD:
            return "Invalid source argument address method";
        case INVALID_TARGET_ARGUMENT_ADDRESS_METHOD:
            return "Invalid target argument address method";
        case OPERATION_NAME_NOT_FOUND:
            return "Operation name was not found";
        case SYMBOL_NOT_FOUND:
            return "Symbol used but not found";
        case INTERNAL_ERROR:
            return "Internal Error occurred";
        default:
            return "Internal Error";
    }
}

char *getFullWarningMessage(enum warningMessages warningType) {
    switch (warningType) {
        case REDUNDANT_SYMBOL:
            return "Redundant symbol declaration found";
        default:
            return "Internal warning";
    }
}
