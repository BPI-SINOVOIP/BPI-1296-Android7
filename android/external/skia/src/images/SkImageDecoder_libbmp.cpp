
/*
 * Copyright 2007 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#include "bmpdecoderhelper.h"
#include "SkColorPriv.h"
#include "SkData.h"
#include "SkImageDecoder.h"
#include "SkScaledBitmapSampler.h"
#include "SkStream.h"
#include "SkStreamPriv.h"
#include "SkTDArray.h"

#define MAX_DIM_W   2048//4096
#define MAX_DIM_H   1080//2160

class SkBMPImageDecoder : public SkImageDecoder {
public:
    SkBMPImageDecoder() {}

    Format getFormat() const override {
        return kBMP_Format;
    }

protected:
    Result onDecode(SkStream* stream, SkBitmap* bm, Mode mode) override;

private:
    typedef SkImageDecoder INHERITED;
};

///////////////////////////////////////////////////////////////////////////////
DEFINE_DECODER_CREATOR(BMPImageDecoder);
///////////////////////////////////////////////////////////////////////////////

static bool is_bmp(SkStreamRewindable* stream) {
    static const char kBmpMagic[] = { 'B', 'M' };


    char buffer[sizeof(kBmpMagic)];

    return stream->read(buffer, sizeof(kBmpMagic)) == sizeof(kBmpMagic) &&
        !memcmp(buffer, kBmpMagic, sizeof(kBmpMagic));
}

static SkImageDecoder* sk_libbmp_dfactory(SkStreamRewindable* stream) {
    if (is_bmp(stream)) {
        return new SkBMPImageDecoder;
    }
    return nullptr;
}

static SkImageDecoder_DecodeReg gReg(sk_libbmp_dfactory);

static SkImageDecoder::Format get_format_bmp(SkStreamRewindable* stream) {
    if (is_bmp(stream)) {
        return SkImageDecoder::kBMP_Format;
    }
    return SkImageDecoder::kUnknown_Format;
}

static SkImageDecoder_FormatReg gFormatReg(get_format_bmp);

///////////////////////////////////////////////////////////////////////////////

class SkBmpDecoderCallback : public image_codec::BmpDecoderCallback {
public:
    // we don't copy the bitmap, just remember the pointer
    SkBmpDecoderCallback(bool justBounds) : fJustBounds(justBounds),fExceedDimLimit(false) {}

    // override from BmpDecoderCallback
    virtual uint8* SetSize(int width, int height) {
        fWidth = width;
        fHeight = height;

        if (fJustBounds) {
            return nullptr;
        }

        if (width*height > MAX_DIM_W*MAX_DIM_H) {
            fRGB.setCount(fWidth * 1 * 3);
            fExceedDimLimit = true;
        }
        else {
            fRGB.setCount(fWidth * fHeight * 3);  // 3 == r, g, b
        }
        return fRGB.begin();
    }

    virtual bool Exceed() { return fExceedDimLimit; }
    int width() const { return fWidth; }
    int height() const { return fHeight; }
    const uint8_t* rgb() const { return fRGB.begin(); }

private:
    SkTDArray<uint8_t> fRGB;
    int fWidth;
    int fHeight;
    bool fJustBounds;
    bool fExceedDimLimit;
};

SkImageDecoder::Result SkBMPImageDecoder::onDecode(SkStream* stream, SkBitmap* bm, Mode mode) {
    // First read the entire stream, so that all of the data can be passed to
    // the BmpDecoderHelper.

#if 0
    SkAutoTUnref<SkData> data(SkCopyStreamToData(stream));
    if (!data) {
        return kFailure;
    }

    // Byte length of all of the data.
    const size_t length = data->size();
#else
    size_t length = 0;
    if(stream->hasLength()) {
       length = stream->getLength();
    }
    else {
        // Arbitrary buffer size.
        const size_t bufferSize = 256 * 1024; // 256KB
        char buffer[bufferSize];
        SkDEBUGCODE(size_t debugLength = 0;)
        do {
            size_t bytesRead = stream->read(buffer, bufferSize);
            length += bytesRead;
        } while (!stream->isAtEnd());
    }
#endif

    if (0 == length) {
        return kFailure;
    }

    bool bRewind = false;
    if( (bRewind = stream->rewind()) == false)
        if( (bRewind = stream->seek(0)) == false)
            bRewind = stream->move(-1*stream->getPosition());

    const bool justBounds = SkImageDecoder::kDecodeBounds_Mode == mode;
    SkBmpDecoderCallback callback(justBounds);

    CHECK_CANCEL_DECODE();

    // Now decode the BMP into callback's rgb() array [r,g,b, r,g,b, ...]
#if 0
    {
        image_codec::BmpDecoderHelper helper;
        const int max_pixels = 16383*16383; // max width*height
        if (!helper.DecodeImage((const char*) data->data(), length,
                                max_pixels, &callback)) {
            return kFailure;
        }
    }

    // we don't need this anymore, so free it now (before we try to allocate
    // the bitmap's pixels) rather than waiting for its destructor
    data.reset(nullptr);
#else
    // Change: Big bitmap decode will exhause too much memory, therefore
    // we just decode header
    image_codec::BmpDecoderHelper helper;
    const int max_pixels = 16383*16383; // max width*height
    if (!helper.DecodeImage(stream, length, max_pixels, &callback)) {
        return kFailure;
    }
#endif

    int width = callback.width();
    int height = callback.height();
    SkColorType colorType = this->getPrefColorType(k32Bit_SrcDepth, false);

    // only accept prefConfig if it makes sense for us
    if (kARGB_4444_SkColorType != colorType && kRGB_565_SkColorType != colorType) {
        colorType = kN32_SkColorType;
    }

    SkScaledBitmapSampler sampler(width, height, getSampleSize());

    bm->setInfo(SkImageInfo::Make(sampler.scaledWidth(), sampler.scaledHeight(),
                                  colorType, kOpaque_SkAlphaType));

    if (justBounds) {
        return kSuccess;
    }

    if (!this->allocPixelRef(bm, nullptr)) {
        return kFailure;
    }

    SkAutoLockPixels alp(*bm);

    if (!sampler.begin(bm, SkScaledBitmapSampler::kRGB, *this)) {
        return kFailure;
    }

    if (!callback.Exceed()) {
        const int srcRowBytes = width * 3;
        const int dstHeight = sampler.scaledHeight();
        const uint8_t* srcRow = callback.rgb();

        srcRow += sampler.srcY0() * srcRowBytes;
        for (int y = 0; y < dstHeight; y++) {
            sampler.next(srcRow);
            srcRow += sampler.srcDY() * srcRowBytes;
            CHECK_CANCEL_DECODE();
        }
    } else {
        const int dstHeight = sampler.scaledHeight();
        const uint8_t* srcRow = callback.rgb();
        helper.GetRow(sampler.srcY0());
        for (int y = 0; y < dstHeight; y++) {
            sampler.next(srcRow);
            helper.GetRow(sampler.srcDY()*(y+1));
            CHECK_CANCEL_DECODE();
        }
    }
    return kSuccess;
}
