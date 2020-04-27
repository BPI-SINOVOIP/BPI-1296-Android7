package com.realtek.addon.widget;

import android.appwidget.AppWidgetManager;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.TrafficStats;
import android.net.wifi.WifiManager;
import android.widget.RemoteViews;

import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

public class NetworkTrafficWidgetProvider extends AbsRTKWidgetProvider {

    private static final int WIFI_SIGNAL_LEVELS=4;
    private static final int WIDGET_UPDATE_INTERVAL=2;
    
    private static final int ACTIVATE_STATE_ETH0 = 0;
    private static final int ACTIVATE_STATE_WIFI = 1;
    private static final int ACTIVATE_STATE_NONE = 2;
    
    private static long mRxTotal = 0;
    private static long mRxTimeStamp = 0;
    
    private static long mTxTotal = 0;
    private static long mTxTimeStamp = 0;
    
	@Override
	public Class<?> getMyClass() {
		// TODO Auto-generated method stub
		return NetworkTrafficWidgetProvider.class;
	}

	@Override
	public int getViewGroupID() {
		// TODO Auto-generated method stub
		return R.id.widget_root_view_group;
	}

	@Override
	public int getLayoutID() {
		// TODO Auto-generated method stub
		return R.layout.wl_network_traffic;
	}

	@Override
	public void onHandleClickIntent(Context c) {
		// TODO Auto-generated method stub

	}
	
	/*
	@Override
	public int getUpdateInterval(){
	    return WIDGET_UPDATE_INTERVAL;
	}
	*/
	
	@Override
	public void updateWidgetViews(Context context,
            AppWidgetManager appWidgetManager,
            int[] appWidgetIds){
	    /*
	    for(int id:appWidgetIds){
            DebugHelper.dump("onUpdate provider:"+getClassName()+" id:"+id);
            RemoteViews views=new RemoteViews(context.getPackageName(), getLayoutID());
            
            // update Text Label:
            views.setTextViewText(R.id.up_text_view, ""+getNetworkUpSpeed());
            views.setTextViewText(R.id.down_text_view, ""+getNetworkDownSpeed());
            
            // update wifi strength:
            views.setImageViewResource(R.id.wifi_strength_image, getBGResourceID(context));
            
            registerOnClickEvent(context, views, id, getOnClickIntent(context,id));
            appWidgetManager.updateAppWidget(id, views);
        }
        */
	}
	
	private static int getBGResourceID(Context c){
	    int net = getActivateNetworkInterface(c);
	    if(net==ACTIVATE_STATE_NONE)
	        return R.drawable.wi_download_upload_no_net;
	    else if(net==ACTIVATE_STATE_ETH0)
	        return R.drawable.wi_download_upload_eth0;
	    else
	        return getWifiStrengthImageID(c);
	}
	
	private static int getActivateNetworkInterface(Context c){
	    ConnectivityManager connManager = (ConnectivityManager) c.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo info = connManager.getActiveNetworkInfo();
	    if(info==null)
	        return ACTIVATE_STATE_NONE;
	    else{
	        DebugHelper.dump("[NET INFO] "+info.getTypeName()+", "+info.getType());
	        if(info.getType()==ConnectivityManager.TYPE_ETHERNET)
	            return ACTIVATE_STATE_ETH0;
	        else if(info.getType()==ConnectivityManager.TYPE_WIFI)
	            return ACTIVATE_STATE_WIFI;
	        else
	            return ACTIVATE_STATE_NONE;
	    }
	}
	
	private static boolean isWifiConnected(Context c){
        ConnectivityManager connManager = (ConnectivityManager) c.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo mWifi = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        
        return mWifi.isConnected();
    }
	
	private static int getWifiStrengthImageID(Context c){
	    
	    if(isWifiConnected(c)){
	        WifiManager wifiMgr = (WifiManager)c.getSystemService(Context.WIFI_SERVICE);
	        int rssi=wifiMgr.getConnectionInfo().getRssi();
            int level = WifiManager.calculateSignalLevel(rssi, WIFI_SIGNAL_LEVELS);
            DebugHelper.dump("[WIFI STRENGTH] Rssi:"+rssi+"level:"+level);
            if(level==0)
                return R.drawable.wi_download_upload_wifi_2;
            else if(level==1)
                return R.drawable.wi_download_upload_wifi_1;
            else if(level==2)
                return R.drawable.wi_download_upload_wifi_2;
            else if(level==3)
                return R.drawable.wi_download_upload_wifi_3;
            else
                return R.drawable.wi_download_upload_wifi_0;
	    }else{
	        return R.drawable.wi_download_upload_wifi_0;   // should not reach here, but keep it
	    }
	}
	
	/**
	 * Get current network upload speed
	 * @return upload speed KB/s
	 */
    private static int getNetworkUpSpeed(){
        
        long txTotalCurrent = TrafficStats.getTotalTxBytes();
        long txCurrentTime = System.currentTimeMillis();
        
        int byteDiff = (int) (txTotalCurrent-mTxTotal);
        int timeDiff = (int) (txCurrentTime-mTxTimeStamp);
        
        DebugHelper.dump("[NET TRAFFIC] TX total bytes="+txTotalCurrent);
        DebugHelper.dump("[NET TRAFFIC] TX byteDiff="+byteDiff);
        DebugHelper.dump("[NET TRAFFIC] TX timeDiff="+timeDiff/1000);
        
        mTxTotal=txTotalCurrent;
        mTxTimeStamp=txCurrentTime;
        
        //int fakeSpeed=(int)(Math.random()*100);
        
        DebugHelper.dump("[NET TRAFFIC] UP:"+byteDiff/timeDiff+" KB/s");
        
        return byteDiff/timeDiff;
    }
    
    /**
     * Get current network download speed
     * @return download speed KB/s
     */
	private static int getNetworkDownSpeed(){
	    
	    long rxTotalCurrent = TrafficStats.getTotalRxBytes();
	    long rxCurrentTime = System.currentTimeMillis();
	    
	    int byteDiff = (int) (rxTotalCurrent-mRxTotal);
	    int timeDiff = (int) (rxCurrentTime-mRxTimeStamp);
	    
	    DebugHelper.dump("[NET TRAFFIC] RX total bytes="+rxTotalCurrent);
	    DebugHelper.dump("[NET TRAFFIC] RX byteDiff="+byteDiff);
	    DebugHelper.dump("[NET TRAFFIC] RX timeDiff="+timeDiff/1000);
	    
	    mRxTotal=rxTotalCurrent;
	    mRxTimeStamp=rxCurrentTime;
	    
	    DebugHelper.dump("[NET TRAFFIC] DOWN:"+byteDiff/timeDiff+" KB/s");
	    
	    return byteDiff/timeDiff;
	}
}
