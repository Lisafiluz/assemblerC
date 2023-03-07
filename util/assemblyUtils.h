//
// Created by Iluz, Lisaf(AWF) on 24/02/2023.
//

#ifndef ASSEMBLERC_ASSEMBLYUTILS_H
#define ASSEMBLERC_ASSEMBLYUTILS_H

int isSymbol(char *str);

int isDataGuidance(char *str);

int isExternalGuidance(char *str);

int isExternalOrEntryGuidance(char *str);

int getNumberOfArgs(const char *command);

char *getArgument(const char *command, int argumentNumber);

char *getParam(const char *argument, int paramNumber);

int getArgumentAddressMethod(char *arg);

char *getLineWithoutSymbol(char *line);

#endif //ASSEMBLERC_ASSEMBLYUTILS_H
