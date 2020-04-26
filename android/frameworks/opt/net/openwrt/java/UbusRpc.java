package com.rtk.net.openwrt;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.LinkProperties;
import android.net.RouteInfo;
import android.os.SystemProperties;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.net.Inet4Address;
import java.io.IOException;
import java.io.File;

import com.android.okhttp.MediaType;
import com.android.okhttp.OkHttpClient;
import com.android.okhttp.Request;
import com.android.okhttp.RequestBody;
import com.android.okhttp.Response;

import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.FileInputStream;

/**
 * Created by archerho on 2016/3/11.
 */
public class UbusRpc {
    private static final boolean DEBUG = true;
    private static final String TAG = "UbusRpc";

    private String mServerUrl = "http://127.0.0.1/ubus";
    private String mSessionId;
    private String mUsername;
    private String mPassword;

    public static boolean isXen(){
        return "xen".equals(SystemProperties.get("ro.boot.storage"));
    }

    public static boolean isOpenWrt(){
        return SystemProperties.getBoolean("persist.rtk.net.openwrt", false);
    }

    public static boolean isOttWifi(){
        return SystemProperties.getBoolean("persist.rtk.net.openwrt.ottwifi", false);
    }

    private static final MediaType JSON
            = MediaType.parse("application/json; charset=utf-8");

    static OkHttpClient client = new OkHttpClient();

    private static String post(String url, String json) throws IOException {
        RequestBody body = RequestBody.create(JSON, json);
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .build();
        Response response = client.newCall(request).execute();
        return response.body().string().trim();
    }

    public static String convertStreamToString(InputStream is) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();
        String line = null;
        while ((line = reader.readLine()) != null) {
          sb.append(line).append("\n");
        }
        reader.close();
        return sb.toString();
    }

    public static String getStringFromFile (String filePath) throws IOException {
        File fl = new File(filePath);
        FileInputStream fin = new FileInputStream(fl);
        String ret = convertStreamToString(fin);
        //Make sure you close all streams.
        fin.close();
        return ret;
    }

    private static final int NAP_TIME = 200;
    private static final String EXEC_STATUS = "tmp.exec_ubus";
    private static boolean waitProperty(String key, String expect, int maxwait){
        int maxnaps = (maxwait * 1000) / NAP_TIME;
        if (maxnaps < 1)
            maxnaps = 1;

        while(maxnaps-- >= 0){
            String val = SystemProperties.get(key, "");
            // log("val="+val+", expect="+expect);
            if(val.equals(expect))
                return true;

            if (maxnaps >= 0) {
                try{Thread.sleep(200);}catch(InterruptedException e){}
            }
        }
        return false;
    }

    private static String execBySh(String action, String path, String method, String args){
        log("execBySh");
        final String tmpParamPath = "/tmp/ubus-args";
        File f = new File(tmpParamPath);
        f.delete();
        final String tmpReturnPath = "/tmp/ubus-ret";
        String tmpArgs = String.format("%s\t%s\t%s\t%s", action, path, method, args);
        try(  PrintWriter out = new PrintWriter( tmpParamPath )  ){
            out.println( tmpArgs );
        } catch (FileNotFoundException e){
            e.printStackTrace();
        }
        SystemProperties.set(EXEC_STATUS, "1");
        SystemProperties.set("ctl.start", "sh_ubus");
        if(!waitProperty(EXEC_STATUS, "0", 30))
            log("waitProperty "+EXEC_STATUS+" timeout");

        f = new File(tmpParamPath);
        f.delete();
        File retFile = new File(tmpReturnPath);
        if(!retFile.exists())
            return null;
        try{
            String ret = getStringFromFile(tmpReturnPath);
            retFile.delete();
            return ret;
        }catch(IOException e){e.printStackTrace();}
        return null;
    }

    private static String exec(String action, String path, String method, String args){
        ProcessBuilder pb = new ProcessBuilder();
        pb.redirectErrorStream(true);
        ArrayList<String> list = new ArrayList<>();
        list.add("/system/bin/ubus");
        list.add(action);
        list.add(path);
        list.add(method);
        list.add(args);
        try {
            Process process = pb.command(list).start();
            process.waitFor();
            BufferedReader bufferedReader = new BufferedReader(
                    new InputStreamReader(process.getInputStream()));

            // Grab the results
            StringBuilder log = new StringBuilder();
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                log.append(line);
            }
            return log.toString();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return "";
    }

    private String postByBin(String paramsInJson){
        // log("before paramsInJson: " + paramsInJson);
        try {
            JSONArray jRoot = new JSONArray("["+paramsInJson+"]");
            String ret = "";
            if(jRoot.length()>=3) {
                paramsInJson = "call "+jRoot.get(0)+" "+jRoot.get(1)+" "+jRoot.get(2);
                log("exec ubus cmd: " + paramsInJson);
                ret = execBySh("call"
                    , jRoot.get(0).toString()
                    , jRoot.get(1).toString()
                    , jRoot.get(2).toString());
            } else if (jRoot.length()>=2) {
                paramsInJson = "call "+jRoot.get(0)+" "+jRoot.get(1);
                log("exec ubus cmd: " + paramsInJson);
                ret = execBySh("call"
                    , jRoot.get(0).toString()
                    , jRoot.get(1).toString()
                    , "");
            } else {
                log("paramsInJson must have least 2 parameters");
            }

            if(isJSONValid(ret)){
                return new JSONObject(ret).toString();
            } else {
                log("postByBin error:"+ret);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return null;
    }

    private String postByHttp(String paramsInJson){
        if(mSessionId==null)
            mSessionId = loginAndGetSessionId(mServerUrl, mUsername, mPassword);
        String json = "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"call\",\"params\":[\"" + mSessionId + "\","+paramsInJson+"]}";
        // log("post json: "+json);
        String resp = "";
        try {
            resp = post(mServerUrl, json);
            JSONObject jRootObj = new JSONObject(resp);
            org.json.JSONArray jsonArray = jRootObj.optJSONArray("result");
            if(jsonArray==null){
                log("postByHttp error:"+resp);
                return "";
            }
            int retCode = jsonArray.getInt(0);
            if(retCode==0){
                return jsonArray.getJSONObject(1).toString();
            } else {
                log("postByHttp error:"+resp);
            }
        } catch (JSONException e) {
            log("postByHttp error:"+resp);
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public String post(String paramsInJson) {
        if(isXen()){
            return postByHttp(paramsInJson);
        } else {
            return postByBin(paramsInJson);
        }
    }

    public static boolean hasUbusError(String json) {
        if(json==null) return true;
        boolean b = !(json.length()==0) && !isJSONValid(json);
        log(String.format("hasUbusError ret=%s json=%s", b, json));
        return b;
    }

    private static boolean isJSONValid(String test) {
        if(test==null) return false;
        try {
            new JSONObject(test);
        } catch (JSONException ex) {
            // edited, to include @Arthur's comment
            // e.g. in case JSONArray is valid as well...
            try {
                new org.json.JSONArray(test);
            } catch (JSONException ex1) {
                return false;
            }
        }
        return true;
    }

    private static void log(String s) {
        if (DEBUG)
            Log.d(TAG, s);
    }

    private static String loginAndGetSessionId(String serverUrl, String username, String password) {
        try {
            String resp = post(serverUrl, "{ \"jsonrpc\": \"2.0\", \"id\": 1, \"method\": \"call\", \"params\": [ \"00000000000000000000000000000000\", \"session\", \"login\", { \"username\": \""+username+"\", \"password\": \""+password+"\"  } ] }");
            if (hasUbusError(resp)) return null;
            String sessionId = null;
            JSONObject reader = new JSONObject(resp);
            JSONArray resultArray = reader.optJSONArray("result");
            if(resultArray!=null)
                sessionId = resultArray.getJSONObject(1).getString("ubus_rpc_session");
            log("sessionId=" + sessionId);
            return sessionId;
        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return "";
    }

    public void commit(String configName) {
        log("commit "+configName);
        String jsonParams = "\"uci\", \"commit\" , {\"config\": \"" + configName + "\"}";
        String resp = post(jsonParams);
        hasUbusError(resp);
    }

    private static final String DEFAULT_HOSTNAME = "127.0.0.1";
    private static final String XEN_OPENWRT_IP = "192.168.253.9";
    public static String getOpenWrtIp(Context ctx) {
        if(!isXen() && !isOpenWrt()) return "";
        if(!isXen() && isOttWifi()) return DEFAULT_HOSTNAME;
        if(isXen()) {
            return XEN_OPENWRT_IP;
        } else if(isOpenWrt()) {
            final ConnectivityManager mConnectivityManager = (ConnectivityManager) ctx.getSystemService(Context.CONNECTIVITY_SERVICE);
            LinkProperties linkProperties = mConnectivityManager.getLinkProperties(ConnectivityManager.TYPE_ETHERNET);
            if(linkProperties==null) return DEFAULT_HOSTNAME;
            for (RouteInfo route : linkProperties.getAllRoutes()) {
                if (route.isDefaultRoute() && route.getGateway() instanceof Inet4Address) {
                    log("getRouterHost() = " + route.getGateway().getHostAddress());
                    return route.getGateway().getHostAddress();
                }
            }
            log("Can NOT find router gateway!");
            return DEFAULT_HOSTNAME;
        } else {
            return DEFAULT_HOSTNAME;
        }
    }

    public static UbusRpc getUbusSession(Context ctx) {
        return getUbusSession(ctx, "root", "");
    }

    public static UbusRpc getUbusSession(Context ctx, String username, String password) {
        if(!isXen() && !isOpenWrt()) return null;
        if(!isXen() && !new File("/system/bin/ubus").exists()) return null;
        UbusRpc rpc = new UbusRpc();
        if(isXen()){
            String openwrt_ip = getOpenWrtIp(ctx);
            rpc.mServerUrl = "http://" + openwrt_ip + "/ubus";
            rpc.mUsername = username;
            rpc.mPassword = password;
        }
        return rpc;
    }
}
