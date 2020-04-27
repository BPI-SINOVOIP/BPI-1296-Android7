package com.android.launcher2;

import android.content.ComponentName;
import android.content.pm.ActivityInfo;

/**
 * PendingAddShortcutInfo is defined by original package,<br>
 * but in Realtek Launcher, we make it to be a public class.<br>
 * So other class can access it.
 * 
 * @author bruce_huang
 *
 */
public class PendingAddShortcutInfo extends PendingAddItemInfo {

    ActivityInfo shortcutActivityInfo;
    
    public PendingAddShortcutInfo(ActivityInfo activityInfo) {
        shortcutActivityInfo = activityInfo;
    }

    @Override
    public String toString() {
        return "RTK Shortcut: " + shortcutActivityInfo.packageName;
    }
    
    public ComponentName getComponentName(){
    	return componentName;
    }
}
