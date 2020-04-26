//-----------------------------------------------------------------------------
// SysHDSurface.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

// Include SysHDSurfaceForward.h in *.h files wherever possible
// in order to minimize Compiler dependencies. Include SysHDSurface.h within
// matching *.cpp files.
#ifndef __SysHDSurface_h__
#define __SysHDSurface_h__

#if defined (USING_SONIC_GD)
	#include "GdSurface.h"			
#elif defined (_WIN32)
	#include "D3DSurface.h"
    #include "D3DRealSurface.h"     // mpc
#elif defined (_DIRECTFB)
	#include "DirectFBSurface.h"
#endif


namespace SonicSurface 
{

	#if defined (USING_SONIC_GD)

		typedef GdSurface		SysHDSurface;

	#elif defined (ST_DEMO)

		typedef ST_DirectFBSurface	SysHDSurface;

	#elif defined (_WIN32)

		typedef D3DSurface		SysHDSurface;
		typedef D3DRealSurface	SysHDRealSurface;

	#elif defined (_DIRECTFB)

		typedef DirectFBSurface		SysHDSurface;

	#endif

	class SysHDSurfacePixelLock {
	public:
		explicit SysHDSurfacePixelLock(SysHDSurface const *pSurface)
			: mpSurface(pSurface)
			{ pSurface->LockPixels(); }
		~SysHDSurfacePixelLock()
		{ try { mpSurface->UnlockPixels(); } catch (std::exception&) {} }
	private:
		//	No copying or assignment.
		SysHDSurfacePixelLock(SysHDSurfacePixelLock const &rhs);
		SysHDSurfacePixelLock& operator=(SysHDSurfacePixelLock const &rhs);
	private:
		SysHDSurface const *mpSurface;
	};
}

#endif // __SysHDSurface_h__
