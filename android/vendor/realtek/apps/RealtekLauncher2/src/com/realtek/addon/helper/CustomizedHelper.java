package com.realtek.addon.helper;

import com.realtek.addon.widget.AppDrawerWidgetProvider;
import com.realtek.addon.widget.DtvWidgetProvider;
import com.realtek.addon.widget.StorageWidgetProvider;
import com.realtek.launcher.R;

import android.appwidget.AppWidgetProviderInfo;
import android.content.ComponentName;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;

public class CustomizedHelper {
	
	//public static final int RTK_WIDGET_PAD=3;
	public static final boolean ENABLE_CLING=false;
	public static final boolean DISABLE_CREATE_FOLDER=true;
	public static final boolean DISABLE_OVERSCROLL_ROTATION_EFFECT=true;
	
	public static final boolean DRAW_DRAG_VIEW_SHADOW=true;
	
	public static ComponentName getFakeSearchActivity(){
		return null;
	}
	
	public static int getPaddingFromResource(Context c){
		final Resources res = c.getResources();
		int dimen=res.getDimensionPixelSize(R.dimen.rtk_workspace_cell_inner_padding);
		DebugHelper.dump("rtk_workspace_dimen value:"+dimen);
		return dimen;
	}
	
	public static boolean isHiddenWidget(AppWidgetProviderInfo pInfo){
		String providerClassName = pInfo.provider.getClassName();
		DebugHelper.dump("[RTKCUSTOM] widget provider class name="+providerClassName);
		
		if(providerClassName.equals(AppDrawerWidgetProvider.class.getName()))
			return true;
		else if(providerClassName.equals(DtvWidgetProvider.class.getName()))
			return true;
		else if(providerClassName.equals(StorageWidgetProvider.class.getName()))
		    return true;
		else
			return false;
	}
	
	/**
	 * Move focus color be decided here, easier to maintain.
	 * @param c
	 * @return focus color used in home menu
	 */
	public static int getShortcutFocusColor(Context c){
		//final Resources res = c.getResources();
		int rst = Color.WHITE;//res.getColor(android.R.color.holo_blue_dark);
		// apply alpha, just for testing.
		rst=rst&0xaaffffff;
		return rst;
		//return Color.WHITE;
		
	}

}
