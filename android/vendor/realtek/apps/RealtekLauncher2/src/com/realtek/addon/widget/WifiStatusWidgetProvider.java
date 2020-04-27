package com.realtek.addon.widget;

import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

import android.appwidget.AppWidgetManager;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.widget.RemoteViews;

public class WifiStatusWidgetProvider extends AbsRTKWidgetProvider {
    
    private static final int WIDGET_UPDATE_INTERVAL=2;
    private static int deviceID=-1;
    private static int pinCode=-1;
    
	@Override
	public Class<?> getMyClass() {
		// TODO Auto-generated method stub
		return WifiStatusWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		// TODO Auto-generated method stub
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		// TODO Auto-generated method stub
		return R.layout.wl_wifi_status_miracast;
	}

	@Override
	public void onHandleClickIntent(Context c) {
		// TODO Auto-generated method stub

	}
	
	@Override
	public void updateWidgetViews(Context context,
            AppWidgetManager appWidgetManager,
            int[] appWidgetIds){
	    /*
        for(int id:appWidgetIds){
            DebugHelper.dump("onUpdate provider:"+getClassName()+" id:"+id);
            RemoteViews views=new RemoteViews(context.getPackageName(), getLayoutID());
            
            // update Text Label:
            views.setTextViewText(R.id.device_name_text, "AirFun "+getDeviceID());
            views.setTextViewText(R.id.pin_code_text, "pin code "+getPinCode());
            views.setTextViewText(R.id.ssid_text, getSSIDName(context));
            
            // update background image
            if(isWifiConnected(context))
                views.setImageViewResource(R.id.wifi_connect_image, R.drawable.wi_network_widgets_1);
            else
                views.setImageViewResource(R.id.wifi_connect_image, R.drawable.wi_network_widgets_2);
            
            registerOnClickEvent(context, views, id, getOnClickIntent(context,id));
            appWidgetManager.updateAppWidget(id, views);
        }
        */
    }
	
	/*
	@Override
    public int getUpdateInterval(){
        return WIDGET_UPDATE_INTERVAL;
    }
	*/
	
	private static int getPinCode(){
	    if(WifiStatusWidgetProvider.pinCode==-1){
	        WifiStatusWidgetProvider.pinCode=(int)(Math.random()*10000);
	    }
	    return WifiStatusWidgetProvider.pinCode;
	}
	
	private static int getDeviceID(){
	    if(WifiStatusWidgetProvider.deviceID==-1){
	        WifiStatusWidgetProvider.deviceID=(int)(Math.random()*1000);
	    }
	    return WifiStatusWidgetProvider.deviceID;
	}
	
	private static boolean isWifiConnected(Context c){
	    ConnectivityManager connManager = (ConnectivityManager) c.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo mWifi = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        
        return mWifi.isConnected();
	}
	
	
	private static String getSSIDName(Context c){
	    
	    String ssid="N/A";
	    if(isWifiConnected(c)){
	        WifiManager wifiMgr = (WifiManager)c.getSystemService(Context.WIFI_SERVICE);
	        ssid=wifiMgr.getConnectionInfo().getSSID();
	    }
	    
	    DebugHelper.dump("[WIFI STATUS WIDGET] getSSID:"+ssid);
	    return ssid;
	}
}
