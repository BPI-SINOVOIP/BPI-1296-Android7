#ifndef IDFBEVENTBUFFER_H
#define IDFBEVENTBUFFER_H

#include "IDFBBase.h"

class IDFBEventBuffer : public IDFBBase<IDirectFBEventBuffer> 
{
	friend class IDFBInputDevice;
	friend class IDFBWindow;

public:
	IDFBEventBuffer(IDirectFBEventBuffer* dfbif = 0) : IDFBBase <IDirectFBEventBuffer> (dfbif) {}

	DFBResult	Reset();
	DFBResult	WaitForEvent();
	DFBResult	WaitForEventWithTimeout(unsigned int seconds, unsigned int milliSeconds);
	DFBResult	WakeUp();
	DFBResult	GetEvent(DFBEvent &event);
	DFBResult	PeekEvent(DFBEvent &event);
	DFBResult	HasEvent();
	DFBResult	PostEvent(const DFBEvent &event);
	DFBResult	CreateFileDescriptor(int &fd);
};

#endif
