/*
 * Copyright (C) 2015 The Realtek Project
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

package com.android.BluetoothRTK3ddService;

import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothClass;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.os.RemoteException;
import android.os.Binder;
import android.os.IBinder;
import android.os.Handler;
import android.widget.Toast;
import java.lang.CharSequence;
import java.lang.InterruptedException;
import android.util.Log;
import android.os.ParcelUuid;
import android.os.Message;
import android.view.Gravity;


import android.bluetooth.BluetoothAdapter;
import android.content.SharedPreferences;
import android.bluetooth.BluetoothRtkbt;
import android.bluetooth.BluetoothUuid;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.BluetoothManager;

import android.os.SystemClock;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class BluetoothRTK3ddService extends Service {
/***************************************************************************************************
            3dd service debug and version message
****************************************************************************************************/
    private static String TAG = "BluetoothRTK3ddService";
    private static boolean RTK_DBG = true;
    private static boolean RTK_INFO = true;
    private static boolean RTK_NOTIFY = true;
    private static String verison = "20150708";

/****************************************************************************************************
        3dd service Internal object
*****************************************************************************************************/
    private IntentFilter                    mIntentFilter = null;
    private BluetoothAdapter                RTKBT_3DD_BluetoothAadapter;
    private BluetoothRTK3ddProfile          m3ddProfile;
	private Handler handler = null;

/****************************************************************************************************
        3dd service member variables
*****************************************************************************************************/
    private boolean             mReveiverRegister;
    private boolean             m3ddService_start       =   true;
	private static int et_delay = 0;

    @Override
    public void onCreate() {
        super.onCreate();
        log("<===3dd===>+++ onCreate version +++"+ verison);
		handler = new Handler() {
			public void handleMessage(Message msg) {
				messageHandle(msg);
			}
		};

		et_delay = 10;
		log("<===3dd===>+++ onCreate et_delay +++"+ et_delay);

        mReveiverRegister = false;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        info("<===3dd===>++++onStartCommand++++"+ verison);
        m3ddProfile = new BluetoothRTK3ddProfile(this);
        int retCode = super.onStartCommand(intent, flags, startId);
        RTKBT_3DD_BluetoothAadapter = BluetoothAdapter.getDefaultAdapter();
        registerBroadCastReceiver();
        return retCode;
    }

    public IBinder onBind(Intent intent) {
        // not implement
        log("<===3dd===>onBind");
        return null;
    }

    @Override
    public void onDestroy() {
        info("<===3dd===>onDestroy");
        super.onDestroy();

		try{
	        if(mReveiverRegister) {
	            unregisterReceiver(mReceiver);
	        }

			//set bluetooth BT3D_MODE_IDLE Mode.
			setBluetooth3DModel(0);
			//close Bluetooth
			//kill my process.
			//android.os.Process.killProcess(android.os.Process.myPid());
		}catch(Exception ex){
			ex.printStackTrace();
		}
    }


    /***************************************************************************
     * registerBroadCastReceiver is used for register receiver device_found event
     ****************************************************************************/
    private void registerBroadCastReceiver() {
        log("<===3dd===>registerBroadCastReceiver");
        mIntentFilter = new IntentFilter();
        mIntentFilter.addAction("android.bluetooth.adapter.action.STATE_CHANGED");
		mIntentFilter.addAction(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT);
        registerReceiver(mReceiver, mIntentFilter);
        mReveiverRegister = true;
    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.e(TAG,"<===3dd===>BroadcastReceive onReceive("+action+")");
            if(action.equals("android.bluetooth.adapter.action.STATE_CHANGED")) {
				switch (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR)) {
                  case BluetoothAdapter.STATE_TURNING_OFF:
                    if (RTK_DBG) Log.e(TAG,  "Receiver BLUETOOTH_STATE_CHANGED_ACTION, STATE_TURNING_OFF");
                    break;
                  case BluetoothAdapter.STATE_ON:
                    if (RTK_DBG) Log.v(TAG,  "Receiver BLUETOOTH_STATE_CHANGED_ACTION, STATE_ON");
					break;
				  default:
                    break;
               }
            }
			if(action.equals(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT)) {
                int id = intent.getIntExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_id, 0);
				int event = intent.getIntExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_event, 0);
				int len	= intent.getIntExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_len, 0);
                info("Receive BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT, id is : " + id);
				if((id == BluetoothRTK3ddProfile.id) && (event == BluetoothRTK3ddProfile.HANDSHAKE_3DG_EVT)) {
					/*byte[] mNotifAssocData = new byte[len];
					   mNotifAssocData = intent.getByteArrayExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_data);
					   String bdaddr = String.format("%02X:%02X:%02X:%02X:%02X:%02X",
					   mNotifAssocData[5],mNotifAssocData[4],mNotifAssocData[3],mNotifAssocData[2],
					   mNotifAssocData[1],mNotifAssocData[0]);
					   handler.sendEmptyMessage(MSG_ON_3DS_ASSOC_NOTIFICATION);*/
					handler.sendEmptyMessage(MSG_ON_3DS_SET_GLASS_DELAY);
					showToast(context, context.getResources().getString(R.string.bluetooth3d_match_success) );
				}
				if((id == BluetoothRTK3ddProfile.id) && (event == BluetoothRTK3ddProfile.TOGGLE_EVT))
				{
					byte[] mToggleData = new byte[len];
					mToggleData = intent.getByteArrayExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_data);
					Log.v(TAG,  "Receiver toggle len = " + len + ",state = "+mToggleData[0]);
					if(mToggleData[0] == 1)
					{
						Log.v(TAG,"3d sync state is on");
						handler.sendEmptyMessage(MSG_START_SERVICE);
					}
					else
					{
						Log.v(TAG,"3d sync state is off");
						handler.sendEmptyMessage(MSG_EXIT_SERVICE);
					}
				}
			}
        }
    };


	//--------------------------------------------------------------------------
	private static final int MSG_START_SERVICE=0x10001;
	private static final int MSG_EXIT_SERVICE=0x10002;
		
	private static final int MSG_ON_3DS_SET_MASTER=0x10000;
		
	private static final int MSG_ON_3DS_ASSOC_NOTIFICATION=0x10003;
	private static final int MSG_ON_3DS_BATTERY_LEVEL_CHANGED=0x10004;
	private static final int MSG_ON_3DS_FRAME_PERIOD_CHANGED=0x10005;
	private static final int MSG_ON_3DS_MASTER_SYNC_STATUS_CHANGED=0x10006;
	private static final int MSG_ON_3DS_SET_GLASS_DELAY=0x10007;

	private void messageHandle(Message msg) {
		// TODO Auto-generated method stub
		switch (msg.what) {	
		case MSG_ON_3DS_SET_MASTER:
			handleBT3D_MSG_ON_3DS_SET_MASTER();
			break;		
		case MSG_ON_3DS_ASSOC_NOTIFICATION:
			handleBT3D_ON_3DS_ASSOC_NOTIFICATION();
			break;
		case MSG_ON_3DS_BATTERY_LEVEL_CHANGED:
			//handleBT3D_ON_3DS_BATTERY_LEVEL_CHANGED();
			break;
		case MSG_ON_3DS_FRAME_PERIOD_CHANGED:
			//handleBT3D_ON_3DS_FRAME_PERIOD_CHANGED();
			break;
		case MSG_ON_3DS_MASTER_SYNC_STATUS_CHANGED:
			//handleBT3D_ON_3DS_FRAME_PERIOD_CHANGED();
			break;
		case MSG_ON_3DS_SET_GLASS_DELAY:
			handleBT3D_ON_3DS_SET_GLASS_DELAY();
			break;
		case MSG_START_SERVICE:
			handleBT3D_MSG_START_SERVICE();
			break;
		case MSG_EXIT_SERVICE:
			handleBT3D_MSG_EXIT_SERVICE();
			break;
		}
	}

	private int current_mode;
	private void setBluetooth3DModel(int modelType){
	    switch(modelType) {
	    case 0: // IDLE mode
	        current_mode = 0;
			m3ddProfile.GenericCommand(null,0,0);
	        break;
	    case 1: // MASTER mode
	        //current_mode = Bluetooth3DSync.BT3D_MODE_MASTER;
			if(RTKBT_3DD_BluetoothAadapter.isDiscovering())
						RTKBT_3DD_BluetoothAadapter.cancelDiscovery();
			m3ddProfile.GenericCommand(null,0,1);
	        break;
	    default:
	        Log.d(TAG, "Unsupported mode:"+modelType);
	        return;
	   	}
	}
	
	private void handleBT3D_MSG_START_SERVICE(){
		if(m3ddProfile == null) {
	           Log.d(TAG, "Not connected to Proxy yet");
	           return;
	    }
		if (!RTKBT_3DD_BluetoothAadapter.isEnabled()) {
            RTKBT_3DD_BluetoothAadapter.enable();  
            Log.v(TAG,"start3ddservice BluetoothAadapter enable ");
        }
		int current_mode = 1;
		setBluetooth3DModel(current_mode);
	}
	private void handleBT3D_MSG_EXIT_SERVICE(){
		if(m3ddProfile == null) {
	           Log.d(TAG, "Not connected to Proxy yet");
	           return;
	    }
		int current_mode = 0;
		setBluetooth3DModel(current_mode);
	}
	private void handleBT3D_MSG_ON_3DS_SET_MASTER(){
		if(m3ddProfile == null) {
	           Log.d(TAG, "Not connected to Proxy yet");
	           return;
	    }
		int current_mode = 1;
		setBluetooth3DModel(current_mode);
	}

	private void handleBT3D_ON_3DS_ASSOC_NOTIFICATION(){}
	private void handleBT3D_ON_3DS_BATTERY_LEVEL_CHANGED(){}
	private void handleBT3D_ON_3DS_FRAME_PERIOD_CHANGED(){}
	private void handleBT3D_ON_3DS_MASTER_SYNC_STATUS_CHANGED(){
		new Thread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				//...........
			}
		}).start();
	}

	private void handleBT3D_ON_3DS_SET_GLASS_DELAY(){
		if(m3ddProfile == null) {
			Log.d(TAG, "Not connected to Proxy yet");
			return;
		}

		//set dongle to 3D glass delay time
		byte[] mDelayTime = new byte[2];
		mDelayTime[0] = (byte) (et_delay&0x00FF);
		mDelayTime[1] = (byte) ((et_delay&0xFF00)>>8);
		String delaytime = String.format("%02X:%02X",mDelayTime[1],mDelayTime[0]);
		Log.d(TAG, "delaytime = "+delaytime);
		m3ddProfile.GenericCommand(mDelayTime,2,2);
	}

    /**************************************************************************************************************
        **      function : notifyUser
        **      decription: toast content to the user
    ***************************************************************************************************************/
	private void showToast(Context mContext, String showMsg){
		Toast toast = Toast.makeText(mContext, showMsg, Toast.LENGTH_LONG);
		toast.setGravity(Gravity.CENTER, 0, 0);
		toast.show();  
	}

    private static void log(String msg) {
        if(RTK_DBG)
            Log.d(TAG, msg);
    }
    private static void info(String msg) {
        if(RTK_INFO)
            Log.i(TAG, msg);
    }

}


