package com.realtek.DLNA_DMP_1p5;

import java.lang.reflect.InvocationTargetException;

import android.util.Log;

import com.realtek.DLNA_DMP_1p5.UPnPObjInfo;
import com.realtek.DLNA_DMP_1p5.DLNACallback;;

public final class DLNA_DMP_1p5
{
	private static DLNACallback mCallback;
	
	public native static void DLNA_DMP_1p5_Init();
	public native static void DLNA_DMP_1p5_UnInit();
	public native static void DLNA_DMP_1p5_Start();
	public native static void DLNA_DMP_1p5_Terminate();
	public native static void DLNA_DMP_1p5_stopWatingResponse();
	public native static void DLNA_DMP_1p5_RegisterBrowserUpdateFunc();
	public native static void DLNA_DMP_1p5_UnregisterBrowserUpdateFunc();
	public native static boolean DLNA_DMP_1p5_UPnPServiceDeviceBrowse();
	public native static boolean DLNA_DMP_1p5_UPnPServiceBrowse(String id);
	public native static boolean DLNA_DMP_1p5_cdupDirectoryStack(int level);
	public native static void DLNA_DMP_1p5_unsetMediaServer();
	public native static boolean DLNA_DMP_1p5_setMediaServerByFriendlyName(String name);
	public native static boolean DLNA_DMP_1p5_setDirectoryByTitleName(String name);
	public native static int DLNA_DMP_1p5_MediaContainerObjectSizes();
	public native static int DLNA_DMP_1p5_MediaItemObjectSizes();
	public native static int DLNA_DMP_1p5_MediaItemVideoSizes();
	public native static int DLNA_DMP_1p5_MediaItemAudioSizes();
	public native static int DLNA_DMP_1p5_MediaItemImageSizes();
	public native static String DLNA_DMP_1p5_queryResourceByFile(String filename, int queryType);
	public native static String DLNA_DMP_1p5_queryResourceByID(String id, int queryType);
	public native static Object[] DLNA_DMP_1p5_querySubtitleURIListByID(String id, String mimeType);
	public native static int DLNA_DMP_1p5_MediaServerSizes();
	public native static String DLNA_DMP_1p5_MediaServerName(int index);
	public native static String DLNA_DMP_1p5_UPnPServiceContentDirectory(int index);
	public native static UPnPObjInfo DLNA_DMP_1p5_queryContainerObjInfoByIndex(int index);
	public native static String DLNA_DMP_1p5_UPnPServiceContentFile(int index);
	public native static UPnPObjInfo DLNA_DMP_1p5_queryItemObjInfoByIndex(int index);
	public native static int DLNA_DMP_1p5_GetMediaType(String filename);
	public native static int DLNA_DMP_1p5_BrowseTotalSizes();
	public native static int DLNA_DMP_1p5_UPnPServiceBrowse(String id, int startingIndex, int requestedCount);
	public native static boolean DLNA_DMP_1p5_UPnPServiceBrowseMetadata(String id);
	
	public static void DLNA_DMP_1p5_RegisterBrowserUpdateFunc(DLNACallback updateCallback)
	{
		mCallback = updateCallback;
		DLNA_DMP_1p5_RegisterBrowserUpdateFunc();
	}
	
	private void InvokeBrowserUpdateFunc()
	{
//		Log.d("+-+-+", "InvokeBrowserUpdateFunc be called i=");
//		if (mCallback != null)
//			try {
//				mCallback.invoke(i);
//			} catch (InvocationTargetException e) {
//				// TODO Auto-generated catch block
//				Log.e("Error: DLNA_DMP_1p5.InvokeBrowserUpdateFunc", ""+e);
//			} catch (IllegalAccessException e) {
//				// TODO Auto-generated catch block
//				Log.e("Error: DLNA_DMP_1p5.InvokeBrowserUpdateFunc", ""+e);
//			} catch (NoSuchMethodException e) {
//				// TODO Auto-generated catch block
//				Log.e("Error: DLNA_DMP_1p5.InvokeBrowserUpdateFunc", ""+e);
//			}
	}
	
	static
	{
		//System.loadLibrary("fileaccess");
		//System.loadLibrary("upnp3");
		System.loadLibrary("DLNADMPClass");
	}
}
