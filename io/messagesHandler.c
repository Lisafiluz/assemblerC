//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <printf.h>
#include "messagesHandler.h"

void printError(enum errorMessages errorType, const char *data, const char *fileName, int rowCounter) {
    fprintf(stdout,
            "ERROR: %s! (params: data=%s  |  file_name=%s  |  row_number=%d)\n",
            getFullErrorMessage(errorType), data, fileName, rowCounter);
}

void printWarning(enum warningMessages warningType,const char *data, const char *fileName, int rowCounter) {
    fprintf(stdout,
            "WARNING: %s! (params: data=%s  |  file_name=%s  |  row_number=%d)\n",
            getFullWarningMessage(warningType), data, fileName, rowCounter);
}
