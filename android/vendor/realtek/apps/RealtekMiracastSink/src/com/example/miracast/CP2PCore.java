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
import java.util.concurrent.Semaphore;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.ContentObserver;
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
import android.net.Uri;
import android.text.format.Time;

import android.widget.TextView.OnEditorActionListener;


import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
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

//import android.app.TvManager;
//import com.rtk.android.miracast.WFDDeviceList.DeviceActionListener;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.app.ActivityManager;
import android.os.SystemProperties;



import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.DataOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.util.Timer;
import java.util.TimerTask;
import java.io.File;   


public class CP2PCore implements ChannelListener, PersistentGroupInfoListener
{	
	private static CP2PCore instance;
	static {
        instance = new CP2PCore();
    }

		private Context context = null;
		public static final int WFD_URL = 0;
		public static final int WFD_CREATEGROUP = 1;
		public static final int WFD_PINANY = 2;
		public static final int WFD_RESET = 3;

		public static final int WFD_STATE_INIT = 0;
		public static final int WFD_STATE_READY = 1;
		public static final int WFD_STATE_CONNECTING = 2;
		public static final int WFD_STATE_CONNECTED = 3;
		public static final int WFD_STATE_DISCONNECTED = 4;

                  public static final int RETRY_RESTART = 0;  //The WFD connection is disconnected or fail, need to use try() to do P2P_LISTEN
                  public static final int RETRY_TIMEOUT = 1;    //Timeout could be normal timeout(120s) or connection timeout(40s)
                  
		private int curState = WFD_STATE_INIT;
                  private boolean bWfdForceOff = false;
                  private boolean bWfdChAutoSwitchEnabled = false;
	
		private WifiP2pManager manager;
		private Channel channel;
		private boolean isWifiP2pEnabled = false;
		
		public static final String TAG = "P2PCore";
	//private static TvManager mTV;		
    	private boolean retryChannel = false;

    	private final IntentFilter intentFilter = new IntentFilter();
    	private BroadcastReceiver receiver = null;
    	private BroadcastReceiver myReceiver = null;
    	public WifiP2pDevice mCurConnectedDevice;
    	public int p2pdevicelost=0;
    	public Process wfdProcess = null;
    	private String ipAddr;
    	//public TextView text;
    	//public TextView myDevName;
		//public TextView myMacName;
		//public TextView myPin;
    	//public EditText myShellCmd;
    
	
    	public WifiP2pDevice thisDev;
		public String strPIN = null;
		private String strDeviceName = null;

		private static final int DEFAULT_CONTROL_PORT = 8554;
		private static final int MAX_THROUGHPUT = 50;
		public boolean mWfdEnabled = false;
		public boolean mWfdEnabling = false;
		public String devname=null;
		private int mListenChannel=6;
    	private int mOperatingChannel=0;//11;

		public WFDDeviceList mDeviceList = new WFDDeviceList(this);
		public WFDDeviceDetail mDeviceDetail = new WFDDeviceDetail(this);
		
	//private WifiManager.WpsListener mWpsListener;
	//private Thread mThread = null;
	//private Runnable mRunnable = null;	
	/*added by kavin*/
		WifiP2pGroupList grouplist;
    //public  TextView mWpsTextView;//as mState
    //private Handler mHandler = new Handler();
		private Timer timer_flag=null;
		MyTimerTask ltimertask = null;

                private Timer AutoListenTimer = null;
                AutoListenTimerTask AutoListenTimerTask = null;

                private Timer PollingTimer = null;
                PollingTimerTask PollingTimerTask = null;

                //This flag was set to true when P2P is begining
                //Reset to false when P2P is finished
               // private boolean mIsP2pConnecting = false;

                private int mP2pCountdownTimeout = 0;
                public static final int RETRY_NORMAL_TIMEOUT = 120000;
                public static final int RETRY_CONNECTION_TIMEOUT = 40000;
                public static final int RETRY_RTSP_TIMEOUT = 20000;

	//private static native void printHello();
	
		// load JNI Library: libwfd.so
		//static 
		//{
	    //	System.loadLibrary("wfd");
		//}	
		
	/*// Handler to update UI
       Handler mHandler = new Handler()
       {
               @Override
        public void handleMessage(Message msg)
        {
            switch (msg.what)
            {
                 case MainActivity.GUIUPDATEID:
                         updateUI();
                 break;
            }
            super.handleMessage(msg);
        }
       };*/
    
	
	public static boolean mDisconnectedSuccess = false;
	public static boolean mCheckRTSPTerminate = false;
	private Thread mCheckRTSPThread = null;
	private CheckRTSPHandler mCheckRTSPHandler = null;
	private boolean CheckRTSPThreadStart = false;
	private boolean mTriggerByRTSPHandler = false;
	private long TryDiscon_t1 = 0, TryDiscon_t2 = 0; 
	private static final int CHECK_RTSP = 46794; //for mantis 46794
	public class CheckRTSPHandler extends Handler{
		public boolean sendMessage(int what, int arg1, int arg2){
			Log.i(TAG, "send message to check_rtsp handler!");
			return super.sendMessage(obtainMessage(what, arg1, arg2));
		}
		@Override
		public void handleMessage (Message msg){
			switch(msg.what){
				case CHECK_RTSP:
					File f = new File("/tmp/rtk_wfd.terminate");
						if(f.exists()){
							//Log.i(TAG, "RTSP Monitoring Handler : /tmp/rtk_wfd.terminate exists!! mCheckRTSPTerminate= " +  mCheckRTSPTerminate + ", mDisconnectedSuccess= " + mDisconnectedSuccess);
							if((!mCheckRTSPTerminate) && (!mDisconnectedSuccess)){
								Log.i(TAG, "RTSP Monitoring Handler : Caught wfd_rtsp terminate signal");
								if((TryDiscon_t1 == 0) && (TryDiscon_t2 == 0)){
									TryDiscon_t1 = System.currentTimeMillis();
									break;
								}
								else{
									TryDiscon_t2 = System.currentTimeMillis();
									if((TryDiscon_t2 - TryDiscon_t1) < 2000){
										Log.i(TAG, "RTSP Monitoring Handler : Hold on");
										break;
									}
								}
								if(!mDisconnectedSuccess){  //P2P disconnect has not been executed
									Log.i(TAG, "RTSP Monitoring Handler : Try to disconnect by P2P manager");
									mCheckRTSPTerminate = true;
									retry(RETRY_RESTART);
								}
							}
							else if(mCheckRTSPTerminate){
								if(!mDisconnectedSuccess)
									Log.i(TAG, "RTSP Monitoring Handler : Keep waiting for the response from MiracastBroadcastReceiver!!");
								else{
									Log.i(TAG, "RTSP Monitoring Handler has received the p2p disconnect response from MiracastBroadcastReceiver!!");
									//setIsWifiP2pEnabled(false);
									//mTriggerByRTSPHandler = true;
									mCheckRTSPTerminate = false;	
									TryDiscon_t1 = TryDiscon_t2 = System.currentTimeMillis();
									//Intent kill = new Intent("MediaPlayerKill");
									//sendBroadcast(kill);
								}
							}
							else{ //mCheckRTSPTerminate == false  &&  mDisconnectedSuccess == true
							//--for 5.0.0_r2 wifi-reconnect
							/***
								if((TryDiscon_t2 == 0) || (TryDiscon_t2 > TryDiscon_t1)){
									TryDiscon_t1 = TryDiscon_t2 = System.currentTimeMillis();
									if(!mTriggerByRTSPHandler)
									{
										Log.i(TAG, "RTSP Monitoring Handler : Try to WFD_FORCE_OFF!!");
										Intent i = new Intent("rtk.provider.miracastsink.STATUS");
										i.putExtra("WFDMode", "WFD_FORCE_OFF");
										sendBroadcast(i);
									}
									break;
								}
								else{
									TryDiscon_t1 = System.currentTimeMillis();
									if((TryDiscon_t1 - TryDiscon_t2) < 8000)
										break;
									else{
										Log.i(TAG, "RTSP Monitoring Handler : Try to WFD_FORCE_ON!!");
										Intent i = new Intent("rtk.provider.miracastsink.STATUS");
										i.putExtra("WFDMode", "WFD_FORCE_ON");
										sendBroadcast(i);
									}
								}
							****/
							//------
								Log.i(TAG, "RTSP Monitoring Handler : not do anything and will be terminated!!");
								f.delete();
								mCheckRTSPTerminate = mDisconnectedSuccess = CheckRTSPThreadStart = false;
								TryDiscon_t1 = TryDiscon_t2 = 0;
								mTriggerByRTSPHandler = false;
							}
						}
						//else
						//	Log.i(TAG, "RTSP Monitoring Handler : /tmp/rtk_wfd.terminate is gone!!");
					break;
				default:
					Log.i(TAG, "can not identify message in check_rtsp handler!");
					break;
			}
		}
	}

	
    private BroadcastReceiver WFDSinkReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
            String wfdMode = intent.getStringExtra("WFDMode");
            if (wfdMode != null) {
            	boolean toggle = Settings.System.getInt( context.getContentResolver(),  Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, 0) != 0;
            	Log.d(TAG, "wfdMode is received. - " + wfdMode + ", toggle =" + toggle + ",bWfdForceOff =" + bWfdForceOff + ",bWfdChAutoSwitchEnabled =" + bWfdChAutoSwitchEnabled );

            if (toggle == true) {
                	if (wfdMode.equalsIgnoreCase("WFD_ON") && !isWifiP2pEnabled) {//wfd on
                	        if(bWfdForceOff==true) {
                		        Log.d(TAG, "Ignore this WFD_ON due to bWfdForceOff is set");
                               }
                            else{
                                    setIsWifiP2pEnabled(true);
                                }
            		}
                    else if( wfdMode.equalsIgnoreCase("WFD_FORCE_ON")  ){
                            setIsWifiP2pEnabled(true);
                            bWfdForceOff = false;
                            StartPollingTimer();
                        }
            		else if (wfdMode.equalsIgnoreCase("WFD_OFF") && isWifiP2pEnabled) {//wfd off
            			setIsWifiP2pEnabled(false);
            			resetData();
            		}
                            else if (wfdMode.equalsIgnoreCase("WFD_FORCE_OFF") && isWifiP2pEnabled) {
            			setIsWifiP2pEnabled(false);
            			resetData();

                                    StopAutoListenTimer();
                                    StopPollingTimer();

                                    bWfdForceOff = true;
            		}
                            else if( wfdMode.equalsIgnoreCase("WFD_CH_AUTO_SWITCH_ON")  && !bWfdForceOff )
                                {
                                    Log.d(TAG, "+++++ Handle WFD_CH_AUTO_SWITCH_ON +++++");

                                    if( bWfdForceOff == false )
                                        {                                            
                                            bWfdChAutoSwitchEnabled = true;
                                            
                                            //Start timer, from low, them high
                                            if ( AutoListenTimer==null ) {
                                                Log.d(TAG, "Start AutoListenTimerTask");

                                                    // From low, then set listen 3 seconds for every 6 seconds
        		                                  manager.stopPeerDiscovery( channel, null);
                                                    AutoListenTimerTask = new AutoListenTimerTask();
                                                    AutoListenTimer = new Timer();
                                                    AutoListenTimer.schedule(AutoListenTimerTask, 3000, 6000);
                                                }                                       
                                        }
                                    else
                                        {
                                            Log.d(TAG, "bWfdForceOff is true, do nothing ~~");
                                        }

                                }
                            else if( wfdMode.equalsIgnoreCase("WFD_CH_AUTO_SWITCH_OFF") && isWifiP2pEnabled )
                                {
                                    Log.d(TAG, "----- Handle WFD_CH_AUTO_SWITCH_OFF -----");
                                    StopAutoListenTimer();
                                     bWfdChAutoSwitchEnabled = false;

                                    // Should we filter out this event if WFD is still in connected state ???
                                    if( curState != WFD_STATE_CONNECTING && curState != WFD_STATE_CONNECTED )
                                        {                                            
                                           manager.ListenPeers(channel, new WifiP2pManager.ActionListener() {
                                            @Override
                                            public void onSuccess() {
                                                Log.i(TAG, "Listen Initiated");             
                                            }
                                            
                                            @Override
                                            public void onFailure(int reasonCode) {
                                                Log.e(TAG, "Listen Failed");
                                            }
                                                });                                   

                                        }
                                }
                            else
                                {
                                    Log.d(TAG, " Ignore WFDMode ~");
                                }
            	}
        	}
        }
    };

    public void StopAutoListenTimer() {
                Log.i(TAG, "StopAutoListenTimer");
                 if( AutoListenTimer!= null )
                {
                    AutoListenTimer.cancel();
                    AutoListenTimer = null;
                    AutoListenTimerTask = null;
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

    public void StartPollingTimer() {

             //Start polling timer if necessary (sink is on/dmr is on)
             final boolean miracast_sink_toggle = Settings.System.getInt( GetContext().getContentResolver(),  Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, 0) != 0;
             boolean dmr_toggle = isDLNADMRModeOn( GetContext());
             
             Log.d( TAG, "Try to start polling timer, miracast sink toogle = " + miracast_sink_toggle + " dmr toggle =" + dmr_toggle);

             if( miracast_sink_toggle == true && dmr_toggle == true)
                {                
                    if ( PollingTimer==null ) {
                           Log.d(TAG, "Start PollingTimerTask");
                    
                           PollingTimerTask = new PollingTimerTask();
                           PollingTimer = new Timer();
                           PollingTimer.schedule( PollingTimerTask, 1000, 200);
                       }
                    else
                       {
                           Log.d(TAG, "PollingTimerTask is existed~~");
                   }
                }
             else
                {
                    Log.d(TAG, "Polling timer is shutdown...");
                }
            
    }
             
    public void StopPollingTimer() {
                Log.i(TAG, "StopPollingTimer");
                 if( PollingTimer!= null )
                {
                    PollingTimer.cancel();
                    PollingTimer = null;
                    PollingTimerTask = null;
                }
        }
		
		
	public void StartMonitoringThread(){
		//-----Create a sub-thread to send message to RTSP monitoring handler
		if(mCheckRTSPThread == null){	
			Log.i(TAG, "RTSP Monitoring Thread can be created!");
			bWfdChAutoSwitchEnabled = true;
			/*File f = new File("/tmp/rtk_wfd.terminate");
			if(f.exists())
				f.delete();*/
			mCheckRTSPHandler = new CheckRTSPHandler();
			CheckRTSPThreadStart = true;
			mCheckRTSPThread = new Thread(new Runnable(){
				@Override
				public void run(){
					Log.i(TAG, "check if wifi rtsp has been terminated or not actively every 1 sec");
					while(CheckRTSPThreadStart){
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						try{
								Message msg = new Message();
								msg.what = CHECK_RTSP;
								mCheckRTSPHandler.sendMessage(msg);
						}
						catch(Exception e){
							e.printStackTrace();
							Log.i(TAG, "exception for check wifi_rtsp thread"); 
						}
					}
					
				}
			});
			mCheckRTSPThread.start();
		}
		else{
			Log.i(TAG, "RTSP Monitoring Thread can not be created, please check if you have stopped that or not!");	
		}
	}	
	
	public void StopMonitoringThread(){
		Log.i(TAG, "RTSP Monitoring Thread >> try to be terminated!");	
		int iwait = 0;
		do {
			File f = new File("/tmp/rtk_wfd.terminate");
			if(f.exists()) {
				Log.i(TAG, "waiting for RTSP Monitor Thread finished.");
				try{
					Thread.sleep(100);
				}catch(InterruptedException e){
					e.printStackTrace();
				}
			}
			else {
				Log.i(TAG, "RTSP Monitor Thread is ready to be terminated.");
				break;
			}
		}while(iwait++<50);     // timeout 5 secs.
		if (iwait>=50) {
			Log.i(TAG, "WARNING: RTSP Monitoring Handler is not finished!");
		}

		mDisconnectedSuccess = false;
		mCheckRTSPTerminate = false;
		if(CheckRTSPThreadStart)
			CheckRTSPThreadStart = false;
		if(mCheckRTSPThread != null)
		{
			mCheckRTSPThread.interrupt();
			mCheckRTSPThread = null;
		}
	}	

    public boolean GetWfdChAutoSwitch() {
        Log.i(TAG, "bWfdChAutoSwitchEnabled = " + bWfdChAutoSwitchEnabled );
            return bWfdChAutoSwitchEnabled;
        }
    
    class AutoListenTimerTask extends TimerTask {
        int i =0;
        public void run() {
                        i++;
                        Log.d(TAG, "[AutoListenTimerTask] timer is run (i= " + i + " )");
                        
             		manager.ListenPeersAuto(channel, new WifiP2pManager.ActionListener() {
			@Override
			public void onSuccess() {
				Log.i(TAG, "Listen Initiated");
			
			}

			@Override
			public void onFailure(int reasonCode) {
				Log.e(TAG, "Listen Failed");
			}
			});
		}
  	}


        class PollingTimerTask extends TimerTask {
        int history_val = -1;
        int current_val = 0;
        
        public void run() {
                        String ch_auto_switch= SystemProperties.get("rtk.miracastsink.STATUS");                        
                        //Log.d(TAG, "[Polling] timer is running " + ch_auto_switch);

                        if( ch_auto_switch.equals("WFD_CH_AUTO_SWITCH_ON") )
                            {
                                current_val = 1;
                            }
                        else
                            {
                                current_val = 0;
                            }

                        if( current_val != history_val )
                            {
                                Log.d(TAG, "[Polling] timer is running " + ch_auto_switch);
                                
                                Intent i = new Intent("rtk.provider.miracastsink.STATUS");
                                if( current_val == 1)
                                    {
                                         i.putExtra("WFDMode", "WFD_CH_AUTO_SWITCH_ON");
                                    }
                                else
                                    {
                                         i.putExtra("WFDMode", "WFD_CH_AUTO_SWITCH_OFF");
                                    }
                                sendBroadcast(i);

                                history_val = current_val;
                            }
	    }
        }
  	

	public static CP2PCore GetInstance() {
        return instance;
    }
	public CP2PCore() {
	}
	public void SetContext(Context context) {
		this.context = context;
	}
	public Context GetContext() {
		return this.context;
	}			
	// Implement Draw UI    	
    public void OnCreate() {
        Log.d(TAG, "onCreate");
    	// add necessary intent values to be matched.
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);
        /*added by kavin*/
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_PERSISTENT_GROUPS_CHANGED_ACTION);        
        //intentFilter.addAction("MediaPlayershutdown");

	// Listen to the Miracast Sink enable setting in P2P service
	 GetContext().getContentResolver().registerContentObserver(Settings.System.getUriFor( Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON), false, mSettingsObserver);
	
	if(Settings.System.getInt( GetContext().getContentResolver(),  Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, 0) == 0)
		Settings.System.putInt( GetContext().getContentResolver(),  Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, 1);
		
	GetContext().getContentResolver().registerContentObserver(Settings.Global.getUriFor( Settings.Global.DEVICE_NAME), false, mDevNameObserver);

		//baili_feng enable wifi if wifi is not enabled		
		ready();
		Log.d(TAG, "setDialogListener");		
		//mRunnable = new Refresh();
		//mThread = new Thread(mRunnable);
		//mThread.start();
		//
		//
    }
    
    public void OnResume()
    {
    	Log.d(TAG, "OnResume");		
    	if(receiver == null) {
    		manager = (WifiP2pManager) context.getSystemService(Context.WIFI_P2P_SERVICE);
			channel = manager.initialize(context, context.getMainLooper(), null);        
			receiver = new MiracastBroadcastReceiver(manager, channel, this);
			context.registerReceiver(receiver, intentFilter);			        	
		}
		if (myReceiver == null) {
			myReceiver = WFDSinkReceiver;
			IntentFilter intentFilter = new IntentFilter("rtk.provider.miracastsink.STATUS");
			context.registerReceiver(myReceiver, intentFilter);
		}

                    if( bWfdChAutoSwitchEnabled == true )
                    {
                             Intent i = new Intent("rtk.provider.miracastsink.STATUS");
                             i.putExtra("WFDMode", "WFD_CH_AUTO_SWITCH_ON");
                             sendBroadcast(i);
                    }

                    // We need to check the current top AP is home menu
                    // Otherwise, we will not start PollingTimer
                    ActivityManager am = (ActivityManager) context.getSystemService(context.ACTIVITY_SERVICE);
                    
                    // get the info from the currently running task
                    List< ActivityManager.RunningTaskInfo > taskInfo = am.getRunningTasks(1);  
                    ComponentName componentInfo = taskInfo.get(0).topActivity;

                	//if( componentInfo.getPackageName().equals("com.realtek.launcher") )
                    //{
					Log.d(TAG, "StartPollingTimer() in OnResume");
                     StartPollingTimer();
                    //}
                    //else
                    //{
                     //       Log.d(TAG, "We will not start polling timer, because current APK is: " + componentInfo.getPackageName());
                    //}
		
		//StartMonitoringThread();
    }
    
    public void onPause()
    {
    	Log.d(TAG, "onPause");
          if (manager != null) {          
              manager.stopPeerDiscovery(channel, null);
              if( bWfdChAutoSwitchEnabled == true )
                {
                    StopAutoListenTimer();
                }
            //manager.listen(channel,false,null);
            //setListenMode(false);
            //unregisterReceiver(receiver);
            }
          else
            {
                Log.d(TAG, "manager is null, ignore~");
            }
    }
    
    class MyTimerTask extends TimerTask {
    	private long INTERVAL=0;
    	private long TIMEOUT=0;
    	private long elapsed=0;
    	private boolean mbStop = false;
    	public void Set(long i,long j)
    	{
    		TIMEOUT = i;
    		INTERVAL = j;
    		elapsed = 0;
    	}
    	public void Stop()
    	{
    		Log.d(TAG, "TimerTask, stop 0.");
    		mbStop = true;
    	}
  		@Override
		public void run() {
                elapsed+=INTERVAL;
                if(elapsed>=TIMEOUT){                	
                    this.cancel();
                    Log.d(TAG, "TimerTask, timeout.");
                    retry(RETRY_TIMEOUT);
                    return;
                }
                else if (mbStop){
                	Log.d(TAG, "TimerTask, stop 1.");
                	this.cancel();
                	return;
            	}
                //else {
                //	Log.d(TAG, "TimerTask, tick: "+elapsed);
                //}
		}
  	}


    public void StartCountDownTimer(int TIMEOUT)
    {
    	long elapsed=0;
    	long INTERVAL=1000;

         mP2pCountdownTimeout = TIMEOUT;
         Log.i(TAG, " StartCountDownTimer(), mP2pCountdownTimeout:  " + mP2pCountdownTimeout);
         
    	if (timer_flag==null) {
    		ltimertask = new MyTimerTask();
    		ltimertask.Set(TIMEOUT,INTERVAL);
        	timer_flag = new Timer();
        	timer_flag.scheduleAtFixedRate(ltimertask, 0, INTERVAL);
    	}

    }
    
    public void StopCountdownTimer()
    {
    	if(timer_flag != null) {
        	Log.i(TAG, "timer_flag.cancel() enter");		
        	//ltimertask.cancel(); 
        	//ltimertask = null;
        	ltimertask.Stop();
        	//Log.i(TAG, "timer_flag.cancel() enter 1");        	        	
        	//Log.i(TAG, "timer_flag.cancel() enter 2");
			timer_flag.cancel(); 
			//Log.i(TAG, "timer_flag.cancel() enter 3");		
			ltimertask = null;
			timer_flag = null;
			Log.i(TAG, "timer_flag.cancel() finish");
		}
    }
    
    public void onDestroy()
    {
    	Log.d(TAG, "onDestroy");		
		
		//StopMonitoringThread();
			
    	//manager.setDialogListener(channel, null);				
		if(receiver != null)
		{
			context.unregisterReceiver(receiver);
			receiver = null;
        }
        
        if (myReceiver != null)
        {
        	context.unregisterReceiver(myReceiver);
        	myReceiver = null;
        }

		StopCountdownTimer();
                  StopAutoListenTimer();
                  StopPollingTimer();
		
		GetContext().getContentResolver().unregisterContentObserver( mSettingsObserver);
		GetContext().getContentResolver().unregisterContentObserver( mDevNameObserver);		
    }
    /** register the BroadcastReceiver with the intent values to be matched */    
		
	public void resume()
	{	
		Log.d(TAG,"resume");		
		StopCountdownTimer();

		//removePersistentGroup();
		Log.d(TAG,"not to remove persistentgroup on resume()!!");	
		strPIN = GetPIN();
		Settings.System.putString(context.getContentResolver(), Settings.System.REALTEK_SETUP_MIRACAST_SINK_PIN_CODE, strPIN);
		Intent i = new Intent("rtk.provider.miracastsink.SETTING");
		i.putExtra("WFDPIN", strPIN);
		sendBroadcast(i);
		
		//PIN = "12345670";
		Log.d(TAG, "PIN:" + strPIN);
		//myPin.setText(String.format(getString(R.string.wifi_wps_pin), PIN));		
		//mPinCode.setText("PBC/PIN:"+PIN);
		String myMac = getMacAddress();

	    	strDeviceName = Settings.Global.getString(context.getContentResolver(), Settings.Global.DEVICE_NAME);
    		Log.d(TAG, "WFD name:" + strDeviceName);
		//mState.setText(GetStateString());
    }
	
	String getDeviceName() {
		return strDeviceName;
	}
	
	String getMacAddress() {
        WifiManager mWifiManager = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInf = mWifiManager.getConnectionInfo();
        return wifiInf.getMacAddress();
	}
	    
    
    /**
     * Remove all peers and clear all fields. This is called on
     * BroadcastReceiver receiving a state change event.
     */
    public void resetData() {
		//setIsWifiP2pEnabled(false);
        if (mDeviceList != null) {
            mDeviceList.clearPeers();
        }
        if (mDeviceDetail != null) {
            //fragmentDetails.resetViews();
        }
    }

	/*
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	// add menu MiracastTest
        menu.add(Menu.NONE, MENU_ID_MIRACAST, Menu.NONE, R.string.miracast_list);
        // add menu settings 
        menu.add(Menu.NONE, MENU_ID_SETTING, Menu.NONE, R.string.miracast_setting);
        return super.onCreateOptionsMenu(menu); 
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item){
    	switch (item.getItemId()) {
    	case MENU_ID_MIRACAST:
    		Intent intent = new Intent(this, MiracastTestList.class);
    		startActivity(intent);
    		return true;
    	case MENU_ID_SETTING:
    		return true;
    	default:
    		return false;
    	}
    }
	*/

    public void showDetails(WifiP2pDevice device) {
    }

    public void connect(WifiP2pConfig config) {
        manager.connect(channel, config, new ActionListener() {

            @Override
            public void onSuccess() {
                // WiFiDirectBroadcastReceiver will notify us. Ignore for now.
            }

            @Override
            public void onFailure(int reason) {
                //Toast.makeText(MainActivity.this, "Connect failed. Retry.",Toast.LENGTH_SHORT).show();
                Log.d(TAG, "Connect failed. Retry.");
            }
        });
    }

	public void cancelDisconnect() {

        /*
         * A cancel abort request by user. Disconnect i.e. removeGroup if
         * already connected. Else, request WifiP2pManager to abort the ongoing
         * request
         */
        Log.d(TAG, "cancelDisconnect 0.");
        if (manager != null) {
            if (mDeviceList.getDevice() == null
                    || mDeviceList.getDevice().status == WifiP2pDevice.CONNECTED) {
                disconnect();
				  Log.i(TAG, "mDeviceList.getDevice() == null: " );
            } else if (mDeviceList.getDevice().status == WifiP2pDevice.AVAILABLE
                    || mDeviceList.getDevice().status == WifiP2pDevice.INVITED) {

                manager.cancelConnect(channel, new ActionListener() {

                    @Override
                    public void onSuccess() {
                        //Toast.makeText(MainActivity.this, "Aborting connection",
                        //        Toast.LENGTH_SHORT).show();
                        Log.i(TAG, "Aborting connection");
                    }

                    @Override
                    public void onFailure(int reasonCode) {
                        //Toast.makeText(MainActivity.this,
                        //        "Connect abort request failed. Reason Code: " + reasonCode,
                        //        Toast.LENGTH_SHORT).show();
                        Log.i(TAG, "Connect abort request failed. Reason Code: " + reasonCode);
                    }
                });
            }
        }
    }

	@Override
    public void onChannelDisconnected() {
        // we will try once more        
		if (manager != null && !retryChannel) {
            //Toast.makeText(this, "Channel lost. Trying again", Toast.LENGTH_LONG).show();
            Log.d(TAG,"Channel lost. Trying again.");
            resetData();
            retryChannel = true;
            //manager.initialize(context, context.getMainLooper(), null);
            channel = manager.initialize(context, context.getMainLooper(), null);
        } else {
            //Toast.makeText(this,
            //        "Severe! Channel is probably lost premanently. Try Disable/Re-Enable P2P.",
            //        Toast.LENGTH_LONG).show();
            Log.d(TAG,"Severe! Channel is probably lost premanently. Try Disable/Re-Enable P2P.");
        }
	}

	public void getPeerAddress()
	{
		new Thread(new Runnable(){
			@Override
			public void run(){
	            String Address= null;
				String port = null;
				String ip = null;
				String name = null;
				int count = 0;

			    if(mCurConnectedDevice!=null)
			    {
					name = mCurConnectedDevice.deviceName;
					Address=mCurConnectedDevice.deviceAddress;
					Log.i(TAG, "Device device kavin getIpAddress is: " + Address);   
			    }

				if(mCurConnectedDevice != null && mCurConnectedDevice.wfdInfo != null){
					if(mCurConnectedDevice.wfdInfo.getControlPort() > 0)
					{
						port = String.valueOf(mCurConnectedDevice.wfdInfo.getControlPort());						
					}
					else {
						Log.i(TAG, "port=0, force to: 7236");
						port = "7236";
					}
				}
				Log.i(TAG, "Device port is: " + port);				
			    
				if(mDeviceDetail.connectionInfo.isGroupOwner)
				{
					while((curState == WFD_STATE_CONNECTED)){
						//ip = mDeviceDetail.thisDevice.getPeerIPAddr(Address);
						//ip = getPeerIPAddr(Address);
						ip = getPeerIPAddr2(Address);
						if(ip != null) 
							break;
						try{
							Thread.sleep(800);
							//count++;
						}catch(InterruptedException e){
							e.printStackTrace();
						}
					}
				} else {
					ip = mDeviceDetail.connectionInfo.groupOwnerAddress.getHostAddress();
				}				
				Log.i(TAG, "Device ip is: " + ip);
				Log.i(TAG, "Device name is: " + name);
				
				if(ip != null && port != null)
				{
					//out put ip,port to /tmp
				
					try{
        				FileWriter fw = new FileWriter("/cache/port.log", false);
        				BufferedWriter bw = new BufferedWriter(fw); 
        				bw.write(port);
        				//bw.newLine();
        				bw.close();
    				}catch(IOException e){
    					e.printStackTrace();
       					Log.i(TAG, "write port.log fail.");
    				}
    				try{
        				FileWriter fw = new FileWriter("/cache/p2pip.result", false);
        				BufferedWriter bw = new BufferedWriter(fw); 
        				bw.write(ip);
        				//bw.newLine();
        				bw.close();
    				}catch(IOException e){
    					e.printStackTrace();
       					Log.i(TAG, "write /tmp/p2pip.result fail.");
    				}
    				//stop discover
    				StopCountdownTimer();
    				//onPause();
					String url = "miracast://" + ip + ":" + port;
					Log.i(TAG, "Device URL: " + url);
					Message msg=new Message();
					msg.what=WFD_URL;
					Bundle b =new Bundle();
					b.putString("URL", url);
					b.putString("device", name);
					msg.setData(b);
					mDeviceDetail.mHandler.sendMessage(msg);
				}
				/*else {
					Log.i(TAG, "No IP, exit getPeerAddress.");
					wfdreset();					
				}*/
			}
		}).start();
		
		Log.i(TAG, "after reading ip thread start()");
    }

    
    
    /*added by kavin*/
    public void onPersistentGroupInfoAvailable(WifiP2pGroupList groups) {        
       grouplist=groups;
	   Log.d(TAG, "onPersistentGroupInfoAvailable: " + groups);
	   if(grouplist!=null)
	   {
			   for(WifiP2pGroup group:grouplist.getGroupList()) {
					   Log.d(TAG, "GROUP: " + group.getNetworkName() + " - " + group.getPassphrase());
			   }
	   }
    }  
    public void removePersistentGroup()
    {
		Log.i(TAG, "removePersistentGroup");
        if(grouplist!=null)
        {
			int size = grouplist.getGroupList().size();
            Log.d(TAG, "grouplist size is :" + size);
			if(size == 0) {
			   Message msg=new Message();
			   msg.what=WFD_CREATEGROUP;
			   mDeviceDetail.mHandler.sendMessage(msg);
			}
			int count = 0;
			for(WifiP2pGroup group:grouplist.getGroupList()) {
				if(++count < size) {
            	manager.deletePersistentGroup(channel,group.getNetworkId(),
                            new WifiP2pManager.ActionListener() {
                                 public void onSuccess() {
                                      Log.d(TAG, " delete groupPersistent success");
                                 }
                                 public void onFailure(int reason) {
                                      Log.d(TAG, " delete groupPersistent fail " + reason);
                                 }
                             });
				}
				else {
            	manager.deletePersistentGroup(channel,group.getNetworkId(),
                            new WifiP2pManager.ActionListener() {
                                 public void onSuccess() {
                                      Log.d(TAG, "LAST delete groupPersistent success");
									  Message msg=new Message();
									  msg.what=WFD_CREATEGROUP;
									  mDeviceDetail.mHandler.sendMessage(msg);
                                 }
                                 public void onFailure(int reason) {
                                      Log.d(TAG, "LAST delete groupPersistent fail " + reason);
									  Message msg=new Message();
									  msg.what=WFD_CREATEGROUP;
									  mDeviceDetail.mHandler.sendMessage(msg);
                                 }
                             });
				}
			}
       } else {
               Log.d(TAG, "NO delete groupPersistent");
			   Message msg=new Message();
			   msg.what=WFD_CREATEGROUP;
			   mDeviceDetail.mHandler.sendMessage(msg);
	   }
    }

	public void createNewGroup() {
			Log.i(TAG, "createNewGroup");
			manager.createGroup(channel, new ActionListener() {    	
				@Override
				public void onFailure(int reasonCode) {
					Log.d(TAG, "GroupCreate failed. Reason :" + reasonCode);
					Message msg=new Message();
					msg.what=WFD_PINANY;
					mDeviceDetail.mHandler.sendMessageDelayed(msg,1000);
				}

				@Override
				public void onSuccess() {
					Log.d(TAG, "GroupCreate success");
					Message msg=new Message();
					msg.what=WFD_PINANY;
					mDeviceDetail.mHandler.sendMessageDelayed(msg,1000);
				}
			});
	}

	public void SetP2PConfig () {
		Log.i(TAG, "SetP2PConfig");
			// start pin any
			WifiP2pConfig config = new WifiP2pConfig();
			config.groupOwnerIntent = 0;
			config.wps.setup = WpsInfo.PBC;
			

			manager.connect(channel, config, new ActionListener() {
				@Override
				public void onSuccess() {
					Log.d("Tandy", "SetP2PConfig");
				}
				@Override
				public void onFailure(int reason) {
					Log.d("Tandy", "SetP2PConfig failed :" + reason);
				}
			});
	}
	public void pinAny() {
			Log.i(TAG, "pinAny");
			// start pin any
			WifiP2pConfig config = new WifiP2pConfig();
			config.wps.setup = WpsInfo.DISPLAY;
			curState = WFD_STATE_READY;

			Log.d(TAG, "start pin any");
			manager.connect(channel, config, new ActionListener() {
				@Override
				public void onSuccess() {
					Log.d(TAG, "start pin any success");
				}
				@Override
				public void onFailure(int reason) {
					Log.d(TAG, "start pin any failed :" + reason);
				}
			});
	}

	public void setListenMode(final boolean enable) {
        Log.i(TAG, "setListenMode: "+enable);
        manager.listen(channel, enable, new ActionListener() {
            @Override
            public void onSuccess() {
                Log.i(TAG, "Successfully " + (enable ? "entered" : "exited")
                            +" listen mode.");
            }

            @Override
            public void onFailure(int reason) {
                Log.i(TAG, "Failed to " + (enable ? "entered" : "exited")
                        +" listen mode with reason " + reason + ".");
            }
        });
    }    
	

	public void wfdreset() {
		Log.i(TAG, "WFD_RESET");
		Message msg=new Message();
		msg.what=WFD_RESET;
		mDeviceDetail.mHandler.sendMessage(msg);
	}
	
	public void retry(int TimeoutCause)
	{
	        Semaphore RetryLock = new Semaphore(1);
                             
                 try {
                            RetryLock.acquire();
                            
                    	Log.d(TAG, "retry() TimeoutCause =" + TimeoutCause  + ", mP2pCountdownTimeout= " + mP2pCountdownTimeout );
            		StopCountdownTimer();
            		cancelDisconnect();
            		SetState(WFD_STATE_DISCONNECTED);
            		//
            		//TODO: if getipaddress is running, need wait thread exit.
            		//
            		mCurConnectedDevice = null;										
            		p2pdevicelost=0;
            		resume();	

                            if( TimeoutCause == RETRY_TIMEOUT && mP2pCountdownTimeout == RETRY_CONNECTION_TIMEOUT)
                            {
                                // Workaround!!
                                // In 40 seconds timeout case (driver do not response), we should not call P2P_LISTEN this time.
                                // cancelDisconnect()[P2P_CANCEL] will cause NetworkInfo.DetailedState.FAILED in MircastBroadcastReceiver.onReceive.
                                // Sink will call retry again there.
                                
                                // To avoid P2P_LISTEN happend earlier than WLAN0 RECONNECT in wifiP2pService.
                                Log.d(TAG, "Timeout for p2p connect for 40 seconds, skip trigger P2P_LISTEN this time");
                            }
                            else
                            {
                                    Log.d(TAG, "retry,isWifiP2pEnabled:"+isWifiP2pEnabled);
                    		setIsWifiP2pEnabled(isWifiP2pEnabled);	
                            }                   
                    } catch (Exception e) {
                            Log.d(TAG, "retry exception");
                            RetryLock.release();
                    } finally {
                            Log.d(TAG, "retry finally");
                            RetryLock.release();
                    }
                    
	}

	void ready()
	{
		Log.i(TAG, "ready.");
		curState = WFD_STATE_INIT;
		resume();
		/*
	     AlertDialog isEnter = new AlertDialog.Builder(this).create();  
         isEnter.setCanceledOnTouchOutside(false);
         isEnter.setMessage(getString(R.string.Ready_dialog));  
         isEnter.setButton(getString(R.string.no), new DialogInterface.OnClickListener() {		   
					public void onClick(DialogInterface dialog, int id) {					
                        finish(); 
                        return;						
					}	   
					});
		isEnter.setButton2(getString(R.string.yes),new DialogInterface.OnClickListener() { 		   
				public void onClick(DialogInterface dialog, int id) { 
                   resume();			   
				 mWpsTextView.setText(R.string.Ready); 
                 return;					 
				 }	   
				});
        
         isEnter.show();
		 */

		/*final AlertDialog isEnter = new AlertDialog.Builder(this).create();
		isEnter.setCanceledOnTouchOutside(false);
		isEnter.show();
		Window window = isEnter.getWindow();
		window.setContentView(R.layout.custom_dialog);
		TextView messageTextView = (TextView) window.findViewById(R.id.message);
		messageTextView.setText(getString(R.string.Ready_dialog));
		Button mButtonOk = (Button) window.findViewById(R.id.button_yes);  
		mButtonOk.setOnClickListener(new Button.OnClickListener()
				{   
					public void onClick(View v){ 
                        resume();			   
				        mState.setText("Ready to connect."); 
						isEnter.dismiss();    

					}   
				}); 
		Button mButtonNo = (Button) window.findViewById(R.id.button_no);  
		mButtonNo.setOnClickListener(new Button.OnClickListener()
				{   
					public void onClick(View v){ 
						finish();
					}   
				}); 
		*/
	}

	public void getRight2Left(Context cnt)
	{
		/*String language= cnt.getResources().getConfiguration().locale.getLanguage();
		if(language.equals("ar")||language.equals("iw")||language.equals("fa"))
			setContentView(R.layout.toshiba);
		else
		{
			setContentView(R.layout.wait);
		}*/
	}

	public void updateString() {
		//mState.setText(GetStateString());
	}

	String  GetPIN()
	{
		//int tmp [] = {1,2,3,4,5,6,7};
		int r=0;
		int snum = 0;
		int checksnum = 0;
		String pin = "";
		for (int i=0;i<7;i++) {

			r=(int)(Math.random()*10);
			//r = tmp[i];
			snum = snum*10+r;
			pin += r;
		}	

		checksnum = ComputeCheckSum(snum);
		return pin+checksnum;
	}

	int ComputeCheckSum ( int PIN)
	{
		long  acccum = 0;
		PIN *= 10;
		acccum += 3*((PIN/10000000)%10);
		acccum += 1*((PIN/1000000)%10);
		acccum += 3*((PIN/100000)%10);
		acccum += 1*((PIN/10000)%10);
		acccum += 3*((PIN/1000)%10);
		acccum += 1*((PIN/100)%10);
		acccum += 3*((PIN/10)%10);
		int digit = (int)(acccum %10);
		return ((10-digit)%10);
	}

	public void setDeviceName(String str) {
		Log.d(TAG, "setDeviceName :" + str);
		manager.setDeviceName(channel, str, new ActionListener() {
			@Override
			public void onSuccess() {
				Log.d(TAG, "setDeviceName ok");
			}   
			@Override
			public void onFailure(int reason) {
				Log.d(TAG, "setDeviceName failed :" + reason);
			}   
		}); 
	}
	
	int GetState()
	{
		//Log.d(TAG, "GetState: "+ curState);
		return curState;
	}
	
	void SetState(int iState)
	{
		//Log.d(TAG, "SetState: "+ iState);
		curState = iState;
	}
	
	String GetStateString()
	{
		String strvalue="";
		switch (curState) {
			case WFD_STATE_INIT:
				strvalue = "Please wait.";
				break;
			case WFD_STATE_READY:
				strvalue = "Ready to connect.";
				break;
			case WFD_STATE_CONNECTING:
				strvalue = "Source connecting...";
				break;
			case WFD_STATE_CONNECTED:
				strvalue = "Source connected.";
				break;
			case WFD_STATE_DISCONNECTED:
				strvalue = "Source disconnected.";
				break;
		}
		Log.d(TAG, "curState="+curState);
		return strvalue;
	}
	
	public String getPeerIPAddr2(String Address){//tandy: works well!
		Log.i(TAG, "Trying to read ip from /proc/net/arp which match "+Address);
		//StringBuffer echo = new StringBuffer();
		String ip=null;  
  		try {
    		BufferedReader br = new BufferedReader(new FileReader("/proc/net/arp"));
    		String line = "";
    		while((line = br.readLine()) != null) {
    			Log.i(TAG, "line: "+line);
    			ip = null;
			if (line.contains("p2p0")) {
    				//if (line.contains(Address)) {//nexu4 will change address
    					String[] aArray = line.split(" "); 
    					for (String d : aArray) {
    						Log.i(TAG, "line, split to: "+d);//the first is ip address
    						ip=d; 
    						break;
    					} 
    				//}
    			}
    			if (ip!=null) {
    				break;
    			}
    			//echo.append(line + "\n");
    		}
    		br.close();
  		}
  		catch(Exception e) { 
  			Log.e(TAG, e.toString()); 
  		}
  		return ip;
	}
		
	public String getPeerIPAddr(String Address){//this just support JB.
		Log.i(TAG, "Trying to read peer IP from /tmp/dnsmasq.leases");
		int j,k;
		int count,same;
		String ip=null;  
		FileReader fileReader = null;
		//since we cannot access /data/misc/dhcp/dnsmasq.leases, copy to tmp and chmod 777
		//doCmds("cp /data/misc/dhcp/dnsmasq.leases /tmp");
		//chmod 777
		//doCmds("chmod 777 /tmp/dnsmasq.leases");		
		//read file
		try {
			//fileReader = new FileReader("/data/misc/dhcp/dnsmasq.leases");			
			fileReader = new FileReader("/tmp/dnsmasq.leases");
		}
		catch(Exception e) { 
  			Log.e(TAG, e.toString()); 
  			return null;
  		}
		char data[] = new char[1024];
		int num;
		try{
			num = fileReader.read(data);
		}catch(IOException e){
			Log.i(TAG,"FileReader cannot read the file !");
		}
		try{
			fileReader.close();
		}catch(IOException e){
			Log.i(TAG,"FileReader close fail");
		}
		String str =new String(data);
		String[] ss=str.split("\n");
		if(ss.length < 2)
		{
				Log.i(TAG,"we can not find any line");
				return null;
		}
		if(Address==null)
		{
			return null;                     
		}
		else
		{
		         // ip  remember the return ip address
			String beginip="192.168.49."; //hotspot begin with 192.168.43, wifidirect begin with 192.168.49."
			Log.i(TAG, "kavin length:" + ss.length);
			String[] MacAddress= Address.split(":");
			same=0;
			for(j=0;j<ss.length-1;j++)
			{ 
				Log.i(TAG, "kavin dnsmasq line j line:" + j + ss[j]);
			    count=0;			
			    String[] mactokens = ss[j].split(" ");
			    Log.i(TAG, "mactokens is " + mactokens[0]+mactokens[1]+mactokens[2]);
			    Log.i(TAG, "MacAddress is"+ MacAddress[0]+MacAddress[1]+MacAddress[2]+MacAddress[5]);
				if(mactokens[2].contains(beginip))
				{
					Log.i(TAG, "mactokens contains ip kavin " + mactokens[2]);
				    String[] mac=mactokens[1].split(":");
					for(k=0;k<6;k++)
						if(MacAddress[k].equals(mac[k]))
							count++;
					Log.i(TAG, "count is  kavin " + count);
					if(count>same)
					{
						same=count;
						ip=mactokens[2];
						Log.i(TAG, "kavin count and ip kavin is:" + count + ip);
					}
				}
			}
			if(same<4)
				return null;
			else return ip;
		}				
    }		

	//no use
	public boolean doCmds(String cmds) {
		Log.i(TAG, "doCmds: "+cmds);
		try { 
    		Process process = Runtime.getRuntime().exec("su");
    		DataOutputStream os = new DataOutputStream(process.getOutputStream());

    	//for (String tmpCmd : cmds) {
        	os.writeBytes(cmds+"\n");
    	//}

    		os.writeBytes("exit\n");
    		os.flush();
    		os.close();

    		process.waitFor();
    		return true;
    	}catch(IOException e){
			Log.i(TAG,"doCmds fail 0 :"+cmds);
			return false;
		}catch(InterruptedException e){
			Log.i(TAG,"doCmds fail 1:"+cmds);
			return false;
		}
	}
	
	/*class WpsListener implements WifiManager.WpsListener {
            public void onStartSuccess(String pin) {
                
            }
            public void onCompletion() {
                
            }

            public void onFailure(int reason) {
                
            }
        }        
	*/
	
	public void setWifiP2pChannels(final int lc, final int oc) {
        Log.i(TAG,"Setting wifi p2p channel: lc=" + lc + ", oc=" + oc);
        manager.setWifiP2pChannels(channel,
                lc, oc, new ActionListener() {
            @Override
            public void onSuccess() {
                    Log.i(TAG,"Successfully set wifi p2p channels.");
            }

            @Override
            public void onFailure(int reason) {
            	Log.i(TAG, "Failed to set wifi p2p channels with reason " + reason + ".");
            }
        });
    }
    
    /*
    private boolean isTopActivity(String packageName)
	{
    	ActivityManager __am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
    	List<ActivityManager.RunningAppProcessInfo> __list = __am.getRunningAppProcesses();
    	if(__list.size() == 0) return false;
    	for(ActivityManager.RunningAppProcessInfo __process:__list)
    	{
        	//Log.d(getTAG(),Integer.toString(__process.importance));
        	//Log.d(getTAG(),__process.processName);
        	Log.i(TAG, "process: "+__process.processName); 
        	if(__process.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND &&
                __process.processName.equals(packageName))
        	{
            	return true;
        	}
    	}
    	return false;
	}
	
    public boolean WaitActivityExec(String packageName)
	{   
		Log.i(TAG, "WaitActivityExec"); 
		while (true) {
    		//if (getPackageManager().resolveActivity(it, 0) != null) {
    		if (isTopActivity(packageName)) {
    			Log.i(TAG, "WaitActivityExec 1"); 
    			break;
    		}
    		else {
    			Log.i(TAG, "WaitActivityExec 2"); 
    			try{
					Thread.sleep(1000);//wait 5 secs
				}catch(InterruptedException e){
					e.printStackTrace();
				}
    		}
    	}
    	return true;
	}
	*/			
	/* core class:
		setIsWifiP2pEnabled
		resetData
		mDeviceList
		GetState
		mDeviceDetail
		SetState
		mCurConnectedDevice
		sendBroadcast
		retry(
	*/
		
		

	public void setIsWifiP2pEnabled(boolean isWifiP2pEnabled) {
		this.isWifiP2pEnabled = isWifiP2pEnabled;
		if(isWifiP2pEnabled) {
			manager.setMiracastMode_AP( WifiP2pManager.MIRACAST_SINK );
			//WFDDiscover();
			WFDListen();
			//setListenMode(true);//work!
		}
		else
		{
			Log.d(TAG, "Try to stopPeerDiscovery.");
			if(manager != null){
				manager.setMiracastMode_AP( WifiP2pManager.MIRACAST_DISABLED );
				manager.stopPeerDiscovery( channel, null);
			} else{
				Log.d(TAG, "WifiP2pManager is null");
			}
			//setListenMode(false);//work!
		}
		updateWfdEnableState(isWifiP2pEnabled);

	}
	
	public boolean getIsWifiP2pEnabled() {
		return this.isWifiP2pEnabled;
	}
    	
    	public void updateWfdEnableState(boolean isEnable) {
    		Log.i(TAG,"updateWfdEnableState, isEnable:" + isEnable);
				if (isEnable) {
					setDeviceName(strDeviceName);
					// WFD should be enabled.
					if (!mWfdEnabled && !mWfdEnabling) {
                	mWfdEnabling = true;
	                //set p2pconfig
    	            //SetP2PConfig();
        	        //
					manager.setMiracastMode(WifiP2pManager.MIRACAST_SINK);
					setWifiP2pChannels(mListenChannel, mOperatingChannel);

                	WifiP2pWfdInfo wfdInfo = new WifiP2pWfdInfo(0x0100,DEFAULT_CONTROL_PORT,MAX_THROUGHPUT);
                	wfdInfo.setWfdEnabled(true);
	                wfdInfo.setDeviceType(WifiP2pWfdInfo.PRIMARY_SINK);
    	            wfdInfo.setSessionAvailable(true);
        	        wfdInfo.setControlPort(DEFAULT_CONTROL_PORT);
            	    wfdInfo.setMaxThroughput(MAX_THROUGHPUT);
                	manager.setWFDInfo(channel, wfdInfo, new ActionListener() {
                    	@Override
                    	public void onSuccess() {
							Log.i(TAG, "Successfully set WFD info.");
                        	if (mWfdEnabling) {
                            	mWfdEnabling = false;
                            	mWfdEnabled = true;
                            	//reportFeatureState();
	                        }
    	                }

        	            @Override
            	        public void onFailure(int reason) {
                	        Log.i(TAG, "Failed to set WFD info with reason " + reason + ".");
                    	    mWfdEnabling = false;
                    	}
                	});
            	}
        	} else {
            	// WFD should be disabled.
            	StopCountdownTimer();
            	mWfdEnabling = false;
							mWfdEnabled = false;
							if(manager != null){
								WifiP2pWfdInfo wfdInfo = new WifiP2pWfdInfo();
								wfdInfo.setWfdEnabled(false);
								wfdInfo.setDeviceType(WifiP2pWfdInfo.PRIMARY_SINK);
								wfdInfo.setSessionAvailable(true);
								wfdInfo.setControlPort(DEFAULT_CONTROL_PORT);
								wfdInfo.setMaxThroughput(MAX_THROUGHPUT);
								manager.setWFDInfo(channel, wfdInfo, new ActionListener() {
									@Override
									public void onSuccess() {
										Log.i(TAG, "Successfully set WFD info.");
										if (mWfdEnabling) {
											mWfdEnabling = false;
											mWfdEnabled = true;
											//reportFeatureState();
										}
									}

									@Override
									public void onFailure(int reason) {
										Log.i(TAG, "Failed to set WFD info with reason " + reason + ".");
										mWfdEnabling = false;
									}
									});
									//reportFeatureState();
									disconnect();
							}
        	}
    	}
    	
    	public void WFDDiscover() {
    		Log.e(TAG, "WFDDiscover");
			manager.discoverPeers(channel, new WifiP2pManager.ActionListener() {
				@Override
				public void onSuccess() {
					Log.i(TAG, "Discovery Initiated");
				
				}

				@Override
				public void onFailure(int reasonCode) {
					Log.e(TAG, "Discovery Failed");
				}
			});
			//120 secs timer
			StartCountDownTimer(RETRY_NORMAL_TIMEOUT);
		}
		
		public void WFDListen() {
    		Log.e(TAG, "WFDListen");
					
			manager.ListenPeers(channel, new WifiP2pManager.ActionListener() {
			//manager.listen(channel,true, new WifiP2pManager.ActionListener() {
				@Override
				public void onSuccess() {
					Log.i(TAG, "Listen Initiated");
				
				}

				@Override
				public void onFailure(int reasonCode) {
					Log.e(TAG, "Listen Failed");
				}
			});
			//120 secs timer
			StartCountDownTimer(RETRY_NORMAL_TIMEOUT);
		}
		
		public void disconnect() {
        //fragment.resetViews();        
        	Log.d(TAG, "disconnect, stop wfd process 0.");
        //SystemProperties.set("ctl.stop", "wfd");
        	manager.removeGroup(channel, new ActionListener() {

            @Override
            	public void onFailure(int reasonCode) {
                	Log.d(TAG, "Disconnect failed. Reason :" + reasonCode);

            	}

            @Override
            	public void onSuccess() {
                	Log.d(TAG, "remove Group is OK");
            	}

        	});
    	}
    	
	void sendBroadcast (Intent intent)
	{
		context.sendBroadcast(intent);
	}

	private final ContentObserver mSettingsObserver = new ContentObserver(new Handler()) {
        @Override
        public void onChange(boolean selfChange, Uri uri) {		
		final boolean toggle = Settings.System.getInt( GetContext().getContentResolver(),  Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON, 0) != 0;
		Log.d( TAG, "Content Observer listen something, toogle = " + toggle + ", isWifiP2pEnabled = " + isWifiP2pEnabled );
		
		if( isWifiP2pEnabled != toggle )
		{
			setIsWifiP2pEnabled( toggle);
		}		
        }
    };

    private final ContentObserver mDevNameObserver = new ContentObserver(new Handler()) {
        @Override
        public void onChange(boolean selfChange, Uri uri) {		
		Log.d( TAG, "Content Observer detect device name change!" );

	    	strDeviceName = Settings.Global.getString(context.getContentResolver(), Settings.Global.DEVICE_NAME);
    		Log.d(TAG, "Change WFD device name to:" + strDeviceName);
		setDeviceName(strDeviceName);		
        }
    };
	
}//p2pcore
