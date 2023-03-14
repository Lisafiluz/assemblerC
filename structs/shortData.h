#ifndef ASSEMBLERC_SHORTDATA_H
#define ASSEMBLERC_SHORTDATA_H

typedef struct shortDataPtr *shortDataPtr;

shortDataPtr createShortData(short value);

short getShortDataValue(shortDataPtr shortDataPtr);

#endif
