package com.realtek.addon.widget;

import android.content.Context;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.launcher.R;

public class GoogleMusicWidgetProvider extends AbsRTKWidgetProvider {

	@Override
	public Class<?> getMyClass() {
		// TODO Auto-generated method stub
		return GoogleMusicWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		// TODO Auto-generated method stub
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		// TODO Auto-generated method stub
		return R.layout.wl_google_music;
	}

	@Override
	public void onHandleClickIntent(Context c) {
	    launchInstalledApkInNewTask(ConstantsDef.ANDROID_MUSIC_APP_PKG_NAME, c);
	}

	/*
	@Override
	public void updateWidgetViews(Context context,
            AppWidgetManager appWidgetManager,
            int[] appWidgetIds){
        
	    // For testing now.
        for(int id:appWidgetIds){
            DebugHelper.dump("onUpdate provider:"+getClassName()+" id:"+id);
            
            RemoteViews views=new RemoteViews(context.getPackageName(), getLayoutID());
            views.setImageViewResource(R.id.icon, R.drawable.wi_google_music);
            registerOnClickEvent(context, views, id, getOnClickIntent(context,id));
            appWidgetManager.updateAppWidget(id, views);
        }
        
    }
    */
}
