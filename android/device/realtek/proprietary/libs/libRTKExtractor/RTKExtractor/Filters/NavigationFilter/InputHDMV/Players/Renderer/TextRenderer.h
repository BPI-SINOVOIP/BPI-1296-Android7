//-----------------------------------------------------------------------------
// TextRenderer.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

//#include "RendererTypes.h"

#include <string>
#include "PiRect.h"
#include "PiRGBColor.h"
#include "SysTextForward.h"
#include "SysHDSurface.h"
#include "TextDefs.h"


namespace TextRenderer {

	struct RENDERER_EXPORT GlyphInfo {
		ptrdiff_t		charOffset;				// offset into the UTF-8 character buffer for this glyph
		int				unichar;				// Unicode character for this glyph
		unsigned int	glyphIndex;				// glyph index into font
		PiPoint			glyphPosition;			// rendering postion for this glyph (surface coords)
		PiRect			glyphBounds;			// rendering bounds for this glyph (surface coords)
	};

	typedef std::vector<GlyphInfo> LineGlyphs;

	struct RENDERER_EXPORT FontMetrics {
		SInt32			ascender;				// vertical pixels above the baseline
		SInt32			descender;				// vertical pixels below the baseline (negative value)
		SInt32			height;					// vertical pixels for overall height
	};


	// shutdown routine
	void
	RENDERER_EXPORT Shutdown();

	// capability routines
	bool
	RENDERER_EXPORT SupportsByteCodeInterpreter();

	// font management routines
	bool
	RENDERER_EXPORT LoadFont(
		void const * fontBuffer,						// IN: font file contents in buffer, caller retains ownership
		size_t fontBufferSize,					// IN: font file contents size
		const std::string& fontKey);			// IN: key/tag/identifier for loaded font

	void
	RENDERER_EXPORT UnloadFont(
		const std::string& fontKey);			// IN: key/tag/identifier for font to unload

	// support for TrueType font collection files (.ttc)
	// get the number of fonts(font faces)
	UInt32 
	RENDERER_EXPORT NumOfFontsInTTC(
		void const * fontBuffer,		// IN: font file contents in buffer, caller retains ownership
		size_t fontBufferSize);			// IN: font file contents size);

	// query the font information through the font index 
	void 
	RENDERER_EXPORT QueryFontInfo(
		void const * fontBuffer,		// IN: font file contents in buffer, caller retains ownership
		size_t fontBufferSize,			// IN: font file contents size
		UInt32 fontIndex,				// IN: font index
		unicode::string16& fontFamily,  // OUT:font family 
		unicode::string16& fontStyle);  // OUT:font style 

	// load the font from TTC stream(loaded from TTC file) 
	bool
	RENDERER_EXPORT LoadFontFromTTC(
		void const * fontBuffer,       	// IN: font file contents in buffer, caller retains ownership
		size_t fontBufferSize,          // IN: font file contents size
		UInt32 fontIndex,				// IN: font index
		const std::string& fontKey);	// IN: key/tag/identifier for loaded font

	// single line measure/draw routines - must call MeasureLine then call DrawLine
	void
	RENDERER_EXPORT MeasureLine(
		const SonicSurface::SysTextData& text,	// IN: text w/ attributes to measure
		size_t inCharOffset,					// IN: offset into text.mText
		LineGlyphs& outLineGlyphs,				// OUT: glyph info for the line of text
		size_t& outCharsUsed);					// OUT: count of characters used for line

	void
	RENDERER_EXPORT MeasureLine(
		const std::vector<SonicSurface::SysTextData>& texts,	// IN: text segments w/ attributes to measure
		size_t inCharOffset,					// IN: offset into text.mText
		LineGlyphs& outLineGlyphs,				// OUT: glyph info for the line of text
		size_t& outCharsUsed);					// OUT: count of characters used for line

	void
	RENDERER_EXPORT DrawLine(
		const SonicSurface::SysTextData& text,	// IN: text w/ attributes to draw
		SonicSurface::SysHDSurface& surface,	// IN: surface to draw text on
		const LineGlyphs& lineGlyphs);			// IN: glyph info for line (from MeasureLine)

	void
	RENDERER_EXPORT DrawLine(
		const std::vector<SonicSurface::SysTextData>& texts,	// IN: text segments w/ attributes to draw
		SonicSurface::SysHDSurface& surface,	// IN: surface to draw text on
		const LineGlyphs& lineGlyphs);			// IN: glyph info for line (from MeasureLine)

#ifndef REALTEK_MODIFY
	// multi-line measure/draw routines - either can be called independently
	void
	RENDERER_EXPORT MeasureText(
		const SonicSurface::SysTextData& text,	// IN: text w/ attributes to measure
		PiRect& outBounds);						// OUT: measured bounds of the text

	void
	RENDERER_EXPORT MeasureText(
		const std::vector<SonicSurface::SysTextData>& texts,	// IN: text segments w/ attributes to measure
		PiRect& outBounds);						// OUT: measured bounds of the text

	void
	RENDERER_EXPORT DrawText(
		const SonicSurface::SysTextData& text,	// IN: text w/ attributes to draw
		SonicSurface::SysHDSurface& surface,	// IN: surface to draw text on
		PiRect& outBounds);						// OUT: measured bounds of the text

	void
	RENDERER_EXPORT DrawText(
		const std::vector<SonicSurface::SysTextData>& texts,	// IN: text segments w/ attributes to draw
		SonicSurface::SysHDSurface& surface,	// IN: surface to draw text on
		PiRect& outBounds);						// OUT: measured bounds of the text
#endif

	// utility routines
	PiRect
	RENDERER_EXPORT GetGlyphsBounds(
		const LineGlyphs& lineGlyphs);			// IN: glyph info for line (from MeasureLine)

	void
	RENDERER_EXPORT GetFontMetrics(
		const SonicSurface::PiFont& inFont,		// IN: font to retrieve metrics for
		FontMetrics& outMetrics);				// OUT: font metrics

	SInt32
	RENDERER_EXPORT DeterminePointSize(
		const std::string& inFontFace,			// IN: font to retrieve metrics for
		SInt32 inMaxHeight);					// IN: the max height of the font

	enum WhiteSpaceTreatment {
		ignoreWS = 0,
		preserveWS = 1,
		ignoreWSBeforeLF = 2,
		ignoreWSAfterLF = 3,
		ignoreWSAroundLF = 4
	};

	enum LinefeedTreatment {
		ignoreLF = 0,
		preserveLF = 1,
		treatLFAsSpace = 2,
		treatLFAsZeroWidthSpace = 3
	};

	void
	RENDERER_EXPORT TreatWhitespaceAndLinefeeds(
		unicode::string16& text,				// IN/OUT: text to apply treatments to
		WhiteSpaceTreatment wsTreatment = ignoreWSAroundLF, 
		LinefeedTreatment lfTreatment = treatLFAsSpace, 
		bool collapseWhitespace = true);

	void
	RENDERER_EXPORT TreatWhitespaceAndLinefeeds(
		unicode::string8& text,					// IN/OUT: text to apply treatments to
		WhiteSpaceTreatment wsTreatment = ignoreWSAroundLF, 
		LinefeedTreatment lfTreatment = treatLFAsSpace, 
		bool collapseWhitespace = true);

	void
	RENDERER_EXPORT TrimWhitespace(
		unicode::string16& text,
		bool trimHead=true, 
		bool trimTail=true);

	void
	RENDERER_EXPORT TrimWhitespace(
		unicode::string8& text,
		bool trimHead=true, 
		bool trimTail=true);

	void
	RENDERER_EXPORT VerticallyAlign(
		SonicSurface::SysHDSurface *surface, 
		PiRect &rgnRect, 
		SInt32 txtHeight,
		SonicSurface::eTextVerticalAlignment vertAlign);
}

#endif // TEXT_RENDERER_H
