/*
 * Copyright (C) 2012 The Android Open Source Project
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

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

/**
 * Encapsulates the information describing the information of media navigation
 *
 * The format of the navigation data is specified as string/value pairs.
 *
 */
public final class NavigationRecord {
    private Map<String, Object> mMap;

    /**
     * A key describing the total number of titles
     * The associated value is Integer
     */
    public static final String KEY_DOMAIN = "domain";
    
    public static final String KEY_NUM_OF_TITLES = "title-count";

    public static final String KEY_NUM_OF_CHAPTERS = "chapter-count";

    public static final String KEY_NUM_OF_ANGLES = "angle-count";

    public static final String KEY_CURRENT_TITLE = "current-title";

    public static final String KEY_CURRENT_CHAPTER = "current-chapter";

    public static final String KEY_CURRENT_ANGLE = "current-angle";

    /* package private */ NavigationRecord(Map<String, Object> map) {
        mMap = map;
    }

    /**
     * Creates an empty NavigationRecord
     */
    public NavigationRecord() {
        mMap = new HashMap();
    }

    /* package private */ Map<String, Object> getMap() {
        return mMap;
    }

    /**
     * Returns true iff a key of the given name exists in the format.
     */
    public final boolean containsKey(String name) {
        return mMap.containsKey(name);
    }

    /**
     * Returns the value of an integer key.
     */
    public final int getInteger(String name) {
        return ((Integer)mMap.get(name)).intValue();
    }

    /**
     * Returns the value of an integer key, or the default value if the
     * key is missing or is for another type value.
     * @hide
     */
    public final int getInteger(String name, int defaultValue) {
        try {
            return getInteger(name);
        }
        catch (NullPointerException  e) { /* no such field */ }
        catch (ClassCastException e) { /* field of different type */ }
        return defaultValue;
    }

    /**
     * Returns the value of a long key.
     */
    public final long getLong(String name) {
        return ((Long)mMap.get(name)).longValue();
    }

    /**
     * Returns the value of a float key.
     */
    public final float getFloat(String name) {
        return ((Float)mMap.get(name)).floatValue();
    }

    /**
     * Returns the value of a string key.
     */
    public final String getString(String name) {
        return (String)mMap.get(name);
    }


    /**
     * Sets the value of an integer key.
     */
    public final void setInteger(String name, int value) {
        mMap.put(name, new Integer(value));
    }

    /**
     * Sets the value of a long key.
     */
    public final void setLong(String name, long value) {
        mMap.put(name, new Long(value));
    }

    /**
     * Sets the value of a float key.
     */
    public final void setFloat(String name, float value) {
        mMap.put(name, new Float(value));
    }

    /**
     * Sets the value of a string key.
     */
    public final void setString(String name, String value) {
        mMap.put(name, value);
    }

    /**
     * Creates a minimal NavigationRecord
     * @param domain The current playing domain (used in DVD menu)
     * @param numOfTitles The total number of titles
     * @param numOfChapters The total number of chapters
     * @param numOfAngles The total number of angles
     */
    public static final NavigationRecord createNavigationRecord(
            int domain,
            int numOfTitles,
            int numOfChapters,
            int numOfAngles) {
        NavigationRecord record = new NavigationRecord();
        record.setInteger(KEY_DOMAIN, domain);
        record.setInteger(KEY_NUM_OF_TITLES, numOfTitles);
        record.setInteger(KEY_NUM_OF_CHAPTERS, numOfChapters);
        record.setInteger(KEY_NUM_OF_ANGLES, numOfAngles);

        return record;
    }

    /**
     * Return number of key-values mappings
     */
    public final int size() {
        return mMap.size();
    }

    @Override
    public String toString() {
        return mMap.toString();
    }
}
