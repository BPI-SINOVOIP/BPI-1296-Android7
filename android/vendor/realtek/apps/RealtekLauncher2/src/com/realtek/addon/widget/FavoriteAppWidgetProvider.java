package com.realtek.addon.widget;

import android.content.Context;
import android.content.Intent;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.launcher.R;

public class FavoriteAppWidgetProvider extends AbsRTKWidgetProvider {

	@Override
	public Class<?> getMyClass() {
		return FavoriteAppWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		return R.layout.wl_favorite_app;
	}

	@Override
	public void onHandleClickIntent(Context c) {
		c.sendBroadcast(new Intent(ConstantsDef.LAUNCH_FAVAPP_CUSTOMIZED_PAGE_INTENT));
	}

}
