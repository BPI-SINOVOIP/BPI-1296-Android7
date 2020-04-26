#ifndef RVSD_COMMON_RTK_H
#define RVSD_COMMON_RTK_H
#include "OSAL_RTK.h"

struct IONStruct
{
	ion_user_handle_t pIonHandle;
#if defined(__LP64__)
	unsigned long nSize;
	unsigned long pIonPhyAddress;
	unsigned long pIonVirtAddress;
#else
	unsigned int nSize;
	unsigned int pIonPhyAddress;
	unsigned int pIonVirtAddress;
#endif
};

enum
{
	VDEC_EOS_NONE,
	VDEC_EOS_END_OF_SEGMENT,	// inband EOS event ID = -1 , no need to notify system
	VDEC_EOS_END_OF_STREAM, 	// inband EOS event ID != -1, need to notify system
	VDEC_SEQ_END				// inband SEQ_END
};

#endif // #ifdef RVSD_COMMON_RTK_H

