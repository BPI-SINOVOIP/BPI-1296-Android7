
package com.realtek.dmr;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.util.Log;

public class BootReceiver extends BroadcastReceiver {
    private static String TAG = "DMR.BootReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "The boot completed event is received.");

//        String setupDMRMode = "realtek_setup_dlna_dmr_mode_on"; //Settings.System.REALTEK_SETUP_DLNA_DMR_MODE_ON;
//
//        int isStartDMR = Settings.System.getInt(context.getContentResolver(), setupDMRMode, 0);
//        Log.d(TAG, "The isStartDMR value: " +  isStartDMR);
//        if (isStartDMR == 1) {
//            Log.d(TAG, "The DMR service is going to start.");
//
//            Intent serviceIntent = new Intent();
//            serviceIntent.setAction("com.realtek.dmr.DMRService");
//            context.startService(serviceIntent);
//        }
    }
}
