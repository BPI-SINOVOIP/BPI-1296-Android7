/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.launcher2;

import android.appwidget.AppWidgetHost;
import android.appwidget.AppWidgetHostView;
import android.appwidget.AppWidgetProviderInfo;
import android.content.Context;

import com.realtek.addon.helper.DebugHelper;
import com.realtek.addon.widget.AppDrawerWidgetProvider;
import com.realtek.addon.widget.DtvWidgetProvider;
import com.realtek.addon.widget.FavoriteAppWidgetProvider;
import com.realtek.addon.widget.FilebrowserWidgetProvider;
import com.realtek.addon.widget.GoogleMusicWidgetProvider;
import com.realtek.addon.widget.NetworkTrafficWidgetProvider;
import com.realtek.addon.widget.SettingWidgetProvider;
import com.realtek.addon.widget.StorageWidgetProvider;
import com.realtek.addon.widget.VideoAppWidgetProvider;
import com.realtek.addon.widget.WidiWidgetProvider;
import com.realtek.addon.widget.WifiStatusWidgetProvider;
import com.realtek.launcher.R;

/**
 * Specific {@link AppWidgetHost} that creates our {@link LauncherAppWidgetHostView}
 * which correctly captures all long-press events. This ensures that users can
 * always pick up and move widgets.
 */
public class LauncherAppWidgetHost extends AppWidgetHost {

    Launcher mLauncher;

    public LauncherAppWidgetHost(Launcher launcher, int hostId) {
        super(launcher, hostId);
        mLauncher = launcher;
    }

    @Override
    protected AppWidgetHostView onCreateView(Context context, int appWidgetId,
            AppWidgetProviderInfo appWidget) {
    	// RTKTODO: check AppWidgetProviderInfo and see if it is a DTV widget.
    	String providerClassName=appWidget.provider.getClassName();
    	DebugHelper.dump("[AppWidgetHost] providerClassName:"+providerClassName);
    	
    	if(providerClassName.equals(DtvWidgetProvider.class.getName())){
    		// create special Dtv widget
    		return new LauncherDtvWidgetHostView(context, mLauncher);
    	}else{
    	    // For fast, self-updated widget
    	    if(providerClassName.equals(WifiStatusWidgetProvider.class.getName())){
    	        return new LauncherWifiStatusWidgetHostView(context, mLauncher);
    	    }
    	    
    	    if(providerClassName.equals(NetworkTrafficWidgetProvider.class.getName())){
    	        return new LauncherNetTrafficWidgetHostView(context,mLauncher);
    	    }
    	    
    	    if(providerClassName.equals(StorageWidgetProvider.class.getName())){
                return new LauncherStorageWidgetHostView(context, mLauncher);
            }
    	    
    	    // For Realtek self static widgets
    	    // 1. app
    	    if(providerClassName.equals(AppDrawerWidgetProvider.class.getName())){
    	        return new LauncherSelfWidgetHostView(context, 
    	                mLauncher,
    	                providerClassName,
    	                context.getResources().getColor(R.color.w_app_color),
    	                R.drawable.wi_apps);
    	    }
    	    // 2. video
            if(providerClassName.equals(VideoAppWidgetProvider.class.getName())){
                return new LauncherSelfWidgetHostView(context, 
                        mLauncher,
                        providerClassName,
                        context.getResources().getColor(R.color.w_video_color),
                        R.drawable.wi_video);
            }
            
            // 3. favorite
            if(providerClassName.equals(FavoriteAppWidgetProvider.class.getName())){
                return new LauncherSelfWidgetHostView(context, 
                        mLauncher,
                        providerClassName,
                        context.getResources().getColor(R.color.w_favorite_color),
                        R.drawable.wi_favorite);
            }
            // 4. google music
            if(providerClassName.equals(GoogleMusicWidgetProvider.class.getName())){
                return new LauncherSelfWidgetHostView(context, 
                        mLauncher,
                        providerClassName,
                        context.getResources().getColor(R.color.w_google_music_color),
                        R.drawable.wi_google_music);
            }
            // 5. file browser
            if(providerClassName.equals(FilebrowserWidgetProvider.class.getName())){
                return new LauncherSelfWidgetHostView(context, 
                        mLauncher,
                        providerClassName,
                        context.getResources().getColor(R.color.w_filebrowser_color),
                        R.drawable.wi_filebrowser);
            }
            // 6. setting
            if(providerClassName.equals(SettingWidgetProvider.class.getName())){
                return new LauncherSelfWidgetHostView(context, 
                        mLauncher,
                        providerClassName,
                        context.getResources().getColor(R.color.w_setting_color),
                        R.drawable.wi_setting);
            }
            // 7. widi
            if(providerClassName.equals(WidiWidgetProvider.class.getName())){
                return new LauncherSelfWidgetHostView(context, 
                        mLauncher,
                        providerClassName,
                        context.getResources().getColor(R.color.w_miracast_color),
                        R.drawable.wi_widi);
            }
            // else, 3rd party widgets
    		return new LauncherAppWidgetHostView(context,mLauncher);
    	}
    }

    @Override
    public void stopListening() {
        super.stopListening();
        clearViews();
    }

    protected void onProvidersChanged() {
        // Once we get the message that widget packages are updated, we need to rebind items
        // in AppsCustomize accordingly.
        mLauncher.bindPackagesUpdated();
    }
}
