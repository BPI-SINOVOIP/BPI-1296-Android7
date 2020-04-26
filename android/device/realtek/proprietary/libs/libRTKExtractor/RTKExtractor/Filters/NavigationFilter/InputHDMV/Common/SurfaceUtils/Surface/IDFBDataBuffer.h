#ifndef IDFBDATABUFFER_H
#define IDFBDATABUFFER_H

#include "IDFBBase.h"
#include "IDFBImageProvider.h"
#include "IDFBVideoProvider.h"

class IDFBDataBuffer : public IDFBBase<IDirectFBDataBuffer> 
{
public:
	IDFBDataBuffer(IDirectFBDataBuffer *dfbif = 0) : IDFBBase <IDirectFBDataBuffer> (dfbif) {}

	DFBResult	Flush();
	DFBResult	SeekTo(unsigned int offset);
	DFBResult	GetPosition(unsigned int &position);
	DFBResult	GetLength(unsigned int &length);
	DFBResult	WaitForData(unsigned int length);
	DFBResult	WaitForDataWithTimeout(unsigned int length, unsigned int seconds, unsigned int milliSeconds);
	DFBResult	GetData(unsigned int length, void *data, unsigned int &dataLength);
	DFBResult	PeekData(unsigned int length, int offset, void *data, unsigned int &dataLength);
	DFBResult	HasData();
	DFBResult	PutData(const void *data, unsigned int length);
	DFBResult	CreateImageProvider(IDFBImageProvider *&imageProvider);
	DFBResult	CreateVideoProvider(IDFBVideoProvider *&videoProvider);
};

#endif
