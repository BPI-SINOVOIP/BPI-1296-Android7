/*
 * Copyright 2007 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkImageDecoder.h"
#include "SkImageEncoder.h"
#include "SkJpegUtility.h"
#include "SkColorPriv.h"
#include "SkDither.h"
#include "SkScaledBitmapSampler.h"
#include "SkStream.h"
#include "SkStreamHelpers.h"
#include "SkTemplates.h"
#include "SkTime.h"
#include "SkUtils.h"
#include "SkRect.h"
#include "SkCanvas.h"

#include "stdio.h"
#include "math.h"

#include "jpuapi.h"
#include "mixer.h"
#include "jpuhelper.h"
#include "jpuapifunc.h"

#define USE_ION_ALLOC_FB
#ifdef USE_ION_ALLOC_FB
#include "ion/ion.h"
#include <sys/mman.h>
#include <unistd.h>
#endif

#define MAX_ROT_BUF_NUM			1
#define EXTRA_FRAME_BUFFER_NUM	0

#define MAX_DECODER_INSTANCE    32

typedef struct
{
    int instNum;    // decoder instance no.
    int rotEnable;
    int rotAngle;
    int mirrorDirection;
    int StreamEndian;
    int FrameEndian;
    int sampleSize;

    //ROI
    int roiEnable;
    int roiWidth;
    int roiHeight;
    int roiOffsetX;
    int roiOffsetY;
    int roiWidthInMcu;
    int roiHeightInMcu;
    int roiOffsetXInMcu;
    int roiOffsetYInMcu;

    //packed
    PackedOutputFormat packedFormat;
    CbCrInterLeave chromaInterleave;

    int usePartialMode;
    int partialBufNum;

} HwDecConfigParam;

// These enable timing code that report milliseconds for an encoding/decoding
//#define TIME_ENCODE
//#define TIME_DECODE

// this enables our rgb->yuv code, which is faster than libjpeg on ARM
#define WE_CONVERT_TO_YUV

// If ANDROID_RGB is defined by in the jpeg headers it indicates that jpeg offers
// support for two additional formats (1) JCS_RGBA_8888 and (2) JCS_RGB_565.

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static void overwrite_mem_buffer_size(jpeg_decompress_struct* cinfo) {
#ifdef SK_BUILD_FOR_ANDROID
    /* Check if the device indicates that it has a large amount of system memory
     * if so, increase the memory allocation to 30MB instead of the default 5MB.
     */
#ifdef ANDROID_LARGE_MEMORY_DEVICE
    cinfo->mem->max_memory_to_use = 30 * 1024 * 1024;
#else
    cinfo->mem->max_memory_to_use = 5 * 1024 * 1024;
#endif
#endif // SK_BUILD_FOR_ANDROID
}

//////////////////////////////////////////////////////////////////////////
#ifdef USE_ION_ALLOC_FB
typedef struct _IONStruct_
{
    ion_user_handle_t pIonHandle;
    Uint32 nSize;
    Uint32 pIonPhyAddress;
    Uint32 pIonVirtAddress;
} IONStruct;

Int32 IonAllocateFb(int ionfd, ion_user_handle_t *ionHandle, unsigned int *phyAddr, unsigned char *virtAddress, unsigned int size)
{
    Int32 ret;
    Int32 mapfd;

    ret = ion_alloc(ionfd, size, getpagesize(), RTK_PHOENIX_ION_HEAP_MEDIA_MASK, (ION_FLAG_NONCACHED | ION_FLAG_SCPUACC | ION_FLAG_HWIPACC), ionHandle);
    if (ret < 0)
    {
        SkDebugf("In[%s][%d] fail to allocate frame buffer, size:%d\n", __func__, __LINE__, size);
        return ret;
    }
    ret = ion_phys(ionfd, *ionHandle, (unsigned long *)phyAddr, (unsigned int *)&size);
    if (ret < 0)
    {
        SkDebugf("In[%s][%d] fail to allocate frame buffer\n", __func__, __LINE__);
        return ret;
    }
    ret = ion_map(ionfd, *ionHandle, size, PROT_READ | PROT_WRITE, MAP_SHARED, 0, (unsigned char **)virtAddress, &mapfd);
    if (ret < 0)
    {
        SkDebugf("In[%s][%d] fail to allocate frame buffer\n", __func__, __LINE__);
        return ret;
    }
    close(mapfd); //we don't need to share with other process, close it
    return ret;
}
#endif

//////////////////////////////////////////////////////////////////////////

static void initialize_info(jpeg_decompress_struct* cinfo, skjpeg_source_mgr* src_mgr) {
    SkASSERT(cinfo != NULL);
    SkASSERT(src_mgr != NULL);
    jpeg_create_decompress(cinfo);
    overwrite_mem_buffer_size(cinfo);
    cinfo->src = src_mgr;
}

#ifdef SK_BUILD_FOR_ANDROID
class SkJPEGImageIndex {
public:
    SkJPEGImageIndex(SkStream* stream, SkImageDecoder* decoder)
        : fSrcMgr(stream, decoder)
        , fInfoInitialized(false)
        , fHuffmanCreated(false)
        , fDecompressStarted(false)
        {
            SkDEBUGCODE(fReadHeaderSucceeded = false;)
        }

    ~SkJPEGImageIndex() {
        if (fHuffmanCreated) {
            // Set to false before calling the libjpeg function, in case
            // the libjpeg function calls longjmp. Our setjmp handler may
            // attempt to delete this SkJPEGImageIndex, thus entering this
            // destructor again. Setting fHuffmanCreated to false first
            // prevents an infinite loop.
            fHuffmanCreated = false;
            jpeg_destroy_huffman_index(&fHuffmanIndex);
        }
        if (fDecompressStarted) {
            // Like fHuffmanCreated, set to false before calling libjpeg
            // function to prevent potential infinite loop.
            fDecompressStarted = false;
            jpeg_finish_decompress(&fCInfo);
        }
        if (fInfoInitialized) {
            this->destroyInfo();
        }
    }

    /**
     *  Destroy the cinfo struct.
     *  After this call, if a huffman index was already built, it
     *  can be used after calling initializeInfoAndReadHeader
     *  again. Must not be called after startTileDecompress except
     *  in the destructor.
     */
    void destroyInfo() {
        SkASSERT(fInfoInitialized);
        SkASSERT(!fDecompressStarted);
        // Like fHuffmanCreated, set to false before calling libjpeg
        // function to prevent potential infinite loop.
        fInfoInitialized = false;
        jpeg_destroy_decompress(&fCInfo);
        SkDEBUGCODE(fReadHeaderSucceeded = false;)
    }

    /**
     *  Initialize the cinfo struct.
     *  Calls jpeg_create_decompress, makes customizations, and
     *  finally calls jpeg_read_header. Returns true if jpeg_read_header
     *  returns JPEG_HEADER_OK.
     *  If cinfo was already initialized, destroyInfo must be called to
     *  destroy the old one. Must not be called after startTileDecompress.
     */
    bool initializeInfoAndReadHeader() {
        SkASSERT(!fInfoInitialized && !fDecompressStarted);
        initialize_info(&fCInfo, &fSrcMgr);
        fInfoInitialized = true;
        const bool success = (JPEG_HEADER_OK == jpeg_read_header(&fCInfo, true));
        SkDEBUGCODE(fReadHeaderSucceeded = success;)
        return success;
    }

    jpeg_decompress_struct* cinfo() { return &fCInfo; }

    huffman_index* huffmanIndex() { return &fHuffmanIndex; }

    /**
     *  Build the index to be used for tile based decoding.
     *  Must only be called after a successful call to
     *  initializeInfoAndReadHeader and must not be called more
     *  than once.
     */
    bool buildHuffmanIndex() {
        SkASSERT(fReadHeaderSucceeded);
        SkASSERT(!fHuffmanCreated);
        jpeg_create_huffman_index(&fCInfo, &fHuffmanIndex);
        SkASSERT(1 == fCInfo.scale_num && 1 == fCInfo.scale_denom);
        fHuffmanCreated = jpeg_build_huffman_index(&fCInfo, &fHuffmanIndex);
        return fHuffmanCreated;
    }

    /**
     *  Start tile based decoding. Must only be called after a
     *  successful call to buildHuffmanIndex, and must only be
     *  called once.
     */
    bool startTileDecompress() {
        SkASSERT(fHuffmanCreated);
        SkASSERT(fReadHeaderSucceeded);
        SkASSERT(!fDecompressStarted);
        if (jpeg_start_tile_decompress(&fCInfo)) {
            fDecompressStarted = true;
            return true;
        }
        return false;
    }

private:
    skjpeg_source_mgr  fSrcMgr;
    jpeg_decompress_struct fCInfo;
    huffman_index fHuffmanIndex;
    bool fInfoInitialized;
    bool fHuffmanCreated;
    bool fDecompressStarted;
    SkDEBUGCODE(bool fReadHeaderSucceeded;)
};
#endif

class SkJPEGImageDecoder : public SkImageDecoder {
public:
#ifdef SK_BUILD_FOR_ANDROID
    SkJPEGImageDecoder() {
        fImageIndex = NULL;
        fImageWidth = 0;
        fImageHeight = 0;

        pthread_mutex_init(&lock, NULL);
    }

    virtual ~SkJPEGImageDecoder() {
        SkDELETE(fImageIndex);

        pthread_mutex_destroy(&lock);
    }
#endif

    virtual Format getFormat() const {
        return kJPEG_Format;
    }

protected:
#ifdef SK_BUILD_FOR_ANDROID
    virtual bool onBuildTileIndex(SkStream *stream, int *width, int *height) SK_OVERRIDE;
    virtual bool onDecodeSubset(SkBitmap* bitmap, const SkIRect& rect) SK_OVERRIDE;
#endif
    virtual bool onDecode(SkStream* stream, SkBitmap* bm, Mode) SK_OVERRIDE;

private:
#ifdef SK_BUILD_FOR_ANDROID
    SkJPEGImageIndex* fImageIndex;
    int fImageWidth;
    int fImageHeight;
#endif

    pthread_mutex_t lock;
    int fDecoderInstFlag;

    /**
     *  Determine the appropriate bitmap config and out_color_space based on
     *  both the preference of the caller and the jpeg_color_space on the
     *  jpeg_decompress_struct passed in.
     *  Must be called after jpeg_read_header.
     */
    SkBitmap::Config getBitmapConfig(jpeg_decompress_struct*);
    SkBitmap::Config getBitmapConfig(JpgDecInitialInfo*);
    int getInstNum();
    int releaseInstNum(int instNum);
    int drawOnBmp(SkBitmap *bm, int destWidth, int destHeight, int x, int y, int width, int height, int planar_format, int pack_format, int interleave, unsigned char* pbImage);
    bool hwDecode(SkStream* stream, SkBitmap* bm, Mode mode, HwDecConfigParam* param);
    bool swDecode(SkStream* stream, SkBitmap* bm, Mode mode);

    typedef SkImageDecoder INHERITED;
};

//////////////////////////////////////////////////////////////////////////

/* Automatically clean up after throwing an exception */
class JPEGAutoClean {
public:
    JPEGAutoClean(): cinfo_ptr(NULL) {}
    ~JPEGAutoClean() {
        if (cinfo_ptr) {
            jpeg_destroy_decompress(cinfo_ptr);
        }
    }
    void set(jpeg_decompress_struct* info) {
        cinfo_ptr = info;
    }
private:
    jpeg_decompress_struct* cinfo_ptr;
};

///////////////////////////////////////////////////////////////////////////////

/*  If we need to better match the request, we might examine the image and
     output dimensions, and determine if the downsampling jpeg provided is
     not sufficient. If so, we can recompute a modified sampleSize value to
     make up the difference.

     To skip this additional scaling, just set sampleSize = 1; below.
 */
static int recompute_sampleSize(int sampleSize,
                                const jpeg_decompress_struct& cinfo) {
    return sampleSize * cinfo.output_width / cinfo.image_width;
}

static bool valid_output_dimensions(const jpeg_decompress_struct& cinfo) {
    /* These are initialized to 0, so if they have non-zero values, we assume
       they are "valid" (i.e. have been computed by libjpeg)
     */
    return 0 != cinfo.output_width && 0 != cinfo.output_height;
}

static bool skip_src_rows(jpeg_decompress_struct* cinfo, void* buffer, int count) {
    for (int i = 0; i < count; i++) {
        JSAMPLE* rowptr = (JSAMPLE*)buffer;
        int row_count = jpeg_read_scanlines(cinfo, &rowptr, 1);
        if (1 != row_count) {
            return false;
        }
    }
    return true;
}

#ifdef SK_BUILD_FOR_ANDROID
static bool skip_src_rows_tile(jpeg_decompress_struct* cinfo,
                               huffman_index *index, void* buffer, int count) {
    for (int i = 0; i < count; i++) {
        JSAMPLE* rowptr = (JSAMPLE*)buffer;
        int row_count = jpeg_read_tile_scanline(cinfo, index, &rowptr);
        if (1 != row_count) {
            return false;
        }
    }
    return true;
}
#endif

// This guy exists just to aid in debugging, as it allows debuggers to just
// set a break-point in one place to see all error exists.
static bool return_false(const jpeg_decompress_struct& cinfo,
                         const SkBitmap& bm, const char msg[]) {
//#ifdef SK_DEBUG
    SkDebugf("libjpeg error %d <%s> from %s [%d %d]\n", cinfo.err->msg_code,
             cinfo.err->jpeg_message_table[cinfo.err->msg_code], msg,
             bm.width(), bm.height());
//#endif
    return false;   // must always return false
}

// Convert a scanline of CMYK samples to RGBX in place. Note that this
// method moves the "scanline" pointer in its processing
static void convert_CMYK_to_RGB(uint8_t* scanline, unsigned int width) {
    // At this point we've received CMYK pixels from libjpeg. We
    // perform a crude conversion to RGB (based on the formulae
    // from easyrgb.com):
    //  CMYK -> CMY
    //    C = ( C * (1 - K) + K )      // for each CMY component
    //  CMY -> RGB
    //    R = ( 1 - C ) * 255          // for each RGB component
    // Unfortunately we are seeing inverted CMYK so all the original terms
    // are 1-. This yields:
    //  CMYK -> CMY
    //    C = ( (1-C) * (1 - (1-K) + (1-K) ) -> C = 1 - C*K
    // The conversion from CMY->RGB remains the same
    for (unsigned int x = 0; x < width; ++x, scanline += 4) {
        scanline[0] = SkMulDiv255Round(scanline[0], scanline[3]);
        scanline[1] = SkMulDiv255Round(scanline[1], scanline[3]);
        scanline[2] = SkMulDiv255Round(scanline[2], scanline[3]);
        scanline[3] = 255;
    }
}

/**
 *  Common code for setting the error manager.
 */
static void set_error_mgr(jpeg_decompress_struct* cinfo, skjpeg_error_mgr* errorManager) {
    SkASSERT(cinfo != NULL);
    SkASSERT(errorManager != NULL);
    cinfo->err = jpeg_std_error(errorManager);
    errorManager->error_exit = skjpeg_error_exit;
}

/**
 *  Common code for turning off upsampling and smoothing. Turning these
 *  off helps performance without showing noticable differences in the
 *  resulting bitmap.
 */
static void turn_off_visual_optimizations(jpeg_decompress_struct* cinfo) {
    SkASSERT(cinfo != NULL);
    /* this gives about 30% performance improvement. In theory it may
       reduce the visual quality, in practice I'm not seeing a difference
     */
    cinfo->do_fancy_upsampling = 0;

    /* this gives another few percents */
    cinfo->do_block_smoothing = 0;
}

/**
 * Common code for setting the dct method.
 */
static void set_dct_method(const SkImageDecoder& decoder, jpeg_decompress_struct* cinfo) {
    SkASSERT(cinfo != NULL);
#ifdef DCT_IFAST_SUPPORTED
    if (decoder.getPreferQualityOverSpeed()) {
        cinfo->dct_method = JDCT_ISLOW;
    } else {
        cinfo->dct_method = JDCT_IFAST;
    }
#else
    cinfo->dct_method = JDCT_ISLOW;
#endif
}
/*

static int FillSdramBurst( SkStream *stream, Uint32 targetAddr, PhysicalAddress bsBufStartAddr, PhysicalAddress bsBufEndAddr, Uint32 size,  int checkeos, int *streameos, int endian )
{
    Uint8 *pBuf;
    int room;

    int count = 0;

    if (checkeos == 1 && stream->isAtEnd())
    {
        *streameos = 1;
        return 0;
    }

    if (stream->getLength() - stream->getPosition() < size)
        count = stream->getLength() - stream->getPosition();
    else
        count = size;

    pBuf = (Uint8*)malloc(count);
    memset(pBuf, 0, count);
    stream->read(pBuf, count);
        
    if( (targetAddr+count) > bsBufEndAddr)
    {
        room = bsBufEndAddr - targetAddr;
        JpuWriteMem(targetAddr, pBuf, room, endian);
        JpuWriteMem(bsBufStartAddr, pBuf+room, (count-room), endian);
    }
    else
    {
        JpuWriteMem(targetAddr, pBuf, count, endian);
    }    

    free(pBuf);
    return count;
}

static JpgRet WriteJpgBsBufHelper(JpgDecHandle handle, SkStream *stream, PhysicalAddress paBsBufStart, PhysicalAddress paBsBufEnd, int defaultsize, int checkeos, int *pstreameos, int endian)
{
    JpgRet ret = JPG_RET_SUCCESS;
    int size = 0;
    int fillSize = 0;
    PhysicalAddress paRdPtr, paWrPtr;

    ret = JPU_DecGetBitstreamBuffer(handle, &paRdPtr, &paWrPtr, &size);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("VPU_DecGetBitstreamBuffer failed Error code is 0x%x \n", ret );
        goto FILL_BS_ERROR;
    }

    if( size <= 0 )
        return JPG_RET_SUCCESS;

    if (defaultsize)
    {
        if( size < defaultsize )
            fillSize = size;
        else
            fillSize = defaultsize;
    }
    else
    {
        fillSize = size;
    }


    fillSize = FillSdramBurst(stream, paWrPtr, paBsBufStart, paBsBufEnd, fillSize, checkeos, pstreameos, endian);

    if( *pstreameos == 0 )
    {
        ret = JPU_DecUpdateBitstreamBuffer( handle, fillSize );
        if( ret != JPG_RET_SUCCESS )
        {
            SkDebugf("VPU_DecUpdateBitstreamBuffer failed Error code is 0x%x \n", ret );
            goto FILL_BS_ERROR;
        }

        if (stream->isAtEnd())
        {
            ret = JPU_DecUpdateBitstreamBuffer(handle, STREAM_END_SIZE) ;
            if( ret != JPG_RET_SUCCESS )
            {
                SkDebugf("VPU_DecUpdateBitstreamBuffer failed Error code is 0x%x \n", ret );
                goto FILL_BS_ERROR;
            }
        }
    }
    else
    {
        if (!stream->isAtEnd())
        {
            ret = JPU_DecUpdateBitstreamBuffer(handle, STREAM_END_SIZE) ;
            if( ret != JPG_RET_SUCCESS )
            {
                SkDebugf("VPU_DecUpdateBitstreamBuffer failed Error code is -x%x \n", ret );
                goto FILL_BS_ERROR;
            }
        }
    }

FILL_BS_ERROR:

    return ret;
}
*/

SkBitmap::Config SkJPEGImageDecoder::getBitmapConfig(JpgDecInitialInfo* pInfo) {
    SkASSERT(pInfo != NULL);

    SrcDepth srcDepth = k32Bit_SrcDepth;

    if (JCS_GRAYSCALE == pInfo->sourceFormat) {
        srcDepth = k8BitGray_SrcDepth;
    }

    SkBitmap::Config config = this->getPrefConfig(srcDepth, /*hasAlpha*/ false);
    switch (config) {
        case SkBitmap::kA8_Config:
            // Only respect A8 config if the original is grayscale,
            // in which case we will treat the grayscale as alpha
            // values.
            if (JCS_GRAYSCALE != pInfo->sourceFormat) {
                config = SkBitmap::kARGB_8888_Config;
            }
            break;
        case SkBitmap::kARGB_8888_Config:
            // Fall through.
        case SkBitmap::kARGB_4444_Config:
            // Fall through.
        case SkBitmap::kRGB_565_Config:
            // These are acceptable destination configs.
            break;
        default:
            // Force all other configs to 8888.
            config = SkBitmap::kARGB_8888_Config;
            break;
    }
    return config;    
}

SkBitmap::Config SkJPEGImageDecoder::getBitmapConfig(jpeg_decompress_struct* cinfo) {
    SkASSERT(cinfo != NULL);

    SrcDepth srcDepth = k32Bit_SrcDepth;
    if (JCS_GRAYSCALE == cinfo->jpeg_color_space) {
        srcDepth = k8BitGray_SrcDepth;
    }

    SkBitmap::Config config = this->getPrefConfig(srcDepth, /*hasAlpha*/ false);
    switch (config) {
        case SkBitmap::kA8_Config:
            // Only respect A8 config if the original is grayscale,
            // in which case we will treat the grayscale as alpha
            // values.
            if (cinfo->jpeg_color_space != JCS_GRAYSCALE) {
                config = SkBitmap::kARGB_8888_Config;
            }
            break;
        case SkBitmap::kARGB_8888_Config:
            // Fall through.
        case SkBitmap::kARGB_4444_Config:
            // Fall through.
        case SkBitmap::kRGB_565_Config:
            // These are acceptable destination configs.
            break;
        default:
            // Force all other configs to 8888.
            config = SkBitmap::kARGB_8888_Config;
            break;
    }

    switch (cinfo->jpeg_color_space) {
        case JCS_CMYK:
            // Fall through.
        case JCS_YCCK:
            // libjpeg cannot convert from CMYK or YCCK to RGB - here we set up
            // so libjpeg will give us CMYK samples back and we will later
            // manually convert them to RGB
            cinfo->out_color_space = JCS_CMYK;
            break;
        case JCS_GRAYSCALE:
            if (SkBitmap::kA8_Config == config) {
                cinfo->out_color_space = JCS_GRAYSCALE;
                break;
            }
            // The data is JCS_GRAYSCALE, but the caller wants some sort of RGB
            // config. Fall through to set to the default.
        default:
            cinfo->out_color_space = JCS_RGB;
            break;
    }
    return config;
}

#ifdef ANDROID_RGB
/**
 *  Based on the config and dither mode, adjust out_color_space and
 *  dither_mode of cinfo.
 */
static void adjust_out_color_space_and_dither(jpeg_decompress_struct* cinfo,
                                              SkBitmap::Config config,
                                              const SkImageDecoder& decoder) {
    SkASSERT(cinfo != NULL);
    cinfo->dither_mode = JDITHER_NONE;
    if (JCS_CMYK == cinfo->out_color_space) {
        return;
    }
    switch(config) {
        case SkBitmap::kARGB_8888_Config:
            cinfo->out_color_space = JCS_RGBA_8888;
            break;
        case SkBitmap::kRGB_565_Config:
            cinfo->out_color_space = JCS_RGB_565;
            if (decoder.getDitherImage()) {
                cinfo->dither_mode = JDITHER_ORDERED;
            }
            break;
        default:
            break;
    }
}
#endif

bool SkJPEGImageDecoder::hwDecode(SkStream* stream, SkBitmap* bm, Mode mode, HwDecConfigParam* param)
{
    SkAutoMalloc storage;

    JpgDecHandle handle = NULL;
    JpgDecOpenParam openParam;
    JpgDecInitialInfo initialInfo;
    JpgDecOutputInfo outputInfo;
    JpgDecParam decParam;    
    BufInfo bufInfo;
    jpu_buffer_t vbStream;
    jpu_buffer_t dispBuffer;

    memset(&openParam, 0x0, sizeof(JpgDecOpenParam));
    memset(&initialInfo, 0x0, sizeof(JpgDecInitialInfo));
    memset(&outputInfo, 0x0, sizeof(JpgDecOutputInfo));
    memset(&decParam, 0x0, sizeof(JpgDecParam));
    memset(&vbStream, 0x0, sizeof(jpu_buffer_t));
    memset(&dispBuffer, 0x0, sizeof(jpu_buffer_t));
    memset(&bufInfo, 0x0, sizeof(BufInfo));

#ifdef USE_ION_ALLOC_FB
    IONStruct pIonFb[MAX_FRAME];
    IONStruct pIonDispFrame;
    jpu_buffer_t vbUserFb;
    Int32 mIonFd = ion_open();
    Int32 Ret;
    
    memset(pIonFb, 0x0, sizeof(IONStruct)*MAX_FRAME);
    memset(&pIonDispFrame, 0x0, sizeof(IONStruct));
#endif

    FrameBuffer frameBuf[MAX_FRAME];
    FRAME_BUF *pFrame[MAX_FRAME];
    memset(pFrame, 0x00, sizeof(FRAME_BUF *)*MAX_FRAME);
    memset(frameBuf, 0x0, sizeof(FrameBuffer)*MAX_FRAME);

    JpgRet ret = JPG_RET_SUCCESS;
    bool bSuccess = true;
    Uint32 framebufSize=0, framebufWidth=0, framebufHeight=0, framebufStride=0, framebufFormat=FORMAT_420;
    int imgWidth = 0, imgHeight = 0;
    int dispWidth = 0, dispHeight = 0;
    int frameIdx = 0, ppIdx = 0, saveIdx = 0, totalNumofErrMbs = 0, streameos = 0;
    Uint8 *pYuv  =  NULL;
    int needFrameBufCount = 0, regFrameBufCount = 0;
    int partPosIdx = 0, partBufIdx = 0;
    int partMaxIdx = 0;
    int partialHeight   = 0;
    int int_reason      = 0;


    /******** JPU config start*******/
    int instIdx         = param->instNum;
    // Partial mode
    int usePartialMode  = param->usePartialMode;
    int partialBufNum   = param->partialBufNum;
    // ROI enable
    int roiEnable       = param->roiEnable;
    // Rotate
    int rotEnable       = param->rotEnable;
    int rotAngle        = param->rotAngle;
    // Mirror
    int mirrorDirection = param->mirrorDirection;
    // Scale    
    int scaleMode       = (param->sampleSize > 0) ? log2(param->sampleSize): 0;
    /******** JPU config end*******/ 
    SkBitmap::Config config;     

    SkDebugf("********Start HW Decode***************");   
    const size_t length = CopyStreamToStorage(&storage, stream);
    if (length == 0)
        return false;

    bufInfo.buf = (unsigned char*)storage.get();
    bufInfo.size = length;
    bufInfo.point = 0;
   
    ret = JPU_Init();
    if (ret != JPG_RET_SUCCESS && ret != JPG_RET_CALLED_BEFORE)
    {
        SkDebugf("JPU_Init failed Error code is 0x%x \n", ret);
        bSuccess = false;
        goto ERR_DEC_INIT;
    }

    vbStream.size = STREAM_BUF_SIZE;
    if (jdi_allocate_dma_memory(&vbStream) < 0)
    {
        SkDebugf("fail to allocate bitstream buffer\n");
        bSuccess = false;
        goto ERR_DEC_INIT;
    }
    
    /* Set JPU open parameters */
    openParam.streamEndian = param->StreamEndian;
    openParam.frameEndian = param->FrameEndian;   
    openParam.bitstreamBuffer = vbStream.phys_addr;
    openParam.bitstreamBufferSize = vbStream.size;
    openParam.pBitStream = (BYTE *)vbStream.virt_addr; // set virtual address mapped of physical address
    openParam.packedFormat = param->packedFormat;
    openParam.chromaInterleave = param->chromaInterleave;
    openParam.roiEnable = roiEnable;
    openParam.roiOffsetX = param->roiOffsetX;
    openParam.roiOffsetY = param->roiOffsetY;
    openParam.roiWidth = param->roiWidth;
    openParam.roiHeight = param->roiHeight; 

    ret = JPU_DecOpen(&handle, &openParam);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("JPU_DecOpen failed Error code is 0x%x \n", ret );
        bSuccess = false;
        goto ERR_DEC_INIT;
    }

    ret = WriteJpgBsBufHelper(handle, &bufInfo, openParam.bitstreamBuffer, openParam.bitstreamBuffer+openParam.bitstreamBufferSize, 0, 0, &streameos, openParam.streamEndian);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("WriteBsBufHelper failed Error code is 0x%x \n", ret );
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }

    ret = JPU_DecGetInitialInfo(handle, &initialInfo);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("JPU_DecGetInitialInfo failed Error code is 0x%x, inst=%d \n", ret, instIdx);
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }    

    config = this->getBitmapConfig(&initialInfo);
            
    /*======================================================================*/
    /*          Start debug JPEG raw data                                   */
    /*======================================================================*/
    if(usePartialMode)
    {
        // disable Rotator, Scaler
        rotEnable = 0;
        scaleMode = 0;
        partialHeight = (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_224) ? 16 : 8;

        partMaxIdx  = ((initialInfo.picHeight +15)&~15) / partialHeight;
        if(partMaxIdx < partialBufNum)
            partialBufNum = partMaxIdx;
    }
    
    if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_422)
        framebufWidth = ((initialInfo.picWidth+15)/16)*16;
    else
        framebufWidth = ((initialInfo.picWidth+7)/8)*8;

    if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_224)
        framebufHeight = ((initialInfo.picHeight+15)/16)*16;
    else
        framebufHeight = ((initialInfo.picHeight+7)/8)*8;

    if(roiEnable)
    {
        framebufWidth  = initialInfo.roiFrameWidth ;
        framebufHeight = initialInfo.roiFrameHeight;
    }

    // scaler constraint when conformance test is disable
    if (framebufWidth < 128 || framebufHeight < 128)
    {
        scaleMode = 0;        
    }
    
    framebufFormat = initialInfo.sourceFormat;
    
    framebufWidth  >>= scaleMode;
    framebufHeight >>= scaleMode;
    if (scaleMode)
    {
        framebufHeight = ((framebufHeight+1)/2)*2;
        framebufWidth = ((framebufWidth+1)/2)*2;
    }

    dispWidth = (rotAngle == 90 || rotAngle == 270) ? framebufHeight : framebufWidth;
    dispHeight = (rotAngle == 90 || rotAngle == 270) ? framebufWidth : framebufHeight;
    imgWidth = initialInfo.picWidth >> scaleMode;
    imgHeight = initialInfo.picHeight >> scaleMode;

    if (rotAngle == 90 || rotAngle == 270)
    {
        framebufStride = framebufHeight;
        framebufHeight = framebufWidth;
        framebufFormat = (framebufFormat == FORMAT_422)?FORMAT_224 : (framebufFormat==FORMAT_224) ? FORMAT_422 : framebufFormat;
    }
    else
    {
        framebufStride = framebufWidth;
    }

    if (scaleMode)
        framebufStride = ((framebufStride+15)/16)*16;

    if(openParam.packedFormat) {
        framebufStride = framebufStride*3;
        framebufFormat = FORMAT_444;
    }

    if (SkImageDecoder::kDecodeBounds_Mode == mode) {
        SkDebugf("Only decode bounds, width=%d, height=%d\n", imgWidth, imgHeight);
        // It only requires width and height
        bm->setConfig(config, imgWidth, imgHeight);
        //bm->setIsOpaque(config != SkBitmap::kA8_Config);

        bSuccess = true;
        goto ERR_DEC_OPEN;
    }    
    
    framebufSize = GetFrameBufSize(framebufFormat, framebufStride, framebufHeight);

    //Allocate frame buffer
    regFrameBufCount = initialInfo.minFrameBufferCount + EXTRA_FRAME_BUFFER_NUM;
    if(usePartialMode)
    {
        if(partialBufNum > 4)
            partialBufNum = 4;

        regFrameBufCount *= partialBufNum;
    }

    needFrameBufCount = regFrameBufCount;

#ifdef USE_ION_ALLOC_FB
    Uint32 allocBufSize, lumaSize, chrSize;
    Uint32 allocHeight;
    Int32 divX, divY;
    divX = framebufFormat == FORMAT_420 || framebufFormat == FORMAT_422 ? 2 : 1;
    divY = framebufFormat == FORMAT_420 || framebufFormat == FORMAT_224 ? 2 : 1;
    if (usePartialMode)
        allocHeight = partialHeight;
    else
        allocHeight = framebufHeight;
    allocBufSize = GetFrameBufSize(framebufFormat, framebufStride, allocHeight);
    lumaSize = ((framebufStride+1)/2*2)*((allocHeight+1)/2*2);
    chrSize = lumaSize/divX/divY;

    for (int i = 0; i < needFrameBufCount; i++)
    {
        pIonFb[i].nSize = allocBufSize;
        Ret = IonAllocateFb(mIonFd, (ion_user_handle_t *)&pIonFb[i].pIonHandle, (unsigned int *)&pIonFb[i].pIonPhyAddress, (unsigned char *)&pIonFb[i].pIonVirtAddress, pIonFb[i].nSize);
        if (Ret < 0)
        {
            SkDebugf("In[%s][%d] fail to allocate frame buffer\n", __func__, __LINE__);
            goto ERR_DEC_OPEN;
        }
        frameBuf[i].bufY = pIonFb[i].pIonPhyAddress;
        frameBuf[i].bufCb = frameBuf[i].bufY + lumaSize;
        if (openParam.chromaInterleave == CBCR_SEPARATED)
            frameBuf[i].bufCr = frameBuf[i].bufCb + chrSize;
        vbUserFb.phys_addr = pIonFb[i].pIonPhyAddress;
        vbUserFb.virt_addr = pIonFb[i].pIonVirtAddress;
        vbUserFb.size = pIonFb[i].nSize;
        if (jdi_attach_dma_memory(&vbUserFb) < 0)
        {
            SkDebugf("fail to attach frame buffer\n" );
            goto ERR_DEC_OPEN;
        }
    }
#else    
    if(usePartialMode)
        AllocateFrameBuffer(instIdx, framebufFormat, framebufStride, partialHeight, needFrameBufCount, 0);
    else
        AllocateFrameBuffer(instIdx, framebufFormat, framebufStride, framebufHeight, needFrameBufCount, 0);
    
    JpgEnterLock();
    for(int i = 0; i < needFrameBufCount; ++i )
    {
        pFrame[i] = GetFrameBuffer(instIdx, i);
        frameBuf[i].bufY = pFrame[i]->vbY.phys_addr;
        frameBuf[i].bufCb = pFrame[i]->vbCb.phys_addr;    
        if (openParam.chromaInterleave == CBCR_SEPARATED)
            frameBuf[i].bufCr = pFrame[i]->vbCr.phys_addr;
        ClearFrameBuffer(instIdx, i);
    }
    JpgLeaveLock();
#endif // USE_ION_ALLOC_FB

    // should we allow the Chooser (if present) to pick a config for us???
    if (!this->chooseFromOneChoice(config, dispWidth, dispHeight)) {
        SkDebugf("Fail to do choice\n");
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }

    bm->setConfig(config, imgWidth, imgHeight);
    //bm->setIsOpaque(config != SkBitmap::kA8_Config);
    
    if (!this->allocPixelRef(bm, NULL)) {
        SkDebugf("Fail to allocate Pixel reference\n");
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }

#ifdef USE_ION_ALLOC_FB
    pIonDispFrame.nSize = framebufSize;
    IonAllocateFb(mIonFd, (ion_user_handle_t *)&pIonDispFrame.pIonHandle, (unsigned int *)&pIonDispFrame.pIonPhyAddress, (unsigned char *)&pIonDispFrame.pIonVirtAddress, pIonDispFrame.nSize);
    pYuv = (Uint8 *)pIonDispFrame.pIonVirtAddress;
#else
    dispBuffer.size = framebufSize;
    jdi_allocate_dma_memory(&dispBuffer);
    pYuv = (Uint8 *)dispBuffer.virt_addr;
#endif // USE_ION_ALLOC_FB

    if (!pYuv)
    {
        SkDebugf("Fail to allocation memory for display buffer\n");
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }
    memset(pYuv, 0x0, sizeof(Uint8) * framebufSize);

    ret = JPU_DecGiveCommand(handle, SET_JPG_USE_PARTIAL_MODE,  &usePartialMode);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("JPU_DecGiveCommand[SET_JPG_USE_PARTIAL_MODE] failed Error code is 0x%x \n", ret );
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }
    ret = JPU_DecGiveCommand(handle, SET_JPG_PARTIAL_FRAME_NUM, &partialBufNum);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("JPU_DecGiveCommand[SET_JPG_PARTIAL_FRAME_NUM] failed Error code is 0x%x \n", ret );
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }
    ret = JPU_DecGiveCommand(handle, SET_JPG_PARTIAL_LINE_NUM,  &(partialHeight));
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("JPU_DecGiveCommand[SET_JPG_PARTIAL_LINE_NUM] failed Error code is 0x%x \n", ret );
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }

    // Register frame buffers requested by the decoder.
    ret = JPU_DecRegisterFrameBuffer(handle, frameBuf, regFrameBufCount, framebufStride);
    if( ret != JPG_RET_SUCCESS )
    {
        SkDebugf("JPU_DecRegisterFrameBuffer failed Error code is 0x%x \n", ret );
        bSuccess = false;
        goto ERR_DEC_OPEN;
    }
    ppIdx = 0;

    while(1) {

        if (rotEnable)
        {
            JPU_DecGiveCommand(handle, SET_JPG_ROTATION_ANGLE, &rotAngle);
            JPU_DecGiveCommand(handle, SET_JPG_MIRROR_DIRECTION, &mirrorDirection);
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_OUTPUT, &frameBuf[ppIdx] );
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_STRIDE, &framebufStride);

            JPU_DecGiveCommand(handle, ENABLE_JPG_ROTATION, 0);
            JPU_DecGiveCommand(handle, ENABLE_JPG_MIRRORING, 0);
        }

        JPU_DecGiveCommand(handle, SET_JPG_SCALE_HOR,  &scaleMode);
        JPU_DecGiveCommand(handle, SET_JPG_SCALE_VER,  &scaleMode);

        if(usePartialMode)
        {
            partPosIdx = 0;
            partBufIdx = 0;
            outputInfo.decodingSuccess = 0;
            JPU_SWReset();
        }
        // Start decoding a frame.
        ret = JPU_DecStartOneFrame(handle, &decParam);
        if (ret != JPG_RET_SUCCESS && ret != JPG_RET_EOS)
        {
            if (ret == JPG_RET_BIT_EMPTY)
            {
                ret = WriteJpgBsBufHelper(handle, &bufInfo, openParam.bitstreamBuffer, openParam.bitstreamBuffer+openParam.bitstreamBufferSize, STREAM_FILL_SIZE, 0, &streameos, openParam.streamEndian);
                if( ret != JPG_RET_SUCCESS )
                {
                    SkDebugf("WriteBsBufHelper failed Error code is 0x%x \n", ret );
                    bSuccess = false;
                    goto ERR_DEC_OPEN;
                }
                continue;
            }

            SkDebugf("JPU_DecStartOneFrame failed Error code is 0x%x \n", ret );
            bSuccess = false;
            goto ERR_DEC_OPEN;
        }
        if (ret == JPG_RET_EOS) {            
            goto JPU_END_OF_STREAM;
        }

        while(1)
        {
            int_reason = JPU_WaitInterrupt(JPU_INTERRUPT_TIMEOUT_MS);
            if (int_reason == -1)
            {
                SkDebugf("Error : timeout happened\n");
                //JPU_SWReset(); Ryan: turnoff this, which causes deadlock in multi-thread environment
                SkDebugf("Error : timeout happened done\n");
                break;
            }
            if( usePartialMode && (int_reason & 0xf0))
            {
                partBufIdx = ((partPosIdx) % partialBufNum);

                if ((1<<partBufIdx) & ((int_reason & 0xf0)>>4))
                {
                    SkDebugf("DECODED : PARTIAL BUFFER IDX %d / POS %d / MAX POS %d / INT_REASON=0x%x\n", partBufIdx, partPosIdx+1, partMaxIdx, int_reason);

                    saveIdx = partBufIdx;
                    if (!SaveYuvPartialImageHelperFormat(NULL, pYuv,
                                                         frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                                         dispWidth, dispHeight, partialHeight, framebufStride, openParam.chromaInterleave, framebufFormat, openParam.frameEndian, partPosIdx, frameIdx,
                                                         openParam.packedFormat))
                        goto ERR_DEC_OPEN;
                    drawOnBmp(bm, dispWidth, partialHeight, 0, 0, dispWidth, partialHeight, framebufFormat, openParam.packedFormat, openParam.chromaInterleave, pYuv);
                    
                    partPosIdx++;
                    JPU_ClrStatus((1<< (INT_JPU_PARIAL_BUF0_EMPTY + partBufIdx)));

                    continue;
                }
                else
                {
                    SkDebugf("Invalid partial interrupt : expected reason =0x%x, actual reason=0x%x \n", (1<<partBufIdx), ((int_reason & 0xF0)>>4));
                    bSuccess = false;
                    goto ERR_DEC_OPEN;
                }
            }
            if (int_reason & (1<<INT_JPU_DONE) || int_reason & (1<<INT_JPU_ERROR))  // Must catch PIC_DONE interrupt before catching EMPTY interrupt
            {
                // Do no clear INT_JPU_DONE and INT_JPU_ERROR interrupt. these will be cleared in JPU_DecGetOutputInfo.
                break;
            }

            if (int_reason & (1<<INT_JPU_BIT_BUF_EMPTY))
            {
                ret = WriteJpgBsBufHelper(handle, &bufInfo, openParam.bitstreamBuffer, openParam.bitstreamBuffer+openParam.bitstreamBufferSize, STREAM_FILL_SIZE, 0, &streameos, openParam.streamEndian);
                if( ret != JPG_RET_SUCCESS )
                {
                    SkDebugf("WriteBsBufHelper failed Error code is 0x%x \n", ret );
                    bSuccess = false;
                    goto ERR_DEC_OPEN;
                }
                JPU_ClrStatus((1<<INT_JPU_BIT_BUF_EMPTY));
            }

            if (int_reason & (1<<INT_JPU_BIT_BUF_STOP))
            {
                ret = JPU_DecCompleteStop(handle);
                if( ret != JPG_RET_SUCCESS )
                {
                    SkDebugf("JPU_DecCompleteStop failed Error code is 0x%x \n", ret );
                    bSuccess = false;
                    goto ERR_DEC_OPEN;
                }
                JPU_ClrStatus((1<<INT_JPU_BIT_BUF_STOP));
                break;
            }

            if (int_reason & (1<<INT_JPU_PARIAL_OVERFLOW))
                JPU_ClrStatus((1<<INT_JPU_PARIAL_OVERFLOW));
        }

JPU_END_OF_STREAM:
        ret = JPU_DecGetOutputInfo(handle, &outputInfo);
        if (ret != JPG_RET_SUCCESS)
        {
            SkDebugf("JPU_DecGetOutputInfo failed Error code is 0x%x \n", ret );
            bSuccess = false;
            goto ERR_DEC_OPEN;
        }

#ifdef MJPEG_ERROR_CONCEAL
        if(outputInfo.numOfErrMBs)
        {
            int errRstIdx, errPosX, errPosY;
            errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
            errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
            errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
            SkDebugf("Error restart Idx : %d, MCU x:%d, y:%d, in Frame : %d \n", errRstIdx, errPosX, errPosY, frameIdx);
            continue;
        }
#endif

        if (outputInfo.decodingSuccess == 0 ) {
            SkDebugf("JPU_DecGetOutputInfo decode fail framdIdx %d \n", frameIdx);            
            bSuccess = false;
        }

        if (outputInfo.indexFrameDisplay == -1) {
            SkDebugf("No other display frame \n");
            break;
        }

        // YUV Dump Done when partial buffer is all displayed.
        int_reason = JPU_GetStatus();
        if(usePartialMode && !(int_reason & 0xF0))
            goto SKIP_BUF_DUMP;
        
        // indexFrameDisplay points to the frame buffer, among ones registered, which holds
        // the output of the decoder.
        if (!rotEnable) {
            saveIdx = outputInfo.indexFrameDisplay;
            if (!SaveYuvImageHelperFormat(NULL, pYuv,
                                          frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                          dispWidth, dispHeight, framebufStride,
                                          openParam.chromaInterleave, framebufFormat, openParam.frameEndian,
                                          openParam.packedFormat))
                goto ERR_DEC_OPEN;            
        }
        else
        {
            saveIdx = ppIdx;
            if (!SaveYuvImageHelperFormat(NULL, pYuv,
                                          frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                          dispWidth, dispHeight,
                                          framebufStride, openParam.chromaInterleave, framebufFormat, openParam.frameEndian,
                                          openParam.packedFormat))
                goto ERR_DEC_OPEN;
            ppIdx = (ppIdx-regFrameBufCount+1)%MAX_ROT_BUF_NUM;
        }
        drawOnBmp(bm, imgWidth, imgHeight, 0, 0, dispWidth, dispHeight, framebufFormat, openParam.packedFormat, openParam.chromaInterleave, pYuv);
        
SKIP_BUF_DUMP: 
        if( outputInfo.numOfErrMBs )
        {
            int errRstIdx, errPosX, errPosY;
            errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
            errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
            errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
            SkDebugf("Error restart Idx : %d, MCU x:%d, y:%d, in Frame : %d \n", errRstIdx, errPosX, errPosY, frameIdx);
        }    
        frameIdx++;
        break;  /* since we only need the first frame */
    }
    
    SkDebugf("***********HW Dec End : Total Frame %d, bSuccess: %d ************ \n", frameIdx, bSuccess);  
    if (totalNumofErrMbs)
    {
        bSuccess = false;
        SkDebugf("Total Num of Error MBs : %d\n", totalNumofErrMbs);
    }
    
ERR_DEC_OPEN:
    // Now that we are done with decoding, close the open instance.
    ret = JPU_DecClose(handle);
   
ERR_DEC_INIT:
    jdi_free_dma_memory(&vbStream);

#ifndef USE_ION_ALLOC_FB
    if (pYuv)
        jdi_free_dma_memory(&dispBuffer);
#endif    

    if (storage.get()) {
        storage.free();
    }

#ifdef USE_ION_ALLOC_FB
    for (int i=0; i < needFrameBufCount; i++)
    {
        if (pIonFb[i].nSize)
        {
            vbUserFb.phys_addr = pIonFb[i].pIonPhyAddress;
            vbUserFb.size = pIonFb[i].nSize;
            jdi_dettach_dma_memory(&vbUserFb);
            munmap((void *)pIonFb[i].pIonVirtAddress, pIonFb[i].nSize);
            ion_free(mIonFd, pIonFb[i].pIonHandle);
        }
    }

    if (pIonDispFrame.nSize)
    {
        munmap((void *)pIonDispFrame.pIonVirtAddress, pIonDispFrame.nSize);
        ion_free(mIonFd, pIonDispFrame.pIonHandle);
    }
    ion_close(mIonFd);
#else
    FreeFrameBuffer(instIdx);
#endif
    
    JPU_DeInit();    

    return bSuccess;
}

bool SkJPEGImageDecoder::swDecode(SkStream* stream, SkBitmap* bm, Mode mode) {
    JPEGAutoClean autoClean;

    jpeg_decompress_struct  cinfo;
    skjpeg_source_mgr       srcManager(stream, this);

    skjpeg_error_mgr errorManager;
    set_error_mgr(&cinfo, &errorManager);

    // All objects need to be instantiated before this setjmp call so that
    // they will be cleaned up properly if an error occurs.
    if (setjmp(errorManager.fJmpBuf)) {
        return return_false(cinfo, *bm, "setjmp");
    }

    initialize_info(&cinfo, &srcManager);
    autoClean.set(&cinfo);

    int status = jpeg_read_header(&cinfo, true);
    if (status != JPEG_HEADER_OK) {
        return return_false(cinfo, *bm, "read_header");
    }

    /*  Try to fulfill the requested sampleSize. Since jpeg can do it (when it
        can) much faster that we, just use their num/denom api to approximate
        the size.
    */
    int sampleSize = this->getSampleSize();

    set_dct_method(*this, &cinfo);

    SkASSERT(1 == cinfo.scale_num);
    cinfo.scale_denom = sampleSize;

    turn_off_visual_optimizations(&cinfo);

    const SkBitmap::Config config = this->getBitmapConfig(&cinfo);

#ifdef ANDROID_RGB
    adjust_out_color_space_and_dither(&cinfo, config, *this);
#endif

    if (1 == sampleSize && SkImageDecoder::kDecodeBounds_Mode == mode) {
        bm->setConfig(config, cinfo.image_width, cinfo.image_height);
        //bm->setIsOpaque(config != SkBitmap::kA8_Config);
        return true;
    }

    /*  image_width and image_height are the original dimensions, available
        after jpeg_read_header(). To see the scaled dimensions, we have to call
        jpeg_start_decompress(), and then read output_width and output_height.
    */
    if (!jpeg_start_decompress(&cinfo)) {
        /*  If we failed here, we may still have enough information to return
            to the caller if they just wanted (subsampled bounds). If sampleSize
            was 1, then we would have already returned. Thus we just check if
            we're in kDecodeBounds_Mode, and that we have valid output sizes.

            One reason to fail here is that we have insufficient stream data
            to complete the setup. However, output dimensions seem to get
            computed very early, which is why this special check can pay off.
         */
        if (SkImageDecoder::kDecodeBounds_Mode == mode && valid_output_dimensions(cinfo)) {
            SkScaledBitmapSampler smpl(cinfo.output_width, cinfo.output_height,
                                       recompute_sampleSize(sampleSize, cinfo));
            bm->setConfig(config, smpl.scaledWidth(), smpl.scaledHeight());
            //bm->setIsOpaque(config != SkBitmap::kA8_Config);
            return true;
        } else {
            return return_false(cinfo, *bm, "start_decompress");
        }
    }
    sampleSize = recompute_sampleSize(sampleSize, cinfo);

    // should we allow the Chooser (if present) to pick a config for us???
    if (!this->chooseFromOneChoice(config, cinfo.output_width, cinfo.output_height)) {
        return return_false(cinfo, *bm, "chooseFromOneChoice");
    }

    SkScaledBitmapSampler sampler(cinfo.output_width, cinfo.output_height, sampleSize);
    bm->setConfig(config, sampler.scaledWidth(), sampler.scaledHeight());
    //bm->setIsOpaque(config != SkBitmap::kA8_Config);
    if (SkImageDecoder::kDecodeBounds_Mode == mode) {
        return true;
    }
    if (!this->allocPixelRef(bm, NULL)) {
        return return_false(cinfo, *bm, "allocPixelRef");
    }

    SkAutoLockPixels alp(*bm);

#ifdef ANDROID_RGB
    /* short-circuit the SkScaledBitmapSampler when possible, as this gives
       a significant performance boost.
    */
    if (sampleSize == 1 &&
        ((config == SkBitmap::kARGB_8888_Config &&
                cinfo.out_color_space == JCS_RGBA_8888) ||
        (config == SkBitmap::kRGB_565_Config &&
                cinfo.out_color_space == JCS_RGB_565)))
    {
        JSAMPLE* rowptr = (JSAMPLE*)bm->getPixels();
        INT32 const bpr =  bm->rowBytes();

        while (cinfo.output_scanline < cinfo.output_height) {
            int row_count = jpeg_read_scanlines(&cinfo, &rowptr, 1);
            // if row_count == 0, then we didn't get a scanline, so abort.
            // if we supported partial images, we might return true in this case
            if (0 == row_count) {
                //return return_false(cinfo, *bm, "read_scanlines");
                memset(rowptr, 0, bpr);
                break;
            }
            if (this->shouldCancelDecode()) {
                return return_false(cinfo, *bm, "shouldCancelDecode");
            }
            rowptr += bpr;
        }
        jpeg_finish_decompress(&cinfo);
        return true;
    }
#endif

    // check for supported formats
    SkScaledBitmapSampler::SrcConfig sc;
    if (JCS_CMYK == cinfo.out_color_space) {
        // In this case we will manually convert the CMYK values to RGB
        sc = SkScaledBitmapSampler::kRGBX;
    } else if (3 == cinfo.out_color_components && JCS_RGB == cinfo.out_color_space) {
        sc = SkScaledBitmapSampler::kRGB;
#ifdef ANDROID_RGB
    } else if (JCS_RGBA_8888 == cinfo.out_color_space) {
        sc = SkScaledBitmapSampler::kRGBX;
    } else if (JCS_RGB_565 == cinfo.out_color_space) {
        sc = SkScaledBitmapSampler::kRGB_565;
#endif
    } else if (1 == cinfo.out_color_components &&
               JCS_GRAYSCALE == cinfo.out_color_space) {
        sc = SkScaledBitmapSampler::kGray;
    } else {
        return return_false(cinfo, *bm, "jpeg colorspace");
    }

    if (!sampler.begin(bm, sc, *this)) {
        return return_false(cinfo, *bm, "sampler.begin");
    }

    // The CMYK work-around relies on 4 components per pixel here
    SkAutoMalloc srcStorage(cinfo.output_width * 4);
    uint8_t* srcRow = (uint8_t*)srcStorage.get();

    //  Possibly skip initial rows [sampler.srcY0]
    if (!skip_src_rows(&cinfo, srcRow, sampler.srcY0())) {
        return return_false(cinfo, *bm, "skip rows");
    }

    // now loop through scanlines until y == bm->height() - 1
    for (int y = 0;; y++) {
        JSAMPLE* rowptr = (JSAMPLE*)srcRow;
        int row_count = jpeg_read_scanlines(&cinfo, &rowptr, 1);
        if (0 == row_count) {
            return return_false(cinfo, *bm, "read_scanlines");
        }
        if (this->shouldCancelDecode()) {
            return return_false(cinfo, *bm, "shouldCancelDecode");
        }

        if (JCS_CMYK == cinfo.out_color_space) {
            convert_CMYK_to_RGB(srcRow, cinfo.output_width);
        }

        sampler.next(srcRow);
        if (bm->height() - 1 == y) {
            // we're done
            break;
        }

        if (!skip_src_rows(&cinfo, srcRow, sampler.srcDY() - 1)) {
            return return_false(cinfo, *bm, "skip rows");
        }
    }

    // we formally skip the rest, so we don't get a complaint from libjpeg
    if (!skip_src_rows(&cinfo, srcRow,
                       cinfo.output_height - cinfo.output_scanline)) {
        return return_false(cinfo, *bm, "skip rows");
    }
    jpeg_finish_decompress(&cinfo);

    return true;
}

bool SkJPEGImageDecoder::onDecode(SkStream* stream, SkBitmap* bm, Mode mode) {
#ifdef TIME_DECODE
    SkAutoTime atm("JPEG Decode");
#endif

    int sampleSize = this->getSampleSize();

    if (mode == kDecodeBounds_Mode && sampleSize == 1) {
        return swDecode(stream, bm, mode);
    }

    bool bSuccess = false;
    /******** JPU config start*******/
    HwDecConfigParam param;
    memset(&param, 0, sizeof(HwDecConfigParam));

    param.instNum = getInstNum();
    param.rotEnable = 0;
    param.rotAngle = 0;
    param.mirrorDirection = 0;
    param.StreamEndian = JPU_STREAM_ENDIAN;
    param.FrameEndian = JPU_FRAME_ENDIAN;
    param.sampleSize = this->getSampleSize();
    param.roiEnable = 0;
    param.packedFormat = PACKED_FORMAT_NONE;
    param.chromaInterleave = CBCR_SEPARATED;
    param.usePartialMode = 0;
    param.partialBufNum = 0;
    /******** JPU config end*******/

    bSuccess = hwDecode(stream, bm, mode, &param);
    releaseInstNum(param.instNum);
    
    if (bSuccess == false)
    {
        SkDebugf("HW decode fail! Try SW decode\n");
        if (stream->rewind() == false) {
            SkDebugf("Fail to rewind!\n");
            return false;
        }
        
        if (!swDecode(stream, bm, mode)) {
            SkDebugf("SW decode fail!\n");
            return false;
        }
        return true;
    }
    
    return true;
}

int SkJPEGImageDecoder::drawOnBmp(SkBitmap *bm, int destWidth, int destHeight, int x, int y, int width, int height, int planar_format, int pack_format, int interleave, unsigned char* pbImage)
{
    SkAutoLockPixels alp(*bm);

    SkDebugf("drawOnBmp: src ==> x=%d, y=%d, width=%d, height=%d\n", x, y, width, height);
    SkDebugf("drawOnBmp: dest ==> width=%d, height=%d\n", destWidth, destHeight);
    SkDebugf("drawOnBmp: planar_format=%d, pack_format=%d, interleave=%d\n", planar_format, pack_format, interleave);

    unsigned char* src_ptr = pbImage;
    unsigned char* dest_ptr = (unsigned char*) bm->getPixels();

    rgb_color_format rgbFormat;
    switch(bm->getConfig())
    {    
    case SkBitmap::kRGB_565_Config:
        rgbFormat = RGB565;
        break;
    case SkBitmap::kARGB_4444_Config:
        rgbFormat = ARGB4444;
        break;
    case SkBitmap::kARGB_8888_Config:
    default:
        rgbFormat = ARGB8888;
    }

    SkDebugf("drawOnBmp: rgbFormat = %d", rgbFormat);
    
    jpu_yuv2rgb(width, height, 
                (yuv2rgb_color_format)convert_jpuapi_format_to_yuv2rgb_color_format(planar_format, pack_format, interleave), src_ptr, 
                destWidth, destHeight, rgbFormat, dest_ptr, 0);

    return 1;
}

int SkJPEGImageDecoder::getInstNum() {
    int i;
    
    pthread_mutex_lock(&lock);
    for(i = 0; i < MAX_DECODER_INSTANCE; i++) {
        if ((fDecoderInstFlag & (1<<i)) == 0) {
            fDecoderInstFlag |= (1<<i);
            break;
        }
    }
    pthread_mutex_unlock(&lock);
    
    return (i >= MAX_DECODER_INSTANCE)? -1:i;
}

int SkJPEGImageDecoder::releaseInstNum(int instNum) {

    pthread_mutex_lock(&lock);
    fDecoderInstFlag &= ~(1<<instNum);
    pthread_mutex_unlock(&lock);
    
    return fDecoderInstFlag;
}


#ifdef SK_BUILD_FOR_ANDROID
bool SkJPEGImageDecoder::onBuildTileIndex(SkStream* stream, int *width, int *height) {

    SkAutoTDelete<SkJPEGImageIndex> imageIndex(SkNEW_ARGS(SkJPEGImageIndex, (stream, this)));
    jpeg_decompress_struct* cinfo = imageIndex->cinfo();

    skjpeg_error_mgr sk_err;
    set_error_mgr(cinfo, &sk_err);

    // All objects need to be instantiated before this setjmp call so that
    // they will be cleaned up properly if an error occurs.
    if (setjmp(sk_err.fJmpBuf)) {
        return false;
    }

    // create the cinfo used to create/build the huffmanIndex
    if (!imageIndex->initializeInfoAndReadHeader()) {
        return false;
    }

    if (!imageIndex->buildHuffmanIndex()) {
        return false;
    }

    // destroy the cinfo used to create/build the huffman index
    imageIndex->destroyInfo();

    // Init decoder to image decode mode
    if (!imageIndex->initializeInfoAndReadHeader()) {
        return false;
    }

    // FIXME: This sets cinfo->out_color_space, which we may change later
    // based on the config in onDecodeSubset. This should be fine, since
    // jpeg_init_read_tile_scanline will check out_color_space again after
    // that change (when it calls jinit_color_deconverter).
    (void) this->getBitmapConfig(cinfo);

    turn_off_visual_optimizations(cinfo);

    // instead of jpeg_start_decompress() we start a tiled decompress
    if (!imageIndex->startTileDecompress()) {
        return false;
    }

    SkASSERT(1 == cinfo->scale_num);
    fImageWidth = cinfo->output_width;
    fImageHeight = cinfo->output_height;

    if (width) {
        *width = fImageWidth;
    }
    if (height) {
        *height = fImageHeight;
    }

    SkDELETE(fImageIndex);
    fImageIndex = imageIndex.detach();

    return true;
}

bool SkJPEGImageDecoder::onDecodeSubset(SkBitmap* bm, const SkIRect& region) {
    if (NULL == fImageIndex) {
        return false;
    }
    jpeg_decompress_struct* cinfo = fImageIndex->cinfo();

    SkIRect rect = SkIRect::MakeWH(fImageWidth, fImageHeight);
    if (!rect.intersect(region)) {
        // If the requested region is entirely outside the image return false
        return false;
    }


    skjpeg_error_mgr errorManager;
    set_error_mgr(cinfo, &errorManager);

    if (setjmp(errorManager.fJmpBuf)) {
        return false;
    }

    int requestedSampleSize = this->getSampleSize();
    cinfo->scale_denom = requestedSampleSize;

    set_dct_method(*this, cinfo);

    const SkBitmap::Config config = this->getBitmapConfig(cinfo);
#ifdef ANDROID_RGB
    adjust_out_color_space_and_dither(cinfo, config, *this);
#endif

    int startX = rect.fLeft;
    int startY = rect.fTop;
    int width = rect.width();
    int height = rect.height();

    jpeg_init_read_tile_scanline(cinfo, fImageIndex->huffmanIndex(),
                                 &startX, &startY, &width, &height);
    int skiaSampleSize = recompute_sampleSize(requestedSampleSize, *cinfo);
    int actualSampleSize = skiaSampleSize * (DCTSIZE / cinfo->min_DCT_scaled_size);

    SkScaledBitmapSampler sampler(width, height, skiaSampleSize);

    SkBitmap bitmap;
    bitmap.setConfig(config, sampler.scaledWidth(), sampler.scaledHeight());
    //bitmap.setIsOpaque(true);

    // Check ahead of time if the swap(dest, src) is possible or not.
    // If yes, then we will stick to AllocPixelRef since it's cheaper with the
    // swap happening. If no, then we will use alloc to allocate pixels to
    // prevent garbage collection.
    int w = rect.width() / actualSampleSize;
    int h = rect.height() / actualSampleSize;
    bool swapOnly = (rect == region) && bm->isNull() &&
                    (w == bitmap.width()) && (h == bitmap.height()) &&
                    ((startX - rect.x()) / actualSampleSize == 0) &&
                    ((startY - rect.y()) / actualSampleSize == 0);
    if (swapOnly) {
        if (!this->allocPixelRef(&bitmap, NULL)) {
            return return_false(*cinfo, bitmap, "allocPixelRef");
        }
    } else {
        if (!bitmap.allocPixels()) {
            return return_false(*cinfo, bitmap, "allocPixels");
        }
    }

    SkAutoLockPixels alp(bitmap);

#ifdef ANDROID_RGB
    /* short-circuit the SkScaledBitmapSampler when possible, as this gives
       a significant performance boost.
    */
    if (skiaSampleSize == 1 &&
        ((config == SkBitmap::kARGB_8888_Config &&
                cinfo->out_color_space == JCS_RGBA_8888) ||
        (config == SkBitmap::kRGB_565_Config &&
                cinfo->out_color_space == JCS_RGB_565)))
    {
        JSAMPLE* rowptr = (JSAMPLE*)bitmap.getPixels();
        INT32 const bpr = bitmap.rowBytes();
        int rowTotalCount = 0;

        while (rowTotalCount < height) {
            int rowCount = jpeg_read_tile_scanline(cinfo,
                                                   fImageIndex->huffmanIndex(),
                                                   &rowptr);
            // if row_count == 0, then we didn't get a scanline, so abort.
            // if we supported partial images, we might return true in this case
            if (0 == rowCount) {
                return return_false(*cinfo, bitmap, "read_scanlines");
            }
            if (this->shouldCancelDecode()) {
                return return_false(*cinfo, bitmap, "shouldCancelDecode");
            }
            rowTotalCount += rowCount;
            rowptr += bpr;
        }

        if (swapOnly) {
            bm->swap(bitmap);
        } else {
            cropBitmap(bm, &bitmap, actualSampleSize, region.x(), region.y(),
                       region.width(), region.height(), startX, startY);
        }
        return true;
    }
#endif

    // check for supported formats
    SkScaledBitmapSampler::SrcConfig sc;
    if (JCS_CMYK == cinfo->out_color_space) {
        // In this case we will manually convert the CMYK values to RGB
        sc = SkScaledBitmapSampler::kRGBX;
    } else if (3 == cinfo->out_color_components && JCS_RGB == cinfo->out_color_space) {
        sc = SkScaledBitmapSampler::kRGB;
#ifdef ANDROID_RGB
    } else if (JCS_RGBA_8888 == cinfo->out_color_space) {
        sc = SkScaledBitmapSampler::kRGBX;
    } else if (JCS_RGB_565 == cinfo->out_color_space) {
        sc = SkScaledBitmapSampler::kRGB_565;
#endif
    } else if (1 == cinfo->out_color_components &&
               JCS_GRAYSCALE == cinfo->out_color_space) {
        sc = SkScaledBitmapSampler::kGray;
    } else {
        return return_false(*cinfo, *bm, "jpeg colorspace");
    }

    if (!sampler.begin(&bitmap, sc, *this)) {
        return return_false(*cinfo, bitmap, "sampler.begin");
    }

    // The CMYK work-around relies on 4 components per pixel here
    SkAutoMalloc  srcStorage(width * 4);
    uint8_t* srcRow = (uint8_t*)srcStorage.get();

    //  Possibly skip initial rows [sampler.srcY0]
    if (!skip_src_rows_tile(cinfo, fImageIndex->huffmanIndex(), srcRow, sampler.srcY0())) {
        return return_false(*cinfo, bitmap, "skip rows");
    }

    // now loop through scanlines until y == bitmap->height() - 1
    for (int y = 0;; y++) {
        JSAMPLE* rowptr = (JSAMPLE*)srcRow;
        int row_count = jpeg_read_tile_scanline(cinfo, fImageIndex->huffmanIndex(), &rowptr);
        if (0 == row_count) {
            return return_false(*cinfo, bitmap, "read_scanlines");
        }
        if (this->shouldCancelDecode()) {
            return return_false(*cinfo, bitmap, "shouldCancelDecode");
        }

        if (JCS_CMYK == cinfo->out_color_space) {
            convert_CMYK_to_RGB(srcRow, width);
        }

        sampler.next(srcRow);
        if (bitmap.height() - 1 == y) {
            // we're done
            break;
        }

        if (!skip_src_rows_tile(cinfo, fImageIndex->huffmanIndex(), srcRow,
                                sampler.srcDY() - 1)) {
            return return_false(*cinfo, bitmap, "skip rows");
        }
    }
    if (swapOnly) {
        bm->swap(bitmap);
    } else {
        cropBitmap(bm, &bitmap, actualSampleSize, region.x(), region.y(),
                   region.width(), region.height(), startX, startY);
    }
    return true;
}
#endif

///////////////////////////////////////////////////////////////////////////////

#include "SkColorPriv.h"

// taken from jcolor.c in libjpeg
#if 0   // 16bit - precise but slow
    #define CYR     19595   // 0.299
    #define CYG     38470   // 0.587
    #define CYB      7471   // 0.114

    #define CUR    -11059   // -0.16874
    #define CUG    -21709   // -0.33126
    #define CUB     32768   // 0.5

    #define CVR     32768   // 0.5
    #define CVG    -27439   // -0.41869
    #define CVB     -5329   // -0.08131

    #define CSHIFT  16
#else      // 8bit - fast, slightly less precise
    #define CYR     77    // 0.299
    #define CYG     150    // 0.587
    #define CYB      29    // 0.114

    #define CUR     -43    // -0.16874
    #define CUG    -85    // -0.33126
    #define CUB     128    // 0.5

    #define CVR      128   // 0.5
    #define CVG     -107   // -0.41869
    #define CVB      -21   // -0.08131

    #define CSHIFT  8
#endif

static void rgb2yuv_32(uint8_t dst[], SkPMColor c) {
    int r = SkGetPackedR32(c);
    int g = SkGetPackedG32(c);
    int b = SkGetPackedB32(c);

    int  y = ( CYR*r + CYG*g + CYB*b ) >> CSHIFT;
    int  u = ( CUR*r + CUG*g + CUB*b ) >> CSHIFT;
    int  v = ( CVR*r + CVG*g + CVB*b ) >> CSHIFT;

    dst[0] = SkToU8(y);
    dst[1] = SkToU8(u + 128);
    dst[2] = SkToU8(v + 128);
}

static void rgb2yuv_4444(uint8_t dst[], U16CPU c) {
    int r = SkGetPackedR4444(c);
    int g = SkGetPackedG4444(c);
    int b = SkGetPackedB4444(c);

    int  y = ( CYR*r + CYG*g + CYB*b ) >> (CSHIFT - 4);
    int  u = ( CUR*r + CUG*g + CUB*b ) >> (CSHIFT - 4);
    int  v = ( CVR*r + CVG*g + CVB*b ) >> (CSHIFT - 4);

    dst[0] = SkToU8(y);
    dst[1] = SkToU8(u + 128);
    dst[2] = SkToU8(v + 128);
}

static void rgb2yuv_16(uint8_t dst[], U16CPU c) {
    int r = SkGetPackedR16(c);
    int g = SkGetPackedG16(c);
    int b = SkGetPackedB16(c);

    int  y = ( 2*CYR*r + CYG*g + 2*CYB*b ) >> (CSHIFT - 2);
    int  u = ( 2*CUR*r + CUG*g + 2*CUB*b ) >> (CSHIFT - 2);
    int  v = ( 2*CVR*r + CVG*g + 2*CVB*b ) >> (CSHIFT - 2);

    dst[0] = SkToU8(y);
    dst[1] = SkToU8(u + 128);
    dst[2] = SkToU8(v + 128);
}

///////////////////////////////////////////////////////////////////////////////

typedef void (*WriteScanline)(uint8_t* SK_RESTRICT dst,
                              const void* SK_RESTRICT src, int width,
                              const SkPMColor* SK_RESTRICT ctable);

static void Write_32_YUV(uint8_t* SK_RESTRICT dst,
                         const void* SK_RESTRICT srcRow, int width,
                         const SkPMColor*) {
    const uint32_t* SK_RESTRICT src = (const uint32_t*)srcRow;
    while (--width >= 0) {
#ifdef WE_CONVERT_TO_YUV
        rgb2yuv_32(dst, *src++);
#else
        uint32_t c = *src++;
        dst[0] = SkGetPackedR32(c);
        dst[1] = SkGetPackedG32(c);
        dst[2] = SkGetPackedB32(c);
#endif
        dst += 3;
    }
}

static void Write_4444_YUV(uint8_t* SK_RESTRICT dst,
                           const void* SK_RESTRICT srcRow, int width,
                           const SkPMColor*) {
    const SkPMColor16* SK_RESTRICT src = (const SkPMColor16*)srcRow;
    while (--width >= 0) {
#ifdef WE_CONVERT_TO_YUV
        rgb2yuv_4444(dst, *src++);
#else
        SkPMColor16 c = *src++;
        dst[0] = SkPacked4444ToR32(c);
        dst[1] = SkPacked4444ToG32(c);
        dst[2] = SkPacked4444ToB32(c);
#endif
        dst += 3;
    }
}

static void Write_16_YUV(uint8_t* SK_RESTRICT dst,
                         const void* SK_RESTRICT srcRow, int width,
                         const SkPMColor*) {
    const uint16_t* SK_RESTRICT src = (const uint16_t*)srcRow;
    while (--width >= 0) {
#ifdef WE_CONVERT_TO_YUV
        rgb2yuv_16(dst, *src++);
#else
        uint16_t c = *src++;
        dst[0] = SkPacked16ToR32(c);
        dst[1] = SkPacked16ToG32(c);
        dst[2] = SkPacked16ToB32(c);
#endif
        dst += 3;
    }
}

static void Write_Index_YUV(uint8_t* SK_RESTRICT dst,
                            const void* SK_RESTRICT srcRow, int width,
                            const SkPMColor* SK_RESTRICT ctable) {
    const uint8_t* SK_RESTRICT src = (const uint8_t*)srcRow;
    while (--width >= 0) {
#ifdef WE_CONVERT_TO_YUV
        rgb2yuv_32(dst, ctable[*src++]);
#else
        uint32_t c = ctable[*src++];
        dst[0] = SkGetPackedR32(c);
        dst[1] = SkGetPackedG32(c);
        dst[2] = SkGetPackedB32(c);
#endif
        dst += 3;
    }
}

static WriteScanline ChooseWriter(const SkBitmap& bm) {
    switch (bm.config()) {
        case SkBitmap::kARGB_8888_Config:
            return Write_32_YUV;
        case SkBitmap::kRGB_565_Config:
            return Write_16_YUV;
        case SkBitmap::kARGB_4444_Config:
            return Write_4444_YUV;
        case SkBitmap::kIndex8_Config:
            return Write_Index_YUV;
        default:
            return NULL;
    }
}

class SkJPEGImageEncoder : public SkImageEncoder {
protected:
    virtual bool onEncode(SkWStream* stream, const SkBitmap& bm, int quality) {
#ifdef TIME_ENCODE
        SkAutoTime atm("JPEG Encode");
#endif

        SkAutoLockPixels alp(bm);
        if (NULL == bm.getPixels()) {
            return false;
        }

        jpeg_compress_struct    cinfo;
        skjpeg_error_mgr        sk_err;
        skjpeg_destination_mgr  sk_wstream(stream);

        // allocate these before set call setjmp
        SkAutoMalloc    oneRow;
        SkAutoLockColors ctLocker;

        cinfo.err = jpeg_std_error(&sk_err);
        sk_err.error_exit = skjpeg_error_exit;
        if (setjmp(sk_err.fJmpBuf)) {
            return false;
        }

        // Keep after setjmp or mark volatile.
        const WriteScanline writer = ChooseWriter(bm);
        if (NULL == writer) {
            return false;
        }

        jpeg_create_compress(&cinfo);
        cinfo.dest = &sk_wstream;
        cinfo.image_width = bm.width();
        cinfo.image_height = bm.height();
        cinfo.input_components = 3;
#ifdef WE_CONVERT_TO_YUV
        cinfo.in_color_space = JCS_YCbCr;
#else
        cinfo.in_color_space = JCS_RGB;
#endif
        cinfo.input_gamma = 1;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
#ifdef DCT_IFAST_SUPPORTED
        cinfo.dct_method = JDCT_IFAST;
#endif

        jpeg_start_compress(&cinfo, TRUE);

        const int       width = bm.width();
        uint8_t*        oneRowP = (uint8_t*)oneRow.reset(width * 3);

        const SkPMColor* colors = ctLocker.lockColors(bm);
        const void*      srcRow = bm.getPixels();

        while (cinfo.next_scanline < cinfo.image_height) {
            JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */

            writer(oneRowP, srcRow, width, colors);
            row_pointer[0] = oneRowP;
            (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
            srcRow = (const void*)((const char*)srcRow + bm.rowBytes());
        }

        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);

        return true;
    }
};

///////////////////////////////////////////////////////////////////////////////
DEFINE_DECODER_CREATOR(JPEGImageDecoder);
DEFINE_ENCODER_CREATOR(JPEGImageEncoder);
///////////////////////////////////////////////////////////////////////////////

static bool is_jpeg(SkStreamRewindable* stream) {
    static const unsigned char gHeader[] = { 0xFF, 0xD8, 0xFF };
    static const size_t HEADER_SIZE = sizeof(gHeader);

    char buffer[HEADER_SIZE];
    size_t len = stream->read(buffer, HEADER_SIZE);

    if (len != HEADER_SIZE) {
        return false;   // can't read enough
    }
    if (memcmp(buffer, gHeader, HEADER_SIZE)) {
        return false;
    }
    return true;
}

#include "SkTRegistry.h"

static SkImageDecoder* sk_libjpeg_dfactory(SkStreamRewindable* stream) {
    if (is_jpeg(stream)) {
        return SkNEW(SkJPEGImageDecoder);
    }
    return NULL;
}

static SkImageDecoder::Format get_format_jpeg(SkStreamRewindable* stream) {
    if (is_jpeg(stream)) {
        return SkImageDecoder::kJPEG_Format;
    }
    return SkImageDecoder::kUnknown_Format;
}

static SkImageEncoder* sk_libjpeg_efactory(SkImageEncoder::Type t) {
    return (SkImageEncoder::kJPEG_Type == t) ? SkNEW(SkJPEGImageEncoder) : NULL;
}

static SkImageDecoder_DecodeReg gDReg(sk_libjpeg_dfactory);
static SkImageDecoder_FormatReg gFormatReg(get_format_jpeg);
static SkImageEncoder_EncodeReg gEReg(sk_libjpeg_efactory);
