// $Id: PortableDivx.h 28 2005-07-01 18:31:30Z bhumphrey $
// Copyright (c) 2001 DivXNetworks, Inc. http://www.divxnetworks.com
// All rights reserved.
//
// This software is the confidential and proprietary information of DivxNetworks
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivXNetworks, Inc.


#ifndef PORTABLEDIVX_H
#define PORTABLEDIVX_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable:4786)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

#include "PortableDivxTypes.h"

#ifdef WIN32

//
// This define is required to include
// wincrypt.h and access the CryptoAPI 
//

#define _WIN32_WINNT 0x0400 

	#include <windows.h>
	#include <wincrypt.h>

#endif


#ifdef LINUX
#define _REENTRANT
#endif


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <math.h>
#include <string>
#include <sstream>
#include <stack>
#include <list>
#include <vector>
#include <iterator>
#include <deque>
#include <map>
#include <time.h>
#include <iomanip>
#include <queue>

#include <sys/stat.h>

//#include <strstream>

#ifdef WIN32

	#include <direct.h>
	#include <windef.h>

	#include <mmsystem.h>
	#include <mmreg.h>
	#include <msacm.h>

	#include <memory.h>
	#include <wininet.h>
	#include <winbase.h>
	#include <winsock.h>

	#include <commctrl.h> 
	#include <shellapi.h>

#endif

#ifdef LINUX

	#include <stdint.h>

	#include <pthread.h>
	#include <unistd.h>

	#include <sys/timeb.h>
	#include <sys/shm.h>

	#include "../../media/format/DivxBitmap.h"
	#include "../../media/format/MP3REG.H"

	#include <sys/types.h>
	#include <dirent.h>

#endif

#ifdef MACOSX

	#include <stdint.h>

	#include <pthread.h>
	#include <unistd.h>

	#include <sys/timeb.h>
	#include <sys/shm.h>

	#include "../../media/format/DivxBitmap.h"
	#include "../../media/format/MP3REG.H"

	#include <sys/types.h>
	#include <dirent.h>
	
	#include <wctype.h>
	
	#include <CoreFoundation/CFString.h>
	#include <CoreServices/CarbonCore/Files.h>
	#include <LaunchServices.h>
	
	#include <extras_wchar.h>
	
	inline std::wstring CFStringTowstring(CFStringRef stringRef)
	{
		int utf16StringLength = CFStringGetMaximumSizeForEncoding(CFStringGetLength(stringRef), kCFStringEncodingUnicode);
		unsigned short *utf16StringBuffer = new unsigned short[utf16StringLength + 1];
		CFStringGetCString(stringRef, (char *)utf16StringBuffer, utf16StringLength, kCFStringEncodingUnicode);
		utf16StringBuffer[utf16StringLength] = 0;
		
		std::wstring string = (wchar_t *)utf16StringBuffer;
		return string;
	}
	
	inline CFStringRef wstringToCFString(std::wstring string)
	{
		CFStringRef stringRef = CFStringCreateWithCharacters(NULL, (const unsigned short *)string.data(), string.length());
		return stringRef;
	}
#endif

using namespace std;

/** @interface */
class PortableDivx {
};

#endif //PORTABLEDIVX_H
