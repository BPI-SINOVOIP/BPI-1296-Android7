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
import android.os.ParcelFileDescriptor;
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
import android.os.AsyncTask;

import java.io.PrintWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.FileNotFoundException;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileWriter;
import java.io.FileOutputStream;
import java.util.List;
import java.io.IOException;
import java.net.MulticastSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.RtkHDMIRxManager.Size;
import com.realtek.hardware.InvalidOpException;
import com.realtek.hardware.HDMIRxStatus;
import com.realtek.hardware.HDMIRxParameters;

/**
 * Descriptions:
 *  the control class of a floating Window
 *  this is the view group to demo RtkHDMIRxManager
 */
public class FloatingWindowViewGroup3 {

    private static final String TAG="FloatingWindowViewGroup3";

    private static final boolean PING_PONG_RECORD_RESOLUTION = false;

    private static final int PING_PONG_RECORD_WIDTH     = 1280;
    private static final int PING_PONG_RECORD_HEIGHT    = 720;

    private static final int TYPE_SURFACEVIEW   = SettingWidgets.TYPE_SURFACEVIEW;
    private static final int TYPE_TEXTUREVIEW   = SettingWidgets.TYPE_TEXTUREVIEW;
    private static final int TYPE_NOPREVIEW     = SettingWidgets.TYPE_NOPREVIEW;

    private static final int TEX_NAME_HDMI_2    = -12348;

    private static int HDMIRxAudioSource = 9;

    public FloatingWindowView mFloatingView;
    public View mPreview;
    public SurfaceView mSurfaceView;
    public SurfaceHolder mSurfaceHolder;
    public TextureView mTextureView;
    public SurfaceTexture mSurfaceTextureForNoPreview;

    public FloatingWindowSurfaceCallback mCallback;
    public FloatingWindowTextureListener mListener;
    public WindowManager.LayoutParams wmParams;

    // to replace android camera interface.
    public RtkHDMIRxManager mHDMIRX;
    public int mWidth = 0;
    public int mHeight = 0;
    public int mFps = 0;
    public int mScanMode = 0;

    public boolean bPlaying = false;
    public boolean bReceiverRegistered = false;
    public final RxBroadcastReceiver mRxBroadcastReceiver = new RxBroadcastReceiver();
    public final HDCPBroadcastReceiver mHDCPBroadcastReceiver = new HDCPBroadcastReceiver();
    private int mViewType = TYPE_TEXTUREVIEW;
    private boolean mRecordOn = false;
    private File mRecordFile = null;

    private Context mContext;
    private Bundle mArg;
    private WindowManager mWindowManager = null;

    private int mOutputPath;

    /**
     * udp streaming related variables
     */
    private MulticastSocket multicastSocket;
    private String mUdpIp = "239.0.0.1";
    private int mUdpPort = 7890;
    // pipe
    private ParcelFileDescriptor[] mPipe;
    private ParcelFileDescriptor mReadPfd = null;
    private ParcelFileDescriptor mWritePfd = null;
    private FileOutputStream mFileOutputStream;
    private boolean mLocalSocketTaskAlive;
    private byte[] mPipeBuffer = new byte[4096];
    private boolean mProcessingUDPSocket;
    private ParcelFileDescriptor mUdpSocketPfd;

    /**
     * If mForceFps is set to true
     * 1. HDMIRx would drop frames to 30 fps if src fps is 60
     * 2. HDMIRx would not drop frames if src fps is 24 or values not 60fps
     *
     * If mForceFps is set to false
     * 1. HDMIRx would not convert fps
     */
    private boolean mForceFps = false;

    /**
     * If mRecResolutionMode is set to 1080p, then recorder would only record 1080p content if src is 4k2k
     */
    private int mRecResolutionMode = SettingWidgets.REC_4K2K;

    /**
     * If mForcePreview1080p is set to true, then preview would forced to 1080p if src tv system is 4k2k
     */
    private boolean mForcePreview1080p = false;

    private int mExitBehavior = SettingWidgets.EXIT_TO_PIP;
    private boolean mActivityIsOn = false;
    private int mWinSize;
    private int mToggleRecordCounter;

    private int mOutputFormat = RtkHDMIRxManager.HDMIRX_FILE_FORMAT_TS;

    public FloatingWindowViewGroup3(Context c, Bundle arg){
        Log.d(TAG, "HDMIRxStatus output test:"+HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS);
        mContext = c;
        mArg = arg;
        mWindowManager = (WindowManager)c.getSystemService(Context.WINDOW_SERVICE);
        mForceFps = arg.getBoolean(SettingWidgets.KEY_FORCE_FPS);
        mWinSize = mArg.getInt(SettingWidgets.KEY_SIZE);
        mRecResolutionMode = mArg.getInt(SettingWidgets.KEY_REC_RESOLUTION);
        mForcePreview1080p = mArg.getBoolean(SettingWidgets.KEY_PREVIEW_RESOLUTION);
        mExitBehavior = mArg.getInt(SettingWidgets.KEY_EXIT_BEHAVIOR);
        mActivityIsOn = true;
        mToggleRecordCounter = 0;
        mOutputPath = arg.getInt(SettingWidgets.KEY_OUTPUT_PATH);

        mOutputFormat = RtkHDMIRxManager.HDMIRX_FILE_FORMAT_TS;

        int format = arg.getInt(SettingWidgets.KEY_OUTPUT_FORMAT);
        if(format == SettingWidgets.FORMAT_MP4){
            mOutputFormat = RtkHDMIRxManager.HDMIRX_FILE_FORMAT_MP4;
        }
        
        IntentFilter filter_hdcp = new IntentFilter("android.intent.action.HDMIRX_HDCP_STATUS");    
        mContext.registerReceiver(mHDCPBroadcastReceiver,filter_hdcp);
    }
    
    private class HDCPBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(action.equals("android.intent.action.HDMIRX_HDCP_STATUS")){
                if (intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false)){
                    //showToast("HDCP on");
                    Log.d(TAG, "HDCP on");
                    if(mRecordOn)
                    {
                        toggleRecord();
                        showToast("record  off for HDCP on");
                    }
                }else{
                    Log.d(TAG, "HDCP off");
                    //showToast("HDCP off");
                }
            }
        }
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
                releaseCamera();
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
                releaseCamera();
            }
    }

    private void releaseCamera(){

        if(mHDMIRX!=null){

            if(mRecordOn){
                try{
                    mHDMIRX.setTranscode(false);
                }catch(InvalidOpException e){
                    e.printStackTrace();
                }
                SystemClock.sleep(1000);
                mRecordOn = false;
            }

            mHDMIRX.stop();
            mHDMIRX.release();
            mHDMIRX = null;
        }

        bPlaying = false;
    }

    void resumePlayCamera(){
        Log.d(TAG, "resume play Camera RtkHDMIRxManager");
        playCamera(mViewType);
    }

    // to replace playCamera..
    void playCamera(int type){
        mHDMIRX = new RtkHDMIRxManager();
        HDMIRxStatus rxStatus = mHDMIRX.getHDMIRxStatus();
        if(rxStatus!=null && rxStatus.status == HDMIRxStatus.STATUS_READY){
            // open HDMIRx
            if(mHDMIRX.open()!=0){
                Log.e(TAG, "open HDMIRx failed");
                mHDMIRX = null;
                return;
            }
            HDMIRxParameters hdmirxGetParam = mHDMIRX.getParameters();
            getSupportedPreviewSize(hdmirxGetParam, rxStatus.width, rxStatus.height);
            mFps=getSupportedPreviewFrameRate(hdmirxGetParam);
            mScanMode=rxStatus.scanMode;

            showToast("preview info "+mWidth+"x"+mHeight+" "+mFps+"fps");
        }else{
            Log.e(TAG, "HDMIRx not ready");
            mHDMIRX = null;
            return;
        }

        // set surface
        try{

            if(type == TYPE_SURFACEVIEW){
                mHDMIRX.setPreviewDisplay(mSurfaceHolder);
            }else if(type == TYPE_TEXTUREVIEW){
                SurfaceTexture surfaceTexture = mTextureView.getSurfaceTexture();
                mHDMIRX.setPreviewDisplay3(surfaceTexture);
            }else{
                mHDMIRX.setPreviewDisplay3(mSurfaceTextureForNoPreview);
            }

            // configureTargetFormat
            HDMIRxParameters hdmirxParam = new HDMIRxParameters();
            hdmirxParam.setPreviewSize(mWidth, mHeight);
            hdmirxParam.setPreviewFrameRate(mFps);
            // set sorce format
            mHDMIRX.setParameters(hdmirxParam);
            // configureTargetFormat end
            mHDMIRX.play();
            Log.d(TAG, "play HDMI RX");
        }catch (IOException e){
            e.printStackTrace();
            releaseCamera();
            return;
        }

        bPlaying = true;

        // register BroadcastReceiver.
        IntentFilter filter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED"); 
        mContext.registerReceiver(mRxBroadcastReceiver,filter);
        bReceiverRegistered = true;
    }

    void releaseMediaRecorder(){
        // Do nothing. maybe add some code later..
    }

    void destroy(){

        if(bReceiverRegistered){
            mContext.unregisterReceiver(mHDCPBroadcastReceiver);
            mContext.unregisterReceiver(mRxBroadcastReceiver);
        }

        releaseCamera();
        mLocalSocketTaskAlive = false;
        new DatagramSocketControlTask().execute(null, null, null);
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
            int name = TEX_NAME_HDMI_2;
            mSurfaceTextureForNoPreview = new SurfaceTexture(name);
            playCamera(mViewType);
        }
    }

    public void attachPreview(){
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        if(mViewType != FloatingWindowViewGroup3.TYPE_NOPREVIEW){
            mPreview.setLayoutParams(param);
            mFloatingView.addView(mPreview);
        }
    }

    public void getSupportedPreviewSize(HDMIRxParameters hdmirxGetParam, int rxWidth, int rxHeight) {
        List<Size> previewSizes = hdmirxGetParam.getSupportedPreviewSizes();
        int retWidth=0, retHeight=0;
        if(previewSizes==null || previewSizes.size()<=0)
            return;
        for(int i=0;i<previewSizes.size();i++) {
            if(previewSizes.get(i)!=null && rxWidth==previewSizes.get(i).width) {
                retWidth=previewSizes.get(i).width;
                retHeight=previewSizes.get(i).height;
                if(rxHeight==previewSizes.get(i).height)
                    break;
            }
        }
        if(retWidth==0 && retHeight==0) {
            if(previewSizes.get(previewSizes.size()-1)!=null) {
                retWidth=previewSizes.get(previewSizes.size()-1).width;
                retHeight=previewSizes.get(previewSizes.size()-1).height;
            }
        }

        if(mForcePreview1080p){
            if(retWidth>1920)
                retWidth = 1920;
            if(retHeight>1080)
                retHeight = 1080;
        }

        Log.d(TAG, "getSupportedPreviewSize width:"+retWidth+" height:"+retHeight);

        mWidth = retWidth;
        mHeight = retHeight;
    }

    public int getSupportedPreviewFrameRate(HDMIRxParameters hdmirxGetParam) {
        List<Integer> previewFrameRates = hdmirxGetParam.getSupportedPreviewFrameRates();
        int fps=0;
        if(previewFrameRates!=null && previewFrameRates.size()>0)
            fps = previewFrameRates.get(previewFrameRates.size()-1);
        else
            fps = 30;

        Log.d(TAG, "getSupportedPreviewFrameRate fps:"+fps);
        if(fps == 60 && mForceFps){
            fps = 30;
        }

        if(fps == 50 && mForceFps){
            fps = 25;
        }

        Log.d(TAG, "fps after calculate:"+fps);

        return fps;
    }

    class LocalSocketReadTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids){
            Log.d(TAG, "LocalSocketReadTask doInBackground..");
            // create InputStream from mReadPfd.
            InputStream ips = new ParcelFileDescriptor.AutoCloseInputStream(mReadPfd);
            FileDescriptor fd = null;
            try{
                fd = mFileOutputStream.getFD();
            }catch(IOException e){
                e.printStackTrace();
            }

            while(mLocalSocketTaskAlive){
                // read from pipe read pfd
                try{
                    int data = ips.available();

                    if(data>0){
                        int readSize = ips.read(mPipeBuffer);
                        if(readSize>0){
                            if(fd != null && fd.valid()){
                                mFileOutputStream.write(mPipeBuffer,0,readSize);
                            }
                        }
                    }
                }catch(IOException e){
                    e.printStackTrace();
                    //mLocalSocketTaskAlive = false;
                    fd = null;
                }
            }

            try{
                // always close FileOutputStream
                Log.d(TAG, "close file");
                mFileOutputStream.close();

                /*
                Log.d(TAG, "close read socket");
                mReadLocalSocket.close();
                SystemClock.sleep(50);
                Log.d(TAG, "close serversocket");
                mLocalServerSocket.close();
                */

                Log.d(TAG, "close pipe");
                mWritePfd.close();
                mReadPfd.close();

            }catch(IOException e){
                e.printStackTrace();
                return false;
            }
            return true;
        }
        @Override
        protected void onPostExecute(Boolean result){
            Log.d(TAG, "LocalSocketReadTask onPostExecute");
        }
    }

    class DatagramSocketControlTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids) {
            Log.d(TAG, "DatagramSocketControlTask doInBackground");
            if(multicastSocket!=null){
                multicastSocket.disconnect();
                multicastSocket.close();
                multicastSocket = null;
            }
            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
        }
    }

    class DatagramSocketCreateTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids) {
            ParcelFileDescriptor fd = createUdpFd();
            Log.d(TAG, "DatagramSocketCreateTask doInBackground done fd:"+fd);
            mProcessingUDPSocket=false;
            if(fd!=null)
                return true;
            else
                return false;
        }
        @Override
        protected void onPostExecute(Boolean result) {
            Log.d(TAG, "DatagramSocketCreateTask set mProcessingUDPSocket to "+mProcessingUDPSocket);
        }
    }

    /**
     * Consider that network IO has protential broken issue,
     * we use a pipe to redirect network I/O
     */
    private ParcelFileDescriptor createUdpFd(){

        // create pipe
        try{
            mPipe = ParcelFileDescriptor.createPipe();
        }catch(IOException e){
            e.printStackTrace();
            return null;
        }

        // assign read/write fd
        mReadPfd = mPipe[0];
        mWritePfd = mPipe[1];

        try{
            multicastSocket = new MulticastSocket(mUdpPort);
            multicastSocket.setBroadcast(true);
            multicastSocket.connect(InetAddress.getByName(mUdpIp),mUdpPort);
            mUdpSocketPfd = ParcelFileDescriptor.fromDatagramSocket(multicastSocket);

            mFileOutputStream = new FileOutputStream(mUdpSocketPfd.getFileDescriptor());
            Log.d(TAG, "udp fd create complete");
            return mWritePfd;
        }catch(SocketException e){
            e.printStackTrace();
            return null;
        }catch(UnknownHostException e){
            e.printStackTrace();
            return null;
        }catch(IOException e){
            e.printStackTrace();
            return null;
        }
    }

    private ParcelFileDescriptor createLocalFd(){

        String extension = ".ts";
        if(mOutputFormat == RtkHDMIRxManager.HDMIRX_FILE_FORMAT_MP4){
            extension = ".mp4";
        }

        mRecordFile = CameraHelper.getOutputMediaFile(CameraHelper.MEDIA_TYPE_VIDEO, "RtkHDMIRxManager", extension);
        try{
            int mode = ParcelFileDescriptor.MODE_CREATE|ParcelFileDescriptor.MODE_READ_WRITE;
            ParcelFileDescriptor pfd = ParcelFileDescriptor.open(mRecordFile,mode);
            return pfd;
        }catch(FileNotFoundException e){
            e.printStackTrace();
            return null;
        }
    }

    public void toggleRecord(){

        if(mHDMIRX!=null){
            int currentAudioMode = mHDMIRX.getAudioMode();
            Log.d(TAG, "currentAudioMode:"+currentAudioMode);
            if(currentAudioMode == RtkHDMIRxManager.HDMIRX_AUDIO_RAW){
                showToast("can not record under RAW mode");
                return;
            }
        }

        if(!mRecordOn){
            if(mHDMIRX!=null){
                try{
                    ParcelFileDescriptor pfd;
                    if(mOutputPath == SettingWidgets.OUTPUT_STORAGE){
                        pfd = createLocalFd();
                    }else{
                        mProcessingUDPSocket = true;
                        new DatagramSocketCreateTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);

                        while(mProcessingUDPSocket){
                            SystemClock.sleep(100);
                        }

                        pfd = mWritePfd;

                        if(pfd!=null){
                            mLocalSocketTaskAlive = true;
                            new LocalSocketReadTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                        }
                    }

                    if(pfd == null){
                        Log.e(TAG, "create fd error");
                        return;
                    }

                    Log.d(TAG, "Current preview size :"+mWidth+","+mHeight);
                    // video parameters
                    int w = mWidth;
                    int h = mHeight;
                    int videoBitrate = 9000000;
                    // audio parameters
                    int channelCount = 2;
                    int sampleRate = 48000;
                    int audioBitrate = 192000;

                    Log.d(TAG, "org w:"+w+" h:"+h);

                    if(mRecResolutionMode == SettingWidgets.REC_1080P){
                        if(w>1920) w = 1920;
                        if(h>1080) h = 1080;
                    }

                    if(PING_PONG_RECORD_RESOLUTION){
                        if(mToggleRecordCounter%2==0){
                            w = PING_PONG_RECORD_WIDTH;
                            h = PING_PONG_RECORD_HEIGHT;
                        }
                        mToggleRecordCounter=mToggleRecordCounter+1;
                    }

                    Log.d(TAG, "set record resolution to "+w+","+h);

                    RtkHDMIRxManager.VideoConfig vConfig =
                        new RtkHDMIRxManager.VideoConfig(w,h,videoBitrate);
                    RtkHDMIRxManager.AudioConfig aConfig =
                        new RtkHDMIRxManager.AudioConfig(channelCount,sampleRate,audioBitrate);
                    mHDMIRX.configureTargetFormat(vConfig, aConfig);
                    if(mOutputFormat == RtkHDMIRxManager.HDMIRX_FILE_FORMAT_TS){
                        Log.d(TAG, "output format : ts");
                    }else{
                        Log.d(TAG, "output format : mp4");
                    }
                    mHDMIRX.setTargetFd(pfd, mOutputFormat);
                    mHDMIRX.setTranscode(true);
                    mRecordOn = true;
                    Log.d(TAG, "set record on");
                    showToast("record on :"+w+"x"+h+" fps:"+mFps);
                }catch(InvalidOpException e){
                    e.printStackTrace();
                    mRecordOn = false;
                }
            }
        }else{
            if(mHDMIRX!=null){
                Log.d(TAG, "set record off");
                showToast("record off");
                try{
                    mHDMIRX.setTranscode(false);
                }catch(InvalidOpException e){
                    e.printStackTrace();
                }
                mRecordOn = false;
                mLocalSocketTaskAlive = false;
                new DatagramSocketControlTask().execute(null, null, null);
            }
        }
    }

    private void updateSurfaceVisibility(boolean on){
        mFloatingView.setVisibile2(on);
    }

    private void showToast(String text){
        Toast.makeText(mContext, text, Toast.LENGTH_SHORT).show();
    }

    private void showHideView(boolean show){
        if(show){
            updateSurfaceVisibility(true);
            if(mHDMIRX!=null){
                mHDMIRX.setPlayback(true,true);
            }
        }else{
            if(mHDMIRX!=null){
                mHDMIRX.setPlayback(false,false);
            }
            updateSurfaceVisibility(false);
        }
    }

    private void updateViewSize(int w, int h){
        mFloatingView.updateSize(w,h);
    }

    public void togglePreview(){

        Log.d(TAG, "togglePreview");
        if(mFloatingView.mVisible){
            Log.d(TAG, "hide preview");
            showHideView(false);
        }else{
            Log.d(TAG, "show preview");
            showHideView(true);
        }
    }

    public void onActivityPauseResume(boolean resume){
        if(!resume){
            if(mExitBehavior==SettingWidgets.EXIT_TO_HIDE){
                showHideView(false);
            }else{
                if(mWinSize==SettingWidgets.WINDOW_HD && mFloatingView.mVisible){
                    updateViewSize(576,324);
                }
            }
        }else{
            if(mExitBehavior==SettingWidgets.EXIT_TO_HIDE){
                showHideView(true);
            }else{
                if(mWinSize==SettingWidgets.WINDOW_HD && mFloatingView.mVisible){
                    updateViewSize(1920,1080);
                }
            }
        }
    }

}
