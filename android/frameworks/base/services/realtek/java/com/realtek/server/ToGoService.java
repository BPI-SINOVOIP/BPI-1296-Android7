package com.realtek.server;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileDescriptor;
import java.io.IOException;
import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import android.util.Log;
import android.os.ParcelFileDescriptor;
import android.os.Bundle;
import android.hardware.Camera;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.app.IToGoManager;
import com.realtek.app.togo.ToGoSourceStatus;
import com.realtek.app.togo.ToGoClientStatus;
import com.realtek.hardware.HDMIRxStatus;

/**
 * Ref to IToGoManager.aidl and com_realtek_server_ToGoService.cpp
 */
public class ToGoService extends IToGoManager.Stub {
	static {
		System.loadLibrary("realtek_runtime");
	}

	private final static String TAG = "ToGoService.java";
	private RtkHDMIRxManager hdmiRxManager;
	private IntentFilter hdmiRxFilter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED");
	private int hdmiRxHandle = -1;	// save the HDMIRx handle for later HDMIRx plug notify
	
	private final Context mContext;
	private ToGoClientStatus mClientStatus;

	private BroadcastReceiver hdmiRxHotPlugReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			boolean hdmiRxPlugged = intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false);
			if(hdmiRxPlugged) {
				Log.v(TAG, "hdmiRxPlugged is plugged in");
			} else {
				Log.v(TAG, "hdmiRxPlugged is pulled out");
				if(hdmiRxHandle != -1) {
					stop(hdmiRxHandle);
					uninit(hdmiRxHandle);
				}
			}
		}
	};
	

	public ToGoService(Context context) {
		Log.v(TAG, "enter.ToGoService.constructor.");
		mContext = context;
		// register the hdmiRxFilter
		hdmiRxManager = new RtkHDMIRxManager();
		mContext.registerReceiver(hdmiRxHotPlugReceiver, hdmiRxFilter);
	}

	protected void finalize() throws Throwable {
		try {
		} finally {
			super.finalize();
		}
	}

	

	//
	private void notifyClientStatus() {
		if (mClientStatus == null) {
			mClientStatus = new ToGoClientStatus();
		}
		mClientStatus.mCount = jniGetHandleCount();
		for (int i=0;i<mClientStatus.mCount;i++) {
			mClientStatus.mClientId[i] = i;
			mClientStatus.mSourceType[i] = jniGetHandleSourceType(i);
			mClientStatus.mState[i] = jniGetHandleState(i);
		}

		Intent intent = new Intent("com.realtek.app.togo.action.TOGO_NOTIFY");
		Bundle bundle = new Bundle();
		bundle.putParcelable("TOGO_CLIENT_STATUS", mClientStatus);
		intent.putExtras(bundle);
		mContext.sendBroadcast(intent);
		Log.v(TAG, "sendBroadcast.TOGO_NOTIFY.");
	}
	//

	public synchronized int init(int type, int cameraId) {
		Log.v(TAG, "enter.init.type=" + type + ".cameraId=" + cameraId);
		int handle = jniInit(type, cameraId);

		if(type == ToGoSourceStatus.ID_OF_HDMI_RX) {
			hdmiRxHandle = handle;
		}

		notifyClientStatus();
		return handle;
	}

	public synchronized void uninit(int handle) {
		Log.v(TAG, "enter.uninit.");

		if(handle == hdmiRxHandle) {
			hdmiRxHandle = -1;
		}

		jniUnInit(handle);
		notifyClientStatus();
	}

	public void setPreview(int handle, boolean isPreview) {
		Log.v(TAG, "enter.setPreview.handle=" + handle + ".isPreview=" + isPreview);
		jniSetPreview(handle, isPreview);
	}

	public int setPreviewSize(int handle, int width, int height) {
		Log.v(TAG, "enter.setVideoWidthHeight.handle=" + handle + ".width=" + width + ".height=" + height);
		return jniSetPreviewSize(handle, width, height);
	}

	public int setVideoFrameRate(int handle, int vfps) {
		Log.v(TAG, "enter.setVideoFrameRate.handle=" + handle + ".vfps=" + vfps);
		return jniSetVideoFrameRate(handle, vfps);
	}

	public void setEnableH264Capture(int handle, boolean isEnableH264Capture) {
		Log.v(TAG, "enter.setEnableH264Capture.handle=" + handle + ".isEnableH264Capture=" + isEnableH264Capture);
		jniSetEnableH264Capture(handle, isEnableH264Capture);
	}

	public void setMuxerFormat(int handle, int muxerFormat) {
		Log.v(TAG, "enter.setMuxerFormat.handle=" + handle + ".muxerFormat=" + muxerFormat);
		jniSetMuxerFormat(handle, muxerFormat);
	}

	public void setOutputType(int handle, int outputType) {
		Log.v(TAG, "enter.setOutputType.handle=" + handle + ".outputType=" + outputType);
		jniSetOutputType(handle, outputType);
	}

	public void setOutputFD(int handle, ParcelFileDescriptor pfd) {
		Log.v(TAG, "enter.setOutputFD.handle=" + handle);
		jniSetOutputFD(handle, pfd.getFileDescriptor());
	}

	public void setDuration(int handle, int duration) {
		Log.v(TAG, "enter.setDuration.handle=" + handle + ".duration=" + duration);
		jniSetDuration(handle, duration);
	}

	public void setUdpIpPort(int handle, String ip, int port) {
		Log.v(TAG, "enter.setUdpIpPort.handle=" + handle + ".ip=" + ip + ".port=" + port);
		jniSetUdpIpPort(handle, ip, port);
	}

	public void setVideoBitrate(int handle, int vbps) {
		Log.v(TAG, "enter.setVideoBitrate.handle=" + handle + ".vbps=" + vbps);
		jniSetVideoBitrate(handle, vbps);
	}

	public void setIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds) {
		Log.v(TAG, "enter.setIFrameIntervalSeconds.handle=" + handle + ".iFramesIntervalSeconds=" + iFramesIntervalSeconds);
		jniSetIFrameIntervalSeconds(handle, iFramesIntervalSeconds);
	}

	public void setAudioChannelCount(int handle, int audioChannelNum) {
		Log.v(TAG, "enter.setAudioChannelCount.handle=" + handle + ".audioChannelNum=" + audioChannelNum);
		jniSetAudioChannelCount(handle, audioChannelNum);
	}

	public void setAudioSampleRate(int handle, int audioSampleRate) {
		Log.v(TAG, "enter.setAudioSampleRate.handle=" + handle + ".audioSampleRate=" + audioSampleRate);
		jniSetAudioSampleRate(handle, audioSampleRate);
	}

	public void setAudioBitrate(int handle, int abps) {
		Log.v(TAG, "enter.setAudioBitrate.handle=" + handle + ".abps=" + abps);
		jniSetAudioBitrate(handle, abps);
	}

	//public void setProfile(int handle, int profile) {
	//	Log.v(TAG, "enter.setProfile.handle=" + handle + ".profile=" + profile);
	//	jniSetProfile(handle, profile);
	//}

	public synchronized void configCompleted(int handle) {
		Log.v(TAG, "enter.configCompleted.handle=" + handle);
		jniConfigCompleted(handle);
	}

	public synchronized int start(int handle) {
		Log.v(TAG, "enter.start.handle=" + handle);
		int ret = jniStart(handle);
		notifyClientStatus();
		return ret;
	}

	public synchronized void stop(int handle) {
		Log.v(TAG, "enter.stop.handle=" + handle);
		jniStop(handle);
		notifyClientStatus();
	}

	public String getCamParams(int handle, int paramType) {
		Log.v(TAG, "enter.getCamParams.handle=" + handle + ".paramType=" + paramType);
		return jniGetCamParams(handle, paramType);
	}

	public int isHandleLegal(int handle) {
		Log.v(TAG, "enter.isHandleLegal.handle=" + handle);
		return jniIsHandleLegal(handle);
	}

	public ToGoSourceStatus getSourceStatus() {
		Log.v(TAG, "enter.getSourceStatus");
//		return jniGetSourceStatus();
		ToGoSourceStatus status = jniGetSourceStatus();

		// Check HDMIRx status
		if(!hdmiRxManager.isHDMIRxPlugged())
			status.mIsSourceAvailable[ToGoSourceStatus.ID_OF_HDMI_RX] = 0;

		// Check USB Camera status
		int numOfCam = Camera.getNumberOfCameras();
		Log.v(TAG, "getNumberOfCameras=" + numOfCam);
		if(numOfCam > 0) {
			// 0: USB camera, 1: HDMIRx
			int testCamId = 0;
			try{
				Camera cam = Camera.open(testCamId);
				if(cam == null) {
					Log.v(TAG, "Camera(0) open fail");
				} else {
					Log.v(TAG, "Camera(0) open success");
					cam.release();
					status.mIsSourceAvailable[ToGoSourceStatus.ID_OF_CAMERA] = 1;
				}
			}catch(Exception ex){
				Log.v(TAG, "Camera(0) connect exception");
			}
		}
		
		// TODO: Check Video and DTV availability
//		status.mIsSourceAvailable[ToGoSourceStatus.ID_OF_FILE] = 1;
//		status.mIsSourceAvailable[ToGoSourceStatus.ID_OF_DTV] = 1;

		for(int i=0; i<status.mCount; i++) {
			Log.v(TAG, "[" + status.mSourceId[i] + "] " + status.mSourceName[i] +  " = " + status.mIsSourceAvailable[i]);
		}
		return status;
	}


	//
	public ToGoClientStatus getClientStatus() {
		if (mClientStatus == null) {
			mClientStatus = new ToGoClientStatus();
		}
		mClientStatus.mClientMax = jniGetHandleMax();
		mClientStatus.mCount = jniGetHandleCount();
		Log.v(TAG, "Session Count=" + mClientStatus.mCount);
		for (int i=0;i<mClientStatus.mCount;i++) {
			mClientStatus.mClientId[i] = i;
			mClientStatus.mSourceType[i] = jniGetHandleSourceType(i);
			mClientStatus.mState[i] = jniGetHandleState(i);
			Log.v(TAG, "Session[" + i + "] type=" + mClientStatus.mSourceType[i] + ", state=" + mClientStatus.mState[i]);
		}

		return mClientStatus;
	}


	public int setTargetFilename(int handle, String filename) {
		Log.v(TAG, "enter.handle="+handle+".filename="+filename);
		return jniSetTargetFilename(handle, filename);
	}

	public String getLCNList(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniGetLCNList(handle);
	}

	public int PlayChannel(int handle, int chIdx) {
		Log.v(TAG, "enter.handle="+handle+".chIdx="+chIdx);
		return jniPlayChannel(handle, chIdx);
	}

	public int PlayNextChannel(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniPlayNextChannel(handle);
	}

	public int PlayPrevChannel(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniPlayPrevChannel(handle);
	}

	public int getFileWidth(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniGetFileWidth(handle);
	}

	public int getFileHeight(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniGetFileHeight(handle);
	}

	public int startListen(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniStartListen(handle);
	}

	public void stopListen(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		jniStopListen(handle);
	}

	public int getHandleSourceType(int handle) {
		Log.v(TAG, "enter.handle="+handle);
		return jniGetHandleSourceType(handle);
	}

	public void seek(int handle, int msec) {
		Log.v(TAG, "seek.handle="+handle);
		jniSeek(handle, msec);	
	}

	public int getDuration(int handle) {
		Log.v(TAG, "getDuration.handle="+handle);
		return jniGetDuration(handle);	
	}

	// JNI functions
	public native int jniInit(int type, int cameraId);
	public native void jniUnInit(int handle);
	public native void jniSetPreview(int handle, boolean isPreview);
	public native int jniSetPreviewSize(int handle, int width, int height);
	public native int jniSetVideoFrameRate(int handle, int vfps);
	public native void jniSetEnableH264Capture(int handle, boolean isEnableH264Capture);
	public native void jniSetMuxerFormat(int handle, int muxerFormat);
	public native void jniSetOutputType(int handle, int outputType);
	public native void jniSetOutputFD(int handle, FileDescriptor fd);
	public native void jniSetDuration(int handle, int duration);
	public native void jniSetUdpIpPort(int handle, String ip, int port);
	public native void jniSetVideoBitrate(int handle, int vbps);	
	public native void jniSetIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds);
	public native void jniSetAudioChannelCount(int handle, int audioChannelNum);
	public native void jniSetAudioSampleRate(int handle, int audioSampleRate);
	public native void jniSetAudioBitrate(int handle, int abps);
	//public native void jniSetProfile(int handle, int profile);
	public native void jniConfigCompleted(int handle);
	public native int jniStart(int handle);
	public native void jniStop(int handle);
	public native String jniGetCamParams(int handle, int paramType);
	public native int jniIsHandleLegal(int handle);
	public native ToGoSourceStatus jniGetSourceStatus();
	public native int jniSetTargetFilename(int handle, String filename);
	public native int jniGetHandleMax();
	public native int jniGetHandleCount();
	public native int jniGetHandleSourceType(int handle);
	public native int jniGetHandleState(int handle);
	public native String jniGetLCNList(int handle);
	public native int jniPlayChannel(int handle, int chIdx);
	public native int jniPlayNextChannel(int handle);
	public native int jniPlayPrevChannel(int handle);
	public native int jniGetFileWidth(int handle);
	public native int jniGetFileHeight(int handle);
	public native int jniStartListen(int handle);
	public native void jniStopListen(int handle);
	public native void jniSeek(int handle, int msec);
	public native int jniGetDuration(int handle);
}


