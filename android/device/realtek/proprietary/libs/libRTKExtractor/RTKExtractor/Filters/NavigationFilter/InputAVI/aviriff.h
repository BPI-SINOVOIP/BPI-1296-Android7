/*

   Header file for AVI Parser
   
   Start Date: 2005/08/24
   Maintainer: Chih-pin Wu <wucp@realtek.com.tw>

   Revision 0.1
   2005/08/24	: made necessary modifications for GNU environment, 
		  add necessary and remove unnecessary code for error-free compilation

*/
// for int$$_t... abstract datatypes
#include <stdint.h>
// for WAVEFORMATEX structure
#include "mmreg.h"
#include "bitmapinfo.h"
//==========================================================================;
//
//  Copyright (c) 1996 - 1999  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;

/*+
 *
 * Structures and defines for the RIFF AVI file format extended to
 * handle very large/long files
 *
 *-=====================================================================*/

#ifndef AVIRIFF_H
#define AVIRIFF_H

#ifndef NUMELMS
#define NUMELMS(aa) (sizeof(aa)/sizeof((aa)[0]))
#endif

// all structures in this file are packed on word boundaries
//

/*
 * heres the general layout of an AVI riff file (new format)
 *
 * RIFF (3F??????) AVI       <- not more than 1 GB in size
 *     LIST (size) hdrl
 *         avih (0038)
 *         LIST (size) strl
 *             strh (0038)
 *             strf (????)
 *             indx (3ff8)   <- size may vary, should be sector sized
 *         LIST (size) strl
 *             strh (0038)
 *             strf (????)
 *             indx (3ff8)   <- size may vary, should be sector sized
 *         LIST (size) odml
 *             dmlh (????)
 *         JUNK (size)       <- fill to align to sector - 12
 *     LIST (7f??????) movi  <- aligned on sector - 12
 *         00dc (size)       <- sector aligned
 *         01wb (size)       <- sector aligned
 *         ix00 (size)       <- sector aligned
 *     idx1 (00??????)       <- sector aligned
 * RIFF (7F??????) AVIX
 *     JUNK (size)           <- fill to align to sector -12
 *     LIST (size) movi
 *         00dc (size)       <- sector aligned
 * RIFF (7F??????) AVIX      <- not more than 2GB in size
 *     JUNK (size)           <- fill to align to sector - 12
 *     LIST (size) movi
 *         00dc (size)       <- sector aligned
 *
 *-===================================================================*/

//
// structures for manipulating RIFF headers
//
#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |     \
                  (((uint32_t)(ch4) & 0xFF00) << 8) |    \
                  (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
                  (((uint32_t)(ch4) & 0xFF000000) >> 24))

typedef struct _riffchunk {
   uint32_t fcc;
   uint32_t  cb;
   } RIFFCHUNK, * LPRIFFCHUNK;

typedef struct _rifflist {
   uint32_t fcc;
   uint32_t cb;
   uint32_t fccListType;
   } RIFFLIST, * LPRIFFLIST;

#define RIFFROUND(cb) ((cb) + ((cb)&1))
#define RIFFNEXT(pChunk) (LPRIFFCHUNK)((LPBYTE)(pChunk) \
                          + sizeof(RIFFCHUNK) \
                          + RIFFROUND(((LPRIFFCHUNK)pChunk)->cb))

//
// ==================== avi header structures ===========================
//

// main header for the avi file (compatibility header)
//
#define ckidMAINAVIHEADER FCC('avih')
typedef struct _avimainheader {
    uint32_t  fcc;                    // 'avih'
    uint32_t  cb;                     // size of this structure -8
    uint32_t  dwMicroSecPerFrame;     // frame display rate (or 0L)
    uint32_t  dwMaxBytesPerSec;       // max. transfer rate
    uint32_t  dwPaddingGranularity;   // pad to multiples of this size; normally 2K.
    uint32_t  dwFlags;                // the ever-present flags
    #define AVIF_HASINDEX        0x00000010 // Index at end of file?
    #define AVIF_MUSTUSEINDEX    0x00000020
    #define AVIF_ISINTERLEAVED   0x00000100
    #define AVIF_TRUSTCKTYPE     0x00000800 // Use CKType to find key frames
    #define AVIF_WASCAPTUREFILE  0x00010000
    #define AVIF_COPYRIGHTED     0x00020000
    uint32_t  dwTotalFrames;          // # frames in first movi list
    uint32_t  dwInitialFrames;
    uint32_t  dwStreams;
    uint32_t  dwSuggestedBufferSize;
    uint32_t  dwWidth;
    uint32_t  dwHeight;
    uint32_t  dwReserved[4];
    } AVIMAINHEADER;

#define ckidODML          FCC('odml')
#define ckidAVIEXTHEADER  FCC('dmlh')
typedef struct _aviextheader {
   uint32_t   fcc;                    // 'dmlh'
   uint32_t   cb;                     // size of this structure -8
   uint32_t   dwGrandFrames;          // total number of frames in the file
   uint32_t   dwFuture[61];           // to be defined later
   } AVIEXTHEADER;

//
// structure of an AVI stream header riff chunk
//
#define ckidSTREAMLIST   FCC('strl')

#ifndef ckidSTREAMHEADER
#define ckidSTREAMHEADER FCC('strh')
#endif
typedef struct _avistreamheader {
   uint32_t  fcc;          // 'strh'
   uint32_t  cb;           // size of this structure - 8

   uint32_t  fccType;      // stream type codes

   #ifndef streamtypeVIDEO
   #define streamtypeVIDEO FCC('vids')
   #define streamtypeAUDIO FCC('auds')
   #define streamtypeMIDI  FCC('mids')
   #define streamtypeTEXT  FCC('txts')
   #endif

   uint32_t  fccHandler;
   uint32_t  dwFlags;
   #define AVISF_DISABLED          0x00000001
   #define AVISF_VIDEO_PALCHANGES  0x00010000

   uint16_t  wPriority;
   uint16_t  wLanguage;
   uint32_t  dwInitialFrames;
   uint32_t  dwScale;
   uint32_t  dwRate;       // dwRate/dwScale is stream tick rate in ticks/sec
   uint32_t  dwStart;
   uint32_t  dwLength;
   uint32_t  dwSuggestedBufferSize;
   uint32_t  dwQuality;
   uint32_t  dwSampleSize;
   struct {
      short int left;
      short int top;
      short int right;
      short int bottom;
      }   rcFrame;
   } AVISTREAMHEADER;


//
// structure of an AVI stream format chunk
//
#ifndef ckidSTREAMFORMAT
#define ckidSTREAMFORMAT FCC('strf')
#endif
//
// avi stream formats are different for each stream type
//
// BITMAPINFOHEADER for video streams
// WAVEFORMATEX or PCMWAVEFORMAT for audio streams
// nothing for text streams
// nothing for midi streams


//
// structure of old style AVI index
//
#define ckidAVIOLDINDEX FCC('idx1')
#define ckidAVIDMFINDEX FCC('1idx')
typedef struct _avioldindex {
   uint32_t  fcc;        // 'idx1'
   uint32_t   cb;         // size of this structure -8
   struct _avioldindex_entry {
      uint32_t   dwChunkId;
      uint32_t   dwFlags;

      #ifndef AVIIF_LIST
      #define AVIIF_LIST       0x00000001
      #define AVIIF_KEYFRAME   0x00000010
      #endif
     
      #define AVIIF_NO_TIME    0x00000100
      #define AVIIF_COMPRESSOR 0x0FFF0000  // unused?
      uint32_t   dwOffset;    // offset of riff chunk header for the data
      uint32_t   dwSize;      // size of the data (excluding riff header size)
      } aIndex[];          // size of this array
   } AVIOLDINDEX;


//
// ============ structures for new style AVI indexes =================
//

// index type codes
//
#define AVI_INDEX_OF_INDEXES       0x00
#define AVI_INDEX_OF_CHUNKS        0x01
#define AVI_INDEX_OF_TIMED_CHUNKS  0x02
#define AVI_INDEX_OF_SUB_2FIELD    0x03
#define AVI_INDEX_IS_DATA          0x80

// index subtype codes
//
#define AVI_INDEX_SUB_DEFAULT     0x00

// INDEX_OF_CHUNKS subtype codes
//
#define AVI_INDEX_SUB_2FIELD      0x01

// meta structure of all avi indexes
//
typedef struct _avimetaindex {
   uint32_t  fcc;
   uint32_t  cb;
   uint16_t  wLongsPerEntry;
   uint8_t   bIndexSubType;
   uint8_t   bIndexType;
   uint32_t  nEntriesInUse;
   uint32_t  dwChunkId;
   uint32_t  dwReserved[3];
   uint32_t  adwIndex[];
   } AVIMETAINDEX;

#define STDINDEXSIZE 0x4000
#define NUMINDEX(wLongsPerEntry) ((STDINDEXSIZE-32)/4/(wLongsPerEntry))
#define NUMINDEXFILL(wLongsPerEntry) ((STDINDEXSIZE/4) - NUMINDEX(wLongsPerEntry))

// structure of a super index (INDEX_OF_INDEXES)
//
#define ckidAVISUPERINDEX FCC('indx')
typedef struct _avisuperindex {
   uint32_t    fcc;               // 'indx'
   uint32_t    cb;                // size of this structure
   uint16_t    wLongsPerEntry;    // ==4
   uint8_t     bIndexSubType;     // ==0 (frame index) or AVI_INDEX_SUB_2FIELD 
   uint8_t     bIndexType;        // ==AVI_INDEX_OF_INDEXES
   uint32_t    nEntriesInUse;     // offset of next unused entry in aIndex
   uint32_t    dwChunkId;         // chunk ID of chunks being indexed, (i.e. RGB8)
   uint32_t    dwReserved[3];     // must be 0
   struct _avisuperindex_entry {
      uint64_t    qwOffset;     // 64 bit offset to sub index chunk
      uint32_t    dwSize;       // 32 bit size of sub index chunk
      uint32_t    dwDuration;   // time span of subindex chunk (in stream ticks)
      } *aIndex;
//      } aIndex[NUMINDEX(4)];
   } AVISUPERINDEX;
#define Valid_SUPERINDEX(pi) (*(uint32_t *)(&((pi)->wLongsPerEntry)) == (4 | (AVI_INDEX_OF_INDEXES << 24)))

// struct of a standard index (AVI_INDEX_OF_CHUNKS)
//
typedef struct _avistdindex_entry {
   uint32_t dwOffset;       // 32 bit offset to data (points to data, not riff header)
   uint32_t dwSize;         // 31 bit size of data (does not include size of riff header), bit 31 is deltaframe bit
   } AVISTDINDEX_ENTRY;
#define AVISTDINDEX_DELTAFRAME ( 0x80000000) // Delta frames have the high bit set
#define AVISTDINDEX_SIZEMASK   (~0x80000000)

typedef struct _avistdindex {
   uint32_t   fcc;               // 'indx' or '##ix'
   uint32_t   cb;                // size of this structure
   uint16_t   wLongsPerEntry;    // ==2
   uint8_t    bIndexSubType;     // ==0
   uint8_t    bIndexType;        // ==AVI_INDEX_OF_CHUNKS
   uint32_t   nEntriesInUse;     // offset of next unused entry in aIndex
   uint32_t   dwChunkId;         // chunk ID of chunks being indexed, (i.e. RGB8)
 //  uint64_t   qwBaseOffset;     // base offset that all index intries are relative to
   uint32_t   qwBaseOffset1;
   uint32_t   qwBaseOffset2;
   uint32_t    dwReserved_3;      // must be 0
   AVISTDINDEX_ENTRY aIndex[NUMINDEX(2)];
   } AVISTDINDEX;

typedef struct _avifieldindex_chunk {
   uint32_t    fcc;               // 'ix##'
   uint32_t    cb;                // size of this structure
   uint16_t    wLongsPerEntry;    // must be 3 (size of each entry in
                                  // aIndex array)
   uint8_t     bIndexSubType;     // AVI_INDEX_2FIELD
   uint8_t     bIndexType;        // AVI_INDEX_OF_CHUNKS
   uint32_t    nEntriesInUse;     //
   uint32_t    dwChunkId;         // '##dc' or '##db'
   uint64_t    qwBaseOffset;      // offsets in aIndex array are relative to this
   uint32_t    dwReserved3;       // must be 0
   struct _avifieldindex_entry {
      uint32_t    dwOffset;
      uint32_t    dwSize;         // size of all fields
                               // (bit 31 set for NON-keyframes)
      uint32_t    dwOffsetField2; // offset to second field
   } aIndex[  ];
} AVIFIELDINDEX, *PAVIFIELDINDEX;


#endif
