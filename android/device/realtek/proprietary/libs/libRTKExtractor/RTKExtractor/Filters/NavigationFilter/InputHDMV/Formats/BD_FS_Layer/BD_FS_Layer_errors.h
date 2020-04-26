//-----------------------------------------------------------------------------
// BD_FS_Layer_errors.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
/*
	File:		BD_FS_Layer_errors.h

	Contains:	BD_FS_Layer errors
*/
#ifndef __BD_FS_LAYER_ERRORS_H__
#define __BD_FS_LAYER_ERRORS_H__

/// @file BD_FS_rt_stream_errors.h
/// @brief BD_FS_rt_stream error codes

// Error Base.
//
//    This value is the used to create errors in the range of 0x800040100 to 0x8000401FF
//    to make it unique in the BDROM Player, (and on windows, keep it within the range for FACILITY_ITF errors.)
//
const SInt32 kBDFSLayerErrorBase = 0xA0040200;

/// @name BD_FS_rt_stream API Error Codes
/// @brief Error codes used by the BD_FS_rt_stream API
//@{

/// @brief List of error codes
///
enum
{
	/// @name Success codes
	//@{
	kBDFSLayerErrorNoErr				= 0,							///< Success
	kBDFSLayerErrorNoErrDeviceExist		= (kBDFSLayerErrorBase - 1),	///< Device exist. Not an error.
	kBDFSLayerErrorNoErrDeviceNotExist	= (kBDFSLayerErrorBase - 2),	///< Device Not Exist. Not an error.
	kBDFSLayerErrorNoErrVolumeExist		= (kBDFSLayerErrorBase - 3),	///< Volume Exist. Not an error.
	kBDFSLayerErrorNoErrVolumeNotExist	= (kBDFSLayerErrorBase - 4),	///< Volume Not Exist. Not an error.
	//@}

	/// @name Error codes
	//@{
	kBDFSLayerErrorInvalidParams		= (kBDFSLayerErrorBase - 31),	///< One or more parameters are invalid.
	kBDFSLayerErrorDeviceCount			= (kBDFSLayerErrorBase - 32),	///< Device count error.
	kBDFSLayerErrorNoDevice				= (kBDFSLayerErrorBase - 33),	///< No device.
	kBDFSLayerErrorNoVolume				= (kBDFSLayerErrorBase - 34),	///< No volume.
	kBDFSLayerErrorDeviceClose			= (kBDFSLayerErrorBase - 35),	///< Device close error.
	kBDFSLayerErrorVolumeClose			= (kBDFSLayerErrorBase - 36),	///< Volume close error.
	kBDFSLayerErrorVolumeMount			= (kBDFSLayerErrorBase - 37),	///< Volume mount error.
	kBDFSLayerErrorVolumeOpen			= (kBDFSLayerErrorBase - 38),	///< Volume open error.
	kBDFSLayerErrorDeviceIndex			= (kBDFSLayerErrorBase - 39),	///< Getting device index has failed.
	kBDFSLayerErrorStreamCreation		= (kBDFSLayerErrorBase - 40),	///< Stream Creation Error.
	kBDFSLayerErrorDeviceOpen			= (kBDFSLayerErrorBase - 41),	///< Device open error.
	kBDFSLayerErrorCreateDirectory		= (kBDFSLayerErrorBase - 42),	///< Create Directory error
	kBDFSLayerErrorFileNotFound			= (kBDFSLayerErrorBase - 64),	///< File could not be found
	kBDFSLayerErrorNotSupported			= (kBDFSLayerErrorBase - 65),	///< Requested operation is not supported
	kBDFSLayerErrorDeviceError			= (kBDFSLayerErrorBase - 66),	///< Device Error. Check extended error information, if available, for more details.
	kBDFSLayerErrorDeviceInUse			= (kBDFSLayerErrorBase - 67),	///< Cannot get exclusive access to Device.
	kBDFSLayerErrorFileDisabled			= (kBDFSLayerErrorBase - 68),	///< For VFS Only -- file is disabled VFS file
	//@}
};
//@}

#endif
