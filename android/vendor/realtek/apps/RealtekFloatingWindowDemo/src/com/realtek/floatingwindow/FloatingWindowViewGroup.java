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

/**
 * Descriptions:
 *  the control class of a floating Window
 *  TODO:
 *  1. [V] record on
 *  2. [V] no preview
 *  3. [X] MediaPlayer -> use another class to control
 *
 */
public class FloatingWindowViewGroup {

    private static final String TAG="FloatingWindowViewGroup";

    private static final int TYPE_SURFACEVIEW   = SettingWidgets.TYPE_SURFACEVIEW;
    private static final int TYPE_TEXTUREVIEW   = SettingWidgets.TYPE_TEXTUREVIEW;
    private static final int TYPE_NOPREVIEW     = SettingWidgets.TYPE_NOPREVIEW;

    private static final int CAMERA_TYPE_HDMI = FloatingWindowService2.ID_HDMIRX;
    private static final int CAMERA_TYPE_USB = FloatingWindowService2.ID_USBCAM;

    private static final int TEX_NAME_HDMI      = -12345;
    private static final int TEX_NAME_USBCAM    = -12346;

    public FloatingWindowView mFloatingView;
    public View mPreview;
    public SurfaceView mSurfaceView;
    public SurfaceHolder mSurfaceHolder;
    public TextureView mTextureView;
    public SurfaceTexture mSurfaceTextureForNoPreview;

    public FloatingWindowSurfaceCallback mCallback;
    public FloatingWindowTextureListener mListener;
    public WindowManager.LayoutParams wmParams;
    public Camera mCamera;
    public MediaRecorder mMediaRecorder;
    public int mCameraType;
    public int x;
    public int y;

    public int width;
    public int height;

    public boolean bPlaying = false;
    public boolean bReceiverRegistered = false;
    public final RxBroadcastReceiver mRxBroadcastReceiver = new RxBroadcastReceiver();
    public final HDCPBroadcastReceiver mHDCPBroadcastReceiver = new HDCPBroadcastReceiver();
    private int mViewType = TYPE_SURFACEVIEW;
    private boolean mRecordOn = false;
    private String mRecordPath = "";

    private Context mContext;
    private Bundle mArg;
    private WindowManager mWindowManager = null;

    public FloatingWindowViewGroup(Context c, Bundle arg){
        mContext = c;
        mArg = arg;
        mWindowManager = (WindowManager)c.getSystemService(Context.WINDOW_SERVICE);

        int winSize = mArg.getInt(SettingWidgets.KEY_SIZE);
        //width = (winSize==SettingWidgets.WINDOW_HD)?1920:640;
        //height = (winSize==SettingWidgets.WINDOW_HD)?1080:480;
        switch(winSize){
            case SettingWidgets.WINDOW_HD_1920:
                width = 1920;
                height = 1080;
                break;
            case SettingWidgets.WINDOW_HD_1280:
                width = 1280;
                height = 720;
                break;
            case SettingWidgets.WINDOW_SD:
                width = 640;
                height = 360;
                break;
        }
        mRecordOn = arg.getBoolean(SettingWidgets.KEY_RECORD);
        if(mRecordOn){
            mRecordPath = arg.getString(SettingWidgets.KEY_RECORD_PATH);
        }
        Log.d(TAG, "record enabled:"+mRecordOn+" path:"+mRecordPath);
        IntentFilter filter_hdcp = new IntentFilter("android.intent.action.HDMIRX_HDCP_STATUS");    
        mContext.registerReceiver(mHDCPBroadcastReceiver,filter_hdcp);
    }

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
    
    //hand hdcp broadcast
    private class HDCPBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {

            String action = intent.getAction();
            if(action.equals("android.intent.action.HDMIRX_HDCP_STATUS")){
                if (intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false)){
                    //showToast("HDCP on");
                    Log.d(TAG, "HDCP on");
                    if(mRecordOn&& mMediaRecorder != null)
                    {
                
                        releaseMediaRecorder();
                        mRecordOn=false;
                        showToast("record  off for HDCP on");
                 
                    }
                }else{
                    Log.d(TAG, "HDCP off");
					//showToast("HDCP off");
                }
            }
        }
    }
    
    
    private void showToast(String text){
        Toast.makeText(mContext, text, Toast.LENGTH_SHORT).show();
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

        Log.d(TAG, "playCamera width:"+width+" height:"+height);

        Camera.Size optimalSize = 
            CameraHelper.getOptimalPreviewSize(mSupportedPreviewSizes, width, height);

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
            }else if(type == TYPE_TEXTUREVIEW) {
                mCamera.setPreviewTexture(mTextureView.getSurfaceTexture());
            }else{
                Log.d(TAG,"set preview texture in NO_PREVIEW mode");
                mCamera.setPreviewTexture(mSurfaceTextureForNoPreview);
            }
        }catch(IOException e){
            e.printStackTrace();
        }

        if(mRecordOn){

            Log.d(TAG, "Start record");

            // BEGIN_INCLUDE (configure_media_recorder)
            mMediaRecorder = new MediaRecorder();

            // Step 1: Unlock and set camera to MediaRecorder
            mCamera.unlock();
            mMediaRecorder.setCamera(mCamera);

            // Step 2: Set sources
            if(mCameraType == CAMERA_TYPE_HDMI){
                mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.HDMIRX);
            }else{
                mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.CAMCORDER);
            }

            mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);

            // Step 3: Set a CamcorderProfile (requires API Level 8 or higher)
            //mMediaRecorder.setProfile(profile);
            mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.OUTPUT_FORMAT_MPEG2TS);
            // Strp 4: set output file path
            // TODO: do it in outside of the control group.
            String path = mArg.getString(SettingWidgets.KEY_RECORD_PATH);//CameraHelper.getOutputMediaFile(CameraHelper.MEDIA_TYPE_VIDEO).toString();
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
            if(mViewType!=TYPE_NOPREVIEW)
                mCamera.startPreview();
        }

        bPlaying = true;

        if(mCameraType == CAMERA_TYPE_HDMI){
            IntentFilter filter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED");
            //filter.addAction("android.hardware.usb.action.USB_CAMERA_DISCONNECTED");
            mContext.registerReceiver(mRxBroadcastReceiver,filter);
            bReceiverRegistered = true;
        }else if(mCameraType == CAMERA_TYPE_USB){
            IntentFilter filter = new IntentFilter("android.hardware.usb.action.USB_CAMERA_CONNECTED");
            filter.addAction("android.hardware.usb.action.USB_CAMERA_DISCONNECTED");
            mContext.registerReceiver(mRxBroadcastReceiver,filter);
            bReceiverRegistered = true;
        }
    }

    void releaseMediaRecorder(){
        if (mRecordOn && mMediaRecorder != null) {
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

        if(bReceiverRegistered){
            mContext.unregisterReceiver(mRxBroadcastReceiver);
        }
            mContext.unregisterReceiver(mHDCPBroadcastReceiver);
        if(mRecordOn && mMediaRecorder != null){
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

        mViewType = type;

        if(type == TYPE_SURFACEVIEW){
            mSurfaceView = new SurfaceView(c);
            mSurfaceHolder = mSurfaceView.getHolder();
            mCallback = new FloatingWindowSurfaceCallback();
            mSurfaceHolder.addCallback(mCallback);
            mPreview = mSurfaceView;
        }else if(type == TYPE_TEXTUREVIEW) {
            mTextureView = new TextureView(c);
            mListener = new FloatingWindowTextureListener();
            mTextureView.setSurfaceTextureListener(mListener);
            mPreview = mTextureView;
        }else{
            Log.d(TAG, "ViewType is set to NO_PREVIEW");
            int name = (mCameraType==CAMERA_TYPE_HDMI)?TEX_NAME_HDMI:TEX_NAME_USBCAM;
            mSurfaceTextureForNoPreview = new SurfaceTexture(name);
            playCamera(mViewType);
        }
    }

    public void attachPreview(){
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        if(mViewType != FloatingWindowViewGroup.TYPE_NOPREVIEW){
            mPreview.setLayoutParams(param);
            mFloatingView.addView(mPreview);
        }
    }

}
