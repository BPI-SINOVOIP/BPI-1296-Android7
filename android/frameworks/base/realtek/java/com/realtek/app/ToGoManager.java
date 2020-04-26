package com.realtek.app;

import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.os.ParcelFileDescriptor;
import com.realtek.app.togo.ToGoSourceStatus;
import com.realtek.app.togo.ToGoClientStatus;

public class ToGoManager {
	private final static String TAG = "ToGoManager";
	private final IToGoManager mService;

	// These values must sync with it in RtkToGo.h
	public final static int TOGO_TYPE_CAMERA = 0;
	public final static int TOGO_TYPE_HDMI_RX = 1;
	public final static int TOGO_TYPE_FILE = 2;
	public final static int TOGO_TYPE_DTV = 3;

	public final static int MUXER_TS = 0;
	public final static int MUXER_MP4 = 1;
	public final static int MUXER_AAC = 2;

	public final static int OUTPUT_FILE = 0;
	public final static int OUTPUT_STDOUT = 1;
	public final static int OUTPUT_ASHMEM = 2;
	public final static int OUTPUT_ASHMEM_RTP = 3;
	public final static int OUTPUT_UDP = 4;
	public final static int OUTPUT_UDP_RTP = 5;
	public final static int OUTPUT_RTP_BOTH = 6;

	public ToGoManager() {
		mService = IToGoManager.Stub.asInterface(ServiceManager.getService("togo"));
		if (mService != null) {
			Log.v(TAG, mService.toString());
		}
		else {
			Log.e(TAG, "Get ToGoService NG!");
		}
	}

	public int init(int type, int cameraId) {
		int ret = -1;
		try {
			ret = mService.init(type, cameraId);
		} catch (RemoteException ex) {
		}		
		return ret;
	}

	public void uninit(int handle) {
		try {
			mService.uninit(handle);
		} catch (RemoteException ex) {
		}
	}

	public void setPreview(int handle, boolean isPreview) {
		try {
			mService.setPreview(handle, isPreview);
		} catch (RemoteException ex) {
		}
	}

	public int setPreviewSize(int handle, int width, int height) {
		int ret = -1;
		try {
			ret = mService.setPreviewSize(handle, width, height);
		} catch (RemoteException ex) {
		}
		return ret;
	}

	public int setVideoFrameRate(int handle, int vfps) {
		int ret = -1;
		try {
			ret = mService.setVideoFrameRate(handle, vfps);
		} catch (RemoteException ex) {
		}
		return ret;
	}

	public void setEnableH264Capture(int handle, boolean isEnableH264Capture) {
		try {
			mService.setEnableH264Capture(handle, isEnableH264Capture);
		} catch (RemoteException ex) {
		}
	}

	public void setMuxerFormat(int handle, int muxerFormat) {
		try {
			mService.setMuxerFormat(handle, muxerFormat);
		} catch (RemoteException ex) {
		}
	}

	public void setOutputType(int handle, int outputType) {
		try {
			mService.setOutputType(handle, outputType);
		} catch (RemoteException ex) {
		}
	}

	public void setOutputFD(int handle, ParcelFileDescriptor pfd) {
		try {
			mService.setOutputFD(handle, pfd);
		} catch (RemoteException ex) {
		}
	}

	public void setDuration(int handle, int duration) {
		try {
			mService.setDuration(handle, duration);
		} catch (RemoteException ex) {
		}
	}

	public void setUdpIpPort(int handle, String ip, int port) {
		try {
			mService.setUdpIpPort(handle, ip, port);
		} catch (RemoteException ex) {
		}
	}

	public void setVideoBitrate(int handle, int vbps) {
		try {
			mService.setVideoBitrate(handle, vbps);
		} catch (RemoteException ex) {
		}
	}

	public void setIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds) {
		try {
			mService.setIFrameIntervalSeconds(handle, iFramesIntervalSeconds);
		} catch (RemoteException ex) {
		}
	}

	public void setAudioChannelCount(int handle, int audioChannelNum) {
		try {
			mService.setAudioChannelCount(handle, audioChannelNum);
		} catch (RemoteException ex) {
		}
	}

	public void setAudioSampleRate(int handle, int audioSampleRate) {
		try {
			mService.setAudioSampleRate(handle, audioSampleRate);
		} catch (RemoteException ex) {
		}
	}

	public void setAudioBitrate(int handle, int abps) {
		try {
			mService.setAudioBitrate(handle, abps);
		} catch (RemoteException ex) {
		}
	}

	//public void setProfile(int handle, int profile) {
	//	try {
	//		mService.setProfile(handle, profile);
	//	} catch (RemoteException ex) {
	//	}
	//}

	public void configCompleted(int handle) {
		try {
			mService.configCompleted(handle);
		} catch (RemoteException ex) {
		}
	}

	public int start(int handle) {
		int ret = -1;
		try {
			ret = mService.start(handle);
		} catch (RemoteException ex) {
		}
		return ret;
	}

	public void stop(int handle) {
		try {
			mService.stop(handle);
		} catch (RemoteException ex) {
		}
	}

	public String getCamParams(int handle, int paramType) {
		String ret = null;
		try {
			ret = mService.getCamParams(handle, paramType);
		} catch (RemoteException ex) {
		}
		return ret;
	}

	public int isHandleLegal(int handle) {
		int ret = -1;
		try {
			ret = mService.isHandleLegal(handle);
		} catch (RemoteException ex) {
		}
		return ret;
	}

	public ToGoSourceStatus getSourceStatus() {
		ToGoSourceStatus ret = null;
		try {
			ret = mService.getSourceStatus();
		} catch (RemoteException ex) {
		}
		return ret;
	}
	
	public ToGoClientStatus getClientStatus() {
		ToGoClientStatus ret = null;
		try {
			ret = mService.getClientStatus();
		} catch (RemoteException ex) {
		}
		return ret;
	}
}

