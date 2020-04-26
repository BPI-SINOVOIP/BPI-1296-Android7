//------------------------------------------------------------------------------
// HDRInterfaceIDs.h
// Copyright (c) 2007, Sonic Solutions.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef _HDRINTERFACEIDS_H_
#define _HDRINTERFACEIDS_H_

// HDR Interface ID:
// The IDs are a combination of a Platform ID (upper 2 bytes) OR-ed with an Interface Number (lower 2 bytes)
//
typedef UInt32			HDRInterfaceID;


//-----------------------------------------------------------------------
// Platforms IDs for interface IDs
//
#define HDR_PLID_ALL								0x00010000		// For Interfaces that all platforms are expected to support
#define HDR_PLID_WIN32								0x00020000		// For Interfaces that only Win32 platforms


//-----------------------------------------------------------------------
// Interface IDs for Interfaces that can be implemented on all platforms
//
const HDRInterfaceID	HDRIID_AVOutputPortCaps			= (HDR_PLID_ALL | 0x0001);		// Interface to get Audio/Video Output properties
const HDRInterfaceID	HDRIID_SurfaceOps				= (HDR_PLID_ALL | 0x0002);		// Interface to perform graphics operations on HDRSurface_BaseSurface objects


//-----------------------------------------------------------------------
// Interface IDs for Interfaces that are implemented only on Windows
//
#ifdef WIN32
const HDRInterfaceID	HDRIID_SecurityModuleControl	= (HDR_PLID_WIN32 | 0x0001);	// Interface to control Security Module and its Decrypters
#endif //WIN32

#endif //_HDRINTERFACEIDS_H_
