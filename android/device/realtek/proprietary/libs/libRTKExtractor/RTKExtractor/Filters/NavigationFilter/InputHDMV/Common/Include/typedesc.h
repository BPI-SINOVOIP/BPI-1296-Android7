//----------------------------------------------------------------------------- 
// typedesc.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
// Use is subject to certain restrictions.
//----------------------------------------------------------------------------- 

#ifndef _TYPEDESC_H_
#define _TYPEDESC_H_

#include "CommonTypes.h"
#include "ref_ptr.h"

typedef enum tagTD_TYPE
{
    TD_TYPE_NONE,
    TD_TYPE_AUDIO,
    TD_TYPE_VIDEO
} TD_TYPE;

typedef enum tagTD_SUBTYPE
{
    TD_SUBTYPE_NONE,
    TD_SUBTYPE_AUDIO_MPEG1,
    TD_SUBTYPE_AUDIO_MPEG2,
    TD_SUBTYPE_AUDIO_AC3,
    TD_SUBTYPE_AUDIO_LPCM,
    TD_SUBTYPE_AUDIO_LPCM_HDC1,
    TD_SUBTYPE_AUDIO_DTS,
    TD_SUBTYPE_AUDIO_DTS_HD,
    TD_SUBTYPE_AUDIO_DDPlus,
    TD_SUBTYPE_AUDIO_AAC,
    TD_SUBTYPE_AUDIO_SDDS,
    TD_SUBTYPE_AUDIO_MLP,
    TD_SUBTYPE_VIDEO_MPEG1,
    TD_SUBTYPE_VIDEO_MPEG2,
    TD_SUBTYPE_VIDEO_VC1,
    TD_SUBTYPE_VIDEO_AVC,
    TD_SUBTYPE_VIDEO_SPU,
    TD_SUBTYPE_VIDEO_HDC1_SPU,
    TD_SUBTYPE_VIDEO_INTERACTIVE_GRAPHICS,
    TD_SUBTYPE_VIDEO_PRESENTATION_GRAPHICS,
    TD_SUBTYPE_AUDIO_LPCM_BD,
    TD_SUBTYPE_VIDEO_DV,
    TD_SUBTYPE_AUDIO_DV_LPCM,
    TD_SUBTYPE_AUDIO_MP3,
    TD_SUBTYPE_AUDIO_RAW_PCM,       // big endian
    TD_SUBTYPE_AUDIO_MLP_BD,        // MLP on BD tiles (TrueHD interleaved with DD)
    TD_SUBTYPE_AUDIO_DTS_HD_LBR,    // LBR extension stream w/o DTS-HD core stream
    TD_SUBTYPE_VIDEO_BD_TEXT,       // Text subtitles on BD
    
    TD_SUBTYPE_MAX_SETTING          // Max value for this enum set
} TD_SUBTYPE;

class STREAM_DESC
{
public:
    STREAM_DESC(TD_TYPE t=TD_TYPE_NONE, TD_SUBTYPE st=TD_SUBTYPE_NONE) 
    : type(t)
    , subtype(st)
    , hdrBytes(0)
    , id(0)
    , subid(0)
    {}
    
    virtual ~STREAM_DESC() {}

    TD_TYPE                 type;
    TD_SUBTYPE              subtype;
    
    UInt32                  hdrBytes;
    ref_ptr_array<UInt8>    hdr;
    
    UInt16                  id;
    UInt16                  subid;
};

class VIDEO_STREAM_DESC : public STREAM_DESC
{
public:
    VIDEO_STREAM_DESC(UInt32 w=0,
                      UInt32 h=0,
                      UInt32 a_x=0,
                      UInt32 a_y=0,
                      UInt64 tpf=0,
                      TD_TYPE t=TD_TYPE_NONE, 
                      TD_SUBTYPE st=TD_SUBTYPE_NONE)
                  
                 : STREAM_DESC(t,st)
                 , width(w)
                 , height(h)
                 , ax(a_x)
                 , ay(a_y)
                 , timePerFrame(tpf)
                 {}

    virtual ~VIDEO_STREAM_DESC() {}
    
    UInt32  width;
    UInt32  height;
    UInt32  ax;
    UInt32  ay;
    UInt64  timePerFrame;
};

class AUDIO_STREAM_DESC : public STREAM_DESC
{
public:
    AUDIO_STREAM_DESC(UInt32 chan=0,
                      UInt32 samp_per_sec=0,
                      UInt32 bits_per_sample=0,
                      TD_TYPE t=TD_TYPE_NONE, 
                      TD_SUBTYPE st=TD_SUBTYPE_NONE,
                      UInt32 chan_config=0)
                  
                 : STREAM_DESC(t,st)
                 , channels(chan)
                 , frequency(samp_per_sec)
                 , bitsPerSample(bits_per_sample)
                 , chanConfig(chan_config)
                 {}

    virtual ~AUDIO_STREAM_DESC() {}
    
    UInt32  channels;
    UInt32  frequency;
    UInt32  bitsPerSample;
    UInt32  chanConfig;
};

class TYPE_DESC : public STREAM_DESC
{
public:
    TYPE_DESC() : STREAM_DESC(TD_TYPE_NONE, TD_SUBTYPE_NONE) {}
    virtual ~TYPE_DESC() {}
    
    UInt16                  packetId;
    UInt16                  ssid;
};

#endif //_TYPEDESC_H_
