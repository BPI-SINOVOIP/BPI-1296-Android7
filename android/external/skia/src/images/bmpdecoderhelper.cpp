
/*
 * Copyright 2007 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

// Author: cevans@google.com (Chris Evans)

#include "bmpdecoderhelper.h"

//#define ARM_ACCEL

namespace image_codec {

static const int kBmpHeaderSize = 14;
static const int kBmpInfoSize = 40;
static const int kBmpOS2InfoSize = 12;
static const int kMaxDim = SHRT_MAX / 2;

bool BmpDecoderHelper::DecodeImage(SkStream* stream,
                                   int len,
                                   int max_pixels,
                                   BmpDecoderCallback* callback) {
  data_ = stream;
  pos_ = 0;
  len_ = len;
  inverted_ = true;
  // Parse the header structure.
  if (len < kBmpHeaderSize + 4) {
    return false;
  }
  GetShort();  // Signature.
  GetInt();  // Size.
  GetInt();  // Reserved.
  imgDataBase_ = GetInt();
  // Parse the info structure.
  int infoSize = GetInt();
  if (infoSize != kBmpOS2InfoSize && infoSize < kBmpInfoSize) {
    return false;
  }
  int cols = 0;
  int comp = 0;
  int colLen = 4;
  if (infoSize >= kBmpInfoSize) {
    if (len < kBmpHeaderSize + kBmpInfoSize) {
      return false;
    }
    width_ = GetInt();
    height_ = GetInt();
    GetShort();  // Planes.
    bpp_ = GetShort();
    comp = GetInt();
    GetInt();  // Size.
    GetInt();  // XPPM.
    GetInt();  // YPPM.
    cols = GetInt();
    GetInt();  // Important colours.
  } else {
    if (len < kBmpHeaderSize + kBmpOS2InfoSize) {
      return false;
    }
    colLen = 3;
    width_ = GetShort();
    height_ = GetShort();
    GetShort();  // Planes.
    bpp_ = GetShort();
  }
  if (height_ < 0) {
    height_ = -height_;
    inverted_ = false;
  }
  if (width_ <= 0 || width_ > kMaxDim || height_ <= 0 || height_ > kMaxDim) {
    return false;
  }
  if (width_ * height_ > max_pixels) {
    return false;
  }
  if (cols < 0 || cols > 256) {
    return false;
  }
  // Allocate then read in the colour map.
  if (cols == 0 && bpp_ <= 8) {
    cols = 1 << bpp_;
  }
  if (bpp_ <= 8 || cols > 0) {
    uint8* colBuf = new uint8[256 * 3];
    memset(colBuf, '\0', 256 * 3);
    colTab_.reset(colBuf);
  }
  if (cols > 0) {
    if (pos_ + (cols * colLen) > len_) {
      return false;
    }
    for (int i = 0; i < cols; ++i) {
      int base = i * 3;
      colTab_[base + 2] = GetByte();
      colTab_[base + 1] = GetByte();
      colTab_[base] = GetByte();
      if (colLen == 4) {
        GetByte();
      }
    }
  }
  // Read in the compression data if necessary.
  redBits_ = 0x7c00;
  greenBits_ = 0x03e0;
  blueBits_ = 0x001f;
  rle_ = false;
  if (comp == 1 || comp == 2) {
    rle_ = true;
  } else if (comp == 3) {
    if (pos_ + 12 > len_) {
      return false;
    }
    redBits_ = GetInt() & 0xffff;
    greenBits_ = GetInt() & 0xffff;
    blueBits_ = GetInt() & 0xffff;
  }
  redShiftRight_ = CalcShiftRight(redBits_);
  greenShiftRight_ = CalcShiftRight(greenBits_);
  blueShiftRight_ = CalcShiftRight(blueBits_);
  redShiftLeft_ = CalcShiftLeft(redBits_);
  greenShiftLeft_ = CalcShiftLeft(greenBits_);
  blueShiftLeft_ = CalcShiftLeft(blueBits_);
  rowPad_ = 0;
  pixelPad_ = 0;
  int rowLen;
  if (bpp_ == 32) {
    rowLen = width_ * 4;
    pixelPad_ = 1;
  } else if (bpp_ == 24) {
    rowLen = width_ * 3;
  } else if (bpp_ == 16) {
    rowLen = width_ * 2;
  } else if (bpp_ == 8) {
    rowLen = width_;
  } else if (bpp_ == 4) {
    rowLen = width_ / 2;
    if (width_ & 1) {
      rowLen++;
    }
  } else if (bpp_ == 1) {
    rowLen = width_ / 8;
    if (width_ & 7) {
      rowLen++;
    }
  } else {
    return false;
  }
  // Round the rowLen up to a multiple of 4.
  if (rowLen % 4 != 0) {
    rowPad_ = 4 - (rowLen % 4);
    rowLen += rowPad_;
  }

  if (imgDataBase_ > 0 && imgDataBase_ > pos_ && imgDataBase_ < len_) {
    pos_ = imgDataBase_;
  }
  // Deliberately off-by-one; a load of BMPs seem to have their last byte
  // missing.
  if (!rle_ && (pos_ + (rowLen * height_) > len_ + 1)) {
    return false;
  }

  output_ = callback->SetSize(width_, height_);
  if (nullptr == output_ || callback->Exceed()) {
    return true;  // meaning we succeeded, but they want us to stop now
  }
 
  if (bpp_ == 24)
  {
    DoStandardDecode_24();
  }
  else if (rle_ && (bpp_ == 4 || bpp_ == 8)) {
    DoRLEDecode();
  } else {
    DoStandardDecode();
  }

  return true;
}

void BmpDecoderHelper::DoRLEDecode() {
  static const uint8 RLE_ESCAPE = 0;
  static const uint8 RLE_EOL = 0;
  static const uint8 RLE_EOF = 1;
  static const uint8 RLE_DELTA = 2;
  int x = 0;
  int y = height_ - 1;
  while (pos_ + 1 < len_) {
    uint8 cmd = GetByte();
    if (cmd != RLE_ESCAPE) {
      uint8 pixels = GetByte();
      int num = 0;
      uint8 col = pixels;
      while (cmd-- && x < width_) {
        if (bpp_ == 4) {
          if (num & 1) {
            col = pixels & 0xf;
          } else {
            col = pixels >> 4;
          }
        }
        PutPixel(x++, y, col);
        num++;
      }
    } else {
      cmd = GetByte();
      if (cmd == RLE_EOF) {
        return;
      } else if (cmd == RLE_EOL) {
        x = 0;
        y--;
        if (y < 0) {
          return;
        }
      } else if (cmd == RLE_DELTA) {
        if (pos_ + 1 < len_) {
          uint8 dx = GetByte();
          uint8 dy = GetByte();
          x += dx;
          if (x > width_) {
            x = width_;
          }
          y -= dy;
          if (y < 0) {
            return;
          }
        }
      } else {
        int num = 0;
        int bytesRead = 0;
        uint8 val = 0;
        while (cmd-- && pos_ < len_) {
          if (bpp_ == 8 || !(num & 1)) {
            val = GetByte();
            bytesRead++;
          }
          uint8 col = val;
          if (bpp_ == 4) {
            if (num & 1) {
              col = col & 0xf;
            } else {
              col >>= 4;
            }
          }
          if (x < width_) {
            PutPixel(x++, y, col);
          }
          num++;
        }
        // All pixel runs must be an even number of bytes - skip a byte if we
        // read an odd number.
        if ((bytesRead & 1) && pos_ < len_) {
          GetByte();
        }
      }
    }
  }
}

void BmpDecoderHelper::DoRLEDecode(int row) {
    // not implemented
}

void BmpDecoderHelper::PutPixel(int x, int y, uint8 col) {
  CHECK(x >= 0 && x < width_);
  CHECK(y >= 0 && y < height_);
  if (!inverted_) {
    y = height_ - (y + 1);
  }

  int base = ((y * width_) + x) * 3;
  int colBase = col * 3;
  output_[base] = colTab_[colBase];
  output_[base + 1] = colTab_[colBase + 1];
  output_[base + 2] = colTab_[colBase + 2];
}

void BmpDecoderHelper::DoStandardDecode() {
  int row = 0;
  uint8 currVal = 0;
  for (int h = height_ - 1; h >= 0; h--, row++) {
    int realH = h;
    if (!inverted_) {
      realH = height_ - (h + 1);
    }
    uint8* line = output_ + (3 * width_ * realH);
    for (int w = 0; w < width_; w++) {
      if (bpp_ >= 24) {
        line[2] = GetByte();
        line[1] = GetByte();
        line[0] = GetByte();
      } else if (bpp_ == 16) {
        uint32 val = GetShort();
        line[0] = ((val & redBits_) >> redShiftRight_) << redShiftLeft_;
        line[1] = ((val & greenBits_) >> greenShiftRight_) << greenShiftLeft_;
        line[2] = ((val & blueBits_) >> blueShiftRight_) << blueShiftLeft_;
      } else if (bpp_ <= 8) {
        uint8 col;
        if (bpp_ == 8) {
          col = GetByte();
        } else if (bpp_ == 4) {
          if ((w % 2) == 0) {
            currVal = GetByte();
            col = currVal >> 4;
          } else {
            col = currVal & 0xf;
          }
        } else {
          if ((w % 8) == 0) {
            currVal = GetByte();
          }
          int bit = w & 7;
          col = ((currVal >> (7 - bit)) & 1);
        }
        int base = col * 3;
        line[0] = colTab_[base];
        line[1] = colTab_[base + 1];
        line[2] = colTab_[base + 2];
      }
      line += 3;
      for (int i = 0; i < pixelPad_; ++i) {
        GetByte();
      }
    }
    for (int i = 0; i < rowPad_; ++i) {
      GetByte();
    }
  }
}
void BmpDecoderHelper::DoStandardDecode_24() {
  int row = 0;
  uint8 currVal = 0;
  for (int h = height_ - 1; h >= 0; h--, row++) {
    int realH = h;
    if (!inverted_) {
      realH = height_ - (h + 1);
    }
    uint8* line = output_ + (3 * width_ * realH);
    for (int w = 0; w < width_; w++) {
        line[2] = GetByte();
        line[1] = GetByte();
        line[0] = GetByte();
      line += 3;
      for (int i = 0; i < pixelPad_; ++i) {
        GetByte();
      }
    }
    for (int i = 0; i < rowPad_; ++i) {
      GetByte();
    }
  }
}

void BmpDecoderHelper::DoStandardDecode(int row) {
    uint8 currVal = 0;
    int offset;
    int bytePerPixel = (bpp_>=24) ? 3: ((bpp_==16) ? 2:1);

    if (!inverted_)
        offset = row * (width_* (bytePerPixel+pixelPad_) + rowPad_);
    else
        offset = ((height_-1)-row) * (width_* (bytePerPixel+pixelPad_) + rowPad_);

    bool seekable = data_->seek(imgDataBase_ + offset);
    uint8* line = output_;
    for (int w = 0; w < width_; w++) {
      if (bpp_ >= 24) {
        line[2] = GetByte();
        line[1] = GetByte();
        line[0] = GetByte();
      } else if (bpp_ == 16) {
        uint32 val = GetShort();
        line[0] = ((val & redBits_) >> redShiftRight_) << redShiftLeft_;
        line[1] = ((val & greenBits_) >> greenShiftRight_) << greenShiftLeft_;
        line[2] = ((val & blueBits_) >> blueShiftRight_) << blueShiftLeft_;
      } else if (bpp_ <= 8) {
        uint8 col;
        if (bpp_ == 8) {
          col = GetByte();
        } else if (bpp_ == 4) {
          if ((w % 2) == 0) {
            currVal = GetByte();
            col = currVal >> 4;
          } else {
            col = currVal & 0xf;
          }
        } else {
          if ((w % 8) == 0) {
            currVal = GetByte();
          }
          int bit = w & 7;
          col = ((currVal >> (7 - bit)) & 1);
        }
        int base = col * 3;
        line[0] = colTab_[base];
        line[1] = colTab_[base + 1];
        line[2] = colTab_[base + 2];
      }
      line += 3;
      for (int i = 0; i < pixelPad_; ++i) {
        GetByte();
      }      
    }
}
#ifdef ARM_ACCEL
#ifdef SK_CPU_ARM64
void BmpDecoderHelper::SwapRow(char* _src, char* _dst, int _size) {

    uint8x8_t vsrc_0, vsrc_1, vsrc_2;

    asm (
        "ld3    {v0.8b - v2.8b}, [%[_src]], #24 \t\n"
        "mov    %[vsrc0].8b, v0.8b             \t\n"
//        "mov    %[vsrc1].8b, v1.8b             \t\n"
        "mov    %[vsrc2].8b, v2.8b             \t\n"
        "mov    v2.8b, %[vsrc0].8b             \t\n"
        "mov    v0.8b, %[vsrc2].8b             \t\n"
        "st3    {v0.8b - v2.8b}, [%[_dst]], #24 \t\n"

        : [vsrc0] "=w" (vsrc_0), [vsrc1] "=w" (vsrc_1),
          [vsrc2] "=w" (vsrc_2), [_src] "+&r" (_src), [_dst] "+&r" (_dst)
        : : "v0", "v1", "v2"
    );


}
#else
void BmpDecoderHelper::SwapRow(char * _src, char * _dst, int _size) {

        asm volatile(
            "stmfd sp!, {r0,r1,r2,r3}\n"

            "mov r0, %0\n"
            "mov r1, %1\n"
            "mov r2, %2\n"
            "mov r3, r2, lsr #3\n"

            "1:\n"
//            "vldmia r0!, {d0-d15}\n"
//            "pld [r0]\n"
            "pld [r0, #64]\n"
            "vld3.8 {d0-d2}, [%r0]!\n"
//            "subs r3, r3, #1\n"
//            "vstmia r1!, {d16-d19}\n"
            "vswp d0, d2\n"
            "vst3.8 {d0-d2}, [%r1]!\n"
            "subs r3, r3, #1\n"
            "bgt 1b\n"

            "ldmfd sp!, {r0,r1,r2,r3}\n"
        :: [_src] "r" (_src), [_dst] "r" (_dst), [_size] "r" (_size) : "r0", "r1", "r2");
}
#endif
#endif

void BmpDecoderHelper::DoStandardDecode_24(int row) {
    uint8 currVal = 0;
    int offset;
    int bytePerPixel = 3;

    if (!inverted_)
        offset = row * (width_* (bytePerPixel+pixelPad_) + rowPad_);
    else
        offset = ((height_-1)-row) * (width_* (bytePerPixel+pixelPad_) + rowPad_);

    bool seekable = data_->seek(imgDataBase_ + offset);
    uint8* line = output_;

    int w1, w2;
    w1 = (width_ >> 3) << 3;
#ifdef ARM_ACCEL
    w2 = width_ - w1;
    data_->read(line, w1*3);
    SwapRow((char *)line, (char *)line, w1);
    line+=w1*3;
#else
    w2 = width_;
#endif
    if (w2 > 0)
    {
        for (int w = 0; w < w2; w++) {
            line[2] = GetByte();
            line[1] = GetByte();
            line[0] = GetByte();
            line+=3;
        }
    }
    
}

const uint8_t* BmpDecoderHelper::GetRow(int row) {

  if (output_ == NULL)
    return NULL;

  if (bpp_ == 24)
  {
    DoStandardDecode_24(row);
  }
  else if (rle_ && (bpp_ == 4 || bpp_ == 8)) {
    DoRLEDecode(row);
  } else {
    DoStandardDecode(row);
  }

  return output_;
}

int BmpDecoderHelper::GetInt() {
  uint8 b1 = GetByte();
  uint8 b2 = GetByte();
  uint8 b3 = GetByte();
  uint8 b4 = GetByte();
  return b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);
}

int BmpDecoderHelper::GetShort() {
  uint8 b1 = GetByte();
  uint8 b2 = GetByte();
  return b1 | (b2 << 8);
}

uint8 BmpDecoderHelper::GetByte() {
  CHECK(pos_ <= len_);
  // We deliberately allow this off-by-one access to cater for BMPs with their
  // last byte missing.
  if (pos_ == len_) {
    return 0;
  }
  pos_++;
  return data_->readU8();
}

int BmpDecoderHelper::CalcShiftRight(uint32 mask) {
  int ret = 0;
  while (mask != 0 && !(mask & 1)) {
    mask >>= 1;
    ret++;
  }
  return ret;
}

int BmpDecoderHelper::CalcShiftLeft(uint32 mask) {
  int ret = 0;
  while (mask != 0 && !(mask & 1)) {
    mask >>= 1;
  }
  while (mask != 0 && !(mask & 0x80)) {
    mask <<= 1;
    ret++;
  }
  return ret;
}

}  // namespace image_codec
