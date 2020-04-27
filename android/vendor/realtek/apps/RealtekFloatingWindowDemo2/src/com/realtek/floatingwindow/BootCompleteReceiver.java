package com.realtek.floatingwindow;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;
import android.media.AudioManager;
import android.os.SystemProperties;

/**
 * Realtek Floating Window Demo
 */
public class BootCompleteReceiver extends BroadcastReceiver {

    static final String TAG= "FloatingWindow" ;
    private AudioManager mSound = null;

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "intent: "+intent.getAction());

        // start service, 
        //Intent serviceIntent = new Intent(context, com.realtek.floatingwindow.FloatingWindowService.class);
        //context.startService(serviceIntent);

    }
}

