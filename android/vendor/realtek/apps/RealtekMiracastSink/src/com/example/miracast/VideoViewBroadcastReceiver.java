package com.example.RTKMiracastSink;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.util.Log;

public class VideoViewBroadcastReceiver extends BroadcastReceiver{
	
    private static final String TAG = "VideoViewBroadcastReceiver";
	private VideoViewActivity activity;
	
	public VideoViewBroadcastReceiver(VideoViewActivity activity){
		super();
		this.activity = activity;
	}

		@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		String action = intent.getAction();
		
		
		if ("MediaPlayerKill".equals(action)){
			Log.i(TAG, "recive MediaPlayerKill");
			activity.shutDown();
		}
	}
	
}
