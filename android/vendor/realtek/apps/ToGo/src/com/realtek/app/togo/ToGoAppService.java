package com.realtek.app.togo;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import com.realtek.server.ToGoService;
import com.realtek.server.ToGoProxyService;

public class ToGoAppService extends Service {
	private final static String TAG = "ToGoAppService";
	ToGoService mToGoService;
	ToGoProxyService mToGoProxyService;

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		Log.v(TAG, "enter.onBind.");
		mToGoService = new ToGoService(this);
		if (mToGoService == null) {
			Log.e(TAG, "mToGoService==null.");
		}
		else {
			Log.v(TAG, "mToGoService=="+mToGoService);
		}
		return mToGoService;
	}

	@Override
	public void onStart(Intent intent, int startId) {
		// TODO Auto-generated method stub
		Log.v(TAG, "enter.onStart.");
		super.onStart(intent, startId);

		mToGoProxyService = (ToGoProxyService)getSystemService("ToGoProxy");
		if (mToGoProxyService == null) {
			Log.e(TAG, "mToGoProxyService==null.");
		}
		else {
			Log.v(TAG, "mToGoProxyService=="+mToGoProxyService);
		}
	}
	
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		Log.v(TAG, "enter.onCreate.");
		super.onCreate();
	}

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		Log.v(TAG, "enter.onDestroy.");
		super.onDestroy();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		// TODO Auto-generated method stub
		Log.v(TAG, "enter.onStartCommand.");
		return super.onStartCommand(intent, flags, startId);
	}

}