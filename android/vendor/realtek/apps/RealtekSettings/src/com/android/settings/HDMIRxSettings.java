/*
 * Copyright (C) 2011 The Android Open Source Project
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

package com.android.settings;

import android.os.Bundle;
import android.os.SystemProperties;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.util.Log;
import com.android.internal.logging.MetricsLogger;
import com.android.internal.logging.MetricsProto.MetricsEvent;

public class HDMIRxSettings extends SettingsPreferenceFragment
        implements Preference.OnPreferenceChangeListener  {

    private static final String TAG = "HDMIRxSettings";
    private static final String KEY_HDMIRX_VERSION = "hdmirx_version_list";
    private static final String PROPERTY_HDMIRX_VERSION = "persist.rtk.hdmirx.version";

    private ListPreference mHdmirxVersionList;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()...");
        super.onCreate(savedInstanceState);

        addPreferencesFromResource(R.xml.hdmirx_settings);

        mHdmirxVersionList = (ListPreference) findPreference(KEY_HDMIRX_VERSION);
        mHdmirxVersionList.setOnPreferenceChangeListener(this);
        mHdmirxVersionList.setValue(getDefaultValueOfVersion());
    }

    @Override
    public void onResume() {
        super.onResume();
        mHdmirxVersionList.setSummary(getDefaultValueOfVersion());
    }

    @Override
    public boolean onPreferenceChange(Preference pref, Object newValue) {
        Log.d(TAG, "onPreferenceChange()...");

        if (pref.getKey().equals(KEY_HDMIRX_VERSION)) {
            SystemProperties.set(PROPERTY_HDMIRX_VERSION, (String)newValue);
            // update summary
            mHdmirxVersionList.setSummary((String)newValue);
        }

        return true;
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.MAIN_SETTINGS;
    }

    private String getDefaultValueOfVersion() {
        return SystemProperties.get(PROPERTY_HDMIRX_VERSION,"1.4");
    }

}

