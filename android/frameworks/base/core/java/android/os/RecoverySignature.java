/*
 * Copyright (C) 2014 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2014.04.21 : First draft
 */
 
package android.os;

import android.util.TypedValue;
import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.nio.charset.Charset;
import java.util.Arrays;

/* The conten of file should look like below: 
 * boot-recovery
 * recovery
 * --update_package=CACHE:update.zip
 *  --locale=en_GB
 *
 * --wipe_data
 * --wipe_cache
 */

/**
 * Implement this class for our purpose
 * @hide
 */
public class RecoverySignature {
	public static String TAG = "RecoverySignature";
    private byte[] command;
    private byte[] status;
    private byte[] recovery;
    private File outFile;


	public RecoverySignature(String filename) {
        if(filename.length() != 0) {
            outFile = new File(filename);
        }

        command = new byte[32];
        status = new byte[32];
        recovery = new byte[1024];   // TODO: too big?
	}

    public Boolean writeRecoverySignature(Boolean flag) {
        Log.d(TAG, "call writeRecoverySignature, flag = " + flag);
        if(flag == true) {
            try {
                BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(outFile));
                Log.d(TAG, "1) call writeRecoverySignature, flag = " + flag);
                if(bos != null) {
                    Log.d(TAG, "2) call writeRecoverySignature, flag = " + flag);
                    bos.write(command, 0, command.length);
                    bos.write(status, 0, status.length);
                    bos.write(recovery, 0, recovery.length);
                    //bos.write(command, 0, 32);
                    //bos.write(status, 0, 32);
                    //bos.write(recovery, 0, 1024);
                    bos.flush();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    
        return true;
    }

    // total 32 bytes length
    public void setRecoverySignatureCommand(String cmd) {
        Log.d(TAG, "call setRecoverySignatureCommand, cmd = " + cmd);
        //Log.d(TAG, "call setRecoverySignatureCommand, command.length = " + command.length);
        for(int i = 0; i < cmd.getBytes().length; i++) {
            command[i] = cmd.getBytes()[i];
        }
        //Log.d(TAG, "call setRecoverySignatureCommand, command = " + new String(command) + ", command.length = " + command.length);
    }

    // total 32 bytes length, so far we don't care the content
    public void setRecoverySignatureStatus(String tstatus) {
        Log.d(TAG, "call setRecoverySignatureStatus, tstatus = " + tstatus);
        //Log.d(TAG, "call setRecoverySignatureStatus, status.length = " + status.length);
        //for(int i = 0; i < tstatus.getBytes().length; i++) {
        for(int i = 0; i < 32; i++) {
            status[i] = '1';
        }
        //Log.d(TAG, "call setRecoverySignatureStatus, status = " + new String(status) + ", status.length = " + status.length);
    }
    
    // total 1024 bytes length
    public void setRecoverySignatureRecovery(String recoveryStr) {
        Log.d(TAG, "call setRecoverySignatureRecovery, recoveryStr = " + recoveryStr);
        //Log.d(TAG, "call setRecoverySignatureRecovery, recovery.length = " + recovery.length);
        for(int i = 0; i < recoveryStr.getBytes().length; i++) {
            recovery[i] = recoveryStr.getBytes()[i];
        }
        //Log.d(TAG, "call setRecoverySignatureRecovery, recovery = " + new String(recovery) + ", recovery.legnth = " + recovery.length);
    }

}


