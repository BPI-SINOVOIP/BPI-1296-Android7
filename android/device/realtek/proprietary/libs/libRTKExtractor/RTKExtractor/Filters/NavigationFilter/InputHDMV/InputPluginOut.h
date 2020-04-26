#ifndef __INPUT_HDMV_OUT_H__
#define __INPUT_HDMV_OUT_H__

#include <Filters/NavigationFilter/SiEngine.h>

typedef struct _tagPIDPrivateData {
	
	unsigned long	srcStreamIndex;		//to get av stream
	void*			pPacketCallback;	//for IG, to receive packets
	
	
	unsigned long	pbEndCallback;		//playback end callback instance ID
	int				pbEndCBData;		//playback end callback data
	int				pbEndCBOutputID;	//playback end dest type
	
} PID_PRIVATE_DATA;

typedef struct _tagINPUT_HDMV_OUT {

	unsigned int instanceID;	//a pointer to input plugin instance
	
	int (*RemovePID)(unsigned int instanceID, int count, unsigned short* pidArray);
	int (*InsertPID)(unsigned int instanceID, int count, SI_FILTER_DESC* filterDescArray);
	void (*FlushPin)(unsigned int instanceID, SI_FILTER_TYPE type);
	void (*UpdatePBEndCallback)(unsigned int instanceID, SI_FILTER_TYPE type, unsigned int pbEndCallback, int pbEndCBData, int pbEndCBOutputID);
	
} INPUT_HDMV_OUT;

#endif

