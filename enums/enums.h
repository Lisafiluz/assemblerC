#ifndef ASSEMBLERC_ENUMS_H
#define ASSEMBLERC_ENUMS_H
enum symbolTypes {
    DATA_TYPE, EXTERNAL_TYPE, ENTRY_TYPE, CODE_TYPE
};
extern enum symbolTypes symbolTypes;

enum errorMessages {
    INVALID_MACRO_NAME,
    LINE_EXCEED_MAX_LEN,
    SYMBOL_EXCEED_MAX_LEN,
    SYMBOL_IS_EMPTY,
    SYMBOL_SYNTAX_NAME,
    DUPLICATE_SYMBOL_NAME,
    SYMBOL_CANT_BE_REGISTER,
    NOT_A_VALID_NUMBER,
    EMPTY_NUMBER,
    NOT_A_VALID_STRING,
    TOO_MUCH_ARGUMENTS,
    NO_ARGUMENTS,
    OPERATION_NAME_NOT_FOUND,
    INVALID_COMMAND_NUMBER_ARGUMENTS,
    INVALID_SOURCE_ARGUMENT_ADDRESS_METHOD,
    INVALID_TARGET_ARGUMENT_ADDRESS_METHOD,
    SYMBOL_NOT_FOUND,
    INTERNAL_ERROR
};
extern enum errorMessages errorMessages;

enum warningMessages {
    REDUNDANT_SYMBOL
};
extern enum warningMessages warningMessages;

char *getFullErrorMessage(enum errorMessages errorType);

char *getFullWarningMessage(enum warningMessages warningType);

#endif
