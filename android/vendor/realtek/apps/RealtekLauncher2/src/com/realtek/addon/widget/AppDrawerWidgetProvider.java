package com.realtek.addon.widget;

import android.content.Context;
import android.content.Intent;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.launcher.R;

public class AppDrawerWidgetProvider extends AbsRTKWidgetProvider {

	@Override
	public Class<?> getMyClass() {
		return AppDrawerWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		return R.id.widget_root_view_group;
	}

	@Override
	public void onHandleClickIntent(Context c) {
		// RTKTODO: launch app view
		// DebugHelper.dump("onHandleClickIntent "+getClassName());
		c.sendBroadcast(new Intent(ConstantsDef.LAUNCH_APP_CUSTOMIZED_PAGE_INTENT));
	}

	@Override
	public int getLayoutID() {
		return R.layout.wl_app_drawer;
	}

}
