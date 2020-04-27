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

package com.android.settings.wfd;

import android.content.ContentResolver;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceScreen;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.CheckBoxPreference;
import android.widget.Switch;
import com.android.internal.logging.MetricsLogger;
import com.android.settings.R;
import com.android.settings.SettingsPreferenceFragment;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.util.Log;
import com.android.internal.logging.MetricsProto.MetricsEvent;

public class MiracastSinkSettings extends SettingsPreferenceFragment implements Preference.OnPreferenceChangeListener 
{
	private static final String TAG = "MiracastSinkSettings";

	private static final String KEY_MIRACAST_SINK_TOGGLE = "miracast_sink_toggle";
	private static final String KEY_MIRACAST_SINK_CONNECT_REMINDER = "miracast_sink_connect_reminder_on";

	private SwitchPreference mSinkSwitch;
	private CheckBoxPreference mReminderCheckbox;

	public MiracastSinkSettings()
	{
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);		
	}

	@Override
	public void onResume() {
		super.onResume();

		Log.d(TAG, " onResume "  );
		
		PreferenceScreen root = getPreferenceScreen();
		if (root != null) {
			root.removeAll();
		}

		// Load the preferences from an XML resource
		addPreferencesFromResource(R.xml.wifi_display_sink_settings);

		root = getPreferenceScreen();

		mSinkSwitch = (SwitchPreference) root.findPreference(KEY_MIRACAST_SINK_TOGGLE);
		mSinkSwitch.setOnPreferenceChangeListener(this);

		mReminderCheckbox = (CheckBoxPreference)root.findPreference(KEY_MIRACAST_SINK_CONNECT_REMINDER);
		mReminderCheckbox.setOnPreferenceChangeListener(this);
		root.removePreference(mSinkSwitch);
		updateSettingUI();
	}

	@Override
	public void onPause() {
		super.onPause();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
	}
	

	@Override
	public boolean onPreferenceChange(Preference pref, Object newValue) {
		final String key = pref.getKey();
		final ContentResolver cr = getContentResolver();

		//Listen for switch/checkbox widget for change, and save the value to DB
		if( KEY_MIRACAST_SINK_TOGGLE.equals(key) ) 
		{			
			Settings.System.putInt( cr, Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, ((Boolean)newValue==true)?1:0);
			Log.d(TAG, " toggle newValue = " + newValue );
		}
		else	if( KEY_MIRACAST_SINK_CONNECT_REMINDER.equals(key) )
		{			
			Settings.System.putInt( cr, Settings.System.REALTEK_SETUP_MIRACAST_SINK_CONNECTION_REMINDER_ON, ((Boolean)newValue==true)?1:0);
			Log.d(TAG, " Reminder newValue = " + newValue );
		}
		
		return true;

	}

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.MAIN_SETTINGS;
    }
	
    /*
     * Creates toggle and checkbox for miracast sink setting
     */
    private void updateSettingUI() {    
    		ContentResolver cr = getContentResolver();
		int enabled = 0;
		
		try{			
			enabled = Settings.System.getInt(cr, Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON);
			Log.d(TAG, " toggle  = " + enabled );
			mSinkSwitch.setChecked(enabled == 1 ? true : false);
			
			enabled = Settings.System.getInt(cr, Settings.System.REALTEK_SETUP_MIRACAST_SINK_CONNECTION_REMINDER_ON);
			Log.d(TAG, " Reminder  = " + enabled );
			mReminderCheckbox.setChecked(enabled == 1 ? true : false);
			
		} catch (SettingNotFoundException snfe) {
			Log.d(TAG, " Miracast Sink setting db can't be found "  );
		}		
    }
	
}
