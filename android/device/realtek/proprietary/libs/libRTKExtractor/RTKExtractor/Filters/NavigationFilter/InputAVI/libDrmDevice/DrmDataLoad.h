#include "DivXInt.h"

#ifdef __cplusplus
extern "C" {
#endif

int drmDataLoadGetPublicKey( uint8_t* publicKeyE,
                             uint8_t* publicKeyN );
int drmDataLoadGetBaseKey( uint8_t* baseKeyIndexString,
                           uint8_t* baseKey );
int drmDataLoadGetRTK( uint8_t* rtk );
int drmDataLoadSaveDrmMemory( uint8_t* marshalledMemory,
                              uint32_t marshalledMemoryLength );
int drmDataLoadLoadDrmMemory( uint8_t* marshalledMemory,
                              uint32_t marshalledMemoryLength );

int drmDataLoadModelId( uint8_t* modelId );

#ifdef __cplusplus
}
#endif

