package com.rtk.debug;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.util.Utils;

/**
 * Created by archerho on 2017/3/30.
 */
public class IntentReceiver extends BroadcastReceiver {
    private static final boolean DEBUG = true;
    static final String TAG = IntentReceiver.class.getSimpleName();
    static final String KEY_BOOT_START_LOG = "persist.rtk.boot.log";
    private boolean isBootCompleted;

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        log("onReceive "+action);
        AutoBootLog bootLog = AutoBootLog.getInstance(context.getApplicationContext());
        final boolean hasExtStorage = Utils.hasExtStorage(context);
        log("hasExtStorage="+ hasExtStorage);
        if (action.equals(Intent.ACTION_BOOT_COMPLETED)){
            int enable = SystemProperties.getInt(KEY_BOOT_START_LOG, 0);
            if (enable == 1) {
//                Intent serviceIntent = new Intent(context, LogcatService.class);
//                serviceIntent.putExtra(LogcatService.EXTRA_TASK, TAG);
//                context.startService(serviceIntent);

                if(hasExtStorage) {
                    log("Start boot logcat");
                    bootLog.on();
                } else {
                    isBootCompleted = true;
                    log("BOOT_COMPLETED, but media isn't ready yet");
                }
            }
        } else if(action.equals(Intent.ACTION_MEDIA_MOUNTED)){
            int enable = SystemProperties.getInt(KEY_BOOT_START_LOG, 0);
            if(enable==1 && isBootCompleted && hasExtStorage){
                log("Start boot logcat");
                bootLog.on();
            }
        }
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }
}
