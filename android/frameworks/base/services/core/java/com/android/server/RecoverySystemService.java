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

package com.android.server;

import android.content.Context;
import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import android.os.IRecoverySystem;
import android.os.IRecoverySystemProgressListener;
import android.os.PowerManager;
import android.os.RecoverySystem;
import android.os.RemoteException;
import android.os.SystemProperties;
import android.system.ErrnoException;
import android.system.Os;
import android.util.Slog;

import libcore.io.IoUtils;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;

/**
 * The recovery system service is responsible for coordinating recovery related
 * functions on the device. It sets up (or clears) the bootloader control block
 * (BCB), which will be read by the bootloader and the recovery image. It also
 * triggers /system/bin/uncrypt via init to de-encrypt an OTA package on the
 * /data partition so that it can be accessed under the recovery image.
 */
public final class RecoverySystemService extends SystemService {
    private static final String TAG = "RecoverySystemService";
    private static final boolean DEBUG = false;

    // The socket at /dev/socket/uncrypt to communicate with uncrypt.
    private static final String UNCRYPT_SOCKET = "uncrypt";

    // The init services that communicate with /system/bin/uncrypt.
    private static final String INIT_SERVICE_UNCRYPT = "init.svc.uncrypt";
    private static final String INIT_SERVICE_SETUP_BCB = "init.svc.setup-bcb";
    private static final String INIT_SERVICE_CLEAR_BCB = "init.svc.clear-bcb";

    private static final int SOCKET_CONNECTION_MAX_RETRY = 30;

    private static final Object sRequestLock = new Object();

    private Context mContext;

    public RecoverySystemService(Context context) {
        super(context);
        mContext = context;
    }

    @Override
    public void onStart() {
        publishBinderService(Context.RECOVERY_SERVICE, new BinderService());
    }

    private final class BinderService extends IRecoverySystem.Stub {
        @Override // Binder call
        public boolean uncrypt(String filename, IRecoverySystemProgressListener listener) {
            if (DEBUG) Slog.d(TAG, "uncrypt: " + filename);

            synchronized (sRequestLock) {
                mContext.enforceCallingOrSelfPermission(android.Manifest.permission.RECOVERY, null);

                final boolean available = checkAndWaitForUncryptService();
                if (!available) {
                    Slog.e(TAG, "uncrypt service is unavailable.");
                    return false;
                }

                // Write the filename into UNCRYPT_PACKAGE_FILE to be read by
                // uncrypt.
                RecoverySystem.UNCRYPT_PACKAGE_FILE.delete();

                try (FileWriter uncryptFile = new FileWriter(RecoverySystem.UNCRYPT_PACKAGE_FILE)) {
                    uncryptFile.write(filename + "\n");
                } catch (IOException e) {
                    Slog.e(TAG, "IOException when writing \"" +
                            RecoverySystem.UNCRYPT_PACKAGE_FILE + "\":", e);
                    return false;
                }

                // Trigger uncrypt via init.
                SystemProperties.set("ctl.start", "uncrypt");

                // Connect to the uncrypt service socket.
                LocalSocket socket = connectService();
                if (socket == null) {
                    Slog.e(TAG, "Failed to connect to uncrypt socket");
                    return false;
                }

                // Read the status from the socket.
                DataInputStream dis = null;
                DataOutputStream dos = null;
                try {
                    dis = new DataInputStream(socket.getInputStream());
                    dos = new DataOutputStream(socket.getOutputStream());
                    int lastStatus = Integer.MIN_VALUE;
                    while (true) {
                        int status = dis.readInt();
                        // Avoid flooding the log with the same message.
                        if (status == lastStatus && lastStatus != Integer.MIN_VALUE) {
                            continue;
                        }
                        lastStatus = status;

                        if (status >= 0 && status <= 100) {
                            // Update status
                            Slog.i(TAG, "uncrypt read status: " + status);
                            if (listener != null) {
                                try {
                                    listener.onProgress(status);
                                } catch (RemoteException ignored) {
                                    Slog.w(TAG, "RemoteException when posting progress");
                                }
                            }
                            if (status == 100) {
                                Slog.i(TAG, "uncrypt successfully finished.");
                                // Ack receipt of the final status code. uncrypt
                                // waits for the ack so the socket won't be
                                // destroyed before we receive the code.
                                dos.writeInt(0);
                                break;
                            }
                        } else {
                            // Error in /system/bin/uncrypt.
                            Slog.e(TAG, "uncrypt failed with status: " + status);
                            // Ack receipt of the final status code. uncrypt waits
                            // for the ack so the socket won't be destroyed before
                            // we receive the code.
                            dos.writeInt(0);
                            return false;
                        }
                    }
                } catch (IOException e) {
                    Slog.e(TAG, "IOException when reading status: ", e);
                    return false;
                } finally {
                    IoUtils.closeQuietly(dis);
                    IoUtils.closeQuietly(dos);
                    IoUtils.closeQuietly(socket);
                }

                return true;
            }
        }

        @Override // Binder call
        public boolean clearBcb() {
            if (DEBUG) Slog.d(TAG, "clearBcb");
            synchronized (sRequestLock) {
                return setupOrClearBcb(false, null);
            }
        }

        @Override // Binder call
        public boolean setupBcb(String command) {
            if (DEBUG) Slog.d(TAG, "setupBcb: [" + command + "]");
            synchronized (sRequestLock) {
                return setupOrClearBcb(true, command);
            }
        }

        @Override // Binder call
        public void rebootRecoveryWithCommand(String command) {
            if (DEBUG) Slog.d(TAG, "rebootRecoveryWithCommand: [" + command + "]");
            synchronized (sRequestLock) {
                if (!setupOrClearBcb(true, command)) {
                    return;
                }

                // Having set up the BCB, go ahead and reboot.
                PowerManager pm = (PowerManager) mContext.getSystemService(Context.POWER_SERVICE);
                pm.reboot(PowerManager.REBOOT_RECOVERY);
            }
        }

        /**
         * Check if any of the init services is still running. If so, we cannot
         * start a new uncrypt/setup-bcb/clear-bcb service right away; otherwise
         * it may break the socket communication since init creates / deletes
         * the socket (/dev/socket/uncrypt) on service start / exit.
         */
        private boolean checkAndWaitForUncryptService() {
            for (int retry = 0; retry < SOCKET_CONNECTION_MAX_RETRY; retry++) {
                final String uncryptService = SystemProperties.get(INIT_SERVICE_UNCRYPT);
                final String setupBcbService = SystemProperties.get(INIT_SERVICE_SETUP_BCB);
                final String clearBcbService = SystemProperties.get(INIT_SERVICE_CLEAR_BCB);
                final boolean busy = "running".equals(uncryptService) ||
                        "running".equals(setupBcbService) || "running".equals(clearBcbService);
                if (DEBUG) {
                    Slog.i(TAG, "retry: " + retry + " busy: " + busy +
                            " uncrypt: [" + uncryptService + "]" +
                            " setupBcb: [" + setupBcbService + "]" +
                            " clearBcb: [" + clearBcbService + "]");
                }

                if (!busy) {
                    return true;
                }

                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    Slog.w(TAG, "Interrupted:", e);
                }
            }

            return false;
        }

        private LocalSocket connectService() {
            LocalSocket socket = new LocalSocket();
            boolean done = false;
            // The uncrypt socket will be created by init upon receiving the
            // service request. It may not be ready by this point. So we will
            // keep retrying until success or reaching timeout.
            for (int retry = 0; retry < SOCKET_CONNECTION_MAX_RETRY; retry++) {
                try {
                    socket.connect(new LocalSocketAddress(UNCRYPT_SOCKET,
                            LocalSocketAddress.Namespace.RESERVED));
                    done = true;
                    break;
                } catch (IOException ignored) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        Slog.w(TAG, "Interrupted:", e);
                    }
                }
            }
            if (!done) {
                Slog.e(TAG, "Timed out connecting to uncrypt socket");
                return null;
            }
            return socket;
        }

        private boolean setupOrClearBcb(boolean isSetup, String command) {
            mContext.enforceCallingOrSelfPermission(android.Manifest.permission.RECOVERY, null);

            //rtk bcb process
            if (!isSetup) {
                Slog.e(TAG, "Clear BCB is not supported.");
                return false;
            }

            RecoverySignature rsig = new RecoverySignature("/tmp/factory/recovery");
            rsig.setRecoverySignatureCommand(new String("boot-recovery"));
            rsig.setRecoverySignatureStatus(new String(" "));
            rsig.setRecoverySignatureRecovery(new String("recovery\n") + command);
            rsig.writeRecoverySignature(true);

			if(SystemProperties.getInt("rtk.vmx.boot",0)==1){
				Slog.d(TAG, "[OTA] vmx");
			    SystemProperties.set("ctl.start", "factory_write");

				//by judging whether flag.txt is exiting judge whether factory_write is end
				File file = new File("/cache/flag.txt");
				while(true){
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					if(!file.exists()){
						break;
					}
				}	
				
			}else{
				Slog.d(TAG, "[OTA] no vmx");
				SystemProperties.set("ctl.start", "factory_recovery");  // load & save the factory section
			}
            return true;

            /*
            final boolean available = checkAndWaitForUncryptService();
            if (!available) {
                Slog.e(TAG, "uncrypt service is unavailable.");
                return false;
            }

            if (isSetup) {
                SystemProperties.set("ctl.start", "setup-bcb");
            } else {
                SystemProperties.set("ctl.start", "clear-bcb");
            }

            // Connect to the uncrypt service socket.
            LocalSocket socket = connectService();
            if (socket == null) {
                Slog.e(TAG, "Failed to connect to uncrypt socket");
                return false;
            }

            DataInputStream dis = null;
            DataOutputStream dos = null;
            try {
                dis = new DataInputStream(socket.getInputStream());
                dos = new DataOutputStream(socket.getOutputStream());

                // Send the BCB commands if it's to setup BCB.
                if (isSetup) {
                    dos.writeInt(command.length());
                    dos.writeBytes(command);
                    dos.flush();
                }

                // Read the status from the socket.
                int status = dis.readInt();

                // Ack receipt of the status code. uncrypt waits for the ack so
                // the socket won't be destroyed before we receive the code.
                dos.writeInt(0);

                if (status == 100) {
                    Slog.i(TAG, "uncrypt " + (isSetup ? "setup" : "clear") +
                            " bcb successfully finished.");
                } else {
                    // Error in /system/bin/uncrypt.
                    Slog.e(TAG, "uncrypt failed with status: " + status);
                    return false;
                }
            } catch (IOException e) {
                Slog.e(TAG, "IOException when communicating with uncrypt:", e);
                return false;
            } finally {
                IoUtils.closeQuietly(dis);
                IoUtils.closeQuietly(dos);
                IoUtils.closeQuietly(socket);
            }

            return true;
            */
        }
    }

    private class RecoverySignature {
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
            if(flag == true) {
                try {
                    BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(outFile));
                    if(bos != null) {
                        bos.write(command, 0, command.length);
                        bos.write(status, 0, status.length);
                        bos.write(recovery, 0, recovery.length);
                        bos.flush();
                    }
                } catch (Exception e) {e.printStackTrace();}
            }
            return true;
        }

        // total 32 bytes length
        public void setRecoverySignatureCommand(String cmd) {
            for(int i = 0; i < cmd.getBytes().length; i++) {
                command[i] = cmd.getBytes()[i];
            }
        }

        // total 32 bytes length, so far we don't care the content
        public void setRecoverySignatureStatus(String tstatus) {
            for(int i = 0; i < 32; i++) {
                status[i] = '1';
            }
        }

        // total 1024 bytes length
        public void setRecoverySignatureRecovery(String recoveryStr) {
            for(int i = 0; i < recoveryStr.getBytes().length; i++) {
                recovery[i] = recoveryStr.getBytes()[i];
            }
        }

    }//end class RecoverySignature
}
