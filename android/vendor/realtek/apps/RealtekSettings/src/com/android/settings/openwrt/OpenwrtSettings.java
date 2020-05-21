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

package com.android.settings.openwrt;

import android.app.Activity;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.net.wifi.WifiConfiguration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.AsyncTask;
import android.os.SystemProperties;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceScreen;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.CheckBoxPreference;
import android.support.v7.preference.EditTextPreference;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.util.Log;
import android.widget.Toast;
import android.widget.Switch;

import com.android.internal.logging.MetricsProto.MetricsEvent;
import com.android.settings.R;
import com.android.settings.SettingsPreferenceFragment;

import java.io.IOException;
import java.net.Inet4Address;
import java.util.ArrayList;

import com.rtk.net.openwrt.*;

public class OpenwrtSettings extends SettingsPreferenceFragment
    implements DialogInterface.OnClickListener, Preference.OnPreferenceChangeListener
{
    private static final String TAG = "OpenwrtSettings";
    private static final boolean DEBUG = false;

    /*private static final String KEY_AFP_TOGGLE = "toggle_afp_service";*/
    /*private static final String KEY_DAAP_TOGGLE = "toggle_daap_service";*/
    private static final String KEY_FTP_TOGGLE = "toggle_ftp_service";
    private static final String KEY_BT_TOGGLE = "toggle_bt_service";
    private static final String KEY_SAMBA_TOGGLE = "toggle_samba_service";
    private static final String KEY_DMS_TOGGLE = "toggle_dms_service";
    private static final String WIFI_AP_SSID_AND_SECURITY_2G = "openwrt_wifi_ap_2g";
    private static final String WIFI_AP_SSID_AND_SECURITY_5G = "openwrt_wifi_ap_5g";
    private static final String KEY_ADVANCED = "openwrt_advanced_key";

    /*private SwitchPreference mAfpSwitch;*/
    /*private SwitchPreference mDaapSwitch;*/
    private SwitchPreference mFtpSwitch;
    private SwitchPreference mBtSwitch;
    private SwitchPreference mSambaSwitch;
    private SwitchPreference mDmsSwitch;
    private final ArrayList<SwitchPreference> mSwitchPrefs
            = new ArrayList<SwitchPreference>();

    private Preference mWifiApSettings_2g;
    private Preference mWifiApSettings_5g;
    private Preference mAdvanced;

    private static final int DIALOG_AP_SETTINGS = 1;
    private static final int DIALOG_AP_SETTINGS_5G = 2;
    private OpenwrtWifiApDialog mDialog;
    private WifiConfiguration mWifiConfig2g = null;
    private WifiConfiguration mWifiConfig5g = null;
    private WifiConfiguration mWifiConfig = null;
    private boolean isWlan0Exist = false;
    private boolean isWlan1Exist = false;

    private SambaStatus smbStatus;
    private FtpStatus ftpStatus;
    /*private AfpdStatus afpdStatus;*/
    private BtStatus btStatus;
    /*private DaapStatus daapStatus;*/
    private DlnaStatus dlnaStatus;

	public static String set_samba = "/nas/set/samba";
	public static String set_ftp = "/nas/set/ftpd";
    /*public static String set_afpd = "/nas/set/afpd";*/
    /*public static String set_daap = "/nas/set/daap";*/
    public static String set_bt = "/nas/set/bt";
    public static String set_dlna = "/nas/set/dlna";

    private static UbusUtil sUbusUtil = null;

    public OpenwrtSettings()
    {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        log("onCreate()");

        // Load the preferences from an XML resource
        addPreferencesFromResource(R.xml.openwrt_settings);

        sUbusUtil = UbusUtil.getInstance(getContext());

        /*mAfpSwitch = findAndInitSwitchPref(KEY_AFP_TOGGLE);*/
        /*mDaapSwitch = findAndInitSwitchPref(KEY_DAAP_TOGGLE);*/
        mFtpSwitch = findAndInitSwitchPref(KEY_FTP_TOGGLE);
        mBtSwitch = findAndInitSwitchPref(KEY_BT_TOGGLE);
        mSambaSwitch = findAndInitSwitchPref(KEY_SAMBA_TOGGLE);
        mDmsSwitch = findAndInitSwitchPref(KEY_DMS_TOGGLE);

        /*
        mWifiApSettings_2g = findPreference(WIFI_AP_SSID_AND_SECURITY_2G);
        mWifiApSettings_5g = findPreference(WIFI_AP_SSID_AND_SECURITY_5G);
        mWifiApSettings_2g.setEnabled(false);
        mWifiApSettings_5g.setEnabled(false);
        if ((SystemProperties.get("persist.rtk.net.openwrt", "false").equals("true")) && (SystemProperties.get("persist.rtk.net.openwrt.ottwifi", "false").equals("true"))) {
            removePreference(WIFI_AP_SSID_AND_SECURITY_2G);
            removePreference(WIFI_AP_SSID_AND_SECURITY_5G);
        }else{
            if (mWifiConfig2g == null)
                mWifiConfig2g = new WifiConfiguration();
            if (mWifiConfig5g == null)
                mWifiConfig5g = new WifiConfiguration();

            new GetWifiApConfigTask().execute((Void) null);
        }*/

        mAdvanced = findPreference(KEY_ADVANCED);

        new HttpGetTask().execute((Void) null);
    }

    @Override
    public void onResume() {
        super.onResume();

        log("onResume()");
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
    public Dialog onCreateDialog(int id) {
        final Activity activity = getActivity();

        if (id == DIALOG_AP_SETTINGS) {
            //2.4GHz Wifi AP
            mDialog = new OpenwrtWifiApDialog(activity, this, mWifiConfig2g, 0);
        } else {
            //5GHz Wifi AP
            mDialog = new OpenwrtWifiApDialog(activity, this, mWifiConfig5g, 1);
        }
        return mDialog;
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        Log.d(TAG, "onPreferenceTreeClick(), key=" + preference.getKey());
        if (preference == mWifiApSettings_2g) {
            showDialog(DIALOG_AP_SETTINGS);
        } else if (preference == mWifiApSettings_5g) {
            showDialog(DIALOG_AP_SETTINGS_5G);
        } else if (preference == mAdvanced) {
            Intent it = new Intent(Intent.ACTION_VIEW, Uri.parse("http://" + sUbusUtil.getServerIp()));
            it.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            getContext().startActivity(it);
        } else {
	        new setEnableTask().execute(preference.getKey(), ((SwitchPreference)preference).isChecked()?"enable":"disable");
        }

        return super.onPreferenceTreeClick(preference);
    }

    @Override
    public boolean onPreferenceChange(Preference pref, Object newValue) {
        final String key = pref.getKey();
        final ContentResolver cr = getContentResolver();
        boolean isChecked;
        final String val = (String)newValue;

        return true;
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.MAIN_SETTINGS;
    }

    private void updateSettingUI() {

        ContentResolver cr = getContentResolver();
        int enabled = 0;

    }

    public class HttpGetTask extends AsyncTask<Void, Void, Boolean> {

        @Override
        protected void onPreExecute() {
            setPrefsEnabledState(false);
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            return refreshStatus();
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            updateAllOptions();
            Log.d(TAG, "success = " + success);
            if (success)
                setPrefsEnabledState(true);
        }
    }

	public class setEnableTask extends AsyncTask<String, Integer, Boolean> {

        private ProgressDialog psDialog;

        @Override
        protected void onPreExecute() {
            psDialog = ProgressDialog.show(getActivity(), null, "Processing...");
        }

		@Override
		protected Boolean doInBackground(String... command) {
            boolean result = false;
            log("Task=" + command[0] + ", enable=" + command[1] +", cmd_length="+command.length);
			if (command.length != 2) {
				return false;
			}

            Server.Service sv = null;
            Server server = new Server(getContext());
            if (server.isConnected() == false) {
                return false;
            }

            /*if (KEY_AFP_TOGGLE.equals(command[0])) {
                sv = Server.Service.AFP;
            } else if (KEY_DAAP_TOGGLE.equals(command[0])) {
                sv = Server.Service.ITUNES;
            } else */if (KEY_FTP_TOGGLE.equals(command[0])) {
                sv = Server.Service.FTP;
            } else if (KEY_BT_TOGGLE.equals(command[0])) {
                sv = Server.Service.DOWNLOADER;
            } else if (KEY_SAMBA_TOGGLE.equals(command[0])) {
                sv = Server.Service.SAMBA;
            } else if (KEY_DMS_TOGGLE.equals(command[0])) {
                sv = Server.Service.DLNA;
            }

            if (sv != null) {
			    result = server.enableService(sv, command[1]);
            }

            refreshStatus();

            return result;
		}

        @Override
        protected void onPostExecute(final Boolean success) {
            log("success = " + success);
            psDialog.dismiss();
            if (!success) {
                Toast.makeText(getActivity(), "Fail", Toast.LENGTH_SHORT).show();
                updateAllOptions();
            }
        }

	}

    public void onClick(DialogInterface dialogInterface, int button) {
        if (button == DialogInterface.BUTTON_POSITIVE) {
            mWifiConfig = mDialog.getConfig();
            if (mWifiConfig != null) {
                String securityMode = null;
                //WPA2_INDEX = 1;
                if (mDialog.getSecurityTypeIndex(mWifiConfig) == 1) {
                    securityMode = "psk2";
                } else {
                    securityMode = "none";
                }
                //new SetWifiApConfigTask().execute(Integer.toString(mDialog.getIface()), mWifiConfig.SSID, securityMode, mWifiConfig.preSharedKey);
            }
        }
    }

	public class GetWifiApConfigTask extends AsyncTask<Void, Void, Boolean> {

        @Override
        protected Boolean doInBackground(Void... iface) {
            //UbusRpc session = new UbusRpc();
            //if (session.fileStat("/proc/wlan0")) {
            if (sUbusUtil.fileStat("/proc/wlan0")) {
                isWlan0Exist = true;
                //mWifiConfig5g = session.getWifiAp(1);
            }
            //if (session.fileStat("/proc/wlan1")) {
            if (sUbusUtil.fileStat("/proc/wlan1")) {
                isWlan1Exist = true;
                //mWifiConfig2g = session.getWifiAp(0);
            }
            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            //Log.d(TAG, "result = " + success);
            if (success) {
                if (isWlan1Exist)
                    mWifiApSettings_2g.setEnabled(true);
                if (isWlan0Exist)
                    mWifiApSettings_5g.setEnabled(true);
            }
        }
    }

    /*
	public class SetWifiApConfigTask extends AsyncTask<String, Void, Boolean> {

        @Override
        protected Boolean doInBackground(String... Params) {
            UbusRpc session = new UbusRpc();
            boolean ret = session.setWifiAp(Params[0], Params[1], Params[2], Params[3]);
            session.commit();
            if (Integer.parseInt(Params[0]) == 0) {
                mWifiConfig2g = session.getWifiAp(0);
            } else {
                mWifiConfig5g = session.getWifiAp(1);
            }
            return ret;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            if (success) {
                mWifiApSettings_2g.setEnabled(true);
                mWifiApSettings_5g.setEnabled(true);
            }
        }
    }*/

    private SwitchPreference findAndInitSwitchPref(String key) {
        SwitchPreference pref = (SwitchPreference) findPreference(key);
        if (pref == null) {
            throw new IllegalArgumentException("Cannot find preference with key = " + key);
        }
        mSwitchPrefs.add(pref);
        return pref;
    }

    private void setPrefsEnabledState(boolean enabled) {
        for (int i=0; i< mSwitchPrefs.size(); i++) {
            SwitchPreference pref = mSwitchPrefs.get(i);
            pref.setEnabled(enabled);
        }
    }
    private void resetPrefs() {
        for (int i=0; i< mSwitchPrefs.size(); i++) {
            SwitchPreference pref = mSwitchPrefs.get(i);
            pref.setChecked(false);
        }
    }

    private boolean refreshStatus() {
        Server server = new Server(getContext());
        if (server.isConnected() == false) {
            //isLoginSuccess = false;
            return false;
        }

        smbStatus = (SambaStatus) server.getServiceStatus(Server.Service.SAMBA);
        ftpStatus = (FtpStatus) server.getServiceStatus(Server.Service.FTP);
        /*afpdStatus = (AfpdStatus) server.getServiceStatus(Server.Service.AFP);*/
        btStatus = (BtStatus) server.getServiceStatus(Server.Service.DOWNLOADER);
        /*daapStatus = (DaapStatus) server.getServiceStatus(Server.Service.ITUNES);*/
        dlnaStatus = (DlnaStatus) server.getServiceStatus(Server.Service.DLNA);

        return true;
    }

    private void updateAllOptions() {
        try{
            mSambaSwitch.setChecked(smbStatus.isRunning ? true : false);
            mFtpSwitch.setChecked(ftpStatus.isRunning ? true : false);
            /*mAfpSwitch.setChecked(afpdStatus.isRunning ? true : false);*/
            mBtSwitch.setChecked(btStatus.isRunning ? true : false);
            /*mDaapSwitch.setChecked(daapStatus.isRunning ? true : false);*/
            mDmsSwitch.setChecked(dlnaStatus.isRunning ? true : false);
        } catch (NullPointerException e) {e.printStackTrace();}
    }

    private static void log(String s) {if(DEBUG) Log.d(TAG, s);}

}
