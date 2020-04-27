package com.realtek.addon.widget;

import android.content.Context;
import android.content.Intent;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.launcher.R;

public class VideoAppWidgetProvider extends AbsRTKWidgetProvider {

	@Override
	public Class<?> getMyClass() {
		return VideoAppWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		return R.layout.wl_video_app;
	}

	@Override
	public void onHandleClickIntent(Context c) {
		c.sendBroadcast(new Intent(ConstantsDef.LAUNCH_VIDEOAPP_CUSTOMIZED_PAGE_INTENT));
	}

}
