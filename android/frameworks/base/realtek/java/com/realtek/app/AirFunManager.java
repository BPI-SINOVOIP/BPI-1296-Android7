/*
 * Copyright (c) 2013 Realtek Semi. co. All Rights Reserved.
 *
 *
 * Author: 	Ben Wu
 */

package com.realtek.app;

import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.io.IOException;

public class AirFunManager {
	private static final String TAG = "AirFunManager";

    public static final int DOWNLOAD_OK = 0;    // download complete
    public static final int DOWNLOAD_NG = 1;    // download fail
    public static final int DOWNLOAD_GO = 2;    // downloading

	private final IAirFunService mAfService;

	public AirFunManager() {
		mAfService = IAirFunService.Stub.asInterface(ServiceManager.getService("airfun"));
		if(mAfService != null) {
			Log.i(TAG, mAfService.toString());
		}
		else {
			Log.e(TAG, "error! CANNOT get AirFunService!");
		}
	}

	public String[] extractMetadata(String url) {
		try {
			String[] metaData = mAfService.extractMetaData(url);
			for( int i=0; i<metaData.length; i++ ) {
				if( metaData[i].isEmpty() ) {
					metaData[i] = null;
				}
			}
			return metaData;
		} catch (RemoteException ex) {
			Log.d(TAG, "RemoteException");
			ex.printStackTrace();
		}
		return null;
	}

	public InputStream getInputStream(String url) throws FileNotFoundException{
		try{
			String filename = mAfService.openAndDownloadImage(url);
            if (filename.equals("AIRFUN.DOWNLOAD.IMAGE.NG")) {
                throw new FileNotFoundException("Fail to open and download image");
            }            
			Log.d(TAG, "====== getInputStream(" + url + "),filename(" + filename + ")");
			return new AirFunInputStream( new File(filename) );
		} catch(FileNotFoundException ex){
			//Log.d(TAG, "FileNotFoundException");
			ex.printStackTrace();
		} catch (RemoteException ex) {
			Log.d(TAG, "RemoteException");
		}

		return null;
	}

	// refer to ParcelFileDescriptor.AutoCloseInputStream
	class AirFunInputStream extends FileInputStream {
		private File mFile;

		public AirFunInputStream(File file) throws FileNotFoundException {
			super(file);
			mFile = file;
		}

		@Override
		public void close() throws IOException {
			super.close();
			Log.d(TAG, "----- AirFunInputStream, close(" + mFile.getName() + ")");
			if( mFile.exists() ) {
				Log.d(TAG, "----- File deleted");
				mFile.delete();
			}
		}

		@Override
		public int read(byte[] buffer, int byteOffset, int byteCount) throws IOException {
			try {
				switch( mAfService.getDownloadStatus() ) {
				case DOWNLOAD_GO:
					return 0;
				case DOWNLOAD_OK:
					return super.read(buffer, byteOffset, byteCount);
				case DOWNLOAD_NG:
					return -2;
				default:
					return -2;
				}
			}
			catch (RemoteException ex) {
			}

			return -1;
		}
	}
}
