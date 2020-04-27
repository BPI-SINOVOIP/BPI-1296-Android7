package com.realtek.floatingwindow;

import android.app.Service;
import android.os.IBinder;
import android.os.Binder;
import android.os.Bundle;
import android.content.Intent;
import android.util.Log;
import android.util.Slog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.SystemClock;
import android.view.View;
import android.view.WindowManager;
import android.view.LayoutInflater;
import android.view.Gravity;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.SurfaceHolder;
import android.graphics.PixelFormat;
import android.graphics.Color;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.widget.RelativeLayout;
import android.view.ViewGroup;
import android.media.MediaRecorder;
import android.widget.Toast;
import android.widget.TextView;

import java.io.PrintWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.List;
import java.io.IOException;

import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.HDMIRxStatus;

import com.realtek.floatingwindow.rtk.R;

public class FloatingWindowService2 extends Service {

    private static final String TAG = "FloatingWindowService2";

    public static final int FLOATING_WINDOW_TYPE = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;

    public static final int ID_HDMIRX = 0;
    public static final int ID_USBCAM = 1;
    public static final int ID_MEDIAPLAYER = 2;

    public static final int ID_HDMIRX_MANAGER = 0;

    public static boolean bServiceIsAlive = false;

    private final IBinder mBinder = new LocalBinder();

    private WindowManager mWindowManager = null;

    private FloatingWindowViewGroup3[] mViewGroup3 = new FloatingWindowViewGroup3[3];
    private static FloatingWindowService2 mInstance;
    private Bundle mRx2;

    public class LocalBinder extends Binder {
        FloatingWindowService2 getService() {
            return FloatingWindowService2.this;
        }
    }

    @Override
    public IBinder onBind (Intent intent) {
        return mBinder;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Slog.d(TAG, "service onCreate");
        bServiceIsAlive = true;
    }

    private void createFloatingWindowEx3(int winId, Bundle arg){
        boolean enabled = arg.getBoolean(SettingWidgets.KEY_ENABLED);
        if(!enabled){
            Log.d(TAG, "WinID:"+winId+" is not enabled");
            return;
        }

        mViewGroup3[winId] = new FloatingWindowViewGroup3(this,arg);
        FloatingWindowViewGroup3 group = mViewGroup3[winId];

        mWindowManager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        group.mFloatingView = (FloatingWindowView) li.inflate(R.layout.floatingwindow,null);

        TextView infoText = (TextView) group.mFloatingView.findViewById(R.id.info_textview);
        if(winId==ID_HDMIRX){
            infoText.setText("HDMIRx: No preview.. (ˊ_>ˋ)");
        }else if(winId==ID_USBCAM){
            infoText.setText("USBCam: No preview.. (ˊ_>ˋ)");
        }else{
            infoText.setText("No preview.. (ˊ_>ˋ)");
        }

        // setup view type
        int viewType = arg.getInt(SettingWidgets.KEY_VIEWTYPE);
        group.setup(viewType, this);

        group.attachPreview();

        int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
            //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        int windowSize = arg.getInt(SettingWidgets.KEY_SIZE);
        int width = (windowSize==SettingWidgets.WINDOW_HD)?1920:576;
        int height = (windowSize==SettingWidgets.WINDOW_HD)?1080:324;

        group.wmParams = new WindowManager.LayoutParams(
            width,  //192*3,//WindowManager.LayoutParams.MATCH_PARENT,
            height, //108*3,//WindowManager.LayoutParams.MATCH_PARENT,
            FLOATING_WINDOW_TYPE, //WindowManager.LayoutParams.TYPE_TOAST,
            flag,
            PixelFormat.TRANSLUCENT);

        group.wmParams.x = 100*winId;
        group.wmParams.y = 100*winId;

        group.wmParams.gravity = Gravity.TOP|Gravity.LEFT;

        group.mFloatingView.setBackgroundColor(Color.BLACK);
        mWindowManager.addView(group.mFloatingView, group.wmParams);
    }

    @Override
    public int onStartCommand (Intent intent, int flags, int startId) {
        mInstance = this;
        mRx2 = intent.getBundleExtra("rx2");

        Log.d(TAG, dumpBundleInfo(mRx2));

        createFloatingWindowEx3(ID_HDMIRX_MANAGER,mRx2);
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Slog.d(TAG, "service destroy");

        for(int i=0;i<3;i++){

            if(mViewGroup3[i]!=null){
                mViewGroup3[i].destroy();
                mViewGroup3[i] = null;
            }
        }

        super.onDestroy();
        bServiceIsAlive = false;
        mInstance = null;
    }

    public static long getCurrentThreadID(){
        long threadId = Thread.currentThread().getId();
        return threadId;
    }

    public static void toggleRecord(){

        if(mInstance==null){
            return;
        }

        for(int i=0;i<3;i++){
            if(mInstance.mViewGroup3[i]!=null){
                mInstance.mViewGroup3[i].toggleRecord();
            }
        }
    }

    public static void togglePreview(){

        if(mInstance==null){
            return;
        }

        for(int i=0;i<3;i++){
            if(mInstance.mViewGroup3[i]!=null){
                mInstance.mViewGroup3[i].togglePreview();
            }
        }
    }

    public static void onActivityPauseResume(boolean resume){
        if(mInstance==null){
            return;
        }

        for(int i=0;i<3;i++){
            if(mInstance.mViewGroup3[i]!=null){
                mInstance.mViewGroup3[i].onActivityPauseResume(resume);
            }
        }
    }

    public static String dumpBundleInfo(Bundle bundle){
        return "Enabled:"+bundle.getBoolean(SettingWidgets.KEY_ENABLED)
                +" ViewType:"+bundle.getInt(SettingWidgets.KEY_VIEWTYPE)
                +" Record:"+bundle.getBoolean(SettingWidgets.KEY_RECORD)
                +" WindowSize:"+bundle.getInt(SettingWidgets.KEY_SIZE)
                +" ForceFPS:"+bundle.getBoolean(SettingWidgets.KEY_FORCE_FPS);
    }
}
