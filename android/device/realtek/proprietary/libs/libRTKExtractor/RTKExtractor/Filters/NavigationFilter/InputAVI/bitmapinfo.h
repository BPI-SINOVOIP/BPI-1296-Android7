#if !defined(_BITMAPINFOHEADER_)
#define _BITMAPINFOHEADER_

typedef struct {
    uint32_t   biSize;
    uint32_t   biWidth;
    uint32_t   biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t   biCompression;
    uint32_t   biSizeImage;
    uint32_t   biXPelsPerMeter;
    uint32_t   biYPelsPerMeter;
    uint32_t   biClrUsed;
    uint32_t   biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER, *LPBITMAPINFOHEADER;

typedef struct {
	BITMAPINFOHEADER bmiHeader;
	int        bmiColors[4];
	int        extraData[18];
} BITMAPINFO, *LPBITMAPINFO;

#endif
