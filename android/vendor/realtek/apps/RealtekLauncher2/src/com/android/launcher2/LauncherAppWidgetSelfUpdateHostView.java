package com.android.launcher2;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RemoteViews;

import com.realtek.addon.helper.DebugHelper;

/**
 * Descriptions:<br>
 *      A special class for fast update widget added by Realtek.
 *      A self-updated widget host view controls it's own display, and update automatically.<br>
 *      It doesn't aware update event from AppWidgetProvider, and ... corresponding Widget should not file any update request
 *      
 * @author bruce_huang
 * 
 * TODO:<br>
 * 1. complete mouse click handling.
 * 2. complete widget update handling.
 */
public class LauncherAppWidgetSelfUpdateHostView extends LauncherAppWidgetHostView {
    
    public View mView=null;
    
    public LauncherAppWidgetSelfUpdateHostView(Context context, Launcher launcher) {
        super(context, launcher);
        DebugHelper.dump2("Create self-update widget host view");
    }
    
    public void inflateRootView(int layoutID,String descriptions){
        if(mView==null){
            LayoutInflater mInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            mView = mInflater.inflate(layoutID, this, false);
            mView.setContentDescription(descriptions);
            addView(mView);
        }
    }
    
    @Override
    public void updateAppWidget(RemoteViews remoteViews) {
        DebugHelper.dump("[FASTUPDATE Widget] Do nothing in updateAppWidget.");
    }
    
    @Override
    public boolean orientationChangedSincedInflation() {
        // In self-updated widget, we do not consider orientation changed case,
        // so Launcher does not have to re-inflate widget.
        return false;
    }
}
