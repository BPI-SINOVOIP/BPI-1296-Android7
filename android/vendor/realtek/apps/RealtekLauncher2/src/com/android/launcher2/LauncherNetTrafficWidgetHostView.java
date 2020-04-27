package com.android.launcher2;

import android.content.Context;
import android.net.TrafficStats;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.Message;
import android.widget.ImageView;
import android.widget.TextView;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

public class LauncherNetTrafficWidgetHostView extends LauncherAppWidgetSelfUpdateHostView {
    
    private static final int WIFI_SIGNAL_LEVELS=4;
    
    private static final int UNDEFINED = -1;

    private long mRxTotal = 0;
    private long mRxTimeStamp = 0;
    
    private long mTxTotal = 0;
    private long mTxTimeStamp = 0;
    
    private ImageView bg;
    
    private WifiManager wifiMgr;
    
    private TextView mUploadView;
    private TextView mDownloadView;
    private TextView mUploadUnitView;
    private TextView mDownloadUnitView;
    
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            doSelfUpdate();
        }
    };
    
    public LauncherNetTrafficWidgetHostView(Context context, Launcher launcher) {
        super(context, launcher);
        
        mRxTotal=UNDEFINED;
        mTxTotal=UNDEFINED;
        
        mRxTimeStamp=UNDEFINED;
        mTxTimeStamp=UNDEFINED;
        
        wifiMgr = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        
        inflateRootView(R.layout.wl_network_traffic, "Network traffic widget");
        if(mView!=null){
            
            mUploadView = (TextView)mView.findViewById(R.id.up_text_view);
            mDownloadView = (TextView)mView.findViewById(R.id.down_text_view);
            mUploadUnitView = (TextView)mView.findViewById(R.id.up_unit_text_view);
            mDownloadUnitView = (TextView)mView.findViewById(R.id.down_unit_text_view);
            
            bg=(ImageView)mView.findViewById(R.id.net_mode_image);
        }
        
        registerUpdateWidgetMessage();
    }
    
    private void registerUpdateWidgetMessage(){
        Message message;
        String obj = "update widget";
        message = handler.obtainMessage(1,obj);
        // 2 secs
        handler.sendMessageDelayed(message, 2000);
    }
    
    private void doUpdate(){
        if(!mLauncher.launcherIsForeground())
            return;
        //DebugHelper.dumpSQA("Update Network traffic Widget");

        // 1. update Rx
        int rx=getNetworkUpSpeed();
        if(rx<0) rx=0;
        updateUploadMessage(rx); //mUploadView.setText(""+rx);
        // 2. update Tx
        int tx=getNetworkDownSpeed();
        if(tx<0) tx=0;
        updateDownloadMessage(tx); //mDownloadView.setText(""+tx);
        // 3. update wifi/eth0/no network
        bg.setImageResource(getBGResourceID());
    }
    
    private void doSelfUpdate(){
        DebugHelper.dump("NetTraffic Update");
        doUpdate();
        registerUpdateWidgetMessage();
    }
    
    private void updateDownloadMessage(int tx){
        boolean bDisplayMBs=(tx>=1024)?true:false;
        
        if(bDisplayMBs){
            mDownloadUnitView.setText("MB/s");
            float txf = ((float)tx)/1024f;
            int txi=(int)(txf*10);
            float txf2 = ((float)txi)/10;
            mDownloadView.setText(""+txf2);
        }else{
            mDownloadUnitView.setText("KB/s");
            mDownloadView.setText(""+tx);
        }
    }
    
    private void updateUploadMessage(int rx){
        boolean bDisplayMBs=(rx>=1024)?true:false;
        
        if(bDisplayMBs){
            mUploadUnitView.setText("MB/s");
            float rxf = ((float)rx)/1024f;
            int rxi=(int)(rxf*10);
            float rxf2 = ((float)rxi)/10;
            mUploadView.setText(""+rxf2);
        }else{
            mUploadUnitView.setText("KB/s");
            mUploadView.setText(""+rx);
        }
    }
    
    private int getBGResourceID(){
        int net = mLauncher.getActivateNetworkInterface();
        if(net==ConstantsDef.NET_NONE)
            return R.drawable.wi_download_upload_no_net;
        else if(net==ConstantsDef.NET_ETH0)
            return R.drawable.wi_download_upload_eth0;
        else
            return getWifiStrengthImageID();
    }
    
    private int getWifiStrengthImageID(){
        
        if(mLauncher.isWifiConnected()){
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
    private int getNetworkUpSpeed(){
        
        long txTotalCurrent = TrafficStats.getTotalTxBytes();
        long txCurrentTime = System.currentTimeMillis();
        
        if(mTxTotal==UNDEFINED || mTxTimeStamp==UNDEFINED){
            
            mTxTotal=txTotalCurrent;
            mTxTimeStamp=txCurrentTime;
            
            return 0;
        }else{
            int byteDiff = (int) (txTotalCurrent-mTxTotal);
            int timeDiff = (int) (txCurrentTime-mTxTimeStamp);
            
            DebugHelper.dump("[NET TRAFFIC] TX total bytes="+txTotalCurrent);
            DebugHelper.dump("[NET TRAFFIC] TX byteDiff="+byteDiff);
            DebugHelper.dump("[NET TRAFFIC] TX timeDiff="+timeDiff/1000);
            
            mTxTotal=txTotalCurrent;
            mTxTimeStamp=txCurrentTime;
            
            DebugHelper.dump("[NET TRAFFIC] UP:"+byteDiff/timeDiff+" KB/s");
            return byteDiff/timeDiff;
        }
    }
    
    /**
     * Get current network download speed
     * @return download speed KB/s
     */
    private int getNetworkDownSpeed(){
        
        long rxTotalCurrent = TrafficStats.getTotalRxBytes();
        long rxCurrentTime = System.currentTimeMillis();
        
        if(mRxTotal==UNDEFINED || mRxTimeStamp==UNDEFINED){
            
            mRxTotal=rxTotalCurrent;
            mRxTimeStamp=rxCurrentTime;
            
            return 0;
        }else{
            int byteDiff = (int) (rxTotalCurrent-mRxTotal);
            if(byteDiff<0) byteDiff=0;
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
}
