/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.25 : First draft, because enable wifi takes long time to complete, add prepare page to notify end user.
 */

package com.realtek.realtekinitialsettings;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActionBar;
import android.app.ActivityManager;
import android.app.Dialog;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemProperties;
import android.view.MotionEvent;
import android.view.View;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.LayoutInflater;
import android.view.View.OnClickListener;
import android.view.Gravity;
import android.view.Window;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Toast;
import android.graphics.drawable.AnimationDrawable;
import android.net.wifi.WifiManager;
import android.util.Log;
import java.util.List;
import java.util.ArrayList;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

/**
 *   Realtek InitialPrepareWirelessSettingsActivity is used to show the initial setting functionalities.
 */
public class InitialPrepareWirelessSettingsActivity extends Activity {
	public static String TAG = "InitialPrepareWirelessSettingsActivity";
	private final Handler handler = new Handler();  
	private ImageView mLoadingView;
	private AnimationDrawable mLoadingAnimation;
	private Timer mCheckingTimer;
	
	/**
	 *  Override this function
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
		final Activity cbt = this;
		
        super.onCreate(savedInstanceState);

		// Remove app title 
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		// Remove status bar and navigation bar

		// Below method only funcion with Android 4.0 and lower.
		//getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		// Below method only funcion with Android 4.1 and higher.
		View decorView = getWindow().getDecorView();
		int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
		decorView.setSystemUiVisibility(uiOptions);
		//ActionBar actionBar = getActionBar();
		//actionBar.hide();

        setContentView(R.layout.activity_initial_prepare_wireless_settings);

		// find image
		mLoadingView = (ImageView) findViewById(R.id.loading_icon);
		mLoadingView.setBackgroundResource(R.drawable.animation_prepare_wireless);

		// set animation
		mLoadingAnimation = (AnimationDrawable) mLoadingView.getBackground();
		mLoadingAnimation.start();

		// set timer
		mCheckingTimer = new Timer(true);
		setTimerTask();
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onResume() {
        super.onResume();
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onPause() {
        super.onPause();
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onDestroy() {
		super.onDestroy();
		mCheckingTimer.cancel();
	}

	/**
	 *  Override this function in order to move the focus between listview and imageview.
	*/
/*
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		
		if(event.getAction() == KeyEvent.ACTION_DOWN) {
			switch(event.getKeyCode()) {
				case KeyEvent.KEYCODE_DPAD_UP:
				break;
				case KeyEvent.KEYCODE_DPAD_DOWN:
				break;
			}
		}
	
		return super.dispatchKeyEvent(event);
	}
*/

	/**
	 *  Override this function in order to move the focus between listview and imageview.
	*/
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		switch(keyCode) {
			case KeyEvent.KEYCODE_DPAD_LEFT:
            	Log.d(TAG,"onKeyDown: LEFT");
				return true;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
            	Log.d(TAG,"onKeyDown: RIGHT");
				return true;
			case KeyEvent.KEYCODE_DPAD_UP:
            	Log.d(TAG,"onKeyDown: UP");
				break;
			case KeyEvent.KEYCODE_DPAD_DOWN:
            	Log.d(TAG,"onKeyDown: DOWN");
				return true;
			default:
				break;
		}		

		return super.onKeyDown(keyCode, event);
	}

    /**
     *  Helper function to go to Home page - Launcher app
    */
    private void goToHomeApp() {
		mCheckingTimer.cancel();

        // write to persist.sys.RunRealtekInitialSettings so that do not need to launch this app again.
        SystemProperties.set("persist.sys.RunRTKIniSettings", "true");
        String propRes = SystemProperties.get("persist.sys.RunRTKIniSettings");
        Log.d("stanely", "gotToHomeApp(), read persist.sys.RunRTKIniSettings = " + propRes);

		// exit the RealtekInitialSettings app and go to Home.
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		startActivity(intent);
		finish();
	}	

    /**
     *  Helper function to go to next page - Wireless Settings
    */
    private void goToWirelessSettings() {
		// Launch next activity, second page.
		Intent intent = new Intent(this, InitialWirelessSettingsActivity.class);
		startActivity(intent);
    }

	/**
	 *  Set timer task.
	*/
	private void setTimerTask() {
		mCheckingTimer.schedule(new TimerTask() {
			@Override
			public void run() {
				Message msg = new Message();
				msg.what = 1;
				doCheckingActionHandler.sendMessage(msg);
			}
		}, 100, 200);  // start after 100ms, checking every 200ms.
	}

	/**
	 *  Create handler for checking
	*/
	private Handler doCheckingActionHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			int msgId = msg.what;
			switch(msgId) {
				case 1:
					WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
					if(wifiManager.isWifiEnabled() == true) {
						mLoadingAnimation.stop();
						goToWirelessSettings();					
						finish();
					} else {  // since the wifi is not available at this moment, we have to exit the app.
						goToHomeApp();
					}
					break;
				default:
					break;
			}

		}
	};


}
