package com.realtek.transcode;

import java.lang.StringBuffer;
import java.util.ArrayList;
import java.io.IOException;
import java.text.DecimalFormat;
import android.app.Activity;

import android.content.Intent;
import android.graphics.Color;
import android.media.MediaMuxer;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.view.MotionEvent;
import android.graphics.PixelFormat;
import android.view.View;
import android.view.View.OnTouchListener;
import android.media.AudioManager;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.SystemProperties;
import android.os.Handler;
import android.os.Message;
import android.os.Messenger;
import android.content.ComponentName;
import android.content.Context;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.widget.Toast;
import android.os.Looper;


import com.realtek.transcode.IRealtekTranscodeService;

public class DecodeActivity extends Activity implements SurfaceHolder.Callback {
    public static final String TAG = "DecodeActivity";
    private static final int ENCODER_INSTANCES_LIMIT = 4;
    private int mInputNum = 1;
    private static final int MAX_GRID_NUM = ENCODER_INSTANCES_LIMIT;
    private String mSourcePath[] = new String[MAX_GRID_NUM];
    private String mTargetPath[] = new String[MAX_GRID_NUM];
    private int mTrackOutput = 2;
    private int surfaceViewArray = 0;
    private muxerMonitorThread mMuxerMonitorThread = new muxerMonitorThread();
    private SurfaceView mSurfaceView = null;
    private SurfaceHolder mSurfaceHolder = null;
    private ArrayList<String> playList = null;
    private ArrayList<String> targetList = null;
    private String mOutoutFormat = "";
    private String mExitType = "";
    private String mAudioMode = "";
    private int mTargetWdith = 0;
    private int mTargetHeight = 0;
    private int mDeinterlace = 1;
    private int mBitrateMode = 0;
    private int mProfile = 1;
    private int mLevel = 1;
    private int mSampleRate = 0;
    private int mChannel = 0;
    private int mIsStreaming = 0;
    private String mIP = "";
    private int mPort = 0;
    private int mRepeat = 0;
    private String mFrameRate = "";
    private int mBitRate = 0;
    private int mIFrameInterval = 0;
    private String stateArray[] = new String[MAX_GRID_NUM];
    private String stateInstantArray[] = new String[MAX_GRID_NUM];
    private IRealtekTranscodeService mService;
    private int mIdx[] = new int[MAX_GRID_NUM];
    private Intent mIntent = null;
    private Handler mHandler = null;
    private Messenger mMessenger = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        
        Intent it = getIntent();
        Bundle bundle = it.getExtras();
        mInputNum = bundle.getInt("DATA_INT");
        mTrackOutput = bundle.getInt("TRACK_INT");
        mOutoutFormat = bundle.getString("OUTPUT_FORMAT");
        mExitType = bundle.getString("EXIT_TYPE");
        mFrameRate = bundle.getString("FRAME_RATE");
        mBitRate = bundle.getInt("BIT_RATE");
        mIFrameInterval = bundle.getInt("FRAME_INTERVAL");
        mAudioMode = bundle.getString("AUDIO_MODE");
        mTargetWdith = bundle.getInt("WIDTH");
        mTargetHeight = bundle.getInt("HEIGHT");
        mDeinterlace = bundle.getInt("DE_INTERLACE");
        mBitrateMode = bundle.getInt("BITRATE_MODE");
        mProfile = bundle.getInt("PROFILE");
        mLevel = bundle.getInt("LEVEL");
        mSampleRate = bundle.getInt("SAMPLE_RATE");
        mChannel = bundle.getInt("CHANNEL");
        mIsStreaming = bundle.getInt("STREAMING");
        mIP = bundle.getString("IP");
        mPort = bundle.getInt("PORT");
        mRepeat = bundle.getInt("REPEAT");
        playList = bundle.getStringArrayList("LIST");
        for(int i=0; i<MAX_GRID_NUM; i++)
        {
            mSourcePath[i] = playList.get(i);
            Log.d(TAG, "Source " + i + " = " + mSourcePath[i]);
        }
        targetList = bundle.getStringArrayList("LIST_TARGET");
        for(int i=0; i<MAX_GRID_NUM; i++)
        {
            if(mIsStreaming == 1)
                mTargetPath[i] = "UDP";
            else
                mTargetPath[i] = targetList.get(i);
            Log.d(TAG, "Target " + i + " = " + mTargetPath[i]);
        }
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.recgrid);
        setupViewArray();
        setupViewComponent();
    }

    private void setupViewArray() {
        surfaceViewArray = R.id.surfaceView1;
    }

    private void setupViewComponent() {
        Log.d(TAG, "mInputNum " + mInputNum);
        mSurfaceView = (SurfaceView) findViewById(surfaceViewArray);
        mSurfaceHolder = mSurfaceView.getHolder();
        mSurfaceHolder.setFixedSize(1920, 1080);
        mSurfaceHolder.addCallback(this);
    }

    private void prepareIntent()
    {
        mHandler = new Handler(Looper.getMainLooper())
        {
            @Override
            public void handleMessage(Message msg) {
                String notice = "";
                if(msg.arg2 == 1)
                    notice = "Transcode finished !!";
                else if(msg.arg2 == -1)
                    notice = "Transcode crash, force to stop current job !!";
                Log.d(TAG, "Received transcode idx " + msg.arg1 + " state " + msg.arg2 + " " + notice);
            }
        };
        mMessenger = new Messenger(mHandler);

        mIntent = new Intent();
        mIntent.setAction("realtek.transcodeservice");
        mIntent.setPackage("com.realtek.transcode");
        mIntent.putExtra("MSG_HANDLER", mMessenger);
    }

    private ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            mService = IRealtekTranscodeService.Stub.asInterface(service);
            int err = 0;
            err = startAllTranscodeService();
            if(err == 0)
			{
				Log.d(TAG,"Monitor log: every button can start MonitorThread");
                if(mMuxerMonitorThread == null)
                    mMuxerMonitorThread = new muxerMonitorThread();
				mMuxerMonitorThread.start();
			}
            else
                DecodeActivity.this.finish();
        }

        public void onServiceDisconnected(ComponentName className) {
            mService = null;
        }
    };

    public int startAllTranscodeService() {
        int err = 0;
        int i;

        for(i=0; i<mInputNum; i++)
        try{
            if(mService == null)
                return -1;
            mIdx[i] = mService.getTranscodeInstanceIdx();
            if(mIdx[i] != -1)
            {
                mService.setDataSource(mIdx[i], mSourcePath[i], mTargetPath[i], mOutoutFormat, mTrackOutput, mRepeat);
                mService.setVideoParameter(mIdx[i], mTargetWdith, mTargetHeight, mFrameRate, mBitRate, mIFrameInterval, mDeinterlace, mBitrateMode, mProfile, mLevel);
                mService.setAudioParameter(mIdx[i], mAudioMode, mSampleRate, mChannel);
                if(mIsStreaming == 1)
                    mService.setStreamingParameter(mIdx[i], mIP, mPort);
                err = mService.startTranscode(mIdx[i]);
                if(err == -1)
                {
                    Log.d(TAG, "Transcode create fail ...");
                    mService.stopTranscode(mIdx[i]);
                }
            }
            else
                err = -1;
        }catch(RemoteException e){
            e.printStackTrace();
        }
        return err;
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "onPause ......... !!!");
        int i;
        ForceStopMuxerMonitorThread();

        if(mExitType.compareTo("Terminated") == 0) {
            for(i=0; i<mInputNum; i++)
            {
                try {
                mService.stopTranscode(mIdx[i]);
                }catch(RemoteException e){
                    e.printStackTrace();
                }
            }
        }else{
			//John
			Log.d(TAG, "keep going and do nothing");
		}
        unbindService(mConnection);
        mHandler.removeCallbacksAndMessages(null);
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop ......... !!!");
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy ......... !!!");
        super.onDestroy();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        ShowTitle();    
        prepareIntent();
        bindService(mIntent, mConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {   
        Log.d(TAG, "SufaceDestroyed ......... !!!");        
    }

    private void ForceStopMuxerMonitorThread(){
        if(mMuxerMonitorThread != null)
        {
            if(mMuxerMonitorThread.isAlive())
            {
                Log.d(TAG, "Force stop ForceStopMuxerMonitorThread");
                mMuxerMonitorThread.stopThread();
                while(mMuxerMonitorThread.isAlive())
                {
                    try {
                        Thread.sleep(500);
                        Log.d(TAG, "ForceStopMuxerMonitorThread WAIT ...");
                    } catch(InterruptedException  ex) {
                    }
                }
            }
            mMuxerMonitorThread = null;
        }
    }
    private void ShowTitle()
    {        
        String s = "Encoder Demo";
        setTitle(s);
    }

    public class muxerMonitorThread extends Thread {
        
        private boolean stopThread = false;
        private int i = 0;
        private int mInstantCount = 0;
        @Override
        public void run() {
            Log.d(TAG, "Prepare to execute muxerMonitorThread !!!");
            while (!stopThread) {
                    try {
                        Thread.sleep(1000);
                        mInstantCount++;
//                        Log.d(TAG, "muxerMonitorThread normally WAIT ...");
                        for (int i = 0; i < ENCODER_INSTANCES_LIMIT; i++) {
                            long tempTimeStamp = 0;
                            DecimalFormat df = new DecimalFormat("#.##");
                            String decodeFrameRate = "";
                            String encodeFrameRate = "";
                            String decodeInstantFrameRate = "";
                            String encodeInstantFrameRate = "";
                            String threadStatus = "";
                            boolean videoAlive = false;
                            boolean audioAlive = false;

                            try{
                            tempTimeStamp = mService.getCurrentPTS(i);
                            decodeFrameRate = df.format(mService.getDecodeAverageFPS(i));
                            encodeFrameRate = df.format(mService.getEncodeAverageFPS(i));
                            videoAlive = mService.isVideoTranscodeAlive(i);
                            audioAlive = mService.isAudioTranscodeAlive(i);
                            }catch(RemoteException e){
                                e.printStackTrace();
                            }
                            
                            if(mTrackOutput == 2 && videoAlive==false && audioAlive==true)
                                threadStatus = " (Waiting Audio)";
                            if(mInstantCount >= 3)
                            {
                                try{
                                decodeInstantFrameRate = df.format(mService.getVideoDecodeCurrentFPS(i)/3.0);
                                encodeInstantFrameRate = df.format(mService.getVideoEncodeCurrentFPS(i)/3.0);
                                }catch(RemoteException e){
                                    e.printStackTrace();
                                }
                            }
                         /** For background transcoding
                            int inputFileIdx = mSourcePath[i].lastIndexOf('/') + 1;
                            int outFileIdx = mTargetPath[i].lastIndexOf('/') + 1;
                            if((mSourcePath[i].length() - inputFileIdx) > 10)
                                inputFileIdx = mSourcePath[i].length() - 10;
                            if((mTargetPath[i].length() - outFileIdx) > 10)
                                outFileIdx = mTargetPath[i].length() - 10;
                            String tempSample = mSourcePath[i].substring(inputFileIdx);
                            String tempTarget = mTargetPath[i].substring(outFileIdx);
                            **/
                            String tempSample = "[" + i + "]";
                            String tempTarget = tempSample;
                            if(videoAlive || audioAlive)
                            {
                                stateArray[i] = tempSample + " current PTS " + tempTimeStamp/60 + "m " + tempTimeStamp%60 + "s " +  "Decode FPS " + decodeFrameRate + " Encode FPS " + encodeFrameRate + threadStatus;
                                stateInstantArray[i] = tempSample + " Decode Instant FPS " + decodeInstantFrameRate + " Encode Instant FPS " + encodeInstantFrameRate;
                                try {
                                if(mService.getTranscodeState(i) == -1)
                                    stateArray[i] = tempTarget + " Something wrong, transcoder TERMINATED !!!";
                                }catch(RemoteException e){
                                    e.printStackTrace();
                                }
                            }
                            else
                            {
                                try {
                                if(mService.getTranscodeState(i) == -1)
                                    stateArray[i] = tempTarget + " Something wrong, transcoder TERMINATED !!!";
                                else if(mService.getTranscodeState(i) == 0)
                                    stateArray[i] = tempTarget + " transcode IDLE ";
                                else if(mService.getTranscodeState(i) == 1)
                                    stateArray[i] = tempTarget + " transcode DONE ";
                                }catch(RemoteException e){
                                    e.printStackTrace();
                                }
                                stateInstantArray[i] = "";
                            }
                            
                        }

                        Canvas canvas = null;
                        StringBuffer result = new StringBuffer();
                        for (int i = 0; i < ENCODER_INSTANCES_LIMIT; i++)
                        {
                            result.append(stateArray[i]);
                            result.append("\r\n");
                        }
                        try {
                            canvas = mSurfaceHolder.lockCanvas();
                            synchronized (mSurfaceHolder) {
                                canvas.drawColor(Color.WHITE);
                                Paint paint = new Paint();
                                paint.setColor(Color.RED);
                                paint.setTextSize(40);
                                paint.setFakeBoldText(true);
                                for (int i = 0; i < ENCODER_INSTANCES_LIMIT; i++)
                                {
                                    canvas.drawText(stateArray[i], 100, 100 + i * 100, paint);
                                    Log.d(TAG, stateArray[i]);
                                    if(mInstantCount == 3)
                                        Log.d(TAG, stateInstantArray[i]);
                                }
                                if(mInstantCount == 3)
                                    mInstantCount = 0;
                            }
                        } catch (Exception e) {
                            Log.e(TAG, "run() lockCanvas()", e);
                        } finally {
                            if (canvas != null) {
                                mSurfaceHolder.unlockCanvasAndPost(canvas);
                            }
                        }
                    } catch(InterruptedException  ex) {
                    }
            }
            
        }
        public void stopThread() {
            Log.d(TAG, "muxerMonitorThread being terminated");
            stopThread = true;
        }
    }
}
