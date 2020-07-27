package com.example.hdmirxdemo;

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
import android.os.PowerManager;

import java.io.PrintWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.List;
import java.io.IOException;

import com.realtek.hardware.HDMIRxStatus;
import com.realtek.hardware.HDMIRxParameters;

public class FloatingWindowService2 extends Service {

    private static final String TAG = "FloatingWindowService2";

    public static final int FLOATING_WINDOW_TYPE = WindowManager.LayoutParams.TYPE_TOAST;

    public static final int ID_HDMIRX_MANAGER = 0;

    public static boolean bServiceIsAlive = false;

    private final IBinder mBinder = new LocalBinder();


    private static final Object sServiceLifeLock = new Object();

    private WindowManager mWindowManager = null;

    private static FloatingWindowViewGroup3[] mViewGroup3 = new FloatingWindowViewGroup3[3];
    private static FloatingWindowService2 mInstance;
    private Bundle mRx2;
    private int[] mInAppLocation;

    public int mAnimationResource = 0; //R.style.Animation_Window; //R.anim.no_anim; //android.R.style.Animation_Translucent;

    public static HDMIRxStatus sHDMIRxStatus = null;
    public static HDMIRxParameters sHDMIRxParameters = null;

    public static BroadcastListener sListener = null;

    private static PowerManager.WakeLock mWakeLock = null;

    public static WindowAliveReplyer mWindowAliveReplyer = null;

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

    public static FloatingWindowService2 getInstance(){
        return mInstance;
    }

    public void acquireWakeLock(){
        if(mWakeLock == null) {
            Log.d(TAG,"acquireWakeLock");
            PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
            mWakeLock = pm.newWakeLock(PowerManager.RTK_STRONG_WAKE_LOCK, "HDMIRxDemo.Record");
            mWakeLock.acquire();
        }else{
            Log.d(TAG,"acquireWakeLock failed, WakeLock already exists");
        }
    }

    public void releaseWakeLock(){
        if(mWakeLock!=null){
            Log.d(TAG, "release wake lock");
            mWakeLock.release();
            mWakeLock = null;
        }else{
            Log.e(TAG, "release wake lock failed - wake_lock is null");
        }
    }

    private void createFloatingWindowEx3(int winId, Bundle arg){

        mViewGroup3[winId] = new FloatingWindowViewGroup3(this,arg);
        FloatingWindowViewGroup3 group = mViewGroup3[winId];

        mWindowManager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        group.mFloatingView = (FloatingWindowView) li.inflate(R.layout.floatingwindow,null);

        // setup view type
        int viewType = Keys.TYPE_SURFACEVIEW;
        group.setup(viewType, this);
        group.attachPreview();
        group.attachFocusOverlay();

        boolean touchable = arg.getBoolean(Keys.KEY_TOUCHABLE,false);

        int flag;

        if(touchable) {
            flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }else{
            flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }

        int[] location = arg.getIntArray(Keys.KEY_DIMENSION);
        int x = location[0];
        int y = location[1];
        int width = location[2];
        int height = location[3];

        if(x<5000 && y<5000 && width>1 && height>1){
            mInAppLocation = location;
        }

        group.wmParams = new WindowManager.LayoutParams(
            width,
            height,
            FLOATING_WINDOW_TYPE,
            flag,
            PixelFormat.TRANSLUCENT);

        group.wmParams.x = x;
        group.wmParams.y = y;

        group.wmParams.gravity = Gravity.TOP|Gravity.LEFT;
        group.wmParams.windowAnimations = mAnimationResource;
        group.mFloatingView.setBackgroundColor(Color.BLACK);
        if(viewType != Keys.TYPE_NOPREVIEW){
            mWindowManager.addView(group.mFloatingView, group.wmParams);
        }
    }

    @Override
    public int onStartCommand (Intent intent, int flags, int startId) {

        synchronized(sServiceLifeLock) {

            Log.d(TAG, "onStartCommand");

            mInstance = this;
            mRx2 = intent.getBundleExtra("rx");
            mInAppLocation = null;

            if(mViewGroup3[ID_HDMIRX_MANAGER] != null){
                Log.d(TAG, "found pending view group, delete it");
                mViewGroup3[ID_HDMIRX_MANAGER].destroy();
                mViewGroup3[ID_HDMIRX_MANAGER] = null;
            }

            createFloatingWindowEx3(ID_HDMIRX_MANAGER,mRx2);
            return Service.START_NOT_STICKY;
        }
    }

    @Override
    public void onDestroy() {

        synchronized(sServiceLifeLock){
            Slog.d(TAG, "service destroy");
            if(mViewGroup3[ID_HDMIRX_MANAGER] != null){
                mViewGroup3[ID_HDMIRX_MANAGER].destroy();
                mViewGroup3[ID_HDMIRX_MANAGER] = null;
            }

            sHDMIRxStatus = null;
            sHDMIRxParameters = null;

            super.onDestroy();
            bServiceIsAlive = false;
            mInstance = null;
        }
    }

    public static long getCurrentThreadID(){
        long threadId = Thread.currentThread().getId();
        return threadId;
    }

    public static void stopRecord() {
        if(mInstance==null){
            return;
        }

        if(!isRecording()){
            Log.e(TAG, "stopRecord, but no record instance");
            return;
        }

        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].stopRecord();
        }
    }

    public static void startRecord(Bundle arg){

        if(mInstance==null){
            return;
        }

        if(isRecording()){
            Log.e(TAG, "startRecord but is already recording");
            return;
        }

        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].startRecord(arg);
        }
    }

    public static void updateWindowLocation(int[] position, boolean touchable) {
        if(mInstance==null) return;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            Log.i(TAG, "updateWindowLocation");
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].updateWindowLocation(position,touchable);
        }else{
            Log.e(TAG, "updateWindowLocation window is null");
        }
    }

    public static boolean isRecording() {
        if(mInstance==null) return false;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            return mInstance.mViewGroup3[ID_HDMIRX_MANAGER].isRecording();
        }
        return false;
    }

    public static int[] getInAppLocation() {
        if(mInstance==null) return null;
        return mInstance.mInAppLocation;
    }

    public static boolean isAudioOn(){
        if(mInstance==null) return false;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            return mInstance.mViewGroup3[ID_HDMIRX_MANAGER].isAudioOn();
        }
        return false;
    }

    public static boolean isVideoOn(){
        if(mInstance==null) return false;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            return mInstance.mViewGroup3[ID_HDMIRX_MANAGER].isVideoOn();
        }
        return false;
    }

    public static void setPlayback(boolean vOn, boolean aOn){
        if(mInstance==null) return;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].setPlaybackLocked(vOn,aOn);
        }
    }

    // hide this API
    private static void handleHotplugBroadcast(Intent intent){
        if(mInstance==null) return;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].handleHotplugBroadcast(intent);
        }
    }

    public static void handleHDCPBroadcast(Intent intent){
        if(mInstance==null) return;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].handleHDCPBroadcast(intent);
        }
    }

    public static int getOutputMode(){
        if(mInstance==null) return Keys.OUTPUT_OFF;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            return mInstance.mViewGroup3[ID_HDMIRX_MANAGER].getOutputMode();
        }
        return Keys.OUTPUT_OFF;
    }

    public static boolean isFocusShowing(){
        if(mInstance==null) return false;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            return mInstance.mViewGroup3[ID_HDMIRX_MANAGER].isFocusShowing();
        }
        return false;
    }

    public static void setInAppLocation(int[] position){
        if(mInstance==null)return;
        mInstance.mInAppLocation = position;
    }

    public static void showFocusOverlay(boolean show) {
        if(mInstance==null) return;
        if(mInstance.mViewGroup3[ID_HDMIRX_MANAGER]!=null){
            mInstance.mViewGroup3[ID_HDMIRX_MANAGER].showFocusOverlay(show);
        }
    }

    public static void stopServiceItSelf(){
        if(mInstance != null){
            mInstance.stopSelf();
        }
    }

    public static FloatingWindowViewGroup3 getViewGroupControl(){
        if(mInstance==null) return null;
        return mInstance.mViewGroup3[ID_HDMIRX_MANAGER];
    }
}
