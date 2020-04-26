//-----------------------------------------------------------------------------
// SysSurface.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

// Include SysSurfaceForward.h in *.h files wherever possible
// in order to minimize Compiler dependencies. Include SysSurface.h within
// matching *.cpp files.
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef __SysSurface_h__
#define __SysSurface_h__

#if defined (USING_SONIC_GD)
	#include "GdSurface.h"
#elif defined (_WIN32)
	#include "WinSurface.h"
#elif defined (_DIRECTFB)
	#include "DirectFBSurface.h"
#endif


namespace SonicSurface 
{

	#if defined (USING_SONIC_GD)

		typedef GdSurface		SysSurface;

	#elif defined (_WIN32)

		typedef WinSurface		SysSurface;

	#elif defined (_DIRECTFB)

		typedef DirectFBSurface		SysSurface;

	#endif

	class SysSurfacePixelLock {
	public:
		explicit SysSurfacePixelLock(SysSurface const *pSurface)
			: mpSurface(pSurface)
			{ pSurface->LockPixels(); }
		~SysSurfacePixelLock()
		{ try { mpSurface->UnlockPixels(); } catch (std::exception&) {} }
	private:
		//	No copying or assignment.
		SysSurfacePixelLock(SysSurfacePixelLock const &rhs);
		SysSurfacePixelLock& operator=(SysSurfacePixelLock const &rhs);
	private:
		SysSurface const *mpSurface;
	};
}

#endif // __SysSurface_h__
