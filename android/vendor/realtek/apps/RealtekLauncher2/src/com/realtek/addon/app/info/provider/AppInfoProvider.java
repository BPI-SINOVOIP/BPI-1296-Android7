package com.realtek.addon.app.info.provider;

import java.util.ArrayList;

import android.content.Context;

import com.android.launcher2.AllAppsList;
import com.android.launcher2.ApplicationInfo;
import com.android.launcher2.AppsCustomizePagedView.ContentType;
import com.realtek.addon.database.helper.DBManager;
import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;

public class AppInfoProvider {
	
	// RTKTODO: remove fakeMode flag.
	private static final boolean fakeMode=false;
	
	public Context context;
	//public PackageManager packageManager;
	
	public DBManager dbMgr;
	
	//private Object mFavoriteLock=new Object();
	//private Object mVideoLock=new Object();
	
	public AppInfoProvider(Context c){
		
		context=c;
		
		dbMgr=new DBManager(context);
		//packageManager = context.getPackageManager();
	}
	
	/**
	 * Update video app list, get video app list from database.<br>
	 * if some app are un-installed,
	 * this function would also update database
	 */
	public void setVideoAppInfo(ArrayList<ApplicationInfo> videoApps,
			ArrayList<ApplicationInfo> allApps){
		
		if(videoApps==null || allApps==null)
			return;
		
		if(fakeMode){
			for(ApplicationInfo info:allApps){
				videoApps.add(info);
			}
			return;
		}
		
		videoApps.clear();
		ArrayList<String> packageNameList=new ArrayList<String>();
		dbMgr.select(ConstantsDef.VIDEO, packageNameList);
		
		// find ApplicationInfo by packageName, and store to list
		for(String appPkgName:packageNameList){
			// note:
			ApplicationInfo app=AllAppsList.findAppInfoByPackageName(appPkgName, allApps);
			//findApplicationInfoByPackageName(appPkgName);
			if(app!=null){
				videoApps.add(app);
			}else{
				DebugHelper.dump("updateVideoAppInfo uninstalled, delete video db "+appPkgName);
				dbMgr.delete(ConstantsDef.VIDEO, appPkgName);
			}
		}
	}
	
	
	/**
	 *  Update favorite app info, (query from SQLite database), and save to favoriteApps lists,<br>
	 *  if app in database is already been deleted, this API would also update the database
	 */
	public void setFavAppInfo(ArrayList<ApplicationInfo> favoriteApps, ArrayList<ApplicationInfo> allApps){
		
		if(favoriteApps==null || allApps==null)
			return;
		
		if(fakeMode){
			for(ApplicationInfo info:allApps){
				favoriteApps.add(info);
			}
			return;
		}
		
		favoriteApps.clear();
		ArrayList<String> packageNameList=new ArrayList<String>();
		dbMgr.select(ConstantsDef.FAV, packageNameList);
		
		// find ApplicationInfo by packageName, and store to list
		for(String appPkgName:packageNameList){
			// note:
			ApplicationInfo app=AllAppsList.findAppInfoByPackageName(appPkgName, allApps);
					//findApplicationInfoByPackageName(appPkgName);
			if(app!=null){
				favoriteApps.add(app);
			}else{
				DebugHelper.dump("updateFavAppInfo uninstalled, delete FAV db "+appPkgName);
				dbMgr.delete(ConstantsDef.FAV, appPkgName);
			}
		}
	}
	
	/**
	 * @deprecated
	 * Read all installed packages on device, and save to inDrawerApps list.
	 * this API is deprecated.
	 *
	public void updateAppInDrawerInfo(ArrayList<AppInfo> inDrawerApps){
		if(inDrawerApps==null)
			return;
		inDrawerApps.clear();
		List<ApplicationInfo> installedApps = packageManager.getInstalledApplications(0);
		
		for(ApplicationInfo app:installedApps) {
			
			final List<ResolveInfo> matches=findActivitiesForPackage(app.packageName);
			if(matches.size()>0){
				//DebugHelper.dump("APP:"+app.packageName);
				inDrawerApps.add(new AppInfo(app));
			}
		}
	}
	*/
	
	/** Copied from Launcher2, get App which has CATEGORY_LAUNCHER intent **/
	/*
	private List<ResolveInfo> findActivitiesForPackage(String packageName) {
       // final PackageManager packageManager = context.getPackageManager();

        final Intent mainIntent = new Intent(Intent.ACTION_MAIN, null);
        mainIntent.addCategory(Intent.CATEGORY_LAUNCHER);
        mainIntent.setPackage(packageName);

        final List<ResolveInfo> apps = packageManager.queryIntentActivities(mainIntent, 0);
        return apps != null ? apps : new ArrayList<ResolveInfo>();
    } 
    */
	
	/**
	 * Add app + category entry into database
	 * @param type
	 * @param packageName
	 */
	public void addAppTo(ContentType type, String packageName){
		int categoryID=-1;
		if(type==ContentType.Favorite)
			categoryID=ConstantsDef.FAV;
		else if(type==ContentType.Video)
			categoryID=ConstantsDef.VIDEO;
		
		if(categoryID==-1 || packageName==null)
			return;
		
		dbMgr.insert(categoryID, packageName);
		
	}
	
	public void removeAppFrom(ContentType type, String packageName){
		int categoryID=-1;
		if(type==ContentType.Favorite)
			categoryID=ConstantsDef.FAV;
		else if(type==ContentType.Video)
			categoryID=ConstantsDef.VIDEO;
		
		if(categoryID==-1 || packageName==null)
			return;
		
		dbMgr.delete(categoryID, packageName);
	}
	
	/**
	 * Allow system to check if given app is already in current page
	 * @param type
	 * @param packageName
	 * @return
	 */
	public boolean checkAppExistance(ContentType type, String packageName){
		int categoryID=-1;
		if(type==ContentType.Favorite)
			categoryID=ConstantsDef.FAV;
		else if(type==ContentType.Video)
			categoryID=ConstantsDef.VIDEO;
		
		if(categoryID==-1 || packageName==null)
			return false;
		
		return dbMgr.checkExistence(categoryID, packageName);
	}
}

