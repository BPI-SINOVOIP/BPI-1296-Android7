package com.realtek.server;

import java.util.ArrayList;

import android.util.Slog;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.RemoteException;
import android.os.SystemProperties;
import com.realtek.hardware.IRtkAoutUtilService;
//import com.realtek.server.RtkTVSystem;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.content.ContentResolver;

/*
 * The implementation of the RtkAoutUtilService.
 */
public class RtkAoutUtilService extends IRtkAoutUtilService.Stub {
    static {
        System.loadLibrary("realtek_runtime");
    }

    private static final String TAG = "RtkAoutUtilService";

    /**
     * Binder context for this service
     */
    private Context mContext;

    /**
     * Constructs a new RtkAoutUtilService instance.
     *
     */
    public RtkAoutUtilService(Context context) {
        mContext = context;
        Slog.w(TAG, "RtkAoutUtilService is constructed!");
        SystemProperties.set("ctl.start", "regService");

    }

    public boolean init() {
        return _init();
    }

    public boolean setAudioSpdifOutput(int mode) {
        return _setAudioSpdifOutput(mode);
    }

    public boolean setAudioHdmiOutput(int mode) {
        return _setAudioHdmiOutput(mode);
    }

    public boolean setAudioAGC(int mode) {
        return _setAudioAGC(mode);
    }

    public boolean setAudioForceChannelCtrl(int mode) {
        return _setAudioForceChannelCtrl(mode);
    }

    public boolean setAudioHdmiFreqMode() {
        return _setAudioHdmiFreqMode();
    }

    public void setAudioSurroundSoundMode(int mode) {
        _setAudioSurroundSoundMode(mode);
    }

    public void setAudioNativeSR(int mode) {
         _setAudioNativeSR(mode);
    }

    public void setAudioDelay(int delay) {
        _setAudioDelay(delay);
    }

    public void setAudioHdmiMute(boolean bMute) {
        _setAudioHdmiMute(bMute);
    }

    /**
     * Below declaration are implemented by c native code
     * that implemented by device/realtek/frameworks/services/jni/com_realtek_server_RtkAoutUtilService.cpp
     */

    private native boolean _init();
    private native boolean _setAudioSpdifOutput(int mode);
    private native boolean _setAudioHdmiOutput(int mode);
    private native boolean _setAudioAGC(int mode);
    private native boolean _setAudioForceChannelCtrl(int mode);
    private native boolean _setAudioHdmiFreqMode();
    private native void _setAudioSurroundSoundMode(int mode);
    private native void _setAudioNativeSR(int mode);
    private native void _setAudioDelay(int delay);
    private native void _setAudioHdmiMute(boolean bMute);

}
