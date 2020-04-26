//-----------------------------------------------------------------------------
// BDROM_NavigatorErrors.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __BDROM_NAVIGATORERRORS_H__
#define __BDROM_NAVIGATORERRORS_H__

/// @file BDROM_NavigatorErrors.h
/// @brief Core BDROM error codes

// Error Base.
//
//    This value is the used to create errors in the range of 0x800040200 to 0x8000402FF
//    to make it unique in the BDROM Player, (and on windows, keep it within the range for FACILITY_ITF errors.)
//
const signed long kBDROMNavStreamErrorBase = 0xA0040500;

/// @name Core BDROM Error Codes
/// @brief Error codes used by the Core BDROM API
//@{

/// @brief List of error codes
///
enum
{
	/// @name Success codes
	//@{
	kBDROMNavStreamErrorNoErr					= 0,								///< Success
	//@}

	/// @name Error codes that are not really errors
	//@{
	kBDROMNavStreamErrorNoErrNewLocation		= (kBDROMNavStreamErrorBase - 1),	///< Not an error. This is used by the read() function to indicate that a new location/segment has started
	kBDROMNavStreamErrorNoErrClipTypeChanged	= (kBDROMNavStreamErrorBase - 2),	///< Not an error. This is used by the *_read() functions to indicate that a new location/segment is of a different type
	kBDROMNavStreamErrorStreamDone				= (kBDROMNavStreamErrorBase - 3),	///< End of stream.
	kBDROMNavStreamErrorNoErrReadWait			= (kBDROMNavStreamErrorBase - 4),	///< Stream data reading not allowed temporarily.

	// success codes returned by navigation command processor (HDMV Module)
	kBDROMNavStreamErrorNoErrJumpObject			= (kBDROMNavStreamErrorBase - 5),	///< Jump to new movie object
	kBDROMNavStreamErrorNoErrJumpTitle			= (kBDROMNavStreamErrorBase - 6),	///< Jump to new Title
	kBDROMNavStreamErrorNoErrCallObject			= (kBDROMNavStreamErrorBase - 7),	///< call new movie object
	kBDROMNavStreamErrorNoErrCallTitle			= (kBDROMNavStreamErrorBase - 8),	///< call new Title
	kBDROMNavStreamErrorNoErrResume				= (kBDROMNavStreamErrorBase - 9),	///< resume
	kBDROMNavStreamErrorNoErrBreak				= (kBDROMNavStreamErrorBase - 10),	///< break execution of commands
	kBDROMNavStreamErrorNoErrStopped			= (kBDROMNavStreamErrorBase - 11),	///< HDMV/BDJ module was stopped
	kBDROMNavStreamErrorNoErrSuspend			= (kBDROMNavStreamErrorBase - 12),	///< HDMV/BDJ module was suspend
	kBDROMNavStreamErrorNoErrNavRunning			= (kBDROMNavStreamErrorBase - 13),	///< HDMV/BDJ module was suspend
	//@}

	/// @name Error codes
	//@{
	kBDROMNavStreamErrorReadErr					= (kBDROMNavStreamErrorBase - 32),	///< Navigation or streamd data read error.
	kBDROMNavStreamErrorBufferTooSmall			= (kBDROMNavStreamErrorBase - 33),	///< Caller provided buffer is invalid or too small.
	kBDROMNavStreamErrorWrongDomain				= (kBDROMNavStreamErrorBase - 34),	///< Wrong domain.
	kBDROMNavStreamErrorFastRevereseDone		= (kBDROMNavStreamErrorBase - 35),	///< Not an error???. 
	kBDROMNavStreamErrorInvalidParams			= (kBDROMNavStreamErrorBase - 36),	///< One or more parameters are invalid.
	kBDROMNavStreamErrorNoCurrentProgram		= (kBDROMNavStreamErrorBase - 37),	///< ???
	kBDROMNavStreamErrorNoCurrentStream			= (kBDROMNavStreamErrorBase - 38),	///< Stream Selection functions return these to indicate no stream was selected
	kBDROMNavStreamErrorInvalidPIN				= (kBDROMNavStreamErrorBase - 39),	///< The provided PIN is incorrect.
	kBDROMNavStreamErrorNotAvailable			= (kBDROMNavStreamErrorBase - 40),	///< The requested data/information/operation is not available.
	kBDROMNavStreamErrorProhibited				= (kBDROMNavStreamErrorBase - 41),	///< The requested operation is prohibited.
	kBDROMNavStreamErrorProtected				= (kBDROMNavStreamErrorBase - 42),	///< Directory or PlayList is protected.
	kBDROMNavStreamErrorNotSupported			= (kBDROMNavStreamErrorBase - 43),	///< The requested operation/feature is not supported.
	kBDROMNavStreamErrorReadOnly				= (kBDROMNavStreamErrorBase - 44),	///< Playlist is read-only.
	kBDROMNavStreamErrorForbiddenUOP			= (kBDROMNavStreamErrorBase - 45),	///< User Operation Forbidden
	kBDROMNavStreamErrorNoCurrBDAVDir			= (kBDROMNavStreamErrorBase - 46),	///< No Active/Current BDAV Directory Set
	kBDROMNavStreamErrorInvalidBDAVDisc			= (kBDROMNavStreamErrorBase - 47),	///< The specified disc is not a valid BDAV disc
	kBDROMNavStreamErrorOutOfMemory				= (kBDROMNavStreamErrorBase - 48),	///< Not enough memory
	kBDROMNavStreamErrorInvalidState			= (kBDROMNavStreamErrorBase - 49),	///< Invalid thread/object state
	kBDROMNavStreamErrorJavaException			= (kBDROMNavStreamErrorBase - 50),	///< Invalid thread/object state
	kBDROMNavStreamErrorUnhandledEvent			= (kBDROMNavStreamErrorBase - 51),	///< Event not handled
	kBDROMNavStreamErrorNotUserOperation		= (kBDROMNavStreamErrorBase - 52),	///< Not a User Operation
	kBDROMNavStreamErrorNotKeyEvent				= (kBDROMNavStreamErrorBase - 53),	///< Not a Key Event
	kBDROMNavStreamErrorWrongDisc				= (kBDROMNavStreamErrorBase - 54),	///< In a disc-unbound BDJ situation, a non-BDJ BD was inserted.
	kBDROMNavStreamErrorDataStarved				= (kBDROMNavStreamErrorBase - 55),	///< Data starvation error
	
	kBDROMNavStreamErrorDeviceRemoved			= (kBDROMNavStreamErrorBase - 56),	///< 
	kBDROMNavStreamErrorIOError					= (kBDROMNavStreamErrorBase - 57),	///< 
	//@}
};
//@}

#endif
