package com.rtk.net.openwrt;

import android.content.Context;
import android.util.Log;
import android.os.SystemProperties;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;

import com.jayway.jsonpath.Configuration;
import com.jayway.jsonpath.JsonPath;
import com.jayway.jsonpath.PathNotFoundException;
import com.jayway.jsonpath.spi.json.JsonProvider;
import net.minidev.json.JSONArray;

public class Wifi {
    private static final boolean DEBUG = true;
    private static final String TAG = "OpenWrtWifi";
    private static final String EMPTY_STR = "";

    static UbusRpc sRpc = null;
    static JsonProvider sJsonPath = Configuration.defaultConfiguration().jsonProvider();

    public static int setupWlan0ToStaModeForAndroid(Context ctx) {
        sRpc = UbusRpc.getUbusSession(ctx);

        if(sRpc==null) return -1;

        String resp;
        resp = sRpc.post("\"uci\",\"get\",{\"config\":\"wireless\"}");
        if(resp==null) return -1;
        Object doc = sJsonPath.parse(resp);
        if(!isRadio0Enabled(doc)){
            log("enable radio 0");
            resp = sRpc.post("\"uci\",\"set\",{\"config\":\"wireless\",\"section\":\"radio0\",\"values\":{\"disabled\":\"0\"}}");
            if (UbusRpc.hasUbusError(resp)) return -1;
        }

        int wifiIfaceIndex = getLastWifiIfaceIndex(doc);
        if(!isRadio0HasStaModeInterface(doc)){
            log("add new wifi interface with sta mode");
            //add new wifi interface with sta mode
            resp = sRpc.post("\"uci\",\"add\",{\"config\":\"wireless\",\"type\":\"wifi-iface\"}");
            if (sRpc.hasUbusError(resp)) return -1;
            wifiIfaceIndex++; //new wifi interface id
            resp = sRpc.post("\"uci\",\"set\",{\"config\":\"wireless\",\"section\":\"@wifi-iface["+wifiIfaceIndex+"]\",\"values\":{\"device\":\"radio0\",\"mode\":\"sta\",\"network\":\"wwan\"}}");
            if (sRpc.hasUbusError(resp)) return -1;
        } else if(!isRadio0StaModeEnabled(doc)){
            log("enable sta interface");
            //enable sta interface
            resp = sRpc.post("\"uci\",\"set\",{\"config\":\"wireless\",\"section\":\"@wifi-iface["+wifiIfaceIndex+"]\",\"values\":{\"disabled\":\"0\"}}");
            if (sRpc.hasUbusError(resp)) return -1;
        }
        sRpc.commit("wireless");

        //1. add interface for wlan0 dhcp client
        if(!isNetworkWwanExist()) {
            resp = sRpc.post("\"uci\",\"add\",{\"config\":\"network\",\"name\":\"wwan\",\"type\":\"interface\",\"values\":{\"proto\":\"dhcp\"}}");
            if (sRpc.hasUbusError(resp)) return -1;
            sRpc.commit("network");
        }

        //3. set firewall wwan to wan zone
        if(!isFirewallWwanInWanExist()) {
            resp = sRpc.post("\"uci\",\"set\",{\"config\":\"firewall\",\"section\":\"@zone[1]\",\"values\":{\"network\":\"wan wwan\"}}");
            if (sRpc.hasUbusError(resp)) return -1;
            sRpc.commit("firewall");
        }

        return wifiIfaceIndex;
    }

    public static boolean disableWifi(Context ctx, int wifiIfaceIndex){
        if(wifiIfaceIndex==-1) return false;
        String resp = sRpc.post("\"uci\",\"set\",{\"config\":\"wireless\",\"section\":\"@wifi-iface["+wifiIfaceIndex+"]\",\"values\":{\"disabled\":\"1\"}}");
        if (sRpc.hasUbusError(resp)) return false;
        sRpc.commit("wireless");
        return true;
    }

    public static String getStaWlanInterface(int wifiIndex) {
        if(wifiIndex==-1) return "";
        //ubus call network.wireless status
        for(int i=0;i<20;i++) {
            String resp;
            String req = "\"network.wireless\", \"status\", {}";
            log("req=" + req);
            resp = sRpc.post(req);
            log("resp=" + resp);
            Object doc = sJsonPath.parse(resp);
            JSONArray ret = JsonPath.read(doc, "$.radio0.interfaces[?(@.section=='@wifi-iface[" + wifiIndex + "]')].ifname");
            if (ret.size()>0) {
                return ret.get(0).toString();
            }
            //wait to get wlan ifname
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return "";
    }


    private static int getLastWifiIfaceIndex(Object doc) {
        JSONArray ret = JsonPath.read(doc, "$.values..[?(@.device)]");
        log("getLastWifiIfaceIndex ret.size()="+ret.size());
        return ret.size()-1;
    }

    private static boolean isRadio0StaModeEnabled(Object doc) {
        JSONArray ret = JsonPath.read(doc, "$.values..[?(@.device=='radio0')][?(@.mode=='sta')][?(@.disabled!='1')]");
        log("isRadio0StaModeEnabled ret="+ret);
        return ret.size()>0;
    }

    private static boolean isRadio0HasStaModeInterface(Object doc) {
        JSONArray ret = JsonPath.read(doc, "$.values..[?(@.device=='radio0')][?(@.mode=='sta')]");
        log("isRadio0HasStaModeInterface ret="+ret);
        return ret.size()>0;
    }

    private static boolean isRadio0Enabled(Object doc) {
        JSONArray ret = JsonPath.read(doc, "$.values.radio0.[?(@.disabled!='1')]");
        log("isRadio0Enabled ret="+ret);
        return ret.size()>0;
    }

    private static boolean isNetworkWwanExist(){
        String resp = sRpc.post("\"uci\",\"get\",{\"config\":\"network\"}");
        if (sRpc.hasUbusError(resp)) return false;
        Object doc = sJsonPath.parse(resp);
        try {
            Object ret = JsonPath.read(doc, "$.values.wwan");
            log("isNetworkWwanExist ret "+ret);
            return ret!=null;
        } catch (PathNotFoundException e){
            e.printStackTrace();
            log("isNetworkWwanExist ret false");
        }
        return false;
    }

    private static boolean isFirewallWwanInWanExist(){
        String resp = sRpc.post("\"uci\",\"get\",{\"config\":\"firewall\"}");
        if (sRpc.hasUbusError(resp)) return false;
        Object doc = sJsonPath.parse(resp);
        JSONArray ret = JsonPath.read(doc, "$.values..[?(@.name=='wan')].network");
        log("isFirewallWwanInWanExist ret="+ret);
        if(ret==null) return false;
        if(ret.size()==0) return false;
        String network = (String) ret.toString();
        log("isFirewallWwanInWanExist network="+network);
        return network.contains("wwan");
    }

    private static void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }
}