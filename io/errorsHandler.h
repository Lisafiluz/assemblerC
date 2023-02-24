//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#ifndef ASSEMBLERC_ERRORSHANDLER_H
#define ASSEMBLERC_ERRORSHANDLER_H

#include "../enums/enums.h"

void printError(enum errorMessages errorType, const char *data, const char *fileName, int rowCounter);
#endif //ASSEMBLERC_ERRORSHANDLER_H
