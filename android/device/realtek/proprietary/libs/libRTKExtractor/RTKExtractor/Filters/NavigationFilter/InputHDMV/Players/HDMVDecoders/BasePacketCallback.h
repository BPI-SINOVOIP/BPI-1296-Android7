//-----------------------------------------------------------------------------
// BasePacketCallback.h
//
// Base Packet Callback class declarations for use by HD Renderer
//
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef BASE_PACKET_CALLBACK_H
#define BASE_PACKET_CALLBACK_H

#ifdef REALTEK_MODIFY
	#include "packetdef.h"
#else
	#include "RendererTypes.h"
	#include "IPacketSink.h" // old style used by HD Demuxer
	#include "IPacketSinkCallback.h" // new style used by HD Nav
#endif
#include "ref_ptr.h"

#ifdef REALTEK_MODIFY
class RENDERER_EXPORT BasePacketCallback 
#else
class RENDERER_EXPORT BasePacketCallback : public IPacketSink, public IPacketSinkCallback
#endif
{
public:
	explicit BasePacketCallback(UInt16 streamID, UInt16 subStreamID) : mStreamID(streamID), mSubStreamID(subStreamID) { }
	virtual ~BasePacketCallback() { }

	virtual void ReceivePacket(UInt16 streamId, PACKET_DATA* pPacket) = 0; // old style used by HD Demuxer

#if 1
	// Using raw pointer instead of ref_ptr. ref_ptr causes mem corruption if DevRelease and Debug builds (of HDAVSink and
	// HDR,Nav - HDR and Nav are always same config) are mixed (don't know why)
	//
	void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, PACKET_DATA * pPacket)
	{
		ref_ptr<PACKET_DATA> pd(pPacket);
		ReceivePacket(id, streamId, subStreamId, pd);
	}

protected:
	virtual void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> pPacket) = 0; // new style used by HD Nav

public:

#else
	virtual void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> pPacket) = 0; // new style used by HD Nav
#endif

	UInt16 mStreamID;
	UInt16 mSubStreamID;

#ifdef COMRENDERER

protected:
	void * mPtr;	// opaque pointer to COM interface

#else	// COMRENDERER
#endif	// COMRENDERER
};

#endif	// BASE_PACKET_CALLBACK_H
