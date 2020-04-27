package com.realtek.suspendhandler;

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

    static final String TAG= "RealtekSuspend" ;
    private AudioManager mSound = null;
    //SuspendHandler mReceiver = null;
    //Handler mHandler = new Handler();

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "intent   --- :"+intent.getAction());
        // register screen on/off receiver
        Log.d(TAG," register SCREEN_ON SCREEN_OFF receiver");
        
        //mReceiver = new SuspendHandler();
        //IntentFilter filter = new IntentFilter();
        //filter.addAction(Intent.ACTION_SCREEN_OFF);
        //filter.addAction(Intent.ACTION_SCREEN_ON);
        //context.registerReceiver(mReceiver, filter, null, mHandler);
        

        // start service...
        Intent serviceIntent = new Intent(context, com.realtek.suspendhandler.SuspendService.class);
        context.startService(serviceIntent);

        /*ATM-475 start
        //to open pcm device for reduce the time of pcm_open and pcm_prepare
        String ctsMode = SystemProperties.get("persist.rtk.ctstest","false");
        boolean bCTSMode = false;
        if(ctsMode.equals("1") || ctsMode.equals("true")){
            bCTSMode = true;
        }
        */

        boolean bCTSMode = true;
        mSound = (AudioManager)context.getSystemService(Context.AUDIO_SERVICE);
        if (mSound != null && bCTSMode == true) {
            try {
                    mSound.playSoundEffect(AudioManager.FX_KEYPRESS_STANDARD, 0.01f);
                } catch (Exception ex) {
                    Log.e(TAG, "Bootcomplete to play EffectSound fail" + ex.toString());
                }
        }
        /*ATM-475 end */

    }
}

