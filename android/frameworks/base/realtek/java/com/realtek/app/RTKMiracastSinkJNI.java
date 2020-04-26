package com.realtek.app;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.MediaScannerConnection;
import android.media.AudioManager;
import android.net.Uri;
import android.provider.MediaStore;
import android.os.Bundle;
import android.os.RemoteException;
import android.os.Handler;
import android.os.Message;
import android.os.UserHandle;
import android.util.Log;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningTaskInfo;
import android.view.Surface;
import android.view.SurfaceHolder;
//import com.realtek.app.IAirFunService;
import java.util.List;
//import com.realtek.hardware.IRTKMiracastSinkJNI;

public class RTKMiracastSinkJNI { //extends IRTKMiracastSinkJNI.Stub {
	private static final String TAG = "RTKMiracastSinkJNI";
	static {
		Log.i(TAG, "load so.");
		System.loadLibrary("realtek_jni");
	}
	// Define the play back status
	

    private static final String JNI_MESSAGE_KEY = "JniMsgKey";
    private static final int JNI_MESSAGE = 1;
    
	
	private IntentFilter pbFilter = null;
	private Context mContext;
	

	public RTKMiracastSinkJNI(Context context) {
		//Log.i(TAG, "create");
		mContext = context;
		//Log.i(TAG, "create1");
	}	
	
	// Native methods
	private native int jni_wfd_sink_Open(String packagename);
	private native void jni_wfd_sink_Close();
	private native void jni_wfd_sink_SetDisplay(Surface surface);
	private native void jni_wfd_sink_SetParameters(int left,int top,int width,int height,int show);
	private native int jni_wfd_sink_GetReady();
	private native void jni_wfd_sink_SetUIBC(int on);
	private native int jni_wfd_sink_Start(String ip,int port);
	private native int jni_wfd_sink_Stop();
	
	public int wfd_sink_Open(String packagename)
	{
		//Log.i(TAG, "jni_wfd_sink_Open");
		return jni_wfd_sink_Open(packagename);
	}
	
	public void wfd_sink_Close()
	{
		//Log.i(TAG, "jni_wfd_sink_Close");
		jni_wfd_sink_Close();
	}
	
	public void wfd_sink_SetDisplay(SurfaceHolder holder)
	{
		//Log.d(TAG, "jni_wfd_sink_SetDisplay, holder:" + holder);
		Surface surface = (holder == null)? null : holder.getSurface();
		jni_wfd_sink_SetDisplay(surface);
	}
	
	public void wfd_sink_SetParameters(int left,int top,int width,int height,int show)
	{
		//Log.i(TAG, "jni_wfd_sink_SetParameters");
		jni_wfd_sink_SetParameters(left,top,width,height,show);
	}
	
	public int wfd_sink_GetReady()
	{
		//Log.i(TAG, "jni_wfd_sink_GetStatus");
		return jni_wfd_sink_GetReady();
	}
	
	public void wfd_sink_SetUIBC(int on)
	{
		//Log.i(TAG, "jni_wfd_sink_SetUIBC");
		jni_wfd_sink_SetUIBC(on);
	}
	
	public int wfd_sink_Start(String ip,int port)
	{
		//Log.i(TAG, "jni_wfd_sink_Start, ip: "+ip+" port: "+port);
		return jni_wfd_sink_Start(ip,port);
	}
	
	public int wfd_sink_Stop()
	{
		//Log.i(TAG, "jni_wfd_sink_Stop");
		return jni_wfd_sink_Stop();
	}
	
}
