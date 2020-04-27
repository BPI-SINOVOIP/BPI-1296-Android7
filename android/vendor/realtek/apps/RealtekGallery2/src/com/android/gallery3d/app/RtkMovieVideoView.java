package com.android.gallery3d.app;

import android.widget.RtkVideoView;
import android.content.Context;
import android.util.AttributeSet;
import android.view.Display;
import android.graphics.Point;
import android.view.ViewGroup;
import android.view.View;
import android.media.AudioManager;
import android.media.MediaPlayer;

public class RtkMovieVideoView extends RtkVideoView implements View.OnLayoutChangeListener, MediaPlayer.OnVideoSizeChangedListener{
    private static final boolean DEBUG=false;
    private static final String TAG="RtkMovieVideoView";
    
    public MovieActivity mActivity=null;
    public MoviePlayer mPlayer=null;
    public View mRootView=null;
    public Context mContext=null;
    
    public RtkMovieVideoView(Context context) {
        super(context);
        mContext = context;
        //addOnLayoutChangeListener(this);
    }

    public RtkMovieVideoView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        mContext = context;
        //addOnLayoutChangeListener(this);
    }

    public RtkMovieVideoView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        mContext = context;
        //addOnLayoutChangeListener(this);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int rW = mRootView.getWidth();
        int rH = mRootView.getHeight();
        if(mActivity.mARFocus==MovieActivity.AR_FULL_INDEX){
            Log.d(TAG, "\033[0;31;31m onMeasure FULL \033[m");
            setMeasuredDimension(rW,rH);
            //super.onMeasure(widthMeasureSpec,heightMeasureSpec);
        }else{
            Log.d(TAG, "\033[0;31;31m onMeasure FIT \033[m");
            super.onMeasure(widthMeasureSpec,heightMeasureSpec);
        }
    }

    public void onLayoutChange (View v,
        int left, int top, int right, int bottom,
        int oldLeft, int oldTop, int oldRight, int oldBottom){

        Log.d(TAG,"\033[0;31;31m onLayoutChange -> left:"+left+" top:"+top+" right:"+right+" bottom:"+bottom+" \033[m");

        if(v==this){
            int h = bottom-top;
            int w = right-left;

            int oldH = oldBottom-oldTop;
            int oldW = oldRight-oldLeft;

            // video is visible
            if(h>1 && w>1){
                if(mContext!=null){
                    //Log.d(TAG,"\033[0;32;32m unmute audio \033[m");
                    //AudioManager amanager = (AudioManager)mContext.getSystemService(Context.AUDIO_SERVICE);
                    //amanager.setStreamMute(AudioManager.STREAM_SYSTEM, false);
                }
            }
        }
    }

    protected void onAttachedToWindow (){
        addOnLayoutChangeListener(this);
        // mute audio first
        if(mContext!=null){
            //Log.d(TAG,"\033[0;32;32m mute audio \033[m");
            //AudioManager amanager=(AudioManager)mContext.getSystemService(Context.AUDIO_SERVICE);
            //amanager.setStreamMute(AudioManager.STREAM_SYSTEM, true);
        }
        super.onAttachedToWindow();
    }

    protected void onDetachedFromWindow() {
        Log.d(TAG,"\033[0;32;32m remove layout change listener \033[m");
        removeOnLayoutChangeListener(this);
        super.onDetachedFromWindow();
    }

    protected void onLayout (boolean changed, int left, int top, int right, int bottom){
        Log.d(TAG,"\033[0;31;31m onLayout -> left:"+left+" top:"+top+" \033[m");
        super.onLayout(changed,left,top,right,bottom);
    }

    public void setOnVideoSizeChangedListener(MediaPlayer mp){
        mp.setOnVideoSizeChangedListener(this);
    }

    public void onVideoSizeChanged (MediaPlayer mp, int width, int height){
        mVideoWidth= mp.getVideoWidth();
        mVideoHeight= mp.getVideoHeight();
        if(mActivity.mARFocus==MovieActivity.AR_FULL_INDEX){
            Log.d(TAG,"\033[0;32;32m on video size changed, do nothing \033[m");
        }else{
            Log.d(TAG,"\033[0;31;31m onVideoSizeChanged width:"+mVideoWidth+" height:"+mVideoHeight+" \033[m");
            if (mVideoWidth != 0 && mVideoHeight != 0) {
                getHolder().setFixedSize(mVideoWidth, mVideoHeight);
                requestLayout();
            }
        }
    }
}
