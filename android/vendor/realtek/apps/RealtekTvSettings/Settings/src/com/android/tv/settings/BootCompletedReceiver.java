package com.android.tv.settings;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;

import android.provider.Settings;
import com.realtek.hardware.RtkAoutUtilManager;
import com.realtek.hardware.RtkVoutUtilManager;
import android.provider.Settings.SettingNotFoundException;

public class BootCompletedReceiver extends BroadcastReceiver {

    private static final String TAG = "BootCompletedReceiver";
    private Context context;
    private RtkAoutUtilManager mRtkAoutUtilManager;
    private RtkVoutUtilManager mRtkVoutUtilManager;

    private static final int AUDIO_DIGITAL_RAW = 0;
    private static final int AUDIO_DIGITAL_LPCM_DUAL_CH = 1;
    private static final int AUDIO_DIGITAL_LPCM_MULTI_CH = 2;
    private static final int AUDIO_DIGITAL_AUTO = 3;

    private static final int FORCE_DTS_2CH = (1 << 0);
    private static final int FORCE_DTSHD_2CH = (1 << 1);
    private static final int FORCE_AC3_2CH = (1 << 2);
    private static final int FORCE_DDP_2CH = (1 << 3);
    private static final int FORCE_MLP_2CH = (1 << 4);
    private static final int FORCE_AAC_2CH = (1 << 5);
    private static final int FORCE_WMAPRO_2CH = (1 << 6);

    private final String PROPERTY_PEAK_LUMINANCE = "persist.rtk.video.peakluminance";

    @Override
    public void onReceive(Context arg0, Intent arg1) {
        Log.d(TAG, "Set initial settings ...");
        this.context = arg0;

        mRtkAoutUtilManager = new RtkAoutUtilManager();
        if(mRtkAoutUtilManager == null)
            Log.d(TAG, "mRtkAoutUtilManager is null!");

        mRtkVoutUtilManager = new RtkVoutUtilManager();
        if(mRtkVoutUtilManager == null)
            Log.d(TAG, "mRtkVoutUtilManager is null!");

        setDefaultAudioHdmiOutput();
        setDefaultAudioSpdifOutput();
        setDefaultPlaybackEffect();
        setDefaultAudioForceChannelCtrl();  // we set DTS/DTSHD/AC3/DDP/TRUE HD format forcely output 2 channel, excluding AAC/WMAPRO.
        setAudioHdmiFreqMode();
        setDefaultAudioSurroundSoundMode();
        setDefaultPeakLuminance();
    }

    private void setDefaultPeakLuminance() {
        if (mRtkVoutUtilManager != null) {
            mRtkVoutUtilManager.setPeakLuminance(SystemProperties.getInt(PROPERTY_PEAK_LUMINANCE, 0));
        }
    }

    //HDMI output
    private void setDefaultAudioHdmiOutput() {
        String str;
        str = Settings.System.getString(this.context.getContentResolver(), Settings.System.REALTEK_SETUP_HDMI_OUTPUT_MODE);
        if ( str.equals("0") ) { //RAW
            Log.d(TAG, "Set HDMI Output mode = RAW");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_RAW);
            }
        } else if ( str.equals("1") ) { //LPCM 2CH
            Log.d(TAG, "Set HDMI Output mode = 2CH");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_LPCM_DUAL_CH);
            }
        } else if ( str.equals("2") ) { //LPCM Multi-CH
            Log.d(TAG, "Set HDMI Output mode = MULTI_CH");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_LPCM_MULTI_CH);
            }
        } else if ( str.equals("3") ) { //AUTO
            Log.d(TAG, "Set HDMI Output mode = AUTO");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_AUTO);
            }
        } else {
            Log.d(TAG, "Unsupportd HDMI Output");
        }
        str = null;
    }

    //S/PDIF output
    private void setDefaultAudioSpdifOutput() {
        String str;
        str = Settings.System.getString(this.context.getContentResolver(), Settings.System.REALTEK_SETUP_SPDIF_OUTPUT_MODE);
        if ( str.equals("0") ) { //LPCM
            Log.d(TAG, "Set SPDIF Output mode = LPCM");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioSpdifOutput(AUDIO_DIGITAL_LPCM_DUAL_CH);
            }
        } else if ( str.equals("1") ) { //Raw
            Log.d(TAG, "Set SPDIF Output mode = RAW");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioSpdifOutput(AUDIO_DIGITAL_RAW);
            }
        } else {
            Log.d(TAG, "Unsupportd S/PDIF Output");
        }
        str = null;
    }

    //Playback effect
    private void setDefaultPlaybackEffect() {
        String str;
        str = Settings.System.getString(this.context.getContentResolver(), Settings.System.REALTEK_SETUP_PLAYBACK_EFFECT_MODE);
        Log.d(TAG, "Set Playback effect mode = " + str);
        int agcMode = 0;
        if ( str.equals("night") ) {
            agcMode=1;
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioAGC(agcMode);
            }
        } else if ( str.equals("comfort") ) {
            agcMode=2;
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioAGC(agcMode);
            }
        } else { //OFF
            agcMode=0;
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioAGC(agcMode);
            }
        }
    }

    // Audio Force 2 channel ctrl, this setting will not affected the RAW output, so set it always.
    private void setDefaultAudioForceChannelCtrl() {
        Log.d(TAG, "Set audio force channel");
        if (mRtkAoutUtilManager != null) {
            int forceCtrl = FORCE_DTS_2CH|FORCE_AC3_2CH|FORCE_DTSHD_2CH|FORCE_DDP_2CH|FORCE_MLP_2CH;
            mRtkAoutUtilManager.setAudioForceChannelCtrl(forceCtrl);
            Log.d(TAG, "Set audio force channel, value = " + forceCtrl);
        }
    }

    // set hdmi frequency to audio.
    private void setAudioHdmiFreqMode() {
        Log.d(TAG, "Set audio hdmi frequency mode");
        if (mRtkAoutUtilManager != null) {
            mRtkAoutUtilManager.setAudioHdmiFreqMode();
        }
    }

    private void setDefaultAudioSurroundSoundMode() {
        Log.d(TAG, "Set default Audio Surround Sound Mode...");
        if (mRtkAoutUtilManager == null)
            return;

        int AUDIO_UNKNOWN_TYPE = 0; //don't care
        int AUDIO_AC3_DECODER_TYPE = 2; //SD audio
        int AUDIO_MLP_DECODER_TYPE = 9; //HD audio

        int mode = AUDIO_UNKNOWN_TYPE;
        try {
            mode = Settings.System.getInt(this.context.getContentResolver(), Settings.System.REALTEK_SETUP_FORCE_SDAUDIO);
        } catch (SettingNotFoundException snfe) {
            Log.e(TAG, Settings.System.REALTEK_SETUP_FORCE_SDAUDIO + " not found");
        }

        mRtkAoutUtilManager.setAudioSurroundSoundMode(mode);
    }

}

