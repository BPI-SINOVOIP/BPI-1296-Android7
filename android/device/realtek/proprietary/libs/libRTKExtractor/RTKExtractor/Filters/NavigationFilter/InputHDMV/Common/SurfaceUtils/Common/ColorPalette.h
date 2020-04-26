//-----------------------------------------------------------------------------
// ColorPalette.h
// Copyright (c) 2003 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef __ColorPalette__
#define __ColorPalette__

#include <list>
#include <vector>

#include "CommonTypes.h"
#include "PiRGBColor.h"

namespace SonicSurface {

	class ColorUsageEntry
	{
	private:
		UInt8 red, green, blue, alpha;
		bool hasAlpha;
		UInt32 usageCount;

	public:
		ColorUsageEntry()
			: red(0), green(0), blue(0), alpha(0), hasAlpha(false), usageCount(0) { }

		ColorUsageEntry(UInt8 r, UInt8 g, UInt8 b, UInt8 a = 0, bool setAlpha = false)
			: red(r), green(g), blue(b), alpha(a), hasAlpha(setAlpha), usageCount(1) { }

		bool operator< (const ColorUsageEntry& rhs)
		{
			return (usageCount < rhs.usageCount);
		}
		UInt8 Red() const {return red;}
		UInt8 Green() const {return green;}
		UInt8 Blue() const {return blue;}
		UInt8 Alpha() const {return alpha;}
		bool  HasAlpha() const { return hasAlpha;}
		UInt32 UsageCount() const {return usageCount;}
		void IncUsageCount() {++usageCount;}
		void DecUsageCount() {--usageCount;}
	};

	//	A ColorUsageList is simply a list of ColorUsageEntry's.  It can be added to,
	//	sorted, etc. but does not have random access.
	typedef std::list<ColorUsageEntry> ColorUsageList;

	//	A ColorPalette is a vector of PiRGBColors.  It does not have usage information
	//	and is not sortable, but is random access.
	typedef std::vector<SonicSurface::PiRGBColor> ColorPalette;


	void	AddColorToPalette(ColorUsageEntry const &entry, ColorUsageList& palette);

	ColorUsageList::iterator ClosestColor(ColorUsageEntry& entry, ColorUsageList& palette, UInt8* pPaletteIndex = 0);
	UInt32	ClosestColor(PiRGBColor const &color, ColorPalette const &palette, bool useAlpha);

	void	OptimizePalette(ColorUsageList& palette, UInt32 numColors);
	//void	OptimizePaletteMedianCut(ColorUsageList& palette, UInt32 numColors);

	void	ColorUsageListToColorPalette(const ColorUsageList& inColorUsageList, ColorPalette& outColorPalette);
};


#endif
