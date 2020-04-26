// $Id: PortableDivxTypes.h 28 2005-07-01 18:31:30Z bhumphrey $
// Copyright (c) 2001 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.

#ifndef PORTABLEDIVXTYPES_H
#define PORTABLEDIVXTYPES_H

#ifdef WIN32

	typedef unsigned __int64 uint64_t;
//	typedef unsigned __int32 uint32_t;  // some std templates don't understand __int32 ( stringstream)
	typedef unsigned int uint32_t;
	typedef unsigned __int16 uint16_t;
	typedef unsigned char uint8_t;
	typedef __int64 int64_t;
	typedef __int32 int32_t;
	typedef __int16 int16_t;
	typedef __int8  int8_t;

#endif

#ifdef LINUX

	typedef unsigned long long uint64_t;
	typedef unsigned int       uint32_t;
	typedef unsigned short     uint16_t;
	typedef unsigned char      uint8_t;
	
    typedef long long int64_t;
	typedef int       int32_t;
	typedef short     int16_t;

	typedef void	*LPVOID;

#endif

#ifdef MACOSX

	typedef unsigned long long uint64_t;
	typedef unsigned long      uint32_t;
	typedef unsigned short     uint16_t;
	typedef unsigned char      uint8_t;
	
    typedef long long int64_t;
	typedef int       int32_t;
	typedef short     int16_t;

	typedef void	*LPVOID;

#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#endif //PORTABLEDIVXTYPES_H
