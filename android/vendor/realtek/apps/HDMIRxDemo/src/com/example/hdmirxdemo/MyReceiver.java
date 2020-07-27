package com.example.hdmirxdemo;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;
import android.media.AudioManager;
import android.os.SystemProperties;
import android.widget.Toast;
import android.os.Bundle;
import android.os.Handler;

import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.RtkHDMIRxManager.Size;
import com.realtek.hardware.InvalidOpException;
import com.realtek.hardware.HDMIRxStatus;
import com.realtek.hardware.HDMIRxParameters;

public class MyReceiver extends BroadcastReceiver {

    public static final String RX_STATUS_STRING = "android.intent.action.HDMIRX_PLUGGED";
    public static final String RX_HDCP_STRING = "android.intent.action.HDMIRX_HDCP_STATUS";

    public static final String VIDEO_STATUS_BC = "android.intent.action.VIDEO_STATUS";

    static final String TAG= "HDMIRxDemoReceiver";
    public static BroadcastListener sListener = null;
    private Context mContext;

    @Override
    public void onReceive(Context context, Intent intent) {
        mContext = context;
        String action = intent.getAction();
        Log.d(TAG, "intent: "+action);
        //Toast.makeText(context,"Intent:"+intent.getAction(),Toast.LENGTH_SHORT).show();

        if(action.equals("android.intent.action.BOOT_COMPLETED")){
            HDMIDbHelper dbHelper = HDMIDbHelper.getInstance(context);
            GeneralService2.getInstance(context);
            startWatchdogService(context);
        }else if(action.equals("android.intent.action.PLATFORM_SCREEN_ON")){
            Log.d(TAG, "receive android.intent.action.PLATFORM_SCREEN_ON");
            // TODO: not sure the side effect
            FloatingWindowViewGroup3.removePendingWarningDialog();
            HDMIDbHelper dbHelper = HDMIDbHelper.getInstance(context);
            GeneralService2.getInstance(context);
            startWatchdogService(context);
        }else if(action.equals("android.intent.action.PLATFORM_SCREEN_OFF")){
            Log.d(TAG+"-Suspend", "receive android.intent.action.PLATFORM_SCREEN_OFF");
            //FloatingWindowService2.stopServiceItSelf();
        }else if(action.equals(RX_STATUS_STRING)){
            processHotPlugEvent(intent);
        }else if(action.equals(RX_HDCP_STRING)){
            processHDCPEvent(intent);
        }else if(action.equals(VIDEO_STATUS_BC)){
            processVideoStatus(intent, context);
        }
    }

    private void startWatchdogService(Context c){
        Intent serviceIntent = new Intent(
                c, com.example.hdmirxdemo.WatchdogService.class);
        c.startService(serviceIntent);
    }

    private void processVideoStatus(Intent i, Context c){
        Log.d(TAG, "processVideoStatus");
        Bundle b = i.getExtras();
        boolean status = b.getBoolean("status",false);
        String mime = b.getString("mime");
        int width = b.getInt("width",-1);
        int height = b.getInt("height",-1);
        int fps = (int) (b.getFloat("frame-rate",-1f));

        if(width==-1 || height==-1 || fps == -1){
            Log.e(TAG,"invalid VIDEO_STATUS_BC, return");
            return;
        }

        //GeneralService2.VideoStatusInfo info = new GeneralService2.VideoStatusInfo(mime,width,height);
        if(status){
            GeneralService2.getInstance(c).insertVideoStatus(mime, width, height, fps);
        }else{
            GeneralService2.getInstance(c).deleteVideoStatus(mime, width, height, fps);
        }
    }

    private void showToast(String message) {
        Toast.makeText(mContext,message,Toast.LENGTH_SHORT).show();
    }

    private void processHotPlugEvent(Intent intent){
        Log.d(TAG, "processHotPlugEvent");

        /*
        if(FloatingWindowService2.bServiceIsAlive){
            FloatingWindowService2.handleHotplugBroadcast(intent);
        }
        */

        if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
            Log.d(TAG, "hdmi plug out");
            FloatingWindowService2.stopServiceItSelf();
            FloatingWindowService2.sHDMIRxStatus = null;
            FloatingWindowService2.sHDMIRxParameters = null;
        }else{
            Log.d(TAG, "hdmi plugged - service state:\""+FloatingWindowService2.bServiceIsAlive+"\"");
            //prepareHDMIInfoParameters();
            FloatingWindowViewGroup3.removePendingRxWarningDialog();
        }

        // sListener is MainActivity, it does following
        // 1. plug in  -> lunch FloatingWindowService
        // 2. plug out -> stop FloatingWindowService
        if(sListener != null){
            sListener.handleBroadcast(intent);
        }
    }

    // don't call me
    /*
    private void prepareHDMIInfoParameters(){
        Log.d(TAG,"prepareHDMIInfoParameters");

        FloatingWindowService2.sHDMIRxStatus = null;
        FloatingWindowService2.sHDMIRxParameters = null;

        FloatingWindowService2.sHDMIRxStatus = HDMIRxManagerController.getHDMIRxStatus();

        if(FloatingWindowService2.sHDMIRxStatus != null &&
            FloatingWindowService2.sHDMIRxStatus.status == HDMIRxStatus.STATUS_READY) {

            if(HDMIRxManagerController.isOpened()){
                FloatingWindowService2.sHDMIRxParameters = HDMIRxManagerController.getParameters();
                return;
            }else{
                // open HDMIRx
                if(HDMIRxManagerController.open()!=0){
                    Log.e(TAG, "open HDMIRx failed");
                    HDMIRxManagerController.release();
                    return;
                }
                FloatingWindowService2.sHDMIRxParameters = HDMIRxManagerController.getParameters();
                //HDMIRxManagerController.stop(); <-- do not call stop since no one called play
                HDMIRxManagerController.release();
                Log.d(TAG, "prepareHDMIInfoParameters - close HDMIRx");
            }
        }
    }
    */

    private void processHDCPEvent(Intent intent){
        Log.d(TAG,"processHDCPEvent");
        if(FloatingWindowService2.bServiceIsAlive){
            FloatingWindowService2.handleHDCPBroadcast(intent);
        }
    }
}

