//-----------------------------------------------------------------------------
// SysImageSprite.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

// Include SysImageSpriteForward.h in *.h files wherever possible
// in order to minimize Compiler dependencies. Include SysImageSprite.h within
// matching *.cpp files.
#ifndef __SysImageSprite_h__
#define __SysImageSprite_h__

#if defined (USING_SONIC_GD)
	#include "BaseImageSprite.h"
#elif defined ST_DEMO 
	#include "STImageSprite.h"
#elif defined (_WIN32)
	#include "WinImageSprite.h"
#elif defined (_DIRECTFB)
	#include "DFBImageSprite.h"
#endif


#if defined (USING_SONIC_GD)
	typedef BaseImageSprite	SysImageSprite;
#elif defined ST_DEMO 
	typedef STImageSprite	SysImageSprite;
#elif defined _WIN32 
	typedef WinImageSprite	SysImageSprite;
#elif defined (_DIRECTFB)
	typedef DFBImageSprite	SysImageSprite;
#endif

#endif // __SysImageSprite_h__
