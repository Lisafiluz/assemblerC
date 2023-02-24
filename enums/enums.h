//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#ifndef ASSEMBLERC_ENUMS_H
#define ASSEMBLERC_ENUMS_H
enum symbolTypes{DATA_TYPE, EXTERNAL_TYPE, ENTRY_TYPE, CODE_TYPE};
extern enum symbolTypes symbolTypes;

enum errorMessages{
    SYMBOL_EXCEED_MAX_LEN,
    SYMBOL_IS_EMPTY,
    SYMBOL_SYNTAX_NAME,
    DUPLICATE_SYMBOL_NAME,
    NOT_A_VALID_NUMBER,
    NOT_A_VALID_STRING
};
extern enum errorMessages errorMessages;

char *getFullErrorMessage(enum errorMessages errorType);
#endif //ASSEMBLERC_ENUMS_H