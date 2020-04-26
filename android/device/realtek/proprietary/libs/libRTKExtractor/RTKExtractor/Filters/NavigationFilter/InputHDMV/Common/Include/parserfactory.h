//----------------------------------------------------------------------------- 
// parserfactory.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
// Use is subject to certain restrictions.
//----------------------------------------------------------------------------- 

#ifndef _PARSERFACTORY_H_
#define _PARSERFACTORY_H_


typedef enum tagPARSER_TYPE
{
    eNone,                  // Creates no parser object
    eMPEG2ProgramStream,    // An MPEG-2 program stream (not ripped from a DVD)
    eMPEG2TransportStream,  // An MPEG-2 transport stream
    eMPEG2DVD,              // A VOB from a DVD (where private_stream_2 contains nav packets etc)
    eHDDVDC1,               // An EVOB from a HD DVD Content 1
    eBDAV,                  // A Bluray transport stream
    eMPEG1SystemStream,     // An MPEG-1 system stream
    eMPEG1VCD,              // A VCD track from a VideoCD
    eElementaryStream,      // An elementary stream of any type(p1 and p2 can 
                            // specify the stream id and substream id used for
                            // packet sink callbacks).
    eRawDigitalVideo        // Raw DV stream (like the ones found on BDRE)                            
} PARSER_TYPE;


#endif //_PARSERFACTORY_H_
