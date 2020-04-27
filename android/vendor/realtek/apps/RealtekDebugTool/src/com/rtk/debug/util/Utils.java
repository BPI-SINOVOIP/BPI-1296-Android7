package com.rtk.debug.util;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.StatFs;
import android.os.SystemProperties;
import android.text.TextUtils;
import android.util.Log;
import com.rtk.debug.SettingsFrag;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

/**
 * Created by archerho on 2015/10/2.
 */
public class Utils {
    private static final String TAG = "Utils";

    public static boolean hasNetwork(Context context){
        ConnectivityManager cm =
                (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        boolean isConnected = activeNetwork != null &&
                activeNetwork.isConnectedOrConnecting();
        return isConnected;
    }

    public static int getActiveNetworkType(Context context){
        int netType = ConnectivityManager.TYPE_NONE;
        if(!hasNetwork(context))
            return netType;
        ConnectivityManager cm =
                (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        if(activeNetwork!=null) {
            netType = activeNetwork.getType();
        }
        return netType;
    }

    public static void exec(String[] cmds){
        try {
            Runtime.getRuntime().exec(cmds);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void execWaitFor(String[] cmds){
        try {
            Process p = Runtime.getRuntime().exec(cmds);
            p.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String mkdirs(String dir){
        File f = new File(dir);
        if(!f.exists())
            f.mkdirs();
        return f.toString();
    }

    public static String getSavedLogsDirectory(Context ctx) {
        String catlogDir = getRtkDumpDirectory(ctx);
        if(TextUtils.isEmpty(catlogDir)) return "";
        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd");
        File savedLogsDir = new File(catlogDir, dateFormat.format(date));

        if (!savedLogsDir.exists()) {
            savedLogsDir.mkdir();
        }
        return savedLogsDir.toString();
    }

    private static long getDirFreeSize(File dir){
        StatFs stat = new StatFs(dir.getPath());
        long sdAvailSize = stat.getAvailableBlocksLong() * stat.getBlockSizeLong();
        //One binary megabyte equals 1048576 bytes.
        long megaAvailable = sdAvailSize / 1048576;
        return megaAvailable;
    }

    public static String getMostFreeSpaceVolumeId(Context ctx){
        List<StorageUtils.StorageInfo> storageInfoList = StorageUtils.getStorageList(ctx);
        long maxFreeSize = 0;
        String maxSpaceVolId = "";
        for(StorageUtils.StorageInfo info : storageInfoList) {
            //ignore internal storage
            if(info.path.contains("/storage/emulated")) continue;
            Log.d(TAG, "storageList=" + info.path);
            long size = getDirFreeSize(new File(info.path));
            Log.d(TAG, "getDirFreeSize="+size+" MB");
            if(size>maxFreeSize){
                maxFreeSize = size;
                maxSpaceVolId = info.volId;
            }
        }
        return maxSpaceVolId;
    }

    public static boolean hasExtStorage(Context ctx){
        List<StorageUtils.StorageInfo> storageInfoList = StorageUtils.getStorageList(ctx);
        for(StorageUtils.StorageInfo info : storageInfoList) {
            //ignore internal storage
            if (!info.path.contains("/storage/emulated")) return true;
        }
        return false;
    }

    private static String getMostFreeSpaceDir(Context ctx){
        List<StorageUtils.StorageInfo> storageInfoList = StorageUtils.getStorageList(ctx);
        long maxFreeSize = 0;
        String maxFreeSizeDir = "";
        for(StorageUtils.StorageInfo info : storageInfoList) {
            //ignore internal storage
            if(info.path.contains("/storage/emulated")) continue;
            Log.d(TAG, "storageList=" + info.path);
            long size = getDirFreeSize(new File(info.path));
            Log.d(TAG, "getDirFreeSize="+size+" MB");
            if(size>maxFreeSize){
                maxFreeSize = size;
                maxFreeSizeDir = info.path;
            }
        }
        return maxFreeSizeDir;
    }

    private static final String RTK_DUMP_DIR = "rtk_dump";
    private static String getRtkDumpDirectory(Context ctx) {
        String mostFreeSpaceFolder = getMostFreeSpaceDir(ctx);
        if(TextUtils.isEmpty(mostFreeSpaceFolder)) return "";
        File catlogDir = new File(mostFreeSpaceFolder, RTK_DUMP_DIR);

        if (!catlogDir.exists()) {
            catlogDir.mkdir();
        }
        return catlogDir.toString();
    }

    public static String getApkVersion(Context ctx){
        PackageInfo pInfo = null;
        try {
            pInfo = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        if(pInfo==null) return "";
        return pInfo.versionName;
//        try{
//            ApplicationInfo ai = ctx.getPackageManager().getApplicationInfo(ctx.getPackageName(), 0);
//            ZipFile zf = new ZipFile(ai.sourceDir);
//            ZipEntry ze = zf.getEntry("classes.dex");
//            long time = ze.getTime();
//            SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd_HHmmss");
//            String s = sdf.format(new java.util.Date(time));
//            zf.close();
//            return s;
//        }catch(Exception e){
//        }
//        return "";
    }

    public static String getLogFilePath(String tag){
        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String filename = dateFormat.format(date)+".log";
        String logFolder = SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+ tag;
        File f = new File(logFolder);
        if(!f.exists()) f.mkdirs();

        String logFilePath = String.format("%s/%s_%s", logFolder, tag, filename);
        f = new File(logFilePath);
        while(f.exists()) {
            logFilePath = logFilePath + "-1";
            f = new File(logFilePath);
        }
        return logFilePath;
    }

    // If targetLocation does not exist, it will be created.
    public static void copyDirectory(File sourceLocation , File targetLocation) throws IOException {
        if (sourceLocation.isDirectory() && !isDirEmpty(sourceLocation)) {
            if (!targetLocation.exists() && !targetLocation.mkdirs()) {
                throw new IOException("Cannot create dir " + targetLocation.getAbsolutePath());
            }

            String[] children = sourceLocation.list();
            for (int i=0; i<children.length; i++) {
                copyDirectory(new File(sourceLocation, children[i]),
                        new File(targetLocation, children[i]));
            }
        } else {

            // make sure the directory we plan to store the recording in exists
            File directory = targetLocation.getParentFile();
            if (directory != null && !directory.exists() && !directory.mkdirs()) {
                throw new IOException("Cannot create dir " + directory.getAbsolutePath());
            }

            if(sourceLocation.isDirectory()) return;
            InputStream in = new BufferedInputStream(new FileInputStream(sourceLocation));
            OutputStream out = new BufferedOutputStream(new FileOutputStream(targetLocation));

            // Copy the bits from instream to outstream
            byte[] buf = new byte[1024];
            int len;
            while ((len = in.read(buf)) > 0) {
                out.write(buf, 0, len);
            }
            in.close();
            out.close();
        }
    }

    public static void emptyFolder(File folder) {
        File[] files = folder.listFiles();
        if(files!=null) { //some JVMs return null for empty dirs
            for(File f: files) {
                if(f.isDirectory()) {
                    emptyFolder(f);
                } else {
                    f.delete();
                }
            }
        }
//        folder.delete();
    }

    private static boolean isDirEmpty(final File directory) throws IOException {
        File[] contents = directory.listFiles();
        return contents!=null && contents.length==0;
    }

    public static void zipFolder(String inputFolderPath, String outZipPath) {
        try (FileOutputStream fos = new FileOutputStream(outZipPath);
             ZipOutputStream zos = new ZipOutputStream(fos)){
            File srcFile = new File(inputFolderPath);
            File[] files = srcFile.listFiles();
            Log.d(TAG, "Zip directory: " + srcFile.getName());
            for (int i = 0; i < files.length; i++) {
                Log.d(TAG, "Adding file: " + files[i].getName());
                byte[] buffer = new byte[1024];
                FileInputStream fis = new FileInputStream(files[i]);
                zos.putNextEntry(new ZipEntry(files[i].getName()));
                int length;
                while ((length = fis.read(buffer)) > 0) {
                    zos.write(buffer, 0, length);
                }
                zos.closeEntry();
            }
        } catch (IOException ioe) {
            Log.e(TAG, ioe.getMessage());
        }
    }
}
