#ifndef ASSEMBLERC_MESSAGESHANDLER_H
#define ASSEMBLERC_MESSAGESHANDLER_H

#include "../enums/enums.h"

void printError(enum errorMessages errorType, const char *data, const char *fileName, int rowCounter);

void printWarning(enum warningMessages warningType,const char *data, const char *fileName, int rowCounter);
#endif
