package com.android.launcher2;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
//import android.os.Handler;
import android.util.Log;
import android.media.AudioManager;
import android.os.SystemProperties;
/**
 * Realtek system suspend/resume handler
 */
public class BootCompleteReceiver extends BroadcastReceiver {

    static final String TAG= "RealtekLauncher2" ;
    Launcher mLauncher;

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "intent   --- :"+intent.getAction());

        Log.d(TAG, "context:"+context+ " applicationContext: "+context.getApplicationContext());

        // refresh focusview again
        if(context.getApplicationContext() instanceof LauncherApplication) {
            LauncherApplication app = (LauncherApplication)context.getApplicationContext();
            if(app.mLauncher != null) {
                Log.d(TAG, "set mLauncher");
                mLauncher = app.mLauncher;
                mLauncher.mBootCompleted = true;
                mLauncher.clearWorkspaceFocusViewFrame();
                mLauncher.redrawWorkspaceFocusViewFrame();
            }
        }
    }
}

