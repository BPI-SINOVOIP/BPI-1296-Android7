//-----------------------------------------------------------------------------
// HDMVDecoders_PacketSink.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODERS_PACKETSINK_H_
#define __HDMVDECODERS_PACKETSINK_H_

#include "BasePacketCallback.h"
#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_Types.h"


//__________________________________________________________________________________________________________________________________________________
// HDMV Decoders Packet Sink
//
class HDMVDecoders_PacketSink : public BasePacketCallback
{
public:
	HDMVDecoders_PacketSink() : BasePacketCallback(0, 0)	{ }
	virtual ~HDMVDecoders_PacketSink()						{ }

	// CL0512: pass connection_type in StartOfStream
	virtual void StartOfStream(void)		= 0;
	virtual void EndOfStream(void)			= 0;
    virtual void Flush(void)				= 0;

	// Set application_type (which is found in the ClipInfo() of the clip that is providing data to this sink)
	virtual void SetApplicationType(BDROM_ApplicationType appType)	= 0;

	// Set PTS to be added to PTS extracted from PES packets to sync it with Video PTS.
	virtual void SetSyncOffsetPTS(SInt64 pts90kHz)	= 0;

	// HDMVStreamTimes will have In_location, Out_location, In_time, and Out_time
	// This data becomes valid at In_location.
	virtual void SetStreamExtent(HDMVStreamExtent & extent)			{}

	// This is the start location for the stream. Location is specified as a start location in the main path
	virtual void SetStartLocation(HDMVStreamPlaylistLocation & startLocation)		{}

	// Set character set (which is found in the StreamCodingInfo of the clip that is providing data to this sink)
	virtual void SetCharacterSet(BDROM_CharSet charSet)	= 0;

	virtual void Play(void)					= 0;
	virtual void Pause(void)				= 0;
	virtual void Stop(void)					= 0;
	virtual void SetRate(double dRate)		= 0;

	virtual void OnOff(bool bOn)			= 0;

	virtual void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> pPacket)	= 0; // new style used by HD Nav
	virtual void ReceivePacket(UInt16 /*streamId*/, PACKET_DATA* /*pPacket*/)									= 0; // old style used by HD Demuxer
};

#endif // __HDMVDECODERS_PACKETSINK_H_
