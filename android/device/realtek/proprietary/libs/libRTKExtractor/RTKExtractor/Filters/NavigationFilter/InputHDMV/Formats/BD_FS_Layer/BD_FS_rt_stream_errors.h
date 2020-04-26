//-----------------------------------------------------------------------------
// BD_FS_rt_stream_errors.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
/*
	File:		BD_FS_rt_stream_errors.h

	Contains:	BD_FS_rt_stream errors
*/
#ifndef __BD_FS_RT_STREAM_ERRORS_H__
#define __BD_FS_RT_STREAM_ERRORS_H__

/// @file BD_FS_rt_stream_errors.h
/// @brief BD_FS_rt_stream error codes

// Error Base.
//
#ifdef WIN32
// Windows:
//    This value is the used to create errors in the range of 0x80040000 to 0x800401FF to
//    match the range for FACILITY_ITF errors.
//
const SInt32 kBDFSRTStreamErrorBase = 0xA0040100;
#else
// All other platforms:
//    This value is used to create errors in the < -50000 range
//
const SInt32 kBDFSRTStreamErrorBase = -50000;
#endif


/// @name BD_FS_rt_stream API Error Codes
/// @brief Error codes used by the BD_FS_rt_stream API
//@{

/// @brief List of error codes
///
enum
{
	/// @name Success codes
	//@{
	kBDFSRTStreamErrorNoErr					= 0,								///< Success
	//@}

	/// @name Error codes
	//@{
	kBDFSRTStreamErrorReadErr				= (kBDFSRTStreamErrorBase - 31),	///< Stream data read error.
	kBDFSRTStreamErrorInvalidParams			= (kBDFSRTStreamErrorBase - 32),	///< One or more parameters are invalid.
	kBDFSRTStreamErrorNoCurrentStream		= (kBDFSRTStreamErrorBase - 33),	///< There's no current stream assigned.
	kBDFSRTStreamErrorReadOnly				= (kBDFSRTStreamErrorBase - 34),	///< Stream is read-only.
	kBDFSRTStreamErrorSeekErr				= (kBDFSRTStreamErrorBase - 35),	///< Stream data seek error.
	kBDFSRTStreamErrorWriteErr				= (kBDFSRTStreamErrorBase - 36),	///< Stream data write error.
	kBDFSRTStreamErrorNotAvailable			= (kBDFSRTStreamErrorBase - 37),	///< Operation/Property not available.
	
	//REALTEK_MODIFY start
	kBDFSRTStreamErrorDeviceRemoved			= (kBDFSRTStreamErrorBase - 38),	///device is removed
	kBDFSRTStreamErrorIOError				= (kBDFSRTStreamErrorBase - 39),	
	kBDFSRTStreamErrorSyncByteMiss			= (kBDFSRTStreamErrorBase - 40),	///offsync
	//REALTEK_MODIFY end
	//@}
};
//@}

#endif
