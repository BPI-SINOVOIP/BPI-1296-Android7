/*
 * Copyright (C) 2007 The Android Open Source Project
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

import android.content.Context;
import android.content.Intent;
import android.database.ContentObserver;
import android.os.Handler;
import android.os.Message;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.support.v7.preference.CheckBoxPreference;
import android.support.v7.preference.Preference;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
//import android.util.Log;

public class DLNADMRModeEnabler implements Preference.OnPreferenceChangeListener {

    //private static final String LOG_TAG = "DLNADMRModeEnabler";
    private final Context mContext;

    private final CheckBoxPreference mCheckBoxPref;

    public DLNADMRModeEnabler(Context context, CheckBoxPreference dlnaDmrModeCheckBoxPreference) {
        mContext = context;
        mCheckBoxPref = dlnaDmrModeCheckBoxPreference;
    }

    public void resume() {
        boolean enabling = isDLNADMRModeOn(mContext);
        mCheckBoxPref.setChecked(enabling);
        if (enabling)
            startDLNADMR();
        else
            stopDLNADMR();

        mCheckBoxPref.setOnPreferenceChangeListener(this);
    }
    
    public void pause() {
        mCheckBoxPref.setOnPreferenceChangeListener(null);
    }

    public static boolean isDLNADMRModeOn(Context context) {
		boolean res = false;
		try {

        	res = Settings.System.getInt(context.getContentResolver(), Settings.System.REALTEK_SETUP_DLNA_DMR_MODE_ON) != 0;
		} catch (SettingNotFoundException snfe) {

		}
		return res;
    }

    private void setDLNADMRModeOn(boolean enabling) {
        // Change the system setting
        Settings.System.putInt(mContext.getContentResolver(), Settings.System.REALTEK_SETUP_DLNA_DMR_MODE_ON, 
                                enabling ? 1 : 0);
        // Update the UI to reflect system setting
        mCheckBoxPref.setChecked(enabling);
        if (enabling)
            startDLNADMR();
        else
            stopDLNADMR();
    }

    public boolean onPreferenceChange(Preference preference, Object newValue) {
        setDLNADMRModeOn((Boolean) newValue);
        return true;
    }

	private void startDLNADMR() {
		Intent dmrServiceIntent = new Intent();
        dmrServiceIntent.setAction("com.realtek.dmr.DMRService");
        dmrServiceIntent.setPackage("com.realtek.dmr");
		mContext.startService(dmrServiceIntent);
	}

	private void stopDLNADMR() {
		Intent dmrServiceIntent = new Intent();
        dmrServiceIntent.setAction("com.realtek.dmr.DMRService");
        dmrServiceIntent.setPackage("com.realtek.dmr");
		mContext.stopService(dmrServiceIntent);
	}
}
