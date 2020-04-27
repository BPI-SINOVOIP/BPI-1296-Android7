package com.rtk.debug.util;

import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.os.storage.StorageManager;
import android.os.storage.VolumeInfo;
import android.text.TextUtils;
import android.util.Log;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

/**
 * Created by archerho on 2015/10/2.
 */
public class StorageUtils {
    private static final boolean DEBUG = true;
    private static final String TAG = "StorageUtils";

    public static class StorageInfo {

        public final String path;
        public final boolean readonly;
        public final boolean removable;
        public final boolean isSdCard;
        public final String uuid;
        private final String label;
        private final String desc;
        private static int usbCount=0;
        private final int usbNo;
        public final String volId;

        StorageInfo(String volId, String path, boolean removable, boolean isSdCard, int usbNo) {
            this.volId = volId;
            this.path = path;
            this.readonly = false;
            this.removable = removable;
            this.isSdCard = isSdCard;
            this.uuid = "";
            this.label = "";
            this.desc = "";
            this.usbNo = usbNo;
        }

        StorageInfo(String volId, String path, boolean removable, boolean isSdCard, String uuid, String label, String desc, int usbNo) {
            this.volId = volId;
            this.path = path;
            this.readonly = false;
            this.removable = removable;
            this.isSdCard = isSdCard;
            this.uuid = uuid;
            this.label = label;
            this.desc = desc;
            this.usbNo = usbNo;
        }

        public String getDisplayName() {
            StringBuilder res = new StringBuilder();
            if (!removable) {
                res.append("Internal storage");
            } else if (isSdCard) {
                if(!TextUtils.isEmpty(label)) {
                    res.append(label);
//                else if(!TextUtils.isEmpty(desc))
//                    res.append(desc);
                } else {
                    res.append("SD Card");
                    if (!TextUtils.isEmpty(uuid))
                        res.append(" [").append(this.uuid).append("]");
                }
            } else {
                if(!TextUtils.isEmpty(label)) {
                    res.append(label);
//                else if(!TextUtils.isEmpty(desc))
//                    res.append(desc);
                }else {
                    res.append("USB_").append(usbNo);
                    if (!TextUtils.isEmpty(uuid))
                        res.append(" [").append(uuid).append("]");
                }
            }
            if (readonly) {
                res.append(" (Read only)");
            }
            return res.toString();
        }
    }

    public static List<StorageInfo> getStorageList(Context ctx) {

        List<StorageInfo> list = new ArrayList<StorageInfo>();
        String def_path = Environment.getExternalStorageDirectory().getPath();
        boolean def_path_removable = Environment.isExternalStorageRemovable();
        String def_path_state = Environment.getExternalStorageState();
        boolean def_path_available = def_path_state.equals(Environment.MEDIA_MOUNTED)
                || def_path_state.equals(Environment.MEDIA_MOUNTED_READ_ONLY);
        boolean def_path_readonly = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED_READ_ONLY);

        HashSet<String> paths = new HashSet<String>();
        int usbCounter = 0;
        if (def_path_available) {
            paths.add(def_path);
            list.add(0, new StorageInfo("", def_path, def_path_removable, false, 0));
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            StorageManager sm = ctx.getSystemService(StorageManager.class);
            List<VolumeInfo> volumeInfos = sm.getVolumes();
            for (VolumeInfo vol : volumeInfos) {
                if(vol.type==VolumeInfo.TYPE_PUBLIC
                        && (vol.state==VolumeInfo.STATE_MOUNTED)){
                    String desc = sm.getBestVolumeDescription(vol);
                    boolean isSdCard = desc.toLowerCase().contains("sd");
                    list.add(new StorageInfo(vol.id, vol.path, true, isSdCard, vol.fsUuid, vol.fsLabel, desc, isSdCard?0:usbCounter++));
                }
                log(vol.toString());
            }
            return list;
        } else {
            BufferedReader buf_reader = null;
            try {
                String line;
                buf_reader = new BufferedReader(new FileReader("/proc/mounts"));

                while ((line = buf_reader.readLine()) != null) {
                    if (DEBUG) Log.d(TAG, line);
                    if (line.contains("/dev/fuse")) {
                        StringTokenizer tokens = new StringTokenizer(line, " ");
                        String unused = tokens.nextToken(); //device
                        String mount_point = tokens.nextToken(); //mount point
                        if (paths.contains(mount_point)) {
                            continue;
                        }
                        unused = tokens.nextToken(); //file system
                        List<String> flags = Arrays.asList(tokens.nextToken().split(",")); //flags
                        boolean readonly = flags.contains("ro");

                        if (line.contains("/storage")) {
                            if (!line.contains("/emulated")) {
                                boolean isSdCard = line.contains("mmcblk");
                                paths.add(mount_point);
                                list.add(new StorageInfo("", mount_point, true, isSdCard, isSdCard?0:usbCounter++));
                            }
                        }
                    }
                }

            } catch (FileNotFoundException ex) {
                ex.printStackTrace();
            } catch (IOException ex) {
                ex.printStackTrace();
            } finally {
                if (buf_reader != null) {
                    try {
                        buf_reader.close();
                    } catch (IOException ex) {
                    }
                }
            }
            return list;
        }
    }

    private static void log(String s) {
        if (DEBUG) Log.d(TAG, s);
    }
}