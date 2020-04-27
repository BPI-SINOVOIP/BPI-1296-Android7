/*
 * Copyright (C) 2015 The Android Open Source Project
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
 * limitations under the License
 */

package com.android.tv.settings.vendor.others;

import android.os.Bundle;
import android.os.SystemProperties;
import android.provider.Settings;
import android.support.v17.preference.LeanbackPreferenceFragment;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.TwoStatePreference;
import android.text.TextUtils;
import com.android.tv.settings.R;

public class VendorOthersFragment extends LeanbackPreferenceFragment implements
        Preference.OnPreferenceChangeListener{

    private static final String TAG = "VendorOthersFragment";
    private static final String IGNORE_HDMI_PLUGOUT_KEY = "ignore_hdmi_plugout";
    private static final String KEY_VIDEO_MAX_BUFFER = "num_video_buffer";
    private static final String VIDEO_MAXACQCNT_PROPERTY = "persist.rtk.video.maxAcqCnt";
    private static final String DISABLE_TRY_MOUNT_KEY = "disable_try_mount_mode";
    private static final String VOLD_TRYMOUNT_PROPERTY = "persist.rtk.vold.trymount";
    private static final String CHECK_VOLUME_KEY = "check_volume_mode";
    private static final String VOLD_CHECKVOLUME_PROPERTY = "persist.rtk.vold.checkvolume";
    private static final String FORMAT_ON_ERROR_KEY = "format_on_error_mode";
    private static final String VOLD_FORMATONERROR_PROPERTY = "persist.rtk.vold.formatonerror";
    private static final String DEEP_SLEEP_MODE_PROPERTY = "persist.rtk.screenoff.suspend";
    private static final String DEEP_SLEEP_KEY = "deep_sleep_mode";
    private static final String KEY_PERFORMANCE_MODE = "performance_mode";
    private static final String PROPERTY_PERFORMANCE_MODE= "persist.rtk.performancemode";
    private TwoStatePreference mIgnoreHdmiPlugout;
    private TwoStatePreference mVideoMaxBuf;
    private TwoStatePreference mDisableTryMountMode;
    private TwoStatePreference mCheckVolumeMode;
    private TwoStatePreference mFormatOnErrorMode;
    private TwoStatePreference mDeepSleepMode;
    private TwoStatePreference mPerformanceMode;
    private TwoStatePreference mSearchKeyFrameMode;
    private static final String KEY_SEARCH_KEY_FRAME = "search_key_frame_mode";
    private static final String PROPERTY_VIDEO_SEARCHI = "persist.rtk.video.searchi";

    public static VendorOthersFragment newInstance() {
        return new VendorOthersFragment();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        setPreferencesFromResource(R.xml.vendor_other, null);
        mIgnoreHdmiPlugout = findAndInitSwitchPref(IGNORE_HDMI_PLUGOUT_KEY);
        mVideoMaxBuf = findAndInitSwitchPref(KEY_VIDEO_MAX_BUFFER);
        mDisableTryMountMode = findAndInitSwitchPref(DISABLE_TRY_MOUNT_KEY);
        mCheckVolumeMode = findAndInitSwitchPref(CHECK_VOLUME_KEY);
        mFormatOnErrorMode = findAndInitSwitchPref(FORMAT_ON_ERROR_KEY);
        mDeepSleepMode = findAndInitSwitchPref(DEEP_SLEEP_KEY);
        mPerformanceMode = findAndInitSwitchPref(KEY_PERFORMANCE_MODE);
        mSearchKeyFrameMode = findAndInitSwitchPref(KEY_SEARCH_KEY_FRAME);

        updateAll();
    }

    private ListPreference addListPreference(String prefKey) {
        ListPreference pref = (ListPreference) findPreference(prefKey);
        pref.setOnPreferenceChangeListener(this);
        return pref;
    }

    private TwoStatePreference findAndInitSwitchPref(String key) {
        TwoStatePreference pref = (TwoStatePreference) findPreference(key);
        if (pref == null) {
            throw new IllegalArgumentException("Cannot find preference with key = " + key);
        }
        return pref;
    }

    void updateSwitchPreference(TwoStatePreference switchPreference, boolean value) {
        switchPreference.setChecked(value);
    }

    private void updateAll() {
        updateIgnoreHdmiPlugoutOptions();
        updateCheckVolume();
        updateDeepSleepMode();
        updateDisableTryMount();
        updateFormatOnError();
        updateVideoMaxBuf();
        updatePerformanceMode();
        updateSearchKeyFrameMode();
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
//        Log.d(TAG, "onPreferenceTreeClick(), p=" + preference.getKey());
        String key = preference.getKey();
        switch (key) {
            case IGNORE_HDMI_PLUGOUT_KEY:
                writeIgnoreHdmiPlugoutOptions();
                break;
            case KEY_VIDEO_MAX_BUFFER:
                writeVideoMaxBuffer();
                break;
            case DISABLE_TRY_MOUNT_KEY:
                writeDisableTryMount();
                break;
            case CHECK_VOLUME_KEY:
                writeCheckVolume();
                break;
            case FORMAT_ON_ERROR_KEY:
                writeFormatOnError();
                break;
            case DEEP_SLEEP_KEY:
                writeDeepSleepMode();
                break;
            case KEY_PERFORMANCE_MODE:
                writePerformanceMode();
                break;
            case KEY_SEARCH_KEY_FRAME:
                writeSearchKeyFrameMode();
                break;
        }
        return super.onPreferenceTreeClick(preference);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
//        Log.d(TAG, "onPreferenceChange(), p=" + preference.getKey());
        String key = preference.getKey();
        if (TextUtils.isEmpty(key)) {
            throw new IllegalStateException("Unknown preference change");
        }
        return true;
    }

    private void updateIgnoreHdmiPlugoutOptions() {
        updateSwitchPreference(mIgnoreHdmiPlugout, Settings.System.getInt(
                getActivity().getContentResolver(),
                Settings.System.REALTEK_IGNORE_HDMI_PLUGOUT, 0) != 0);
    }

    private void writeIgnoreHdmiPlugoutOptions() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.REALTEK_IGNORE_HDMI_PLUGOUT,
                mIgnoreHdmiPlugout.isChecked() ? 1 : 0);
    }

    private void updateVideoMaxBuf() {
        updateSwitchPreference(mVideoMaxBuf, "1".equals(SystemProperties.get(VIDEO_MAXACQCNT_PROPERTY,"3")));
    }

    private void writeVideoMaxBuffer() {
        SystemProperties.set(VIDEO_MAXACQCNT_PROPERTY, mVideoMaxBuf.isChecked() ? "1" : "3");
    }

    private void writeDisableTryMount() {
        SystemProperties.set(VOLD_TRYMOUNT_PROPERTY, mDisableTryMountMode.isChecked() ? "0" : "1");
    }
    private void updateDisableTryMount() {
        updateSwitchPreference(mDisableTryMountMode, "0".equals(SystemProperties.get(VOLD_TRYMOUNT_PROPERTY,"1")));
    }

    private void writeCheckVolume() {
        SystemProperties.set(VOLD_CHECKVOLUME_PROPERTY, mCheckVolumeMode.isChecked() ? "1" : "0");

        if (mCheckVolumeMode.isChecked()) {
            mDisableTryMountMode.setEnabled(true);
            mFormatOnErrorMode.setEnabled(true);
        } else {
            mDisableTryMountMode.setEnabled(false);
            mFormatOnErrorMode.setEnabled(false);
            SystemProperties.set(VOLD_TRYMOUNT_PROPERTY, "1");
            SystemProperties.set(VOLD_FORMATONERROR_PROPERTY, "0");
            updateFormatOnError();
            updateDisableTryMount();
        }
    }
    private void updateCheckVolume() {
        updateSwitchPreference(mCheckVolumeMode, "1".equals(SystemProperties.get(VOLD_CHECKVOLUME_PROPERTY,"0")));

        if (mCheckVolumeMode.isChecked()) {
            mDisableTryMountMode.setEnabled(true);
            mFormatOnErrorMode.setEnabled(true);
        } else {
            mDisableTryMountMode.setEnabled(false);
            mFormatOnErrorMode.setEnabled(false);
            SystemProperties.set(VOLD_TRYMOUNT_PROPERTY, "1");
            SystemProperties.set(VOLD_FORMATONERROR_PROPERTY, "0");
            updateFormatOnError();
            updateDisableTryMount();
        }
    }

    private void writeFormatOnError() {
        SystemProperties.set(VOLD_FORMATONERROR_PROPERTY, mFormatOnErrorMode.isChecked() ? "1" : "0");
    }
    private void updateFormatOnError() {
        updateSwitchPreference(mFormatOnErrorMode, "1".equals(SystemProperties.get(VOLD_FORMATONERROR_PROPERTY,"0")));
    }

    private void writeDeepSleepMode() {
        SystemProperties.set(DEEP_SLEEP_MODE_PROPERTY, mDeepSleepMode.isChecked() ? "1" : "0");
    }
    private void updateDeepSleepMode() {
        updateSwitchPreference(mDeepSleepMode, "1".equals(SystemProperties.get(DEEP_SLEEP_MODE_PROPERTY,"0")));
    }

    private void writePerformanceMode() {
        SystemProperties.set(PROPERTY_PERFORMANCE_MODE, mPerformanceMode.isChecked() ? "1" : "0");
    }
    private void updatePerformanceMode() {
        updateSwitchPreference(mPerformanceMode, "1".equals(SystemProperties.get(PROPERTY_PERFORMANCE_MODE,"0")));
    }
    private void writeSearchKeyFrameMode() {
        SystemProperties.set(PROPERTY_VIDEO_SEARCHI,mSearchKeyFrameMode.isChecked()? "true":"false");
    }
    private void updateSearchKeyFrameMode() {
        updateSwitchPreference(mSearchKeyFrameMode,"true".equals(SystemProperties.get(PROPERTY_VIDEO_SEARCHI,"false")));
    }
}
