
package com.example.RTKMiracastSink;


import java.lang.ref.WeakReference;
import java.util.UUID;
import java.io.File;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.net.Uri;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.WifiP2pManager.Channel;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Bundle;
import android.provider.MediaStore;
import android.provider.Settings;
import android.util.Log;
import java.util.Timer;
import java.util.TimerTask;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;



public class MiracastSinkService extends Service {	
    private static String TAG = "MiracastSinkService";
    /*public static final int WFD_STATE_INIT = 0;
	public static final int WFD_STATE_READY = 1;
	public static final int WFD_STATE_CONNECTING = 2;
	public static final int WFD_STATE_CONNECTED = 3;
	public static final int WFD_STATE_DISCONNECTED = 4;
	*/
	//private int curState = WFD_STATE_INIT;
	
	private WifiManager mWifiManager;
	//private WifiP2pManager manager;
	//private Channel channel;
	//public String strPIN = null;
	//private String strDeviceName = null;
	CP2PCore p2pcore = null;

         private static final boolean Sigma_enabled = false;
         private Timer SigmaPollingTimer = null;
         SigmaPollingTimerTask SigmaPollingTimerTask = null;
         private boolean Sigma_rtk_wfd_launched = false;
	
    //private static String SharedPreferencesName = "MiracastSinkService";        
    
    private BroadcastReceiver pbBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
        	final String action = intent.getAction();
  			if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
    			int extraWifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE , WifiManager.WIFI_STATE_UNKNOWN);
    			switch(extraWifiState){
    				case WifiManager.WIFI_STATE_DISABLED:
    					Log.d(TAG, "WIFI STATE DISABLED");
    					if (p2pcore!=null) {
    						p2pcore.cancelDisconnect();
								p2pcore.onPause();
    						p2pcore.onDestroy();
    					}
    					break;
    				case WifiManager.WIFI_STATE_DISABLING:
    					Log.d(TAG, "WIFI STATE DISABLING");
    					break;
    				case WifiManager.WIFI_STATE_ENABLED:
    					Log.d(TAG, "WIFI STATE ENABLED");
    					if (p2pcore!=null) {
    						p2pcore.OnCreate();
    						p2pcore.OnResume();
							Log.d(TAG, "setIsWifiP2pEnabled(true)"); 
							p2pcore.setIsWifiP2pEnabled(true);
    					}
    					break;
    				case WifiManager.WIFI_STATE_ENABLING:
    					Log.d(TAG, "WIFI STATE ENABLING");
    					break;
    				case WifiManager.WIFI_STATE_UNKNOWN:
    					Log.d(TAG, "WIFI STATE UNKNOWN");
    				break;
    			}
  		}
                else if(action.equals("rtk.miracast.sigma.EVENT"))
                    {
                    /*
               Sigma tool will trigger <<am broadcast --user 0 -a "rtk.miracast.sigma.EVENT" -e "VideoView" "ON" -e "IP" "192.168.100.100" -e "PORT" "1234">>               
               MiracastSinkService will retrieve IP/Port infomation then pass to VideoViewActivity.
               Finally, VideoViewActivity will pass all the sigma information to rtk_wfd.
              */
                        String VideoView = intent.getStringExtra("VideoView");
                        if(VideoView.equalsIgnoreCase("ON"))
                            {
                                Log.d(TAG, " Receive rtk.miracast.sigma.EVENT, Video view is ON");

                                String s_IP = intent.getStringExtra("IP");
                                String s_Port = intent.getStringExtra("PORT");
                                Log.d(TAG, "[Sigma] IP= " + s_IP + "PORT= " + s_Port );
                                
            		     ComponentName componetName = new ComponentName("com.example.RTKMiracastSink",
            									"com.example.RTKMiracastSink.VideoViewActivity");
            		     Intent it= new Intent();
                                Bundle bundle = new Bundle();
                                bundle.putInt("sigma", 1);
                                bundle.putString("ip", s_IP);
                                bundle.putString("port", s_Port);
                                it.putExtras(bundle);
            		     it.setComponent(componetName);
            		     it.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            		     context.startActivity(it);
                            }
                        else if(VideoView.equalsIgnoreCase("OFF"))
                            {
                                Log.d(TAG, "[Collin] Receive rtk.miracast.sigma.EVENT, Video view is OFF");
                                
                                Intent kill = new Intent("MediaPlayerKill");
                                context.sendBroadcast(kill);
                            }
                        else
                            {
                                Log.d(TAG, "Receive rtk.miracast.sigma.EVENT, Unknown command");
                                
                            }                        
                    }          
        }
    };
    private static class InternalHandler extends Handler {
        private final WeakReference<MiracastSinkService> mMiracastSinkService;

        public InternalHandler(MiracastSinkService service) {
            mMiracastSinkService = new WeakReference<MiracastSinkService>(service);
        }

        @Override
        public void handleMessage(Message msg) {
            MiracastSinkService service = mMiracastSinkService.get();
            if (service == null)
                return;
            Log.d(TAG, "mHandler : Get the message " + msg.what);
            switch (msg.what)
            {
                default:
                    break;
            }
        }
    }

    private final InternalHandler mHandler = new InternalHandler(this);

    @Override
    public IBinder onBind(Intent arg0) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void onCreate() {
    	super.onCreate();
        Log.d(TAG, "The onCreate is called.");
        IntentFilter intentFilter = new IntentFilter();
    	intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
            	intentFilter.addAction("rtk.miracast.sigma.EVENT");
		registerReceiver(pbBroadcastReceiver, intentFilter);
		//
		mWifiManager = (WifiManager)getSystemService(Context.WIFI_SERVICE);
		if (p2pcore==null) {
			p2pcore = CP2PCore.GetInstance();
			p2pcore.SetContext(this);
		}
		
		//If we play GO role, need to start DHCP server
		//It need to access /data/misc/dhcp/dnsmasq.leases (Sometimes, dhcp folder will not be created)
		//Need to reference to #define LEASEFILE "/data/misc/dhcp/dnsmasq.leases" in dnsmasq/src/config.h
		File dhcp_dir = new File("/data/misc/dhcp");
		if(!dhcp_dir.exists())
		{
			Log.d(TAG, " /data/misc/dhcp is NOT existed, create for it!!!");
			dhcp_dir.mkdir();
		}

                  if( Sigma_enabled == true)
                    {
                        // Create sigma polling timer to read the IP/Port info created by WIFI sigma tool
                        Log.d(TAG, "Sigma is enabled, start SigmaPollingTimerTask");
                        
                        SigmaPollingTimerTask = new SigmaPollingTimerTask();
                        SigmaPollingTimer = new Timer();
                        
                        //Start a regular 500ms timer for 5 seconds later
                        SigmaPollingTimer.schedule( SigmaPollingTimerTask, 3000, 1000);
                    }
    }

    
    @Override
    public void onDestroy() {
		super.onDestroy();
    	Log.d(TAG, "The onDestroy is called."); 
    	if (p2pcore!=null) {
				Log.d(TAG, "setIsWifiP2pEnabled(false) and resetData()"); 
				p2pcore.setIsWifiP2pEnabled(false);
				p2pcore.resetData();
				//p2pcore.cancelDisconnect();
				p2pcore.onPause();
				p2pcore.onDestroy();
			}
	unregisterReceiver(pbBroadcastReceiver);

         if( Sigma_enabled == true)
        {
                if(SigmaPollingTimer!=null)
                {
                   SigmaPollingTimer.cancel();
                   SigmaPollingTimer = null;
                   SigmaPollingTimerTask = null;
                }
        }
		//android.os.Process.killProcess(android.os.Process.myPid());
    }

    @Override
    public void onStart(Intent intent, int startId) {
        Log.d(TAG, "The onStart is called");
    }       

    class SigmaPollingTimerTask extends TimerTask {
            public void run() {

                    if( Sigma_rtk_wfd_launched == false )
                        {
                            //RTK_WFD is not existed, try to polling for IP/PORT info from Sigma tool
                            // Sigma tool should echo IP/PORT to /tmp/sigma_ip_info
                            String filePath = "/tmp/sigma_ip_info";
                            File file = new File( filePath );
                           String s_IP = null;
                           String s_Port = null;

                            if( file.exists())
                            {
                                Log.d(TAG, "[Sigma] File is  existed: " + filePath);
                                BufferedReader br = null;
                                String line = "";
                                try {
                                    br = new BufferedReader(new FileReader( filePath ));
                                    line = br.readLine();
                                    if( line != null)
                                    {
                                        String[] aArray = line.split(" ");
                                        s_IP = aArray[0];
                                        s_Port = aArray[1];
                                    }
                                } 
                                catch ( Exception e ) 
                                {
                                    Log.e(TAG, e.toString()); 
                                 }
                                finally {
                                    try {
                                            if( br!= null)
                                            {
                                                    Log.d(TAG, " File is closed" );
                                                   br.close();
                                            }
                                        }
                                    catch ( IOException e ) 
                                    {
                                        Log.d(TAG, "Something wrong when try to close file" );
                                    }
                                }                                                                

                                Log.d(TAG, "[Sigma] IP= " + s_IP + "PORT= " + s_Port );

                                if( s_IP != null && s_Port != null)
                                {

                                ComponentName componetName = new ComponentName("com.example.RTKMiracastSink",
                                                       "com.example.RTKMiracastSink.VideoViewActivity");
                                Intent it= new Intent();
                               Bundle bundle = new Bundle();
                               bundle.putInt("sigma", 1);
                               bundle.putString("ip", s_IP);
                               bundle.putString("port", s_Port);
                               it.putExtras(bundle);
                                it.setComponent(componetName);
                                it.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                                startActivity(it);

                                Sigma_rtk_wfd_launched = true;
                                //remove the "/tmp/sigma_ip_info"

                                if( file.delete() == false)
                                {
                                    Log.d(TAG, "Delete file fail... : " +  filePath);
                                }

                                }
                                else
                                {
                                    Log.d(TAG, "IP or Port is null, can't launch rtk_wfd..." );
                                }

                            }
                            else
                            {
                                Log.d(TAG, "[Sigma] File is still not existed: " + filePath);
                            }
                        }
                    else
                        {
                            //RTK_WFD is launched by sigma, try to polling the close info from rtk_wfd process
                            String filePath = "/tmp/sigma_close_rtk_wfd";

                            File file = new File( filePath );

                            if( file.exists())
                                {
                                    Log.d(TAG, "[Sigma] rtk_wfd is closed ");
                                     Sigma_rtk_wfd_launched = false;
                                     
                                    //remove the "/tmp/sigma_close_rtk_wfd"
                                    if( file.delete() == false)
                                    {
                                        Log.d(TAG, "Delete file fail... : " +  filePath);
                                    }
                                }
                            else
                                {
                                    Log.d(TAG, "[Sigma] rtk_wfd is working, waiting close: ");
                                }
                            
                        }
                }
        }
}
