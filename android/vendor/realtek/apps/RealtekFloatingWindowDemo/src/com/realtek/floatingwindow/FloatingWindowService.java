package com.realtek.floatingwindow;

import android.app.Service;
import android.os.IBinder;
import android.os.Binder;
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

public class FloatingWindowService extends Service {

    private static final String TAG = "FloatingWindowService";

    private static final int WIDTH  = 320; //(192*3);
    private static final int HEIGHT = 240; //(108*3);

    private static final int TYPE_SURFACEVIEW = 0;
    private static final int TYPE_TEXTUREVIEW = 1;

    private static final int CAMERA_TYPE_HDMI = 0;
    private static final int CAMERA_TYPE_USB = 1;
    private static final int CAMERA_TYPE_BOTH = 2;

    private static int HDMIRxAudioSource = 9;

    public static boolean bServiceIsAlive = false;

    private final IBinder mBinder = new LocalBinder();

    private WindowManager mWindowManager = null;

    private FloatingWindowViewGroup[] mViewGroup = new FloatingWindowViewGroup[2];

    private int mViewType = TYPE_SURFACEVIEW;
    private int mRecordOn = 0;
    private int mServiceCameraType = CAMERA_TYPE_HDMI;


    // We use a class to manage these views and callbacks.
    class FloatingWindowViewGroup {
        FloatingWindowView mFloatingView;
        View mPreview;
        SurfaceView mSurfaceView;
        SurfaceHolder mSurfaceHolder;
        TextureView mTextureView;
        FloatingWindowSurfaceCallback mCallback;
        FloatingWindowTextureListener mListener;
        WindowManager.LayoutParams wmParams;
        Camera mCamera;
        MediaRecorder mMediaRecorder;
        int mCameraType;
        int x;
        int y;

        boolean bPlaying = false;
        final RxBroadcastReceiver mRxBroadcastReceiver = new RxBroadcastReceiver();

        // handle broadcast
        private class RxBroadcastReceiver extends BroadcastReceiver {
            public void onReceive(Context context, Intent intent) {

                String action = intent.getAction();
                boolean bPlutOut = false;

                if(action.equals("android.intent.action.HDMIRX_PLUGGED")){
                    if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                        bPlutOut = true;
                    }else{
                        bPlutOut = false;
                    }
                }else if(action.equals("android.hardware.usb.action.USB_CAMERA_DISCONNECTED")){
                    bPlutOut = true;
                }else if(action.equals("android.hardware.usb.action.USB_CAMERA_CONNECTED")){
                    bPlutOut = false;
                }

                if(bPlutOut){
                    releaseCamera();
                }else{
                    if(!bPlaying){
                        resumePlayCamera();
                    }
                }
            }
        }

        class FloatingWindowTextureListener implements TextureView.SurfaceTextureListener {
            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
                Log.d(TAG, "onSurfaceTextureAvailable");
                playCamera(mViewType);
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
                return true;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surface) {
            }
        }

        class FloatingWindowSurfaceCallback implements SurfaceHolder.Callback {
            @Override
            public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
            }

            @Override
            public void surfaceCreated(SurfaceHolder arg0) {
                Log.d(TAG, "SurfaceCreated, start playback");
                playCamera(mViewType);
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder arg0) {
            }
        }

        private void releaseCamera(){
            Log.d(TAG, "release Camera "+mCameraType);

            if(mCamera != null){
                mCamera.release();
                mCamera = null;
            }
            bPlaying = false;
        }

        void resumePlayCamera(){
            Log.d(TAG, "resume play Camera "+mCameraType);
            playCamera(mViewType);
        }

        void playCamera(int type){
            mCamera = CameraHelper.getCameraInstance(mCameraType); //getDefaultCameraInstance();
            Camera.Parameters parameters = mCamera.getParameters();
            List<Camera.Size> mSupportedPreviewSizes = parameters.getSupportedPreviewSizes();
            Camera.Size optimalSize = CameraHelper.getOptimalPreviewSize(mSupportedPreviewSizes, 640, 480);

            int previewFrameRate = parameters.getPreviewFrameRate();
            int encodeBitRate = 5000000;

            int audioChannels = 2;
            int audioSampleRate = 48000;
            int audioBitRate = 64000;

            parameters.setPreviewSize(optimalSize.width, optimalSize.height);
            mCamera.setParameters(parameters);

            try{
                if(type == TYPE_SURFACEVIEW){
                    mCamera.setPreviewDisplay(mSurfaceHolder);
                }else{
                    mCamera.setPreviewTexture(mTextureView.getSurfaceTexture());
                }
            }catch(IOException e){
                e.printStackTrace();
            }

            // should not reach here.
            if(mRecordOn == 1){
                // BEGIN_INCLUDE (configure_media_recorder)
                mMediaRecorder = new MediaRecorder();

                // Step 1: Unlock and set camera to MediaRecorder
                mCamera.unlock();
                mMediaRecorder.setCamera(mCamera);

                // Step 2: Set sources
                mMediaRecorder.setAudioSource(HDMIRxAudioSource);
                //mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.DEFAULT);

                mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);

                // Step 3: Set a CamcorderProfile (requires API Level 8 or higher)
                //mMediaRecorder.setProfile(profile);
                mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.OUTPUT_FORMAT_MPEG2TS);
                // Strp 4: set output file path
                String path = CameraHelper.getOutputMediaFile(CameraHelper.MEDIA_TYPE_VIDEO,"").toString();
                mMediaRecorder.setOutputFile(path);

                mMediaRecorder.setVideoSize(optimalSize.width, optimalSize.height);
                mMediaRecorder.setVideoFrameRate(previewFrameRate);
                mMediaRecorder.setVideoEncodingBitRate(encodeBitRate);
                mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);

                mMediaRecorder.setAudioChannels(audioChannels);
                mMediaRecorder.setAudioSamplingRate(audioSampleRate);
                mMediaRecorder.setAudioEncodingBitRate(audioBitRate);
                mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AAC);

                try {
                    mMediaRecorder.prepare();
                } catch (IllegalStateException e) {
                    Log.d(TAG, "IllegalStateException preparing MediaRecorder: " + e.getMessage());
                    releaseMediaRecorder();
                    return;
                } catch (IOException e) {
                    Log.d(TAG, "IOException preparing MediaRecorder: " + e.getMessage());
                    releaseMediaRecorder();
                    return;
                }
                mMediaRecorder.start();
            }else{
                mCamera.startPreview();
            }

            bPlaying = true;

            if(mCameraType == CAMERA_TYPE_HDMI){
                IntentFilter filter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED");
                //filter.addAction("android.hardware.usb.action.USB_CAMERA_DISCONNECTED");
                registerReceiver(mRxBroadcastReceiver,filter);
            }else{
                IntentFilter filter = new IntentFilter("android.hardware.usb.action.USB_CAMERA_CONNECTED");
                filter.addAction("android.hardware.usb.action.USB_CAMERA_DISCONNECTED");
                registerReceiver(mRxBroadcastReceiver,filter);
            }
        }

        void releaseMediaRecorder(){
            if (mRecordOn == 1 && mMediaRecorder != null) {
                // clear recorder configuration
                mMediaRecorder.reset();
                // release the recorder object
                mMediaRecorder.release();
                mMediaRecorder = null;
                // Lock camera for later use i.e taking it back from MediaRecorder.
                // MediaRecorder doesn't need it anymore and we will release it if the activity pauses.
                mCamera.lock();
            }
        }

        void destroy(){

            unregisterReceiver(mRxBroadcastReceiver);

            if(mRecordOn == 1 && mMediaRecorder != null){
                mMediaRecorder.stop();
                releaseMediaRecorder();
                mCamera.lock();
            }
            releaseCamera();
            //SystemClock.sleep(500);
            if(mViewType == TYPE_SURFACEVIEW) {
                if(mSurfaceView != null && mSurfaceHolder != null && mCallback != null) {
                    mSurfaceHolder.removeCallback(mCallback);
                }
            }

            if(mWindowManager != null && mFloatingView != null){
                mWindowManager.removeView(mFloatingView);
            }
        }

        void setup(int type, Context c){
            if(type == TYPE_SURFACEVIEW){
                mSurfaceView = new SurfaceView(c);
                mSurfaceHolder = mSurfaceView.getHolder();
                mCallback = new FloatingWindowSurfaceCallback();
                mSurfaceHolder.addCallback(mCallback);
                mPreview = mSurfaceView;
            }else{
                mTextureView = new TextureView(c);
                mListener = new FloatingWindowTextureListener();
                mTextureView.setSurfaceTextureListener(mListener);
                mPreview = mTextureView;
            }
        }


    } // end class FloatingWindowViewGroup

    public class LocalBinder extends Binder {
        FloatingWindowService getService() {
            return FloatingWindowService.this;
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

    /**
     * API to create floating window with more argument
     */ 
    private void createFloatingWindow2(int type, int cameraType, int viewId, int x, int y){
        Log.d(TAG, "createFloatingWindow type:"+type);

        mViewGroup[viewId] = new FloatingWindowViewGroup();
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

    @Override
    public int onStartCommand (Intent intent, int flags, int startId) {

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
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Slog.d(TAG, "service destroy");

        for(int i=0;i<2;i++){
            if(mViewGroup[i]!=null){
                mViewGroup[i].destroy();
                mViewGroup[i] = null;
            }
        }

        super.onDestroy();
        bServiceIsAlive = false;
    }

    public static long getCurrentThreadID(){
        long threadId = Thread.currentThread().getId();
        return threadId;
    }
}
