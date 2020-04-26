//-----------------------------------------------------------------------------
// SysTextForward.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

// Include this instead of SysText.h in *.h files wherever possible
// in order to minimize Compiler dependencies. Include SysText.h within
// matching *.cpp files.
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef __SysTextForward_h__
#define __SysTextForward_h__

#include "DirectFBText.h"	//REALTEK_MODIFY

namespace SonicSurface 
{
	class PiFont;

	class BaseTextData;
	class BaseTextRenderer;

	#if defined (USING_SONIC_GD)
		class GdTextData;
		typedef GdTextData		SysTextData;

		class GdTextRenderer;
		typedef GdTextRenderer	SysTextRenderer;

	#elif defined ST_DEMO
		class STTextData;
		typedef STTextData			SysTextData;

		class STTextRenderer;
		typedef STTextRenderer		SysTextRenderer;

	#elif defined (_WIN32)
		class WinTextData;
		typedef WinTextData			SysTextData;

		class WinTextRenderer;
		typedef WinTextRenderer		SysTextRenderer;

		
	#elif defined (_DIRECTFB)
		class DirectFBTextData;
		typedef DirectFBTextData	SysTextData;

	#endif

}

#endif // __SysTextForward_h__

