package com.realtek.addon.widget;

import android.content.Context;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.launcher.R;


public class WidiWidgetProvider extends AbsRTKWidgetProvider{

	@Override
	public Class<?> getMyClass() {
		return WidiWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		return R.layout.wl_widi;
	}

	@Override
	public void onHandleClickIntent(Context c) {
	    launchInstalledApkInNewTask(ConstantsDef.RTK_MIRACAST_APP_PKG_NAME, c);
	}

}
