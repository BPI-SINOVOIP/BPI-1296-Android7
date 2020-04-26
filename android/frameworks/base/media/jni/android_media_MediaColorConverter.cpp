/*
**
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#define LOG_TAG "ColorConverterJNI"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
#include <libyuv.h>

using namespace android;

#define UNUSED(x) (void)(x)
static const char* const kClassPathName = "android/media/MediaColorConverter";

void cropYUV420SP(uint8_t* srcBuffer, int srcWidth, int srcHeight, int srcActualWidth,
                  int srcActualHeight, uint8_t* dstBuffer, int offset_x, int offset_y, int dstWidth, int dstHeight)
{
    if(offset_x + srcActualWidth > dstWidth || offset_y + srcActualHeight > dstHeight)
    {
        ALOGE("cropYUV420SP param error: offset_x(%d), offset_y(%d), srcActualWidth(%d), srcActualHeight(%d), dstWidth(%d), dstHeight(%d)",
              offset_x, offset_y, srcActualWidth, srcActualHeight, dstWidth, dstHeight);
    }

    for(int i=0; i<srcActualHeight; i++)
    {
        memcpy(dstBuffer+offset_x + offset_y*dstWidth + i*dstWidth, srcBuffer+i*srcWidth, srcActualWidth);
        if(i%2==0)
        {
            memcpy(dstBuffer+dstWidth*dstHeight + offset_x + offset_y/2*dstWidth + i/2*dstWidth, srcBuffer+srcWidth*srcHeight + i/2*srcWidth, srcActualWidth);
        }
    }
}

void android_media_MediaColorConverter_native_NV12ToARGB(JNIEnv * env,
        jobject thiz, jbyteArray buf, jint width, jint height, jint actual_width, jint actual_height, jintArray rgbarray)
{
    UNUSED(thiz);
    uint8_t* yuvFrame = (uint8_t*)env->GetByteArrayElements(buf, 0);
    uint8_t* rgb= (uint8_t*)env->GetIntArrayElements(rgbarray, 0);
    if(actual_width<width || actual_height<height)
    {
        uint8_t yuv[actual_width*actual_height + actual_width*actual_height/2];
        cropYUV420SP(yuvFrame, width, height, actual_width, actual_height, yuv, 0, 0, actual_width, actual_height);
        libyuv::NV12ToARGB(yuv, actual_width, yuv+actual_width*actual_height, actual_width, rgb, actual_width*4, actual_width, actual_height);
    }
    else
    {
        libyuv::NV12ToARGB(yuvFrame, width, yuvFrame+width*height, width, rgb, width*4, width, height);
    }
    env->ReleaseByteArrayElements(buf, (jbyte*)yuvFrame, 0);
    env->ReleaseIntArrayElements(rgbarray, (jint*)rgb, 0);
}

// JNI mapping between Java methods and native methods
static const JNINativeMethod nativeMethods[] =
{
    {"nativeNV12ToARGB","([BIIII[I)V", (void*) android_media_MediaColorConverter_native_NV12ToARGB},
};

// This function only registers the native methods, and is called from
// JNI_OnLoad in android_media_MediaPlayer.cpp
int register_android_media_MediaColorConverter(JNIEnv *env)
{
    return AndroidRuntime::registerNativeMethods
           (env, kClassPathName, nativeMethods, NELEM(nativeMethods));
}
