
package com.realtek.transcode;

import android.os.RemoteException;
import android.os.ServiceManager;

import android.util.Log;

//import com.realtek.server.ITranscodeService;
import com.realtek.transcode.IRealtekTranscodeService;

public class TranscodeManager {
    private static final String TAG = "TranscodeManager";
    
    //private final ITranscodeService mTranscodeService;
    private final IRealtekTranscodeService mTranscodeService;

    private int mTranscodeIndex = -1;

    public TranscodeManager() {
        //mTranscodeService = ITranscodeService.Stub.asInterface(ServiceManager.getService("transcode"));
        mTranscodeService = IRealtekTranscodeService.Stub.asInterface(ServiceManager.getService("RealtekTranscode"));
        if(mTranscodeService != null){
            Log.d(TAG, mTranscodeService.toString());
        }else{
            Log.e(TAG, "Can't get TranscodeService!");
        }
    }

    public int initTranscode() {
        try {
            mTranscodeIndex = mTranscodeService.getTranscodeInstanceIdx();
        } catch (RemoteException ex) {
            Log.e(TAG, "initTranscode RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        if(mTranscodeIndex == -1){
            Log.e(TAG, "Unable to init transcode index");
            return 1;
        }else{
            Log.i(TAG, "transcode index:" + mTranscodeIndex);
            return 0;
        }
    }

    private void uninitTranscode() {
        mTranscodeIndex = -1;
    }

    public void setDataSource(String source, String target, String output, int track, int repeat) {
        try {
            mTranscodeService.setDataSource(mTranscodeIndex, source, target, output, track, repeat);
        } catch (RemoteException ex) {
            Log.e(TAG, "setDataSource RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
    }

    public void setVideoParameter(int width, int height, String frameRate, int bitRate, int iFrame, 
        int enableDeinterlace, int bitrateMode, int profile, int level) {
        try {
            mTranscodeService.setVideoParameter(mTranscodeIndex, width, height, frameRate, bitRate, iFrame, 
                enableDeinterlace, bitrateMode, profile, level);
        } catch (RemoteException ex) {
            Log.e(TAG, "setVideoParameter RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
    }

    public void setAudioParameter(String audioMode, int sampleRate, int channel) {
        try {
            mTranscodeService.setAudioParameter(mTranscodeIndex, audioMode, sampleRate, channel);
        } catch (RemoteException ex) {
            Log.e(TAG, "setAudioParameter RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
    }

    public void setNetworkParameter(String ip, int port) {
        try {
            mTranscodeService.setStreamingParameter(mTranscodeIndex, ip, port);
        } catch (RemoteException ex) {
            Log.e(TAG, "setStreamingParameter RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
    }

    public int startTranscode() {
        int ret = -1;
        try {
            ret =  mTranscodeService.startTranscode(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "startTranscode RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public int stopTranscode() {
        int ret = -1;
        try {
            ret = mTranscodeService.stopTranscode(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "stopTranscode RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        uninitTranscode();
        return ret;
    }

    public boolean isVideoTranscodeAlive() {
        boolean ret = false;
        try {
            ret =  mTranscodeService.isVideoTranscodeAlive(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "isVideoTranscodeAlive RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public boolean isAudioTranscodeAlive() {
        boolean ret = false;
        try {
            ret = mTranscodeService.isAudioTranscodeAlive(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "isAudioTranscodeAlive RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public long getCurrentPTS() {
        long ret = 0;
        try {
            ret = mTranscodeService.getCurrentPTS(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "getCurrentPTS RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public double getDecodeAverageFPS() {
        double ret = 0.0;
        try {
            ret = mTranscodeService.getDecodeAverageFPS(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "getDecodeAverageFPS RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public double getEncodeAverageFPS() {
        double ret = 0.0;
        try {
            ret = mTranscodeService.getEncodeAverageFPS(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "getEncodeAverageFPS RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public double getVideoDecodeCurrentFPS() {
        double ret = 0.0;
        try {
            ret = mTranscodeService.getVideoDecodeCurrentFPS(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "getVideoDecodeCurrentFPS RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public double getVideoEncodeCurrentFPS() {
        double ret = 0.0;
        try {
            ret = mTranscodeService.getVideoEncodeCurrentFPS(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "getVideoEncodeCurrentFPS RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public int getTranscodeState() {
        int ret = 0;
        try {
            ret = mTranscodeService.getTranscodeState(mTranscodeIndex);
        } catch (RemoteException ex) {
            Log.e(TAG, "getTranscodeState RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
        return ret;
    }

    public void killSpecificTranscodeIdx(int idx) {
        int ret = -1;
        try {
            if(idx != 4)
                ret = mTranscodeService.stopTranscode(idx);
            else {
                int i;
                for(i=0; i<4; i++)
                    ret = mTranscodeService.stopTranscode(i);
            }
        } catch (RemoteException ex) {
            Log.e(TAG, "stopTranscode RemoteException: " + ex.toString() + ": " + ex.getMessage());
        }
    }
}

