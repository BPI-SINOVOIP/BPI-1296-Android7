/*
 * Copyright (C) 2010 The Android Open Source Project
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
import android.app.ActivityManager;
import android.app.UiModeManager;
import android.app.WallpaperManager;
import android.app.admin.DevicePolicyManager;
import android.content.ContentResolver;
import android.content.Context;
import android.content.ComponentName;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.os.UserManager;
import android.provider.SearchIndexableResource;
import android.provider.Settings;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.DropDownPreference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.Preference.OnPreferenceChangeListener;
import android.text.TextUtils;
import android.util.Log;

import com.android.internal.app.NightDisplayController;
import com.android.internal.logging.MetricsLogger;
import com.android.internal.logging.MetricsProto.MetricsEvent;
import com.android.internal.view.RotationPolicy;
import com.android.settings.accessibility.ToggleFontSizePreferenceFragment;
import com.android.settings.dashboard.SummaryLoader;
import com.android.settings.search.BaseSearchIndexProvider;
import com.android.settings.search.Indexable;
import com.android.settingslib.RestrictedLockUtils;
import com.android.settingslib.RestrictedPreference;

import java.util.ArrayList;
import java.util.List;

import static android.provider.Settings.Secure.CAMERA_GESTURE_DISABLED;
import static android.provider.Settings.Secure.DOUBLE_TAP_TO_WAKE;
import static android.provider.Settings.Secure.DOZE_ENABLED;
import static android.provider.Settings.Secure.WAKE_GESTURE_ENABLED;
import static android.provider.Settings.System.SCREEN_BRIGHTNESS_MODE;
import static android.provider.Settings.System.SCREEN_BRIGHTNESS_MODE_AUTOMATIC;
import static android.provider.Settings.System.SCREEN_BRIGHTNESS_MODE_MANUAL;
import static android.provider.Settings.System.SCREEN_OFF_TIMEOUT;

import static com.android.settingslib.RestrictedLockUtils.EnforcedAdmin;

//realtek
import com.realtek.hardware.RtkTVSystem;
import com.realtek.hardware.RtkHDMIManager;
import com.realtek.hardware.RtkVoutUtilManager;
import android.provider.Settings.SettingNotFoundException;
import android.support.v7.preference.CheckBoxPreference;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.os.CountDownTimer;

public class DisplaySettings extends SettingsPreferenceFragment implements
        Preference.OnPreferenceChangeListener, Indexable {
    private static final String TAG = "DisplaySettings";

    /** If there is no setting in the provider, use this. */
    private static final int FALLBACK_SCREEN_TIMEOUT_VALUE = 30000;

    private static final String KEY_SCREEN_TIMEOUT = "screen_timeout";
    private static final String KEY_FONT_SIZE = "font_size";
    private static final String KEY_SCREEN_SAVER = "screensaver";
    private static final String KEY_LIFT_TO_WAKE = "lift_to_wake";
    private static final String KEY_DOZE = "doze";
    private static final String KEY_TAP_TO_WAKE = "tap_to_wake";
    private static final String KEY_AUTO_BRIGHTNESS = "auto_brightness";
    private static final String KEY_AUTO_ROTATE = "auto_rotate";
    private static final String KEY_NIGHT_DISPLAY = "night_display";
    private static final String KEY_NIGHT_MODE = "night_mode";
    private static final String KEY_CAMERA_GESTURE = "camera_gesture";
    private static final String KEY_WALLPAPER = "wallpaper";
    private static final String KEY_VR_DISPLAY_PREF = "vr_display_pref";
    
    //realtek
    private static final String KEY_TV_SYSTEM_LIST = "tv_system_list";
    private static final String KEY_AUTO_1080P24 = "auto_1080p24_onoff";
    private static final String KEY_CVBS_ONOFF = "cvbs_onoff";
    private static final String PROPERTY_VIDEO_CVBS = "persist.rtk.video.cvbs";
    private Preference mFontSizePref;

    private TimeoutListPreference mScreenTimeoutPreference;
    private ListPreference mNightModePreference;
    private Preference mScreenSaverPreference;
    private SwitchPreference mLiftToWakePreference;
    private SwitchPreference mDozePreference;
    private SwitchPreference mTapToWakePreference;
    private SwitchPreference mAutoBrightnessPreference;
    private SwitchPreference mCameraGesturePreference;
   
    //realtek    
    private ListPreference mTVSystemPreference;
    private CheckBoxPreference mAuto1080p24OnOff;
    private RtkHDMIManager mRtkHDMIManager;
    private AlertDialog countDlg;
    private int oldVideoSystem = 0;
    private int valueToBeStoredForTVSystem = 0;
    private CountDownTimer timer;
    private int timeToWait = 15000; //15 sec
    private SwitchPreference mCvbsOnoffPreference;

    @Override
    protected int getMetricsCategory() {
        return MetricsEvent.DISPLAY;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        final Activity activity = getActivity();
        final ContentResolver resolver = activity.getContentResolver();

        addPreferencesFromResource(R.xml.display_settings);

        mScreenSaverPreference = findPreference(KEY_SCREEN_SAVER);
        if (mScreenSaverPreference != null
                && getResources().getBoolean(
                        com.android.internal.R.bool.config_dreamsSupported) == false) {
            getPreferenceScreen().removePreference(mScreenSaverPreference);
        }

        mScreenTimeoutPreference = (TimeoutListPreference) findPreference(KEY_SCREEN_TIMEOUT);
        final long currentTimeout = Settings.System.getLong(resolver, SCREEN_OFF_TIMEOUT,
                FALLBACK_SCREEN_TIMEOUT_VALUE);
        mScreenTimeoutPreference.setValue(String.valueOf(currentTimeout));
        mScreenTimeoutPreference.setOnPreferenceChangeListener(this);
        disableUnusableTimeouts(mScreenTimeoutPreference);
        updateTimeoutPreferenceDescription(currentTimeout);

        mFontSizePref = findPreference(KEY_FONT_SIZE);

        if (isAutomaticBrightnessAvailable(getResources())) {
            mAutoBrightnessPreference = (SwitchPreference) findPreference(KEY_AUTO_BRIGHTNESS);
            mAutoBrightnessPreference.setOnPreferenceChangeListener(this);
        } else {
            removePreference(KEY_AUTO_BRIGHTNESS);
        }

        if (!NightDisplayController.isAvailable(activity)) {
            removePreference(KEY_NIGHT_DISPLAY);
        }

        if (isLiftToWakeAvailable(activity)) {
            mLiftToWakePreference = (SwitchPreference) findPreference(KEY_LIFT_TO_WAKE);
            mLiftToWakePreference.setOnPreferenceChangeListener(this);
        } else {
            removePreference(KEY_LIFT_TO_WAKE);
        }

        if (isDozeAvailable(activity)) {
            mDozePreference = (SwitchPreference) findPreference(KEY_DOZE);
            mDozePreference.setOnPreferenceChangeListener(this);
        } else {
            removePreference(KEY_DOZE);
        }

        if (isTapToWakeAvailable(getResources())) {
            mTapToWakePreference = (SwitchPreference) findPreference(KEY_TAP_TO_WAKE);
            mTapToWakePreference.setOnPreferenceChangeListener(this);
        } else {
            removePreference(KEY_TAP_TO_WAKE);
        }

        if (isCameraGestureAvailable(getResources())) {
            mCameraGesturePreference = (SwitchPreference) findPreference(KEY_CAMERA_GESTURE);
            mCameraGesturePreference.setOnPreferenceChangeListener(this);
        } else {
            removePreference(KEY_CAMERA_GESTURE);
        }

        if (RotationPolicy.isRotationLockToggleVisible(activity)) {
            DropDownPreference rotatePreference =
                    (DropDownPreference) findPreference(KEY_AUTO_ROTATE);
            int rotateLockedResourceId;
            // The following block sets the string used when rotation is locked.
            // If the device locks specifically to portrait or landscape (rather than current
            // rotation), then we use a different string to include this information.
            if (allowAllRotations(activity)) {
                rotateLockedResourceId = R.string.display_auto_rotate_stay_in_current;
            } else {
                if (RotationPolicy.getRotationLockOrientation(activity)
                        == Configuration.ORIENTATION_PORTRAIT) {
                    rotateLockedResourceId =
                            R.string.display_auto_rotate_stay_in_portrait;
                } else {
                    rotateLockedResourceId =
                            R.string.display_auto_rotate_stay_in_landscape;
                }
            }
            rotatePreference.setEntries(new CharSequence[] {
                    activity.getString(R.string.display_auto_rotate_rotate),
                    activity.getString(rotateLockedResourceId),
            });
            rotatePreference.setEntryValues(new CharSequence[] { "0", "1" });
            rotatePreference.setValueIndex(RotationPolicy.isRotationLocked(activity) ?
                    1 : 0);
            rotatePreference.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
                @Override
                public boolean onPreferenceChange(Preference preference, Object newValue) {
                    final boolean locked = Integer.parseInt((String) newValue) != 0;
                    MetricsLogger.action(getActivity(), MetricsEvent.ACTION_ROTATION_LOCK,
                            locked);
                    RotationPolicy.setRotationLock(activity, locked);
                    return true;
                }
            });
        } else {
            removePreference(KEY_AUTO_ROTATE);
        }

        if (isVrDisplayModeAvailable(activity)) {
            DropDownPreference vrDisplayPref =
                    (DropDownPreference) findPreference(KEY_VR_DISPLAY_PREF);
            vrDisplayPref.setEntries(new CharSequence[] {
                    activity.getString(R.string.display_vr_pref_low_persistence),
                    activity.getString(R.string.display_vr_pref_off),
            });
            vrDisplayPref.setEntryValues(new CharSequence[] { "0", "1" });

            final Context c = activity;
            int currentUser = ActivityManager.getCurrentUser();
            int current = Settings.Secure.getIntForUser(c.getContentResolver(),
                            Settings.Secure.VR_DISPLAY_MODE,
                            /*default*/Settings.Secure.VR_DISPLAY_MODE_LOW_PERSISTENCE,
                            currentUser);
            vrDisplayPref.setValueIndex(current);
            vrDisplayPref.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
                @Override
                public boolean onPreferenceChange(Preference preference, Object newValue) {
                    int i = Integer.parseInt((String) newValue);
                    int u = ActivityManager.getCurrentUser();
                    if (!Settings.Secure.putIntForUser(c.getContentResolver(),
                            Settings.Secure.VR_DISPLAY_MODE,
                            i, u)) {
                        Log.e(TAG, "Could not change setting for " +
                                Settings.Secure.VR_DISPLAY_MODE);
                    }
                    return true;
                }
            });
        } else {
            removePreference(KEY_VR_DISPLAY_PREF);
        }

        //realtek
        mRtkHDMIManager = new RtkHDMIManager();
        if(mRtkHDMIManager == null)
            Log.d(TAG, "mRtkHDMIManager is null!");
        mTVSystemPreference = (ListPreference) findPreference(KEY_TV_SYSTEM_LIST);
        mTVSystemPreference.setOnPreferenceChangeListener(this);
        prepareAvailableItemsForTVSystem();
        mTVSystemPreference.setValue(getDefaultValueOfTVSystem());  // set default selection.
        updateTVSystemSummary();
        oldVideoSystem = Integer.parseInt(getDefaultValueOfTVSystem());

        mAuto1080p24OnOff = (CheckBoxPreference) findPreference(KEY_AUTO_1080P24);
        try {
            mAuto1080p24OnOff.setChecked(Settings.System.getInt(resolver, Settings.System.REALTEK_SETUP_AUTO_1080P24) == 1);
        } catch (SettingNotFoundException snfe) {
            Log.e(TAG, Settings.System.REALTEK_SETUP_AUTO_1080P24 + " not found");
        }

        mNightModePreference = (ListPreference) findPreference(KEY_NIGHT_MODE);
        if (mNightModePreference != null) {
            final UiModeManager uiManager = (UiModeManager) getSystemService(
                    Context.UI_MODE_SERVICE);
            final int currentNightMode = uiManager.getNightMode();
            mNightModePreference.setValue(String.valueOf(currentNightMode));
            mNightModePreference.setOnPreferenceChangeListener(this);
        }
        mCvbsOnoffPreference = (SwitchPreference) findPreference(KEY_CVBS_ONOFF);
        mCvbsOnoffPreference.setOnPreferenceChangeListener(this);
    }

    private static boolean allowAllRotations(Context context) {
        return Resources.getSystem().getBoolean(
                com.android.internal.R.bool.config_allowAllRotations);
    }

    private static boolean isLiftToWakeAvailable(Context context) {
        SensorManager sensors = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        return sensors != null && sensors.getDefaultSensor(Sensor.TYPE_WAKE_GESTURE) != null;
    }

    private static boolean isDozeAvailable(Context context) {
        String name = Build.IS_DEBUGGABLE ? SystemProperties.get("debug.doze.component") : null;
        if (TextUtils.isEmpty(name)) {
            name = context.getResources().getString(
                    com.android.internal.R.string.config_dozeComponent);
        }
        return !TextUtils.isEmpty(name);
    }

    private static boolean isTapToWakeAvailable(Resources res) {
        return res.getBoolean(com.android.internal.R.bool.config_supportDoubleTapWake);
    }

    private static boolean isAutomaticBrightnessAvailable(Resources res) {
        return res.getBoolean(com.android.internal.R.bool.config_automatic_brightness_available);
    }

    private static boolean isCameraGestureAvailable(Resources res) {
        boolean configSet = res.getInteger(
                com.android.internal.R.integer.config_cameraLaunchGestureSensorType) != -1;
        return configSet &&
                !SystemProperties.getBoolean("gesture.disable_camera_launch", false);
    }

    private static boolean isVrDisplayModeAvailable(Context context) {
        PackageManager pm = context.getPackageManager();
        return pm.hasSystemFeature(PackageManager.FEATURE_VR_MODE_HIGH_PERFORMANCE);
    }

    private void updateTimeoutPreferenceDescription(long currentTimeout) {
        TimeoutListPreference preference = mScreenTimeoutPreference;
        String summary;
        if (preference.isDisabledByAdmin()) {
            summary = getString(R.string.disabled_by_policy_title);
        } else if (currentTimeout < 0) {
            // Unsupported value
            summary = "";
        } else {
            final CharSequence[] entries = preference.getEntries();
            final CharSequence[] values = preference.getEntryValues();
            if (entries == null || entries.length == 0) {
                summary = "";
            } else {
                int best = 0;
                for (int i = 0; i < values.length; i++) {
                    long timeout = Long.parseLong(values[i].toString());
                    if (currentTimeout >= timeout) {
                        best = i;
                    }
                }
                summary = getString(R.string.screen_timeout_summary, entries[best]);
            }
        }
        preference.setSummary(summary);
    }

    private void disableUnusableTimeouts(ListPreference screenTimeoutPreference) {
        final DevicePolicyManager dpm =
                (DevicePolicyManager) getActivity().getSystemService(
                Context.DEVICE_POLICY_SERVICE);
        final long maxTimeout = dpm != null ? dpm.getMaximumTimeToLock(null) : 0;
        if (maxTimeout == 0) {
            return; // policy not enforced
        }
        final CharSequence[] entries = screenTimeoutPreference.getEntries();
        final CharSequence[] values = screenTimeoutPreference.getEntryValues();
        ArrayList<CharSequence> revisedEntries = new ArrayList<CharSequence>();
        ArrayList<CharSequence> revisedValues = new ArrayList<CharSequence>();
        for (int i = 0; i < values.length; i++) {
            long timeout = Long.parseLong(values[i].toString());
            if (timeout <= maxTimeout) {
                revisedEntries.add(entries[i]);
                revisedValues.add(values[i]);
            }
        }
        if (revisedEntries.size() != entries.length || revisedValues.size() != values.length) {
            final int userPreference = Integer.parseInt(screenTimeoutPreference.getValue());
            screenTimeoutPreference.setEntries(
                    revisedEntries.toArray(new CharSequence[revisedEntries.size()]));
            screenTimeoutPreference.setEntryValues(
                    revisedValues.toArray(new CharSequence[revisedValues.size()]));
            if (userPreference <= maxTimeout) {
                screenTimeoutPreference.setValue(String.valueOf(userPreference));
            } else if (revisedValues.size() > 0
                    && Long.parseLong(revisedValues.get(revisedValues.size() - 1).toString())
                    == maxTimeout) {
                // If the last one happens to be the same as the max timeout, select that
                screenTimeoutPreference.setValue(String.valueOf(maxTimeout));
            } else {
                // There will be no highlighted selection since nothing in the list matches
                // maxTimeout. The user can still select anything less than maxTimeout.
                // TODO: maybe append maxTimeout to the list and mark selected.
            }
        }
        screenTimeoutPreference.setEnabled(revisedEntries.size() > 0);
    }

    int floatToIndex(float val) {
        String[] indices = getResources().getStringArray(R.array.entryvalues_font_size);
        float lastVal = Float.parseFloat(indices[0]);
        for (int i=1; i<indices.length; i++) {
            float thisVal = Float.parseFloat(indices[i]);
            if (val < (lastVal + (thisVal-lastVal)*.5f)) {
                return i-1;
            }
            lastVal = thisVal;
        }
        return indices.length-1;
    }

    @Override
    public void onResume() {
        super.onResume();
        updateState();

        final long currentTimeout = Settings.System.getLong(getActivity().getContentResolver(),
                SCREEN_OFF_TIMEOUT, FALLBACK_SCREEN_TIMEOUT_VALUE);
        mScreenTimeoutPreference.setValue(String.valueOf(currentTimeout));
        mScreenTimeoutPreference.setOnPreferenceChangeListener(this);
        final DevicePolicyManager dpm = (DevicePolicyManager) getActivity().getSystemService(
                Context.DEVICE_POLICY_SERVICE);
        if (dpm != null) {
            final EnforcedAdmin admin = RestrictedLockUtils.checkIfMaximumTimeToLockIsSet(
                    getActivity());
            final long maxTimeout = dpm
                    .getMaximumTimeToLockForUserAndProfiles(UserHandle.myUserId());
            mScreenTimeoutPreference.removeUnusableTimeouts(maxTimeout, admin);
        }
        updateTimeoutPreferenceDescription(currentTimeout);

        disablePreferenceIfManaged(KEY_WALLPAPER, UserManager.DISALLOW_SET_WALLPAPER);
    }

    private void updateState() {
        updateFontSizeSummary();
        updateScreenSaverSummary();

        // Update auto brightness if it is available.
        if (mAutoBrightnessPreference != null) {
            int brightnessMode = Settings.System.getInt(getContentResolver(),
                    SCREEN_BRIGHTNESS_MODE, SCREEN_BRIGHTNESS_MODE_MANUAL);
            mAutoBrightnessPreference.setChecked(brightnessMode != SCREEN_BRIGHTNESS_MODE_MANUAL);
        }

        // Update lift-to-wake if it is available.
        if (mLiftToWakePreference != null) {
            int value = Settings.Secure.getInt(getContentResolver(), WAKE_GESTURE_ENABLED, 0);
            mLiftToWakePreference.setChecked(value != 0);
        }

        //realtek
        updateTVSystemSummary();
        if (mCvbsOnoffPreference != null) {
            mCvbsOnoffPreference.setChecked(new RtkVoutUtilManager().isCVBSOn());
        }

        // Update tap to wake if it is available.
        if (mTapToWakePreference != null) {
            int value = Settings.Secure.getInt(getContentResolver(), DOUBLE_TAP_TO_WAKE, 0);
            mTapToWakePreference.setChecked(value != 0);
        }

        // Update doze if it is available.
        if (mDozePreference != null) {
            int value = Settings.Secure.getInt(getContentResolver(), DOZE_ENABLED, 1);
            mDozePreference.setChecked(value != 0);
        }

        // Update camera gesture #1 if it is available.
        if (mCameraGesturePreference != null) {
            int value = Settings.Secure.getInt(getContentResolver(), CAMERA_GESTURE_DISABLED, 0);
            mCameraGesturePreference.setChecked(value == 0);
        }
    }

    private void updateScreenSaverSummary() {
        if (mScreenSaverPreference != null) {
            mScreenSaverPreference.setSummary(
                    DreamSettings.getSummaryTextWithDreamName(getActivity()));
        }
    }

    private void updateFontSizeSummary() {
        final Context context = mFontSizePref.getContext();
        final float currentScale = Settings.System.getFloat(context.getContentResolver(),
                Settings.System.FONT_SCALE, 1.0f);
        final Resources res = context.getResources();
        final String[] entries = res.getStringArray(R.array.entries_font_size);
        final String[] strEntryValues = res.getStringArray(R.array.entryvalues_font_size);
        final int index = ToggleFontSizePreferenceFragment.fontSizeValueToIndex(currentScale,
                strEntryValues);
        mFontSizePref.setSummary(entries[index]);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object objValue) {
        final String key = preference.getKey();
        if (KEY_SCREEN_TIMEOUT.equals(key)) {
            try {
                int value = Integer.parseInt((String) objValue);
                Settings.System.putInt(getContentResolver(), SCREEN_OFF_TIMEOUT, value);
                updateTimeoutPreferenceDescription(value);
            } catch (NumberFormatException e) {
                Log.e(TAG, "could not persist screen timeout setting", e);
            }
        }
        if (preference == mAutoBrightnessPreference) {
            boolean auto = (Boolean) objValue;
            Settings.System.putInt(getContentResolver(), SCREEN_BRIGHTNESS_MODE,
                    auto ? SCREEN_BRIGHTNESS_MODE_AUTOMATIC : SCREEN_BRIGHTNESS_MODE_MANUAL);
        }
        if (preference == mLiftToWakePreference) {
            boolean value = (Boolean) objValue;
            Settings.Secure.putInt(getContentResolver(), WAKE_GESTURE_ENABLED, value ? 1 : 0);
        }
        if (preference == mDozePreference) {
            boolean value = (Boolean) objValue;
            Settings.Secure.putInt(getContentResolver(), DOZE_ENABLED, value ? 1 : 0);
        }
        if (KEY_TV_SYSTEM_LIST.equals(key)) {
            Log.d(TAG, "Set HDMI TV System...");
            valueToBeStoredForTVSystem = Integer.parseInt((String) objValue);;
            Log.d(TAG, "new video system=" + valueToBeStoredForTVSystem);
            Log.d(TAG, "old video system=" + oldVideoSystem);

            if ((valueToBeStoredForTVSystem >= 0) && (valueToBeStoredForTVSystem != oldVideoSystem)) {
                if (countDlg != null) dismissDialogs();
                countDlg = new AlertDialog.Builder(getActivity()).create();
                countDlg.setTitle(getString(R.string.tv_system_confirm_title));
                countDlg.setCancelable(false);
                countDlg.setMessage(String.format(getString(R.string.tv_system_confirm_message),timeToWait));
                countDlg.setButton("YES", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String mystr = getDefaultValueOfTVSystem();
                        Log.d(TAG, "TV System Setting(DB)=" + mystr);
                        Log.d(TAG, "Save TV System Setting=" + valueToBeStoredForTVSystem);
                        Settings.System.putInt(getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER, valueToBeStoredForTVSystem);
                        oldVideoSystem = valueToBeStoredForTVSystem;
                        timer.cancel();
                        updateTVSystemSummary();
                    }
                });
                countDlg.setOnCancelListener(new OnCancelListener() {
                    @Override
                    public void onCancel(DialogInterface dialog) {
                        Log.d(TAG, "Dialog has been cancel?");
                        if (timer != null) timer.cancel();
                        mRtkHDMIManager.setTVSystem(oldVideoSystem);
                    }
                });
                countDlg.show();

                timer = new CountDownTimer(timeToWait, 1000) {
                    String str = getString(R.string.tv_system_confirm_message);
                    @Override
                    public void onTick(long millisUntilFinished) {
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

                if(mRtkHDMIManager != null) {
                    Log.d(TAG, "Call RtkHDMIManager to set HDMI TV System:" + valueToBeStoredForTVSystem);
                    mRtkHDMIManager.setTVSystem(valueToBeStoredForTVSystem);
                }
            }
        }
        if (preference == mTapToWakePreference) {
            boolean value = (Boolean) objValue;
            Settings.Secure.putInt(getContentResolver(), DOUBLE_TAP_TO_WAKE, value ? 1 : 0);
        }
        if (preference == mCameraGesturePreference) {
            boolean value = (Boolean) objValue;
            Settings.Secure.putInt(getContentResolver(), CAMERA_GESTURE_DISABLED,
                    value ? 0 : 1 /* Backwards because setting is for disabling */);
        }
        if (preference == mNightModePreference) {
            try {
                final int value = Integer.parseInt((String) objValue);
                final UiModeManager uiManager = (UiModeManager) getSystemService(
                        Context.UI_MODE_SERVICE);
                uiManager.setNightMode(value);
            } catch (NumberFormatException e) {
                Log.e(TAG, "could not persist night mode setting", e);
            }
        }
        if (preference == mCvbsOnoffPreference) {
            boolean value = (Boolean) objValue;
            Log.d(TAG, "Set CVBS " + value);
            SystemProperties.set(PROPERTY_VIDEO_CVBS, value?"1":"0");
            new RtkVoutUtilManager().setCVBSOff(value?0:1);
        }
        return true;
    }

    @Override
    public boolean onPreferenceTreeClick(Preference preference) {
        if (preference == mDozePreference) {
            MetricsLogger.action(getActivity(), MetricsEvent.ACTION_AMBIENT_DISPLAY);
        }
        if (preference == mAuto1080p24OnOff) {
            Settings.System.putInt(getContentResolver(), Settings.System.REALTEK_SETUP_AUTO_1080P24, (mAuto1080p24OnOff.isChecked())?1:0);
        }
        return super.onPreferenceTreeClick(preference);
    }

    @Override
    protected int getHelpResource() {
        return R.string.help_uri_display;
    }

    private void disablePreferenceIfManaged(String key, String restriction) {
        final RestrictedPreference pref = (RestrictedPreference) findPreference(key);
        if (pref != null) {
            pref.setDisabledByAdmin(null);
            if (RestrictedLockUtils.hasBaseUserRestriction(getActivity(), restriction,
                    UserHandle.myUserId())) {
                pref.setEnabled(false);
            } else {
                pref.checkRestrictionAndSetDisabled(restriction);
            }
        }
    }

    private static class SummaryProvider implements SummaryLoader.SummaryProvider {
        private final Context mContext;
        private final SummaryLoader mLoader;

        private SummaryProvider(Context context, SummaryLoader loader) {
            mContext = context;
            mLoader = loader;
        }

        @Override
        public void setListening(boolean listening) {
            if (listening) {
                updateSummary();
            }
        }

        private void updateSummary() {
            boolean auto = Settings.System.getInt(mContext.getContentResolver(),
                    SCREEN_BRIGHTNESS_MODE, SCREEN_BRIGHTNESS_MODE_AUTOMATIC)
                    == SCREEN_BRIGHTNESS_MODE_AUTOMATIC;
            mLoader.setSummary(this, mContext.getString(auto ? R.string.display_summary_on
                    : R.string.display_summary_off));
        }
    }

    public static final SummaryLoader.SummaryProviderFactory SUMMARY_PROVIDER_FACTORY
            = new SummaryLoader.SummaryProviderFactory() {
        @Override
        public SummaryLoader.SummaryProvider createSummaryProvider(Activity activity,
                                                                   SummaryLoader summaryLoader) {
            return new SummaryProvider(activity, summaryLoader);
        }
    };

    public static final Indexable.SearchIndexProvider SEARCH_INDEX_DATA_PROVIDER =
            new BaseSearchIndexProvider() {
                @Override
                public List<SearchIndexableResource> getXmlResourcesToIndex(Context context,
                        boolean enabled) {
                    ArrayList<SearchIndexableResource> result =
                            new ArrayList<SearchIndexableResource>();

                    SearchIndexableResource sir = new SearchIndexableResource(context);
                    sir.xmlResId = R.xml.display_settings;
                    result.add(sir);

                    return result;
                }

                @Override
                public List<String> getNonIndexableKeys(Context context) {
                    ArrayList<String> result = new ArrayList<String>();
                    if (!context.getResources().getBoolean(
                            com.android.internal.R.bool.config_dreamsSupported)) {
                        result.add(KEY_SCREEN_SAVER);
                    }
                    if (!isAutomaticBrightnessAvailable(context.getResources())) {
                        result.add(KEY_AUTO_BRIGHTNESS);
                    }
                    if (!NightDisplayController.isAvailable(context)) {
                        result.add(KEY_NIGHT_DISPLAY);
                    }
                    if (!isLiftToWakeAvailable(context)) {
                        result.add(KEY_LIFT_TO_WAKE);
                    }
                    if (!isDozeAvailable(context)) {
                        result.add(KEY_DOZE);
                    }
                    if (!RotationPolicy.isRotationLockToggleVisible(context)) {
                        result.add(KEY_AUTO_ROTATE);
                    }
                    if (!isTapToWakeAvailable(context.getResources())) {
                        result.add(KEY_TAP_TO_WAKE);
                    }
                    if (!isCameraGestureAvailable(context.getResources())) {
                        result.add(KEY_CAMERA_GESTURE);
                    }
                    if (!isVrDisplayModeAvailable(context)) {
                        result.add(KEY_VR_DISPLAY_PREF);
                    }
                    return result;
                }
            };
    private void updateTVSystemSummary() {
        if (mTVSystemPreference != null) {

            try {
                int defaultSet = (Integer) Settings.System.getInt(getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER);
                final CharSequence[] entries = mTVSystemPreference.getEntries();
                final CharSequence[] entryValues = mTVSystemPreference.getEntryValues();
                for (int i=0; i< entryValues.length; i++) {
                    if (Integer.parseInt(entryValues[i].toString()) == defaultSet) {
                        mTVSystemPreference.setSummary(entries[i]);
                    }
                }
            } catch (SettingNotFoundException snfe) {
                Log.d(TAG, "Setting Not Found");
            }
        }
    }
    
    /* Utility function for TV System List Preference.
     * 1. Have EDID info: List EDID resolution & Auto.
     * 2. No EDID: List NTSC (or PAL) 480P, 576P, 720P/I@50Hz, 720P/I@60Hz, 1080P/I@50Hz, 1080P/I@60Hz.
    */
    private boolean prepareAvailableItemsForTVSystem() {
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

//            int numTvSystem = supportVideoFormat.length > availableTvSystems.length? availableTvSystems.length: supportVideoFormat.length;
            int iVideoColorMode = Integer.parseInt(SystemProperties.get("persist.rtk.video.colormode", "0"));
            boolean highBitVideoColorMode = false;
            //Video Color Mode: RGB444 10BIT RGB444 12BIT YCbCr444 10BIT YCbCr444 12BIT YCbCr420 12BIT
            if (iVideoColorMode == 2 || iVideoColorMode == 3 || iVideoColorMode == 5 || iVideoColorMode == 6 || iVideoColorMode == 10) {
                highBitVideoColorMode = true;
            }

            for( int i=0; i< supportVideoFormat.length; i++) {
                Log.d(TAG,"VideoFormat["+i+"] : "+supportVideoFormat[i]);
                if (highBitVideoColorMode && (i >= 22 && i <= 25)) {
                    continue;
                }
                if ( supportVideoFormat[i] == 1)
                    numSupportVideoFormat++;
            }
            Log.d(TAG,"Num of SupoortVideoFormat:"+numSupportVideoFormat);
            CharSequence[] entries = new CharSequence[numSupportVideoFormat+1];
            CharSequence[] entryValues = new CharSequence[numSupportVideoFormat+1];

            int j=0;
            for(int i=0; i< availableTvSystems.length; i++) {
                int tvSysId = Integer.parseInt(availableTvSystems[i][1].toString());
                if (highBitVideoColorMode && (tvSysId >= 22 && tvSysId <= 25)) {
                    continue;
                }
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

        return true;
    }

    /* Utility function will be called to get the default value from Settings.System.REALTEK_SETUP_TV_SYSTEM
     *
    */
    private String getDefaultValueOfTVSystem() {
        Integer oldTVSystem;
        String finalRes;

        try {
            oldTVSystem = (Integer) Settings.System.getInt(getContentResolver(), Settings.System.REALTEK_SETUP_TV_SYSTEM_USER);

            Log.d(TAG, "TVSystem[DB]=" + oldTVSystem);
            finalRes = Integer.toString(oldTVSystem);
        } catch (SettingNotFoundException snfe) {
            Log.d(TAG, "Exception found in getDefaultValueOfTVSystem()");
            finalRes = "0";
            //return null;
        }
        return finalRes.toString();
    }

    private void dismissDialogs() {
        Log.d(TAG, "dismissDialogs...");
        if (countDlg != null) {
            Log.d(TAG, "Stop timer...");
            if (timer != null) timer.cancel();
            countDlg.dismiss();
            countDlg = null;
        }
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "onDestroy()...");
        dismissDialogs();
        super.onDestroy();
    }
}
