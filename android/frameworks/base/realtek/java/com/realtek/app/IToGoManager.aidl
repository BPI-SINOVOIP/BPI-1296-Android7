package com.realtek.app;

import android.os.ParcelFileDescriptor;
import com.realtek.app.togo.ToGoSourceStatus;
import com.realtek.app.togo.ToGoClientStatus;

interface IToGoManager {
	int init(int type, int cameraId);
	void uninit(int handle);
	void setPreview(int handle, boolean isPreview);
	int setPreviewSize(int handle, int width, int height);
	int setVideoFrameRate(int handle, int vfps);
	void setEnableH264Capture(int handle, boolean isEnableH264Capture);
	void setMuxerFormat(int handle, int muxerFormat);
	void setOutputType(int handle, int outputType);
	void setOutputFD(int handle, in ParcelFileDescriptor pfd);
	void setDuration(int handle, int duration);
	void setUdpIpPort(int handle, String ip, int port);
	void setVideoBitrate(int handle, int vbps);
	void setIFrameIntervalSeconds(int handle, int iFramesIntervalSeconds);
	void setAudioChannelCount(int handle, int audioChannelNum);
	void setAudioSampleRate(int handle, int audioSampleRate);
	void setAudioBitrate(int handle, int abps);
	//void setProfile(int handle, int profile);
	void configCompleted(int handle);
	int start(int handle);
	void stop(int handle);
	String getCamParams(int handle, int paramType);
	int isHandleLegal(int handle);
	ToGoSourceStatus getSourceStatus();
	ToGoClientStatus getClientStatus();
	int setTargetFilename(int handle, String filename);
	String getLCNList(int handle);
	int PlayChannel(int handle, int chIdx);
	int PlayNextChannel(int handle);
	int PlayPrevChannel(int handle);
	int getFileWidth(int handle);
	int getFileHeight(int handle);
	int startListen(int handle);
	void stopListen(int handle);
	int getHandleSourceType(int handle);
	void seek(int handle, int msec);
	int getDuration(int handle);

}
