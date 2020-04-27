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

import java.io.PrintWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.util.List;
import java.io.IOException;

public class BackgroundRecordService extends Service {

    private static final String TAG = "BackgroundRecordService";

    private static int HDMIRxAudioSource = 9;

    public static boolean bServiceIsAlive = false;

    private FloatingWindowView mView = null;
    private TextureView mTextureView;
    private SurfaceTexture mTexture;

    private WindowManager mWindowManager = null;
    private WindowManager.LayoutParams wmParams = null;

    private final IBinder mBinder = new LocalBinder();

    private Camera mCamera;
    private MediaRecorder mMediaRecorder;

    private static BackgroundRecordService mInstance;
    private static boolean bIsShow = false;

    private int texName = -12345;
    public class LocalBinder extends Binder {
        BackgroundRecordService getService() {
            return BackgroundRecordService.this;
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
        mInstance = this;
    }

    private void releaseCamera(){
        if(mCamera != null){
            mCamera.release();
            mCamera = null;
        }
    }

    public static void toggleShowView(boolean show){

        if(mInstance == null)
            return;

        if(show){
            if(mInstance.mView == null){
                // show floating window
                mInstance.createFloatingWindow();
            }
        }else{
            if(mInstance.mView != null){
                // remove floating window
                mInstance.removeFloatingWindow();
                //mInstance.texName--;
                //mInstance.mTexture.attachToGLContext(mInstance.texName);
            }
        }
    }

    private void removeFloatingWindow(){
        if(mWindowManager != null && mView != null){
            mWindowManager.removeView(mView);
        }
        mWindowManager = null;
        mView = null;
    }

    private void showFloatingWindow(){
        if(mWindowManager != null && mView != null){
            int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

            WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                192*3,//WindowManager.LayoutParams.MATCH_PARENT,
                108*3,//WindowManager.LayoutParams.MATCH_PARENT,
                FloatingWindowView.VIEW_TYPE,
                flag,
                PixelFormat.TRANSLUCENT);

            params.x = 200;
            params.y = 10;
            params.gravity = Gravity.TOP|Gravity.LEFT;

            mWindowManager.addView(mView, params);
            bIsShow = true;
        }
    }

    private void hideFloatingWindow(){
        if(mWindowManager != null && mView != null){
            mWindowManager.removeView(mView);
            bIsShow = false;
        }
    }

    private void createFloatingWindow(){
        Log.d(TAG, "createFloatingWindow");

        mWindowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        LayoutInflater li = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mView = (FloatingWindowView) li.inflate(R.layout.floatingwindow,null);

        mTexture.detachFromGLContext();

        mTextureView = new TextureView(this);
        mTextureView.setSurfaceTexture(mTexture);

        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        mTextureView.setLayoutParams(param);
        mView.setBackgroundColor(Color.RED);
        mView.addView(mTextureView);

        bIsShow = false;
        int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
            //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        wmParams = new WindowManager.LayoutParams(
            192*3,//WindowManager.LayoutParams.MATCH_PARENT,
            108*3,//WindowManager.LayoutParams.MATCH_PARENT,
            FloatingWindowView.VIEW_TYPE,
            flag,
            PixelFormat.TRANSLUCENT);

        wmParams.x = 200;
        wmParams.y = 10;
        wmParams.gravity = Gravity.TOP|Gravity.LEFT;

        mWindowManager.addView(mView, wmParams);
    }

    private void recordCamera(){
        mCamera = CameraHelper.getDefaultCameraInstance();
        Camera.Parameters parameters = mCamera.getParameters();
        List<Camera.Size> mSupportedPreviewSizes = parameters.getSupportedPreviewSizes();
        Camera.Size optimalSize = CameraHelper.getOptimalPreviewSize(mSupportedPreviewSizes, 1920, 1080);

        int previewFrameRate = parameters.getPreviewFrameRate();
        int encodeBitRate = 5000000;

        int audioChannels = 2;
        int audioSampleRate = 48000;
        int audioBitRate = 64000;

        parameters.setPreviewSize(optimalSize.width, optimalSize.height);
        mCamera.setParameters(parameters);

        mTexture = new SurfaceTexture(texName);

        try{
            mCamera.setPreviewTexture(mTexture);
        }catch(IOException e){
            e.printStackTrace();
        }

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
    }

    private void releaseMediaRecorder(){
        if (mMediaRecorder != null) {
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

    @Override
    public int onStartCommand (Intent intent, int flags, int startId) {
        Slog.d(TAG , "service onStartCommand");
        recordCamera();
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        Slog.d(TAG, "service destroy");

        if(mMediaRecorder != null){
            mMediaRecorder.stop();
            releaseMediaRecorder();
            mCamera.lock();
        }

        releaseCamera();

        removeFloatingWindow();

        if(mTexture!=null){
            mTexture.release();
            mTexture = null;
        }

        super.onDestroy();
        bServiceIsAlive = false;
    }

    public static long getCurrentThreadID(){
        long threadId = Thread.currentThread().getId();
        return threadId;
    }
}
