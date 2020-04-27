package com.realtek.rtksmb;

import android.content.Context;
import android.util.Log;
import jcifs.CIFSContext;
import jcifs.CIFSException;
import jcifs.DialectVersion;
import jcifs.config.PropertyConfiguration;
import jcifs.context.BaseContext;
import jcifs.context.SingletonContext;

import java.io.IOException;
import java.util.Properties;

public class SmbUtils {
    private static final String TAG = "SmbUtils";
    static CIFSContext sInstance;

    public static CIFSContext getContext() {
//        close();
        Properties prop = new Properties();
//        prop.putAll(System.getProperties());
        prop.setProperty("jcifs.resolveOrder", "DNS");
//        prop.setProperty("jcifs.netbios.soTimeout", "5");
//        prop.setProperty("jcifs.smb.client.disableSMB1", "true");
//        prop.setProperty("jcifs.smb.client.enableSMB2", "false");
//        prop.setProperty("jcifs.smb.client.minVersion", String.valueOf(DialectVersion.SMB1));
//        prop.setProperty("jcifs.smb.client.maxVersion", String.valueOf(DialectVersion.SMB202))
        PropertyConfiguration propertyConfiguration = null;
        try {
            propertyConfiguration = new PropertyConfiguration(prop);
        } catch (CIFSException e) {
            e.printStackTrace();
        }
        sInstance = new BaseContext(propertyConfiguration);
//        sInstance = SingletonContext.getInstance();
        return sInstance;
    }

    public static void close() {
        if (sInstance != null) {
            try {
                sInstance.close();
            } catch (CIFSException e) {
                e.printStackTrace();
            }
            sInstance = null;
        }
    }


    //smb://172.21.174.216/DATA_smb/pictures/081280.jpg
    public static String getIpFromSmbPath(String smbPath) {
        String[] tokens = smbPath.split("/");
        log("getIpFromSmbPath " + tokens[2]);
        return tokens[2];
    }

    private static void log(String s) {
        Log.d(TAG, s);
    }

    public static String getShareFromSmbPath(String smbPath) {
        String[] tokens = smbPath.split("/");
        log("getShareFromSmbPath " + tokens[3]);
        return tokens[3];
    }

    /**
     * @param str
     * @param c
     * @param n   0 means first occurrence
     * @return
     */
    public static int nthOccurrence(String str, char c, int n) {
        int pos = str.indexOf(c, 0);
        while (n-- > 0 && pos != -1)
            pos = str.indexOf(c, pos + 1);
        return pos;
    }

    public static int nthLastIndexOf(String str, char c, int n) {
        log(String.format("nthLastIndexOf %s %c %d", str, c, n));
        if (n < 1)
            return -1;
        int pos = str.length();
        while (n-- > 0 && pos != -1)
            pos = str.lastIndexOf(c, pos - 1);
        log(String.format("nthLastIndexOf :%d", pos));
        return pos;
    }

    public static String getPathFromSmbPath(String smbPath) {
        int slash = nthOccurrence(smbPath, '/', 3);
        if (slash == -1)
            return "";
        log("getPathFromSmbPath " + smbPath.substring(slash + 1));
        return smbPath.substring(slash + 1);
    }

    public static String[] tryGetSmbLoginInfo(Context ctx, String smbPath) {
        log(String.format("tryGetSmbLoginInfo smbPath=%s", smbPath));
        SmbUserMgr mgr = SmbUserMgr.getInstance(ctx);
        log("getUserInfo smbPath=" + smbPath);
        String userInfo = mgr.getUserInfo(smbPath);
        if (userInfo != null) {
//            log("getUserInfo userInfo="+userInfo);
            return userInfo.split(SmbUserMgr.USERINFO_SPLIT);
        } else {
            String rootPath = smbPath.substring(0, SmbUtils.nthOccurrence(smbPath, '/', 2) + 1);
            log("getUserInfo smbPath=" + smbPath + " ret null, fallback to " + rootPath);
            //fallback to search root path
            userInfo = mgr.getUserInfo(rootPath);
            if (userInfo != null)
                return userInfo.split(SmbUserMgr.USERINFO_SPLIT);
        }
        return null;
    }
}
