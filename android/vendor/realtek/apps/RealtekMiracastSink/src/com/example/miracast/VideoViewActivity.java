package com.example.RTKMiracastSink;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.View;
//import android.media.MediaPlayer;
import android.util.Log;
import android.os.Handler;
import android.os.Message;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.os.SystemProperties;
import java.io.IOException;
import android.provider.Settings;
import com.realtek.app.RTKMiracastSinkJNI;
import android.view.WindowManager;

public class VideoViewActivity extends Activity 
{
	private static final String TAG = "MiracastVideoView";
	private final IntentFilter intentFilter = new IntentFilter();
	private BroadcastReceiver receiver = null;
	private RTKMiracastSinkJNI MyJNI = new RTKMiracastSinkJNI(null);
	private CustomVideoView videoView = null;
	private int inbandready = 0;
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
                // Stop background music
		Intent it = new Intent("com.android.music.musicservicecommand");
		it.putExtra("command", "pause");
		sendBroadcast(it);
                   
		intentFilter.addAction("MediaPlayerKill");
		if (receiver == null) {
			receiver = new VideoViewBroadcastReceiver(this);
		Log.i(TAG, "new VideoPlayerBroadcastReceiver(this)");	
		}
		registerReceiver(receiver, intentFilter);
		
		//Move the create surface view to rtk_wfd.cpp

		setContentView(R.layout.activity_video_view);
		
		// play video stream
		Intent intent = getIntent();

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		/*videoView = (CustomVideoView)this.findViewById(R.id.VideoRTSP);
		Log.i(TAG, "VideoViewActivity 1");
		//videoView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
		videoView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
		//videoView.requestFocus();
		Log.i(TAG, "VideoViewActivity 2");
		videoView.setZOrderOnTop(true);
		Log.i(TAG, "VideoViewActivity 3");
		videoView.setVisibility(View.VISIBLE);
		//videoView.start();
		*/

        Bundle bundle = getIntent().getExtras();
        int bSigma = bundle.getInt("sigma");
        if( bSigma == 0)
		{
			// Full screen miracast
			int lcount = 0;

			SystemProperties.set("ctl.start", "wfd_auto");
			do {//need wait service ready
				inbandready = MyJNI.wfd_sink_GetReady();
				if (inbandready ==1) {
					break;
				}
				try{
					Thread.sleep(100);
				}catch(InterruptedException e){
					e.printStackTrace();
				}
			}while(lcount++< 50);
			//SystemProperties.set("ctl.start", "wfd:fixed 500 200 720 480");
		}
        else
		{
                String s_IP = bundle.getString("ip");
                String s_Port = bundle.getString("port");
                //Log.i(TAG, "VideoView is launched by sigma, bSigma= "+ bSigma);
                String s_rtk_wfd = "wfd:sigma "+s_IP+" "+s_Port;
                Log.i(TAG, "s_rtk_wfd= "+ s_rtk_wfd);

                SystemProperties.set("ctl.start", s_rtk_wfd);
		}

         Intent i = new Intent("com.android.action.FORCE_COLLAPSE_NOTIFICATION");
         sendBroadcast(i);
         //Log.i(TAG, "send out - FORCE_COLLAPSE_NOTIFICATION");
    	//startWFD();
    	// create thread to check rtsp		
	}
	
	/** register the BroadcastReceiver with the intent values to be matched */
	@Override
	public void onResume() {
		Log.i(TAG, "videoPlayerActivity->onResume()!");
		super.onResume();
		//disable mouse cursor
		Settings.System.putInt( getContentResolver(), Settings.System.SHOW_MOUSECURSOR, 0);
				
		// partial view test code here...
		/*if (inbandready == 1) {//need wait
			int x = 0,y=0 ,width=1920, height=1080,lcount=0;
			do {
				if (height>100) {
				   height -= 18;	width -= 32; x += 16; y += 9;
				}
				else {
					x = 0;y=0;width=1920;height=1080;
				}
				MyJNI.wfd_sink_SetParameters(x,y,width,height,1);//x,y,w,h,show
				try{
					Thread.sleep(100);
				}catch(InterruptedException e){
					e.printStackTrace();
				}
			}while(lcount++<300);
			//MyJNI.wfd_sink_SetParameters(x,y,width,height,1);//x,y,w,h,show
			//MyJNI.wfd_sink_SetParameters(500,200,720,480,0);//x,y,w,h,hide
			//MyJNI.wfd_sink_SetUIBC(0);//0: disable UIBC, 1: enable UIBC, need cooperate with SHOW_MOUSECURSOR
		}//end of partial view test code.*/
		/*try {
			Runtime.getRuntime().exec("touch /tmp/rtk_wfd.txt");//rtk_wfd miracast://172.29.51.162:8554
		}
		catch(IOException e){
			Log.i(TAG, "touch wfdterminate fail.");
		}*/
		Log.i(TAG, "registerReceiver(receiver, intentFilter);");	
	}
	
	@Override
	protected void onPause() {
		Log.d(TAG, "onPause");
		try {
			Runtime.getRuntime().exec("touch /tmp/wfdterminate");//rtk_wfd miracast://172.29.51.162:8554
		}
		catch(IOException e){
			Log.i(TAG, "touch wfdterminate fail.");
		}        
		if (receiver != null) {
			unregisterReceiver(receiver);
			receiver = null;
			Log.e(TAG, "unregisterReceiver(receiver);");
		}
		
		//disable mouse cursor
		Settings.System.putInt( getContentResolver(), Settings.System.SHOW_MOUSECURSOR, 1);
		super.onPause();
	}

	@Override
	public void onStop() {
		Log.d(TAG, "onStop");
		if (receiver != null) {
			Log.e(TAG,"unregisterReceiver(receiver);");
			unregisterReceiver(receiver);
			receiver = null;
		}
		super.onStop();

	}
	@Override     
    protected void onDestroy()
    {   
    	Log.d(TAG, "onDestroy");
    	super.onDestroy();        
        if (receiver != null) {
			unregisterReceiver(receiver);
			receiver = null;
		}    	        
    }   
	
	public void shutDown() {
		//Intent intent = new Intent("MediaPlayershutdown");
		//sendBroadcast(intent);
		finish();
	}
}
