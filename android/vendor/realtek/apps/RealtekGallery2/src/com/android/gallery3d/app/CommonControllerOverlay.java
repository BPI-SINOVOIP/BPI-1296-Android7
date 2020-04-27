/*
 * Copyright (C) 2012 The Android Open Source Project
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
import android.content.res.Resources;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnHoverListener;
import android.widget.RelativeLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.view.LayoutInflater;
import com.android.gallery3d.R;
import android.widget.ImageButton;
import android.widget.Toast;
import android.os.SystemProperties;

/**
 * The common playback controller for the Movie Player or Video Trimming.
 */
public abstract class CommonControllerOverlay extends RelativeLayout implements
        ControllerOverlay,
        OnClickListener,
        TimeBar.Listener {

    protected enum State {
        PLAYING,
        PAUSED,
        ENDED,
        ERROR,
        LOADING
    }

    private static final float ERROR_MESSAGE_RELATIVE_PADDING = 1.0f / 6;

    protected Listener mListener; //Will be set as the MoviePlayer instance

    //protected final View mBackground;
    protected TimeBar mTimeBar;
    protected final RelativeLayout mController;
    protected final View mControlPanel;
    protected ImageButton mPlayPauseButton;
    protected ImageButton mStopButton;
    protected ImageButton mInsReplayButton;
    protected ImageButton mCmSkipButton;
    protected ImageButton mFrButton;
    protected ImageButton mFfButton;
    protected ImageButton mPreviousButton;
    protected ImageButton mNextButton;
    protected ImageButton mRepeatModeButton;
    protected ImageButton mOptionButton;
        

    protected View mMainView;
    protected final LinearLayout mLoadingView;
    protected final TextView mErrorView;
    protected final ImageView mPlayPauseReplayView;

    protected State mState;

    protected boolean mCanReplay = true;
    protected boolean mBroadcastStatus = false;
    protected int mRepeatMode = 0;

	private LinearLayout.LayoutParams mControlButtonsLL=null;

    public void setSeekable(boolean canSeek) {
        mTimeBar.setSeekable(canSeek);
    }

    public CommonControllerOverlay(Context context) {
        super(context);

        mState = State.LOADING;
        // TODO: Move the following layout code into xml file.
        LayoutParams wrapContent =
                new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		wrapContent.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);
        
        LayoutParams ppl =
                new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        // let all children always be center in parent.
		ppl.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);
				
        LayoutParams matchParent =
                new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);

        //mBackground = new View(context);
        //mBackground.setBackgroundColor(context.getResources().getColor(R.color.darker_transparent));
        //addView(mBackground, matchParent);

        // Depending on the usage, the timeBar can show a single scrubber, or
        // multiple ones for trimming.
        LayoutInflater inflate = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mController = (RelativeLayout)inflate.inflate(R.layout.movie_controller, null);

        int navigation_bar_height=0;
		Resources resources = context.getResources();
        int resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android");
        boolean mHasNavigationBar = resources.getBoolean(com.android.internal.R.bool.config_showNavigationBar);
        if (resourceId > 0 && mHasNavigationBar==true) {
            navigation_bar_height = resources.getDimensionPixelSize(resourceId);
            if(navigation_bar_height<=1)
                navigation_bar_height=0;
        }
		
        // add bottom padding
        LinearLayout linear = (LinearLayout) mController.findViewById(R.id.linear);
		linear.setPadding(0,0,0,navigation_bar_height);
		
		// -- Time Bar
        mTimeBar = (TimeBar)mController.findViewById(R.id.controller_timebar);
        mTimeBar.setListener(this);
		mTimeBar.setContentDescription(
                context.getResources().getString(R.string.accessibility_time_bar));

		// -- Control Panel
		mControlPanel = mController.findViewById(R.id.controller_panel);
		mControlButtonsLL = (LinearLayout.LayoutParams) mControlPanel.getLayoutParams();
        //mControlPanel.setVisibility(View.GONE);
		
        initButtons();
       
        addView(mController);
        // -- Loading animation + TextView
        mLoadingView = new LinearLayout(context);
        mLoadingView.setOrientation(LinearLayout.VERTICAL);
        mLoadingView.setGravity(Gravity.CENTER_HORIZONTAL);
        ProgressBar spinner = new ProgressBar(context);
        spinner.setIndeterminate(true);
        mLoadingView.addView(spinner, wrapContent);
        TextView loadingText = createOverlayTextView(context);
        loadingText.setText(R.string.loading_video);
        mLoadingView.addView(loadingText, wrapContent);
        addView(mLoadingView, wrapContent);
		
        // -- Play/Pause toggle button
        mPlayPauseReplayView = new ImageView(context);
        mPlayPauseReplayView.setImageResource(R.drawable.ic_vidcontrol_play);
        mPlayPauseReplayView.setContentDescription(
                context.getResources().getString(R.string.accessibility_play_video));
        mPlayPauseReplayView.setBackgroundResource(R.drawable.bg_vidcontrol);
        mPlayPauseReplayView.setScaleType(ScaleType.CENTER);
        mPlayPauseReplayView.setFocusable(true);
        mPlayPauseReplayView.setClickable(true);
        mPlayPauseReplayView.setOnClickListener(this);

        //RelativeLayout.LayoutParams ppl = (RelativeLayout.LayoutParams) mPlayPauseReplayView.getLayoutParams();
		//ppl.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);
		//mPlayPauseReplayView.setLayoutParams(ppl);

        addView(mPlayPauseReplayView, ppl);
		
        // -- Error Text View
        mErrorView = createOverlayTextView(context);
        addView(mErrorView, new LayoutParams(0,0));

        RelativeLayout.LayoutParams params =
                new RelativeLayout.LayoutParams(
                        LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        setLayoutParams(params);
        hide();
    }

    abstract protected void createTimeBar(Context context);

    private void initButtons(){
        
        mPlayPauseButton = (ImageButton)mController.findViewById(R.id.play_pause_button);
        mPlayPauseButton.setImageResource(R.drawable.cursor_bar_pause);
        mPlayPauseButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true){
                       mPlayPauseButton.setImageResource(mState == State.PLAYING ? R.drawable.cursor_bar_pause_focus:
                           R.drawable.cursor_bar_play_focus);
                   }
                   else{
                       mPlayPauseButton.setImageResource(mState == State.PLAYING ? R.drawable.cursor_bar_pause:
                           R.drawable.cursor_bar_play);
                   }
               }
           }
        );
        mPlayPauseButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus) {
                        mPlayPauseButton.setImageResource(mState == State.PLAYING ? R.drawable.cursor_bar_pause_focus:
                           R.drawable.cursor_bar_play_focus);
                    }
                    else {
                        mPlayPauseButton.setImageResource(mState == State.PLAYING ? R.drawable.cursor_bar_pause:
                           R.drawable.cursor_bar_play);
                    }

                    return true;
                }
            }
        );

        mStopButton = (ImageButton)mController.findViewById(R.id.stop_button);
        mStopButton.setImageResource(R.drawable.cursor_bar_stop);
        mStopButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true){
                       mStopButton.setImageResource(R.drawable.cursor_bar_stop_focus);
                   }
                   else{
                       mStopButton.setImageResource(R.drawable.cursor_bar_stop);
                   }
               }
           }
        );
        mStopButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus) {
                        mStopButton.setImageResource(R.drawable.cursor_bar_stop_focus);
                    }
                    else {
                        mStopButton.setImageResource(R.drawable.cursor_bar_stop);
                    }

                    return true;
                }
            }
        );

        mInsReplayButton = (ImageButton)mController.findViewById(R.id.ins_replay_button);
        mInsReplayButton.setImageResource(R.drawable.cursor_bar_ins_replay);
        mInsReplayButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mInsReplayButton.setImageResource(R.drawable.cursor_bar_ins_replay_focus);
                   }
                   else{
                       mInsReplayButton.setImageResource(R.drawable.cursor_bar_ins_replay);
                   }
               }
           }
        );
        mInsReplayButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mInsReplayButton.setImageResource(R.drawable.cursor_bar_ins_replay_focus);
                    }
                    else {
                        mInsReplayButton.setImageResource(R.drawable.cursor_bar_ins_replay);
                    }

                    return true;
                }
            }
        );
        
        mCmSkipButton = (ImageButton)mController.findViewById(R.id.cm_skip_button);
        mCmSkipButton.setImageResource(R.drawable.cursor_bar_cm_skip);
        mCmSkipButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mCmSkipButton.setImageResource(R.drawable.cursor_bar_cm_skip_focus);
                   }
                   else{
                       mCmSkipButton.setImageResource(R.drawable.cursor_bar_cm_skip);
                   }
               }
           }
        );
        mCmSkipButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mCmSkipButton.setImageResource(R.drawable.cursor_bar_cm_skip_focus);
                    }
                    else {
                        mCmSkipButton.setImageResource(R.drawable.cursor_bar_cm_skip);
                    }

                    return true;
                }
            }
        );
        
        mFrButton = (ImageButton)mController.findViewById(R.id.fr_button);
        mFrButton.setImageResource(R.drawable.cursor_bar_fr);
        mFrButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mFrButton.setImageResource(R.drawable.cursor_bar_fr_focus);
                   }
                   else{
                       mFrButton.setImageResource(R.drawable.cursor_bar_fr);
                   }
               }
           }
        );
        mFrButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mFrButton.setImageResource(R.drawable.cursor_bar_fr_focus);
                    }
                    else {
                        mFrButton.setImageResource(R.drawable.cursor_bar_fr);
                    }

                    return true;
                }
            }
        );
        
        mFfButton = (ImageButton)mController.findViewById(R.id.ff_button);
        mFfButton.setImageResource(R.drawable.cursor_bar_ff);
        mFfButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mFrButton.setImageResource(R.drawable.cursor_bar_ff_focus);
                   }
                   else{
                       mFrButton.setImageResource(R.drawable.cursor_bar_ff);
                   }
               }
           }
        );
        mFfButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mFfButton.setImageResource(R.drawable.cursor_bar_ff_focus);
                    }
                    else {
                        mFfButton.setImageResource(R.drawable.cursor_bar_ff);
                    }

                    return true;
                }
            }
        );
        
        mPreviousButton = (ImageButton)mController.findViewById(R.id.previous_button);
        mPreviousButton.setImageResource(R.drawable.cursor_bar_previous);
        mPreviousButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mPreviousButton.setImageResource(R.drawable.cursor_bar_previous_focus);
                   }
                   else{
                       mPreviousButton.setImageResource(R.drawable.cursor_bar_previous);
                   }
               }
           }
        );
        mPreviousButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mPreviousButton.setImageResource(R.drawable.cursor_bar_previous_focus);
                    }
                    else {
                        mPreviousButton.setImageResource(R.drawable.cursor_bar_previous);
                    }

                    return true;
                }
            }
        );
        
        mNextButton = (ImageButton)mController.findViewById(R.id.next_button);
        mNextButton.setImageResource(R.drawable.cursor_bar_next);
        mNextButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mNextButton.setImageResource(R.drawable.cursor_bar_next_focus);
                   }
                   else{
                       mNextButton.setImageResource(R.drawable.cursor_bar_next);
                   }
               }
           }
        );
        mNextButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mNextButton.setImageResource(R.drawable.cursor_bar_next_focus);
                    }
                    else {
                        mNextButton.setImageResource(R.drawable.cursor_bar_next);
                    }

                    return true;
                }
            }
        );
        
        mRepeatModeButton = (ImageButton)mController.findViewById(R.id.repeat_mode_button);
        mRepeatModeButton.setImageResource(R.drawable.cursor_bar_no_repeat);
        mRepeatModeButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)
                        bHasFocus = false;
                    else
                        return true;
                    setRepeatIcon(bHasFocus);
                    return true;
                }
            }
        );
        
        mOptionButton = (ImageButton)mController.findViewById(R.id.option_button);
        mOptionButton.setImageResource(R.drawable.cursor_bar_option);
        mOptionButton.setOnFocusChangeListener(
           new OnFocusChangeListener(){
               public void onFocusChange(View arg0, boolean isFocused){              
                   if (isFocused==true && mBroadcastStatus==false){
                       mOptionButton.setImageResource(R.drawable.cursor_bar_option_focus);
                   }
                   else{
                       mOptionButton.setImageResource(R.drawable.cursor_bar_option);
                   }
               }
           }
        );
        mOptionButton.setOnHoverListener(
            new OnHoverListener(){
                public boolean onHover(View v, MotionEvent event){  
                    boolean bHasFocus = true;
                    if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_ENTER)
                        bHasFocus = true;
                    else if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_HOVER_EXIT)   
                        bHasFocus = false;
                    else
                        return true;

                    if (bHasFocus && mBroadcastStatus==false) {
                        mOptionButton.setImageResource(R.drawable.cursor_bar_option_focus);
                    }
                    else {
                        mOptionButton.setImageResource(R.drawable.cursor_bar_option);
                    }

                    return true;
                }
            }
        );
    }

    private TextView createOverlayTextView(Context context) {
        TextView view = new TextView(context);
        view.setGravity(Gravity.CENTER);
        view.setTextColor(0xFFFFFFFF);
        view.setPadding(0, 15, 0, 15);
        android.util.DisplayMetrics metrics = context.getResources().getDisplayMetrics();
        int densityDpi = (int)(metrics.density * 160f);
        if(densityDpi>=320) {
            view.setTextAppearance(context,android.R.style.TextAppearance_Medium);
        }
        else {
            // make text bigger for better display.
            view.setTextAppearance(context,android.R.style.TextAppearance_Large);
        }
        return view;
    }

    @Override
    public void setListener(Listener listener) {
        this.mListener = listener;
        mPlayPauseButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mPlayPauseButton) {
                    if (mState == State.PLAYING || mState == State.PAUSED)
                        mListener.onPlayPause(true, true);
                }
            }
        });
        
        mStopButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mStopButton) {
                        mListener.onStop();
                }
            }
        });

        mInsReplayButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mInsReplayButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onInsReplay();
                }
            }
        });

        mCmSkipButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mCmSkipButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onCmSkip();
                }
            }
        });

        mFrButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mFrButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onFastRewind();
                }
            }
        });

        mFfButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mFfButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onFastForward();
                }
            }
        });

        mNextButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mNextButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onNext();
                }
            }
        });

        mPreviousButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mPreviousButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onPrevious();
                }
            }
        });

        mRepeatModeButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mRepeatModeButton && mBroadcastStatus==false) {
                        show(true, true);
                        mListener.onRepeatModeChange();
                }
            }
        });

        mOptionButton.setOnClickListener (new View.OnClickListener() {
            public void onClick(View v) {
                if (v == mOptionButton && mBroadcastStatus==false) {
                        mListener.onShowOption();
                }
            }
        });
    }

    @Override
    public void setCanReplay(boolean canReplay) {
        this.mCanReplay = canReplay;
    }

    public void setBroadcastStatus(boolean broadcastStatus) {
        this.mBroadcastStatus = broadcastStatus;
    }

    @Override
    public void setRepeatMode(int repeatMode, boolean bHasFocus)
    {
        mRepeatMode = repeatMode;
        setRepeatIcon(bHasFocus);
    }

    private void setRepeatIcon(boolean bHasFocus)
    {
        switch(mRepeatMode)
        {
            case 0:
                if(bHasFocus && mBroadcastStatus==false)
                    mRepeatModeButton.setImageResource(R.drawable.cursor_bar_no_repeat_focus);
                else
                    mRepeatModeButton.setImageResource(R.drawable.cursor_bar_no_repeat);
                break;
            case 1:
                if(bHasFocus && mBroadcastStatus==false)
                    mRepeatModeButton.setImageResource(R.drawable.cursor_bar_repeat_one_focus);
                else
                    mRepeatModeButton.setImageResource(R.drawable.cursor_bar_repeat_one);
                break;
            case 2:
                if(bHasFocus && mBroadcastStatus==false)
                    mRepeatModeButton.setImageResource(R.drawable.cursor_bar_repeat_all_focus);
                else
                    mRepeatModeButton.setImageResource(R.drawable.cursor_bar_repeat_all);
                break;
            default:
                break;
        }
    }

    @Override
    public View getView() {
        return this;
    }

    @Override
    public void showPlaying(boolean bShowController, boolean bShowControlButtons) {
        if (mState == State.PAUSED)
            mPlayPauseButton.setImageResource(R.drawable.cursor_bar_pause_focus);
        else
            mPlayPauseButton.setImageResource(R.drawable.cursor_bar_pause);
        mState = State.PLAYING;
        showMainView(mPlayPauseReplayView, false);
        show(bShowController, bShowControlButtons);
    }

    @Override
    public void showPaused(boolean bShowController, boolean bShowControlButtons) {
        mState = State.PAUSED;
        mPlayPauseButton.setImageResource(R.drawable.cursor_bar_play_focus);
        showMainView(mPlayPauseReplayView, false);
        show(bShowController, bShowControlButtons);
    }

    @Override
    public void showEnded() {
        mState = State.ENDED;
        showMainView(mPlayPauseReplayView, false);
    }

    @Override
    public void showLoading() {
        mState = State.LOADING;
        showMainView(mLoadingView, false);
    }

    @Override
    public void showErrorMessage(String message) {
        mState = State.ERROR;
        int padding = (int) (getMeasuredWidth() * ERROR_MESSAGE_RELATIVE_PADDING);
        mErrorView.setPadding(
                padding, mErrorView.getPaddingTop(), padding, mErrorView.getPaddingBottom());
        mErrorView.setText(message);
        showMainView(mErrorView, false);
    }

    @Override
    public void setTimes(int currentTime, int totalTime,
            int trimStartTime, int trimEndTime) {
        mTimeBar.setTime(currentTime, totalTime, trimStartTime, trimEndTime);
    }

    public void hide() {
        mPlayPauseReplayView.setVisibility(View.INVISIBLE);
        mLoadingView.setVisibility(View.INVISIBLE);
        //mBackground.setVisibility(View.INVISIBLE);
        mController.setVisibility(View.INVISIBLE);
        setVisibility(View.INVISIBLE);
        setFocusable(true);
        requestFocus();
    }

    private void showMainView(View view, boolean bShowController) {
        mMainView = view;
        mErrorView.setVisibility(mMainView == mErrorView ? View.VISIBLE : View.INVISIBLE);
        mLoadingView.setVisibility(mMainView == mLoadingView ? View.VISIBLE : View.INVISIBLE);
        mPlayPauseReplayView.setVisibility(
                mMainView == mPlayPauseReplayView ? View.VISIBLE : View.INVISIBLE);
        show(bShowController,bShowController);
    }

    @Override
    public void show(boolean bShowController, boolean bShowControlButtons) {
        SystemProperties.set("rtk.hwc.osd.hide", Long.toString(0));
        updateViews(bShowController,bShowControlButtons);
        setVisibility(View.VISIBLE);
        setFocusable(false);
    }

    @Override
    public void onClick(View view) {
        if (mListener != null) {
            if (view == mPlayPauseReplayView) {
                if (mState == State.ENDED) {
                    if (mCanReplay) {
                        mListener.onReplay();
                    }
                } else if (mState == State.PAUSED || mState == State.PLAYING) {
                    mListener.onPlayPause(true, true);
                }
            }
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (super.onTouchEvent(event)) {
            return true;
        }
        return false;
    }

    // The paddings of 4 sides which covered by system components. E.g.
    // +-----------------+\
    // | Action Bar | insets.top
    // +-----------------+/
    // | |
    // | Content Area | insets.right = insets.left = 0
    // | |
    // +-----------------+\
    // | Navigation Bar | insets.bottom
    // +-----------------+/
    // Please see View.fitSystemWindows() for more details.
    private final Rect mWindowInsets = new Rect();

    @Override
    protected boolean fitSystemWindows(Rect insets) {
        // We don't set the paddings of this View, otherwise,
        // the content will get cropped outside window
        mWindowInsets.set(insets);
        return true;
    }

    private void relayoutController(int LayoutWidth, int LayoutHeight){
        Rect insets = mWindowInsets;
        int pl = insets.left; // the left paddings
        int pr = insets.right;
        int pt = insets.top;
        int pb = insets.bottom;

        boolean error = mErrorView.getVisibility() == View.VISIBLE;

        int y = LayoutHeight - pb;
        // Put both TimeBar and Background just above the bottom system
        // component.
        // But extend the background to the width of the screen, since we don't
        // care if it will be covered by a system component and it looks better.
        //mBackground.layout(0, y - mTimeBar.getBarHeight(), w, y);
       // mTimeBar.layout(pl, y - mTimeBar.getPreferredHeight(), w - pr, y);
        if(mController!=null){
            mController.layout(0, y - mTimeBar.getPreferredHeight() - mControlPanel.getHeight(),  LayoutWidth, y);

            // Needed, otherwise the framework will not re-layout in case only the
            // padding is changed
//            mController.requestLayout();
        }

        // Put the play/pause/next/ previous button in the center of the screen
        layoutCenteredView(mPlayPauseReplayView, 0, 0, LayoutWidth, LayoutHeight);

        if (mMainView != null) {
            layoutCenteredView(mMainView, 0, 0, LayoutWidth, LayoutHeight);
        }
    }

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            Bundle bundle = msg.getData();
            int LayoutWidth = bundle.getInt("width");
            int LayoutHeight = bundle.getInt("height");
            relayoutController(LayoutWidth, LayoutHeight);
        }
    };

    /*
         Bruce remove this block, do not re-layout child views
	@Override
        protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        
	        int LayoutHeight = bottom - top;
	        int LayoutWidth = right - left;
	        Message msg = new Message();
	        Bundle data = new Bundle();
	        data.putInt("width", LayoutWidth);
	        data.putInt("height", LayoutHeight);
	        msg.setData(data);
	        handler.sendMessage(msg);
        }
    */
    private void layoutCenteredView(View view, int l, int t, int r, int b) {
        int cw = view.getMeasuredWidth();
        int ch = view.getMeasuredHeight();
        int cl = (r - l - cw) / 2;
        int ct = (b - t - ch) / 2;
        view.layout(cl, ct, cl + cw, ct + ch);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        measureChildren(widthMeasureSpec, heightMeasureSpec);
    }

    protected void updateViews(boolean bShowController, boolean bShowControlButton) {
        //mBackground.setVisibility(View.VISIBLE);

		if(bShowControlButton)
			mControlPanel.setLayoutParams(mControlButtonsLL);
		else
			mControlPanel.setLayoutParams(new LinearLayout.LayoutParams(0,0));

        if (bShowController)
            mController.setVisibility(View.VISIBLE);
        else
            mController.setVisibility(View.INVISIBLE);
        Resources resources = getContext().getResources();
        int imageResource = R.drawable.ic_vidcontrol_reload;
        String contentDescription = resources.getString(R.string.accessibility_reload_video);
        if (mState == State.PAUSED) {
            imageResource = R.drawable.ic_vidcontrol_play;
            contentDescription = resources.getString(R.string.accessibility_play_video);
        } else if (mState == State.PLAYING) {
            imageResource = R.drawable.ic_vidcontrol_pause;
            contentDescription = resources.getString(R.string.accessibility_pause_video);
        }

        mPlayPauseReplayView.setImageResource(imageResource);
        mPlayPauseReplayView.setContentDescription(contentDescription);
        mPlayPauseReplayView.setVisibility(
                (mState != State.LOADING && mState != State.ERROR &&
                !(mState == State.ENDED && !mCanReplay))
                ? View.VISIBLE : View.GONE);
        requestLayout();
    }

    // TimeBar listener

    @Override
    public void onScrubbingStart() {
        mListener.onSeekStart();
    }

    @Override
    public void onScrubbingMove(int time) {
        mListener.onSeekMove(time);
    }

    @Override
    public void onScrubbingEnd(int time, int trimStartTime, int trimEndTime) {
        mListener.onSeekEnd(time, trimStartTime, trimEndTime);
    }
}
