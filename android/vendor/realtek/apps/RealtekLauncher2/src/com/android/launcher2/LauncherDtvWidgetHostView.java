package com.android.launcher2;

import java.io.IOException;
import java.util.List;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.MediaController;
import android.widget.RemoteViews;
import android.widget.VideoView;
import android.widget.Toast;
import android.app.ActivityManager;

import com.android.launcher2.Launcher.State;
import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

import com.realtek.app.TvManager;
import com.realtek.app.tv.*;
//import com.realtek.app.TvManager;
//import com.realtek.app.tv.*;
import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.HDMIRxParameters;
import com.realtek.hardware.HDMIRxStatus;

import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraAccessException;

public class LauncherDtvWidgetHostView extends LauncherAppWidgetHostView {

    public static final String RED="\033[0;31;31m";
    public static final String END="\033[m";

    private final static boolean ENABLE_1195_DTV_FLOW = true;

    static final int SOURCE_DTV        = 0;
    static final int SOURCE_CAMERA     = 1;
    static final int SOURCE_HDMI       = 2;
    static final int SOURCE_LOCAL_FILE = 3;
    static final int SOURCE_NUMBER     = 3;

    final String TAG = "LauncherDtvWidgetHostView";
    
    final String PREFS_NAME          = "DTV_PREFS";
    final String PREFS_REGION        = "PREFS_REGION";
    final String PREFS_TV_SERVICE    = "PREFS_TV_SERVICE";
    final String PREFS_SOURCE        = "PREFS_SOURCE";
    final String PREFS_TV_CHANNEL    = "PREFS_TV_CHANNEL";
    final String PREFS_RADIO_CHANNEL = "PREFS_RADIO_CHANNEL";
    final String FORBIDDEN_STRING    = "XXX";
    final String DTV_PATH            = "stb://0";
    final String TEST_FILE           = "/storage/sda1/1.wmv";

    final int DEFAULT_FILTER = TvManager.CHANNEL_FLAGS_IS_DELETED|TvManager.CHANNEL_FLAGS_IS_HIDDEN;

    TvManager m_tvManager = null;
    private boolean isServerConnected = false;
    private int tunerStatus[] = new int[2];
    private int appType[] = new int[2];
    SharedPreferences settings;
    int tunerId = 0;
    int decodingTunerId = 0;
    int region = -1;
    int serviceType = 0;
    int channelCount = 0;
    int tvChannel = -1, radioChannel = -1, prevChannel = -1, currentChannel = -1;
    int filterOutFlags = DEFAULT_FILTER;
    private static Context context;
    
    public static MediaPlayer mp = null;

    private Object mHDMIRXLock = new Object();

    int videoWidth, videoHeight;
    boolean isVideoReady, isVideoSizeKnown;

    Camera mCamera = null;
    RtkHDMIRxManager mHDMIRX = null;
    int mWidth;
    int mHeight;

    boolean previewing = false;
    public static boolean isDTVInitialized = false;
    public static boolean isCameraInitialized = false;
    public static boolean isLocalFileInitialized = false;
    public static boolean isHDMIRXInitialized = false;
    static int currentSource = SOURCE_HDMI; // to identify the first setting

    public static Launcher mLauncher = null;
    public static View mDtvRootView = null;
    public static VideoView videoView = null;

    public static SurfaceView mSurfaceView = null;
    public static SurfaceHolder mSurfaceHolder = null;

    private View mRootView=null;
    private Context mContext = null;
    private EventReceiver er = null;
    public static boolean isDTVSupported = true;
    
    //private Launcher mLauncher;
    private LinearLayout.LayoutParams lp;
    private FrameLayout.LayoutParams flp;
    
    CameraSurfaceCallback cameraCallback = new CameraSurfaceCallback();
    DTVSurfaceCallback DTVCallback = new DTVSurfaceCallback();
    HDMIRXSurfaceCallback HDMIRXCallback = new HDMIRXSurfaceCallback();
    
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            relayoutSurfaceView();
        }
    };

    public LauncherDtvWidgetHostView(Context context, Launcher launcher) {
        super(context, launcher);
        mContext=context;
        mLauncher=launcher;
        DebugHelper.dump("[DTV Widget] Create LauncherDtvWidgetHostView");
        // RTKTODO: add a Surface view inside itself.
        Log.d(TAG, "LauncherDtvWidgetHostView");
        IntentFilter i = new IntentFilter();
        i.addAction(ConstantsDef.INIT_SURFACEVIEW_WIDGET_INTENT);
        i.addAction(ConstantsDef.UNINIT_SURFACEVIEW_WIDGET_INTENT);
        i.addAction(ConstantsDef.SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT);
        i.addAction("android.intent.action.HDMIRX_PLUGGED");
        if (er == null) {
            er = new EventReceiver();
        }
        mContext.registerReceiver(er, i);
        inflateRootView();//addSurfaceView();
    }
    
    private void inflateRootView(){
        if(mRootView==null){
            LayoutInflater mInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            mRootView = mInflater.inflate(R.layout.surfaceview_widget_view, this, false);
            mLauncher.mDtvRootView = mRootView;
            mSurfaceView = (SurfaceView) mRootView.findViewById(R.id.surface_view);
            mSurfaceHolder = mSurfaceView.getHolder();
            Log.d(TAG, "inflateRootView() mSurfaceView true,VISIBLE");
            mSurfaceView.setZOrderOnTop(true);
            //mSurfaceView.setVisibility(View.VISIBLE);
            videoView = (VideoView) mRootView.findViewById(R.id.videoView);
            videoView.setVisibility(View.INVISIBLE);
            Log.d(TAG, "inflateRootView() SurfaceHolder:" + mSurfaceHolder.toString());
            
            addView(mRootView);
            if(mSurfaceView!=null){
                if(mLauncher.mState==State.WORKSPACE){
                    mLauncher.initSurfaceViewWidget("SurfaceView Create Complete");
                }else{
                    DebugHelper.dump("inflateRootView, but not in WORKSPACE state, withdraw");
                }
            }
        }
    }
    
    @Override
    public void updateAppWidget(RemoteViews remoteViews) {
        DebugHelper.dump("[DTV Widget] Do nothing in updateAppWidget.");
        Log.d(TAG, "[DTV Widget] Do nothing in updateAppWidget.");
        inflateRootView();
    }

    @Override
    protected void onLayout (boolean changed, int left, int top, int right, int bottom){
        super.onLayout(changed, left, top, right, bottom);
        Log.d(TAG, "onLayout()");
        if(changed){
            DebugHelper.dump("[DTV HOST] onLayout");
            Message message;
            String obj = "relayout surface view";
            message = handler.obtainMessage(1,obj);
            handler.sendMessage(message);
        }
    }

    @Override
    protected void onAttachedToWindow (){
        super.onAttachedToWindow();
        Log.d(TAG, "------------> onAttachedToWindow, register");
        
    }

    @Override
    protected void onDetachedFromWindow (){
        super.onDetachedFromWindow();
        Log.d(TAG, "------------> onDetachedFromWindow, un-register");
        stopSource(currentSource);
        mContext.unregisterReceiver(er);
    }

    private void relayoutSurfaceView(){
        DebugHelper.dump("[DTV HOST] relayoutSurfaceView");
        Log.d(TAG, "relayoutSurfaceView()");
        int viewH = getHeight();
        int labelH = mContext.getResources().getDimensionPixelSize(R.dimen.rtk_dtv_widget_label_height);

        int width = getWidth();
        int height = viewH-labelH;

        if(mSurfaceView!=null){
            DebugHelper.dump("[DTV HOST] relayout surfaceView as height="+height+" width="+width);
            Log.d(TAG, "[DTV HOST] relayout surfaceView as width=" + width + " height=" + height);
            int margin = 3; // in dip
            int px = (int)Math.ceil((double)TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, margin * 2, getResources().getDisplayMetrics()));
            Log.d(TAG, "px=" + px);
            flp = new FrameLayout.LayoutParams(width - px, height - px, Gravity.CENTER);
            mSurfaceView.setLayoutParams(flp);
            mSurfaceView.requestLayout();
        }
    }

    /*
    private void addSurfaceView(){
        if(mSurfaceView==null){
            DebugHelper.dump("[DTV Widget] add SurfaceView");
            dumpSelfSize();
            mSurfaceView = new SurfaceView(mContext);
            mSurfaceView.setBackgroundColor(Color.BLACK);
            
            FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
                    FrameLayout.LayoutParams.MATCH_PARENT,
                    FrameLayout.LayoutParams.MATCH_PARENT);
            
            addView(mSurfaceView, params);
        }
    }
    */

    @Override
    public boolean orientationChangedSincedInflation() {
        // In DTV widget, we do not consider orientation changed case,
        // so Launcher does not have to re-inflate widget.
        return false;
    }
    
    private void dumpSelfSize(){
        
        int width=getWidth();
        int height=getHeight();
        
        DebugHelper.dump("[DTV widget] w="+width+" h="+height);
    }

    private Camera.Size getBestPreviewSize(int width, int height, Camera.Parameters parameters) {
        Camera.Size result = null;

        List<Size> sizes = parameters.getSupportedPreviewSizes();
        if (sizes == null) {
            Log.d(TAG, "getBestPreviewSize() sizes==null");
            return null;
        }

        for (Camera.Size size : parameters.getSupportedPreviewSizes()) {
            if (result == null) {
                result = size;
            }
            if (size.width <= width && size.height <= height) {
                int resultArea = result.width * result.height;
                int newArea = size.width * size.height;
                if (newArea > resultArea) {
                    result = size;
                }
            }
        }
        Log.d(TAG, "getBestPreviewSize result: w:" + result.width + " h:" + result.height + " (size w:" + width + " h:" + height + ")");
        return result;
    }

    private RtkHDMIRxManager.Size getBestPreviewSize(int width, int height, HDMIRxParameters parameters) {
        Log.d(TAG, "[hdmirx]getBestPreviewSize, surface(w,h)=(" + width + "," + height + ")");
        if (parameters == null) {
            Log.w(TAG, "parameters is null");
            return null;
        }

        RtkHDMIRxManager.Size result = null;

        List<RtkHDMIRxManager.Size> sizes = parameters.getSupportedPreviewSizes();
        if (sizes == null) {
            Log.d(TAG, "[hdmirx]getBestPreviewSize() sizes==null");
            return null;
        }

        for (RtkHDMIRxManager.Size size : sizes) {
            if (result == null) {
                result = size;
            }
            if (size.width <= width && size.height <= height) {
                int resultArea = result.width * result.height;
                int newArea = size.width * size.height;
                if (newArea > resultArea) {
                    result = size;
                }
            }
        }
        Log.d(TAG, "[hdmirx]getBestPreviewSize result: w:" + result.width + " h:" + result.height + " (size w:" + width + " h:" + height + ")");
        return result;
    }

    class CameraSurfaceCallback implements SurfaceHolder.Callback {
        @Override
        public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
            Log.d(TAG, "CameraSurfaceCallback.surfaceChanged(): " + arg0.toString());
            Surface s = arg0.getSurface();
            if (s == null) {
                Log.d(TAG, "surfaceChanged s==null");
                return;
            }
    
            if (mCamera == null) {
                Log.d(TAG, "surfaceChanged mCamera==null");
                return;
            }
    
            if(previewing){
                mCamera.stopPreview();
                try {
                    mCamera.setPreviewDisplay(null);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                previewing = false;
            }

            try {
                Log.d(TAG, "surfaceChanged mCamera.startPreview()");
                mCamera.setPreviewDisplay(arg0);
                initCameraPreview(width, height);
                mCamera.startPreview();
                previewing = true;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void surfaceCreated(SurfaceHolder arg0) {
            Surface s = arg0.getSurface();
            Log.d(TAG, "CameraSurfaceCallback.surfaceCreated(): " + s.toString());
            if (s == null) {
                Log.d(TAG, "surface == null");
            }

            CameraManager manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
            boolean cameraExist = false;
            mCamera = null;
            try{
                CameraCharacteristics cameraCharas = manager.getCameraCharacteristics("0");
                Log.d(TAG, "cameraCharas:"+cameraCharas);
                cameraExist = true;
            }catch (IllegalArgumentException e){
                Log.e(TAG, "Camera not exist");
                cameraExist = false;
            }catch (CameraAccessException e){
                Log.e(TAG, "Camera not exist");
                cameraExist = false;
            }

            //int cameraCount = 0;
            //Camera.CameraInfo cameraInfo = new Camera.CameraInfo();

            if(cameraExist){
                int camIdx = 0;
                mCamera = Camera.open(camIdx);
            }

            if (mCamera == null) {
                Log.d(TAG, "mCamera == null");
            }
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder arg0) {
            // TODO Auto-generated method stub
            Log.d(TAG, "CameraSurfaceCallback.surfaceDestroyed(): " + arg0.toString());
            if (mCamera != null) {
                mCamera.stopPreview();
                mCamera.release();
                mCamera = null;
            }
            previewing = false;
        }

        public void initCameraPreview(int width, int height) {
            Camera.Parameters parameters = mCamera.getParameters();
            Camera.Size size = getBestPreviewSize(width, height, parameters);

            if (size != null) {
                parameters.setPreviewSize(size.width, size.height);
                mCamera.setParameters(parameters);
                //cameraConfigured=true;
            }
        }
    }

    class HDMIRXSurfaceCallback implements SurfaceHolder.Callback {
        @Override
        public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
            Log.d(TAG, "HDMIRXSurfaceCallback.surfaceChanged(): " + arg0.toString());
            Log.d(TAG, RED+"HDMIRXSurfaceCallback.surfaceChanged arg0"+arg0+" mSurfaceHolder:"+mSurfaceHolder+END);

            Surface s = arg0.getSurface();
            mWidth=width;
            mHeight=height;
            if (s == null) {
                Log.d(TAG, "surfaceChanged s==null");
                return;
            }

            if (mHDMIRX == null) {
                Log.d(TAG, "surfaceChanged mHDMIRX==null");
                return;
            }

            if(previewing){
                //mHDMIRX.stop();
                /*
                try {
                    mHDMIRX.setPreviewDisplay(null);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                */
                //previewing = false;
                return;
            }

            try {
                if(mLauncher.mScreenState == false) {
                    Log.e(TAG, "HDMIRXSurfaceCallback surfaceChanged mScreenState is off");
                    mLauncher.mHasPendingPreviewRequest = true;
                    return;
                }
                Log.d(TAG, "surfaceChanged mHDMIRX.startPreview()");
                mHDMIRX.setPreviewDisplay(arg0);
                initHDMIRXPreview(width, height);
                mHDMIRX.play();
                previewing = true;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void surfaceCreated(SurfaceHolder arg0) {
            Surface s = arg0.getSurface();
            Log.d(TAG, "HDMIRXSurfaceCallback.surfaceCreated(): " + s.toString());
            if (s == null) {
                Log.d(TAG, "surface == null");
            }
            mHDMIRX = new RtkHDMIRxManager();
            if ((mHDMIRX.getHDMIRxStatus()).status ==HDMIRxStatus.STATUS_READY){
                if (mHDMIRX.open()!=0){
                    Log.d(TAG, "open HDMIRX failed.");
                    mHDMIRX = null;
                }
            }
            else{
                Log.d(TAG, "HDMIRX not ready");
                mHDMIRX = null;
            }
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder arg0) {
            Log.d(TAG, "HDMIRXSurfaceCallback.surfaceDestroyed(): " + arg0.toString());
            stopSource(SOURCE_HDMI);
            /*
            if (mHDMIRX != null) {
                mHDMIRX.stop();
                mHDMIRX.release();
                mHDMIRX = null;
            }
            previewing = false;
            */
        }
    }

    public void initHDMIRXPreview(int width, int height) {
        HDMIRxParameters param = mHDMIRX.getParameters();
        RtkHDMIRxManager.Size size = getBestPreviewSize(width, height, param);

        //set preview size
        if (size != null) {
            param.setPreviewSize(size.width, size.height);
            //set preview frame rate
            //param.setPreviewFrameRate(30);
            //set parameters to HDMIRX
            mHDMIRX.setParameters(param);
        }
    }

    private OnPreparedListener onVideoPreparedListener = new OnPreparedListener() {
        @Override
        public void onPrepared(MediaPlayer mp) {
            Log.d(TAG, "videoView onPrepared()");
            if (videoView != null) {
                videoView.start();
                mp.setLooping(true);
            } else {
                Log.e(TAG, "onPrepared() videoView == null");
            }
        }
    };

    private OnCompletionListener onVideoCompletionListener = new OnCompletionListener(){
        @Override
        public void onCompletion(MediaPlayer mp) {
            Log.d(TAG, "onCompletion");
            //videoView.stopPlayback();
            //videoView.start();
        }
    };
    
    private OnErrorListener onVideoErrorListener = new OnErrorListener() {
        @Override
        public boolean onError(MediaPlayer mp, int what, int extra) {
            Log.e(TAG, "onError what:" + what + " extra:" + extra);
            return true;
        }
    };

    public static boolean isFloatingWindowServiceRunning(Context c){
        ActivityManager manager = (ActivityManager) c.getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            //Log.d(TAG, "Service: "+service.service.getClassName());
            if (service.service.getClassName().equals("com.realtek.floatingwindow.FloatingWindowService")){
                return true;
            }
        }
        return false;
    }

    public class EventReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            DebugHelper.dump("[DTV widget provider] [onReceive] Receive following action intent: "+action);
            Log.d(TAG, "[DTV widget provider] [onReceive] Receive following action intent: "+action);


            // check if has floating window service exists
            if(isFloatingWindowServiceRunning(context)){

                if(action.equals(ConstantsDef.INIT_SURFACEVIEW_WIDGET_INTENT)){
                    Toast.makeText(context,"FloatingWindowService is running, won't show preview",Toast.LENGTH_SHORT).show();
                }
                return;
            }

            // basic action handling
            if(action.equals(ConstantsDef.INIT_SURFACEVIEW_WIDGET_INTENT)){
                String extra = intent.getExtras().getString(ConstantsDef.INIT_EXTRA_NAME);
                //DebugHelper.dump("[DTV widget provider] Extra:"+extra);
                Log.d(TAG, "INIT_SURFACEVIEW_WIDGET_INTENT");
                selectSource();
                //Log.d(TAG, "onReceive() mSurfaceView true,VISIBLE");
                //mSurfaceView.setZOrderOnTop(true);
                //mSurfaceView.setVisibility(View.VISIBLE);
                
            } else if(action.equals(ConstantsDef.UNINIT_SURFACEVIEW_WIDGET_INTENT)){
                String extra = intent.getExtras().getString(ConstantsDef.UNINIT_EXTRA_NAME);
                DebugHelper.dump("[DTV widget provider] Extra:"+extra);
                Log.d(TAG, RED+"UNINIT_SURFACEVIEW_WIDGET_INTENT"+END);
                stopSource(currentSource);
                // TODO: more...
            } else if(action.equals(ConstantsDef.SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT)){
                String extra = intent.getExtras().getString(ConstantsDef.SOURCE_EXTRA_NAME);
                DebugHelper.dump("[DTV widget provider] Extra:"+extra);
                // TODO: more...
                Log.d(TAG, "SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT currentSource:" + currentSource);
                selectNextSource();
                Log.d(TAG, "currentSource:" + currentSource);
            } else if (action.equals("android.intent.action.HDMIRX_PLUGGED")){
                if (!(intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false))){
                    Log.d(TAG, RED+"HDMI Rx is plugged out."+END);
                    if (SOURCE_HDMI == currentSource){
                       stopSource(currentSource); 
                    }
                }else{
                    Log.d(TAG, RED+"HDMI Rx is plugged in."+END);

                    if(mLauncher.mScreenState == false) {
                        Log.e(TAG, "Plugin EVENT surfaceChanged mScreenState is off");
                        return;
                    }

                    // check state
                    boolean bShowSourceSurface = false;
                    boolean bLauncherIsForeground = mLauncher.launcherIsForeground();
                    if(mLauncher.launcherIsForeground()){
                        if(mLauncher.mState == State.WORKSPACE)
                            bShowSourceSurface = true;
                    }
                    Log.d(TAG,RED+"launcher is foreground:"+bLauncherIsForeground+" state:"+mLauncher.mState+" show surface:"+bShowSourceSurface+END);
                    if(!bShowSourceSurface){
                        Log.d(TAG,RED+"Do nothing in Rx plugged in"+END);
                        return;
                    }
                    if (SOURCE_HDMI == currentSource){
                        // check mHDMIRx status
                        if(mHDMIRX==null && isHDMIRXInitialized){
                            int retryCount = 0;
                            // new HDMIRxManager
                            Log.d(TAG,RED+"on HDMIRx hotplug but mHDMIRX is null -> Create RtkHDMIRxManager with retry"+END);
                            while(mHDMIRX==null && retryCount<10){
                                Log.d(TAG,RED+"try create RtkHDMIRxManager retry count:"+retryCount+END);
                                retryCount++;
                                mHDMIRX = new RtkHDMIRxManager();
                                if ((mHDMIRX.getHDMIRxStatus()).status ==HDMIRxStatus.STATUS_READY){
                                    // open failed, back to null
                                    if (mHDMIRX.open()!=0){
                                        Log.d(TAG, "open HDMIRX failed.");
                                        mHDMIRX = null;
                                    }

                                }else{
                                    Log.d(TAG, RED+"HDMIRX not ready"+END);
                                    mHDMIRX = null;
                                }
                            }

                            // play
                            if(mHDMIRX!=null){
                                if(previewing==false){
                                    try {
                                        mHDMIRX.setPreviewDisplay(mSurfaceHolder);
                                        initHDMIRXPreview(mWidth, mHeight);
                                        mHDMIRX.play();
                                        previewing = true;
                                    }catch(Exception e){
                                        e.printStackTrace();
                                    }
                                }
                            }
                        }
                        selectSource();
                    }
                }
            }
        }
        
    }

    private void startPlayback() {
        Log.d(TAG, "startPlayback");
        if (videoView == null) {
            Log.d(TAG, "videoView == null");
        }
        MediaController mc = new MediaController(mContext);
        videoView.setMediaController(mc);
        videoView.setVideoPath(TEST_FILE);
        videoView.setOnCompletionListener(onVideoCompletionListener);
        /*videoView.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(TAG, "videoView is Clicked");
            }
        });*/
        videoView.setOnErrorListener(onVideoErrorListener);
        videoView.setOnPreparedListener(onVideoPreparedListener);
        videoView.setZOrderOnTop(true);
        videoView.setVisibility(View.VISIBLE);
        //Log.d(TAG, "videoView.start()");
        //videoView.start();
    }

    private void stopPlayback() {
        Log.d(TAG, "stopPlayback()");
        if (videoView != null) {
            if (videoView.isPlaying()) {
                Log.d(TAG, "videoView.stopPlayback()");
                videoView.stopPlayback();
            }
            Log.d(TAG, "videoView.setVisibility(INVISIBLE)");
            videoView.setZOrderOnTop(false);
            videoView.setVisibility(View.INVISIBLE);
        }
    }

    
    // select current source. If current source is unavailable, select the next one.
    // used in INIT_SURFACEVIEW_WIDGET_INTENT
    private int selectSource() {
        int returnedSource = -1;
        int failCounter = 0, targetSource = /*(currentSource == -1) ? 0 :*/ currentSource;
        
        while (true) {
            if ( setSource(targetSource) ) {
                returnedSource = currentSource = targetSource;
                break;
            } else if(targetSource==-1){
                break;
            } else {
                // find the next supported source
                failCounter++;
                targetSource = (targetSource + 1 < SOURCE_NUMBER) ? (targetSource + 1) : 0;
                // handle infinite loop
                if (failCounter == SOURCE_NUMBER) {
                    returnedSource = -1;
                    break;
                }
            }
        }
        return returnedSource;
    }

    // select available source. If current source is not available, it find the next available source. If all sources are unavailable, returns -1.
    // used in SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT
    private int selectNextSource() {
        int returnedSource = -1;
        int failCounter = 0, targetSource = currentSource;
        
        while (true) {
            targetSource = (targetSource + 1 < SOURCE_NUMBER) ? (targetSource + 1) : 0;
            Log.d(TAG, "selectNextSource:" + targetSource + " currentSource=" + currentSource);
            if (setSource(targetSource)) {
                returnedSource = currentSource = targetSource;
                break;
            } else { // source not supported
                //Log.d(TAG, "source:" + targetSource + " is not supported");
                failCounter++;
            }
            // handle infinite loop
            if (failCounter == SOURCE_NUMBER) {
                returnedSource = -1;
                break;
            }
        }
        return returnedSource;
    }

    private void doCleanUp() {
        videoWidth = 0;
        videoHeight = 0;
        isVideoReady = false;
        isVideoSizeKnown = false;
    }
    
    private void playVideo() {
        Log.d(TAG, "playVideo videoWidth:" + videoWidth + " videoHeight:" + videoHeight);
        mSurfaceHolder.setFixedSize(videoWidth, videoHeight);
        mp.start();
    }
    
    private void releaseMediaPlayer() {
        if (mp != null) {
            Log.d(TAG, "releaseMediaPlayer()");
            mp.stop();
            mp.release();
            mp = null;
        } else {
            Log.d(TAG, "releaseMediaPlayer() mp==null");
        }
    }
    private void initMediaPlayer() {
        Log.d(TAG, "initMediaPlayer()");
        doCleanUp();
        
        if (mp == null) {
            mp = new MediaPlayer();
        }
        try {
            mp.setDataSource(DTV_PATH);
            //Log.d(TAG, "initMediaPlayer() chk00");
            mp.setDisplay(mSurfaceHolder);
            Log.d(TAG, "initMediaPlayer() chk01");
            //mp.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mp.prepare();
            
            mp.setOnPreparedListener(new OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    Log.d(TAG, "mp.onPrepared()");
                    isVideoReady = true;
                    if (isVideoSizeKnown) {
                        playVideo();
                    }
                }
            });

            mp.setOnVideoSizeChangedListener(new OnVideoSizeChangedListener() {
                @Override
                public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
                    if (width == 0 || height == 0) {
                        Log.e(TAG, "onVideoSizeChanged Invalid width:" + width + " height:" + height);
                        return;
                    } else {
                        Log.d(TAG, "onVideoSizeChanged width:" + width + " height:" + height);
                    }
                    
                    isVideoSizeKnown = true;
                    videoWidth = width;
                    videoHeight = height;
                    if (isVideoReady) {
                        playVideo();
                    }
                }
            });
        } catch (Exception e) {
            Log.e(TAG, e.toString());
        }
    }

    class DTVSurfaceCallback implements SurfaceHolder.Callback {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            Log.d(TAG, "DTVSurfaceCallback.surfaceCreated()");
            initMediaPlayer();
            initializeDTV();
        }
        
        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            //Log.d(TAG, "DTVSurfaceCallback.surfaceChanged w:" + width + " h:" + height);
        }
        
        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.d(TAG, "DTVSurfaceCallback.surfaceDestroyed()");
            releaseMediaPlayer();
            doCleanUp();
        }
    }
    
    private void init() {
        if (ENABLE_1195_DTV_FLOW == true) {
            tunerStatus[0] = tunerStatus[1] = TvManager.DTV_TUNER_STATUS_INVALID;
        }
        else {
            tunerStatus[0] = tunerStatus[1] = TvManager.DTV_TUNER_STATUS_IDLE;
        }
        appType[0] = appType[1] = TvManager.NO_APP;
    }

    private void getPreference() {
        Log.d(TAG, "getPreference()");
        settings     = mContext.getSharedPreferences(PREFS_NAME, Context.MODE_MULTI_PROCESS);
        region       = settings.getInt(PREFS_REGION, -1);
        serviceType  = settings.getInt(PREFS_TV_SERVICE, DvbChannelInfo.SERVICE_TYPE_DTV);
        if (serviceType == DvbChannelInfo.SERVICE_TYPE_DTV) {
            prevChannel = tvChannel = settings.getInt(PREFS_TV_CHANNEL, -1);
        } else if (serviceType == DvbChannelInfo.SERVICE_TYPE_RADIO) {
            prevChannel = radioChannel = settings.getInt(PREFS_RADIO_CHANNEL, -1);
        }
    }
    
    private int acquireTunerForPreview() {
        Log.d(TAG, "acquireTunerForPreview()");
        int i = 0;
        for (i=0; i<2; i++) {
            if (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_PREVIEW) {
                return i;
            }
        }

        if (ENABLE_1195_DTV_FLOW == true) {
            int tunerId = m_tvManager.GetAvailableTunerId();
            Log.v(TAG, "got tunerId="+tunerId);
            if (tunerId >= 0) {
                appType[tunerId] = m_tvManager.Init(tunerId, TvManager.APP_DTV);
                if (appType[tunerId] > 0) {
                    tunerStatus[tunerId] = TvManager.DTV_TUNER_STATUS_PREVIEW;
                    return tunerId;
                }
            }
        }
        else {
            for (i=0;i<2;i++) {
                if (((ENABLE_1195_DTV_FLOW == true) && (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_INVALID))
                    || (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_IDLE)) {
                    if (m_tvManager.Init(i, TvManager.APP_DTV) > 0) {
                        tunerStatus[i] = TvManager.DTV_TUNER_STATUS_PREVIEW;
                        return i;
                    }               
                }
            }
        }

        return (-1);
    }

    private void uninitDTV() {
        if (m_tvManager != null) {
            Log.d(TAG, "uninitDTV()");

            if (ENABLE_1195_DTV_FLOW == true) {
                if (m_tvManager.DestroyDecodeFlow(decodingTunerId) == false) {
                    Log.e(TAG, "DestroyDecodeFlow() NG.decodingTunerId="+decodingTunerId);
                }
            }

            releaseMediaPlayer();

            for (int i=0;i<2;i++) {
                if (tunerStatus[i] == TvManager.DTV_TUNER_STATUS_PREVIEW) {
                    if (m_tvManager.UnInit(i, TvManager.APP_DTV) > 0) {
                        Log.d(TAG, "m_tvManager.UnInit() Ok.");
                    } else {
                        Log.e(TAG, "m_tvManager.UnInit() NG.");
                    }
                    if (ENABLE_1195_DTV_FLOW == true) {
                        tunerStatus[i] = TvManager.DTV_TUNER_STATUS_INVALID;
                    }
                    else {
                        tunerStatus[i] = TvManager.DTV_TUNER_STATUS_IDLE;
                    }
                }
            }
            m_tvManager.SetBroadcastIntentAction(null);
            m_tvManager = null;
            isServerConnected = false;
        } else {
            Log.d(TAG, "uninitDTV() m_tvManager==null");
        }
        Log.d(TAG, "uninitDTV() mSurfaceView false,INVISIBLE");
        mSurfaceView.setZOrderOnTop(false);
        mSurfaceView.setVisibility(View.INVISIBLE);
        Log.d(TAG, "uninitDTV() removeCallback(DTVCallback)");
        mSurfaceHolder.removeCallback(DTVCallback);
    }
    
    private void initCamera() {
        Log.d(TAG, "initCamera()");
        if (mSurfaceHolder == null) {
            Log.d(TAG, "initCamera mSurfaceHolder == null");
        } else {
            //Log.d(TAG, "initCamera mSurfaceHolder NOT null");
            //if (mSurfaceHolder.getCallback())
            Log.d(TAG, "initCamera() addCallback(cameraCallback)");

            ImageView icon = (ImageView) mRootView.findViewById(R.id.icon);
            icon.setVisibility(View.VISIBLE);
            icon.setImageResource(R.drawable.camera_100);
            mSurfaceView.setVisibility(View.GONE);
            mSurfaceHolder.addCallback(cameraCallback);
            Log.d(TAG, "initCamera() mSurfaceView true,VISIBLE");
            mSurfaceView.setZOrderOnTop(true);
            mSurfaceView.setVisibility(View.VISIBLE);
            isCameraInitialized = true;
        }
    }

    private void uninitCamera() {
        Log.d(TAG, "uninitCamera()");

        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
        previewing = false;
        Log.d(TAG, "uninitCamera() removeCallback(cameraCallback)");
        mSurfaceHolder.removeCallback(cameraCallback);
        Log.d(TAG, "uninitCamera() mSurfaceView false,GONE");
        mSurfaceView.setZOrderOnTop(false);
        mSurfaceView.setVisibility(View.GONE);
    }

    private void initHDMIRX() {
        synchronized(mHDMIRXLock) {
            Log.d(TAG, "initHDMIRX()");
            if (mSurfaceHolder == null) {
                Log.d(TAG, "initHDMIRX mSurfaceHolder == null");
            } else {
                Log.d(TAG, "initHDMIRX() addCallback(HDMIRXSurfaceCallback)");

                ImageView icon = (ImageView) mRootView.findViewById(R.id.icon);
                icon.setVisibility(View.VISIBLE);
                icon.setImageResource(R.drawable.hdmi_in_100);

                mSurfaceHolder.addCallback(HDMIRXCallback);
                Log.d(TAG, "initHDMIRX() mSurfaceView true,VISIBLE");
                mSurfaceView.setZOrderOnTop(true);
                mSurfaceView.setVisibility(View.VISIBLE);
                isHDMIRXInitialized = true;
            }
        }
    }

    private void uninitHDMIRX() {
        synchronized(mHDMIRXLock) {
            Log.d(TAG, RED+"uninitHDMIRX()"+END);
            mSurfaceView.setVisibility(View.INVISIBLE);
            if (mHDMIRX != null) {
                //mHDMIRX.stop();
                mHDMIRX.release();
                mHDMIRX = null;
            }
            previewing = false;
            Log.d(TAG, RED+"uninitHDMIRX() removeCallback(HDMIRXSurfaceCallback)"+END);
            mSurfaceHolder.removeCallback(HDMIRXCallback);
            Log.d(TAG, RED+"uninitHDMIRX() mSurfaceView false,GONE"+END);
            mSurfaceView.setZOrderOnTop(false);
        }
    }

    private boolean isSourceInitialized(int s) {
        
        boolean isInitialized = false;
        
        switch (s) {
        case SOURCE_DTV:
            isInitialized = isDTVInitialized;
            break;
        case SOURCE_CAMERA:
            isInitialized = isCameraInitialized;
            break;
        case SOURCE_HDMI:
            isInitialized = isHDMIRXInitialized;
            break;
        case SOURCE_LOCAL_FILE:
            isInitialized = isLocalFileInitialized;
        default:
            break;
        }
        Log.d(TAG, "isSourceInitialized:" + s + " isInitialized=" + isInitialized);
        return isInitialized;
    }

    public void stopSource() {
        stopSource(currentSource);
    }

    private synchronized void stopSource(int s) {
        Log.d(TAG, "stopSerivce:" + s);
        switch (s) {
        case SOURCE_DTV:
            if (isDTVInitialized) {
                uninitDTV();
                isDTVInitialized = false;
            }
            break;
        case SOURCE_CAMERA:
            if (isCameraInitialized) {
                uninitCamera();
                isCameraInitialized = false;
            }
            break;
        case SOURCE_HDMI:
            if (isHDMIRXInitialized) {
                uninitHDMIRX();
                isHDMIRXInitialized = false;
            }
            break;

        case SOURCE_LOCAL_FILE:
            if (isLocalFileInitialized) {
                isLocalFileInitialized = false;
                stopPlayback();
            }
            break;

        default:
            break;
        }
    }

    // uninit current source and initialize preferred one
    private boolean setSource(int s) {
        boolean result = false;
        Log.d(TAG, "setSource:" + s + " currentSource=" + currentSource);
        
        // check if source is supported
        switch(s) {
        case SOURCE_DTV:
            if (m_tvManager == null) {
                m_tvManager = new TvManager();
            }

            if (m_tvManager.ConnectToServer() == false) {
                Log.d(TAG, "ConnectToServer failed");
                return result;
            }
            
            if (m_tvManager.GetAvailableTunerId() == -1) { // -1 means no available tuner
                Log.d(TAG, "GetAvailableTunerId() == -1");
                return result;
            }
            break;
        case SOURCE_CAMERA:
            // if there is criteria for checking camera, put them here
            break;
        case SOURCE_HDMI:
            // if there is criteria for checking hdmi, put them here
            break;
        case SOURCE_LOCAL_FILE:
            break;
        default:
            break;
        }

        if (s != currentSource) {
            stopSource(currentSource);
        }
        
        // start service
        switch (s) {
        case SOURCE_DTV:
            if (!isSourceInitialized(SOURCE_DTV)) {
                Log.d(TAG, "Inside setSource(DTV)");
                //initMediaPlayer();
                //initializeDTV();

                ImageView icon = (ImageView) mRootView.findViewById(R.id.icon);
                icon.setVisibility(View.VISIBLE);
                icon.setImageResource(R.drawable.dtv_100);

                Log.d(TAG, "setSource() addCallback(DTVCallback)");
                mSurfaceHolder.addCallback(DTVCallback);
                mSurfaceView.setZOrderOnTop(true);
                mSurfaceView.setVisibility(View.VISIBLE);
            }
            result = true;
            break;
        case SOURCE_CAMERA:
            if (!isSourceInitialized(SOURCE_CAMERA)) {
                initCamera();
            }
            result = true;
            break;
        case SOURCE_LOCAL_FILE:
            if (!isLocalFileInitialized) {
                startPlayback();
                isLocalFileInitialized = true;
            }
            result = true;
            break;
        case SOURCE_HDMI:
            if (!isSourceInitialized(SOURCE_HDMI)) {
                initHDMIRX();
            }
            result = true;
			break;
        default:
            ImageView icon = (ImageView) mRootView.findViewById(R.id.icon);
            icon.setVisibility(View.INVISIBLE);
            break;
        }
        
        return result;
    }

    private void initializeDTV() {
        Log.d(TAG, "initializeDTV()");
        
        init();
        
        if (m_tvManager == null) {
            m_tvManager = new TvManager();
        }
 
        isServerConnected = m_tvManager.ConnectToServer();
        if (isServerConnected) {
            Log.d(TAG, "ConnectToServer() OK!!");
        } else {
            Log.e(TAG, "ConnectToServer() NG");
            return;
        }
        
        //m_tvManager.SetBroadcastIntentAction("com.example.testtvmanager.action.DtvEvent");
        getPreference();
        //Log.d(TAG, "after getPreference()");
        m_tvManager.SetTvRegion(region);
        tunerId = acquireTunerForPreview();
        Log.d(TAG, "tunerId:" + tunerId);
        if (serviceType == DvbChannelInfo.SERVICE_TYPE_DTV) {
            filterOutFlags = filterOutFlags & ~TvManager.CHANNEL_FLAG_IS_DTV | TvManager.CHANNEL_FLAG_IS_RADIO;
        } else if (serviceType == DvbChannelInfo.SERVICE_TYPE_RADIO) {
            filterOutFlags = filterOutFlags & ~TvManager.CHANNEL_FLAG_IS_RADIO | TvManager.CHANNEL_FLAG_IS_DTV;
        }
        //channelCount = 0; // for debugging
        channelCount = m_tvManager.GetChannelCount(filterOutFlags); // need to check different service??
        
        Log.d(TAG, "onCreate channelCount:" + channelCount);
        if (channelCount > 0) { // scan has been performed
            currentChannel = prevChannel; // return -1 if there is no previous selected channel
            
            if (ENABLE_1195_DTV_FLOW == true) {
                if (m_tvManager.CreateDecodeFlow(tunerId) == false) {
                    Log.e(TAG, "CreateDecodeFlow() NG.tunerId="+tunerId);
                    return;
                }
                decodingTunerId = tunerId;
            }

            if ((ENABLE_1195_DTV_FLOW == true) && (appType[tunerId] != TvManager.APP_DTV)) {    // this tuner used by APP_TOGO and APP_DTV
                DvbChannelInfo dvbChannelInfo = m_tvManager.GetCurChannel(tunerId);
                if (null == dvbChannelInfo) {
                    Log.e(TAG, "GetCurChannel() NG.");
                    return;
                }
                if (m_tvManager.PlayChannel(tunerId, dvbChannelInfo.logicalChNum, true, true) == false) {
                    Log.e(TAG, "PlayChannel() NG.tunerId="+tunerId+".LCN="+dvbChannelInfo.logicalChNum);
                    return;
                }
                else {
                    Log.v(TAG, "PlayChannel() OK.tunerId="+tunerId+".LCN="+dvbChannelInfo.logicalChNum);
                }
            }
            else {  // this tuner only used by APP_DTV
                if (prevChannel == -1) { // no prev channel, play the first one
                    DvbChannelInfo dvbChannelInfo = m_tvManager.GetFirstChannel(filterOutFlags);
                    if (null == dvbChannelInfo) {
                        Log.e(TAG, "GetFirstChannel() NG.");
                        return;
                    }
                    if (m_tvManager.PlayChannel(tunerId, dvbChannelInfo.logicalChNum, true, true) == false) {
                        //Toast.makeText(getApplicationContext(), "Preview DVB NG", Toast.LENGTH_SHORT).show();
                        Log.e(TAG, "PlayChannel(0) NG.");
                        return;
                    } else {
                        Log.d(TAG, "PlayChannel(0) OK.");
                        currentChannel = dvbChannelInfo.logicalChNum;
                    }
                } else {
                    if (m_tvManager.PlayChannel(tunerId, prevChannel, true, true) == false) {
                        Log.e(TAG, "PlayChannel(" + prevChannel + ") NG.");
                        return;
                    } else {
                        Log.d(TAG, "PlayChannel(" + prevChannel + ") OK.");
                        currentChannel = prevChannel;
                    }
                }
            }   
            //displayChannelNumber(null);
        }
        isDTVInitialized = true;
        //uiHandler = new UIHandler(Looper.getMainLooper());
        Log.d(TAG, "initializeDTV() mSurfaceView true,VISIBLE");
        mSurfaceView.setZOrderOnTop(true);
        mSurfaceView.setVisibility(View.VISIBLE);
    }

    public void startActivity() {
        Intent launchIntent = null;

        // check intent
        switch (currentSource) {
        case SOURCE_DTV:
            //launchIntent = mLauncher.getPackageManager().getLaunchIntentForPackage("com.realtek.dtv");
            break;
        case SOURCE_CAMERA:
            //launchIntent = mLauncher.getPackageManager().getLaunchIntentForPackage("com.android.testingcamera");
            break;
        case SOURCE_HDMI:
            launchIntent = mLauncher.getPackageManager().getLaunchIntentForPackage("com.rtk.SourceIn");
            break;
        case SOURCE_LOCAL_FILE:
            break;
        default:
            break;
        }

        if (launchIntent != null) {
            stopSource(currentSource);
            launchIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            mLauncher.startActivity(launchIntent);
        } else {
            Log.e(TAG, "application not installed. currentSource:" + currentSource);
        }
    }
}
