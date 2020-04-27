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

import java.lang.ref.WeakReference;
import java.util.HashMap;

import com.rtk.mediabrowser.R;
import com.realtek.DataProvider.FileFilterType;

//import android.annotation.TargetApi;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.TransitionDrawable;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.ImageView;
import android.media.MediaMetadataRetriever;

import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.MediaStore;

/**
 * This class wraps up completing some arbitrary long running work when loading
 * a bitmap to an ImageView. It handles things like using a memory and disk
 * cache, running the work in a background thread and setting a placeholder
 * image.
 */
public abstract class ImageWorker {
    private static final boolean DEBUG = true;
    private static final String TAG = "ImageWorker";
    private static final int FADE_IN_TIME = 200;

    public static final int OBJECT_TO_STRING = 1;
    public static final int OBJECT_TO_BYTEARRAY = 2;
    public static final int OBJECT_TO_BITMAP = 3;


    private ImageCache mImageCache=null;
    private Bitmap mLoadingBitmap;
    private boolean mFadeInBitmap = true;
    private boolean mExitTasksEarly = false;
    private boolean misBusyOnVideoframeDecode = false;
    private Object lock = new Object();

    protected Context mContext;
    protected ImageWorkerAdapter mImageWorkerAdapter;
    protected LoadingControl loadingControl;
    
    protected static boolean bCancel=false;

    protected ImageWorker(Context context, LoadingControl loadingControl)
    {
        mContext = context;
        this.loadingControl = loadingControl;
    }

    /**
     * Load an image specified by the data parameter into an ImageView (override
     * {@link ImageWorker#processBitmap(Object)} to define the processing
     * logic). A memory and disk cache will be used if an {@link ImageCache} has
     * been set using {@link ImageWorker#setImageCache(ImageCache)}. If the
     * image is found in the memory cache, it is set immediately, otherwise an
     * {@link AsyncTask} will be created to asynchronously load the bitmap.
     * 
     * @param data The URL of the image to download.
     * @param imageView The ImageView to bind the downloaded image to.
     */
    public void loadImage(Object data, ImageView imageView)
    {
        String path = String.valueOf(data);
//        Picasso.with(mContext).load(path).into(imageView);
        loadImage(data, imageView, loadingControl);
    }

    /**
     * Load an image specified by the data parameter into an ImageView (override
     * {@link ImageWorker#processBitmap(Object)} to define the processing
     * logic). A memory and disk cache will be used if an {@link ImageCache} has
     * been set using {@link ImageWorker#setImageCache(ImageCache)}. If the
     * image is found in the memory cache, it is set immediately, otherwise an
     * {@link AsyncTask} will be created to asynchronously load the bitmap.
     * 
     * @param data The URL of the image to download.
     * @param imageView The ImageView to bind the downloaded image to.
     * @param loadingControl the loading control.
     */
    public void loadImage(Object data, ImageView imageView, LoadingControl loadingControl)
    {
        Bitmap bitmap = null;
        
        Log.d(TAG,"IW:loadImage[1]: path:"+String.valueOf(data));
        if (mImageCache != null) {
            bitmap = mImageCache.getBitmapFromMemCache(String.valueOf(data));
        }

        if (bitmap != null) {
            Log.d(TAG,"IW:Bitmap found in memory cache");
            imageView.setImageBitmap(bitmap);
        } else if (cancelPotentialWork(data, imageView)) {
            final BitmapWorkerTask task = new BitmapWorkerTask(imageView, loadingControl);
            
            //Log.d("GridViewAdapter","IW:loadImage task  : "+ task.toString());
            
            final AsyncDrawable asyncDrawable =
                    new AsyncDrawable(mContext.getResources(), mLoadingBitmap, task);
            imageView.setImageDrawable(asyncDrawable);
            task.execute(data);
        }
    }

    public void loadImage(Object data, ImageView imageView, LoadingControl loadingControl, int type)
    {
        Bitmap bitmap = null;
		
        Log.d(TAG,"IW:loadImage[2]: path:"+String.valueOf(data));
        if (mImageCache != null) {
            bitmap = mImageCache.getBitmapFromMemCache(String.valueOf(data));
        }

        if (bitmap != null) {
            // Bitmap found in memory cache
            imageView.setImageBitmap(bitmap);
        } else if (cancelPotentialWork(data, imageView)) {
            final BitmapWorkerTask task = new BitmapWorkerTask(imageView, loadingControl, type);
            
            //Log.d("GridViewAdapter","IW:loadImage task  : "+ task.toString());
            
            final AsyncDrawable asyncDrawable =
                    new AsyncDrawable(mContext.getResources(), mLoadingBitmap, task);
            imageView.setImageDrawable(asyncDrawable);
            task.execute(data);
        }
    }
    
    //zzh add
    public void loadImage(int position,int totalCnt,Object data, ImageView imageView, LoadingControl loadingControl)
    {
        Bitmap bitmap = null;
        
         if (mImageCache != null) {
            bitmap = mImageCache.getBitmapFromMemCache(String.valueOf(data));
        }

        if (bitmap != null) {
            // Bitmap found in memory cache
            imageView.setImageBitmap(bitmap);
        } else if (cancelPotentialWork(data, imageView)) {
            final BitmapWorkerTask task = new BitmapWorkerTask(position,totalCnt,imageView, loadingControl);
            
              
            final AsyncDrawable asyncDrawable =
                    new AsyncDrawable(mContext.getResources(), mLoadingBitmap, task);
            imageView.setImageDrawable(asyncDrawable);
            task.execute(data);
        }
    }

    public void recycleOldBitmap(Object data, ImageView imageView) {
        Drawable drawable = imageView.getDrawable();
        if (drawable != null &&
                drawable instanceof BitmapDrawable
                && !(drawable instanceof AsyncDrawable))
        {
            if (mImageCache != null
                    && mImageCache.getBitmapFromMemCache(String.valueOf(data)) != null)
            {
                return;
            }
            Bitmap bitmapToRecycle = ((BitmapDrawable) drawable).getBitmap();
            imageView.setImageDrawable(null);
            bitmapToRecycle.recycle();
            //System.gc();
        }
    }

    /**
     * Load an image specified from a set adapter into an ImageView (override
     * {@link ImageWorker#processBitmap(Object)} to define the processing
     * logic). A memory and disk cache will be used if an {@link ImageCache} has
     * been set using {@link ImageWorker#setImageCache(ImageCache)}. If the
     * image is found in the memory cache, it is set immediately, otherwise an
     * {@link AsyncTask} will be created to asynchronously load the bitmap.
     * {@link ImageWorker#setAdapter(ImageWorkerAdapter)} must be called before
     * using this method.
     * 
     * @param data The URL of the image to download.
     * @param imageView The ImageView to bind the downloaded image to.
     */
    public void loadImage(int num, ImageView imageView) {
        if (mImageWorkerAdapter != null) {
            loadImage(mImageWorkerAdapter.getItem(num), imageView);
        } else {
            throw new NullPointerException("Data not set, must call setAdapter() first.");
        }
    }
    
    
     /**
     * Set placeholder bitmap that shows when the the background thread is
     * running.
     * 
     * @param bitmap
     */
    public void setLoadingImage(Bitmap bitmap) {
        mLoadingBitmap = bitmap;
    }

    /**
     * Set placeholder bitmap that shows when the the background thread is
     * running.
     * 
     * @param resId
     */
    public void setLoadingImage(int resId) {
        mLoadingBitmap = BitmapFactory.decodeResource(mContext.getResources(), resId);
    }

    /**
     * Set the {@link ImageCache} object to use with this ImageWorker.
     * 
     * @param cacheCallback
     */
    public void setImageCache(ImageCache cacheCallback) {
        mImageCache = cacheCallback;
    }

    public ImageCache getImageCache() {
        return mImageCache;
    }

    /**
     * If set to true, the image will fade-in once it has been loaded by the
     * background thread.
     * 
     * @param fadeIn
     */
    public void setImageFadeIn(boolean fadeIn) {
        mFadeInBitmap = fadeIn;
    }

    public void setExitTasksEarly(boolean exitTasksEarly) {
        mExitTasksEarly = exitTasksEarly;
    }
    public boolean isBusyOnVideoframeDecode(){
        boolean rlt = false;
        synchronized(lock){
            rlt = misBusyOnVideoframeDecode;
        } 
        return rlt;
    }

    /**
     * Subclasses should override this to define any processing or work that
     * must happen to produce the final bitmap. This will be executed in a
     * background thread and be long running. For example, you could resize a
     * large bitmap here, or pull down an image from the network.
     * 
     * @param data The data to identify which image to process, as provided by
     *            {@link ImageWorker#loadImage(Object, ImageView)}
     * @return The processed bitmap
     */
    protected abstract Bitmap processBitmap(Object data, BitmapFactory.Options options);
    protected abstract Bitmap processBitmap(int resId, BitmapFactory.Options options);
    protected abstract Bitmap processBitmap(Object data, int type, BitmapFactory.Options options);

    public static void cancelWork(ImageView imageView) {
        
        Log.d(TAG,"IW:cancelWork imageview  :"+ imageView.toString());
        
        final BitmapWorkerTask bitmapWorkerTask = getBitmapWorkerTask(imageView);
         
        if (bitmapWorkerTask != null) {
            if(bitmapWorkerTask.mOptions!=null) {
                Log.d(TAG, "requestCancelDecode");
                bitmapWorkerTask.mOptions.requestCancelDecode();
            }
            bitmapWorkerTask.cancel(true);
            imageView.setImageDrawable(null);
            bCancel = true;
            final Object bitmapData = bitmapWorkerTask.data;
            Log.d(TAG, "cancelWork - cancelled work for " + bitmapData);
            /*if (BuildConfig.DEBUG) {
                final Object bitmapData = bitmapWorkerTask.data;
                CommonUtils.debug(TAG, "cancelWork - cancelled work for " + bitmapData);
            }*/
        }
    }

    /**
     * Returns true if the current work has been canceled or if there was no
     * work in progress on this image view. Returns false if the work in
     * progress deals with the same data. The work is not stopped in that case.
     */
    public static boolean cancelPotentialWork(Object data, ImageView imageView) {
        final BitmapWorkerTask bitmapWorkerTask = getBitmapWorkerTask(imageView);

        //Log.d(TAG,"cancelPotentialWork: imageView: "+imageView.toString());
        
           
        if (bitmapWorkerTask != null) {
            final Object bitmapData = bitmapWorkerTask.data;
            if (bitmapData == null || !bitmapData.equals(data)) {
                bitmapWorkerTask.cancel(true);
                //Log.d(TAG, "cancelPotentialWork - cancelled work for " + data);
              
            } else {
                // The same work is already in progress.
                return false;
            }
        }
        return true;
    }

    /**
     * @param imageView Any imageView
     * @return Retrieve the currently active work task (if any) associated with
     *         this imageView. null if there is no such task.
     * get ImageView's drawable -->check this drawable is AsyncDrawable --> If it's, get it's work task .
     */
    private static BitmapWorkerTask getBitmapWorkerTask(ImageView imageView) {
        if (imageView != null) {
            final Drawable drawable = imageView.getDrawable();
            if (drawable instanceof AsyncDrawable) {
                final AsyncDrawable asyncDrawable = (AsyncDrawable) drawable;
                return asyncDrawable.getBitmapWorkerTask();
            }
        }
        return null;
    }

    /**
     * The actual AsyncTask that will asynchronously process the image.
     */
    private class BitmapWorkerTask extends AsyncTask<Object, Void, Bitmap> {
        private Object data;
        private int position=0;
        private int totalCnt=0;
        private int type=FileFilterType.DEVICE_FILE_PHOTO;	
        private final WeakReference<ImageView> imageViewReference;
        public BitmapFactory.Options mOptions=null;
        LoadingControl loadingControl;
        //zzh add
        public BitmapWorkerTask(int position, int totalCnt,ImageView imageView, LoadingControl loadingControl)
        {
            imageViewReference = new WeakReference<ImageView>(imageView);
            this.totalCnt=totalCnt;
            this.position=position;
            this.loadingControl = loadingControl;
        }
        
        public BitmapWorkerTask(ImageView imageView, LoadingControl loadingControl)
        {
            imageViewReference = new WeakReference<ImageView>(imageView);
            this.loadingControl = loadingControl;
        }
     
        public BitmapWorkerTask(ImageView imageView, LoadingControl loadingControl, int type)
        {
            imageViewReference = new WeakReference<ImageView>(imageView);
            this.loadingControl = loadingControl;
            this.type = type;
        }

        @Override
        protected void onPreExecute()
        {
            super.onPreExecute();
            //Log.d(TAG,"oppreexecute ImageView:"+imageViewReference.toString());
            startLoading();
        }

        public void startLoading()
        {
            if (loadingControl != null)
            {
                loadingControl.startLoading();
            }
        }

        public void stopLoading()
        {
            if (loadingControl != null)
            {
                loadingControl.stopLoading();
            }
        }

        @Override
        protected void onCancelled()
        {
            super.onCancelled();
            stopLoading();
        }

        /**
         * Background processing.
         */
        @Override
        protected Bitmap doInBackground(Object... params) {
          
        	data = params[0];
            //Log.d(TAG,"doInBackground [Begin]ImageView:"+imageViewReference.toString());
            
           
            		
            final String dataString = String.valueOf(data);
            Bitmap bitmap = null;

            // If the image cache is available and this task has not been
            // cancelled by another
            // thread and the ImageView that was originally bound to this task
            // is still bound back
            // to this task and our "exit early" flag is not set then try and
            // fetch the bitmap from
            // the cache
             
            if (mImageCache != null && !isCancelled() && getAttachedImageView() != null
                    && !mExitTasksEarly) {
                bitmap = mImageCache.getBitmapFromMemCache(dataString);
            }

            // If the bitmap was not found in the cache and this task has not
            // been cancelled by
            // another thread and the ImageView that was originally bound to
            // this task is still
            // bound back to this task and our "exit early" flag is not set,
            // then call the main
            // process method (as implemented by a subclass)
            if (bitmap == null && !isCancelled() && getAttachedImageView() != null
                    && !mExitTasksEarly)
            {
                Log.d(TAG,"IM:doInBackground: do ProcessBitmap:"+String.valueOf(params[0]));
				if(type == FileFilterType.DEVICE_FILE_AUDIO)
				{
					bitmap = GetID3BitmapViaMediaMetadataRetriever(String.valueOf(params[0]));
				}
				else if(type == FileFilterType.DEVICE_FILE_VIDEO)
				{
                    String FileName = String.valueOf(params[0]);
                    if(FileName.length()>7 && !(FileName.startsWith("http://"))
                       && !isCancelled() && !mExitTasksEarly) { //Only decode local video.
                        Log.d(TAG,"IM:GetVideoThumbnailViaDB");
                        bitmap = GetVideoThumbnailViaDB(FileName);
                    }
                    if (bitmap == null && !isCancelled() && !mExitTasksEarly) {
                        Log.d(TAG,"IM:GetVideoThumbnailViaMediaMetadataRetriever");
					    bitmap = GetVideoThumbnailViaMediaMetadataRetriever(String.valueOf(params[0]));
                    }   
                    if (bitmap == null && !isCancelled() && !mExitTasksEarly) {//For save system loading, add default icon to cache
                        Log.d(TAG,"IM:process default preview bitmap");
                        mOptions = new BitmapFactory.Options();
					    bitmap = processBitmap(R.drawable.icon_default_preview, mOptions);
                    }
				}
				else	
				{
                    mOptions = new BitmapFactory.Options();
					bitmap = processBitmap(params[0], mOptions);
				}
            }

            // If the bitmap was processed and the image cache is available,
            // then add the processed
            // bitmap to the cache for future use. Note we don't check if the
            // task was cancelled
            // here, if it was, and the thread is still running, we may as well
            // add the processed
            // bitmap to our cache as it might be used again in the future
            if (bitmap != null && mImageCache != null) {
                mImageCache.addBitmapToCache(dataString, bitmap);
            }

            //Log.d(TAG,"doInBackground [End]ImageView:"+imageViewReference.toString());
            return bitmap;
        }

        /**
         * Once the image is processed, associates it to the imageView
         */
        @Override
        protected void onPostExecute(Bitmap bitmap) {
            
            //Log.d(TAG,"onPostExecute ImageView:"+imageViewReference.toString());
            
            stopLoading();
            // if cancel was called on this task or the "exit early" flag is set
            // then we're done
            if (isCancelled() || mExitTasksEarly) {
                
                bitmap = null;
            }

            final ImageView imageView = getAttachedImageView();
            if (bitmap != null && imageView != null) {
                  setImageBitmap(imageView, bitmap);
                
                //zzh add for setting the first image a selected background
                //use "position"&"totalCnt"to judge which is the first image
                //these two params comes from PhotoPlayerActivity,through a series of functions
                if(totalCnt!=0)
                {
	                if(position%totalCnt==0)
					{
						if(position==totalCnt*100)
						{
							//imageView.setBackgroundResource(R.drawable.selectdbkgd);
							imageView.setFocusable(true);
							imageView.setFocusableInTouchMode(true);
							imageView.requestFocus();
						} 
					}
                }
                
            }
            //zzh add
            else if(imageView != null && type != FileFilterType.DEVICE_FILE_AUDIO)
            {
            	imageView.setImageResource(R.drawable.icon_default_preview);
            }
        }

        /**
         * Returns the ImageView associated with this task as long as the
         * ImageView's task still points to this task as well. Returns null
         * otherwise.
         */
        private ImageView getAttachedImageView() {
            
              final ImageView imageView = imageViewReference.get();
            final BitmapWorkerTask bitmapWorkerTask = getBitmapWorkerTask(imageView);

            if (this == bitmapWorkerTask) {
                 return imageView;
            }
            return null;
        }

		//query ID3 tag 
		private Bitmap GetID3BitmapViaMediaMetadataRetriever(String FileName)
		{
			Log.d(TAG,"GetID3BitmapViaMediaMetadataRetriever begin");
			MediaMetadataRetriever mmr = new MediaMetadataRetriever();
			Bitmap bitmap=null;
			
			try
			{
				if(FileName.length()>7 && FileName.startsWith("http://"))
				{
					Log.d(TAG,"NETWORK FileName="+FileName);
					mmr.setDataSource(FileName, new HashMap<String, String>());// It's work for network file (http), but failed with local path file(USB,SMB).
				}
				else
				{
					Log.d(TAG,"LOCAL FileName="+FileName);
					mmr.setDataSource(FileName);// It's work for local path file(USB,SMB), but failed with network file(http).
				}
				byte[] m_MMR_Picture = mmr.getEmbeddedPicture();
				if(m_MMR_Picture != null)
				{
                    mOptions = new BitmapFactory.Options();
					bitmap = processBitmap(m_MMR_Picture, OBJECT_TO_BYTEARRAY, mOptions);
				}
			} catch (Exception e) {
				Log.d(TAG,"mmr.setDataSource exception");
			}
			mmr.release();
			Log.d(TAG,"GetID3BitmapViaMediaMetadataRetriever end");
			return bitmap;
		}
        private Bitmap GetVideoThumbnailViaDB(String FileName)
        {
            //Get cover_path from BD for local storage only. For save the time, pls don't ask it, if video is come from outside(samba/dms).
            Bitmap Cover = null;
            ContentResolver cr = null;
            Cursor cursor = null; 
            cr = mContext.getContentResolver();
            String CoverPath = null;
            String[] columns ={MediaStore.Video.VideoColumns.COVER_PATH};
            String Selection=MediaStore.MediaColumns.DATA+"=?";
            String[] SelectionArgs = new String[]{FileName};
            String order = MediaStore.MediaColumns.DISPLAY_NAME + " ASC";
            try {
                cursor = cr.query(MediaStore.Video.Media.EXTERNAL_CONTENT_URI,columns, Selection, SelectionArgs, order);
                if (cursor.moveToNext())
                {
                   CoverPath = cursor.getString(cursor.getColumnIndex(MediaStore.Video.VideoColumns.COVER_PATH));
                   mOptions = new BitmapFactory.Options();
                   Cover = processBitmap((Object)CoverPath, mOptions);
                }
            } catch (Exception e) {
                Log.d(TAG,"GetVideoThumbnailViaDB exception: " + e);
            }
            if (cursor != null)
            {
                cursor.close();
            }
            if (Cover==null)
            {
               if (CoverPath==null)
                  Log.d(TAG,"GetVideoThumbnailViaDB fail, can not get cover_path from:"+FileName);
               else
                  Log.d(TAG,"GetVideoThumbnailViaDB fail, can not decode cover_path:"+CoverPath);
            }
            return Cover;
        }
    }

    private Bitmap GetVideoThumbnailViaMediaMetadataRetriever(String FileName) {
        synchronized(lock){
            misBusyOnVideoframeDecode = true;
        }
        log("GetVideoThumbnailViaMediaMetadataRetriever begin");
        MediaMetadataRetriever mmr = new MediaMetadataRetriever();
        Bitmap bitmap=null;

        try{
            if(FileName.length()>7 && FileName.startsWith("http://")) {
                log("NETWORK FileName=" + FileName);
                mmr.setDataSource(FileName, new HashMap<String, String>());// It's work for network file (http), but failed with local path file(USB,SMB).
            } else {
                log("LOCAL FileName=" + FileName);
                mmr.setDataSource(FileName);// It's work for local path file(USB,SMB), but failed with network file(http).
            }
            Bitmap framebitmap= mmr.getFrameAtTime();
            if (framebitmap != null) {
//              log("processBitmap begin");
                final BitmapFactory.Options options = new BitmapFactory.Options();
                bitmap = processBitmap(framebitmap, OBJECT_TO_BITMAP, options);
                framebitmap.recycle();
//              log("processBitmap end");
            }
        } catch (Exception e) {
            Log.d(TAG,"mmr.setDataSource exception");
        }

//      log("mmr.release() begin");
        mmr.release();
//      log("mmr.release() end");
        log("GetVideoThumbnailViaMediaMetadataRetriever end");
        synchronized(lock){
            misBusyOnVideoframeDecode = false;
        }
        return bitmap;
    }

    private static void log(String s){
        if(DEBUG)
            Log.d(TAG, s);
    }

    /**
     * A custom Drawable that will be attached to the imageView while the work
     * is in progress. Contains a reference to the actual worker task, so that
     * it can be stopped if a new binding is required, and makes sure that only
     * the last started worker process can bind its result, independently of the
     * finish order.
     */
    private static class AsyncDrawable extends BitmapDrawable {
        private final WeakReference<BitmapWorkerTask> bitmapWorkerTaskReference;

        public AsyncDrawable(Resources res, Bitmap bitmap, BitmapWorkerTask bitmapWorkerTask) {
            super(res, bitmap);

            bitmapWorkerTaskReference =
                    new WeakReference<BitmapWorkerTask>(bitmapWorkerTask);
        }

        public BitmapWorkerTask getBitmapWorkerTask() {
            return bitmapWorkerTaskReference.get();
        }
    }

    /**
     * Called when the processing is complete and the final bitmap should be set
     * on the ImageView.
     * 
     * @param imageView
     * @param bitmap
     */
    private void setImageBitmap(ImageView imageView, Bitmap bitmap) {
        if (mFadeInBitmap) {
            // Transition drawable with a transparent drwabale and the final
            // bitmap
            final TransitionDrawable td =
                    new TransitionDrawable(new Drawable[] {
                            new ColorDrawable(android.R.color.transparent),
                            new BitmapDrawable(mContext.getResources(), bitmap)
                    });
            // Set background to loading bitmap
			/*
            int sdk = android.os.Build.VERSION.SDK_INT;
            if (sdk < android.os.Build.VERSION_CODES.JELLY_BEAN)
            {
                setBackgroundDrawable(imageView);
            } else
            {
                setBackground(imageView);
            }
			*/
			setBackgroundDrawable(imageView);
            imageView.setImageDrawable(td);
            td.startTransition(FADE_IN_TIME);
        } else {
            imageView.setImageBitmap(bitmap);
        }
    }

	
    //@TargetApi(16)
	/*
    public void setBackground(ImageView imageView)
    {
        imageView
                .setBackground(
                new BitmapDrawable(mContext.getResources(), mLoadingBitmap));
    }
	*/

    //@SuppressWarnings("deprecation")
    public void setBackgroundDrawable(ImageView imageView)
    {
        imageView
                .setBackgroundDrawable(
                new BitmapDrawable(mContext.getResources(), mLoadingBitmap));
    }

    /**
     * Set the simple adapter which holds the backing data.
     * 
     * @param adapter
     */
    public void setAdapter(ImageWorkerAdapter adapter) {
        mImageWorkerAdapter = adapter;
    }

    /**
     * Get the current adapter.
     * 
     * @return
     */
    public ImageWorkerAdapter getAdapter() {
        return mImageWorkerAdapter;
    }

    /**
     * A very simple adapter for use with ImageWorker class and subclasses.
     */
    public static abstract class ImageWorkerAdapter {
        public abstract Object getItem(int num);

        public abstract int getSize();
    }

    public Bitmap loadRecommendationImage(int id, String title, Object data, Object coverPath) {
        Bitmap bitmap = null;

        Log.d(TAG,"IW:loadRecommendationImage: path:"+String.valueOf(data));
        if (mImageCache != null) {
            bitmap = mImageCache.getBitmapFromMemCache(String.valueOf(data));
        }

        if (bitmap == null) {
            Log.d(TAG,"IM:loadRecommendationImage: do not get bitmap from imagecache");
            Log.d(TAG,"IM:loadRecommendationImage: do ProcessBitmap:"+String.valueOf(data));
            String FileName = String.valueOf(data);
            if(FileName.length()>7 && !(FileName.startsWith("http://"))) {
                if (coverPath != null) {
                    final BitmapFactory.Options options = new BitmapFactory.Options();
                    bitmap = processBitmap((Object)coverPath, options);
                    if (bitmap == null) {
                        Log.d(TAG,"IM:loadRecommendationImage: Can not decode cover_path:"+coverPath);
                    }
                }
            }
            if (bitmap == null) {
                bitmap = GetVideoThumbnailViaMediaMetadataRetriever(String.valueOf(data));
            }
            if (bitmap != null && mImageCache != null) {
                mImageCache.addBitmapToCache(FileName, bitmap);
            }
        }
        return bitmap;
    }
}
