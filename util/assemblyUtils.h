#ifndef ASSEMBLERC_ASSEMBLYUTILS_H
#define ASSEMBLERC_ASSEMBLYUTILS_H

int isDataGuidance(char *str);

int isExternalGuidance(char *str);

int isExternalOrEntryGuidance(char *str);

int getNumberOfArgs(const char *command);

char *getArgument(const char *command, int argumentNumber);

char *getParam(const char *argument, int paramNumber);

int getArgumentAddressMethod(char *arg);

char *getLineWithoutSymbol(char *line);

#endif
