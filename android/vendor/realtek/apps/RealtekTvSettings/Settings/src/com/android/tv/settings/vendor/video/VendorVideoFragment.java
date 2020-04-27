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

package com.android.tv.settings.vendor.video;

import android.app.AlertDialog;
import android.content.*;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.SystemProperties;
import android.provider.Settings;
import android.support.v17.preference.LeanbackPreferenceFragment;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.TwoStatePreference;
import android.text.TextUtils;
import android.util.Log;
import com.android.tv.settings.R;

import com.realtek.hardware.RtkHDMIManager;
import com.realtek.hardware.RtkTVSystem;
import com.realtek.hardware.RtkVoutUtilManager;

public class VendorVideoFragment extends LeanbackPreferenceFragment implements
        Preference.OnPreferenceChangeListener{

    private static final String TAG = "VendorVideoFragment";
    private static final String KEY_TV_SYSTEM = "tv_system";
    private static final String KEY_PEAK_LUMINANCE = "peak_luminance";
    private static final String PROPERTY_PEAK_LUMINANCE = "persist.rtk.video.peakluminance";
    private static final String KEY_VIDEO_COLOR_MODE = "video_color_mode";
    private static final String PROPERTY_VIDEO_COLOR_MODE = "persist.rtk.video.colormode";
    private static final String KEY_AUTO_1080P24 = "auto_1080p24_onoff";
    private static final String KEY_CVBS_ONOFF = "cvbs_onoff";
    private static final String KEY_AUTO_2997_MODE = "auto_2997_mode";
    private static final String PROPERTY_SCREEN_ZOOM_IN = "persist.rtk.vout.zoom";
    private static final String PROPERTY_SCREEN_ZOOM_IN_RATIO = "persist.rtk.vout.zoom.ratio";
    private static final String KEY_SCREEN_ZOOM_IN = "screen_zoom_in_mode";
    private static final String KEY_IGNORE_EDID_MODE = "ignore_edid_mode";

    private ListPreference mTVSystemPreference;
    private ListPreference mPeakLuminancePreference;
    private ListPreference mVideoColorMode;
    private TwoStatePreference mAuto1080p24OnOff;
    private TwoStatePreference mCvbsOnOffPreference;
    private TwoStatePreference mYstMode;
    private TwoStatePreference mAuto2997Mode;
    private TwoStatePreference mScreenZoomIn;
    private RtkHDMIManager mRtkHDMIManager;
    private int oldVideoSystem = 0;
    private int valueToBeStoredForTVSystem = 0;
    private AlertDialog countDlg;
    private AlertDialog mCountDialog;
    private CountDownTimer timer;
    private int timeToWait = 15000; //15 sec

    private final BroadcastReceiver mHdmiPluggedeceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            boolean hdmiPlugged = intent.getBooleanExtra("state", false);
            Log.d(TAG, "HDMI Plugged event=" + hdmiPlugged);
            if (!hdmiPlugged) {
                prepareAvailableItemsForTVSystem();
                getFragmentManager().popBackStack();
            }
        }
    };

    public static VendorVideoFragment newInstance() {
        return new VendorVideoFragment();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        mRtkHDMIManager = new RtkHDMIManager();

        super.onCreate(savedInstanceState);
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        setPreferencesFromResource(R.xml.vendor_video, null);
        mTVSystemPreference = addListPreference(KEY_TV_SYSTEM);
        mTVSystemPreference.setValue(getDefaultValueOfTVSystem());
        prepareAvailableItemsForTVSystem();
        oldVideoSystem = Integer.parseInt(getDefaultValueOfTVSystem());

        mPeakLuminancePreference = addListPreference(KEY_PEAK_LUMINANCE);
        mPeakLuminancePreference.setValue(getPeakLuminanceSetting());

        mVideoColorMode = addListPreference(KEY_VIDEO_COLOR_MODE);

        mAuto1080p24OnOff = findAndInitTwoStatePref(KEY_AUTO_1080P24);
        mCvbsOnOffPreference = findAndInitTwoStatePref(KEY_CVBS_ONOFF);
        mScreenZoomIn = findAndInitTwoStatePref(KEY_SCREEN_ZOOM_IN);
        mYstMode = findAndInitTwoStatePref(KEY_IGNORE_EDID_MODE);
        mAuto2997Mode = findAndInitTwoStatePref(KEY_AUTO_2997_MODE);

        updateAll();
    }

    @Override
    public void onResume() {
        super.onResume();
        prepareAvailableItemsForTVSystem();
        final IntentFilter filter =
                new IntentFilter("android.intent.action.HDMI_PLUGGED");
        Log.d(TAG, "register HDMI_PLUGGED receiver...");
        getActivity().registerReceiver(mHdmiPluggedeceiver, filter);
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "Un-register HDMI_PLUGGED receiver...");
        getActivity().unregisterReceiver(mHdmiPluggedeceiver);
        super.onDestroy();
    }
    private ListPreference addListPreference(String prefKey) {
        ListPreference pref = (ListPreference) findPreference(prefKey);
        pref.setOnPreferenceChangeListener(this);
        return pref;
    }

    private TwoStatePreference findAndInitTwoStatePref(String key) {
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
        updateTVSystemSummary();
        updateVideoColorOptions();
        if (mCvbsOnOffPreference != null) {
            mCvbsOnOffPreference.setChecked(new RtkVoutUtilManager().isCVBSOn());
        }
        try {
            mAuto1080p24OnOff.setChecked(Settings.System.getInt(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_AUTO_1080P24) == 1);
        } catch (Settings.SettingNotFoundException snfe) {
            Log.e(TAG, Settings.System.REALTEK_SETUP_AUTO_1080P24 + " not found");
        }
        updateYstModeOptions();
        updateAuto2997Options();
        updateScreenZoomIn();
    }

    private String getPeakLuminanceSetting() {
        return Integer.toString(SystemProperties.getInt(PROPERTY_PEAK_LUMINANCE, 0));
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
//        Log.d(TAG, "onPreferenceTreeClick(), p=" + preference.getKey());
        String key = preference.getKey();
        switch (key) {
            case KEY_AUTO_1080P24:
                Settings.System.putInt(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_AUTO_1080P24, (mAuto1080p24OnOff.isChecked())?1:0);
                break;
            case KEY_AUTO_2997_MODE:
                writeAuto2997Options();
                break;
            case KEY_CVBS_ONOFF:
                new RtkVoutUtilManager().setCVBSOff(mCvbsOnOffPreference.isChecked()?0:1);
                break;
            case KEY_IGNORE_EDID_MODE:
                writeYstModeOptions();
                break;
            case KEY_SCREEN_ZOOM_IN:
                writeScreenZoomIn();
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
        if (TextUtils.equals(key, KEY_TV_SYSTEM)) {
            valueToBeStoredForTVSystem = Integer.parseInt((String) newValue);
            Log.d(TAG, "new TvSystem=" + newValue);
            if ((valueToBeStoredForTVSystem >= 0) && (valueToBeStoredForTVSystem != oldVideoSystem)) {
                if(mRtkHDMIManager != null) {
                    Log.d(TAG, "Call RtkHDMIManager to set HDMI TV System:" + valueToBeStoredForTVSystem);
                    mRtkHDMIManager.setTVSystem(valueToBeStoredForTVSystem);
                }
                if (countDlg != null) dismissDialogs();

                timer = new CountDownTimer(timeToWait, 1000) {
                    String str = getString(R.string.tv_system_confirm_message);
                    @Override
                    public void onTick(long millisUntilFinished) {
                        //wait 2 sec
                        if ((timeToWait - millisUntilFinished) < 2000) {
                            return;
                        }

                        if (countDlg == null) {
                            countDlg = new AlertDialog.Builder(getActivity()).create();
                            countDlg.setTitle(getString(R.string.tv_system_confirm_title));
                            countDlg.setCancelable(false);
                            countDlg.setMessage(String.format(getString(R.string.tv_system_confirm_message), timeToWait));
                            countDlg.setButton(getString(R.string.yes), new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    String mystr = getDefaultValueOfTVSystem();
                                    Log.d(TAG, "TV System Setting(DB)=" + mystr);
                                    Log.d(TAG, "Save TV System Setting=" + valueToBeStoredForTVSystem);
                                    Settings.System.putInt(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER, valueToBeStoredForTVSystem);
                                    oldVideoSystem = valueToBeStoredForTVSystem;
                                    timer.cancel();
                                    updateTVSystemSummary();
                                }
                            });
                            countDlg.setOnCancelListener(new DialogInterface.OnCancelListener() {
                                @Override
                                public void onCancel(DialogInterface dialog) {
                                    Log.d(TAG, "Dialog has been cancel?");
                                    if (timer != null) timer.cancel();
                                    mRtkHDMIManager.setTVSystem(oldVideoSystem);
                                }
                            });
                            countDlg.show();
                        }

                        if (countDlg != null) {
                            countDlg.setMessage(String.format(str, (millisUntilFinished/1000)));
                            Log.d(TAG, "TV system will restore to previous setting in " + (millisUntilFinished/1000) + " seconds");
                        } else {
                            Log.d(TAG, "Dialog is not exist. Timer should stop");
                        }
                    }

                    @Override
                    public void onFinish() {
                        Log.d(TAG, "Timer finish...");
                        if(mRtkHDMIManager != null) {
                            if(mRtkHDMIManager != null) {
                                Log.d(TAG, "Call RtkHDMIManager to restore the old HDMI TV System:" + oldVideoSystem);
                                mRtkHDMIManager.setTVSystem(oldVideoSystem);
                            }
                            if(mTVSystemPreference != null) {
                                Log.d(TAG, "Set HDMI default selection");
                                mTVSystemPreference.setValue(getDefaultValueOfTVSystem());  // set default selection.
                            }
                        }
                        if (countDlg != null) dismissDialogs();
                    }
                }.start();

            }
            return true;
        }
        else if (TextUtils.equals(key, KEY_PEAK_LUMINANCE)) {
            int mNewLevel = Integer.parseInt((String) newValue);
            new RtkVoutUtilManager().setPeakLuminance(mNewLevel);
            SystemProperties.set(PROPERTY_PEAK_LUMINANCE, Integer.toString(mNewLevel));
            return true;
        }
        else if (TextUtils.equals(key, KEY_VIDEO_COLOR_MODE)) {
            writeVideoColorOptions(newValue);
            return true;
        }
        return false;
    }

    private void prepareAvailableItemsForTVSystem() {
        CharSequence[][] availableTvSystems = {
                {"AUTO", String.valueOf(RtkTVSystem.TV_SYS_HDMI_AUTO_DETECT)},//"0"
                {"NTSC", String.valueOf(RtkTVSystem.TV_SYS_NTSC)},//"1"
                {"PAL", String.valueOf(RtkTVSystem.TV_SYS_PAL)},//"2"
                {"480P", String.valueOf(RtkTVSystem.TV_SYS_480P)},//"3"
                {"576P", String.valueOf(RtkTVSystem.TV_SYS_576P)},//"4"
                {"720P @ 50Hz", String.valueOf(RtkTVSystem.TV_SYS_720P_50HZ)},//"5"
                //{"720P @ 59Hz", String.valueOf(RtkTVSystem.TV_SYS_720P_59HZ)},//"16"
                {"720P @ 60Hz", String.valueOf(RtkTVSystem.TV_SYS_720P_60HZ)},//"6"
                {"1080I @ 50Hz", String.valueOf(RtkTVSystem.TV_SYS_1080I_50HZ)},//"7"
                //{"1080I @ 59Hz", String.valueOf(RtkTVSystem.TV_SYS_1080I_59HZ)},//"17"
                {"1080I @ 60Hz", String.valueOf(RtkTVSystem.TV_SYS_1080I_60HZ)},//"8"
                //{"1080P @ 23Hz", String.valueOf(RtkTVSystem.TV_SYS_1080P_23HZ)},//"18"
                {"1080P @ 24Hz", String.valueOf(RtkTVSystem.TV_SYS_1080P_24HZ)},//"15"
                {"1080P @ 25HZ", String.valueOf(RtkTVSystem.TV_SYS_1080P_25HZ)},//29
                {"1080P @ 30HZ", String.valueOf(RtkTVSystem.TV_SYS_1080P_30HZ)},//30
                {"1080P @ 50Hz", String.valueOf(RtkTVSystem.TV_SYS_1080P_50HZ)},//"9"
                //{"1080P @ 59Hz", String.valueOf(RtkTVSystem.TV_SYS_1080P_59HZ)},//"19"
                {"1080P @ 60Hz", String.valueOf(RtkTVSystem.TV_SYS_1080P_60HZ)},//"10"
                //{"3840x2160P @ 23Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_23HZ)},//"20"
                {"3840x2160P @ 24Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_24HZ)},//"11"
                {"3840x2160P @ 25Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_25HZ)},//"12"
                //{"3840x2160P @ 29Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_29HZ)},//"21"
                {"3840x2160P @ 30Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_30HZ)},//"13"
                {"3840x2160P @ 50Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_50HZ)},//"23"
                //{"3840x2160P @ 59HZ", String.valueOf(RtkTVSystem.TV_SYS_2160P_59HZ)},//28
                {"3840x2160P @ 60Hz", String.valueOf(RtkTVSystem.TV_SYS_2160P_60HZ)},//"22"
                {"4096x2160P @ 24Hz", String.valueOf(RtkTVSystem.TV_SYS_4096_2160P_24HZ)},//"14"
                {"4096x2160P @ 25Hz", String.valueOf(RtkTVSystem.TV_SYS_4096_2160P_25HZ)},//"26"
                {"4096x2160P @ 30Hz", String.valueOf(RtkTVSystem.TV_SYS_4096_2160P_30HZ)},//"27"
                {"4096x2160P @ 50Hz", String.valueOf(RtkTVSystem.TV_SYS_4096_2160P_50HZ)},//"24"
                {"4096x2160P @ 60Hz", String.valueOf(RtkTVSystem.TV_SYS_4096_2160P_60HZ)},//"25"
        };

        if (mRtkHDMIManager != null &&
                mRtkHDMIManager.checkIfHDMIPlugged() &&
                mRtkHDMIManager.getHDMIEDIDReady() &&
                (Settings.System.getInt(getActivity().getContentResolver(),Settings.System.REALTEK_YST_MODE, 0) == 0)
                ) {

            int[] supportVideoFormat = mRtkHDMIManager.getVideoFormat();
            int numSupportVideoFormat = 0;

            for(int i=0; i< supportVideoFormat.length; i++) {
                Log.d(TAG,"VideoFormat["+i+"] : "+supportVideoFormat[i]);
                if ( supportVideoFormat[i] == 1)
                    numSupportVideoFormat++;
            }
            Log.d(TAG,"Num of SupoortVideoFormat:"+numSupportVideoFormat);
            CharSequence[] entries = new CharSequence[numSupportVideoFormat+1];
            CharSequence[] entryValues = new CharSequence[numSupportVideoFormat+1];

            int j=0;
            for(int i=0; i< availableTvSystems.length; i++) {
                int tvSysId = Integer.parseInt(availableTvSystems[i][1].toString());
                if ( (i ==0) || ((tvSysId < supportVideoFormat.length) && (supportVideoFormat[tvSysId] == 1))) {
                    entries[j] = availableTvSystems[i][0];
                    entryValues[j] = availableTvSystems[i][1];
                    j++;
                }
            }

            for(int i=0; i< entries.length; i++) {
                Log.d(TAG, entries[i] + " : " + entryValues[i]);
            }
            mTVSystemPreference.setEntries(entries);
            mTVSystemPreference.setEntryValues(entryValues);

        } else {
            Log.d(TAG, "List all TV Systems...");
            CharSequence[] entries = new CharSequence[availableTvSystems.length-1];
            CharSequence[] entryValues = new CharSequence[availableTvSystems.length-1];
            int j=0;
            for(int i=1; i< availableTvSystems.length; i++) {
                entries[j] = availableTvSystems[i][0];
                entryValues[j] = availableTvSystems[i][1];
                j++;
            }

            for(int i=0; i< entries.length; i++) {
                Log.d(TAG, entries[i] + " : " + entryValues[i]);
            }
            mTVSystemPreference.setEntries(entries);
            mTVSystemPreference.setEntryValues(entryValues);
        }
    }

    private String getDefaultValueOfTVSystem() {
        Integer oldTVSystem;
        String finalRes;

        try {
            oldTVSystem = Settings.System.getInt(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER);

            Log.d(TAG, "getDefaultValueOfTVSystem() = " + oldTVSystem);
            finalRes = Integer.toString(oldTVSystem);
        } catch (Settings.SettingNotFoundException snfe) {
            Log.d(TAG, "Exception found in getDefaultValueOfTVSystem()");
            finalRes = "0";
            //return null;
        }
        return finalRes;
    }

    private void dismissDialogs() {
        Log.d(TAG, "dismissDialogs...");
        if (countDlg != null) {
            Log.d(TAG, "Stop timer...");
            if (timer != null) timer.cancel();
            countDlg.dismiss();
            countDlg = null;
        }
        if (mCountDialog != null) {
            if (timer != null) timer.cancel();
            mCountDialog.dismiss();
            mCountDialog = null;
            updateVideoColorOptions();
        }
    }

    private void updateTVSystemSummary() {
        if (mTVSystemPreference != null) {

            try {
                int defaultSet = Settings.System.getInt(getActivity().getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER);
                final CharSequence[] entries = mTVSystemPreference.getEntries();
                final CharSequence[] entryValues = mTVSystemPreference.getEntryValues();
                for (int i=0; i< entryValues.length; i++) {
                    if (Integer.parseInt(entryValues[i].toString()) == defaultSet) {
                        mTVSystemPreference.setSummary(entries[i]);
                    }
                }
            } catch (Settings.SettingNotFoundException snfe) {
                Log.d(TAG, "Setting Not Found");
            }
        }
    }

    private void writeVideoColorOptions(Object newValue) {
        Log.d(TAG, "New Video Color Mode = " + newValue);
        String colorModeNew = newValue != null ? (newValue.toString()) : "0";
        String colorModeOld = SystemProperties.get(PROPERTY_VIDEO_COLOR_MODE, "0");

        int isSupported = mRtkHDMIManager.checkTVSystem(mRtkHDMIManager.getTVSystem(), Integer.parseInt(colorModeNew));
        if ("0".equals(colorModeNew) ||isSupported == 0) {
            Log.d(TAG, "checkTVSystem return OK.");
            SystemProperties.set(PROPERTY_VIDEO_COLOR_MODE, colorModeNew);
            setVideoColorMode(Integer.parseInt(colorModeNew));
            updateVideoColorOptions();
        } else {
            Log.d(TAG, "[Warning] checkTVSystem get Error=" + isSupported + ", TVSystem=" + mRtkHDMIManager.getTVSystem() + ", colorMode=" + colorModeNew);
            confirmForceSetVideoColorMode(colorModeOld, colorModeNew);
        }
    }

    private void confirmForceSetVideoColorMode(final String oldColor, final String newColor) {
        DialogInterface.OnClickListener onConfirmListener = new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (which == DialogInterface.BUTTON_POSITIVE) {
                    setVideoColorMode(Integer.parseInt(newColor));
                    countdownWindow(oldColor, newColor);
                } else {
                    updateVideoColorOptions();
                }
            }
        };

        new AlertDialog.Builder(getActivity())
                .setTitle(R.string.confirm_force_set_video_color_title)
                .setMessage(R.string.confirm_force_set_video_color_text)
                .setPositiveButton(R.string.yes, onConfirmListener)
                .setNegativeButton(android.R.string.no, onConfirmListener)
                .create()
                .show();
    }

    private void countdownWindow(final String oldColor, final String newColor) {
        DialogInterface.OnClickListener onConfirmListener = new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                timer.cancel();
                SystemProperties.set(PROPERTY_VIDEO_COLOR_MODE, newColor);
                updateVideoColorOptions();
            }
        };

        if (mCountDialog != null) dismissDialogs();
        int timeToWait = 15000;
        mCountDialog = new AlertDialog.Builder(getActivity()).setMessage(
                String.format(getString(R.string.tv_system_confirm_message), timeToWait))
                .setTitle(R.string.tv_system_confirm_title)
                .setPositiveButton(R.string.yes, onConfirmListener)
                .setCancelable(false)
                .show();

        timer = new CountDownTimer(timeToWait, 1000) {
            String str = getString(R.string.tv_system_confirm_message);
            @Override
            public void onTick(long millisUntilFinished) {
                if (mCountDialog != null) {
                    mCountDialog.setMessage(String.format(str, (millisUntilFinished/1000)));
                    Log.d(TAG, "Video Color Mode will restore to previous setting in " + (millisUntilFinished/1000) + " seconds");
                } else {
                    Log.d(TAG, "Dialog is not exist. Timer should stop");
                }
            }

            @Override
            public void onFinish() {
                Log.d(TAG, "Timer finish...");
                //restore to old color settings
                setVideoColorMode(Integer.parseInt(oldColor));
                SystemProperties.set(PROPERTY_VIDEO_COLOR_MODE, oldColor);
                if (mCountDialog != null) dismissDialogs();
            }
        }.start();

    }

    private void setVideoColorMode(int colorMode) {
        Log.d(TAG, "Set Video Color Mode = " + colorMode);
        mRtkHDMIManager.setHDMIVideoColorMode(colorMode);
    }

    private void updateVideoColorOptions() {
        int iVideoColorMode = Integer.parseInt(SystemProperties.get(PROPERTY_VIDEO_COLOR_MODE, "0"));
        mVideoColorMode.setValueIndex(iVideoColorMode);
        mVideoColorMode.setSummary(mVideoColorMode.getEntries()[iVideoColorMode]);
    }

    private void updateYstModeOptions() {
        updateSwitchPreference(mYstMode, Settings.System.getInt(
                getActivity().getContentResolver(),
                Settings.System.REALTEK_YST_MODE, 0) != 0);
    }

    private void writeYstModeOptions() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.REALTEK_YST_MODE,
                mYstMode.isChecked() ? 1 : 0);
        prepareAvailableItemsForTVSystem();
    }

    private void updateAuto2997Options() {
        updateSwitchPreference(mAuto2997Mode, Settings.System.getInt(
                getActivity().getContentResolver(),
                Settings.System.REALTEK_SETUP_AUTO_2997HZ, 0) != 0);
    }

    private void writeAuto2997Options() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.REALTEK_SETUP_AUTO_2997HZ,
                mAuto2997Mode.isChecked() ? 1 : 0);
    }

    private void writeScreenZoomIn() {
        SystemProperties.set(PROPERTY_SCREEN_ZOOM_IN, mScreenZoomIn.isChecked() ? "1" : "0");
        if (mScreenZoomIn.isChecked()) {
            int zoom_ratio = Integer.parseInt(SystemProperties.get(PROPERTY_SCREEN_ZOOM_IN_RATIO,"95"));
            Log.d(TAG, "Set default Screen Zoom to " + zoom_ratio + "%");
            new RtkVoutUtilManager().setDisplayRatio(zoom_ratio);  //default 95% Zoom
        } else {
            new RtkVoutUtilManager().setDisplayRatio(100);
        }
    }
    private void updateScreenZoomIn() {
        updateSwitchPreference(mScreenZoomIn, "1".equals(SystemProperties.get(PROPERTY_SCREEN_ZOOM_IN,"0")));
    }

}
