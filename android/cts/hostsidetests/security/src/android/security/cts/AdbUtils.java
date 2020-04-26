/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.security.cts;

import com.android.tradefed.device.CollectingOutputReceiver;
import com.android.tradefed.device.DeviceNotAvailableException;
import com.android.tradefed.device.ITestDevice;
import com.android.tradefed.testtype.DeviceTestCase;

import android.platform.test.annotations.RootPermissionTest;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Scanner;

public class AdbUtils {

    /** Runs a commandline on the specified device
     *
     * @param command the command to be ran
     * @param device device for the command to be ran on
     * @return the console output from running the command
     */
    public static String runCommandLine(String command, ITestDevice device) throws Exception
    {
        return device.executeShellCommand(command);
    }

    /**
     * Pushes and runs a binary to the selected device
     *
     * @param pathToPoc a string path to poc from the /res folder
     * @param device device to be ran on
     * @return the console output from the binary
     */
    public static String runPoc(String pathToPoc, ITestDevice device) throws Exception {
        String fullResourceName = pathToPoc;
        File pocFile = File.createTempFile("poc", "");
        try {
            pocFile = extractResource(fullResourceName, pocFile);
            device.pushFile(pocFile, "/data/local/tmp/poc");
            device.executeShellCommand("chmod +x /data/local/tmp/poc");
            return device.executeShellCommand("/data/local/tmp/poc");
        } finally {
            pocFile.delete();
        }
    }

    /**
     * Pushes and installs an apk to the selected device
     *
     * @param pathToApk a string path to apk from the /res folder
     * @param device device to be ran on
     * @return the output from attempting to install the apk
     */
    public static String installApk(String pathToApk, ITestDevice device) throws Exception {

        String fullResourceName = pathToApk;
        File apkFile = File.createTempFile("apkFile", ".apk");
        try {
            apkFile = extractResource(fullResourceName, apkFile);
            return device.installPackage(apkFile, true);
        } finally {
            apkFile.delete();
        }
    }

   /**
     * Extracts the binary data from a resource and writes it to a temp file
     */
    private static File extractResource(String fullResourceName, File file) throws Exception {
        try (InputStream in = AdbUtils.class.getResourceAsStream(fullResourceName);
            OutputStream out = new BufferedOutputStream(new FileOutputStream(file))) {
            if (in == null) {
                throw new IllegalArgumentException("Resource not found: " + fullResourceName);
            }
            byte[] buf = new byte[65536];
            int chunkSize;
            while ((chunkSize = in.read(buf)) != -1) {
                out.write(buf, 0, chunkSize);
            }
            return file;
        }

    }
}
