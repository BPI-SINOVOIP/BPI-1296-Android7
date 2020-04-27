package com.realtek.Utils;

import android.content.Context;
import android.os.Build;
import android.os.Environment;
import android.os.storage.DiskInfo;
import android.os.storage.StorageManager;
import android.os.storage.VolumeInfo;
import android.text.TextUtils;
import android.util.Log;

import java.io.*;
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
        public final String sysPath;
        public final String uuid;
        private final String label;
        private final String desc;
        private final String volId;
        private final String TYPE_SDCARD = "98000000.sdmmc";
        private final String TYPE_USB = "/usb";
        private final String TYPE_SATA = "9803f000.sata";

        StorageInfo(String path, boolean removable, boolean isSdCard, String volId) {
            this.path = path;
            this.readonly = false;
            this.removable = removable;
            this.sysPath = isSdCard? TYPE_SDCARD:"";
            this.uuid = "";
            this.label = "";
            this.desc = "";
            this.volId = volId;
        }

        StorageInfo(String path, boolean removable, boolean isSdCard, String uuid, String label, String desc, String volId) {
            this.path = path;
            this.readonly = false;
            this.removable = removable;
            this.sysPath = isSdCard? TYPE_SDCARD:"";
            this.uuid = uuid;
            this.label = label;
            this.desc = desc;
            this.volId = volId;
        }

        public StorageInfo(String path, boolean removable, String sysPath, String uuid, String label, String desc, String volId) {
            this.path = path;
            this.readonly = false;
            this.removable = removable;
            this.sysPath = sysPath;
            this.uuid = uuid;
            this.label = label;
            this.desc = desc;
            this.volId = volId;
        }

        public String getDisplayName() {
            StringBuilder res = new StringBuilder();
            if (!removable) {
                res.append("Internal storage");
            } else if (sysPath.contains(TYPE_SDCARD)) {
                res.append("SD card ");
                if(!TextUtils.isEmpty(label)) {
                    res.append(label);
//                else if(!TextUtils.isEmpty(desc))
//                    res.append(desc);
                } else {
//                    res.append(volId);
                    if (!TextUtils.isEmpty(uuid))
                        res.append("[").append(this.uuid).append("]");
                }
            } else if (sysPath.contains(TYPE_USB)) {
                res.append("USB ");
                if (!TextUtils.isEmpty(label)) {
                    res.append(label);
//                else if(!TextUtils.isEmpty(desc))
//                    res.append(desc);
                } else {
//                    res.append(volId);
                    if (!TextUtils.isEmpty(uuid))
                        res.append("[").append(uuid).append("]");
                }
            } else if (sysPath.contains(TYPE_SATA)) {
                    res.append("SATA ");
                    if(!TextUtils.isEmpty(label)) {
                        res.append(label);
//                else if(!TextUtils.isEmpty(desc))
//                    res.append(desc);
                    } else {
//                    res.append(volId);
                        if (!TextUtils.isEmpty(uuid))
                            res.append("[").append(uuid).append("]");
                    }
            } else {
                res.append("UNKNOWN_");
                if(!TextUtils.isEmpty(label)) {
                    res.append(label);
//                else if(!TextUtils.isEmpty(desc))
//                    res.append(desc);
                } else {
//                    res.append(volId);
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
            list.add(0, new StorageInfo(def_path, def_path_removable, false, "0"));
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            StorageManager sm = ctx.getSystemService(StorageManager.class);
            List<VolumeInfo> volumeInfos = sm.getVolumes();
            for (VolumeInfo vol : volumeInfos) {
                if(vol.type==VolumeInfo.TYPE_PUBLIC
                        && (vol.state==VolumeInfo.STATE_MOUNTED || vol.state==VolumeInfo.STATE_MOUNTED_READ_ONLY)){
                    String desc = sm.getBestVolumeDescription(vol);
                    String volId = "";
                    log(vol.disk.sysPath);
                    list.add(new StorageInfo(vol.path, true, vol.disk.sysPath, vol.fsUuid, vol.fsLabel, desc, volId));
//                    log(vol.toString());
                }
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
                                list.add(new StorageInfo(mount_point, true, isSdCard, "0"));
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
                    } catch (IOException e) {
                        e.printStackTrace();
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