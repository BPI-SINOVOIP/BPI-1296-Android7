#ifndef RENDER_H
#define RENDER_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_CACHE_MANAGER_H
#include FT_GLYPH_H

#include <Platform_Lib/FontRenderer/otf.h>

// we support DirectGraphics
#include <Platform_Lib/Graphics/DirectGraphics.h>
#include <pthread.h>
#include <Platform_Lib/FontRenderer/charset_def.h>
#include <Platform_Lib/GradientTool/GradientTool.h>

// FLAGS
	// alignment
#define H_LEFT				0x00000000
#define H_CENTER			0x00000001
#define H_RIGHT				0x00000002
#define H_JUSTIFY			0x00000004
#define V_TOP				0x00000000
#define V_CENTER			0x00000010
#define V_BOTTOM			0x00000020
#define DOT_RIGHT			0x00000040
#define DOT_LEFT			0x00000080
#define DOT_CENTER			0x00000008
	// for rendering scheme
#define MONOCHROME			0x00000100
#define EFFECT_SHADOW		0x00000200
#define EFFECT_UNDERLINE	0x00000400
#define EFFECT_ITALIC		0x00000800
#define EFF_OUTLINE_THICK	0x00001000
#define COMPLETE_GLYPH		0x00002000
#define RIGHT_TO_LEFT		0x00004000
	// Manual Multiline (line wrap by \n)
#define M_MULTILINE			0x00010000
	// Automatic Multiline (line wrap automatically but not sensing '\n')
#define A_MULTILINE			0x00020000
	// Hybrid Multiline (automatically line wrapping with awareness of '\n')
#define AM_MULTILINE		0x00040000
#define BG_BLEND			0x00100000
#define VIDEO_BLEND			0x00200000
#define GRADIENT_COLORING	0x00400000
#define CJK_SCHEME			0x01000000
#define NONCJK_SCHEME		0x02000000
#define MIXED_SCHEME		0x04000000
#define DISABLE_BIDI		0x08000000

enum tagFontEngineID {
	FONT_ENGINE_UNDEFINED = 0,
	FONT_ENGINE_FREETYPE = 1,
	FONT_ENGINE_MFONT = 2,
};

typedef struct _tagRenderResult {
	int pixels;	// pixel count, to indicate how many pixels has been used for rendering, value will be 0 for multiline rendering
	int offset; // string offset, to indicate where the last rendered character is, value will be 0 for multiline rendering
	int lines;	// line count, to inidicate how many lines has been used for rendering, value will be 0 for non-multiline rendering
} RenderResult;

class RenderEngine;

class FontRenderer {
	private:
		int x_start;    // x-axis of origin point
		int y_start;    // y-axis of origin point

		pthread_mutex_t rendering_mutex;

		int m_spacePixel;
		int m_spaceCount;

		GradientTool *m_pGradientTool;
		RenderEngine *m_engine;

	private:
		void ChangeHeight(int height, bool lock, bool viaScale);

		void _DrawText(HANDLE handleDisplay,
				int top,
				int left,
				int bottom,
				int right,
				int color_fg,
				int color_bg,
				OTF_GlyphString string,
				int FLAG,
				int fontHeight,
				int startLine = 0,
				RenderResult *result = NULL,
				float rotation=0,
				float pixel_h_w=-1,
				int xPos=-1,
				int yPos=-1,
				int xMin=-1,
				int xMax=-1,
				int yMin=-1,
				int yMax=-1);

		void DrawOntoBuffer_2bit(FT_BitmapGlyph bitmap,
						char *buffer,
						int height,
						int width,
						char cr_fg1,
						char cr_fg2,
						float rotation=0,
						float pixel_h_w=-1,
						int xPos=-1,
						int yPos=-1,
						int xMin=-1,
						int xMax=-1,
						int yMin=-1,
						int yMax=-1);

		// DG version
		void DrawOntoSurface_Mono(SURFACEDESC *pDesc,
						FT_BitmapGlyph bitmap,
						OTF_Glyph glyphInfo,
						int top,
						int left,
						int bottom,
						int right,
						int color_fg,
						int FLAG,
						float rotation=0,
						float pixel_h_w=-1,
						int xPos=-1,
						int yPos=-1,
						int xMin=-1,
						int xMax=-1,
						int yMin=-1,
						int yMax=-1);

		void DrawOntoSurface(SURFACEDESC *pDesc,
						FT_BitmapGlyph bitmap,
						OTF_Glyph glyphInfo,
						int top,
						int left,
						int bottom,
						int right,
						int color_fg,
						int color_bg,
						int FLAG,
						float rotation=0,
						float pixel_h_w=-1,
						int xPos=-1,
						int yPos=-1,
						int xMin=-1,
						int xMax=-1,
						int yMin=-1,
						int yMax=-1);

		void DrawOntoSurface_BGBlend(SURFACEDESC *pDesc,
						FT_BitmapGlyph bitmap,
						OTF_Glyph glyphInfo,
						int top,
						int left,
						int bottom,
						int right,
						int color_fg,
						int FLAG,
						float rotation=0,
						float pixel_h_w=-1,
						int xPos=-1,
						int yPos=-1,
						int xMin=-1,
						int xMax=-1,
						int yMin=-1,
						int yMax=-1);

		void DrawOntoSurface_Gradient(SURFACEDESC *pDesc,
						FT_BitmapGlyph bitmap,
						OTF_Glyph glyphInfo,
						int top,
						int left,
						int bottom,
						int right,
						int color_fg,
						int FLAG,
						float rotation=0,
						float pixel_h_w=-1,
						int xPos=-1,
						int yPos=-1,
						int xMin=-1,
						int xMax=-1,
						int yMin=-1,
						int yMax=-1);

		void DrawOutlineOntoSurface(SURFACEDESC *pDesc,
						FT_BitmapGlyph bitmap,
						OTF_Glyph glyphInfo,
						int top,
						int left,
						int bottom,
						int right,
						int color_fg,
						int FLAG,
						float rotation=0,
						float pixel_h_w=-1,
						int xPos=-1,
						int yPos=-1,
						int xMin=-1,
						int xMax=-1,
						int yMin=-1,
						int yMax=-1);

		void DG_MLProcessor(HANDLE handleDisplay,
			int top,
			int left,
			int bottom,
			int right,
			int color_fg,
			int color_bg,
			OTF_GlyphString string,
			int FLAG,
			int fontHeight,
			RenderResult *result,
			int startLine,
			float rotation=0,
			float pixel_h_w=-1,
			int xPos=-1,
			int yPos=-1,
			int xMin=-1,
			int xMax=-1,
			int yMin=-1,
			int yMax=-1);


		void DG_AMLProcessor(HANDLE handleDisplay,
			int top,
			int left,
			int bottom,
			int right,
			int color_fg,
			int color_bg,
			OTF_GlyphString string,
			int FLAG,
			int fontHeight,
			RenderResult *result,
			int startLine,
			float rotation=0,
			float pixel_h_w=-1,
			int xPos=-1,
			int yPos=-1,
			int xMin=-1,
			int xMax=-1,
			int yMin=-1,
			int yMax=-1);


		unsigned int _CapableWordCount(OTF_GlyphString GlyphString, unsigned int width, int FLAG, int startIdx = 0, unsigned int *originStrIdx=0);
		unsigned int _TextWidth(char *string, char charset, int fontHeight, bool lock);

		// utility functions for OTF_GlyphString
		unsigned int getCharIndexByGlyphIndex(char *string, char charset, unsigned int glyphIndex);

		void Get_OTF_Glyph_String(char *string, char charset, OTF_GlyphString **glyphString);
		void Free_OTF_Glyph_String(OTF_GlyphString **glyphString);

		// copy [length] of glyph from src to dest, dest will be automatically enlarged or allocated
		void OTF_strncpy(OTF_GlyphString **dest, const OTF_GlyphString src, size_t start, size_t length);

		static unsigned int calcNewline(OTF_GlyphString string);
              int m_str_tail_width;
              int m_str_tail_width_font_height;
		int m_pos_dot_center;
	public:
        enum ENUM_UNICODE_LANG {
            UNICODE_LANG_UNKNOWN = 0,
            UNICODE_LANG_ARABIC,
            UNICODE_LANG_HEBREW,
            UNICODE_LANG_Indic_Devanagari,
            UNICODE_LANG_Indic_Bengali,
            UNICODE_LANG_Indic_Gurmukhi,
            UNICODE_LANG_Indic_Gujarati,
            UNICODE_LANG_Indic_Oriya,
            UNICODE_LANG_Indic_Tamil,
            UNICODE_LANG_Indic_Telugu,
            UNICODE_LANG_Indic_Kannada,
            UNICODE_LANG_Indic_Malayalam,
            UNICODE_LANG_Indic_Sinhala_alphabet,
            UNICODE_LANG_Indic_Limbu,
            UNICODE_LANG_Indic_Syloti,
            UNICODE_LANG_THAI,
            UNICODE_LANG_CJK,
        };
		FontRenderer(char *fontname);
		~FontRenderer();

		void SetGradientTool(GradientTool *instance);

		void DrawText_2bit(char *buffer,
			 int height,
			 int width,
			 char cr_fg1,
			 char cr_fg2,
			 char *string,
			 char charset,
			 int FLAG);

		void DG_DrawText(HANDLE handleDisplay,
			int top,
			int left,
			int bottom,
			int right,
			int color_fg,
			int color_bg,
			char *string,
			char charset,
			int FLAG,
			int fontHeight = -1,
			RenderResult *result = NULL,
			int startLine = 0,
			int color_fg2 = 0,
			int vert_offset = 0, // for shadow effect
			int hori_offset = 0,
			FT_Fixed xy = 0x4000,
			PIXEL_FORMAT colorFormat = NO_OF_PIXEL_FORMAT
			);
/*
support rotate
1. rotation : degree , positive is clockwise
2. pixel_h_w : h_w = (TVsystem)/(OSD dimension)
    ex : h_w = (4/3)/(1280/720) = (3/4)
3. xPos , yPos : the point of rotation
4. xMin , xMax , yMin, yMax : clipping rectangle
*/
		void DG_DrawText_Rotate(HANDLE handleDisplay,
			int top,
			int left,
			int bottom,
			int right,
			int color_fg,
			int color_bg,
			char *string,
			char charset,
			int FLAG,
			int fontHeight = -1,
			RenderResult *result = NULL,
			int startLine = 0,
			int color_fg2 = 0,
			int vert_offset = 0, // for shadow effect
			int hori_offset = 0,
			FT_Fixed xy = 0x4000,
			PIXEL_FORMAT colorFormat = NO_OF_PIXEL_FORMAT,
			float rotation=0,
			float pixel_h_w=-1,
			int xPos=-1,
			int yPos=-1,
			int xMin=-1,
			int xMax=-1,
			int yMin=-1,
			int yMax=-1);

		inline void DG_TextOut(HANDLE handleDisplay,
			int left,
			int top,
			int width,
			int height,
			int color_fg,
			int color_bg,
			char *string,
			char charset,
			int FLAG,
			int fontHeight = -1,
			RenderResult *result = NULL,
			int startLine = 0,
			int color_fg2 = 0,
			int hori_offset = 0, // for shadow effect
			int vert_offset = 0,
			FT_Fixed xy = 0x4000,
			PIXEL_FORMAT colorFormat = NO_OF_PIXEL_FORMAT,
			float rotation=0,
			float pixel_h_w=-1,
			int xPos=-1,
			int yPos=-1,
			int xMin=-1,
			int xMax=-1,
			int yMin=-1,
			int yMax=-1
			) { // for slanted effect
				DG_DrawText_Rotate(handleDisplay, top, left, top+height-1, left+width-1,
								color_fg, color_bg, string, charset, FLAG,
								fontHeight, result, startLine, color_fg2, vert_offset, hori_offset, xy, colorFormat,
								rotation,pixel_h_w,xPos,yPos,xMin,xMax,yMin,yMax);
			}

		void SetHeight(int height);
		int GetLineHeight(void);
		void SetCCHeight(int height);
		bool SetPAR(int horizontal, int vertical);
		void setLanguage(int language_id);
        int GetFontAscend();
        int GetFontDescent();
		void setPosDotCenter(int pos);
		static size_t mystrlen(const char *string, char charset); // as an alternative wcslen()
		inline unsigned int TextWidth(char *string, char charset, int fontHeight = -1) {
			return _TextWidth(string, charset, fontHeight, true);
		}

		unsigned int CapableWordCount(char *string, char charset, unsigned int width, int FLAG = A_MULTILINE|CJK_SCHEME, int fontHeight = -1);
		unsigned int GetTextLineNumber(char *string, char charset, unsigned int width, int FLAG = A_MULTILINE|CJK_SCHEME, int fontHeight = -1);
		unsigned int getDotWidthCriterion(char *string, char charset, unsigned int width, int fontHeight = -1);

		// utility functions
		static size_t charset_convert(char from, char to, char *inbuf, char *outbuf, size_t outbufSize);
		static unsigned long get_char_code(char *string, char charset, int *offset, FontRenderer *renderer=NULL, bool getLigatureOffset=FALSE);
		int getFontEngineType(void);
		int getLigatureWidth(unsigned long *string, int strLength);

		/*
		Return value:
		    reference #include <Platform_Lib/FontRenderer/charset_def.h>
		*/
		static int get_charset_from_string(unsigned char *string,int str_len=-1,char charset=UTF8);

		static inline bool IS_UNICODE(char charset) {
			if(charset == UTF16_LE || charset == UTF16_BE || charset == UTF8)
				return true;
			else
				return false;
		}

		static bool IS_CJK(unsigned long charcode);

#if AP_CONFIG_WITH(APCFG_LAG_ARABIC)
		static bool IS_ARABIC(unsigned long charcode);
		static bool IS_ARABIC_STRING(OTF_GlyphString &GlyphString);
#endif

#if AP_CONFIG_WITH(APCFG_LAG_THAI)
		static bool IS_THAI(unsigned long charcode);
		static bool IS_THAI_STRING(OTF_GlyphString &GlyphString);
#endif

#if AP_CONFIG_WITH(APCFG_LAG_HEBREW)
		static bool IS_HEBREW(unsigned long charcode);
		static bool IS_HEBREW_STRING(OTF_GlyphString &GlyphString);
#endif

//#if AP_CONFIG_WITH(APCFG_LAG_HINDI)
		static bool IS_HINDI(unsigned long charcode);
		static bool IS_HINDI_STRING(OTF_GlyphString &glyphString);
//#endif

		static ENUM_UNICODE_LANG getUnicodeLang(unsigned long charcode);
		static ENUM_UNICODE_LANG getUnicodeLang_STRING(OTF_GlyphString GlyphString);
		// deprecated
#ifdef APCFG_DTV_CC
		void SetCCTSPColor(int color) { return; }
#endif
};

#endif	// RENDER_H

