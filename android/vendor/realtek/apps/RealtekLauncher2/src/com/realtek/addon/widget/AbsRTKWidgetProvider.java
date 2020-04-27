package com.realtek.addon.widget;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.SystemClock;
import android.widget.RemoteViews;
import android.widget.Toast;

import com.realtek.addon.helper.DebugHelper;

/**
 * Abstract class as super class of all Realtek widget, the prototype of this widget provider class
 * allows basic onClick implementation.<p>Maybe sliding effect would be added later.
 * @author bruce_huang
 *
 */
public abstract class AbsRTKWidgetProvider extends AppWidgetProvider {
	
	public static final String ON_WIDGET_CLICK_ACT=".ON_WIDGET_CLICK";
	
	/**
	 * Return Class of WidgetProvider
	 * @return class of widgetProvider
	 */
	public abstract Class<?> getMyClass();
	
	/**
	 * Return ViewGroup ID of widget RemoteViews view group, used when registering touch event.
	 * @return ViewGroup ID
	 */
	public abstract int getViewGroupID();

	/**
	 * get layout file ID of the widget, used when creating RemoteViews
	 * @return
	 */
	public abstract int getLayoutID();
	
	/**
	 * how widget should handle onClickIntent
	 */
	public abstract void onHandleClickIntent(Context c);
	
	/**
	 * Get widget normal update interval, -1 for no update request. 
	 * @return update interval (sec).
	 */
	public int getUpdateInterval(){
	    return -1;
	}

	@Override
	public void onEnabled(Context context){
		super.onEnabled(context);
		DebugHelper.dump("onEnabled Provider:"+getClassName());
	}
	
	@Override
	public void onReceive (Context context, Intent intent) {
		
		String actionStr=intent.getAction();
		DebugHelper.dump("Class:"+getClassName()+" OnReceive:"+actionStr);
		
		AppWidgetManager appWidgetManager = AppWidgetManager.getInstance(context);
		ComponentName thisAppWidget = new ComponentName(context.getPackageName(), getClassName());
		int[] appWidgetIds = appWidgetManager.getAppWidgetIds(thisAppWidget);
		
		if(actionStr.equals(context.getPackageName()+ON_WIDGET_CLICK_ACT)){
			onHandleClickIntent(context);
		}else if(actionStr.equals(AppWidgetManager.ACTION_APPWIDGET_UPDATE)){
		    // One should not have to add this implementation, but we add it however.
			onUpdate(context, appWidgetManager, appWidgetIds);
		}
		super.onReceive(context, intent);
	}
	
	/**
	 * Standard function to update widget view.<br>One could overwrite this API and do it's own task.
	 */
	public void updateWidgetViews(Context context,
            AppWidgetManager appWidgetManager,
            int[] appWidgetIds){
	    
	    for(int id:appWidgetIds){
            DebugHelper.dump("onUpdate provider:"+getClassName()+" id:"+id);
            RemoteViews views=new RemoteViews(context.getPackageName(), getLayoutID());
            registerOnClickEvent(context, views, id, getOnClickIntent(context,id));
            appWidgetManager.updateAppWidget(id, views);
        }
	    
	}
	
	@Override
	public void onUpdate(Context context,
			AppWidgetManager appWidgetManager,
			int[] appWidgetIds) {
		
	    // call standard function to update widget view.
		updateWidgetViews(context, appWidgetManager, appWidgetIds);
		
		// CASE regular update if widget needs to be updated repeatedly, book next update event
		if(getUpdateInterval()>0){
		    registerWidgetUpdateNextTimer(context,
		            getUpdateInterval(),
		            AppWidgetManager.ACTION_APPWIDGET_UPDATE);
		}
		
		super.onUpdate(context, appWidgetManager, appWidgetIds);
	}
	
	/**
	 * Register widget onClickEvent based on the behavior defined in intent
	 * @param c context
	 * @param views clicked RemoteViews
	 * @param id widget ID
	 * @param i intent to be processed
	 */
	public void registerOnClickEvent(Context c, RemoteViews views, int id, Intent i) {
		PendingIntent pendingIntent = PendingIntent.getBroadcast(c, id, i, PendingIntent.FLAG_UPDATE_CURRENT);
		views.setOnClickPendingIntent(getViewGroupID(), pendingIntent);
	}
	
	/**
	 * return intent to be executed when widget is clicked
	 * @param c context
	 * @param id widgetID
	 * @return intent
	 */
	public Intent getOnClickIntent(Context c, int id) {
		Intent intent = new Intent(c, getMyClass());
		intent.setAction(c.getPackageName()+ON_WIDGET_CLICK_ACT);
		// RTKCOMMENT: don't need this part, since each widget instance of same type acts the same.
		intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID, id);
		
		return intent;
	}
	
	/**
	 * Return Class name of WidgetProvider
	 * @return class name of widgetProvider
	 */
	public String getClassName(){
		return getMyClass().getName();
	}
	
	/**
	 * Launch installed apk.
	 * @param packageName
	 * @param context
	 */
	protected void launchInstalledApkInNewTask(String packageName, Context context){
		final PackageManager packageManager = context.getPackageManager();
		Intent intent=packageManager.getLaunchIntentForPackage(packageName);
		if(intent==null){
		    String message = "Package: "+packageName+" not found";
		    Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
		}else{
		    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	        context.startActivity(intent);
		}
	}
	
	public void registerWidgetUpdateNextTimer(Context context, long trigger_time, String actionStr) {
        
        if(context==null) {
            DebugHelper.dump("registerWidgetUpdateNextTimer but context is null!!");
            return;
        }
        // DebugHelper.dump("registerWidgetUpdateNextTimer");
        DebugHelper.dump("Class:"+getClassName()+" register next update event:"+actionStr);
        Intent intent = new Intent(context, getMyClass());
        intent.setAction(actionStr);
        PendingIntent pi = PendingIntent.getBroadcast(context, 1, intent, PendingIntent.FLAG_ONE_SHOT);
        
        AlarmManager am = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        long triggerTime = SystemClock.elapsedRealtime() + trigger_time*1000;   // convert to milliseconds
        am.set(AlarmManager.ELAPSED_REALTIME, triggerTime, pi);
    }
}
