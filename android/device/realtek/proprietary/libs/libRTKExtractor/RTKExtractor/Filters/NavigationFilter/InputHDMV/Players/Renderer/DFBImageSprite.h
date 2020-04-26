//-----------------------------------------------------------------------------
// WinImageSprite.h
//
// Image Sprite class derived from BaseImageSprite, but with additional windows specific data/functionality
//
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef DFB_IMAGE_SPRITE_H
#define DFB_IMAGE_SPRITE_H

#include "BaseImageSprite.h"
#include "ref_ptr.h"

class DFBImageSprite : public BaseImageSprite
{
public:
	
	// Constructor
	DFBImageSprite() {mReservedForVideo = false;}
	virtual ~DFBImageSprite() {}

	
protected:
	
    bool mReservedForVideo; // no more than one sprite should have this flag set
};

#endif	// DFB_IMAGE_SPRITE_H
