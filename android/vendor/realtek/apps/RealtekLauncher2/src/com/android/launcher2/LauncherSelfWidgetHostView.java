package com.android.launcher2;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.RemoteViews;

import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

/**
 * A simple, generic widget host view to display realtek type color wall widget
 * @author bruce_huang
 *
 */
@SuppressLint("ViewConstructor") 
public class LauncherSelfWidgetHostView extends LauncherAppWidgetHostView {

    private String mProviderName;
    private View mView;
    private ImageView mIconView;
    int mBackgroundColor;
    int mIconID;
    
    public LauncherSelfWidgetHostView(Context context, Launcher launcher, String providerName, int backgroundColor, int iconID) {
        super(context, launcher);
        mProviderName=providerName;
        mBackgroundColor=backgroundColor;
        mIconID=iconID;
        
        LayoutInflater inflator = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mView = inflator.inflate(R.layout.wl_template, this, false);
        // set background color
        mView.setBackgroundColor(backgroundColor);
        // set icon
        mIconView = (ImageView)mView.findViewById(R.id.wl_template_image);
        Drawable iconDrawable = context.getResources().getDrawable(iconID);
        mIconView.setImageDrawable(iconDrawable);
        // set onTouchListener
        mView.setOnTouchListener(new OnTouchListener() {
            
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                Log.i("RTKWidgetHost", "OnTouchEvent:"+mProviderName);
                return false;
            }
        });
        this.addView(mView);
    }

    @Override
    public void updateAppWidget(RemoteViews remoteViews) {
        DebugHelper.dump("Do nothing in updateAppWidget.");
    }

    @Override
    public boolean orientationChangedSincedInflation() {
        // In self-updated widget, we do not consider orientation changed case,
        // so Launcher does not have to re-inflate widget.
        return false;
    }
}
