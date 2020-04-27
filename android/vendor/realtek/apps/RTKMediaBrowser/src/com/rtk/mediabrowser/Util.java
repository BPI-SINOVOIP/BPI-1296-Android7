package com.rtk.mediabrowser;

import java.io.File;
import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.support.v4.content.FileProvider;
import org.xmlpull.v1.XmlPullParserException;

import android.content.res.XmlResourceParser;
import android.util.Log;

import com.realtek.Utils.MimeTypeParser;
import com.realtek.Utils.MimeTypes;

public class Util{

	private static final boolean DEBUG = true;
	private static final String TAG = "Util";
	/**
	 * 
	 * 
	 * @param time
	 * @return
	 */

	static MimeTypes mMimeTypes=null;
	public static String toTime(long time) {

		time /= 1000;
		long minute = time / 60;
		long second = time % 60;
		long hour = minute / 60;
		minute %= 60;
		return String.format("%02d:%02d:%02d",hour, minute, second);
	}

	private static Context appCtx;
	public static Context getAppCtx(){
		return appCtx;
	}

	public static void setAppCtx(Context ctx){
		appCtx = ctx;
	}

	public static Uri getUri(Context ctx, String path) {
		if(Build.VERSION.SDK_INT>=24) {
			if(path.startsWith("http://"))
				return Uri.parse(path);

			File f = null;
			if(!path.contains("://"))
				f = new File(path);
			if(path.startsWith("file://")) {
				String p = path.substring("file://".length());
				f = new File(p);
			}
//			log("getUri before path="+path);
			Uri uri = FileProvider.getUriForFile(ctx, ctx.getApplicationContext().getPackageName() + ".provider", f);
//			log("getUri after uri="+uri);
			return uri;
		} else {
			return Uri.parse(path);
		}
	}

	static void grantReadPermission(Context ctx, Intent intent, Uri uri) {
		if(intent==null) return;
		if(intent.getComponent()==null) return;
		String packageName = intent.getComponent().getPackageName();
		Log.d(TAG, "grantReadPermission packageName="+packageName);
		ctx.grantUriPermission(packageName, uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);
	}

	private static void log(String s) {
		if(DEBUG)
			Log.d(TAG, s);
	}


	public static  MimeTypes GetMimeTypes(XmlResourceParser mimeTypeXml) 
	{
		if(mMimeTypes != null)
			return mMimeTypes;

		MimeTypeParser mtp = new MimeTypeParser();

		try  
		{
			mMimeTypes = mtp.fromXmlResource(mimeTypeXml);   		 	
		} 
		catch (XmlPullParserException e)
		{
			Log.d("Utils.GetMimeTypes","PreselectedChannelsActivity: XmlPullParserException",e);
			throw new RuntimeException("PreselectedChannelsActivity: XmlPullParserException");
		} 
		catch (IOException e) 
		{
			Log.d("Utils.GetMimeTypes", "PreselectedChannelsActivity: IOException", e);
			throw new RuntimeException("PreselectedChannelsActivity: IOException");
		}

		return mMimeTypes;
	}

	public static void DestoryMimeTypes ()
	{
		mMimeTypes = null;
	}

	public static boolean hasNetwork(Context ctx) {
		boolean rlt = false;
		ConnectivityManager connManager = (ConnectivityManager)ctx.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo info = connManager.getActiveNetworkInfo();
		if(info==null) {
			Log.d(TAG," Can not get ActiveNetworkInfo");
		} else {
			if(info.isConnected()) {
				Log.d(TAG, "Network is connected Type:" + info.getType());
				rlt = true;
			} else {
				Log.d(TAG, "Network is disconnected Type:" + info.getType());
			}
		}
		return rlt;
	}

	/**
	 * Get IP address from first non-localhost interface
	 * @param ipv4  true=return ipv4, false=return ipv6
	 * @return  address or empty string
	 */
	public static String getIPAddress(boolean useIPv4) {
		try {
			List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces) {
				List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
				for (InetAddress addr : addrs) {
					if (!addr.isLoopbackAddress()) {
						String sAddr = addr.getHostAddress();
						//boolean isIPv4 = InetAddressUtils.isIPv4Address(sAddr);
						boolean isIPv4 = sAddr.indexOf(':')<0;

						if (useIPv4) {
							if (isIPv4)
								return sAddr;
						} else {
							if (!isIPv4) {
								int delim = sAddr.indexOf('%'); // drop ip6 zone suffix
								return delim<0 ? sAddr.toUpperCase() : sAddr.substring(0, delim).toUpperCase();
							}
						}
					}
				}
			}
		} catch (Exception ex) { } // for now eat exceptions
		return "";
	}
}
