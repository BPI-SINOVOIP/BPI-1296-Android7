package com.realtek.suspendhandler;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.util.Log;

public class UnmountTimeoutReceiver extends BroadcastReceiver {

    static final String TAG= "RealtekSuspendUnmountTimeout" ;

    // use service to manage wakeLock
    private SuspendService mService;
    public UnmountTimeoutReceiver(SuspendService service){
        mService = service;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "onReceive :"+intent.getAction());
    }
}
