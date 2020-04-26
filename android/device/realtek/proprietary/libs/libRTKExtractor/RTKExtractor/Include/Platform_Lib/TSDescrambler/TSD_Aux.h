#ifndef  __TSD_AUX_H__
#define  __TSD_AUX_H__

#include "TSD_Def.h"

const char* GetTSDAlgorithmStr(DESCRAMBLE_ALGO Algo);
const char* GetTSDModeStr(DESCRAMBLE_MODE Mode);

DESCRAMBLE_ALGO GetTSDAlgorithmByStr(const char* Str);
DESCRAMBLE_MODE GetTSDModeByStr(const char* Str);

void TSD_DumpMemory(unsigned char* pData, unsigned long len);
#define TSD_DumpMemoryWithText(pData, len, fmt, args...)     do { printf(fmt, ## args); TSD_DumpMemory(pData, len); }while(0)



#endif   //__TSD_AUX_H__
