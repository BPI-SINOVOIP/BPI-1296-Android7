//-----------------------------------------------------------------------------
// HDMVDecoder_EB.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_EB_H__
#define __HDMVDECODER_EB_H__

#include "HDMVDecoder_Types_Internal.h"
#include "SonicCriticalSection.h"

using namespace BDROM_HDMVDecoder;

//__________________________________________________________________________________________________________________________________________________
// HDMVDec_EBSegment
// Stores info about a Full or Partial Segment in EB.
//
struct HDMVDec_EBSegment
{
public:
	HDMVDec_EBSegment();
	~HDMVDec_EBSegment();

	void Reset(void);

	void AddMetadata(HDMVPrivateMetadataList & metadataList);

public:
	//	DisplaySet
	//	SegmentState
	HDMVSegmentType	SegmentType;

	UInt8 *			pSegmentStart;		// starting location of the segment
	UInt32			uiSegmentLen;		// length of the segment (this is the amount of data that has been received).

	UInt64			mDTS;				// 90KHz DTS extracted from PES packet header
	UInt64			mPTS;				// 90KHz PTS extracted from PES packet header
	UInt64			mDuration90k;		// Display Duration (in 90KHz) of this segment. Set to 0, if unknown.

	// PTS, DTS in the current Playback chain's (in HDR) timebase (aka Platform Presentation Time). E.g., DShow time
	SInt64			mPlatformDTS;		// mDTS converted to Platform Presentation Time
	SInt64			mPlatformPTS;		// mPTS extracted to Platform Presentation Time

	bool			bSequence;			// true if this segment is a sequence of fragments (i.e, multiple PES). false when segment fits in 1 PES.
	bool			bSequenceStartFlag;	// true if this sequence start flag has been encountered
	bool			bSequenceEndFlag;	// true if this sequence end flag has been encountered

	bool			bLinear;			// true if all the data for this segment is in one contigious memory area

	UInt32			uiCounter;			// unique counter to ID a segment in EB

	// metadata
	bool					mbHasMetadata;		// true if there is metadata associated with this segment
	HDMVPrivateMetadataList	mmetadataList;		// list of metadata
};

bool operator==(const HDMVDec_EBSegment & left, const HDMVDec_EBSegment & right);



//__________________________________________________________________________________________________________________________________________________
// HDMVDec_EBSegmentArray
// Stores array of HDMVDec_EBSegment structures
//
class HDMVDec_EBSegmentArray
{
public:
	HDMVDec_EBSegmentArray();
	~HDMVDec_EBSegmentArray();

	void Reset(void);

	void AddSegment(HDMVDec_EBSegment & ebSeg);
	HDMVDec_EBSegment & GetSegment(UInt32 uiIndex);
	void RemoveSegment(void);
	bool IsEmpty(void)									{ return m_Array.empty(); }

private:
	std::vector<HDMVDec_EBSegment>			m_Array;				// array of all Segments available in the EB
	UInt32									m_Counter;
};


//__________________________________________________________________________________________________________________________________________________
// HDMVDec_EBPtr
// Acts as a pointer into the data of a Segment. It can be used only to read data from a Segment.
//
class HDMVDec_EBPtr
{
public:
	HDMVDec_EBPtr();
	HDMVDec_EBPtr(UInt8 * pBufStart, UInt8 * pBufEnd, UInt8 * pRangeStart, UInt32 uiRangeSize);
	~HDMVDec_EBPtr()	{ }

	void Init(UInt8 * pBufStart, UInt8 * pBufEnd, UInt8 * pRangeStart, UInt32 uiRangeSize);

	UInt8& operator[](int nIndex);
	HDMVDec_EBPtr& operator+(int nNum);
	HDMVDec_EBPtr& operator-(int nNum);
	HDMVDec_EBPtr& operator+=(int nNum);
	HDMVDec_EBPtr& operator-=(int nNum);
	HDMVDec_EBPtr& operator++();
	HDMVDec_EBPtr operator++(int);
	HDMVDec_EBPtr& operator--();
	HDMVDec_EBPtr operator--(int);

	UInt8* GetBufferStart(void)				{ return m_pBufStart; }
	UInt8* GetBufferEnd(void)				{ return m_pBufEnd; }
	UInt8* GetRangeStart(void)				{ return m_pRangeStart; }
	UInt8* GetRangeEnd(void)				{ return m_pRangeEnd; }
	UInt8* GetCurrentLocation(void);

private:
//	HDMVDec_EBPtr(const HDMVDec_EB &);
//	HDMVDec_EBPtr& operator=(const HDMVDec_EBPtr &);

private:
	UInt8 *		m_pBufStart;		// start address of the Buffer
	UInt8 *		m_pBufEnd;			// end address of the Buffer
	UInt8 *		m_pRangeStart;		// start address of the allowed range for this EBPtr
	UInt8 *		m_pRangeEnd;		// end address of the allowed range for this EBPtr

	UInt32		m_uiCurrOffset;		// Additional offset within the range.

	UInt32		m_uiMaxIndex;		// max index value for the logical buffer size
	UInt32		m_uiWrapIndex;		// any index beyond this is a wrap around
};


//__________________________________________________________________________________________________________________________________________________
// HDMVDec_EB
// This is the EB, that has the allocated buffer memory and has a HDMVDec_EBSegmentArray to store information about all Segments in the EB
//
class HDMVDec_EB
{
public:
	HDMVDec_EB();
	~HDMVDec_EB();

	void Init(const UInt32 uiEBSize);
	void Reset(void);

	// write functions
	SonicError BeginSegmentWrite(HDMVDec_EBSegment & ebSeg);
	SonicError WriteSegmentData(UInt8 * pSource, UInt32 uiSourceLen, HDMVDec_EBSegment & ebSeg);
	SonicError CompleteSegmentWrite(HDMVDec_EBSegment & ebSeg);

	// read functions
	SonicError GetSegment(UInt32 uiIndex, HDMVDec_EBSegment & ebSeg);
	SonicError GetEBPtr(HDMVDec_EBSegment & ebSeg, HDMVDec_EBPtr & ebPtr);
	SonicError RemoveSegment(HDMVDec_EBSegment & ebSeg);

	// metadata functions.
	SonicError AddMetadata(HDMVPrivateMetadata & metadata);		// all metadata passed into EB will get added to next Segment written into buffer.

	// to be removed
//	SonicError AddNewSegment(UInt8 * pSource, UInt32 uiSourceLen, HDMVDec_EBSegment & ebSeg);
//	SonicError AppendToSegment(UInt8 * pSource, UInt32 uiSourceLen, HDMVDec_EBSegment & ebSeg);

//	bool HasFreeSpace(UInt32 uiDataSize);
//	UInt32 GetFreeSpace(void);

private:
	HDMVDec_EB(const HDMVDec_EB &);
	HDMVDec_EB& operator=(const HDMVDec_EB &);

private:
	SonicError Write(UInt8 * pSource, UInt32 uiSourceLen);		// copies data and advanced write pointer

private:
	SonicCriticalSection		m_csLock;
	UInt8 *						m_pBuffer;			// The EB
	UInt32						m_uiBufferSize;		// EB size
	UInt8 *						m_pBufferWrite;		// EB Write Pointer
	UInt8 *						m_pBufferRead;		// EB Read Pointer
	UInt8 *						m_pBufferTop;		// marks the top of the usable buffer.
	UInt32						m_uiFreeSpace;		// free space in the buffer
//	UInt32						m_uiFreeSpace_1;	// free space between write ptr and read ptr / buffer end
//	UInt32						m_uiFreeSpace_2;	// free space between buffer start and read ptr (when total free space is wraps around buffer end)

	HDMVDec_EBSegmentArray		m_SegmentArray;		// array of all complete Segments available in the EB

	HDMVPrivateMetadataList		m_MetadataList;	// all metadata received before first Segment or between 2 Segments
};


#endif // __HDMVDECODER_EB_H__
