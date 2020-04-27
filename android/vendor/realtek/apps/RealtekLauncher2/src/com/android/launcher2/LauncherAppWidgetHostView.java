/*
 * Copyright (C) 2009 The Android Open Source Project
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

package com.android.launcher2;

import android.appwidget.AppWidgetHostView;
import android.appwidget.AppWidgetProviderInfo;
import android.content.ComponentName;
import android.content.Context;
import android.graphics.Color;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RemoteViews;

import com.realtek.addon.helper.CustomizedHelper;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

/**
 * {@inheritDoc}
 */
public class LauncherAppWidgetHostView extends AppWidgetHostView {
    private CheckLongPressHelper mLongPressHelper;
    private LayoutInflater mInflater;
    protected Context mContext;
    private int mPreviousOrientation;
    // RTKCOMMENT: added for Realtek platform
    //public RemoteViews myRemoteViews=null;
    
    public String appWidgetLabel=null;
    public Launcher mLauncher=null;

    public LauncherAppWidgetHostView(Context context,Launcher launcher) {
        super(context);
        mContext = context;
        mLauncher=launcher;
        mLongPressHelper = new CheckLongPressHelper(this);
        mInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        // COMMENT: let widget be focusable, for testing
        setFocusable(true);
        setFocusableInTouchMode(true);
        //DebugHelper.dump("LauncherAppWidgetHostView hasFocusable:"+hasFocusable());
        appWidgetLabel=context.getResources().getString(R.string.w_app_drawer_label);
        setOnClickListener(mLauncher);
    }
    
	// RTKCOMMENT: Bruce hack to reduce gaps between widgets.
	public void setPadding(int left, int top, int right, int bottom) {
		int pad=CustomizedHelper.getPaddingFromResource(mContext);
		super.setPadding(pad, pad, pad, pad);
	}
	
	public boolean performClick() {
		boolean rst=super.performClick();
		DebugHelper.dump("LauncherAppWidgetHostView performClick....");
		return rst;
	}

    @Override
    protected View getErrorView() {
        return mInflater.inflate(R.layout.appwidget_error, this, false);
    }

    @Override
    public void updateAppWidget(RemoteViews remoteViews) {
        // Store the orientation in which the widget was inflated
        mPreviousOrientation = mContext.getResources().getConfiguration().orientation;
        super.updateAppWidget(remoteViews);
    }

    public boolean orientationChangedSincedInflation() {
        int orientation = mContext.getResources().getConfiguration().orientation;
        if (mPreviousOrientation != orientation) {
           return true;
       }
       return false;
    }
    
    
    public boolean onTouchEvent (MotionEvent event){
    	DebugHelper.dump("--------------> AppWidgetHostView onTouchEvent");
    	return super.onTouchEvent(event);
    }
    
    public boolean onInterceptTouchEvent(MotionEvent ev) {
    	
    	DebugHelper.dump("--------------> AppWidgetHostView onInterceptTouchEvent");
    	
        // Consume any touch events for ourselves after longpress is triggered
        if (mLongPressHelper.hasPerformedLongPress()) {
            mLongPressHelper.cancelLongPress();
            return true;
        }

        // Watch for longpress events at this level to make sure
        // users can always pick up this widget
        switch (ev.getAction()) {
            case MotionEvent.ACTION_DOWN: {
                mLongPressHelper.postCheckForLongPress();
                break;
            }

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL:
                mLongPressHelper.cancelLongPress();
                break;
        }

        // Otherwise continue letting touch events fall through to children
        return false;
    }

    @Override
    public void cancelLongPress() {
        super.cancelLongPress();

        mLongPressHelper.cancelLongPress();
    }

    @Override
    public int getDescendantFocusability() {
        return ViewGroup.FOCUS_BLOCK_DESCENDANTS;
    }
    
    /*
    @Override
    public boolean onHoverEvent (MotionEvent event){
    	// ignore all hover event
    	return true;
    }
    */
    
    public String getWidgetLabel(){
    	return getAppWidgetInfo().label;
    }
    
    /*
    private boolean isAppWidgetType(){
    	String label=getWidgetLabel();
    	return label.equals(appWidgetLabel);
    }
    */
    
    public boolean isInMyRange(int touchX, int touchY){
    	int[] location=new int[2];
    	getLocationOnScreen(location);
    	
    	int x = location[0];
    	int y = location[1];
    	
    	int w = getWidth();
    	int h = getHeight();
    	
    	boolean inX = touchX>=x && touchX<(x+w);
    	boolean inY = touchY>=y && touchY<(y+h);
    	
    	if(inX && inY)
    		return true;
    	else
    		return false;
    }
    
    
    protected void drawableStateChanged(){
    	DebugHelper.dump("----> LauncherAppWidgetHostView drawableStateChanged");
    	super.drawableStateChanged();
    }
    
    /*
    @Override
    protected void drawableStateChanged (){
    	super.drawableStateChanged();
    	
    	
    	if(isHovered()){
    		DebugHelper.dump("by pass hover event, break "+ this);
    		return;
    	}
    	
    	
    	//DebugHelper.dump("------------- AppWidgetHostView drawableStateChanged");
    	if(isFocused()||isPressed()){
    		//DebugHelper.dump("AppWidgetHostView gain focus:"+this+" focus:"+isFocused()+" pressed:"+isPressed()+" hover:"+isHovered());
    		setBackgroundColor(CustomizedHelper.getShortcutFocusColor(getContext()));
    		
    		if(isAppWidgetType()) {
    			int width=getWidth();
    			int height=getHeight();
    			int[] location=new int[2];
    			getLocationOnScreen(location);
    			DebugHelper.dump("x:"+location[0]+" y:"+location[1]+" w:"+width+" h:"+height);
    			
    			FocusView fv=new FocusView(mLauncher,this);
    			fv.show();
    		}
    		
    		
    	}else{
    		DebugHelper.dump("AppWidgetHostView no focus:"+this);
    		setBackgroundColor(Color.TRANSPARENT);
    	}
    	
    	postInvalidate();
    }
	*/
    
    /**
     * Added for Realtek Launcher.<br>
     * API for testing, check if LauncherAppWidgetHostView can get belong package info
     */
    public void dumpBelongApkInfo() {
    	String packageName = getAppWidgetInfo().provider.getPackageName();
    	DebugHelper.dump("LauncherAppWidgetHostView, packageName="+packageName);
    }
    
    @Override
    public String toString(){
    	AppWidgetProviderInfo pInfo = getAppWidgetInfo();
    	
    	return "LauncherAppWidgetHostView:"+pInfo.label;
    }
    
    @Override
    public boolean onKeyDown (int keyCode, KeyEvent event){
    	DebugHelper.dump("LauncherAppWidgetHostView onKeyDown");
    	return super.onKeyDown(keyCode, event);
    }
    
    public void clearBackground(){
    	DebugHelper.dump("LauncherAppWidgetHostView clear background");
    	setBackgroundColor(Color.TRANSPARENT);
    	postInvalidate();
    }
    
}
