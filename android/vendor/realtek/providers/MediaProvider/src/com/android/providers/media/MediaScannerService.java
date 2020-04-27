/* //device/content/providers/media/src/com/android/providers/media/MediaScannerService.java
**
** Copyright 2007, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License"); 
** you may not use this file except in compliance with the License. 
** You may obtain a copy of the License at 
**
**     http://www.apache.org/licenses/LICENSE-2.0 
**
** Unless required by applicable law or agreed to in writing, software 
** distributed under the License is distributed on an "AS IS" BASIS, 
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
** See the License for the specific language governing permissions and 
** limitations under the License.
*/

package com.android.providers.media;

import android.app.Service;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.IContentProvider;
import android.database.Cursor;
import android.media.IMediaScannerListener;
import android.media.IMediaScannerService;
import android.media.MediaScanner;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.FileUtils;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.PowerManager;
import android.os.Process;
import android.os.SystemProperties;
import android.os.storage.StorageManager;
import android.os.storage.StorageVolume;
import android.provider.MediaStore;
import android.util.Log;

import java.io.File;
import java.util.Arrays;
import java.util.Locale;

public class MediaScannerService extends Service implements Runnable
{
    private static final String TAG = "MediaScannerService";

    private volatile Looper mServiceLooper;
    private volatile ServiceHandler mServiceHandler;
    private PowerManager.WakeLock mWakeLock;
    private String[] mExternalStoragePaths;
    
    //[RTK Begin]
    private void openDatabase(String volumeName) {
        openDatabase(volumeName, "-");
    }

    private void openDatabase(String volumeName, String path) {
        try {
            ContentValues values = new ContentValues();
            values.put("name", volumeName);
            values.put("path", path);
            getContentResolver().insert(Uri.parse("content://media/"), values);
        } catch (IllegalArgumentException ex) {
            Log.w(TAG, "failed to open media database");
        }         
    }

    private String findMountPoint(String path) {
        for (String name : mExternalStoragePaths) {
            if (path.startsWith(name))
                return name;
        }
        return null;
    }
    //[RTK End]

    private MediaScanner createMediaScanner(String volumeName, String path) {
        //Log.d(TAG, "createMediaScanner volume " + volumeName + " path " + path);
        MediaScanner scanner = new MediaScanner(this, volumeName, path);
        Locale locale = getResources().getConfiguration().locale;
        if (locale != null) {
            String language = locale.getLanguage();
            String country = locale.getCountry();
            String localeString = null;
            if (language != null) {
                if (country != null) {
                    scanner.setLocale(language + "_" + country);
                } else {
                    scanner.setLocale(language);
                }
            }    
        }
        
        return scanner;
    }

    private void scan(String[] directories, String volumeName) {
        Uri uri = Uri.parse("file://" + directories[0]);
        // don't sleep while scanning
        mWakeLock.acquire();

        try {
            ContentValues values = new ContentValues();
            values.put(MediaStore.MEDIA_SCANNER_VOLUME, volumeName);
            //[RTK Begin]
            if (directories.length > 1) {
                values.put("path", "-");
            } else {
                values.put("path", directories[0]);
            }
            //[RTK End]
            Uri scanUri = getContentResolver().insert(MediaStore.getMediaScannerUri(), values);

            //Log.d(TAG, "send Intent.ACTION_MEDIA_SCANNER_STARTED -- uri = " + uri + ", scanUri = " + scanUri);
            sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_STARTED, uri));

            try {
                if (volumeName.equals(MediaProvider.EXTERNAL_VOLUME)) {
                    //[RTK Begin]
                    if (directories.length > 1)
                        openDatabase(volumeName);
                    else
                        openDatabase(volumeName, directories[0]);
                    //[RTK End]
                }

                MediaScanner scanner = createMediaScanner(volumeName, directories[0]);
                scanner.scanDirectories(directories);
            } catch (Exception e) {
                Log.e(TAG, "exception in MediaScanner.scan()", e);
            }

            getContentResolver().delete(scanUri, null, null);

        } finally {
            //Log.d(TAG, "send Intent.ACTION_MEDIA_SCANNER_FINISHED -- uri = " + uri);
            sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_FINISHED, uri));
            mWakeLock.release();
        }
    }
    
    @Override
    public void onCreate()
    {
        PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
        StorageManager storageManager = (StorageManager)getSystemService(Context.STORAGE_SERVICE);
        mExternalStoragePaths = storageManager.getVolumePaths();

        // Start up the thread running the service.  Note that we create a
        // separate thread because the service normally runs in the process's
        // main thread, which we don't want to block.
        Thread thr = new Thread(null, this, "MediaScannerService");
        thr.start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        while (mServiceHandler == null) {
            synchronized (this) {
                try {
                    wait(100);
                } catch (InterruptedException e) {
                }
            }
        }

        if (intent == null) {
            Log.e(TAG, "Intent is null in onStartCommand: ",
                new NullPointerException());
            return Service.START_NOT_STICKY;
        }

        Message msg = mServiceHandler.obtainMessage();
        msg.arg1 = startId;
        msg.obj = intent.getExtras();
        mServiceHandler.sendMessage(msg);

        // Try again later if we are killed before we can finish scanning.
        return Service.START_REDELIVER_INTENT;
    }

    @Override
    public void onDestroy()
    {
        // Make sure thread has started before telling it to quit.
        while (mServiceLooper == null) {
            synchronized (this) {
                try {
                    wait(100);
                } catch (InterruptedException e) {
                }
            }
        }
        mServiceLooper.quit();
    }

    public void run()
    {
        // reduce priority below other background threads to avoid interfering
        // with other services at boot time.
        Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND +
                Process.THREAD_PRIORITY_LESS_FAVORABLE);
        Looper.prepare();

        mServiceLooper = Looper.myLooper();
        mServiceHandler = new ServiceHandler();

        Looper.loop();
    }
   
    private Uri scanFile(String path, String mimeType) {
        String volumeName = MediaProvider.EXTERNAL_VOLUME;
        //[RTK Begin]
        openDatabase(volumeName, findMountPoint(path));
        //[RTK End]
        MediaScanner scanner = createMediaScanner(volumeName, path);
        try {
            // make sure the file path is in canonical form
            String canonicalPath = new File(path).getCanonicalPath();
            return scanner.scanSingleFile(canonicalPath, mimeType);
        } catch (Exception e) {
            Log.e(TAG, "bad path " + path + " in scanFile()", e);
            return null;
        }
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return mBinder;
    }
    
    public static final int MEDIASCANNERCONNECTION_DOACTION_RESCANVOLUME = 1;
    public static final int MEDIASCANNERCONNECTION_DOACTION_DELFILE = 2;
    
    private final IMediaScannerService.Stub mBinder = 
            new IMediaScannerService.Stub() {
        public void requestScanFile(String path, String mimeType, IMediaScannerListener listener)
        {
            if (false) {
                Log.d(TAG, "IMediaScannerService.scanFile: " + path + " mimeType: " + mimeType);
            }
            Bundle args = new Bundle();
            args.putString("filepath", path);
            args.putString("mimetype", mimeType);
            if (listener != null) {
                args.putIBinder("listener", listener.asBinder());
            }
            startService(new Intent(MediaScannerService.this,
                    MediaScannerService.class).putExtras(args));
        }

        public void scanFile(String path, String mimeType) {
            requestScanFile(path, mimeType, null);
        }
        
        public void rescanVolume(String volumeRootPath) {
            Log.d(TAG, "IMediaScannerService.rescanVolume  volumeRootPath: "
                    + volumeRootPath);
            try {
                // Stop MediaScanner scanning procedure....
                int nWaitTimeoutCount = 0;
                String volumeName = MediaProvider.EXTERNAL_VOLUME;
                MediaScanner scanner = createMediaScanner(volumeName, volumeRootPath);
                scanner.setForceStopProperty(true);
                while (scanner.isCurrentScanning()) {
                    if (nWaitTimeoutCount > 15)
                        break;
                    Thread.sleep(1000);
                    nWaitTimeoutCount++;
                }
                scanner.setForceStopProperty(false);
            } catch (Exception e) {
                Log.d(TAG, "rescanVolume Exception.");
            }

            Bundle args = new Bundle();
            args.putString("doAction", "Action");
            args.putInt("ActionType",
                    MEDIASCANNERCONNECTION_DOACTION_RESCANVOLUME);
            args.putString("param_volumeRootPath", volumeRootPath);
            startService(new Intent(MediaScannerService.this,
                    MediaScannerService.class).putExtras(args));
            Log.d(TAG, "IMediaScannerService.rescanVolume  startService ok.");
        }
    };

    private final class ServiceHandler extends Handler
    {
        @Override
        public void handleMessage(Message msg)
        {
            Bundle arguments = (Bundle) msg.obj;
            String filePath = arguments.getString("filepath");
            String doAction = arguments.getString("doAction");
            
            try {
                if (filePath != null) {
                    IBinder binder = arguments.getIBinder("listener");
                    IMediaScannerListener listener = 
                            (binder == null ? null : IMediaScannerListener.Stub.asInterface(binder));
                    Uri uri = null;
                    try {
                        uri = scanFile(filePath, arguments.getString("mimetype"));
                    } catch (Exception e) {
                        Log.e(TAG, "Exception scanning file", e);
                    }
                    if (listener != null) {
                        listener.scanCompleted(filePath, uri);
                    }
                } else if ((doAction != null) && (doAction.equals("Action"))) {
                    int iActionType = arguments.getInt("ActionType");
                    switch (iActionType) {
                    case MEDIASCANNERCONNECTION_DOACTION_RESCANVOLUME:

                        Log.i(TAG,
                                "Service is receive func call - MEDIASCANNERCONNECTION_DOACTION_RESCANVOLUME");
                        // Clear database -->
                        // /data/data/com.android.providers.media/databases/external.db
                        // /data/data/com.android.providers.media/databases/external-xxx.db
                        // /storage/{External-Device}/Android/data/com.android.providers.media/databases/external-xxxx.db
                        String volumeRootPath = arguments
                                .getString("param_volumeRootPath");
                        Log.i(TAG,
                                "Service is receive func call - rescan volume.  volumeRootPath:"
                                        + volumeRootPath);
                        IContentProvider mMediaProvider = getContentResolver()
                                .acquireProvider("media");
                        mMediaProvider.call(getPackageName(),
                                "clearVolumeDatabase", volumeRootPath, null);

                        // rescan the external storage device
                        Bundle args = new Bundle();
                        args.putString("volume", MediaProvider.EXTERNAL_VOLUME);
                        args.putString("path", volumeRootPath);
                        Message msgsend = new Message();
                        msgsend.obj = args;
                        handleMessage(msgsend);
                        break;
                    case MEDIASCANNERCONNECTION_DOACTION_DELFILE:
                        break;
                    default:
                        break;
                    }
                } else {
                    
                    // reset the forcestopscan flag to false 
                    String volume = arguments.getString("volume");
                    //[RTK Begin]
                    String path = arguments.getString("path");
                    MediaScanner scanner;
                    if(path==null) {
                        path="-";
                        scanner = createMediaScanner(volume, mExternalStoragePaths[0]);
                    }
                    else
                        scanner = createMediaScanner(volume, path);
                    //[RTK End]
                    scanner.setForceStopProperty(false);
                    String[] directories = null;
                    
                    if (MediaProvider.INTERNAL_VOLUME.equals(volume)) {
                        // scan internal media storage
                        directories = new String[] {
                                Environment.getRootDirectory() + "/media",
                        };
                    }
                    else if (MediaProvider.EXTERNAL_VOLUME.equals(volume)) {
                        // scan external storage volumes
                        //[RTK Begin]
                        if (path.equals("-"))
                            directories = mExternalStoragePaths;
                        else {
                            directories = new String[1];
                            directories[0] = path;
                        }
                        //[RTK End]
                    }

                    if (directories != null) {
                        if (false) Log.d(TAG, "start scanning volume " + volume + ": "
                                + Arrays.toString(directories));
                        scan(directories, volume);
                        if (false) Log.d(TAG, "done scanning volume " + volume);
                    }
                }
            } catch (Exception e) {
                Log.e(TAG, "Exception in handleMessage", e);
            }

            stopSelf(msg.arg1);
        }
    };
}

