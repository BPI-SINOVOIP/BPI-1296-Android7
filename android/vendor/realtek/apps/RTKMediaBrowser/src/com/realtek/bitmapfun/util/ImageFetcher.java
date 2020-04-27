/*
 * Copyright (C) 2012 The Android Open Source Project
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

package com.realtek.bitmapfun.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import android.widget.Toast;
import android.media.ThumbnailUtils;
import com.realtek.Utils.UseRtMediaPlayer;
import com.rtk.mediabrowser.MediaBrowser;

/**
 * A simple subclass of {@link ImageResizer} that fetches and resizes images
 * fetched from a URL.
 */
public class ImageFetcher extends ImageResizer {
    private static final String TAG = "ImageFetcher";
    private static final int HTTP_CACHE_SIZE = 10 * 1024 * 1024; // 10MB
    public static final String HTTP_CACHE_DIR = "http_download";
    private static final String IMAGE_CACHE_DIR = "images";
    private static ImageFetcher sInstance;

    public static ImageFetcher getInstance(Activity activity, LoadingControl loadingControl,
                                           int imageWidth, int imageHeight) {
        log("getInstance");
        sInstance = getInstance(activity);
        sInstance.setImageFadeIn(false);
        sInstance.setImageSize(imageWidth, imageHeight);
        sInstance.setExitTasksEarly(false);
        return sInstance;
    }

    public static ImageFetcher getInstance(Activity activity) {
        if(sInstance==null){
            sInstance = new ImageFetcher(activity, null, 0, 0);
            sInstance.setImageCache(ImageCache.findOrCreateCache(activity, IMAGE_CACHE_DIR));
        }
        return sInstance;
    }

    public static ImageFetcher getInstance() {
        return sInstance;
    }
    /**
     * Initialize providing a target image width and height for the processing
     * images.
     * 
     * @param context
     * @param loadingControl
     * @param imageWidth
     * @param imageHeight
     */
    public ImageFetcher(Context context, LoadingControl loadingControl,
            int imageWidth, int imageHeight)
    {
        super(context, loadingControl, imageWidth, imageHeight);
        init(context);
    }

    /**
     * Initialize providing a single target image size (used for both width and
     * height);
     * 
     * @param context
     * @param loadingControl
     * @param imageSize
     */
    public ImageFetcher(Context context, LoadingControl loadingControl,
            int imageSize)
    {
        super(context, loadingControl, imageSize);
        init(context);
    }

    private void init(Context context) {
        checkConnection(context);
    }

    /**
     * Simple network connection check.
     * 
     * @param context
     */
    private void checkConnection(Context context) {
    	/*
        final ConnectivityManager cm =
                (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        final NetworkInfo networkInfo = cm.getActiveNetworkInfo();
        if (networkInfo == null || !networkInfo.isConnectedOrConnecting()) {
            Toast.makeText(context, "No network connection found.", Toast.LENGTH_LONG).show();
            Log.e(TAG, "checkConnection - no connection found");
        }*/
    }

    /**
     * The main process method, which will be called by the ImageWorker in the
     * AsyncTask background thread.
     * 
     * @param data The data to load the bitmap, in this case, a regular http URL
     * @return The downloaded and resized bitmap
     */
    private Bitmap processBitmap(String data, BitmapFactory.Options options) {
        return processBitmap(data, mImageWidth, mImageHeight, options);
    }

    /**
     * The main process method, which will be called by the ImageWorker in the
     * AsyncTask background thread.
     * 
     * @param data The data to load the bitmap, in this case, a regular http URL
     * @param imageWidth
     * @param imageHeight
     * @return The downloaded and resized bitmap
     */
    protected Bitmap processBitmap(String data, int imageWidth, int imageHeight, BitmapFactory.Options options) {
        /*if (BuildConfig.DEBUG) {
            Log.d(TAG, "processBitmap - " + data);
        }*/

    	Log.d(TAG, "processBitmap - " + data);
    	
    	
    	Log.d(TAG, "processBitmap - " + data);
        if (data == null)
        {
            return null;
        }
        
        
    	if(data !=null)
    	{
    		//android.resource:// 
    		log("data is "+data);
    		String tmpStr=null,
    				httpTmpStr=null;
    		if(data.length()>19)
    			 tmpStr = data.substring(0, 19);
    		log("tmpStr is" +tmpStr);
    		
    		if(data.length()>19)
    			 httpTmpStr = data.substring(0, 7);
    		
    		log("httpTmpStr - " + httpTmpStr);
        	
    		
    		
    		if(tmpStr != null && tmpStr.equals("android.resource://"))
    		{
				return processBitmap(Integer.parseInt( data.substring(tmpStr.length(), data.length())), imageWidth,imageHeight, options);
    		}
    		else if(httpTmpStr!=null && httpTmpStr.equals("http://"))
    		{
    			bCancel = false;
    	        // Download a bitmap, write it to a file
    	        final File f = downloadBitmap(mContext, data);

    	        if (f != null) {
    	            // Return a sampled down version
    	            Bitmap bitmap = decodeSampledBitmapFromFile(f.toString(), imageWidth, imageHeight, options);
    	        
    	        	boolean deleted = f.delete();
    	        	return bitmap;
    	        }

    		}
    		else
    		{
    			 // Download a bitmap, write it to a file
    	        final File f = new File(data);

    	        if (f == null || f.exists() == false)
    	        {	
    	        	return null;
    	        }
    			// Return a sampled down version
                if(data.toLowerCase().endsWith("heic") || data.toLowerCase().endsWith("heif")) {
                    HeifReader heifReader = new HeifReader(mContext, data, null);
                    return heifReader.getThumbnail();
                }
                else {
                    return decodeSampledBitmapFromFile(data, imageWidth, imageHeight, options);
                }
    		}	
    			
    	}	
    	
        return null;
    }

    protected Bitmap processBitmap(byte[] data, int imageWidth, int imageHeight, BitmapFactory.Options options) {

		if(data !=null)
		{
			return decodeSampledBitmapFromByteArray(data, imageWidth, imageHeight, options);
		}	
		return null;
    }

    protected Bitmap processBitmap(Bitmap bitmap, int imageWidth, int imageHeight, BitmapFactory.Options options) {

		if(bitmap !=null)
		{
			//BitmapFactory.Options options = new BitmapFactory.Options();
			options.outWidth = bitmap.getWidth();
			options.outHeight = bitmap.getHeight();
			options.inSampleSize = calculateInSampleSize(options, imageWidth, imageHeight);

			int outWidth = options.outWidth/options.inSampleSize;
			int outHeight = options.outHeight/options.inSampleSize;
			return ThumbnailUtils.extractThumbnail(bitmap, outWidth, outHeight);
		}	
		return null;
    }	
	
    @Override
    protected Bitmap processBitmap(Object data, BitmapFactory.Options options) {
        return processBitmap(String.valueOf(data), options);
    }

    @Override
    protected Bitmap processBitmap(Object data, int type, BitmapFactory.Options options) {
		switch(type)
		{
			case OBJECT_TO_STRING:
				return processBitmap(String.valueOf(data), options);
			case OBJECT_TO_BYTEARRAY:
				return processBitmap((byte[])data, mImageWidth, mImageHeight, options);
			case OBJECT_TO_BITMAP:
				return processBitmap((Bitmap)data, mImageWidth, mImageHeight, options);
			default:
				return processBitmap(String.valueOf(data), options);
		}
    }
   
    /**
     * Download a bitmap from a URL, write it to a disk and return the File
     * pointer. This implementation uses a simple disk cache.
     * 
     * @param context The context to use
     * @param urlString The URL to fetch
     * @return A File pointing to the fetched bitmap
     */
    public static File downloadBitmap(Context context, String urlString) {
    	
    	Log.d(TAG, "downloadBitmap - found in http cache - " + urlString);
    	
    	if (bCancel== true) 
    		return null;
    	
        final File cacheDir = DiskLruCache.getDiskCacheDir(context, HTTP_CACHE_DIR);

        final DiskLruCache cache =
                DiskLruCache.openCache(context, cacheDir, HTTP_CACHE_SIZE);

        final File cacheFile = new File(cache.createFilePath(urlString));

        if (cache.containsKey(urlString)) {
            
        	/*
        	if (BuildConfig.DEBUG) {
                Log.d(TAG, "downloadBitmap - found in http cache - " + urlString);
            }*/
        	Log.d(TAG, "downloadBitmap - found in http cache - " + urlString);
            return cacheFile;
        }

        /*
        if (BuildConfig.DEBUG) {
            Log.d(TAG, "downloadBitmap - downloading - " + urlString);
        }*/
        
        Log.d(TAG, "downloadBitmap - downloading - " + urlString);

        Utils.disableConnectionReuseIfNecessary();
        HttpURLConnection urlConnection = null;
        BufferedOutputStream out = null;
        InputStream in = null;

        try {
            final URL url = new URL(urlString);
            urlConnection = (HttpURLConnection) url.openConnection();
            in = new BufferedInputStream(urlConnection.getInputStream(), Utils.IO_BUFFER_SIZE);
            out = new BufferedOutputStream(new FileOutputStream(cacheFile), Utils.IO_BUFFER_SIZE);

            int b;
            while ((b = in.read()) != -1) 
            {
            	if(bCancel == true)
            	{
            		out.close();
            		urlConnection.disconnect();
            		cacheFile.delete();
            		break;
            	}	
                out.write(b);
            }
            //Log.d(TAG,"return cache file:"+cacheFile.toString());
            return cacheFile;

        } catch (final IOException e) {
            Log.e(TAG, "Error in downloadBitmap - " + e);
        } finally {
            if (urlConnection != null) {
                urlConnection.disconnect();
            }
            if (out != null) {
                try {
                    out.close();
                } catch (final IOException e) {
                    Log.e(TAG, "Error in downloadBitmap - " + e);
                }
            }
            if (in != null){
                try {
                    in.close();
                } catch (final IOException e) {
                    Log.e(TAG, "Error in downloadBitmap - " + e);
                }
            }
            //Log.d(TAG,"Clean down");
        }

        return null;
    }

    public static void destroy() {
        if(sInstance!=null){
            sInstance.getImageCache().clearMemoryCache();
            sInstance = null;
        }
    }

    private static void log(String s) {
        Log.d(TAG, s);
    }

    public void clearMemoryCache() {
        if (sInstance != null && sInstance.getImageCache() != null) {
            sInstance.getImageCache().clearMemoryCache();
        }
    }
}
