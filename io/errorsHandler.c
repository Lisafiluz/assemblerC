//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#include <printf.h>
#include "errorsHandler.h"

void printError(enum errorMessages errorType, const char *data, const char *fileName, int rowCounter) {
    fprintf(stdout,
            "ERROR: %s! (params: data=%s file_name=%s; row_number=%d)\n",
            getFullErrorMessage(errorType), data, fileName, rowCounter);
}
