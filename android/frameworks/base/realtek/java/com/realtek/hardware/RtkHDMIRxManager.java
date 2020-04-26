/*
 * Copyright (c) 2014 Realtek Semi. co. All Rights Reserved.
 * Author Kevin Su
 * Version V0.1
 * Date:  2014-08-19
 * Comment:This class lets you access the HDMIRx function.
 */

package com.realtek.hardware;

import java.util.StringTokenizer;
import java.io.IOException;
import java.io.File;

import android.app.ActivityThread;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.ParcelFileDescriptor;
import android.os.SystemProperties;
import android.os.UEventObserver;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.TextureView;
import android.graphics.SurfaceTexture;

import com.realtek.hardware.HDMIRxStatus;
import com.realtek.hardware.HDMIRxParameters;
import com.realtek.hardware.IRtkHDMIRxService;

public class RtkHDMIRxManager {
	static {
                System.loadLibrary("realtek_jni");
	}

	private static final String TAG = "RtkHDMIRxManager";

    private final String HdmiRxAudioSwitch = "/sys/devices/virtual/switch/rx_audio/state";
    private final String HdmiRxAudioUeventKeyword = "DEVPATH=/devices/virtual/switch/rx_audio";

	private IRtkHDMIRxService mRtkHDMIRxService;
	
	public static final int HDMIRX_FORMAT_NV12 = 0;
	public static final int HDMIRX_FORMAT_ARGB = 1;
	public static final int HDMIRX_FORMAT_JPEG = 2;

    public static final int HDMIRX_FILE_FORMAT_TS  = 0;
    public static final int HDMIRX_FILE_FORMAT_MP4 = 1;

    public static final int HDMIRX_AUDIO_RAW    = 0;
    public static final int HDMIRX_AUDIO_PCM    = 1;

    /**
     * A simple event listener to listen Rx event
     */
    public interface HDMIRxListener {

        /**
         * Event: Camera error, could be connected by another client
         */
        public static final int EVENT_CAMERA_ERROR = 1;

        public abstract void onEvent(int eventId);
    }

    public HDMIRxListener mListener = null;

	private UEventObserver mHDMIRxAudioObserver = new UEventObserver() {
		@Override
		public void onUEvent(UEventObserver.UEvent event) {
            Log.d(TAG, "[RXA] mHDMIRxAudioObserver:onUEvent "+event.get("SWITCH_STATE")+" start");
			boolean state = "1".equals(event.get("SWITCH_STATE"));
            RtkHDMIRxManager.notifyHDMIRxAudioHotplug(state?1:0);
            Log.d(TAG, "[RXA] mHDMIRxAudioObserver:onUEvent "+event.get("SWITCH_STATE")+" done");
		}
	};

	public RtkHDMIRxManager() {
		mRtkHDMIRxService = IRtkHDMIRxService.Stub.asInterface(ServiceManager.getService("RtkHDMIRxService"));
		if(mRtkHDMIRxService != null) {
			Log.i(TAG, mRtkHDMIRxService.toString());
		}
		else {
			Log.e(TAG, "error! CANNOT get RtkHDMIService!");
		}
	}

    private void monitorRxAudioHotplug() {
        
		if(new File(HdmiRxAudioSwitch).exists()) {
			Log.d(TAG, "[AUDIO-UEvent]startObserving");
			mHDMIRxAudioObserver.startObserving(HdmiRxAudioUeventKeyword);
        }
    }

    public void setListener(HDMIRxListener listener){
        mListener = listener;
    }

    private void handleEventFromNative(int event){
        Log.d(TAG, "native event "+event);
        if(mListener != null) {
            mListener.onEvent(event);
        }
    }


	public boolean isHDMIRxPlugged() {
		boolean result = false;

		if(mRtkHDMIRxService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIRxService!");
			return false;
		}

		try {
			result = mRtkHDMIRxService.isHDMIRxPlugged();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIRxService isHDMIRxPlugged failed!", e);
		}

		return result;
	}

	public HDMIRxStatus getHDMIRxStatus() {
		HDMIRxStatus hdmiRxStatus = null;

		if(mRtkHDMIRxService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIRxService!");
			return null;
		}

		try {
			hdmiRxStatus = mRtkHDMIRxService.getHDMIRxStatus();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIRxService getHDMIRxStatus failed!", e);
		}

		return hdmiRxStatus;
	}

	public boolean setHDMIRxAudio() {
		boolean result = false;

		if(mRtkHDMIRxService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIRxService!");
			return false;
		}

		try {
			result = mRtkHDMIRxService.setHDMIRxAudio();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIRxService setHDMIRxAudio failed!", e);
		}

		return result;
        }

	public boolean muteHDMIRxAudio() {
		boolean result = false;

		if(mRtkHDMIRxService == null) {
			Log.e(TAG, "failed to connect to RtkHDMIRxService!");
			return false;
		}

		try {
			result = mRtkHDMIRxService.muteHDMIRxAudio();
		} catch (Exception e) {
			Log.e(TAG, "RtkHDMIRxService muteHDMIRxAudio failed!", e);
		}

		return result;
        }

    public int open() {
        String packageName = ActivityThread.currentPackageName();
		Log.d(TAG, "hdmi rx open(), package name:" + packageName);
        monitorRxAudioHotplug();
		return native_open(packageName);
    }

    public void release() {
		Log.d(TAG, "hdmi rx release()");
        mHDMIRxAudioObserver.stopObserving();
		native_release();
    }

    public final void setPreviewDisplay(SurfaceHolder holder) throws IOException {
		Log.d(TAG, "hdmi rx setPreviewDisplay(), holder:" + holder);
		Surface surface = (holder == null)? null : holder.getSurface();
		native_setPreviewDisplay(surface);
    }

    // added for TIF
    public final void setPreviewDisplay2(Surface surface) throws IOException {
        Log.d(TAG, "hdmi rx setPreviewDisplay2(), surface:" + surface);
        native_setPreviewDisplay(surface);
    }

    public final void setPreviewDisplay3(SurfaceTexture surfaceTexture) throws IOException {
        Surface surface = new Surface(surfaceTexture);
        native_setPreviewDisplay(surface);
    }

    public HDMIRxParameters getParameters() {
        HDMIRxParameters p = new HDMIRxParameters();
        String s = new String();
        try {
            s = native_getParameters();
        }
        catch(RuntimeException e) {
            Log.e(TAG, "getParameters Error: "+e);
        }
        p.unflatten(s);
        return p;
    }

    public void setParameters(HDMIRxParameters param){
		Log.d(TAG, "hdmi rx setParameters()");
		int width = 720;
		int height = 480;
		int fps = 30;

		if (param != null) {
			Size size = param.getPreviewSize();
			width = size.width;
			height = size.height;
			fps = param.getPreviewFrameRate();
		}

		Log.d(TAG, "hdmi rx preview size:(" + width + "," + height + "), fps:" + fps);
		native_setParameters(width, height, fps);
    }

    public int play() {
		Log.d(TAG, "hdmi rx play()");
		int result = native_startPreview();
                setHDMIRxAudio();// info AudioPilicy to set volume
		return result;
    }

    public int stop() {
		Log.d(TAG, "hdmi rx stop()");
		muteHDMIRxAudio();// to mute AO, avoid "pop noise"
		int result = native_stopPreview();
		return result;
    }

    public byte[] getSnapshot(int format, int x, int y, int cropWidth, int cropHeight, int outWidth, int outHeight) {
        //Log.d(TAG, "hdmi rx getSnapshot fmt:" + format + "(" + x + "," + y + "), " + cropWidth + "x" + cropHeight + " => " + outWidth + "x" + outHeight);
        return native_getSnapshot(format, x, y, cropWidth, cropHeight, outWidth, outHeight);
    }

    public void setTranscode(boolean on) throws InvalidOpException {
        Log.d(TAG, "setTranscode on:"+on);

        //if(on && getAudioMode() == HDMIRX_AUDIO_RAW){
        //    Log.e(TAG, "enable record during RAW mode");
        //    throw new InvalidOpException();
        //}

        native_setTranscode(on);
    }

    public void configureTargetFormat(
            RtkHDMIRxManager.VideoConfig vConfig,
            RtkHDMIRxManager.AudioConfig aConfig){
        Log.d(TAG, "configureTargetFormat..");
        native_configureTargetFormat(vConfig.width, vConfig.height, 60,
                        vConfig.bitrate, 1, 1, 0, 0, 0,
                        aConfig.channelCount, 0,
                        aConfig.sampleRate, aConfig.bitrate);
    }

    public void setTargetFd(ParcelFileDescriptor fd, int fileFormat){
        Log.d(TAG, "setTargetFd fd:"+fd);
        native_setTargetFd(fd.getFd(), fileFormat);
    }

    public void setPlayback(boolean videoEn, boolean audioEn){
        native_setPlayback(videoEn,audioEn);
    }

    public int getAudioMode(){
        int rawOn = SystemProperties.getInt("persist.rtk.hdmirx.raw",0);
        if(rawOn == 1)
            return HDMIRX_AUDIO_RAW;
        else
            return HDMIRX_AUDIO_PCM;
    }

    public static void notifyHDMIRxAudioHotplug(int state) {
        native_notifyHDMIRxAudioHotplug(state);
    }

    //-------------------------------------------------------------------
    private native int native_open(String packageName);
	private native void native_release();
	private native void native_setPreviewDisplay(Surface surface);
	private native final String native_getParameters();
	private native void native_setParameters(int width, int height, int fps);
	private native int native_startPreview();
	private native int native_stopPreview();
    private native byte[] native_getSnapshot(int format, int x, int y, int cropWidth, int cropHeight, int outWidth, int outHeight);

    // rx record function
    private native void native_setTranscode(boolean on);
    private native void native_setTargetFd(int fd, int format);
    private native void native_setPlayback(boolean videoEn, boolean audioEn);
    private native void native_configureTargetFormat(
            // video setting
            int w, int h, int fps,
            int vbitrate, int iframeInterval,
            int rateControl, int aspectRatio,
            int interlace, int shareWBbuf,
            // audio setting
            int channelCount, int channelMode, int sampleRate, int abitrate);


    private static native void native_notifyHDMIRxAudioHotplug(int state);

    /**
     * Image size (width and height dimensions).
     */
    static public class Size {
	    /**
         * Sets the dimensions for pictures.
         *
         * @param w the photo width (pixels)
         * @param h the photo height (pixels)
         */
        public Size(int w, int h) {
           width = w;
            height = h;
        }
        /**
         * Compares {@code obj} to this size.
         *
         * @param obj the object to compare this size with.
         * @return {@code true} if the width and height of {@code obj} is the
         *		   same as those of this size. {@code false} otherwise.
         */
        @Override
        public boolean equals(Object obj) {
            if (!(obj instanceof Size)) {
                return false;
            }
            Size s = (Size) obj;
            return width == s.width && height == s.height;
        }
        @Override
        public int hashCode() {
            return width * 32713 + height;
        }
        /** width of the picture */
        public int width;
        /** height of the picture */
        public int height;
    }

    /**
     * A simple class to put all video config parameters
     */
    public static class VideoConfig{

        public int width;
        public int height;
        public int bitrate;

        public VideoConfig(
                int w,
                int h,
                int bitrate){

            this.width = w;
            this.height = h;
            this.bitrate = bitrate;
        }
    }

    /**
     * A simple class to put all audio config parameters
     */
    public static class AudioConfig{

        public int channelCount;
        public int sampleRate;
        public int bitrate;

        public AudioConfig(
                int channelCount,
                int sampleRate,
                int bitrate){
            this.channelCount = channelCount;
            this.sampleRate = sampleRate;
            this.bitrate = bitrate;
        }
    }
}
