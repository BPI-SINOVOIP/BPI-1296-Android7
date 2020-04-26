//-----------------------------------------------------------------------------
// SysPrimitivesCommon.h
// Copyright (c) 2002 - 2004, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------
//SR FS: Reviewed [wwt 20040921] - safe input param; no MBCS; no dangerous API; no registry/sys folder/temp file 
//SR FS: Reviewed [DDT 20040922] Second pass.

#ifndef _SysPrimitivesCommon_H
#define _SysPrimitivesCommon_H

namespace SonicSurface 
{
//_____________________________________________________________________________
//
// Enumerations
//
//_____________________________________________________________________________

	//for generic rendering types
	enum RBotType
	{
		eRBT_GDI =		0x00000001,		//default GDI bot
		eRBT_GL	 =		0x00000010,		//default GL bot
		eRBT_D3D =		0x00000100,		//default D3D bot
		eRBT_GD	 =		0x00001000,		//default GD bot
		eRBT_DFB =		0x00010000,		

		eRBT_UNKNOWN =	0x01000000,		//unknown type

		eRBT_USER =		0x10000000		//user bot flag (non default)
	};

	// names aren't my fault - they're from GDI+ :)  -sds 2/6/02
	enum HexColor {
        eColorAliceBlue            = 0xF0F8FF,
        eColorAntiqueWhite         = 0xFAEBD7,
        eColorAqua                 = 0x00FFFF,
        eColorAquamarine           = 0x7FFFD4,
        eColorAzure                = 0xF0FFFF,
        eColorBeige                = 0xF5F5DC,
        eColorBisque               = 0xFFE4C4,
        eColorBlack                = 0x000000,
        eColorBlanchedAlmond       = 0xFFEBCD,
        eColorBlue                 = 0x0000FF,
        eColorBlueViolet           = 0x8A2BE2,
        eColorBrown                = 0xA52A2A,
        eColorBurlyWood            = 0xDEB887,
        eColorCadetBlue            = 0x5F9EA0,
        eColorChartreuse           = 0x7FFF00,
        eColorChocolate            = 0xD2691E,
        eColorCoral                = 0xFF7F50,
        eColorCornflowerBlue       = 0x6495ED,
        eColorCornsilk             = 0xFFF8DC,
        eColorCrimson              = 0xDC143C,
        eColorCyan                 = 0x00FFFF,
        eColorDarkBlue             = 0x00008B,
        eColorDarkCyan             = 0x008B8B,
        eColorDarkGoldenrod        = 0xB8860B,
        eColorDarkGray             = 0xA9A9A9,
        eColorDarkGreen            = 0x006400,
        eColorDarkKhaki            = 0xBDB76B,
        eColorDarkMagenta          = 0x8B008B,
        eColorDarkOliveGreen       = 0x556B2F,
        eColorDarkOrange           = 0xFF8C00,
        eColorDarkOrchid           = 0x9932CC,
        eColorDarkRed              = 0x8B0000,
        eColorDarkSalmon           = 0xE9967A,
        eColorDarkSeaGreen         = 0x8FBC8B,
        eColorDarkSlateBlue        = 0x483D8B,
        eColorDarkSlateGray        = 0x2F4F4F,
        eColorDarkTurquoise        = 0x00CED1,
        eColorDarkViolet           = 0x9400D3,
        eColorDeepPink             = 0xFF1493,
        eColorDeepSkyBlue          = 0x00BFFF,
        eColorDimGray              = 0x696969,
        eColorDodgerBlue           = 0x1E90FF,
        eColorFirebrick            = 0xB22222,
        eColorFloralWhite          = 0xFFFAF0,
        eColorForestGreen          = 0x228B22,
        eColorFuchsia              = 0xFF00FF,
        eColorGainsboro            = 0xDCDCDC,
        eColorGhostWhite           = 0xF8F8FF,
        eColorGold                 = 0xFFD700,
        eColorGoldenrod            = 0xDAA520,
        eColorGray                 = 0x808080,
        eColorGreen                = 0x00ff00,
        eColorGreenYellow          = 0xADFF2F,
        eColorHoneydew             = 0xF0FFF0,
        eColorHotPink              = 0xFF69B4,
        eColorIndianRed            = 0xCD5C5C,
        eColorIndigo               = 0x4B0082,
        eColorIvory                = 0xFFFFF0,
        eColorKhaki                = 0xF0E68C,
        eColorLavender             = 0xE6E6FA,
        eColorLavenderBlush        = 0xFFF0F5,
        eColorLawnGreen            = 0x7CFC00,
        eColorLemonChiffon         = 0xFFFACD,
        eColorLightBlue            = 0xADD8E6,
        eColorLightCoral           = 0xF08080,
        eColorLightCyan            = 0xE0FFFF,
        eColorLightGoldenrodYellow = 0xFAFAD2,
        eColorLightGray            = 0xD3D3D3,
        eColorLightGreen           = 0x90EE90,
        eColorLightPink            = 0xFFB6C1,
        eColorLightSalmon          = 0xFFA07A,
        eColorLightSeaGreen        = 0x20B2AA,
        eColorLightSkyBlue         = 0x87CEFA,
        eColorLightSlateGray       = 0x778899,
        eColorLightSteelBlue       = 0xB0C4DE,
        eColorLightYellow          = 0xFFFFE0,
        eColorLime                 = 0x008000,
        eColorLimeGreen            = 0x32CD32,
        eColorLinen                = 0xFAF0E6,
        eColorMagenta              = 0xFF00FF,
        eColorMaroon               = 0x800000,
        eColorMediumAquamarine     = 0x66CDAA,
        eColorMediumBlue           = 0x0000CD,
        eColorMediumOrchid         = 0xBA55D3,
        eColorMediumPurple         = 0x9370DB,
        eColorMediumSeaGreen       = 0x3CB371,
        eColorMediumSlateBlue      = 0x7B68EE,
        eColorMediumSpringGreen    = 0x00FA9A,
        eColorMediumTurquoise      = 0x48D1CC,
        eColorMediumVioletRed      = 0xC71585,
        eColorMidnightBlue         = 0x191970,
        eColorMintCream            = 0xF5FFFA,
        eColorMistyRose            = 0xFFE4E1,
        eColorMoccasin             = 0xFFE4B5,
        eColorNavajoWhite          = 0xFFDEAD,
        eColorNavy                 = 0x000080,
        eColorOldLace              = 0xFDF5E6,
        eColorOlive                = 0x808000,
        eColorOliveDrab            = 0x6B8E23,
        eColorOrange               = 0xFFA500,
        eColorOrangeRed            = 0xFF4500,
        eColorOrchid               = 0xDA70D6,
        eColorPaleGoldenrod        = 0xEEE8AA,
        eColorPaleGreen            = 0x98FB98,
        eColorPaleTurquoise        = 0xAFEEEE,
        eColorPaleVioletRed        = 0xDB7093,
        eColorPapayaWhip           = 0xFFEFD5,
        eColorPeachPuff            = 0xFFDAB9,
        eColorPeru                 = 0xCD853F,
        eColorPink                 = 0xFFC0CB,
        eColorPlum                 = 0xDDA0DD,
        eColorPowderBlue           = 0xB0E0E6,
        eColorPurple               = 0x800080,
        eColorRed                  = 0xFF0000,
        eColorRosyBrown            = 0xBC8F8F,
        eColorRoyalBlue            = 0x4169E1,
        eColorSaddleBrown          = 0x8B4513,
        eColorSalmon               = 0xFA8072,
        eColorSandyBrown           = 0xF4A460,
        eColorSeaGreen             = 0x2E8B57,
        eColorSeaShell             = 0xFFF5EE,
        eColorSienna               = 0xA0522D,
        eColorSilver               = 0xC0C0C0,
        eColorSkyBlue              = 0x87CEEB,
        eColorSlateBlue            = 0x6A5ACD,
        eColorSlateGray            = 0x708090,
        eColorSnow                 = 0xFFFAFA,
        eColorSpringGreen          = 0x00FF7F,
        eColorSteelBlue            = 0x4682B4,
        eColorTan                  = 0xD2B48C,
        eColorTeal                 = 0x008080,
        eColorThistle              = 0xD8BFD8,
        eColorTomato               = 0xFF6347,
        eColorTurquoise            = 0x40E0D0,
        eColorViolet               = 0xEE82EE,
        eColorWheat                = 0xF5DEB3,
        eColorWhite                = 0xFFFFFF,
        eColorWhiteSmoke           = 0xF5F5F5,
        eColorYellow               = 0xFFFF00,
        eColorYellowGreen          = 0x9ACD32
	};

	enum BrushStyle {
		eBrushSolidColor		= 0,
		eBrushHatchFill			= 1,
		eBrushLinearGradient	= 2
	};
	// Since Win32 GDI+ is being implemented first, it got to decide the list of
	// hatch styles.  Adding is OK (but you'll have to fix up the GDI+ renderBot)
	// but DO NOT CHANGE the existing enum values, unless you want to do a lot of
	// work... -sds 2/4/02
	enum HatchStyle {
		eHatchHorizontal = 0,
		eHatchVertical = 1,
		eHatchForwardDiagonal = 2,
		eHatchBackwardDiagonal = 3,
		eHatchCross = 4,
		eHatchDiagonalCross = 5,
		eHatch05Percent = 6,
		eHatch10Percent = 7,
		eHatch20Percent = 8,
		eHatch25Percent = 9,
		eHatch30Percent = 10,
		eHatch40Percent = 11,
		eHatch50Percent = 12,
		eHatch60Percent = 13,
		eHatch70Percent = 14,
		eHatch75Percent = 15,
		eHatch80Percent = 16,
		eHatch90Percent = 17,
		eHatchLightDownwardDiagonal = 18,
		eHatchLightUpwardDiagonal = 19,
		eHatchDarkDownwardDiagonal = 20,
		eHatchDarkUpwardDiagonal = 21,
		eHatchWideDownwardDiagonal = 22,
		eHatchWideUpwardDiagonal = 23,
		eHatchLightVertical = 24,
		eHatchLightHorizontal = 25,
		eHatchNarrowVertical = 26,
		eHatchNarrowHorizontal = 27,
		eHatchDarkVertical = 28,
		eHatchDarkHorizontal = 29,
		eHatchDashedDownwardDiagonal = 30,
		eHatchDashedUpwardDiagonal = 31,
		eHatchDashedHorizontal = 32,
		eHatchDashedVertical = 33,
		eHatchSmallConfetti = 34,
		eHatchLargeConfetti = 35,
		eHatchZigZag = 36,
		eHatchWave = 37,
		eHatchDiagonalBrick = 38,
		eHatchHorizontalBrick = 39,
		eHatchWeave = 40,
		eHatchPlaid = 41,
		eHatchDivot = 42,
		eHatchDottedGrid = 43,
		eHatchDottedDiamond = 44,
		eHatchShingle = 45,
		eHatchTrellis = 46,
		eHatchSphere = 47,
		eHatchSmallGrid = 48,
		eHatchSmallCheckerBoard = 49,
		eHatchLargeCheckerBoard = 50,
		eHatchOutlinedDiamond = 51,
		eHatchSolidDiamond = 52,
		// aliases
		eHatchLargeGrid = eHatchCross
// **CodeWizzard** - Possible Violation:  User Defined item 740  - When using enum, the values of each member should be explicitly declared
	};
	// again, these map to Win32 GDI+ enums, so don't change the
	// values, or you'll be making more work for yourself -sds 2/4/02
	enum GradientMode {
		eGradientHorizontal = 0,
		eGradientVertical   = 1,
		eGradientForwardDiagonal  = 2,
		eGradientBackwardDiagonal = 3
	};
}


#endif	//_SysPrimitivesCommon_H
