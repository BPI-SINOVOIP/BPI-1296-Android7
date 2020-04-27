/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gallery3d.app;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
//import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.View;

import com.android.gallery3d.R;
import com.android.gallery3d.common.Utils;
import android.util.AttributeSet;
import android.graphics.RectF;

/**
 * The time bar view, which includes the current and total time, the progress
 * bar, and the scrubber.
 */
public class TimeBar extends View {

    public interface Listener {
        void onScrubbingStart();

        void onScrubbingMove(int time);

        void onScrubbingEnd(int time, int start, int end);
    }

    // Padding around the scrubber to increase its touch target
    private static final int SCRUBBER_PADDING_IN_DP = 10;

    // The total padding, top plus bottom
    private static final int V_PADDING_IN_DP = 30;

    private static final int TEXT_SIZE_IN_DP = 30;

    protected Listener mListener;

    // the bars we use for displaying the progress
    protected Rect mProgressBar;
    protected Rect mPlayedBar;

    protected Paint mProgressPaint;
    protected Paint mPlayedPaint;
    protected Paint mTimeTextPaint;
    protected Paint mTotalTimeTextPaint;

    protected Bitmap mScrubber;
    protected Bitmap mProgressBarBitmap;
    protected Bitmap mPlayedBarBitmap;
    protected int mScrubberPadding; // adds some touch tolerance around the
                                    // scrubber

    protected int mScrubberLeft;
    protected int mScrubberTop;
    protected int mScrubberCorrection;
    protected boolean mScrubbing;
    protected boolean mShowTimes;
    protected boolean mShowScrubber;

    protected int mTotalTime;
    protected int mCurrentTime;

    protected Rect mTimeBounds;

    protected int mVPaddingInPx;
    protected float mTextSizeInPx;

    public TimeBar (Context context, AttributeSet attrs){
        super(context,attrs);
        initTimeBar(context);
    }
 
    public TimeBar (Context context, AttributeSet attrs, int defStyle){
        super(context,attrs,defStyle);
        initTimeBar(context);
    }
    public TimeBar(Context context) {
        super(context);
        initTimeBar(context);
    }

    public void initTimeBar(Context context) {
        mShowTimes = true;
        mShowScrubber = true;

        mProgressBar = new Rect();
        mPlayedBar = new Rect();

        mProgressPaint = new Paint();
        //mProgressPaint.setColor(0xFF808080);
        mPlayedPaint = new Paint();
        //mPlayedPaint.setColor(0xFFFFFFFF);

//        DisplayMetrics metrics = context.getResources().getDisplayMetrics();
//        mTextSizeInPx = metrics.density * TEXT_SIZE_IN_DP;
        mTextSizeInPx = (int)context.getResources().getDimension(R.dimen.video_timebar_text_size);
        mTimeTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG|Paint.FAKE_BOLD_TEXT_FLAG);
        mTimeTextPaint.setColor(0xFF19AEE6);
        mTimeTextPaint.setTextSize(mTextSizeInPx);
        mTimeTextPaint.setTextAlign(Paint.Align.RIGHT);

        mTotalTimeTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG|Paint.FAKE_BOLD_TEXT_FLAG);
        mTotalTimeTextPaint.setColor(0xFF6D828A);
        mTotalTimeTextPaint.setTextSize(mTextSizeInPx);
        mTotalTimeTextPaint.setTextAlign(Paint.Align.RIGHT);

        mTimeBounds = new Rect();
        mTimeTextPaint.getTextBounds("00:00:00", 0, 8, mTimeBounds);
        mTotalTimeTextPaint.getTextBounds("00:00:00", 0, 8, mTimeBounds);

        mScrubber = BitmapFactory.decodeResource(getResources(), R.drawable.scrubber_knob);
        mScrubberPadding = (int)context.getResources().getDimension(R.dimen.video_timebar_scrubber_padding);
//        mScrubberPadding = (int) (metrics.density * SCRUBBER_PADDING_IN_DP);

        mProgressBarBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.progress_bar_2);
        mPlayedBarBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.progress_bar_3);   

//        mVPaddingInPx = (int) (metrics.density * V_PADDING_IN_DP);
        mVPaddingInPx = (int)context.getResources().getDimension(R.dimen.video_timebar_v_padding);
    }
    public void setListener(Listener listener) {
        mListener = Utils.checkNotNull(listener);
    }
    private void update() {
        mPlayedBar.set(mProgressBar);

        if (mTotalTime > 0) {
            mPlayedBar.right =
                    mPlayedBar.left + (int) ((mProgressBar.width() * (long) mCurrentTime) / mTotalTime);
        } else {
            mPlayedBar.right = mProgressBar.left;
        }

        if (!mScrubbing) {
            mScrubberLeft = mPlayedBar.right - mScrubber.getWidth() / 2;
        }
        invalidate();
    }

    /**
     * @return the preferred height of this view, including invisible padding
     */
    public int getPreferredHeight() {
        return mTimeBounds.height() + mVPaddingInPx + mScrubberPadding;
    }

    /**
     * @return the height of the time bar, excluding invisible padding
     */
    public int getBarHeight() {
        return mTimeBounds.height() + mVPaddingInPx;
    }

    public void setTime(int currentTime, int totalTime,
            int trimStartTime, int trimEndTime) {
        if(currentTime>totalTime)
            currentTime = totalTime;
        if (mCurrentTime == currentTime && mTotalTime == totalTime) {
            return;
        }
        mCurrentTime = currentTime;
        mTotalTime = totalTime;
        update();
    }

    private boolean inScrubber(float x, float y) {
        int scrubberRight = mScrubberLeft + mScrubber.getWidth();
        int scrubberBottom = mScrubberTop + mScrubber.getHeight();
        return mScrubberLeft - mScrubberPadding < x && x < scrubberRight + mScrubberPadding
                && mScrubberTop - mScrubberPadding < y && y < scrubberBottom + mScrubberPadding;
    }

    private void clampScrubber() {
        int half = mScrubber.getWidth() / 2;
        int max = mProgressBar.right - half;
        int min = mProgressBar.left - half;
        mScrubberLeft = Math.min(max, Math.max(min, mScrubberLeft));
    }

    private int getScrubberTime() {
        return (int) ((long) (mScrubberLeft + mScrubber.getWidth() / 2 - mProgressBar.left)
                * mTotalTime / mProgressBar.width());
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        int w = r - l;
        int h = b - t;
        if (!mShowTimes && !mShowScrubber) {
            mProgressBar.set(0, 0, w, h);
        } else {
            int margin = mScrubber.getWidth() / 3;
            if (mShowTimes) {
                margin += mTimeBounds.width();
            }
            Log.d("TimeBar", "h = " + h);
            Log.d("TimeBar", "mScrubberPadding = " + mScrubberPadding);
           
            //int progressY = (h + mScrubberPadding) / 2;
           // mScrubberTop = progressY - mScrubber.getHeight() / 2 + 1;
            mProgressBar.set(getPaddingLeft(), mTimeBounds.height(), w - getPaddingRight(), h);
        }
        update();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        // draw progress bars
        //canvas.drawRect(mProgressBar, mProgressPaint);
        //canvas.drawRect(mPlayedBar, mPlayedPaint);

        canvas.drawBitmap(mProgressBarBitmap, null, new RectF(mProgressBar), mProgressPaint);
        canvas.drawBitmap(mPlayedBarBitmap, null, new RectF(mPlayedBar), mPlayedPaint);

        // draw scrubber and timers
        //if (mShowScrubber) {
        //    canvas.drawBitmap(mScrubber, mScrubberLeft, mScrubberTop, null);
        //}
        if (mShowTimes) {
            Log.d("TimeBar", "drawText x = " + (mPlayedBar.right -  mTimeBounds.width()));
         
            canvas.drawText(
                    stringForTime(mTotalTime),
                            getWidth() - getPaddingRight() /*- mTimeBounds.width() / 2*/,
                            mTimeBounds.height(),
                    mTotalTimeTextPaint);

            canvas.drawText(
                    stringForTime(mCurrentTime),
//                            (mPlayedBar.right>mTimeBounds.right)? mPlayedBar.right : mTimeBounds.right,
                            mTimeBounds.right,
                            mTimeBounds.height() , mTimeTextPaint);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (mShowScrubber) {
            int x = (int) event.getX();
            int y = (int) event.getY();

            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN: {
                    mScrubberCorrection = inScrubber(x, y)
                            ? x - mScrubberLeft
                            : mScrubber.getWidth() / 2;
                    mScrubbing = true;
                    mListener.onScrubbingStart();
                }
                // fall-through
                case MotionEvent.ACTION_MOVE: {
                    mScrubberLeft = x - mScrubberCorrection;
                    clampScrubber();
                    mCurrentTime = getScrubberTime();
                    mListener.onScrubbingMove(mCurrentTime);
                    update();
                    invalidate();
                    return true;
                }
                case MotionEvent.ACTION_CANCEL:
                case MotionEvent.ACTION_UP: {
                    mListener.onScrubbingEnd(getScrubberTime(), 0, 0);
                    mScrubbing = false;
                    return true;
                }
            }
        }
        return false;
    }

    
    public void setSeekable(boolean canSeek) {
        mShowScrubber = canSeek;
    }
    protected String stringForTime(long millis) {
        int totalSeconds = (int) millis / 1000;
        int seconds = totalSeconds % 60;
        int minutes = (totalSeconds / 60) % 60;
        int hours = totalSeconds / 3600;

        return String.format("%02d:%02d:%02d", hours, minutes, seconds).toString();
      
    }

}
