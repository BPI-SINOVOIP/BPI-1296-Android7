/*
 * Copyright (C) 2009 The Android Open Source Project
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

import android.util.Log;
import java.util.HashMap;
import java.util.Map;

/**
   Class to hold the media's media type.

   The caller is expected to know the type of the media and call
   the right get* method to fetch its value.
 */
public final class MediaType {
    private Map<String, Object> mMap;

    /**
     * A key describing the major type of the MediaType.
     * The associated value is a integer.
     */
    public static final String KEY_MAJOR_TYPE = "major-type";

    /**
     * A key describing the sub type of the MediaType.
     * The associated value is a integer.
     */
    public static final String KEY_SUB_TYPE = "sub-type";

    /**
     * Media major and sub type, must be synchronized with EType.h in DvdPlayer
     */
    public static final int MEDIA_TYPE_NONE = 0;
    public static final int MEDIATYPE_DVD = 61;
    public static final int MEDIASUBTYPE_DVD_VIDEO_ROM = 91; // DVD ROM
    public static final int MEDIASUBTYPE_DVD_VIDEO = 90; // Recordable DVD -VIDEO
	public static final int MEDIASUBTYPE_BD = 172;
	public static final int MEDIASUBTYPE_BD_ROM = 173;

    /* package private */ MediaType(Map<String, Object> map) {
        mMap = map;
    }

    /**
     * Creates an empty MediaType
     */
    public MediaType() {
        mMap = new HashMap();
    }

    /**
     * Sets the value of an integer key.
     */
    public final void setInteger(String name, int value) {
        mMap.put(name, new Integer(value));
    }

    /**
     * Creates a minimal audio format.
     * @param majorType The major type of the content.
     * @param subType The sub type of the content.
     */
    public static final MediaType createMediaType(
            int majorType,
            int subType) {
        MediaType type = new MediaType();
        type.setInteger(KEY_MAJOR_TYPE, majorType);
        type.setInteger(KEY_SUB_TYPE, subType);

        return type;
    }


    /**
     * Check if DVD media
     */
    public final boolean isDvdMedia() {
        int majorType = ((Integer)mMap.get(KEY_MAJOR_TYPE)).intValue();
        int subType = ((Integer)mMap.get(KEY_SUB_TYPE)).intValue();
        return (majorType == MEDIATYPE_DVD &&
            (subType == MEDIASUBTYPE_DVD_VIDEO_ROM || subType == MEDIASUBTYPE_DVD_VIDEO));
    }

    /**
     * Check if BD-HDMV media
     */
    public final boolean isBdHdmvMedia() {
        int subType = ((Integer)mMap.get(KEY_SUB_TYPE)).intValue();    
        return (subType == MEDIASUBTYPE_BD_ROM);
    }

    @Override
    public String toString() {
        return mMap.toString();
    }
}
