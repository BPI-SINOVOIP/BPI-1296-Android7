//-----------------------------------------------------------------------------
// TextDefs.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef _TextDefs_H
#define _TextDefs_H

namespace SonicSurface
{
//_____________________________________________________________________________
//
// Enumerations
//
//_____________________________________________________________________________
enum eTextAlignment {
	eTextAlignLeft			= 0,
	eTextAlignCenter		= 1,
	eTextAlignRight			= 2,
	eTextAlignBoth			= 3,
	eTextAlignNoWrap		= 4,
	eTextAlignLeftNoWrap	= 4,
	eTextAlignCenterNoWrap	= 5,
	eTextAlignRightNoWrap	= 6,
	eTextAlignBothNoWrap	= 7,
};

typedef enum {
    eTextVerticalAlignTop			= 0,
    eTextVerticalAlignCenter		= 1,
    eTextVerticalAlignBottom			= 2,
} eTextVerticalAlignment;

typedef enum {
	eTextDirectionLeftToRight	= 0,
	eTextDirectionRightToLeft	= 1,
} eTextDirection;

enum eTextStyle {
	eStyleRegular			= 0x00000000,
	eStyleBold				= 0x00000001,
	eStyleItalic			= 0x00000002,
	eStyleUnderline			= 0x00000004,
	eStyleStrikeout			= 0x00000008,
	eStyleAllCaps			= 0x00000010,
	eStyleOblique			= 0x00000020,		// similar to italic
	eStyleBackslant			= 0x00000040,		// opposite of italic
	eStyleReverseOblique	= 0x00000080,		// opposite of oblique
	// combination styles
	eStyleBoldItalic	= eStyleBold | eStyleItalic
// **CodeWizzard** - Possible Violation:  User Defined item 740  - When using enum, the values of each member should be explicitly declared
};

}
#endif
