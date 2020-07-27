package com.example.hdmirxdemo;

import android.content.Context;
import android.view.MotionEvent;
import android.util.AttributeSet;
import android.view.ViewGroup;
import android.view.View;
import android.view.ViewParent;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.util.Log;
import android.view.Gravity;
import android.graphics.PixelFormat;

public class FloatingWindowView extends FrameLayout {

    private static final String TAG = "FloatingWindowView";

    private int mDragStartX;
    private int mDragStartY;

    private int mXPos;
    private int mYPos;
    private int mWidth;
    private int mHeight;

    private Context mContext;

    private int xBeforeHide;
    private int yBeforeHide;

    public boolean mVisible = true;
    public int mAnimationResource = 0; //R.style.Animation_Window; //R.anim.no_anim; //android.R.style.Animation_Translucent;

    public static int[] sInvisibleLocation = {5000,5000,1920,1080};

    public FloatingWindowView(Context c){
        super(c);
        mContext = c;
    }

    public FloatingWindowView(Context c, AttributeSet attrs){
        super(c,attrs);
        mContext = c;
    }

    public FloatingWindowView(Context c, AttributeSet attrs, int defStyleAttr){
        super(c,attrs,defStyleAttr);
        mContext = c;
    }

    public FloatingWindowView(Context c, AttributeSet attrs, int defStyleAttr, int defStyleRes){
        super(c,attrs,defStyleAttr,defStyleRes);
        mContext = c;
    }

    public void updateLocation(int[] position, boolean touchable) {
        Log.d(TAG, "updateLocation");
        //updateLocation_2(sInvisibleLocation,touchable);
        updateLocation_2(position,touchable);
    }

    public void updateLocation_2(int[] position, boolean touchable) {

        int flag;
        if(touchable) {
            flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                    //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                    WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }else{
            flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                    WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                    WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }


        Log.i(TAG, "updateLocation_2 :"+position[0]+","+position[1]+","+position[2]+","+position[3]);

        WindowManager.LayoutParams wmParams = new WindowManager.LayoutParams(
                position[2],
                position[3],
                FloatingWindowService2.FLOATING_WINDOW_TYPE, //WindowManager.LayoutParams.TYPE_TOAST,
                flag,
                PixelFormat.TRANSLUCENT);

        wmParams.x = position[0];
        wmParams.y = position[1];

        wmParams.windowAnimations = mAnimationResource; //android.R.style.Animation_Toast;

        wmParams.gravity = Gravity.TOP|Gravity.LEFT;
        WindowManager wm = (WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE);
        wm.updateViewLayout(this, wmParams);
    }

    public void updateSize(int w, int h){
        int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                //WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        WindowManager.LayoutParams wmParams = new WindowManager.LayoutParams(
                w,
                h,
                FloatingWindowService2.FLOATING_WINDOW_TYPE, //WindowManager.LayoutParams.TYPE_TOAST,
                flag,
                PixelFormat.TRANSLUCENT);

        wmParams.x = 0;
        wmParams.y = 0;

        wmParams.gravity = Gravity.TOP|Gravity.LEFT;
        wmParams.windowAnimations = mAnimationResource; //android.R.style.Animation_Toast;
        WindowManager wm = (WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE);
        wm.updateViewLayout(this, wmParams);
    }

    public void setVisibile2(boolean on){

        if(on == mVisible)
            return;

        int flag;

        WindowManager.LayoutParams lParams = (WindowManager.LayoutParams) getLayoutParams();

        int x = lParams.x;
        int y = lParams.y;
        int w = lParams.width;
        int h = lParams.height;

        if(on){
            flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                //WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }else{
            flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        }

        if(!on){
            xBeforeHide = lParams.x;
            yBeforeHide = lParams.y;
        }

        WindowManager.LayoutParams wmParams = new WindowManager.LayoutParams(
                w,
                h,
                FloatingWindowService2.FLOATING_WINDOW_TYPE, //WindowManager.LayoutParams.TYPE_TOAST,
                flag,
                PixelFormat.TRANSLUCENT);

        if(on){
            //this.setAlpha(1);
            wmParams.x = xBeforeHide;
            wmParams.y = yBeforeHide;
        }else{
            //this.setAlpha(0);
            wmParams.x = 5000;
            wmParams.y = 5000;
        }

        //wmParams.x = x;
        //wmParams.y = y;

        wmParams.gravity = Gravity.TOP|Gravity.LEFT;
        wmParams.windowAnimations = mAnimationResource; //android.R.style.Animation_Toast;
        WindowManager wm = (WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE);
        wm.updateViewLayout(this, wmParams);
        mVisible = on;
    }


    public boolean onTouchEvent (MotionEvent event){
        final int x = (int) event.getRawX();
        final int y = (int) event.getRawY();

        //Log.d(TAG, "action:"+event.getAction()+" X:"+x+" y:"+y);

        switch (event.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
                {
                    WindowManager.LayoutParams lParams = (WindowManager.LayoutParams) getLayoutParams();
                    //Log.d(TAG, "MotionEvent.ACTION_DOWN "+lParams);
                    mXPos = lParams.x;
                    mYPos = lParams.y;
                    mWidth = lParams.width;
                    mHeight = lParams.height;

                    mDragStartX = x;
                    mDragStartY = y;
                    Log.d(TAG, "view pos "+mXPos+" "+mYPos+" - "+mDragStartX+" "+mDragStartY);
                }
            break;
            case MotionEvent.ACTION_MOVE:
                {
                    //ViewGroup.LayoutParams lParams = getLayoutParams();
                    //Log.d(TAG, "MotionEvent.ACTION_MOVE "+lParams);
                    int dragX = x - mDragStartX;
                    int dragY = y - mDragStartY;

                    int newPosX = mXPos+dragX;
                    int newPosY = mYPos+dragY;
                    int flag = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN|
                        //WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE|
                        //WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS|
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

                    // update layout
                    WindowManager.LayoutParams wmParams = new WindowManager.LayoutParams(
                            mWidth,//WindowManager.LayoutParams.MATCH_PARENT,
                            mHeight,//WindowManager.LayoutParams.MATCH_PARENT,
                            FloatingWindowService2.FLOATING_WINDOW_TYPE, //WindowManager.LayoutParams.TYPE_TOAST,
                            flag,
                            PixelFormat.TRANSLUCENT);

                    wmParams.x = newPosX;
                    wmParams.y = newPosY;
                    wmParams.gravity = Gravity.TOP|Gravity.LEFT;
                    wmParams.windowAnimations = mAnimationResource; //android.R.style.Animation_Toast;

                    WindowManager wm = (WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE);
                    wm.updateViewLayout(this, wmParams);

                    /*
                    setLayoutParams(wmParams);
                    Log.d(TAG, "ACTION_MOVE: setlayout:"+wmParams);
                    ViewParent parent = getParent();
                    Log.d(TAG, "parent:"+parent);
                    if(parent instanceof View){
                        View p = (View) parent;
                        Log.d(TAG, "invalidate");
                        p.invalidate();
                    }
                    */
                }
            break;
            default:
            break;
        }
        return true;
    }
}
