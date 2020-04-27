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

package com.android.settings;

import android.Manifest;
import android.app.Activity;
import android.app.ActivityManagerNative;
import android.app.AlertDialog;
import android.app.AppOpsManager;
import android.app.AppOpsManager.PackageOps;
import android.app.Dialog;
import android.app.admin.DevicePolicyManager;
import android.app.backup.IBackupManager;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.IShortcutService;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ResolveInfo;
import android.content.res.Resources;
import android.hardware.usb.IUsbManager;
import android.hardware.usb.UsbManager;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.IBinder;
import android.os.Parcel;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.StrictMode;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.os.UserManager;
import android.os.storage.IMountService;
import android.provider.SearchIndexableResource;
import android.provider.Settings;
import android.service.persistentdata.PersistentDataBlockManager;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.Preference.OnPreferenceChangeListener;
import android.support.v7.preference.PreferenceGroup;
import android.support.v7.preference.PreferenceScreen;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.IWindowManager;
import android.view.LayoutInflater;
import android.view.ThreadedRenderer;
import android.view.View;
import android.view.ViewGroup;
import android.view.accessibility.AccessibilityManager;
import android.webkit.IWebViewUpdateService;
import android.webkit.WebViewProviderInfo;
import android.widget.Switch;
import android.widget.Toast;

import com.android.internal.app.LocalePicker;
import com.android.internal.logging.MetricsProto.MetricsEvent;
import com.android.settings.applications.BackgroundCheckSummary;
import com.android.settings.fuelgauge.InactiveApps;
import com.android.settings.search.BaseSearchIndexProvider;
import com.android.settings.search.Indexable;
import com.android.settings.widget.SwitchBar;
import com.android.settingslib.RestrictedLockUtils;
import com.android.settingslib.RestrictedLockUtils.EnforcedAdmin;
import com.android.settingslib.RestrictedSwitchPreference;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;

//realtek
import android.provider.Settings.SettingNotFoundException;
import com.realtek.hardware.RtkTVSystem;
import com.realtek.hardware.RtkHDMIManager;
import com.realtek.hardware.RtkAoutUtilManager;
import com.realtek.hardware.RtkVoutUtilManager;
/*
 * Displays preferences for application developers.
 */
public class DevelopmentSettings extends RestrictedSettingsFragment
        implements DialogInterface.OnClickListener, DialogInterface.OnDismissListener,
                OnPreferenceChangeListener, SwitchBar.OnSwitchChangeListener, Indexable {
    private static final String TAG = "DevelopmentSettings";

    /**
     * Preference file were development settings prefs are stored.
     */
    public static final String PREF_FILE = "development";

    /**
     * Whether to show the development settings to the user.  Default is false.
     */
    public static final String PREF_SHOW = "show";

    private static final String ENABLE_ADB = "enable_adb";
    private static final String CLEAR_ADB_KEYS = "clear_adb_keys";
    private static final String ENABLE_TERMINAL = "enable_terminal";
    private static final String KEEP_SCREEN_ON = "keep_screen_on";
    private static final String BT_HCI_SNOOP_LOG = "bt_hci_snoop_log";
    private static final String WEBVIEW_PROVIDER_KEY = "select_webview_provider";
    private static final String WEBVIEW_MULTIPROCESS_KEY = "enable_webview_multiprocess";
    private static final String ENABLE_OEM_UNLOCK = "oem_unlock_enable";
    private static final String HDCP_CHECKING_KEY = "hdcp_checking";
    private static final String HDCP_CHECKING_PROPERTY = "persist.sys.hdcp_checking";
    private static final String LOCAL_BACKUP_PASSWORD = "local_backup_password";
    private static final String HARDWARE_UI_PROPERTY = "persist.sys.ui.hw";
    private static final String MSAA_PROPERTY = "debug.egl.force_msaa";
    private static final String BUGREPORT = "bugreport";
    private static final String BUGREPORT_IN_POWER_KEY = "bugreport_in_power";
    private static final String OPENGL_TRACES_PROPERTY = "debug.egl.trace";
    private static final String TUNER_UI_KEY = "tuner_ui";
    private static final String COLOR_TEMPERATURE_PROPERTY = "persist.sys.debug.color_temp";

    private static final String DEBUG_APP_KEY = "debug_app";
    private static final String WAIT_FOR_DEBUGGER_KEY = "wait_for_debugger";
    private static final String MOCK_LOCATION_APP_KEY = "mock_location_app";
    private static final String VERIFY_APPS_OVER_USB_KEY = "verify_apps_over_usb";
    private static final String DEBUG_VIEW_ATTRIBUTES =  "debug_view_attributes";
    private static final String FORCE_ALLOW_ON_EXTERNAL_KEY = "force_allow_on_external";
    private static final String STRICT_MODE_KEY = "strict_mode";
    private static final String POINTER_LOCATION_KEY = "pointer_location";
    private static final String SHOW_TOUCHES_KEY = "show_touches";
    private static final String SHOW_SCREEN_UPDATES_KEY = "show_screen_updates";
    private static final String DISABLE_OVERLAYS_KEY = "disable_overlays";
    private static final String SIMULATE_COLOR_SPACE = "simulate_color_space";
    private static final String USB_AUDIO_KEY = "usb_audio";
    private static final String FORCE_HARDWARE_UI_KEY = "force_hw_ui";
    private static final String FORCE_MSAA_KEY = "force_msaa";
    private static final String TRACK_FRAME_TIME_KEY = "track_frame_time";
    private static final String SHOW_NON_RECTANGULAR_CLIP_KEY = "show_non_rect_clip";
    private static final String SHOW_HW_SCREEN_UPDATES_KEY = "show_hw_screen_udpates";
    private static final String SHOW_HW_LAYERS_UPDATES_KEY = "show_hw_layers_udpates";
    private static final String DEBUG_HW_OVERDRAW_KEY = "debug_hw_overdraw";
    private static final String DEBUG_LAYOUT_KEY = "debug_layout";
    private static final String FORCE_RTL_LAYOUT_KEY = "force_rtl_layout_all_locales";
    private static final String WINDOW_ANIMATION_SCALE_KEY = "window_animation_scale";
    private static final String TRANSITION_ANIMATION_SCALE_KEY = "transition_animation_scale";
    private static final String ANIMATOR_DURATION_SCALE_KEY = "animator_duration_scale";
    private static final String OVERLAY_DISPLAY_DEVICES_KEY = "overlay_display_devices";
    private static final String DEBUG_DEBUGGING_CATEGORY_KEY = "debug_debugging_category";
    private static final String SELECT_LOGD_SIZE_KEY = "select_logd_size";
    private static final String SELECT_LOGD_SIZE_PROPERTY = "persist.logd.size";
    private static final String SELECT_LOGD_TAG_PROPERTY = "persist.log.tag";
    // Tricky, isLoggable only checks for first character, assumes silence
    private static final String SELECT_LOGD_TAG_SILENCE = "Settings";
    private static final String SELECT_LOGD_SNET_TAG_PROPERTY = "persist.log.tag.snet_event_log";
    private static final String SELECT_LOGD_RUNTIME_SNET_TAG_PROPERTY = "log.tag.snet_event_log";
    private static final String SELECT_LOGD_DEFAULT_SIZE_PROPERTY = "ro.logd.size";
    private static final String SELECT_LOGD_DEFAULT_SIZE_VALUE = "262144";
    private static final String SELECT_LOGD_SVELTE_DEFAULT_SIZE_VALUE = "65536";
    // 32768 is merely a menu marker, 64K is our lowest log buffer size we replace it with.
    private static final String SELECT_LOGD_MINIMUM_SIZE_VALUE = "65536";
    private static final String SELECT_LOGD_OFF_SIZE_MARKER_VALUE = "32768";
    private static final String SELECT_LOGPERSIST_KEY = "select_logpersist";
    private static final String SELECT_LOGPERSIST_PROPERTY = "persist.logd.logpersistd";
    private static final String ACTUAL_LOGPERSIST_PROPERTY = "logd.logpersistd";
    private static final String SELECT_LOGPERSIST_PROPERTY_SERVICE = "logcatd";
    private static final String SELECT_LOGPERSIST_PROPERTY_CLEAR = "clear";
    private static final String SELECT_LOGPERSIST_PROPERTY_STOP = "stop";
    private static final String SELECT_LOGPERSIST_PROPERTY_BUFFER = "persist.logd.logpersistd.buffer";
    private static final String ACTUAL_LOGPERSIST_PROPERTY_BUFFER = "logd.logpersistd.buffer";
    private static final String ACTUAL_LOGPERSIST_PROPERTY_ENABLE = "logd.logpersistd.enable";

    private static final String WIFI_DISPLAY_CERTIFICATION_KEY = "wifi_display_certification";
    private static final String WIFI_VERBOSE_LOGGING_KEY = "wifi_verbose_logging";
    private static final String WIFI_AGGRESSIVE_HANDOVER_KEY = "wifi_aggressive_handover";
    private static final String WIFI_ALLOW_SCAN_WITH_TRAFFIC_KEY = "wifi_allow_scan_with_traffic";
    private static final String USB_CONFIGURATION_KEY = "select_usb_configuration";
    private static final String MOBILE_DATA_ALWAYS_ON = "mobile_data_always_on";
    private static final String KEY_COLOR_MODE = "color_mode";
    private static final String FORCE_RESIZABLE_KEY = "force_resizable_activities";
    private static final String COLOR_TEMPERATURE_KEY = "color_temperature";

    private static final String BLUETOOTH_DISABLE_ABSOLUTE_VOLUME_KEY =
                                    "bluetooth_disable_absolute_volume";
    private static final String BLUETOOTH_DISABLE_ABSOLUTE_VOLUME_PROPERTY =
                                    "persist.bluetooth.disableabsvol";

    private static final String INACTIVE_APPS_KEY = "inactive_apps";

    private static final String IMMEDIATELY_DESTROY_ACTIVITIES_KEY
            = "immediately_destroy_activities";
    private static final String APP_PROCESS_LIMIT_KEY = "app_process_limit";

    private static final String BACKGROUND_CHECK_KEY = "background_check";

    private static final String SHOW_ALL_ANRS_KEY = "show_all_anrs";

    private static final String PACKAGE_MIME_TYPE = "application/vnd.android.package-archive";

    private static final String TERMINAL_APP_PACKAGE = "com.android.terminal";

    private static final String KEY_CONVERT_FBE = "convert_to_file_encryption";

    private static final String OTA_DISABLE_AUTOMATIC_UPDATE_KEY = "ota_disable_automatic_update";

    private static final int RESULT_DEBUG_APP = 1000;
    private static final int RESULT_MOCK_LOCATION_APP = 1001;

    private static final String PERSISTENT_DATA_BLOCK_PROP = "ro.frp.pst";
    private static final String FLASH_LOCKED_PROP = "ro.boot.flash.locked";

    private static final String SHORTCUT_MANAGER_RESET_KEY = "reset_shortcut_manager_throttling";

    private static final int REQUEST_CODE_ENABLE_OEM_UNLOCK = 0;

    private static final int[] MOCK_LOCATION_APP_OPS = new int[] {AppOpsManager.OP_MOCK_LOCATION};

    private IWindowManager mWindowManager;
    private IBackupManager mBackupManager;
    private IWebViewUpdateService mWebViewUpdateService;
    private DevicePolicyManager mDpm;
    private UserManager mUm;
    private WifiManager mWifiManager;
    private PersistentDataBlockManager mOemUnlockManager;
    private TelephonyManager mTelephonyManager;

    private SwitchBar mSwitchBar;
    private boolean mLastEnabledState;
    private boolean mHaveDebugSettings;
    private boolean mDontPokeProperties;

    private SwitchPreference mEnableAdb;
    private Preference mClearAdbKeys;
    private SwitchPreference mEnableTerminal;
    private Preference mBugreport;
    private SwitchPreference mBugreportInPower;
    private RestrictedSwitchPreference mKeepScreenOn;
    private SwitchPreference mBtHciSnoopLog;
    private RestrictedSwitchPreference mEnableOemUnlock;
    private SwitchPreference mDebugViewAttributes;
    private SwitchPreference mForceAllowOnExternal;

    private PreferenceScreen mPassword;
    private String mDebugApp;
    private Preference mDebugAppPref;

    private String mMockLocationApp;
    private Preference mMockLocationAppPref;

    private SwitchPreference mWaitForDebugger;
    private SwitchPreference mVerifyAppsOverUsb;
    private SwitchPreference mWifiDisplayCertification;
    private SwitchPreference mWifiVerboseLogging;
    private SwitchPreference mWifiAggressiveHandover;
    private SwitchPreference mMobileDataAlwaysOn;
    private SwitchPreference mBluetoothDisableAbsVolume;
    private SwitchPreference mOtaDisableAutomaticUpdate;

    private SwitchPreference mWifiAllowScansWithTraffic;
    private SwitchPreference mStrictMode;
    private SwitchPreference mPointerLocation;
    private SwitchPreference mShowTouches;
    private SwitchPreference mShowScreenUpdates;
    private SwitchPreference mDisableOverlays;
    private SwitchPreference mForceHardwareUi;
    private SwitchPreference mForceMsaa;
    private SwitchPreference mShowHwScreenUpdates;
    private SwitchPreference mShowHwLayersUpdates;
    private SwitchPreference mDebugLayout;
    private SwitchPreference mForceRtlLayout;
    private ListPreference mDebugHwOverdraw;
    private ListPreference mLogdSize;
    private ListPreference mLogpersist;
    private ListPreference mUsbConfiguration;
    private ListPreference mTrackFrameTime;
    private ListPreference mShowNonRectClip;
    private ListPreference mWindowAnimationScale;
    private ListPreference mTransitionAnimationScale;
    private ListPreference mAnimatorDurationScale;
    private ListPreference mOverlayDisplayDevices;

    private SwitchPreference mWebViewMultiprocess;
    private ListPreference mWebViewProvider;

    private ListPreference mSimulateColorSpace;

    private SwitchPreference mUSBAudio;
    private SwitchPreference mImmediatelyDestroyActivities;

    private ListPreference mAppProcessLimit;

    private SwitchPreference mShowAllANRs;

    private ColorModePreference mColorModePreference;

    private SwitchPreference mForceResizable;

    private SwitchPreference mColorTemperaturePreference;

    private final ArrayList<Preference> mAllPrefs = new ArrayList<Preference>();

    private final ArrayList<SwitchPreference> mResetSwitchPrefs
            = new ArrayList<SwitchPreference>();

    private final HashSet<Preference> mDisabledPrefs = new HashSet<Preference>();
    // To track whether a confirmation dialog was clicked.
    private boolean mDialogClicked;
    private Dialog mEnableDialog;
    private Dialog mAdbDialog;

    private Dialog mAdbKeysDialog;
    private boolean mUnavailable;

    private boolean mLogpersistCleared;
    private Dialog mLogpersistClearDialog;
    
    //realtek
    private static final int AUDIO_UNKNOWN_TYPE = 0; //don't care
    private static final int AUDIO_AC3_DECODER_TYPE = 2; //SD audio
    private static final int AUDIO_MLP_DECODER_TYPE = 9; //HD audio
    private static final String DEEP_COLOR_MODE_KEY = "deep_color_mode";
    private static final String IGNORE_HDMI_PLUGOUT_KEY = "ignore_hdmi_plugout";
    private static final String YST_MODE_KEY = "yst_mode";
    private static final String KEY_VIDEO_MAX_BUFFER = "num_video_buffer";
    private static final String VIDEO_MAXACQCNT_PROPERTY = "persist.rtk.video.maxAcqCnt";
    private static final String ABSTRACT_SOCKET_PROPERTY = "persist.rtk.abstract_socket";
    private static final String AUTO_2997_MODE_KEY = "auto_2997_mode";
    private static final String FORCE_SD_AUDIO_MODE_KEY = "force_sd_audio_mode";
    private static final String DISABLE_TRY_MOUNT_KEY = "disable_try_mount_mode";
    private static final String VOLD_TRYMOUNT_PROPERTY = "persist.rtk.vold.trymount";
    private static final String CHECK_VOLUME_KEY = "check_volume_mode";
    private static final String VOLD_CHECKVOLUME_PROPERTY = "persist.rtk.vold.checkvolume";
    private static final String FORMAT_ON_ERROR_KEY = "format_on_error_mode";
    private static final String VOLD_FORMATONERROR_PROPERTY = "persist.rtk.vold.formatonerror";
    private static final String SCREEN_ZOOM_IN_PROPERTY = "persist.rtk.vout.zoom";
    private static final String SCREEN_ZOOM_IN_RATIO_PROPERTY = "persist.rtk.vout.zoom.ratio";
    private static final String SCREEN_ZOOM_IN_KEY = "screen_zoom_in_mode";
    private static final String DEEP_SLEEP_MODE_PROPERTY = "persist.rtk.screenoff.suspend";
    private static final String DEEP_SLEEP_KEY = "deep_sleep_mode";
//    private static final String KEY_AUDIO_I2S_IN = "audio_i2s_in_mode";
//    private static final String PROPERTY_AUDIO_I2S_IN = "persist.rtk.audio.in.i2s";
    private static final String KEY_VIDEO_COLOR_MODE = "video_color_mode";
    private static final String PROPERTY_VIDEO_COLOR_MODE = "persist.rtk.video.colormode";
    private static final String DEEP_COLOR_PROPERTY = "persist.rtk.deepcolor";
    private static final String PROPERTY_HDMI_COLORSPACE = "persist.rtk.hdmitx.colorspace";
    private static final String KEY_PERFORMANCE_MODE = "performance_mode";
    private static final String PROPERTY_PERFORMANCE_MODE= "persist.rtk.performancemode";
    private static final String KEY_SEARCH_KEY_FRAME = "search_key_frame_mode";
    private static final String PROPERTY_VIDEO_SEARCHI = "persist.rtk.video.searchi";
    private static final String KEY_AUDIO_NATIVE_SR = "audio_native_sr";
    private static final String PROPERTY_AUDIO_NATIVE_SR = "persist.rtk.audio.native_sr";
    private static final String KEY_HDMI_RANGE_MODE = "hdmi_range_mode";
    private static final String PROPERTY_HDMI_RANGE_MODE = "persist.rtk.hdmi.rangemode";
    private static final String KEY_AUDIO_IN_MODE = "audio_in_mode";
    private static final String PROPERTY_AUDIO_IN_MODE = "persist.rtk.audio.in";
    private static final String KEY_CVBS_ZOOM_IN = "cvbs_zoom_in_mode";
    private static final String PROPERTY_CVBS_ZOOM_IN_RATIO = "persist.rtk.vout.cvbszoom.ratio";
    private static final String KEY_HDR_OUTPUT_MODE = "hdr_output_mode";
    private static final String PROPERTY_HDR_OUTPUT_MODE = "persist.rtk.hdr.output";
    private static final String KEY_SUB_FIXED = "sub_fixed";
    private static final String PROPERTY_SUB_FIXED = "persist.rtk.video.subfixed";
    private static final String KEY_SUPER_RESOLUTION_ONOFF = "super_resolution_onoff";
    private static final String PROPERTY_SUPER_RESOLUTION_ONOFF = "persist.rtk.video.sr";
    private static final String KEY_HDR_TO_SDR_GMA = "hdr_to_sdr_gma";
    private static final String PROPERTY_HDR_TO_SDR_GMA_MODE = "persist.rtk.vout.hdrtosdr.gma";
    private static final String KEY_MAXCLL_MAXFALL_ONOFF = "maxcll_maxfall_onoff";
    private static final String PROPERTY_VIDEO_MAXCLL_MAXFALL = "persist.rtk.video.maxcllmaxfall";
    private ListPreference mVideoColorMode;
    private RtkHDMIManager mRtkHDMIManager;
    private RtkAoutUtilManager mRtkAoutUtilManager;
    private RtkVoutUtilManager mRtkVoutUtilManager;
    private SwitchPreference mIgnoreHdmiPlugout;
    private SwitchPreference mYstMode;
    private SwitchPreference mVideoMaxBuf;
    private SwitchPreference mAuto2997Mode;
    private SwitchPreference mForceSdAudioMode;
    private SwitchPreference mDisableTryMountMode;
    private SwitchPreference mCheckVolumeMode;
    private SwitchPreference mFormatOnErrorMode;
    private SwitchPreference mScreenZoomIn;
    private SwitchPreference mDeepSleepMode;
//    private SwitchPreference mAudioI2sInMode;
    private SwitchPreference mPerformanceMode;
    private SwitchPreference mSearchKeyFrameMode;
    private SwitchPreference mAudioNativeSRMode;
    private ListPreference mHdmiRangeMode;
    private ListPreference mAudioInMode;
    private SwitchPreference mCVBSZoomIn;
    private AlertDialog mCountDialog;
    private CountDownTimer timer;
    private ListPreference mHdrOutputMode;
    private SwitchPreference mSubFixed;
    private SwitchPreference mSuperResolutionOnoff;
    private ListPreference mHdrToSdrGmaMode;
    private SwitchPreference mMaxCLLMaxFALLOnoff;

    public DevelopmentSettings() {
        super(UserManager.DISALLOW_DEBUGGING_FEATURES);
    }

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.DEVELOPMENT;
    }

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        mWindowManager = IWindowManager.Stub.asInterface(ServiceManager.getService("window"));
        mBackupManager = IBackupManager.Stub.asInterface(
                ServiceManager.getService(Context.BACKUP_SERVICE));
        mWebViewUpdateService  =
            IWebViewUpdateService.Stub.asInterface(ServiceManager.getService("webviewupdate"));
        mOemUnlockManager = (PersistentDataBlockManager)getActivity()
                .getSystemService(Context.PERSISTENT_DATA_BLOCK_SERVICE);
        mTelephonyManager = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);

        mDpm = (DevicePolicyManager)getActivity().getSystemService(Context.DEVICE_POLICY_SERVICE);
        mUm = (UserManager) getSystemService(Context.USER_SERVICE);

        mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);

        setIfOnlyAvailableForAdmins(true);
        if (isUiRestricted() || !Utils.isDeviceProvisioned(getActivity())) {
            // Block access to developer options if the user is not the owner, if user policy
            // restricts it, or if the device has not been provisioned
            mUnavailable = true;
            setPreferenceScreen(new PreferenceScreen(getPrefContext(), null));
            return;
        }

        addPreferencesFromResource(R.xml.development_prefs);

        final PreferenceGroup debugDebuggingCategory = (PreferenceGroup)
                findPreference(DEBUG_DEBUGGING_CATEGORY_KEY);
        mEnableAdb = findAndInitSwitchPref(ENABLE_ADB);
        mClearAdbKeys = findPreference(CLEAR_ADB_KEYS);
        if (!SystemProperties.getBoolean("ro.adb.secure", false)) {
            if (debugDebuggingCategory != null) {
                debugDebuggingCategory.removePreference(mClearAdbKeys);
            }
        }
        mAllPrefs.add(mClearAdbKeys);
        mEnableTerminal = findAndInitSwitchPref(ENABLE_TERMINAL);
        if (!isPackageInstalled(getActivity(), TERMINAL_APP_PACKAGE)) {
            debugDebuggingCategory.removePreference(mEnableTerminal);
            mEnableTerminal = null;
        }

        mBugreport = findPreference(BUGREPORT);
        mBugreportInPower = findAndInitSwitchPref(BUGREPORT_IN_POWER_KEY);
        mKeepScreenOn = (RestrictedSwitchPreference) findAndInitSwitchPref(KEEP_SCREEN_ON);
        mBtHciSnoopLog = findAndInitSwitchPref(BT_HCI_SNOOP_LOG);
        mEnableOemUnlock = (RestrictedSwitchPreference) findAndInitSwitchPref(ENABLE_OEM_UNLOCK);
        if (!showEnableOemUnlockPreference()) {
            removePreference(mEnableOemUnlock);
            mEnableOemUnlock = null;
        }

        mDebugViewAttributes = findAndInitSwitchPref(DEBUG_VIEW_ATTRIBUTES);
        mForceAllowOnExternal = findAndInitSwitchPref(FORCE_ALLOW_ON_EXTERNAL_KEY);
        mPassword = (PreferenceScreen) findPreference(LOCAL_BACKUP_PASSWORD);
        mAllPrefs.add(mPassword);

        if (!mUm.isAdminUser()) {
            disableForUser(mEnableAdb);
            disableForUser(mClearAdbKeys);
            disableForUser(mEnableTerminal);
            disableForUser(mPassword);
        }

        mDebugAppPref = findPreference(DEBUG_APP_KEY);
        mAllPrefs.add(mDebugAppPref);
        mWaitForDebugger = findAndInitSwitchPref(WAIT_FOR_DEBUGGER_KEY);

        mMockLocationAppPref = findPreference(MOCK_LOCATION_APP_KEY);
        mAllPrefs.add(mMockLocationAppPref);

        mVerifyAppsOverUsb = findAndInitSwitchPref(VERIFY_APPS_OVER_USB_KEY);
        if (!showVerifierSetting()) {
            if (debugDebuggingCategory != null) {
                debugDebuggingCategory.removePreference(mVerifyAppsOverUsb);
            } else {
                mVerifyAppsOverUsb.setEnabled(false);
            }
        }
        mStrictMode = findAndInitSwitchPref(STRICT_MODE_KEY);
        mPointerLocation = findAndInitSwitchPref(POINTER_LOCATION_KEY);
        mShowTouches = findAndInitSwitchPref(SHOW_TOUCHES_KEY);
        mShowScreenUpdates = findAndInitSwitchPref(SHOW_SCREEN_UPDATES_KEY);
        mDisableOverlays = findAndInitSwitchPref(DISABLE_OVERLAYS_KEY);
        mForceHardwareUi = findAndInitSwitchPref(FORCE_HARDWARE_UI_KEY);
        mForceMsaa = findAndInitSwitchPref(FORCE_MSAA_KEY);
        mTrackFrameTime = addListPreference(TRACK_FRAME_TIME_KEY);
        mShowNonRectClip = addListPreference(SHOW_NON_RECTANGULAR_CLIP_KEY);
        mShowHwScreenUpdates = findAndInitSwitchPref(SHOW_HW_SCREEN_UPDATES_KEY);
        mShowHwLayersUpdates = findAndInitSwitchPref(SHOW_HW_LAYERS_UPDATES_KEY);
        mDebugLayout = findAndInitSwitchPref(DEBUG_LAYOUT_KEY);
        mForceRtlLayout = findAndInitSwitchPref(FORCE_RTL_LAYOUT_KEY);
        mDebugHwOverdraw = addListPreference(DEBUG_HW_OVERDRAW_KEY);
        mWifiDisplayCertification = findAndInitSwitchPref(WIFI_DISPLAY_CERTIFICATION_KEY);
        mWifiVerboseLogging = findAndInitSwitchPref(WIFI_VERBOSE_LOGGING_KEY);
        mWifiAggressiveHandover = findAndInitSwitchPref(WIFI_AGGRESSIVE_HANDOVER_KEY);
        mWifiAllowScansWithTraffic = findAndInitSwitchPref(WIFI_ALLOW_SCAN_WITH_TRAFFIC_KEY);
        mMobileDataAlwaysOn = findAndInitSwitchPref(MOBILE_DATA_ALWAYS_ON);
        mLogdSize = addListPreference(SELECT_LOGD_SIZE_KEY);
        if ("1".equals(SystemProperties.get("ro.debuggable", "0"))) {
            mLogpersist = addListPreference(SELECT_LOGPERSIST_KEY);
        } else {
            mLogpersist = (ListPreference) findPreference(SELECT_LOGPERSIST_KEY);
            if (mLogpersist != null) {
                mLogpersist.setEnabled(false);
                if (debugDebuggingCategory != null) {
                    debugDebuggingCategory.removePreference(mLogpersist);
                }
            }
            mLogpersist = null;
        }
        mUsbConfiguration = addListPreference(USB_CONFIGURATION_KEY);
        mWebViewProvider = addListPreference(WEBVIEW_PROVIDER_KEY);
        mWebViewMultiprocess = findAndInitSwitchPref(WEBVIEW_MULTIPROCESS_KEY);
        mBluetoothDisableAbsVolume = findAndInitSwitchPref(BLUETOOTH_DISABLE_ABSOLUTE_VOLUME_KEY);

        mWindowAnimationScale = addListPreference(WINDOW_ANIMATION_SCALE_KEY);
        mTransitionAnimationScale = addListPreference(TRANSITION_ANIMATION_SCALE_KEY);
        mAnimatorDurationScale = addListPreference(ANIMATOR_DURATION_SCALE_KEY);
        mOverlayDisplayDevices = addListPreference(OVERLAY_DISPLAY_DEVICES_KEY);
        mSimulateColorSpace = addListPreference(SIMULATE_COLOR_SPACE);
        mUSBAudio = findAndInitSwitchPref(USB_AUDIO_KEY);
        mForceResizable = findAndInitSwitchPref(FORCE_RESIZABLE_KEY);

        mImmediatelyDestroyActivities = (SwitchPreference) findPreference(
                IMMEDIATELY_DESTROY_ACTIVITIES_KEY);
        mAllPrefs.add(mImmediatelyDestroyActivities);
        mResetSwitchPrefs.add(mImmediatelyDestroyActivities);

        mAppProcessLimit = addListPreference(APP_PROCESS_LIMIT_KEY);

        mShowAllANRs = (SwitchPreference) findPreference(
                SHOW_ALL_ANRS_KEY);
        mAllPrefs.add(mShowAllANRs);
        mResetSwitchPrefs.add(mShowAllANRs);

        Preference hdcpChecking = findPreference(HDCP_CHECKING_KEY);
        if (hdcpChecking != null) {
            mAllPrefs.add(hdcpChecking);
            removePreferenceForProduction(hdcpChecking);
        }
        
        //realtek
        mRtkHDMIManager = new RtkHDMIManager();
        mVideoColorMode = addListPreference(KEY_VIDEO_COLOR_MODE);
        prepareAvailableItemsForVideoColorMode();

        mIgnoreHdmiPlugout = findAndInitSwitchPref(IGNORE_HDMI_PLUGOUT_KEY);
        mYstMode = findAndInitSwitchPref(YST_MODE_KEY);
        mVideoMaxBuf = findAndInitSwitchPref(KEY_VIDEO_MAX_BUFFER);
        mAuto2997Mode = findAndInitSwitchPref(AUTO_2997_MODE_KEY);
        mForceSdAudioMode = findAndInitSwitchPref(FORCE_SD_AUDIO_MODE_KEY);
        mDisableTryMountMode = findAndInitSwitchPref(DISABLE_TRY_MOUNT_KEY);
        mCheckVolumeMode = findAndInitSwitchPref(CHECK_VOLUME_KEY);
        mFormatOnErrorMode = findAndInitSwitchPref(FORMAT_ON_ERROR_KEY);
        mPerformanceMode = findAndInitSwitchPref(KEY_PERFORMANCE_MODE);

        mRtkAoutUtilManager = new RtkAoutUtilManager();
        if(mRtkAoutUtilManager == null)
            Log.d(TAG, "mRtkAoutUtilManager is null!");
        mScreenZoomIn = findAndInitSwitchPref(SCREEN_ZOOM_IN_KEY);
        mDeepSleepMode = findAndInitSwitchPref(DEEP_SLEEP_KEY);
//        mAudioI2sInMode = findAndInitSwitchPref(KEY_AUDIO_I2S_IN);
        mSearchKeyFrameMode = findAndInitSwitchPref(KEY_SEARCH_KEY_FRAME);
        mAudioNativeSRMode = findAndInitSwitchPref(KEY_AUDIO_NATIVE_SR);

        PreferenceScreen convertFbePreference =
            (PreferenceScreen) findPreference(KEY_CONVERT_FBE);

        try {
            IBinder service = ServiceManager.getService("mount");
            IMountService mountService = IMountService.Stub.asInterface(service);
            if (!mountService.isConvertibleToFBE()) {
                removePreference(KEY_CONVERT_FBE);
            } else if ("file".equals(SystemProperties.get("ro.crypto.type", "none"))) {
                convertFbePreference.setEnabled(false);
                convertFbePreference.setSummary(getResources()
                                   .getString(R.string.convert_to_file_encryption_done));
            }
        } catch(RemoteException e) {
            removePreference(KEY_CONVERT_FBE);
        }

        mOtaDisableAutomaticUpdate = findAndInitSwitchPref(OTA_DISABLE_AUTOMATIC_UPDATE_KEY);

        mRtkVoutUtilManager = new RtkVoutUtilManager();
        if(mRtkVoutUtilManager == null)
            Log.d(TAG, "mRtkVoutUtilManager is null!");

        mColorModePreference = (ColorModePreference) findPreference(KEY_COLOR_MODE);
        mColorModePreference.updateCurrentAndSupported();
        if (mColorModePreference.getColorModeCount() < 2) {
            removePreference(KEY_COLOR_MODE);
            mColorModePreference = null;
        }
        updateWebViewProviderOptions();

        mColorTemperaturePreference = (SwitchPreference) findPreference(COLOR_TEMPERATURE_KEY);
        if (getResources().getBoolean(R.bool.config_enableColorTemperature)) {
            mAllPrefs.add(mColorTemperaturePreference);
            mResetSwitchPrefs.add(mColorTemperaturePreference);
        } else {
            removePreference(COLOR_TEMPERATURE_KEY);
            mColorTemperaturePreference = null;
        }

        mHdmiRangeMode = addListPreference(KEY_HDMI_RANGE_MODE);
        mAudioInMode = addListPreference(KEY_AUDIO_IN_MODE);
        mCVBSZoomIn = findAndInitSwitchPref(KEY_CVBS_ZOOM_IN);
        mHdrOutputMode = addListPreference(KEY_HDR_OUTPUT_MODE);
        mSubFixed = findAndInitSwitchPref(KEY_SUB_FIXED);
        mSuperResolutionOnoff = findAndInitSwitchPref(KEY_SUPER_RESOLUTION_ONOFF);
        mHdrToSdrGmaMode = addListPreference(KEY_HDR_TO_SDR_GMA);
        mMaxCLLMaxFALLOnoff = findAndInitSwitchPref(KEY_MAXCLL_MAXFALL_ONOFF);
    }

    private ListPreference addListPreference(String prefKey) {
        ListPreference pref = (ListPreference) findPreference(prefKey);
        mAllPrefs.add(pref);
        pref.setOnPreferenceChangeListener(this);
        return pref;
    }

    private void disableForUser(Preference pref) {
        if (pref != null) {
            pref.setEnabled(false);
            mDisabledPrefs.add(pref);
        }
    }

    private SwitchPreference findAndInitSwitchPref(String key) {
        SwitchPreference pref = (SwitchPreference) findPreference(key);
        if (pref == null) {
            throw new IllegalArgumentException("Cannot find preference with key = " + key);
        }
        mAllPrefs.add(pref);
        mResetSwitchPrefs.add(pref);
        return pref;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        final SettingsActivity activity = (SettingsActivity) getActivity();

        mSwitchBar = activity.getSwitchBar();
       if (mUnavailable) {
            mSwitchBar.setEnabled(false);
            return;
        }

        mSwitchBar.addOnSwitchChangeListener(this);
    }

    private boolean removePreferenceForProduction(Preference preference) {
        if ("user".equals(Build.TYPE)) {
            removePreference(preference);
            return true;
        }
        return false;
    }

    private void removePreference(Preference preference) {
        getPreferenceScreen().removePreference(preference);
        mAllPrefs.remove(preference);
        mResetSwitchPrefs.remove(preference);
    }

    private void setPrefsEnabledState(boolean enabled) {
        for (int i = 0; i < mAllPrefs.size(); i++) {
            Preference pref = mAllPrefs.get(i);
            pref.setEnabled(enabled && !mDisabledPrefs.contains(pref));
        }
        updateAllOptions();
    }

    @Override
    public void onResume() {
        super.onResume();

        if (mUnavailable) {
            // Show error message
            if (!isUiRestrictedByOnlyAdmin()) {
                getEmptyTextView().setText(R.string.development_settings_not_available);
            }
            getPreferenceScreen().removeAll();
            return;
        }

        // A DeviceAdmin has specified a maximum time until the device
        // will lock...  in this case we can't allow the user to turn
        // on "stay awake when plugged in" because that would defeat the
        // restriction.
        final EnforcedAdmin admin = RestrictedLockUtils.checkIfMaximumTimeToLockIsSet(
                getActivity());
        mKeepScreenOn.setDisabledByAdmin(admin);
        if (admin == null) {
            mDisabledPrefs.remove(mKeepScreenOn);
        } else {
            mDisabledPrefs.add(mKeepScreenOn);
        }

        final ContentResolver cr = getActivity().getContentResolver();
        mLastEnabledState = Settings.Global.getInt(cr,
                Settings.Global.DEVELOPMENT_SETTINGS_ENABLED, 0) != 0;
        mSwitchBar.setChecked(mLastEnabledState);
        setPrefsEnabledState(mLastEnabledState);

        if (mHaveDebugSettings && !mLastEnabledState) {
            // Overall debugging is disabled, but there are some debug
            // settings that are enabled.  This is an invalid state.  Switch
            // to debug settings being enabled, so the user knows there is
            // stuff enabled and can turn it all off if they want.
            Settings.Global.putInt(getActivity().getContentResolver(),
                    Settings.Global.DEVELOPMENT_SETTINGS_ENABLED, 1);
            mLastEnabledState = true;
            mSwitchBar.setChecked(mLastEnabledState);
            setPrefsEnabledState(mLastEnabledState);
        }
        mSwitchBar.show();

        if (mColorModePreference != null) {
            mColorModePreference.startListening();
            mColorModePreference.updateCurrentAndSupported();
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        if (mColorModePreference != null) {
            mColorModePreference.stopListening();
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        IntentFilter filter = new IntentFilter();
        filter.addAction(UsbManager.ACTION_USB_STATE);
        if (getActivity().registerReceiver(mUsbReceiver, filter) == null) {
            updateUsbConfigurationValues();
        }
        return super.onCreateView(inflater, container, savedInstanceState);
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        if (mUnavailable) {
            return;
        }
        mSwitchBar.removeOnSwitchChangeListener(this);
        mSwitchBar.hide();
        getActivity().unregisterReceiver(mUsbReceiver);
    }

    void updateSwitchPreference(SwitchPreference switchPreference, boolean value) {
        switchPreference.setChecked(value);
        mHaveDebugSettings |= value;
    }

    private void updateAllOptions() {
        final Context context = getActivity();
        final ContentResolver cr = context.getContentResolver();
        mHaveDebugSettings = false;
        updateSwitchPreference(mEnableAdb, Settings.Global.getInt(cr,
                Settings.Global.ADB_ENABLED, 0) != 0);
        if (mEnableTerminal != null) {
            updateSwitchPreference(mEnableTerminal,
                    context.getPackageManager().getApplicationEnabledSetting(TERMINAL_APP_PACKAGE)
                            == PackageManager.COMPONENT_ENABLED_STATE_ENABLED);
        }
        updateSwitchPreference(mBugreportInPower, Settings.Secure.getInt(cr,
                Settings.Global.BUGREPORT_IN_POWER_MENU, 0) != 0);
        updateSwitchPreference(mKeepScreenOn, Settings.Global.getInt(cr,
                Settings.Global.STAY_ON_WHILE_PLUGGED_IN, 0) != 0);
        updateSwitchPreference(mBtHciSnoopLog, Settings.Secure.getInt(cr,
                Settings.Secure.BLUETOOTH_HCI_LOG, 0) != 0);
        updateSwitchPreference(mDebugViewAttributes, Settings.Global.getInt(cr,
                Settings.Global.DEBUG_VIEW_ATTRIBUTES, 0) != 0);
        updateSwitchPreference(mForceAllowOnExternal, Settings.Global.getInt(cr,
                Settings.Global.FORCE_ALLOW_ON_EXTERNAL, 0) != 0);
        updateHdcpValues();
        updatePasswordSummary();
        updateDebuggerOptions();
        updateMockLocation();
        updateStrictModeVisualOptions();
        updatePointerLocationOptions();
        updateShowTouchesOptions();
        updateFlingerOptions();
        updateHardwareUiOptions();
        updateMsaaOptions();
        updateTrackFrameTimeOptions();
        updateShowNonRectClipOptions();
        updateShowHwScreenUpdatesOptions();
        updateShowHwLayersUpdatesOptions();
        updateDebugHwOverdrawOptions();
        updateDebugLayoutOptions();
        updateAnimationScaleOptions();
        updateOverlayDisplayDevicesOptions();
        updateImmediatelyDestroyActivitiesOptions();
        updateAppProcessLimitOptions();
        updateShowAllANRsOptions();
        updateVerifyAppsOverUsbOptions();
        updateOtaDisableAutomaticUpdateOptions();
        updateBugreportOptions();
        updateForceRtlOptions();
        updateLogdSizeValues();
        updateLogpersistValues();
        updateWifiDisplayCertificationOptions();
        updateWifiVerboseLoggingOptions();
        updateWifiAggressiveHandoverOptions();
        updateWifiAllowScansWithTrafficOptions();
        updateMobileDataAlwaysOnOptions();
        updateSimulateColorSpace();
        updateUSBAudioOptions();
        updateForceResizableOptions();
        updateWebViewMultiprocessOptions();
        updateWebViewProviderOptions();
        updateOemUnlockOptions();
        if (mColorTemperaturePreference != null) {
            updateColorTemperature();
        }
        updateBluetoothDisableAbsVolumeOptions();

        updateIgnoreHdmiPlugoutOptions();
        UpdateYstModeOptions();
        updateVideoMaxBuf();
        UpdateAuto2997Options();
        UpdateForceSdAudio();
        updateDisableTryMount();
        updateCheckVolume();
        updateFormatOnError();
        updateScreenZoomIn();
        updateDeepSleepMode();
//        updateAudioI2sInMode();
        updateVideoColorOptions();
        updatePerformanceMode();
        updateSearchKeyFrameMode();
        updateAudioNativeSRMode();
        updateHdmiRangeModeOptions();
        updateAudioInModeOptions();
        updateCVBSZoomIn();
        updateHdrOutputModeOptions();
        updateSubFixedMode();
        updateSuperResolutionMode();
        updateHdrToSdrGmaMode();
        updateMaxCLLMaxFALLOnoff();
    }

    private void resetDangerousOptions() {
        mDontPokeProperties = true;
        for (int i=0; i< mResetSwitchPrefs.size(); i++) {
            SwitchPreference cb = mResetSwitchPrefs.get(i);
            if (cb.isChecked()) {
                cb.setChecked(false);
                onPreferenceTreeClick(cb);
            }
        }
        resetDebuggerOptions();
        writeLogpersistOption(null, true);
        writeLogdSizeOption(null);
        writeAnimationScaleOption(0, mWindowAnimationScale, null);
        writeAnimationScaleOption(1, mTransitionAnimationScale, null);
        writeAnimationScaleOption(2, mAnimatorDurationScale, null);
        // Only poke the color space setting if we control it.
        if (usingDevelopmentColorSpace()) {
            writeSimulateColorSpace(-1);
        }
        writeOverlayDisplayDevicesOptions(null);
        writeAppProcessLimitOptions(null);
        mHaveDebugSettings = false;
        updateAllOptions();
        mDontPokeProperties = false;
        pokeSystemProperties();
    }

    private void updateWebViewProviderOptions() {
        try {
            WebViewProviderInfo[] providers = mWebViewUpdateService.getValidWebViewPackages();
            if (providers == null) {
                Log.e(TAG, "No WebView providers available");
                return;
            }
            ArrayList<String> options = new ArrayList<String>();
            ArrayList<String> values = new ArrayList<String>();
            for(int n = 0; n < providers.length; n++) {
                if (Utils.isPackageEnabled(getActivity(), providers[n].packageName)) {
                    options.add(providers[n].description);
                    values.add(providers[n].packageName);
                }
            }
            mWebViewProvider.setEntries(options.toArray(new String[options.size()]));
            mWebViewProvider.setEntryValues(values.toArray(new String[values.size()]));

            String value = mWebViewUpdateService.getCurrentWebViewPackageName();
            if (value == null) {
                value = "";
            }

            for (int i = 0; i < values.size(); i++) {
                if (value.contentEquals(values.get(i))) {
                    mWebViewProvider.setValueIndex(i);
                    return;
                }
            }
        } catch(RemoteException e) {
        }
    }

    private void updateWebViewMultiprocessOptions() {
        updateSwitchPreference(mWebViewMultiprocess,
                Settings.Global.getInt(getActivity().getContentResolver(),
                        Settings.Global.WEBVIEW_MULTIPROCESS, 0) != 0);
    }

    private void writeWebViewMultiprocessOptions() {
        boolean value = mWebViewMultiprocess.isChecked();
        Settings.Global.putInt(getActivity().getContentResolver(),
                Settings.Global.WEBVIEW_MULTIPROCESS, value ? 1 : 0);

        try {
            String wv_package = mWebViewUpdateService.getCurrentWebViewPackageName();
            ActivityManagerNative.getDefault().killPackageDependents(
                    wv_package, UserHandle.USER_ALL);
        } catch(RemoteException e) {
        }
    }

    private void updateHdcpValues() {
        ListPreference hdcpChecking = (ListPreference) findPreference(HDCP_CHECKING_KEY);
        if (hdcpChecking != null) {
            String currentValue = SystemProperties.get(HDCP_CHECKING_PROPERTY);
            String[] values = getResources().getStringArray(R.array.hdcp_checking_values);
            String[] summaries = getResources().getStringArray(R.array.hdcp_checking_summaries);
            int index = 1; // Defaults to drm-only. Needs to match with R.array.hdcp_checking_values
            for (int i = 0; i < values.length; i++) {
                if (currentValue.equals(values[i])) {
                    index = i;
                    break;
                }
            }
            hdcpChecking.setValue(values[index]);
            hdcpChecking.setSummary(summaries[index]);
            hdcpChecking.setOnPreferenceChangeListener(this);
        }
    }

    private void updatePasswordSummary() {
        try {
            if (mBackupManager.hasBackupPassword()) {
                mPassword.setSummary(R.string.local_backup_password_summary_change);
            } else {
                mPassword.setSummary(R.string.local_backup_password_summary_none);
            }
        } catch (RemoteException e) {
            // Not much we can do here
        }
    }

    private void writeBtHciSnoopLogOptions() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        adapter.configHciSnoopLog(mBtHciSnoopLog.isChecked());
        Settings.Secure.putInt(getActivity().getContentResolver(),
                Settings.Secure.BLUETOOTH_HCI_LOG,
                mBtHciSnoopLog.isChecked() ? 1 : 0);
    }

    private boolean writeWebViewProviderOptions(Object newValue) {
        try {
            String updatedProvider = mWebViewUpdateService.changeProviderAndSetting(
                    newValue == null ? "" : newValue.toString());
            updateWebViewProviderOptions();
            return newValue != null && newValue.equals(updatedProvider);
        } catch(RemoteException e) {
        }
        return false;
    }

    private void writeDebuggerOptions() {
        try {
            ActivityManagerNative.getDefault().setDebugApp(
                mDebugApp, mWaitForDebugger.isChecked(), true);
        } catch (RemoteException ex) {
        }
    }

    private void writeMockLocation() {
        AppOpsManager appOpsManager = (AppOpsManager) getSystemService(Context.APP_OPS_SERVICE);

        // Disable the app op of the previous mock location app if such.
        List<PackageOps> packageOps = appOpsManager.getPackagesForOps(MOCK_LOCATION_APP_OPS);
        if (packageOps != null) {
            // Should be one but in case we are in a bad state due to use of command line tools.
            for (PackageOps packageOp : packageOps) {
                if (packageOp.getOps().get(0).getMode() != AppOpsManager.MODE_ERRORED) {
                    String oldMockLocationApp = packageOp.getPackageName();
                    try {
                        ApplicationInfo ai = getActivity().getPackageManager().getApplicationInfo(
                                oldMockLocationApp, PackageManager.GET_DISABLED_COMPONENTS);
                        appOpsManager.setMode(AppOpsManager.OP_MOCK_LOCATION, ai.uid,
                                oldMockLocationApp, AppOpsManager.MODE_ERRORED);
                    } catch (NameNotFoundException e) {
                        /* ignore */
                    }
                }
            }
        }

        // Enable the app op of the new mock location app if such.
        if (!TextUtils.isEmpty(mMockLocationApp)) {
            try {
                ApplicationInfo ai = getActivity().getPackageManager().getApplicationInfo(
                        mMockLocationApp, PackageManager.GET_DISABLED_COMPONENTS);
                appOpsManager.setMode(AppOpsManager.OP_MOCK_LOCATION, ai.uid,
                        mMockLocationApp, AppOpsManager.MODE_ALLOWED);
            } catch (NameNotFoundException e) {
                /* ignore */
            }
        }
    }

    private static void resetDebuggerOptions() {
        try {
            ActivityManagerNative.getDefault().setDebugApp(
                    null, false, true);
        } catch (RemoteException ex) {
        }
    }

    private void updateDebuggerOptions() {
        mDebugApp = Settings.Global.getString(
                getActivity().getContentResolver(), Settings.Global.DEBUG_APP);
        updateSwitchPreference(mWaitForDebugger, Settings.Global.getInt(
                getActivity().getContentResolver(), Settings.Global.WAIT_FOR_DEBUGGER, 0) != 0);
        if (mDebugApp != null && mDebugApp.length() > 0) {
            String label;
            try {
                ApplicationInfo ai = getActivity().getPackageManager().getApplicationInfo(mDebugApp,
                        PackageManager.GET_DISABLED_COMPONENTS);
                CharSequence lab = getActivity().getPackageManager().getApplicationLabel(ai);
                label = lab != null ? lab.toString() : mDebugApp;
            } catch (PackageManager.NameNotFoundException e) {
                label = mDebugApp;
            }
            mDebugAppPref.setSummary(getResources().getString(R.string.debug_app_set, label));
            mWaitForDebugger.setEnabled(true);
            mHaveDebugSettings = true;
        } else {
            mDebugAppPref.setSummary(getResources().getString(R.string.debug_app_not_set));
            mWaitForDebugger.setEnabled(false);
        }
    }

    private void updateMockLocation() {
        AppOpsManager appOpsManager = (AppOpsManager) getSystemService(Context.APP_OPS_SERVICE);

        List<PackageOps> packageOps = appOpsManager.getPackagesForOps(MOCK_LOCATION_APP_OPS);
        if (packageOps != null) {
            for (PackageOps packageOp : packageOps) {
                if (packageOp.getOps().get(0).getMode() == AppOpsManager.MODE_ALLOWED) {
                    mMockLocationApp = packageOps.get(0).getPackageName();
                    break;
                }
            }
        }

        if (!TextUtils.isEmpty(mMockLocationApp)) {
            String label = mMockLocationApp;
            try {
                ApplicationInfo ai = getActivity().getPackageManager().getApplicationInfo(
                        mMockLocationApp, PackageManager.GET_DISABLED_COMPONENTS);
                CharSequence appLabel = getPackageManager().getApplicationLabel(ai);
                if (appLabel != null) {
                    label = appLabel.toString();
                }
            } catch (PackageManager.NameNotFoundException e) {
                /* ignore */
            }

            mMockLocationAppPref.setSummary(getString(R.string.mock_location_app_set, label));
            mHaveDebugSettings = true;
        } else {
            mMockLocationAppPref.setSummary(getString(R.string.mock_location_app_not_set));
        }
    }

    private void updateVerifyAppsOverUsbOptions() {
        updateSwitchPreference(mVerifyAppsOverUsb, Settings.Global.getInt(getActivity().getContentResolver(),
                Settings.Global.PACKAGE_VERIFIER_INCLUDE_ADB, 1) != 0);
        mVerifyAppsOverUsb.setEnabled(enableVerifierSetting());
    }

    private void writeVerifyAppsOverUsbOptions() {
        Settings.Global.putInt(getActivity().getContentResolver(),
                Settings.Global.PACKAGE_VERIFIER_INCLUDE_ADB,
                mVerifyAppsOverUsb.isChecked() ? 1 : 0);
    }

    private void updateOtaDisableAutomaticUpdateOptions() {
        // We use the "disabled status" in code, but show the opposite text
        // "Automatic system updates" on screen. So a value 0 indicates the
        // automatic update is enabled.
        updateSwitchPreference(mOtaDisableAutomaticUpdate, Settings.Global.getInt(
                getActivity().getContentResolver(),
                Settings.Global.OTA_DISABLE_AUTOMATIC_UPDATE, 0) != 1);
    }

    private void writeOtaDisableAutomaticUpdateOptions() {
        // We use the "disabled status" in code, but show the opposite text
        // "Automatic system updates" on screen. So a value 0 indicates the
        // automatic update is enabled.
        Settings.Global.putInt(getActivity().getContentResolver(),
                Settings.Global.OTA_DISABLE_AUTOMATIC_UPDATE,
                mOtaDisableAutomaticUpdate.isChecked() ? 0 : 1);
    }

    private boolean enableVerifierSetting() {
        final ContentResolver cr = getActivity().getContentResolver();
        if (Settings.Global.getInt(cr, Settings.Global.ADB_ENABLED, 0) == 0) {
            return false;
        }
        if (Settings.Global.getInt(cr, Settings.Global.PACKAGE_VERIFIER_ENABLE, 1) == 0) {
            return false;
        } else {
            final PackageManager pm = getActivity().getPackageManager();
            final Intent verification = new Intent(Intent.ACTION_PACKAGE_NEEDS_VERIFICATION);
            verification.setType(PACKAGE_MIME_TYPE);
            verification.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            final List<ResolveInfo> receivers = pm.queryBroadcastReceivers(verification, 0);
            if (receivers.size() == 0) {
                return false;
            }
        }
        return true;
    }

    private boolean showVerifierSetting() {
        return Settings.Global.getInt(getActivity().getContentResolver(),
                Settings.Global.PACKAGE_VERIFIER_SETTING_VISIBLE, 1) > 0;
    }

    private static boolean showEnableOemUnlockPreference() {
        return !SystemProperties.get(PERSISTENT_DATA_BLOCK_PROP).equals("");
    }

    private boolean enableOemUnlockPreference() {
        return !isBootloaderUnlocked() && isOemUnlockAllowed();
    }

    private void updateOemUnlockOptions() {
        if (mEnableOemUnlock != null) {
            updateSwitchPreference(mEnableOemUnlock, Utils.isOemUnlockEnabled(getActivity()));
            updateOemUnlockSettingDescription();
            // Showing mEnableOemUnlock preference as device has persistent data block.
            mEnableOemUnlock.setDisabledByAdmin(null);
            mEnableOemUnlock.setEnabled(enableOemUnlockPreference());
            if (mEnableOemUnlock.isEnabled()) {
                // Check restriction, disable mEnableOemUnlock and apply policy transparency.
                mEnableOemUnlock.checkRestrictionAndSetDisabled(UserManager.DISALLOW_FACTORY_RESET);
            }
            if (mEnableOemUnlock.isEnabled()) {
                // Check restriction, disable mEnableOemUnlock and apply policy transparency.
                mEnableOemUnlock.checkRestrictionAndSetDisabled(UserManager.DISALLOW_OEM_UNLOCK);
            }
        }
    }

    private void updateBugreportOptions() {
        mBugreport.setEnabled(true);
        mBugreportInPower.setEnabled(true);
        setBugreportStorageProviderStatus();
    }

    private void setBugreportStorageProviderStatus() {
        final ComponentName componentName = new ComponentName("com.android.shell",
                "com.android.shell.BugreportStorageProvider");
        final boolean enabled = mBugreportInPower.isChecked();
        getPackageManager().setComponentEnabledSetting(componentName,
                enabled ? PackageManager.COMPONENT_ENABLED_STATE_ENABLED
                        : PackageManager.COMPONENT_ENABLED_STATE_DEFAULT,
                0);
    }

    // Returns the current state of the system property that controls
    // strictmode flashes.  One of:
    //    0: not explicitly set one way or another
    //    1: on
    //    2: off
    private static int currentStrictModeActiveIndex() {
        if (TextUtils.isEmpty(SystemProperties.get(StrictMode.VISUAL_PROPERTY))) {
            return 0;
        }
        boolean enabled = SystemProperties.getBoolean(StrictMode.VISUAL_PROPERTY, false);
        return enabled ? 1 : 2;
    }

    private void writeStrictModeVisualOptions() {
        try {
            mWindowManager.setStrictModeVisualIndicatorPreference(mStrictMode.isChecked()
                    ? "1" : "");
        } catch (RemoteException e) {
        }
    }

    private void updateStrictModeVisualOptions() {
        updateSwitchPreference(mStrictMode, currentStrictModeActiveIndex() == 1);
    }

    private void writePointerLocationOptions() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.POINTER_LOCATION, mPointerLocation.isChecked() ? 1 : 0);
    }

    private void updatePointerLocationOptions() {
        updateSwitchPreference(mPointerLocation,
                Settings.System.getInt(getActivity().getContentResolver(),
                        Settings.System.POINTER_LOCATION, 0) != 0);
    }

    private void writeShowTouchesOptions() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.SHOW_TOUCHES, mShowTouches.isChecked() ? 1 : 0);
    }

    private void updateShowTouchesOptions() {
        updateSwitchPreference(mShowTouches,
                Settings.System.getInt(getActivity().getContentResolver(),
                        Settings.System.SHOW_TOUCHES, 0) != 0);
    }

    private void updateFlingerOptions() {
        // magic communication with surface flinger.
        try {
            IBinder flinger = ServiceManager.getService("SurfaceFlinger");
            if (flinger != null) {
                Parcel data = Parcel.obtain();
                Parcel reply = Parcel.obtain();
                data.writeInterfaceToken("android.ui.ISurfaceComposer");
                flinger.transact(1010, data, reply, 0);
                @SuppressWarnings("unused")
                int showCpu = reply.readInt();
                @SuppressWarnings("unused")
                int enableGL = reply.readInt();
                int showUpdates = reply.readInt();
                updateSwitchPreference(mShowScreenUpdates, showUpdates != 0);
                @SuppressWarnings("unused")
                int showBackground = reply.readInt();
                int disableOverlays = reply.readInt();
                updateSwitchPreference(mDisableOverlays, disableOverlays != 0);
                reply.recycle();
                data.recycle();
            }
        } catch (RemoteException ex) {
        }
    }

    private void writeShowUpdatesOption() {
        try {
            IBinder flinger = ServiceManager.getService("SurfaceFlinger");
            if (flinger != null) {
                Parcel data = Parcel.obtain();
                data.writeInterfaceToken("android.ui.ISurfaceComposer");
                final int showUpdates = mShowScreenUpdates.isChecked() ? 1 : 0;
                data.writeInt(showUpdates);
                flinger.transact(1002, data, null, 0);
                data.recycle();

                updateFlingerOptions();
            }
        } catch (RemoteException ex) {
        }
    }

    private void writeDisableOverlaysOption() {
        try {
            IBinder flinger = ServiceManager.getService("SurfaceFlinger");
            if (flinger != null) {
                Parcel data = Parcel.obtain();
                data.writeInterfaceToken("android.ui.ISurfaceComposer");
                final int disableOverlays = mDisableOverlays.isChecked() ? 1 : 0;
                data.writeInt(disableOverlays);
                flinger.transact(1008, data, null, 0);
                data.recycle();

                updateFlingerOptions();
            }
        } catch (RemoteException ex) {
        }
    }

    private void updateHardwareUiOptions() {
        updateSwitchPreference(mForceHardwareUi,
                SystemProperties.getBoolean(HARDWARE_UI_PROPERTY, false));
    }

    private void writeHardwareUiOptions() {
        SystemProperties.set(HARDWARE_UI_PROPERTY, mForceHardwareUi.isChecked() ? "true" : "false");
        pokeSystemProperties();
    }

    private void updateMsaaOptions() {
        updateSwitchPreference(mForceMsaa, SystemProperties.getBoolean(MSAA_PROPERTY, false));
    }

    private void writeMsaaOptions() {
        SystemProperties.set(MSAA_PROPERTY, mForceMsaa.isChecked() ? "true" : "false");
        pokeSystemProperties();
    }

    private void updateTrackFrameTimeOptions() {
        String value = SystemProperties.get(ThreadedRenderer.PROFILE_PROPERTY);
        if (value == null) {
            value = "";
        }

        CharSequence[] values = mTrackFrameTime.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if (value.contentEquals(values[i])) {
                mTrackFrameTime.setValueIndex(i);
                mTrackFrameTime.setSummary(mTrackFrameTime.getEntries()[i]);
                return;
            }
        }
        mTrackFrameTime.setValueIndex(0);
        mTrackFrameTime.setSummary(mTrackFrameTime.getEntries()[0]);
    }

    private void writeTrackFrameTimeOptions(Object newValue) {
        SystemProperties.set(ThreadedRenderer.PROFILE_PROPERTY,
                newValue == null ? "" : newValue.toString());
        pokeSystemProperties();
        updateTrackFrameTimeOptions();
    }

    private void updateShowNonRectClipOptions() {
        String value = SystemProperties.get(
                ThreadedRenderer.DEBUG_SHOW_NON_RECTANGULAR_CLIP_PROPERTY);
        if (value == null) {
            value = "hide";
        }

        CharSequence[] values = mShowNonRectClip.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if (value.contentEquals(values[i])) {
                mShowNonRectClip.setValueIndex(i);
                mShowNonRectClip.setSummary(mShowNonRectClip.getEntries()[i]);
                return;
            }
        }
        mShowNonRectClip.setValueIndex(0);
        mShowNonRectClip.setSummary(mShowNonRectClip.getEntries()[0]);
    }

    private void writeShowNonRectClipOptions(Object newValue) {
        SystemProperties.set(ThreadedRenderer.DEBUG_SHOW_NON_RECTANGULAR_CLIP_PROPERTY,
                newValue == null ? "" : newValue.toString());
        pokeSystemProperties();
        updateShowNonRectClipOptions();
    }

    private void updateShowHwScreenUpdatesOptions() {
        updateSwitchPreference(mShowHwScreenUpdates,
                SystemProperties.getBoolean(ThreadedRenderer.DEBUG_DIRTY_REGIONS_PROPERTY, false));
    }

    private void writeShowHwScreenUpdatesOptions() {
        SystemProperties.set(ThreadedRenderer.DEBUG_DIRTY_REGIONS_PROPERTY,
                mShowHwScreenUpdates.isChecked() ? "true" : null);
        pokeSystemProperties();
    }

    private void updateShowHwLayersUpdatesOptions() {
        updateSwitchPreference(mShowHwLayersUpdates, SystemProperties.getBoolean(
                ThreadedRenderer.DEBUG_SHOW_LAYERS_UPDATES_PROPERTY, false));
    }

    private void writeShowHwLayersUpdatesOptions() {
        SystemProperties.set(ThreadedRenderer.DEBUG_SHOW_LAYERS_UPDATES_PROPERTY,
                mShowHwLayersUpdates.isChecked() ? "true" : null);
        pokeSystemProperties();
    }

    private void updateDebugHwOverdrawOptions() {
        String value = SystemProperties.get(ThreadedRenderer.DEBUG_OVERDRAW_PROPERTY);
        if (value == null) {
            value = "";
        }

        CharSequence[] values = mDebugHwOverdraw.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if (value.contentEquals(values[i])) {
                mDebugHwOverdraw.setValueIndex(i);
                mDebugHwOverdraw.setSummary(mDebugHwOverdraw.getEntries()[i]);
                return;
            }
        }
        mDebugHwOverdraw.setValueIndex(0);
        mDebugHwOverdraw.setSummary(mDebugHwOverdraw.getEntries()[0]);
    }

    private void writeDebugHwOverdrawOptions(Object newValue) {
        SystemProperties.set(ThreadedRenderer.DEBUG_OVERDRAW_PROPERTY,
                newValue == null ? "" : newValue.toString());
        pokeSystemProperties();
        updateDebugHwOverdrawOptions();
    }

    private void updateDebugLayoutOptions() {
        updateSwitchPreference(mDebugLayout,
                SystemProperties.getBoolean(View.DEBUG_LAYOUT_PROPERTY, false));
    }

    private void writeDebugLayoutOptions() {
        SystemProperties.set(View.DEBUG_LAYOUT_PROPERTY,
                mDebugLayout.isChecked() ? "true" : "false");
        pokeSystemProperties();
    }

    private void updateSimulateColorSpace() {
        final ContentResolver cr = getContentResolver();
        final boolean enabled = Settings.Secure.getInt(
                cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER_ENABLED, 0) != 0;
        if (enabled) {
            final String mode = Integer.toString(Settings.Secure.getInt(
                    cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER,
                    AccessibilityManager.DALTONIZER_DISABLED));
            mSimulateColorSpace.setValue(mode);
            final int index = mSimulateColorSpace.findIndexOfValue(mode);
            if (index < 0) {
                // We're using a mode controlled by accessibility preferences.
                mSimulateColorSpace.setSummary(getString(R.string.daltonizer_type_overridden,
                        getString(R.string.accessibility_display_daltonizer_preference_title)));
            } else {
                mSimulateColorSpace.setSummary("%s");
            }
        } else {
            mSimulateColorSpace.setValue(
                    Integer.toString(AccessibilityManager.DALTONIZER_DISABLED));
        }
    }

    /**
     * @return <code>true</code> if the color space preference is currently
     *         controlled by development settings
     */
    private boolean usingDevelopmentColorSpace() {
        final ContentResolver cr = getContentResolver();
        final boolean enabled = Settings.Secure.getInt(
                cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER_ENABLED, 0) != 0;
        if (enabled) {
            final String mode = Integer.toString(Settings.Secure.getInt(
                    cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER,
                    AccessibilityManager.DALTONIZER_DISABLED));
            final int index = mSimulateColorSpace.findIndexOfValue(mode);
            if (index >= 0) {
                // We're using a mode controlled by developer preferences.
                return true;
            }
        }
        return false;
    }

    private void writeSimulateColorSpace(Object value) {
        final ContentResolver cr = getContentResolver();
        final int newMode = Integer.parseInt(value.toString());
        if (newMode < 0) {
            Settings.Secure.putInt(cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER_ENABLED, 0);
        } else {
            Settings.Secure.putInt(cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER_ENABLED, 1);
            Settings.Secure.putInt(cr, Settings.Secure.ACCESSIBILITY_DISPLAY_DALTONIZER, newMode);
        }
    }

    private void updateColorTemperature() {
        updateSwitchPreference(mColorTemperaturePreference,
                SystemProperties.getBoolean(COLOR_TEMPERATURE_PROPERTY, false));
    }

    private void writeColorTemperature() {
        SystemProperties.set(COLOR_TEMPERATURE_PROPERTY,
                mColorTemperaturePreference.isChecked() ? "1" : "0");
        pokeSystemProperties();
        Toast.makeText(getActivity(), R.string.color_temperature_toast, Toast.LENGTH_LONG).show();
    }

    private void updateUSBAudioOptions() {
        updateSwitchPreference(mUSBAudio, Settings.Secure.getInt(getContentResolver(),
                Settings.Secure.USB_AUDIO_AUTOMATIC_ROUTING_DISABLED, 0) != 0);
    }

    private void writeUSBAudioOptions() {
        Settings.Secure.putInt(getContentResolver(),
                Settings.Secure.USB_AUDIO_AUTOMATIC_ROUTING_DISABLED,
                mUSBAudio.isChecked() ? 1 : 0);
    }

    private void updateForceResizableOptions() {
        updateSwitchPreference(mForceResizable, Settings.Global.getInt(getContentResolver(),
                Settings.Global.DEVELOPMENT_FORCE_RESIZABLE_ACTIVITIES, 0) != 0);
    }

    private void writeForceResizableOptions() {
        Settings.Global.putInt(getContentResolver(),
                Settings.Global.DEVELOPMENT_FORCE_RESIZABLE_ACTIVITIES,
                mForceResizable.isChecked() ? 1 : 0);
    }

    private void updateForceRtlOptions() {
        updateSwitchPreference(mForceRtlLayout,
                Settings.Global.getInt(getActivity().getContentResolver(),
                        Settings.Global.DEVELOPMENT_FORCE_RTL, 0) != 0);
    }

    private void writeForceRtlOptions() {
        boolean value = mForceRtlLayout.isChecked();
        Settings.Global.putInt(getActivity().getContentResolver(),
                Settings.Global.DEVELOPMENT_FORCE_RTL, value ? 1 : 0);
        SystemProperties.set(Settings.Global.DEVELOPMENT_FORCE_RTL, value ? "1" : "0");
        LocalePicker.updateLocale(getActivity().getResources().getConfiguration().locale);
    }

    private void updateWifiDisplayCertificationOptions() {
        updateSwitchPreference(mWifiDisplayCertification, Settings.Global.getInt(
                getActivity().getContentResolver(),
                Settings.Global.WIFI_DISPLAY_CERTIFICATION_ON, 0) != 0);
    }

    private void writeWifiDisplayCertificationOptions() {
        Settings.Global.putInt(getActivity().getContentResolver(),
                Settings.Global.WIFI_DISPLAY_CERTIFICATION_ON,
                mWifiDisplayCertification.isChecked() ? 1 : 0);
    }

    private void updateWifiVerboseLoggingOptions() {
        boolean enabled = mWifiManager.getVerboseLoggingLevel() > 0;
        updateSwitchPreference(mWifiVerboseLogging, enabled);
    }

    private void writeWifiVerboseLoggingOptions() {
        mWifiManager.enableVerboseLogging(mWifiVerboseLogging.isChecked() ? 1 : 0);
    }

    private void updateWifiAggressiveHandoverOptions() {
        boolean enabled = mWifiManager.getAggressiveHandover() > 0;
        updateSwitchPreference(mWifiAggressiveHandover, enabled);
    }

    private void writeWifiAggressiveHandoverOptions() {
        mWifiManager.enableAggressiveHandover(mWifiAggressiveHandover.isChecked() ? 1 : 0);
    }

    private void updateWifiAllowScansWithTrafficOptions() {
        boolean enabled = mWifiManager.getAllowScansWithTraffic() > 0;
        updateSwitchPreference(mWifiAllowScansWithTraffic, enabled);
    }

    private void writeWifiAllowScansWithTrafficOptions() {
        mWifiManager.setAllowScansWithTraffic(mWifiAllowScansWithTraffic.isChecked() ? 1 : 0);
    }

    private void updateBluetoothDisableAbsVolumeOptions() {
        updateSwitchPreference(mBluetoothDisableAbsVolume,
                SystemProperties.getBoolean(BLUETOOTH_DISABLE_ABSOLUTE_VOLUME_PROPERTY, false));
    }

    private void writeBluetoothDisableAbsVolumeOptions() {
        SystemProperties.set(BLUETOOTH_DISABLE_ABSOLUTE_VOLUME_PROPERTY,
                mBluetoothDisableAbsVolume.isChecked() ? "true" : "false");
    }

    private void updateMobileDataAlwaysOnOptions() {
        updateSwitchPreference(mMobileDataAlwaysOn, Settings.Global.getInt(
                getActivity().getContentResolver(),
                Settings.Global.MOBILE_DATA_ALWAYS_ON, 0) != 0);
    }

    private void writeMobileDataAlwaysOnOptions() {
        Settings.Global.putInt(getActivity().getContentResolver(),
                Settings.Global.MOBILE_DATA_ALWAYS_ON,
                mMobileDataAlwaysOn.isChecked() ? 1 : 0);
    }

    private String defaultLogdSizeValue() {
        String defaultValue = SystemProperties.get(SELECT_LOGD_DEFAULT_SIZE_PROPERTY);
        if ((defaultValue == null) || (defaultValue.length() == 0)) {
            if (SystemProperties.get("ro.config.low_ram").equals("true")) {
                defaultValue = SELECT_LOGD_SVELTE_DEFAULT_SIZE_VALUE;
            } else {
                defaultValue = SELECT_LOGD_DEFAULT_SIZE_VALUE;
            }
        }
        return defaultValue;
    }

    private void updateLogdSizeValues() {
        if (mLogdSize != null) {
            String currentTag = SystemProperties.get(SELECT_LOGD_TAG_PROPERTY);
            String currentValue = SystemProperties.get(SELECT_LOGD_SIZE_PROPERTY);
            if ((currentTag != null) && currentTag.startsWith(SELECT_LOGD_TAG_SILENCE)) {
                currentValue = SELECT_LOGD_OFF_SIZE_MARKER_VALUE;
            }
            if (mLogpersist != null) {
                String currentLogpersistEnable
                    = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY_ENABLE);
                if ((currentLogpersistEnable == null)
                        || !currentLogpersistEnable.equals("true")
                        || currentValue.equals(SELECT_LOGD_OFF_SIZE_MARKER_VALUE)) {
                    writeLogpersistOption(null, true);
                    mLogpersist.setEnabled(false);
                } else if (mLastEnabledState) {
                    mLogpersist.setEnabled(true);
                }
            }
            if ((currentValue == null) || (currentValue.length() == 0)) {
                currentValue = defaultLogdSizeValue();
            }
            String[] values = getResources().getStringArray(R.array.select_logd_size_values);
            String[] titles = getResources().getStringArray(R.array.select_logd_size_titles);
            int index = 2; // punt to second entry if not found
            if (SystemProperties.get("ro.config.low_ram").equals("true")) {
                mLogdSize.setEntries(R.array.select_logd_size_lowram_titles);
                titles = getResources().getStringArray(R.array.select_logd_size_lowram_titles);
                index = 1;
            }
            String[] summaries = getResources().getStringArray(R.array.select_logd_size_summaries);
            for (int i = 0; i < titles.length; i++) {
                if (currentValue.equals(values[i])
                        || currentValue.equals(titles[i])) {
                    index = i;
                    break;
                }
            }
            mLogdSize.setValue(values[index]);
            mLogdSize.setSummary(summaries[index]);
            mLogdSize.setOnPreferenceChangeListener(this);
        }
    }

    private void writeLogdSizeOption(Object newValue) {
        boolean disable = (newValue != null) &&
            (newValue.toString().equals(SELECT_LOGD_OFF_SIZE_MARKER_VALUE));
        String currentTag = SystemProperties.get(SELECT_LOGD_TAG_PROPERTY);
        if (currentTag == null) {
            currentTag = "";
        }
        // filter clean and unstack all references to our setting
        String newTag = currentTag.replaceAll(
                ",+" + SELECT_LOGD_TAG_SILENCE, "").replaceFirst(
                "^" + SELECT_LOGD_TAG_SILENCE + ",*", "").replaceAll(
                ",+", ",").replaceFirst(
                ",+$", "");
        if (disable) {
            newValue = SELECT_LOGD_MINIMUM_SIZE_VALUE;
            // Make sure snet_event_log get through first, but do not override
            String snetValue = SystemProperties.get(SELECT_LOGD_SNET_TAG_PROPERTY);
            if ((snetValue == null) || (snetValue.length() == 0)) {
                snetValue = SystemProperties.get(SELECT_LOGD_RUNTIME_SNET_TAG_PROPERTY);
                if ((snetValue == null) || (snetValue.length() == 0)) {
                    SystemProperties.set(SELECT_LOGD_SNET_TAG_PROPERTY, "I");
                }
            }
            // Silence all log sources, security logs notwithstanding
            if (newTag.length() != 0) {
                newTag = "," + newTag;
            }
            // Stack settings, stack to help preserve original value
            newTag = SELECT_LOGD_TAG_SILENCE + newTag;
        }
        if (!newTag.equals(currentTag)) {
            SystemProperties.set(SELECT_LOGD_TAG_PROPERTY, newTag);
        }
        String defaultValue = defaultLogdSizeValue();
        final String size = ((newValue != null) && (newValue.toString().length() != 0)) ?
            newValue.toString() : defaultValue;
        SystemProperties.set(SELECT_LOGD_SIZE_PROPERTY, defaultValue.equals(size) ? "" : size);
        SystemProperties.set("ctl.start", "logd-reinit");
        pokeSystemProperties();
        updateLogdSizeValues();
    }

    private void updateLogpersistValues() {
        if (mLogpersist == null) {
            return;
        }
        String currentValue = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY);
        if (currentValue == null) {
            currentValue = "";
        }
        String currentBuffers = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY_BUFFER);
        if ((currentBuffers == null) || (currentBuffers.length() == 0)) {
            currentBuffers = "all";
        }
        int index = 0;
        if (currentValue.equals(SELECT_LOGPERSIST_PROPERTY_SERVICE)) {
            index = 1;
            if (currentBuffers.equals("kernel")) {
                index = 3;
            } else if (!currentBuffers.equals("all") &&
                    !currentBuffers.contains("radio") &&
                    currentBuffers.contains("security") &&
                    currentBuffers.contains("kernel")) {
                index = 2;
                if (!currentBuffers.contains("default")) {
                    String[] contains = { "main", "events", "system", "crash" };
                    for (int i = 0; i < contains.length; i++) {
                        if (!currentBuffers.contains(contains[i])) {
                            index = 1;
                            break;
                        }
                    }
                }
            }
        }
        mLogpersist.setValue(getResources().getStringArray(R.array.select_logpersist_values)[index]);
        mLogpersist.setSummary(getResources().getStringArray(R.array.select_logpersist_summaries)[index]);
        mLogpersist.setOnPreferenceChangeListener(this);
        if (index != 0) {
            mLogpersistCleared = false;
        } else if (!mLogpersistCleared) {
            // would File.delete() directly but need to switch uid/gid to access
            SystemProperties.set(ACTUAL_LOGPERSIST_PROPERTY, SELECT_LOGPERSIST_PROPERTY_CLEAR);
            pokeSystemProperties();
            mLogpersistCleared = true;
        }
    }

    private void setLogpersistOff(boolean update) {
        SystemProperties.set(SELECT_LOGPERSIST_PROPERTY_BUFFER, "");
        // deal with trampoline of empty properties
        SystemProperties.set(ACTUAL_LOGPERSIST_PROPERTY_BUFFER, "");
        SystemProperties.set(SELECT_LOGPERSIST_PROPERTY, "");
        SystemProperties.set(ACTUAL_LOGPERSIST_PROPERTY,
            update ? "" : SELECT_LOGPERSIST_PROPERTY_STOP);
        pokeSystemProperties();
        if (update) {
            updateLogpersistValues();
        } else {
            for (int i = 0; i < 3; i++) {
                String currentValue = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY);
                if ((currentValue == null) || currentValue.equals("")) {
                    break;
                }
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                }
            }
        }
    }

    private void writeLogpersistOption(Object newValue, boolean skipWarning) {
        if (mLogpersist == null) {
            return;
        }
        String currentTag = SystemProperties.get(SELECT_LOGD_TAG_PROPERTY);
        if ((currentTag != null) && currentTag.startsWith(SELECT_LOGD_TAG_SILENCE)) {
            newValue = null;
            skipWarning = true;
        }

        if ((newValue == null) || newValue.toString().equals("")) {
            if (skipWarning) {
                mLogpersistCleared = false;
            } else if (!mLogpersistCleared) {
                // if transitioning from on to off, pop up an are you sure?
                String currentValue = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY);
                if ((currentValue != null) &&
                        currentValue.equals(SELECT_LOGPERSIST_PROPERTY_SERVICE)) {
                    if (mLogpersistClearDialog != null) dismissDialogs();
                    mLogpersistClearDialog = new AlertDialog.Builder(getActivity()).setMessage(
                            getActivity().getResources().getString(
                                    R.string.dev_logpersist_clear_warning_message))
                            .setTitle(R.string.dev_logpersist_clear_warning_title)
                            .setPositiveButton(android.R.string.yes, this)
                            .setNegativeButton(android.R.string.no, this)
                            .show();
                    mLogpersistClearDialog.setOnDismissListener(this);
                    return;
                }
            }
            setLogpersistOff(true);
            return;
        }

        String currentBuffer = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY_BUFFER);
        if ((currentBuffer != null) && !currentBuffer.equals(newValue.toString())) {
            setLogpersistOff(false);
        }
        SystemProperties.set(SELECT_LOGPERSIST_PROPERTY_BUFFER, newValue.toString());
        SystemProperties.set(SELECT_LOGPERSIST_PROPERTY, SELECT_LOGPERSIST_PROPERTY_SERVICE);
        pokeSystemProperties();
        for (int i = 0; i < 3; i++) {
            String currentValue = SystemProperties.get(ACTUAL_LOGPERSIST_PROPERTY);
            if ((currentValue != null)
                    && currentValue.equals(SELECT_LOGPERSIST_PROPERTY_SERVICE)) {
                break;
            }
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
            }
        }
        updateLogpersistValues();
    }

    private void updateUsbConfigurationValues() {
        if (mUsbConfiguration != null) {
            UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);

            String[] values = getResources().getStringArray(R.array.usb_configuration_values);
            String[] titles = getResources().getStringArray(R.array.usb_configuration_titles);
            int index = 0;
            for (int i = 0; i < titles.length; i++) {
                if (manager.isFunctionEnabled(values[i])) {
                    index = i;
                    break;
                }
            }
            mUsbConfiguration.setValue(values[index]);
            mUsbConfiguration.setSummary(titles[index]);
            mUsbConfiguration.setOnPreferenceChangeListener(this);
        }
    }

    private void writeUsbConfigurationOption(Object newValue) {
        UsbManager manager = (UsbManager)getActivity().getSystemService(Context.USB_SERVICE);
        String function = newValue.toString();
        manager.setCurrentFunction(function);
        if (function.equals("none")) {
            manager.setUsbDataUnlocked(false);
        } else {
            manager.setUsbDataUnlocked(true);
        }
    }

    private void writeImmediatelyDestroyActivitiesOptions() {
        try {
            ActivityManagerNative.getDefault().setAlwaysFinish(
                    mImmediatelyDestroyActivities.isChecked());
        } catch (RemoteException ex) {
        }
    }

    private void updateImmediatelyDestroyActivitiesOptions() {
        updateSwitchPreference(mImmediatelyDestroyActivities, Settings.Global.getInt(
                getActivity().getContentResolver(), Settings.Global.ALWAYS_FINISH_ACTIVITIES, 0) != 0);
    }

    private void updateAnimationScaleValue(int which, ListPreference pref) {
        try {
            float scale = mWindowManager.getAnimationScale(which);
            if (scale != 1) {
                mHaveDebugSettings = true;
            }
            CharSequence[] values = pref.getEntryValues();
            for (int i=0; i<values.length; i++) {
                float val = Float.parseFloat(values[i].toString());
                if (scale <= val) {
                    pref.setValueIndex(i);
                    pref.setSummary(pref.getEntries()[i]);
                    return;
                }
            }
            pref.setValueIndex(values.length-1);
            pref.setSummary(pref.getEntries()[0]);
        } catch (RemoteException e) {
        }
    }

    private void updateAnimationScaleOptions() {
        updateAnimationScaleValue(0, mWindowAnimationScale);
        updateAnimationScaleValue(1, mTransitionAnimationScale);
        updateAnimationScaleValue(2, mAnimatorDurationScale);
    }

    private void writeAnimationScaleOption(int which, ListPreference pref, Object newValue) {
        try {
            float scale = newValue != null ? Float.parseFloat(newValue.toString()) : 1;
            mWindowManager.setAnimationScale(which, scale);
            updateAnimationScaleValue(which, pref);
        } catch (RemoteException e) {
        }
    }

    private void updateOverlayDisplayDevicesOptions() {
        String value = Settings.Global.getString(getActivity().getContentResolver(),
                Settings.Global.OVERLAY_DISPLAY_DEVICES);
        if (value == null) {
            value = "";
        }

        CharSequence[] values = mOverlayDisplayDevices.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if (value.contentEquals(values[i])) {
                mOverlayDisplayDevices.setValueIndex(i);
                mOverlayDisplayDevices.setSummary(mOverlayDisplayDevices.getEntries()[i]);
                return;
            }
        }
        mOverlayDisplayDevices.setValueIndex(0);
        mOverlayDisplayDevices.setSummary(mOverlayDisplayDevices.getEntries()[0]);
    }

    private void writeOverlayDisplayDevicesOptions(Object newValue) {
        Settings.Global.putString(getActivity().getContentResolver(),
                Settings.Global.OVERLAY_DISPLAY_DEVICES, (String) newValue);
        updateOverlayDisplayDevicesOptions();
    }

    private void updateAppProcessLimitOptions() {
        try {
            int limit = ActivityManagerNative.getDefault().getProcessLimit();
            CharSequence[] values = mAppProcessLimit.getEntryValues();
            for (int i=0; i<values.length; i++) {
                int val = Integer.parseInt(values[i].toString());
                if (val >= limit) {
                    if (i != 0) {
                        mHaveDebugSettings = true;
                    }
                    mAppProcessLimit.setValueIndex(i);
                    mAppProcessLimit.setSummary(mAppProcessLimit.getEntries()[i]);
                    return;
                }
            }
            mAppProcessLimit.setValueIndex(0);
            mAppProcessLimit.setSummary(mAppProcessLimit.getEntries()[0]);
        } catch (RemoteException e) {
        }
    }

    private void writeAppProcessLimitOptions(Object newValue) {
        try {
            int limit = newValue != null ? Integer.parseInt(newValue.toString()) : -1;
            ActivityManagerNative.getDefault().setProcessLimit(limit);
            updateAppProcessLimitOptions();
        } catch (RemoteException e) {
        }
    }

    private void writeShowAllANRsOptions() {
        Settings.Secure.putInt(getActivity().getContentResolver(),
                Settings.Secure.ANR_SHOW_BACKGROUND,
                mShowAllANRs.isChecked() ? 1 : 0);
    }

    private void updateShowAllANRsOptions() {
        updateSwitchPreference(mShowAllANRs, Settings.Secure.getInt(
                getActivity().getContentResolver(), Settings.Secure.ANR_SHOW_BACKGROUND, 0) != 0);
    }

    private void confirmEnableOemUnlock() {
        DialogInterface.OnClickListener onClickListener = new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (which == DialogInterface.BUTTON_POSITIVE) {
                    Utils.setOemUnlockEnabled(getActivity(), true);
                }
            }
        };

        DialogInterface.OnDismissListener onDismissListener = new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialog) {
                if (getActivity() == null) {
                    return;
                }
                updateAllOptions();
            }
        };

        new AlertDialog.Builder(getActivity())
                .setTitle(R.string.confirm_enable_oem_unlock_title)
                .setMessage(R.string.confirm_enable_oem_unlock_text)
                .setPositiveButton(R.string.enable_text, onClickListener)
                .setNegativeButton(android.R.string.cancel, null)
                .setOnDismissListener(onDismissListener)
                .create()
                .show();
    }

    @Override
    public void onSwitchChanged(Switch switchView, boolean isChecked) {
        if (switchView != mSwitchBar.getSwitch()) {
            return;
        }
        if (isChecked != mLastEnabledState) {
            if (isChecked) {
                mDialogClicked = false;
                if (mEnableDialog != null) dismissDialogs();
                mEnableDialog = new AlertDialog.Builder(getActivity()).setMessage(
                        getActivity().getResources().getString(
                                R.string.dev_settings_warning_message))
                        .setTitle(R.string.dev_settings_warning_title)
                        .setPositiveButton(android.R.string.yes, this)
                        .setNegativeButton(android.R.string.no, this)
                        .show();
                mEnableDialog.setOnDismissListener(this);
            } else {
                resetDangerousOptions();
                Settings.Global.putInt(getActivity().getContentResolver(),
                        Settings.Global.DEVELOPMENT_SETTINGS_ENABLED, 0);
                mLastEnabledState = isChecked;
                setPrefsEnabledState(mLastEnabledState);
            }
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == RESULT_DEBUG_APP) {
            if (resultCode == Activity.RESULT_OK) {
                mDebugApp = data.getAction();
                writeDebuggerOptions();
                updateDebuggerOptions();
            }
        } else if (requestCode == RESULT_MOCK_LOCATION_APP) {
            if (resultCode == Activity.RESULT_OK) {
                mMockLocationApp = data.getAction();
                writeMockLocation();
                updateMockLocation();
            }
        } else if (requestCode == REQUEST_CODE_ENABLE_OEM_UNLOCK) {
            if (resultCode == Activity.RESULT_OK) {
                if (mEnableOemUnlock.isChecked()) {
                    confirmEnableOemUnlock();
                } else {
                    Utils.setOemUnlockEnabled(getActivity(), false);
                }
            }
        } else {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        if (Utils.isMonkeyRunning()) {
            return false;
        }

        if (preference == mEnableAdb) {
            if (mEnableAdb.isChecked()) {
                mDialogClicked = false;
                if (mAdbDialog != null) dismissDialogs();
                mAdbDialog = new AlertDialog.Builder(getActivity()).setMessage(
                        getActivity().getResources().getString(R.string.adb_warning_message))
                        .setTitle(R.string.adb_warning_title)
                        .setPositiveButton(android.R.string.yes, this)
                        .setNegativeButton(android.R.string.no, this)
                        .show();
                mAdbDialog.setOnDismissListener(this);
            } else {
                Settings.Global.putInt(getActivity().getContentResolver(),
                        Settings.Global.ADB_ENABLED, 0);
                mVerifyAppsOverUsb.setEnabled(false);
                mVerifyAppsOverUsb.setChecked(false);
                updateBugreportOptions();
            }
        } else if (preference == mClearAdbKeys) {
            if (mAdbKeysDialog != null) dismissDialogs();
            mAdbKeysDialog = new AlertDialog.Builder(getActivity())
                        .setMessage(R.string.adb_keys_warning_message)
                        .setPositiveButton(android.R.string.ok, this)
                        .setNegativeButton(android.R.string.cancel, null)
                        .show();
        } else if (preference == mEnableTerminal) {
            final PackageManager pm = getActivity().getPackageManager();
            pm.setApplicationEnabledSetting(TERMINAL_APP_PACKAGE,
                    mEnableTerminal.isChecked() ? PackageManager.COMPONENT_ENABLED_STATE_ENABLED
                            : PackageManager.COMPONENT_ENABLED_STATE_DEFAULT, 0);
        } else if (preference == mBugreportInPower) {
            Settings.Secure.putInt(getActivity().getContentResolver(),
                    Settings.Global.BUGREPORT_IN_POWER_MENU,
                    mBugreportInPower.isChecked() ? 1 : 0);
            setBugreportStorageProviderStatus();
        } else if (preference == mKeepScreenOn) {
            Settings.Global.putInt(getActivity().getContentResolver(),
                    Settings.Global.STAY_ON_WHILE_PLUGGED_IN,
                    mKeepScreenOn.isChecked() ?
                            (BatteryManager.BATTERY_PLUGGED_AC | BatteryManager.BATTERY_PLUGGED_USB) : 0);
        } else if (preference == mBtHciSnoopLog) {
            writeBtHciSnoopLogOptions();
        } else if (preference == mEnableOemUnlock && mEnableOemUnlock.isEnabled()) {
            if (mEnableOemUnlock.isChecked()) {
                if (!showKeyguardConfirmation(getResources(), REQUEST_CODE_ENABLE_OEM_UNLOCK)) {
                    confirmEnableOemUnlock();
                }
            } else {
                Utils.setOemUnlockEnabled(getActivity(), false);
            }
        } else if (preference == mMockLocationAppPref) {
            Intent intent = new Intent(getActivity(), AppPicker.class);
            intent.putExtra(AppPicker.EXTRA_REQUESTIING_PERMISSION,
                    Manifest.permission.ACCESS_MOCK_LOCATION);
            startActivityForResult(intent, RESULT_MOCK_LOCATION_APP);
        } else if (preference == mDebugViewAttributes) {
            Settings.Global.putInt(getActivity().getContentResolver(),
                    Settings.Global.DEBUG_VIEW_ATTRIBUTES,
                    mDebugViewAttributes.isChecked() ? 1 : 0);
        } else if (preference == mForceAllowOnExternal) {
            Settings.Global.putInt(getActivity().getContentResolver(),
                    Settings.Global.FORCE_ALLOW_ON_EXTERNAL,
                    mForceAllowOnExternal.isChecked() ? 1 : 0);
        } else if (preference == mDebugAppPref) {
            Intent intent = new Intent(getActivity(), AppPicker.class);
            intent.putExtra(AppPicker.EXTRA_DEBUGGABLE, true);
            startActivityForResult(intent, RESULT_DEBUG_APP);
        } else if (preference == mWaitForDebugger) {
            writeDebuggerOptions();
        } else if (preference == mVerifyAppsOverUsb) {
            writeVerifyAppsOverUsbOptions();
        } else if (preference == mOtaDisableAutomaticUpdate) {
            writeOtaDisableAutomaticUpdateOptions();
        } else if (preference == mStrictMode) {
            writeStrictModeVisualOptions();
        } else if (preference == mPointerLocation) {
            writePointerLocationOptions();
        } else if (preference == mShowTouches) {
            writeShowTouchesOptions();
        } else if (preference == mShowScreenUpdates) {
            writeShowUpdatesOption();
        } else if (preference == mDisableOverlays) {
            writeDisableOverlaysOption();
        } else if (preference == mImmediatelyDestroyActivities) {
            writeImmediatelyDestroyActivitiesOptions();
        } else if (preference == mShowAllANRs) {
            writeShowAllANRsOptions();
        } else if (preference == mForceHardwareUi) {
            writeHardwareUiOptions();
        } else if (preference == mForceMsaa) {
            writeMsaaOptions();
        } else if (preference == mShowHwScreenUpdates) {
            writeShowHwScreenUpdatesOptions();
        } else if (preference == mShowHwLayersUpdates) {
            writeShowHwLayersUpdatesOptions();
        } else if (preference == mDebugLayout) {
            writeDebugLayoutOptions();
        } else if (preference == mForceRtlLayout) {
            writeForceRtlOptions();
        } else if (preference == mWifiDisplayCertification) {
            writeWifiDisplayCertificationOptions();
        } else if (preference == mWifiVerboseLogging) {
            writeWifiVerboseLoggingOptions();
        } else if (preference == mWifiAggressiveHandover) {
            writeWifiAggressiveHandoverOptions();
        } else if (preference == mWifiAllowScansWithTraffic) {
            writeWifiAllowScansWithTrafficOptions();
        } else if (preference == mMobileDataAlwaysOn) {
            writeMobileDataAlwaysOnOptions();
        } else if (preference == mColorTemperaturePreference) {
            writeColorTemperature();
        } else if (preference == mUSBAudio) {
            writeUSBAudioOptions();
        }else if (preference == mIgnoreHdmiPlugout) {
            writeIgnoreHdmiPlugoutOptions();
        } else if (preference == mYstMode) {
            writeYstModeOptions();
        } else if (preference == mVideoMaxBuf) {
            //confirmEnableCts();
            writeVideoMaxBuffer();
        } else if (preference == mAuto2997Mode) {
            writeAuto2997Options();
        } else if (preference == mForceResizable) {
            writeForceResizableOptions();
        } else if (INACTIVE_APPS_KEY.equals(preference.getKey())) {
            startInactiveAppsFragment();
        } else if (BACKGROUND_CHECK_KEY.equals(preference.getKey())) {
            startBackgroundCheckFragment();
        } else if (preference == mBluetoothDisableAbsVolume) {
            writeBluetoothDisableAbsVolumeOptions();
        } else if (preference == mWebViewMultiprocess) {
            writeWebViewMultiprocessOptions();
        } else if (SHORTCUT_MANAGER_RESET_KEY.equals(preference.getKey())) {
            resetShortcutManagerThrottling();
        } else if (preference == mForceSdAudioMode) {
            writeForceSdAudio();
        } else if (preference == mDisableTryMountMode) {
            writeDisableTryMount();
        } else if (preference == mCheckVolumeMode) {
            writeCheckVolume();
        } else if (preference == mFormatOnErrorMode) {
            writeFormatOnError();
        } else if (preference == mScreenZoomIn) {
            writeScreenZoomIn();
        } else if (preference == mDeepSleepMode) {
            writeDeepSleepMode();
//        } else if (preference == mAudioI2sInMode) {
//            writeAudioI2sInMode();
        } else if (preference == mPerformanceMode) {
            writePerformanceMode();
        } else if (preference == mSearchKeyFrameMode) {
            writeSearchKeyFrameMode();
        } else if (preference == mAudioNativeSRMode) {
            writeAudioNativeSRMode();
        } else if (preference == mCVBSZoomIn) {
            writeCVBSZoomIn();
        } else if (preference == mSubFixed) {
            writeSubFixedMode();
        } else if (preference == mSuperResolutionOnoff) {
            writeSuperResolutionMode();
        } else if (preference == mMaxCLLMaxFALLOnoff) {
            writeMaxCLLMaxFALLOnoff();
        } else {
            return super.onPreferenceTreeClick(preference);
        }

        return false;
    }

    private void startInactiveAppsFragment() {
        ((SettingsActivity) getActivity()).startPreferencePanel(
                InactiveApps.class.getName(),
                null, R.string.inactive_apps_title, null, null, 0);
    }

    private void startBackgroundCheckFragment() {
        ((SettingsActivity) getActivity()).startPreferencePanel(
                BackgroundCheckSummary.class.getName(),
                null, R.string.background_check_title, null, null, 0);
    }

    private boolean showKeyguardConfirmation(Resources resources, int requestCode) {
        return new ChooseLockSettingsHelper(getActivity(), this).launchConfirmationActivity(
                requestCode, resources.getString(R.string.oem_unlock_enable));
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        if (HDCP_CHECKING_KEY.equals(preference.getKey())) {
            SystemProperties.set(HDCP_CHECKING_PROPERTY, newValue.toString());
            updateHdcpValues();
            pokeSystemProperties();
            return true;
        } else if (preference == mWebViewProvider) {
            if (newValue == null) {
                Log.e(TAG, "Tried to set a null WebView provider");
                return false;
            }
            if (writeWebViewProviderOptions(newValue)) {
                return true;
            } else {
                // The user chose a package that became invalid since the list was last updated,
                // show a Toast to explain the situation.
                Toast toast = Toast.makeText(getActivity(),
                        R.string.select_webview_provider_toast_text, Toast.LENGTH_SHORT);
                toast.show();
            }
            return false;
        } else if (preference == mLogdSize) {
            writeLogdSizeOption(newValue);
            return true;
        } else if (preference == mLogpersist) {
            writeLogpersistOption(newValue, false);
            return true;
        } else if (preference == mUsbConfiguration) {
            writeUsbConfigurationOption(newValue);
            return true;
        } else if (preference == mWindowAnimationScale) {
            writeAnimationScaleOption(0, mWindowAnimationScale, newValue);
            return true;
        } else if (preference == mTransitionAnimationScale) {
            writeAnimationScaleOption(1, mTransitionAnimationScale, newValue);
            return true;
        } else if (preference == mAnimatorDurationScale) {
            writeAnimationScaleOption(2, mAnimatorDurationScale, newValue);
            return true;
        } else if (preference == mOverlayDisplayDevices) {
            writeOverlayDisplayDevicesOptions(newValue);
            return true;
        } else if (preference == mTrackFrameTime) {
            writeTrackFrameTimeOptions(newValue);
            return true;
        } else if (preference == mDebugHwOverdraw) {
            writeDebugHwOverdrawOptions(newValue);
            return true;
        } else if (preference == mShowNonRectClip) {
            writeShowNonRectClipOptions(newValue);
            return true;
        } else if (preference == mAppProcessLimit) {
            writeAppProcessLimitOptions(newValue);
            return true;
        } else if (preference == mSimulateColorSpace) {
            writeSimulateColorSpace(newValue);
            return true;
        } else if (preference == mVideoColorMode) {
            writeVideoColorOptions(newValue);
            return true;
        } else if (preference == mHdmiRangeMode) {
            writeHdmiRangeModeOptions(newValue);
            return true;
        } else if (preference == mAudioInMode) {
            writeAudioInModeOptions(newValue);
            return true;
        } else if (preference == mHdrOutputMode) {
            writeHdrOutputModeOptions(newValue);
            return true;
        }  else if (preference == mHdrToSdrGmaMode) {
            writeHdrToSdrGmaMode(newValue);
            return true;
        }
        return false;
    }

    private void dismissDialogs() {
        if (mAdbDialog != null) {
            mAdbDialog.dismiss();
            mAdbDialog = null;
        }
        if (mAdbKeysDialog != null) {
            mAdbKeysDialog.dismiss();
            mAdbKeysDialog = null;
        }
        if (mEnableDialog != null) {
            mEnableDialog.dismiss();
            mEnableDialog = null;
        }
        if (mLogpersistClearDialog != null) {
            mLogpersistClearDialog.dismiss();
            mLogpersistClearDialog = null;
        }
        if (mCountDialog != null) {
            if (timer != null) timer.cancel();
            mCountDialog.dismiss();
            mCountDialog = null;
            updateVideoColorOptions();
        }
    }

    public void onClick(DialogInterface dialog, int which) {
        if (dialog == mAdbDialog) {
            if (which == DialogInterface.BUTTON_POSITIVE) {
                mDialogClicked = true;
                Settings.Global.putInt(getActivity().getContentResolver(),
                        Settings.Global.ADB_ENABLED, 1);
                mVerifyAppsOverUsb.setEnabled(true);
                updateVerifyAppsOverUsbOptions();
                updateBugreportOptions();
            } else {
                // Reset the toggle
                mEnableAdb.setChecked(false);
            }
        } else if (dialog == mAdbKeysDialog) {
            if (which == DialogInterface.BUTTON_POSITIVE) {
                try {
                    IBinder b = ServiceManager.getService(Context.USB_SERVICE);
                    IUsbManager service = IUsbManager.Stub.asInterface(b);
                    service.clearUsbDebuggingKeys();
                } catch (RemoteException e) {
                    Log.e(TAG, "Unable to clear adb keys", e);
                }
            }
        } else if (dialog == mEnableDialog) {
            if (which == DialogInterface.BUTTON_POSITIVE) {
                mDialogClicked = true;
                Settings.Global.putInt(getActivity().getContentResolver(),
                        Settings.Global.DEVELOPMENT_SETTINGS_ENABLED, 1);
                mLastEnabledState = true;
                setPrefsEnabledState(mLastEnabledState);
            } else {
                // Reset the toggle
                mSwitchBar.setChecked(false);
            }
        } else if (dialog == mLogpersistClearDialog) {
            if (which == DialogInterface.BUTTON_POSITIVE) {
                setLogpersistOff(true);
            } else {
                updateLogpersistValues();
            }
        }
    }

    public void onDismiss(DialogInterface dialog) {
        // Assuming that onClick gets called first
        if (dialog == mAdbDialog) {
            if (!mDialogClicked) {
                mEnableAdb.setChecked(false);
            }
            mAdbDialog = null;
        } else if (dialog == mEnableDialog) {
            if (!mDialogClicked) {
                mSwitchBar.setChecked(false);
            }
            mEnableDialog = null;
        } else if (dialog == mLogpersistClearDialog) {
            mLogpersistClearDialog = null;
        }
    }

    @Override
    public void onDestroy() {
        dismissDialogs();
        super.onDestroy();
    }

    void pokeSystemProperties() {
        if (!mDontPokeProperties) {
            //noinspection unchecked
            (new SystemPropPoker()).execute();
        }
    }

    private BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            updateUsbConfigurationValues();
        }
    };

    public static class SystemPropPoker extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            String[] services = ServiceManager.listServices();
            for (String service : services) {
                IBinder obj = ServiceManager.checkService(service);
                if (obj != null) {
                    Parcel data = Parcel.obtain();
                    try {
                        obj.transact(IBinder.SYSPROPS_TRANSACTION, data, null, 0);
                    } catch (RemoteException e) {
                    } catch (Exception e) {
                        Log.i(TAG, "Someone wrote a bad service '" + service
                                + "' that doesn't like to be poked: " + e);
                    }
                    data.recycle();
                }
            }
            return null;
        }
    }

    private static boolean isPackageInstalled(Context context, String packageName) {
        try {
            return context.getPackageManager().getPackageInfo(packageName, 0) != null;
        } catch (NameNotFoundException e) {
            return false;
        }
    }


    /**
     * For Search.
     */
    public static final Indexable.SearchIndexProvider SEARCH_INDEX_DATA_PROVIDER =
            new BaseSearchIndexProvider() {

                private boolean isShowingDeveloperOptions(Context context) {
                    return context.getSharedPreferences(DevelopmentSettings.PREF_FILE,
                            Context.MODE_PRIVATE).getBoolean(
                                    DevelopmentSettings.PREF_SHOW,
                                    android.os.Build.TYPE.equals("eng"));
                }

                @Override
                public List<SearchIndexableResource> getXmlResourcesToIndex(
                        Context context, boolean enabled) {

                    if (!isShowingDeveloperOptions(context)) {
                        return null;
                    }

                    final SearchIndexableResource sir = new SearchIndexableResource(context);
                    sir.xmlResId = R.xml.development_prefs;
                    return Arrays.asList(sir);
                }

                @Override
                public List<String> getNonIndexableKeys(Context context) {
                    if (!isShowingDeveloperOptions(context)) {
                        return null;
                    }

                    final List<String> keys = new ArrayList<String>();
                    if (!showEnableOemUnlockPreference()) {
                        keys.add(ENABLE_OEM_UNLOCK);
                    }
                    return keys;
                }
            };

    private void resetShortcutManagerThrottling() {
        final IShortcutService service = IShortcutService.Stub.asInterface(
                ServiceManager.getService(Context.SHORTCUT_SERVICE));
        if (service != null) {
            try {
                service.resetThrottling();
                Toast.makeText(getActivity(), R.string.reset_shortcut_manager_throttling_complete,
                        Toast.LENGTH_SHORT).show();
            } catch (RemoteException e) {
                Log.e(TAG, "Failed to reset rate limiting", e);
            }
        }
    }

    private void updateOemUnlockSettingDescription() {
        if (mEnableOemUnlock != null) {
            int oemUnlockSummary = R.string.oem_unlock_enable_summary;
            if (isBootloaderUnlocked()) {
                oemUnlockSummary = R.string.oem_unlock_enable_disabled_summary_bootloader_unlocked;
            } else if (isSimLockedDevice()) {
                oemUnlockSummary = R.string.oem_unlock_enable_disabled_summary_sim_locked_device;
            } else if (!isOemUnlockAllowed()) {
                // If the device isn't SIM-locked but OEM unlock is disabled by the system via the
                // user restriction, this means either some other carrier restriction is in place or
                // the device hasn't been able to confirm which restrictions (SIM-lock or otherwise)
                // apply.
                oemUnlockSummary =
                    R.string.oem_unlock_enable_disabled_summary_connectivity_or_locked;
            }
            mEnableOemUnlock.setSummary(getString(oemUnlockSummary));
        }
    }

    /** Returns {@code true} if the device is SIM-locked. Otherwise, returns {@code false}. */
    private boolean isSimLockedDevice() {
        int phoneCount = mTelephonyManager.getPhoneCount();
        for (int i = 0; i < phoneCount; i++) {
            if (mTelephonyManager.getAllowedCarriers(i).size() > 0) {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns {@code true} if the bootloader has been unlocked. Otherwise, returns {code false}.
     */
    private boolean isBootloaderUnlocked() {
        int flashLockState = PersistentDataBlockManager.FLASH_LOCK_UNKNOWN;
        if (mOemUnlockManager != null) {
            flashLockState = mOemUnlockManager.getFlashLockState();
        }

        return flashLockState == PersistentDataBlockManager.FLASH_LOCK_UNLOCKED;
    }

    /**
     * Returns {@code true} if OEM unlock is disallowed by user restriction
     * {@link UserManager#DISALLOW_FACTORY_RESET} or {@link UserManager#DISALLOW_OEM_UNLOCK}.
     * Otherwise, returns {@code false}.
     */
    private boolean isOemUnlockAllowed() {
        UserHandle userHandle = UserHandle.of(UserHandle.myUserId());
        return !(mUm.hasBaseUserRestriction(UserManager.DISALLOW_OEM_UNLOCK, userHandle)
                || mUm.hasBaseUserRestriction(UserManager.DISALLOW_FACTORY_RESET, userHandle));
    }

     //realtek

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

    private void UpdateYstModeOptions() {
        updateSwitchPreference(mYstMode, Settings.System.getInt(
                getActivity().getContentResolver(),
                Settings.System.REALTEK_YST_MODE, 0) != 0);
    }

    private void writeYstModeOptions() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.REALTEK_YST_MODE,
                mYstMode.isChecked() ? 1 : 0);
    }

    private void updateVideoMaxBuf() {
        updateSwitchPreference(mVideoMaxBuf, "1".equals(SystemProperties.get(VIDEO_MAXACQCNT_PROPERTY,"3")));
    }

    private void writeVideoMaxBuffer() {
        final Context mContext = getActivity();
        SystemProperties.set(VIDEO_MAXACQCNT_PROPERTY, mVideoMaxBuf.isChecked() ? "1" : "3");
    }

    private void UpdateAuto2997Options() {
        updateSwitchPreference(mAuto2997Mode, Settings.System.getInt(
                getActivity().getContentResolver(),
                Settings.System.REALTEK_SETUP_AUTO_2997HZ, 0) != 0);
    }

    private void writeAuto2997Options() {
        Settings.System.putInt(getActivity().getContentResolver(),
                Settings.System.REALTEK_SETUP_AUTO_2997HZ,
                mAuto2997Mode.isChecked() ? 1 : 0);
   }

    private void UpdateForceSdAudio() {
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

    private void writeScreenZoomIn() {
        SystemProperties.set(SCREEN_ZOOM_IN_PROPERTY, mScreenZoomIn.isChecked() ? "1" : "0");
        if (mScreenZoomIn.isChecked()) {
            int zoom_ratio = Integer.parseInt(SystemProperties.get(SCREEN_ZOOM_IN_RATIO_PROPERTY,"95"));
            Log.d(TAG, "Set default Screen Zoom to " + zoom_ratio + "%");
            new RtkVoutUtilManager().setDisplayRatio(zoom_ratio);  //default 95% Zoom
        } else {
            new RtkVoutUtilManager().setDisplayRatio(100);
        }
    }
    private void updateScreenZoomIn() {
        updateSwitchPreference(mScreenZoomIn, "1".equals(SystemProperties.get(SCREEN_ZOOM_IN_PROPERTY,"0")));
    }

    private void writeDeepSleepMode() {
        SystemProperties.set(DEEP_SLEEP_MODE_PROPERTY, mDeepSleepMode.isChecked() ? "1" : "0");
    }
    private void updateDeepSleepMode() {
        updateSwitchPreference(mDeepSleepMode, "1".equals(SystemProperties.get(DEEP_SLEEP_MODE_PROPERTY,"0")));
    }

//    private void writeAudioI2sInMode() {
//        SystemProperties.set(PROPERTY_AUDIO_I2S_IN, mAudioI2sInMode.isChecked() ? "1" : "0");
//        SystemProperties.set("persist.rtk.audio.period_size", mAudioI2sInMode.isChecked() ? "1024" : "384");
//    }

//    private void updateAudioI2sInMode() {
//        updateSwitchPreference(mAudioI2sInMode, "1".equals(SystemProperties.get(PROPERTY_AUDIO_I2S_IN,"0")));
//    }

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
        final CharSequence[] entries = mVideoColorMode.getEntries();
        final CharSequence[] entryValues = mVideoColorMode.getEntryValues();
        for (int i=0; i< entryValues.length; i++) {
            if (Integer.parseInt(entryValues[i].toString()) == iVideoColorMode) {
                mVideoColorMode.setSummary(entries[i]);
                mVideoColorMode.setValueIndex(i);
                break;
            }
        }
        //mVideoColorMode.setValueIndex(iVideoColorMode);
        //mVideoColorMode.setSummary(mVideoColorMode.getEntries()[iVideoColorMode]);
    }

    private void writePerformanceMode() {
        SystemProperties.set(PROPERTY_PERFORMANCE_MODE, mPerformanceMode.isChecked() ? "1" : "0");
    }

    private void updatePerformanceMode() {
        updateSwitchPreference(mPerformanceMode, "1".equals(SystemProperties.get(PROPERTY_PERFORMANCE_MODE,"0")));
    }

    private void writeSearchKeyFrameMode() {
        SystemProperties.set(PROPERTY_VIDEO_SEARCHI, mSearchKeyFrameMode.isChecked() ? "true" : "false");
    }

    private void updateSearchKeyFrameMode() {
        updateSwitchPreference(mSearchKeyFrameMode, "true".equals(SystemProperties.get(PROPERTY_VIDEO_SEARCHI,"false")));
    }

    private void prepareAvailableItemsForVideoColorMode() {
        CharSequence[] entries = mVideoColorMode.getEntries();
        CharSequence[] entryValues = mVideoColorMode.getEntryValues();
        int mTVSystem = 0;
        try {
            mTVSystem = (Integer) Settings.System.getInt(getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER);
            Log.d(TAG, "TVSystem[DB]=" + mTVSystem);
        } catch (SettingNotFoundException snfe) {
            Log.d(TAG, "Setting Not Found");
        }
        //Tv System : TV_SYS_2160P_60HZ TV_SYS_2160P_50HZ TV_SYS_4096_2160P_50HZ TV_SYS_4096_2160P_60HZ
        //Video Color Mode does not show more than 8 bit items except YCbCr420 10BIT.
        if (mTVSystem >= 22 && mTVSystem <= 25) {
            CharSequence[] myEntries = {
                "AUTO",
                "RGB444 8BIT",
                "YCbCr444 8BIT",
                "YCbCr422",
                "YCbCr420 8BIT",
                "YCbCr420 10BIT",
                "AUTO DeepColor"
            };
            CharSequence[] myEntryValues = {
                "0",
                "1",
                "4",
                "7",
                "8",
                "9",
                "11"
            };
            mVideoColorMode.setEntries(myEntries);
            mVideoColorMode.setEntryValues(myEntryValues);
        }
    }

    private void writeAudioNativeSRMode() {
        boolean isChecked = mAudioNativeSRMode.isChecked();
        SystemProperties.set(PROPERTY_AUDIO_NATIVE_SR, isChecked ? "1" : "0");
        mRtkAoutUtilManager.setAudioNativeSR(isChecked ? 1 : 0);
    }

    private void updateAudioNativeSRMode() {
        updateSwitchPreference(mAudioNativeSRMode, "1".equals(SystemProperties.get(PROPERTY_AUDIO_NATIVE_SR, "0")));
    }

    //Auto:0  Limited range:1  Full range:2
    private void writeHdmiRangeModeOptions(Object newValue) {
        Log.d(TAG, "New HDMI Range Mode = " + newValue);
        SystemProperties.set(PROPERTY_HDMI_RANGE_MODE, newValue == null ? "0" : newValue.toString());
        updateHdmiRangeModeOptions();
        int mode = Integer.parseInt((String) newValue);
        mRtkVoutUtilManager.setHdmiRange(mode);
    }

    private void updateHdmiRangeModeOptions() {
        String sHdmiRangeMode = SystemProperties.get(PROPERTY_HDMI_RANGE_MODE, "0");
        CharSequence[] entries = mHdmiRangeMode.getEntries();
        CharSequence[] values = mHdmiRangeMode.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if ((values[i].toString()).equals(sHdmiRangeMode)) {
                mHdmiRangeMode.setValueIndex(i);
                mHdmiRangeMode.setSummary(entries[i]);
                return;
            }
        }
        mHdmiRangeMode.setValueIndex(0);
        mHdmiRangeMode.setSummary(entries[0]);
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

    private void writeCVBSZoomIn() {
        if (mCVBSZoomIn.isChecked()) {
            SystemProperties.set(PROPERTY_CVBS_ZOOM_IN_RATIO,"95");
            new RtkVoutUtilManager().setCVBSDisplayRatio(95);
        } else {
            SystemProperties.set(PROPERTY_CVBS_ZOOM_IN_RATIO,"100");
            new RtkVoutUtilManager().setCVBSDisplayRatio(100);
        }
    }
    private void updateCVBSZoomIn() {
        updateSwitchPreference(mCVBSZoomIn, "95".equals(SystemProperties.get(PROPERTY_CVBS_ZOOM_IN_RATIO,"100")));
    }

    private void writeHdrOutputModeOptions(Object newValue) {
        String sHdrOutputMode = newValue == null ? "0" : newValue.toString();
        SystemProperties.set(PROPERTY_HDR_OUTPUT_MODE, sHdrOutputMode);
        mRtkHDMIManager.setHDREotfMode(Integer.parseInt(sHdrOutputMode));
        updateHdrOutputModeOptions();
    }

    private void updateHdrOutputModeOptions() {
        String sHdrOutputMode = SystemProperties.get(PROPERTY_HDR_OUTPUT_MODE, "0");
        CharSequence[] entries = mHdrOutputMode.getEntries();
        CharSequence[] values = mHdrOutputMode.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if ((values[i].toString()).equals(sHdrOutputMode)) {
                mHdrOutputMode.setValueIndex(i);
                mHdrOutputMode.setSummary(entries[i]);
                return;
            }
        }
        mHdrOutputMode.setValueIndex(0);
        mHdrOutputMode.setSummary(entries[0]);
    }

    private void writeSubFixedMode() {
        SystemProperties.set(PROPERTY_SUB_FIXED, mSubFixed.isChecked() ? "1" : "0");
        new RtkVoutUtilManager().setEmbedSubDisplayFixed(mSubFixed.isChecked() ? 1 : 0);
    }

    private void updateSubFixedMode() {
        updateSwitchPreference(mSubFixed, "1".equals(SystemProperties.get(PROPERTY_SUB_FIXED,"0")));
    }

    private void writeSuperResolutionMode() {
        SystemProperties.set(PROPERTY_SUPER_RESOLUTION_ONOFF, mSuperResolutionOnoff.isChecked() ? "1" : "0");
        new RtkVoutUtilManager().setSuperResolutionOff(mSuperResolutionOnoff.isChecked() ? 0 : 1);
    }

    private void updateSuperResolutionMode() {
        updateSwitchPreference(mSuperResolutionOnoff, "1".equals(SystemProperties.get(PROPERTY_SUPER_RESOLUTION_ONOFF,"1")));
    }

    private void writeHdrToSdrGmaMode(Object newValue) {
        String sHdrToSdrGmaMode = newValue == null ? "0" : newValue.toString();
        SystemProperties.set(PROPERTY_HDR_TO_SDR_GMA_MODE, sHdrToSdrGmaMode);
        new RtkVoutUtilManager().setHdrtoSDRgma(Integer.parseInt(sHdrToSdrGmaMode));
        updateHdrToSdrGmaMode();
    }

    private void updateHdrToSdrGmaMode() {
        String sHdrToSdrGmaMode = SystemProperties.get(PROPERTY_HDR_TO_SDR_GMA_MODE, "0");
        CharSequence[] entries = mHdrToSdrGmaMode.getEntries();
        CharSequence[] values = mHdrToSdrGmaMode.getEntryValues();
        for (int i = 0; i < values.length; i++) {
            if ((values[i].toString()).equals(sHdrToSdrGmaMode)) {
                mHdrToSdrGmaMode.setValueIndex(i);
                mHdrToSdrGmaMode.setSummary(entries[i]);
                return;
            }
        }
        mHdrToSdrGmaMode.setValueIndex(0);
        mHdrToSdrGmaMode.setSummary(entries[0]);
    }

    private void writeMaxCLLMaxFALLOnoff() {
        SystemProperties.set(PROPERTY_VIDEO_MAXCLL_MAXFALL, mMaxCLLMaxFALLOnoff.isChecked() ? "1" : "0");
        new RtkVoutUtilManager().setMaxCLLMaxFALLDisable(mMaxCLLMaxFALLOnoff.isChecked() ? 0 : 1);
    }

    private void updateMaxCLLMaxFALLOnoff() {
        updateSwitchPreference(mMaxCLLMaxFALLOnoff, "1".equals(SystemProperties.get(PROPERTY_VIDEO_MAXCLL_MAXFALL,"1")));
    }
}
