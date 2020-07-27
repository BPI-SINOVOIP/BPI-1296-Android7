package com.example.hdmirxdemo;

import android.app.Service;
import android.app.Dialog;
import android.os.IBinder;
import android.os.Binder;
import android.os.Bundle;
import android.content.Intent;
import android.content.DialogInterface;
import android.util.Log;
import android.util.Slog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.SystemClock;
import android.os.ParcelFileDescriptor;
import android.view.View;
import android.view.KeyEvent;
import android.view.Window;
import android.widget.TextView;
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
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.storage.StorageManager;
import android.os.storage.VolumeInfo;
import android.os.PowerManager;

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
import java.text.SimpleDateFormat;
import java.util.Date;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

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
public class FloatingWindowViewGroup3 implements RtkHDMIRxManager.HDMIRxListener {

    private static final boolean DEBUG_VERBOSE = Keys.FLT_WIN_VG_DEBUG_VERBOSE;
    private static final boolean DEBUG_VERBOSE2 = true;

    private static final String HDCP_CONTROL_FILE = "/sys/devices/platform/98037000.hdmirx/hdcp_status";

    private static final String TAG="FloatingWindowViewGroup3";
    private static final String TAG_VERBOSE = TAG+"-VERBOSE";
    private static final String TAG_VERBOSE_2 = TAG+"-VERBOSE2";

    private static final int TYPE_SURFACEVIEW   = Keys.TYPE_SURFACEVIEW;
    private static final int TYPE_TEXTUREVIEW   = Keys.TYPE_TEXTUREVIEW;
    private static final int TYPE_NOPREVIEW     = Keys.TYPE_NOPREVIEW;

    private static final int TEX_NAME_HDMI_2    = -12348;

    private static final boolean SHOW_TOAST=false;

    private static int HDMIRxAudioSource = 9;

    private static final long FREE_SPACE_SAVE_THRESHOLD = 50*1024*1024; // 50M

    private static final Object sCameraLock = new Object();

    private final Handler mHandler = new Handler();

    public FloatingWindowView mFloatingView;
    public RecordInfoWindowView mRecordInfoView;
    public View mPreview;
    public SurfaceView mSurfaceView;
    public SurfaceHolder mSurfaceHolder;
    public TextureView mTextureView;
    public SurfaceTexture mSurfaceTextureForNoPreview;

    public FloatingWindowSurfaceCallback mCallback;
    public FloatingWindowTextureListener mListener;
    public WindowManager.LayoutParams wmParams;

    // to replace android camera interface.
    //public RtkHDMIRxManager mHDMIRX;
    public int mWidth = 0;
    public int mHeight = 0;
    public int mFps = 0;
    public int mScanMode = 0;

    private static boolean bPlaying = false;
    //public boolean bReceiverRegistered = false;
    //public final RxBroadcastReceiver mRxBroadcastReceiver = new RxBroadcastReceiver();
    //public final HDCPBroadcastReceiver mHDCPBroadcastReceiver = new HDCPBroadcastReceiver();
    private int mViewType = TYPE_SURFACEVIEW;
    private boolean mRecordOn = false;
    private File mRecordFile = null;
    private String mOutputFolderPath = null;

    private Context mContext;
    private Bundle mArg;
    private WindowManager mWindowManager = null;

    private boolean mVideoOn;
    private boolean mAudioOn;
    private int     moutputMode = Keys.OUTPUT_OFF;   // udp, save to file

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
    private byte[] mPipeBuffer = new byte[8192];
    private boolean mProcessingUDPSocket;
    private ParcelFileDescriptor mUdpSocketPfd = null;
    private ParcelFileDescriptor mLocalFilePfd = null;

    // direct I/O
    private ParcelFileDescriptor mLocalFilePfdNoPipe = null;
    private boolean mFileCheckTaskAlive;

    private Dialog mWarningDialog;

    private static Dialog mPendingWarningOnDestory;
    private static Dialog mPendingRxWarningOnDestory;

    private RelativeLayout mFocusOverlay;
    private boolean mFocusShowing;
    private boolean mIsValidateInstance;

    /**
     * If mForcePreview1080p is set to true, then preview would forced to 1080p if src tv system is 4k2k
     */
    //private boolean mForcePreview1080p = false;

    private boolean mActivityIsOn = false;
    private int mWinSize;

    private Calendar mRecordStartTime;
    //private int mRecordDurationMin;
    private long mRecordEndTimeMillis;
    private RecInfoUpdateRunnable mRunnable = new RecInfoUpdateRunnable();
    private boolean mHDCP = false;
    private PowerManager.WakeLock mWakeLock = null;

    class RecInfoUpdateRunnable implements Runnable{
        public void run() {
            if(isRecording() && mRecordInfoView != null) {

                Calendar calendar = new GregorianCalendar();
                Date trialTime = new Date();
                calendar.setTime(trialTime);
                
                long startTimeMillis = mRecordStartTime.getTimeInMillis();
                long nowTimeMillis = calendar.getTimeInMillis();

                int diff = (int) (nowTimeMillis-startTimeMillis)/1000;
                int sec = diff%60;
                int min = (diff/60)%60;
                int hour = diff/3600;

                String text = /*createRecDisplayString()+" "+*/ createDurationString(hour,min,sec);
                mRecordInfoView.setText(text);
                mHandler.postDelayed(mRunnable,1000);

                if(mRecordEndTimeMillis > 0){
                    long actualEndTimeMillis = mRecordEndTimeMillis - 2000;
                    if(nowTimeMillis>=actualEndTimeMillis){
                        stopRecord();

                        // then check if service should keep alive?
                        checkAndStopService();
                    }
                }
            }
        }
    }

    public FloatingWindowViewGroup3(Context c, Bundle arg){

        if(DEBUG_VERBOSE2){
            Log.d(TAG_VERBOSE_2,"Create FloatingWindowViewGroup3 this:"+this);
        }

        //Log.d(TAG, "HDMIRxStatus output test:"+HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS);
        //acquireWakeLock(c);
        mContext = c;
        mArg = arg;
        mWindowManager = (WindowManager)c.getSystemService(Context.WINDOW_SERVICE);
        mActivityIsOn = true;
        mVideoOn = arg.getBoolean(Keys.KEY_VIDEO_ON,true);
        mAudioOn = arg.getBoolean(Keys.KEY_AUDIO_ON,true);

        mWarningDialog = null;
        mIsValidateInstance = true;

        mHDCP = readHDCPCurrentState();
        LayoutInflater li = (LayoutInflater) c.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mRecordInfoView = (RecordInfoWindowView) li.inflate(R.layout.record_info_window,null);
    }

    public void acquireWakeLock(Context c){
        if(mWakeLock == null) {
            Log.d(TAG,"acquireWakeLock");
            PowerManager pm = (PowerManager)c.getSystemService(Context.POWER_SERVICE);
            mWakeLock = pm.newWakeLock(PowerManager.RTK_WAKE_LOCK, "HDMIRx.ViewGroup");
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

    private boolean readHDCPCurrentState(){
        try{
            BufferedReader br = new BufferedReader(new FileReader(HDCP_CONTROL_FILE));
            String line;
            while((line=br.readLine()) != null){
                Log.i(TAG, "HDCP current state:"+line);
                if(line.contains("None")){
                    br.close();
                    return false;
                }
            }
            br.close();
            return true;
        } catch (FileNotFoundException e){
            Slog.e(TAG, "Read FileNotFoundException "+HDCP_CONTROL_FILE);
            return false;
        } catch (IOException e) {
            Slog.e(TAG, "Read IOException "+HDCP_CONTROL_FILE);
            return false;
        }
    }

    /*
    private class HDCPBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(action.equals("android.intent.action.HDMIRX_HDCP_STATUS")){
                if (intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false)){
                    //showToast("HDCP on");
                    Log.d(TAG, "HDCP on");
                    if(mRecordOn)
                    {
                        stopRecord();
                        showToast("record  off for HDCP on");
                    }
                }else{
                    Log.d(TAG, "HDCP off");
                    //showToast("HDCP off");
                }
            }
        }
    }
    */

    public void handleHDCPBroadcast(Intent intent) {
        String action = intent.getAction();
        if(action.equals("android.intent.action.HDMIRX_HDCP_STATUS")){
            if (intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_HDCP_STATUS, false)){
                //showToast("HDCP on");
                Log.d(TAG, "HDCP on");
                if(mRecordOn)
                {
                    stopRecord();
                    //showToast("record off for HDCP on");
                    dismissWarningDialog();
                    showWarningDialog("Record stopped, HDCP content detected.");
                }
                mHDCP = true;
            }else{
                Log.d(TAG, "HDCP off");
                //showToast("HDCP off");
                mHDCP = false;
            }
        }
    }

    public void handleHotplugBroadcast(Intent intent){
        String action = intent.getAction();
        boolean bPlugOut = false;
        if(action.equals("android.intent.action.HDMIRX_PLUGGED")){
            if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                bPlugOut = true;
            }else{
                bPlugOut = false;
            }
        }

        if(bPlugOut){
            FloatingWindowService2.stopServiceItSelf();
            //releaseCamera();
        }else{
            // suppose not reach here
            /*
            if(!bPlaying){
                resumePlayCamera();
            }
            */
        }
    }

    /*
    // handle broadcast
    private class RxBroadcastReceiver extends BroadcastReceiver {
        public void onReceive(Context context, Intent intent) {

            String action = intent.getAction();
            boolean bPlugOut = false;
            if(action.equals("android.intent.action.HDMIRX_PLUGGED")){
                if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                    bPlugOut = true;
                }else{
                    bPlugOut = false;
                }
            }

            if(bPlugOut){
                releaseCamera();
            }else{
                if(!bPlaying){
                    resumePlayCamera();
                }
            }
        }
    }
    */

    class FloatingWindowTextureListener implements TextureView.SurfaceTextureListener {
        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
            Log.d(TAG, "onSurfaceTextureAvailable");
            playCameraLocked(mViewType);
        }

        @Override
        public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        }

        @Override
        public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
            releaseCameraLocked();
            return true;
        }

        @Override
        public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        }
    }

    class FloatingWindowSurfaceCallback implements SurfaceHolder.Callback {

        FloatingWindowViewGroup3 mParent;

        FloatingWindowSurfaceCallback(FloatingWindowViewGroup3 parent){
            mParent = parent;
        }

        @Override
        public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
        }

        @Override
        public void surfaceCreated(SurfaceHolder arg0) {
            Log.d(TAG, "SurfaceCreated, start playback this:"+this+" validate state:"+mIsValidateInstance);

            if(DEBUG_VERBOSE2){
                Log.d(TAG_VERBOSE_2,"SurfaceCreated - FloatingWindowViewGroup3 :"+mParent);
            }

            if(mIsValidateInstance){
                playCameraLocked(mViewType);
            }
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder arg0) {
            Log.d(TAG, "surfaceDestroyed, release camera this:"+this);
            releaseCameraLocked();
        }
    }

    private void releaseCameraLocked(){
        synchronized(sCameraLock){

            if(DEBUG_VERBOSE2){
                Log.d(TAG_VERBOSE_2,"releaseCameraLocked this:"+this);
            }


            long threadId = Thread.currentThread().getId();
            Log.d(TAG, "tid:["+threadId+"] Call releaseCamera with lock:"+sCameraLock);
            releaseCamera();
        }
    }

    private void releaseCamera(){
        if(mRecordOn){
            /*
            try{
                mHDMIRX.setTranscode(false);
            }catch(InvalidOpException e){
                e.printStackTrace();
            }
            SystemClock.sleep(1000);
            mRecordOn = false;
            */
            stopRecord();
        }

        /* just call release is okay.
        if(HDMIRxManagerController.isOpened()){
            Log.d(TAG, "release - stop start");
            if(bPlaying){
                HDMIRxManagerController.stop();
            }
            Log.d(TAG, "release - release start");
        }
        */
        HDMIRxManagerController.release();
        Log.d(TAG, "release - release done");

        bPlaying = false;
    }

    void resumePlayCamera(){
        Log.d(TAG, "resume play Camera HDMIRxManager");
        playCameraLocked(mViewType);
    }

    boolean needDownScaleFPS(){
        // TODO
        return true;
    }

    private void playCameraLocked(int type){
        synchronized(sCameraLock){
            long threadId = Thread.currentThread().getId();

            if(DEBUG_VERBOSE2){
                Log.d(TAG_VERBOSE_2,"playCameraLocked this:"+this);
            }

            Log.d(TAG, "tid:["+threadId+"] call playCamera in playCameraLocked lock:"+sCameraLock);
            playCamera(type);
        }
    }

    private void playCamera(int type){

        if(bPlaying) {
            Log.e(TAG_VERBOSE_2, "playCamera - already under playing state this:"+this);
            return;
        }

        Utils.getInstance().prepare();

        // register RxListener
        HDMIRxManagerController.setListener(this);

        HDMIRxStatus rxStatus = HDMIRxManagerController.getHDMIRxStatus();

        if(FloatingWindowService2.sHDMIRxStatus==null){
            FloatingWindowService2.sHDMIRxStatus = rxStatus;
        }

        if(rxStatus!=null && rxStatus.status == HDMIRxStatus.STATUS_READY){
            // open HDMIRx
            if(HDMIRxManagerController.open()!=0){
                Log.e(TAG, "open HDMIRx failed");
                HDMIRxManagerController.release();
                // stop service
                FloatingWindowService2.stopServiceItSelf();
                return;
            }
            HDMIRxParameters hdmirxGetParam = HDMIRxManagerController.getParameters();

            if(FloatingWindowService2.sHDMIRxParameters == null){
                FloatingWindowService2.sHDMIRxParameters = hdmirxGetParam;
            }

            getSupportedPreviewSize(hdmirxGetParam, rxStatus.width, rxStatus.height);

            int nativeFps = getSupportedPreviewFrameRate(hdmirxGetParam);
            mFps = nativeFps;
            if(needDownScaleFPS()){
                if(mFps == 60) mFps = 30;
                if(mFps == 50) mFps = 25;
            }

            //mFps=getSupportedPreviewFrameRate(hdmirxGetParam);
            mScanMode=rxStatus.scanMode;

            showToast("preview info "+mWidth+"x"+mHeight+" "+nativeFps+"fps");

            if(DEBUG_VERBOSE){
                Log.i(TAG_VERBOSE,"playCamera OK, size : "+mWidth+"x"+mHeight+" fps:"+nativeFps);
            }

            dismissWarningDialog();

        }else{
            Log.e(TAG, "HDMIRx not ready");

            if(DEBUG_VERBOSE) {
                Log.e(TAG_VERBOSE,"Try to playCamera but HDMIRx is not ready");
            }

            showWarningDialog("No HDMI Signal");
            mPendingRxWarningOnDestory = mWarningDialog;
            FloatingWindowService2.stopServiceItSelf();
            return;
        }

        // set surface
        try{

            if(type == TYPE_SURFACEVIEW){
                HDMIRxManagerController.setPreviewDisplay(mSurfaceHolder);
            }else if(type == TYPE_TEXTUREVIEW){
                SurfaceTexture surfaceTexture = mTextureView.getSurfaceTexture();
                HDMIRxManagerController.setPreviewDisplay3(surfaceTexture);
            }else{
                HDMIRxManagerController.setPreviewDisplay3(mSurfaceTextureForNoPreview);
            }

            // configureTargetFormat
            HDMIRxParameters hdmirxParam = new HDMIRxParameters();
            hdmirxParam.setPreviewSize(mWidth, mHeight);
            hdmirxParam.setPreviewFrameRate(mFps);
            // set sorce format
            HDMIRxManagerController.setParameters(hdmirxParam);
            // always mute first..
            //setPlayback(true, false);
            // configureTargetFormat end
            HDMIRxManagerController.play();
            Log.d(TAG, "play HDMI RX");
        }catch (IOException e){
            e.printStackTrace();
            releaseCamera();
            return;
        }

        bPlaying = true;

        setPlayback(mVideoOn,mAudioOn);
        dismissWarningDialog();
        /*
        // register BroadcastReceiver.
        IntentFilter filter = new IntentFilter("android.intent.action.HDMIRX_PLUGGED"); 
        mContext.registerReceiver(mRxBroadcastReceiver,filter);
        IntentFilter filter_hdcp = new IntentFilter("android.intent.action.HDMIRX_HDCP_STATUS");    
        mContext.registerReceiver(mHDCPBroadcastReceiver,filter_hdcp);
        bReceiverRegistered = true;
        */

        if(mPendingWarningOnDestory != null) {
            mPendingWarningOnDestory.dismiss();
            mPendingWarningOnDestory = null;
        }
        Log.d(TAG, "playCamera - done");
    }

    public static void removePendingWarningDialog(){
        if(mPendingWarningOnDestory != null) {
            mPendingWarningOnDestory.dismiss();
            mPendingWarningOnDestory = null;
        }
    }

    public static void removePendingRxWarningDialog(){
        if(mPendingRxWarningOnDestory != null) {
            mPendingRxWarningOnDestory.dismiss();
            mPendingRxWarningOnDestory = null;
        }
    }

    void releaseMediaRecorder(){
        // Do nothing. maybe add some code later..
    }

    void restart(){
        boolean vOn = mVideoOn;
        boolean aOn = mAudioOn;

        Log.d(TAG, "restart vOn:"+vOn+" aOn:"+aOn);

        releaseCameraLocked();
        playCameraLocked(mViewType);
        setPlaybackLocked(vOn,aOn);
    }

    void destroy(){

        if(DEBUG_VERBOSE2){
            Log.d(TAG_VERBOSE_2,"destroy FloatingWindowViewGroup3 this:"+this);
        }


        Log.d(TAG+"-Suspend", "destroy start");
        mIsValidateInstance = false;

        if(mViewType == TYPE_SURFACEVIEW) {
            if(mSurfaceView != null && mSurfaceHolder != null && mCallback != null) {
                Log.d(TAG, "removeCallback of SurfaceView");
                mSurfaceHolder.removeCallback(mCallback);
            }
        }
        if(mWarningDialog != null) {
            mPendingWarningOnDestory = mWarningDialog;
        }

        /*
        if(bReceiverRegistered){
            mContext.unregisterReceiver(mHDCPBroadcastReceiver);
            mContext.unregisterReceiver(mRxBroadcastReceiver);
        }
        */

        releaseCameraLocked();
        Log.d(TAG, "destroy, releaseCamera done");
        // already done in releaseCamera() -> stopRecord()
        //mLocalSocketTaskAlive = false;
        //new DatagramSocketControlTask().execute(null, null, null);
        //SystemClock.sleep(500);

        if(mWindowManager != null && mFloatingView != null && mViewType != TYPE_NOPREVIEW){
            mWindowManager.removeView(mFloatingView);
        }

        // maybe GC will release it..?
        mPipe = null;
        //releaseWakeLock();
        Log.d(TAG+"-Suspend", "destroy done");
    }

    void setup(int type, Context c){

        mViewType = type;

        if(type == TYPE_SURFACEVIEW){
            mSurfaceView = new SurfaceView(c);
            mSurfaceHolder = mSurfaceView.getHolder();
            mCallback = new FloatingWindowSurfaceCallback(this);
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
            playCameraLocked(mViewType);
        }
    }

    public void showFocusOverlay(boolean show){
        if(show){
            mFocusOverlay.setVisibility(View.VISIBLE);
        }else{
            mFocusOverlay.setVisibility(View.INVISIBLE);
        }
        mFocusShowing = show;
    }

    public boolean isFocusShowing() {
        return mFocusShowing;
    }

    public void attachFocusOverlay(){
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        mFocusOverlay = new RelativeLayout(mContext);
        mFocusOverlay.setLayoutParams(param);
        mFocusOverlay.setBackgroundColor(0x44009688);
        mFloatingView.addView(mFocusOverlay);
        showFocusOverlay(false);
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

        if(retWidth*retHeight > (1920*1080)){
            retWidth = 1920;
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
        /*
        if(fps == 60){
            fps = 30;
        }
        if(fps == 50){
            fps = 25;
        }
        */

        Log.d(TAG, "fps after calculate:"+fps);
        return fps;
    }

    private String getFileSystem(String path){
        Log.d(TAG, "getFileSystem of "+path);
        StorageManager sm = mContext.getSystemService(StorageManager.class);
        List<VolumeInfo> volumeInfos = sm.getVolumes();
        for (VolumeInfo vol : volumeInfos) {
            Log.d(TAG, ""+vol.path+" fsType:"+vol.fsType);
            if(path.contains(vol.path)){
                return (vol.fsType==null)?"null":vol.fsType;
            }
        }
        return "null";
    }

    private long getSingleFileMaxSizeMByte(String _fsType){
        String fsType = _fsType.toLowerCase();

        Log.d(TAG, "getSingleFileMaxSizeMByte fsType:"+fsType);

        if(fsType.contains("fat") /* || fsType.contains("null") */ ){
            long rst = (4*1024) - (1);
            Log.d(TAG, "getSingleFileMaxSizeMByte rst:"+rst);
            return rst;
        }else{
            Log.d(TAG, "getSingleFileMaxSizeMByte rst: -1");
            return -1;
        }
    }

    class FileExistanceCheckTask extends AsyncTask<Void, Void, Boolean> {

        String mErrorMessage = "Record stopped due to I/O error.";

        @Override
        protected Boolean doInBackground(Void... voids){

            /*
            long freeSpace = -1;
            if(mRecordFile.exists()){
                try{
                    freeSpace = mRecordFile.getFreeSpace();
                    Log.d(TAG, "freeSpace :"+freeSpace/(1024*1024)+" MB");
                } catch(SecurityException e){
                    e.printStackTrace();
                    freeSpace = -1;
                }
            }
            */

            long freeSpaceMB_disp = -1;

            // check max file size for file system
            String fsType = getFileSystem(mOutputFolderPath);
            Log.d(TAG, "-- fsType:"+fsType);

            long maxfileSizeMByte = getSingleFileMaxSizeMByte(fsType);

            Log.d(TAG, "maxfileSizeMByte:"+maxfileSizeMByte);

            while(mFileCheckTaskAlive){
                if(mRecordFile.exists()){
                    try{
                        long freeSpace = mRecordFile.getFreeSpace();
                        long freeSpaceMB = freeSpace/(1024*1024);
                        long fileSizeMB = mRecordFile.length()/(1024*1024);

                        if((fileSizeMB >= maxfileSizeMByte) && (maxfileSizeMByte > 0)) {
                            mErrorMessage = "Record stopped, reach maximum size of a single file.";
                            return false;
                        }

                        if(freeSpace <= FREE_SPACE_SAVE_THRESHOLD){
                            Log.e(TAG, "No free space left, return");
                            mErrorMessage = "Record stopped, no more space in storage.";
                            return false;
                        }

                        // dump debug message.
                        if(freeSpaceMB_disp != freeSpaceMB){
                            Log.d(TAG, "FreeSpace :"+freeSpaceMB+" MB filesize:"+fileSizeMB+" MB");
                            freeSpaceMB_disp = freeSpaceMB;
                        }
                    } catch(SecurityException e){
                        e.printStackTrace();
                    }
                    SystemClock.sleep(500);
                }else{
                    mErrorMessage = "Record stopped due to I/O error.";
                    return false;
                }
            }
            return true;
        }

        @Override
        protected void onPostExecute(Boolean result){
            try{
                if(mLocalFilePfdNoPipe != null){
                    mLocalFilePfdNoPipe.close();
                    mLocalFilePfdNoPipe = null;
                }
            }catch(IOException e){
                e.printStackTrace();
            }

            if(result.equals(Boolean.FALSE)){
                //stopRecord();
                restart();
                showWarningDialog(mErrorMessage);
            }

            // then check if service should keep alive?
            checkAndStopService();
        }
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

            boolean result = true;
            int idleLoop = 0;
            long noDataStartTimeMillis = -1;
            long noDataTimeMillis;
            long noDataDurationMillis;

            while(mLocalSocketTaskAlive){
                // read from pipe read pfd
                try{
                    int data = ips.available();
                    if(data>0){
                        // reset timer.
                        noDataStartTimeMillis = -1;
                        //Log.d(TAG, "LocalSocketReadTask data : "+data+ " idleLoop:"+idleLoop);
                        idleLoop=0;
                        int readSize = ips.read(mPipeBuffer);

                        //Log.d(TAG, "LocalSocketReadTask readSize:"+readSize);

                        if(readSize>0){
                            if(fd != null && fd.valid()){
                                mFileOutputStream.write(mPipeBuffer,0,readSize);
                            }
                        }
                    }else{
                        idleLoop++;

                        if(noDataStartTimeMillis==-1){
                            noDataStartTimeMillis = System.currentTimeMillis();
                        }

                        noDataTimeMillis = System.currentTimeMillis();
                        noDataDurationMillis = noDataTimeMillis - noDataStartTimeMillis;

                        //Log.d(TAG,"No Incoming Data duration:"+noDataDurationMillis+" (ms)");

                        SystemClock.sleep(10);
                    }
                }catch(IOException e){
                    Log.e(TAG, "write pipe error");
                    e.printStackTrace();
                    mLocalSocketTaskAlive = false;
                    fd = null;
                    result = false;
                }
            }

            try{
                // always close FileOutputStream
                Log.d(TAG, "close file");

                if(mFileOutputStream!=null){
                    mFileOutputStream.close();
                    mFileOutputStream=null;
                }

                if(mLocalFilePfd != null && moutputMode == Keys.OUTPUT_STORAGE){
                    Log.d(TAG, "close localfilePfd");
                    mLocalFilePfd.close();
                    mLocalFilePfd = null;
                }

                if(mUdpSocketPfd != null && moutputMode == Keys.OUTPUT_UDP){
                    Log.d(TAG, "close udpSocketPfd");
                    mUdpSocketPfd.close();
                    mUdpSocketPfd = null;
                }

            }catch(IOException e){
                e.printStackTrace();
                result = false;
            }
            return result;
        }
        @Override
        protected void onPostExecute(Boolean result){
            Log.d(TAG, "LocalSocketReadTask onPostExecute - call stopRecord");

            String modeStr = "Record";
            if(moutputMode==Keys.OUTPUT_UDP){
                modeStr = "Streaming";
            }

            stopRecord();

            if(result.equals(Boolean.FALSE)){
                multicastSocket = null;
            }
            new DatagramSocketControlTask().execute(null, null, null);

            closePipe();

            if(result.equals(Boolean.FALSE)){
                showWarningDialog(modeStr+" stopped due to I/O error");
            }
            // then check if service should keep alive?
            checkAndStopService();
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
            Log.d(TAG, "DatagramSocketControlTask done");
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
            multicastSocket = null;
            closePipe();
            return null;
        }catch(UnknownHostException e){
            e.printStackTrace();
            multicastSocket = null;
            closePipe();
            return null;
        }catch(IOException e){
            e.printStackTrace();
            multicastSocket = null;
            closePipe();
            return null;
        }
    }

    private void closePipe(){
        try{
            if(mWritePfd != null){
                Log.d(TAG, "close pipe writePfd");
                mWritePfd.close();
                mWritePfd = null;
            }
            if(mReadPfd != null){
                Log.d(TAG, "close pipe readPfd");
                mReadPfd.close();
                mReadPfd = null;
            }
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    private ParcelFileDescriptor createLocalFdViaPipe(String folderPath, int ext){
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

        String extension = ".ts";
        if(ext == Keys.FORMAT_MP4){
            extension = ".mp4";
        }

        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String filePath = folderPath + File.separator + "VID_" + timeStamp + extension;

        Log.d(TAG,"record file:"+filePath);

        mRecordFile = new File(filePath);
        try{
            int mode = ParcelFileDescriptor.MODE_CREATE|ParcelFileDescriptor.MODE_READ_WRITE;
            mLocalFilePfd = ParcelFileDescriptor.open(mRecordFile,mode);
            mFileOutputStream = new FileOutputStream(mLocalFilePfd.getFileDescriptor());
            return mWritePfd;
        }catch(FileNotFoundException e){
            e.printStackTrace();
            closePipe();
            return null;
        }
    }

    private ParcelFileDescriptor createLocalFd(String folderPath, int ext){

        String extension = ".ts";
        if(ext == Keys.FORMAT_MP4){
            extension = ".mp4";
        }

        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String filePath = folderPath + File.separator + "VID_" + timeStamp + extension;

        Log.d(TAG,"record file (no pipe mode) :"+filePath);

        mRecordFile = new File(filePath);
        try{
            int mode = ParcelFileDescriptor.MODE_CREATE|ParcelFileDescriptor.MODE_READ_WRITE;
            mLocalFilePfdNoPipe = ParcelFileDescriptor.open(mRecordFile,mode);
            return mLocalFilePfdNoPipe;
        }catch(FileNotFoundException e){
            e.printStackTrace();
            mLocalFilePfdNoPipe = null;
            return null;
        }
    }

    // TODO
    public boolean startRecord(Bundle arg) {
        if(mRecordOn) {
            Log.e(TAG, "startRecord error , stop current record instance first");
            return false;
        }

        int currentAudioMode = HDMIRxManagerController.getAudioMode();
        Log.d(TAG, "currentAudioMode:"+currentAudioMode);
        if(currentAudioMode == RtkHDMIRxManager.HDMIRX_AUDIO_RAW){

            dismissWarningDialog();
            showWarningDialog("Can not record (or streaming) under audio RAW mode.");

            //showToast("can not record under RAW mode");
            return false;
        }

        if(mHDCP){
            dismissWarningDialog();
            showWarningDialog("Unable to record or streaming.\nProtected content detected.");
            return false;
        }

        int outputMode = arg.getInt(Keys.KEY_OUTPUT_MODE,Keys.OUTPUT_STORAGE);
        moutputMode = outputMode;
        String outputPath = arg.getString(Keys.KEY_OUTPUT_PATH);
        mUdpPort = arg.getInt(Keys.KEY_PORT,-1);
        int extension = arg.getInt(Keys.KEY_OUTPUT_FORMAT,Keys.FORMAT_TS);

        int rec_w = arg.getInt(Keys.KEY_RECORD_WIDTH,-1);
        int rec_h = arg.getInt(Keys.KEY_RECORD_HEIGHT,-1);

        Log.d(TAG, "rec_w:"+rec_w+" rec_h:"+rec_h);

        //mRecordDurationMin = arg.getInt(Keys.KEY_DURATION_MIN,-1);
        mRecordEndTimeMillis = arg.getLong(Keys.KEY_RECORD_END_TIME_MILLIS,-1);

        if(outputMode == Keys.OUTPUT_UDP) {
            mUdpIp = outputPath;
        }

        Log.d(TAG, "outputPath:"+outputPath);
        mOutputFolderPath = outputPath;

        if(HDMIRxManagerController.isOpened()){
            boolean bPipeMode = false;
            try{
                ParcelFileDescriptor pfd;
                if(outputMode == Keys.OUTPUT_STORAGE){
                    //pfd = createLocalFdViaPipe(outputPath, extension);
                    pfd = createLocalFd(outputPath,extension);

                    // Check space left..
                    if(!mRecordFile.exists()){
                        dismissWarningDialog();
                        showWarningDialog("Unable to start record due to I/O error");
                        return false;
                    }

                    if(mRecordFile.exists()){
                        long freeSpace = mRecordFile.getFreeSpace();
                        if(freeSpace < (51*1024*1024)){
                            dismissWarningDialog();
                            showWarningDialog("Unable to start record due to I/O error");
                            return false;
                        }
                    }

                }else{

                    if(!hasNetwork()){
                        showWarningDialog("No network available");
                        return false;
                    }

                    // UDP mode
                    mProcessingUDPSocket = true;
                    new DatagramSocketCreateTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);

                    while(mProcessingUDPSocket){
                        SystemClock.sleep(100);
                    }

                    pfd = mWritePfd;
                    bPipeMode = true;
                }

                if(pfd!=null){
                    if(bPipeMode){
                        mLocalSocketTaskAlive = true;
                        new LocalSocketReadTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                    }else{
                        mFileCheckTaskAlive = true;
                        new FileExistanceCheckTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                    }
                }

                if(pfd == null){
                    Log.e(TAG, "create fd error");
                    showWarningDialog("I/O error when recording");
                    return false;
                }

                Log.d(TAG, "Current preview size :"+mWidth+","+mHeight);
                // video parameters
                int w = mWidth;
                int h = mHeight;
                int videoBitrate = 10000000;
                // audio parameters
                int channelCount = 2;
                int sampleRate = 48000;
                int audioBitrate = 64000;

                Log.d(TAG, "org w:"+w+" h:"+h);
                if(rec_w > 0 && rec_h > 0) {
                    // record size should not bigger than preview size
                    if(rec_w <= w || rec_h <= h){
                        w = rec_w;
                        h = rec_h;
                    }
                }

                if((w*h) > 1920*1080){
                    w = 1920;
                    h = 1080;
                }

                Log.d(TAG, "set record resolution to "+w+","+h);

                RtkHDMIRxManager.VideoConfig vConfig =
                    new RtkHDMIRxManager.VideoConfig(w,h,videoBitrate);
                RtkHDMIRxManager.AudioConfig aConfig =
                    new RtkHDMIRxManager.AudioConfig(channelCount,sampleRate,audioBitrate);
                HDMIRxManagerController.configureTargetFormat(vConfig, aConfig);

                int mOutputFormat = (extension==Keys.FORMAT_TS)?RtkHDMIRxManager.HDMIRX_FILE_FORMAT_TS:RtkHDMIRxManager.HDMIRX_FILE_FORMAT_MP4;

                if(mOutputFormat == RtkHDMIRxManager.HDMIRX_FILE_FORMAT_TS){
                    Log.d(TAG, "output format : ts");
                }else{
                    Log.d(TAG, "output format : mp4");
                }

                HDMIRxManagerController.setTargetFd(pfd, mOutputFormat);
                HDMIRxManagerController.setTranscode(true);
                mRecordOn = true;
                Log.d(TAG, "set record on");

                if(FloatingWindowService2.getInstance() != null){
                    FloatingWindowService2.getInstance().acquireWakeLock();
                }

                attachRecordInfoView();
                setRecordInfoViewIcon(moutputMode);

                mRecordStartTime = new GregorianCalendar();
                Date trialTime = new Date();
                mRecordStartTime.setTime(trialTime);
                mHandler.postDelayed(mRunnable,1);
                showToast("record on :"+w+"x"+h+" fps:"+mFps);
                refreshRecordwidgets();
                return true;
            }catch(InvalidOpException e){
                e.printStackTrace();
                mRecordOn = false;
                refreshRecordwidgets();
                return false;
            }
        }else{
            Log.e(TAG, "startRecord ERROR , no rx instance");
            refreshRecordwidgets();
            return false;
        }
    }

    public void stopRecord() {
        if(mRecordOn) {
            if(HDMIRxManagerController.isOpened()){
                Log.d(TAG, "set record off");
                showToast("record off");
                try{
                    HDMIRxManagerController.setTranscode(false);
                }catch(InvalidOpException e){
                    e.printStackTrace();
                }
                mRecordOn = false;

                if(FloatingWindowService2.getInstance() != null){
                    FloatingWindowService2.getInstance().releaseWakeLock();
                }

                moutputMode = Keys.OUTPUT_OFF;
                mLocalSocketTaskAlive = false;
                mFileCheckTaskAlive = false;
                detachRecordInfoView();
                refreshRecordwidgets();
            }
        }else{
            Log.e(TAG, "stop record error, no record instance");
        }
    }

    private void refreshRecordwidgets(){
        if(FloatingWindowService2.sListener != null){
            FloatingWindowService2.sListener.refreshRecordwidgets();
        }
    }

    private void updateSurfaceVisibility(boolean on){
        mFloatingView.setVisibile2(on);
    }

    public void updateWindowLocation(int[] position, boolean touchable) {
        mFloatingView.updateLocation(position,touchable);
    }

    private void showToast(String text){
        if(SHOW_TOAST)
            Toast.makeText(mContext, text, Toast.LENGTH_SHORT).show();
    }

    private void showHideView(boolean show){
        synchronized(sCameraLock){
            if(show){
                mVideoOn = true;
                mAudioOn = true;
                updateSurfaceVisibility(true);
                if(HDMIRxManagerController.isOpened()){
                    HDMIRxManagerController.setPlayback(mVideoOn,mAudioOn);
                }
            }else{
                mVideoOn = false;
                mAudioOn = false;
                if(HDMIRxManagerController.isOpened()){
                    HDMIRxManagerController.setPlayback(mVideoOn,mAudioOn);
                }
                updateSurfaceVisibility(false);
            }
        }
    }

    public boolean isVideoOn(){
        return mVideoOn;
    }

    public boolean isAudioOn(){
        return mAudioOn;
    }

    public int getOutputMode() {
        return moutputMode;
    }

    public void setPlaybackLocked(boolean vOn, boolean aOn){
        synchronized(sCameraLock){
            setPlayback(vOn, aOn);
        }
    }

    private void setPlayback(boolean vOn, boolean aOn) {
        mVideoOn = vOn;
        mAudioOn = aOn;
        if(HDMIRxManagerController.isOpened()){
            HDMIRxManagerController.setPlayback(mVideoOn,mAudioOn);
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

    public boolean isRecording() {
        return mRecordOn;
    }

    public void detachRecordInfoView(){
        if(mRecordInfoView != null) {
            mWindowManager.removeView(mRecordInfoView);
        }
    }

    private String get2DigitString(int value){
        String rst="";
        if(value<10)
            rst=rst+"0";
        rst=rst+value;
        return rst;
    }

    private String createDurationString(int h, int m, int s){
        return get2DigitString(h)+":"+get2DigitString(m)+":"+get2DigitString(s);
    }

    private String createRecDisplayString(){
        if(moutputMode == Keys.OUTPUT_STORAGE){
            return "REC";
        }else if(moutputMode == Keys.OUTPUT_UDP){
            return "UDP";
        }
        return "";
    }

    private void setRecordInfoViewIcon(int outputMode){
        if(outputMode==Keys.OUTPUT_STORAGE){
            mRecordInfoView.setIcon(RecordInfoWindowView.ICON_REC);
        }else{
            mRecordInfoView.setIcon(RecordInfoWindowView.ICON_UDP);
        }
    }

    private void attachRecordInfoView(){
        if(mRecordInfoView != null) {
            int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

            int width = 300;
            int height = 100;
            int x = 1920 - width - 50;
            int y = 20;

            WindowManager.LayoutParams wmParams = new WindowManager.LayoutParams(
                width,
                height,
                FloatingWindowService2.FLOATING_WINDOW_TYPE,
                flag,
                PixelFormat.TRANSLUCENT);

            wmParams.x = x;
            wmParams.y = y;
            wmParams.gravity = Gravity.TOP|Gravity.LEFT;
			wmParams.type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
            mWindowManager.addView(mRecordInfoView,wmParams);
        }
    }

    private void dismissWarningDialog(){
        Log.d(TAG, "dismissWarningDialog instance:"+this);
        if(mWarningDialog!=null){
            mWarningDialog.dismiss();
            mWarningDialog = null;
        }
    }

    public void showWarningDialog(String message){
        Log.d(TAG, "showWarningDialog "+message+ " - instance:"+this);
        dismissWarningDialog();
        mWarningDialog = new Dialog(mContext);
        mWarningDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        mWarningDialog.setContentView(R.layout.layout_warning_dialog);
        Window w = mWarningDialog.getWindow();
        w.getAttributes().type = WindowManager.LayoutParams.TYPE_TOAST;

        TextView warningTextView = (TextView) mWarningDialog.findViewById(R.id.warning_textview);
        TextView confirmTextView = (TextView) mWarningDialog.findViewById(R.id.ok_textview);

        confirmTextView.setText("Press \"OK\" button to close dialog");
        warningTextView.setText(message);

        // support on click event
        View okArea = mWarningDialog.findViewById(R.id.ok_area);
        okArea.setOnClickListener(new View.OnClickListener(){
            public void onClick (View v){
                dismissWarningDialog();
            }
        });

        mWarningDialog.setOnKeyListener( new DialogInterface.OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_BACK){
                    if(event.getAction() == KeyEvent.ACTION_UP){
                        Log.d(TAG,"keypress dismiss mWarningDialog");
                        mWarningDialog.dismiss();
                        mWarningDialog = null;
                    }
                }
                return true;
            }
        });

        mWarningDialog.show();
        Log.d(TAG, "showWarningDialog - show msg:\""+message+"\"");
    }

    private boolean hasNetwork(){
        ConnectivityManager cm =
                (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        boolean isConnected = activeNetwork != null &&
                activeNetwork.isConnectedOrConnecting();

        return isConnected;
    }

    @Override
    public void onEvent(int eventId) {
        if(eventId == RtkHDMIRxManager.HDMIRxListener.EVENT_CAMERA_ERROR) {
            FloatingWindowService2.stopServiceItSelf();
        }
    }

    private void checkAndStopService(){
        if(FloatingWindowService2.mWindowAliveReplyer == null) {

            Log.d(TAG, "Record complete normally, no one needs the service, terminate");

            FloatingWindowService2.stopServiceItSelf();
        }else{
            if(!FloatingWindowService2.mWindowAliveReplyer.needAlive()){
                Log.d(TAG, "Record complete normally, replyer does not need the service, terminate");
                FloatingWindowService2.stopServiceItSelf();
            }else{
                Log.d(TAG, "Record complete normally, needAlive, keeps service on");
            }
        }
    }

}
