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

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.util.Log;
import android.os.ParcelFileDescriptor;
import android.app.Activity;

/**
 * A simple subclass of {@link ImageWorker} that resizes images from resources
 * given a target width and height. Useful for when the input images might be
 * too large to simply load directly into memory.
 */
public class ImageResizer extends ImageWorker {
    private static final String TAG = "ImageWorker";
    protected int mImageWidth;
    protected int mImageHeight;

    /**
     * Initialize providing a single target image size (used for both width and
     * height);
     * 
     * @param context
     * @param loadingControl
     * @param imageWidth
     * @param imageHeight
     */
    public ImageResizer(Context context, LoadingControl loadingControl,
            int imageWidth, int imageHeight)
    {
        super(context, loadingControl);
        setImageSize(imageWidth, imageHeight);
    }

    /**
     * Initialize providing a single target image size (used for both width and
     * height);
     * 
     * @param context
     * @param loadingControl
     * @param imageSize
     */
    public ImageResizer(Context context, LoadingControl loadingControl,
            int imageSize)
    {
        super(context, loadingControl);
        setImageSize(imageSize);
    }

    /**
     * Set the target image width and height.
     * 
     * @param width
     * @param height
     */
    public void setImageSize(int width, int height) {
        mImageWidth = width;
        mImageHeight = height;
    }

    /**
     * Set the target image size (width and height will be the same).
     * 
     * @param size
     */
    public void setImageSize(int size) {
        setImageSize(size, size);
    }

    /**
     * The main processing method. This happens in a background task. In this
     * case we are just sampling down the bitmap and returning it from a
     * resource.
     * 
     * @param resId
     * @return
     */
    private Bitmap processBitmap(int resId, BitmapFactory.Options options) {
        return processBitmap(resId, mImageWidth, mImageHeight, options);
    }

    /**
     * The main processing method. This happens in a background task. In this
     * case we are just sampling down the bitmap and returning it from a
     * resource.
     * 
     * @param resId
     * @param imageWidth
     * @param imageHeight
     * @return
     */
    protected Bitmap processBitmap(int resId, int imageWidth, int imageHeight, BitmapFactory.Options options) {
        /*if (BuildConfig.DEBUG) {
            CommonUtils.debug(TAG, "processBitmap - " + resId);
        }*/
        Log.d(TAG, "processBitmap ResID - " + resId);
        return decodeSampledBitmapFromResource(
                mContext.getResources(), resId, imageWidth, imageHeight, options);
    }

    @Override
    protected Bitmap processBitmap(Object data, BitmapFactory.Options options) {
        return processBitmap(Integer.parseInt(String.valueOf(data)), options);
    }

    @Override
    protected Bitmap processBitmap(Object data, int type, BitmapFactory.Options options) {
        return processBitmap(Integer.parseInt(String.valueOf(data)), options);
    }

    /**
     * Decode and sample down a bitmap from resources to the requested width and
     * height.
     * 
     * @param res The resources object containing the image data
     * @param resId The resource id of the image data
     * @param reqWidth The requested width of the resulting bitmap
     * @param reqHeight The requested height of the resulting bitmap
     * @return A bitmap sampled down from the original with the same aspect
     *         ratio and dimensions that are equal to or greater than the
     *         requested width and height
     */
    public static Bitmap decodeSampledBitmapFromResource(Resources res, int resId,
            int reqWidth, int reqHeight, BitmapFactory.Options options) {

        // First decode with inJustDecodeBounds=true to check dimensions
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeResource(res, resId, options);

        // Calculate inSampleSize
        options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);

        // Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;
        options.inPurgeable = true;
        options.inInputShareable = true;
        options.inPreferredConfig = Bitmap.Config.RGB_565;
        return BitmapFactory.decodeResource(res, resId, options);
    }

    /**
     * Decode and sample down a bitmap from a file to the requested width and
     * height.
     * 
     * @param filename The full path of the file to decode
     * @param reqWidth The requested width of the resulting bitmap
     * @param reqHeight The requested height of the resulting bitmap
     * @return A bitmap sampled down from the original with the same aspect
     *         ratio and dimensions that are equal to or greater than the
     *         requested width and height
     */
    public static synchronized Bitmap decodeSampledBitmapFromFile(String filename,
            int reqWidth, int reqHeight, BitmapFactory.Options options) {

        // First decode with inJustDecodeBounds=true to check dimensions
        options = calculateImageSize(filename, options);

        // Calculate inSampleSize
        options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);

        // Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;
        return decode(filename,options,reqWidth,reqHeight);
    }
    
    public static synchronized Bitmap decodeSampledBitmapFromByteArray(byte[] data,
            int reqWidth, int reqHeight, BitmapFactory.Options options) {

        // First decode with inJustDecodeBounds=true to check dimensions
        options = calculateImageSize(data, options);

        // Calculate inSampleSize
        options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);

        // Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;
        options.inPurgeable = true;
        options.inInputShareable = true;
        options.inPreferredConfig = Bitmap.Config.RGB_565;
        return BitmapFactory.decodeByteArray(data, 0, data.length, options);
    }

    public static synchronized Bitmap decodeHttpImage(String url)
   	{
    	Bitmap bmp = null;
    	try
    	{   
    		URL ulrn = new URL(url);
    	    HttpURLConnection con = (HttpURLConnection)ulrn.openConnection();
    	    InputStream is = con.getInputStream();
    	    bmp = BitmapFactory.decodeStream(is);
    	}
    	catch(Exception e) 
        {
        	e.printStackTrace(); 
        } 
    	return bmp;
    	
   	}
    
    public static synchronized Bitmap decode(String path,BitmapFactory.Options options, int reqWidth, int reqHeight)
    {
        FileDescriptor fd;
        Bitmap bm = null;
        FileInputStream fis = null;

        try
        {
            int rotationInDegrees=0;
            if(path.toLowerCase().indexOf("jpeg")>=0 || path.toLowerCase().indexOf("jpg")>=0) {
                ExifInterface exif = new ExifInterface(path);
                int rotation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
                rotationInDegrees = exifToDegrees(rotation);
            }

            if(android.os.Build.VERSION.SDK_INT>=android.os.Build.VERSION_CODES.N && path.contains("rtk_media")) {
                ParcelFileDescriptor mFileDescriptor = ((Activity)mContext).getApplication().getContentResolver().openFileDescriptor(Uri.parse(path), "r");
                fd = mFileDescriptor.getFileDescriptor();
            }
            else {
                fis = new FileInputStream(path);
                fd = fis.getFD();
            }
            options.inJustDecodeBounds = false;
            options.inPreferredConfig = Bitmap.Config.RGB_565;
            bm = BitmapFactory.decodeFileDescriptor(fd, null, options);
            if(fis!=null)
                fis.close();

            if (bm == null) return null;

            // We need to resize down if the decoder does not support inSampleSize
            // (For example, GIF images)
            float scale = Math.min( (float) reqWidth /bm.getWidth() , (float) reqHeight /bm.getHeight());
            if (scale <= 0.5) bm = resizeBitmapByScale(bm, scale, true);

            if(rotationInDegrees!=0) {
                bm = rotateBitmap(bm, rotationInDegrees, true);
            }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        return bm;
    }

    public static Bitmap resizeBitmapByScale(Bitmap bitmap, float scale, boolean recycle) {
        int width = Math.round(bitmap.getWidth() * scale);
        int height = Math.round(bitmap.getHeight() * scale);
        if (width == bitmap.getWidth() && height == bitmap.getHeight()) return bitmap;
        Bitmap target = Bitmap.createScaledBitmap(bitmap, width, height, true);
        if (recycle) {
            bitmap.recycle();
            bitmap=null;
            System.gc();
        }
        return target;
    }

    public static Bitmap rotateBitmap(Bitmap bitmap, float rotationInDegrees, boolean recycle)
    {
        Matrix matrix = new Matrix();
        matrix.postRotate(rotationInDegrees);
        Bitmap bmpout = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
        if (recycle) {
            bitmap.recycle();
            bitmap=null;
            System.gc();
        }
        return bmpout;
    }

    private static int exifToDegrees(int exifOrientation) {
        if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_90) { return 90; }
        else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_180) {  return 180; }
        else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_270) {  return 270; }
        return 0;
    }

    /**
     * Calculate the image size for the given file name.
     * 
     * @param filename
     * @return
     */
    public static BitmapFactory.Options calculateImageSize(String filename, BitmapFactory.Options options) {
        options.inJustDecodeBounds = true;
        FileDescriptor fd;
        FileInputStream fis = null;
        try{
            if(android.os.Build.VERSION.SDK_INT>=android.os.Build.VERSION_CODES.N && filename.contains("rtk_media")) {
                ParcelFileDescriptor mFileDescriptor = ((Activity)mContext).getApplication().getContentResolver().openFileDescriptor(Uri.parse(filename), "r");
                fd = mFileDescriptor.getFileDescriptor();
            }
            else {
                fis = new FileInputStream(filename);
                fd = fis.getFD();
            }
            BitmapFactory.decodeFileDescriptor(fd, null, options);
            if(fis!=null)
                fis.close();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        //Log.d(TAG,"calculateImageSize :"+ filename);
        return options;
    }
	
    public static BitmapFactory.Options calculateImageSize(byte[] data, BitmapFactory.Options options) {
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeByteArray(data, 0, data.length, options);
        
        //Log.d(TAG,"calculateImageSize :");
        return options;
    }
	
    /**
     * Calculate an inSampleSize for use in a {@link BitmapFactory.Options}
     * object when decoding bitmaps using the decode* methods from
     * {@link BitmapFactory}. This implementation calculates the closest
     * inSampleSize that will result in the final decoded bitmap having a width
     * and height equal to or larger than the requested width and height. This
     * implementation does not ensure a power of 2 is returned for inSampleSize
     * which can be faster when decoding but results in a larger bitmap which
     * isn't as useful for caching purposes.
     * 
     * @param options An options object with out* params already populated (run
     *            through a decode* method with inJustDecodeBounds==true
     * @param reqWidth The requested width of the resulting bitmap
     * @param reqHeight The requested height of the resulting bitmap
     * @return The value to be used for inSampleSize
     */
    public static int calculateInSampleSize(BitmapFactory.Options options,
            int reqWidth, int reqHeight) {
        // Raw height and width of image
        final int height = options.outHeight;
        final int width = options.outWidth;
        int inSampleSize = 1;

        if (height > reqHeight || width > reqWidth) {
            if (width > height) {
                inSampleSize = Math.round((float) height / (float) reqHeight);
            } else {
                inSampleSize = Math.round((float) width / (float) reqWidth);
            }

            // This offers some additional logic in case the image has a strange
            // aspect ratio. For example, a panorama may have a much larger
            // width than height. In these cases the total pixels might still
            // end up being too large to fit comfortably in memory, so we should
            // be more aggressive with sample down the image (=larger
            // inSampleSize).

            final float totalPixels = width * height;

            // Anything more than 2x the requested pixels we'll sample down
            // further.
            final float totalReqPixelsCap = reqWidth * reqHeight * 2;

            while (totalPixels / (inSampleSize * inSampleSize) > totalReqPixelsCap) {
                inSampleSize++;
            }
        }

        // Calculates the largest inSampleSize that, is a power of two and, keeps either width or
        // height larger or equal to the requested width and height.
        inSampleSize = Math.max(1, Integer.highestOneBit(inSampleSize));
        return inSampleSize;
    }
}
