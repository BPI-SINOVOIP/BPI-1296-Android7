
/*
 * Copyright 2014 Realtek 1195
 *
 * Author : Ryan Chou
 *
 */

#include "SkColorPriv.h"
#include "SkImageDecoder.h"
#include "SkScaledBitmapSampler.h"
#include "SkStream.h"
#include "SkTDArray.h"

#include "tiffiop.h"

class SkTIFFImageDecoder : public SkImageDecoder {
public:
    SkTIFFImageDecoder() {}

    virtual Format getFormat() const {
        return kTIFF_Format;
    }

protected:
    virtual Result onDecode(SkStream* stream, SkBitmap* bm, Mode mode);

private:
    typedef SkImageDecoder INHERITED;
};

///////////////////////////////////////////////////////////////////////////////

static tsize_t TiffDec_ReadProc (thandle_t clientData, tdata_t data, tsize_t length)
{
    SkStreamRewindable* stream = (SkStreamRewindable*) clientData;
    return (int) stream->read(data, length);
}

static tsize_t TiffDec_WriteProc (thandle_t clientData, tdata_t buf, tsize_t size)
{
    return 0 ;
}

#ifdef SK_BUILD_FOR_ANDROID
static toff_t TiffDec_SeekProc (thandle_t clientData, toff_t offset, int whence) {
    SkStreamRewindable* sk_stream = (SkStreamRewindable*) clientData;

    if (whence == SEEK_SET) {
        if (!sk_stream->rewind()) {
            // handle error here
        }
    } else if (whence == SEEK_END) {
        toff_t length = sk_stream->getLength();
        toff_t offset = length - offset;

        if (!sk_stream->rewind()) {
            // handle error here
        }
    } else { // SEEK_CUR
        // do nothing
    }

    (void)sk_stream->skip(offset);

    return offset;
}
#endif

static int TiffDec_CloseProc (thandle_t clientData)
{
    return 0;
}

static toff_t TiffDec_SizeProc (thandle_t clientData)
{
    SkStreamRewindable* sk_stream = (SkStreamRewindable*) clientData;
    
    toff_t length = 0;
    if(sk_stream->hasLength()) {
       length = sk_stream->getLength();
    }
    else {
        const toff_t bufferSize = 256 * 1024; // 256KB
        do {
            toff_t bytesRead = sk_stream->skip(bufferSize);
            length += bytesRead;
        } while (!sk_stream->isAtEnd());
    }

    return length;
}
///////////////////////////////////////////////////////////////////////////////

static SkImageDecoder::Result error_return(TIFF *tiff, const SkBitmap& bm,
                         const char msg[]) {
#if 0
    SkDebugf("libtiff error <%s> bitmap [%d %d] pixels %p colortable %p\n",
             msg, bm.width(), bm.height(), bm.getPixels(), bm.getColorTable());
#endif
    if (tiff != NULL)
    {
        TIFFClose(tiff);
    }

    return SkImageDecoder::kFailure;
}

SkImageDecoder::Result SkTIFFImageDecoder::onDecode(SkStream* stream, SkBitmap* bm, Mode mode)
{

    TIFF *tiff = TIFFClientOpen("", "r", 
                                (thandle_t) stream,
                                TiffDec_ReadProc,
                                TiffDec_WriteProc,
                                TiffDec_SeekProc,
                                TiffDec_CloseProc,
                                TiffDec_SizeProc, NULL, NULL);
    if (NULL == tiff) {
        return error_return(tiff, *bm, "TIFFClientOpen");
    }

    int width, height;
    int tile_width, tile_height;
    uint32 *tile_buf = NULL;

    if (!TIFFGetField (tiff, TIFFTAG_IMAGEWIDTH,  &width)
        || !TIFFGetField (tiff, TIFFTAG_IMAGELENGTH, &height))
    {
        return error_return(tiff, *bm, "image width and height");
    }

    bool bTiled = TIFFIsTiled(tiff);

    if ( bTiled )
    {
        if ( !TIFFGetField(tiff, TIFFTAG_TILEWIDTH,  &tile_width )
            || !TIFFGetField(tiff, TIFFTAG_TILELENGTH, &tile_height) )
            return error_return(tiff, *bm, "tiled width/height");

        if ( !(tile_buf = (uint32*)_TIFFmalloc(tile_width * tile_height * sizeof(uint32))) )
            return error_return(tiff, *bm, "no enough memory");

    }


    int sampleSize = this->getSampleSize();
    SkColorType colorType = this->getPrefColorType(k32Bit_SrcDepth, false);

    // only accept prefConfig if it makes sense for us
    if (kARGB_4444_SkColorType != colorType && kRGB_565_SkColorType != colorType) {
        colorType = kN32_SkColorType;
    }

    SkScaledBitmapSampler sampler(width, height, sampleSize);
//    bm->setConfig(config, sampler.scaledWidth(), sampler.scaledHeight());
    bm->setInfo(SkImageInfo::Make(sampler.scaledWidth(), sampler.scaledHeight(),
                                  colorType, kOpaque_SkAlphaType));
    if (SkImageDecoder::kDecodeBounds_Mode == mode) {
        TIFFClose(tiff);
        return kSuccess;
    }


    if (!this->allocPixelRef(bm, NULL)) {
        TIFFClose(tiff);
        return kFailure;
    }

    SkAutoLockPixels alp(*bm);

    if (!sampler.begin(bm, SkScaledBitmapSampler::kRGBA, *this)) {
        return error_return(tiff, *bm, "sampler.begin");
    }

    // time to decode the scanlines
    //
    uint8_t*  scanline = bm->getAddr8(0, 0);
    const int rowBytes = bm->rowBytes();
    const int bmpWidth = bm->width();
    const int bmpHeight = bm->height();

    int x, y;
    int y0 = sampler.srcY0();
    int dy = sampler.srcDY();

    if (bTiled) {

        SkAutoMalloc storage(width*tile_height*4);
        uint8_t* srcRow = (uint8_t*)storage.get();
        SkAutoMalloc storage_sampler(width*4);
        uint8_t* srcRowSampler = (uint8_t*)storage_sampler.get();

        int offset_y=0;
        int sample_index = 0;
        while (offset_y < height) {            

            int sample_num = (tile_height + (offset_y % dy) - y0 > 0) ? ((tile_height + (offset_y % dy) - y0) / dy): 0;
            for ( x = 0 ; x < width ; x += tile_width )
            {
                CHECK_CANCEL_DECODE_TIFF();

                if ( !TIFFReadRGBATile(tiff, x, offset_y, tile_buf) )
                {
                    return error_return (tiff, *bm, "TIFFReadRGBATile") ;
                }
                /*
                * For some reason the TIFFReadRGBATile() function chooses the
                * lower left corner as the origin.  Vertically mirror scanlines.
                */
                for ( int tile_y = 0 ; tile_y < tile_height ; tile_y++ )
                {
                    void *src = tile_buf + tile_width * tile_y ;
                    void *dst = srcRow + width*4 * (tile_height-tile_y-1) + x*4 ;
                    _TIFFmemcpy (dst, src, tile_width * 4) ;
                }
            }

            while(y0 + sample_index*dy < offset_y + tile_height)
            {
                void *src = srcRow + (y0 + (sample_index)*dy - offset_y) * width*4;
                void *dst = srcRowSampler;
                memcpy(dst, src, width*4);
            
                sampler.next(srcRowSampler);
                sample_index++;
            }
            offset_y += tile_height ;
        }
    }
    else { /* strip */
        SkAutoMalloc storage(width*4);
        uint8_t* srcRow = (uint8_t*)storage.get();

        for (y = 0 ; y < bmpHeight ; y++)
        {
            CHECK_CANCEL_DECODE_TIFF();
        
            void *rowBuf = srcRow;
            if (!TIFFReadRGBAScanline(tiff, y0 + y*dy, (uint32*)rowBuf))
            {
                SkDebugf("libtiff error <TIFFReadRGBAScanline>\n");
                memset(rowBuf, 0, width * rowBytes/bmpWidth);
            }
            sampler.next(srcRow);
        }
    }

    TIFFClose(tiff);        
    return kSuccess;
}

///////////////////////////////////////////////////////////////////////////////
DEFINE_DECODER_CREATOR(TIFFImageDecoder);
///////////////////////////////////////////////////////////////////////////////

static bool is_tiff(SkStreamRewindable* stream) {
    static const unsigned char gHeader1[] = { 'I', 'I' };
    static const unsigned char gHeader2[] = { 'M', 'M' };
    static const size_t HEADER_SIZE = sizeof(gHeader1);

    char buffer[HEADER_SIZE];
    size_t len = stream->read(buffer, HEADER_SIZE);

    if (len != HEADER_SIZE) {
        return false;   // can't read enough
    }
    if (memcmp(buffer, gHeader1, HEADER_SIZE) && memcmp(buffer, gHeader2, HEADER_SIZE)) {
        return false;
    }
    return true;
}

#include "SkTRegistry.h"

static SkImageDecoder* sk_libtiff_dfactory(SkStreamRewindable* stream) {
    if (is_tiff(stream)) {
        return new SkTIFFImageDecoder;
    }
    return NULL;
}

static SkImageDecoder_DecodeReg gReg(sk_libtiff_dfactory);

static SkImageDecoder::Format get_format_tiff(SkStreamRewindable* stream) {
    if (is_tiff(stream)) {
        return SkImageDecoder::kTIFF_Format;
    }
    return SkImageDecoder::kUnknown_Format;
}

static SkImageDecoder_FormatReg gFormatReg(get_format_tiff);

