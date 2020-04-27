package com.android.settings;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;

import android.provider.Settings;
import com.realtek.hardware.RtkAoutUtilManager;
import com.realtek.hardware.RtkVoutUtilManager;
import android.provider.Settings.SettingNotFoundException;
import com.rtk.net.openwrt.Server;
import com.rtk.net.openwrt.UbusRpc;
import com.rtk.net.openwrt.DlnaStatus;
import android.os.AsyncTask;
import com.realtek.hardware.RtkHDMIManager;

public class BootCompletedReceiver extends BroadcastReceiver {

    private static final String TAG = "BootCompletedReceiver";
    private Context context;
    private RtkAoutUtilManager mRtkAoutUtilManager;
    private RtkVoutUtilManager mRtkVoutUtilManager;
    private RtkHDMIManager mRtkHDMIManager;

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
    private final String PROPERTY_VIDEO_CVBS = "persist.rtk.video.cvbs";
    private static final String PROPERTY_AUDIO_NATIVE_SR = "persist.rtk.audio.native_sr";
    private static final String PROPERTY_HDMI_RANGE_MODE = "persist.rtk.hdmi.rangemode";
    private static final String PROPERTY_CVBS_ZOOM_IN_RATIO = "persist.rtk.vout.cvbszoom.ratio";
    private static final String PROPERTY_HDR_OUTPUT_MODE = "persist.rtk.hdr.output";
    private static final String PROPERTY_SUB_FIXED = "persist.rtk.video.subfixed";
    private static final String PROPERTY_SUPER_RESOLUTION_ONOFF = "persist.rtk.video.sr";
    private static final String PROPERTY_HDR_TO_SDR_GMA_MODE = "persist.rtk.vout.hdrtosdr.gma";
    private static final String HDMI_AUDIO_ONOFF_PROPERTY = "persist.rtk.hdmi.audio";
    private static final String PROPERTY_VIDEO_MAXCLL_MAXFALL = "persist.rtk.video.maxcllmaxfall";

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

        mRtkHDMIManager = new RtkHDMIManager();
        if(mRtkHDMIManager == null)
            Log.d(TAG, "mRtkHDMIManager is null!");

        setDefaultAudioHdmiOutput();
        setDefaultAudioSpdifOutput();
        setDefaultPlaybackEffect();
        setDefaultAudioForceChannelCtrl();  // we set DTS/DTSHD/AC3/DDP/TRUE HD format forcely output 2 channel, excluding AAC/WMAPRO.
        setAudioHdmiFreqMode();
        setDefaultAudioSurroundSoundMode();
        setDefaultPeakLuminance();
        setDefaultVideoCVBS();
        if (UbusRpc.isOpenWrt() || UbusRpc.isXen()) {
            new SetInitDMSNameSameWithDeviceName().execute();
        }
        setDefaultAudioNativeSR();
        setDefaultHdmiRangeMode();
        setDefaultCVBSZoomIn();
        setDefaultHdrOutput();
        setDefaultSubDisplayMode();
        setDefaultSuperResolution();
        setDefaultHdrToSdrGmaMode();
        setDefaultMaxCLLMaxFALL();
    }

    private void setDefaultVideoCVBS() { 
        if (mRtkVoutUtilManager != null) {
            boolean cvbsOn = new RtkVoutUtilManager().isCVBSOn();
            String cvbsProp = SystemProperties.get(PROPERTY_VIDEO_CVBS, cvbsOn?"1":"0");
            if(cvbsProp.equals("0") && cvbsOn == true) {
                mRtkVoutUtilManager.setCVBSOff(1);
            } else if(cvbsProp.equals("1") && cvbsOn == false) {
                mRtkVoutUtilManager.setCVBSOff(0);
            }
           // mRtkVoutUtilManager.setCVBSOff(SystemProperties.get(PROPERTY_VIDEO_CVBS, new RtkVoutUtilManager().isCVBSOn()?"1":"0").equals("1")?0:1);
        }
    }

    private void setDefaultPeakLuminance() {
        if (mRtkVoutUtilManager != null) {
            mRtkVoutUtilManager.setPeakLuminance(SystemProperties.getInt(PROPERTY_PEAK_LUMINANCE, 4));
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
                muteHdmi(false);
            }
        } else if ( str.equals("1") ) { //LPCM 2CH
            Log.d(TAG, "Set HDMI Output mode = 2CH");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_LPCM_DUAL_CH);
                muteHdmi(false);
            }
        } else if ( str.equals("2") ) { //LPCM Multi-CH
            Log.d(TAG, "Set HDMI Output mode = MULTI_CH");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_LPCM_MULTI_CH);
                muteHdmi(false);
            }
        } else if ( str.equals("3") ) { //AUTO
            Log.d(TAG, "Set HDMI Output mode = AUTO");
            if (mRtkAoutUtilManager != null) {
                mRtkAoutUtilManager.setAudioHdmiOutput(AUDIO_DIGITAL_AUTO);
                muteHdmi(false);
            }
        } else {
            Log.d(TAG, "Mute HDMI Output");
            muteHdmi(true);
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

    public class SetInitDMSNameSameWithDeviceName extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... unused) {
            Server server = new Server(context);
            if (server.isConnected()) {
                String mDeviceName = Settings.Global.getString(context.getContentResolver(), Settings.Global.DEVICE_NAME);
                String mDMSName = server.getDMSName();
                if (!mDeviceName.equals(mDMSName)) {
                    server.setDMSName(mDeviceName);
                    Server.Service sv = null;
                    sv = Server.Service.DLNA;
                    DlnaStatus dlnaStatus = (DlnaStatus) server.getServiceStatus(Server.Service.DLNA);
                    if (dlnaStatus.isRunning) {
                        server.enableService(sv, "disable");
                        server.enableService(sv, "enable");
                    }
                }
            }
            return null;
        }
    }

    private void setDefaultAudioNativeSR() {
        if (mRtkAoutUtilManager == null)
            return;
        if (SystemProperties.get(PROPERTY_AUDIO_NATIVE_SR, "0").equals("1")) {
            Log.d(TAG, "Set default Audio Native Sampling Rate.");
            mRtkAoutUtilManager.setAudioNativeSR(1);
        }
    }

    private void setDefaultHdmiRangeMode() {
        Log.d(TAG, "Set default HDMI Range Mode");
        if (mRtkAoutUtilManager != null) {
            int mode = Integer.parseInt(SystemProperties.get(PROPERTY_HDMI_RANGE_MODE, "0"));
            if (mode != 0)
                mRtkVoutUtilManager.setHdmiRange(mode);
        }
    }

    private void setDefaultCVBSZoomIn() {
        Log.d(TAG, "Set default CVBS Zoom In Mode");
        if (mRtkAoutUtilManager != null) {
            int mode = Integer.parseInt(SystemProperties.get(PROPERTY_CVBS_ZOOM_IN_RATIO, "100"));
            if (mode == 95)
                mRtkVoutUtilManager.setCVBSDisplayRatio(95);
        }
    }

    private void setDefaultHdrOutput() {
        if (mRtkHDMIManager == null)
            return;
        String sHdrOutputMode = SystemProperties.get(PROPERTY_HDR_OUTPUT_MODE, "0");
        if (!(sHdrOutputMode.equals("0"))) {
            Log.d(TAG, "Set default HDR Output mode.");
            mRtkHDMIManager.setHDREotfMode(Integer.parseInt(sHdrOutputMode));
        }
    }

    private void setDefaultSubDisplayMode() {
        Log.d(TAG, "Set default Sub Display Mode");
        if (mRtkVoutUtilManager != null) {
            String subfixed = SystemProperties.get(PROPERTY_SUB_FIXED, "0");
            if(subfixed.equals("1")) {
                mRtkVoutUtilManager.setEmbedSubDisplayFixed(1);
            }
        }
    }

    private void setDefaultSuperResolution() {
        Log.d(TAG, "Set default Super Resolution Mode");
        if (mRtkVoutUtilManager != null) {
            String sr = SystemProperties.get(PROPERTY_SUPER_RESOLUTION_ONOFF, "1");
            if(sr.equals("0")) {
                mRtkVoutUtilManager.setSuperResolutionOff(1);
            }
        }
    }

    private void setDefaultHdrToSdrGmaMode() {
        if (mRtkVoutUtilManager != null) {
            String sHdrToSdrGmaMode = SystemProperties.get(PROPERTY_HDR_TO_SDR_GMA_MODE, "0");
            if (!(sHdrToSdrGmaMode.equals("0"))) {
                Log.d(TAG, "Set default HDR To SDR Gma mode="+sHdrToSdrGmaMode);
                mRtkVoutUtilManager.setHdrtoSDRgma(Integer.parseInt(sHdrToSdrGmaMode));
            }
        }
    }

    private void muteHdmi(boolean _mute) {
        Log.d(TAG, "muteHdmi(), mute=" + _mute);
        if (_mute) {
            mRtkAoutUtilManager.setAudioHdmiMute(true);
            SystemProperties.set(HDMI_AUDIO_ONOFF_PROPERTY, "off");
        } else {
            mRtkAoutUtilManager.setAudioHdmiMute(false);
            SystemProperties.set(HDMI_AUDIO_ONOFF_PROPERTY, "on");
        }
    }

    private void setDefaultMaxCLLMaxFALL() {
        if (mRtkVoutUtilManager != null) {
            String sMaxCLLMaxFALLenable = SystemProperties.get(PROPERTY_VIDEO_MAXCLL_MAXFALL, "1");
            if (sMaxCLLMaxFALLenable.equals("0")) {
                Log.d(TAG, "Set default MaxCLLMaxFALL="+sMaxCLLMaxFALLenable);
                mRtkVoutUtilManager.setMaxCLLMaxFALLDisable(1);
            }
        }
    }
}

