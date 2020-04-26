/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.media;

/**
 * MediaColorConverter class provides a unified interface for retrieving
 * frame and meta data from an input media file.
 */
public class MediaColorConverter
{
    static
    {
        System.loadLibrary("media_jni");
    }

    // The field below is accessed by native methods
    @SuppressWarnings("unused")

    public MediaColorConverter()
    {
    }

    public void NV12ToARGB(byte[] array, int width, int height, int actual_width, int actual_height, int[] rgb)
    {
        nativeNV12ToARGB(array, width, height, actual_width, actual_height, rgb);
    }

    public native void nativeNV12ToARGB(byte[] array, int width, int height, int actual_width, int actual_height, int[] rgb);

    @Override
    protected void finalize() throws Throwable
    {
        super.finalize();
    }

}
