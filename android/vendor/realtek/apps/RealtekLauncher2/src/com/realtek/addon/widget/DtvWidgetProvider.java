package com.realtek.addon.widget;

import java.io.IOException;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;

import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.android.launcher2.Launcher;
import com.android.launcher2.LauncherApplication;
import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

public class DtvWidgetProvider extends AppWidgetProvider {
    final String TAG                 = "DtvWidgetProvider";

    @Override
    public void onUpdate(Context context,
            AppWidgetManager appWidgetManager,
            int[] appWidgetIds) {
        
        DebugHelper.dump("[DTV widget provider] [onUpdate] check and run DTV manager only once.!");
    }

    @Override
    public void onEnabled (Context context){
        DebugHelper.dump("[DTV widget provider] [onEnable] check and run DTV manager only once!");
    }

	@Override
	public void onReceive (Context context, Intent intent) {
	    String action = intent.getAction();
        if (AppWidgetManager.ACTION_APPWIDGET_UPDATE.equals(action)) {
	        Log.d("DTV","\033[0;32;32m onReceive:"+intent+" and do nothing\033[m");
        }else{
            super.onReceive(context,intent);
        }
    }
}
