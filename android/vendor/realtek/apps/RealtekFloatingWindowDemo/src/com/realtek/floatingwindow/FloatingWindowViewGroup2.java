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
import android.view.Surface;
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
import android.media.MediaPlayer;

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
 *  the control class of a floating Window with MediaPlayer.
 *
 */
public class FloatingWindowViewGroup2 {

    private static final String TAG="FloatingWindowViewGroup2";

    private static final int TYPE_SURFACEVIEW   = SettingWidgets.TYPE_SURFACEVIEW;
    private static final int TYPE_TEXTUREVIEW   = SettingWidgets.TYPE_TEXTUREVIEW;
    private static final int TYPE_NOPREVIEW     = SettingWidgets.TYPE_NOPREVIEW;

    public FloatingWindowView mFloatingView;
    public View mPreview;
    public SurfaceView mSurfaceView;
    public SurfaceHolder mSurfaceHolder;
    public TextureView mTextureView;
    public SurfaceTexture mSurfaceTextureForNoPreview;

    public FloatingWindowSurfaceCallback mCallback;
    public FloatingWindowTextureListener mListener;
    public WindowManager.LayoutParams wmParams;

    public int x;
    public int y;
    public int width;
    public int height;

    public boolean bPlaying = false;

    private int mViewType = TYPE_SURFACEVIEW;

    private Context mContext;
    private Bundle mArg;
    private WindowManager mWindowManager = null;

    private MediaPlayer mPlayer;
    private String mFilePath;
    private boolean mBRTOn;

    public FloatingWindowViewGroup2(Context c, Bundle arg){
        mContext = c;
        mArg = arg;
        mWindowManager = (WindowManager)c.getSystemService(Context.WINDOW_SERVICE);

        int winSize = mArg.getInt(SettingWidgets2.KEY_SIZE);
        width = (winSize==SettingWidgets2.WINDOW_HD)?1920:640;
        height = (winSize==SettingWidgets2.WINDOW_HD)?1080:480;
        mFilePath = arg.getString(SettingWidgets2.KEY_FILE_PATH);
        mBRTOn = arg.getBoolean(SettingWidgets2.KEY_RTMEDIAPLAYER);
    }

    class FloatingWindowTextureListener implements TextureView.SurfaceTextureListener {
        @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
                Log.d(TAG, "onSurfaceTextureAvailable");
                play(new Surface(surface));
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
                play(arg0.getSurface());
            }

        @Override
            public void surfaceDestroyed(SurfaceHolder arg0) {
            }
    }

    void play(Surface surface){
        if(mPlayer == null){
            try{
                mPlayer = new MediaPlayer();
                mPlayer.setSurface(surface);
                mPlayer.setLooping(true);
                if(mBRTOn){
                    mPlayer.useRTMediaPlayer(MediaPlayer.FORCE_RT_MEDIAPLAYER);
                }
                mPlayer.setDataSource(mFilePath);
                mPlayer.prepare();
                mPlayer.start();
            }catch(IOException e){
                Toast.makeText(mContext,"MediaPlayer error",Toast.LENGTH_SHORT).show();
                Log.e(TAG, "IOException...");
            }
        }else{
            Log.e(TAG, "play is called, but mPlayer already been created.");
        }
    }

    void destroy(){
        Log.d(TAG, "destroy");
        if(mPlayer!=null){
            Log.d(TAG, "release MediaPlayer");
            mPlayer.release();
            mPlayer = null;
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
            Log.d(TAG, "ViewType is set to NO_PREVIEW, but should not reach here");
        }
    }

    public void attachPreview(){
        RelativeLayout.LayoutParams param = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);

        if(mViewType != FloatingWindowViewGroup2.TYPE_NOPREVIEW){
            mPreview.setLayoutParams(param);
            mFloatingView.addView(mPreview);
        }
    }

}
