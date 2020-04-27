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

public class FloatingWindowService2 extends Service {

    private static final String TAG = "FloatingWindowService2";

    //private static final int WIDTH  = 320; //(192*3);
    //private static final int HEIGHT = 240; //(108*3);

    public static final int ID_HDMIRX = 0;
    public static final int ID_USBCAM = 1;
    public static final int ID_MEDIAPLAYER = 2;

    /*
    private static final int TYPE_SURFACEVIEW = 0;
    private static final int TYPE_TEXTUREVIEW = 1;

    private static final int CAMERA_TYPE_HDMI = 0;
    private static final int CAMERA_TYPE_USB = 1;
    private static final int CAMERA_TYPE_BOTH = 2;
    */

    //private static int HDMIRxAudioSource = 9;

    public static boolean bServiceIsAlive = false;

    private final IBinder mBinder = new LocalBinder();

    private WindowManager mWindowManager = null;

    private FloatingWindowViewGroup[] mViewGroup = new FloatingWindowViewGroup[3];
    private FloatingWindowViewGroup2[] mViewGroup2 = new FloatingWindowViewGroup2[3];

    //private int mViewType = TYPE_SURFACEVIEW;
    //private int mRecordOn = 0;
    //private int mServiceCameraType = CAMERA_TYPE_HDMI;

    private Bundle mRx;
    private Bundle mCam;
    private Bundle mMP;

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

    // this is for mediaplayer floating window
    private void createFloatingWindowEx2(int winId, Bundle arg){
        boolean enabled = arg.getBoolean(SettingWidgets2.KEY_ENABLED);
        if(!enabled){
            Log.d(TAG, "WinID:"+winId+" is not enabled");
            return;
        }

        mViewGroup2[winId] = new FloatingWindowViewGroup2(this,arg);
        FloatingWindowViewGroup2 group = mViewGroup2[winId];

        mWindowManager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        group.mFloatingView = (FloatingWindowView) li.inflate(R.layout.floatingwindow,null);

        TextView infoText = (TextView) group.mFloatingView.findViewById(R.id.info_textview);
        infoText.setText("Please hide OSD if you can't see video");

        // setup view type
        int viewType = arg.getInt(SettingWidgets2.KEY_VIEWTYPE);
        group.setup(viewType, this);

        /*
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        if(group.mViewType != FloatingWindowViewGroup.TYPE_NOPREVIEW){
            group.mPreview.setLayoutParams(param);
            group.mFloatingView.addView(group.mPreview);
        }
        */
        group.attachPreview();

        int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
            //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        int windowSize = arg.getInt(SettingWidgets2.KEY_SIZE);
        int width = (windowSize==SettingWidgets2.WINDOW_HD)?192*3:320;
        int height = (windowSize==SettingWidgets2.WINDOW_HD)?108*3:240;

        group.wmParams = new WindowManager.LayoutParams(
            width,  //192*3,//WindowManager.LayoutParams.MATCH_PARENT,
            height, //108*3,//WindowManager.LayoutParams.MATCH_PARENT,
            FloatingWindowView.VIEW_TYPE,
            flag,
            PixelFormat.TRANSLUCENT);
		int screenWidth = mWindowManager.getDefaultDisplay().getWidth();
		int screenHeight = mWindowManager.getDefaultDisplay().getHeight();
		group.wmParams.x = (screenWidth- width)/2;
		group.wmParams.y = screenHeight - height;
        group.wmParams.gravity = Gravity.TOP|Gravity.LEFT;

        group.mFloatingView.setBackgroundColor(Color.BLACK);
        mWindowManager.addView(group.mFloatingView, group.wmParams);
    }

    private void createFloatingWindowEx(int winId, Bundle arg){
        boolean enabled = arg.getBoolean(SettingWidgets.KEY_ENABLED);
        if(!enabled){
            Log.d(TAG, "WinID:"+winId+" is not enabled");
            return;
        }

        mViewGroup[winId] = new FloatingWindowViewGroup(this,arg);
        FloatingWindowViewGroup group = mViewGroup[winId];

        int sourceType = winId;
        group.mCameraType = sourceType;

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

        /*
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        if(group.mViewType != FloatingWindowViewGroup.TYPE_NOPREVIEW){
            group.mPreview.setLayoutParams(param);
            group.mFloatingView.addView(group.mPreview);
        }
        */
        group.attachPreview();

        int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
            //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        int windowSize = arg.getInt(SettingWidgets.KEY_SIZE);
        int width = 0;
        int height = 0;
        switch(windowSize){
            case SettingWidgets.WINDOW_HD_1920:
                width = 576;
                height = 324;
                break;
            case SettingWidgets.WINDOW_HD_1280:
				width = 448;
                height = 252;
                break;
            case SettingWidgets.WINDOW_SD:
                width = 256;
                height = 144;
				break;
        }
        group.wmParams = new WindowManager.LayoutParams(
            width,  //192*3,//WindowManager.LayoutParams.MATCH_PARENT,
            height, //108*3,//WindowManager.LayoutParams.MATCH_PARENT,
            FloatingWindowView.VIEW_TYPE,
            flag,
            PixelFormat.TRANSLUCENT);

        //group.wmParams.x = 100*winId;
        //group.wmParams.y = 100*winId;
		int screenWidth = mWindowManager.getDefaultDisplay().getWidth();
		int screenHeight = mWindowManager.getDefaultDisplay().getHeight();
		switch(winId){
			case ID_HDMIRX:
				group.wmParams.x = 0;
				group.wmParams.y = screenHeight - height;
				break;
			case ID_USBCAM:
				group.wmParams.x = screenWidth- width;
				group.wmParams.y = screenHeight - height;
				break;
		}
       
        group.wmParams.gravity = Gravity.TOP|Gravity.LEFT;

        group.mFloatingView.setBackgroundColor(Color.BLACK);
        mWindowManager.addView(group.mFloatingView, group.wmParams);
    }

    /*
    private void createFloatingWindow2(int type, int cameraType, int viewId, int x, int y){
        Log.d(TAG, "createFloatingWindow type:"+type);

        mViewGroup[viewId] = new FloatingWindowViewGroup(this,null);
        FloatingWindowViewGroup group = mViewGroup[viewId];

        group.mCameraType = cameraType;

        mWindowManager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        group.mFloatingView = (FloatingWindowView) li.inflate(R.layout.floatingwindow,null);

        group.setup(type, this);

        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        group.mPreview.setLayoutParams(param);
        group.mFloatingView.addView(group.mPreview);

        int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
            //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        group.wmParams = new WindowManager.LayoutParams(
            WIDTH,  //192*3,//WindowManager.LayoutParams.MATCH_PARENT,
            HEIGHT, //108*3,//WindowManager.LayoutParams.MATCH_PARENT,
            FloatingWindowView.VIEW_TYPE,
            flag,
            PixelFormat.TRANSLUCENT);

        //wmParams.x = 200;
        //wmParams.y = 10;
        group.wmParams.x = x;
        group.wmParams.y = y;

        group.wmParams.gravity = Gravity.TOP|Gravity.LEFT;

        group.mFloatingView.setBackgroundColor(Color.RED);
        mWindowManager.addView(group.mFloatingView, group.wmParams);
    }
    */

    @Override
    public int onStartCommand (Intent intent, int flags, int startId) {
        /*
        mViewType = intent.getIntExtra("surface",TYPE_SURFACEVIEW);

        // in this demo, force floating window using TextureView.
        mViewType =  TYPE_TEXTUREVIEW;

        mRecordOn = intent.getIntExtra("record",0);
        mServiceCameraType = intent.getIntExtra("camera",CAMERA_TYPE_HDMI);

        Slog.d(TAG , "service onStartCommand surface:"+mViewType+" record:"+mRecordOn);

        if(mServiceCameraType != CAMERA_TYPE_BOTH){
            createFloatingWindow2(mViewType,mServiceCameraType,mServiceCameraType,200,100);
        }else{
            createFloatingWindow2(mViewType, CAMERA_TYPE_HDMI, CAMERA_TYPE_HDMI, 200, 100);
            createFloatingWindow2(mViewType, CAMERA_TYPE_USB, CAMERA_TYPE_USB, 200, 100 + HEIGHT + 10);
        }
        */

        mRx = intent.getBundleExtra("rx");
        mCam = intent.getBundleExtra("usbcam");
        mMP = intent.getBundleExtra("mp");

        Log.d(TAG, dumpBundleInfo(mRx));
        Log.d(TAG, dumpBundleInfo(mCam));
        Log.d(TAG, dumpBundleInfo2(mMP));

        createFloatingWindowEx(ID_HDMIRX,mRx);
        createFloatingWindowEx(ID_USBCAM,mCam);
        createFloatingWindowEx2(ID_MEDIAPLAYER,mMP);
        // TODO create floating window for MediaPlayer
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Slog.d(TAG, "service destroy");

        for(int i=0;i<3;i++){

            if(mViewGroup[i]!=null){
                mViewGroup[i].destroy();
                mViewGroup[i] = null;
            }

            if(mViewGroup2[i]!=null){
                mViewGroup2[i].destroy();
                mViewGroup2[i] = null;
            }
        }

        super.onDestroy();
        bServiceIsAlive = false;
    }

    public static long getCurrentThreadID(){
        long threadId = Thread.currentThread().getId();
        return threadId;
    }

    public static String dumpBundleInfo2(Bundle bundle){
        return "Enabled:"+bundle.getBoolean(SettingWidgets2.KEY_ENABLED)
                +" ViewType:"+bundle.getInt(SettingWidgets2.KEY_VIEWTYPE)
                +" RTMediaPlayer:"+bundle.getBoolean(SettingWidgets2.KEY_RTMEDIAPLAYER)
                +" WindowSize:"+bundle.getInt(SettingWidgets2.KEY_SIZE)
                +" File:"+bundle.getString(SettingWidgets2.KEY_FILE_PATH);
    }

    public static String dumpBundleInfo(Bundle bundle){
        return "Enabled:"+bundle.getBoolean(SettingWidgets.KEY_ENABLED)
                +" ViewType:"+bundle.getInt(SettingWidgets.KEY_VIEWTYPE)
                +" Record:"+bundle.getBoolean(SettingWidgets.KEY_RECORD)
                +" WindowSize:"+bundle.getInt(SettingWidgets.KEY_SIZE);
    }
}
