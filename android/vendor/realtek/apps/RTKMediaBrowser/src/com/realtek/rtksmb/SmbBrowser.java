package com.realtek.rtksmb;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Message;

import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;
import java.io.*;

import android.os.UserHandle;
import android.text.TextUtils;
import com.realtek.Utils.Const;
import com.realtek.Utils.MimeTypes;
import com.realtek.DataProvider.DeviceFileDataProvider;
import com.realtek.DataProvider.FileFilterType;

import android.util.Log;
import jcifs.CIFSContext;
import jcifs.context.SingletonContext;
import jcifs.smb.*;

public class SmbBrowser {

    private static final String TAG = "SmbBrowser";
    private static final boolean DEBUG = true;
	private static final String RootPath = "//";
    private static final String MOUNT_ROOT = "/tmp/ramfs/mnt/";
    public static final int SMB_AUTH_EXCEPTION = 100;
    public static final int SMB_EXCEPTION = 101;
    private String mPath;
	private String mMountPath;
    private String mDomain = "?";
    private String mUser = Const.GUEST;
    private String mPassword;
    private int mQueryType = 0;
	private List<String> mSmbServerList = new ArrayList<String>();
	private List<String> mSmbDirList = new ArrayList<String>();
    private Context mContext;
    private DeviceFileDataProvider mntDataProvider = null;
    private MimeTypes mMimeTypes;
    private Handler mHandler;
    private int mFileType;

    public static final int SMB_QUERY_DOMAIN = 0;
    public static final int SMB_QUERY_SERVER = 1;
    public static final int SMB_QUERY_SHARE =  2;
    public static final int SMB_QUERY_FOLDER = 3;
    public static final int SMB_QUERY_SHARE_FAILED = -1;

    private static final String SMB_MOUNT_ACTION  = "android.intent.action.SMB_MOUNT";
    private static final String SMB_UMOUNT_ACTION  = "android.intent.action.SMB_UMOUNT";

    //Smb Command Response code. Those code should sync with MountService.java.  
    public static final int SmbMountOkay                  = 933;
    public static final int SmbMountFailed                = 934;
    public static final int SmbUmountOkay                 = 935;
    public static final int SmbUmountFailed               = 936;

    public static final int SmbAuthFailed                 = 12345;

    private final BroadcastReceiver SmbStatusReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if(mHandler==null) return;
            String action = intent.getAction();
            log("Action = " + action);
            int code = intent.getIntExtra("code" , 0);
            if (code == SmbMountOkay)
            {
                String mountPath = intent.getStringExtra("mountPath");
                log("Receive SmbMountOkay:" + mountPath);
                if (CheckSmbMount(mountPath))
                {
                    mMountPath = mountPath;
                    if (mntDataProvider != null)
                    {
                        mntDataProvider.StopQuery();// Stop Current Query. 
                    }
                    mntDataProvider =  new DeviceFileDataProvider(mountPath,
//                    mntDataProvider =  new SMBFileDataProvider2 (mSmbPath, mMountPath, mDomain, mUser, mPassword,
                    		mFileType ,
                    		-1, 0, mMimeTypes, mHandler);
                    Message message = new Message();
                    message.what = SmbMountOkay;
                    mHandler.sendMessage(message);
                }
                
            }
            else if (code == SmbMountFailed)
            {
                log("Receive SmbMountFailed");
                mQueryType = SMB_QUERY_SHARE;
                Message message = new Message();
                message.what = SmbMountFailed;
                mHandler.sendMessage(message);
            }
            else if (code == SmbUmountOkay)
            {
                log("Receive SmbUmountOkay");
                if (!CheckSmbMount(mMountPath))
                {
                    log("CheckSmbMount return false !");
                    Message message = new Message();
                    message.what = SmbUmountOkay;
                    mHandler.sendMessage(message);
                }
            }
            else if (code == SmbUmountFailed)
            {
                log("Receive SmbUmountFailed");
                Message message = new Message();
                message.what = SmbUmountFailed;
                mHandler.sendMessage(message);
            }
        }
    };
	
	
	private static SmbBrowser objSmbBrowser = null;
    private static String mSmbPath;
    public Exception lastException = null;

    public synchronized static SmbBrowser GetInstance(int fileType, Context context, MimeTypes mimetypes, Handler handler)
	{
		if (objSmbBrowser == null)
			objSmbBrowser = new SmbBrowser(fileType, context, mimetypes, handler);
        //Have to record new context,handler... because the that will be change maybe
        objSmbBrowser.mContext = context;
        objSmbBrowser.mHandler = handler;
        objSmbBrowser.mFileType= fileType;
        objSmbBrowser.mMimeTypes= mimetypes;
		return objSmbBrowser;
	}

	public static void DelInstance()
	{
	    Log.d(TAG, "DelInstance");
        if (objSmbBrowser != null)
        {
            objSmbBrowser.mContext.unregisterReceiver(objSmbBrowser.SmbStatusReceiver);
            if (objSmbBrowser.CheckSmbMount(null))
            {
            	Intent intent = new Intent(SMB_UMOUNT_ACTION)
                        .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                        .putExtra("smb_mount_cmd", true);
                objSmbBrowser.mContext.sendBroadcast(intent);
            }
        }
		objSmbBrowser = null;
		//System.gc();
	}
	
	public SmbBrowser(int fileType, Context context, MimeTypes mimetypes, Handler handler)
	{
	    mFileType = fileType;
	    mContext = context;
        mMimeTypes = mimetypes;
        mHandler = handler;
        //if the process crashed last time, umount smb when SmbBrowser was created
        /*
        if (CheckSmbMount())
        {
            Intent intent = new Intent(SMB_UMOUNT_ACTION)
                                    .putExtra("smb_mount_cmd", true);
            mContext.sendBroadcast(intent);
        }
        */
        if (mContext != null)
        {
            final IntentFilter userFilter = new IntentFilter();
            userFilter.addAction("android.rtksmb.resp_code");
            mContext.registerReceiver(SmbStatusReceiver, userFilter);
        }
	}
    /*	
	@Override
	protected void finalize() throws Throwable
	{
		//do finalization here
		Log.d(TAG, "finalize");
		super.finalize(); //not necessary if extending Object.
	} 
    */

    public int GetSmbQueryType(String path)
    {
        log("GetSmbQueryType path="+path);
        if (path.equals("smb://"))
        {
            return SMB_QUERY_DOMAIN;
        }

        String[] segments = path.substring(6).split("/");

//        if (segments.length == 1)
//            return SMB_QUERY_SERVER;
//        else

        if (segments.length == 1) {
            log("GetSmbQueryType ret SMB_QUERY_SHARE");
            return SMB_QUERY_SHARE;
        }else if (segments.length >= 2) {
            log("GetSmbQueryType ret SMB_QUERY_FOLDER");
            return SMB_QUERY_FOLDER;
        }
        return -1;
    }

    //If mountPath is null will check /tmp/ramfs/mnt instead. 
    public boolean CheckSmbMount(String mountPath)
    {
        String CheckPath = mountPath!=null? mountPath:MOUNT_ROOT;
        File file = new File("/proc/mounts");
        if (file.canRead())
        {
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(file));
                String text;
                while ((text = reader.readLine()) != null) {
                    String[] segs = text.split(" ");
                    if (segs.length > 1)
                    {
                        if (segs[1].indexOf(CheckPath) == 0)
                        {
                            reader.close();
                            Log.d(TAG,"CheckSmbMount Find SMB Mount path, return true");
                            return true;
                        }
                    }
                }
            }catch (IOException e) {
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
        log("CheckSmbMount return false");
        return false;
    }

    public void SetAuthentication(String user, String password)
    {
        if(TextUtils.isEmpty(user)){
            resetLoginInfo();
        } else {
            if(user.indexOf('\\')>0){
                int idxOfSlash = user.indexOf('\\');
                mDomain = user.substring(0, idxOfSlash);
                if(idxOfSlash>=user.length())
                    mUser = "";
                else
                    mUser = user.substring(idxOfSlash+1);
            } else {
                mUser = user;
            }
            mPassword = password;
        }
        log(String.format("SetAuthentication: %s %s", mDomain, mUser));
    }

    private static void log(String s){
        if(DEBUG)
            Log.d(TAG, s);
    }

	public synchronized int SmbQuery(String path)
	{
		if (path == null || path.equals("")) return 0;
		
		int size = 0;
        int queryType = GetSmbQueryType(path);
        Log.d(TAG,"SmbQuery path:"+path);
        Log.d(TAG,"QueryType="+queryType+ ", Previous QueryType="+mQueryType);
        if (queryType == SMB_QUERY_DOMAIN)
        {
            log("SmbQuery queryType == SMB_QUERY_DOMAIN");
            Integer num = new Integer(0);
            long handle = SmbFunction.Samba_QueryDomainList(num);
            mSmbServerList.clear();
            if (handle != 0)
            {
                for (int i = 0; i < num; i++)
                {
                    String domain = SmbFunction.Samba_GetDomainList(handle, i);
                    mSmbServerList.add(domain);
                }
                SmbFunction.Samba_freeHandle(handle);
            }
            /*
            for (int i = 0; i < mSmbServerList.size(); i++)
            {
                Log.d(TAG, "SMB_QUERY_DOMAIN: mSmbServerList[" + i + "] = " + mSmbServerList.get(i));
            }
            */
        }
        else if (queryType == SMB_QUERY_SERVER)
        {
            log("SmbQuery queryType == SMB_QUERY_SERVER");
            String[] segments = path.substring(2).split("/");
            Integer num = new Integer(0);
            long handle = SmbFunction.Samba_QueryServerList(num, segments[0]);
            mSmbServerList.clear();
//            String[] serverList = list(mPath);
//            if(serverList==null) {
//                Log.d(TAG, "Log in failed with User["+mUser+"]Password["+mPassword+"]");
//                //Log in failed, Should pass this message to up level
//                queryType = SMB_QUERY_SHARE_FAILED;
//            } else {
//                mSmbServerList.addAll(Arrays.asList(serverList));
//            }

            if (handle != 0)
            {
                for (int i = 0; i < num; i++)
                {
                    String server = SmbFunction.Samba_GetServerList(handle, i);
                    mSmbServerList.add(server);
                }
                SmbFunction.Samba_freeHandle(handle);
            }
        }
        else if (queryType == SMB_QUERY_SHARE)
        {
            log("SmbQuery queryType == SMB_QUERY_SHARE");
            mSmbPath = path;
            if (CheckSmbMount(null)) // move out from the smb share folder
            {
                Intent intent = new Intent(SMB_UMOUNT_ACTION)
                        .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                        .putExtra("smb_mount_cmd", true);
                mContext.sendBroadcast(intent);
            }

            if(true){
                mSmbDirList.clear();
//                mDomain = segments[0];
                SmbUserMgr mgr = SmbUserMgr.getInstance(mContext);
                String userInfo = mgr.getUserInfo(path);
                log("path="+path+"+userInfo="+userInfo);
                if (userInfo != null) {
                    String[] infos = userInfo.split(SmbUserMgr.USERINFO_SPLIT);
                    if(infos.length==0) {
                        resetLoginInfo();
                    } else {
                        if (infos.length >= 1)
                            mUser = infos[0];
                        if (infos.length >= 2)
                            mPassword = infos[1];
                    }
                } else {
                    resetLoginInfo();
                }
                log(mDomain+","+mUser);
                NtlmPasswordAuthenticator auth;
                CIFSContext cifsContext = null;
                try {
                    if(Const.GUEST.equals(mUser) && "".equals(mPassword)){
                        cifsContext = SmbUtils.getContext().withDefaultCredentials();
                    } else {
                        auth = new NtlmPasswordAuthenticator(mDomain, mUser, mPassword);
                        cifsContext = SmbUtils.getContext().withCredentials(auth);
                    }
                    SmbFile f = null;
                    String serverUrl = path;
                    log("serverUrl="+ serverUrl);
                    f = new SmbFile(path, cifsContext);
                    SmbFile[] listFiles = f.listFiles();
                    for (SmbFile smb : listFiles) {
                        String simpleName = smb.getName();
                        if(simpleName!=null) {
                            simpleName = simpleName.substring(0, simpleName.lastIndexOf("/"));
                            if (!simpleName.endsWith("$")) //share folder
                            {
                                log("simpleName="+simpleName);
                                mSmbDirList.add(simpleName);
                            }
                        }
                    }
                } catch (MalformedURLException e) {
                    e.printStackTrace();
                    queryType = SMB_QUERY_SHARE_FAILED;
                    lastException = e;
                } catch (SmbAuthException e){
                    e.printStackTrace();
                    queryType = SMB_QUERY_SHARE_FAILED;
                    lastException = e;
                } catch (SmbException e) {
                    e.printStackTrace();
                    queryType = SMB_QUERY_SHARE_FAILED;
                    lastException = e;
                }

            } else {
                String[] segments = path.substring(6).split("/");
//            String serverName = segments[1];
                String serverIP = "";//SmbFunction.Samba_NetBiosName2Ip(serverName);
//            if (serverIP.equals("0.0.0.0"))
//                serverIP = serverName;
                Integer num = new Integer(0);
                Integer ret = new Integer(0);

                long handle = SmbFunction.Samba_QueryServerShareList(ret, num, serverIP, mUser, mPassword, segments[0]);
                mSmbDirList.clear();
                log(String.format("Samba_QueryServerShareList ret=%x", ret));
//            String smbPath = getSmbPath(mPath);
//            String[] serverShareList = list(smbPath);
//            if(serverShareList==null){
//                Log.d(TAG, "Log in failed with["+serverIP+"] User["+mUser+"]Password["+mPassword+"]");
//                //Log in failed, Should pass this message to up level
//                queryType = SMB_QUERY_SHARE_FAILED;
//            }else{
//                mSmbDirList.addAll(Arrays.asList(serverShareList));
//            }
                if (handle != 0) {
                    for (int i = 0; i < num; i++) {
                        Integer type = new Integer(0);
                        String server = SmbFunction.Samba_GetServerShareList(handle, i, type);
                        if (type == 0 && !server.endsWith("$")) //share folder
                        {
                            mSmbDirList.add(server);
                        }
                    }
                    SmbFunction.Samba_freeHandle(handle);
                } else {
                    log(String.format("Log in failed(%x) with[%s] User[%s] Password[%s]", ret, serverIP, mUser, ""));
                    //Log in failed, Should pass this message to up level
                    queryType = SMB_QUERY_SHARE_FAILED;
                }
            }
        }
        else if (queryType == SMB_QUERY_FOLDER)
        {
            log("SmbQuery queryType == SMB_QUERY_FOLDER");
            mSmbPath = path;
            log("SMB_QUERY_FOLDER smbPath="+path);
            int subStartIdx = 1;
            if(path.startsWith("smb://")){
                subStartIdx = 6;
            } else {
                subStartIdx = 1;
            }
            String[] segments = path.substring(subStartIdx).split("/");
            if (segments.length == 2) // Move into the smb share folder
            {
                log("SMB_QUERY_FOLDER mount share and query folder");
                String serverName = new String(segments[0]);
                String shareName = new String(segments[1]);
                String smbPath = "//"+serverName+"/"+shareName;
                serverName = serverName.replaceAll("\\s+","_");
                shareName = shareName.replaceAll("\\s+","_");
                String mountPath = MOUNT_ROOT+serverName+"_"+shareName;
                if (CheckSmbMount(mountPath))
                {
                    mMountPath = mountPath;
                    if (mntDataProvider != null)
                    {
                        mntDataProvider.StopQuery();// Stop Current Query. 
                    }
                    mntDataProvider =  new DeviceFileDataProvider(mountPath,
//                    mntDataProvider =  new SMBFileDataProvider2 (smbPath, mMountPath, mDomain, mUser, mPassword,
                            mFileType ,
                    		-1, 0, mMimeTypes,mHandler);
                    Message message = new Message();
                    message.what = SmbMountOkay;
                    mHandler.sendMessage(message);
                }
                else
                {
                    CreateMountPath(mountPath);
                    log("getUserInfo mUser=" + mUser);
                    Intent intent = new Intent(SMB_MOUNT_ACTION)
                        .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                        .putExtra("smb_mount_cmd", true)
                        .putExtra("smbPath", smbPath)
                        .putExtra("mountPath", mountPath)
                        .putExtra("domain", mDomain)
                        .putExtra("user", mUser)
                        .putExtra("passwd", mPassword);
                    
                    mContext.sendBroadcast(intent);
                    log("sendBroadcast SMB_MOUNT_ACTION end");
                    mntDataProvider = null;//Clean old Data.
                }
            }
            else if (segments.length > 2)
            {
                log("SMB_QUERY_FOLDER query deep folder");
                //  /tmp/ramfs/mnt/172.21.181.120_tmp_smb
                String mntPath = mMountPath;
                for (int i = (mMountPath.substring(1).split("/")).length; i < segments.length; i++)
                {
                    mntPath = mntPath + "/"+ segments[i];
                }
                Log.d(TAG, "SMB_QUERY_FOLDER mntPath = " + mntPath);
                if (!mPath.equals(path))
                {
                    if (mntDataProvider != null)
                    {
                        mntDataProvider.StopQuery();// Stop Current Query. 
                    }
                    mntDataProvider =  new DeviceFileDataProvider(mntPath,
//                    mntDataProvider =  new SMBFileDataProvider2 (path, mMountPath, mDomain, mUser, mPassword,
							mFileType ,
							-1, 0, mMimeTypes,mHandler);
                }
                
            }
        }
        
        mQueryType = queryType;
        mPath = path;
        
        return mQueryType;


    }

    private void resetLoginInfo() {
        mDomain = "?";
        mUser = Const.GUEST;
        mPassword = "";
    }

    public boolean CreateMountPath(String mountPath)
    {
        File file = new File(mountPath);
        if (!file.isDirectory())
        {
            return file.mkdirs();
        }
        return true;
    }
    public List<String> GetSmbDirList(int queryType)
    {
//        log("GetSmbDirList queryType="+queryType);
//        log("GetSmbDirList mSmbDirList.size()="+mSmbDirList.size());
        if (queryType == SMB_QUERY_DOMAIN || queryType == SMB_QUERY_SERVER)
            return mSmbServerList;
        else 
            return mSmbDirList;
    }
    
    public int GetMntSize()
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetSize();
        }

        return 0;
    }

    public String GetMntDataAt(int index)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetDataAt(index);
        }

        return null;
    }

    public int GetMntFileTypeAt(int index)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetFileTypeAt(index);
        }

        return FileFilterType.DEVICE_FILE_NONE;
    }

    public String GetMntMimeTypeAt(int index)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetMimeTypeAt(index);
        }

        return null;
    }

    public File GetMntFileAt(int index)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetFileAt(index);
        }

        return null;
    }

    public String GetMntTitleAt(int index)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetTitleAt(index);
        }

        return null;
    }
    public void UpdateData(int Type)
    {
        if (mntDataProvider != null)
        {
            mntDataProvider.UpdateData(Type);
        }
    }
     
    public ArrayList<String> GetPlayList(int type)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetPlayList(type);
        }
        return null;
    }
    public ArrayList<String> GetNameList(int type)
    {
        if (mntDataProvider != null)
        {
            return mntDataProvider.GetNameList(type);
        }
        return null;
    }

    public synchronized static String getSmbPath(){
        return mSmbPath;
    }
}

