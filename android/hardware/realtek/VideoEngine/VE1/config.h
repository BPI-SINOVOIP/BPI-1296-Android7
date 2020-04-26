//------------------------------------------------------------------------------
// File: config.h
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
// This file should be modified by some developers of C&M according to product version.
//------------------------------------------------------------------------------
#ifndef __CONFIG_H__
#define __CONFIG_H__

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#	define PLATFORM_WIN32
#elif defined(linux) || defined(__linux) || defined(ANDROID)
#	define PLATFORM_LINUX
#elif defined(unix) || defined(__unix)
#   define PLATFORM_QNX
#else
#	define PLATFORM_NON_OS
#endif

#if defined(_MSC_VER)
#	include <windows.h>
#	define inline _inline
#elif defined(__GNUC__)
#elif defined(__ARMCC__)
#else
#  error "Unknown compiler."
#endif

#define API_VERSION_MAJOR       5
#define API_VERSION_MINOR       5
#define API_VERSION_PATCH       15
#define API_VERSION             ((API_VERSION_MAJOR<<16) | (API_VERSION_MINOR<<8) | API_VERSION_PATCH)

#if defined(PLATFORM_NON_OS) || defined (ANDROID) || defined(MFHMFT_EXPORTS) || defined(PLATFORM_QNX)
//#define SUPPORT_FFMPEG_DEMUX
#else
//#define SUPPORT_FFMPEG_DEMUX
#endif

//------------------------------------------------------------------------------
// COMMON
//------------------------------------------------------------------------------
#define SUPPORT_SECONDARY_AXI

#define SUPPORT_CONF_TEST

// do not define BIT_CODE_FILE_PATH in case of multiple product support. becuase wave410 and coda980 has different firmware binary format.
#define CORE_0_BIT_CODE_FILE_PATH   "cezzane.bin"                       // for wave420
#define CORE_1_BIT_CODE_FILE_PATH   "/system/vendor/modules/ve1.bin"    // for coda980
#define CORE_2_BIT_CODE_FILE_PATH   "pissarro.bin"                      // for wave412
#define CORE_3_BIT_CODE_FILE_PATH   "/system/vendor/modules/ve2.bin"    // for wave410
#define CORE_4_BIT_CODE_FILE_PATH   "coda960.out"                       // for coda960

//------------------------------------------------------------------------------
// OMX
//------------------------------------------------------------------------------
#define API_VERSION_BASE_50
#define SUPPORT_MULTI_PRODUCT


//------------------------------------------------------------------------------
// CODA980
//------------------------------------------------------------------------------
//#define ROI_50
#define STRICT_CBR
//#define SUPPORT_CDB
#define SUPPORT_NV21
#define SUPPORT_ENCODER
#define CODA980_ENCODER
//#define SUPPORT_ROLLBACK
#define MAVERICK_CACHE_II
//#define SUPPORT_DEC_REPORT
//#define SUPPORT_ENC_REPORT
//#define SUPPORT_PROFILE_SET
#define SUPPORT_AVC_FRAME_RC
#define SUPPORT_NEW_SLICE_MODE
#define SUPPORT_ENC_SUB_FRAME_SYNC
#define SUPPORT_AVC_FRAME_RC_QCOMPRESS
//#define SUPPORT_AVC_ERROR_CONCEAL_MODE

#define SUPPORT_MVC_ENC

#define NEW_INTRA_REFRESH
#define RC_CHANGE_PARAMETER_DEF



//------------------------------------------------------------------------------
// WAVE4102
//------------------------------------------------------------------------------
#define PIXEL_ORDER
#define SUPPORT_FIO_ACCESS
//#define SUPPORT_MULTI_VCORE
//#define SUPPORT_MINI_PIPPEN
//#define SUPPORT_COMMAND_QUEUE
//#define SUPPORT_PACKED_STREAM_FORMAT



#endif /* __CONFIG_H__ */
