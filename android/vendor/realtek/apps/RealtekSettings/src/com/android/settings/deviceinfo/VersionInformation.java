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

package com.android.settings.deviceinfo;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.support.v7.preference.Preference;
import android.preference.PreferenceActivity;
import android.support.v7.preference.PreferenceScreen;
import android.support.v7.preference.PreferenceGroup;
import android.telephony.ServiceState;
import android.text.TextUtils;

import com.android.settings.R;
import com.android.settings.Utils;

import java.lang.ref.WeakReference;
import android.widget.Toast;
import android.util.Log;
import java.io.FileReader;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.InputStreamReader;


/**
 * Display the Realtek Version Information
 * # Android
 * # DvdPlayer
 * # FW
 * # Bootcode
 * # Linux Kernel
 * # AndroidDT
 * # RescueDT
 * # AndroidRootfs
 * # RescueRootfs
 * # Mali
 * # NAS
 *
 */
public class VersionInformation extends PreferenceActivity {
    private static final String LOG_TAG = "VersionInformation";

    private static final String KEY_VERSION_INFORMATION_ANDROID = "version_information_android";
    private static final String KEY_VERSION_INFORMATION_DVDPLAYER = "version_information_dvdplayer";
    private static final String KEY_VERSION_INFORMATION_FW = "version_information_fw";
    private static final String KEY_VERSION_INFORMATION_BOOTCODE = "version_information_bootcode";
    private static final String KEY_VERSION_INFORMATION_LINUXKERNEL = "version_information_linuxkernel";
    private static final String KEY_VERSION_INFORMATION_ANDROIDDT = "version_information_androiddt";
    private static final String KEY_VERSION_INFORMATION_RESCUEDT = "version_information_rescuedt";
    private static final String KEY_VERSION_INFORMATION_ANDROIDROOTFS = "version_information_androidrootfs";
    private static final String KEY_VERSION_INFORMATION_RESCUEROOTFS = "version_information_rescuerootfs";
    private static final String KEY_VERSION_INFORMATION_MALI = "version_information_mali";
    private static final String KEY_VERSION_INFORMATION_NAS = "version_information_nas";
/*
    private Preference mVersionInformationAndroid;
    private Preference mVersionInformationDvdPlayer;
    private Preference mVersionInformationFW;
    private Preference mVersionInformationBootcode;
    private Preference mVersionInformationLinuxKernel;
    private Preference mVersionInformationAndroidDT;
    private Preference mVersionInformationRescueDT;
    private Preference mVersionInformationAndroidRootfs;
    private Preference mVersionInformationRescueRootfs;
    private Preference mVersionInformationMali;
    private Preference mVersionInformationNAS;
*/
    private Resources mRes;
    private String sUnknown;

    private String sVersionInfoAndroid;
    private String sVersionInfoDvdPlayer;
    private String sVersionInfoFW;
    private String sVersionInfoBootcode;
    private String sVersionInfoLinuxKernel;
    private String sVersionInfoAndroidDT;
    private String sVersionInfoRescueDT;
    private String sVersionInfoAndroidRootfs;
    private String sVersionInfoRescueRootfs;
    private String sVersionInfoMali;
    private String sVersionInfoNAS;

    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        addPreferencesFromResource(R.xml.version_information);
/*
        mVersionInformationAndroid = findPreference(KEY_VERSION_INFORMATION_ANDROID);
        mVersionInformationDvdPlayer = findPreference(KEY_VERSION_INFORMATION_DVDPLAYER);
        mVersionInformationFW = findPreference(KEY_VERSION_INFORMATION_FW);
        mVersionInformationBootcode = findPreference(KEY_VERSION_INFORMATION_BOOTCODE);
        mVersionInformationLinuxKernel = findPreference(KEY_VERSION_INFORMATION_LINUXKERNEL);
        mVersionInformationAndroidDT = findPreference(KEY_VERSION_INFORMATION_ANDROIDDT);
        mVersionInformationRescueDT = findPreference(KEY_VERSION_INFORMATION_RESCUEDT);
        mVersionInformationAndroidRootfs = findPreference(KEY_VERSION_INFORMATION_ANDROIDROOTFS);
        mVersionInformationRescueRootfs = findPreference(KEY_VERSION_INFORMATION_RESCUEROOTFS);
        mVersionInformationMali = findPreference(KEY_VERSION_INFORMATION_MALI);
        mVersionInformationNAS = findPreference(KEY_VERSION_INFORMATION_NAS);
*/
        mRes = getResources();
        sUnknown = mRes.getString(R.string.device_info_default);
        setSummaryText(KEY_VERSION_INFORMATION_ANDROID, sUnknown);

        if(parseRealtekVersionInformation() == true) {
            Log.d(LOG_TAG, "parseRealtekVersionInformation() == true ");
            setSummaryText(KEY_VERSION_INFORMATION_ANDROID, sVersionInfoAndroid);
            setSummaryText(KEY_VERSION_INFORMATION_DVDPLAYER, sVersionInfoDvdPlayer);
            setSummaryText(KEY_VERSION_INFORMATION_FW, sVersionInfoFW);
            setSummaryText(KEY_VERSION_INFORMATION_BOOTCODE, sVersionInfoBootcode);
            setSummaryText(KEY_VERSION_INFORMATION_LINUXKERNEL, sVersionInfoLinuxKernel);
            setSummaryText(KEY_VERSION_INFORMATION_ANDROIDDT, sVersionInfoAndroidDT);
            setSummaryText(KEY_VERSION_INFORMATION_RESCUEDT, sVersionInfoRescueDT);
            setSummaryText(KEY_VERSION_INFORMATION_ANDROIDROOTFS, sVersionInfoAndroidRootfs);
            setSummaryText(KEY_VERSION_INFORMATION_RESCUEROOTFS, sVersionInfoRescueRootfs);
            setSummaryText(KEY_VERSION_INFORMATION_MALI, sVersionInfoMali);
            setSummaryText(KEY_VERSION_INFORMATION_NAS, sVersionInfoNAS);
        } else {
            Log.d(LOG_TAG, "parseRealtekVersionInformation() == false ");
            setSummaryText(KEY_VERSION_INFORMATION_ANDROID, sVersionInfoAndroid);
        }
    }
  
    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    /**
     * Removes the specified preference, if it exists.
     * @param key the key for the Preference item
     */
    private void removePreferenceFromScreen(String key) {
        if ((findPreference(key)) != null) {
            getPreferenceScreen().removePreference(findPreference(key));
        }
    }

    /**
     * @param preference The key for the Preference item
     * @param property The system property to fetch
     * @param alt The default value, if the property doesn't exist
     */
    private void setSummary(String preference, String property, String alt) {
        try {
            findPreference(preference).setSummary(
                    SystemProperties.get(property, alt));
        } catch (RuntimeException e) {

        }
    }

    private void setSummaryText(String preference, String text) {
            if (TextUtils.isEmpty(text)) {
               text = sUnknown;
             }
             // some preferences may be missing
             if (findPreference(preference) != null) {
                 findPreference(preference).setSummary(text);
             }
    }

    private boolean parseRealtekVersionInformation() {
        FileInputStream fis = null;
        String readStr;
        boolean isParse = false;
        try {
            fis = new FileInputStream("/system/vendor/resource/rtk_version.txt");
            InputStreamReader isr = new InputStreamReader(fis);
            StringBuilder sb = new StringBuilder();
            char[] inputBuf = new char[512];
            int len = 0;

            while((len = isr.read(inputBuf)) != -1) {
                sb.append(inputBuf, 0, len);
            }

            readStr = sb.toString();
            Log.d(LOG_TAG, "rtk_version: " + readStr);
            fis.close();

            String[] resArr = readStr.split("\n");
            //String[] resArr = readStr.split("Realtek Version Information:");

            /*
            *  Android: resArr[1]
            *  DvdPlayer: resArr[2]
            *  FW: resArr[3]
            *  Bootcode: resArr[4]
            *  linux-kernel: resArr[5]
            *  AndroidDT: resArr[6]
            *  RescueDT: resArr[7]
            *  AndroidRootfs: resArr[8]
            *  RescueRootfs: resArr[9]
            *  Mali: resArr[10]
            *  NAS: resArr[11]
            */

            // assign value 
            sVersionInfoAndroid = resArr[1].split(" ")[1];
            sVersionInfoDvdPlayer = resArr[2].split(" ")[1];
            sVersionInfoFW = resArr[3].split(" ")[1];
            sVersionInfoBootcode = resArr[4].split(" ")[1];
            sVersionInfoLinuxKernel = resArr[5].split(" ")[1];
            sVersionInfoAndroidDT = resArr[6].split(" ")[1];
            sVersionInfoRescueDT = resArr[7].split(" ")[1];
            sVersionInfoAndroidRootfs = resArr[8].split(" ")[1];
            sVersionInfoRescueRootfs = resArr[9].split(" ")[1];
            sVersionInfoMali = resArr[10].split(" ")[1];
            sVersionInfoNAS = resArr[11].split(" ")[1];
            isParse = true;

        } catch (Exception e) {
            Log.d(LOG_TAG, "Open /system/vendor/resource/rtk_version.txt failed!");
            sVersionInfoAndroid = "Open /system/vendor/resource/rtk_version.txt failed!";
            isParse = false;

        } finally {
            if (fis != null) {
                fis = null;
            }
        }
        
        return isParse;
    }

}
