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
import java.util.Locale;
import java.util.List;  


import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.WifiP2pManager.Channel;
import android.net.wifi.p2p.WifiP2pManager.PeerListListener;
import android.os.Bundle;
import android.util.Log;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.app.ActivityManager;
import java.io.IOException;
import java.io.*;

//import android.widget.TextView;
//import android.widget.Toast;
//import android.os.CountDownTimer;
/**
 * A BroadcastReceiver that notifies of important wifi p2p events.
 */
public class MiracastBroadcastReceiver extends BroadcastReceiver {

    private static final String TAG = "MiracastBroadcastReceiver";
    private WifiP2pManager manager;
    private Channel channel;
    //private MainActivity activity;
    CP2PCore activity;
    private String url = null;
    public boolean flag = false;
 
    //CountDownTimer timer2 = null;
    /**
     * @param manager WifiP2pManager system service
     * @param channel Wifi p2p channel
     * @param activity activity associated with the receiver
     */
    public MiracastBroadcastReceiver(WifiP2pManager manager, Channel channel,
            CP2PCore activity) {
        super();
        this.manager = manager;
        this.channel = channel;
        this.activity = activity;
        this.flag =false;
    }

    /*
     * (non-Javadoc)
     * @see android.content.BroadcastReceiver#onReceive(android.content.Context,
     * android.content.Intent)
     */
    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)) {
        	Log.i(TAG, "WIFI_P2P_STATE_CHANGED_ACTION");
            // UI update to indicate wifi p2p status.
            int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);

	final boolean toggle = Settings.System.getInt( context.getContentResolver(),  Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, 0) != 0;
	Log.i( TAG, "MiracastSink  toogle = " + toggle  );
		
            if (state == WifiP2pManager.WIFI_P2P_STATE_ENABLED && toggle ==true ) {
                // Wifi Direct mode is enabled
		//activity.setIsWifiP2pEnabled(true);
		
                // We need to check the current top AP is home menu
                // Otherwise, we will not start PollingTimer
                ActivityManager am = (ActivityManager) context.getSystemService(context.ACTIVITY_SERVICE);
                
                // get the info from the currently running task
                List< ActivityManager.RunningTaskInfo > taskInfo = am.getRunningTasks(1);  
                ComponentName componentInfo = taskInfo.get(0).topActivity;
                
                if( componentInfo.getPackageName().equals("com.realtek.launcher") )
                    {

	           Intent i = new Intent("rtk.provider.miracastsink.STATUS");
                    i.putExtra("WFDMode", "WFD_ON");
                    activity.sendBroadcast(i);

		Log.i(TAG, "p2p enabled");
                    }
                else
                    {
                       Log.d(TAG, "We will not send out WFD_ON, because current APK is: " + componentInfo.getPackageName());
                    }
            } else {

                  //activity.setIsWifiP2pEnabled(false);
		//activity.resetData();

                  Intent i = new Intent("rtk.provider.miracastsink.STATUS");
                  i.putExtra("WFDMode", "WFD_OFF");
                  activity.sendBroadcast(i);

		Log.i(TAG, "p2p disabled");
            }
        } else if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)) {
        	Log.i(TAG, "WIFI_P2P_PEERS_CHANGED_ACTION");
            if (manager != null) {
                manager.requestPeers(channel, (PeerListListener)activity.mDeviceList);
            }
        } else if (WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.equals(action)) {
        	Log.i(TAG, "WIFI_P2P_CONNECTION_CHANGED_ACTION");
            if (manager == null) {
            	Log.i(TAG, "WIFI_P2P_CONNECTION_CHANGED_ACTION 1");
                return;
            }

            NetworkInfo networkInfo = (NetworkInfo)intent.getParcelableExtra(WifiP2pManager.EXTRA_NETWORK_INFO);
            //WifiP2pInfo p2pInfo = (WifiP2pInfo)intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_INFO);
			//Log.i(TAG, "EXTRA_WIFI_P2P_INFO :" + p2pInfo);
            if (networkInfo.isConnected()) {
            	Log.i(TAG, "p2p connect 0");
            	if(activity.GetState() != CP2PCore.WFD_STATE_CONNECTED) {
					manager.requestConnectionInfo(channel, activity.mDeviceDetail);
					activity.StopCountdownTimer();
					activity.StartCountDownTimer(CP2PCore.RETRY_RTSP_TIMEOUT);  // set 20s timeout for rtsp
					Log.i(TAG, "p2p connect");
                               
                                            if( isDLNADMRModeOn(context) )
                                            {                                                
                                                //Stop DMR temporary
                                                Log.i(TAG, "Stop DMR...");
                                                Intent serviceIntent = new Intent();
                                                serviceIntent.setAction("com.realtek.dmr.DMRService");
                                                serviceIntent.setPackage("com.realtek.dmr");
                                                context.stopService(serviceIntent);        
                                            }                                            
				}
				else {
					Log.i(TAG, "p2p already connect, skip this.");
				}
            } else {
            	int istate = activity.GetState();
            	Log.e(TAG, "p2p istate="+istate);
		//if(istate == CP2PCore.WFD_STATE_CONNECTED || istate == CP2PCore.WFD_STATE_CONNECTING)
		NetworkInfo.DetailedState lState = networkInfo.getDetailedState();
		Log.e(TAG, "p2p DetailedState="+lState);
		if ((lState == NetworkInfo.DetailedState.DISCONNECTED) || (lState == NetworkInfo.DetailedState.FAILED))
		{
			Log.e(TAG, "p2p disconnect 0");
			//check mCheckRTSPTerminate to make sure rtk_wfd terminateed, timeout 5 secs.
			int iwait = 0;
			do {
				if(activity.mCheckRTSPTerminate) {
					Log.i(TAG, "MiracastBroadcastReceiver receives the terminate signal from RTSP Monitor thread.");
					activity.SetState(CP2PCore.WFD_STATE_DISCONNECTED);
					activity.mCurConnectedDevice = null;
					Intent kill = new Intent("MediaPlayerKill");
					activity.sendBroadcast(kill);
					activity.retry(CP2PCore.RETRY_RESTART);
					Log.e(TAG, "p2p disconnect 1");
                                       
					if( isDLNADMRModeOn(context) )
					{
						//Restart DMR
						Log.i(TAG, "Restart DMR...");
						Intent serviceIntent = new Intent();
						serviceIntent.setAction("com.realtek.dmr.DMRService");
						serviceIntent.setPackage("com.realtek.dmr");
						context.startService(serviceIntent);
					}
					if(!activity.mDisconnectedSuccess)
						activity.mDisconnectedSuccess = true;

					// Restore to p2p_listen when p2p is disconnected
					activity.StartPollingTimer();
					/*
					    if( activity.GetWfdChAutoSwitch() == true )
					    {
						Intent i = new Intent("rtk.provider.miracastsink.STATUS");
						i.putExtra("WFDMode", "WFD_CH_AUTO_SWITCH_ON");
						activity.sendBroadcast(i);
					    }
					*/
					
					activity.mCheckRTSPTerminate = false;
					break;
				} //&& !f.isDirectory()) { /* do something */ }
				else {
					try{
						Thread.sleep(100);
					}catch(InterruptedException e){
						e.printStackTrace();
					}
				}
			}while(iwait++<50);
			if (iwait>=50) {
				Log.i(TAG, "WARNING: rtk_wfd seems not terminate! Do nothing.");
			}
			//end of check
		}
            }
        } else if (WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION.equals(action)) {
        	Log.i(TAG, "WIFI_P2P_THIS_DEVICE_CHANGED_ACTION");
            WifiP2pDevice thisDevice = (WifiP2pDevice) intent.getParcelableExtra(
                    WifiP2pManager.EXTRA_WIFI_P2P_DEVICE);            
            if(flag==false){
            	//TextView myDevName=(TextView)activity.findViewById(R.id.myDevName);
            	//String strid = thisDevice.deviceName+"_"+thisDevice.deviceAddress.toUpperCase(Locale.getDefault());
                //myDevName.setText(strid);
               
                flag =true;
            }
            activity.mDeviceList.updateThisDevice(thisDevice);           
            activity.mDeviceDetail.thisDevice = thisDevice;
        //}else if ("MediaPlayershutdown".equals(action)){
		//	activity.SetState(MainActivity.WFD_STATE_DISCONNECTED);
		//	Log.i(TAG, "MediaPlayershutdown"); 
        //	activity.disconnect();
        //	flag=false;
			//String strip=activity.getString(R.string.connect_try);
		//	activity.retry("retry");
		//	Log.e(TAG, "disconnect");
        }else if (WifiP2pManager.WIFI_P2P_PERSISTENT_GROUPS_CHANGED_ACTION.equals(action)) {
			Log.i(TAG, "WIFI_P2P_PERSISTENT_GROUPS_CHANGED_ACTION");
            if (manager!= null) {
                manager.requestPersistentGroupInfo(channel, activity);
            }
        }
    }      

        public static boolean isDLNADMRModeOn(Context context) {
		boolean res = false;
		try {
                    	res = Settings.System.getInt(context.getContentResolver(), Settings.System.REALTEK_SETUP_DLNA_DMR_MODE_ON) != 0;
		} catch (SettingNotFoundException snfe) {

		}
		return res;
         }
}
   
