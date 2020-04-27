package com.realtek.hdmirxinput;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.ComponentName;
import android.database.Cursor;
import android.media.MediaPlayer;
import android.media.tv.TvContract;
import android.media.tv.TvInputManager;
import android.media.tv.TvInputService;
import android.media.tv.TvInputHardwareInfo;
import android.media.tv.TvInputInfo;
import android.media.tv.TvStreamConfig;
import android.net.Uri;
import android.view.Surface;
import android.util.Log;
import android.content.pm.ResolveInfo;
import android.content.pm.PackageManager;
import android.content.Intent;
import android.content.ContentResolver;
import java.io.IOException;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.os.SystemProperties;

public class HDMIRxTvInputService extends TvInputService {
    private static final boolean DEBUG = true;

    private static final String TAG="HDMITvInputService";
    private static final String TAG_SESSION=TAG+"-Session";
    private static final String TAG_DUMMY=TAG+"-DummySession";

    private TvInputInfo mInputInfo = null;
    private TvInputManager mTvInputManager=null;
    private TvInputManager.Hardware mHardware=null;
    private TvStreamConfig mSidebandConfig=null;

    private String mInputInfoId = null;
    private static HDMIRxSessionImpl mSession=null;

    @Override
    public Session onCreateSession(String inputId) {
        Log.d(TAG, "onCreateSession inputId:"+inputId);
        // check hardware info first
        if(mHardware==null){
            Log.e(TAG, "no tv hardwar plugged, return DummySessionImpl");
            return new DummySessionImpl(this);
        }

        if(HDMIRxTvInputService.mSession == null){
            HDMIRxTvInputService.mSession = new HDMIRxSessionImpl(this);
            return HDMIRxTvInputService.mSession;
        }else{
            return new DummySessionImpl(this);
        }
    }

    // Callback to acquire hardware info and list
    class HDMIRxHardwareCallback extends TvInputManager.HardwareCallback{

        public void onReleased(){
            if(DEBUG) Log.d(TAG, "onRelease");
        }

        public void onStreamConfigChanged(TvStreamConfig[] configs) {
            if(DEBUG) Log.d(TAG, "onStreamConfigChanged");
            for(int i=0;i<configs.length;i++){
                TvStreamConfig config = configs[i];
                if(config.getType() == TvStreamConfig.STREAM_TYPE_INDEPENDENT_VIDEO_SOURCE){
                    if(DEBUG) Log.d(TAG, "Device support sideband - config:"+config);
                    mSidebandConfig = config;
                }
            }
        }
    }

    private void cleanControls(){
        mInputInfo = null;
        mHardware = null;
        mSidebandConfig = null;
    }

    private TvInputManager getTvInputManager(){
        if(mTvInputManager==null){
            mTvInputManager = (TvInputManager) getSystemService(Context.TV_INPUT_SERVICE);
        }
        return mTvInputManager;
    }

    @Override
    public TvInputInfo onHardwareAdded(TvInputHardwareInfo hardwareInfo) {
        Log.d(TAG, "onHardwareAdded hardwareInfo:"+hardwareInfo);
        // only report none null object if type is HDMI
        // maybe also check id (of HDMI Rx)
        if(hardwareInfo.getType() == TvInputHardwareInfo.TV_INPUT_TYPE_HDMI){
            PackageManager manager = getPackageManager();
            Intent intent = new Intent(this, HDMIRxTvInputService.class);
            int flags = PackageManager.GET_META_DATA|PackageManager.GET_SERVICES;
            ResolveInfo serviceInfo = manager.resolveService(intent, flags);
            try{
                mInputInfo = TvInputInfo.createTvInputInfo(this, serviceInfo, hardwareInfo, "HDMIRx", null);
                if(DEBUG) Log.d(TAG, "my hardware is added, mInputInfo:"+mInputInfo);
            }catch(IOException e){
                e.printStackTrace();
                mInputInfo = null;
            }catch(XmlPullParserException e){
                e.printStackTrace();
                mInputInfo = null;
            }
        }

        if(mInputInfo!=null){
            if(DEBUG) Log.d(TAG, "acquire hardware info");
            HDMIRxHardwareCallback cb = new HDMIRxHardwareCallback();
            mHardware = getTvInputManager().acquireTvInputHardware(hardwareInfo.getDeviceId(),cb,mInputInfo);
            mInputInfoId = mInputInfo.getId();
        }

        if(mSession!=null){
            mSession.notifySessionEvent("add",null);
        }

        return mInputInfo;
    }

    @Override
    public String onHardwareRemoved(TvInputHardwareInfo hardwareInfo) {
        Log.d(TAG, "onHardwareRemoved: "+hardwareInfo+" mInputInfo:"+mInputInfoId);
        if(hardwareInfo.getType() == TvInputHardwareInfo.TV_INPUT_TYPE_HDMI /*&& mInputInfo!=null */){
            if(mSession!=null){
                mSession.notifySessionEvent("remove",null);
            }

            // remove callback
            if(mHardware != null){
                if(DEBUG) Log.d(TAG, "release tv input hardware");
                getTvInputManager().releaseTvInputHardware(hardwareInfo.getDeviceId(), mHardware);
            }
            if(DEBUG) Log.d(TAG, " my hardware is removed, report id:"+mInputInfoId);
            cleanControls();
            return mInputInfoId;
        }else{
            return null;
        }
    }

    // HDMIRx session
    private class HDMIRxSessionImpl extends TvInputService.Session {

        private Uri mChannelUri;
        private TvInputManager mManager;

        HDMIRxSessionImpl(Context context){
            super(context);
            mManager = (TvInputManager) context.getSystemService(Context.TV_INPUT_SERVICE);
        }

        public void onRelease() {
            if(DEBUG) Log.d(TAG_SESSION,"onRelease");
            HDMIRxTvInputService.mSession = null;
        }

        public void onSetStreamVolume(float volume) {
        }

        public void onSetCaptionEnabled(boolean enabled) {
        }

        public boolean onSetSurface(Surface surface) {
            if(DEBUG) Log.d(TAG_SESSION, "onSetSurface surface:"+surface+" this :"+this);

            if(mHardware == null || mSidebandConfig == null){
                Log.e(TAG_SESSION, "onSetSurface error");
                return false;
            }

            if(surface !=null){
                mHardware.setSurface(surface, mSidebandConfig);
                notifyVideoAvailable();
            }else{
                mHardware.setSurface(surface, mSidebandConfig);
                HDMIRxTvInputService.mSession = null;
            }
            return true;
        }

        public boolean onTune(Uri channelUri) {
            if(DEBUG) Log.d(TAG_SESSION, "onTune uri:"+channelUri);
            mChannelUri = channelUri;
            return true;
        }

    } // end Session

    // Dummy session
    private class DummySessionImpl extends TvInputService.Session {

        DummySessionImpl(Context context){
            super(context);
        }

        public void onRelease() {
            if(DEBUG){Log.d(TAG_DUMMY, "onRelease");}
        }

        public void onSetStreamVolume(float volume) {
        }

        public void onSetCaptionEnabled(boolean enabled) {
        }

        public boolean onSetSurface(Surface surface) {
            if(DEBUG) Log.d(TAG_DUMMY, "onSetSurface surface:"+surface);
            return false;
        }

        public boolean onTune(Uri channelUri) {
            return false;
        }

    } // end Session

}
