package com.realtek.server;

import android.util.Slog;
import android.os.IBinder;
import android.os.SystemProperties;
import android.content.Context;
import android.content.ContentResolver;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.KeyMgmt;
import android.net.wifi.WifiConfiguration.AuthAlgorithm;
import android.net.wifi.WifiInfo;
import android.net.wifi.ScanResult;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import java.util.List;
import java.util.ArrayList;
import android.text.TextUtils;
import android.text.format.Formatter;

import org.json.JSONObject;
import org.json.JSONException;
import com.realtek.server.JSONTAG;

import com.realtek.provider.ISettingsService;

public class SettingsService extends ISettingsService.Stub {
	
	public static final String TAG = "SettingsService";

    private static final boolean LOCAL_LOGV = false;

    /**
     * These table names must be consistent with the names
     * defined in device/realtek/libs/Include/Application/AppClass/SetupClassAndroid.h"
     */
    private final String TABLE_SYSTEM = "system";
    private final String TABLE_GLOBAL = "global";
    private final String TABLE_SECURE = "secure";

    static final int WIFI_MAX_RETRY_TIME = 100;
    
    static final int SECURITY_NONE = 0;	
	static final int SECURITY_WEP = 1;
	static final int SECURITY_PSK = 2;
	static final int SECURITY_EAP = 3;
	
	enum PskType {
		UNKNOWN,
		WPA,
		WPA2,
		WPA_WPA2
	}

    /**
     * Binder context for this service
     */
    private Context mContext;
    private WifiLock mWifiLock = null;
    private WifiManager mWifiMgr = null;
    private ConnectivityManager mConnectionMgr = null;

    /**
     * Constructs a new SettingsService instance.
     * 
     */
    public SettingsService(Context context) {
        mContext = context;
        mWifiMgr = (WifiManager) mContext.getSystemService(Context.WIFI_SERVICE);
        mConnectionMgr = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        Slog.w(TAG, "SettingService is constructed!");
    }

    @Override
    public boolean putString(String table, String name, String value) {
        // TODO Auto-generated method stub
        if (LOCAL_LOGV) Slog.d(TAG, "putString -- table = " + table + ", name = " + name + ", value = " + value);
        boolean result = false;
        if (table.equals(TABLE_SYSTEM)) {
            result = Settings.System.putString(mContext.getContentResolver(), name, value);
        }
        else if (table.equals(TABLE_SECURE)) {
            result = Settings.Secure.putString(mContext.getContentResolver(), name, value);
        }
        else if (table.equals(TABLE_GLOBAL)) {
            result = Settings.Global.putString(mContext.getContentResolver(), name, value);
        }
        else {
            Slog.e(TAG, "unknown table " + table + " ...");
            return false;
        }
        return result;
    }
    
    @Override
    public boolean putLong(String table, String name, long value) {
        // TODO Auto-generated method stub
        if (LOCAL_LOGV) Slog.d(TAG, "putLong -- table = " + table + ", name = " + name + ", value = " + value);
        boolean result = false;
        if (table.equals(TABLE_SYSTEM)) {
            result = Settings.System.putLong(mContext.getContentResolver(), name, value);
        } 
        else if (table.equals(TABLE_SECURE)) {
            result = Settings.Secure.putLong(mContext.getContentResolver(), name, value);
        }   
        else if (table.equals(TABLE_GLOBAL)) {
            result = Settings.Global.putLong(mContext.getContentResolver(), name, value);
        }   
        else {
            Slog.e(TAG, "unknown table " + table + " ...");
            return false;
        }
        return result;
    }
    
    @Override
    public boolean putInt(String table, String name, int value) {
        // TODO Auto-generated method stub
        boolean result = false;
        if (LOCAL_LOGV) Slog.d(TAG, "putInt -- table = " + table + ", name = " + name + ", value = " + value);
        if (table.equals(TABLE_SYSTEM)) {
            result = Settings.System.putInt(mContext.getContentResolver(), name, value);
        }
        else if (table.equals(TABLE_SECURE)) {
            result = Settings.Secure.putInt(mContext.getContentResolver(), name, value);
        }   
        else if (table.equals(TABLE_GLOBAL)) {
            result = Settings.Global.putInt(mContext.getContentResolver(), name, value);
        }
        else {
            Slog.e(TAG, "unknown table " + table + " ...");
            return false;
        }
        return result;
    }
    
    @Override
    public boolean putFloat(String table, String name, float value) {
        // TODO Auto-generated method stub
        boolean result = false;
        Slog.d(TAG, "putFloat -- table = " + table + ", name = " + name + ", value = " + value);
        if (table.equals(TABLE_SYSTEM)) {
            result = Settings.System.putFloat(mContext.getContentResolver(), name, value);
        }
        else if (table.equals(TABLE_SECURE)) {
            result = Settings.Secure.putFloat(mContext.getContentResolver(), name, value);
        }   
        else if (table.equals(TABLE_GLOBAL)) {
            result = Settings.Global.putFloat(mContext.getContentResolver(), name, value);
        }
        else {
            Slog.e(TAG, "unknown table " + table + " ...");
            return false;
        }
        return result;
    }
    
    @Override
    public String getString(String table, String name) /*throws SettingNotFoundException*/ {
        // TODO Auto-generated method stub
        if (LOCAL_LOGV) Slog.d(TAG, "getString -- table = " + table + ", name = " + name);
        try {
            if (table.equals(TABLE_SYSTEM)) {
                return Settings.System.getString(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_SECURE)) {
                return Settings.Secure.getString(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_GLOBAL)) {
                return Settings.Global.getString(mContext.getContentResolver(), name);
            }
            else {
                Slog.e(TAG, "unknown table " + table + " ...");
                return null;
            }
        } catch (Exception e) {
            Slog.e(TAG, "Exception: " + e.getMessage());
            return null;
        }
    }
    
    @Override
    public long getLong(String table, String name) /*throws SettingNotFoundException*/ {
        // TODO Auto-generated method stub
        if (LOCAL_LOGV) Slog.d(TAG, "getLong -- table = " + table + ", name = " + name);
        try {
            if (table.equals(TABLE_SYSTEM)) {
                return Settings.System.getLong(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_SECURE)) {
                return Settings.Secure.getLong(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_GLOBAL)) {
                return Settings.Global.getLong(mContext.getContentResolver(), name);
            }
            else {
                Slog.e(TAG, "unknown table " + table + " ...");
                return 0;
            }
        } catch (SettingNotFoundException e) {
            Slog.e(TAG, "Exception: SettingsNotFound - " + e.getMessage());
            return 0;
        } 
    }
    
    @Override
    public int getInt(String table, String name) /*throws SettingNotFoundException*/ {
        // TODO Auto-generated method stub
        if (LOCAL_LOGV) Slog.d(TAG, "getInt -- table = " + table + ", name = " + name);
        try {
            if (table.equals(TABLE_SYSTEM)) {
                return Settings.System.getInt(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_SECURE)) {
                return Settings.Secure.getInt(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_GLOBAL)) {
                return Settings.Global.getInt(mContext.getContentResolver(), name);
            }
            else {
                Slog.e(TAG, "unknown table " + table + " ...");
                return 0;
            }
        } catch (SettingNotFoundException e) {
            Slog.e(TAG, "Exception: SettingsNotFound - " + e.getMessage());
            return 0;
        } 
    }
    
    @Override
    public float getFloat(String table, String name) /*throws SettingNotFoundException*/ {
        // TODO Auto-generated method stub
        if (LOCAL_LOGV) Slog.d(TAG, "getFloat -- table = " + table + ", name = " + name);
        try {
            if (table.equals(TABLE_SYSTEM)) {
                return Settings.System.getFloat(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_SECURE)) {
                return Settings.Secure.getFloat(mContext.getContentResolver(), name);
            }
            else if (table.equals(TABLE_GLOBAL)) {
                return Settings.Global.getFloat(mContext.getContentResolver(), name);
            }
            else {
                Slog.e(TAG, "unknown table " + table + " ...");
                return 0;
            }
        } catch (SettingNotFoundException e) {
            Slog.e(TAG, "Exception: SettingsNotFound - " + e.getMessage());
            return 0;
        } 
    }

    @Override
    public String getProperty(String prop, String def) {
        if (LOCAL_LOGV) Slog.d(TAG,  "getProperty -- prop = " + prop + ", default = " + def);
        return SystemProperties.get(prop, def);
    }

    @Override
    public boolean setProperty(String prop, String value) {
        if (LOCAL_LOGV) Slog.d(TAG,  "setProperty -- prop = " + prop + ", value = " + value);
        SystemProperties.set(prop, value);
        return true;
    }

    @Override
    public String wifiCmd(List<String> cmd) {
        if (LOCAL_LOGV) {
			String log = "wifiCmd - cmd:";
			for (int i = 0; i < cmd.size(); i++) {
				log += " " + cmd.get(i);
			}
			Slog.d(TAG, log);
        }
        
		JSONObject response = new JSONObject();
		try
		{
			if (cmd.get(1).equals("enabled"))
			{
				boolean enabled = mWifiMgr.isWifiEnabled();
				
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.WIFI_STATUS, enabled);
				
			} 
			else if (cmd.get(1).equals("switch"))
			{				
				if (cmd.get(2).equals("on"))
				{
					if (mWifiMgr.setWifiEnabled(true))
						response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);			
					else
						response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE); 			
				}
				else
				{
					if (mWifiMgr.setWifiEnabled(false))
						response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);			
					else
						response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE); 			
				}
			}
			else if (cmd.get(1).equals("scan"))
			{
				List<ScanResult> apList;
				
				//mWifiMgr.startScan();				
				apList = mWifiMgr.getScanResults();
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				response.put(JSONTAG.NUM_OF_ITEM, apList.size());

				for (int i = 0; i < apList.size(); i++)
				{
					JSONObject ap = new JSONObject();
					ap.put(JSONTAG.WIFI_AP_SSID, apList.get(i).SSID);
					ap.put(JSONTAG.WIFI_AP_SECURITY, apList.get(i).capabilities);
					ap.put(JSONTAG.WIFI_AP_STRENGTH, apList.get(i).level);
					response.accumulate(JSONTAG.WIFI_AP_LIST, ap);
				}
			}
			else if (cmd.get(1).equals("connect"))
			{
				if ( mWifiMgr.isWifiEnabled() == false)
				{	// Wifi must be turned on before connecting AP					
					mWifiMgr.setWifiEnabled(true);

                    int retry = WIFI_MAX_RETRY_TIME;
                    while(mWifiMgr.getWifiState() != WifiManager.WIFI_STATE_ENABLED && retry > 0)  
                    {
                        Slog.w(TAG, "Wait for Wifi enabling...");
                        try{  
                            // To avoid busy looping, it sleeps for 100 ms each time
                            Thread.currentThread();  
                            Thread.sleep(100);
                            retry--;
                        }  
                        catch(InterruptedException ie){
                            Slog.w(TAG, "Interrupt exception");
                        }  
                    }
				}
				String ssid = cmd.get(2), passwd = cmd.get(3);
				int security = Integer.parseInt(cmd.get(4));                

                if(isCurrentConnectedWifi(ssid))
                {
                    Slog.w(TAG, "already connected with " + ssid);
                    response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
                    response.put(JSONTAG.CMD_ERR_DESC, "already connected with " + ssid);
                    return response.toString();                    
                }

                WifiConfiguration conf = lookupMatchedWifiCfg(ssid);

                if (conf != null) {
                    // okay, using users prefer ap
                    // now it has to remove the old cfg
                    mWifiMgr.removeNetwork(conf.networkId);
    				conf = null;
                } 

				conf = new WifiConfiguration();
				conf.SSID = convertToQuotedString(ssid); // Please note the quotes. String should contain ssid in quotes
				setWifiCfgProtocol(conf, security);
				setWifiCfgKey(conf, security, passwd);

                int newNetId = conf.networkId;
                conf.status = WifiConfiguration.Status.ENABLED;
                newNetId = mWifiMgr.addNetwork(conf); 
				if (newNetId == -1)
                {
                    Slog.w(TAG, "unable to add netCfg. break!");
                    response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
                    response.put(JSONTAG.CMD_ERR_DESC, "unable to add netCfg!");
                    return response.toString();
                } 

                mWifiMgr.saveConfiguration();
                aquireWifiLock();
                
                // force to disable others (??)
                mWifiMgr.disconnect();                
                if (mWifiMgr.enableNetwork(newNetId, true)) {
                    mWifiMgr.reconnect();                    
                }
                releaseWifiLock();                      
			}
			else if (cmd.get(1).equals("disconnect"))
			{
				if (mWifiMgr.disconnect())
					response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				else
					response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
			}
            else if (cmd.get(1).equals("remove"))
            {
                int netId = Integer.parseInt(cmd.get(2)); 
                if (mWifiMgr.removeNetwork(netId))
                    response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
                else
                    response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
            }
			else if (cmd.get(1).equals("query_connection_info"))
			{
				NetworkInfo networkInfo = mConnectionMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_SUCCESS);
				if (mWifiMgr.isWifiEnabled() && networkInfo != null && networkInfo.isConnected())
				{
					// wifi is connected
					WifiInfo wifiInfo= mWifiMgr.getConnectionInfo();
					DhcpInfo dhcpInfo = mWifiMgr.getDhcpInfo();

					response.put(JSONTAG.WIFI_INFO_CONNECTED, true);
					if (wifiInfo != null) {
						int networkId = wifiInfo.getNetworkId();
						response.put(JSONTAG.WIFI_INFO_NETWORKID, networkId);
						response.put(JSONTAG.WIFI_INFO_SSID, removeQuotesMark(wifiInfo.getSSID()));

						// security
						List<WifiConfiguration> wifiConfList= mWifiMgr.getConfiguredNetworks();						
						for (WifiConfiguration i: wifiConfList) {
							if (i.networkId == networkId) {
								int security = getSecurity(i);
								response.put(JSONTAG.WIFI_INFO_SECURITY, getSecurityString(security, PskType.UNKNOWN, true));
								break;
							}
						}
					}
                    
					if (dhcpInfo != null) {
						response.put(JSONTAG.WIFI_INFO_IP, Formatter.formatIpAddress(dhcpInfo.ipAddress));
						response.put(JSONTAG.WIFI_INFO_SUBMASK, Formatter.formatIpAddress(dhcpInfo.netmask));
						response.put(JSONTAG.WIFI_INFO_GATEWAY, Formatter.formatIpAddress(dhcpInfo.gateway));
					}
				}
				else 
				{
					// wifi is not connected
					response.put(JSONTAG.WIFI_INFO_CONNECTED, false);					
				}
			}
			else
			{
				response.put(JSONTAG.CMD_STATUS, JSONTAG.CMD_STATUS_CODE_FAILURE);
				response.put(JSONTAG.CMD_ERR_DESC, "Unsupported wifi command");
			}
		} catch (JSONException e) {
			Slog.e(TAG,  "JSON exception");
		}

        Slog.i(TAG, response.toString());
        return response.toString();
    }

    private String getValidSSID(String ssid) {
        String _s = removeQuotesMark(ssid);
        return !TextUtils.isEmpty(_s) ? _s.trim() : null;
    }
    
    // SSID fully match
    private boolean isSSIDMatched(final String ssid, final String key) {
        String _s = getValidSSID(ssid);
        return !TextUtils.isEmpty(_s) && _s.equals(getValidSSID(key));
    }    

    private WifiInfo getConnectionInfo() {
        if (mWifiMgr.isWifiEnabled()) {
            NetworkInfo wifiInfo = mConnectionMgr.getNetworkInfo(ConnectivityManager.TYPE_WIFI);

            if (wifiInfo != null && wifiInfo.isConnected()) {
                WifiInfo info = mWifiMgr.getConnectionInfo();
                if (info != null && getValidSSID(info.getSSID()) != null) {
                    NetworkInfo.DetailedState wifiState = WifiInfo.getDetailedStateOf(info.getSupplicantState());
                    if (wifiState != NetworkInfo.DetailedState.CONNECTED) {
                        Slog.i(TAG, "ConnectionInfo is not CONNECTED! state=" + wifiState);
                    }
                    return info;
                } else {
                    Slog.w(TAG, "unable to get wifiConnInfo:" + info);
                }
            } else {
                Slog.w(TAG, "There's no connected wifiInfo:" + wifiInfo);
            }
        }
        Slog.i(TAG, "wifi is disable currently");
        
        return null;
    }

    private boolean isCurrentConnectedWifi(String ssid)
    {
        WifiInfo wifiInfo= getConnectionInfo();
        if (wifiInfo != null) {
            return isSSIDMatched(wifiInfo.getSSID(), ssid);
        }
        return false;
    }

    private WifiConfiguration lookupMatchedWifiCfg(String sSSID) {
        Slog.v(TAG, "lookup WifiConfiguration for SSID=" + sSSID);
        WifiConfiguration wifiCfg = null;
        
        if (sSSID != null) {
            List <WifiConfiguration> cfgList = mWifiMgr.getConfiguredNetworks();
            try {
                for (WifiConfiguration cfg:cfgList){                   
                    if (isSSIDMatched(cfg.SSID, sSSID)) {
                        boolean bMatched = true;
                        
                        if (bMatched) {
                        	wifiCfg = cfg;
                            break;
                            
                        }
                    }
                }
            } catch (NullPointerException e) {
                Slog.e(TAG, "there's no cfgList");
                e.printStackTrace();
            }
        }
        return wifiCfg;
    }

	private void setWifiCfgProtocol(WifiConfiguration config, int security) {
        if (!(config instanceof WifiConfiguration)) {
            return;
        }

		Slog.v(TAG, "init wifiCnf with security = " + security);
		
        if (security == SECURITY_NONE) {
            config.allowedKeyManagement.set(KeyMgmt.NONE);
        } else if (security == SECURITY_WEP) {
            config.allowedKeyManagement.set(KeyMgmt.NONE);
            config.allowedAuthAlgorithms.set(AuthAlgorithm.OPEN);
            config.allowedAuthAlgorithms.set(AuthAlgorithm.SHARED);

            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
        } else if (security == SECURITY_PSK) {
            config.hiddenSSID = true;
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
            config.allowedAuthAlgorithms.set(AuthAlgorithm.OPEN);
            
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);  
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);  
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);  
            config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
            config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
            config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
            config.allowedProtocols.set(WifiConfiguration.Protocol.RSN); 
            config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
        } else if (security == SECURITY_EAP) {
            config.allowedKeyManagement.set(KeyMgmt.WPA_EAP);
            config.allowedKeyManagement.set(KeyMgmt.IEEE8021X);
            Slog.w(TAG, "it's going to config an EAP wifi-ap!");
            /*
            // Initialize other fields.
            config.phase2.setValue("");
            config.ca_cert.setValue("");
            config.client_cert.setValue("");
            config.private_key.setValue("");
            config.identity.setValue("");
            config.anonymous_identity.setValue("");
            */
         	// since the connection method for eap wifi-ap is modified
        }		
	}

	private void setWifiCfgKey(WifiConfiguration config, int security, String passwd) {
        if (!(config instanceof WifiConfiguration)) {
            return;
        }
		Slog.v(TAG, "attempt to set a security key. security/key=(" + security + "," + passwd + ")");

        if (security == SECURITY_NONE) {
            //config.wepTxKeyIndex = 0;
            //config.wepKeys[0] = "";
            Slog.v(TAG, "set key to an Open security AP.");
            return;
        }

        if (TextUtils.isEmpty(passwd)) {
            return;
        }

        int passwdLen = passwd.length();
        if (security == SECURITY_WEP) {
            if ((passwdLen == 10 || passwdLen == 26 || passwdLen == 58) &&
                    passwd.matches("[0-9A-Fa-f]*")) {
                config.wepKeys[0] = passwd;                
            } else {
                config.wepKeys[0] = convertToQuotedString(passwd);
            }
        } else if (security == SECURITY_PSK) {
            if (passwd.matches("[0-9A-Fa-f]{64}")) {
                config.preSharedKey = passwd;
            } else {
                config.preSharedKey = convertToQuotedString(passwd);
            }
        } else if (security == SECURITY_EAP) {
            //config.eap.setValue(passwd);
            // since the connection method for eap wifi-ap is modified
        }		
	}
	
    private int getSecurity(WifiConfiguration config) {
        if (config.allowedKeyManagement.get(KeyMgmt.WPA_PSK)) {
            return SECURITY_PSK;
        }
        if (config.allowedKeyManagement.get(KeyMgmt.WPA_EAP) ||
                config.allowedKeyManagement.get(KeyMgmt.IEEE8021X)) {
            return SECURITY_EAP;
        }
        return (config.wepKeys[0] != null) ? SECURITY_WEP : SECURITY_NONE;
    }	
	
    private String getSecurityString(int security, PskType pskType, boolean concise) {
        switch(security) {
            case SECURITY_EAP:
                return concise ? "802.1x" : "802.1x EAP";
            case SECURITY_PSK:
                switch (pskType) {
                    case WPA:
                        return concise ? "WPA" : "WPA PSK";
                    case WPA2:
                        return concise ? "WPA2" : "WPA2 PSK";
                    case WPA_WPA2:
                        return concise ? "WPA/WPA2" : "WPA/WPA2 PSK";
                    case UNKNOWN:
                    default:
                        return concise ? "WPA/WPA2" : "WPA/WPA2 PSK";
                }
            case SECURITY_WEP:
                return "WEP";
            case SECURITY_NONE:
            default:
                return concise ? "" : "None";
        }
    }
    
    private void aquireWifiLock() {
        if (mWifiLock == null) {
            mWifiLock = mWifiMgr.createWifiLock(WifiManager.WIFI_MODE_FULL, TAG);
            if (mWifiLock != null) {
            	mWifiLock.acquire();
            }
        }
    }
    
    private void releaseWifiLock() {
        if (mWifiLock != null) {
            if (mWifiLock.isHeld()) {  
                mWifiLock.release();  
            }
            mWifiLock = null;
        }
    }
    
	private String removeQuotesMark(String qstr) {
	    if (qstr != null) {
            int strLen = qstr.length();
            if ((strLen > 1) && (qstr.charAt(0) == '"')
                    && (qstr.charAt(strLen - 1) == '"')) {
                return qstr.substring(1, strLen - 1);
            }
        }
        return qstr;
    }

    private String convertToQuotedString(String content) {
        return content != null ? ("\"" + content + "\"") : null;
    }
    
}
