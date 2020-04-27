package com.realtek.addon.widget;

import android.content.Context;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.launcher.R;


public class FilebrowserWidgetProvider extends AbsRTKWidgetProvider {

	@Override
	public Class<?> getMyClass() {
		return FilebrowserWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		return R.layout.wl_filebrowser;
	}

	@Override
	public void onHandleClickIntent(Context c) {
		launchInstalledApkInNewTask(ConstantsDef.MEDIA_BROWSER_PKG_NAME, c);
	}

}
