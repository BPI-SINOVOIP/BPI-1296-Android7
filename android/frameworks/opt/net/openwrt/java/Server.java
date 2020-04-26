package com.rtk.net.openwrt;

import android.content.Context;
import android.util.Log;
import android.net.LinkProperties;
import android.net.RouteInfo;
import java.net.Inet4Address;
/*
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
*/

//import org.json.JSONException;
import com.rtk.net.openwrt.UbusRpc;
//import com.jayway.jsonpath.Configuration;
import com.jayway.jsonpath.JsonPath;
//import com.jayway.jsonpath.spi.json.JsonProvider;

public class Server {
    private final static String TAG = "NASServer";
    private static final boolean DEBUG = false;

    /**
     * NAS Supported Services
     */
    public enum Service{
        SAMBA("samba", "/nas/set/samba", new SambaStatus()),
        AFP("netatalk", "/nas/set/afpd", new AfpdStatus()),
        FTP("vsftpd", "/nas/set/ftpd", new FtpStatus()),
        ITUNES("forked-daapd", "/nas/set/daap", new DaapStatus()),
        DOWNLOADER("transmission", "/nas/set/bt", new BtStatus()),
        DLNA("minidlna", "/nas/set/dlna", new DlnaStatus());
        //ADVANCED("/nas/get/advance", "/nas/set/advance", new AdvanceStatus()),
        //CAMERA("/nas/get/ipcam", "/nas/set/ipcam", new IPCamStatus()),
        //NETWORK("/nas/get/network", "/nas/set/network", new NetworkStatus());
        // MEDIA("/nas/get/media", "/nas/set/media", new xxxxxxx()),
        // SMTP("/nas/get/smtp", "/nas/set/smtp", new xxxxxxx()),
        // DATE("/nas/get/date", "/nas/set/date", new xxxxxxx()),
        // ZONES("/nas/get/zones", "/nas/set/zones", new xxxxxxx()),
        // DATE("/nas/get/ntp", "/nas/set/ntp", new xxxxxxx()),
        // SPINDOWN("/nas/get/spindown", "/nas/set/spindown", new xxxxxxx()),   // Power Management
        // ADVANCE("/nas/get/advance", "/nas/set/advance", new xxxxxxx()),


        private final String getPath;
        private final String setPath;
        private final ServiceStatus status;

        Service(String getPath, String setPath, ServiceStatus status){
            this.getPath = getPath;
            this.setPath = setPath;
            this.status = status;
        }

    }

    private boolean isConnected = false;

    UbusRpc ubusRpc = null;
    //static JsonProvider sJsonPath = Configuration.defaultConfiguration().jsonProvider();

    /**
     * Create a server instance using default host name, user name and password
     */
    public Server(Context context){
        ubusRpc = UbusRpc.getUbusSession(context);

        if(ubusRpc == null) {
            this.isConnected = false;
        } else {
            this.isConnected =true;
        }

    }

    public boolean isConnected(){
        return this.isConnected;
    }

    public ServiceStatus getServiceStatus(Service service){
        try {
            String sJsonRequest = createJsonRequest(service.getPath);
            String sJsonRespone = ubusRpc.post(sJsonRequest);
            log("(getServiceStatus) cmd=" + sJsonRequest + ", respone=" + sJsonRespone);

            service.status.parse(sJsonRespone);
        }
        catch(Exception e) {e.printStackTrace();}

        log(service.status.toString());
        return service.status;
    }

    public boolean enableService(Service service, String enable) {
        boolean ret = false;

        try {
            String sJsonRequest = createJsonRequest(service.getPath, enable);
            String sJsonRespone = ubusRpc.post(sJsonRequest);
            log("(enableService) cmd=" + sJsonRequest + ", respone=" + sJsonRespone);
            //Object doc = sJsonPath.parse(sJsonRespone);
            Integer iRet = JsonPath.read(sJsonRespone, "$.code");
            if (iRet == 0) {
                log("enable/disable service "+service+" is success");
                ret = true;
            }
        }
        catch(Exception e) {e.printStackTrace();}

        return ret;
    }

    private String createJsonRequest(String serviceName) {
        return "\"uci\", \"get\" , {\"config\": \""+serviceName+"\", \"section\": \"@"+serviceName+"[0]\", \"option\": \"enabled\"}";
    }

    private String createJsonRequest(String serviceName, String enable) {
        return "\"file\", \"exec\" , {\"command\": \"/usr/local/sbin/nas-service\", \"params\": [ \""+serviceName+"\", \""+enable+"\" ] }";
    }

    private static void log(String s) {
        if (DEBUG)
            Log.d(TAG, s);
    }

    public String getDMSName() {
        String mDMSName = null;
        try {
            String sJsonRespone = ubusRpc.post("\"uci\",\"get\",{\"config\":\"minidlna\",\"section\":\"config\",\"option\": \"friendly_name\"}");
            Log.d(TAG,"jyy getDMSName sJsonRespone "+ sJsonRespone);
            mDMSName = JsonPath.read(sJsonRespone, "$.value");
        }
        catch(Exception e) {e.printStackTrace();}
        return mDMSName;
    }

    public void setDMSName(String mName) {
        String resp = ubusRpc.post("\"uci\",\"set\",{\"config\":\"minidlna\",\"section\":\"config\",\"values\":{\"friendly_name\":\""+mName+"\"}}");
        ubusRpc.commit("minidlna");
    }

    public void updateDNSServer(LinkProperties lp) {
        if(UbusRpc.isOpenWrt() && UbusRpc.isOttWifi()){
            log("updateDNSServer");
            if(lp==null) {
                Log.e(TAG, "lp==null");
                return;
            }
            final String DEFAULT_HOSTNAME = "127.0.0.1";
            String server=null;

            for (RouteInfo route : lp.getAllRoutes()) {
                if (route.isDefaultRoute() && route.getGateway() instanceof Inet4Address) {
                    log("getRouterHost() = " + route.getGateway().getHostAddress());
                    server = route.getGateway().getHostAddress();
                }
            }
            if(server==null) {
                Log.e(TAG, "server==null");
                return;
            }
            ubusRpc.post("\"file\", \"write\", {\"path\":\"/etc/resolv.conf\",\"data\":\"nameserver "+server+"\n\",\"append\":\"False\",\"mode\":\"644\",\"base64\":\"False\"}");
        }
    }
}
