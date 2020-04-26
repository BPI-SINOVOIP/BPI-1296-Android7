#ifndef RC_H
#define RC_H

#include <Platform_Lib/FontRenderer/render.h>
#include <Platform_Lib/Graphics/Theme.h>
#include <Application/AppClass/setupdef.h>
//#include <Utility/ResourceMgr/str_table.h>
#include "pthread.h"


#define STRING_MAX_LENGTH 128
// in RSS script, we've to translate "$[STRING_ID]" to real text string.
// TRANSLATE_TXT_BUF_LEN is the tmp buffer length used to store the translated string.
#define TRANSLATE_TXT_BUF_LEN  2048
//#define HI_FONT_GB

/*
 * string-to-id mappping table
 */

typedef struct _tagString2ID {
	char *string;
	int id;
} STRINGID_MAPPING_TABLE;

/*
Font Resource
*/
typedef enum
{
    FONT_FREETYPE,
    FONT_SCH,
    FONT_BDF,
    FONT_NUM
} ENUM_FONT;

    /*
    Font Style
    */
    typedef enum
    {
        FONT_STYLE_ARIAL,
        FONT_STYLE_TIMES_NEW_ROMAN,
		FONT_STYLE_CJK,
        FONT_STYLE_NUM
    } ENUM_FONT_STYLE;
    /*
    Font Size
    */
    typedef enum
    {
        FONT_SIZE_LARGE,
        FONT_SIZE_MEDIUM,
        FONT_SIZE_SMALL,
	FONT_SIZE_CUSTOM,
        FONT_SIZE_NUM
    } ENUM_FONT_SIZE;


typedef enum
{
    RC_STRING=0x8000,
    RC_IMAGE=0x4000,
    RC_FONT=0x2000
}ENUM_RC;


typedef int IMAGEHANDLE;

class ResourceMgr {
public:
    bool GetFontRenderer(FontRenderer **ppFontRenderer, ENUM_OSD_LANGUAGE language_id = OSD_LANG_INVALID);

	inline bool GetFontRenderer(ENUM_FONT font_id, ENUM_FONT_STYLE style_id, ENUM_FONT_SIZE size_id, FontRenderer **ppFontRenderer, ENUM_OSD_LANGUAGE language_id = OSD_LANG_INVALID) {
		return GetFontRenderer(ppFontRenderer, language_id);
	}

#if defined(USE_DFB_SUPPORT)
    bool GetFontFilePath(char *path);
#endif

    bool SetLanguage(int language_id);
    int GetLanguage(int language_id);

    bool GetString(int string_id, char **ppString);
	bool GetString(char *pString, char **ppString);


    // GetString() Extended Version. Added by Zack.
    //
    // This function can convert complex token string, like "123$[ABC]456$[DEF],
    // into multi-language string.
    //
    //  Parameters:
    //      1. pInBuf: The buffer contains the string which wants to be parsed.
    //      2. pOutBuf: The buffer contains converted string.
    //      3. iOutBufSize: The buffer size of pOutBuf.
    //
    //  Return:
    //      true --> Success, false --> Failed.
    //
    bool GetStringEx(char* pInBuf, char* pOutBuf, size_t iOutBufSize);

    bool LoadImage(int image_id, PDGIMAGEHDR *ppImageHdr);
    void UnloadImage(int image_id);

    bool GetImageFilePath(char *path, int image_id);

    //assign surface to image
    bool SetImageCache(PDGIMAGEHDR pImageHdr, SURFACEHANDLE hSurface, int x=0, int y=0);
	static ResourceMgr *GetInstance(void) {
		if(p.m_ptr) return p.m_ptr;
		else return (p.m_ptr = new ResourceMgr());
	}
friend class AutoPtr;
private:
    bool LoadStringTable(const char *filename);
    ResourceMgr(void);
    ~ResourceMgr(void);
	class AutoPtr
	{
	public:
		ResourceMgr *m_ptr;

		AutoPtr () { m_ptr=0;}
	    ~AutoPtr () {
            if(m_ptr) {delete m_ptr; m_ptr=0;}
        }
	};
	static AutoPtr p;

    PDGIMAGEHDR *m_pImghdr;
    int *m_pImgRefContArray;
    FontRenderer *m_pFontRenderer[OSD_LANG_MAX_NUM];
    char **m_pStringTable;
    char *m_pStringContent;
    int m_language_id;
    pthread_mutex_t m_lock_mutex;
private:
    static char *m_ResourcePath;
    static int m_StringNum;
    static char * const *m_pFontFileNameArray;

public:
    // resourcePath must be a pointer to a string
    //   that is valid until the program is terminated
    static char *Init (char *resourcePath, int stringNum,
                        const char * const *pImageFileNameArray,
                        int pImageFileNameArraySize,
                        char * const *pFontFileNameArray = NULL);

	static void Uninit(void) {
		if(p.m_ptr) {
			delete p.m_ptr;
			p.m_ptr = NULL;
		}
	}

// yuyu
// for theme setting
private:
	bool m_bThemeSet;
	int m_ThemeNum;
	struct Theme **m_pTheme;

	void loadThemeSetting();
public:
	int GetFocusBgColor(int index);
	int GetUnfocusBgColor(int index);
	int GetInactiveBgColor(int index);
	int GetFocusTextColor(int index);
	int GetUnfocusTextColor(int index);
	int GetInactiveTextColor(int index);
	int GetStaticColor(int index);
	int GetStatusColor(int index);
	int GetWarningColor(int index);
	int GetReserveColor(int index);
#if 0
	int GetFocusTextSize(int index);
	int GetUnfocusTextSize(int index);
	int GetInactiveTextSize(int index);
#endif

	// 2008/10/16
	// Chih-pin Wu <wucp@realtek.com>
	// String-indexed string supporting functions
	STRINGID_MAPPING_TABLE *m_mapTable;
	void _probe_string_id_list(void);
	void _free_string_id_list(void);
	int _get_string_id_index(char *pString);

	static int _string_id_compare(const void *s, const void *t);
};


#endif /* RC_H */
