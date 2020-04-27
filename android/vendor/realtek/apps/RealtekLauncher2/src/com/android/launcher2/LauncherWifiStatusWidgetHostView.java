package com.android.launcher2;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings.System;
import android.provider.Settings;
import android.widget.ImageView;
import android.widget.TextView;

import com.realtek.addon.helper.ConstantsDef;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

public class LauncherWifiStatusWidgetHostView extends
        LauncherAppWidgetSelfUpdateHostView {
    
    private WifiManager wifiMgr;
    
    private TextView mMiracastTextView;
    private TextView mMiracastModeTextView;
    private TextView mDeviceNameTextView;
    private TextView mPinCodeTextView;
    private TextView mSSIDTextView;
    private ImageView bg;
    private ImageView breakIcon;
    
    private String mMiracastText=null;
    private String mMiracastSourceText=null;
    private String mMiracastSinkText=null;
    private String mNoSSIDText=null;
    
    private int id_source = -1;
    private int id_sink = -1;
    private int id_source_sink = -1;
    private int id_miracast_off = -1;
    
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            doSelfUpdate();
        }
    };
    
    public LauncherWifiStatusWidgetHostView(Context context, Launcher launcher) {
        super(context, launcher);
        
        wifiMgr = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        
        mMiracastText=context.getString(R.string.rtk_miracast_name);
        mMiracastSourceText=context.getString(R.string.rtk_miracast_mode_source_name);
        mMiracastSinkText=context.getString(R.string.rtk_miracast_mode_sink_name);
        mNoSSIDText=context.getString(R.string.rtk_no_ssid_name);
        
        inflateRootView(R.layout.wl_wifi_status_miracast, "Wifi status widget");
        
        id_source = R.drawable.wi_networkinfo_widgets_2;
        id_sink = R.drawable.wi_networkinfo_widgets_3;
        id_source_sink = R.drawable.wi_networkinfo_widgets_1;
        id_miracast_off = R.drawable.wi_networkinfo_widgets_0;
        
        if(mView!=null){
            mMiracastTextView=(TextView)mView.findViewById(R.id.miracast_text);
            mMiracastModeTextView=(TextView)mView.findViewById(R.id.miracast_mode_text);
            mDeviceNameTextView=(TextView)mView.findViewById(R.id.device_name_text);
            mPinCodeTextView=(TextView)mView.findViewById(R.id.pin_code_text);
            mSSIDTextView=(TextView)mView.findViewById(R.id.ssid_text);
            bg=(ImageView)mView.findViewById(R.id.wifi_connect_image);
            breakIcon=(ImageView)mView.findViewById(R.id.connect_break_image);
        }
        
        registerUpdateWidgetMessage(0);
    }
    
    private void doSelfUpdate(){
        doUpdate();
        registerUpdateWidgetMessage(2000);
    }
    
    /*
    private void checkDialogValidation(int p2pSoftAP){
        if(p2pSoftAP!=ConstantsDef.P2P){
            mLauncher.closeSourceSinkPopupDialog();
        }
    }
    */
    
    private void doUpdate(){
        
        // do nothing if launcher is not foreground.
        if(!mLauncher.launcherIsForeground())
            return;
        //DebugHelper.dumpSQA("Update Wifi Status Widget");

        int actNet=mLauncher.getActivateNetworkInterface();
        
        boolean bMiracastSourceOn=mLauncher.getMiracastSourceMode();
        boolean bMiracastSinkOn=mLauncher.getMiracastSinkMode();
        boolean bWifiEnabled = wifiMgr.isWifiEnabled();
        
        // Note: P2P is always enabled
        
        // 1. update miracast text, shows only under P2P mode.
        updateMiracastTextViews(bMiracastSourceOn,bMiracastSinkOn);
        
        // 2. update device name text
        mDeviceNameTextView.setText(getDeviceName());
        
        // 3. update Pin Code, or display "" if only source is on
        if(bMiracastSinkOn && bWifiEnabled)
            mPinCodeTextView.setText("pin: "+getPinCode());
        else
            mPinCodeTextView.setText("");
        
        breakIcon.setVisibility(INVISIBLE);
        
        // 4. update SSID, if wifi is connected
        if(actNet==ConstantsDef.NET_WIFI){
            mSSIDTextView.setText(getSSIDName());
        }else{
            mSSIDTextView.setText("");
        }

        if(actNet==ConstantsDef.NET_PPPOE){
            mSSIDTextView.setText("PPPoE link");
        }
        
        // 5. update network break icon
        if(actNet==ConstantsDef.NET_WIFI){
            if(mLauncher.isWifiConnected()){
                breakIcon.setVisibility(INVISIBLE);
            }else{
                breakIcon.setVisibility(VISIBLE);
            }
        }else if(actNet==ConstantsDef.NET_NONE){
            breakIcon.setVisibility(VISIBLE);
        }
        
        // 6. update background
        int id = getBackgroundImageID(bMiracastSourceOn,bMiracastSinkOn);
        bg.setImageResource(id);
        
        // it is unnecessary to check dialog validation, cause P2P is always enabled
        // checkDialogValidation(p2pSoftAP);
    }
    
    private void registerUpdateWidgetMessage(long delay){
        Message message;
        String obj = "update widget";
        message = handler.obtainMessage(1,obj);
        if(delay>0)
            handler.sendMessageDelayed(message, delay);
        else
            handler.sendMessage(message);
    }
    
    private String getPinCode(){
        String pin=android.provider.Settings.System.getString(mContext.getContentResolver(), ConstantsDef.KEY_MIRACAST_SINK_PIN_CODE);
        //DebugHelper.dumpSQA("PIN:"+pin);
        return pin;
    }
    
    private String getDeviceName(){
        String name = Settings.Global.getString(mContext.getContentResolver(), ConstantsDef.KEY_DEVICE_NAME);
        return name;
    }
    
    private void updateMiracastTextViews(boolean bSourceOn,boolean bSinkOn){
        mMiracastTextView.setText(mMiracastText);
        if(bSinkOn&&bSourceOn){
            mMiracastModeTextView.setText("");
        }else{
            if(bSinkOn)
                mMiracastModeTextView.setText(mMiracastSinkText);
            else if(bSourceOn)
                mMiracastModeTextView.setText(mMiracastSourceText);
            else
                mMiracastModeTextView.setText("");
        }
    }
     
    private String getSSIDName(){
        
        String ssid=mNoSSIDText;
        if(mLauncher.isWifiConnected()){
            ssid=wifiMgr.getConnectionInfo().getSSID();
        }
        
        DebugHelper.dump("[WIFI STATUS WIDGET] getSSID:"+ssid);
        return ssid;
    }
    
    private int getBackgroundImageID(boolean bSourceOn,boolean bSinkOn){
        if(bSourceOn && bSinkOn)
            return id_source_sink;
        else{
            if(bSourceOn)
                return id_source;
            else if(bSinkOn)
                return id_sink;
            else
                return id_miracast_off;
        }
    }
}
