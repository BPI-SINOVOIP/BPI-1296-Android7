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

import android.content.Context;
import android.content.Intent;
import android.content.ContentResolver;
import android.database.Cursor;
import android.support.v7.preference.CheckBoxPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.PreferenceScreen;
import android.support.v14.preference.SwitchPreference;
import android.provider.Settings;
import android.provider.Settings.Global;
import android.provider.Settings.SettingNotFoundException;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;
import android.util.Log;

import java.util.Observable;
import java.util.Observer;
import android.app.Activity;
import android.os.Bundle;
import android.os.SystemProperties;

import com.android.internal.logging.MetricsLogger;
import com.android.internal.logging.MetricsProto.MetricsEvent;

public class HDMICecSettings extends SettingsPreferenceFragment
        implements Preference.OnPreferenceChangeListener  {

    private static final String TAG = "HDMICecSettings";
    private static final String KEY_HDMI_CEC_TOGGLE = "hdmi_cec_toggle";
    private static final String KEY_HDMI_CEC_ONE_TOUCH_PLAY = "hdmi_cec_one_touch_play";
    private static final String KEY_HDMI_CEC_ONE_TOUCH_STANDBY = "hdmi_cec_one_touch_standby";
    private static final String KEY_HDMI_CEC_AUTO_POWER_ON_FROM_TV = "hdmi_cec_auto_power_on_from_tv";
    private static final String KEY_HDMI_CEC_AUTO_OSD_LANGUAGE = "hdmi_cec_auto_osd_language";
    private static final String KEY_IRDA_CEC_SWITCH_LIST = "irda_cec_switch_list";

    private SwitchPreference mCECAccess;
    private CheckBoxPreference mOneTouchPlay;
    private CheckBoxPreference mOneTouchStandby;
    private CheckBoxPreference mAutoPowerOnFromTV;
    private CheckBoxPreference mAutoOsdLanguage;
    private ListPreference mIrdaCecSwitchList;

    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    public void onStop() {
        super.onStop();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()...");
        super.onCreate(savedInstanceState);
        final Activity activity = getActivity();
        final ContentResolver resolver = activity.getContentResolver();

        addPreferencesFromResource(R.xml.hdmi_cec_settings);

        mCECAccess = (SwitchPreference) findPreference(KEY_HDMI_CEC_TOGGLE);
        mCECAccess.setOnPreferenceChangeListener(this);
        mOneTouchPlay = (CheckBoxPreference) findPreference(KEY_HDMI_CEC_ONE_TOUCH_PLAY);
        mOneTouchStandby = (CheckBoxPreference) findPreference(KEY_HDMI_CEC_ONE_TOUCH_STANDBY);
        mAutoPowerOnFromTV = (CheckBoxPreference) findPreference(KEY_HDMI_CEC_AUTO_POWER_ON_FROM_TV);
        mAutoOsdLanguage = (CheckBoxPreference) findPreference(KEY_HDMI_CEC_AUTO_OSD_LANGUAGE);

        mIrdaCecSwitchList = (ListPreference) findPreference(KEY_IRDA_CEC_SWITCH_LIST);
        mIrdaCecSwitchList.setOnPreferenceChangeListener(this);
        mIrdaCecSwitchList.setValue(getDefaultValueOfIrdaCecSwitch());  // set default selection.
    }

    @Override
    public void onResume() {
        super.onResume();

        updateCECToggles();

        // update summary
        mIrdaCecSwitchList.setSummary(getDefaultValueOfIrdaCecSwitch());
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        Log.d(TAG, "onPreferenceTreeClick()...");
        final ContentResolver cr = getContentResolver();
        if (preference == mOneTouchPlay) {
            if(mOneTouchPlay.isChecked() == true)
                SystemProperties.set("persist.rtk.cec.onetouchplay","true");
            else
                SystemProperties.set("persist.rtk.cec.onetouchplay","false");
        } else if (preference == mOneTouchStandby) {
            if(mOneTouchStandby.isChecked() == true)
                SystemProperties.set("persist.rtk.cec.standby","true");
            else
                SystemProperties.set("persist.rtk.cec.standby","false");
        } else if (preference == mAutoPowerOnFromTV) {
            if(mAutoPowerOnFromTV.isChecked() == true)
                Settings.Global.putInt(cr, Settings.Global.HDMI_CEC_POWER_ON_FROM_TV, 1);
            else
                Settings.Global.putInt(cr, Settings.Global.HDMI_CEC_POWER_ON_FROM_TV, 0);
        } else if (preference == mAutoOsdLanguage) {
            if(mAutoOsdLanguage.isChecked() == true)
                SystemProperties.set("persist.rtk.cec.setlanguage","true");
            else
                SystemProperties.set("persist.rtk.cec.setlanguage","false");
        } else {
            // If we didn't handle it, let preferences handle it.
            return super.onPreferenceTreeClick(preference);
        }

        return true;
    }

    /*
     * Creates toggles for each available cec provider
     */
    private void updateCECToggles() {
        Log.d(TAG, "updateCECToggles()...");
        ContentResolver cr = getContentResolver();

        // turn on/off the option on GUI.
        int enabled = 0;
        enabled = Settings.Global.getInt(cr, Settings.Global.HDMI_CONTROL_ENABLED, 1);
        mCECAccess.setChecked(enabled == 1 ? true : false);

        mOneTouchPlay.setChecked(SystemProperties.getBoolean("persist.rtk.cec.onetouchplay",true));
        mOneTouchStandby.setChecked(SystemProperties.getBoolean("persist.rtk.cec.standby",true));
        enabled = Settings.Global.getInt(cr, Settings.Global.HDMI_CEC_POWER_ON_FROM_TV, 1);
        mAutoPowerOnFromTV.setChecked(enabled == 1 ? true : false);
        mAutoOsdLanguage.setChecked(SystemProperties.getBoolean("persist.rtk.cec.setlanguage",true));
    }

    /** Enable or disable all providers when the master toggle is changed. */
    private void onToggleCECAccess(boolean checked) {
        final ContentResolver cr = getContentResolver();
        updateCECToggles();

        // put the setting value into database.
        Settings.Global.putInt(cr, Settings.Global.HDMI_CONTROL_ENABLED, (checked == true) ? 1 : 0);
    }

    @Override
    public boolean onPreferenceChange(Preference pref, Object newValue) {
        Log.d(TAG, "onPreferenceChange()...");

        if (pref.getKey().equals(KEY_HDMI_CEC_TOGGLE)) {
            onToggleCECAccess((Boolean) newValue);
        }

        if (pref.getKey().equals(KEY_IRDA_CEC_SWITCH_LIST)) {
            // write to db
            String str = (String) newValue;
            Settings.System.putString(getContentResolver(), Settings.System.REALTEK_SETUP_IRDA_CEC_SWITCH, str);

            // update summary
            mIrdaCecSwitchList.setSummary(str);
        }
        return true;
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.MAIN_SETTINGS;
    }

    /* Utility function will be called to get the default value from Settings.System.REALTEK_SETUP_IRDA_CEC_SWITCH
     *
    */
    private String getDefaultValueOfIrdaCecSwitch() {
        String res;
        //try {
            res = Settings.System.getString(getContentResolver(), Settings.System.REALTEK_SETUP_IRDA_CEC_SWITCH);
        //} catch (SettingNotFoundException snfe) {
            //Log.d(TAG, "Exception found in getDefaultValueOfIrdaCecSwitch()");
            //return null;
        //}
        return res;
    }

}

