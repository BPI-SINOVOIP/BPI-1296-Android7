package com.mediacodec;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.content.Intent;
import android.os.Handler;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup;
import android.util.Log;
import android.widget.Toast;

import com.realtek.hardware.RtkHDMIRxManager;
import com.realtek.hardware.RtkHDMIRxManager.Size;
import com.realtek.hardware.HDMIRxParameters;
import com.realtek.hardware.HDMIRxStatus;

import java.io.IOException;
import java.util.List;

public class HDMIRxPlayer
{
    private final String TAG = "HDMIRxPlayer";
    private RtkHDMIRxManager mHDMIRX = null;
    private SurfaceView mSurfaceView = null;
    private SurfaceHolder mSurfaceHolder = null;
    //private HDMIRXSurfaceCallback HDMIRXCallback = new HDMIRXSurfaceCallback();
    private Context mContext;
    private final Handler mHandler = new Handler();
    private boolean mIsPlaying = false;
    private int mPlayCount = 0;
    private int mWidth = 0;
    private int mHeight = 0;
    private int mFps = 0;
    private int mScanMode = 0;
    private Toast mToast;

    private final Runnable mPlay = new Runnable() {
        @Override
        public void run() {
            if(mPlayCount++<4)
                play();
        }
    };

    private BroadcastReceiver hdmiRxHotPlugReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
                boolean hdmiRxPlugged = intent.getBooleanExtra(HDMIRxStatus.EXTRA_HDMIRX_PLUGGED_STATE, false);
                if(hdmiRxPlugged) {
                    mPlayCount=0;
                    play();
                    Log.d(TAG, "HDMI Rx is plugged in ( "+ mWidth + "x" + mHeight +" )\n");
                } else {
                    stop();
                    Log.d(TAG, "HDMI Rx is pulled out\n");
                }
        }
    };
    
    public HDMIRxPlayer(Context context, SurfaceView view, SurfaceHolder holder)
    {
        mContext = context;
        mSurfaceView = view;
        mSurfaceHolder = holder;
        //mSurfaceHolder.addCallback(HDMIRXCallback);
        //mSurfaceHolder.setFixedSize(width, height);
        //mSurfaceView.setZOrderOnTop(false);
        //mSurfaceView.setVisibility(View.VISIBLE);

        mHDMIRX = new RtkHDMIRxManager();

        IntentFilter hdmiRxFilter = new IntentFilter(HDMIRxStatus.ACTION_HDMIRX_PLUGGED);
        mContext.registerReceiver(hdmiRxHotPlugReceiver, hdmiRxFilter);
    }

    public boolean setFixedSize(int width, int height)
    {
        mSurfaceHolder.setFixedSize(width, height);
        return true;
    }

    public boolean isPlaying()
    {
        return mIsPlaying;
    }

    //Implementation for HDMIRxHandlerInterface
    public boolean play()
    {
        //mSurfaceView.setVisibility(View.VISIBLE);

        if (mIsPlaying==false)
        {
            HDMIRxStatus rxStatus = mHDMIRX.getHDMIRxStatus();
            if (rxStatus!=null && rxStatus.status ==HDMIRxStatus.STATUS_READY){
                if (mHDMIRX.open()!=0){
                    mWidth = 0;
                    mHeight = 0;
                    Log.d(TAG, "open HDMI RX failed.");
                    mHandler.postDelayed(mPlay, 500);
                    return false;
                }
                HDMIRxParameters hdmirxGetParam = mHDMIRX.getParameters();
                getSupportedPreviewSize(hdmirxGetParam, rxStatus.width, rxStatus.height);
                mFps=getSupportedPreviewFrameRate(hdmirxGetParam);
                mScanMode=rxStatus.scanMode;
            }
            else{
                Log.d(TAG, "HDMIRX not ready");
                mHandler.postDelayed(mPlay, 500);
                return false;
            }
            try{
                mHDMIRX.setPreviewDisplay(mSurfaceHolder);
                HDMIRxParameters hdmirxParam = new HDMIRxParameters();
                hdmirxParam.setPreviewSize(mWidth, mHeight);
                hdmirxParam.setPreviewFrameRate(mFps);
                mHDMIRX.setParameters(hdmirxParam);
                if(mToast != null)
                    mToast.cancel();
                if(mScanMode==rxStatus.SCANMODE_INTERLACED)
                    mToast = Toast.makeText(mContext,"HDMI In plays ( "+ mWidth + "x" + mHeight +", "+mFps+"fps, Interlace )",Toast.LENGTH_LONG);
                else
                    mToast = Toast.makeText(mContext,"HDMI In plays ( "+ mWidth + "x" + mHeight +", "+mFps+"fps, Progressive )",Toast.LENGTH_LONG);
                mToast.show();
                Log.d(TAG, "play HDMI RX");
                mHDMIRX.play();
                mIsPlaying = true;
            }catch (IOException e){
                e.printStackTrace();
            }
        }
        else
        {
            Log.d(TAG, "It's playing");
            return false;
        }
        return true;
    }
    public boolean stop()
    {
        boolean rlt = true;
        //mSurfaceView.setVisibility(View.INVISIBLE);
        if (mIsPlaying==true)
        {
            mIsPlaying = false;
            if(mToast != null)
                mToast.cancel();
            mToast = Toast.makeText(mContext,"HDMI In stops.",Toast.LENGTH_LONG);
            mToast.show();
            Log.d(TAG, "stop HDMI RX");
            mHDMIRX.stop();
        }
        else
        {
            rlt =false;
        }
        mWidth = 0;
        mHeight = 0;
        mFps = 0;
        return rlt;
    }

    public void release()
    {
        mContext.unregisterReceiver(hdmiRxHotPlugReceiver);
        //mSurfaceHolder.removeCallback(HDMIRXCallback);
        if (mHDMIRX!= null)
        {
            mHDMIRX.release();
            mHDMIRX = null;
        }
    } 


    public void setSurfaceSize(int width, int height)
    {
        mSurfaceHolder.setFixedSize(width, height);
    }

    public int getHdmiInWidth()
    {
        return mWidth;
    } 

    public int getHdmiInHeight()
    {
        return mHeight;
    } 

    public int getHdmiInFps()
    {
        return mFps;
    }

    public int getHdmiInScanMode()
    {
        return mScanMode;
    }

    private void getSupportedPreviewSize(HDMIRxParameters hdmirxGetParam, int rxWidth, int rxHeight) {
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
        mWidth = retWidth;
        mHeight = retHeight;
    }

    private int getSupportedPreviewFrameRate(HDMIRxParameters hdmirxGetParam) {
        List<Integer> previewFrameRates = hdmirxGetParam.getSupportedPreviewFrameRates();
        int fps=0;
        if(previewFrameRates!=null && previewFrameRates.size()>0)
            fps = previewFrameRates.get(previewFrameRates.size()-1);
        else
            fps = 30;
        return fps;
    }

/**
    class HDMIRXSurfaceCallback implements SurfaceHolder.Callback {
        boolean SurfaceIsReady = false;
        int SurfaceWidth;
        int SurfaceHeight;
        @Override
        public void surfaceChanged(SurfaceHolder arg0, int arg1, int width, int height) {
            Log.d(TAG, "HDMIRXSurfaceCallback.surfaceChanged(): " + arg0.toString() + ", width: " + width + ", height: " + height);
            SurfaceIsReady = true;
            SurfaceWidth = width;
            SurfaceHeight = height;
            mPlayCount=0;
            play();
        }
        
        @Override
        public void surfaceCreated(SurfaceHolder arg0) {
            Log.d(TAG, "HDMIRXSurfaceCallback.surfaceCreated(): ");
        }
        @Override
        public void surfaceDestroyed(SurfaceHolder arg0) {
            Log.d(TAG, "HDMIRXSurfaceCallback.surfaceDestroyed(): ");
            SurfaceIsReady = false;
        }
    }
**/
}
