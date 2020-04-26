package com.realtek.server;

import android.util.Log;
import android.content.Context;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.IBinder;
import android.content.Context;
import android.content.Intent;
import com.realtek.app.togo.IToGoProxyService;

public class ToGoProxyService extends IToGoProxyService.Stub {
	private final static String TAG = "ToGoProxyService";
	private final Context mContext;
	private ToGoConnection mConn;
	private IBinder mToGoSerivce = null;

	static {
		System.loadLibrary("realtek_runtime");
	}

	class ToGoConnection implements ServiceConnection {
		@Override
		public void onServiceConnected(ComponentName arg0, IBinder arg1) {
			// TODO Auto-generated method stub
			mToGoSerivce = arg1;
			if (mToGoSerivce == null) {
				Log.e(TAG, "onServiceConnected.mToGoSerivce==null.");
			}
			else {
				Log.v(TAG, "onServiceConnected.mToGoSerivce=="+mToGoSerivce);
			}
		}

		@Override
		public void onServiceDisconnected(ComponentName arg0) {
			// TODO Auto-generated method stub
			Log.v(TAG, "disconnected.");
		}		
	}

	public ToGoProxyService(Context context) {
		mContext = context;
		Log.v(TAG, "enter.ToGoProxyService.constructor.");
	}

	public synchronized int bindToGoAppService() {
		if (mToGoSerivce == null) {
		mConn = new ToGoConnection();
		Intent intent = new Intent();
		intent.setClassName("com.realtek.app.togo", "com.realtek.app.togo.ToGoAppService");
		if (mContext.bindService(intent, mConn, Context.BIND_AUTO_CREATE) == false) {
			Log.e(TAG, "bindService() NG.");
			return -1;
		}
		}
		else {
			Log.v(TAG, "mToGoSerivce != null.No need to bindService.");
		}
		return 0;
	}

	public IBinder getToGoAppService() {
		//Log.v(TAG, "getToGoAppService.");
		return mToGoSerivce;		
	}

	public synchronized IBinder getGraphicBufferProducer(int handle, int type, int width, int height) {
		Log.v(TAG, "getGraphicBufferProducer.handle="+handle+".type="+type+".width="+width+".height="+height);
		return jniGetGraphicBufferProducer(handle, type, width, height);
	}

	public synchronized void clearGraphicBufferProducer(int handle) {
		Log.v(TAG, "clearGraphicBufferProducer.handle="+handle);
		jniClearGraphicBufferProducer(handle);
	}

	public native IBinder jniGetGraphicBufferProducer(int handle, int type, int width, int height);
	public native void jniClearGraphicBufferProducer(int handle);
}
