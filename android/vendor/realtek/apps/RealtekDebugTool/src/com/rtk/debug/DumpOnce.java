package com.rtk.debug;

import android.content.Context;
import android.net.*;
import android.os.Build;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.util.UtilLogger;
import com.rtk.debug.util.Utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import static android.content.Context.MODE_APPEND;

/**
 * Created by archerho on 2017/2/10.
 */
public class DumpOnce implements IDump {
    UtilLogger log = new UtilLogger(DumpOnce.class);
    private static DumpOnce mIns;
    private Context mCtx;

    public DumpOnce(Context ctx) {
        mCtx = ctx;
    }

    public static IDump getInstance(Context ctx) {
        if(mIns==null)
            mIns = new DumpOnce(ctx);
        return mIns;
    }

    private static String getLogPath(String tag){
        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String filename = dateFormat.format(date)+".log";
        String logFolder = SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+ DumpOnce.class.getSimpleName();
        File f = new File(logFolder);
        if(!f.exists()) f.mkdirs();

        String logFilePath = String.format("%s/%s_%s", logFolder, tag, filename);
        return logFilePath;
    }

    @Override
    public void on() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                //Dump dmesg
                String logPath = getLogPath("dmesg");
                String [] commands = { "sh", "-c", "dmesg > "+logPath };
                Utils.exec(commands);

                //Dump ifconfig
                logPath = getLogPath("ifconfig");
                String[] cmdIfconfig = { "sh", "-c", "ifconfig > "+logPath };
                Utils.exec(cmdIfconfig);

                //Dump apk version
                String logFolder = SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+ DumpOnce.class.getSimpleName();
                String logFilePath = String.format("%s/%s", logFolder, "ver."+Utils.getApkVersion(mCtx));
                String[] cmdApkVersion = { "sh", "-c", "touch "+logFilePath };
                Utils.exec(cmdApkVersion);

                //Dump dns servers
                dumpDnsServers();
            }
        }).start();


    }

    private void dumpDnsServers() {
        if(Build.VERSION.SDK_INT<Build.VERSION_CODES.M) return;
        String logPath = getLogPath("dns");
        ConnectivityManager cm =
                (ConnectivityManager) mCtx.getSystemService(Context.CONNECTIVITY_SERVICE);

        try(FileWriter fw = new FileWriter(logPath, true)) {

            LinkProperties lp = cm.getActiveLinkProperties();
            if(lp==null) {
                fw.append("No active network");
                return;
            } else {
                Network activeNet = cm.getActiveNetwork();
                NetworkInfo ni = cm.getNetworkInfo(activeNet);
                fw.append("active netId " + activeNet.netId + "," + ni.getTypeName()+"\n");
                fw.append('\n');
            }
            List<InetAddress> addresses = lp.getDnsServers();
            for(int i=0;i<addresses.size();i++){
                fw.append("dns"+(i+1)+": "+ addresses.get(i).toString()+"\n");
            }
            fw.append('\n');
        } catch (IOException e) {
            e.printStackTrace();
        }

        //dump dns by ndc resolver dump
//        Network[] nets = cm.getAllNetworks();
//
//        for (Network net : nets) {
//            NetworkInfo ni = cm.getNetworkInfo(net);
//
//            //dump all net info
//            String[] cmdAppendNetInfo = {"sh", "-c",
//                    "echo netId " + net.netId + "," + ni.getTypeName() + " >> " + logPath};
//            Utils.execWaitFor(cmdAppendNetInfo);
//
//            if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.N &&
//                    Build.VERSION.SDK_INT <=Build.VERSION_CODES.N_MR1) {
//                String tmpFilePath = "/tmp/dnsInfo";
//                File f = new File(tmpFilePath);
//                try {
//                    if (f.exists())
//                        f.delete();
//                    f.createNewFile();
//                    String[] cmdChmod = {"sh", "-c", "chmod 755 " + tmpFilePath};
//                    Utils.execWaitFor(cmdChmod);
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//
//                String[] cmdDumpDns = {"sh", "-c", "ndc resolver dump " + net.netId + " " + tmpFilePath};
//                Utils.execWaitFor(cmdDumpDns);
//
//                String[] cmdAppendDns = {"sh", "-c", "cat " + tmpFilePath + " >> " + logPath};
//                Utils.execWaitFor(cmdAppendDns);
//            }
//        }
    }

    @Override
    public void off() {

    }

    @Override
    public boolean isOn() {
        return true;
    }
}
