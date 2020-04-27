/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.RTKMiracastSink;

import java.util.Date;
import java.util.Locale;
import java.util.List;  

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pWfdInfo;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.WifiP2pManager.ActionListener;
import android.net.wifi.p2p.WifiP2pManager.GroupInfoListener;
import android.net.wifi.p2p.WifiP2pManager.Channel;
import android.net.wifi.p2p.WifiP2pManager.ChannelListener;
import android.net.wifi.p2p.WifiP2pManager.PeerListListener;
//import android.net.wifi.p2p.WifiP2pManager.DialogListener;
/*added by kavin*/
import android.net.wifi.p2p.WifiP2pManager.PersistentGroupInfoListener;
import android.net.wifi.p2p.WifiP2pGroupList;

import android.widget.TextView.OnEditorActionListener;


import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.view.KeyEvent;
import android.provider.Settings;
import android.app.ActivityManager;



//import android.app.TvManager;
//import com.rtk.android.miracast.WFDDeviceList.DeviceActionListener;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.CountDownTimer;
import android.os.SystemProperties;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.DataOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;


public class MainActivity extends Activity
{

    public static final String TAG = "RTKMiracastSink";
    private TextView mState;
	private TextView mDeviceName;
	private TextView mPinCode;
	private static final int MENU_ID_MIRACAST = Menu.FIRST;
    private static final int MENU_ID_SETTING = Menu.FIRST + 1;    
	protected static final int GUIUPDATEID = 0x101; 
	private boolean mSoftAPEnable = false;
	private BroadcastReceiver myReceiver = null;
		
	private void findViews()
    {
    	mState = (TextView) findViewById(R.id.STATE);
    	mDeviceName = (TextView) findViewById(R.id.DEVICE_NAME);
    	mPinCode = (TextView) findViewById(R.id.PIN_CODE);
    }
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");        
	
		setContentView(R.layout.activity_main);
		findViews();

         /****         
		// Force turn on WFD in P2P only mode?
		Log.i( TAG, "MainActivity try to turn on WFD mode");
		Intent i = new Intent("rtk.provider.miracastsink.STATUS");
		i.putExtra("WFDMode", "WFD_FORCE_ON");
		sendBroadcast(i);
		*****/	
	}
	
	@Override
    public void onResume() {
        super.onResume();
        Log.i(TAG,"onResume()");
        
		WifiManager mWifiManager = (WifiManager)getSystemService(Context.WIFI_SERVICE);
		if(mWifiManager.getWifiState() != WifiManager.WIFI_STATE_ENABLED) {
			if(mWifiManager.getWifiState() == WifiManager.WIFI_STATE_DISABLED 
				&& mWifiManager.isWifiApEnabled()){
				Log.d(TAG, "softAP has been enabled. Try to disable it and toggle-on  WIFI.");	
				mSoftAPEnable = true;
				mWifiManager.setWifiApEnabled(null, false);
			}
			if (mWifiManager.setWifiEnabled(true)) {
				Log.d(TAG, "setWifiEnabled ok");
			} else {
				Log.d(TAG, "setWifiEnabled failed");
			}
		}
		Log.d(TAG, "start MiracastSinkService!!");  
		Intent i = new Intent(MainActivity.this, MiracastSinkService.class);
		startService(i);

		mState.setText("");
		mDeviceName.setText("Device name: " + Settings.Global.getString(this.getContentResolver(), Settings.Global.DEVICE_NAME) );
		mPinCode.setText( "PIN code: " + Settings.System.getString(this.getContentResolver(), Settings.System.REALTEK_SETUP_MIRACAST_SINK_PIN_CODE) );

		if (myReceiver == null) {
			myReceiver = WFDSinkReceiver;
			IntentFilter intentFilter = new IntentFilter("rtk.provider.miracastsink.SETTING");
			registerReceiver(myReceiver, intentFilter);
		}
	}
	
	@Override
    public void onPause() {    	
        super.onPause();
        Log.i(TAG,"onPause()"); 		

    }
    
    @Override
    public void onStop() {
        super.onStop();
        Log.i(TAG,"onStop()");

		//We will stop the miracast function when user exit the home menu
	//Except the Miracast is connecting and displaying 
		ActivityManager am = (ActivityManager) this.getSystemService(ACTIVITY_SERVICE);
	// get the info from the currently running task
         List< ActivityManager.RunningTaskInfo > taskInfo = am.getRunningTasks(1);	
		ComponentName componentInfo = taskInfo.get(0).topActivity;
		Log.i(TAG, "Package name= " + componentInfo.getPackageName());
		
		if(!componentInfo.getPackageName().equals("com.example.RTKMiracastSink")) //for Home Key case
			StopMiracastService();
	}
    
    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "onDestroy");
		if (myReceiver != null)
        {
        	unregisterReceiver(myReceiver);
        	myReceiver = null;
        }
		/****
		Intent i = new Intent("rtk.provider.miracastsink.STATUS");
		i.putExtra("WFDMode", "WFD_FORCE_OFF");
		sendBroadcast(i);
        ****/
		StopMiracastService();
		//android.os.Process.killProcess(android.os.Process.myPid());
    }	    	
    
    //tiny_li
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
    	if (event.getAction() == KeyEvent.ACTION_DOWN) {
        	if (keyCode == KeyEvent.KEYCODE_ESCAPE || keyCode == KeyEvent.KEYCODE_BACK) {
					/*if(timer_flag != null) {
					Log.i(TAG, "KeyEvent.KEYCODE_ESCAPE: timer_flag.cancel() enter");
					timer_flag.cancel(); timer_flag = null;
					Log.i(TAG, "KeyEvent.KEYCODE_ESCAPE: timer_flag.cancel() finish");
					}*/
					finish();
        		return true;
			}
			/*
			else if (keyCode == KeyEvent.KEYCODE_MENU) {
        		Intent intent = new Intent(this, MiracastTestList.class);
        		startActivity(intent);
        		return true;
        	} else if (keyCode == KeyEvent.KEYCODE_PROG_RED) {
        		myShellCmd.setVisibility(View.VISIBLE);
        		return true;
        	} else if (keyCode == KeyEvent.KEYCODE_PROG_GREEN) {
        		myShellCmd.setVisibility(View.INVISIBLE);
        		return true;
        	}
			*/
    	}
    	
    	return super.onKeyDown(keyCode, event);
	}		
	
	public void StopMiracastService(){
		Log.d(TAG, "Stop MiracastSinkService!!");  
		Intent i = new Intent(MainActivity.this, MiracastSinkService.class);
		stopService(i);
		
		if(mSoftAPEnable){
			Log.d(TAG, "Turn off Wifi and restart SoftAP mode !!");
			WifiManager mWifiManager = (WifiManager)getSystemService(Context.WIFI_SERVICE);
			if (mWifiManager.setWifiEnabled(false)) {
				Log.d(TAG, "setWifiDisabled ok");
			} else {
				Log.d(TAG, "setWifiDisaled failed");
			}
			mSoftAPEnable = false;
			mWifiManager.setWifiApEnabled(null, true);
		}
	}
	
	private BroadcastReceiver WFDSinkReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
            String wfdpin = intent.getStringExtra("WFDPIN");
            Log.d(TAG, "WFDPIN change:" + wfdpin);
            mPinCode.setText( "PIN code: " + wfdpin);
        }
    };
}
