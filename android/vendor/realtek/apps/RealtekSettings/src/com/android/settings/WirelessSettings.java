/*
 * Copyright (C) 2009 The Android Open Source Project
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


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.admin.DevicePolicyManager;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.nfc.NfcAdapter;
import android.nfc.NfcManager;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.os.UserManager;
import android.provider.SearchIndexableResource;
import android.provider.Settings;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.PreferenceScreen;
import android.support.v7.preference.Preference.OnPreferenceChangeListener;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

import com.android.ims.ImsManager;
import com.android.internal.logging.MetricsProto.MetricsEvent;
import com.android.internal.telephony.TelephonyIntents;
import com.android.internal.telephony.TelephonyProperties;
import com.android.settings.nfc.NfcEnabler;
import com.android.settings.search.BaseSearchIndexProvider;
import com.android.settings.search.Indexable;
import com.android.settingslib.RestrictedLockUtils;
import com.android.settingslib.RestrictedPreference;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import android.support.v7.preference.CheckBoxPreference;
import com.android.settings.name.DeviceNameDialogFragment;
import com.android.settings.name.DeviceManager;
import android.content.BroadcastReceiver;
import android.support.v4.content.LocalBroadcastManager;
import com.rtk.net.openwrt.UbusRpc;
import com.jayway.jsonpath.JsonPath;
import com.rtk.net.openwrt.Server;
import com.rtk.net.openwrt.DlnaStatus;
import android.os.AsyncTask;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothProfile;
import android.widget.Toast;
import com.android.settings.a2dpsink.BluetoothMediaControlFragment;
import java.util.Set;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothA2dp;
import android.bluetooth.BluetoothA2dpSink;
import android.bluetooth.BluetoothAvrcp;
import android.bluetooth.BluetoothAvrcpController;
import android.bluetooth.BluetoothProfile;
import com.android.settingslib.bluetooth.LocalBluetoothManager;
import com.android.settingslib.bluetooth.LocalBluetoothProfileManager;
import com.android.settingslib.bluetooth.CachedBluetoothDevice;
import java.util.Collection;
import com.android.settingslib.bluetooth.BluetoothDeviceFilter;

public class WirelessSettings extends SettingsPreferenceFragment implements Indexable {
    private static final String TAG = "WirelessSettings";

    private static final String KEY_TOGGLE_AIRPLANE = "toggle_airplane";
    private static final String KEY_TOGGLE_NFC = "toggle_nfc";
    private static final String KEY_WIMAX_SETTINGS = "wimax_settings";
    private static final String KEY_ANDROID_BEAM_SETTINGS = "android_beam_settings";
    private static final String KEY_VPN_SETTINGS = "vpn_settings";
    private static final String KEY_TETHER_SETTINGS = "tether_settings";
    private static final String KEY_PROXY_SETTINGS = "proxy_settings";
    private static final String KEY_MOBILE_NETWORK_SETTINGS = "mobile_network_settings";
    private static final String KEY_MANAGE_MOBILE_PLAN = "manage_mobile_plan";
    private static final String KEY_WFC_SETTINGS = "wifi_calling_settings";
    private static final String KEY_NETWORK_RESET = "network_reset";
    private static final String KEY_DEVICE_NAME = "device_name";

    public static final String EXIT_ECM_RESULT = "exit_ecm_result";
    public static final int REQUEST_CODE_EXIT_ECM = 1;

    private AirplaneModeEnabler mAirplaneModeEnabler;
    private SwitchPreference mAirplaneModePreference;
    private NfcEnabler mNfcEnabler;
    private NfcAdapter mNfcAdapter;

    private ConnectivityManager mCm;
    private TelephonyManager mTm;
    private PackageManager mPm;
    private UserManager mUm;

    private static final int MANAGE_MOBILE_PLAN_DIALOG_ID = 1;
    private static final String SAVED_MANAGE_MOBILE_PLAN_MSG = "mManageMobilePlanMessage";

    private PreferenceScreen mButtonWfc;

    //realtek
    private static final String KEY_TOGGLE_DLNA_DMR = "toggle_dlna_dmr";
    private CheckBoxPreference mDLNADMRModePreference;
    private DLNADMRModeEnabler mDLNADMRModeEnabler;
    private Preference mDeviceName;
    private static final String KEY_OPENWRT = "openwrt_settings";
    private static final boolean SHOW_DMR = false;
    private static final String KEY_TOGGLE_BT_SINK_SERVICE = "toggle_bt_sink_service";
    private static final String KEY_BT_MEDIA_CONTROLLER = "bt_media_controller";
    private SwitchPreference mBtSinkServicePreference;
    private Preference mBtMediaControlPreference;
    private BluetoothAdapter mAdapter;
    private BluetoothA2dp mBluetoothA2dp;
    private BluetoothAvrcpController mBluetoothAvrcp_contrl;
    private BluetoothA2dpSink mBluetoothA2dpSink;
    private int btServiceStatus = 0;
    private BluetoothDevice sinkDevice = null;
    private BluetoothDevice srcDevice = null;
    private LocalBluetoothProfileManager mProfileManager;
    private LocalBluetoothManager mLocalManager;
    private BluetoothDeviceFilter.Filter mFilter = BluetoothDeviceFilter.BONDED_DEVICE_FILTER;
    /**
     * Invoked on each preference click in this hierarchy, overrides
     * PreferenceFragment's implementation.  Used to make sure we track the
     * preference click events.
     */
    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        log("onPreferenceTreeClick: preference=" + preference);
        if (preference == mAirplaneModePreference && Boolean.parseBoolean(
                SystemProperties.get(TelephonyProperties.PROPERTY_INECM_MODE))) {
            // In ECM mode launch ECM app dialog
            startActivityForResult(
                new Intent(TelephonyIntents.ACTION_SHOW_NOTICE_ECM_BLOCK_OTHERS, null),
                REQUEST_CODE_EXIT_ECM);
            return true;
        } else if (preference == findPreference(KEY_MANAGE_MOBILE_PLAN)) {
            onManageMobilePlanClick();
        } else if (preference == findPreference(KEY_DEVICE_NAME)) {
            new DeviceNameDialogFragment().show(getFragmentManager(), "rename device");
        } else if (preference == findPreference(KEY_TOGGLE_BT_SINK_SERVICE)) {
            toggleBtSinkService();
        } else if (preference == findPreference(KEY_BT_MEDIA_CONTROLLER)) {
            if(mBluetoothA2dpSink != null) {
                List<BluetoothDevice> mDevices = mBluetoothA2dpSink.getConnectedDevices();
                if (mDevices != null && mDevices.size() > 0) {
                    new BluetoothMediaControlFragment().show(getFragmentManager(), "Bluetooth media controller");
                } else {
                    Log.d(TAG, "There is no connected media devices");
                    Toast.makeText(getContext(), "There is no connected media devices", Toast.LENGTH_SHORT).show();
                }
            } else {
                Toast.makeText(getContext(), "Bluetooth A2dp Sink service disconnect", Toast.LENGTH_SHORT).show();
            }
        }
        // Let the intents be launched by the Preference manager
        return super.onPreferenceTreeClick(preference);
    }

    private final BroadcastReceiver mDeviceStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(BluetoothA2dpSink.ACTION_CONNECTION_STATE_CHANGED)) {
                int A2dpSourceState = intent.getIntExtra(BluetoothProfile.EXTRA_STATE,4);
                Log.e(TAG, "receive a2dp sink intent state:" + A2dpSourceState);
                if(A2dpSourceState == BluetoothProfile.STATE_DISCONNECTED){
                    BluetoothDevice mdevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    Log.e(TAG, "sink device state disconnect ");
                    boolean isdisconnect = false;
                    if (sinkDevice != null) {
                        if (sinkDevice.getAddress().equals(mdevice.getAddress())) {
                            isdisconnect = true;
                        }
                    }
                    if (btServiceStatus == 2 && isdisconnect) {
                        Log.e(TAG, "BT sink service state change, disconnect connected device.");
                        btServiceStatus = 0;
                        if (mAdapter != null) {
                            mAdapter.stopA2dpSinkService();
                            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpProfileListener, BluetoothProfile.A2DP);
                            mAdapter.setScanMode(BluetoothAdapter.SCAN_MODE_CONNECTABLE);
                            Toast.makeText(getContext(), "switch to bluetooth src service...", Toast.LENGTH_SHORT).show();
                            if (mProfileManager != null) {
                                mProfileManager.updateA2dpProfile();
                            }
                        }
                    } else {
                        Toast.makeText(getContext(), "Connected device disconnect,Please connect again", Toast.LENGTH_LONG).show();
                    }
                }
            }
            if (intent.getAction().equals(BluetoothA2dp.ACTION_CONNECTION_STATE_CHANGED)) {
                int A2dpSourceState = intent.getIntExtra(BluetoothProfile.EXTRA_STATE,4);
                Log.e(TAG, "receive a2dp source intent state:" + A2dpSourceState);
                if(A2dpSourceState == BluetoothProfile.STATE_DISCONNECTED){
                    BluetoothDevice mdevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    boolean isdisconnect = false;
                    if (srcDevice != null) {
                        if (srcDevice.getAddress().equals(mdevice.getAddress())) {
                            isdisconnect = true;
                        }
                    }
                    if (btServiceStatus == 1 && isdisconnect) {
                        Log.e(TAG, "BT src service state change, disconnect connected device.");
                        btServiceStatus = 0;
                        if (mAdapter != null) {
                            mAdapter.startA2dpSinkService();
                            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mAvrcpProfileListener, BluetoothProfile.AVRCP_CONTROLLER);
                            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpSinkProfileListener, BluetoothProfile.A2DP_SINK);
                            Toast.makeText(getContext(), "switch to bluetooth sink service...", Toast.LENGTH_SHORT).show();
                            if (mProfileManager != null) {
                                mProfileManager.updateA2dpSinkProfile();
                            }
                        }
                    } else {
                        Toast.makeText(getContext(), "Connected device disconnect,Please connect again", Toast.LENGTH_LONG).show();
                    }
                }
            }
        }
    };

    private final BroadcastReceiver mDeviceNameReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            refreshDeviceName();
            if (UbusRpc.isOpenWrt() || UbusRpc.isXen()) {
                new SetDMSNameSameWithDeviceName().execute();
            }
            writeApConfig(DeviceManager.getDeviceName(getActivity()));
        }
    };

    private void refreshDeviceName() {
        if (mDeviceName != null) {
            mDeviceName.setSummary(getString(R.string.device_summary_description, Build.MODEL, DeviceManager.getDeviceName(getActivity())));
        }
    }

    private BluetoothProfile.ServiceListener mA2dpProfileListener =
            new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            Log.d(TAG, "bluetooth a2dp service connected mBluetoothA2dp");
            mBluetoothA2dp = (BluetoothA2dp) proxy;
            updateBtSinkServiceOption(false);
            mBtSinkServicePreference.setEnabled(true);
        }
        public void onServiceDisconnected(int profile) {
            Log.d(TAG, "bluetooth a2dp service disconnected");
            if (mBluetoothA2dp != null) {
                try {
                    BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
                    adapter.closeProfileProxy(BluetoothProfile.A2DP, mBluetoothA2dp);
                    mBluetoothA2dp = null;
                } catch (Throwable t) {
                    Log.w(TAG, "Error cleaning up A2DP proxy", t);
                }
            }
        }
    };

    private BluetoothProfile.ServiceListener mAvrcpProfileListener =
            new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            Log.d(TAG, "bluetooth a2dp service connected mBluetoothAvrcp_contrl");
            mBluetoothAvrcp_contrl= (BluetoothAvrcpController) proxy;
            updateBtSinkServiceOption(true);
            mBtSinkServicePreference.setEnabled(true);
            mAdapter.setScanMode(BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE, 300);
        }
        public void onServiceDisconnected(int profile) {
            Log.d(TAG, "bluetooth avrcp service disconnected");
            if (mBluetoothAvrcp_contrl != null) {
                try {
                    BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
                    adapter.closeProfileProxy(BluetoothProfile.AVRCP_CONTROLLER, mBluetoothAvrcp_contrl);
                    mBluetoothAvrcp_contrl = null;
                } catch (Throwable t) {
                    Log.w(TAG, "Error cleaning up Avrcp proxy", t);
                }
            }
        }
    };

    private BluetoothProfile.ServiceListener mA2dpSinkProfileListener =
            new BluetoothProfile.ServiceListener() {
        public void onServiceConnected(int profile, BluetoothProfile proxy) {
            Log.d(TAG, "bluetooth a2dp service connected mBluetoothA2dpSink");
            mBluetoothA2dpSink = (BluetoothA2dpSink) proxy;
        }
        public void onServiceDisconnected(int profile) {
            Log.d(TAG, "bluetooth a2dpsink service disconnected");
            if (mBluetoothA2dpSink != null) {
                try {
                    BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
                    adapter.closeProfileProxy(BluetoothProfile.A2DP_SINK, mBluetoothA2dpSink);
                    mBluetoothA2dpSink = null;
                } catch (Throwable t) {
                    Log.w(TAG, "Error cleaning up Avrcp proxy", t);
                }
            }
        }
    };

    private String mManageMobilePlanMessage;
    public void onManageMobilePlanClick() {
        log("onManageMobilePlanClick:");
        mManageMobilePlanMessage = null;
        Resources resources = getActivity().getResources();

        NetworkInfo ni = mCm.getActiveNetworkInfo();
        if (mTm.hasIccCard() && (ni != null)) {
            // Check for carrier apps that can handle provisioning first
            Intent provisioningIntent = new Intent(TelephonyIntents.ACTION_CARRIER_SETUP);
            List<String> carrierPackages =
                    mTm.getCarrierPackageNamesForIntent(provisioningIntent);
            if (carrierPackages != null && !carrierPackages.isEmpty()) {
                if (carrierPackages.size() != 1) {
                    Log.w(TAG, "Multiple matching carrier apps found, launching the first.");
                }
                provisioningIntent.setPackage(carrierPackages.get(0));
                startActivity(provisioningIntent);
                return;
            }

            // Get provisioning URL
            String url = mCm.getMobileProvisioningUrl();
            if (!TextUtils.isEmpty(url)) {
                Intent intent = Intent.makeMainSelectorActivity(Intent.ACTION_MAIN,
                        Intent.CATEGORY_APP_BROWSER);
                intent.setData(Uri.parse(url));
                intent.setFlags(Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT |
                        Intent.FLAG_ACTIVITY_NEW_TASK);
                try {
                    startActivity(intent);
                } catch (ActivityNotFoundException e) {
                    Log.w(TAG, "onManageMobilePlanClick: startActivity failed" + e);
                }
            } else {
                // No provisioning URL
                String operatorName = mTm.getSimOperatorName();
                if (TextUtils.isEmpty(operatorName)) {
                    // Use NetworkOperatorName as second choice in case there is no
                    // SPN (Service Provider Name on the SIM). Such as with T-mobile.
                    operatorName = mTm.getNetworkOperatorName();
                    if (TextUtils.isEmpty(operatorName)) {
                        mManageMobilePlanMessage = resources.getString(
                                R.string.mobile_unknown_sim_operator);
                    } else {
                        mManageMobilePlanMessage = resources.getString(
                                R.string.mobile_no_provisioning_url, operatorName);
                    }
                } else {
                    mManageMobilePlanMessage = resources.getString(
                            R.string.mobile_no_provisioning_url, operatorName);
                }
            }
        } else if (mTm.hasIccCard() == false) {
            // No sim card
            mManageMobilePlanMessage = resources.getString(R.string.mobile_insert_sim_card);
        } else {
            // NetworkInfo is null, there is no connection
            mManageMobilePlanMessage = resources.getString(R.string.mobile_connect_to_internet);
        }
        if (!TextUtils.isEmpty(mManageMobilePlanMessage)) {
            log("onManageMobilePlanClick: message=" + mManageMobilePlanMessage);
            showDialog(MANAGE_MOBILE_PLAN_DIALOG_ID);
        }
    }

    @Override
    public Dialog onCreateDialog(int dialogId) {
        log("onCreateDialog: dialogId=" + dialogId);
        switch (dialogId) {
            case MANAGE_MOBILE_PLAN_DIALOG_ID:
                return new AlertDialog.Builder(getActivity())
                            .setMessage(mManageMobilePlanMessage)
                            .setCancelable(false)
                            .setPositiveButton(com.android.internal.R.string.ok,
                                    new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int id) {
                                    log("MANAGE_MOBILE_PLAN_DIALOG.onClickListener id=" + id);
                                    mManageMobilePlanMessage = null;
                                }
                            })
                            .create();
        }
        return super.onCreateDialog(dialogId);
    }

    private void log(String s) {
        Log.d(TAG, s);
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.WIRELESS;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (savedInstanceState != null) {
            mManageMobilePlanMessage = savedInstanceState.getString(SAVED_MANAGE_MOBILE_PLAN_MSG);
        }
        log("onCreate: mManageMobilePlanMessage=" + mManageMobilePlanMessage);

        mCm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        mTm = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
        mPm = getPackageManager();
        mUm = (UserManager) getSystemService(Context.USER_SERVICE);

        addPreferencesFromResource(R.xml.wireless_settings);

        final boolean isAdmin = mUm.isAdminUser();

        final Activity activity = getActivity();
        mAirplaneModePreference = (SwitchPreference) findPreference(KEY_TOGGLE_AIRPLANE);
        SwitchPreference nfc = (SwitchPreference) findPreference(KEY_TOGGLE_NFC);
        RestrictedPreference androidBeam = (RestrictedPreference) findPreference(
                KEY_ANDROID_BEAM_SETTINGS);

        mAirplaneModeEnabler = new AirplaneModeEnabler(activity, mAirplaneModePreference);
        mNfcEnabler = new NfcEnabler(activity, nfc, androidBeam);

        mButtonWfc = (PreferenceScreen) findPreference(KEY_WFC_SETTINGS);

        String toggleable = Settings.Global.getString(activity.getContentResolver(),
                Settings.Global.AIRPLANE_MODE_TOGGLEABLE_RADIOS);

        //enable/disable wimax depending on the value in config.xml
        final boolean isWimaxEnabled = isAdmin && this.getResources().getBoolean(
                com.android.internal.R.bool.config_wimaxEnabled);
        if (!isWimaxEnabled || RestrictedLockUtils.hasBaseUserRestriction(activity,
                UserManager.DISALLOW_CONFIG_MOBILE_NETWORKS, UserHandle.myUserId())) {
            PreferenceScreen root = getPreferenceScreen();
            Preference ps = findPreference(KEY_WIMAX_SETTINGS);
            if (ps != null) root.removePreference(ps);
        } else {
            if (toggleable == null || !toggleable.contains(Settings.Global.RADIO_WIMAX )
                    && isWimaxEnabled) {
                Preference ps = findPreference(KEY_WIMAX_SETTINGS);
                ps.setDependency(KEY_TOGGLE_AIRPLANE);
            }
        }

        // Manually set dependencies for Wifi when not toggleable.
        if (toggleable == null || !toggleable.contains(Settings.Global.RADIO_WIFI)) {
            findPreference(KEY_VPN_SETTINGS).setDependency(KEY_TOGGLE_AIRPLANE);
        }
        // Disable VPN.
        // TODO: http://b/23693383
        if (!isAdmin || RestrictedLockUtils.hasBaseUserRestriction(activity,
                UserManager.DISALLOW_CONFIG_VPN, UserHandle.myUserId())) {
            removePreference(KEY_VPN_SETTINGS);
        }

        // Manually set dependencies for Bluetooth when not toggleable.
        if (toggleable == null || !toggleable.contains(Settings.Global.RADIO_BLUETOOTH)) {
            // No bluetooth-dependent items in the list. Code kept in case one is added later.
        }

        // Manually set dependencies for NFC when not toggleable.
        if (toggleable == null || !toggleable.contains(Settings.Global.RADIO_NFC)) {
            findPreference(KEY_TOGGLE_NFC).setDependency(KEY_TOGGLE_AIRPLANE);
            findPreference(KEY_ANDROID_BEAM_SETTINGS).setDependency(KEY_TOGGLE_AIRPLANE);
        }

        // Remove NFC if not available
        mNfcAdapter = NfcAdapter.getDefaultAdapter(activity);
        if (mNfcAdapter == null) {
            getPreferenceScreen().removePreference(nfc);
            getPreferenceScreen().removePreference(androidBeam);
            mNfcEnabler = null;
        }

        // Remove Mobile Network Settings and Manage Mobile Plan for secondary users,
        // if it's a wifi-only device.
        if (!isAdmin || Utils.isWifiOnly(getActivity()) ||
                RestrictedLockUtils.hasBaseUserRestriction(activity,
                        UserManager.DISALLOW_CONFIG_MOBILE_NETWORKS, UserHandle.myUserId())) {
            removePreference(KEY_MOBILE_NETWORK_SETTINGS);
            removePreference(KEY_MANAGE_MOBILE_PLAN);
        }
        // Remove Mobile Network Settings and Manage Mobile Plan
        // if config_show_mobile_plan sets false.
        final boolean isMobilePlanEnabled = this.getResources().getBoolean(
                R.bool.config_show_mobile_plan);
        if (!isMobilePlanEnabled) {
            Preference pref = findPreference(KEY_MANAGE_MOBILE_PLAN);
            if (pref != null) {
                removePreference(KEY_MANAGE_MOBILE_PLAN);
            }
        }

        // Remove Airplane Mode settings if it's a stationary device such as a TV.
        if (mPm.hasSystemFeature(PackageManager.FEATURE_TELEVISION)) {
            removePreference(KEY_TOGGLE_AIRPLANE);
        }

        // Enable Proxy selector settings if allowed.
        Preference mGlobalProxy = findPreference(KEY_PROXY_SETTINGS);
        final DevicePolicyManager mDPM = (DevicePolicyManager)
                activity.getSystemService(Context.DEVICE_POLICY_SERVICE);
        // proxy UI disabled until we have better app support
        getPreferenceScreen().removePreference(mGlobalProxy);
        mGlobalProxy.setEnabled(mDPM.getGlobalProxyAdmin() == null);

        // Disable Tethering if it's not allowed or if it's a wifi-only device
        final ConnectivityManager cm =
                (ConnectivityManager) activity.getSystemService(Context.CONNECTIVITY_SERVICE);

        final boolean adminDisallowedTetherConfig = RestrictedLockUtils.checkIfRestrictionEnforced(
                activity, UserManager.DISALLOW_CONFIG_TETHERING, UserHandle.myUserId()) != null;
        if ((!cm.isTetheringSupported() && !adminDisallowedTetherConfig) ||
                RestrictedLockUtils.hasBaseUserRestriction(activity,
                        UserManager.DISALLOW_CONFIG_TETHERING, UserHandle.myUserId())) {
            getPreferenceScreen().removePreference(findPreference(KEY_TETHER_SETTINGS));
        } else if (!adminDisallowedTetherConfig) {
            Preference p = findPreference(KEY_TETHER_SETTINGS);
            p.setTitle(com.android.settingslib.Utils.getTetheringLabel(cm));

            // Grey out if provisioning is not available.
            p.setEnabled(!TetherSettings
                    .isProvisioningNeededButUnavailable(getActivity()));
        }

        // Remove network reset if not allowed
        if (RestrictedLockUtils.hasBaseUserRestriction(activity,
                UserManager.DISALLOW_NETWORK_RESET, UserHandle.myUserId())) {
            removePreference(KEY_NETWORK_RESET);
        }
        //realtek
        mDLNADMRModePreference = (CheckBoxPreference) findPreference(KEY_TOGGLE_DLNA_DMR);
        if (SHOW_DMR) {
            mDLNADMRModeEnabler = new DLNADMRModeEnabler(activity, mDLNADMRModePreference);
        } else {
            removePreference(KEY_TOGGLE_DLNA_DMR);
        }

        mDeviceName = (Preference) findPreference(KEY_DEVICE_NAME);
        mDeviceName.setSummary(getString(R.string.device_summary_description, Build.MODEL, DeviceManager.getDeviceName(getActivity())));

        if (!(UbusRpc.isXen()) && !(UbusRpc.isOpenWrt())) {
            getPreferenceScreen().removePreference(findPreference(KEY_OPENWRT));
        }
        mBtSinkServicePreference = (SwitchPreference) findPreference(KEY_TOGGLE_BT_SINK_SERVICE);
        mBtMediaControlPreference = (Preference) findPreference(KEY_BT_MEDIA_CONTROLLER);
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        mLocalManager = LocalBluetoothManager.getInstance(getActivity(), null);
        if (mLocalManager == null)  {
            Log.d(TAG, "Failed to retrieve LocalBluetoothManager instance");
            return;
        }
        mProfileManager = mLocalManager.getProfileManager();
    }

    @Override
    public void onResume() {
        super.onResume();

        mAirplaneModeEnabler.resume();
        if (mNfcEnabler != null) {
            mNfcEnabler.resume();
        }

        // update WFC setting
        final Context context = getActivity();
        if (ImsManager.isWfcEnabledByPlatform(context) &&
                ImsManager.isWfcProvisionedOnDevice(context)) {
            getPreferenceScreen().addPreference(mButtonWfc);

            mButtonWfc.setSummary(WifiCallingSettings.getWfcModeSummary(
                    context, ImsManager.getWfcMode(context, mTm.isNetworkRoaming())));
        } else {
            removePreference(KEY_WFC_SETTINGS);
        }

        //realtek
        if (SHOW_DMR) mDLNADMRModeEnabler.resume();

        updateBtSinkServiceOption(false);
        mBtSinkServicePreference.setEnabled(false);
        if (mAdapter != null) {
            Log.d(TAG,"mAdapter != null");
            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpProfileListener, BluetoothProfile.A2DP);
            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mAvrcpProfileListener, BluetoothProfile.AVRCP_CONTROLLER);
            mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpSinkProfileListener, BluetoothProfile.A2DP_SINK);
        } else {
            Log.d(TAG,"mAdapter = null");
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        refreshDeviceName();
       // LocalBroadcastManager.getInstance(getContext()).registerReceiver(mDeviceNameReceiver, new IntentFilter(DeviceManager.ACTION_DEVICE_NAME_UPDATE));
        getContext().registerReceiver(mDeviceNameReceiver, new IntentFilter(DeviceManager.ACTION_DEVICE_NAME_UPDATE));
        IntentFilter mA2dpSourceStateFilter = new IntentFilter();
        mA2dpSourceStateFilter.addAction(BluetoothA2dpSink.ACTION_CONNECTION_STATE_CHANGED);
        mA2dpSourceStateFilter.addAction(BluetoothA2dp.ACTION_CONNECTION_STATE_CHANGED);
        getContext().registerReceiver(mDeviceStateReceiver, mA2dpSourceStateFilter);
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        if (!TextUtils.isEmpty(mManageMobilePlanMessage)) {
            outState.putString(SAVED_MANAGE_MOBILE_PLAN_MSG, mManageMobilePlanMessage);
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        Log.d(TAG,"onPause");
        mAirplaneModeEnabler.pause();
        if (mNfcEnabler != null) {
            mNfcEnabler.pause();
        }

        //realtek
        if (SHOW_DMR) mDLNADMRModeEnabler.resume();

        if (mAdapter != null) {
            mAdapter.setScanMode(BluetoothAdapter.SCAN_MODE_CONNECTABLE);
        }
    }

    @Override
    public void onStop() {
        super.onStop();
        //LocalBroadcastManager.getInstance(getContext()).unregisterReceiver(mDeviceNameReceiver);
        getContext().unregisterReceiver(mDeviceNameReceiver);
        getContext().unregisterReceiver(mDeviceStateReceiver);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG,"onDestroy");
        if (mAdapter != null) {
            mAdapter.closeProfileProxy(BluetoothProfile.A2DP, mBluetoothA2dp);
            mAdapter.closeProfileProxy(BluetoothProfile.AVRCP_CONTROLLER, mBluetoothAvrcp_contrl);
            mAdapter.closeProfileProxy(BluetoothProfile.A2DP_SINK, mBluetoothA2dpSink);
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_CODE_EXIT_ECM) {
            Boolean isChoiceYes = data.getBooleanExtra(EXIT_ECM_RESULT, false);
            // Set Airplane mode based on the return value and checkbox state
            mAirplaneModeEnabler.setAirplaneModeInECM(isChoiceYes,
                    mAirplaneModePreference.isChecked());
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected int getHelpResource() {
        return R.string.help_url_more_networks;
    }

    /**
     * For Search.
     */
    public static final Indexable.SearchIndexProvider SEARCH_INDEX_DATA_PROVIDER =
        new BaseSearchIndexProvider() {
            @Override
            public List<SearchIndexableResource> getXmlResourcesToIndex(
                    Context context, boolean enabled) {
                // Remove wireless settings from search in demo mode
                if (UserManager.isDeviceInDemoMode(context)) {
                    return Collections.emptyList();
                }
                SearchIndexableResource sir = new SearchIndexableResource(context);
                sir.xmlResId = R.xml.wireless_settings;
                return Arrays.asList(sir);
            }

            @Override
            public List<String> getNonIndexableKeys(Context context) {
                final ArrayList<String> result = new ArrayList<String>();

                final UserManager um = (UserManager) context.getSystemService(Context.USER_SERVICE);
                final boolean isSecondaryUser = !um.isAdminUser();
                final boolean isWimaxEnabled = !isSecondaryUser
                        && context.getResources().getBoolean(
                        com.android.internal.R.bool.config_wimaxEnabled);
                if (!isWimaxEnabled) {
                    result.add(KEY_WIMAX_SETTINGS);
                }

                if (isSecondaryUser) { // Disable VPN
                    result.add(KEY_VPN_SETTINGS);
                }

                // Remove NFC if not available
                final NfcManager manager = (NfcManager)
                        context.getSystemService(Context.NFC_SERVICE);
                if (manager != null) {
                    NfcAdapter adapter = manager.getDefaultAdapter();
                    if (adapter == null) {
                        result.add(KEY_TOGGLE_NFC);
                        result.add(KEY_ANDROID_BEAM_SETTINGS);
                    }
                }

                // Remove Mobile Network Settings and Manage Mobile Plan if it's a wifi-only device.
                if (isSecondaryUser || Utils.isWifiOnly(context)) {
                    result.add(KEY_MOBILE_NETWORK_SETTINGS);
                    result.add(KEY_MANAGE_MOBILE_PLAN);
                }

                // Remove Mobile Network Settings and Manage Mobile Plan
                // if config_show_mobile_plan sets false.
                final boolean isMobilePlanEnabled = context.getResources().getBoolean(
                        R.bool.config_show_mobile_plan);
                if (!isMobilePlanEnabled) {
                    result.add(KEY_MANAGE_MOBILE_PLAN);
                }

                final PackageManager pm = context.getPackageManager();

                // Remove Airplane Mode settings if it's a stationary device such as a TV.
                if (pm.hasSystemFeature(PackageManager.FEATURE_TELEVISION)) {
                    result.add(KEY_TOGGLE_AIRPLANE);
                }

                // proxy UI disabled until we have better app support
                result.add(KEY_PROXY_SETTINGS);

                // Disable Tethering if it's not allowed or if it's a wifi-only device
                ConnectivityManager cm = (ConnectivityManager)
                        context.getSystemService(Context.CONNECTIVITY_SERVICE);
                if (isSecondaryUser || !cm.isTetheringSupported()) {
                    result.add(KEY_TETHER_SETTINGS);
                }

                if (!ImsManager.isWfcEnabledByPlatform(context) ||
                        !ImsManager.isWfcProvisionedOnDevice(context)) {
                    result.add(KEY_WFC_SETTINGS);
                }

                if (RestrictedLockUtils.hasBaseUserRestriction(context,
                        UserManager.DISALLOW_NETWORK_RESET, UserHandle.myUserId())) {
                    result.add(KEY_NETWORK_RESET);
                }

                return result;
            }
        };

    public class SetDMSNameSameWithDeviceName extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... unused) {
            Server server = new Server(getContext());
            Server.Service sv = null;
            if (server.isConnected()) {
                String mDevName = DeviceManager.getDeviceName(getActivity());
                server.setDMSName(mDevName);
                //Restart DMS service if dlna is running
                sv = Server.Service.DLNA;
                DlnaStatus dlnaStatus = (DlnaStatus) server.getServiceStatus(Server.Service.DLNA);
                if (dlnaStatus.isRunning) {
                    server.enableService(sv, "disable");
                    server.enableService(sv, "enable");
                }
            }
            return null;
        }
    }

    private void writeApConfig(String ssid) {
        WifiManager mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        WifiConfiguration mWifiConfig = mWifiManager.getWifiApConfiguration();
        if (mWifiConfig == null) {
            Log.d(TAG, "WifiConfig is null!!");
            return;
        }
        mWifiConfig.SSID = ssid;

        if (mWifiManager.getWifiApState() == WifiManager.WIFI_AP_STATE_ENABLED) {
            mWifiManager.setWifiApEnabled(null, false);
            mWifiManager.setWifiApEnabled(mWifiConfig, true);
        } else {
            mWifiManager.setWifiApConfiguration(mWifiConfig);
        }

    }//End of writeApConfig()

    private void toggleBtSinkService() {
        mBtSinkServicePreference.setEnabled(false);
        if (mBtSinkServicePreference.isChecked()) {
            if (mAdapter == null || mLocalManager == null || mBluetoothA2dp == null) {
                Log.d(TAG, "BluetoothAdapter or BluetoothA2dp is null!");
                mBtSinkServicePreference.setChecked(false);
                mBtSinkServicePreference.setEnabled(true);
                return;
            }
            if(mAdapter!= null) {
                if(mBluetoothA2dp != null && mLocalManager != null) {
                    List<BluetoothDevice> mDevices = mBluetoothA2dp.getConnectedDevices();
                    if (mDevices != null && mDevices.size() > 0) {
                        btServiceStatus = 1;
                        for (BluetoothDevice device : mDevices) {
                            srcDevice = device;
                            Log.d(TAG, device.getName() + "," + device.getAddress()+","+device.isConnected());
                            //mBluetoothA2dp.disconnect(device);
                            Collection<CachedBluetoothDevice> mCachedDevices =
                                mLocalManager.getCachedDeviceManager().getCachedDevicesCopy();
                            if (mCachedDevices != null && mCachedDevices.size() > 0) {
                                for (CachedBluetoothDevice cacheddevice : mCachedDevices) {
                                    if (BluetoothDeviceFilter.BONDED_DEVICE_FILTER.matches(cacheddevice.getDevice())) {
                                        Log.d(TAG, cacheddevice.getDevice().getName() + "," + cacheddevice.getDevice().getAddress()+","+cacheddevice.isConnected());
                                        if (cacheddevice.isConnected() && cacheddevice.getDevice().getAddress().equals(srcDevice.getAddress())) {
                                            cacheddevice.disconnect();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        mAdapter.startA2dpSinkService();
                        mAdapter.getProfileProxy(getActivity().getApplicationContext(), mAvrcpProfileListener, BluetoothProfile.AVRCP_CONTROLLER);
                        mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpSinkProfileListener, BluetoothProfile.A2DP_SINK);
                        Toast.makeText(getContext(), "switch to bluetooth sink service...", Toast.LENGTH_SHORT).show();
                        if (mProfileManager != null) {
                            mProfileManager.updateA2dpSinkProfile();
                        }
                    }
                }
            }
        } else {
            if (mAdapter == null || mBluetoothA2dpSink == null) {
                Log.d(TAG, "BluetoothAdapter BluetoothA2dpSink is null!");
                mBtSinkServicePreference.setChecked(true);
                mBtSinkServicePreference.setEnabled(true);
                return;
            }
            if(mAdapter!= null) {
                if(mBluetoothA2dpSink != null) {
                    List<BluetoothDevice> mDevices = mBluetoothA2dpSink.getConnectedDevices();
                    if (mDevices != null && mDevices.size() > 0) {
                        btServiceStatus = 2;
                        for (BluetoothDevice device : mDevices) {
                            sinkDevice = device;
                            Log.d(TAG, device.getName() + "," + device.getAddress()+","+device.isConnected());
                            mBluetoothA2dpSink.disconnect(device);
                        }
                    } else {
                        mAdapter.stopA2dpSinkService();
                        mAdapter.getProfileProxy(getActivity().getApplicationContext(), mA2dpProfileListener, BluetoothProfile.A2DP);
                        mAdapter.setScanMode(BluetoothAdapter.SCAN_MODE_CONNECTABLE);
                        Toast.makeText(getContext(), "switch to bluetooth src service...", Toast.LENGTH_SHORT).show();
                        if (mProfileManager != null) {
                            mProfileManager.updateA2dpProfile();
                        }
                    }
                }
            }
        }
    }

    private void updateBtSinkServiceOption(boolean isSink) {
        mBtSinkServicePreference.setChecked(isSink);
    }

}
