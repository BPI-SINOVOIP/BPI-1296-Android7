
package com.example.RTKMiracastSink;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.util.Log;

public class MiracastSinkBootReceiver extends BroadcastReceiver {//ok
    private static String TAG = "MiracastSink.BootReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
    	Log.d(TAG, "The boot completed event is received.");

        //String setupWFDMode = "realtek_setup_wfd_mode_on"; //Settings.System.REALTEK_SETUP_DLNA_DMR_MODE_ON;
        
        int isStartWFD = 1;//Settings.System.getInt(context.getContentResolver(), setupWFDMode, 0);
        Log.d(TAG, "The isStartWFD value: " +  isStartWFD);
        if (isStartWFD == 1) {
            Log.d(TAG, "The DMR service is going to start.");

            Intent serviceIntent = new Intent();
            serviceIntent.setAction("com.example.RTKMiracastSink.MiracastSinkService");
            context.startService(serviceIntent);
        }
    }
}
