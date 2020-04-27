package com.realtek.transcode;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
//import android.os.Handler;
import android.util.Log;

public class BootReceiver extends BroadcastReceiver {

    static final String TAG= RealtekTranscodeService.TAG;

    public BootReceiver(){
        super();
        Log.d(TAG, "BootReceiver()");
    }

    public void onReceive(Context context, Intent intent) {
        // start TranscodeService on BootComplete
        //Log.d(TAG, "Receive BOOT_COMPLETED, start RealtekTranscodeService");
        String action = intent.getAction();
        Log.d(TAG, "Receive :"+action);
        if(action.equals("android.intent.action.BOOT_COMPLETED")){
            Intent serviceIntent = new Intent(context,
                    com.realtek.transcode.RealtekTranscodeService.class);
            context.startService(serviceIntent);
        }else{
            RealtekTranscodeService.getInstance().onBind(intent);
        }
    }
}

