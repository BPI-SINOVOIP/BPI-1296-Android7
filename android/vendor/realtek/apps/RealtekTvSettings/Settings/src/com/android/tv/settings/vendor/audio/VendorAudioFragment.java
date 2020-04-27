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

package com.android.tv.settings.vendor.audio;

import android.os.Bundle;
import android.os.SystemProperties;
import android.provider.Settings;
import android.support.v17.preference.LeanbackPreferenceFragment;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.TwoStatePreference;
import android.text.TextUtils;
import android.util.Log;
import com.android.tv.settings.R;
import com.realtek.hardware.RtkAoutUtilManager;

public class VendorAudioFragment extends LeanbackPreferenceFragment implements
        Preference.OnPreferenceChangeListener{

    private static final String TAG = "VendorAudioFragment";
    private static final int AUDIO_DIGITAL_RAW = 0;
    private static final int AUDIO_DIGITAL_LPCM_DUAL_CH = 1;
    private static final int AUDIO_DIGITAL_LPCM_MULTI_CH = 2;
    private static final int AUDIO_DIGITAL_AUTO = 3;
    private static final int AUDIO_UNKNOWN_TYPE = 0; //don't care
    private static final int AUDIO_AC3_DECODER_TYPE = 2; //SD audio
    private static final int AUDIO_MLP_DECODER_TYPE = 9; //HD audio
    private static final String KEY_SOUND_PLAYBACK_EFFECT_MODE = "sound_playback_effect";
    private static final String KEY_HDMI_OUTPUT = "hdmi_output";
    private static final String KEY_SPDIF_OUTPUT = "spdif_output";
    private static final String KEY_HDMIRX_AUDIO_FORMAT = "hdmirx_audio";
    private static final String HDMIRX_AUDIO_FORMAT_PROPERTY = "persist.rtk.hdmirx.raw";
    private static final String FORCE_SD_AUDIO_MODE_KEY = "force_sd_audio_mode";
//    private static final String KEY_AUDIO_I2S_IN = "audio_i2s_in_mode";
//    private static final String PROPERTY_AUDIO_I2S_IN = "persist.rtk.audio.in.i2s";
    private static final String KEY_AUDIO_IN_MODE = "audio_in_mode";
    private static final String PROPERTY_AUDIO_IN_MODE = "persist.rtk.audio.in";
    private ListPreference mHdmiOutputMode;
    private ListPreference mSpdifOutputMode;
    private ListPreference mSoundPlaybackEffectMode;
    private ListPreference mHdmiRxAudioFormat;
    private TwoStatePreference mForceSdAudioMode;
//    private TwoStatePreference mAudioI2sInMode;
    private ListPreference mAudioInMode;
    private RtkAoutUtilManager mRtkAoutUtilManager;

    public static VendorAudioFragment newInstance() {
        return new VendorAudioFragment();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        mRtkAoutUtilManager = new RtkAoutUtilManager();
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        setPreferencesFromResource(R.xml.vendor_audio, null);
        mHdmiOutputMode = addListPreference(KEY_HDMI_OUTPUT);
        mSpdifOutputMode = addListPreference(KEY_SPDIF_OUTPUT);
        mSoundPlaybackEffectMode = addListPreference(KEY_SOUND_PLAYBACK_EFFECT_MODE);
        mHdmiRxAudioFormat = addListPreference(KEY_HDMIRX_AUDIO_FORMAT);
        mForceSdAudioMode = findAndInitSwitchPref(FORCE_SD_AUDIO_MODE_KEY);
//        mAudioI2sInMode = findAndInitSwitchPref(KEY_AUDIO_I2S_IN);
        mAudioInMode = addListPreference(KEY_AUDIO_IN_MODE);

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
        updateHdmiOutputMode();
        updateSpdifOutputMode();
        updateSoundPlaybackEffectMode();
        updateHdmiRxAudioFormat();
        updateForceSdAudio();
//        updateAudioI2sInMode();
        updateAudioInModeOptions();
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
//        Log.d(TAG, "onPreferenceTreeClick(), p=" + preference.getKey());
        String key = preference.getKey();
        switch (key) {
            case FORCE_SD_AUDIO_MODE_KEY:
                writeForceSdAudio();
                break;
//            case KEY_AUDIO_I2S_IN:
//                writeAudioI2sInMode();
//                break;
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
        switch (key) {
            case KEY_HDMI_OUTPUT:
                writeHdmiOutputMode(newValue);
                break;
            case KEY_SPDIF_OUTPUT:
                writeSpdifOutputMode(newValue);
                break;
            case KEY_SOUND_PLAYBACK_EFFECT_MODE:
                writeSoundPlaybackEffectMode(newValue);
                break;
            case KEY_HDMIRX_AUDIO_FORMAT:
                writeHdmiRxAudioFormat(newValue);
                break;
            case KEY_AUDIO_IN_MODE:
                writeAudioInModeOptions(newValue);
                break;
        }
        return true;
    }

    private void writeHdmiOutputMode(Object newValue) {
        final String str = (String) newValue;
        Settings.System.putString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_HDMI_OUTPUT_MODE, str);

        Log.d(TAG, "Set HDMI Output mode = " + str);
        mHdmiOutputMode.setSummary(getResources().getStringArray(R.array.hdmi_output_entries)[Integer.parseInt(str)]);
        if ( str.equals("0") ) { //RAW
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_RAW);
            }
        } else if ( str.equals("1") ) { //LPCM 2Ch
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_LPCM_DUAL_CH);
            }
        } else if ( str.equals("2") ) { //LPCM Multi-Ch
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_LPCM_MULTI_CH);
            }
        } else if ( str.equals("3") ) { //AUTO
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_AUTO);
            }
        } else {
            Log.d(TAG, "Unsupportd HDMI Output");
        }
    }

    private void updateHdmiOutputMode() {
        mHdmiOutputMode.setValue(getDefaultValueOfHdmiOutput());
    }

    private void writeSpdifOutputMode(Object newValue) {
        final String str = (String) newValue;
        Settings.System.putString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_SPDIF_OUTPUT_MODE, str);

        Log.d(TAG, "Set SPDIF Output mode = " + str);
        if ( str.equals("0") ) { //LPCM
            mSpdifOutputMode.setSummary(getResources().getStringArray(R.array.spdif_output_entries)[0]);
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioSpdifOutput(AUDIO_DIGITAL_LPCM_DUAL_CH);
            }
        } else if ( str.equals("1") ) { //Raw
            mSpdifOutputMode.setSummary(getResources().getStringArray(R.array.spdif_output_entries)[1]);
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioSpdifOutput(AUDIO_DIGITAL_RAW);
            }
        } else {
            Log.d(TAG, "Unsupportd S/PDIF Output");
        }
    }

    private void updateSpdifOutputMode() {
        mSpdifOutputMode.setValue(getDefaultValueOfSpdifOutput());
    }

    private void writeSoundPlaybackEffectMode(Object newValue) {
        final String str = (String) newValue;
        Settings.System.putString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_PLAYBACK_EFFECT_MODE, str);

        Log.d(TAG, "Set Playback effect mode = " + str);
        int agcMode = 0;
        if ( str.equals("night") ) {
            agcMode=1;
            mSoundPlaybackEffectMode.setSummary("Night Mode");
            if (mRtkAoutUtilManager != null) {
                Log.d(TAG, "Set Playback effect mode = " + agcMode);
                mRtkAoutUtilManager.setAudioAGC(agcMode);
            }
        } else if ( str.equals("comfort") ) {
            agcMode=2;
            mSoundPlaybackEffectMode.setSummary("Comfort Mode");
            if (mRtkAoutUtilManager != null) {
                Log.d(TAG, "Set Playback effect mode = " + agcMode);
                mRtkAoutUtilManager.setAudioAGC(agcMode);
            }
        } else {
            agcMode=0;
            mSoundPlaybackEffectMode.setSummary("OFF");
            if (mRtkAoutUtilManager != null) {
                Log.d(TAG, "Set Playback effect mode = " + agcMode);
                mRtkAoutUtilManager.setAudioAGC(agcMode);
            }
        }
    }

    private void updateSoundPlaybackEffectMode() {
        mSoundPlaybackEffectMode.setValue(getDefaultValueOfPlaybackEffect());
    }

    private void writeHdmiRxAudioFormat(Object newValue) {
        final String str = (String) newValue;

        Log.d(TAG, "Set HDMI Rx Audio format = " + str);
        SystemProperties.set(HDMIRX_AUDIO_FORMAT_PROPERTY, str);
        mHdmiRxAudioFormat.setSummary(getResources().getStringArray(R.array.hdmirx_audio_entries)[Integer.parseInt(str)]);
    }

    private void updateHdmiRxAudioFormat() {
        mHdmiRxAudioFormat.setValue(getDefaultValueOfHdmiRxAudioFormat());
    }

    private void updateForceSdAudio() {
        updateSwitchPreference(mForceSdAudioMode, Settings.System.getInt(
                getActivity().getContentResolver(),
                Settings.System.REALTEK_SETUP_FORCE_SDAUDIO, 0) != 0);
    }

    private void writeForceSdAudio() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.REALTEK_SETUP_FORCE_SDAUDIO,
                mForceSdAudioMode.isChecked() ? 1 : 0);
        int audioMode = AUDIO_UNKNOWN_TYPE;
        if (mForceSdAudioMode.isChecked()) {
            audioMode = AUDIO_AC3_DECODER_TYPE;
        }
        mRtkAoutUtilManager.setAudioSurroundSoundMode(audioMode);
    }

//    private void writeAudioI2sInMode() {
//        SystemProperties.set(PROPERTY_AUDIO_I2S_IN, mAudioI2sInMode.isChecked() ? "1" : "0");
//        SystemProperties.set("persist.rtk.audio.period_size", mAudioI2sInMode.isChecked() ? "1024" : "384");
//    }

//    private void updateAudioI2sInMode() {
//        updateSwitchPreference(mAudioI2sInMode, "1".equals(SystemProperties.get(PROPERTY_AUDIO_I2S_IN,"0")));
//    }

    private String getDefaultValueOfPlaybackEffect() {
        Log.d(TAG, "getDefaultValueOfPlaybackEffect()...");
        String res;
        res = Settings.System.getString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_PLAYBACK_EFFECT_MODE);
        return res;
    }

    private String getDefaultValueOfHdmiOutput() {
        String res;
        res = Settings.System.getString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_HDMI_OUTPUT_MODE);
        Log.d(TAG, "getDefaultValueOfHdmiOutput(), ret=" + res);
        return res;
    }

    private String getDefaultValueOfSpdifOutput() {
        String res;
        res = Settings.System.getString(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_SPDIF_OUTPUT_MODE);
        Log.d(TAG, "getDefaultValueOfSpdifOutput(), ret=" + res);
        return res;
    }

    private String getDefaultValueOfHdmiRxAudioFormat() {
        String res;
        res = SystemProperties.get(HDMIRX_AUDIO_FORMAT_PROPERTY, "0");
        return res;
    }

    //HDMI:0  I2S:1  ADC:3
    private void writeAudioInModeOptions(Object newValue) {
        SystemProperties.set(PROPERTY_AUDIO_IN_MODE, newValue == null ? "0" : newValue.toString());
        updateAudioInModeOptions();
    }

    private void updateAudioInModeOptions() {
        String sAudioInMode = SystemProperties.get(PROPERTY_AUDIO_IN_MODE, "0");
        CharSequence[] entries = mAudioInMode.getEntries();
        CharSequence[] values = mAudioInMode.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if ((values[i].toString()).equals(sAudioInMode)) {
                mAudioInMode.setValueIndex(i);
                mAudioInMode.setSummary(entries[i]);
                return;
            }
        }
        mAudioInMode.setValueIndex(0);
        mAudioInMode.setSummary(entries[0]);
    }

}
