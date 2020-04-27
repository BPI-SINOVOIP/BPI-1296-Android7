package com.realtek.app.provider;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Resources;
import android.graphics.drawable.Drawable;
import android.util.DisplayMetrics;

public class InstalledAppInfoProvider {

	/** Copied from Launcher2, get App which has CATEGORY_LAUNCHER intent **/
	public static List<ResolveInfo> findActivitiesForPackage(String packageName, PackageManager packageManager) {
       // final PackageManager packageManager = context.getPackageManager();

        final Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
        mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);
        mainIntent.setPackage(packageName);

        final List<ResolveInfo> apps = packageManager.queryIntentActivities(mainIntent, 0);
        return apps != null ? apps : new ArrayList<ResolveInfo>();
    }
	
	public static Drawable getFullResIcon(Resources resources, int iconId) {
	    Drawable d;
	    try {
	        //ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
	        //int iconDpi = activityManager.getLauncherLargeIconDensity();
	        int iconDpi=DisplayMetrics.DENSITY_XHIGH;
	        
	        d = resources.getDrawableForDensity(iconId, iconDpi);
	    } catch (Resources.NotFoundException e) {
	        d = null;
	    }

	    return (d != null) ? d : getFullResDefaultActivityIcon();
	}
	
	private static Drawable getFullResDefaultActivityIcon() {
	    return getFullResIcon(Resources.getSystem(), android.R.mipmap.sym_def_app_icon);
	}
	
	public static Drawable getFullResIcon(String packageName, int iconId, Context context) {
	    Resources resources;
	    try {
	        resources = context.getPackageManager().getResourcesForApplication(packageName);
	    } catch (PackageManager.NameNotFoundException e) {
	        resources = null;
	    }
	    if (resources != null) {
	        if (iconId != 0) {
	            return getFullResIcon(resources, iconId);
	        }
	    }
	    return getFullResDefaultActivityIcon();
	}
	
	public static boolean checkAppInstalled(String packageName,Context c){
		PackageManager pm = c.getPackageManager();
		boolean bInstalled=false;
		try{
			pm.getPackageInfo(packageName, PackageManager.GET_ACTIVITIES);
			bInstalled=true;
		}catch(PackageManager.NameNotFoundException e){
			bInstalled=false;
		}
		return bInstalled;
	}
}
