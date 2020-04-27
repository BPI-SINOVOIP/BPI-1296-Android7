package com.realtek.addon.widget;

import com.realtek.launcher.R;

import android.content.Context;

public class StorageWidgetProvider extends AbsRTKWidgetProvider {

    @Override
    public Class<?> getMyClass() {
        return StorageWidgetProvider.class;
    }

    @Override
    public int getViewGroupID() {
        return R.id.widget_root_view_group;
    }

    @Override
    public int getLayoutID() {
        return R.layout.wl_storage;
    }

    @Override
    public void onHandleClickIntent(Context c) {
    }

}
