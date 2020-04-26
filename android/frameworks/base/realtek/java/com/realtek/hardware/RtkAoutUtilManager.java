/*
 * Copyright (c) 2013 Realtek Semi. co. All Rights Reserved.
 * Author:  Stanely
 * Version: V0.1
 * Date:  2013-12-27
 * Comment: This class lets you access the AudioUtil function.
 */

package com.realtek.hardware;

import java.util.StringTokenizer;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import com.realtek.hardware.IRtkAoutUtilService;

public class RtkAoutUtilManager {
    private static final String TAG = "RtkAoutUtilManager";

    private IRtkAoutUtilService mRtkAoutUtilService;

    public RtkAoutUtilManager() {
        mRtkAoutUtilService = IRtkAoutUtilService.Stub.asInterface(ServiceManager.getService("RtkAoutUtilService"));
        if(mRtkAoutUtilService != null) {
            Log.i(TAG, mRtkAoutUtilService.toString());
        }
        else {
            Log.e(TAG, "error! CANNOT get RtkAoutUtilService!");
        }
    }

    public boolean init() {
        boolean result = false;

        if(mRtkAoutUtilService != null) {
            try {
                result = mRtkAoutUtilService.init();
                Log.d(TAG, "RtkAoutUtilManager::init() [1]");
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::init() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkAoutUtilManager::init() [2]");
            result = false;
        }
        return result;
    }

    public boolean setAudioSpdifOutput(int mode) {
        boolean result = false;

        if(mRtkAoutUtilService != null) {
            try {
                result = mRtkAoutUtilService.setAudioSpdifOutput(mode);
                Log.d(TAG, "RtkAoutUtilManager::setAudioSpdifOutput() [1], mode = " + mode);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioSpdifOutput() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkAoutUtilManager::setAudioSpdifOutput() [2]");
            result = false;
        }
        return result;
    }

    public boolean setAudioHdmiOutput(int mode) {
        boolean result = false;

        if(mRtkAoutUtilService != null) {
            try {
                result = mRtkAoutUtilService.setAudioHdmiOutput(mode);
                Log.d(TAG, "RtkAoutUtilManager::setAudioHdmiOutput() [1], mode = " + mode);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioHdmiOutput() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkAoutUtilManager::setAudioHdmiOutput() [2]");
            result = false;
        }
        return result;
    }

    public boolean setAudioAGC(int mode) {
        boolean result = false;

        if(mRtkAoutUtilService != null) {
            try {
                result = mRtkAoutUtilService.setAudioAGC(mode);
                Log.d(TAG, "RtkAoutUtilManager::setAudioAGC() [1], mode = " + mode);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioAGC() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkAoutUtilManager::setAudioAGC() [2]");
            result = false;
        }
        return result;
    }

    public boolean setAudioForceChannelCtrl(int mode) {
        boolean result = false;

        if(mRtkAoutUtilService != null) {
            try {
                result = mRtkAoutUtilService.setAudioForceChannelCtrl(mode);
                Log.d(TAG, "RtkAoutUtilManager::setAudioForceChannelCtrl() [1], mode = " + mode);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioForceChannelCtrl() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkAoutUtilManager::setAudioForceChannelCtrl() [2]");
            result = false;
        }
        return result;
    }

    public boolean setAudioHdmiFreqMode() {
        boolean result = false;

        if(mRtkAoutUtilService != null) {
            try {
                result = mRtkAoutUtilService.setAudioHdmiFreqMode();
                Log.d(TAG, "RtkAoutUtilManager::setAudioHdmiFreqMode() [1]");
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioHdmiFreqMode() [1] failed", e);
            }
        } else {
            Log.d(TAG, "RtkAoutUtilManager::setAudioHdmiFreqMode() [2]");
            result = false;
        }
        return result;
    }

    public void setAudioSurroundSoundMode(int mode) {
        if(mRtkAoutUtilService != null) {
            try {
                mRtkAoutUtilService.setAudioSurroundSoundMode(mode);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioSurroundSoundMode failed", e);
            }
        } else {
            Log.d(TAG, "mRtkAoutUtilService is NULL !!");
        }
    }

    public void setAudioNativeSR(int mode) {
        if(mRtkAoutUtilService != null) {
            try {
                mRtkAoutUtilService.setAudioNativeSR(mode);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioNativeSR failed", e);
            }
        } else {
            Log.d(TAG, "mRtkAoutUtilService is NULL !!");
        }
    }

    public void setAudioDelay(int delay) {
        if(mRtkAoutUtilService != null) {
            try {
                mRtkAoutUtilService.setAudioDelay(delay);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioDelay failed", e);
            }
        } else {
            Log.d(TAG, "mRtkAoutUtilService is NULL !!");
        }
    }

    public void setAudioHdmiMute(boolean bMute) {
        if(mRtkAoutUtilService != null) {
            try {
                mRtkAoutUtilService.setAudioHdmiMute(bMute);
            } catch (Exception e) {
                Log.e(TAG, "RtkAoutUtilManager::setAudioI2SDelay failed", e);
            }
        } else {
            Log.d(TAG, "mRtkAoutUtilService is NULL !!");
        }
    }
}
