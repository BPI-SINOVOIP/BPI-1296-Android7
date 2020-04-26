//----------------------------------------------------------------------------- 
// packetdef.h
// Copyright (c) 2002 - 2005, Sonic Solutions.  All rights reserved.
// Use is subject to certain restrictions.
//----------------------------------------------------------------------------- 

#ifndef _PACKETDEF_H_
#define _PACKETDEF_H_

#include "CommonTypes.h"
#include "stdio.h"	//for size_t

// Packets begin with a header followed by data.  If no header exists,
// nHeaderLen is 0.
class PACKET_DATA
{
public:
    UInt8*      pData;
    size_t      nPacketLen;
    size_t      nHeaderLen;
    UInt16      nStreamId;
    UInt16      nSubStreamId;
    bool        bHasPts;
    bool        bScrambled;
    SInt64      llPts;
    
    virtual ~PACKET_DATA() { /*printf("[%s %d] delete PACKET_DATA = [%p]\n", __FILE__, __LINE__, this);*/ }

};

#endif //_PACKETDEF_H_
