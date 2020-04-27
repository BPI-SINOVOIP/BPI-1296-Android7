package com.android.settings;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.storage.StorageManager;
import android.os.storage.VolumeInfo;
import android.support.v7.preference.DialogPreference;
import android.util.AttributeSet;
import android.view.View;
import com.realtek.hardware.RtkHDMIManager;
import android.util.Log;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.File;
import java.util.List;
import android.widget.Toast;
import android.view.Gravity;

import com.android.settings.CustomDialogPreference;

public class EdidSavePreference extends CustomDialogPreference {
    private static final String TAG = "EdidSavePreference";
    private static Context mContext;
    public EdidSavePreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;

    }

    @Override
    protected void onPrepareDialogBuilder(Builder builder, DialogInterface.OnClickListener listener) {
        super.onPrepareDialogBuilder(builder, listener);
        builder.setTitle(R.string.edid_save_confirm_title)
               .setMessage(R.string.edid_save_confirm_message)
               .setPositiveButton(android.R.string.ok, listener)
               .setNegativeButton(android.R.string.cancel, listener);
    }
    
    @Override
    public void onClick(DialogInterface dialog, int which) {
        RtkHDMIManager mRtkHDMIManager;
        byte[] edidRawData = null;
        FileOutputStream fo = null;
        boolean gotPath = false;
        String edidDirname = null;

        if (which == DialogInterface.BUTTON_POSITIVE) {
            final List<VolumeInfo> volumes = mContext.getSystemService(StorageManager.class).getVolumes();
            for (VolumeInfo vol : volumes) {
                if (vol.getType() == VolumeInfo.TYPE_PUBLIC && vol.isMountedWritable()) {
                    Log.d(TAG, "volume is public/writable, path=" + vol.getPath());
                    gotPath = true;
                    edidDirname = String.format("%s/Android/data/com.android.settings/", vol.getPath());
                    break;
                }
            }
            if(!gotPath || edidDirname == null) {
                Toast.makeText(mContext, R.string.usb_not_found_title, Toast.LENGTH_LONG).show();
                Log.d(TAG, "USB stick is NOT found!!");
                return;
            }

            // Call HDMI manager function to get EDID & Save it
            mRtkHDMIManager = new RtkHDMIManager();
            if(mRtkHDMIManager == null) {
                Log.d(TAG, "mRtkHDMIManager is null!");
                return;
            }

            edidRawData = mRtkHDMIManager.getEDIDRawData();
            File d = new File(edidDirname);
            if (!d.exists()) d.mkdirs();
            for(int k = 0; k < 1000; k++) {
                String edidFilename = String.format("edid%04d.bin", k);
                File f  = new File(edidDirname, edidFilename);

                if(!f.exists()) {
                    Log.d(TAG, "Save EDID to " + edidFilename);
                    try {
                        fo = new FileOutputStream(f);
                        fo.write(edidRawData);
                        Toast toast = Toast.makeText(mContext, "Save EDID to " + edidDirname + edidFilename, Toast.LENGTH_LONG);
                        toast.setGravity(Gravity.CENTER,0,0);
                        toast.show();
                    } catch (IOException e) {
                        Log.d(TAG, "Something wrong during saving edid on the USB stick! - " + e);
                        Toast toast = Toast.makeText(mContext, "Fail to save EDID -  " + e, Toast.LENGTH_LONG);
                        toast.setGravity(Gravity.CENTER,0,0);
                        toast.show();
                    } finally {
                        if (fo != null) try { fo.close(); } catch (IOException e) {}
                    }
                    break;
                }
            }
        }
    }

}

