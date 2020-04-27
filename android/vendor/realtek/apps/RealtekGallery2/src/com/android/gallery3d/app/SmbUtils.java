package com.android.gallery3d.app;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import android.content.Context;
import android.net.Uri;
import android.util.Log;

import android.widget.Toast;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;

import jcifs.Config;
import jcifs.UniAddress;
import jcifs.netbios.NbtAddress;
import jcifs.smb.NtlmPasswordAuthentication;
import jcifs.smb.SmbFile;

public class SmbUtils {
    private static final String TAG = "SmbUtils";
    private static final boolean DEBUG = true;
    
    public static final String SMB_MOUNT_ACTION  = "android.intent.action.SMB_MOUNT";
    public static final String SMB_UMOUNT_ACTION = "android.intent.action.SMB_UMOUNT";
    public static final String MOUNT_ROOT = "/tmp/ramfs/mnt/";
    public static final int SmbMountOkay = 933;
    public static final int SmbMountFailed = 934;
    public static final int SmbUmountOkay  = 935;
    public static final int SmbUmountFailed = 936;
    
    private static SmbUtils objSmbUtils = null;
    private static Context mContext = null;
    private String mServerIP = null;    
    
    static {
        configure();
    }
    
    private final BroadcastReceiver SmbStatusReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            log("Smb Status Action = " + action);
            int code = intent.getIntExtra("code" , 0);
            if (code == SmbMountOkay) {
                String mountPath = intent.getStringExtra("mountPath");
                log("Receive SmbMountSuccess: " + mountPath);
                //mountPath.replace(0, mountPath.length(), intent.getStringExtra("mountPath"));
            }
            else if (code == SmbMountFailed) {
                log("Receive SmbMountFailed");
                //mountPath.replace(0, mountPath.length(),"");
                Toast.makeText(context, "Samba mount failed!", Toast.LENGTH_SHORT).show();
            }
            else if (code == SmbUmountOkay) {
                log("Receive SmbUmountOkay");
            }
            else if (code == SmbUmountFailed) {
                log("Receive SmbUmountFailed");
            }
        }
    };
        
        
	private SmbUtils(Context context) {
		mContext = context;
        if(context != null) {
            final IntentFilter userFilter = new IntentFilter();
            userFilter.addAction("android.rtksmb.resp_code");
            context.registerReceiver(SmbStatusReceiver, userFilter);
        }
	}

	public static SmbUtils getInstance(Context context) {
        log("Obtain SmbUtils Instance");
		if(objSmbUtils == null) {
			objSmbUtils = new SmbUtils(context);
		}
		objSmbUtils.mContext = context;
		return objSmbUtils;
	}
    
	public static void deleteInstance()
	{
        log("Delete SmbUtils Instance");
        if (objSmbUtils != null) {
            objSmbUtils.mContext.unregisterReceiver(objSmbUtils.SmbStatusReceiver);
            if (objSmbUtils.CheckSmbMount(null)) {
                Intent intent = new Intent(SMB_UMOUNT_ACTION)
                                    .putExtra("smb_mount_cmd", true);
                objSmbUtils.mContext.sendBroadcast(intent);
            }
        }
		objSmbUtils = null;
	}

    public Uri mountSambaServer(Uri videoUri) {
        log("Samba file uri: "+videoUri.toString());
        
        String server = videoUri.getHost();
        String share = null;
        String password = null;
        String username = null;
        String domain = null;
        String fileName = null;            
                   
        String path = videoUri.getPath();
        if(path != null) {
            int length = path.length();
            int index = path.lastIndexOf('/');
            if(index > 1 && index < length-1) {
                fileName = path.substring(index+1);
                share = path.substring(1, index);
            }
        }
        
        String userInfo = videoUri.getUserInfo();
        if(userInfo != null) {
            int index = userInfo.lastIndexOf(':');
            if(index > -1) {
                password = userInfo.substring(index+1);
                userInfo = userInfo.substring(0, index);
            }
            index = userInfo.lastIndexOf(';');
            if(index > -1) {
                domain = userInfo.substring(0, index);
                userInfo = userInfo.substring(index+1);
                
            } 
            username = userInfo;
        }
        log("server="+server+", domain="+domain+", user="+username+", password="+password+", share="+share+", file="+fileName);

        if(!ipCheck(server)) {
            ScannerSmbServerIP Scanner = new ScannerSmbServerIP();
            Scanner.scannerSmbServerIP(server);
            
            try  {
                Scanner.join();
            } catch(InterruptedException e) {
                e.printStackTrace () ;
            }        
            
            server = Scanner.getSmbServerIP();
        }
        
        Log.d(TAG, "server="+server+", domain="+domain+", user="+username+", password="+password+", share="+share+", file="+fileName);
        
        mServerIP = server;
        
        // Samba mount
        SmbMount(server, share, domain, username, password);
        
        // The actual video URI to be used by MediaPlayer
        return Uri.parse("file://" + SmbUtils.MOUNT_ROOT + server + "_" + share + "/" + fileName);        
    }
    
    public void SmbMount(String server, String share, String domain, String user, String password) { 
        String mountPath = MOUNT_ROOT+server+"_"+share;
        CreateMountPath(mountPath);
        String smbPath = "//"+server+"/"+share;
        if(!CheckSmbMount(mountPath)) {
            Intent intent = new Intent(SMB_MOUNT_ACTION)
                                .putExtra("smb_mount_cmd", true)
                                .putExtra("smbPath", smbPath)
                                .putExtra("mountPath", mountPath)
                                .putExtra("domain", domain)
                                .putExtra("user", user)
                                .putExtra("passwd", password);
            mContext.sendBroadcast(intent);
            try {
                Thread.sleep(20);
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else {
            log(smbPath + "already mount sucess");
        }      
    }
    
    //Create new directory used to mount samba server
    private boolean CreateMountPath(String mountPath)
    {
        File file = new File(mountPath);
        if (!file.exists()) {
            log("mkdir success");
            return file.mkdirs();
        }
        log("mkdir failed");
        return true;
    }

 	//Check to see if the Samba server has mounted 
    public static boolean CheckSmbMount(String mountPath) {
        String checkPath = (mountPath != null) ? mountPath : MOUNT_ROOT;
        File file = new File("/proc/mounts");
        if (file.canRead()) {
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(file));
                String text = null;
                while ((text = reader.readLine()) != null) { 
                    String[] segs = text.split(" ");
                    if (segs.length > 1) {
                        if (segs[1].indexOf(checkPath) == 0) {
                            reader.close();
                            log("the Samba server has mounted, return true");
                            return true;
                        }
                    }
                }
            } catch (IOException e) {
                Log.d(TAG, "IOException", e);
            } finally {
                try {
                    if (reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    Log.d(TAG, "IOException", e);
                }
            }
        }
        log("the Samba server has not mounted");
        return false;
    }
     
    // Unmount samba server
    public void UnmountSmb()
    {
	    log("UnmountSmb");
        if (CheckSmbMount(null)) {
            Intent intent = new Intent(SMB_UMOUNT_ACTION)
                                .putExtra("smb_mount_cmd", true);
            mContext.sendBroadcast(intent);
        }		
    }
    
    public String getSmbServerIP() {
        return mServerIP;
    } 
 
    public static void configure() {
        Config.setProperty("jcifs.resolveOrder", "BCAST");
        Config.setProperty("jcifs.smb.client.responseTimeout", "30000");
        Config.setProperty("jcifs.netbios.soTimeout", "1000");
        Config.setProperty("jcifs.netbios.cachePolicy", "-1");
    } 
    
    public class ScannerSmbServerIP  extends Thread {        
        private String mServerName;
        private String mServerIP;
        
        @Override
        public void run() {
            try {
                SmbFile smbFile = new SmbFile("smb://", NtlmPasswordAuthentication.ANONYMOUS);
                smbFile.setConnectTimeout(5000);
                SmbFile[] domains = smbFile.listFiles();
                
                for (SmbFile d : domains) {
                    SmbFile[] machines = d.listFiles();
                    for (SmbFile m : machines) {
                        try {                            
                            String substring = m.getName().substring(0, m.getName().length()-1);
                            log("Server_name: " + substring);
                            
                            if(substring.equals(mServerName)) {
                                UniAddress byName = UniAddress.getByName(substring);
                                if (byName != null) {
                                    mServerIP = byName.getHostAddress();
                                    Log.d(TAG, mServerName+": "+mServerIP);
                                    break;
                                }
                            }
                        } catch (Throwable e) {
//                            e.printStackTrace();
                        }
                    }
                }
            } catch (Throwable e2) {
//                e2.printStackTrace();
            }            
        }
        
        public void scannerSmbServerIP(String server) {
            this.mServerName = server;
            this.start();
        }
        
        public String getSmbServerIP() {
            return mServerIP;
        }      
    } 
    
    //public static class Tool {
        public boolean ipCheck(String text) {
            if (text != null && !text.isEmpty()) {            
                String regex = "^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\."
                             + "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                             + "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
                             + "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$";
                
                if (text.matches(regex)) {                   
                    return true;
                } else {
                    return false;
                }
            }
            return false;
        //}
    }
       
    // Debug method
    private static void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }
}
