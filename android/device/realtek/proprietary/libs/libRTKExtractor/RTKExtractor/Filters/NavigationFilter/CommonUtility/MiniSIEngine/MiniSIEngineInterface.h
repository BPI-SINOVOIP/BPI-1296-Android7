#ifndef MINI_SI_ENGINE_INTERFACE_H
#define MINI_SI_ENGINE_INTERFACE_H

typedef enum
{
	MS_EVENT_GOT_PAT = 0,
	MS_EVENT_GOT_PMT = 1,
	
} MINI_SI_EVENT_E;

typedef struct
{
	MINI_SI_EVENT_E eventId;
	unsigned char* pEventData;
	int eventDataSize;
} MINI_SI_EVENT_T;

typedef struct _tagMINI_SI_ENGINE_OUT
{
	long instanceID;
	
	int (*siEventHandler)(long instanceID, MINI_SI_EVENT_T* pEvent);
	
}MINI_SI_ENGINE_OUT;

#endif//MINI_SI_ENGINE_INTERFACE_H
